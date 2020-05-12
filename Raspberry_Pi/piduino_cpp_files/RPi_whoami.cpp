#include <Piduino.h>
#include <SPI.h>

#define SS 10 // Serial Select -> CS on IIS3DHH
#define MOSI 12 // MasterOutSlaveIn -> SDI
#define MISO 13 // MasterInSlaveOut -> SDO
#define SCK 14 // Serial Clock -> SPC on IIS3DHH
#define SCALE 0.076294;

float   xAcc, yAcc, zAcc;
int xVal, yVal, zVal;
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

byte AddressByte_WHO = 0x0F; 
byte readBit = B10000000; 
byte whoByte = AddressByte_WHO | readBit;

SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE3));
digitalWrite(SS, LOW);
delay(10);
SPI.transfer(whoByte); 
byte who = SPI.transfer(0x0); 
delay(10);
digitalWrite(SS, HIGH);
SPI.endTransaction(); 
Console.println("WHO_AM_I read request:");
Console.println(whoByte);
Console.println("WHO_AM_I response:");
Console.println(who);

delay(5000);
}