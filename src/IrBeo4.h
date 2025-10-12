#ifndef IRBEO4_H_
#define IRBEO4_H_

#include "Arduino.h"
#include "driver/rmt_tx.h"
#include "driver/rmt_rx.h"
#include "esp_log.h"

constexpr const char* IrBeo4Version = "1.4.0";
constexpr const char* IR_BEO4 = "IrBeo4";

// ----------------------------------------------------------------------------
// Helper c-funtions const expressions
// ----------------------------------------------------------------------------

// beo4 source codes
constexpr uint8_t BEO_SRC_VIDEO        = 0x00;
constexpr uint8_t BEO_SRC_AUDIO        = 0x01;
constexpr uint8_t BEO_SRC_VTAPE        = 0x05;
constexpr uint8_t BEO_SRC_ALL          = 0x0F;
constexpr uint8_t BEO_SRC_SPDEMO       = 0x1D;
constexpr uint8_t BEO_SRC_LIGHT        = 0x1B;

// beo4 command codes
constexpr uint8_t BEO_CMD_NUM_0        = 0x00;
constexpr uint8_t BEO_CMD_NUM_1        = 0x01;
constexpr uint8_t BEO_CMD_NUM_2        = 0x02;
constexpr uint8_t BEO_CMD_NUM_3        = 0x03;
constexpr uint8_t BEO_CMD_NUM_4        = 0x04;
constexpr uint8_t BEO_CMD_NUM_5        = 0x05;
constexpr uint8_t BEO_CMD_NUM_6        = 0x06;
constexpr uint8_t BEO_CMD_NUM_7        = 0x07;
constexpr uint8_t BEO_CMD_NUM_8        = 0x08;
constexpr uint8_t BEO_CMD_NUM_9        = 0x09;
constexpr uint8_t BEO_CMD_CLEAR        = 0x0A;
constexpr uint8_t BEO_CMD_STORE        = 0x0B;
constexpr uint8_t BEO_CMD_STANDBY      = 0x0C;
constexpr uint8_t BEO_CMD_MUTE         = 0x0D;
constexpr uint8_t BEO_CMD_INDEX        = 0x0E;
constexpr uint8_t BEO_CMD_UP           = 0x1E;
constexpr uint8_t BEO_CMD_DOWN         = 0x1F;
constexpr uint8_t BEO_CMD_TUNE         = 0x20;
constexpr uint8_t BEO_CMD_CLOCK        = 0x28;
constexpr uint8_t BEO_CMD_FORMAT       = 0x2A;
constexpr uint8_t BEO_CMD_LEFT         = 0x32;
constexpr uint8_t BEO_CMD_RETURN       = 0x33;
constexpr uint8_t BEO_CMD_RIGHT        = 0x34;
constexpr uint8_t BEO_CMD_GO           = 0x35;
constexpr uint8_t BEO_CMD_STOP         = 0x36;
constexpr uint8_t BEO_CMD_RECORD       = 0x37;
constexpr uint8_t BEO_CMD_SELECT       = 0x3F;
constexpr uint8_t BEO_CMD_SPEAKER      = 0x44;
constexpr uint8_t BEO_CMD_PICTURE      = 0x45;
constexpr uint8_t BEO_CMD_TURN         = 0x46; // list->mono
constexpr uint8_t BEO_CMD_LOUDNESS     = 0x48;
constexpr uint8_t BEO_CMD_BASS         = 0x4D;
constexpr uint8_t BEO_CMD_TREBLE       = 0x4E;
constexpr uint8_t BEO_CMD_BALANCE      = 0x4F;
constexpr uint8_t BEO_CMD_LIST         = 0x58;
constexpr uint8_t BEO_CMD_MENU         = 0x5C;
constexpr uint8_t BEO_CMD_VOL_UP       = 0x60;
constexpr uint8_t BEO_CMD_VOL_DOWN     = 0x64;
constexpr uint8_t BEO_CMD_LEFT_REPEAT  = 0x70;
constexpr uint8_t BEO_CMD_RIGHT_REPEAT = 0x71;
constexpr uint8_t BEO_CMD_UP_REPEAT    = 0x72;
constexpr uint8_t BEO_CMD_DOWN_REPEAT  = 0x73;
constexpr uint8_t BEO_CMD_GO_REPEAT    = 0x75;
constexpr uint8_t BEO_CMD_GREEN_REPEAT = 0x76;
constexpr uint8_t BEO_CMD_YELLOW_REPEAT= 0x77;
constexpr uint8_t BEO_CMD_BLUE_REPEAT  = 0x78;
constexpr uint8_t BEO_CMD_RED_REPEAT   = 0x79;
constexpr uint8_t BEO_CMD_EXIT         = 0x7F;
constexpr uint8_t BEO_CMD_TV           = 0x80;
constexpr uint8_t BEO_CMD_RADIO        = 0x81;
constexpr uint8_t BEO_CMD_VIDEO_AUX    = 0x82;
constexpr uint8_t BEO_CMD_AUDIO_AUX    = 0x83;
constexpr uint8_t BEO_CMD_VTAPE        = 0x85;
constexpr uint8_t BEO_CMD_DVD          = 0x86;
constexpr uint8_t BEO_CMD_CAMCORD      = 0x87;
constexpr uint8_t BEO_CMD_TEXT         = 0x88;
constexpr uint8_t BEO_CMD_SP_DEMO      = 0x89;
constexpr uint8_t BEO_CMD_SAT          = 0x8A;
constexpr uint8_t BEO_CMD_PC           = 0x8B;
constexpr uint8_t BEO_CMD_DOOR_CAM     = 0x8D;
constexpr uint8_t BEO_CMD_ATAPE        = 0x91;
constexpr uint8_t BEO_CMD_CD           = 0x92;
constexpr uint8_t BEO_CMD_PHONO        = 0x93;
constexpr uint8_t BEO_CMD_ATAPE2       = 0x94;
constexpr uint8_t BEO_CMD_CD2          = 0x97;
constexpr uint8_t BEO_CMD_LIGHT        = 0x9B;
constexpr uint8_t BEO_CMD_AV           = 0xBF;
constexpr uint8_t BEO_CMD_YELLOW       = 0xD4;
constexpr uint8_t BEO_CMD_GREEN        = 0xD5;
constexpr uint8_t BEO_CMD_BLUE         = 0xD8;
constexpr uint8_t BEO_CMD_RED          = 0xD9;
constexpr uint8_t BEO_CMD_STAND        = 0xf7; 


