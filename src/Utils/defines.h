// defines.h

#ifndef _DEFINES_h
#define _DEFINES_h

#include <Arduino.h>


/**
*COMMUNICATION
*/

//Utilise la s�rie si true, l'ethernet si false
#define DEBUG true

//Nombre d'octets acceptables depuis le HL
#define RX_BUFFER_SIZE	64		//Taille max des messages
#define RX_WORD_COUNT	10		//Nombre de mots max par ordre

//Divers headers
#define HEADER_LENGTH   2

#define DEBUG_HEADER    {0x02, 0x20}
#define EVENT_HEADER    {0x13, 0x17}
#define SENSOR_HEADER	{0x01, 0x10}
#define POSITION_HEADER	{0x07, 0x05}

//Separateurs des mots d'ordres
#define SEPARATOR " "


/**
*ASSERVISSEMENT
*/

#define MC_FREQUENCY 1000 //1kHz, � modifier
#define MC_PERIOD	(double)(1000000/MC_FREQUENCY)  //Dur�e en �s entre deux mesures

// 65,5mm diametre des roues
// ~1000 ticks par tour de roue
// 17cm ?cartement des roues

#define RAYON_COD_GAUCHE 140
#define RAYON_COD_DROITE 144
#define TICK_TO_MM 0.2088			// unit? : mm/ticks

//#define TICK_TO_RADIAN 0.0014569	// unit? : radians/ticks
#define TICK_TO_RADIAN (TICK_TO_MM/RAYON_COD_GAUCHE)

#define AVERAGE_SPEED_SIZE	25
#define AVERAGE_DERIVATIVE_SIZE 100

#define WHEEL_DISTANCE_TO_CENTER 145.1
#define TOLERANCY 50

#endif