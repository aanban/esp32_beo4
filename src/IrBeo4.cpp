#include "IrBeo4.h"

// ----------------------------------------------------------------------------
// Helper c-functions 
// ----------------------------------------------------------------------------

// checks if a given beoCode inlcudes a beoCommand that is repeatable 
// @param beoCode code to be checked
// @return 0=standard 1=repeatable 
uint8_t isRepeatable(uint32_t beo_code){
  uint32_t beo_cmd = beo_code & 0xff; 
  switch(beo_cmd){ 
    case BEO_CMD_UP:
    case BEO_CMD_DOWN: 
    case BEO_CMD_LEFT: 
    case BEO_CMD_RIGHT: 
    case BEO_CMD_GO: 
    case BEO_CMD_YELLOW: 
    case BEO_CMD_GREEN: 
    case BEO_CMD_BLUE: 
    case BEO_CMD_RED:   return 1; 
  }
  return 0; 
}


// lookup the Beo4 source from a given beoCode
// @param beo_code=Beo4 code
// return Beo-Source or "invalid_src" for unknown beoCode
const char* beo_src_tbl(uint32_t beo_code) {
  uint8_t source=(uint8_t) ((beo_code>>8) & 255u);
  switch(source) {
    case BEO_SRC_VIDEO:  return(PSTR("video"));
    case BEO_SRC_AUDIO:  return(PSTR("audio"));
    case BEO_SRC_VTAPE:  return(PSTR("vtape"));
    case BEO_SRC_ALL:    return(PSTR("all"));
    case BEO_SRC_SPDEMO: return(PSTR("spdemo"));
    case BEO_SRC_LIGHT:  return(PSTR("light"));
  }
  return(PSTR("invalid_src"));
}

