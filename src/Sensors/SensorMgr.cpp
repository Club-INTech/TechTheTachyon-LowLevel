#include "SensorMgr.h"

SensorMgr::SensorMgr()
	:highLevel(ComMgr::Instance())
{
}

void SensorMgr::init() {
	pinMode(PIN_JMPR,INPUT_PULLUP);

	Wire.begin();

	/* CHANGEMENT PIN I2C */
	CORE_PIN18_CONFIG = 0;  // turn off primary pins before enable alternates
	CORE_PIN19_CONFIG = 0;
	CORE_PIN16_CONFIG = PORT_PCR_MUX(2)|PORT_PCR_ODE|PORT_PCR_SRE|PORT_PCR_DSE;
	CORE_PIN17_CONFIG = PORT_PCR_MUX(2)|PORT_PCR_ODE|PORT_PCR_SRE|PORT_PCR_DSE;

	for(int i = 0;i<NBR_OF_DISTANCE_SENSOR;i++)
    	distanceSensors[i] = SICKDT35_B15851(SICK_PINS[i]);

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

bool SensorMgr::isCont1Engaged()
{
	return digitalRead(PIN_CONT1);
}

SICKDT35_B15851& SensorMgr::getDistanceSensor(size_t index) {
	return distanceSensors[index];
}