//
// Created by jglrxavpok on 16/05/19.
//
#include "pin_mapping.h"

void InitAllPins() {
    // Jumper
    pinMode(PIN_JMPR, INPUT_PULLUP);

    // Moteurs
    pinMode(PIN_PWM_LEFT, OUTPUT);
    digitalWrite(PIN_PWM_LEFT, LOW);
    pinMode(PIN_PWM_RIGHT, OUTPUT);
    digitalWrite(PIN_PWM_RIGHT, LOW);

    pinMode(INA_LEFT, OUTPUT);
    digitalWrite(INA_LEFT, LOW);
    pinMode(INB_LEFT, OUTPUT);
    digitalWrite(INB_LEFT, LOW);

    pinMode(INA_RIGHT, OUTPUT);
    digitalWrite(INA_RIGHT, LOW);
    pinMode(INB_RIGHT, OUTPUT);
    digitalWrite(INB_RIGHT, LOW);

    // Steppers
    pinMode(DIR_PIN_LEFT, OUTPUT);
    digitalWrite(DIR_PIN_LEFT, LOW);
    pinMode(DIR_PIN_RIGHT, OUTPUT);
    digitalWrite(DIR_PIN_RIGHT, LOW);
    pinMode(STEP_PIN_LEFT, OUTPUT);
    digitalWrite(STEP_PIN_LEFT, LOW);
    pinMode(STEP_PIN_RIGHT, OUTPUT);
    digitalWrite(STEP_PIN_RIGHT, LOW);

    // Pneumatique
    pinMode(LEFT_VALVE_PIN, OUTPUT);
    digitalWrite(LEFT_VALVE_PIN, LOW);
    pinMode(RIGHT_VALVE_PIN, OUTPUT);
    digitalWrite(RIGHT_VALVE_PIN, LOW);
    pinMode(LEFT_PUMP_PIN, OUTPUT);
    digitalWrite(LEFT_PUMP_PIN, LOW);
    pinMode(RIGHT_PUMP_PIN, OUTPUT);
    digitalWrite(RIGHT_PUMP_PIN, LOW);
}