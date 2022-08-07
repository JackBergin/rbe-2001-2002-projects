// This example reads the raw values from the LSM6DS33
// accelerometer and gyro and prints those raw values to the
// serial monitor.

#include <Romi32U4.h>
#include "IMU.h"

Romi32U4ButtonA buttonA;
Romi32U4ButtonB buttonB;
Romi32U4ButtonC buttonC;
Romi32U4Motors motors;

IMU IMU_sensor imu;

void setup()
{
  Wire.begin();
}

void loop()
{
  imu.PrintAcceleration();
}
