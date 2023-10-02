#ifndef IRBEO4_H_
#define IRBEO4_H_

#include "Arduino.h"

// external call back functions
extern __attribute__((weak)) void beo_code_cb(uint32_t beo_code);

// interupt-serice-routine
// trigger on rising edge of IR signal, get timestamp and send to q_irs_queue 
void IRAM_ATTR ir_pulse_isr(void);

// internal callback function for one-shot-timer
void IRAM_ATTR one_shot_timer_cb(void* arg);

// timeout values, 20ms or infinite
#define WAIT_20ms     20/portTICK_PERIOD_MS 
#define WAIT_Infinite portMAX_DELAY

// table of Beo4 sources
#define BEO_SRC_VIDEO         0x00
#define BEO_SRC_AUDIO         0x01
#define BEO_SRC_VTAPE         0x05
#define BEO_SRC_ALL           0x0F
#define BEO_SRC_SPDEMO        0x1D
#define BEO_SRC_LIGHT         0x1B
// table of Beo4 commands
#define BEO_CMD_NUMBER_0      0x00
#define BEO_CMD_NUMBER_1      0x01
#define BEO_CMD_NUMBER_2      0x02
#define BEO_CMD_NUMBER_3      0x03
#define BEO_CMD_NUMBER_4      0x04
#define BEO_CMD_NUMBER_5      0x05
#define BEO_CMD_NUMBER_6      0x06
#define BEO_CMD_NUMBER_7      0x07
#define BEO_CMD_NUMBER_8      0x08
#define BEO_CMD_NUMBER_9      0x09
#define BEO_CMD_CLEAR         0x0A
#define BEO_CMD_STORE         0x0B
#define BEO_CMD_STANDBY       0x0C
#define BEO_CMD_MUTE          0x0D
#define BEO_CMD_INDEX         0x0E
#define BEO_CMD_UP            0x1E
#define BEO_CMD_DOWN          0x1F
#define BEO_CMD_TUNE          0x20
#define BEO_CMD_CLOCK         0x28
#define BEO_CMD_FORMAT        0x2A
#define BEO_CMD_LEFT          0x32
#define BEO_CMD_RETURN        0x33
#define BEO_CMD_RIGHT         0x34
#define BEO_CMD_GO            0x35
#define BEO_CMD_STOP          0x36
#define BEO_CMD_RECORD        0x37
#define BEO_CMD_SELECT        0x3F
#define BEO_CMD_SPEAKER       0x44
#define BEO_CMD_PICTURE       0x45 
#define BEO_CMD_TURN          0x46 // list->mono
#define BEO_CMD_LOUDNESS      0x48
#define BEO_CMD_BASS          0x4D
#define BEO_CMD_TREBLE        0x4E
#define BEO_CMD_BALANCE       0x4F
#define BEO_CMD_LIST          0x58
#define BEO_CMD_MENU          0x5C
#define BEO_CMD_VOL_UP        0x60
#define BEO_CMD_VOL_DOWN      0x64
#define BEO_CMD_LEFT_REPEAT   0x70
#define BEO_CMD_RIGHT_REPEAT  0x71
#define BEO_CMD_UP_REPEAT     0x72
#define BEO_CMD_DOWN_REPEAT   0x73
#define BEO_CMD_GO_REPEAT     0x75
#define BEO_CMD_GREEN_REPEAT  0x76
#define BEO_CMD_YELLOW_REPEAT 0x77
#define BEO_CMD_BLUE_REPEAT   0x78
#define BEO_CMD_RED_REPEAT    0x79
#define BEO_CMD_EXIT          0x7F
#define BEO_CMD_TV            0x80
#define BEO_CMD_RADIO         0x81
#define BEO_CMD_VIDEO_AUX     0x82
#define BEO_CMD_AUDIO_AUX     0x83
#define BEO_CMD_VTAPE         0x85
#define BEO_CMD_DVD           0x86
#define BEO_CMD_CAMCORD       0x87
#define BEO_CMD_TEXT          0x88
#define BEO_CMD_SP_DEMO       0x89
#define BEO_CMD_SAT           0x8A
#define BEO_CMD_PC            0x8B
#define BEO_CMD_DOOR_CAM      0x8D
#define BEO_CMD_ATAPE         0x91
#define BEO_CMD_CD            0x92
#define BEO_CMD_PHONO         0x93
#define BEO_CMD_ATAPE_2       0x94
#define BEO_CMD_CD2           0x97
#define BEO_CMD_LIGHT         0x9B
#define BEO_CMD_AV            0xBF
#define BEO_CMD_YELLOW        0xD4
#define BEO_CMD_GREEN         0xD5
#define BEO_CMD_BLUE          0xD8
#define BEO_CMD_RED           0xD9
#define BEO_CMD_STAND         0xf7 

