//
// Created by jglrxavpok on 16/05/19.
//
#include "pin_mapping.h"


#if defined(MAIN)

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

    // Éclairages
    pinMode(LED1,OUTPUT);
    digitalWrite(LED1,LOW);
    pinMode(LED2,OUTPUT);
    digitalWrite(LED2,LOW);
    pinMode(LED3,OUTPUT);
    digitalWrite(LED3,LOW);
    pinMode(LED4,OUTPUT);
    digitalWrite(LED4,LOW);
}

#elif defined(SLAVE)

void InitAllPins()
{
    // Jumper
    pinMode(PIN_JMPR, INPUT);

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
    pinMode(DIR_PIN_RIGHT, OUTPUT);
    digitalWrite(DIR_PIN_RIGHT, LOW);
    pinMode(STEP_PIN_RIGHT, OUTPUT);
    digitalWrite(STEP_PIN_RIGHT, LOW);

    // Pneumatique
    pinMode(LEFT_VALVE_PIN, OUTPUT);
    digitalWrite(LEFT_VALVE_PIN, LOW);
    pinMode(RIGHT_VALVE_PIN, OUTPUT);
    digitalWrite(RIGHT_VALVE_PIN, LOW);
    pinMode(RIGHT_PUMP_PIN, OUTPUT);
    digitalWrite(RIGHT_PUMP_PIN, LOW);

    pinMode(LED1_1, OUTPUT);
    pinMode(LED2_1, OUTPUT);
    pinMode(LED3_1, OUTPUT);
    pinMode(LED1_2, OUTPUT);
    pinMode(LED2_2, OUTPUT);
    pinMode(LED3_2, OUTPUT);
    pinMode(LED1_3, OUTPUT);
    pinMode(LED2_3, OUTPUT);
    pinMode(LED3_3, OUTPUT);

    digitalWrite(LED1_1, HIGH);
    digitalWrite(LED2_1, HIGH);
    digitalWrite(LED3_1, HIGH);
    digitalWrite(LED1_2, HIGH);
    digitalWrite(LED2_2, HIGH);
    digitalWrite(LED3_2, HIGH);
    digitalWrite(LED1_3, HIGH);
    digitalWrite(LED2_3, HIGH);
    digitalWrite(LED3_3, HIGH);
}

#endif