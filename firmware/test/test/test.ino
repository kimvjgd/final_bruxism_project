// #define   MUX_SEL           2
// #define   EMG_OUTPUT_PIN    25
// #define   LO_PLUS           26
// #define   LO_MINUS          27
// #define   MUX_S1A           4

// #define   SELECT_ES_MODE    digitalWrite(MUX_SEL, 1)
// #define   SELECT_EMG_MODE   digitalWrite(MUX_SEL, 0)


// int EMG_THRESHOLD_VALUE =   3600;
// int emg_value;

// void setup() {
//   // put your setup code here, to run once:
//   Serial.begin(115200);
//   pinMode(MUX_S1A, OUTPUT);
//   pinMode(MUX_SEL, OUTPUT);
//   pinMode(LO_MINUS, INPUT);
//   pinMode(LO_PLUS, INPUT);

//   // MUX_SEL = 0 -> EMG, 1 -> E.S. Mode
//   // SELECT_EMG_MODE;
//   digitalWrite(MUX_SEL, 0);
//   // digitalWrite(MUX_S1A, LOW);
// }

// void loop() {
//   // put your main code here, to run repeatedly:
//   Serial.print("MINUS : ");
//   Serial.print(digitalRead(LO_MINUS));
//   Serial.print("    PLUS : ");
//   Serial.println(digitalRead(LO_PLUS));
  
//   if((digitalRead(LO_MINUS) == 1) || (digitalRead(LO_PLUS) == 1)){
//     }else {

//     emg_value = analogRead(EMG_OUTPUT_PIN);
//     Serial.println(emg_value);
//     // if(emg_value > EMG_THRESHOLD_VALUE) {
//     //   SELECT_ES_MODE;
//     //   delay(100);
//     //   ES_FUNC(40);
//     //   delay(100);
//     //   SELECT_EMG_MODE;
//     //   emg_value = 0;
//     // }      
//   }
// }

// void ES_FUNC(int count) {
//   for(int i = 0; i < count; i++) {
//     digitalWrite(MUX_S1A, HIGH);
//     delayMicroseconds(175);
//     digitalWrite(MUX_S1A, LOW);
//     delay(20);
//   }
// }


void setup() {
  Serial.begin(115200);
  pinMode(26, INPUT);
  pinMode(27, INPUT);
  
}

void loop () {
  Serial.print("MINUS : ");
  Serial.print(digitalRead(26));
  Serial.print("    PLUS : ");
  Serial.println(digitalRead(27));
  
  if((digitalRead(26)==1)||(digitalRead(27)==1)) {

  }else {
    Serial.println(analogRead(25));
  }
  delay(1);
}

