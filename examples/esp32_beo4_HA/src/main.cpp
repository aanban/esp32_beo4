#include <Arduino.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include <IrBeo4.h>
#include <IrBeo4Info.h>

uint32_t ts_m=0;                 // timestamp mqtt loop
uint32_t ts_l=0;                 // timestamp status loop

// Beo4 stuff
#define IR_RX_PIN 34             // IR receive pin
#define IR_TX_PIN 33             // IR transmit pin 
IrBeo4 beo4(IR_RX_PIN,IR_TX_PIN);// receive, transmit
xQueueHandle beo4_rx_queue;      // queue for beo4codes from receiver
xQueueHandle beo4_tx_queue;      // queue for beo4codes from transmitter
TaskHandle_t beo4_task_h;

// WiFi stuff
#include "secrets.h"             // wifi user/pass mqtt user/pass

// MQTT stuff
#include "PubSubClient.h"         // from https://github.com/knolleary/pubsubclient.git
constexpr const char* gVersion    = "1.2.0";                // application version HA
constexpr const char* gModel      = "ESP32 Beo4 gateway";   // device information HA
constexpr const char* gDevice     = "esp32beo4";            // device name HA
constexpr const char* topic_diag  = "esp32beo4/diag";       // diagnostics section
constexpr const char* topic_sens  = "esp32beo4/sens";       // sensor section
constexpr const char* topic_trans = "esp32beo4/trans";      // command to be transmitted
constexpr const char* topic_HA    = "homeassistant/status"; // status from Home Assistant


WiFiClient wClient;              // general wifi-client 
PubSubClient mqttClient(wClient);// mqtt client derived from wifi-client
static char json_diag[1024]={0}; // diag sensors
static char json_sens[1024]={0}; // beo4 sensors
static char topic[1024]={0};     // topics to be filled
static String gUniqueId;         // MAC-Address used as unique ID
static int  gNumBeoCodes=0;      // counts received BeoCodes

// generates Home Assistant auto discovery MQTT message for BeoCode text sensor
void discover_BeoCode(void) {
  if(mqttClient.connected()) {
    String data;                 // payload for mqtt message
    size_t n_data;               // payload length
    snprintf(&topic[0],1024,"homeassistant/sensor/%s_BeoCode/config",topic_sens);
    Serial.printf_P(PSTR("\n %s \n"),topic);
    JsonDocument doc;
    doc["name"]                = "BeoCode";
    doc["unique_id"]           = gUniqueId + "_BeoCode";
    doc["icon"]                = "mdi:numeric";
    doc["value_template"]      = "{{ value_json.BeoCode}}";
    doc["state_topic"]         = topic_sens;

    JsonObject device = doc["device"].to<JsonObject>();
    device["name"]             = gDevice;
    device["sw_version"]       = gVersion;
    device["model"]            = gModel;
    device["identifiers"][0]   = gUniqueId;
    Serial.println(" ");
    serializeJsonPretty(doc, Serial);
    doc.shrinkToFit();  // optional
    serializeJson(doc, data);
    mqttClient.publish(topic, data.c_str());
    Serial.println(" ");
  }
}
// generates Home Assistant auto discovery MQTT message for BeoSource text sensor
void discover_BeoSource(void){
  if(mqttClient.connected()) {
    String data;                 // payload for mqtt message
    size_t n_data;               // payload length
    snprintf(&topic[0],1024,"homeassistant/sensor/%s_BeoSource/config",topic_sens);
    Serial.printf_P(PSTR("\n %s \n"),topic);
    JsonDocument doc;
    doc["name"]                = "BeoSource";
    doc["unique_id"]           = gUniqueId + "_BeoSource";
    doc["icon"]                = "mdi:television-classic";
    doc["value_template"]      = "{{ value_json.BeoSource}}";
    doc["state_topic"]         = topic_sens;
    
    JsonObject device = doc["device"].to<JsonObject>();
    device["name"]             = gDevice;
    device["sw_version"]       = gVersion;
    device["model"]            = gModel;
    device["identifiers"][0]   = gUniqueId;
    Serial.println(" ");
    serializeJsonPretty(doc, Serial);
    serializeJson(doc, data);
    mqttClient.publish(topic, data.c_str());
    Serial.println(" ");
  }
}

