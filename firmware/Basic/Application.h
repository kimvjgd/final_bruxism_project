
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"
#include "driver/gpio.h"
#include "soc/wdev_reg.h"
#include "xtensa/core-macros.h"

#include "myBLE.h"
#include "Adafruit_MCP9808.h"

void Temper_task(void *pvParameter)
{
  (void)pvParameter;

  Serial.println("Test_task Start");
  

  while (1)
  {


    vTaskDelay(500 / portTICK_RATE_MS);
  }
}

void freeRTOS_setup()
{
  xTaskCreatePinnedToCore(&Temper_task, "Temper_task", 8192, NULL, 5, NULL, 1);
}