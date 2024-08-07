#define   BTN_PIN       21
#define   MUX_SEL_PIN   15
#define   H_1_PIN       25
#define   H_2_PIN       26
#define   MUX_B_PIN     32
#define   POTENTIO_PIN  14
#define   TENS_MODE     0
#define   EMG_MODE      1


bool btn_chk = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(BTN_PIN, INPUT);
  pinMode(MUX_SEL_PIN, OUTPUT);
  pinMode(H_1_PIN, OUTPUT);
  pinMode(H_2_PIN, OUTPUT);
  pinMode(MUX_B_PIN, OUTPUT);
  
  // digitalWrite(MUX_SEL_PIN, HIGH);
  
  Serial.begin(115200);
}
int temp_emg_buffer_value = 0;
int temp_emg_value = 0;
int potentio_value = 0;
void loop() {
  

  potentio_value = analogRead(POTENTIO_PIN);
  // Serial.print("Current Potentio Value : ");
  // Serial.print(potentio_value);
  // Serial.print("  &  ");
  if(potentio_value > 2330) {
    Serial.println("TENS MODE");
    digitalWrite(MUX_SEL_PIN, TENS_MODE);      // 
    TENS_FUNC(H_1_PIN, H_2_PIN, 20);
  }else {
    Serial.println("EMG MODE");
    digitalWrite(MUX_SEL_PIN, EMG_MODE);       // 
    digitalWrite(MUX_B_PIN, HIGH);
    delay(1);
    digitalWrite(MUX_B_PIN, LOW);
    delay(1);
  }



  // temp_emg_buffer_value++;
  // if(temp_emg_buffer_value %5 == 0) {
  //   temp_emg_value++;
  //   temp_emg_buffer_value = 0;
  // }
  // analogWrite(MUX_B_PIN, temp_emg_buffer_value);
  // // if(temp_emg_value);
  // Serial.println(temp_emg_buffer_value);
  // if(temp_emg_buffer_value >=1023) {
  //   temp_emg_buffer_value = 0;
  // }


  // put your main code here, to run repeatedly:
  // btn_chk = digitalRead(BTN_PIN);
  // if(btn_chk){
  //   digitalWrite(MUX_SEL_PIN, LOW);
  //   TENS_FUNC(H_1_PIN, H_2_PIN, 20);
  // }else {
  //   digitalWrite(MUX_SEL_PIN, HIGH);
  //   digitalWrite(MUX_B_PIN, HIGH);
  //   delay(1);
  //   digitalWrite(MUX_B_PIN, LOW);
  //   delay(1);
    
  // }

  // Serial.print("   ///   Current Btn state : ");
  // Serial.println(btn_chk);
  // Serial.println(btn_chk);
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

