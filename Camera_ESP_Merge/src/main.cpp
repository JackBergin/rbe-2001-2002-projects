/**
 * When connecting a device to the client, 
 * first go into the serial monitor of arduino. 
 * From there enter in the WIFI of the client 
 * device and the password. This should 
 * connect the two devices and is only needed 
 * to be done once because the devices 
 * connected remember the password afterwards.
 */

#include <Preferences.h>
#include <WiFi.h>
#include <Esp32WifiManager.h>
#include "UDPSimplePacket.h"
#include "visionTranslator.h"

//Create a wifi manager
WifiManager manager;
// modify the preferences
UDPSimplePacketComs *sendData = NULL;
#define numberOfBytes 16

uint8_t buffer[numberOfBytes];
static IPAddress broadcast;

void setup()
{
  //manager.erase();// This will erase the stored passwords
  manager.setupAP();
  Serial.begin(115200);
}

void bufferDef()
{
  //Each of these can have refreshed values sent to them by
  //the camera to update x, y, and angle

  visionTranslator robot_1();
  AprilTagDatum tag1;
  tag1.id = 0;

  visionTranslator robot_2();
  AprilTagDatum tag2;
  tag2.id = 1;

  visionTranslator robot_3();
  AprilTagDatum tag3;

  visionTranslator ramp();
  AprilTagDatum tag4;

  AprilTagDatum robotOneTag;
  AprilTagDatum robotTwoTag;
  AprilTagDatum robotThreeTag;
  AprilTagDatum robotFourTag;

  //This will allow for the proper bot to have the proper tag for data transfer
  if (tag1.id == 1)
  {
    robotOneTag = tag1;
  }
  if (tag1.id == 2)
  {
    robotTwoTag = tag1;
  }
  if (tag1.id == 3)
  {
    robotThreeTag = tag1;
  }
  if (tag1.id == 4)
  {
    robotFourTag = tag1;
  }

  if (tag2.id == 1)
  {
    robotOneTag = tag2;
  }
  if (tag2.id == 2)
  {
    robotTwoTag = tag2;
  }
  if (tag2.id == 3)
  {
    robotThreeTag = tag2;
  }
  if (tag2.id == 4)
  {
    robotFourTag = tag2;
  }

  if (tag3.id == 1)
  {
    robotOneTag = tag3;
  }
  if (tag3.id == 2)
  {
    robotTwoTag = tag3;
  }
  if (tag3.id == 3)
  {
    robotThreeTag = tag3;
  }
  if (tag3.id == 4)
  {
    robotFourTag = tag3;
  }

  if (tag4.id == 1)
  {
    robotOneTag = tag4;
  }
  if (tag4.id == 2)
  {
    robotTwoTag = tag4;
  }
  if (tag4.id == 3)
  {
    robotThreeTag = tag4;
  }
  if (tag4.id == 4)
  {
    robotFourTag = tag4;
  }

  //This sets up the x, y, theta, and status values that will be transmitted from camera to 32U4
  int romi_1_X = robot_1().getPose(robotOneTag).getX() * 128 / 50;
  int romi_1_Y = robot_1().getPose(tag1).getY() * 128 / 50;
  int romi_1_Theta = robot_1().getPose(tag1).getTheta() * 128 / (2 * PI);
  int romi_1_Status = robot_1().getPose(tag1).getStatus();

  int romi_2_X = robot_2().getPose(robotTwoTag).getX() * 128 / 50;
  int romi_2_Y = robot_2().getPose(tag2).getY() * 128 / 50;
  int romi_2_Theta = robot_2().getPose(tag2).getTheta() * 128 / (2 * PI);
  int romi_2_Status = robot_2().getPose(tag2).getStatus();

  int romi_3_X = robot_3().getPose(robotThreeTag).getX() * 128 / 50;
  int romi_3_Y = robot_3().getPose(tag3).getY() * 128 / 50;
  int romi_3_Theta = robot_3().getPose(tag3).getTheta() * 128 / (2 * PI);
  int romi_3_Status = robot_3().getPose(tag3).getStatus();

  int ramp_X = robot_3().getPose(robotFourTag).getX() * 128 / 50;
  int ramp_Y = robot_3().getPose(tag3).getY() * 128 / 50;
  int ramp_Theta = robot_3().getPose(tag3).getTheta() * 128 / (2 * PI);
  int ramp_Status = robot_3().getPose(tag3).getStatus();

  //Sets all of the values being shot from esp32 on camera to esp32 on romis
  buffer[0] = romi_1_X;
  buffer[1] = romi_1_Y;
  buffer[2] = romi_1_Theta;
  buffer[3] = romi_1_Status;

  buffer[4] = romi_2_X;
  buffer[5] = romi_2_Y;
  buffer[6] = romi_2_Theta;
  buffer[7] = romi_2_Status;

  buffer[8] = romi_3_X;
  buffer[9] = romi_3_Y;
  buffer[10] = romi_3_Theta;
  buffer[11] = romi_3_Status;

  buffer[12] = ramp_X;
  buffer[13] = ramp_Y;
  buffer[14] = ramp_Theta;
  buffer[15] = ramp_Status;
}

void loop()
{
  // read the serial port for new passwords and maintain connections
  manager.loop();
  bufferDef();
  if (manager.getState() == Connected)
  {
    if (sendData == NULL)
    {
      broadcast = WiFi.softAPIP();
      broadcast[3] = 255;
      sendData = new UDPSimplePacketComs(&broadcast, true);
    }

    sendData->write(buffer, numberOfBytes);
  }

  else
    Serial.println("I am the camera, I have no robots to talk to!");

  delay(2000);
}
