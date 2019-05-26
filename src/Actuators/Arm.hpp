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
    SyncWrite* syncVelocityLimit = new SyncWrite(manager, 3, MotorType::velocityLimit);
    SyncWrite* syncAngleWriteData = new SyncWrite(manager, 3, MotorType::goalAngle);
    SyncWrite* syncToggleTorqueWriteData = new SyncWrite(manager, 3, MotorType::torqueEnable);
    SyncWrite* syncReturnDelay = new SyncWrite(manager, 3, MotorType::returnDelay);
    SyncRead* syncMovingRead = new SyncRead(manager, 3, MotorType::moving);
    SyncRead* syncMovingStatus = new SyncRead(manager, 3, MotorType::movingStatus);
    SyncRead* syncHardwareError = new SyncRead(manager, 3, MotorType::hardwareError);
    SyncWrite* syncWatchdog = new SyncWrite(manager, 3, MotorType::watchdog);
    SyncWrite* syncHomePos = new SyncWrite(manager, 3, MotorType::movingOffset);
    ArmStatus status = OK;

    float targetPositions[3] = {0.0f};
    uint16_t retryMovementAttempts = 0;
    float positionsAsked[ARM_POSITION_BUFFER_SIZE][3] = { {0.0f} };
    bool noRetryAsked[ARM_POSITION_BUFFER_SIZE] = { false };
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

    bool noRetry = false;

