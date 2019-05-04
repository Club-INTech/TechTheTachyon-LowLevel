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
#include "../Utils/Singleton.hpp"
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
    DynamixelManager* dynamixelManager;

    // Liste des moteurs du bras 1
    AX12* motor1 = (AX12*) dynamixelManager->createMotor(1, AX12GeneratorFunction);//new XL430(1,*manager);
    AX12* motor2 = (AX12*) dynamixelManager->createMotor(2, AX12GeneratorFunction);//new XL430(2,*manager);
    AX12* motor3 = (AX12*) dynamixelManager->createMotor(3, AX12GeneratorFunction);//new XL430(3,*manager);
	//XL qui pousse les palets
	XL430* motor7 = (XL430*) dynamixelManager->createMotor(7, XL430GeneratorFunction);//new XL430(7,*manager);

	Arm* rightArm = new Arm(*dynamixelManager, *motor1, *motor2, *motor3);
	Arm* robot2Arm = new Arm(*dynamixelManager, *motor1, *motor2, *motor3);

	ActuatorsMgr();
	~ActuatorsMgr();

	/**
    * Appelé tous les 800 µs pour faire bouger les steppers
    */
	void handleInterrupt();
	void initPWMs();
	void initTorques();
	void moveRightStepper(int32_t count);

};

void stepperInterrupt();
#endif