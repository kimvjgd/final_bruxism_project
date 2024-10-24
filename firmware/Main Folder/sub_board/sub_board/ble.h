#include <NimBLEDevice.h>

// BLE 서버의 서비스와 특성 UUID
#define SERVICE_UUID          "0000fff0-0000-1000-8000-00805f9b34fb"
#define CHARACTERISTIC_UUID_RX "0000fff4-0000-1000-8000-00805f9b34fb"

// BLE 장치 주소 (실제 주소로 변경)
static BLEAddress bleAddress("C4:D3:6A:9A:5C:08");

bool isConnected = false;
NimBLEClient* client = nullptr;
uint16_t temp_number = 0; // 데이터를 저장할 변수

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE Client...");

  // BLE 초기화
  NimBLEDevice::init("");

  // BLE 서버에 연결 시도
  connectToServer();
}

void loop() {
  if (isConnected) {
    // 연결되어 있으면 여기서 데이터를 처리하거나, 필요시 재연결 등을 할 수 있습니다.
  } else {
    Serial.println("Not connected, retrying connection...");
    connectToServer();
  }
  delay(1000); // 1초마다 연결 상태 확인
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

// Notify 콜백 함수
void notificationCallback(NimBLERemoteCharacteristic* characteristic, uint8_t* data, size_t length, bool isNotify) {
  Serial.print("Notification received: ");
  Serial.write(data, length);


  // 수신된 데이터를 문자열로 변환
  String receivedString = "";
  for (size_t i = 0; i < length; i++) {
    // 아스키 코드 값을 문자열로 변환
    receivedString += (char)data[i];
  }

  // 문자열에서 줄 바꿈 문자 제거 (CR, LF)
  receivedString.trim();

  // 문자열을 정수로 변환하여 temp_number에 저장
  temp_number = receivedString.toInt();
  Serial.print("temp_number: ");
  Serial.println(temp_number);
}
