#include <Arduino.h>
#include <Romi32U4.h>
#include "PIDController.h"

PIDController::PIDController() {
    setPID(0, 0, 0);
}

PIDController::PIDController(float p, float i, float d) {
    setPID(p, i, d);
}

void PIDController::setPID(float p, float i, float d) {
    errorSum = 0;
    previousError = 0;
    Kp = p;
    Ki = i;
    Kd = d;
    setpoint = 0;
}

void PIDController::setSetpoint(float target) {
    setpoint = target;
}

void PIDController::setTolerance(float t) {
    tolerance = t;
}

float PIDController::calculateEffort(float inputData) {
    int effort;
    float error = inputData - setpoint;
    float delta = previousError - error;

    previousError = error;
    errorSum += error;
    effort = error * Kp + errorSum * Ki + delta * Kd;

    return effort;
}

bool PIDController::onTarget(float inputData) {
    float error;
    noInterrupts();
    error = inputData - setpoint;
    interrupts();
    return abs(error) <= tolerance;
}

/**
 * Get the gain values of the PID controller
 * @param index 1 for Kp, 2 for Ki, 3 for Kd
 * @return Gain value
 */
float PIDController::getGainValue(int index) {
    float gain = 0;

    switch(index) {
        case 1:
            gain = Kp;
            break;
        case 2:
            gain = Ki;
            break;
        case 3:
            gain = Kd;
            break;
        default:
            gain = 0;
    }

    return gain;
}

float PIDController::getSetpoint() {
    return setpoint;
}