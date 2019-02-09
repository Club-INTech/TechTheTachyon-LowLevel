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
#include "COM/ComMgr.h"
#include "AbstractSensorUS.h"
#include "MCS/RobotStatus.h"
#include "SICKDT35_B15851.h"


class SensorMgr : public Singleton<SensorMgr>
{


private:

	ComMgr& highLevel;

	SICKDT35_B15851 distanceSensors[NBR_OF_DISTANCE_SENSOR];
	MOVEMENT measure_direction;

	bool jumperPlugged;
	bool basicBlocked;
public:

	SensorMgr();
	void init();

	bool isJumperEngaged();
	bool isReadyToGo();
	bool isCont1Engaged();

	/**
	 * Récupère le capteur à l'indice donné
	 * @param index l'indice du capteur
	 * @return
	 */
	SICKDT35_B15851& getDistanceSensor(size_t index);
};

#endif
