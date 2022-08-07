//Libraries for the code
#include <Arduino.h>
#include <Romi32U4Encoders.h>
#include "Chassis.h"
#include "Arm.h"

Romi32U4ButtonA pb;

//Sets up the motors and encoders from the above libraries
Romi32U4Motors motors;
Romi32U4Encoders encoder;

//Creates a new chassis class object executed in loop
Chassis a = Chassis();
Arm b = Arm();

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
////////////////////////////////////////////////////////////LAB 3 CODE////////////////////////////////////////////////////////////////////

void Arm::setEffort(int effort)
{
    digitalWrite(AIN1, HIGH);
    digitalWrite(AIN2, LOW);
    OCR1C = effort;
    delay(10);
    Serial.println(OCR1C);
}
/*
void Arm::getPosition()
{
   
}
*/
void pwmSetup() 
{ 
    TCCR1A = 0xA8; 
    TCCR1B = 0x11; 
    ICR1 = 400; 
    OCR1C = 0; 
}

void Arm::reset()
{
   OCR1C = 0;
}
void Arm::stop ()
{
    digitalWrite(PWMOutPin, LOW);
}

void Arm::setup()
{
   motors.setEfforts(0,0);
   pinMode(PWMOutPin,OUTPUT);
   pinMode(AIN2,OUTPUT);
   pinMode(AIN1,OUTPUT);
   pwmSetup();
   Serial.begin(9600);
}

void loop()
{
   b.setEffort(200);
   b.stop();
   delay(1000);
}
/*
   pinMode(2, INPUT);   
   pinMode(3, INPUT);   
   Serial.begin(9600);
   attachInterrupt(digitalPinToInterrupt(2), isr, CHANGE);
   attachInterrupt(digitalPinToInterrupt(3), isr, CHANGE);   
   pwmSetup();
   */



/*
void loop() 
{ 
  if (pb.isPressed()) 
  { 
     unsignedlong endTime = millis() + 100;     
     motor.setEffort(200); // effort test value    
     Serial.println(motor.getPosition()); 
     while (millis() < endTime) ;   
     } 
     else {     
       motor.setEffort(0);     
       motor.reset();   
    } 
     }
*/