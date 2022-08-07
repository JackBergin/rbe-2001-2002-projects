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

//Create a wifi manager
WifiManager manager;
// modify the preferences
UDPSimplePacketComs *sendData = NULL;
#define numberOfBytes 12

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
  int romi_1_X = 1000;
  int romi_1_Y = 1000;
  int romi_1_Theta = 1000;
  int romi_1_Status = 1;

  int romi_2_X = 1000;
  int romi_2_Y = 1000;
  int romi_2_Theta = 1000;
  int romi_2_Status = 1;

  int romi_3_X = 1000;
  int romi_3_Y = 1000;
  int romi_3_Theta = 1000;
  int romi_3_Status = 1;

  buffer[0] = romi_1_X;      //Identifier for bot 1
  buffer[1] = romi_1_Y;      //X Loc
  buffer[2] = romi_1_Theta;  //Y loc
  buffer[3] = romi_1_Status; //Identifier for bot 2

  buffer[4] = romi_2_X;      //X Loc
  buffer[5] = romi_2_Y;      //Y Loc
  buffer[6] = romi_2_Theta;  //Identifier for bot 3
  buffer[7] = romi_2_Status; //X Loc

  buffer[8] = romi_3_X;       //Y Loc
  buffer[9] = romi_3_Y;       //Y Loc
  buffer[10] = romi_3_Theta;  //Y Loc
  buffer[11] = romi_3_Status; //Y Loc
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
