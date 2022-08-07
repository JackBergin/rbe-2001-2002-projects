#pragma once
#include <Romi32U4.h>
#include <Arduino.h>


class Arm 
{ 
    public:   
       void setEffort(int effort);
       void moveTo (long position); 
       void sweep();
       void stop();
       void getPosition();
       void reset();
       void setup();
       
    private:
       void setEffort(int effort, bool clockwise);
       static void isr(){
          int newValue = 0;
          int oldValue = 0;
          int count = 0;
          int errorCount = 0;
          const char X = 5; 
          char encoderArray[4][4] = {
          {0, -1, 1, X},     
          {1, 0, X, -1},     
          {-1, X, 0, 1},     
          {X, 1, -1, 0}};

          newValue = (digitalRead(3)<<1) | digitalRead(2);
          char value = encoderArray[oldValue][newValue];
          if (value == X){
             errorCount++;
          }
          else{
             count -= value;
          }
          oldValue = newValue;
       }   

       const int tolerance = 3; 
       const int PWMOutPin = 11; 
       const int AIN2 = 4; 
       const int AIN1 = 13; 
       const int ENCA = 2; 
       const int ENCB = 3;
}; 