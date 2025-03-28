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

// extracts BeoSource from a given beo_code as string
// @param beo_code=Beo4 code
// return BeoSource or "invalid_src" for unknown beoCodes
inline const char* beo_src_tbl(uint8_t source) {
  switch(source) {
    case BEO_SRC_VIDEO:  return("video");
    case BEO_SRC_AUDIO:  return("audio");
    case BEO_SRC_VTAPE:  return("vtape");
    case BEO_SRC_ALL:    return("all");
    case BEO_SRC_SPDEMO: return("spdemo");
    case BEO_SRC_LIGHT:  return("light");
  }
  return("invalid_src");
}

// extracts BeoCommand from a given beo_code as string
// @param beo_code=Beo4 code 
// return BeoCommand or "invalid_cmd" for unknwon beoCodes
inline const char* beo_cmd_tbl(uint32_t command) {
  switch(command) {
    case BEO_CMD_NUM_0:        return("num-0");
    case BEO_CMD_NUM_1:        return("num-1");
    case BEO_CMD_NUM_2:        return("num-2");
    case BEO_CMD_NUM_3:        return("num-3");
    case BEO_CMD_NUM_4:        return("num-4");
    case BEO_CMD_NUM_5:        return("num-5");
    case BEO_CMD_NUM_6:        return("num-6");
    case BEO_CMD_NUM_7:        return("num-7");
    case BEO_CMD_NUM_8:        return("num-8");
    case BEO_CMD_NUM_9:        return("num-9");
    case BEO_CMD_CLEAR:        return("clear");
    case BEO_CMD_STORE:        return("store");
    case BEO_CMD_STANDBY:      return("standby");
    case BEO_CMD_MUTE:         return("mute");
    case BEO_CMD_INDEX:        return("index");
    case BEO_CMD_UP:           return("up");
    case BEO_CMD_DOWN:         return("down");
    case BEO_CMD_TUNE:         return("tune");
    case BEO_CMD_CLOCK:        return("clock");
    case BEO_CMD_FORMAT:       return("format");
    case BEO_CMD_LEFT:         return("left");
    case BEO_CMD_RETURN:       return("return");
    case BEO_CMD_RIGHT:        return("right");
    case BEO_CMD_GO:           return("go");
    case BEO_CMD_STOP:         return("stop");
    case BEO_CMD_RECORD:       return("return");
    case BEO_CMD_SELECT:       return("select");
    case BEO_CMD_SPEAKER:      return("speaker");
    case BEO_CMD_PICTURE:      return("picture");
    case BEO_CMD_TURN:         return("turn");
    case BEO_CMD_LOUDNESS:     return("loudness");
    case BEO_CMD_BASS:         return("bass");
    case BEO_CMD_TREBLE:       return("treble");
    case BEO_CMD_BALANCE:      return("balance");
    case BEO_CMD_LIST:         return("list");
    case BEO_CMD_MENU:         return("menu");
    case BEO_CMD_VOL_UP:       return("vol++");
    case BEO_CMD_VOL_DOWN:     return("vol--");
    case BEO_CMD_LEFT_REPEAT:  return("left++");
    case BEO_CMD_RIGHT_REPEAT: return("right++");
    case BEO_CMD_UP_REPEAT:    return("up++");
    case BEO_CMD_DOWN_REPEAT:  return("down++");
    case BEO_CMD_GO_REPEAT:    return("go++");
    case BEO_CMD_GREEN_REPEAT: return("green++");
    case BEO_CMD_YELLOW_REPEAT:return("yellow++");
    case BEO_CMD_BLUE_REPEAT:  return("blue++");
    case BEO_CMD_RED_REPEAT:   return("red++");
    case BEO_CMD_EXIT:         return("exit");
    case BEO_CMD_TV:           return("tv-on");
    case BEO_CMD_RADIO:        return("radio-on");
    case BEO_CMD_VIDEO_AUX:    return("video-aux");
    case BEO_CMD_AUDIO_AUX:    return("audio-aux");
    case BEO_CMD_VTAPE:        return("vtape-on");
    case BEO_CMD_DVD:          return("dvd-on");
    case BEO_CMD_CAMCORD:      return("camcord-on");
    case BEO_CMD_TEXT:         return("text");
    case BEO_CMD_SP_DEMO:      return("sp-demo");
    case BEO_CMD_SAT:          return("sat-on");
    case BEO_CMD_PC:           return("pc");
    case BEO_CMD_DOOR_CAM:     return("door-cam");
    case BEO_CMD_ATAPE:        return("atape-on");
    case BEO_CMD_CD:           return("cd-on");
    case BEO_CMD_PHONO:        return("phono-on");
    case BEO_CMD_ATAPE2:       return("atape2-on");
    case BEO_CMD_CD2:          return("cd2-on");
    case BEO_CMD_LIGHT:        return("light-on");
    case BEO_CMD_AV:           return("av");
    case BEO_CMD_YELLOW:       return("yellow");
    case BEO_CMD_GREEN:        return("green");
    case BEO_CMD_BLUE:         return("blue");
    case BEO_CMD_RED:          return("red");
    case BEO_CMD_STAND:        return("stand");
  }
  return("invalid_cmd");
}
