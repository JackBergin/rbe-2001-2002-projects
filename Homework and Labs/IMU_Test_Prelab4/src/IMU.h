#ifndef IMU
#define IMU

#include <Romi32U4.h>

class IMU_sensor
{
private:
    int data[3] = {0};
    char report[120];
    const int threshold = 300; //collision threshold for 50-100mm/s (may need retesting)
    const int crossover = -30; //ramp crossover point in y term gyro, as it hits the ground at the end of the ramp

public:
    struct acceleration_data
    {
        int X;
        int Y;
        int Z;
    };
    void Init(void);
    void PrintAcceleration(void);
    void PrintGyro(void);
    acceleration_data ReadAcceleration(void);
    acceleration_data ReadGyro(void);
};

#endif
