//
// Created by jglrxavpok on 20/12/18.
//

#include "MCSReborn.h"
#include "Motor.h"
#include "SelfContainedPID.hpp"
#include "Utils/utils.h"
#include "Utils/defines.h"

MCS::MCS(): leftMotor(Side::LEFT), rightMotor(Side::LEFT) {}

void MCS::control() {
    if(!robotStatus.controlled)
        return;
    int32_t leftTicks = Encoder1.count();
    int32_t rightTicks = Encoder2.count();
    Encoder1.reset();
    Encoder2.reset();
    if(translationPID.active) {
        int32_t distance = (leftTicks+rightTicks)/2;
        int32_t state = translationPID.getCurrentState();
        translationPID.compute(state+distance);
    }
    if(rotationPID.active) {
        int32_t sign = 1;
        if(robotStatus.movement == MOVEMENT::ANTITRIGO) {
            sign = -1;
        }
        int32_t distance = sign*(rightTicks-leftTicks)/2; // moyenne des deux movements
        int32_t state = rotationPID.getCurrentState();
        rotationPID.compute(state+distance);
    }
}

void MCS::manageStop() {
    if(ABS(translationPID.getError()) <= controlSettings.tolerancyTranslation) {
        translationPID.active = false;
    }
    if(ABS(rotationPID.getError()) <= controlSettings.tolerancyAngle) {
        rotationPID.active = false;
    }

    if(!translationPID.active && !rotationPID.active) {
        stop();
    }
}

void MCS::stop() {
    leftMotor.stop();
    rightMotor.stop();
    robotStatus.movement = MOVEMENT::NONE;
}

void MCS::translate(int16_t amount) {
    if(amount == 0)
        return;
    if( ! translationPID.active) {
        translationPID.fullReset();
        translationPID.active = true;
    }
    int32_t ticks = amount/TICK_TO_MM;
    robotStatus.movement = amount > 0 ? MOVEMENT::FORWARD : MOVEMENT::BACKWARD;
    translationPID.setGoal(translationPID.getCurrentGoal()+ticks);
}

void MCS::rotate(float angle) {
    if( ! rotationPID.active) {
        rotationPID.fullReset();
        rotationPID.active = true;
    }
    robotStatus.movement = angle > 0.0 ? MOVEMENT::TRIGO : MOVEMENT::ANTITRIGO;
    int32_t targetTick = angle/TICK_TO_RADIAN;
    rotationPID.setGoal(rotationPID.getCurrentGoal()+targetTick);
}

void MCS::gotoPoint(int16_t x, int16_t y, bool sequential) {

}

void MCS::toggleControl() {
    robotStatus.controlled = !robotStatus.controlled;
}

void MCS::toggleTranslation() {
    robotStatus.controlledTranslation = !robotStatus.controlledTranslation;
}

void MCS::toggleRotation() {
    robotStatus.controlledRotation = !robotStatus.controlledRotation;
}

int16_t MCS::getX() {
    return 0;
}

int16_t MCS::getY() {
    return 0;
}

float MCS::getAngle() {
    return 0.0;
}

void MCS::setX(int16_t x) {

}

void MCS::setY(int16_t y) {

}

void MCS::setAngle(float angle) {

}
