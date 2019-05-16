// 
// 
// 

#include <COM/InterruptStackPrint.h>
#include "ActuatorsMgr.h"

ActuatorsMgr::ActuatorsMgr() : dynamixelManager(new DynamixelManager(&XLSerial, &DebugSerial))
{
}

ActuatorsMgr::~ActuatorsMgr()
{
}

void stepperInterrupt() {
    ActuatorsMgr::Instance().handleInterrupt();
}

void ActuatorsMgr::initTorques() {
    ActuatorsMgr::Instance().leftArm->initTorque();
    ActuatorsMgr::Instance().rightArm->initTorque();
}

void ActuatorsMgr::initPWMs() {
    // FIXME
    pinMode(STEP_PIN_LEFT, INPUT);
    pinMode(STEP_PIN_RIGHT, INPUT);

    pinMode(DIR_PIN_LEFT, INPUT);
    pinMode(DIR_PIN_RIGHT, INPUT);
    // FIXME

    pinMode(RST_PIN_LEFT, OUTPUT);
    pinMode(RST_PIN_RIGHT, OUTPUT);
    digitalWrite(RST_PIN_LEFT, HIGH);
    digitalWrite(RST_PIN_RIGHT, HIGH);
}

void ActuatorsMgr::handleInterrupt() {
    if(leftStepCount > 0) {
        if(leftStepCount % 2 == 0) {
            digitalWrite(STEP_PIN_LEFT, HIGH);
        } else {
            digitalWrite(STEP_PIN_LEFT, LOW);
        }
        leftStepCount--;

        if(leftStepCount == 0 && nextLeftStepCount == 0) {
            InterruptStackPrint::Instance().push(EVENT_HEADER, "leftElevatorStopped");
        }
    } else {
        leftStepCount = 0;
        digitalWrite(STEP_PIN_LEFT, LOW);

        if(timerForLeftStepper < 0) {
            timerForLeftStepper = STEPPER_DELAY;
        }
        if(timerForLeftStepper > 0) {
            timerForLeftStepper--;
        }
        if(timerForLeftStepper == 0) {
            moveLeftStepper(nextLeftStepCount);
            nextLeftStepCount = 0;
        }
    }

    if(rightStepCount > 0) {
        if (rightStepCount % 2 == 0) {
            digitalWrite(STEP_PIN_RIGHT, HIGH);
        } else {
            digitalWrite(STEP_PIN_RIGHT, LOW);
        }
        rightStepCount--;

        if(rightStepCount == 0 && nextRightStepCount == 0) {
            InterruptStackPrint::Instance().push(EVENT_HEADER, "rightElevatorStopped");
        }
    } else {
        rightStepCount = 0;
        digitalWrite(STEP_PIN_RIGHT, LOW);

        if(timerForRightStepper < 0) { // lance l'attente
            timerForRightStepper = STEPPER_DELAY;
        }
        if(timerForRightStepper > 0) {
            timerForRightStepper--;
        }
        if(timerForRightStepper == 0) { // si l'attente est finie
            moveRightStepper(nextRightStepCount);
            nextRightStepCount = 0;
        }
    }
}

void ActuatorsMgr::moveLeftStepper(int32_t count, int32_t nextCount) {
    timerForLeftStepper = STEPPER_DELAY;
    this->leftDirection = count > 0 ? UP : DOWN;

    // inversé par rapport à droite
    if(count > 0) {
        digitalWrite(DIR_PIN_LEFT, HIGH);
    } else {
        digitalWrite(DIR_PIN_LEFT, LOW);
    }
    leftStepCount = ABS(count)*STEP_COUNT;

    nextLeftStepCount = nextCount;
   // analogWrite(STEP_PIN_LEFT, 128);
/*    leftStepCount += count*STEP_COUNT;
    leftStepper.setTargetAbs(leftStepCount);
    stepControl.moveAsync(leftStepper);*/
}

void ActuatorsMgr::moveRightStepper(int32_t count, int32_t nextCount) {
    timerForRightStepper = STEPPER_DELAY;
    this->rightDirection = count > 0 ? UP : DOWN;
    if(count > 0) {
        digitalWrite(DIR_PIN_RIGHT, HIGH);
    } else {
        digitalWrite(DIR_PIN_RIGHT, LOW);
    }
    rightStepCount = ABS(count)*STEP_COUNT;

    nextRightStepCount = nextCount;
 //   analogWrite(STEP_PIN_RIGHT, 128);
/*    rightStepCount += count*STEP_COUNT;
    rightStepper.setTargetAbs(rightStepCount);
    stepControl.moveAsync(rightStepper);*/
}

void ActuatorsMgr::checkArmMovements() {
    leftArm->update();
    rightArm->update();
}
