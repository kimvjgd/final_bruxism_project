// For Debugging
#define     DEBUG_MODE      true

// Pin Settings
#define     MUX_SEL         2
#define     MUX_S1A         4
#define     SIGNAL_OUT      25          // EMG_SIGNAL PIN
#define     LO_PLUS         26
#define     LO_MINUS        27

// Value Constants.
// E.S.는 한번의 큰 전체 주기가 있다.
// 그 큰 전체 주기 안에 여러 펄스들와 relaxation 이 있는 작은 주기가 있다.
// 그 작은 주기 안에 HIGH, LOW 가 몇번 반복되고 Relaxation도 포함 되어있다.
// Basic 폴더 안에 Electrical_Stimulation_Terms 사진을 참고하자
#define     TOTAL_DURATION_CNT      2500    // 한 주기에 
#define     PULSE_DURATION_CNT      500
#define     EMG_THR_VALUE   2500        // Temp - 나중에 EEPROM으로 Calibration 시킬 예정



////////////////// Changeable Values //////////////////
uint8_t CUR_CNT = 0;
byte    PULSE_PERIOD_CNT = 15;              // 나중에 연구실 가서 데이터 보고 pulse count를 세자.... 지금은 잘 기억이 안난다.
byte    PULSE_
// true - EMG, false - E.S.
bool CURRENT_MODE = true;
////////////////////////////////////////




// Funcs
void INIT_SETTING() {
    Serial.begin(115200);

    pinMode(MUX_SEL, OUTPUT);
    pinMode(MUX_S1A, OUTPUT);
    pinMode(LO_PLUS, INPUT);
    pinMode(LO_MINUS, INPUT);
    

    digitalWrite(MUX_SEL, 0);
    digitalWrite(MUX_S1A, 0);
}

// MUX_SEL == 1 -> B MUX Connect -> EMG MODE 
 

// MUX_SEL == 0 -> A MUX Connect -> E.S. MODE 
bool SET_ELEC_STIMUL_MODE(){
    digitalWrite(MUX_SEL, 0);
    return false;
}