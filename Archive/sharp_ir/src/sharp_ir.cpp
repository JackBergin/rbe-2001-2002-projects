#include <Arduino.h>
uint32_t INTERVAL = 20;

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    uint32_t lastTime;
    uint32_t currentTime = millis();
    if ((currentTime - lastTime) >= INTERVAL)
    {
        double sensorValue = analogRead(A0);

        double voltageFromAnalog = sensorValue / 1024 * 5;

        Serial.print(sensorValue);
        Serial.print('\t');
        Serial.print(voltageFromAnalog);
        Serial.println('\t');
        lastTime = currentTime;
    }
}