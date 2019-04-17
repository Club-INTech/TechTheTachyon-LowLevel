// ActuatorsMgr.h


#ifndef _ACTUATORSMGR_h
#define _ACTUATORSMGR_h

#include "Arduino.h"

#include "DynamixelManager.h"
#include "XL430.h"
#include "SyncWrite.h"

#include "Utils/pin_mapping.h"
#include "Utils/Singleton.hpp"
#include "Utils/utils.h"
#include "Utils/defines.h"

#include "ActuatorValues.h"
#include "Arm.h"
#include <vector>

static HardwareSerial& XLSerial = Serial1;

enum StepperDirection {
	UP, DOWN
};

class ActuatorsMgr : public Singleton<ActuatorsMgr>
{
private:
    StepperDirection leftDirection;
    StepperDirection rightDirection;
	volatile int leftStepCount;
	volatile int rightStepCount;
   /* Stepper leftStepper = Stepper(STEP_PIN_LEFT, DIR_PIN_LEFT);
    Stepper rightStepper = Stepper(STEP_PIN_RIGHT, DIR_PIN_RIGHT);
    StepControl<> stepControl = StepControl<>();*/

public:
    //Gestion des XL430
    DynamixelManager* dynamixelManager = new DynamixelManager(&XLSerial, &DebugSerial);

    // Liste des moteurs du bras 1
	XL430* motor1 = (XL430*) dynamixelManager->createMotor(1, XL430GeneratorFunction);//new XL430(1,*manager);
	XL430* motor2 = (XL430*) dynamixelManager->createMotor(2, XL430GeneratorFunction);//new XL430(2,*manager);
	XL430* motor3 = (XL430*) dynamixelManager->createMotor(3, XL430GeneratorFunction);//new XL430(3,*manager);
	// Liste des moteurs du bras 2
	XL430* motor4 = (XL430*) dynamixelManager->createMotor(4, XL430GeneratorFunction);//new XL430(4,*manager);
	XL430* motor5 = (XL430*) dynamixelManager->createMotor(5, XL430GeneratorFunction);//new XL430(5,*manager);
	XL430* motor6 = (XL430*) dynamixelManager->createMotor(6, XL430GeneratorFunction);//new XL430(6,*manager);

	Arm* leftArm = new Arm("left", *dynamixelManager, *motor1, *motor2, *motor3);
	Arm* rightArm = new Arm("right", *dynamixelManager, *motor4, *motor5, *motor6);

	ActuatorsMgr();
	~ActuatorsMgr();

	/**
    * Appelé tous les 800 µs pour faire bouger les steppers
    */
	void handleInterrupt();
	void initPWMs();
	void initTorques();
	void moveLeftStepper(int32_t count);
	void moveRightStepper(int32_t count);

};

void stepperInterrupt();
#endif