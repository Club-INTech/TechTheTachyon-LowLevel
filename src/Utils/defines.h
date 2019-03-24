// defines.h

#ifndef _DEFINES_h
#define _DEFINES_h

#include <Arduino.h>
#include "define_com_options.h"

/**
*COMMUNICATION
*/


constexpr uint32_t MIN_TIME_BETWEEN_GOTO_TR_ROT = 500; // en ticks d'asserv'
// Nombre d'octets acceptables depuis le HL
constexpr uint8_t RX_BUFFER_SIZE = 64; // Taille max des messages
constexpr uint8_t RX_WORD_COUNT = 10; // Nombre de mots max par ordre

constexpr uint8_t INTERRUPT_PRINT_STACK_MAX_SIZE = 20;

constexpr uint8_t NBR_OF_US_SENSOR = 0;
constexpr uint8_t NBR_OF_DISTANCE_SENSOR = 6;

// Divers headers de communication pour l'éthernet
constexpr uint8_t HEADER_LENGTH = 2;

using Header = const char[HEADER_LENGTH];
Header STD_HEADER = {0x40,0x40};
Header DEBUG_HEADER = {0x40,0x43};
Header SENSOR_HEADER = {0x01,0x06};
Header POSITION_HEADER = {0x07,0x05};
Header EVENT_HEADER = {0x40,0x42};

Header SICK_HEADER = { 0x40,0x41 };
Header ATOM_COLOR_HEADER = { 0x20, 0x26 };


// Séparateurs des mots d'ordres
#define SEPARATOR  " "

// Fréquence d'envoi de la position
constexpr uint8_t F_ENV_POS = 50;


/**
* Asservissement
*/

constexpr uint16_t  MCS_FREQ = 1000; //1Khz
constexpr double    MCS_PERIOD = 1000000.0 / MCS_FREQ; // Durée en µs entre deux mesures
constexpr uint16_t  STEPPER_FREQUENCY = 625; // 625/2 Hz
constexpr double    STEPPER_PERIOD = 1000000.0 / STEPPER_FREQUENCY; // Durée en µs entre deux mesures

constexpr uint16_t  TICKS_PER_TURN =            4096;   // Unité : ticks
constexpr float     COD_WHEEL_DIAMETER =        64.32;  // Unité : mm 63.57

//distance roue codeuse pneu = 14.36mm

constexpr uint8_t   DISTANCE_COD_GAUCHE_CENTRE = 117; // Unité : mm 115
constexpr uint8_t   DISTANCE_COD_DROITE_CENTRE = 115; // Unité : mm

constexpr float     TICK_TO_MM = static_cast<float>(PI*COD_WHEEL_DIAMETER/TICKS_PER_TURN); // Unité : mm/ticks
constexpr float     TICK_TO_RADIAN = TICK_TO_MM / DISTANCE_COD_GAUCHE_CENTRE; // Unité : rad/ticks

/**
* Capteurs
*/

/**
 * Steppers
 */
const unsigned int ELEVATOR_TEMPO = 800; //temporistaion entre les commandes du pas à pas
const unsigned int STEP_COUNT = 1400; //nombre de pas par palet
#endif