// ----------------------------------------------------------------------------
// external c-helper functions
// ----------------------------------------------------------------------------

extern const char* beo_src_tbl(uint32_t beo_code);
extern const char* beo_cmd_tbl(uint32_t beo_code);
extern const uint8_t isRepeatable(uint32_t beo_code);

// ----------------------------------------------------------------------------
// external call back functions
// ----------------------------------------------------------------------------

// is called when transmit starts to turn LED on and when transmit ends 
// to turn LED off
extern __attribute__((weak)) void beo_led_cb(uint8_t mode);


// ----------------------------------------------------------------------------
// const expressions and enums
// ----------------------------------------------------------------------------

// timeout values, 20ms or infinite
constexpr TickType_t WAIT_infi  = portMAX_DELAY;  
constexpr TickType_t WAIT_300ms = (300/portTICK_PERIOD_MS);

// ----------------------------------------------------------------------------
// the class
// ----------------------------------------------------------------------------
class IrBeo4 {
  private:
    int8_t     m_rx_pin  = -1;               ///< IR receive pin
    int8_t     m_tx_pin  = -1;               ///< IR transmit pin
    int8_t     m_lc_mode = 0;                ///< long-code-mode 0=skip waiting code 1=keep waiting code
    uint8_t    m_bc      = 0;                ///< data bit counter 0..16
    uint8_t    m_preBit  = 0;                ///< previous bit code
    uint8_t    m_pCode   = 0;                ///< pulse code
    uint32_t   m_pWidth  = 0;                ///< pulse width 
    uint32_t   m_beoCode = 0;                ///< decoded Beo4 code
    uint8_t    m_beoWait = 0;                ///< repeatable code is waiting 
    TickType_t m_wait    = WAIT_infi;        ///< initial timeout reading isr-queue

    QueueHandle_t m_beo4_rx_queue;           ///< receive queue beoCodes
    QueueHandle_t m_beo4_rmt_rx_queue;       ///< receive queue beoCodes from RMT
    QueueHandle_t m_beo4_quarantine_queue;   ///< quarantine queue for waiting beoCodes
    
    QueueHandle_t m_beo4_tx_queue;           ///< input queue for beoCodes to be send

    // rmt transmit variables
    rmt_tx_channel_config_t  m_tx_cfg;       ///< rmt transmit channel configuration 
    rmt_carrier_config_t     m_tx_carr_cfg;  ///< rmt transmit carrier configuration
    rmt_transmit_config_t    m_transmit_cfg; ///< rmt transmit configuration