public:
    Arm(const char* sideName, DynamixelManager& manager, MotorType* list): sideName(sideName), manager(manager), XLlist(list), base(list[0]), elbow(list[1]), wrist(list[2]), status(ArmStatus::OK) {

    }

    void initTorque() {
        setupSync(syncMovingRead);
        setupSync(syncMovingStatus);
        setupSync(syncHardwareError);
        setupSync(syncAngleWriteData);
        setupSync(syncVelocityLimit);
        setupSync(syncReturnDelay);
        setupSync(syncWatchdog);
        setupSync(syncHomePos);
        setupSync(syncToggleTorqueWriteData);

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
        char velocityLimit[] = {0, 0, 0, 0};
        syncVelocityLimit->setData(0, velocityLimit);
        syncVelocityLimit->setData(1, velocityLimit);
        syncVelocityLimit->setData(2, velocityLimit);
        syncVelocityLimit->send();

        ComMgr::Instance().printfln(DEBUG_HEADER, "Setting home position");
        char homePos[MotorType::movingOffset.length*3] = {0,0,0,0, 0,0,0,0, 0,0,0,0};
/*        prepareAngleData(homePos, 0, xlOffsets[base.getId()-1]);
        prepareAngleData(homePos, 1, xlOffsets[elbow.getId()-1]);
        prepareAngleData(homePos, 2, xlOffsets[wrist.getId()-1]);*/
        syncHomePos->setData(0, &homePos[0*MotorType::movingOffset.length]);
        syncHomePos->setData(1, &homePos[1*MotorType::movingOffset.length]);
        syncHomePos->setData(2, &homePos[2*MotorType::movingOffset.length]);
        syncHomePos->send();

        ComMgr::Instance().printf(DEBUG_HEADER, "Configuring moving threshold... ");
        char threshold[] = {10, 0, 0, 0};
        ComMgr::Instance().printfln(DEBUG_HEADER, "id:%i => %i", base.getId(), base.decapsulatePacket(manager.sendPacket(base.makeWritePacket(MotorType::movingThreshold, threshold))));
        ComMgr::Instance().printfln(DEBUG_HEADER, "id:%i => %i", elbow.getId(), elbow.decapsulatePacket(manager.sendPacket(elbow.makeWritePacket(MotorType::movingThreshold, threshold))));
        ComMgr::Instance().printfln(DEBUG_HEADER, "id:%i => %i", wrist.getId(), wrist.decapsulatePacket(manager.sendPacket(wrist.makeWritePacket(MotorType::movingThreshold, threshold))));

        ComMgr::Instance().printfln(DEBUG_HEADER, "Done!");

        ComMgr::Instance().printf(DEBUG_HEADER, "Toggling torque (and locking EEPROM)... ");
        setTorque(true);
        ComMgr::Instance().printfln(DEBUG_HEADER, "Done!");
        ComMgr::Instance().printfln(DEBUG_HEADER, "Resetting watchdog... ");
        char data[] = { 0 }; // reset le watchdog
        syncWatchdog->setData(0, data);
        syncWatchdog->setData(1, data);
        syncWatchdog->setData(2, data);
        resetWatchdog();
        ComMgr::Instance().printfln(DEBUG_HEADER, "Done!");
    }

    void setPositionNoRetry(const float* positions, bool resetRetryCounter = true) {
        setPosition(positions, resetRetryCounter, true);
    }

    void setPosition(const float* positions, bool resetRetryCounter = true, bool noRetry = false) {
        if(status == MOVING) {
            positionsAsked[writeIndex][0] = positions[0];
            positionsAsked[writeIndex][1] = positions[1];
            positionsAsked[writeIndex][2] = positions[2];
            noRetryAsked[writeIndex] = noRetry;
            writeIndex++;
            writeIndex %= ARM_POSITION_BUFFER_SIZE;
            return;
        }
        
        this->noRetry = noRetry;

        if(resetRetryCounter) {
            retryMovementAttempts = 0;
        }
        movementStartTime = millis();

        status = MOVING;
        prepareAngleData(syncAngles, 0, positions[0]+xlOffsets[base.getId()-1]);
        prepareAngleData(syncAngles, 1, positions[1]+xlOffsets[elbow.getId()-1]);
        prepareAngleData(syncAngles, 2, positions[2]+xlOffsets[wrist.getId()-1]);
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
        resetWatchdog();
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
                    resetWatchdog();
                    if(ask(MotorType::goalAngle, base, tmp, true) && ask(MotorType::goalAngle, elbow, tmp, true) && ask(MotorType::goalAngle, wrist, tmp, true)) {
                        mute = false;
                        attemptsBeforeMute = ARM_ATTEMPTS_BEFORE_MUTE;
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

    void rebootXLsIfNecessary() {
        rebootIfNeeded(base);
        rebootIfNeeded(elbow);
        rebootIfNeeded(wrist);
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
        resetWatchdog();
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
            if (retryMovementAttempts >= MAX_RETRY_ATTEMPTS || noRetry) { // on a essayé trop de fois
                ComMgr::Instance().printfln(DEBUG_HEADER,
                                            "Position non atteinte sur le bras (%i-%i-%i) après %i tentatives, abandon",
                                            base.getId(), elbow.getId(), wrist.getId(), retryMovementAttempts);
                if(writeIndex != currentPositionIndex) { // si on a d'autres mouvements à faire
                    ComMgr::Instance().printfln(EVENT_HEADER, "armPositionFailing %s", sideName);
                } else {
                    ComMgr::Instance().printfln(EVENT_HEADER, "armPositionFail %s", sideName);
                }
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

    void resetWatchdog() {
        syncWatchdog->send();
    }

    void setupSync(SyncRead* syncRead) {
        syncRead->setMotorID(0, base.getId());
        syncRead->setMotorID(1, elbow.getId());
        syncRead->setMotorID(2, wrist.getId());
    }

    void setupSync(SyncWrite* syncWrite) {
        syncWrite->setMotorID(0, base.getId());
        syncWrite->setMotorID(1, elbow.getId());
        syncWrite->setMotorID(2, wrist.getId());
    }

    /**
     * Si on a une autre position à laquelle il faut aller, on y va!
     */
    void gotoNextPosition() {
        if(writeIndex != currentPositionIndex) {
            ComMgr::Instance().printfln(DEBUG_HEADER, "Arm %s goto next position: %f %f %f (writeIndex: %i; currentPositionIndex: %i", sideName, positionsAsked[currentPositionIndex][0], positionsAsked[currentPositionIndex][1], positionsAsked[currentPositionIndex][2], writeIndex, currentPositionIndex);
            uint16_t positionIndex = currentPositionIndex;
            currentPositionIndex++;
            currentPositionIndex %= ARM_POSITION_BUFFER_SIZE;
            if(noRetryAsked[positionIndex]) {
                setPositionNoRetry(positionsAsked[positionIndex]);
            } else {
                setPosition(positionsAsked[positionIndex]);
            }
        }
    }

    void prepareAngleData(char* angles, unsigned int motorIndex, float angle) {
        uint32_t targetAngleValue = (uint32_t)(angle/base.getAngleFromValue());
        char* parameter = &angles[motorIndex * MotorType::goalAngle.length];

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
        ComMgr::Instance().printfln(DEBUG_HEADER, "Asking for Dynamixel data %02X%02X", data.address[1], data.address[0]);
        DynamixelPacketData* requestPacket = xl.makeReadPacket(data);
        const char* answer = manager.sendPacket(requestPacket);
        bool validPacket = xl.decapsulatePacket(answer, value);
        if(validPacket) {
            attemptsBeforeMute = ARM_ATTEMPTS_BEFORE_MUTE;
        } else if(!force) /* Si on force la lecture, c'est qu'on veut vérifier que le bras est toujours muet, pas besoin de le redire */ {
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
        ComMgr::Instance().printfln(DEBUG_HEADER, "Asking for Dynamixel data %02X%02X", data.address[1], data.address[0]);
        DynamixelPacketData* requestPacket = xl.makeReadPacket(data);
        const char* answer = manager.sendPacket(requestPacket);
        bool validPacket = xl.decapsulatePacket(answer, value);
        if(validPacket) {
            attemptsBeforeMute = ARM_ATTEMPTS_BEFORE_MUTE;
        } else if(!force) /* Si on force la lecture, c'est qu'on veut vérifier que le bras est toujours muet, pas besoin de le redire */ {
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

    void rebootIfNeeded(MotorType& xl) {
        char* answer = manager.sendPacket(xl.makeReadPacket(MotorType::hardwareError));
        int error = 0;
        bool valid = xl.decapsulatePacket(answer, error);
        if(valid) {
            if( ! xl.checkAlert(answer)) {
                // on lance un reboot
                ComMgr::Instance().printfln(DEBUG_HEADER, "Rebooting xl %i", xl.getId());
                xl.reboot();
            } else {
                // on réactive le couple périodiquement pour être sûr que le XL désasservi est réasservi dès que possible
                xl.toggleTorque(true);
            }
        }
    }

};


#endif //TECHTHETACHYON_LOWLEVEL_ARM_H
