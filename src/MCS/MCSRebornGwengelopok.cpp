//
// Created by jglrxavpok aka Coin-Coin Ier <3 (27/02) on 20/12/18.
//

#include "MCSReborn.h"
#include "../../../.platformio/packages/framework-arduinoteensy/libraries/Tlc5940/tlc_animations.h"


MCS::MCS(): leftMotor(Side::LEFT), rightMotor(Side::RIGHT)  {

    encoderLeft = new Encoder(ENCODER_LEFT_B,ENCODER_LEFT_A);
    encoderRight = new Encoder(ENCODER_RIGHT_B,ENCODER_RIGHT_A);

    initSettings();
    initStatus();
    robotStatus.controlled = true;
    robotStatus.controlledRotation = true;
    robotStatus.controlledTranslation = true;
    robotStatus.controlledP2P = false;
    robotStatus.movement = MOVEMENT::NONE;


    /*leftSpeedPID.setTunings(1.65, 0.005, 40, 0);
    leftSpeedPID.enableAWU(false);
    rightSpeedPID.setTunings(1.35, 0.005, 40, 0);
    rightSpeedPID.enableAWU(false);*/

    leftSpeedPID.setTunings(1.65, 0.005, 50, 0);
    leftSpeedPID.enableAWU(false);
    rightSpeedPID.setTunings(1.35, 0.005, 50, 0);
    rightSpeedPID.enableAWU(false);

    translationPID.setTunings(4.35,0.000001,0,0);
    translationPID.enableAWU(false);
//    rotationPID180.setTunings(6.5,0.0001,0,0);
//    rotationPID.setTunings(8.75,0.000001,0,0);
    rotationPID.setTunings(18,0.000001,0,0);
//    rotationPID90.setTunings(10.3,0.0001,12,0);
//    rotationPID180.enableAWU(false);
//    rotationPID90.enableAWU(false);
    leftMotor.init();
    rightMotor.init();
}

void MCS::initSettings() {
    robotStatus.controlledP2P = false;
    robotStatus.movement = MOVEMENT::NONE;


    /* mm/s/MCS_PERIOD */
    controlSettings.maxAcceleration = 2;
    controlSettings.maxDeceleration = 2;

    /* rad/s */
    controlSettings.maxRotationSpeed = PI;

    /* mm/s */
    controlSettings.maxTranslationSpeed = 1000;
    controlSettings.tolerancySpeed = 100;

    /* rad */
    controlSettings.tolerancyAngle = 0.005;

    /* mm */
    controlSettings.tolerancyTranslation = 1;

    controlSettings.tolerancyDerivative = 100;

    /* ms */
    controlSettings.stopDelay = 25;

    /* mm/s */
    controlSettings.tolerancyDerivative = 0;
}

void MCS::initStatus() {
    robotStatus.movement = MOVEMENT::NONE;
    robotStatus.moving = false;
    robotStatus.controlledP2P = false;
    robotStatus.controlled = true;
    robotStatus.controlledRotation = true;
    robotStatus.controlledTranslation = true;
    previousLeftSpeedGoal = 0;
    previousRightSpeedGoal = 0;
    previousLeftTicks = 0;
    previousRightTicks = 0;
}

void MCS::updatePositionOrientation() {

    int32_t leftDistance = leftTicks * TICK_TO_MM;
    int32_t rightDistance = rightTicks * TICK_TO_MM;

    float cos = cosf(getAngle());
    float sin = sinf(getAngle());

    // somme des résultantes
    int32_t distance = (leftDistance+rightDistance)/2;

    robotStatus.x = distance*cos;
    robotStatus.y = distance*sin;

    currentDistance = distance;
    currentRotation = ((rightTicks - currentDistance/TICK_TO_MM) - (leftTicks - currentDistance/TICK_TO_MM)) / 2 * TICK_TO_RADIAN;
    robotStatus.orientation = currentRotation;

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
}

