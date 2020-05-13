// 3-axis Accelerometer
// SMicroelectronics IIS3DHH
// Arduino UNO

#include <SPI.h>

#define SS 10 // Serial Select -> CS on IIS3DHH
#define MOSI 11 // MasterOutSlaveIn -> SDI
#define MISO 12 // MasterInSlaveOut -> SDO
#define SCK 13 // Serial Clock -> SPC on IIS3DHH

#define SCALE 0.076294;
// scale factor: +/- 2.5G full range = 5000mG total range / 65536 counts (16 bit)

double xAcc, yAcc, zAcc;
volatile unsigned long elapsedTimeInMillis = 0;
volatile unsigned long interval = 0;


void setup() {

  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);
  Serial.begin(115200);
  SPI.begin();

  // Configure accelerometer
  Accelerometer_Setup();

}

void loop() {
  readVal(); // get acc values and put into global variables

  static unsigned long previousTimeInMillis = 0; // set first sample timer at 0
  unsigned long time = millis(); // timestamp first sample
  elapsedTimeInMillis = time - previousTimeInMillis; // - gives mS time interval between samples; + gives mS time interval from start of sampling
  previousTimeInMillis = time; 
  interval += elapsedTimeInMillis; // calculate cumulative time intervals between sample reads ****16.04.20 - test change to seconds for Matplotlib python csvdataprocessing


  Serial.print(interval);
  Serial.print(",");
  Serial.print(xAcc, 0);
  Serial.print(",");
  Serial.print(yAcc, 0);
  Serial.print(",");
  Serial.println(zAcc, 0);
  //Serial.println(",");

  delay(11);
}





//=========== READ IIS3DHHC REGISTERS ==========
void readVal()
{
  byte xAddressByteL = 0x28; // Low Byte of X value (the first data register)
  byte readBit = B10000000; // bit 0 (MSB) HIGH means read register,
  byte dataByte = xAddressByteL | readBit;
  byte b0 = 0x0; // an empty byte to initiate read register
  //Serial.println("read request:");
  //Serial.println(dataByte);

  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
  digitalWrite(SS, LOW); // SS must be LOW to communicate
  delay(9);
  SPI.transfer(dataByte); // request a read, starting at X low byte
  byte xL = SPI.transfer(b0); // get the low byte of X data
  byte xH = SPI.transfer(b0); // get the high byte of X data
  byte yL = SPI.transfer(b0); // get the low byte of Y data
  byte yH = SPI.transfer(b0); // get the high byte of Y data
  byte zL = SPI.transfer(b0); // get the low byte of Z data
  byte zH = SPI.transfer(b0); // get the high byte of Z data
  delay(9);
  digitalWrite(SS, HIGH);
  SPI.endTransaction();

  // shift the high byte left 8 bits and merge the high and low
  int xVal = (xL | (xH << 8));
  int yVal = (yL | (yH << 8));
  int zVal = (zL | (zH << 8));

  // scale the values into mG
  xAcc = xVal * SCALE;
  yAcc = yVal * SCALE;
  zAcc = zVal * SCALE;
}




//=========== SETUP IIS3DHHC ==========
void Accelerometer_Setup()
{

  // write to Control register 1: address 20h
  byte addressByte = 0x20;
  byte ctrlRegByte = 0xC0; // 0xC0 = 1100 0000 : normal mode, incremented addressing // 0x80 = normal mode, do not increment

  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
  digitalWrite(SS, LOW);
  delay(10);
  SPI.transfer(addressByte);
  SPI.transfer(ctrlRegByte);
  delay(10);
  digitalWrite(SS, HIGH);
  SPI.endTransaction();

  delay(100);

  // write to Control Register 4: address 23h
  addressByte = 0x23;
  // This register configures filter and bandwidth
  ctrlRegByte = 0x00; // FIR Linear Phase, 440Hz

  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
  digitalWrite(SS, LOW);
  delay(10);
  SPI.transfer(addressByte);
  SPI.transfer(ctrlRegByte);
  delay(10);
  digitalWrite(SS, HIGH);
  SPI.endTransaction();

  delay(1000);

  /*
    // read WHO_AM_I Register
    byte AddressByte_WHO = 0x0F; // Low Byte of X value (the first data register)
    byte readBit = B10000000; // bit 0 (MSB) HIGH means read register,
    byte whoByte = AddressByte_WHO | readBit;

    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
    digitalWrite(SS, LOW);
    delay(10);
    SPI.transfer(whoByte); // Request read register 0Fh
    byte who = SPI.transfer(0x0); // Get accelerometer ID (IIS3DHHC = 11h)
    delay(10);
    digitalWrite(SS, HIGH);
    SPI.endTransaction();
    Serial.println("WHO_AM_I read request:");
    Serial.println(whoByte);
    Serial.println("WHO_AM_I response:");
    Serial.println(who);

    delay(5000);
  */
}
