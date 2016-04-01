/*
Example for working with AT93Cxx EEPROMs
You can find spi pins for your arduino in here:
URL: http://www.arduino.cc/en/Reference/SPI

Write by Hossein Boustani
*/

#include <SPI.h>
// Chip select pin
#define SS  53

void setup() {
  pinMode(SS, OUTPUT);
  Serial.begin(9600);
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV128);
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  digitalWrite(SS, LOW);
}

void loop() {
  EWEN();
  ERAL();
  WRITE(2, 0x1D);
  byte buf;
  for(byte i = 0x00; i < 0x80; i++)
  {
    buf = READ(i);
    Serial.print(i, DEC);
    Serial.print(" : ");
    Serial.println(buf, HEX);
    delay(200);
  }
}

// Write enable must precede all programming modes
bool EWEN()
{
  digitalWrite(SS, HIGH);
  SPI.transfer(0x02);
  SPI.transfer(0x60);
  digitalWrite(SS, LOW);
  delay(10);
  return true;
}

// Disables all programming instructions
bool EWDS()
{
  digitalWrite(SS, HIGH);
  SPI.transfer(0x02);
  SPI.transfer(0x00);
  digitalWrite(SS, LOW);
  delay(10);
  return true;
}

// Writes memory location
bool WRITE(byte loc, byte data)
{
  if(loc < 0x80)
  {
    digitalWrite(SS, HIGH);
    SPI.transfer(0x02);
    SPI.transfer(0x80|loc);
    SPI.transfer(data);
    digitalWrite(SS, LOW);
    delay(100);
    return true;
  }
  else
    return false;
}

// Reads data stored in memory, at specified address
byte READ(byte loc)
{
  byte buf, buf1, buf2;
  if(loc < 0x80)
  {
    digitalWrite(SS, HIGH);
    SPI.transfer(0x03);
    SPI.transfer(loc);
    delay(5);
    buf1 = SPI.transfer(0x00);
    buf2 = SPI.transfer(0x00);
    digitalWrite(SS, LOW);
    buf1 = buf1 << 1;
    buf2 = buf2 >> 7;
    buf = buf1 | buf2;
  }
  return buf;
}

// Erase memory location
bool ERASE(byte loc)
{
  if(loc < 0x80)
  {
    digitalWrite(SS, HIGH);
    SPI.transfer(0x03);
    SPI.transfer(0x80|loc);
    digitalWrite(SS, LOW);
    delay(100);
    return true;
  }
  else
    return false;
}

// Erases all memory locations
bool ERAL()
{
  digitalWrite(SS, HIGH);
  SPI.transfer(0x02);
  SPI.transfer(0x40);
  digitalWrite(SS, LOW);
  delay(100);
  return true;
}
