#include <Arduino.h>
#include <Romi32U4.h>
#include "BlueMotor.h"

static BlueMotor *bm_instance; // Create an instance of the BlueMotor class to reference non-static members.

/**
 * Constructor for class BlueMotor  
 */
BlueMotor::BlueMotor() {
}

/**
 * Sets the effort for the BlueMotor.
 * @param effort Desired effort
 */
void BlueMotor::setEffort(int effort) {
    int analog1, analog2 = 0;
    int PWM = 0;
    
    if (effort > 0) { // Set pins HIGH and LOW for forward
        analog1 = 1;
        analog2 = 0;
        PWM = effort;
    } else if (effort < 0) { // Set pins LOW and HIGH for reverse
        analog1 = 0;
        analog2 = 1;
        PWM = -effort;
    } else PWM = 0;

    OCR1C = PWM;
    digitalWrite(AIN1, analog1);
    digitalWrite(AIN2, analog2);
}

void BlueMotor::setEffortWithoutDB(int effort) {
    float minPosDBEffort = 120;
    float minNegDBEffort = -130;

    float posDBEffortRange = 400 - minPosDBEffort;
    float negDBEffortRange = 400 + minNegDBEffort;

    float posDBEffort = effort * (posDBEffortRange/400)+minPosDBEffort;
    float negDBEffort = effort * (negDBEffortRange/400)-minNegDBEffort;
    
    int analog1, analog2 = 0;
    int PWM = 0;
    
    if (effort > 0) { // Set pins HIGH and LOW for forward
        analog1 = 1;
        analog2 = 0;
        PWM = posDBEffort;
    } else if (effort < 0) { // Set pins LOW and HIGH for reverse
        analog1 = 0;
        analog2 = 1;
        PWM = -negDBEffort;
    } 
    else PWM = 0;
    OCR1C = PWM;
    digitalWrite(AIN1, analog1);
    digitalWrite(AIN2, analog2);
}

/**
 * Move to a specified position
 * @param longPosition Desired position, in [UNITS]
 */
float BlueMotor::moveTo(float longPosition) {
    return 1.00;
}

/**
 * Get the position of the motor
 * @return Position in [UNITS]  
 */
float BlueMotor::getPosition() {
    return count; // Units are encoder ticks for now.
}

/**
 * Reset the count on the encoder
 */        
void BlueMotor::reset() {
    count = 0;
}

/**
 * Setup pins and interrupts for the class
 */
void BlueMotor::setup() {
    bm_instance = this;

    //motors.setEfforts(0, 0);
    pinMode(PWMOutPin, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(ENCA, INPUT);
    pinMode(ENCB, INPUT);
    pwmSetup();

    attachInterrupt(digitalPinToInterrupt(ENCA), isr, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCB), isr, CHANGE);
}

// ----- PRIVATE CLASS METHODS BELOW ----- //

void BlueMotor::setEffort(int effort, bool clockwise) {
    int effortInput = (clockwise) ? -effort : effort;
    setEffort(effortInput);
}

/**
 * Setup PWM interrupt registers
 */
void BlueMotor::pwmSetup() {
    TCCR1A = 0xA8; 
    TCCR1B = 0x11; 
    ICR1 = 400;
    OCR1C = 0;
}

/**
 * Interrupt Serice Routine for encoder operation
 */
void BlueMotor::isr() {
    bm_instance->encoderInterrupt(); // Call the encoder function from a pointer to avoid nonstatic reference.
}

/**
 * Event handler for encoder ISR.
 */
void BlueMotor::encoderInterrupt() {
  newValue = (digitalRead(ENCA)<<1) | digitalRead(ENCB);
    char value = encoderArray[oldValue] [newValue];
    if (value == X) {
        errorCount++;
    } else {
        count -= value;
    }
    oldValue = newValue;
}
