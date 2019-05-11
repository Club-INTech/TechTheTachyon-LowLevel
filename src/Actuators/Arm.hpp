//
// Created by jglrxavpok on 07/01/19.
//

#ifndef TECHTHETACHYON_LOWLEVEL_ARM_H
#define TECHTHETACHYON_LOWLEVEL_ARM_H


#include <XL430.h>
#include <SyncWrite.h>
#include <SyncRead.h>

enum ArmStatus {
    OK,
    MOVING,
    WRONG_POSITION,
};

template <typename MotorType>
class Arm {
private:
    DynamixelManager& manager;
    MotorType& base;
    MotorType& elbow;
    MotorType& wrist;
    const char* sideName;
    char* syncAngles = new char[MotorType::goalAngle.length*3];
    MotorType* XLlist;
    SyncWrite* syncVelocityLimit = new SyncWrite(manager, 3, MotorType::velocityLimit, 1);
    SyncWrite* syncAngleWriteData = new SyncWrite(manager, 3, MotorType::goalAngle, 1);
    SyncWrite* syncToggleTorqueWriteData = new SyncWrite(manager, 3, MotorType::torqueEnable, 1);
    SyncWrite* syncReturnDelay = new SyncWrite(manager, 3, MotorType::returnDelay, 1);
    SyncRead* syncMovingRead = new SyncRead(manager, 3, MotorType::moving);
    SyncRead* syncMovingStatus = new SyncRead(manager, 3, MotorType::movingStatus);
    SyncRead* syncHardwareError = new SyncRead(manager, 3, MotorType::hardwareError);
    ArmStatus status = OK;

    float targetPositions[3] = {0.0f};
    uint16_t retryMovementAttempts = 0;
    float positionsAsked[ARM_POSITION_BUFFER_SIZE][3] = { {0.0f} };
    uint16_t writeIndex = 0;
    uint16_t currentPositionIndex = 0;

    /**
     * Est ce que le bras veut pas répondre? Dans ce cas on retient et on attend plus de réponse
     */
    bool mute = false;

    /**
     * Nombre d'essais de lecture de paquets Dynamixel avant de déclarer ce bras muet
     */
    uint8_t attemptsBeforeMute = ARM_ATTEMPTS_BEFORE_MUTE;

    /**
     * Temps de début du mouvement, utilisé au cas où le bras ne réponde pas
     */
    long movementStartTime = 0;

    /**
     * Temps de la dernière vérification que le bras répondait pas
     */
    long lastMuteCheck = 0;

public:
    Arm(const char* sideName, DynamixelManager& manager, MotorType* list): sideName(sideName), manager(manager), XLlist(list), base(list[0]), elbow(list[1]), wrist(list[2]), status(ArmStatus::OK) {

    }

