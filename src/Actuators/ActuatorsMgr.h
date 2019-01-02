// ActuatorsMgr.h


#ifndef _ACTUATORSMGR_h
#define _ACTUATORSMGR_h

#include "Arduino.h"

#include "DynamixelManager.h"

#include "Utils/pin_mapping_select.h"
#include "Utils/Singleton.hpp"

static HardwareSerial& XLSerial = Serial1;
static HardwareSerial& DebugSerial = Serial2;

class ActuatorsMgr : public Singleton<ActuatorsMgr>
{
private:


public:

	ActuatorsMgr();
	~ActuatorsMgr();


	//Gestion des AX12
    DynamixelManager* dynamixelManager;

};

#endif