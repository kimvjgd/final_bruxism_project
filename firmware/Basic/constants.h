// Pin Settings
#define     MUX_SEL         2
#define     MUX_S1A         4
#define     SIGNAL_OUT      25
#define     LO_PLUS         26
#define     LO_MINUS        27

// Value Constants.
#define     MAX_COUNT       200
#define     EMG_THR_VALUE   2500        // Temp - 나중에 EEPROM으로 Calibration 시킬 예정




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
bool SET_EMG_MODE() {
    digitalWrite(MUX_SEL, 1);
    return true;
}

// MUX_SEL == 0 -> A MUX Connect -> E.S. MODE 
bool SET_ELEC_STIMUL_MODE(){
    digitalWrite(MUX_SEL, 0);
    return false;
}