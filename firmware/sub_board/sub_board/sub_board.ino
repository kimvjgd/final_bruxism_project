// #include "Application.h"

// void setup() {
//   freeRTOS_setup();
//   INIT_SETTING();
// }

// void loop() {

// }




#define SENSOR_PIN A0  // 센서가 연결된 핀 번호

const int BUFFER_SIZE = 50;  // 버퍼 크기

const int TEMP_BUFFER_SIZE = 50;

int buffer[BUFFER_SIZE];  // 데이터 저장 버퍼
int temp_buffer[TEMP_BUFFER_SIZE];
int bufferIndex = 0;  // 현재 버퍼 인덱스
long sum = 0;  // 데이터 합계
float temp_sum = 0;
float average = 0.0f;
// 청년 몽땅

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < BUFFER_SIZE; i++) {
    buffer[i] = 0;
  }
  for (int i = 0; i < TEMP_BUFFER_SIZE; i++) {
    temp_buffer[i] = 0;
  }
  
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  delay(3000);
  
  if((digitalRead(10) == 1) || (digitalRead(11) == 1)){

  }else {

    for (int i = 0; i < TEMP_BUFFER_SIZE; i++) {
      temp_buffer[i] = analogRead(SENSOR_PIN);
      temp_sum += analogRead(SENSOR_PIN);
    }
    average = (float) temp_sum / TEMP_BUFFER_SIZE;
    Serial.println(average);
  }
}

void loop() {
  if((digitalRead(10) == 1)||(digitalRead(11) == 1)){

  } else {
    int sensorValue = analogRead(SENSOR_PIN) - average;
    sum = sum - buffer[bufferIndex] + sensorValue;
    buffer[bufferIndex] = sensorValue;
    bufferIndex++;
    if (bufferIndex >= BUFFER_SIZE) {
      bufferIndex = 0;  // 버퍼 순환
    }

    // 평균값 계산
    float average = sum / (float)BUFFER_SIZE;

    // 평균값 출력
    // Serial.print("Average value: ");
    // Serial.println(average);

    // Serial.println(analogRead(A0));
  }
  delay(10);
}


