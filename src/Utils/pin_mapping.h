#ifndef _PIN_MAPPING_h
#define _PIN_MAPPING_h

/* Ethernet */
constexpr uint8_t PWD = 24;
constexpr uint8_t RST = 9;

/* Moteurs */
constexpr uint8_t PIN_DIR_LEFT = 7;
constexpr uint8_t PIN_PWM_LEFT = 8;

constexpr uint8_t PIN_DIR_RIGHT = 36;
constexpr uint8_t PIN_PWM_RIGHT = 35;

/* CAPTEURS */
constexpr uint8_t PIN_JMPR = 99;
constexpr uint8_t PIN_CONT1 = 99;
constexpr uint8_t PIN_SICK_1 = 15;
constexpr uint8_t PIN_SICK_2 = 16;
constexpr uint8_t PIN_SICK_3 = 17;
constexpr uint8_t PIN_SICK_4 = 18;
constexpr uint8_t PIN_SICK_5 = 19;
constexpr uint8_t PIN_SICK_6 = 20;

// TechTheTachyon
/* Ascenceurs */
constexpr uint8_t STEP_PIN_RIGHT = 99; //vitesse
constexpr uint8_t RST_PIN_RIGHT = 99; //reset
constexpr uint8_t DIR_PIN_RIGHT = 99; //direction

constexpr uint8_t STEP_PIN_LEFT = 99; //vitesse
constexpr uint8_t DIR_PIN_LEFT = 99; //direction
constexpr uint8_t RST_PIN_LEFT = 99; //reset

constexpr uint8_t RIGHT_VALVE_PIN = 99; // électrovanne droite
constexpr uint8_t LEFT_VALVE_PIN = 99;  // électrovanne gauche

constexpr uint8_t LEFT_PUMP_PIN = 99;
constexpr uint8_t RIGHT_PUMP_PIN = 99;


#endif //_PIN_MAPPING_h
