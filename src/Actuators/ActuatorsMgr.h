// ActuatorsMgr.h


#ifndef _ACTUATORSMGR_h
#define _ACTUATORSMGR_h

#include "Arduino.h"

#include "DynamixelManager.h"
#include "XL430.h"
#include "SyncWrite.h"

#include "Utils/pin_mapping_select.h"
#include "Utils/Singleton.hpp"
#include "ActuatorValues.h"
#include "Arm.h"
#include <vector>

static HardwareSerial& XLSerial = Serial1;

class ActuatorsMgr : public Singleton<ActuatorsMgr>
{
private:


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

	Arm* leftArm = new Arm(*dynamixelManager, *motor4, *motor5, *motor6);
	Arm* rightArm = new Arm(*dynamixelManager, *motor1, *motor2, *motor3);

	ActuatorsMgr();
	~ActuatorsMgr();


};

#endif