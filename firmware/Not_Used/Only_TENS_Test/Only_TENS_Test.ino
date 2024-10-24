#define   H_1_PIN       25
#define   H_2_PIN       26

void setup() {
  pinMode(H_1_PIN, OUTPUT);
  pinMode(H_2_PIN, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  
    TENS_FUNC(H_1_PIN, H_2_PIN, 20);
    Delay(10000);
}


void TENS_FUNC(int PIN1, int PIN2, int period) {
  for(int i = 0; i < period; i++){
    digitalWrite(PIN1, LOW);
    digitalWrite(PIN2, HIGH);
    delayMicroseconds(175);
    // delay(1000);
    digitalWrite(PIN1, HIGH);
    digitalWrite(PIN2, LOW);
    delayMicroseconds(175);
    // delay(1000);
    digitalWrite(PIN1, LOW);
    digitalWrite(PIN2, LOW);
    delay(20);
  }
}

