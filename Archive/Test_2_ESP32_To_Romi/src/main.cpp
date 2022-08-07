#include <Arduino.h>

int buffer;

void setup()
{
  Serial.begin(115200);
}

void loop()
{
  buffer = 12;
  Serial.write(buffer);
}
