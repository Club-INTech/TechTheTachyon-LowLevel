// 
// 
// 

#include <COM/InterruptStackPrint.h>
#include "ActuatorsMgr.h"

ActuatorsMgr::ActuatorsMgr() : dynamixelManager(new DynamixelManager(&XLSerial, &DebugSerial))
{
    motor1 = (XL430*) dynamixelManager->createMotor(1, XL430GeneratorFunction);//new XL430(1,*manager);
    motor2 = (XL430*) dynamixelManager->createMotor(2, XL430GeneratorFunction);//new XL430(2,*manager);
    motor3 = (XL430*) dynamixelManager->createMotor(3, XL430GeneratorFunction);//new XL430(3,*manager);
    motor254 = (XL430*) dynamixelManager->createMotor(254, XL430GeneratorFunction);//new XL430(3,*manager);

    motor1->changeLED(true);
    motor2->changeLED(true);
    motor3->changeLED(true);



    rightArm = new Arm<XL430>("right", *dynamixelManager, new XL430[3]{*motor1, *motor2, *motor3});

#if defined(MAIN)

    motor4 = (XL430*) dynamixelManager->createMotor(4, XL430GeneratorFunction);//new XL430(1,*manager);
    motor5 = (XL430*) dynamixelManager->createMotor(5, XL430GeneratorFunction);//new XL430(2,*manager);
    motor6 = (XL430*) dynamixelManager->createMotor(6, XL430GeneratorFunction);//new XL430(3,*manager);
    motor4->changeLED(true);
    motor5->changeLED(true);
    motor6->changeLED(true);

    leftArm = new Arm<XL430>("left", *dynamixelManager, new XL430[3]{*motor4,*motor5,*motor6});

#elif defined(SLAVE)
    motor4 = (XL430*) dynamixelManager->createMotor(4, XL430GeneratorFunction);//new XL430(7,*manager);
    motor5 = (XL430*) dynamixelManager->createMotor(5, XL430GeneratorFunction);//new XL430(7,*manager);
    motor6 = (XL430*) dynamixelManager->createMotor(6, XL430GeneratorFunction);//new XL430(7,*manager);

#endif
}

ActuatorsMgr::~ActuatorsMgr()
{
}

void stepperInterrupt() {
    ActuatorsMgr::Instance().handleInterrupt();
}

void ActuatorsMgr::initTorques() {
    ActuatorsMgr::Instance().rightArm->initTorque();
#if defined(MAIN)
    ActuatorsMgr::Instance().leftArm->initTorque();
#elif defined(SLAVE)
    ActuatorsMgr::Instance().motor4->toggleTorque(true);
#endif
}


void ActuatorsMgr::handleInterrupt() {
#if defined(MAIN)
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
#endif

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

#if defined(MAIN)
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
}
#endif

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
}

#if defined(SLAVE)

void ActuatorsMgr::moveRightStepperOust(int32_t count, int32_t nextCount) {
    timerForRightStepper = STEPPER_DELAY;
    this->rightDirection = count > 0 ? UP : DOWN;
    if(count > 0) {
        digitalWrite(DIR_PIN_RIGHT, HIGH);
    } else {
        digitalWrite(DIR_PIN_RIGHT, LOW);
    }
    rightStepCount = ABS(count)*STEP_COUNT_OUST;

    nextRightStepCount = nextCount;
    //   analogWrite(STEP_PIN_RIGHT, 128);
/*    rightStepCount += count*STEP_COUNT_OUST;
    rightStepper.setTargetAbs(rightStepCount);
    stepControl.moveAsync(rightStepper);*/
}

void ActuatorsMgr::moveRightStepperOust2(int32_t count, int32_t nextCount) {
    timerForRightStepper = STEPPER_DELAY;
    this->rightDirection = count > 0 ? UP : DOWN;
    if(count > 0) {
        digitalWrite(DIR_PIN_RIGHT, HIGH);
    } else {
        digitalWrite(DIR_PIN_RIGHT, LOW);
    }
    rightStepCount = ABS(count)*(STEP_COUNT+STEP_COUNT_OUST);

    nextRightStepCount = nextCount;
    //   analogWrite(STEP_PIN_RIGHT, 128);
/*    rightStepCount += count*STEP_COUNT_OUST;
    rightStepper.setTargetAbs(rightStepCount);
    stepControl.moveAsync(rightStepper);*/
}

#endif

void ActuatorsMgr::checkArmMovements() {
#if defined(MAIN)
    leftArm->update();
#endif
    rightArm->update();
}

void ActuatorsMgr::rebootArms() {
#if defined(MAIN)
    leftArm->rebootXLsIfNecessary();
#endif
    rightArm->rebootXLsIfNecessary();
}