// generates Home Assistant auto discovery MQTT message for BeoCommand text sensor
void discover_BeoCommand(void){
  if(mqttClient.connected()) {
    String data;                 // payload for mqtt message
    size_t n_data;               // payload length
    snprintf(&topic[0],1024,"homeassistant/sensor/%s_BeoCommand/config",topic_sens);
    Serial.printf_P(PSTR("\n %s \n"),topic);
    JsonDocument doc;
    doc["name"]                = "BeoCommand";
    doc["unique_id"]           = gUniqueId + "_BeoCommand";
    doc["icon"]                = "mdi:radio";
    doc["value_template"]      = "{{ value_json.BeoCommand}}";
    doc["state_topic"]         = topic_sens;
    
    JsonObject device = doc["device"].to<JsonObject>();
    device["name"]             = gDevice;
    device["sw_version"]       = gVersion;
    device["model"]            = gModel;
    device["identifiers"][0]   = gUniqueId;
    Serial.println(" ");
    serializeJsonPretty(doc, Serial);
    serializeJson(doc, data);
    mqttClient.publish(topic, data.c_str());
    Serial.println(" ");
  }
}

// generates Home Assistant auto discovery MQTT message for diagnostic NumBeoCodes sensor
void discover_NumBeoCodes(void){
  if(mqttClient.connected()) {
    String data;                 // payload for mqtt message
    size_t n_data;               // payload length
    snprintf(&topic[0],1024,"homeassistant/sensor/%s_NumBeoCodes/config",topic_diag);
    Serial.printf_P(PSTR("\n %s \n"),topic);
    JsonDocument doc;
    doc["name"]                = "NumBeoCodes";
    doc["unique_id"]           = gUniqueId + "_NumBeoCodes";
    doc["icon"]                = "mdi:counter";
    doc["value_template"]      = "{{ value_json.NumBeoCodes}}";
    doc["state_topic"]         = topic_diag;
    doc["entity_category"]     = "diagnostic";

    JsonObject device = doc["device"].to<JsonObject>();
    device["name"]             = gDevice;
    device["sw_version"]       = gVersion;
    device["model"]            = gModel;
    device["identifiers"][0]   = gUniqueId;
    Serial.println(" ");
    serializeJsonPretty(doc, Serial);
    serializeJson(doc, data);
    mqttClient.publish(topic, data.c_str());
    Serial.println(" ");
  }
}

// generates Home Assistant auto discovery MQTT message for WiFi RSSI sensor in diagnostic area
void discover_WiFi(void){
  if(mqttClient.connected()) {
    String data;                 // payload for mqtt message
    size_t n_data;               // payload length
    snprintf(&topic[0],1024,"homeassistant/sensor/%s_WiFi/config",topic_diag);
    Serial.printf_P(PSTR("\n %s \n"),topic);
    JsonDocument doc;
    doc["name"]                = "WiFi";
    doc["unique_id"]           = gUniqueId + "_WiFi";
    doc["value_template"]      = "{{ value_json.WiFi}}";
    doc["state_topic"]         = topic_diag;
    doc["state_class"]         = "measurement";
    doc["device_class"]        = "signal_strength";
    doc["entity_category"]     = "diagnostic";
    doc["unit_of_measurement"] = "dBm";
    
    JsonObject device = doc["device"].to<JsonObject>();
    device["name"]             = gDevice;
    device["sw_version"]       = gVersion;
    device["model"]            = gModel;
    device["identifiers"][0]   = gUniqueId;
    Serial.println(" ");
    serializeJsonPretty(doc, Serial);
    serializeJson(doc, data);
    mqttClient.publish(topic, data.c_str());
    Serial.println(" ");
  }
}

