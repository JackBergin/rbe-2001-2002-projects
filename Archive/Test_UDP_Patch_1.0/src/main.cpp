#include <Preferences.h>
#include <WiFi.h>
#include <Esp32WifiManager.h>
#include "UDPSimplePacket.h"
#include <Timer.h>
//Create a wifi manager

//This acts as the initializer
WifiManager manager;
UDPSimplePacket *recieveData = NULL;
static IPAddress broadcast;

#define numberOfBytes 17
uint8_t buffer[numberOfBytes];

int progressPin = 4;

#define numberOfBytes_Recieving 1
uint8_t testSend[numberOfBytes_Recieving];
// Timer progressSendTimer = Timer(50);

void setup()
{
  Serial.begin(115200);
  // Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  Serial.println("Starting core");
  manager.setup(); // Don't want to go into ap mode. Already taken by client, we just want to connect
  recieveData = new UDPSimplePacket();
  pinMode(progressPin, INPUT);
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
    int cue = buffer[16];
    // Serial.println(cue);
    for (int i = 0; i < numberOfBytes; i++)
    {
      Serial.write((int)buffer[i]);
    }
  }
}