    void initTorque() {
        syncMovingRead->setMotorID(0, base.getId());
        syncMovingRead->setMotorID(1, elbow.getId());
        syncMovingRead->setMotorID(2, wrist.getId());

        syncMovingStatus->setMotorID(0, base.getId());
        syncMovingStatus->setMotorID(1, elbow.getId());
        syncMovingStatus->setMotorID(2, wrist.getId());

        syncHardwareError->setMotorID(0, base.getId());
        syncHardwareError->setMotorID(1, elbow.getId());
        syncHardwareError->setMotorID(2, wrist.getId());

        syncAngleWriteData->setMotorID(0, base.getId());
        syncAngleWriteData->setMotorID(1, elbow.getId());
        syncAngleWriteData->setMotorID(2, wrist.getId());

        syncVelocityLimit->setMotorID(0, base.getId());
        syncVelocityLimit->setMotorID(1, elbow.getId());
        syncVelocityLimit->setMotorID(2, wrist.getId());

        syncReturnDelay->setMotorID(0, base.getId());
        syncReturnDelay->setMotorID(1, elbow.getId());
        syncReturnDelay->setMotorID(2, wrist.getId());

        ComMgr::Instance().printf(DEBUG_HEADER, "Unlocking EEPROM by disabling torque...");
        setTorque(false);
        ComMgr::Instance().printfln(DEBUG_HEADER, "Done!");

        char returnDelay[] = {(char)0xFA};
        ComMgr::Instance().printf(DEBUG_HEADER, "Setting XL return delay to no delay... ");
        syncReturnDelay->setData(0, returnDelay);
        syncReturnDelay->setData(1, returnDelay);
        syncReturnDelay->setData(2, returnDelay);
        syncReturnDelay->send();
        ComMgr::Instance().printfln(DEBUG_HEADER, "Done!");
/* A utiliser que quand un imbécile comme Xavier (moi) a tenté de changer le baudrate
    uint8_t baudrateSelector = 1;
    SyncWrite* baud = new SyncWrite(manager, 3, 8, 1);
    baud->setMotorID(0, base.getId());
    baud->setMotorID(1, elbow.getId());
    baud->setMotorID(2, wrist.getId());
    baud->setData(0, (char*)&baudrateSelector);
    baud->setData(1, (char*)&baudrateSelector);
    baud->setData(2, (char*)&baudrateSelector);
    baud->send();*/

        ComMgr::Instance().printf(DEBUG_HEADER, "Setting velocity limit... ");
        char velocityLimit[] = {static_cast<char>(200), 0, 0, 0};
        syncVelocityLimit->setData(0, velocityLimit);
        syncVelocityLimit->setData(1, velocityLimit);
        syncVelocityLimit->setData(2, velocityLimit);
        syncVelocityLimit->send();

        ComMgr::Instance().printf(DEBUG_HEADER, "Configuring moving threshold... ");
        char threshold[] = {10, 0, 0, 0};
        ComMgr::Instance().printfln(DEBUG_HEADER, "id:%i => %i", base.getId(), base.decapsulatePacket(manager.sendPacket(base.makeWritePacket(MotorType::movingThreshold, threshold))));
        ComMgr::Instance().printfln(DEBUG_HEADER, "id:%i => %i", elbow.getId(), elbow.decapsulatePacket(manager.sendPacket(elbow.makeWritePacket(MotorType::movingThreshold, threshold))));
        ComMgr::Instance().printfln(DEBUG_HEADER, "id:%i => %i", wrist.getId(), wrist.decapsulatePacket(manager.sendPacket(wrist.makeWritePacket(MotorType::movingThreshold, threshold))));

        ComMgr::Instance().printfln(DEBUG_HEADER, "Done!");

        ComMgr::Instance().printf(DEBUG_HEADER, "Toggling torque (and locking EEPROM)... ");
        setTorque(true);
        ComMgr::Instance().printfln(DEBUG_HEADER, "Done!");
    }

    void setPosition(const float* positions, bool resetRetryCounter = true) {
        if(status == MOVING) {
            positionsAsked[writeIndex][0] = positions[0];
            positionsAsked[writeIndex][1] = positions[1];
            positionsAsked[writeIndex][2] = positions[2];
            writeIndex++;
            writeIndex %= ARM_POSITION_BUFFER_SIZE;
            return;
        }

        if(resetRetryCounter) {
            retryMovementAttempts = 0;
        }
        movementStartTime = millis();

        status = MOVING;
        prepareAngleData(0, positions[0]);
        prepareAngleData(1, positions[1]);
        prepareAngleData(2, positions[2]);
        syncAngleWriteData->setData(0, &syncAngles[0*MotorType::goalAngle.length]);
        syncAngleWriteData->setData(1, &syncAngles[1*MotorType::goalAngle.length]);
        syncAngleWriteData->setData(2, &syncAngles[2*MotorType::goalAngle.length]);

        float sent[3];
        for(int i = 0;i<3;i++) {
            float v = 0.0f;
            char* param = &syncAngles[i*MotorType::goalAngle.length];
            for(int j = 0; j<MotorType::goalAngle.length; j++)
            {
                v += (int)(param[j] << 8*j);
            }
            sent[i] = v*base.getAngleFromValue();
        }

        ComMgr::Instance().printfln(DEBUG_HEADER, "Sending syncAngles: %f/%f/%f\n",
                                    sent[0],
                                    sent[1],
                                    sent[2]);
        syncAngleWriteData->send();
        savePositions(positions);
    }

    MotorType* getXLlist() {
        return XLlist;
    }

    void fetchAngles(float *angles) {
//    setTorque(false);
        base.getCurrentAngle(angles[0]);
        elbow.getCurrentAngle(angles[1]);
        wrist.getCurrentAngle(angles[2]);
        //setTorque(true);
    }

    void setTorque(bool enabled) {
        syncToggleTorqueWriteData->setMotorID(0, base.getId());
        syncToggleTorqueWriteData->setMotorID(1, elbow.getId());
        syncToggleTorqueWriteData->setMotorID(2, wrist.getId());

        char toggleData[] = {enabled};
        syncToggleTorqueWriteData->setData(0, toggleData);
        syncToggleTorqueWriteData->setData(1, toggleData);
        syncToggleTorqueWriteData->setData(2, toggleData);

        syncToggleTorqueWriteData->send();
    }

