#include "IrBeo4.h"

// global variables
DRAM_ATTR xQueueHandle       q_irs_queue;  // isr queue:  ir_pulse_isr() --queue--> beo4_rx_task() 
DRAM_ATTR EventGroupHandle_t g_eg_handle;  // handle eventgroup timer-events

// Event do-pulse
#define evPulse (1 << 0)

// ------------------------------------------------------------------------------------------------
// c functions
// ------------------------------------------------------------------------------------------------
// Lookup table for beo Sources
const char* beo_src_tbl(uint32_t beo_code) {
  uint32_t source=((beo_code>>8) & 0xff);
  switch(source) {
    case BEO_SRC_VIDEO:  return(PSTR("video"));
    case BEO_SRC_AUDIO:  return(PSTR("audio"));
    case BEO_SRC_VTAPE:  return(PSTR("vtape"));
    case BEO_SRC_ALL:    return(PSTR("all  "));
    case BEO_SRC_SPDEMO: return(PSTR("spdmo"));
    case BEO_SRC_LIGHT:  return(PSTR("light"));
  }
  return(PSTR("invalid_src"));
}

// Lookup-Table for beo Commands
const char* beo_cmd_tbl(uint32_t beo_code) {
  uint32_t command=(beo_code & 0xff);
  switch(command) {
    case BEO_CMD_NUMBER_0:     return(PSTR("0"));
    case BEO_CMD_NUMBER_1:     return(PSTR("1"));
    case BEO_CMD_NUMBER_2:     return(PSTR("2"));
    case BEO_CMD_NUMBER_3:     return(PSTR("3"));
    case BEO_CMD_NUMBER_4:     return(PSTR("4"));
    case BEO_CMD_NUMBER_5:     return(PSTR("5"));
    case BEO_CMD_NUMBER_6:     return(PSTR("6"));
    case BEO_CMD_NUMBER_7:     return(PSTR("7"));
    case BEO_CMD_NUMBER_8:     return(PSTR("8"));
    case BEO_CMD_NUMBER_9:     return(PSTR("9"));
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
    case BEO_CMD_LEFT_REPEAT:  return(PSTR("left-repeat"));
    case BEO_CMD_RIGHT_REPEAT: return(PSTR("right-repeat"));
    case BEO_CMD_UP_REPEAT:    return(PSTR("up-repeat"));
    case BEO_CMD_DOWN_REPEAT:  return(PSTR("down-repeat"));
    case BEO_CMD_GO_REPEAT:    return(PSTR("go-repeat"));
    case BEO_CMD_GREEN_REPEAT: return(PSTR("green-repeat"));
    case BEO_CMD_YELLOW_REPEAT:return(PSTR("yellow-repeat"));
    case BEO_CMD_BLUE_REPEAT:  return(PSTR("blue-repeat"));
    case BEO_CMD_RED_REPEAT:   return(PSTR("red-repeat"));
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
    case BEO_CMD_ATAPE_2:      return(PSTR("atape2-on"));
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

//  Beo4 receiver state machine
//  receives timestamps from ISR-queue, computes PulseWith from timestamps and 
//  converts to PulseCode=(PulseWidth+1562)/3125. The first queue access is made 
//  waiting infinite. The subsequent reads are done with a 20 ms timeout. This is 
//  done because the TSOP7000 replica sometimes produces dummy pulses.
void beo4_rx_task(void *param) {
  if(NULL!=param) {
    IrBeo4 *beo=(IrBeo4*) param;      // get access to class
    TickType_t wait = WAIT_Infinite;  // waiting infinite in idle state
    int8_t     fsm = FSM_IDLE;        // wake up in IDLE state
    int64_t    tsSta=0;               // frame start timestamp
    int64_t    tsNew=0;               // new edge timestamp
    int64_t    tsPre=0;               // previous edge timestamp
    uint8_t    bitCnt=0;              // internal bit-counter, runs to 17
    uint8_t    preBit=0;              // previous bitCode
    uint32_t   pWidth=0;              // pulseWidth
    uint8_t    pCode=0;               // pulseCode
    uint32_t   beoCode=0;             // decoded Beo4 command

    // macro:
    // - log-message
    // - set state=FSM_IDLE
    // - set timeout=infinite
    #define RESET_FSM(a){ \
      log_i("%s: t=%lld fsm=%d pc=%d \n",a,tsNew,fsm,pCode); \
      wait=WAIT_Infinite; \
      fsm=FSM_IDLE; \
      continue; \
    }

    while(1) { // get pulses from ISR queue
      if(pdTRUE!=xQueueReceive(q_irs_queue,&tsNew,wait))
        RESET_FSM("TIMEOUT occured ");  // suppress hiccups of TSOP7000

      pWidth = (uint32_t)(tsNew-tsPre);           // compute pulse-width
      pCode  = (uint8_t)((pWidth + 1562) / 3125); // compute pulse-code
      tsPre  = tsNew;
      switch(fsm) {
        case FSM_IDLE: {
          wait=WAIT_20ms; // short timeout within frame (supress TSOP7000 hiccups )
          beoCode=0;      // reset beoCode
          bitCnt=0;       // reset bit counter
          tsSta=tsNew;    // mark start of frame
          fsm=FSM_S0;     // enter start sequence
          break;
        } 
        case FSM_S0: {
          if(BEO_ZERO!=pCode) RESET_FSM("ERROR: FSM_S_0 failed ");
          fsm=FSM_S1;     // continue start sequence
          break; 
        } 
        case FSM_S1: {
          if(BEO_ZERO!=pCode) RESET_FSM("ERROR: FSM_S_1 failed ");
          fsm=FSM_START;  // continue start sequence 
          break;
        } 
        case FSM_START: {
          if(BEO_START!=pCode) RESET_FSM("ERROR: FSM_START failed ");
          fsm=FSM_DATA;   // start with data collection
          break;
        } 
        case FSM_DATA: {
          int8_t curBit=0;
          switch(pCode) { // the pulsecode determines the bitCode
           case BEO_ZERO: curBit=preBit=0; break; 
           case BEO_SAME: curBit=preBit;   break;
           case BEO_ONE:  curBit=preBit=1; break;
           default: RESET_FSM("ERROR: invalid Data ");
          }
          beoCode=(beoCode<<1)+curBit;          // fill beoCode
          if((++bitCnt)==17) fsm=FSM_STOP;      // data ready, next: check the stopCodee
          break;
        }
        case FSM_STOP: {
          if(BEO_STOP!=pCode) RESET_FSM("ERROR: FSM_STOP failed ");
          beoCode=beoCode & 0xffff;             // store valid beoCode and start over
          if(beo_code_cb) beo_code_cb(beoCode); 
          if(beo->m_beo4_rx_queue) xQueueSend(beo->m_beo4_rx_queue,&beoCode,0);
          RESET_FSM("SUCCESS beoCode OK ");     // done, return to IDLE
          break;
        } 
        default: {
          RESET_FSM("ERROR:INVALID state");     // should not occur
          break;
        }
      } // switch(fsm)
    } // while(1)
  } // if(NULL!=parameter)
  #undef RESET_FSM
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
      ledcWrite(beo4->m_ir_tx_pwm,7); \
      esp_timer_start_once(beo4->m_OneShotTimer_h,200-TIME_CORR); \
      xEventGroupWaitBits(g_eg_handle, evPulse, pdTRUE, pdTRUE, portMAX_DELAY); \
      ledcWrite(beo4->m_ir_tx_pwm,0); \
      if(0<pulsecode) { \
        esp_timer_start_once(beo4->m_OneShotTimer_h, (pulsecode*3125)-200); \
        xEventGroupWaitBits(g_eg_handle, evPulse, pdTRUE, pdTRUE, portMAX_DELAY); \
      } \
    }

    // setup the PMW and attach to IR transmit pin
    ledcSetup(beo4->m_ir_tx_pwm,455000,4);  // pCarrier=455kHz,duty_range=[0..15]
    ledcAttachPin(beo4->m_ir_tx_pin,beo4->m_ir_tx_pwm); // attach to transmit pin
    ledcWrite(beo4->m_ir_tx_pwm,0);         // duty=0 --> wake up with output=low

    while(1){ // wait until beoCodes arrive at queue
      if(pdTRUE != xQueueReceive(beo4->m_beo4_tx_queue,&beoCode,portMAX_DELAY)) { 
        ets_printf("TaskBeoTx().xQueueReceive() failed ");  
        continue;
      }
      TX_PC(BEO_ZERO);         // transmit start sequence: 1,1,5
      TX_PC(BEO_ZERO);
      TX_PC(BEO_START);
      TX_PC(BEO_ZERO);         // transmit beoLink (always BEO_ZERO)
      curBit=preBit=0;         // init current and previous Bit
      for(ic=0;ic<16;ic++) {   // transmit 16 bits of BeoCode
        uint8_t pulseCode=0;
        curBit=(beoCode>>ic) & 1;
        if(curBit==preBit) pulseCode=BEO_SAME;
        else if(1==curBit) pulseCode=BEO_ONE;
        else               pulseCode=BEO_ZERO;
        preBit=curBit;
        TX_PC(pulseCode);       // transmit pulsecode for current Bit
      }
      TX_PC(BEO_STOP);          // transmit stop code
      TX_PC(0);                 // finish frame with final carrier pulse
    }
  } // if(NULL!=parameter)
  #undef TX_PC
}

