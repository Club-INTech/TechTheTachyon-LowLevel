// 
// 
// 

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
    pinMode(STEP_PIN_RIGHT, OUTPUT);
    pinMode(DIR_PIN_RIGHT, OUTPUT);
}

void ActuatorsMgr::handleInterrupt() {

    if(rightStepCount > 0) {
        if (rightStepCount % 2 == 0) {
            digitalWrite(STEP_PIN_RIGHT, HIGH);
        } else {
            digitalWrite(STEP_PIN_RIGHT, LOW);
        }
        rightStepCount--;
    } else {
        rightStepCount = 0;
        //analogWrite(STEP_PIN_RIGHT, 0);
        digitalWrite(STEP_PIN_RIGHT, LOW);
    }
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

