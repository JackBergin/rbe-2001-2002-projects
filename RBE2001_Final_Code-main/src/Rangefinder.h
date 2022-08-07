#include <Arduino.h>
#include <Romi32U4.h>

#pragma once

class Rangefinder {
    public:
        void setup();
        void loop();
        float getDistanceCM();
        float getDistance();
    private:
        float distanceOutput = 0;
};