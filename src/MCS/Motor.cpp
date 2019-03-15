#include "Motor.h"
#include "../Utils/pin_mapping.h"

void Motor::setDirection(Direction directionToSet)
{
	direction = directionToSet;
	if (side == Side::LEFT) {
		if (direction == Direction::FORWARD) {
			digitalWrite(pin_ina, LOW);
			digitalWrite(pin_inb, HIGH);
		}
		if (direction == Direction::BACKWARD) {
			digitalWrite(pin_ina, HIGH);
			digitalWrite(pin_inb, LOW);
		}
		if( direction == Direction::NONE)
		{
			digitalWrite(pin_ina, LOW);
			digitalWrite(pin_inb, LOW);
		}
		if( direction == Direction::BRAKE)
		{
			digitalWrite(pin_ina, HIGH);
			digitalWrite(pin_inb, HIGH);
		}
	}
	else {
		if (direction == Direction::FORWARD) {
			digitalWrite(pin_ina, LOW);
			digitalWrite(pin_inb, HIGH);
		}
		if (direction == Direction::BACKWARD) {
			digitalWrite(pin_ina, HIGH);
			digitalWrite(pin_inb, LOW);
		}
		if( direction == Direction::NONE)
		{
			digitalWrite(pin_ina, LOW);
			digitalWrite(pin_inb, LOW);
		}
		if( direction == Direction::BRAKE)
		{
			digitalWrite(pin_ina, HIGH);
			digitalWrite(pin_inb, HIGH);
		}
	}
}

Motor::Motor(Side definedSide):side(definedSide), direction(Direction::NONE)
{
	if (side == Side::LEFT) {
		pin_pwm = PIN_PWM_LEFT;
		pin_ina = INA_LEFT;
		pin_inb = INB_LEFT;

	}
	else if (side == Side::RIGHT) {
		pin_pwm = PIN_PWM_RIGHT;
		pin_ina = INA_RIGHT;
		pin_inb = INB_RIGHT;
	}
	pinMode(pin_ina, OUTPUT);
	pinMode(pin_inb, OUTPUT);
	pinMode(pin_pwm, OUTPUT);

	setDirection(Direction::NONE);
	pwm = 0;
}

//Initialise les pins, le pwm, bref tout ce dont le moteur a besoin
void Motor::init()
{
	//TODO: Initialiser les PWM
	analogWriteResolution(8);
	analogWriteFrequency(pin_pwm, 20000); //FIXME: A CHANGER APRES NOUVEAU PONT EN H
}

void Motor::run(int16_t newpwm)
{
	pwm = newpwm;
	if (pwm > 0) {
		setDirection(Direction::FORWARD);
		pwm = (int16_t)MIN(pwm, 255);
	}
	else if (pwm < 0) {
		setDirection(Direction::BACKWARD);
		pwm = (int16_t)MIN(-pwm, 255);
	}
	else
	{
		setDirection(Direction::NONE);
	}
	analogWrite(pin_pwm, pwm);
}

void Motor::stop() {
	run(0);
}

void Motor::brake(){
	setDirection(Direction::BRAKE);
}