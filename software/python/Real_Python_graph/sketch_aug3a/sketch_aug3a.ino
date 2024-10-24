int value_EMG = 0;
int value_CURRENT = 0;
int value_VOLTAGE = 0;
int value_POWER = 0;

String receivedData = "";  // 데이터 저장 버퍼

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

    // Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
    // Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
    // Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
    // Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
    // Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
    // Serial.println("");
}


void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  INA_219_INIT_SETTING();
}

void loop() {
  while (Serial2.available()) {
    char a = Serial2.read();
    if (a == '\n') {  // \n 문자를 기준으로 데이터 처리
      value_EMG = receivedData.toInt();  // 문자열을 정수로 변환
      receivedData = "";  // 버퍼 초기화
      
      // 다른 데이터와 함께 시리얼 출력
      // Serial.print("Received EMG Value: ");


      SEND_INA_INFO();


      Serial.print(value_EMG);
      Serial.print(",");
      Serial.print(loadvoltage);
      Serial.print(",");
      Serial.print(current_mA);
      Serial.print(",");
      Serial.println(power_mW);
    } else if (a != '\r') {  // \r 문자는 무시
      receivedData += a;  // 데이터를 버퍼에 추가
    }
  }

  // 테스트를 위해 value_CURRENT, value_VOLTAGE 값 변경
  // if (value_CURRENT > 20) {
  //   value_CURRENT = 1;
  // }
  // if (value_VOLTAGE > 5) {
  //   value_VOLTAGE = 1;
  // }

  // value_VOLTAGE = value_VOLTAGE + 1;
  // value_CURRENT = value_CURRENT + 2;
  // value_POWER = (value_CURRENT * value_VOLTAGE) % 100;

  delay(50);  // 데이터 수집 속도 조절을 위한 지연 (필요에 따라 조정)
}
