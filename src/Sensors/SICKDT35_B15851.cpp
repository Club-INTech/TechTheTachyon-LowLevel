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
    double alpha = ((double)valueRead)/(1<<ANALOG_RESOLUTION);

    // le courant est entre 4 mA et 20 mA donc une tension minimale de 0.6V
    const double minCurrent = 0.004;
    const double maxCurrent = 0.020;
    const double minVoltage = (minCurrent * resistorValue);
    const double maxVoltage = (maxCurrent * resistorValue);
    double tension = alpha * 3.3;
    tension -= minVoltage;
    double t = tension/(maxVoltage-minVoltage);
//    Serial.printf("[DEBUG] >> %f (%i - %i)\n", t, rangeMin, rangeMax);
    return (uint16_t ) (t*rangeMax + (1.0-t) * rangeMin);
}

void SICKDT35_B15851::setRange(uint16_t min, uint16_t max) {
    rangeMin = min;
    rangeMax = max;
}

void SICKDT35_B15851::setResistorValue(double value) {
    resistorValue = value;
}
