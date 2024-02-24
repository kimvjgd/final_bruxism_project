
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include "driver/gpio.h"
#include "soc/wdev_reg.h"
#include "xtensa/core-macros.h"

#include "constants.h"
#include "myBLE.h"
#include "Adafruit_MCP9808.h"


////////////////// Changeable Values //////////////////
uint8_t  CUR_CNT = 0;

// true - EMG, false - E.S.
bool CURRENT_MODE = true;
////////////////////////////////////////





void EMG_task(void *pvParameter)
{
  (void)pvParameter;
  Serial.println("EMG_task Start");

  ////////////////////////////////////// Variables /////////////////////////////////////
  uint8_t CURRENT_EMG_VALUE = 0;
  CURRENT_MODE = true;

  //////////////////////////////////////// Loop ////////////////////////////////////////
  while (1)
  {
    if(CURRENT_EMG_VALUE > EMG_THR_VALUE) {

    }
    vTaskDelay(100 / portTICK_RATE_MS);
  }
}

void ES_task(void *pvParameter) 
{
  (void)pvParameter;

  Serial.println("ES_task Start");

  ////////////////////////////////////// Variables /////////////////////////////////////
  CUR_CNT = 0;

  //////////////////////////////////////// Loop ////////////////////////////////////////
  while (1)
  {
    Serial.println("");
    vTaskDelay(10 / portTICK_RATE_MS);
  }
}


void freeRTOS_setup()
{
  xTaskCreatePinnedToCore(&EMG_task, "EMG_Task", 8192, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(&ES_task, "ES_Task", 8192, NULL, 2, NULL, 1);
  
}


/*
&EMG_task: 태스크가 시작될 때 호출될 함수의 포인터입니다. 여기서는 EMG_task라는 함수에서 태스크가 시작됩니다.
"EMG_Task": 태스크의 이름입니다. 디버깅 시에 유용하게 사용될 수 있습니다.
8192: 태스크 스택의 크기를 바이트 단위로 지정합니다. 여기서는 8192바이트, 즉 8KB의 스택 크기를 할당했습니다.
NULL: 태스크에 전달될 파라미터의 포인터입니다. 여기서는 파라미터가 없으므로 NULL로 설정되었습니다.
5: 태스크의 우선순위입니다. FreeRTOS에서는 숫자가 클수록 높은 우선순위를 갖습니다. 따라서 이 태스크는 우선순위 5를 가지게 됩니다.
NULL: 태스크 핸들을 저장할 변수의 포인터입니다. 태스크 핸들을 저장하지 않으려면 NULL로 설정합니다.
1: 태스크가 실행될 코어의 ID입니다. ESP32는 보통 0번과 1번 코어를 가지고 있으며, 여기서는 1번 코어에 태스크를 고정하여 실행하도록 설정했습니다.
이 함수 호출은 결국 EMG_task라는 함수를 기반으로 하는 태스크를 생성하고, 이 태스크는 8KB의 스택을 가지며, 우선순위 5로 설정되어 1번 코어에 고정되어 실행될 것입니다.


*/