#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>

BLECharacteristic *pCharacteristic;
bool device Connected = false;
int txValue = 0;

#define SERVICE_UUID              "0000fff0-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID_RX    "0000fff4-0000-1000-8000-00805f9b34fb"
// #define CHARACTERISTIC_UUID_TX    "0000fff1-0000-1000-8000-00805f9b34fb"


void setup(){

}

void loop() {

}