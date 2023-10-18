#include "IrBeo4.h"

// global variables
DRAM_ATTR xQueueHandle       g_isr_queue;  // isr queue:  ir_pulse_isr() --queue--> beo4_rx_task() 
DRAM_ATTR EventGroupHandle_t g_eg_handle;  // handle eventgroup timer-events

// Event do-pulse
constexpr uint32_t evPulse = (1 << 0);     // BIT_1

// Beo4 PulseCode = PulseWidth / 3125
constexpr uint8_t bZero  = 1;  // pulsewidth= 3125
constexpr uint8_t bSame  = 2;  // pulsewidth= 6250
constexpr uint8_t bOne   = 3;  // pulsewidth= 9375
constexpr uint8_t bStop  = 4;  // pulsewidth=12500
constexpr uint8_t bStart = 5;  // pulsewidth=15625

constexpr uint8_t nBit   = 17; // number of data bits in frame

void beo4_rx_task(void *handle) {
  if(NULL!=handle) {
    IrBeo4   *beo=(IrBeo4*) handle; // get access to class
    int64_t  tsNew=0;               // new edge timestamp
    for(;;) {
      if(pdTRUE!=xQueueReceive(g_isr_queue,&tsNew,beo->get_wait())) {
        beo->ResetRxFSM("TIMEOUT occured "); // TSO7000 hicups lead to timeout
      }
      beo->RxFsm(tsNew); // call decode state machine
    }
  } // if(NULL!=parameter)
}

// timing correction by 50us
#define TIME_CORR 50

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

    // MACRO: 
    // - generate 200us carrier-pulse (time-correction)
    // - add pause according to pulsecode
    #define TX_PC(pulsecode) { \
      ledcWrite(beo4->m_tx_pwm,7); \
      esp_timer_start_once(beo4->m_OneShotTimer_h,200-TIME_CORR); \
      xEventGroupWaitBits(g_eg_handle, evPulse, pdTRUE, pdTRUE, portMAX_DELAY); \
      ledcWrite(beo4->m_tx_pwm,0); \
      if(0<pulsecode) { \
        esp_timer_start_once(beo4->m_OneShotTimer_h, (pulsecode*3125)-200); \
        xEventGroupWaitBits(g_eg_handle, evPulse, pdTRUE, pdTRUE, portMAX_DELAY); \
      } \
    }

    // setup the PMW and attach to IR transmit pin
    ledcSetup(beo4->m_tx_pwm,455000,4);  // pCarrier=455kHz,duty_range=[0..15]
    ledcAttachPin(beo4->m_tx_pin,beo4->m_tx_pwm); // attach to transmit pin
    ledcWrite(beo4->m_tx_pwm,0);         // duty=0 --> wake up with output=low

    while(1){ // wait until beoCodes arrive at queue
      if(pdTRUE != xQueueReceive(beo4->m_beo4_tx_queue,&beoCode,portMAX_DELAY)) { 
        ets_printf("TaskBeoTx().xQueueReceive() failed ");  
        continue;
      }
      TX_PC(bZero);         // transmit start sequence: 1,1,5
      TX_PC(bZero);
      TX_PC(bStart);
      TX_PC(bZero);         // transmit beoLink (always bZero)
      curBit=preBit=0;         // init current and previous Bit
      for(ic=0;ic<16;ic++) {   // transmit 16 bits of BeoCode
        uint8_t pulseCode=0;
        curBit=(beoCode>>ic) & 1;
        if(curBit==preBit) pulseCode=bSame;
        else if(1==curBit) pulseCode=bOne;
        else               pulseCode=bZero;
        preBit=curBit;
        TX_PC(pulseCode);       // transmit pulsecode for current Bit
      }
      TX_PC(bStop);          // transmit stop code
      TX_PC(0);                 // finish frame with final carrier pulse
    }
  } // if(NULL!=parameter)
  #undef TX_PC
}

// Interrupt service routine
void IRAM_ATTR ir_pulse_isr(void) {
  int64_t tsNew=esp_timer_get_time();
  xQueueSend(g_isr_queue,&tsNew,0);
}

// callback function one-shot-timer
void IRAM_ATTR one_shot_timer_cb(void* arg) {
  BaseType_t xHigherPriorityTaskWoken;
  xEventGroupSetBitsFromISR(g_eg_handle, evPulse, &xHigherPriorityTaskWoken);
}

// ------------------------------------------------------------------------------------------------
// member functions
// ------------------------------------------------------------------------------------------------
IrBeo4::IrBeo4(int8_t rx_pin,int8_t tx_pin, int8_t tx_pwm) {
  m_rx_pin = rx_pin;
  m_tx_pin = tx_pin;
  m_tx_pwm = tx_pwm;
  m_rxFSM  = bState::Idle;
}

