#include <Piduino.h>
#include <SPI.h>

#define SS 10 // Serial Select -> CS on IIS3DHH
#define MOSI 12 // MasterOutSlaveIn -> SDI
#define MISO 13 // MasterInSlaveOut -> SDO
#define SCK 14 // Serial Clock -> SPC on IIS3DHH
#define SCALE 0.076294;

float xAcc, yAcc, zAcc;
int16_t xVal, yVal, zVal;
byte addressByte, ctrlRegByte;

void setup() {

pinMode(SS, OUTPUT);
digitalWrite(SS,HIGH);
SPI.begin();

delay(1000);

SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
digitalWrite(SS, LOW);
delay(10);
SPI.transfer(0x20);
SPI.transfer(0xC0);
delay(10);
digitalWrite(SS, HIGH);
SPI.endTransaction();

delay(100);

SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
digitalWrite(SS, LOW);
delay(10);
SPI.transfer(0x23);
SPI.transfer(0x00);
delay(10);
digitalWrite(SS, HIGH);
SPI.endTransaction();

delay(1000);

}
void loop() {

byte xAddressByteL = 0x28; // Low Byte of X value (the first data register)
byte readBit = B10000000; // bit 0 (MSB) HIGH means read register, 
byte dataByte = xAddressByteL | readBit;
byte b0 = 0x0; // an empty byte to initiate read register
//Console.println("read request:");
//Console.println(dataByte);

SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
digitalWrite(SS, LOW); // SS must be LOW to communicate
delay(10);
SPI.transfer(dataByte); // request a read, starting at X low byte
byte xL = SPI.transfer(b0); // get the low byte of X data
byte xH = SPI.transfer(b0); // get the high byte of X data
byte yL = SPI.transfer(b0); // get the low byte of Y data
byte yH = SPI.transfer(b0); // get the high byte of Y data
byte zL = SPI.transfer(b0); // get the low byte of Z data
byte zH = SPI.transfer(b0); // get the high byte of Z data
delay(10);
digitalWrite(SS, HIGH);
SPI.endTransaction();

// shift the high byte left 8 bits and merge the high and low
int16_t xVal = (xL | (xH << 8));
int16_t yVal = (yL | (yH << 8));
int16_t zVal = (zL | (zH << 8));

// scale the values into mG
xAcc = xVal * SCALE;
yAcc = yVal * SCALE;
zAcc = zVal * SCALE;

//Console.print(interval);
//Console.print(",");
Console.print(xAcc, 0);
Console.print(",");
Console.print(yAcc, 0);
Console.print(",");
Console.println(zAcc, 0);

delay(10); // Adjust for sampling rate (e.g. 10mS = 100Hz. 2mS = 500Hz etc.)
}
