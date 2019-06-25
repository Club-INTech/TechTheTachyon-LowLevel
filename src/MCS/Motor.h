/**
* Moteur.cpp
*
* Classe de gestion d'un moteur (PWM, direction...)
*
* Recapitulatif pins utilisees pour controler les deux moteurs :
*
* Gauche :
* 	-pins de sens : ??
* 	-pin de pwm : ??
* Droit :
* 	-pins de sens : ??
* 	-pin de pwm : ??
*
*/

#ifndef COMPLETE_LOW_LEVEL_MOTOR_H
#define COMPLETE_LOW_LEVEL_MOTOR_H

#include "Arduino.h"
#include "Utils/safe_enum.hpp"
#include "Utils/utils.h"
#include "Config/pin_mapping.h"
#include <stdint.h>

struct direction_def {
	enum type {
		BACKWARD, FORWARD, NONE, BRAKE
	};
};
struct side_def {
	enum type {
		LEFT, RIGHT
	};
};

typedef safe_enum<direction_def> Direction;
typedef safe_enum<side_def> Side;

class Motor {
private:
	Side side;
	Direction direction;
	uint8_t pin_pwm;
	uint8_t pin_ina;
	uint8_t pin_inb;
public:
	Motor(Side);
	int16_t pwm;
	void setDirection(Direction);
	void init();
	void run(int16_t);

	/**
	 * Stoppe le moteur.
	 *
	 * C'est juste mettre le PWM à 0 mais c'est plus propre niveau code :)
	 */
	void stop();
	void brake();
};

#endif //COMPLETE_LOW_LEVEL_MOTOR_H