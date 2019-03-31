#include "SensorMgr.h"

SensorMgr::SensorMgr()
	:highLevel(ComMgr::Instance())
{
}

void SensorMgr::init() {
	pinMode(PIN_JMPR,INPUT_PULLUP);

	Wire.begin();

	for(int i = 0 ; i < NBR_OF_DISTANCE_SENSOR; i++) {
		distanceSensors[i] = SICKDT35_B15851(SICK_PINS[i], 50, 1074);
	}

	jumperPlugged = isJumperEngaged();
	basicBlocked = false;
}

//Contacteurs et Jumper

bool SensorMgr::isJumperEngaged()
/**
 * Check l'état du jumper
 * @return : Vrai si le jumper est présent
 */
{
	return !digitalRead(PIN_JMPR);				// Inversé car le switch descend à faux quand il est inséré
}

bool SensorMgr::isReadyToGo()
/**
 * Vérifie si on doit lancer le match
 * @return : Vrai si on lance le match
 */
{
	if(jumperPlugged)							// Si le jumper était présent au test précédent
	{
		jumperPlugged = isJumperEngaged();
		return(!jumperPlugged);					// Alors on part si il ne l'est plus à ce test
	}
	jumperPlugged = isJumperEngaged();
	return(false);								// Sinon on ne part pas de toutes façons
}

SICKDT35_B15851& SensorMgr::getDistanceSensor(size_t index) {
	return distanceSensors[index];
}