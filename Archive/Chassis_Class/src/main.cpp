//Libraries for the code
#include <Arduino.h>
#include <Romi32U4Encoders.h>
#include "Chassis.h"

//Sets up the motors and encoders from the above libraries
Romi32U4Motors motors;
Romi32U4Encoders encoder;

//Creates a new chassis class object
Chassis a = Chassis();

//Chassis drive method definition
void Chassis::driveDistance(float inches)
{
    //Resets the encoders when different distances want to be driven
    encoders.getCountsAndResetRight();
    encoders.getCountsAndResetLeft();
    
    //This is the comparer for the encoder values
    float encoderComparer = inches *(Chassis::CPR / (Chassis::wheelDiameter * 3.14159265));
    
    //Logic for driving a specific distance
    if((abs(encoder.getCountsLeft()) < encoderComparer) && (abs(encoder.getCountsRight()) < encoderComparer))
    {
        //Turns on motors and operates at 100%
        motors.setEfforts(100,100);
    }
    //Stops motors
    motors.setEfforts(0,0);
}

//Chassis angle method definition
void Chassis::turnAngle(float degrees)
{     
    //Resets the encoders when different distances want to be driven
    encoders.getCountsAndResetRight();
    encoders.getCountsAndResetLeft();

    //This is the comparer for the encoder values
    float encoderComparer = ((degrees / 360) * Chassis::wheelTrack) * (Chassis::CPR / (Chassis::wheelDiameter));
    
    //Logic for turning a specific angle
    if((abs(encoder.getCountsLeft()) < encoderComparer) && (abs(encoder.getCountsRight()) < encoderComparer))
    {
        //Turns on motors and operates at 100% in opposite directions
        motors.setEfforts(100,-100);
    }
    //Stops motors
    motors.setEfforts(0,0);
}
void setup()
{
    //Sets the motor pins to output voltage
    pinMode(13, OUTPUT);
}
void loop()
{
    //declares the drive distance method from the object a
    a.driveDistance(20);
    //declares the drive angle method for the object a
    a.turnAngle(360);
    //Loops this process again until complete
    while(1==1);
}