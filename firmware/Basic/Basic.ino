#include "constants.h"

void setup() {
  // put your setup code here, to run once:
  INIT_SETTING();
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

/*
#include "myBLE.h"

void setup() {
  initBLE();
  Serial.begin(115200);
}
int a= 0;
void loop() {
  // pCharacteristicTX->notify();
  // String str;
  // char data[256];
  
  // str = "VBAT,"  + String(vbat_level);
  // str.toCharArray(data, str.length() + 1);
  // BLE_Send_Msg(data);
  a++;
  char data[256];
  String str;
  str = "Dong : " + String(a);
  str.toCharArray(data, str.length() + 1);
  BLE_Send_Msg(data);
  delay(1000);
}
*/