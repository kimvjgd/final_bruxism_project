int value_EMG = 0;
int value_CURRENT = 0;
int value_VOLTAGE = 0;
int value_POWER = 0;

#define ANALOG_READ_PIN   34

void setup() {
  // put your setup code here, to run once:
  pinMode(ANALOG_READ_PIN, INPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  value_EMG = analogRead(ANALOG_READ_PIN);
  if(value_CURRENT > 20) {
    value_CURRENT = 1;
  }
  if(value_VOLTAGE > 5) {
    value_VOLTAGE = 1;
  }

  value_VOLTAGE = value_VOLTAGE + 1;
  value_CURRENT = value_CURRENT + 2;
  value_POWER = (value_CURRENT * value_VOLTAGE) % 100;
  Serial.print(value_EMG);
  Serial.print(",");
  Serial.print(value_VOLTAGE);
  Serial.print(",");
  Serial.print(value_CURRENT);
  Serial.print(",");
  Serial.println(value_POWER);
  
  delay(50);
}
