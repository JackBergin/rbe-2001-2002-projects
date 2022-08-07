#include <Romi32U4.h>
#include "PIDController.h"

#pragma once

class Chassis
{
public:
    Chassis();
    void drive(float effort);
    void driveDistance(float inches);
    void startUltraDrive(float inches, float ultraInput);
    void loopUltraPID(float ultraInput);
    bool pullOnTarget(float ultraInput);
    void turnAngle(float degrees);
    void startTurn(float degrees);
    bool turnComplete();
    void setup();

    const int SPEED_VAL = 100;       // Default driving speed for chassis commands
    const int CPR = 1440;            // Encoder count per revolution (adjusted for GR)
    const float wheelDiameter = 2.8; // Diameter in inches of chassis wheels
    const float wheelTrack = 5.75;   // Distance in inches between wheels from side to side

private:
    Romi32U4Motors motors;
    Romi32U4Encoders encoders;
    PIDController chassis_PID;

    const float tolerance = 0.3;
};