// lookup the Beo4 command from a given beoCode
// @param beo_code=Beo4 code 
// return Beo-Source or "invalid_cmd" for unknwon beoCode
const char* beo_cmd_tbl(uint32_t beo_code) {
  uint8_t command=(uint8_t) (beo_code & 255u);
  switch(command) {
    case BEO_CMD_NUM_0:        return(PSTR("num-0"));
    case BEO_CMD_NUM_1:        return(PSTR("num-1"));
    case BEO_CMD_NUM_2:        return(PSTR("num-2"));
    case BEO_CMD_NUM_3:        return(PSTR("num-3"));
    case BEO_CMD_NUM_4:        return(PSTR("num-4"));
    case BEO_CMD_NUM_5:        return(PSTR("num-5"));
    case BEO_CMD_NUM_6:        return(PSTR("num-6"));
    case BEO_CMD_NUM_7:        return(PSTR("num-7"));
    case BEO_CMD_NUM_8:        return(PSTR("num-8"));
    case BEO_CMD_NUM_9:        return(PSTR("num-9"));
    case BEO_CMD_CLEAR:        return(PSTR("clear"));
    case BEO_CMD_STORE:        return(PSTR("store"));
    case BEO_CMD_STANDBY:      return(PSTR("standby"));
    case BEO_CMD_MUTE:         return(PSTR("mute"));
    case BEO_CMD_INDEX:        return(PSTR("index"));
    case BEO_CMD_UP:           return(PSTR("up"));
    case BEO_CMD_DOWN:         return(PSTR("down"));
    case BEO_CMD_TUNE:         return(PSTR("tune"));
    case BEO_CMD_CLOCK:        return(PSTR("clock"));
    case BEO_CMD_FORMAT:       return(PSTR("format"));
    case BEO_CMD_LEFT:         return(PSTR("left"));
    case BEO_CMD_RETURN:       return(PSTR("return"));
    case BEO_CMD_RIGHT:        return(PSTR("right"));
    case BEO_CMD_GO:           return(PSTR("go"));
    case BEO_CMD_STOP:         return(PSTR("stop"));
    case BEO_CMD_RECORD:       return(PSTR("return"));
    case BEO_CMD_SELECT:       return(PSTR("select"));
    case BEO_CMD_SPEAKER:      return(PSTR("speaker"));
    case BEO_CMD_PICTURE:      return(PSTR("picture"));
    case BEO_CMD_TURN:         return(PSTR("turn"));
    case BEO_CMD_LOUDNESS:     return(PSTR("loudness"));
    case BEO_CMD_BASS:         return(PSTR("bass"));
    case BEO_CMD_TREBLE:       return(PSTR("treble"));
    case BEO_CMD_BALANCE:      return(PSTR("balance"));
    case BEO_CMD_LIST:         return(PSTR("list"));
    case BEO_CMD_MENU:         return(PSTR("menu"));
    case BEO_CMD_VOL_UP:       return(PSTR("vol++"));
    case BEO_CMD_VOL_DOWN:     return(PSTR("vol--"));
    case BEO_CMD_LEFT_REPEAT:  return(PSTR("left++"));
    case BEO_CMD_RIGHT_REPEAT: return(PSTR("right++"));
    case BEO_CMD_UP_REPEAT:    return(PSTR("up++"));
    case BEO_CMD_DOWN_REPEAT:  return(PSTR("down++"));
    case BEO_CMD_GO_REPEAT:    return(PSTR("go++"));
    case BEO_CMD_GREEN_REPEAT: return(PSTR("green++"));
    case BEO_CMD_YELLOW_REPEAT:return(PSTR("yellow++"));
    case BEO_CMD_BLUE_REPEAT:  return(PSTR("blue++"));
    case BEO_CMD_RED_REPEAT:   return(PSTR("red++"));
    case BEO_CMD_EXIT:         return(PSTR("exit"));
    case BEO_CMD_TV:           return(PSTR("tv-on"));
    case BEO_CMD_RADIO:        return(PSTR("radio-on"));
    case BEO_CMD_VIDEO_AUX:    return(PSTR("video-aux"));
    case BEO_CMD_AUDIO_AUX:    return(PSTR("audio-aux"));
    case BEO_CMD_VTAPE:        return(PSTR("vtape-on"));
    case BEO_CMD_DVD:          return(PSTR("dvd-on"));
    case BEO_CMD_CAMCORD:      return(PSTR("camcord-on"));
    case BEO_CMD_TEXT:         return(PSTR("text"));
    case BEO_CMD_SP_DEMO:      return(PSTR("sp-demo"));
    case BEO_CMD_SAT:          return(PSTR("sat-on"));
    case BEO_CMD_PC:           return(PSTR("pc"));
    case BEO_CMD_DOOR_CAM:     return(PSTR("door-cam"));
    case BEO_CMD_ATAPE:        return(PSTR("atape-on"));
    case BEO_CMD_CD:           return(PSTR("cd-on"));
    case BEO_CMD_PHONO:        return(PSTR("phono-on"));
    case BEO_CMD_ATAPE2:       return(PSTR("atape2-on"));
    case BEO_CMD_CD2:          return(PSTR("cd2-on"));
    case BEO_CMD_LIGHT:        return(PSTR("light-on"));
    case BEO_CMD_AV:           return(PSTR("av"));
    case BEO_CMD_YELLOW:       return(PSTR("yellow"));
    case BEO_CMD_GREEN:        return(PSTR("green"));
    case BEO_CMD_BLUE:         return(PSTR("blue"));
    case BEO_CMD_RED:          return(PSTR("red"));
  }
  return(PSTR("invalid_cmd"));
}


// ----------------------------------------------------------------------------
// global variables
// ----------------------------------------------------------------------------

DRAM_ATTR xQueueHandle       g_isr_queue;  // isr queue:  ir_pulse_isr() --queue--> beo4_rx_task() 
DRAM_ATTR EventGroupHandle_t g_eg_handle;  // handle eventgroup repeated beocode event
constexpr uint32_t evRptCode = (1 << 0);   // repeated-codes flag used by xEventGroupWaitBits()

// Beo4 PulseCode = PulseWidth / 3125
constexpr uint8_t bZero  = 1;  // pulsewidth= 3125
constexpr uint8_t bSame  = 2;  // pulsewidth= 6250
constexpr uint8_t bOne   = 3;  // pulsewidth= 9375
constexpr uint8_t bStop  = 4;  // pulsewidth=12500
constexpr uint8_t bStart = 5;  // pulsewidth=15625
constexpr uint8_t nBit   = 17; // data bits in frame

static portMUX_TYPE beo4_tx_mutex = portMUX_INITIALIZER_UNLOCKED;


// ----------------------------------------------------------------------------
// Tasks
// ----------------------------------------------------------------------------