// Interrupt service routine
void IRAM_ATTR ir_pulse_isr(void) {
  int64_t tsNew=esp_timer_get_time();
  xQueueSend(q_irs_queue,&tsNew,0);
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
  m_ir_rx_pin = rx_pin;
  m_ir_tx_pin = tx_pin;
  m_ir_tx_pwm = tx_pwm;
}

IrBeo4::~IrBeo4() {
}

int IrBeo4::Begin(QueueHandle_t beo4_rx_queue, QueueHandle_t beo4_tx_queue) {
  // take result queue
  m_beo4_rx_queue=beo4_rx_queue;
  m_beo4_tx_queue=beo4_tx_queue;
  // create internal timestamp queue (ISR-->FSM)
  if(NULL==(q_irs_queue=xQueueCreate(100, sizeof(int64_t))))
    ESP_ERROR_CHECK(ESP_ERR_INVALID_ARG);
  // create Task beo4 decoder
  if(pdPASS!=xTaskCreatePinnedToCore(beo4_rx_task,"beo4_rx_task",2048,this,1,&m_beo4_rx_task,0))
    ESP_ERROR_CHECK(ESP_FAIL);
  // create beo4 transmitter task if transmit pin and pwm-channel are set
  if(-1!=m_ir_tx_pin && -1!=m_ir_tx_pwm) {
    if(pdPASS!=xTaskCreate(beo4_tx_task, "beo4_tx_task", 2048,this,1,&m_beo4_tx_task))
      ESP_ERROR_CHECK(ESP_FAIL);
  }
  // activate ISR (handling the ir input signal)
  if(-1!=m_ir_rx_pin) { 
    pinMode(m_ir_rx_pin, INPUT);
    attachInterrupt(m_ir_rx_pin, ir_pulse_isr, RISING);
  } else {
    ESP_ERROR_CHECK(ESP_FAIL);
  }
  return 0;
}

