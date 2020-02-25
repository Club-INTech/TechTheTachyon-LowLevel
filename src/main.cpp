/**
*Initialisation et boucle principale du programme
*
* @author caillou, sylvain, rémi, melanie, Ug
*
**/

#include "Utils/Monitoring.h"
#include "Config/PinMapping.h"
#include "COM/InterruptStackPrint.h"
#include "COM/Order/OrderManager.h"

//#include "MotionControlSystem/HardwareEncoder_ISRDEF.h"

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
	/*************************
	 * Initialisation du LL, gère:
	 * La série
	 * Les actionneurs
	 * L'asservissement
	 *************************/

    /* Série */
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

    /* InterruotStackPrint */
    InterruptStackPrint& interruptStackPrint = InterruptStackPrint::Instance();

    // MotionControlSystem interrupt on timer
    IntervalTimer motionControlInterruptTimer;
    motionControlInterruptTimer.priority(0);
    motionControlInterruptTimer.begin(motionControlInterrupt, MCS_PERIOD); // Setup de l'interruption d'asservissement


    // Timer pour steppers
//    IntervalTimer stepperTimer;
//    stepperTimer.priority(253);
//    stepperTimer.begin(stepperInterrupt, STEPPER_PERIOD); // Setup de l'interruption pour les steppers


	Serial.println("Starting...");
	pinMode(LED_BUILTIN,OUTPUT);
	digitalWrite(LED_BUILTIN,HIGH);
//    delay(3000);//Laisse le temps aux capteurs de clignotter leur ID
    digitalWrite(LED_BUILTIN,LOW);
    ActuatorsMgr::Instance().initTorques();


    Serial.println("Ready!");
	/**
	 * Boucle principale, y est géré:
	 * La communication HL
	 * L'execution des ordres de ce dernier
	 * Les capteurs
	 */
//
    delay(200);

////    delay(200);

    int i = 0;
//    orderMgr.execute("montlhery");
//    orderMgr.execute("ct0");
//    orderMgr.execute("cr0");
//    orderMgr.execute("cv0");
//
//    delay(1000);
//
//    digitalWrite(INA_LEFT, LOW);
//    digitalWrite(INB_LEFT, HIGH);
//
//    digitalWrite(INA_RIGHT, LOW);
//    digitalWrite(INB_LEFT, HIGH);
//
//    analogWrite(PIN_PWM_LEFT, 255);
//    analogWrite(PIN_PWM_RIGHT, 255);
//    orderMgr.execute("t 1.57");

//    orderMgr.execute("d 1000");

//    orderMgr.execute("d 1000");
//    orderMgr.execute("goto 500 500");


    int stopTick = 110;

    while (true) {
//
        i++;
//        orderMgr.execute("rawposdata");
        delay(20);



        interruptStackPrint.print();
        orderMgr.communicate();
//        if(i==stopTick){
//            orderMgr.execute("sstop");
//            orderMgr.execute("stop");
//        }
//        if (i==stopTick*2)
//        {
//            Serial.println("DATAEND");
//        }
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