// handles repeatable BeoCodes that wait in quarantine
void beo4_quarantine_task(void *handle){
  if(NULL!=handle){
    IrBeo4  *beo=(IrBeo4*) handle;  // get access to class
    uint32_t repeatableBeoCode=0;   // repeatable BeoCode could have a successor
    EventBits_t uxBits;             // used as trigger in case successor arrived
    for(;;){
      if(pdTRUE!=xQueueReceive(beo->m_beo4_quarantine_queue,&repeatableBeoCode,WAIT_infi)) {
        ets_printf("xQueueReceive() something went wrong, waiting infinite does not work!");  
      }
      // repeatable BeoCode has arrived, wait for trigger-event or time out
      uxBits=xEventGroupWaitBits(g_eg_handle, evRptCode, pdTRUE, pdTRUE, WAIT_250ms);
      if(evRptCode==(uxBits & evRptCode)) { // successor BeoCode detected 
        beo->m_beoWait=0;                   // reset flag, BeoCode is removed
        //ets_printf(".\n");                        
      } else {  // time out detected, delayed code can be send
        xQueueSend(beo->m_beo4_rx_queue,&repeatableBeoCode,0);
        beo->m_beoWait=0;                   // reset flag, BeoCode is send to standard qeuue
        //ets_printf("_\n");
      }
    }
  }
}

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
    static uint32_t ic=0,jc=0;      // bit counter
    static uint32_t curBit=0;       // current bitCode
    static uint32_t preBit=0;       // previous bitCode

    // attach to pin, set carrier=455kHz and set resolution for duty-range=[0..15]
    ledcAttach(beo4->m_tx_pin,t_freq,t_resolution);  
    ledcWrite(beo4->m_tx_pin,0);  // duty=0 to start everything with output=low

    for(;;) {  // wait until beoCodes arrive at queue
      if(pdTRUE != xQueueReceive(beo4->m_beo4_tx_queue,&beoCode,portMAX_DELAY)) { 
        ets_printf("TaskBeoTx().xQueueReceive() failed ");  
        continue;
      }
      beo4->LED(HIGH);             // start transmitting frame --> LED ON 
      // generate carrier pulses and pauses according to beoCode
      // a complete frame needs < 150ms. To avoid interrupts, that may affect 
      // the timing this block is protected with an mutex. 
      taskENTER_CRITICAL(&beo4_tx_mutex);
        beo4->tx_pc(bZero);        // send start sequence: 1,1,5
        beo4->tx_pc(bZero);
        beo4->tx_pc(bStart);
        beo4->tx_pc(bZero);        // send beoLink (always bZero)
        curBit=preBit=0;           // init current and previous Bit
        for(jc=15,ic=0;ic<16;ic++,jc--) {     // generate 16 data bits of BeoCode
          uint8_t pulseCode=0;
          curBit=(beoCode>>jc) & 1;
          if(curBit==preBit) pulseCode=bSame;
          else if(1==curBit) pulseCode=bOne;
          else               pulseCode=bZero;
          preBit=curBit;
          beo4->tx_pc(pulseCode);  // send pulsecode for current Bit
        }
        beo4->tx_pc(bStop);        // send stop code
        beo4->tx_pc(0);            // finish frame with final carrier pulse
      taskEXIT_CRITICAL(&beo4_tx_mutex);
      beo4->LED(LOW);              // frame transmitted --> LED OFF 
    }
  } // if(NULL!=parameter)
}

// ----------------------------------------------------------------------------
// Interrupt service routines
// ----------------------------------------------------------------------------
void IRAM_ATTR ir_pulse_isr(void) {
  static int64_t tsPre=0;              // timestamp of previous edge
  int64_t tsNew=esp_timer_get_time();  // timestamp of new edge
  if((tsNew-tsPre) > t_debounce ) {    // debounce TSOP7000 output
    xQueueSend(g_isr_queue,&tsNew,0);  // send timestamp to queue if valid low pulse
    tsPre=tsNew;
  }
}

// ------------------------------------------------------------------------------------------------
// Member functions
// ------------------------------------------------------------------------------------------------

// constructor
IrBeo4::IrBeo4(int8_t rx_pin,int8_t tx_pin) {
  m_rx_pin = rx_pin;
  m_tx_pin = tx_pin;
  m_rxFSM  = rxSt::Idle;
}

// destructor
IrBeo4::~IrBeo4() {
}