    rmt_channel_handle_t     m_tx_chan=NULL; ///< rmt transmit channel 
    rmt_simple_encoder_config_t m_beo_encoder_cfg;    ///< encoder-config for beo4 codes
    rmt_encoder_handle_t        m_beo_encoder = NULL; ///< encoder handle

    // rmt receive variables
    rmt_rx_channel_config_t m_rx_cfg;        ///< rmt receive channel configuration
    rmt_receive_config_t m_rx_receive_cfg;   ///< rmt receive configuration
    rmt_channel_handle_t m_rx_chan=NULL;     ///< rmt receive channel
    rmt_rx_event_callbacks_t m_rx_event_cbs; ///< rmt event callback config


    // setup for RMT channel m_tx_chan for transmitting beo4 codes. 
    // - prepares tx-channel config and create m_tx_chan
    // - prepares carrier config and apply to m_tx_chan
    // - prepares simple encoder config and create m_beo_encoder  
    // - calls rmt_enable() to start m_tx_chan
    int rmt_tx_setup(void);

    // setup for RMT channel m_rx_chan for receiving beo4 codes. 
    // - creates receive queues and tasks 
    // - prepares rx-channel config and create m_rx_chan 
    // - registers callback for received frame handling
    // - calls rmt_enable() to start m_rx_chan
    int rmt_rx_setup(void);
    
    // filter and parse raw date from RMT receiver
    // - suppress short dummy codes (n_sym <15) from TSOP7000 hiccups
    // - suppress invalid pulses (pulses < 1500) 
    // - find stop-symbol
    // - decodes 17 data-symbols
    // - send beoCode to queue
    // @param sym pointer to raw data symbols
    // @param n_sym number of raw data symbols
    void parse_raw_data(rmt_symbol_word_t *sym, size_t n_sym);

    // if led callback function is set, turn ON/OFF the LED
    // @param mode HIGH=turn LED on  LOW=turn LED off
    inline void LED(uint8_t mode) {
      if(beo_led_cb) { 
        beo_led_cb(mode);
      }
    }
            
    // Beo4 Receive Task, reads RMT symbols from m_beo4_rmt_rx_queue and 
    // calls parse_raw_data() to decode raw-data symbols to beoCodes.
    // Correctly received beoCodes are sent in separate queues. Repeatable 
    // beoCodes end up in the m_beo4_quarantine_queue, single beoCodes are 
    // send directly to the m_beo4_rx_queue. 
    // @param param handle to the class
    friend void beo4_rx_task(void *param); 
    TaskHandle_t m_beo4_rx_task; ///< task handle
    
    // Beo4 Quarantine Task, handles repeatable BeoCodes. 
    // Repeatable beoCodes arrives at m_beo4_quarantine_queue. They have to
    // wait until either a timeout has expired or a successor BeoCode arrived.
    // In case of a successor beoCode the event-Bis are set and the beoCode 
    // will be skipped. In case of a timeout the beoCode is forwarded to the 
    // m_beo4_rx_queue and is treated as a single BeoCode.
    // @param param the class handle
    friend void beo4_quarantine_task(void *param); 
    TaskHandle_t m_beo4_quarantine_task; ///< task handle
    
    
    // Beo4 Transmit Task, reads beoCode from m_queue_beo_tx. 
    // RMT symbols are generated within the callback function 
    // beo4_encode_cb() and finally transmitted via RMT channel 
    // m_tx_chan. 
    // @param handle handle to the class
    friend void beo4_tx_task(void *handle); 
    TaskHandle_t m_beo4_tx_task; ///< task handle

  protected:
  public:
    // constructor
    // @param rx_pin input IR receiver pin
    // @param tx_pin output IR transmit pin (default=-1, not used)
    // @param lc_mode long-code mode, 0=skip waiting code 1=keep waiting code
    IrBeo4(int8_t rx_pin, int8_t tx_pin=-1, int8_t lc_mode=0);
    
    // destructor
    ~IrBeo4();

    // initialize queues, tasks and setup m_rx_chan and m_tx_chan depending
    // of the given beo4_rx_queue and beo4_tx_queue
    // @param beo4_rx_queue Beo4 receive queue 
    // @param beo4_tx_queue Beo4 transmit queue 
    // @return ESP_OK=setup Ok ESP_FAIL=setup failed
    int Begin(QueueHandle_t beo4_rx_queue=NULL, QueueHandle_t beo4_tx_queue=NULL);

    // returns Version of IrBeo4 Lib
    inline const char* GetVersion(void) {
      return IrBeo4Version;
    }

};
#endif /* IRBEO4_H_ */
