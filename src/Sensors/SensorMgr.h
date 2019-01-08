// SensorMgr.h

#ifndef _SENSORMGR_h
#define _SENSORMGR_h

#include <Arduino.h>
#include <array>
#include <algorithm>
#include <i2c_t3.h>
//#include <Wire.h>

#include "Utils/Median.h"
#include "Utils/Singleton.hpp"
#include "Utils/pin_mapping.h"
#include "MCS/MCSReborn.h"
#include "Utils/pin_mapping.h"
#include "COM/ComMgr.h"
#include "AbstractSensorUS.h"
#include "MCS/RobotStatus.h"


class SensorMgr : public Singleton<SensorMgr>
{


private:

	ComMgr& highLevel;

	std::array<Median<uint16_t ,MEDIAN_US_SIZE>,NBR_OF_US_SENSOR> distances;
	MOVEMENT measure_direction;

	bool jumperPlugged;
	bool basicBlocked;
public:

	SensorMgr();
	void init();

	bool isJumperEngaged();
	bool isReadyToGo();
	bool isCont1Engaged();
};

#endif
