#ifndef _PIN_MAPPING_h
#define _PIN_MAPPING_h

//TODO:pin mapping

/* Ethernet */
//Ne pas oublier: PIN13 (LED) utilisée par l'ethernet
#define PWD 24
#define RST 9

/* Serial RXTX */
	//AX
#define RX1 31
#define TX1 32

/* Codeuses */
	//Gauche
#define PIN_A_LEFT_ENCODER 18
#define PIN_B_LEFT_ENCODER 19
	//Droite
#define PIN_A_RIGHT_ENCODER 20
#define PIN_B_RIGHT_ENCODER 21

/* Moteurs */
	//Gauche
#define PIN_DIR_LEFT 5
#define PIN_PWM_LEFT 6
	//Droite
#define PIN_DIR_RIGHT 7
#define PIN_PWM_RIGHT 8

/* Electrovannes */
#define PIN_ELECTROVANNE_AV 2
#define PIN_ELECTROVANNE_AR 4

/* Pompes à vide */
#define PUMP_PIN_RIGHT 1
#define PUMP_PIN_LEFT 2



/* CAPTEURS */
    // IR
#define PIN_CUBE_AR_DETECTION_SLEEP 14

    // CONT
#define PIN_JMPR 0
#define PIN_CONT1 0

/* Ascenceurs */
#define STEP_PIN_RIGHT 18 //vitesse
#define RST_PIN_RIGHT 19 //reset
#define DIR_PIN_RIGHT 20 //direction

#define STEP_PIN_LEFT 18 //vitesse
#define RST_PIN_LEFT 19 //reset
#define DIR_PIN_LEFT 20 //direction

#endif //_PIN_MAPPING_h
