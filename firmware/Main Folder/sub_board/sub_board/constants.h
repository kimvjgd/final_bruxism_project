//////////////////////////////// INA_219 ////////////////////////////////

// INA219 SDA   -   ESP 21 pin
// INA219 SCK   -   ESP 22 pin

#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float power_mW = 0;
uint16_t emg_value = 0;


void INIT_SETTING() {
    Serial.begin(115200);
}