#include <Arduino.h>
#include <apriltags.h>
#include <Wire.h>
#include <Timer.h>

const int camResX = 160;
const int camResY = 120;
int tagX;
int tagY;
uint8_t tagCount;

Timer driveUpdateTimer(10);
// Chassis chassis;
void setup()
{
  // chassis.Init();
  Serial.begin(115200);
  delay(1000);

  Wire.begin();
  Wire.setClock(100000ul); // Wire should default to 100kHz, but let's be explicit

  Serial.println(F("Welcome."));
}

uint8_t FindAprilTags()
{
  tagCount = getTagCount();
  if (tagCount)
  {
    Serial.println(tagCount);
    AprilTagDatum tag;
    if (readTag(&tag))
    {
      Serial.print(F("Tag cx="));
      Serial.print(tag.cx);
      Serial.print(F(", cy="));
      Serial.print(tag.cy);
      Serial.print(F(", w="));
      Serial.print(tag.w);
      Serial.print(F(", h="));
      Serial.print(tag.h);
      Serial.print(F(", id="));
      Serial.print(tag.id);
      Serial.print(F(", rot="));
      Serial.print(tag.rot);
      Serial.println(F("]"));
      tagX = tag.cx;
      tagY = tag.cy;
    }
  }

  return tagCount;
}

void loop()
{
  delay(1);
  FindAprilTags();
  float rel_x = tagX - camResX / 2;
  float rel_y = tagY - camResY / 2;
  float fracX = rel_x / (camResX / 2);
  float fracY = rel_y / (camResY / 2);
}