// initialize beo4 decoder
int IrBeo4::Begin(QueueHandle_t beo4_rx_queue, QueueHandle_t beo4_tx_queue) {
  if(-1!=m_rx_pin && NULL!=beo4_rx_queue) { 
    if(NULL==(m_beo4_quarantine_queue=xQueueCreate(5, sizeof(uint32_t))))
      return -1;
    if(pdPASS!=xTaskCreatePinnedToCore(beo4_quarantine_task,"beo4_quarantine_task",2048,this,1,&m_beo4_quarantine_task,1))
      return -2; 
    g_eg_handle = xEventGroupCreate(); // create event group for delayed beo codes
    m_beo4_rx_queue=beo4_rx_queue;
    if(NULL==(g_isr_queue=xQueueCreate(100, sizeof(int64_t))))
      return -1;
    if(pdPASS!=xTaskCreatePinnedToCore(beo4_rx_task,"beo4_rx_task",2048,this,1,&m_beo4_rx_task,1))
      return -2; 
    pinMode(m_rx_pin, INPUT_PULLUP);
    attachInterrupt(m_rx_pin, ir_pulse_isr, FALLING);

  } 
  if(-1!=m_tx_pin && NULL!=beo4_tx_queue) {
    m_beo4_tx_queue=beo4_tx_queue;
    if(pdPASS!=xTaskCreate(beo4_tx_task, "beo4_tx_task", 2048,this,1,&m_beo4_tx_task))
      return -3;
  }
  return 0;
}

// receiver task state machine
void IrBeo4::RxFsm(int64_t tsNew) {
  m_tsNew  = tsNew;            // new arrived edge
  m_pWidth = (uint32_t)(m_tsNew-m_tsPre); // compute pulse-width from timestamps
  m_pCode  = (uint8_t)((m_pWidth+t_pc_half)/t_pc); // compute pulse-code from pulse-width (rounded)
  m_tsPre  = m_tsNew;          // set previous timestamp
  switch(m_rxFSM) {
    case rxSt::Idle: {         // wake up in idle state
      m_wait=WAIT_20ms;        // set short timeout within frame (supress TSOP7000 hiccups)
      m_beoCode=0;             // new frame started --> reset beoCode 
      m_bc=0;                  // new frame started --> reset bit-counter
      m_tsFrm=m_tsNew;         // take timestamp start-of-frame
      m_rxFSM=rxSt::S0;        // next state -->  begin sequence next
      break;
    } 
    case rxSt::S0: {
      if(bZero==m_pCode) {
        m_rxFSM=rxSt::S1;      // next state -->  continue start sequence
      } else if(bStart==m_pCode) {
        m_rxFSM=rxSt::Data;    // next state -->  collect data
      } else {                 // frame corrupt
        resetRxFsm("ERR: FSM_S_0 state failed "); 
      }
      break; 
    } 
    case rxSt::S1: {
      if(bZero==m_pCode) {
        m_rxFSM=rxSt::Start;   //  next state -->  continue start sequence
      } else if(bStart==m_pCode) { 
        m_rxFSM=rxSt::Data;    // next state -->  collect data
      } else {                 // frame corrupt
        resetRxFsm("ERR: S1 state failed ");
      }
      break; 
    } 
    case rxSt::Start: {
      if(bStart==m_pCode) {
        m_rxFSM=rxSt::Data;    // next state -->  collect data
        if(1==m_beoWait) {
          m_beoWait=0;         
          // a beoCode is waiting in quarantine and its successor beoCode 
          // arrived, lets trigger the beo4_quarantine_task() to skip the 
          // waiting BeoCode
          xEventGroupSetBits(g_eg_handle,evRptCode); 
        }
        LED(HIGH);             // active frame --> LED ON 
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
      m_beoCode=(m_beoCode<<1)+curBit;  // shift Bit into beoCode
      if((++m_bc)==nBit) {
        m_rxFSM=rxSt::Stop;    // next state -->  check the stopCode
      }
      break;
    }
    case rxSt::Stop: {
      if(bStop==m_pCode) {     // final check of stop-code
        m_beoCode &= mskBC;    // mask and store valid beoCode
        if(isRepeatable(m_beoCode)){ // is repeat-code possible
          xQueueSend(m_beo4_quarantine_queue,&m_beoCode,0);
          m_beoWait=1;         // send code into quarantine 
        } else {
          if(m_beo4_rx_queue){ // send to queue
            m_beoWait=0;       // code not waiting 
            xQueueSend(m_beo4_rx_queue,&m_beoCode,0);
          }
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
