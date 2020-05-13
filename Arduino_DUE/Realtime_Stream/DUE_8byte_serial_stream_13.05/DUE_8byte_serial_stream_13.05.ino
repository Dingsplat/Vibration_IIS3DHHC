// 3-axis Accelerometer
// SMicroelectronics IIS3DHH 
// Arduino UNO to RealTime_3Axis_Stream_UNO.py

#include <SPI.h>

#define SCALE 0.000076294;
// scale factor: +/- 2.5G full range = 5G total range / 65536 counts (16 bit)
//#define SCALE 0.076294;
// scale factor: +/- 2500mG full range = 5000mG total range / 65536 counts (16 bit)

double xAcc, yAcc, zAcc, x, y, z;
int xVal, yVal, zVal;


void setup() {
  
  SerialUSB.begin(115200);
  SPI.begin(10);

// Configure accelerometer
  Accelerometer_Setup();

}

void loop() {
  readVal(); 
  sendToPC(&x, &y, &z);

 }




//=========== SEND DATA TO SERIAL PORT ==========
void sendToPC(double* data1, double* data2, double* data3)
//{
//  byte* byteData1 = (byte*)(data1);
//  byte* byteData2 = (byte*)(data2);
//  byte* byteData3 = (byte*)(data3);
//  byte buf[12] = {byteData1[0], byteData1[1], byteData1[2], byteData1[3],
//                 byteData2[0], byteData2[1], byteData2[2], byteData2[3],
//                 byteData3[0], byteData3[1], byteData3[2], byteData3[3]};
//  SerialUSB.write(buf, 12);
//}

{
  byte* byteData1 = (byte*)(data1);
  byte* byteData2 = (byte*)(data2);
  byte* byteData3 = (byte*)(data3);
  byte buf[24] = {byteData1[0], byteData1[1], byteData1[2], byteData1[3], byteData1[4], byteData1[5], byteData1[6], byteData1[7],
                 byteData2[0], byteData2[1], byteData2[2], byteData2[3], byteData2[4], byteData2[5], byteData2[6], byteData2[7],
                 byteData3[0], byteData3[1], byteData3[2], byteData3[3], byteData3[4], byteData3[5], byteData3[6], byteData3[7]};
  SerialUSB.write(buf, 24);
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
  delay(5);
  SPI.endTransaction();

  // shift the high byte left 8 bits and merge the high and low
  int16_t xVal = (xL | (xH << 8));
  int16_t yVal = (yL | (yH << 8));
  int16_t zVal = (zL | (zH << 8));

x = xVal * SCALE;
y = yVal * SCALE;
z = zVal * SCALE;
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

  delay(100);

}