// generates Home Assistant auto discovery MQTT message for free Heap sensor in diagnostic area
void discover_Heap(void){
  if(mqttClient.connected()) {
    String data;                 // payload for mqtt message
    size_t n_data;               // payload length
    snprintf(&topic[0],1024,"homeassistant/sensor/%s_Heap/config",topic_diag);
    Serial.printf_P(PSTR("\n %s \n"),topic);
    JsonDocument doc;
    doc["name"]                = "Heap";
    doc["unique_id"]           = gUniqueId + "_Heap";
    doc["value_template"]      = "{{ value_json.Heap}}";
    doc["state_topic"]         = topic_diag;
    doc["state_class"]         = "measurement";
    doc["device_class"]        = "data_size";
    doc["entity_category"]     = "diagnostic";
    doc["unit_of_measurement"] = "kb";
    
    JsonObject device = doc["device"].to<JsonObject>();
    device["name"]             = gDevice;
    device["sw_version"]       = gVersion;
    device["model"]            = gModel;
    device["identifiers"][0]   = gUniqueId;
    Serial.println(" ");
    serializeJsonPretty(doc, Serial);
    serializeJson(doc, data);
    mqttClient.publish(topic, data.c_str());
    Serial.println(" ");
  }
}

// sends discovery MQTT messages to MQTT integration within Home Assistant 
void HomeAssistantDiscovery(void) {
  Serial.printf_P(PSTR("===> Home Assistant MQTT auto discovery ...\n"));
  discover_BeoCode();
  discover_BeoCommand();
  discover_BeoSource();
  discover_Heap();
  discover_WiFi();
  discover_NumBeoCodes();
}

// generates and publish json mqtt message from received beo4 code
// @param BeoCode received beo4 Code
void publish_sens(uint32_t BeoCode) {
  if(mqttClient.connected()) {
    snprintf(&json_sens[0],
      1023,
      "{\n"
      "\"BeoCode\": \"%04x\",\n"
      "\"BeoSource\": \"%s\",\n"
      "\"BeoCommand\": \"%s\"\n"
      "}",
      BeoCode,
      beo_src_tbl(BeoCode),
      beo_cmd_tbl(BeoCode)
    );
    if(false==mqttClient.publish(topic_sens, json_sens, false)){
      Serial.printf_P(PSTR("publish_sens() failed\n"));
    }
  }
}

// generate diag messages, Wifi-RSSI value and free Heap memory
void publish_diag(void) {
  if(mqttClient.connected()) {
    snprintf(&json_diag[0],
      1023,
      "{\n"
      "\"WiFi\": \"%d\",\n"
      "\"Heap\": \"%.3f\",\n"
      "\"NumBeoCodes\": \"%d\"\n"
      "}",
      WiFi.RSSI(),
      ((float) esp_get_minimum_free_heap_size() / (float) 1000.0),
      gNumBeoCodes
    );
    if(false==mqttClient.publish(topic_diag, json_diag, true)) {
      Serial.printf_P(PSTR("publish_diag failed\n"));
    }
  }
}

// mqtt callback: handles received mqtt messages, the payload can't 
// be processed directly, it need to be copied into internal MqttMsg 
// String. If status==online the MQTT integration was restarted, 
// so the HomeAssistantDiscovery() has to be send again, to re-new 
// the device within the MQTT Integration
// @param topic, mqtt identifier
// @param payload, mqtt data
// @param length, length mqtt data
void mqtt_cb(char* topic, byte* payload, unsigned int length) {
  String MqttMsg;
  for(unsigned int ii=0;ii<length;ii++) {
    MqttMsg += (char)payload[ii]; 
  }
  String sTopic=String(topic);
  Serial.printf_P(PSTR("%s = %s len: %d \n"), topic, MqttMsg.c_str(), length);
  if(sTopic == String(topic_trans)){
    int32_t beoCode=0;
    char *str_mqtt=(char *) MqttMsg.c_str();
    sscanf(str_mqtt,"%x",&beoCode);
    Serial.printf_P(PSTR("MQTT --> %04x --> (%s,%s) \n"),beoCode,beo_src_tbl(beoCode), beo_cmd_tbl(beoCode),beoCode);
    xQueueSend(beo4_tx_queue,&beoCode,0);
  }
  else if(sTopic == String("homeassistant/status")) {
    if(MqttMsg == "online") {
      HomeAssistantDiscovery();
    }
  }
}

