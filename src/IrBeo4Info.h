#ifndef IRBEO4INFO_H_
#define IRBEO4INFO_H_

#include "Arduino.h"

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

// lookup the Beo4 source from a given beoCode
// @param beo_code=Beo4 code
// return Beo-Source or "invalid_src" for unknown beoCode
const char* beo_src_tbl(uint32_t beo_code) {
  uint8_t source=(uint8_t) ((beo_code>>8) & 255u);
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

// lookup the Beo4 command from a given beoCode
// @param beo_code=Beo4 code 
// return Beo-Source or "invalid_cmd" for unknwon beoCode
const char* beo_cmd_tbl(uint32_t beo_code) {
  uint8_t command=(uint8_t) (beo_code & 255u);
  switch(command) {
    case BEO_CMD_NUM_0:        return(PSTR("0"));
    case BEO_CMD_NUM_1:        return(PSTR("1"));
    case BEO_CMD_NUM_2:        return(PSTR("2"));
    case BEO_CMD_NUM_3:        return(PSTR("3"));
    case BEO_CMD_NUM_4:        return(PSTR("4"));
    case BEO_CMD_NUM_5:        return(PSTR("5"));
    case BEO_CMD_NUM_6:        return(PSTR("6"));
    case BEO_CMD_NUM_7:        return(PSTR("7"));
    case BEO_CMD_NUM_8:        return(PSTR("8"));
    case BEO_CMD_NUM_9:        return(PSTR("9"));
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

#endif /* IRBEO4INFO_H_ */
