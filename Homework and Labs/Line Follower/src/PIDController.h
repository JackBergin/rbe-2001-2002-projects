#include <Romi32U4.h>
#include <Arduino.h>

#pragma once

class PIDController {
    public:
        PIDController();
        PIDController(float p, float i, float d);
        void setPID(float p, float i, float d);
        void setSetpoint(float targetValue);
        void setTolerance(float t);
        float calculateEffort(float inputData);
        bool onTarget(float inputData);
        float getGainValue(int index);
        float getSetpoint();
    private:
        float setpoint = 0.0;
        float errorSum = 0.0;
        float previousError = 0.0;
        float tolerance = 0.0;
        float Kp = 1.0;
        float Ki = 0.0;
        float Kd = 0.0; 
};