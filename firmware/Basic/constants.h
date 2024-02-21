#define     MUX_SEL     2
#define     MUX_S1A     4
#define     SIGNAL_OUT  25
#define     LO_PLUS     26
#define     LO_MINUS    27


void INIT_SETTING() {
    pinMode(MUX_SEL, OUTPUT);
    digitalWrite(MUX_SEL, 0);
    pinMode(MUX_S1A, OUTPUT);
    digitalWrite(MUX_S1A, 1);
}