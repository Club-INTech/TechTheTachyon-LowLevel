#ifndef _PIN_MAPPING_TEST_CARD_h
#define _PIN_MAPPING_TEST_CARD_h

//TODO:pin mapping

/* Ethernet */
//Ne pas oublier: PIN13 (LED) utilisée par l'ethernet
#define PWD 15
#define RST 14

/* Moteurs */

// FIXME: Pins pour Nocturne2
//Gauche
#define PIN_DIR_LEFT 2
#define PIN_PWM_LEFT 16
//Droite
#define PIN_DIR_RIGHT 4
#define PIN_PWM_RIGHT 17

/* CAPTEURS */
//CONT
#define PIN_JMPR 0
#define PIN_CONT1 0


// TechTheTachyon
/* Ascenceurs */
constexpr uint8_t STEP_PIN_RIGHT = 1; //vitesse
constexpr uint8_t RST_PIN_RIGHT = RST; //reset
constexpr uint8_t DIR_PIN_RIGHT = 4; //direction

constexpr uint8_t STEP_PIN_LEFT = 8; //vitesse
constexpr uint8_t DIR_PIN_LEFT = 3; //direction
constexpr uint8_t RST_PIN_LEFT = RST; //reset
constexpr uint8_t RIGHT_VALVE_PIN = 5; // électrovanne droite

constexpr uint8_t LEFT_VALVE_PIN = 0; /*FIXME*/ // électrovanne gauche

// TODO: PWM ou pas ?
constexpr uint8_t LEFT_PUMP_PIN = 0; // TODO
constexpr uint8_t RIGHT_PUMP_PIN = 0; // TODO

constexpr uint8_t LEFT_ENCODER_A = 28;
constexpr uint8_t LEFT_ENCODER_B = 29;
constexpr uint8_t RIGHT_ENCODER_A = 21;
constexpr uint8_t RIGHT_ENCODER_B = 20;
#endif //_PIN_MAPPING_TEST_CARD_h