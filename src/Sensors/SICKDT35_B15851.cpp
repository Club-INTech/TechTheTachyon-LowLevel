//
// Created by jglrxavpok on 14/01/19.
//

#include "Arduino.h"
#include "SICKDT35_B15851.h"

SICKDT35_B15851::SICKDT35_B15851(uint8_t pin, uint16_t rangeMin, uint16_t rangeMax): pin(pin), rangeMin(rangeMin), rangeMax(rangeMax) {
    pinMode(pin, INPUT);
}

uint16_t SICKDT35_B15851::readDistance() {
    uint16_t tension = (uint16_t) analogRead(pin);
    double alpha = tension/1024.0;
    Serial.printf("[DEBUG] >> %f (%i - %i)\n", alpha, rangeMin, rangeMax);
    return (uint16_t ) (alpha*rangeMax + (1.0-alpha) * rangeMin);
}

void SICKDT35_B15851::setRange(uint16_t min, uint16_t max) {
    rangeMin = min;
    rangeMax = max;
}