IrBeo4::~IrBeo4() {
}

// init the beo4 decoder
int IrBeo4::Begin(QueueHandle_t beo4_rx_queue, QueueHandle_t beo4_tx_queue) {
  m_beo4_rx_queue=beo4_rx_queue;  // take the queues
  m_beo4_tx_queue=beo4_tx_queue;
  // create queue for interupt-service-routine
  if(NULL==(g_isr_queue=xQueueCreate(100, sizeof(int64_t))))
    ESP_ERROR_CHECK(ESP_ERR_INVALID_ARG);
  // create receive task
  if(pdPASS!=xTaskCreatePinnedToCore(beo4_rx_task,"beo4_rx_task",2048,this,1,&m_beo4_rx_task,0))
    ESP_ERROR_CHECK(ESP_FAIL);
  // create transmit task
  if(-1!=m_tx_pin && -1!=m_tx_pwm) {
    if(pdPASS!=xTaskCreate(beo4_tx_task, "beo4_tx_task", 2048,this,1,&m_beo4_tx_task))
      ESP_ERROR_CHECK(ESP_FAIL);
  }
  // attach interrupt service routine to  receive pin
  if(-1!=m_rx_pin) { 
    pinMode(m_rx_pin, INPUT);
    attachInterrupt(m_rx_pin, ir_pulse_isr, RISING);
  } else {
    ESP_ERROR_CHECK(ESP_FAIL);
  }
  return 0;
}

// receiver state machine
void IrBeo4::RxFsm(int64_t tsNew) {
  m_tsNew  = tsNew;                               // timestamp of current edge
  m_pWidth = (uint32_t)(m_tsNew-m_tsPre);         // compute pulse-width
  m_pCode  = (uint8_t)((m_pWidth + 1562) / 3125); // compute pulse-code (rounded)
  m_tsPre  = m_tsNew;                             // set previous timestamp
  switch(m_rxFSM) {            // process state machine
    case bState::Idle: {       // wake up in idle state
      m_wait=WAIT_20ms;        // set short timeout within frame (supress TSOP7000 hiccups)
      m_beoCode=0;             // new frame started -->  so reset beoCode and bit counter
      m_bc=0;                  
      m_tsFrm=m_tsNew;         // timestamp start-of-frame
      m_rxFSM=bState::S0;      // next state -->  begin sequence next
      if(beo_led_cb) beo_led_cb(LOW);  // turn LED on
      break;
    } 
    case bState::S0: {
      if(bZero==m_pCode) {
        m_rxFSM=bState::S1;    // next state -->  continue start sequence
      } else { 
        ResetRxFSM("ERROR: FSM_S_0 failed "); 
      }
      break; 
    } 
    case bState::S1: {
      if(bZero==m_pCode) {
        m_rxFSM=bState::Start; //  next state -->  continue start sequence
      } else {
        ResetRxFSM("ERROR: bState::S1 failed ");
      }
      break; 
    } 
    case bState::Start: {
      if(bStart==m_pCode) {
        m_rxFSM=bState::Data;  // next state -->  collect data
      } else {
        ResetRxFSM("ERROR: bState::Start failed ");
      }
      break; 
    } 
    case bState::Data: {
      int8_t curBit=0;
      switch(m_pCode) {        // bitCode depends on Pulse code
        case bZero: { curBit=m_preBit=0; break; }
        case bSame: { curBit=m_preBit;   break; }
        case bOne:  { curBit=m_preBit=1; break; }
        default:    { 
          ResetRxFSM("ERROR: invalid Data "); 
          return; 
        }
      }
      m_beoCode=(m_beoCode<<1)+curBit;  // fill current Bit into beoCode
      if((++m_bc)==nBit) {
        m_rxFSM=bState::Stop;           // next state -->  check the stopCode
      }
      break;
    }
    case bState::Stop: {
      if(bStop==m_pCode) {                       // final check of stop-code
        m_beoCode=m_beoCode & 0x0000FFFFul;      // store valid beoCode
        if(beo_code_cb) {
          beo_code_cb(m_beoCode);
        }
        if(m_beo4_rx_queue) {
          xQueueSend(m_beo4_rx_queue,&m_beoCode,0);
        }
        ResetRxFSM("SUCCESS beoCode OK ");
      } else {
        ResetRxFSM("ERROR: bState::Stop failed ");
      }
      break;
    } 
    default: {  // something went wrong
      ResetRxFSM("ERROR:INVALID state");
      break;
    }
  } // switch(m_rxFSM)
}

