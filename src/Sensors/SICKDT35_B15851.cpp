//
// Created by jglrxavpok on 14/01/19.
//

#include "Arduino.h"
#include "SICKDT35_B15851.h"

SICKDT35_B15851::SICKDT35_B15851(uint8_t pin, uint16_t rangeMin, uint16_t rangeMax): pin(pin), rangeMin(rangeMin), rangeMax(rangeMax) {
    pinMode(pin, INPUT);
}

uint16_t SICKDT35_B15851::readDistance() {
    uint16_t valueRead = (uint16_t) analogRead(pin);
    double alpha = valueRead/1024.0;
    double tension = alpha * 3.3;
    tension -= 0.6; // le courant est entre 4 mA et 20 mA et la résistance est de 162 Ohm donc une tension minimale de 0.6V
    double t = tension/(3.3-0.6);
    Serial.printf("[DEBUG] >> %f (%i - %i)\n", t, rangeMin, rangeMax);
    return (uint16_t ) (t*rangeMax + (1.0-t) * rangeMin);
}

void SICKDT35_B15851::setRange(uint16_t min, uint16_t max) {
    rangeMin = min;
    rangeMax = max;
}

