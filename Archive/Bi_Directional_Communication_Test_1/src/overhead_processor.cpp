#include <Arduino.h>
#include <apriltags.h>
#include <Wire.h>
#include <Timer.h>
#include <Preferences.h>
#include <WiFi.h>
#include <Esp32WifiManager.h>
#include "UDPSimplePacket.h"
#include "visionTranslator.h"

const int cueTimeout = 10000;
Timer cueTimer(cueTimeout);
const int transmitTime = 100;
Timer transmitTimer(transmitTime);
int cue = 0;

//Wifi com setup
WifiManager manager;
UDPSimplePacketComs *sendData = NULL;

#define numberOfBytes 17
uint8_t buffer[numberOfBytes];
#define numberOfBytes_Recieving 1
uint8_t testSend[numberOfBytes_Recieving];

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

  Wire.begin(21, 22);
  Wire.setClock(100000ul); // Wire should default to 100kHz, but let's be explicit

  Serial.println(F("Welcome."));
}

void FindAprilTags()
{
  buffer[16] = 0;
  tagCount = getTagCount();
  Serial.println("Found tags " + String(tagCount));
  if (tagCount == 0x55)
  {
    Serial.println("TAG FAULT!! " + String(tagCount));
    return;
  }
  for (int i = 0; i < tagCount; i++)
  {
    AprilTagDatum tag;
    if (readTag(&tag))
    {
      Serial.println("Waiting for if identity access for buffer definition");
      //if it doesn't enter this logic may be related to timing prevention from the wifi class or
      if (tag.id == 0)
      {
        int romi_1_X = accessToPose.getPose(tag).getX() * 128 / 50; //-50-50 cm output * 128/50, is the output in decimal format?
        int romi_1_Y = accessToPose.getPose(tag).getY() * 128 / 50;
        int romi_1_Theta = accessToPose.getPose(tag).getTheta() * 128 / (2 * PI);
        int romi_1_Status = accessToPose.getPose(tag).getStatus();

        buffer[0] = romi_1_X;
        buffer[1] = romi_1_Y;
        buffer[2] = romi_1_Theta;
        buffer[3] = romi_1_Status;
        Serial.println("0 - This is the output of the romi_1_X as test for method access and id recognition: " + romi_1_X);
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
        //This should also see if conversion facor is in the 0-255 bound and not in decimal form, maybe ask Kevin about converting to float
        Serial.println("1 - This is the output of the romi_2_X as test for method access and id recognition: " + romi_2_X);
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
        Serial.println("2 - This is the output of the romi_3_X as test for method access and id recognition: " + romi_3_X);
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
        Serial.println("3 - This is the output of the ramp_X as test for method access and id recognition: " + ramp_X);
      }
      buffer[16] = 123;
    }
  }
}
void loop()
{

  if (transmitTimer.isExpired())
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

      if (sendData->isResponseReady())
      {
        //Recieves the data that was sent

        uint8_t storeValue = sendData->read(testSend);
        Serial.print("Data was recieved: ");
        Serial.println(testSend[0]);
        Serial.print("Bytes recieved: ");
        Serial.println(storeValue);
      }

      //Check if this is being accessed properly in the code
      FindAprilTags();

      sendData->write(buffer, numberOfBytes);
      delay(1);
      //This is the send data being utilized in test

      delay(100);
    }
    else
      Serial.println("I am the camera, I have no robots to talk to!");
  }

  if (cueTimer.isExpired())
  {
    cue++;
  }
  delay(1);
}
