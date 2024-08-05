//////////////////////////////// U A R T ////////////////////////////////
#define RXD2 2
#define TXD2 4

void UART_INIT_SETTING() {
    Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
}

void TRANSFER_UART_DATA() {
    if(Serial2.available()) {
        char a = Serial2.read();
        if(a == ',') {
            Serial.println();
        } else {
            Serial.print(a);
        }
    }
}


//////////////////////////////// INA_219 ////////////////////////////////
#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float power_mW = 0;

void INA_219_INIT_SETTING() {
    while(!Serial) {
        delay(1);
        Serial.println("0");
    }
    if (! ina219.begin()) {
        Serial.println("Failed to find INA219 chip");
        while (1) { delay(10); }
    }
}

void SEND_INA_INFO() {
    shuntvoltage = ina219.getShuntVoltage_mV();
    busvoltage = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();
    power_mW = ina219.getPower_mW();
    loadvoltage = busvoltage + (shuntvoltage / 1000);

    Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
    Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
    Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
    Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
    Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
    Serial.println("");
}





void INIT_SETTING() {
    Serial.begin(115200);
    UART_INIT_SETTING();
    INA_219_INIT_SETTING();
}