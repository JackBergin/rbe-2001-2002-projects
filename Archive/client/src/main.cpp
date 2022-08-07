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
#define numberOfBytes 9

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
  buffer[0] = 1; //Identifier for bot 1
  buffer[1] = 0; //X Loc
  buffer[2] = 0; //Y loc
  buffer[3] = 2; //Identifier for bot 2
  buffer[4] = 1; //X Loc
  buffer[5] = 1; //Y Loc
  buffer[6] = 3; //Identifier for bot 3
  buffer[7] = 2; //X Loc
  buffer[8] = 2; //Y Loc
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