    void update() {
        if(status != MOVING) {
            if(mute) {
                if(millis()-lastMuteCheck >= MUTE_ARM_CHECK_DELAY) {
                    float tmp = 0.0f;
                    if(ask(MotorType::goalAngle, base, tmp, true) && ask(MotorType::goalAngle, elbow, tmp, true) && ask(MotorType::goalAngle, wrist, tmp, true)) {
                        mute = false;
                        ComMgr::Instance().printfln(EVENT_HEADER, "armIsSpeaking %s", sideName);
                    }
                    lastMuteCheck = millis();
                }
            }

            gotoNextPosition();
        }
        if(status == MOVING) {
            checkArmMovement();
        }

    }

    /**
     * Vérifies où en sont les XL. Si le bras est dans l'état 'ArmStatus::OK' ou 'ArmStatus::WRONG_POSITION', cette méthode retourne immédiatement
     * Si le bras n'est pas à la position demandée, cette méthode rappelle 'setPosition' pour essayer de corriger la position
     */
    void checkArmMovement() {
        if(status == OK || status == WRONG_POSITION) {
            movementStartTime = millis();
            return;
        }

        // on bouge, il faut tester les XL
        if(mute) { // on ne reçoit aucune réponse des bras, on va attendre un petit temps que les bras bougent
            if(millis()-movementStartTime >= MUTE_ARM_DELAY) {
                status = WRONG_POSITION;
                ComMgr::Instance().printfln(EVENT_HEADER, "armFinishedMovement %s", sideName);
            }
            return;
        }
        bool wristMoving;
        bool elbowMoving;
        bool baseMoving;
        baseMoving = !askSpeed(base);
        elbowMoving = !askSpeed(elbow);
        wristMoving = !askSpeed(wrist);
        ComMgr::Instance().printfln(DEBUG_HEADER, "Arm Moving = {%i %i %i}", baseMoving, elbowMoving, wristMoving);
        if(wristMoving || elbowMoving || baseMoving) {
            status = MOVING;
            return;
        }
        if (askPosition(base, targetPositions[0]) && askPosition(elbow, targetPositions[1]) && askPosition(wrist, targetPositions[2])) { // si chacune des positions est correcte
            status = OK;
            if(writeIndex == currentPositionIndex) {
                ComMgr::Instance().printfln(EVENT_HEADER, "armFinishedMovement %s", sideName);
            }
        } else { // si les positions n'ont pas été atteintes
            if (retryMovementAttempts >= MAX_RETRY_ATTEMPTS) { // on a essayé trop de fois
                ComMgr::Instance().printfln(DEBUG_HEADER,
                                            "Position non atteinte sur le bras (%i-%i-%i) après %i tentatives, abandon",
                                            base.getId(), elbow.getId(), wrist.getId(), retryMovementAttempts);
                ComMgr::Instance().printfln(EVENT_HEADER, "armPositionFail %s", sideName);
                status = WRONG_POSITION;
            } else { // on retente
                ComMgr::Instance().printfln(DEBUG_HEADER,
                                            "Position non atteinte sur le bras (%i-%i-%i), nouvelle tentative (n°%i)",
                                            base.getId(), elbow.getId(), wrist.getId(), retryMovementAttempts);
                retryMovementAttempts++;
                ComMgr::Instance().printfln(EVENT_HEADER, "armRetrying %s", sideName);
                status = WRONG_POSITION;
                setPosition(targetPositions, false); // on renvoie l'ordre de position!
            }
        }
    }

    ArmStatus getStatus() {
        return status;
    }

private:

    /**
     * Si on a une autre position à laquelle il faut aller, on y va!
     */
    void gotoNextPosition() {
        if(writeIndex != currentPositionIndex) {
            ComMgr::Instance().printfln(DEBUG_HEADER, "Arm %s goto next position: %f %f %f (writeIndex: %i; currentPositionIndex: %i", sideName, positionsAsked[currentPositionIndex][0], positionsAsked[currentPositionIndex][1], positionsAsked[currentPositionIndex][2], writeIndex, currentPositionIndex);
            uint16_t positionIndex = currentPositionIndex;
            currentPositionIndex++;
            currentPositionIndex %= ARM_POSITION_BUFFER_SIZE;
            setPosition(positionsAsked[positionIndex]);
        }
    }

    void prepareAngleData(unsigned int motorIndex, float angle) {
        uint32_t targetAngleValue = (uint32_t)(angle/base.getAngleFromValue());
        char* parameter = &syncAngles[motorIndex * MotorType::goalAngle.length];

        for(int i = 0;i<MotorType::goalAngle.length;i++) {
            parameter[i] = targetAngleValue & 0xFF;
            targetAngleValue = targetAngleValue >> 8;
        }
    }

