//
// Created by trotfunky on 26/11/18.
//

#ifndef LL_CONTROLSETTINGS_H
#define LL_CONTROLSETTINGS_H

#include "Arduino.h"

struct ControlSettings {

public:
    /* mm/s/MC_FREQ */
    float maxAcceleration;
    float maxDeceleration;

    /* mm/s */
    float maxTranslationSpeed;
    uint8_t tolerancySpeed;

    /*rad/s*/
    float maxRotationSpeed;

    /* ms */
    uint8_t stopDelay;

    /* mm */
    uint8_t tolerancyTranslation;
    float tolerancyRadius;

    /* rad */
    float tolerancyAngle;

    uint8_t tolerancyDiferential;
    uint8_t tolerancyDerivative;
};

#endif //LL_CONTROLSETTINGS_H
