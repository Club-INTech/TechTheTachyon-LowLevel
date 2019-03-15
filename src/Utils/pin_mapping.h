#ifndef _PIN_MAPPING_h
#define _PIN_MAPPING_h

#include "defines.h"

/* Ethernet */
constexpr uint8_t CS = 9;
constexpr uint8_t RST = 29;
constexpr uint8_t LED_DEBUG_ETH = 2;

/* Moteurs */
constexpr uint8_t INA_LEFT = 2;
constexpr uint8_t INB_LEFT = 16;
constexpr uint8_t PIN_PWM_LEFT = 5;

constexpr uint8_t INA_RIGHT = 28;
constexpr uint8_t INB_RIGHT = 27;
constexpr uint8_t PIN_PWM_RIGHT = 8;

/* Roues codeuses */
constexpr uint8_t ENCODER_LEFT_A = 3;
constexpr uint8_t ENCODER_LEFT_B = 4;
constexpr uint8_t ENCODER_RIGHT_A = 25;
constexpr uint8_t ENCODER_RIGHT_B = 32;

/* CAPTEURS */
constexpr uint8_t PIN_JMPR = 10;
constexpr uint8_t SICK_PINS[NBR_OF_DISTANCE_SENSOR] = {
        A22, // 1er
        A21, // 2e
        A20, // 3e
        A19, // 4e
        A18, // 5e
        A17, // 6e
};

// TechTheTachyon
/* Ascenceurs */
constexpr uint8_t STEP_PIN_RIGHT = 17; //vitesse
constexpr uint8_t DIR_PIN_RIGHT = 20; //direction
constexpr uint8_t RST_PIN_RIGHT = 21; //reset

constexpr uint8_t STEP_PIN_LEFT = 22; //vitesse
constexpr uint8_t DIR_PIN_LEFT = 23; //direction
constexpr uint8_t RST_PIN_LEFT = 21; //reset

constexpr uint8_t RIGHT_VALVE_PIN = 24; // électrovanne droite
constexpr uint8_t LEFT_VALVE_PIN = 26;  // électrovanne gauche

constexpr uint8_t LEFT_PUMP_PIN = 6;
constexpr uint8_t RIGHT_PUMP_PIN = 7;




#endif //_PIN_MAPPING_h
