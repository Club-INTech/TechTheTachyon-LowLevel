#ifndef _PIN_MAPPING_h
#define _PIN_MAPPING_h

//TODO:pin mapping

/* Ethernet */
//Ne pas oublier: PIN13 (LED) utilisée par l'ethernet
#define PWD 24
#define RST 9

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

    // CONT
#define PIN_JMPR 0
#define PIN_CONT1 0

// TechTheTachyon
/* Ascenceurs */
constexpr uint8_t STEP_PIN_RIGHT = 1; //vitesse
constexpr uint8_t RST_PIN_RIGHT = 14; //reset
constexpr uint8_t DIR_PIN_RIGHT = 4; //direction

constexpr uint8_t STEP_PIN_LEFT = 8; //vitesse
constexpr uint8_t RST_PIN_LEFT = 14; //reset
constexpr uint8_t DIR_PIN_LEFT = 3; //direction

#endif //_PIN_MAPPING_h
