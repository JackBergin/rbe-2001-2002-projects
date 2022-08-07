#include <Arduino.h>
#include <apriltags.h>
#include <Wire.h>
#include <Timer.h>
#include <Preferences.h>
#include <WiFi.h>
#include <Esp32WifiManager.h>
#include "UDPSimplePacket.h"
#include "visionTranslator.h"

WifiManager manager;
UDPSimplePacketComs *sendData = NULL;
#define numberOfBytes 16
uint8_t buffer[numberOfBytes];
static IPAddress broadcast;

visionTranslator accessToPose = visionTranslator();

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
  manager.setupAP();
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
      if (tag.id == 0)
      {
        int romi_1_X = accessToPose.getPose(tag).getX() * 128 / 50;
        int romi_1_Y = accessToPose.getPose(tag).getY() * 128 / 50;
        int romi_1_Theta = accessToPose.getPose(tag).getTheta() * 128 / (2 * PI);
        int romi_1_Status = accessToPose.getPose(tag).getStatus();

        buffer[0] = romi_1_X;
        buffer[1] = romi_1_Y;
        buffer[2] = romi_1_Theta;
        buffer[3] = romi_1_Status;
      }
      if (tag.id == 1)
      {
        int romi_2_X = accessToPose.getPose(tag).getX() * 128 / 50;
        int romi_2_Y = accessToPose.getPose(tag).getY() * 128 / 50;
        int romi_2_Theta = accessToPose.getPose(tag).getTheta() * 128 / (2 * PI);
        int romi_2_Status = accessToPose.getPose(tag).getStatus();

        buffer[4] = romi_2_X;
        buffer[5] = romi_2_Y;
        buffer[6] = romi_2_Theta;
        buffer[7] = romi_2_Status;
      }
      if (tag.id == 2)
      {
        int romi_3_X = accessToPose.getPose(tag).getX() * 128 / 50;
        int romi_3_Y = accessToPose.getPose(tag).getY() * 128 / 50;
        int romi_3_Theta = accessToPose.getPose(tag).getTheta() * 128 / (2 * PI);
        int romi_3_Status = accessToPose.getPose(tag).getStatus();

        buffer[8] = romi_3_X;
        buffer[9] = romi_3_Y;
        buffer[10] = romi_3_Theta;
        buffer[11] = romi_3_Status;
      }
      if (tag.id == 3)
      {
        int ramp_X = accessToPose.getPose(tag).getX() * 128 / 50;
        int ramp_Y = accessToPose.getPose(tag).getY() * 128 / 50;
        int ramp_Theta = accessToPose.getPose(tag).getTheta() * 128 / (2 * PI);
        int ramp_Status = accessToPose.getPose(tag).getStatus();

        buffer[12] = ramp_X;
        buffer[13] = ramp_Y;
        buffer[14] = ramp_Theta;
        buffer[15] = ramp_Status;
      }
    }
  }

  return tagCount;
}

void loop()
{
  // read the serial port for new passwords and maintain connections
  manager.loop();
  if (manager.getState() == Connected)
  {
    if (sendData == NULL)
    {
      broadcast = WiFi.softAPIP();
      broadcast[3] = 255;
      sendData = new UDPSimplePacketComs(&broadcast, true);
    }
    FindAprilTags();

    sendData->write(buffer, numberOfBytes);
    delay(1);
  }
  else
    Serial.println("I am the camera, I have no robots to talk to!");
}
