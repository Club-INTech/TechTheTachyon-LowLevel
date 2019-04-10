#ifndef _PIN_MAPPING_h
#define _PIN_MAPPING_h

#include "defines.h"

/* Ethernet */
constexpr uint8_t CS = 10;
constexpr uint8_t RST = 24;


/* Moteurs */
constexpr uint8_t INA_LEFT = 28;
constexpr uint8_t INB_LEFT = 27;
constexpr uint8_t PIN_PWM_LEFT = 25;

constexpr uint8_t INA_RIGHT = 8;
constexpr uint8_t INB_RIGHT = 9;
constexpr uint8_t PIN_PWM_RIGHT = 7;

/* Roues codeuses */
constexpr uint8_t ENCODER_LEFT_B = 3;
constexpr uint8_t ENCODER_LEFT_A = 4;
constexpr uint8_t ENCODER_RIGHT_B = 29;
constexpr uint8_t ENCODER_RIGHT_A = 30;

/* CAPTEURS */
constexpr uint8_t PIN_JMPR = 10;
constexpr uint8_t SICK_PINS[NBR_OF_DISTANCE_SENSOR] = {
        15, // 1er
        16, // 2e
        17, // 3e
};

// TechTheTachyon
/* Ascenceurs */

// ! Inversées par rapport au pin mapping !
constexpr uint8_t STEP_PIN_RIGHT =5; //vitesse
constexpr uint8_t DIR_PIN_RIGHT = 6; //direction


// ! Inversées par rapport au pin mapping !

constexpr uint8_t RIGHT_VALVE_PIN = 31; // électrovanne droite
constexpr uint8_t LEFT_VALVE_PIN = 26; // électrovanne gauche

constexpr uint8_t RIGHT_PUMP_PIN = 32;

/* LEDs RGB debug ATTENTION logique inversée*/
constexpr uint8_t LED1_1 = 35;
constexpr uint8_t LED1_2 = 36;
constexpr uint8_t LED1_3 = 37;
constexpr uint8_t LED2_1 = 38;
constexpr uint8_t LED2_2 = 14;
constexpr uint8_t LED2_3 = 20;
constexpr uint8_t LED3_1 = 21;
constexpr uint8_t LED3_2 = 22;
constexpr uint8_t LED3_3 = 23;


#endif //_PIN_MAPPING_h
