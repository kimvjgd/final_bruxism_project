#include <SPI.h>

#define DRDY_PIN  2
#define CS_PIN    10
#define RST_PIN   9

void setup() {
  Serial.begin(9600);
  
  pinMode(DRDY_PIN, INPUT);
  pinMode(CS_PIN, OUTPUT);
  pinMode(RST_PIN, OUTPUT);

  // Reset ADS1291
  digitalWrite(RST_PIN, LOW);
  delay(100);
  digitalWrite(RST_PIN, HIGH);
  delay(100);

  // Initialize SPI
  SPI.begin();
  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(SPI_CLOCK_DIV16); // Adjust clock divider as needed

  // Initialize ADS1291
  resetADS1291();
  configureADS1291();
}

void loop() {
  // if (digitalRead(DRDY_PIN) == LOW) {
    // Data is ready to read
    readData();
  // }
  
  delay(100); // Adjust delay as needed
}

void resetADS1291() {
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(0x06); // Send SDATAC command to stop continuous read mode
  delay(1);
  digitalWrite(CS_PIN, HIGH);

  // Reset command
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(0x11); // Send RESET command
  delay(1);
  digitalWrite(CS_PIN, HIGH);
}

void configureADS1291() {
  // Set CONFIG1 register
  writeRegister(0x01, 0x02); // Example: Set to 250SPS, refer to datasheet for other configurations

  // Set CONFIG2 register
  writeRegister(0x02, 0xE0); // Example: Enable internal reference

  // Set LOFF register (optional, if needed)
  writeRegister(0x03, 0x10);
}

void writeRegister(byte reg, byte value) {
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(0x40 | reg);
  SPI.transfer(0x00);
  SPI.transfer(value);
  digitalWrite(CS_PIN, HIGH);
}

void readData() {
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(0x12); // Send RDATA command
  delayMicroseconds(10);
  
  byte data[3];
  data[0] = SPI.transfer(0x00);
  data[1] = SPI.transfer(0x00);
  data[2] = SPI.transfer(0x00);
  digitalWrite(CS_PIN, HIGH);
  
  long result = ((long)data[0] << 16) | ((long)data[1] << 8) | (long)data[2];
  
  // Serial.print("EMG Value: ");
  Serial.println(result);
}
