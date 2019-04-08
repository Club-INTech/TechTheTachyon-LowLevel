/**
*Initialisation et boucle principale du programme
*
* @author caillou, sylvain, rémi, melanie, Ug
*
**/

#include "COM/Order/OrderManager.h"
#include "Utils/Monitoring.h"
#include "Utils/pin_mapping.h"
#include "COM/InterruptStackPrint.h"

//#include "MCS/HardwareEncoder_ISRDEF.h"

/* Interruptions d'asservissements */
void motionControlInterrupt() {
	static MCS &motionControlSystem = MCS::Instance();
	motionControlSystem.control();
	motionControlSystem.manageStop();
}

void positionInterrupt() {
	static MCS &motionControlSystem = MCS::Instance();
	motionControlSystem.sendPositionUpdate();
}

int main() {
	pinMode(LED1,OUTPUT);
	pinMode(LED2,OUTPUT);
	pinMode(LED3,OUTPUT);
	pinMode(LED4,OUTPUT);


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
	pinMode(LEFT_PUMP_PIN,OUTPUT);
	pinMode(RIGHT_PUMP_PIN,OUTPUT);
	digitalWrite(LEFT_PUMP_PIN,LOW);
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

    /* InterruotStackPrint */
    InterruptStackPrint& interruptStackPrint = InterruptStackPrint::Instance();

    // MotionControlSystem interrupt on timer
    IntervalTimer motionControlInterruptTimer;
    motionControlInterruptTimer.priority(253);
    motionControlInterruptTimer.begin(motionControlInterrupt, MCS_PERIOD); // Setup de l'interruption d'asservissement


    // Timer pour steppers
    IntervalTimer stepperTimer;
    stepperTimer.priority(253);
    stepperTimer.begin(stepperInterrupt, STEPPER_PERIOD); // Setup de l'interruption pour les steppers

	// Timer pour la mise à jour de la position
	IntervalTimer posTimer; // TODO: Passer sur un Metro?
	posTimer.priority(253);
// FIXME 	posTimer.begin(positionInterrupt, POSITION_UPDATE_PERIOD);

	Serial.println("Starting...");
    delay(2000);//Laisse le temps aux capteurs de clignotter leur ID
    ActuatorsMgr::Instance().initTorques();

    Serial.println("Ready!");
	/**
	 * Boucle principale, y est géré:
	 * La communication HL
	 * L'execution des ordres de ce dernier
	 * Les capteurs
	 */


	int i=0;

    while (true) {

        interruptStackPrint.print();
        orderMgr.communicate();
        //orderMgr.execute("cod");
//		orderMgr.refreshUS();
//		orderMgr.isHLWaiting() ? orderMgr.checkJumper() : void();
		/*orderMgr.execute("rawposdata");

		delay(10);
        orderMgr.execute("rawposdata");

        delay(10);
        orderMgr.execute("rawposdata");

        delay(10);
        orderMgr.execute("rawposdata");

        delay(10);
        orderMgr.execute("rawposdata");

        delay(10);
        orderMgr.execute("rawposdata");

        delay(10);

		if (i==0){
			orderMgr.execute("goto 1000 0");
		}
		int multiplier = 2;
		if (i==100*multiplier){
            orderMgr.execute("goto 0 0");
        }
        if (i==200*multiplier){
            orderMgr.execute("goto 1000 0");
        }
        if (i==300*multiplier){
            orderMgr.execute("goto 0 0");
        }
        /*if (i==400*multiplier){
            orderMgr.execute("goto 200 0");
        }
        if (i==500*multiplier){
            orderMgr.execute("goto 0 0");
        }*/
        /*if (i==600*multiplier){
            orderMgr.execute("d 200");
        }
        if (i==700*multiplier){
            orderMgr.execute("t 0");
        }*/
    	//if (i==1500){
    	/*if (i==600*multiplier){
			Serial.println("DATAEND");
    	}
		i++;*/
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
