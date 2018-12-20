//
// Created by jglrxavpok on 20/12/18.
//

#include "MCSReborn.h"
#include "Motor.h"
#include "SelfContainedPID.hpp"
#include "Utils/utils.h"
#include "Utils/defines.h"
#include "../Utils/utils.h"

MCS::MCS(): leftMotor(Side::LEFT), rightMotor(Side::LEFT) {}

void MCS::updatePosition(int32_t leftTicks, int32_t rightTicks) {

    int32_t leftDistance = leftTicks * TICK_TO_MM;
    int32_t rightDistance = rightTicks * TICK_TO_MM;
    float cos = cosf(getAngle());
    float sin = sinf(getAngle());

    uint32_t dt = millis() - lastPositionUpdateTime;
    // somme des résultantes
    int32_t distance = (leftDistance+rightDistance)/2;
    robotStatus.x += distance*cos;
    robotStatus.y += distance*sin;


    robotStatus.speedTranslation = distance/dt;
    robotStatus.speedLeftWheel = leftDistance/dt;
    robotStatus.speedRightWheel = rightDistance/dt;

    robotStatus.orientation = (rightTicks-leftTicks)/2 * TICK_TO_RADIAN;
    robotStatus.speedRotation = robotStatus.orientation / dt;

/*    // vitesse des roues
    float speedLeftX = cos*leftDistance/dt;
    float speedLeftY = sin*leftDistance/dt;
    float speedRightX = cos*rightDistance/dt;
    float speedRightY = sin*rightDistance/dt;

    // calcul du centre de rotation
    // https://www.xr6805.fr/school/sti_web/cinematique/cir.pdf

    // vecteurs perpendiculaires aux vitesses
    float perpLeftX = speedLeftY;
    float perpLeftY = speedLeftX;
    float perpRightX = speedRightY;
    float perpRightY = speedRightX;

    // Centres de rotation des roues
    float leftWheelX = -WHEEL_DISTANCE_TO_CENTER;
    float leftWheelY = 0.0f;
    float rightWheelX = WHEEL_DISTANCE_TO_CENTER;
    float rightWheelY = 0.0f;

    float dy = rightWheelY - leftWheelY;
    float dx = rightWheelX - leftWheelX;
    float cirX = rightWheelX + perpRightX * (-dy + perpLeftY*dx) / (perpLeftX*perpRightY-perpLeftY*perpRightX);
    float cirY = rightWheelY + perpRightY * (-dy + perpLeftY*dx) / (perpLeftX*perpRightY-perpLeftY*perpRightX);

    // calcul de la vitesse de rotation
    float pulsation = sqrtf((speedLeftX*speedLeftX + speedLeftY*speedLeftY) / ((leftWheelX-cirX)*(leftWheelX-cirX)+(leftWheelY-cirY)*(leftWheelY-cirY)));
    robotStatus.speedRotation = pulsation / ((float)PI*2);
    robotStatus.orientation += robotStatus.speedRotation * dt;
*/
    lastPositionUpdateTime = millis();
}

void MCS::control() {
    if(!robotStatus.controlled)
        return;
    int32_t leftTicks = Encoder1.count();
    int32_t rightTicks = Encoder2.count();
    updatePosition(leftTicks, rightTicks);
    Encoder1.reset();
    Encoder2.reset();
    if(translationPID.active) {
        int32_t distance = (leftTicks+rightTicks)/2;
        int32_t state = translationPID.getCurrentState();
        translationPID.compute(state+distance);
    }
    if(rotationPID.active) {
        int32_t distance = (rightTicks-leftTicks)/2; // moyenne des deux movements
        int32_t state = rotationPID.getCurrentState();
        rotationPID.compute(state+distance);
    }


    // update PWMs
    if(translationPID.active) {
        int16_t pwm = robotStatus.movement == MOVEMENT::FORWARD ? (int16_t)16 : (int16_t)-16; // TODO: calcul de la vitesse
        leftMotor.run(pwm);
        rightMotor.run(pwm);
    }

    if(rotationPID.active) {
        int16_t pwm = robotStatus.movement == MOVEMENT::TRIGO ? (int16_t)16 : (int16_t)-16; // TODO: calcul de la vitesse
        leftMotor.run(-pwm);
        rightMotor.run(pwm);
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
    targetDistance = amount;
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
    targetAngle = angle;
    if( ! rotationPID.active) {
        rotationPID.fullReset();
        rotationPID.active = true;
    }
    robotStatus.movement = angle > 0.0 ? MOVEMENT::TRIGO : MOVEMENT::ANTITRIGO;
    int32_t targetTick = angle/TICK_TO_RADIAN;
    rotationPID.setGoal(rotationPID.getCurrentGoal()+targetTick);
}

void MCS::gotoPoint(int16_t x, int16_t y, bool sequential) {
    targetX = x;
    targetY = y;
    // TODO
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
    return robotStatus.x;
}

int16_t MCS::getY() {
    return robotStatus.y;
}

float MCS::getAngle() {
    return robotStatus.orientation;
}

void MCS::setX(int16_t x) {
    robotStatus.x = x;
}

void MCS::setY(int16_t y) {
    robotStatus.y = y;
}

void MCS::setAngle(float angle) {
    robotStatus.orientation = angle;
}
