//Libraries for the code
#include <Arduino.h>
#include <Romi32U4Encoders.h>
#include "Chassis.h"
#include "LineFollower.h"
#include "Rangefinder.h"
#include "BlueMotor.h"
#include "PIDController.h"

//Creates qtr obj
QTRSensors qtr;

Romi32U4ButtonA pb;

//Sets up the motors and encoders from the above libraries
Romi32U4Motors motors;
Romi32U4Encoders encoder;

//Creates a new chassis class object executed in loop
Chassis a = Chassis();

//Creates a new rangefinder class object executed in loop
Rangefinder b = Rangefinder();

//Creates a new LineFollower class object executed in loop
LineFollower c = LineFollower();

//Variables for the line sensor method
 int left_sensor_state;
 int right_sensor_state; 
 double right_sensor_fix;
 const int left_sensor_pin =A2;
 const int right_sensor_pin =A3;
 const int lineFollowerEffort = 60;

//Pin values for the ultrasonic sensor
static const int triggerPin = 12;
static const int echoPin = 0;

 //Creates a counter for the loop loop:
int counter;

const uint8_t SensorCount = 2;
uint16_t sensorValues[SensorCount];
/////////////////////////////////////////////////CHASSIS CODE/////////////////////////////////////////////////////////
void Chassis::driveDistance(float inches){
    //Resets the encoders when different distances want to be driven
    encoders.getCountsAndResetRight();
    encoders.getCountsAndResetLeft();
    
    //This is the comparer for the encoder values
    float encoderComparer = inches *(Chassis::CPR / (Chassis::wheelDiameter * 3.14159265));
    
    //Logic for driving a specific distance
    if((abs(encoder.getCountsLeft()) < encoderComparer) && (abs(encoder.getCountsRight()) < encoderComparer)){
        //Turns on motors and operates at 100%
        motors.setEfforts(100,100);
    }
}

void Chassis::turnAngle(float degrees){     
    //Resets the encoders when different distances want to be driven
    encoders.getCountsAndResetRight();
    encoders.getCountsAndResetLeft();

    //This is the comparer for the encoder values
    float encoderComparer = ((degrees / 360) * Chassis::wheelTrack) * (Chassis::CPR / (Chassis::wheelDiameter));
    
    //Logic for turning a specific angle
    if((abs(encoder.getCountsLeft()) < encoderComparer) && (abs(encoder.getCountsRight()) < encoderComparer)){
        //Turns on motors and operates at 100% in opposite directions
        motors.setEfforts(100,-100);
    }
}

void stop(){
  motors.setEfforts(0,0);
}
////////////////////////////////////////////////LINE FOLLOWER CODE///////////////////////////////////////////////////////
void LineFollower::setLineFollowForward() {
  left_sensor_state = analogRead(left_sensor_pin);
  right_sensor_state = analogRead(right_sensor_pin);

  if(right_sensor_state > 500 && left_sensor_state < 500){
    Serial.println("turning right");
    Serial.println(right_sensor_state);
    Serial.println(left_sensor_state);
    Serial.println("------------------");
    motors.setEfforts(lineFollowerEffort,0);
    delay(10);
  }
  if(right_sensor_state < 500 && left_sensor_state > 500){
    Serial.println("turning left");
    Serial.println(right_sensor_state);
    Serial.println(left_sensor_state);
    Serial.println("------------------");   
    motors.setEfforts(0,lineFollowerEffort);
    delay(10);
  }

  if(right_sensor_state > 500 && left_sensor_state > 500){
    Serial.println("going forward");
    Serial.println(right_sensor_state);
    Serial.println(left_sensor_state);
    Serial.println("------------------");
    motors.setEfforts(lineFollowerEffort,lineFollowerEffort);
    delay(10);
  }

  if(right_sensor_state < 500 && left_sensor_state < 500){ 
    Serial.println("stop");
    Serial.println(right_sensor_state);
    Serial.println(left_sensor_state);
    Serial.println("------------------");    
    motors.setEfforts(0,0);
    delay(10);
  }
}

void LineFollower::setLineFollowBackward() {
  left_sensor_state = analogRead(left_sensor_pin);
  right_sensor_state = analogRead(right_sensor_pin);

  if(right_sensor_state > 500 && left_sensor_state < 500){
    Serial.println("turning left");
    Serial.println(right_sensor_state);
    Serial.println(left_sensor_state);
    Serial.println("------------------");
    motors.setEfforts(0,lineFollowerEffort);
    delay(100);
  }
  if(right_sensor_state < 500 && left_sensor_state > 500){
    Serial.println("turning right");
    Serial.println(right_sensor_state);
    Serial.println(left_sensor_state);
    Serial.println("------------------");    
    motors.setEfforts(lineFollowerEffort,0);
    delay(100);
  }

  if(right_sensor_state > 500 && left_sensor_state > 500){
    Serial.println("going backward");
    Serial.println(right_sensor_state);
    Serial.println(left_sensor_state);
    Serial.println("------------------");
    motors.setEfforts(-lineFollowerEffort,-lineFollowerEffort);
    delay(100);
  }

  if(right_sensor_state < 500 && left_sensor_state < 500){ 
    Serial.println("stop");
    Serial.println(right_sensor_state);
    Serial.println(left_sensor_state);
    Serial.println("------------------");    
    motors.setEfforts(0,0);
    delay(100);
  }
}
////////////////////////////////////////////////RANGE FINDER CODE//////////////////////////////////////////////////
void Rangefinder::ultrasonicDistanceTurn(float desDistance){
    // trigger the ping
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(100);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(100);
    digitalWrite(triggerPin, LOW);

    //Calculations of the ultrasonic distance
    long duration = pulseIn(echoPin, HIGH);
    float distance = duration * 0.034 / 2; // Speed of sound wave divided by 2
    
    //For
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

void Rangefinder::ultrasonicTo(float desDistance){
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
void Rangefinder::ultrasonicFrom(float desDistance){
    // trigger the ping
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(100);
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(100);
    digitalWrite(triggerPin, LOW);

    //Calculations of the ultrasonic distance
    long duration = pulseIn(echoPin, HIGH);
    float distance = duration * 0.034 / 2; // Speed of sound wave divided by 2
    
    if(distance < desDistance){
       motors.setEfforts(-100,-100);
       Serial.print("Backward: ");
       Serial.println(distance);
       delay(10);
    }
    //if the distance is less than or equal to the desired distance: inches, the bot will stop
    if(distance >= desDistance){
       motors.setEfforts(0,0);
       Serial.print("Stop: ");
       Serial.println(distance);
       delay(10);
    }
}
/////////////////////////////////////////////////CLASS LOOP AND SETUP///////////////////////////////////////////////
void setup(){
    //Sets the motor pins to output voltage
    pinMode(13, OUTPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);
    
    //Sets the ultrasonic pins to have an output and i
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);

    //Sets up the serial monitor and prints out the 
    //declaration that the program setup has begun
    Serial.begin(9600);
    Serial.println("Beginning Program");
}
void loop()
{
  int desiredDistance = 13.5/0.393701;
  b.ultrasonicFrom(desiredDistance);
  stop();
  delay(50);
  a.turnAngle(90);
  stop();
  delay(50);
  b.ultrasonicTo(2);
  stop();
}






