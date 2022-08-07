#include <Romi32U4.h>

class Chassis
{
    public:
        //dribeDistance method creation
        void driveDistance(float inches);
        //turnAngle method creation
        void turnAngle(float angle);
        
        //Needed constant values
        const float wheelDiameter = 2.8;
        const int CPR = 1440;
        const float wheelTrack = 5.75;

    private: 
        Romi32U4Motors motors;
        Romi32U4Encoders encoders;
};