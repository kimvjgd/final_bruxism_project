// const int DEBUG_MODE   = 1;    // 1이면 debug mode   0이면 main mode, 2면 분산 Test

//     // 아날로그 핀 설정
// const int emgPin = 33;          // EMG 데이터 입력 핀 (GPIO36, ADC1 채널)
// const int samplingRate = 10;   // 샘플링 주파수 (Hz)

// // Rectified Signal 변수
// float rectifiedSignal = 0.0;

// // Integrated Signal 변수
// float integratedSignal = 0.0;
// const int integrationWindow = 10;  // 통합 윈도우 (샘플 단위)
// float integrationBuffer[10] = {0}; // 통합 데이터를 저장할 버퍼
// int bufferIndex = 0;

// void setup() {
//   Serial.begin(115200); // 시리얼 통신 초기화 (속도: 115200bps)
//   pinMode(emgPin, INPUT);
// }

// void loop() {
//   // 1. Raw EMG 데이터 읽기
//   int emgRaw = analogRead(emgPin);

// ////////////////////////////////////////////////////////////////////////////////////////////////////////////
// //1. emgRaw 2. rectifiedSignal, 1/integratedSignal
//     if(DEBUG_MODE == 0) {

//         // ESP32의 ADC는 기본적으로 0~4095의 값을 반환합니다 (12비트 해상도).
//         // 정규화하여 0~3.3V의 전압 값으로 변환
//         float rawVoltage = (emgRaw / 4095.0) * 3.3;

//         // 2. Rectified Signal 계산
//         rectifiedSignal = abs(rawVoltage - 1.65); // 기준 전압 1.65V를 중심으로 절대값 계산

//         // 3. Integrated Signal 계산
//         integrationBuffer[bufferIndex] = rectifiedSignal;
//         bufferIndex = (bufferIndex + 1) % integrationWindow;

//         // 버퍼 평균 계산 (간단한 이동 평균)
//         integratedSignal = 0.0;

//         for (int i = 0; i < integrationWindow; i++) {
//             integratedSignal += integrationBuffer[i];
//         }

//         integratedSignal /= integrationWindow;
//     }

//     if(DEBUG_MODE == 1) {

//         float rawVoltage = emgRaw * 1.0 - 1500;            // 이건 무조건 양수가 나오고
//         integrationBuffer[bufferIndex] = abs(rawVoltage);
//         bufferIndex = (bufferIndex + 1) % integrationWindow;
        
//         integratedSignal = 0.0;
//         for(int i=0; i<integrationWindow; i++) {
//             integratedSignal += integrationBuffer[i];
//         }
//         integratedSignal /= integrationWindow;
//     }

//     if(DEBUG_MODE == 0) {
//         // 4. Arduino Serial Plotter 출력
//         // Serial.print("Raw EMG: ");
//         Serial.print(emgRaw);  // Raw Signal (전압)
//         Serial.print(',');          // 탭으로 구분
//         // Serial.print("Rectified EMG: ");
//         Serial.print(rectifiedSignal, 3); // Rectified Signal
//         Serial.print(',');
//         // Serial.print("Integrated EMG: ");
//         Serial.println(integratedSignal, 3); // Integrated Signal

//         // 샘플링 주파수 유지
//         delay(1000 / samplingRate); // 10Hz 샘플링 속도 유지
//     }else if (DEBUG_MODE == 1) {
//       Serial.print(emgRaw);
//       Serial.print(',');
//       Serial.print(integratedSignal, 3);
//       Serial.print(',');
//       Serial.println(0);
//       delay(1000 / samplingRate); // 10Hz 샘플링 속도 유지
//     }
// }

// // #define pin_no 33
// // void setup() {
// //   // pinMode(pin_no, INPUT)
// //   Serial.begin(115200);
// // }

// // void loop() {
// //   Serial.println(analogRead(pin_no));
// //   delay(100);
// // }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// #define ANALOG_PIN 33
// #define BUFFER_SIZE 60
// #define READ_INTERVAL 100

