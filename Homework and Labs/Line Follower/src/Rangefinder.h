#include <Arduino.h>

class Rangefinder
{
    public:
    //Setup for the ultrasonic sensor
    void ultrasonicTo(float desDistance);     
    void ultrasonicFrom(float desDistance);  
    void ultrasonicDistanceTurn(float desDistance);   
};