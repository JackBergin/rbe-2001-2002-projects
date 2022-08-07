#include <Romi32U4.h>
#include <Arduino.h>
#include "PIDController.h"

#pragma once

class BlueMotor {
    public:
        BlueMotor();
        void setEffort(int effort);
        void setEffortWithoutDB(int effort);
        void moveTo(float longPosition);
        void startMoveTo(float position);
        void loopController();
        float getPosition();
        void reset();
        void setup();
        float pullSetpoint();
        float pullGain(int index);
        bool pullOnTarget();

        int applEff = 0;
        
    private:
        PIDController bm_PID;
        void setEffort(int effort, bool clockwise);
        void pwmSetup();
        static void isr();
        void encoderInterrupt();

        const int encoderCountPerRev = 540;
        const float tolerance = 5.0;
        
        const int PWMOutPin = 11;
        const int AIN2 = 4;
        const int AIN1 = 13;
        const int ENCA = 2;
        const int ENCB = 3;   

        int newValue = 0;
        int oldValue = 0;
        long count = 0;
        long errorCount = 0;
        
        const char X = 5;
        char encoderArray[4][4] = {
            {0, -1, 1, X},
            {1, 0, X, -1},
            {-1, X, 0, 1},
            {X, 1, -1, 0}
        };
};