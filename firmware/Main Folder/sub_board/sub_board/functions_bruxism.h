#include "constants.h"

//////////////////////// INA219 ////////////////////////
void INA_INIT_SETTING() {
    if(! ina219.begin()) {
      Serial.println("Failed to find INA219 chip");
      while(1) {delay(10);}
    }
}

void SEND_INA_INFO() {
    shuntvoltage = ina219.getShuntVoltage_mV();
    busvoltage = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();
    power_mW = ina219.getPower_mW();
    loadvoltage = busvoltage + (shuntvoltage / 1000);
    
    Serial.print(emg_value);
    Serial.print(",");
    Serial.print(loadvoltage);
    Serial.print(",");
    Serial.print(current_mA);
    Serial.print(",");
    Serial.println(power_mW);

    // Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
    // Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
    // Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
    // Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
    // Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
    // Serial.println("");
}

// void INA_219_INIT_SETTING() {
//     while(!Serial) {
//         delay(1);
//         Serial.println("0");
//     }
//     if (! ina219.begin()) {
//         Serial.println("Failed to find INA219 chip");
//         while (1) { delay(10); }
//     }
// }



//////////////////////// BLE ////////////////////////
#include <NimBLEDevice.h>

// BLE 서버의 서비스와 특성 UUID
#define SERVICE_UUID          "0000fff0-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID_RX "0000fff4-0000-1000-8000-00805f9b34fb"

// BLE 장치 주소 (실제 주소로 변경)
static BLEAddress bleAddress("C4:D3:6A:9A:5C:08");

bool isConnected = false;
NimBLEClient* client = nullptr;

void BLE_INIT() {
    // BLE 초기화
    NimBLEDevice::init("");
}
// Notify 콜백 함수
void notificationCallback(NimBLERemoteCharacteristic* characteristic, uint8_t* data, size_t length, bool isNotify) {
  // Serial.print("Notification received: ");
  // Serial.write(data, length);


  // 수신된 데이터를 문자열로 변환
  String receivedString = "";
  for (size_t i = 0; i < length; i++) {
    // 아스키 코드 값을 문자열로 변환
    receivedString += (char)data[i];
  }

  // 문자열에서 줄 바꿈 문자 제거 (CR, LF)
  receivedString.trim();

  // 문자열을 정수로 변환하여 emg_value에 저장
  emg_value = receivedString.toInt();
  // Serial.print("temp_number: ");
  // Serial.println(emg_value);
}
// 서버에 연결 함수
void connectToServer() {
  Serial.println("Connecting to BLE server...");

  client = NimBLEDevice::createClient();
  
  if (!client->connect(bleAddress)) {
    Serial.println("Failed to connect");
    return;
  }

  Serial.println("Connected to server");
  isConnected = true;

  NimBLERemoteService* remoteService = client->getService(SERVICE_UUID);
  if (remoteService == nullptr) {
    Serial.println("Failed to find service UUID");
    client->disconnect();
    return;
  }

  NimBLERemoteCharacteristic* rxCharacteristic = remoteService->getCharacteristic(CHARACTERISTIC_UUID_RX);
  if (rxCharacteristic == nullptr) {
    Serial.println("Failed to find RX characteristic UUID");
    client->disconnect();
    return;
  }

  // Notify 기능 활성화
  if (rxCharacteristic->canNotify()) {
    rxCharacteristic->subscribe(true, notificationCallback);
  }
}



