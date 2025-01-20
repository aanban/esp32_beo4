#ifndef IRBEO4_H_
#define IRBEO4_H_

#include "Arduino.h"

// external call back functions
extern __attribute__((weak)) void beo_code_cb(uint32_t beo_code);
extern __attribute__((weak)) void beo_led_cb(uint8_t mode);

// Beo4 receicver state machine
enum class rxSt { Idle, S0, S1, Start, Data, Stop };


// Event do-pulse
constexpr uint32_t evPulse = (1 << 0);     // BIT_1

// one-shut-timer and event handler for transmit task
extern EventGroupHandle_t g_eg_handle;

// timeout values, 20ms or infinite
constexpr TickType_t WAIT_infi = portMAX_DELAY;  
constexpr TickType_t WAIT_20ms = (20/portTICK_PERIOD_MS);

// transmit timing
constexpr uint64_t t_corr=50;          ///< correction [microseconds]
constexpr uint64_t t_low_pulse=200;    ///< carrier pulse time [microseconds]
constexpr uint64_t t_pc=3125;          ///< ZERO pulsecode time [microseconds]
constexpr uint64_t t_pc_half=1562;     ///< 0.5 pulsecode time [microseconds]
constexpr uint32_t t_freq=455000;      ///< 455kHz carrier frequency
constexpr uint32_t t_resolution=4;     ///< duty resolution (bits) --> range = [0..15] --> duty=8=50%
constexpr uint32_t mskBC=0x0000FFFFul; ///< limit beoCode to 16 Bit

// TSOP7000 noise debounce-time 
constexpr int64_t t_debounce=600;      ///< debounce = 3 x t_low_pulse

// interupt-serice-routine
// - triggers on rising edge of IR signal, 
// - get timestamp and debounce possible noise pulses
// - put valid timestamps to q_irs_queue 
void IRAM_ATTR ir_pulse_isr(void);

// internal callback function for one-shot-timer
// @param arg not used here
void IRAM_ATTR one_shot_timer_cb(void* arg);

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
    rxSt       m_rxFSM   = rxSt::Idle;   ///< initial state receive statemachine
    TickType_t m_wait    = WAIT_infi;    ///< initial timeout reading isr-queue

    QueueHandle_t m_beo4_rx_queue;       ///< output queue for received beoCodes
    QueueHandle_t m_beo4_tx_queue;       ///< input queue for beoCodes to be send
    esp_timer_handle_t m_OneShotTimer_h; ///< ont-shot-timer handle 
    
    //  Beo4 receiver state machine
    //  gets timestamps from ISR-queue, computes PulseWith and converts to 
    //  PulseCode=(PulseWidth+1562)/3125. The first queue access is made waiting 
    //  infinite. The subsequent reads are done with a 20 ms timeout. This is 
    //  done because the TSOP7000 replica sometimes produces dummy pulses.
    //  @param tsNew timestamp of received rising edge
    void RxFsm(int64_t tsNew);
    
    // reset receive state machine, print logmessage, set timeout to infinite
    // set state to idle, turn LED off
    // @param str_log tacemessage
    inline void resetRxFsm(const char* str_log) {
      log_i("%s: t=%lld fsm=%d pc=%d \n",str_log,m_tsNew,m_rxFSM,m_pCode);
      m_wait=WAIT_infi;       // set timeout to infinite
      m_rxFSM=rxSt::Idle;     // start with idle
      if(beo_led_cb) {        // if led available turn LED off
        beo_led_cb(HIGH);
      }
    }
    
    // get timeout value
    // @return m_wait WAIT_infi or WAIT_20ms
    inline TickType_t get_wait(void){
      return this->m_wait;
    }

    // // generate a 455kHz carrier-pulse and start the timer for the pulsecode pause
    // // @param pulscode beo4 pulsecode [1..5]
    // inline void tx_pc(uint8_t pulsecode) { 
    //   ledcWrite(m_tx_pin,8); // burst start
    //   esp_timer_start_once(m_OneShotTimer_h,t_low_pulse-t_corr);
    //   xEventGroupWaitBits(g_eg_handle, evPulse, pdTRUE, pdTRUE, portMAX_DELAY);
    //   ledcWrite(m_tx_pin,0); // burst end
    //   if(0 < pulsecode) {    // no timer for the final carrier-pulse
    //     esp_timer_start_once(m_OneShotTimer_h, (pulsecode*t_pc)-t_low_pulse);
    //     xEventGroupWaitBits(g_eg_handle, evPulse, pdTRUE, pdTRUE, portMAX_DELAY); 
    //   } 
    // }

    // generate a 455kHz carrier-pulse and start the timer for the pulsecode pause
    // @param pulscode beo4 pulsecode [1..5]
    inline void tx_pc(uint8_t pulsecode) { 
      uint32_t pulse_width=(uint32_t)(((uint64_t)pulsecode*t_pc)-t_low_pulse);
      ledcWrite(m_tx_pin,8); // turn carrier on
      delayMicroseconds(200);
      ledcWrite(m_tx_pin,0); // turn carrier off
      if(0 < pulsecode) {    // no timer for the final carrier-pulse
        delayMicroseconds(pulse_width);
      }
    }


    // Beo4 Receive Task, reads timestamps from ISR queue and call state-machine 
    // decoding start, data and stop codes. 
    // @param param handle to the class
    friend void beo4_rx_task(void *param); 
    TaskHandle_t m_beo4_rx_task; ///< task handle
    
    // Beo4 Transmit Task, reads beoCode from m_queue_beo_tx. The 455kHz
    // carrier is generted by a PWM channel that is attached to m_ir_tx_pin 
    // (ledcSetup(),ledcAttachPin(),ledcWrite()). The Pulses are generated by 
    // setting the PWM-channel's duty-cycle, i.e. ledcWrite(channel,0)=pause
    // ledcWrite(channel,7)=carrier-burst. Start/stop pulses are added to 
    // complete the frame. 
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
