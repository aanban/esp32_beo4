#include "IrBeo4.h"

// ----------------------------------------------------------------------------
// Helper c-functions 
// ----------------------------------------------------------------------------

// checks if the beoCommand of a given beo_codeis repeatable 
// @param beo_code to be checked
// @return 0=single code 1=repeatable code
const uint8_t isRepeatable(uint32_t beo_code){
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

// extracts BeoSource from a given beo_code as string
// @param beo_code=Beo4 code
// return BeoSource or "invalid_src" for unknown beoCodes
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

// extracts BeoCommand from a given beo_code as string
// @param beo_code=Beo4 code 
// return BeoCommand or "invalid_cmd" for unknwon beoCodes
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
// constexpr
// ----------------------------------------------------------------------------

// Beo4 pulseWidth 
constexpr uint16_t pwCarr = 200;    // carrier pulse length
constexpr uint16_t pwZero = 2925;   // + 200 =  3125 µs
constexpr uint16_t pwSame = 6050;   // + 200 =  6250 µs
constexpr uint16_t pwOne  = 9175;   // + 200 =  9375 µs
constexpr uint16_t pwStop = 12300;  // + 200 = 12500 µs
constexpr uint16_t pwStart= 15425;  // + 200 = 15625 µs
constexpr uint16_t pwEOT  = 0;      // last symbol has no duration1

// Beo4 pulseCode = pwCarr+pulseWidth / 3125
constexpr uint8_t pcZero  = (pwCarr+pwZero ) / 3125;
constexpr uint8_t pcSame  = (pwCarr+pwSame ) / 3125;
constexpr uint8_t pcOne   = (pwCarr+pwOne  ) / 3125;
constexpr uint8_t pcStop  = (pwCarr+pwStop ) / 3125;
constexpr uint8_t pcStart = (pwCarr+pwStart) / 3125;

constexpr uint32_t nBit = 17;          // beoLink + beoSrc + beoCmd = 1+8+8=17
constexpr size_t min_sym_free  = 22;   // RMT-symbols per beoFrame


// ----------------------------------------------------------------------------
// global variables
// ----------------------------------------------------------------------------

DRAM_ATTR EventGroupHandle_t g_eg_handle;   // eventgroup handle for quarantine task
constexpr uint32_t evRptCode = (1 << 0);    // event bits for quarantine task

static rmt_symbol_word_t raw_symbols[1024]; // rmt receive buffer for raw-data 
static rmt_rx_done_event_data_t rx_data;    // rmt receive event-data

// Beo4 Pulse Codes mapped to rmt_symbol_word_t symbols 
static const rmt_symbol_word_t BeoZero  = { .duration0=pwCarr, .level0=1, .duration1=pwZero , .level1=0 };
static const rmt_symbol_word_t BeoSame  = { .duration0=pwCarr, .level0=1, .duration1=pwSame , .level1=0 };
static const rmt_symbol_word_t BeoOne   = { .duration0=pwCarr, .level0=1, .duration1=pwOne  , .level1=0 };
static const rmt_symbol_word_t BeoStart = { .duration0=pwCarr, .level0=1, .duration1=pwStart, .level1=0 };
static const rmt_symbol_word_t BeoStop  = { .duration0=pwCarr, .level0=1, .duration1=pwStop , .level1=0 };
static const rmt_symbol_word_t BeoEOT   = { .duration0=pwCarr, .level0=1, .duration1=pwEOT  , .level1=0 };


// ----------------------------------------------------------------------------
// callback c-funtions
// ----------------------------------------------------------------------------

// rmt transmit callback function. Encodes rmt_symbol_word symbols from a given beoCode. 
// A complete frame is encoded, i.e. sym_free must be at least 22 symbols, else 
// the function will return immediately. After the 22 symbols are filled, the 
// function returns with # of symbols written and sets *done=true to state the 
// end-of-transmit info to the rmt-driver.
// @param data pointer to the current beo_code 
// @param n_data (not used here) size of data in bytes
// @param sym_wr (not used here) symbols written
// @param sym_free number of free symbols in symbol-buffer of rmt-driver 
// @param sym pointer to first free symbol in symbol-buffer of rmt-driver
// @param done is set to true, afte the complete frame is written to the symbol-buffer
// @param arg (not uses here)
static size_t beo4_encode_cb(const void *data, size_t n_data, size_t sym_wr, size_t sym_free, 
                             rmt_symbol_word_t *sym, bool *done, void *arg) {
  size_t sc=0;                               // encoded symbols counter 
  if(sym_free >= min_sym_free) {             // required free symbols for a complete BeoCode frame
    uint16_t *beo_code=(uint16_t*) data;     // the beo_code to be encoded
    size_t   jc=0,ic=0;                      // loop counters
    uint32_t curBit=0;                       // current bitCode
    uint32_t preBit=0;                       // previous bitCode
    sym[sc++]=BeoZero;                       // preset START sequence=zero,zero,start
    sym[sc++]=BeoZero; 
    sym[sc++]=BeoStart;
    sym[sc++]=BeoZero;                       // BeoLink bit is always zero
    for(jc=15,ic=0;ic<16;ic++,jc--) {        // extract bits of beo_code form left to right
      curBit=((*beo_code)>>jc) & 1;          // current and previous bits are compared
      if(curBit==preBit) sym[sc++]=BeoSame;  // and symbols are set like so:
      else if(1==curBit) sym[sc++]=BeoOne;
      else               sym[sc++]=BeoZero;
      preBit=curBit;                         // remember previous bit for next compare
    }
    sym[sc++]=BeoStop;                       // stop symbol
    sym[sc++]=BeoEOT;                        // carrier pulse only
    *done=1;                                 // inform rmt-driver: complete frame in buffer
  }
  return sc;                                 // number of encoded symbols
}


// rmt receive callback function. Once a complete raw-data frame is recorded 
// on the rmt channel this function pushes the data-pointer (=edata) to the 
// queue m_beo4_rmt_rx_queue (=user_data)to trigger the beo4_rx_task. 
//
// Note: the queue and the data-pointers were set during setup 
//       the rmt-receive-channel within rmt_rx_setup().
//
// @param channel = the running rmt channel
// @param edata = data-pointers (here: data within raw_symbols[])
// @param user_data = queue (here: m_beo4_rmt_rx_queue)
// @return 1=hight_task_wakeup==pdTRUE 0=else
static bool beo4_rx_done_cb(rmt_channel_handle_t channel, const rmt_rx_done_event_data_t *edata, void *user_data) {
  BaseType_t high_task_wakeup = pdFALSE;
  QueueHandle_t receive_queue = (QueueHandle_t)user_data;
  xQueueSendFromISR(receive_queue, edata, &high_task_wakeup);
  return high_task_wakeup == pdTRUE;
}


// ----------------------------------------------------------------------------
// Tasks
// ----------------------------------------------------------------------------

// controls repeatable BeoCodes that are waiting in quarantine
void beo4_quarantine_task(void *handle){
  if(NULL!=handle){
    IrBeo4  *beo=(IrBeo4*) handle;  // get access to class
    uint32_t repeatableBeoCode=0;   // repeatable BeoCode could have a successor
    EventBits_t uxBits;             // used as trigger in case successor arrived
    for(;;){
      if(pdTRUE==xQueueReceive(beo->m_beo4_quarantine_queue,&repeatableBeoCode,WAIT_infi)) {
        // repeatable BeoCode has arrived, wait for trigger-event or time out
        uxBits=xEventGroupWaitBits(g_eg_handle, evRptCode, pdTRUE, pdTRUE, WAIT_250ms);
        if(evRptCode==(uxBits & evRptCode)) { // successor BeoCode detected 
          beo->m_beoWait=0;                   // reset flag, BeoCode is removed
          ESP_LOGI(IR_BEO4,".\n");
        } else {  // timeout detected, delayed code can be send
          xQueueSend(beo->m_beo4_rx_queue,&repeatableBeoCode,0);
          beo->m_beoWait=0;                   // reset flag, BeoCode is send to standard qeuue
          ESP_LOGI(IR_BEO4,"_\n");
        }
      }
    }
  }
}

// receive task 
void beo4_rx_task(void *handle) {
  if(NULL!=handle) {
    IrBeo4   *beo=(IrBeo4*) handle; // get access to class
    int64_t  tsNew=0;               // new edge timestamp
    for(;;) {
      if(pdPASS==xQueueReceive(beo->m_beo4_rmt_rx_queue, &rx_data, WAIT_infi)){
        // raw data available, parse and decode
        beo->parse_raw_data(rx_data.received_symbols, rx_data.num_symbols);
        // start next RMT receive cycle
        rmt_receive(beo->m_rx_chan, raw_symbols, sizeof(raw_symbols), &beo->m_rx_receive_cfg);
      }
    }
  }
}

// transmit task
void beo4_tx_task(void *param){
  if(NULL!=param) {  
    IrBeo4 *beo4 = (IrBeo4*) param;  // get access beo4 class
    uint32_t data=0;                 // data-word from queue
    uint16_t beoCode=0;              // BeoCode to be transmitted
    beo4->rmt_tx_setup();            // init RMT driver
    for(;;){
      if(pdTRUE==xQueueReceive(beo4->m_beo4_tx_queue,&data,portMAX_DELAY)) { 
        beo4->LED(HIGH);             // trasnmit starts --> LED ON 
        beoCode=(uint16_t)data;
        if(ESP_OK==rmt_transmit(beo4->m_tx_chan, beo4->m_beo_encoder, &beoCode,sizeof(beoCode), &beo4->m_transmit_cfg)) {
          rmt_tx_wait_all_done(beo4->m_tx_chan, portMAX_DELAY);
        }
        beo4->LED(LOW);              // trasmit finished --> LED OFF 
      }
    }
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

// setup for RMT transmit channel and carrier
int IrBeo4::rmt_tx_setup(void) {
  m_tx_cfg.gpio_num          = (gpio_num_t) m_tx_pin; // output GPIO pin
  m_tx_cfg.clk_src           = RMT_CLK_SRC_DEFAULT;   // RMT_CLK_SRC_APB
  m_tx_cfg.resolution_hz     = 1000000u;              // 1MHz --> 1µs resolution
  m_tx_cfg.trans_queue_depth = 4; 
  m_tx_cfg.mem_block_symbols = 64;
  m_tx_cfg.flags.invert_out  = false;                 // idle=low, carrier_pules=high
  m_tx_cfg.flags.with_dma    = false;                 // does not work with esp32
  ESP_ERROR_CHECK(rmt_new_tx_channel(&m_tx_cfg, &m_tx_chan)); 
  
  m_tx_carr_cfg.duty_cycle   = 0.5;                   // duty-cycle = 50%
  m_tx_carr_cfg.frequency_hz = 455000u;               // beo4 freq  = 455kHz
  m_tx_carr_cfg.flags.polarity_active_low = false; 
  ESP_ERROR_CHECK(rmt_apply_carrier(m_tx_chan, &m_tx_carr_cfg));

  m_beo_encoder_cfg.callback=beo4_encode_cb;          // callback for beo4 encoder
  ESP_ERROR_CHECK(rmt_new_simple_encoder(&m_beo_encoder_cfg, &m_beo_encoder));

  m_transmit_cfg.loop_count = 0;                      // prepare transmit configuration

  ESP_ERROR_CHECK(rmt_enable(m_tx_chan));             // activate the transmit channel
  return ESP_OK;
}

// setup queues, tasks, and RMT receive channel 
int IrBeo4::rmt_rx_setup(void) {
  pinMode(m_rx_pin, INPUT_PULLUP);
  if(NULL==(g_eg_handle = xEventGroupCreate())){ 
    ESP_LOGE(IR_BEO4,"g_eg_handle failed\n");
    return -1; 
  }
  if(NULL==(m_beo4_quarantine_queue=xQueueCreate(5, sizeof(uint32_t)))) {
    ESP_LOGE(IR_BEO4,"m_beo4_quarantine_queue failed\n");
    return -1;
  }
  if(pdPASS!=xTaskCreatePinnedToCore(beo4_quarantine_task,"beo4_quarantine_task",2048,this,1,&m_beo4_quarantine_task,1)){
    ESP_LOGE(IR_BEO4,"beo4_quarantine_task failed\n");
    return -1;
  }
  if(NULL==(m_beo4_rmt_rx_queue=xQueueCreate(1, sizeof(rmt_rx_done_event_data_t)))) {
    ESP_LOGE(IR_BEO4,"m_beo4_rmt_rx_queue failed\n");
    return -3;
  }
  if(pdPASS!=xTaskCreatePinnedToCore(beo4_rx_task,"beo4_rx_task",2048,this,1,&m_beo4_rx_task,1)) {
    ESP_LOGE(IR_BEO4,"beo4_rx_task failed\n");
    return -4; 
  }
  m_rx_cfg.gpio_num          = (gpio_num_t) m_rx_pin;
  m_rx_cfg.clk_src           = RMT_CLK_SRC_DEFAULT; 
  m_rx_cfg.resolution_hz     = 1000000u;             // 1MHz --> 1µs resolution
  m_rx_cfg.mem_block_symbols = 512;
  m_rx_cfg.intr_priority     = 3; 
  m_rx_cfg.flags.invert_in   = true;
  m_rx_cfg.flags.with_dma    = false; 
  ESP_ERROR_CHECK(rmt_new_rx_channel(&m_rx_cfg, &m_rx_chan));
  
  m_rx_event_cbs.on_recv_done=beo4_rx_done_cb;      // register callback for "frame received" event
  ESP_ERROR_CHECK(rmt_rx_register_event_callbacks(m_rx_chan, &m_rx_event_cbs, m_beo4_rmt_rx_queue));

  m_rx_receive_cfg.signal_range_min_ns =     2000;   //    10µs <   200 µs
  m_rx_receive_cfg.signal_range_max_ns = 20000000;   // 20000µs > 15625 µs
  m_rx_receive_cfg.flags.en_partial_rx = false; 

  ESP_ERROR_CHECK(rmt_enable(m_rx_chan));            // activate the recieve channel
  return ESP_OK;
}


// setup of beo4 receiver and beo4 transmitter
int IrBeo4::Begin(QueueHandle_t beo4_rx_queue, QueueHandle_t beo4_tx_queue) {
  // init receiver, if gpio-pin and queue are set
  if(-1!=m_rx_pin && NULL!=beo4_rx_queue) {
    m_beo4_rx_queue=beo4_rx_queue;
    if(ESP_OK==rmt_rx_setup()) {
      ESP_ERROR_CHECK(rmt_receive(m_rx_chan, raw_symbols, sizeof(raw_symbols), &m_rx_receive_cfg)); 
      return ESP_OK; 
    }
  } 
  // init transmitter, if gpio-pin and queue are set
  if(-1!=m_tx_pin && NULL!=beo4_tx_queue) {
    m_beo4_tx_queue=beo4_tx_queue;
    if(ESP_OK==rmt_tx_setup()) {
      if(pdPASS!=xTaskCreate(beo4_tx_task, "beo4_tx_task", 2048,this,1,&m_beo4_tx_task)) {
        ESP_LOGE(IR_BEO4,"beo4_tx_task failed");
        return ESP_FAIL;
      }
    }
  }
  return ESP_OK;
}

// fsm that decodes pulseCodes to beoCode
void IrBeo4::beo_decode_fsm(uint32_t pulseCode, size_t n_sym) {
  static uint32_t beoCode=0;   // decode beoCode
  static uint32_t preBit=0;    // previous bit 
  static size_t   cntBit=0;    // count bits until 17
  
  switch(m_rxFSM) {
    case rxSt::Idle: { // waiting for start-code
      beoCode=0;
      cntBit=0;
      preBit=0;
      if(pcStart==pulseCode) {
        m_rxFSM=rxSt::Data;    // next--> collect data
        if(1==m_beoWait) {     // a code waits in quarantine
          m_beoWait=0;         // set event for quarantine_task
          xEventGroupSetBits(g_eg_handle,evRptCode); 
        }
      }
      break;
    }
    case rxSt::Data: { // collecting data
      uint32_t curBit=0; 
      switch(pulseCode) {
        case pcZero: { curBit=preBit=0; break; }
        case pcSame: { curBit=preBit;   break; }
        case pcOne:  { curBit=preBit=1; break; }
        default: { 
          m_rxFSM=rxSt::Idle;
          return;
        }
      }
      beoCode = (beoCode << 1) + curBit; 
      if(++cntBit == nBit) {    // collecting data done
        m_rxFSM=rxSt::Stop;     // next--> validate stop-code
      }
      break; 
    }
    case rxSt::Stop: { // validate stop code
      if(pcStop == pulseCode) { // stop code valid
        uint32_t data = ((uint32_t)n_sym << 16) + (beoCode & 0xffff); 
        uint32_t beoCmd = beoCode & 0xff;
        if(isRepeatable(beoCmd)) {
          m_beoWait=1; // let code wait in quarantine
          xQueueSend(m_beo4_quarantine_queue,&data,0);
        } else {
          m_beoWait=0; // send out directly
          xQueueSend(m_beo4_rx_queue,&data,0);
        }
      }
      m_rxFSM=rxSt::Idle;
      break; 
    }
  }
}

// filtes and parses raw-data using the beo_decode_fsm() to decode pulseCode 
void IrBeo4::parse_raw_data(rmt_symbol_word_t *sym, size_t n_sym) {
  ESP_LOGI(IR_BEO4,"n_sym=%d ",  n_sym);
  if(n_sym > 10) {  // suppress short dummy codes (TSO7000 hiccups )
    ESP_LOGI(IR_BEO4,"\nstart-------------------");
    m_rxFSM=rxSt::Idle;
    for(size_t ic=0; ic<n_sym; ic++) {
      uint16_t pulseWidth = sym[ic].duration0 + sym[ic].duration1;
      if(pulseWidth>1500) {                              // suppress invalid pulses
        uint16_t pulseCode = beo_pulse_code(pulseWidth); // round to pulse code [1..5]
        beo_decode_fsm(pulseCode, n_sym);                // call beo4 decoder 
        ESP_LOGI(IR_BEO4,"\n%5d %5d %5d %5d  ",sym[ic].duration0,sym[ic].duration1,pulseWidth,pulseCode);
      }
    }
    ESP_LOGI(IR_BEO4,"\nend---------------------\n");
  }
}

