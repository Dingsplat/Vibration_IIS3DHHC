// 3-axis Accelerometer
// SMicroelectronics IIS3DHH
// Arduino DUE - 3.3V SPI on ICSP Connector

#include <SPI.h>

#define SCALE 0.076294;
// scale factor: +/- 2.5G full range = 5000mG total range / 65536 counts (16 bit)

double xAcc, yAcc, zAcc;
volatile unsigned long elapsedTimeInMillis = 0;
volatile unsigned long interval = 0;


void setup() {

  SerialUSB.begin(115200);
  SPI.begin(10);

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


  SerialUSB.print(interval);
  SerialUSB.print(",");
  SerialUSB.print(xAcc, 0);
  SerialUSB.print(",");
  SerialUSB.print(yAcc, 0);
  SerialUSB.print(",");
  SerialUSB.println(zAcc, 0);

  delay(2); // Adjust for sampling rate (e.g. 10mS = 100Hz. 2mS = 500Hz etc.)
}





//=========== READ IIS3DHHC REGISTERS ==========
void readVal()
{
  byte xAddressByteL = 0x28; // Low Byte of X value (the first data register)
  byte readBit = B10000000; // bit 0 (MSB) HIGH means read register,
  byte dataByte = xAddressByteL | readBit;
  byte b0 = 0x0; // an empty byte to initiate read register
  //SerialUSB.println("read request:");
  //SerialUSB.println(dataByte);

  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE3));
  //delay(2);
  SPI.transfer(10, dataByte, SPI_CONTINUE); // request a read, starting at X low byte
  byte xL = SPI.transfer(10, b0, SPI_CONTINUE); // get the low byte of X data
  byte xH = SPI.transfer(10, b0, SPI_CONTINUE); // get the high byte of X data
  byte yL = SPI.transfer(10, b0, SPI_CONTINUE); // get the low byte of Y data
  byte yH = SPI.transfer(10, b0, SPI_CONTINUE); // get the high byte of Y data
  byte zL = SPI.transfer(10, b0, SPI_CONTINUE); // get the low byte of Z data
  byte zH = SPI.transfer(10, b0); // get the high byte of Z data
  //delay(2);
  SPI.endTransaction();

  // shift the high byte left 8 bits and merge the high and low
  int16_t xVal = (xL | (xH << 8));
  int16_t yVal = (yL | (yH << 8));
  int16_t zVal = (zL | (zH << 8));

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

  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE3));
  SPI.transfer(10, addressByte, SPI_CONTINUE);
  SPI.transfer(10, ctrlRegByte);
  delay(10);
  SPI.endTransaction();

  delay(100);

  // write to Control Register 4: address 23h
  addressByte = 0x23;
  // This register configures filter and bandwidth
  ctrlRegByte = 0x00; // FIR Linear Phase, 440Hz

  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE3));
  delay(10);
  SPI.transfer(10,addressByte, SPI_CONTINUE);
  SPI.transfer(ctrlRegByte);
  SPI.endTransaction();

  delay(1000);

  /*
    // read WHO_AM_I Register
    byte AddressByte_WHO = 0x0F; // Low Byte of X value (the first data register)
    byte readBit = B10000000; // bit 0 (MSB) HIGH means read register,
    byte whoByte = AddressByte_WHO | readBit;

    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE3));
    delay(10);
    SPI.transfer(10, whoByte, SPI_CONTINUE); // Request read register 0Fh
    byte who = SPI.transfer(10,0x0); // Get accelerometer ID (IIS3DHHC = 11h)
    delay(10);
    SPI.endTransaction();
    
    SerialUSB.println("WHO_AM_I read request:");
    SerialUSB.println(whoByte);
    SerialUSB.println("WHO_AM_I response:");
    SerialUSB.println(who);

    delay(5000);
  */
}
