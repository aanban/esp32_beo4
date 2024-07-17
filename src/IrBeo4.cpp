#include "IrBeo4.h"

// global variables
DRAM_ATTR xQueueHandle       g_isr_queue;  // isr queue:  ir_pulse_isr() --queue--> beo4_rx_task() 
DRAM_ATTR EventGroupHandle_t g_eg_handle;  // handle eventgroup timer-events


// Beo4 PulseCode = PulseWidth / 3125
constexpr uint8_t bZero  = 1;  // pulsewidth= 3125
constexpr uint8_t bSame  = 2;  // pulsewidth= 6250
constexpr uint8_t bOne   = 3;  // pulsewidth= 9375
constexpr uint8_t bStop  = 4;  // pulsewidth=12500
constexpr uint8_t bStart = 5;  // pulsewidth=15625

constexpr uint8_t nBit   = 17; // number of data bits in frame

// receiver task
void beo4_rx_task(void *handle) {
  if(NULL!=handle) {
    IrBeo4   *beo=(IrBeo4*) handle; // get access to class
    int64_t  tsNew=0;               // new edge timestamp
    for(;;) {
      if(pdTRUE!=xQueueReceive(g_isr_queue,&tsNew,beo->get_wait())) {
        beo->resetRxFsm("TIMEOUT occured "); // TSO7000 hicups lead to timeout
      }
      beo->RxFsm(tsNew); // call decode state machine
    }
  } // if(NULL!=parameter)
}

// transmit task
void beo4_tx_task(void *param) {
  if(NULL!=param) {
    IrBeo4 *beo4 = (IrBeo4*) param; // get access to class
    static uint32_t beoCode=0;      // beoCode to be transmitted
    static uint32_t ic=0;           // bit counter
    static uint32_t curBit=0;       // current bitCode
    static uint32_t preBit=0;       // previous bitCode

    // create events and timer
    g_eg_handle = xEventGroupCreate();
    const esp_timer_create_args_t OneShotTimer_args = {
      .callback = &one_shot_timer_cb,
      .name = "OneShotTimer"
    };
    esp_timer_create(&OneShotTimer_args, &beo4->m_OneShotTimer_h);

    // setup the PMW and attach to IR transmit pin
    ledcSetup(beo4->m_tx_pwm,455000,4);  // pCarrier=455kHz,duty_range=[0..15]
    ledcAttachPin(beo4->m_tx_pin,beo4->m_tx_pwm); // attach to transmit pin
    ledcWrite(beo4->m_tx_pwm,0);         // duty=0 --> wake up with output=low

    for(;;) {  // wait until beoCodes arrive at queue
      if(pdTRUE != xQueueReceive(beo4->m_beo4_tx_queue,&beoCode,portMAX_DELAY)) { 
        ets_printf("TaskBeoTx().xQueueReceive() failed ");  
        continue;
      }
      // generate carrier pulses and pauses according to beoCode
      beo4->tx_pc(bZero);        // send start sequence: 1,1,5
      beo4->tx_pc(bZero);
      beo4->tx_pc(bStart);
      beo4->tx_pc(bZero);        // send beoLink (always bZero)
      curBit=preBit=0;           // init current and previous Bit
      for(ic=0;ic<16;ic++) {     // generate 16 data bits of BeoCode
        uint8_t pulseCode=0;
        curBit=(beoCode>>ic) & 1;
        if(curBit==preBit) pulseCode=bSame;
        else if(1==curBit) pulseCode=bOne;
        else               pulseCode=bZero;
        preBit=curBit;
        beo4->tx_pc(pulseCode);  // send pulsecode for current Bit
      }
      beo4->tx_pc(bStop);        // send stop code
      beo4->tx_pc(0);            // finish frame with final carrier pulse
    }
  } // if(NULL!=parameter)
}

// interrupt service routine
void IRAM_ATTR ir_pulse_isr(void) {
  static int64_t tsPre=0;              // timestamp of previous edge
  int64_t tsNew=esp_timer_get_time();  // timestamp of new edge
  if((tsNew-tsPre) > t_debounce ) {    // debounce TSOP7000 output
    xQueueSend(g_isr_queue,&tsNew,0);  // send timestamp to queue if valid low pulse
    tsPre=tsNew;
  }
}

// one-shot-timer callback function 
void IRAM_ATTR one_shot_timer_cb(void* arg) {
  BaseType_t xHigherPriorityTaskWoken;
  xEventGroupSetBitsFromISR(g_eg_handle, evPulse, &xHigherPriorityTaskWoken);
}

// ------------------------------------------------------------------------------------------------
// member functions
// ------------------------------------------------------------------------------------------------

