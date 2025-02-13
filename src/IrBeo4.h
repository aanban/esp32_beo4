#ifndef IRBEO4_H_
#define IRBEO4_H_

#include "Arduino.h"


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

extern const char* beo_src_tbl(uint32_t beo_code);
extern const char* beo_cmd_tbl(uint32_t beo_code);
extern const uint8_t isRepeatable(uint32_t beo_code);

// ----------------------------------------------------------------------------
// external call back functions
// ----------------------------------------------------------------------------
extern __attribute__((weak)) void beo_led_cb(uint8_t mode);


// ----------------------------------------------------------------------------
// Interrupt service routines
// ----------------------------------------------------------------------------

// - triggers on rising edge of IR signal, 
// - get timestamp and debounce possible noise pulses
// - put valid timestamps to q_irs_queue 
void IRAM_ATTR ir_pulse_isr(void);


// ----------------------------------------------------------------------------
// const expressions and enums
// ----------------------------------------------------------------------------

// Beo4 receicver state machine
enum class rxSt { Idle, S0, S1, Start, Data, Stop };

// timeout values, 20ms or infinite
constexpr TickType_t WAIT_infi  = portMAX_DELAY;  
constexpr TickType_t WAIT_20ms  = (20/portTICK_PERIOD_MS);
constexpr TickType_t WAIT_250ms = (250/portTICK_PERIOD_MS);

// transmit timing
constexpr uint64_t t_low_pulse=200;    ///< 455kHz carrier pulse time [microseconds]
constexpr uint64_t t_pc=3125;          ///< ZERO pulsecode time [microseconds]
constexpr uint64_t t_pc_half=1562;     ///< 0.5 * pulsecode time [microseconds]
constexpr uint32_t t_freq=455000;      ///< carrier frequency [Hz] --> 455 kHz
constexpr uint32_t t_resolution=4;     ///< duty resolution (bits) --> range = [0..15] --> duty=8=50%
constexpr uint32_t mskBC=0x0000FFFFul; ///< limit beoCode to 16 Bit

// TSOP7000 noise debounce-time 
constexpr int64_t t_debounce=600;      ///< debounce = 3 x t_low_pulse


// ----------------------------------------------------------------------------
// the class
// ----------------------------------------------------------------------------
class IrBeo4 {
  private:
    int8_t     m_rx_pin  = -1;           ///< IR receive pin
    int8_t     m_tx_pin  = -1;           ///< IR transmit pin
    int64_t    m_tsFrm   = 0;            ///< timestamp frame start
    int64_t    m_tsNew   = 0;            ///< timestamp new edge arrived
    int64_t    m_tsPre   = 0;            ///< timestamp previous edge
    uint8_t    m_bc      = 0;            ///< data bit counter 0..16
    uint8_t    m_preBit  = 0;            ///< previous bit code
    uint8_t    m_pCode   = 0;            ///< pulse code
    uint32_t   m_pWidth  = 0;            ///< pulse width 
    uint32_t   m_beoCode = 0;            ///< decoded Beo4 code
    uint8_t    m_beoWait = 0;            ///< repeatable code is waiting 
    rxSt       m_rxFSM   = rxSt::Idle;   ///< initial state receive statemachine
    TickType_t m_wait    = WAIT_infi;    ///< initial timeout reading isr-queue

    QueueHandle_t m_beo4_rx_queue;       ///< output queue for received beoCodes
    QueueHandle_t m_beo4_quarantine_queue;  ///< output queue for delayed received beoCodes
    QueueHandle_t m_beo4_tx_queue;       ///< input queue for beoCodes to be send
    
    
    //  Beo4 receiver state machine
    //  gets timestamps from ISR-queue, computes PulseWith and converts to 
    //  PulseCode=(PulseWidth+1562)/3125. The first queue access is made waiting 
    //  infinite. The subsequent reads are done with a 20 ms timeout. This is 
    //  done because the TSOP7000 replica sometimes produces dummy pulses.
    //  @param tsNew timestamp of received rising edge
    void RxFsm(int64_t tsNew);
    

