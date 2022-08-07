/**
 * Main controller class for the RBE 2001 Final Project
 * @author Brian Shin, with the guidance of Prof. Lewin & Prof. Miller
 */

#include <Arduino.h>
#include <Romi32U4.h>
#include "Chassis.h"
#include "BlueMotor.h"
#include "RangeFinder.h"
#include "servo32u4.h"
#include "RemoteConstants.h"
#include "IRdecoder.h"

Chassis chassis;
BlueMotor blueMotor;
Rangefinder rangeFinder;
Servo32U4 servo;
Romi32U4ButtonA pushButton;
IRDecoder decoder;

// ----- CONFIG START ----- //
const bool TESTING = false;             // Enable testing sequence instead of autonomous sequence
const bool SKIP_TRAVERSE = true;        // Skip the ambitious traversal across the field to the other side
const float GEAR_RATIO_LIFTER = 30.8;   // Lifter gear ratio

                                        // LIFTER POSITIONS BELOW MUST BE MULTIPLIED BY GEAR RATIO
const float LIFTER_MAX = -142.0;        // Max height for the lifter, if zeroed when linkage touches nut
const float LIFTER_PLATFORM = -5.0;     // Lifter position for height of platform
const float LIFTER_25ROOF = -135.0;     // Lifter position for height of 25-degree roof
const float LIFTER_45ROOF = -81.5;      // Lifter position for height of 45-degree roof
const float DIST_PLATFORM = 13.75;      // Distance (in.) between front of robot and platform with wheels on intersection
const float DIST_ROOF = 13.45;          // DIstance (in.) between front of robot and roof inner panel with wheels on intersection
const int GRIPPER_CLOSED = 815;         // Servo position for closed gripper
const int GRIPPER_OPEN = 1700;          // Servo position for open gripper

/* Additional configuration parameters can be found in the header files of specific classes.*/
// ----- CONFIG END -----//

bool paused = false;
bool tweak = false;

enum States {
    SETUP_RAISE,
    CONFIRM_SETUP,
    GRIPPING_1,
    CONFIRM_1,
    DRIVE_REV_LOWER_1,
    TURN_LEFT_1,
    DRIVE_FWD_PLATFORM,
    CONFIRM_2, 
    RELEASE_1,
    CONFIRM_3,
    GRIPPING_2,
    DRIVE_REV_LIFT_1,
    TURN_RIGHT_1,
    DRIVE_FWD_ROOF,
    CONFIRM_DEPOSIT,
    RELEASE_2,

    IDLE,           // Universal Idle
    STOPPED         // Universal E-Stop
} state;

const char *stateNames[] = {"SETUP_RAISE", "CONFIRM_SETUP", "GRIPPING_1", "CONFIRM_1", "DRIVE_REV_LOWER_1", 
    "TURN_LEFT_1", "DRIVE_FWD_PLATFORM", "CONFIRM_2", "RELEASE_1", "CONFIRM_3", "GRIPPING_2", "DRIVE_REV_LIFT_1", 
    "TURN_RIGHT_1", "DRIVE_FWD_ROOF", "CONFIRM_DEPOSIT", "RELEASE_2", "IDLE", "STOPPED"};

void checkRemote() {
    int keyCode = decoder.getKeyCode();
    if (keyCode == remotePlayPause) { // E-Stop Feature
        paused = !paused;
        Serial.println(paused ? "Paused" : "Running");
    } else if (keyCode == remoteSetup) {
        tweak = !tweak;
        Serial.println(tweak ? "Manual adjustment mode" : "Normal mode");
    } else if (keyCode == remoteUp && tweak) {
        Serial.println("Up Button: Adjusting lifter up");
        blueMotor.setEffortWithoutDB(-20);
    } else if (keyCode == remoteDown && tweak) {
        Serial.println("Down Button: Adjusting lifter down");
        blueMotor.setEffortWithoutDB(50);
    } else if (keyCode == remoteEnterSave && tweak) {
        Serial.println("Enter Button: Stopped lifter adjustment");
        blueMotor.setEffort(0);
    } else if (keyCode == remoteLeft && tweak) {
        Serial.println("Left Button: Gripper Open");
        servo.Write(GRIPPER_OPEN);
    } else if (keyCode == remoteRight && tweak) {
        Serial.println("Left Button: Gripper Closed");
        servo.Write(GRIPPER_CLOSED);
    }
}

// Test things
void testSequence() {
    
    // Serial.println(rangeFinder.getDistance());
    // chassis.drive(100);
    // if(rangeFinder.getDistance() <= 10.0) {
    //     chassis.drive(0);
    // }
    Serial.print("Dist: ");
    Serial.print(rangeFinder.getDistance());
    Serial.print("\t Targ: ");
    Serial.println(chassis.pullOnTarget(rangeFinder.getDistance()));
    chassis.startUltraDrive(10.0, rangeFinder.getDistance());
    chassis.loopUltraPID(rangeFinder.getDistance());
    if(chassis.pullOnTarget(rangeFinder.getDistance())) {
        chassis.drive(0);
        Serial.println("Target reached");
    }

}

