#include <Preferences.h>
#include <WiFi.h>
#include <Esp32WifiManager.h>
#include "UDPSimplePacket.h"

//Create a wifi manager
WifiManager manager;
UDPSimplePacket *recieveData = NULL;

#define numberOfBytes 16
uint8_t buffer[numberOfBytes];

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting core");
  manager.setup(); // Don't want to go into ap mode. Already taken by client, we just want to connect
  recieveData = new UDPSimplePacket();
}

void loop()
{
  delay(20);
  manager.loop();
  if (manager.getState() != Connected)
  {
    return;
  }

  if (recieveData->isPacketAvailible())
  {
    Serial.println("\n\nGot Packet!!");
    recieveData->getPacket(buffer, numberOfBytes);

    for (int i = 0; i < numberOfBytes; i++)
    {
      Serial.println("Data byte " + String(i) + " value = " + String(buffer[i]));
    }
    //Serial.write(buffer);
  }
}

/*
#include <Preferences.h>
#include <WiFi.h>
#include <Esp32WifiManager.h>
#include "UDPSimplePacket.h"

//Create a wifi manager
WifiManager manager;
UDPSimplePacket *recieveData = NULL;

#define numberOfBytes 9
uint8_t buffer[numberOfBytes];

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting core");
  manager.setup(); // Don't want to go into ap mode. Already taken by client, we just want to connect
  recieveData = new UDPSimplePacket();
}

void loop()
{
  delay(20);
  manager.loop();
  if (manager.getState() != Connected)
  {

    return;
  }
  //

  if (recieveData->isPacketAvailible())
  {
    Serial.println("\n\nGot Packet!!");
    recieveData->getPacket(buffer, numberOfBytes);

    for (int i = 0; i < numberOfBytes; i++)
    {
      Serial.println("Data byte " + String(i) + " value = " + String(buffer[i]));
    }
  }
}
*/