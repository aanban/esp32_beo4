# Example for Home Assistant MQTT auto discovery
The example shows how to integrate the esp32beo4 device into the MQTT integration within Home Assistant. That is done by sending the auto discovery MQTT messages, in order to introduce the several sensors, e.g. the BeoCode sensor that is the received beo4 code from the remote control. The example provides the `BeoCode`, the `BeoCommand` and the `BeoSource` text sensors that appears inside the **Sensor** section and the `Heap`, the `NumBeoCodes` and the `WiFi` sensors that appear inside the **Diagnostics** section

![HA_esp32beo4_device](doc/HA_esp32beo4_device.png)

The auto discovery MQTT messages are JSON formatted, as seen in following example for the `BeoCode` Sensor. 


```JSON
{
  "name": "BeoCode",
  "unique_id": "2CB8B81FB608_BeoCode",
  "icon": "mdi:numeric",
  "value_template": "{{ value_json.BeoCode}}",
  "state_topic": "esp32beo4/sens",
  "device": {
    "name": "esp32beo4",
    "sw_version": "1.0.0",
    "model": "ESP32 Beo4 gateway",
    "identifiers": [
      "2CB8B81FB608"
    ]
  }
}
```
The `unique_id` is build from the name and parts of the MAC address of the EPS32. The `name` is found in several parameters like the `value_template` Important is that the Topic of the discovery message is build from the home assistant path, the `state_topic` and the `name` (concatenated with an `_`) 


```
path:        homeassistant/sensor/
state_topic: esp32beo4/sens
name:        BeoCode
suffix:      /config
Topic:       homeassistant/sensor/esp32beo4/sens_BeoCode/config
```

Once the device is introduced the `state_topic` is used as topic for sending the MQTT sensor data message, as seen in following example. 

```
topic: esp32beo4/sens
```
```JSON
{
  "BeoCode":    "1b9b",
  "BeoSource":  "light",
  "BeoCommand": "light-on"
}
```

## Building a Device 
The `device` entry within the discovery JSON structure is the same for all entities that are to be introduced. The the MQTT integration groups the single entities to one single device. `model` and `sw_version`  are just informations to shown in the device-information box top left. The `name` sets the device name and the identifiers is the unique ID from above. 

```JSON

  "device": {
    "name": "esp32beo4",
    "sw_version": "1.0.0",
    "model": "ESP32 Beo4 gateway",
    "identifiers": [
      "2CB8B81FB608"
    ]
  }

```

# ArduinoJson
The MQTT Json messages were generated with the ArduinoJson library. An online code generator could be found here https://arduinojson.org/v7/assistant/#/step1

Cope a needed Json message into the clipboard, then follow the steps and the corresponding code snipped is generated.

# Example of discovery message

```c++
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
```