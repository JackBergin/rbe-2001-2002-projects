//Libraries for the code
#include <Arduino.h>
#include <Romi32U4Encoders.h>
#include "Chassis.h"
#include "Rangefinder.h"

//Sets up the motors and encoders from the above libraries
Romi32U4Motors motors;
Romi32U4Encoders encoder;

//Creates a new chassis class object executed in loop
Chassis a = Chassis();

//Creates a new rangefinder class object executed in loop
Rangefinder b = Rangefinder();

//Pin values for the ultrasonic sensor
static const int triggerPin = 12;
static const int echoPin = 0;

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
}

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
}

void Rangefinder::ultrasonicDistanceTurn(float desDistance)
{
    // trigger the ping
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(100);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(100);
    digitalWrite(triggerPin, LOW);

    //Calculations of the ultrasonic distance
    long duration = pulseIn(echoPin, HIGH);
    float distance = duration * 0.034 / 2; // Speed of sound wave divided by 2
    
    if(distance > desDistance){
       motors.setEfforts(100,100);
       Serial.print("DRIVE, Ultrasonic Distance: ");
       Serial.println(distance);
       delay(10);
    }
    //if the distance is less than or equal to the desired distance: inches, the bot will stop
    if(distance <= desDistance){
       a.turnAngle(360);
       Serial.print("TURN, Ultrasonic Distance: ");
       Serial.println(distance);
       delay(10);
    }
}
void setup()
{
    //Sets the motor pins to output voltage
    pinMode(13, OUTPUT);
    //Sets the ultrasonic pins to have an output and i
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
    Serial.begin(9600);
}
void loop()
{
    Serial.println("In the loop");
    b.ultrasonicDistanceTurn(20);
    delay(200);
}