#include <SoftwareSerial.h>
#include <Arduino.h>

SoftwareSerial BTHC08Serial(0, 1); //RX | TX

void setup()
{
  Serial.begin(9600);
  Serial.println("Enter AT Commands");
  BTHC08Serial.begin(9600);
}

void loop()
{
  if (BTHC08Serial.available())
  {
    Serial.write(BTHC08Serial.read());
  }
  if (Serial.available())
  {
    BTHC08Serial.write(Serial.read());
  }
}
