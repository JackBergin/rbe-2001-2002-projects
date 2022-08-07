#include <LSM6.h>
#include <Arduino.h>
#include <Wire.h>
class LSM6Corrected{
    private:
    LSM6 IMU;
    const int aXOffset = 0;
    const int aYOffset = 0;
    const int aZOffset = 0;

    const int gXOffset = 0;
    const int gYOffset = 0;
    const int gZOffset = 0;

    const float readToGFactor = .061 / 1000;
    public:
    void init();
    uint8_t getStatus();
    void read();

    float aXRaw();
    float aYRaw();
    float aZRaw();

    float gXRaw();
    float gYRaw();
    float gZRaw();

    float aX();
    float aY();
    float aZ();

};