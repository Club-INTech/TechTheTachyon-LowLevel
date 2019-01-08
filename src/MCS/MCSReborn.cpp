//
// Created by jglrxavpok on 20/12/18.
//

#include "MCSReborn.h"
#include "Motor.h"
#include "SelfContainedPID.hpp"
#include "Utils/utils.h"
#include "Utils/defines.h"

MCS::MCS(): leftMotor(Side::LEFT), rightMotor(Side::RIGHT) {
    robotStatus.controlled = true;
    robotStatus.controlledRotation = true;
    robotStatus.controlledTranslation = true;
    robotStatus.controlledP2P = false;
    robotStatus.movement = MOVEMENT::NONE;

    leftMotor.init();
    rightMotor.init();
}

void MCS::updatePosition(int32_t leftTicks, int32_t rightTicks) {
    // mise à jour de la position interne grâce aux infos des codeuses
    this->leftTicks = leftTicks;
    this->rightTicks = rightTicks;
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

    if(robotStatus.controlledP2P) { // si point-à-point
        int16_t dx = robotStatus.x - targetX;
        int16_t dy = robotStatus.y - targetY;
        float angleToGoal = atan2f(dy, dx);
        int16_t norm = (int16_t) sqrtf(dx*dx+dy*dy);

        // remets l'angle dans ]-PI; PI[
        if(ABS(robotStatus.orientation-angleToGoal) > (float)PI) {
            if(angleToGoal < 0)
                angleToGoal += TWO_PI;
            else
                angleToGoal -= TWO_PI;
        }
        if(!sequentialMovement || (sequentialMovement && ABS(robotStatus.orientation - angleToGoal) <= controlSettings.tolerancyAngle)) { // on est à peu près en face de la cible
            translate(norm);
        }
        rotate(angleToGoal);

        if(ABS(norm) <= controlSettings.tolerancyTranslation) { // le robot est arrivé à destination
            if(trajectory) { // si on suit une trajectoire et qu'il y a encore un point à atteindre
                std::pair<double, double> point = trajectory.query();
                gotoPoint(point.first, point.second, sequentialMovement);
            } else { // sinon on a fini le mouvement demandé
                robotStatus.forcedMovement = false;
                robotStatus.controlledP2P = false;
                trajectory.clear();

                if(ABS(angleOffset) > controlSettings.tolerancyAngle) { // rotation demandée à la fin du mouvement
                    rotate(robotStatus.orientation + angleOffset);
                }
                angleOffset = 0.0f; // reset de l'offset pour pas décaler le reste des mouvements
            }
        }
    }

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
    if(robotStatus.controlledTranslation && translationPID.active) {
        int32_t distance = (leftTicks+rightTicks)/2;
        int32_t state = translationPID.getCurrentState();
        translationPID.compute(state+distance);
    }
    if(robotStatus.controlledRotation && rotationPID.active) {
        int32_t distance = (rightTicks-leftTicks)/2; // moyenne des deux movements
        int32_t state = rotationPID.getCurrentState();
        rotationPID.compute(state+distance);
    }

    int32_t leftSpeed = (leftTicks - previousLeftTicks) * MC_FREQUENCY;
    int32_t rightSpeed = (rightTicks - previousRightTicks) * MC_FREQUENCY;
    robotStatus.speedLeftWheel = leftSpeed;
    robotStatus.speedRightWheel = leftSpeed;

    int32_t translationSpeed = MAX(-controlSettings.maxTranslationSpeed, MIN(controlSettings.maxTranslationSpeed, translationPID.getOutput())); // TODO: limites
    int32_t rotationSpeed = MAX(-controlSettings.maxRotationSpeed, MIN(controlSettings.maxRotationSpeed, rotationPID.getOutput()));

    leftSpeedPID.setGoal(translationSpeed-rotationSpeed);
    rightSpeedPID.setGoal(translationSpeed+rotationSpeed);

    int32_t leftPWM = leftSpeedPID.compute(leftSpeed);
    int32_t rightPWM = rightSpeedPID.compute(rightSpeed);
    leftMotor.run(leftPWM);
    leftMotor.run(rightPWM);

    previousLeftTicks = leftTicks;
    previousRightTicks = rightTicks;
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
    robotStatus.controlledP2P = false;
    trajectory.clear();
    translationPID.fullReset();
    rotationPID.fullReset();
    leftSpeedPID.fullReset();
    rightSpeedPID.fullReset();
    robotStatus.movement = MOVEMENT::NONE;
}

void MCS::translate(int16_t amount) {
    if(!robotStatus.controlledTranslation)
        return;
    targetDistance = amount;
    if(amount == 0)
        return;
    if( ! translationPID.active) {
        translationPID.fullReset();
        translationPID.active = true;
    }
    int32_t ticks = amount/TICK_TO_MM;
    robotStatus.movement = amount > 0 ? MOVEMENT::FORWARD : MOVEMENT::BACKWARD;
    translationPID.setGoal(ticks);
}

void MCS::rotate(float angle) {
    if(!robotStatus.controlledRotation)
        return;
    targetAngle = angle;
    if( ! rotationPID.active) {
        rotationPID.fullReset();
        rotationPID.active = true;
    }
    robotStatus.movement = angle > 0.0 ? MOVEMENT::TRIGO : MOVEMENT::ANTITRIGO;
    int32_t targetTick = angle/TICK_TO_RADIAN;
    rotationPID.setGoal(targetTick);
}

void MCS::gotoPoint(int16_t x, int16_t y, bool sequential) {
    targetX = x;
    targetY = y;
    robotStatus.controlledP2P = true;
    sequentialMovement = sequential;
}

void MCS::followTrajectory(const double* xTable, const double* yTable, int count) {
    trajectory.set(xTable, yTable, count);
    if(count > 0) { // s'il y a bien des points dans cette trajectoire
        std::pair<double, double> point = trajectory.query();
        gotoPoint(point.first, point.second, false);
    }
}

void MCS::disableP2P() {
    trajectory.clear();
    robotStatus.controlledP2P = false;
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

void MCS::setAngleOffset(float offset) {
    angleOffset = offset;
}

int32_t MCS::getLeftTicks() {
    return leftTicks;
}

int32_t MCS::getRightTicks() {
    return rightTicks;
}