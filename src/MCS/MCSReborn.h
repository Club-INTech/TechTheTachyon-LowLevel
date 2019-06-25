//
// Created by trotfunky on 26/11/18.
//

#ifndef LL_MCSREBORN_H
#define LL_MCSREBORN_H

#include "Utils/Singleton.hpp"
#include "Utils/average.hpp"
#include "Config/defines.h"
#include "Config/pin_mapping.h"
#include "Utils/utils.h"
#include "ControlSettings.h"
#include "RobotStatus.h"
#include "Motor.h"
#include "pid.hpp"
#include "SelfContainedPID.hpp"
#include "PointToPointTrajectory.h"
#include <cmath>
//#include "HardwareEncoder.h"

#define ENCODER_OPTIMIZE_INTERRUPTS
#include "Encoder.h"
#include "../COM/InterruptStackPrint.h"

// TODO : Tout docu
// TODO : P'tet passer les config dans un fichier dans Config/ ?
class MCS : public Singleton<MCS>
{

private:

    Encoder* encoderRight = nullptr;
    Encoder* encoderLeft = nullptr;

    RobotStatus robotStatus;
    ControlSettings controlSettings;

    Motor leftMotor;
    Motor rightMotor;

    SelfContainedPID<float> leftSpeedPID;
    SelfContainedPID<float> rightSpeedPID;
    SelfContainedPID<float> translationPID;
//    SelfContainedPID<float> rotationPID180;
//    SelfContainedPID<float> rotationPID90;
    SelfContainedPID<float> rotationPID;

    int32_t currentDistance;
    int16_t targetX;
    int16_t targetY;

    int32_t leftTicks;
    int32_t rightTicks;
    int32_t previousLeftTicks;
    int32_t previousRightTicks;
    float previousLeftSpeedGoal;
    float previousRightSpeedGoal;
    int16_t targetDistance;
    float targetAngle;
    float angleOffset;
    bool expectedWallImpact;

    Average<float, 100> averageLeftSpeed;
    Average<float, 100> averageRightSpeed;
    Average<float, 25> averageRotationDerivativeError;
    Average<float, 25> averageTranslationDerivativeError;

    bool sequentialMovement;
    PointToPointTrajectory trajectory;

    // Timer entre translation et rotation pour les goto
    uint32_t gotoTimer;


public:
    MCS();

    void initEncoders();

    void manageStop();
    void updatePositionOrientation();
    void updateSpeed();
    void control();
    void stop();
    void stopTranslation();
    void stopRotation();

    void translate(int16_t);
    void rotate(float);
    void gotoPoint(int16_t,int16_t,bool);
    void gotoPoint2(int16_t,int16_t);
    void followTrajectory(const double* xTable, const double* yTable, int count);

    void speedBasedMovement(MOVEMENT);

    void setControl(bool);
    void controlledTranslation(bool);
    void controlledRotation(bool);
    void setForcedMovement(bool);
    void setTranslationSpeed(float);
    void setRotationSpeed(float);
    void setMaxTranslationSpeed(float);
    void setMaxRotationSpeed(float);

    void initSettings();
    void initStatus();

    /**
     * Méthode appelée par un InterruptTimer afin d'envoyer au HL la position du robot
     */
    void sendPositionUpdate();

    /**
     * Reset des codeuses, utilisé quand le HL reset la position du robot (grâce aux SICK par exemple)
     */
    void resetEncoders();

    int16_t getX();
    int16_t getY();
    float getAngle();
    int32_t getLeftTicks();
    int32_t getRightTicks();
    float getLeftSpeed();
    float getRightSpeed();

    void getSpeedGoals(long&,long&);

    /**
     * Permet de définir une rotation à la fin d'un mouvement (au lieu de devoir attendre la fin du mouvement et de donner un ordre de rotation)
     * /!\\ Cette valeur est réinitialisée dès la fin du mouvement!!! (Histoire de pas se décaler avec les mouvements suivants)
     * @param offset l'angle, en radians, duquel le robot doit tourner à la fin du mouvement
     */
    void setAngleOffset(float offset);

    /**
     * Annule le suivi de trajectoire courant
     */
    void disableP2P();

    void setX(int16_t);
    void setY(int16_t);
    void setAngle(float);

    void expectWallImpact();

    bool sentMoveAbnormal();
    bool isMoveAbnormal();
    void setMoveAbnormalSent(bool);
};

#endif //LL_MCSREBORN_H
