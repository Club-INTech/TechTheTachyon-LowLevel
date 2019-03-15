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

void ActuatorsMgr::initPWMs() {
    pinMode(STEP_PIN_LEFT, OUTPUT);
    pinMode(STEP_PIN_RIGHT, OUTPUT);
    pinMode(DIR_PIN_LEFT, OUTPUT);
    pinMode(DIR_PIN_RIGHT, OUTPUT);
    analogWriteFrequency(STEP_PIN_LEFT, STEPPER_FREQUENCY);
    analogWriteFrequency(STEP_PIN_RIGHT, STEPPER_FREQUENCY);
}

void ActuatorsMgr::handleInterrupt() {
    if(leftStepCount > 0) {
        leftStepCount--;
    } else {
        leftStepCount = 0;
        analogWrite(STEP_PIN_LEFT, 0);
    }

    if(rightStepCount > 0) {
        rightStepCount--;
    } else {
        rightStepCount = 0;
        analogWrite(STEP_PIN_RIGHT, 0);
    }
}

void ActuatorsMgr::moveLeftStepper(int32_t count) {
    this->leftDirection = count > 0 ? UP : DOWN;
    noInterrupts();
    if(count > 0) {
        digitalWrite(DIR_PIN_LEFT, HIGH);
    } else {
        digitalWrite(DIR_PIN_LEFT, LOW);
    }
    leftStepCount = ABS(count)*STEP_COUNT;
    analogWrite(STEP_PIN_LEFT, 128);
    interrupts();
}

void ActuatorsMgr::moveRightStepper(int32_t count) {
    this->rightDirection = count > 0 ? UP : DOWN;
    noInterrupts();
    if(count > 0) {
        digitalWrite(DIR_PIN_RIGHT, HIGH);
    } else {
        digitalWrite(DIR_PIN_RIGHT, LOW);
    }
    rightStepCount = ABS(count)*STEP_COUNT;
    analogWrite(STEP_PIN_RIGHT, 128);
    interrupts();
}

