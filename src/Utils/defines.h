// defines.h

#ifndef _DEFINES_h
#define _DEFINES_h

#include <Arduino.h>
#include "define_com_options.h"

/**
*COMMUNICATION
*/


// Nombre d'octets acceptables depuis le HL
constexpr uint8_t RX_BUFFER_SIZE = 64; // Taille max des messages
constexpr uint8_t RX_WORD_COUNT = 10; // Nombre de mots max par ordre

constexpr uint8_t NBR_OF_US_SENSOR = 0;
constexpr uint8_t NBR_OF_DISTANCE_SENSOR = 6;

// Divers headers de communication pour l'éthernet
constexpr uint8_t HEADER_LENGTH = 2;

using Header = const char[HEADER_LENGTH];
Header STD_HEADER = {0x40,0x40};
Header DEBUG_HEADER = {0x02,0x20};
Header SENSOR_HEADER = {0x01,0x06};
Header POSITION_HEADER = {0x07,0x05};

Header SICK_HEADER = { 0x20, 0x25 };


// Séparateurs des mots d'ordres
#define SEPARATOR  " "

// Fréquence d'envoi de la position
constexpr uint8_t F_ENV_POS = 50;


/**
* Asservissement
*/

constexpr uint16_t  MC_FREQUENCY =              1000; //1Khz
constexpr double    MC_PERIOD = 1000000.0 / MC_FREQUENCY; // Durée en µs entre deux mesures
constexpr uint16_t  STEPPER_FREQUENCY = 625/2; // 625/2 Hz
constexpr double    STEPPER_PERIOD = 1000000.0 / STEPPER_FREQUENCY; // Durée en µs entre deux mesures

constexpr uint16_t  TICKS_PER_TURN =            1024;   // Unité : ticks
constexpr float     COD_WHEEL_DIAMETER =        65.948;  // Unité : mm

constexpr uint8_t   DISTANCE_COD_GAUCHE_CENTRE = 112; // Unité : mm
constexpr uint8_t   DISTANCE_COD_DROITE_CENTRE = 111; // Unité : mm

constexpr float     TICK_TO_MM = static_cast<float>(PI*COD_WHEEL_DIAMETER/TICKS_PER_TURN); // Unité : mm/ticks
constexpr float     TICK_TO_RADIAN = TICK_TO_MM / DISTANCE_COD_GAUCHE_CENTRE; // Unité : rad/ticks

// Nombre de valeurs par moyenne
constexpr uint8_t   MEDIAN_US_SIZE =            3;

/**
* Capteurs
*/

/**
 * Steppers
 */
const unsigned int ELEVATOR_TEMPO = 800; //temporistaion entre les commandes du pas à pas
const unsigned int STEP_COUNT = 700; //nombre de pas par palet
#endif
