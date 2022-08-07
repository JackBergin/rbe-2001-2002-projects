#include <Arduino.h>
#include <Romi32U4.h>
#include "BlueMotor.h"
#include "lineFollwer.h"

Romi32U4ButtonA pb;
BlueMotor motor;

//Variables for the PID Method
const float Kp = 3;
const float Ki = 0.1;
const float Kd = 0;
long timeToPrint = 0;
float sumOfErrors = 0;

//Method for the PID control of the motor
void pidControl(int degree){
  //We give the motor our the degree we want adjusted
  //and then convert it to the proportional encoder value
  float desiredPosition = degree/360*540;
  //This is the current encoder value of our motor
  float currentPosition = motor.getPosition();
  //This is the difference between the current and desired position
  //to initialize the PID control
  float error = currentPosition-desiredPosition;
  sumOfErrors += error;
  int effort = error * Kp + sumOfErrors * Ki;
  motor.setEffortWithoutDB(effort);
  if(millis() > timeToPrint){
    Serial.print("Distance: ");
    Serial.println(currentPosition);
    Serial.print("Effort: ");
    Serial.println(effort);
    timeToPrint += 100;
  }
  delay(10);
}

void setup() {
    Serial.begin(9600);
    Serial.println("Beginning Program");
    motor.setup();
}

void loop() {
  //First part of Lab 4
  for(int i = 0; i<=400; i++){
     motor.setEffortWithoutDB(i);
     Serial.print("Input Effort: ");
     Serial.println(i);
     Serial.print("Applied Effort: ");
     Serial.println(i*0.7+120);
     delay(100);
  }
  //Second part of Lab 4

   /*
    if (pb.isPressed()) {
        unsigned long endTime = millis() + 100; 
        motor.setEffort(390); // effort test value
        int oldPos = motor.getPosition();
        //Serial.print("Old Position: ");
        //Serial.print(oldPos); 
        while (millis() < endTime);
        int newPos = motor.getPosition();
        Serial.print("\t Count: ");
        Serial.print(newPos);
        long angularVel = (((abs(newPos) - abs(oldPos)) * 60) / 54);
        Serial.print("\t Angular Velocity: ");
        Serial.print(angularVel);
        Serial.print(" RPM");
        Serial.print("\tTime: ");
        Serial.print(millis());
        Serial.println(" ms");
    } else {
        motor.setEffort(0);
        motor.reset();
    }
    */
}
