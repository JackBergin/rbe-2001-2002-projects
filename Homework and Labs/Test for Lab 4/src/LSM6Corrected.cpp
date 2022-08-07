#include "LSM6Corrected.h"
void LSM6Corrected::init(){
    Wire.begin();

  if (!IMU.init())
  {
    // Failed to detect the LSM6.
    // ledRed(1);
    while(1)
    {
      Serial.println(F("Failed to detect the LSM6."));
      delay(100);
    }
  }

  IMU.enableDefault();

  // Set the gyro full scale and data rate
  IMU.setGyroDataOutputRate(LSM6::ODR13);

  // Set the accelerometer full scale and data rate
  IMU.setAccDataOutputRate(LSM6::ODR13);
}

uint8_t LSM6Corrected::getStatus(){
    return IMU.getStatus();
}

void LSM6Corrected::read(){
    IMU.read();
}

float LSM6Corrected::aXRaw(){
    return IMU.a.x + aXOffset;
}
float LSM6Corrected::aYRaw(){
    return IMU.a.y + aYOffset;
}
float LSM6Corrected::aZRaw(){
    return IMU.a.z + aZOffset;
}

float LSM6Corrected::gXRaw(){
    return IMU.g.x + gXOffset;
}
float LSM6Corrected::gYRaw(){
    return IMU.g.y + gYOffset;
}
float LSM6Corrected::gZRaw(){
    return IMU.g.z + gZOffset;
}

float LSM6Corrected::aX(){
    return aXRaw() * readToGFactor;
}
float LSM6Corrected::aY(){
    return aYRaw() * readToGFactor;
}
float LSM6Corrected::aZ(){
    return aZRaw() * readToGFactor;
}