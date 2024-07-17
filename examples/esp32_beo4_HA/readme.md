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