// #include <Arduino.h>
// #include <math.h>

// float buffer[BUFFER_SIZE];
// int bufferIndex = 0;
// bool bufferFull = false;

// void setup() {
//   Serial.begin(115200);
//   pinMode(ANALOG_PIN, INPUT);
// }

// void loop() {
//   // 1. 아날로그 값 읽기
//   float analogValue = analogRead(ANALOG_PIN);

//   // 2. 버퍼에 값 저장
//   buffer[bufferIndex] = analogValue;
//   bufferIndex++;

//   // 버퍼가 가득 찼을 경우 순환
//   if (bufferIndex >= BUFFER_SIZE) {
//     bufferIndex = 0;
//     bufferFull = true;
//   }

//   // 3. 표준편차와 평균 계산 (버퍼가 가득 찼을 때)
//   if (bufferFull) {
//     float mean = calculateMean(buffer, BUFFER_SIZE);
//     float stddev = calculateStdDev(buffer, BUFFER_SIZE, mean);

//     // 결과 출력
//     // Serial.print("Mean: ");
//     Serial.print(analogValue);
//     Serial.print(',');
//     // Serial.print(", StdDev: ");
//     Serial.print(stddev);
//     Serial.print(',');
//     Serial.println(0);
//   }

//   // 4. 100ms 대기
//   delay(READ_INTERVAL);
// }

// // 평균 계산 함수
// float calculateMean(float* data, int size) {
//   float sum = 0.0;
//   for (int i = 0; i < size; i++) {
//     sum += data[i];
//   }
//   return sum / size;
// }

// // 표준편차 계산 함수
// float calculateStdDev(float* data, int size, float mean) {
//   float sumSquares = 0.0;
//   for (int i = 0; i < size; i++) {
//     float diff = data[i] - mean;
//     sumSquares += diff * diff;
//   }
//   return sqrt(sumSquares / size);
// }


///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
#define ANALOG_PIN 33
#define BUFFER_SIZE 60
#define READ_INTERVAL 100

#include <Arduino.h>
#include <math.h>

// 변수 선언
float buffer[BUFFER_SIZE];
int bufferIndex = 0;
bool bufferFull = false;
float previousValue = 0.0;

// 함수 선언
float calculateMean(float* data, int size);
float calculateStdDev(float* data, int size, float mean);

void setup() {
  Serial.begin(115200);
  pinMode(ANALOG_PIN, INPUT);
}

void loop() {
  // 1. 아날로그 값 읽기
  float analogValue = analogRead(ANALOG_PIN);

  // 2. 현재 값과 바로 직전 값의 차 계산
  float diff = abs(analogValue - previousValue);
  previousValue = analogValue;

  // 3. 버퍼에 차이를 저장
  buffer[bufferIndex] = diff;
  bufferIndex++;

  // 버퍼가 가득 찼을 경우 순환
  if (bufferIndex >= BUFFER_SIZE) {
    bufferIndex = 0;
    bufferFull = true;
  }

  // 4. 표준편차 계산 (버퍼가 가득 찼을 때)
  if (bufferFull) {
    float mean = calculateMean(buffer, BUFFER_SIZE);
    float stddev = calculateStdDev(buffer, BUFFER_SIZE, mean);

    // 결과 출력
    // Serial.print("Mean: ");
    Serial.print(analogValue);
    Serial.print(',');
    // Serial.print(", StdDev: ");
    Serial.print(stddev);
    Serial.print(',');
    Serial.println(0);
  }

  // 5. 샘플링 주기 유지
  delay(READ_INTERVAL);
}

// 평균 계산 함수
float calculateMean(float* data, int size) {
  float sum = 0.0;
  for (int i = 0; i < size; i++) {
    sum += data[i];
  }
  return sum / size;
}

// 표준편차 계산 함수
float calculateStdDev(float* data, int size, float mean) {
  float sumSquares = 0.0;
  for (int i = 0; i < size; i++) {
    float diff = data[i] - mean;
    sumSquares += diff * diff;
  }
  return sqrt(sumSquares / size);
}

