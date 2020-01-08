//
// Created by trotfunky on 26/11/18.
//

#ifndef LL_ROBOTSTATUS_H
#define LL_ROBOTSTATUS_H

#include <cstdint>

enum class MOVEMENT { FORWARD, BACKWARD, TRIGO, ANTITRIGO, CURVE, NONE };

struct RobotStatus
{
    bool translation;
    bool controlled;
    bool controlledTranslation;
    bool controlledRotation;
    bool inRotationInGoto;
    bool inGoto;
    bool forcedMovement;
    bool moving;
    bool stuck;

    float x;
    float y;
    float orientation;

    MOVEMENT movement;

    float speedTranslation;
    float speedRotation;
    float speedLeftWheel;
    float speedRightWheel;

    bool sentMoveAbnormal;

    RobotStatus();
    void updateStatus();
};

#endif //LL_ROBOTSTATUS_H
