#include <Arduino.h>
#include "IrBeo4.h"

#define IR_RX_PIN 34            // IR receive pin
IrBeo4 beo4(IR_RX_PIN);         // instance

// solution with task and queue
// 
xQueueHandle beo4_rx_queue;     // queue 
TaskHandle_t beo4_task_h;       // task handle

void beo4_task(void *parameter) {
  static uint32_t beo4Code=0;
  while(1) {
    if(pdTRUE==xQueueReceive(beo4_rx_queue,(void*)(&beo4Code),portMAX_DELAY)) {
      Serial.printf("beo4_task:   %04x %s %s\n",beo4Code,beo_src_tbl(beo4Code),beo_cmd_tbl(beo4Code));
    }
  }
}


// alternative solution with callback function
//
void beo_code_cb(uint32_t beo_code) {
  Serial.printf("beo_code_cb: %04x %s %s \n", beo_code,beo_src_tbl(beo_code),beo_cmd_tbl(beo_code));
}

void setup() {
  Serial.begin(115200);
  beo4_rx_queue = xQueueCreate(50, sizeof(uint32_t));
  xTaskCreatePinnedToCore(beo4_task,"beo4_task",10000,NULL,0,&beo4_task_h,0);
  beo4.Begin(beo4_rx_queue);
  printf("beo4 started, RX=%d\n",IR_RX_PIN);
}

void loop() {
}