    /**
     * Sauvegarde la position cible pour pouvoir réessayer le mouvement
     * @param positions la position à atteindre
     */
    void savePositions(const float *positions) {
        this->targetPositions[0] = positions[0];
        this->targetPositions[1] = positions[1];
        this->targetPositions[2] = positions[2];
    }

    bool askPosition(MotorType &xl, const float askedPosition) {
        float value = 0.0f;
        bool valid = xl.getCurrentAngle(value);
        float value2 = 0.0f;
        bool valid2 = ask(MotorType::goalAngle, xl, value2);
        value2 *= xl.getAngleFromValue();
        if(valid && valid2) {
            ComMgr::Instance().printfln(DEBUG_HEADER, "Current position for XL n°%i is %f\n", xl.getId(), value);
            if(ABS(value-askedPosition) >= POSITION_THRESHOLD) {
                ComMgr::Instance().printfln(DEBUG_HEADER, "Threshold fail on XL %i (goal: %f; expected: %f; reached: %f; diff: %f)", xl.getId(), value2, askedPosition, value, ABS(value-askedPosition));
                return false;
            } else {
                ComMgr::Instance().printfln(DEBUG_HEADER, "Threshold success on XL %i (goal: %f; expected: %f; reached: %f; diff: %f)", xl.getId(), value2, askedPosition, value, ABS(value-askedPosition));
            }
        } else {
            ComMgr::Instance().printfln(DEBUG_HEADER, "Invalid packet received when asking position");
        }
        return valid;
    }

    bool askSpeed(MotorType &xl) {
        int value = 0;
        bool valid = ask(MotorType::currentVelocity, xl, value);
        if(valid) {
            ComMgr::Instance().printfln(DEBUG_HEADER, "Current velocity for XL n°%i is %i", xl.getId(), value);
        } else {
            ComMgr::Instance().printfln(DEBUG_HEADER, "Invalid packet received when asking threshold");
        }
        return valid && ABS(value) < VELOCITY_THRESHOLD;
    }

    bool ask(const DynamixelAccessData& data, MotorType& xl, float& value, bool force=false) {
        if(mute && !force) {
            ComMgr::Instance().printfln(DEBUG_HEADER, "Mute arm (base #%i)", base.getId());
            return false;
        }
        ComMgr::Instance().printfln(DEBUG_HEADER, "Asking for movement...");
        DynamixelPacketData* requestPacket = xl.makeReadPacket(data);
        const char* answer = manager.sendPacket(requestPacket);
        bool validPacket = xl.decapsulatePacket(answer, value);
        if(validPacket) {
            attemptsBeforeMute = ARM_ATTEMPTS_BEFORE_MUTE;
        } else {
            ComMgr::Instance().printfln(DEBUG_HEADER, "Invalid packet, contents of rxBuffer:");
            for(unsigned int i = 0; i < 30; i++)
            {
                ComMgr::Instance().printfln(DEBUG_HEADER, "%i", (int)manager.rxBuffer[i]);
            }
            ComMgr::Instance().printfln(DEBUG_HEADER, "END OF RX BUFFER");
            if(attemptsBeforeMute > 0) {
                attemptsBeforeMute--;
            } else {
                ComMgr::Instance().printfln(EVENT_HEADER, "armIsMute %s", sideName);
                mute = true;
            }
        }
        return validPacket;
    }

    bool ask(const DynamixelAccessData& data, MotorType& xl, int& value, bool force=false) {
        if(mute && !force) {
            ComMgr::Instance().printfln(DEBUG_HEADER, "Mute arm (base #%i)", base.getId());
            return false;
        }
        ComMgr::Instance().printfln(DEBUG_HEADER, "Asking for movement...");
        DynamixelPacketData* requestPacket = xl.makeReadPacket(data);
        const char* answer = manager.sendPacket(requestPacket);
        bool validPacket = xl.decapsulatePacket(answer, value);
        if(validPacket) {
            attemptsBeforeMute = ARM_ATTEMPTS_BEFORE_MUTE;
        } else {
            ComMgr::Instance().printfln(DEBUG_HEADER, "Invalid packet, contents of rxBuffer:");
            for(unsigned int i = 0; i < 30; i++)
            {
                ComMgr::Instance().printfln(DEBUG_HEADER, "%i", (int)manager.rxBuffer[i]);
            }
            ComMgr::Instance().printfln(DEBUG_HEADER, "END OF RX BUFFER");
            if(attemptsBeforeMute > 0) {
                attemptsBeforeMute--;
            } else {
                ComMgr::Instance().printfln(EVENT_HEADER, "armIsMute %s", sideName);
                mute = true;
            }
        }
        return validPacket;
    }

};


#endif //TECHTHETACHYON_LOWLEVEL_ARM_H
