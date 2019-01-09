//
// Created by jglrxavpok on 20/12/18.
//

#include "MCSReborn.h"
#include "Motor.h"
#include "SelfContainedPID.hpp"
#include "Utils/utils.h"
#include "Utils/defines.h"

MCS::MCS(): leftMotor(Side::LEFT), rightMotor(Side::RIGHT) {
    initSettings();
    initStatus();
    robotStatus.controlled = true;
    robotStatus.controlledRotation = true;
    robotStatus.controlledTranslation = true;
    robotStatus.controlledP2P = false;
    robotStatus.movement = MOVEMENT::NONE;

    // TODO: constantes d'asserv' à mettre à jour
    leftSpeedPID.setTunings(0.2165,0.00005,0.414);
    rightSpeedPID.setTunings(0.225,0.00005,0.4121);

    translationPID.setTunings(6.5,0,1.08);
    rotationPID.setTunings(12,0.00001,0);

    leftMotor.init();
    rightMotor.init();
}

void MCS::initSettings() {
    // TODO: valeurs de TechTheTown, à changer si besoin
    controlSettings.maxRotationSpeed = 314*2; //0.25 tour/s max
    controlSettings.maxTranslationSpeed = 1000; // 1m/s max
    controlSettings.tolerancyAngle = 0.25;
    controlSettings.tolerancyTranslation = 10; // 1cm
    controlSettings.tolerancyRadius = 10; // 1cm
    controlSettings.maxSpeed = controlSettings.maxTranslationSpeed;
    controlSettings.stopDelay = 25;
    controlSettings.tolerancySpeed = 24;
    controlSettings.maxAcceleration = 30;
    controlSettings.minAcceleration = 1; // TODO
}

void MCS::initStatus() {
    robotStatus.movement = MOVEMENT::NONE;
    robotStatus.controlledP2P = false;
    robotStatus.controlled = true;
    robotStatus.controlledRotation = true;
    robotStatus.controlledTranslation = true;
}

void MCS::initEncoders() {
    Serial.println("Coucou MCS");

    this->Encoder1 = new Encoder(28, 29);//new Encoder(18, 19);
    this->Encoder2 = new Encoder(21, 20);
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

    currentDistance = distance;
    currentRotation = ((rightTicks - currentDistance/TICK_TO_MM) - (leftTicks - currentDistance/TICK_TO_MM)) / 2 * TICK_TO_RADIAN;
    robotStatus.orientation += currentRotation;
    robotStatus.speedRotation = robotStatus.orientation / dt;

    if(robotStatus.controlledP2P) { // si point-à-point
        int16_t dx = robotStatus.x - targetX;
        int16_t dy = robotStatus.y - targetY;
        float angleToGoal = atan2f(dy, dx);
        int16_t norm = (int16_t) sqrtf(dx*dx+dy*dy);

        translationPID.setGoal(currentDistance);

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

                rotate(robotStatus.orientation + angleOffset);
                angleOffset = 0.0f; // reset de l'offset pour pas décaler le reste des mouvements
            }
        }
    }

    lastPositionUpdateTime = millis();
}

void MCS::control() {
    if(!robotStatus.controlled)
        return;
    int32_t leftTicks = Encoder1->read();//count();
    int32_t rightTicks = Encoder2->read();//count();
    updatePosition(leftTicks, rightTicks);
    //Encoder1.reset();
    //Encoder2.reset();
//    Encoder1.write(0);
//    Encoder2.write(0);
    if(robotStatus.controlledTranslation) {
        translationPID.compute(currentDistance);
    }
    if(robotStatus.controlledRotation) {
        rotationPID.compute(currentRotation);
    }

    int32_t leftSpeed = (leftTicks - previousLeftTicks) * TICK_TO_MM * MC_FREQUENCY;
    int32_t rightSpeed = (rightTicks - previousRightTicks) * TICK_TO_MM  * MC_FREQUENCY;

    robotStatus.speedLeftWheel = leftSpeed;
    robotStatus.speedRightWheel = rightSpeed;

    int32_t translationSpeed = MAX(-controlSettings.maxTranslationSpeed, MIN(controlSettings.maxTranslationSpeed, translationPID.getOutput())); // TODO: limites
    int32_t rotationSpeed = MAX(-controlSettings.maxRotationSpeed, MIN(controlSettings.maxRotationSpeed, rotationPID.getOutput())) * DISTANCE_COD_GAUCHE_CENTRE;

    leftSpeedPID.setGoal(translationSpeed-rotationSpeed);
    rightSpeedPID.setGoal(translationSpeed+rotationSpeed);

    averageLeftSpeed.add(leftSpeed);
    averageRightSpeed.add(rightSpeed);

    leftSpeed = averageLeftSpeed.value();
    rightSpeed = averageRightSpeed.value();

    int32_t leftPWM = leftSpeedPID.compute(leftSpeed);
    int32_t rightPWM = rightSpeedPID.compute(rightSpeed);
    leftMotor.run(leftPWM);
    rightMotor.run(rightPWM);

    previousLeftTicks = leftTicks;
    previousRightTicks = rightTicks;
}

void MCS::manageStop() {
    if(translationPID.active) {
        if(ABS(translationPID.getError()) <= controlSettings.tolerancyTranslation) {
            translationPID.active = false;
            Serial.println("[DEBUG] On s'arrête la translation de la tolérance en translation!");
        }
    }
    if(rotationPID.active) {
        if(ABS(rotationPID.getError()) <= controlSettings.tolerancyRotation) {
            rotationPID.active = false;
            Serial.println("[DEBUG] On arrête la rotation à cause de la tolérance en rotation!");
        }
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
    translationPID.resetErrors();
    rotationPID.resetErrors();
    leftSpeedPID.resetErrors();
    rightSpeedPID.resetErrors();

    leftSpeedPID.setGoal(0);
    rightSpeedPID.setGoal(0);

    translationPID.resetOutput(0);
    rotationPID.resetOutput(0);
    translationPID.setGoal(currentDistance);
    rotationPID.setGoal(currentRotation);

    if(robotStatus.movement != MOVEMENT::NONE) {
        Serial.println("[DEBUG] On s'arrête!!");
    }
    robotStatus.movement = MOVEMENT::NONE;
}

void MCS::translate(int16_t amount) {
    if(!robotStatus.controlledTranslation)
        return;
    targetDistance = amount;
    Serial.printf("[DEBUG] Début d'une translation de %i mm\n", amount);
    if(amount == 0)
        return;
    if( ! translationPID.active) {
        Serial.printf("[DEBUG] Reset de translationPID\n");
        translationPID.fullReset();
        translationPID.active = true;
    }
    robotStatus.movement = amount > 0 ? MOVEMENT::FORWARD : MOVEMENT::BACKWARD;
    translationPID.setGoal(amount + translationPID.getCurrentGoal());
}

void MCS::rotate(float angle) {
    if(!robotStatus.controlledRotation)
        return;
    targetAngle = angle;
    Serial.printf("[DEBUG] Rotation vers l'angle %f radians\n", angle);
    if( ! rotationPID.active) {
        Serial.printf("[DEBUG] Reset de rotationPID\n");
        rotationPID.fullReset();
        rotationPID.active = true;
    }
    robotStatus.movement = angle > 0.0 ? MOVEMENT::TRIGO : MOVEMENT::ANTITRIGO;
    rotationPID.setGoal(angle + rotationPID.getCurrentGoal());
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