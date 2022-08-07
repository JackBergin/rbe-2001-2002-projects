#include <Arduino.h>
#include <Romi32U4.h>
#include "BlueMotor.h"

static BlueMotor *bm_instance; // Create an instance of the BlueMotor class to reference non-static members.

/**
 * Constructor for class BlueMotor  
 */
BlueMotor::BlueMotor() {
    bm_PID = PIDController(4.5, 0.02, 0.02);
}

/**
 * Sets the effort for the BlueMotor.
 * NOTE: ON ROBOT, POSITIVE EFFORT IS DOWN.
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

/**
 * Sets the effort for the BlueMotor.
 * @param effort Desired effort
 */
void BlueMotor::setEffortWithoutDB(int effort) {
    int minPosDBEffort = 120;
    int minNegDBEffort = -130;

    float posDBEffortRange = 400 - minPosDBEffort;
    float negDBEffortRange = 400 + minNegDBEffort;

    float posDBEffort = effort * (posDBEffortRange/400) + minPosDBEffort;
    float negDBEffort = effort * (negDBEffortRange/400) - minNegDBEffort;

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
    } else PWM = 0;

    applEff = PWM; // Dirty global variable because I'm lazy.

    OCR1C = PWM;
    digitalWrite(AIN1, analog1);
    digitalWrite(AIN2, analog2);
}

/**
 * Move to a specified position, and then stop. Blocking Function.
 * @param longPosition Desired position, in degrees
 */
void BlueMotor::moveTo(float longPosition) {
    float PIDOut;
    bm_PID.setSetpoint(longPosition);

    while(!bm_PID.onTarget(getPosition())) {
        PIDOut = bm_PID.calculateEffort(getPosition());
        setEffortWithoutDB(-PIDOut);

        Serial.print("Current Pos.: ");
        Serial.print(getPosition());
        Serial.print("  || Setpoint: ");
        Serial.print(pullSetpoint());
        Serial.print("  || PID Output: ");
        Serial.print(PIDOut);
        Serial.print("  || Applied Effort: ");
        Serial.print(applEff);
        Serial.print("  || PID Kp: ");
        Serial.print(pullGain(1));
        Serial.print("  || PID Ki: ");
        Serial.print(pullGain(2));
        Serial.print("  || PID Kd: ");
        Serial.println(pullGain(3));

        delay(10); // 100Hz controller.
    }
    setEffort(0);
}

/**
 * A non-blocking version of the `moveTo()` method that sets the setpoint
 * and begins turning the motors.
 * @param position Desired position, in degrees
 */
void BlueMotor::startMoveTo(float position) {
    bm_PID.setSetpoint(position);  
    setEffortWithoutDB( -bm_PID.calculateEffort(getPosition()) );
    delay(10); // Run at 100Hz
}

/**
 * Accompanying function to `startMoveTo()` that runs a non-blocking PID algorithm
 */
void BlueMotor::loopController() {
    if (!bm_PID.onTarget(getPosition())) {
        Serial.print("Current Pos.: ");
        Serial.print(getPosition());
        Serial.print("  || Setpoint: ");
        Serial.println(pullSetpoint());
        setEffortWithoutDB( -bm_PID.calculateEffort(getPosition()) );
        delay(10); // Run at 100Hz
    } else {
        setEffort(0);
    }
}

/**
 * Get the position of the motor
 * @return Position in degrees  
 */
float BlueMotor::getPosition() {
    float pos;
    noInterrupts();
    pos = (float)count / (float)encoderCountPerRev * 360.0; // Degrees
    interrupts();
    return pos;
}

/**
 * Reset the count on the encoder
 */        
void BlueMotor::reset() {
    noInterrupts();
    count = 0;
    interrupts();
}

/**
 * Setup pins and interrupts for the class
 */
void BlueMotor::setup() {
    bm_instance = this;

    bm_PID.setTolerance(tolerance);

    //motors.setEfforts(0, 0);
    pinMode(PWMOutPin, OUTPUT);
    pinMode(AIN2, OUTPUT);
    pinMode(AIN1, OUTPUT);
    pinMode(ENCA, INPUT);
    pinMode(ENCB, INPUT);
    pwmSetup();

    attachInterrupt(digitalPinToInterrupt(ENCA), isr, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCB), isr, CHANGE);
    reset();
}

/**
 * Pull the setpoint value from the local PIDController instance
 * @return Setpoint for the PID controller
 */
float BlueMotor::pullSetpoint() {
    return bm_PID.getSetpoint();
}

/**
 * Pull the PID gain value from the local PIDController instance
 * @param index 1 for Kp, 2 for Ki, 3 for Kd
 * @return Gain value
 */
float BlueMotor::pullGain(int index) {
    return bm_PID.getGainValue(index);
} 

/**
 * Pull the PID onTarget status from the local PIDController instance
 * @return True if the lifter is on target. 
 */
bool BlueMotor::pullOnTarget() {
    return bm_PID.onTarget(getPosition());
}

// ----- PRIVATE CLASS METHODS BELOW ----- //

/**
 * Another setEffort method but private. I don't really know why this exists when it can be done in one method...
 * @param effort Desired effort 
 * @param clockwise Direction of rotation, true for clockwise
 */
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
    newValue = (digitalRead(ENCA) << 1) | digitalRead(ENCB);
    char value = encoderArray[oldValue] [newValue];
    if (value == X) {
        errorCount++;
    } else {
        noInterrupts();
        count -= value;
        interrupts();
    }
    oldValue = newValue;
}