void MCS::updateSpeed()
{

    averageLeftSpeed.add((leftTicks - previousLeftTicks) * TICK_TO_MM * MCS_FREQ);
    averageRightSpeed.add((rightTicks - previousRightTicks) * TICK_TO_MM  * MCS_FREQ);
    robotStatus.speedLeftWheel = averageLeftSpeed.value();
    robotStatus.speedRightWheel = averageRightSpeed.value();

    if(robotStatus.controlledTranslation)
    {
        robotStatus.speedTranslation = translationPID.getOutput();
    }

    if(robotStatus.controlledRotation)
    {
        robotStatus.speedRotation = rotationPID.getOutput();
    }


    robotStatus.speedTranslation = MAX(-controlSettings.maxTranslationSpeed, MIN(controlSettings.maxTranslationSpeed, robotStatus.speedTranslation));
    robotStatus.speedRotation = MAX(-controlSettings.maxRotationSpeed, MIN(controlSettings.maxRotationSpeed, robotStatus.speedRotation)) * DISTANCE_COD_GAUCHE_CENTRE;

    leftSpeedPID.setGoal(robotStatus.speedTranslation-robotStatus.speedRotation);
    rightSpeedPID.setGoal(robotStatus.speedTranslation+robotStatus.speedRotation);

    if( leftSpeedPID.getCurrentGoal() - previousLeftSpeedGoal > controlSettings.maxAcceleration ) {
        leftSpeedPID.setGoal( previousLeftSpeedGoal + controlSettings.maxAcceleration );
    }
    if( previousLeftSpeedGoal - leftSpeedPID.getCurrentGoal() > controlSettings.maxDeceleration ) {
        leftSpeedPID.setGoal( previousLeftSpeedGoal - controlSettings.maxDeceleration );
    }

    if( rightSpeedPID.getCurrentGoal() - previousRightSpeedGoal > controlSettings.maxAcceleration ) {
        rightSpeedPID.setGoal( previousRightSpeedGoal + controlSettings.maxAcceleration );
    }
    if( previousRightSpeedGoal - rightSpeedPID.getCurrentGoal() > controlSettings.maxDeceleration ) {
        rightSpeedPID.setGoal( previousRightSpeedGoal - controlSettings.maxDeceleration );
    }

    previousLeftSpeedGoal = leftSpeedPID.getCurrentGoal();
    previousRightSpeedGoal = rightSpeedPID.getCurrentGoal();
}
void MCS::control()
{
    if(!robotStatus.controlled)
        return;

    leftTicks = encoderLeft->read();
    rightTicks = encoderRight->read();

    updatePositionOrientation();

    if(robotStatus.controlledTranslation) {
        translationPID.compute(currentDistance);
    }
    if(robotStatus.controlledRotation) {
        rotationPID.compute(currentRotation);
    }

    updateSpeed();

    int32_t leftPWM = leftSpeedPID.compute(robotStatus.speedLeftWheel);
    int32_t rightPWM = rightSpeedPID.compute(robotStatus.speedRightWheel);
    leftMotor.run(leftPWM);
    rightMotor.run(rightPWM);

    //leftMotor.run(125);
    //rightMotor.run(125);

    previousLeftTicks = leftTicks;
    previousRightTicks = rightTicks;

}

void MCS::manageStop() {

    if(translationPID.active) {
        if((ABS(translationPID.getError()) <= controlSettings.tolerancyTranslation) && (ABS(translationPID.getDerivativeError()) <= controlSettings.tolerancyDerivative)){
            translationPID.active = false;
            InterruptStackPrint::Instance().push("arret tolerance translation");
            Serial.println("Tolérance translation");
        }
    }
    if(rotationPID.active) {
        if((ABS(rotationPID.getError()) <= controlSettings.tolerancyAngle && (ABS(rotationPID.getDerivativeError()) <= controlSettings.tolerancyDerivative ))){
            rotationPID.active = false;
            InterruptStackPrint::Instance().push("arret tolerance rotation");
            Serial.println("Tolérance rotation");
        }
    }


    if(!translationPID.active && !rotationPID.active ) {
        if( !robotStatus.forcedMovement )
        {
            leftMotor.brake();
            rightMotor.brake();
            if(ABS(robotStatus.speedLeftWheel) <= controlSettings.tolerancySpeed && ABS(robotStatus.speedRightWheel) <= controlSettings.tolerancySpeed){
                stop();
            }
        }
    }
    /*if(translationPID.getDerivativeError()==0 && ABS(translationPID.getCurrentOutput()-translationPID.getCurrentGoal())<=controlSettings.tolerancyTranslation && rotationPID.getDerivativeError()==0 && ABS(rotationPID.getCurrentOutput()-rotationPID.getCurrentGoal())<=controlSettings.tolerancyAngle){
        leftMotor.setDirection(Direction::NONE);
        rightMotor.setDirection(Direction::NONE);
        digitalWrite(LED1,HIGH);
    }*/
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
        InterruptStackPrint::Instance().push("[DEBUG] On s'arrête!!");
    }
    robotStatus.movement = MOVEMENT::NONE;
    robotStatus.moving = false;
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
    robotStatus.movement = amount > 0 ? MOVEMENT::FORWARD : MOVEMENT::BACKWARD;
    translationPID.setGoal(amount + translationPID.getCurrentGoal());
    robotStatus.moving = true;
}

