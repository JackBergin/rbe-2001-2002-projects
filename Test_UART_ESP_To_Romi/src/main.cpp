#include <Arduino.h>

#define numberOfBytes 17
uint8_t buffer[numberOfBytes];

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  for (int i = 0; i < 17; i++)
  {
    buffer[i] = Serial.read();
    Serial.println(buffer[i]);
  }
  delay(100);
  Serial.println("-------------------------");
}
