//
// Created by trotfunky on 26/11/18.
//

#ifndef LL_ROBOTSTATUS_H
#define LL_ROBOTSTATUS_H

#include <cstdint>

enum class MOVEMENT { FORWARD, BACKWARD, TRIGO, ANTITRIGO, CURVE, NONE };

struct RobotStatus
{
    bool controlled;
    bool controlledTranslation;
    bool controlledRotation;
    bool controlledP2P;
    bool Lbooly;
    bool forcedMovement;
    bool moving;
    bool stuck;

    int16_t x;
    int16_t y;
    float orientation;

    MOVEMENT movement;

    float speedTranslation;
    float speedRotation;
    float speedLeftWheel;
    float speedRightWheel;

    RobotStatus();
    void updateStatus();
};

#endif //LL_ROBOTSTATUS_H
