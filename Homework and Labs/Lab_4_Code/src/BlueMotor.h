#include <Romi32U4.h>
#include <Arduino.h>

#pragma once

class BlueMotor {
    public:
        BlueMotor();
        void setEffort(int effort);
        void setEffortWithoutDB(int effort);
        float moveTo(float longPosition);
        float getPosition();
        void reset();
        void setup();
        
    private:
        void setEffort(int effort, bool clockwise);
        void pwmSetup();
        static void isr();
        void encoderInterrupt();

        const int tolerance = 3;
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
