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
    pinMode(STEP_PIN_LEFT, OUTPUT);
    pinMode(STEP_PIN_RIGHT, OUTPUT);
    pinMode(DIR_PIN_LEFT, OUTPUT);
    pinMode(DIR_PIN_RIGHT, OUTPUT);
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

        if(leftStepCount == 0) {
            InterruptStackPrint::Instance().push(EVENT_HEADER, "leftElevatorStopped");
        }
    } else {
        leftStepCount = 0;
        digitalWrite(STEP_PIN_LEFT, LOW);
//        analogWrite(STEP_PIN_LEFT, 0);
    }

    if(rightStepCount > 0) {
        if (rightStepCount % 2 == 0) {
            digitalWrite(STEP_PIN_RIGHT, HIGH);
        } else {
            digitalWrite(STEP_PIN_RIGHT, LOW);
        }
        rightStepCount--;

        if(rightStepCount == 0) {
            InterruptStackPrint::Instance().push(EVENT_HEADER, "rightElevatorStopped");
        }
    } else {
        rightStepCount = 0;
        digitalWrite(STEP_PIN_RIGHT, LOW);
        //analogWrite(STEP_PIN_RIGHT, 0);
    }
}

void ActuatorsMgr::moveLeftStepper(int32_t count) {
    this->leftDirection = count > 0 ? UP : DOWN;

    // inversé par rapport à droite
    if(count > 0) {
        digitalWrite(DIR_PIN_LEFT, HIGH);
    } else {
        digitalWrite(DIR_PIN_LEFT, LOW);
    }
    leftStepCount = ABS(count)*STEP_COUNT;
   // analogWrite(STEP_PIN_LEFT, 128);
/*    leftStepCount += count*STEP_COUNT;
    leftStepper.setTargetAbs(leftStepCount);
    stepControl.moveAsync(leftStepper);*/
}

void ActuatorsMgr::moveRightStepper(int32_t count) {
    this->rightDirection = count > 0 ? UP : DOWN;
    if(count > 0) {
        digitalWrite(DIR_PIN_RIGHT, HIGH);
    } else {
        digitalWrite(DIR_PIN_RIGHT, LOW);
    }
    rightStepCount = ABS(count)*STEP_COUNT;
 //   analogWrite(STEP_PIN_RIGHT, 128);
/*    rightStepCount += count*STEP_COUNT;
    rightStepper.setTargetAbs(rightStepCount);
    stepControl.moveAsync(rightStepper);*/
}