void MCS::rotate(float angle) {
    rotationPID.active = false;
    if(!robotStatus.controlledRotation){
        return;
    }
    targetAngle = angle;

    float differenceAngle = rotationPID.getCurrentState()-targetAngle;

    /*if((45<ABS(differenceAngle) and ABS(differenceAngle)<135)){
        rotationPID.setTunings(10.3,0.0001,12,0);
    }
    else{
        rotationPID.setTunings(6.5,0.0001,0,0);
    }*/
    if( ! rotationPID.active) {
        rotationPID.fullReset();
        rotationPID.active = true;
    }
    robotStatus.movement = angle > 0.0 ? MOVEMENT::TRIGO : MOVEMENT::ANTITRIGO;
    rotationPID.setGoal(angle + rotationPID.getCurrentGoal());
    robotStatus.moving = true;
}

void MCS::gotoPoint(int16_t x, int16_t y, bool sequential) {
    targetX = x;
    targetY = y;
    robotStatus.controlledP2P = true;
    sequentialMovement = sequential;
    robotStatus.moving = true;
}

void MCS::followTrajectory(const double* xTable, const double* yTable, int count) {
    trajectory.set(xTable, yTable, count);
    if(count > 0) { // s'il y a bien des points dans cette trajectoire
        std::pair<double, double> point = trajectory.query();
        gotoPoint(point.first, point.second, false);
    }
}

void MCS::speedBasedMovement(MOVEMENT movement) {
    if(!robotStatus.controlled)
    {
        return;
    }

    robotStatus.moving = true;

    switch(movement)
    {
        case MOVEMENT::FORWARD:
            leftSpeedPID.setGoal(controlSettings.maxTranslationSpeed);
            rightSpeedPID.setGoal(controlSettings.maxTranslationSpeed);
            robotStatus.speedTranslation = controlSettings.maxTranslationSpeed;
            break;
        case MOVEMENT::BACKWARD:
            leftSpeedPID.setGoal(-controlSettings.maxTranslationSpeed);
            rightSpeedPID.setGoal(-controlSettings.maxTranslationSpeed);
            robotStatus.speedTranslation = -controlSettings.maxTranslationSpeed;
            break;
        case MOVEMENT::TRIGO:
            leftSpeedPID.setGoal(-controlSettings.maxRotationSpeed);
            rightSpeedPID.setGoal(controlSettings.maxRotationSpeed);
            robotStatus.speedRotation = controlSettings.maxRotationSpeed;
            break;
        case MOVEMENT::ANTITRIGO:
            leftSpeedPID.setGoal(controlSettings.maxRotationSpeed);
            rightSpeedPID.setGoal(-controlSettings.maxRotationSpeed);
            robotStatus.speedRotation = -controlSettings.maxRotationSpeed;
            break;
        case MOVEMENT::NONE:
        default:
            leftSpeedPID.setGoal(0);
            rightSpeedPID.setGoal(0);
            robotStatus.speedRotation = 0;
            robotStatus.speedTranslation = 0;
            robotStatus.movement = MOVEMENT::NONE;
            return;
    }
    robotStatus.movement = movement;
}

void MCS::disableP2P() {
    trajectory.clear();
    robotStatus.controlledP2P = false;
}

void MCS::setControl(bool b) {
    robotStatus.controlled = b;
}

void MCS::controlledTranslation(bool b) {
    robotStatus.controlledTranslation = b;
}

void MCS::controlledRotation(bool b) {
    robotStatus.controlledRotation = b;
}

void MCS::setForcedMovement(bool newState) {
    robotStatus.forcedMovement = newState;
}

void MCS::setTranslationSpeed(float speed) {
    robotStatus.speedTranslation = speed;
}

void MCS::setRotationSpeed(float speed) {
    robotStatus.speedRotation = speed;
}

void MCS::setMaxTranslationSpeed(float speed) {
    controlSettings.maxTranslationSpeed = speed;
}

void MCS::setMaxRotationSpeed(float speed) {
    controlSettings.maxRotationSpeed = speed;
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

float MCS::getLeftSpeed() {
    return robotStatus.speedLeftWheel;
}

float MCS::getRightSpeed() {
    return robotStatus.speedRightWheel;
}

void MCS::getSpeedGoals(long &leftGoal, long &rightGoal) {
    leftGoal = leftSpeedPID.getCurrentGoal();
    rightGoal = rightSpeedPID.getCurrentGoal();
}