// constructor
IrBeo4::IrBeo4(int8_t rx_pin,int8_t tx_pin, int8_t tx_pwm) {
  m_rx_pin = rx_pin;
  m_tx_pin = tx_pin;
  m_tx_pwm = tx_pwm;
  m_rxFSM  = rxSt::Idle;
}

// destructor
IrBeo4::~IrBeo4() {
}

// initialize beo4 decoder
int IrBeo4::Begin(QueueHandle_t beo4_rx_queue, QueueHandle_t beo4_tx_queue) {
  // store receive and transmit queue
  m_beo4_rx_queue=beo4_rx_queue;
  m_beo4_tx_queue=beo4_tx_queue;
  // create queue for interupt-service-routine
  if(NULL==(g_isr_queue=xQueueCreate(100, sizeof(int64_t))))
    return -1;
  // create receive task
  if(pdPASS!=xTaskCreatePinnedToCore(beo4_rx_task,"beo4_rx_task",2048,this,1,&m_beo4_rx_task,0))
    return -2; 
  // create transmit task
  if(-1!=m_tx_pin && -1!=m_tx_pwm) {
    if(pdPASS!=xTaskCreate(beo4_tx_task, "beo4_tx_task", 2048,this,1,&m_beo4_tx_task))
      return -3;
  }
  // attach interrupt to TSOP7000 output, and set to falling edge
  if(-1!=m_rx_pin) { 
    pinMode(m_rx_pin, INPUT_PULLUP);
    attachInterrupt(m_rx_pin, ir_pulse_isr, FALLING);
  } 
  return 0;
}

// receiver task state machine
void IrBeo4::RxFsm(int64_t tsNew) {
  m_tsNew  = tsNew;            // new arrived edge
  m_pWidth = (uint32_t)(m_tsNew-m_tsPre); // compute pulse-width from timestamps
  m_pCode  = (uint8_t)((m_pWidth+t_pc_half)/t_pc); // compute pulse-code from pulse-width (rounded)
  m_tsPre  = m_tsNew;          // set previous timestamp
  switch(m_rxFSM) {            // process state machine
    case rxSt::Idle: {         // wake up in idle state
      m_wait=WAIT_20ms;        // set short timeout within frame (supress TSOP7000 hiccups)
      m_beoCode=0;             // new frame started -->  so reset beoCode and bit counter
      m_bc=0;
      m_tsFrm=m_tsNew;         // timestamp start-of-frame
      m_rxFSM=rxSt::S0;        // next state -->  begin sequence next
      if(beo_led_cb) {
        beo_led_cb(LOW);       // turn LED on to indicate active frame
      }
      break;
    } 
    case rxSt::S0: {
      if(bZero==m_pCode) {
        m_rxFSM=rxSt::S1;      // next state -->  continue start sequence
      } else {                 // frame corrupt
        resetRxFsm("ERR: FSM_S_0 state failed "); 
      }
      break; 
    } 
    case rxSt::S1: {
      if(bZero==m_pCode) {
        m_rxFSM=rxSt::Start;   //  next state -->  continue start sequence
      } else {                 // frame corrupt
        resetRxFsm("ERR: S1 state failed ");
      }
      break; 
    } 
    case rxSt::Start: {
      if(bStart==m_pCode) {
        m_rxFSM=rxSt::Data;    // next state -->  collect data
      } else {                 // frame corrupt
        resetRxFsm("ERR: Start state failed ");
      }
      break; 
    } 
    case rxSt::Data: {
      int8_t curBit=0;
      switch(m_pCode) {        // bitCode depends on Pulse code
        case bZero: { curBit=m_preBit=0; break; }
        case bSame: { curBit=m_preBit;   break; }
        case bOne:  { curBit=m_preBit=1; break; }
        default:    {          // frame corrupt 
          resetRxFsm("ERR: invalid Data "); 
          return; 
        }
      }
      m_beoCode=(m_beoCode<<1)+curBit;  // fill current Bit into beoCode
      if((++m_bc)==nBit) {
        m_rxFSM=rxSt::Stop;     // next state -->  check the stopCode
      }
      break;
    }
    case rxSt::Stop: {
      if(bStop==m_pCode) {     // final check of stop-code
        m_beoCode &= mskBC;    // mask and store valid beoCode
        if(beo_code_cb) {      // send to callback function 
          beo_code_cb(m_beoCode); 
        }
        if(m_beo4_rx_queue) {  // send to queue
          xQueueSend(m_beo4_rx_queue,&m_beoCode,0);
        }
        resetRxFsm("SUCCESS beoCode OK ");
      } else {                 // frame corrupt 
        resetRxFsm("ERR: Stop state failed ");
      }
      break;
    } 
    default: {                 // handle invalid state 
      resetRxFsm("ERR:INVALID state");
      break;
    }
  } // switch(m_rxFSM)
}