// connects to mqtt broker, subscribe to topic_HA and send discovery MQTT messages for the sensors 
void connect_mqtt(void) {
  String clientID = "esp32beo4-" + gUniqueId;
  Serial.printf_P(PSTR("===> start MQTT %s ..."),clientID.c_str());
  if (mqttClient.connect(clientID.c_str(), mqtt_user, mqtt_pass)) {
    mqttClient.setBufferSize(1024);  // default size is 256, increase to 1024
    Serial.printf_P(PSTR(" OK\n"));
    mqttClient.subscribe(topic_HA);
    mqttClient.subscribe(topic_trans);
    HomeAssistantDiscovery(); 
  } else {
    Serial.printf_P(PSTR("failed, rc=%d \n try again in 5 seconds \n"),mqttClient.state());
  }  
}

// keep mqtt running, if connected just call the mqttClient.loop() else, try to reconnect to broker
void loop_mqtt(void) {
  if(mqttClient.connected()) {
    mqttClient.loop(); 
  } else {
    if((millis()-ts_m) >= 2000 ) {
      ts_m=millis();
      connect_mqtt();
    }
  }
}

// publish diag infos every 5 sec
void loop_status(void){
  if(5000 < (millis() - ts_l)) {
    publish_diag();
    ts_l=millis();
  }
}

// task for receiving beo4codes from queue
// @param parameter, not used 
void beo4_task(void *parameter) {
  static uint32_t BeoCode=0;
  while(1) {
    if(pdTRUE==xQueueReceive(beo4_rx_queue,(void*)(&BeoCode),portMAX_DELAY)) {
      Serial.printf_P(PSTR("Beo4 %04x %s %s\n"),BeoCode,beo_src_tbl(BeoCode),beo_cmd_tbl(BeoCode));
      ++gNumBeoCodes; // count number of received BeoCodes
      publish_sens(BeoCode);
      publish_diag();
    }
  }
}

// prepares queue, starts beo4_task and the internal beo4 receiver tasks 
void setup_beo4(void) {
  static int beo4_ok=-1; 
  Serial.printf(PSTR("===> start beo4... ")); 
  beo4_rx_queue = xQueueCreate(50, sizeof(uint32_t));
  beo4_tx_queue = xQueueCreate(50, sizeof(uint32_t));
  xTaskCreatePinnedToCore(beo4_task,"beo4_task",10000,NULL,0,&beo4_task_h,0);
  beo4_ok=beo4.Begin(beo4_rx_queue,beo4_tx_queue);  // start beo4 receiver only
  Serial.printf(PSTR("%s\n"),beo4_ok==0? "OK":"failed");
}

// starts and connects to mqtt broker
void setup_mqtt(void) {
  mqttClient.setServer(mqtt_server, 1883);   // connect to mqtt broker
  mqttClient.setCallback(mqtt_cb);           // callback function for incoming MQTT messages
  connect_mqtt();                            // try to connect to MQTT broker
  publish_diag();                            // initial publish of diag infos
}

// extracts the ESP32's MAC address using it as unique identifier (gUniqueID)
void extractUniqueId(void) {
  byte mac[6]={0};
  char uID[13]={0};
  WiFi.macAddress(mac);
  sprintf(&uID[0],"%02X%02X%02X%02X%02X%02X",mac[5],mac[4],mac[3],mac[2],mac[1],mac[0]);
  gUniqueId=(String)uID;
  Serial.printf_P(PSTR("===> get unique ID from MAC address = %s\n"),gUniqueId.c_str());
}

// connects to wifi
void setup_wifi(void) {
  uint8_t connect_cnt=0;
  Serial.printf_P(PSTR("===> start WiFi "));
  WiFi.begin(wifi_ssid,wifi_pass);
  while(WL_CONNECTED!= WiFi.status() && 10 > connect_cnt) {
    delay(1000);
    Serial.print(".");
    ++connect_cnt;
  }
  Serial.printf_P(PSTR("%s\n"),(WL_CONNECTED==WiFi.status())? "OK" :"Failed");
  extractUniqueId();
}

void setup() {
  ts_l = ts_m=millis(); // initial timestamps for status and mqtt 
  Serial.begin(115200); // open serial for trace messages
  Serial.printf_P(PSTR("\n===> setup() \n"));
  setup_beo4();
  setup_wifi();
  setup_mqtt();
}

void loop() {
  loop_mqtt();
  loop_status();
}