/*
Start directly under the 25 deg roof, gripper open.
1. Close Gripper                            2. Wait for confirm, Back up X CM
3. Lower Arm to platform height (2in)       4. Go back to platform using line follow + dead reckon
5. Wait for confirm, Open Gripper           6. Wait for confirm, Close gripper
7. Lift arm to 25 deg height                8. Return to structure with line follow + dead + ultrasonic (last leg)
9. Wait for confirm, Open Gripper           10. Back up X CM
*/

void autoSequence1() {
    static States previousState = IDLE;
    if (state != previousState) {
        Serial.println(stateNames[state]);
        previousState = state;
    } 

    switch (state) {
        case SETUP_RAISE:
        // Raise the arm from its zeroed position then open gripper.
        blueMotor.startMoveTo(LIFTER_25ROOF * GEAR_RATIO_LIFTER);
        blueMotor.loopController();
        if (blueMotor.pullOnTarget()) {
            Serial.println("Lifter arm movement complete");
            blueMotor.setEffort(0);
            servo.Write(GRIPPER_OPEN);
            state = CONFIRM_SETUP;
        }
        break;

        case CONFIRM_SETUP:
        // Wait for confirmation from user that robot is properly placed
        static int tempCount1 = 0;
        if(tempCount1 == 0) {
            Serial.print("Awaiting user confirmation");
            paused = true;
            tempCount1++;
        }

        if(!paused) state = GRIPPING_1;
        break;

        case GRIPPING_1:
        // Close gripper on existing plate for removal
        servo.Write(GRIPPER_CLOSED);
        state = CONFIRM_1;
        break;
        
        case CONFIRM_1:
        // Wait for confirmation from user that plate is securely gripped
        static int tempCount2 = 0;
        if(tempCount2 == 0) {
            Serial.print("Awaiting user confirmation");
            paused = true;
            tempCount2++;
        }

        if(!paused) state = DRIVE_REV_LOWER_1;
        break;

        case DRIVE_REV_LOWER_1:
        // Line follow and drive in reverse until intersection, also lower arm to platform height

        // TODO: Add linefollow/drive code
        blueMotor.startMoveTo(LIFTER_45ROOF * GEAR_RATIO_LIFTER);
        blueMotor.loopController();
        if (blueMotor.pullOnTarget()) {
            Serial.println("Lifter arm movement complete");
            blueMotor.setEffort(0);
            state = TURN_LEFT_1;
            chassis.startTurn(-90);
        }
        break;

        case TURN_LEFT_1:
        // Turn left 90 degrees to face platform
        if(chassis.turnComplete()) {
            Serial.println("Turn 90 left complete");
            chassis.drive(0);
            state = DRIVE_FWD_PLATFORM;
        }
        break;

        case DRIVE_FWD_PLATFORM:
        // Drive to platform with linefollow/ultrasonic/both
        break;

        case CONFIRM_2:
        // Wait for confirmation from user that plate is placed on platform
        break;

        case RELEASE_1:
        // Open gripper to place plate on platform
        break;

        case CONFIRM_3:
        // Wait for confirmation from user that old plate is removed and new plate is on platform
        break;

        case GRIPPING_2:
        // Close gripper on new plate
        break;

        case DRIVE_REV_LIFT_1:
        // Line follow and drive in reverse until intersection, also raise arm to 25 deg roof height
        break;

        case TURN_RIGHT_1:
        // Turn right 90 degrees to face roof
        break;

        case DRIVE_FWD_ROOF:
        // Drive to roof with linefollow/ultrasonic/both
        break;

        case CONFIRM_DEPOSIT:
        // Wait for confirmation from user that plate is alligned with roof
        break;

        case RELEASE_2:
        // Open gripper to place plate on 25 deg roof
        break;

        case IDLE:
        
        break;

        case STOPPED:
        paused = true;
        break;
    }
}

/**
 * Set up the various subsystems.
 */
void setup() {
    Serial.begin(9600);
    Serial.println("Beginning Program");

    decoder.init();

    blueMotor.setup();
    blueMotor.setEffort(0);
    blueMotor.reset();

    servo.Init();
    servo.Attach();
    servo.SetMinMaxUS(GRIPPER_CLOSED, GRIPPER_OPEN);
    pinMode(18, INPUT);
    servo.Write(GRIPPER_CLOSED); // Start the gripper closed.

    rangeFinder.setup();
    chassis.setup();

    state = SETUP_RAISE;

    while(!pushButton.isPressed()) delay(10); // Wait for button to start
}

/** 
 * Core loop of the controller, runs remote check and then the main sequencing/functions.
 * Note that the runtime/latency of any functions here will inhibit the latency of the remote functionality,
 * namely the E-Stop feature (which isn't an E-Stop if its not instantaneous).
 */
void loop() {
  
  checkRemote();
  rangeFinder.loop();
  
  if(paused) {
      //Serial.print("Code paused. Last state: ");
      //Serial.println(stateNames[state]);
      
      chassis.drive(0);     // Turn off all motors
      blueMotor.setEffort(0);
  } else {
      // Run main sequencing
      if(!TESTING) autoSequence1(); 
      else testSequence();
  }
}