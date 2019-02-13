/**
 * pid.hpp
 *
 * Classe PID : impl�mente un r�gulateur PID (proportionnel int�gral d�riv�)
 *
 * Auteur : Paul BERNIER - bernier.pja@gmail.com
 */

#ifndef PID_HPP
#define PID_HPP

#include <stdint.h>
#include "Utils/utils.h"

template <typename T>
class PID
{
public:


	PID<T>(volatile T* input, volatile T* output, volatile T* setPoint)
	{
		this->output = output;
		this->input = input;
		this->setPoint = setPoint;

		setOutputLimits(-2147483647, 2147483647);
		setTunings(0, 0, 0);
		epsilon = 0;
		pre_error = 0;
		derivative = 0;
		integral = 0;
		resetErrors();
	}

	void compute() {

		T error = (*setPoint) - (*input);
		derivative = error - pre_error;
		integral += error;
		if( AWU_enabled && integral > integral_max_value )
			integral = integral_max_value;
		pre_error = error;

		T result = (T)(
				kp * error + ki * integral + kd * derivative);

		//Seuillage de la commande
		if (ABS(result) < epsilon)
			result = 0;

		(*output) = result;
	}

	void setTunings(float kp, float ki, float kd, float integral_max_value = 0) {
		if (kp < 0 || ki < 0 || kd < 0)
			return;

		this->integral_max_value = integral_max_value;
		this->kp = kp;
		this->ki = ki;
		this->kd = kd;
	}

	void setOutputLimits(T min, T max) {
		if (min >= max)
			return;

		outMin = min;
		outMax = max;

		if ((*output) > outMax)
			(*output) = outMax;
		else if ((*output) < outMin)
			(*output) = outMin;
	}

	T getOutputLimit() const {
		return outMax;
	}

	void setEpsilon(T seuil) {
		if(seuil < 0)
			return;
		epsilon = seuil;
	}

	T getEpsilon() const {
		return epsilon;
	}

	void resetErrors() {
		pre_error = 0;
		integral = 0;
	}
	float getKp() const {
		return kp;
	}
	float getKi() const {
		return ki;
	}
	float getKd() const {
		return kd;
	}
	T getAWU() const {
		return integral_max_value;
	}

	T getError() const {
		return pre_error;
	}

	T getDerivativeError() const {
		return derivative;
	}

	T getIntegralErrol() const {
		return integral;
	}

	void enableAWU(bool b) {
		AWU_enabled = b;
	}

	int getInput(){
		return (int)(*input);
	}
	int getOutput(){
		return (int)(*output);
	}
	int getSet(){
		return (int)(*setPoint);
	}

private:

	float kp;
	float ki;
	float kd;

	volatile T* input; 	//Valeur du codeur
	volatile T* output; 	//Output : pwm
	volatile T* setPoint; //Valeur à atteindre

	T epsilon;
	T outMin, outMax;

	T pre_error;
	T derivative;
	T integral;

	float integral_max_value = 0;
	bool AWU_enabled = false;
};

#endif
