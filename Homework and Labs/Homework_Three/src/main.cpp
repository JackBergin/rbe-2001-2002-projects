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

void Rangefinder::ultrasonicDistanceTo(float inches)
{
    // trigger the ping
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(10);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    //Calculations of the ultrasonic distance
    long duration = pulseIn(echoPin, HIGH);
    float distance = duration * 0.034 / 2; // Speed of sound wave divided by 2
    
    //if the distance is greater than desired distance: inches then it will drive the bot
    if(distance > inches){
       motors.setEfforts(100,100);
       Serial.print("HIGH, Ultrasonic Distance: ");
       Serial.println(distance);
       delay(10);
    }
    //if the distance is less than or equal to the desired distance: inches, the bot will stop
    if(distance <= inches){
       motors.setEfforts(0,0);
       Serial.print("LOW, Ultrasonic Distance: ");
       Serial.println(distance);
       delay(10);
    }
}

//Rangefinder distance calculation
void Rangefinder::ultrasonicDistanceFrom(float inches)
{
    // trigger the ping
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(10);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    //Calculations of the ultrasonic distance
    long duration = pulseIn(echoPin, HIGH);
    float distance = duration * 0.034 / 2; // Speed of sound wave divided by 2
    
    //if the distance is less than the desired distance: inches then it will drive the bot
    if(distance < inches){
       motors.setEfforts(100,100);
       Serial.println("HIGH");
    }
    //if the distance is equal or greater than the desired distance: inches, the bot will stop
    if(distance >= inches){
       motors.setEfforts(0,0);
       Serial.print("LOW");
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
    a.driveDistance(30);
    //Drives to an object over a specified distance
    b.ultrasonicDistanceTo(12);
    //Drives from a object over a specified distance
    b.ultrasonicDistanceFrom(12);
    
    //Loops this process again until complete
    while(1);
}