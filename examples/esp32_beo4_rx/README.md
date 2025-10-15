A simple receiver main.cpp example looks like so. The instance gets the receiver Pin, and the lc_mode determine wether the waiting code is skipped (lc_mode=0) or kept (lc_mode=1) on repeated code bursts, i.e. when a repeatable button is pressed longer:

```cpp
#include <Arduino.h>
#include "IrBeo4.h"

constexpr int8_t con_IR_RX_PIN = 34;  // IR receive pin

// instance of beo4 decoder, here configured as receiver, 
// set lc_mode=1 keeping the waiting code
IrBeo4 beo4(con_IR_RX_PIN,-1,1);
xQueueHandle beo4_rx_queue;     // queue 
TaskHandle_t beo4_task_h;       // task handle
static unsigned long t1=0;      // timing

void beo4_task(void *parameter) {
  static uint32_t beo4Code=0;
  while(1) {
    if(pdTRUE==xQueueReceive(beo4_rx_queue,(void*)(&beo4Code),portMAX_DELAY)) {
      unsigned long t2=millis(); 
      printf("beo4_task:%6lums %05x %s-%s\r\n",
            t2-t1,beo4Code,
            beo_src_tbl(beo4Code),
            beo_cmd_tbl(beo4Code));
      t1=t2; 
    }
  }
}

void setup() {
  Serial.begin(115200);
  t1=millis();
  beo4_rx_queue = xQueueCreate(50, sizeof(uint32_t));
  xTaskCreatePinnedToCore(beo4_task,"beo4_task",10000,NULL,0,&beo4_task_h,0);
  beo4.Begin(beo4_rx_queue);
  printf("beo4 %s started, RX=%d\r\n",beo4.GetVersion(),con_IR_RX_PIN);
}

void loop() {
}

```
