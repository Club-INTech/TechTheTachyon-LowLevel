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
#include "Arm.hpp"
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
	volatile int32_t leftStepCount;
	volatile int32_t rightStepCount;
	int32_t nextLeftStepCount;
	int32_t nextRightStepCount;
	volatile int32_t timerForLeftStepper = -1;
	volatile int32_t timerForRightStepper = -1;

public:
    //Gestion des XL430
    DynamixelManager* dynamixelManager = new DynamixelManager(&XLSerial, &DebugSerial);

    // Liste des moteurs du bras 1
	XL430* motor1 = (XL430*) dynamixelManager->createMotor(1, XL430GeneratorFunction);//new XL430(1,*manager);
	XL430* motor2 = (XL430*) dynamixelManager->createMotor(2, XL430GeneratorFunction);//new XL430(2,*manager);
	XL430* motor3 = (XL430*) dynamixelManager->createMotor(3, XL430GeneratorFunction);//new XL430(3,*manager);
	//XL qui pousse les palets
	XL430* motor4 = (XL430*) dynamixelManager->createMotor(4, XL430GeneratorFunction);//new XL430(7,*manager);

	Arm<XL430>* rightArm = new Arm<XL430>("right", *dynamixelManager, new XL430[3]{*motor1, *motor2, *motor3});

	ActuatorsMgr();
	~ActuatorsMgr();

	/**
    * Appelé tous les 800 µs pour faire bouger les steppers
    */
	void handleInterrupt();
	void initPWMs();
	void initTorques();
	void moveRightStepper(int32_t count, int32_t nextCount = 0);
	void moveRightStepperOust(int32_t count, int32_t nextCount = 0);

	void checkArmMovements();

};

void stepperInterrupt();
#endif