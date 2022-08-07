#include <Arduino.h>
#include "IMU.h"
#include <Romi32U4.h>
#include <LSM6.h>
#include "Complementary_Filter.h"

LSM6 imu;

//median filter
MedianFilter med_x;
MedianFilter med_y;
MedianFilter med_z;

MedianFilter med_gx;
MedianFilter med_gy;
MedianFilter med_gz;

void IMU_sensor::Init(void)
{
    Wire.begin();
    if (!imu.init())
    {
        while (1)
        {
            Serial.println("Failed to detect the LSM6.");
            delay(100);
        }
    }
    imu.setFullScaleAcc(imu.ACC_FS2);
    imu.setFullScaleGyro(imu.GYRO_FS245);
    imu.enableDefault();

    med_x.Init();
    med_y.Init();
    med_z.Init();

    med_gx.Init();
    med_gy.Init();
    med_gz.Init();
}

IMU_sensor::acceleration_data IMU_sensor::ReadAcceleration(void)
{
    imu.read();
    return {(med_x.Filter((int)(imu.a.x * 0.061))), (med_y.Filter((int)(imu.a.y * 0.061))), (med_z.Filter((int)(imu.a.z * 0.061)))};
}

IMU_sensor::acceleration_data IMU_sensor::ReadGyro(void)
{
    imu.read();
    return {(med_gx.Filter((int)imu.dps.x)), (med_gy.Filter((int)imu.dps.y)), (med_gz.Filter((int)imu.dps.z))};
}

void IMU_sensor::PrintAcceleration(void)
{
    IMU_sensor::acceleration_data data = ReadAcceleration();
    snprintf_P(report, sizeof(report), ("A: %10d %10d %10d"), data.X, data.Y, data.Z);
    Serial.println(report);
}

void IMU_sensor::PrintGyro(void)
{
    IMU_sensor::acceleration_data data = ReadGyro();
    snprintf_P(report, sizeof(report), PSTR("G: %10d %10d %10d"), data.X, data.Y, data.Z);
    Serial.println(report);
}