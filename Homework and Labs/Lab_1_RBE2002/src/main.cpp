/*
 * Code for using TCC4 for precision PID timing.
 * You'll need to set TOP to set the interval
 * 
 * This code adds the ability to tune the gains and change the targets
 */

#include <Romi32U4.h>

#include "serial_comm.h"
#include "PIDcontroller.h"

Romi32U4ButtonA buttonA;

PIDController leftMotorController(1);           //start with  Kp = 1
volatile uint8_t PIDController::readyToPID = 0; //a flag that is set when the PID timer overflows

Romi32U4Motors motors;
Romi32U4Encoders encoders;

volatile int16_t countsLeft = 0;
volatile int16_t countsRight = 0;

void setup()
{
  Serial.begin(115200);
  //while(!Serial) {}  //IF YOU DON'T COMMENT THIS OUT, YOU MUST OPEN THE SERIAL MONITOR TO START
  Serial.println("Hi");

  noInterrupts(); //disable interupts while we mess with the Timer4 registers

  //sets up timer 4
  TCCR4A = 0x00; //disable some functionality -- no need to worry about this
  TCCR4B = 0x0B; //sets the prescaler -- look in the handout for values
  TCCR4C = 0x04; //toggles pin 6 at the timer frequency
  TCCR4D = 0x00; //normal mode

  /*
   * EDIT THE LINE BELOW WITH YOUR VALUE FOR TOP
   */

  /* 
    
    Motor drives at 4854 steps per second
    Target of 50 steps / PID tick, should tick every 0.0103 s (10.3 ms) (97.1 Hz)

    Prescaler of 1024
    16 mHz / 1024 = 15625 Hz
    Rollover at 160.9375 ticks
    TOP = 160
  */

  const uint8_t TOP = 160;

  OCR4C = TOP; //TOP goes in OCR4C

  TIMSK4 = 0x04; //enable overflow interrupt

  interrupts(); //re-enable interrupts

  // pinMode(6, OUTPUT); //COMMENT THIS OUT TO SHUT UP THE PIEZO!!!
}

float targetLeft = 0;
// long int lastTime = 0;
void loop()
{
  if (buttonA.getSingleDebouncedPress())
  {
    targetLeft = targetLeft < 40 ? 50 : 25;
  }

  if (PIDController::readyToPID) //timer flag set
  {
    // reset the flag
    PIDController::readyToPID = 0;

    // for tracking previous counts
    static int16_t prevLeft = 0;

    /*
     * Do PID stuffs here. Note that we turn off interupts while we read countsLeft/Right
     * so that it won't get accidentally updated (in the ISR) while we're reading it.
     */
    noInterrupts();

    int16_t speedLeft = countsLeft - prevLeft;
    prevLeft = countsLeft;

    interrupts();

    int16_t errorLeft = targetLeft - speedLeft;                      //calculate the error
    float effortLeft = leftMotorController.ComputeEffort(errorLeft); //calculate effort from error

    motors.setEfforts(effortLeft, 0);

    // int timeElapsed = millis() - lastTime;
    // lastTime = millis();
    // Serial.print(timeElapsed);
    Serial.print(millis());
    Serial.print('\t');
    Serial.print(targetLeft);
    Serial.print('\t');
    Serial.print(speedLeft);
    Serial.print('\t');
    Serial.print(effortLeft / 10.0); //divide effort by 10 for better plotting

    //you may want to add more serial printout here for testing

    Serial.print('\n');
  }

  /* for reading in gain settings
   * CheckSerialInput() returns true when it gets a complete string, which is
   * denoted by a newline character ('\n'). Be sure to set your Serial Monitor to 
   * append a newline
   */
  if (CheckSerialInput())
  {
    ParseSerialInput();
  }
}

/*
 * ISR for timing. On overflow, it takes a 'snapshot' of the encoder counts and raises a flag to let
 * the main program it is time to execute the PID calculations.
 */
ISR(TIMER4_OVF_vect)
{
  //Capture a "snapshot" of the encoder counts for later processing
  countsLeft = encoders.getCountsLeft();
  countsRight = encoders.getCountsRight();

  PIDController::readyToPID = 1;
}
