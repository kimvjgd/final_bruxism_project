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

void SEND_INA_INFO() {
    shuntvoltage = ina219.getShuntVoltage_mV();
    busvoltage = ina219.getBusVoltage_V();
    current_mA = ina219.getCurrent_mA();
    power_mW = ina219.getPower_mW();
    loadvoltage = busvoltage + (shuntvoltage / 1000);
}


void setup() {
  Serial.begin(115200);
//   INA_219_INIT_SETTING();
}

void loop() {
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
    

  delay(50);  // 데이터 수집 속도 조절을 위한 지연 (필요에 따라 조정)
}
