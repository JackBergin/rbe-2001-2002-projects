// This example reads the raw values from the LSM6DS33
// accelerometer and gyro and prints those raw values to the
// serial monitor.
#include <Romi32U4.h>
#include "LSM6Corrected.h"

Romi32U4ButtonA buttonA;
Romi32U4ButtonB buttonB;
Romi32U4ButtonC buttonC;
Romi32U4Motors motors;

LSM6Corrected imu;

void setup()
{
  imu.init();
}

bool showAcc = true;
bool showGyro = false;

void loop()
{
  if (buttonA.isPressed())
    motors.setEfforts(200, 200);
  else
    motors.setEfforts(0, 0);

  if (buttonB.getSingleDebouncedPress())
    showAcc = !showAcc;
  if (buttonC.getSingleDebouncedPress())
    showGyro = !showGyro;

  if (imu.getStatus() & 0x01)
  {
    imu.read();

    Serial.print(millis());
    Serial.print('\t');

    if (showAcc)
    {
      Serial.print(imu.aX());
      Serial.print(' ');
      Serial.print(imu.aY());
      Serial.print(' ');
      Serial.print(imu.aZ());
      Serial.print(' ');
    }

    if (showGyro)
    {
      Serial.print(imu.gXRaw());
      Serial.print(' ');
      Serial.print(imu.gYRaw());
      Serial.print(' ');
      Serial.print(imu.gZRaw());
      Serial.print(' ');
    }

    Serial.print('\n');
  }
}