// lookup the Beo4 source from a given beoCode
// @param beo_code=Beo4 code
// return Beo-Source or "invalid_src" for unknown beoCode
extern const char* beo_src_tbl(uint32_t beo_code);

// lookup the Beo4 command from a given beoCode
// @param beo_code=Beo4 code 
// return Beo-Source or "invalid_cmd" for unknwon beoCode
extern const char* beo_cmd_tbl(uint32_t beo_code);

// states of beo4_rx_task() state-machine
#define FSM_IDLE   0
#define FSM_S0     1
#define FSM_S1     2
#define FSM_START  3
#define FSM_DATA   4
#define FSM_STOP   5

// Beo4 PulseCodes depending of measuered PulseWidth
#define BEO_ZERO   1  // pulsewidth =  3125
#define BEO_SAME   2  // pulsewidth =  6250
#define BEO_ONE    3  // pulsewidth =  9375
#define BEO_STOP   4  // pulsewidth = 12500
#define BEO_START  5  // pulsewidth = 15625

class IrBeo4 {
  private:
    int8_t m_ir_rx_pin = -1;             // IR receive pin
    int8_t m_ir_tx_pin = -1;             // IR transmit pin
    int8_t m_ir_tx_pwm = -1;             // IR transmit pwm channel
    QueueHandle_t m_beo4_rx_queue;       // output queue for received beoCodes
    QueueHandle_t m_beo4_tx_queue;       // input queue for beoCodes to be send
    esp_timer_handle_t m_OneShotTimer_h; // ont-shot-timer handle 
    
    // Beo4 Receive Task (state-machine), reads timestamps from ISR and 
    // run state-machine while decoding the start, data and stop codes. 
    // @param param = handle to the class
    friend void beo4_rx_task(void *param); 
    TaskHandle_t m_beo4_rx_task; // task handle
    
    // Beo4 Transmit Task, reads beoCode from m_queue_beo_tx. The 455kHz
    // carrier is generted by a PWM channel that is attached to m_ir_tx_pin 
    // (ledcSetup(),ledcAttachPin(),ledcWrite()). The Pulses are generated by 
    // setting the PWM-channel's duty-cycle, i.e. ledcWrite(channel,0)=pause
    // ledcWrite(channel,7)=carrier-burst. Start/stop pulses are added to 
    // complete the frame. 
    // @param param = handle to the class
    friend void beo4_tx_task(void *param); 
    TaskHandle_t m_beo4_tx_task; // task handle


  protected:
  public:
    // constructor
    // @param rx_pin  = IR receiver input pin
    // @param tx_pin  = IR transmit output pin (default=-1, not used)
    // @param tx_pwm  = IR transmit pwm-channel (default=-1, not used)
    IrBeo4(int8_t rx_pin,int8_t tx_pin=-1, int8_t tx_pwm=-1);
    
    // destructor
    ~IrBeo4();

    // attach the interrupt service routine to the rx_pin, and trigger by
    // rising edge. Start tasks for receive and transmit
    // @param beo4_rx_queue = queue for received beo4-codes
    // @param beo4_tx_queue = queue for beo4-codes to be transmitted
    // @return 0=OK, -1=Failed
    int Begin(QueueHandle_t beo4_rx_queue, QueueHandle_t beo4_tx_queue=NULL);

};
#endif /* IRBEO4_H_ */
