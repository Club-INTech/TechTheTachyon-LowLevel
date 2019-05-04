/**
*Initialisation et boucle principale du programme
*
* @author caillou, sylvain, rémi, melanie, Ug
*
**/

#include "COM/Order/OrderManager.h"
#include "Utils/Monitoring.h"
#include <string>
#include "Utils/pin_mapping.h"
//#include "MCS/HardwareEncoder_ISRDEF.h"

/* Interruptions d'asservissements */
void motionControlInterrupt() {
	static MCS &motionControlSystem = MCS::Instance();
	motionControlSystem.control();
	motionControlSystem.manageStop();
}

int main() {
	/*************************
	 * Initialisation du LL, gère:
	 * La série
	 * Les actionneurs
	 * L'asservissement
	 *************************/

    /* Série */
	ActuatorsMgr::Instance().initPWMs();
    SensorMgr::Instance().init();


    Serial.flush();
	Serial.println("Série OK");
	delay(250);

	/* Actuators */
	// Par sécurité on met tout les actuators à LOW quand on les initialise
	/* Pompe */
	pinMode(RIGHT_PUMP_PIN,OUTPUT);
	digitalWrite(RIGHT_PUMP_PIN,LOW);

	/* Electrovanne */
	pinMode(LEFT_VALVE_PIN,OUTPUT);
	digitalWrite(LEFT_VALVE_PIN,LOW);
	pinMode(RIGHT_VALVE_PIN,OUTPUT);
	digitalWrite(RIGHT_VALVE_PIN,LOW);

	Serial.println("Fin du setup");
	OrderManager& orderMgr = OrderManager::Instance();
	orderMgr.init();
    Serial.println("Order manager ok");

    // MotionControlSystem interrupt on timer
    IntervalTimer motionControlInterruptTimer;
    motionControlInterruptTimer.priority(253);
    motionControlInterruptTimer.begin(motionControlInterrupt, MCS_PERIOD); // Setup de l'interruption d'asservissement


    // Timer pour steppers
    IntervalTimer stepperTimer;
    stepperTimer.priority(253);
    stepperTimer.begin(stepperInterrupt, STEPPER_PERIOD); // Setup de l'interruption pour les steppers


    Serial.println("Starting 5s wait");
    delay(5000);//Laisse le temps aux capteurs de clignotter leur ID
    ActuatorsMgr::Instance().initTorques();

    Serial.println("Ready!");
    delay(60);
	/**
	 * Boucle principale, y est géré:
	 * La communication HL
	 * L'execution des ordres de ce dernier
	 * Les capteurs
	 */
    //orderMgr.execute("montlhery");

	int i=0;

    while (true) {
        orderMgr.communicate();
       // orderMgr.execute("cod");
//		orderMgr.refreshUS();
//		orderMgr.isHLWaiting() ? orderMgr.checkJumper() : void();

		orderMgr.execute("rawposdata");
		//orderMgr.execute("cod");

		delay(60);

    	if (i==5){
    	    orderMgr.execute("t 0.9");
    	    //orderMgr.execute("d 100");
    	}
    	if (i==150){
			Serial.println("DATAEND");
    	}
		i++;
    }
}

                   /*``.           `-:--.`
                  `.-::::/.        .:::::::y
                 `::::::::o``````.-::::::/h-
                 `::::::::::::::::::::::/h.
                  -::::::://++++//::::::::.
               `-::::/oso+/:-....--://:::::-.``.---:`
      ````   .-:::/so/-      ``````  `.:::::::::::::+ ``````````````````````````````````````````````````````````````````````
    `-:::::--:::os+.       `/sssssss`   ./syyyyyys+/y+sssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssssy- `-:::://
   `-:::::::::+y/`        .shhhhhdd:   `/hhhhhhhhhdo:/dddddddddddddddddhhhhhhhdddddddddddddddddddddddddddddddddddddddddddddo`.:////sy.
   ::::::::::so`         :yhhhhhmy`   .ohhhhhhhhhhd  `//oooooo+//////+hhhhhhdd+///////+++++++/////////////////+++++++/////-`-////+h+` `
   `://:::::y/         `+hhhhhdm+    -yhhhhhdhhhhhm`  `oyyyyyhs     .shhhhhdh-  ``.-:::////:::-.       ``.-::::////::://  `://///o+:::::::-.
      `::::y/         .shhhhhdh-   `+hhhhhdd+shhhhd- -yhhhhhmo`    /yhhhhhms` .-://+ssso+++/////o   `.:://+ossoo+++o+oy- -/////+ssoo+//////h
      .:::/y         :yhhhhhms`   .shhhhhdy::hhhhhho+hhhhhdd:    `ohhhhhdd/ .:///oyo::-----////oy `-:///oyo:.`      `-``:////oy+-` `:////+h:
      -:::o:       `+hhhhhdm/    :yhhhhddo:::+hhhhhhhhhhhms.    -yhhhhhdh. -////yo.`ooooooooooss``:////yo.            -/////yo`   .:////ys.
   ``.::::+-      .shhhhhdh-   `+hhhhhdd/::::/hhhhhhhhhdd/     /hhhhhhmo` `/////s```..----:+:..  -/////o``````..:.  `:////oh:   `-////+h/
`.-::::::::+     :yhhhhhms`   .shhhhhmy:::::::hhhhhhhhdh.    `ohhhhhdd:    :++////:::///+oys`    `/++/////://++yo` .:////ys`   .:////ys.
-::::::::::/-    /oooooo/     -sssyyyo:+o/++//hyssssss+`     .sssssss.      `-/++oooo++//:.        .:/+oooo++/:-   /ooooo:     :ooooo/
.:::::/o:::::-`               `.--:::os+```.---
 :+ooo/.`::::::-..`````````.--::::+ss:`
  ``     `-/::::::::::::::::::::::s.
          `:::::::::::://+::::::::o-
         `:::::::/h////::.-:::::::y-
         :::::::ss`        -:/+sso:
         .:/++sy:          `//*/


/*
 *   Dead Pingu in the Main !
 *      	  . --- .
		    /        \
		   |  X  _  X |
		   |  ./   \. |
		   /  `-._.-'  \
		.' /         \ `.
	.-~.-~/    o   o  \~-.~-.
.-~ ~    |    o  o     |    ~ ~-.
`- .     |      o  o   |     . -'
	 ~ - |      o      | - ~
	 	 \             /
	 	__\           /___
	 	~;_  >- . . -<  _i~
	 	  `'         `'
*/
