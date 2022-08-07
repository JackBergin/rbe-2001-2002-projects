#include "Rangefinder.h"


static const int triggerPin = 12;
static const int echoPin = 0;
static unsigned long startTime;
static unsigned long roundTripTime;
static unsigned long pingTimer;    

/**
 * Interrupt service routine for the echo pin
 * The echo pin goes high when the ultrasonic burst is sent out and goes low when the
 * echo is received. On the burst, the time is recorded, and on the echo the round trip
 * time is computed. This is used when requesting a distance by the getDistance methods.
 */
void ultrasonicISR()
{
    if (digitalRead(echoPin)) {
        startTime = micros();
    } else {
        roundTripTime = micros() - startTime;
    }
}

void Rangefinder::setup() {
    pingTimer = 0;
    pinMode(triggerPin, OUTPUT);
    pinMode(echoPin, INPUT);
    attachInterrupt(digitalPinToInterrupt(echoPin), ultrasonicISR, CHANGE);
}

void Rangefinder::loop() {
    if(millis() > pingTimer) {
        // trigger the ping
        digitalWrite(triggerPin, LOW);
        delayMicroseconds(2);
        digitalWrite(triggerPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(triggerPin, LOW);
        pingTimer = millis() + 100;
    }
}

/**
 * Return the distance of the ultrasonic sensor
 * @return Detected distance in CM.
 */
float Rangefinder::getDistanceCM() {
    long rt;
    cli();
    rt = roundTripTime;
    sei();
    float distance = (rt * 0.0343) / 2.0;
    return distance;
}

/**
 * Return the distance of the ultrasonic sensor, but freedom units.
 * @return Detected distance in inches. 
 */
float Rangefinder::getDistance() {
    float inches = getDistanceCM() * 0.393701;
    return inches;
}