    // if led callback function is set, turn ON/OFF the LED
    // @param mode HIGH=turn LED on  LOW=turn LED off
    inline void LED(uint8_t mode) {
      if(beo_led_cb) { 
        beo_led_cb(mode);
      }
    }

    // reset receive state machine, print logmessage, set timeout to infinite
    // set state to idle, turn LED off
    // @param str_log tacemessage
    inline void resetRxFsm(const char* str_log) {
      log_i("%s: t=%lld fsm=%d pc=%d \n",str_log,m_tsNew,m_rxFSM,m_pCode);
      m_wait=WAIT_infi;       // set timeout to infinite
      m_rxFSM=rxSt::Idle;     // start with idle
      LED(LOW);               // turn led OFF
    }
    
    // get timeout value
    // @return m_wait WAIT_infi or WAIT_20ms
    inline TickType_t get_wait(void){
      return this->m_wait;
    }

    // generate a 455kHz carrier-pulse and delay according to pulsecode
    // @param pulscode beo4 pulsecode [1..5]
    inline void tx_pc(uint8_t pulsecode) { 
      uint32_t pulse_width=(uint32_t)(((uint64_t)pulsecode*t_pc)-t_low_pulse);
      ledcWrite(m_tx_pin,8); // turn carrier on
      delayMicroseconds(t_low_pulse);
      ledcWrite(m_tx_pin,0); // turn carrier off
      if(0 < pulsecode) {
        delayMicroseconds(pulse_width);
      }
    }

    // Beo4 Receive Task, reads timestamps from ISR queue and call state-machine 
    // RxFsm() to decode start, data and stop codes. Correctly received codes are
    // pushed to the m_beo4_rx_queue. 
    // @param param handle to the class
    friend void beo4_rx_task(void *param); 
    TaskHandle_t m_beo4_rx_task; ///< task handle
    
    // Beo4 Quarantine Task, handels repeatable BeoCodes. 
    // The beo_rx_task sends all repeatabel BeoCodes to the 
    // m_beo4_quarantine_queue. They have to wait until either a timeout has 
    // expired or a successor BeoCode arrived. In case of successor BeoCode 
    // the event-Bis are set and the BeoCode will be skipped. In case of a
    // timeout the BeoCode is forwarded to the m_beo4_rx_queue and is treated 
    // as a single BeoCode.
    // @param param the class handle
    friend void beo4_quarantine_task(void *param); 
    TaskHandle_t m_beo4_quarantine_task; ///< task handle
    
    
    // Beo4 Transmit Task, reads beoCode from m_queue_beo_tx. The 455kHz carrier 
    // is generted by a PWM attached to m_ir_tx_pin ledcAttach(),ledcWrite(). The 
    // Pulses are generated by setting the duty-ratio of the m_tx_pin, i.e. 
    // ledcWrite(m_tx_pin,8)=carrier and ledcWrite(m_tx_pin,0)=pulsecode. 
    // Start/stop pulses are added to complete the frame. 
    // @param handle handle to the class
    friend void beo4_tx_task(void *handle); 
    TaskHandle_t m_beo4_tx_task; ///< task handle

  protected:
  public:
    // constructor
    // @param rx_pin input IR receiver pin
    // @param tx_pin output IR transmit pin (default=-1, not used)
    IrBeo4(int8_t rx_pin, int8_t tx_pin=-1);
    
    // destructor
    ~IrBeo4();

    // initialize ISR and queues, attach the interrupt service routine to the 
    // rx_pin (rising edge) ,create isr queue, start rx task, start tx task
    // @param beo4_rx_queue Beo4 receive  queue 
    // @param beo4_tx_queue Beo4 transmit queue 
    // @return 0=OK -1=create queue failed -2=create rx task failed -3=create tx task failed
    int Begin(QueueHandle_t beo4_rx_queue=NULL, QueueHandle_t beo4_tx_queue=NULL);

};
#endif /* IRBEO4_H_ */
