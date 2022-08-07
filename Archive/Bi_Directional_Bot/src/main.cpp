#include <Preferences.h>
#include <WiFi.h>
#include <Esp32WifiManager.h>
#include "UDPSimplePacket.h"

//Create a wifi manager
WifiManager manager;
UDPSimplePacket *recieveData = NULL;
static IPAddress broadcast;

#define numberOfBytes 17
uint8_t buffer[numberOfBytes];

uint8_t testSend[1];

void setup()
{
  Serial.begin(115200);
  // Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
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
    //Recieves the data that was sent
    recieveData->getPacket(buffer, numberOfBytes);

    for (int i = 0; i < numberOfBytes; i++)
    {
      Serial.write(buffer[i]);
      Serial.print("Buffer Values: ");
      Serial.println(buffer[i]);
    }
    testSend[0] = 123;
    recieveData->sendPacket(testSend, 1);
  }
  //The following code primes the send of the data
}
