//
// Created by jglrxavpok on 07/01/19.
//

#include <Utils/defines.h>
#include <COM/ComMgr.h>
#include "Arm.h"

Arm::Arm(const char* sideName, DynamixelManager& manager, XL430 &base, XL430 &elbow, XL430 &wrist): sideName(sideName), manager(manager), base(base), elbow(elbow), wrist(wrist) {}

void Arm::initTorque() {
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
    char velocityLimit[] = {100, 0, 0, 0};
    syncVelocityLimit->setData(0, velocityLimit);
    syncVelocityLimit->setData(1, velocityLimit);
    syncVelocityLimit->setData(2, velocityLimit);
    syncVelocityLimit->send();

    ComMgr::Instance().printf(DEBUG_HEADER, "Configuring moving threshold... ");
    char threshold[] = {10, 0, 0, 0};
    ComMgr::Instance().printfln(DEBUG_HEADER, "id:%i => %i", base.getId(), base.decapsulatePacket(manager.sendPacket(base.makeWritePacket(XL430::xl430MovingThreshold, threshold))));
    ComMgr::Instance().printfln(DEBUG_HEADER, "id:%i => %i", elbow.getId(), elbow.decapsulatePacket(manager.sendPacket(elbow.makeWritePacket(XL430::xl430MovingThreshold, threshold))));
    ComMgr::Instance().printfln(DEBUG_HEADER, "id:%i => %i", wrist.getId(), wrist.decapsulatePacket(manager.sendPacket(wrist.makeWritePacket(XL430::xl430MovingThreshold, threshold))));

    ComMgr::Instance().printfln(DEBUG_HEADER, "Done!");

    ComMgr::Instance().printf(DEBUG_HEADER, "Toggling torque (and locking EEPROM)... ");
    setTorque(true);
    ComMgr::Instance().printfln(DEBUG_HEADER, "Done!");

    askThreshold(base);
    askThreshold(elbow);
    askThreshold(wrist);
    askOffset(base);
    askOffset(elbow);
    askOffset(wrist);
}

void Arm::prepareAngleData(unsigned int motorIndex, float angle) {
    uint32_t targetAngleValue = (uint32_t)(angle/base.getAngleFromValue());
    char* parameter = &syncAngles[motorIndex * XL430::xl430GoalAngle.length];

    for(int i = 0;i<XL430::xl430GoalAngle.length;i++) {
        parameter[i] = targetAngleValue & 0xFF;
        targetAngleValue = targetAngleValue >> 8;
    }
}

void Arm::setPosition(const float* positions) {
    // récupère la position courante au cas où le mouvement échoue. Dans ce cas, les XL vont retourner à cette position courante et réessayer le mouvement
    float basePos = 0.0f;
    float elbowPos = 0.0f;
    float wristPos = 0.0f;
    // lecture des positions, avec réessai si échec de la comm
    while(!base.getCurrentAngle(basePos)) {
        ComMgr::Instance().printfln(DEBUG_HEADER, "Echec de la lecture de la position actuelle du XL n°%i, réessai", base.getId());
        delay(1);
    }
    while(!elbow.getCurrentAngle(elbowPos)) {
        ComMgr::Instance().printfln(DEBUG_HEADER, "Echec de la lecture de la position actuelle du XL n°%i, réessai", elbow.getId());
        delay(1);
    }
    while(!wrist.getCurrentAngle(wristPos)) {
        ComMgr::Instance().printfln(DEBUG_HEADER, "Echec de la lecture de la position actuelle du XL n°%i, réessai", wrist.getId());
        delay(1);
    }
    float actualPositions[] = {basePos, elbowPos, wristPos};
    prepareAngleData(0, positions[0]);
    prepareAngleData(1, positions[1]);
    prepareAngleData(2, positions[2]);
    syncAngleWriteData->setData(0, &syncAngles[0*XL430::xl430GoalAngle.length]);
    syncAngleWriteData->setData(1, &syncAngles[1*XL430::xl430GoalAngle.length]);
    syncAngleWriteData->setData(2, &syncAngles[2*XL430::xl430GoalAngle.length]);

    float sent[3];
    for(int i = 0;i<3;i++) {
        float v = 0.0f;
        char* param = &syncAngles[i*XL430::xl430GoalAngle.length];
        for(int j = 0; j<XL430::xl430GoalAngle.length; j++)
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
    waitForStop(positions, actualPositions);
}

void Arm::waitForStop(const float* positions, const float* previousPositions) {
    bool wristMoving;
    bool elbowMoving;
    bool baseMoving;

    do {
        askThreshold(base);
        askThreshold(elbow);
        askThreshold(wrist);
        baseMoving = !askSpeed(base);
        elbowMoving = !askSpeed(elbow);
        wristMoving = !askSpeed(wrist);
        ComMgr::Instance().printfln(DEBUG_HEADER, "Arm Moving = {%i %i %i}", baseMoving, elbowMoving, wristMoving); // TODO
    } while (wristMoving || elbowMoving || baseMoving);
    if( ! (askPosition(base, positions[0]) && askPosition(elbow, positions[1]) && askPosition(wrist, positions[2]))) { // si les positions n'ont pas été atteintes
        if(retryMovementAttempts >= MAX_RETRY_ATTEMPTS) {
            ComMgr::Instance().printfln(DEBUG_HEADER, "Position non atteinte sur le bras (%i-%i-%i) après %i tentatives, abandon", base.getId(), elbow.getId(), wrist.getId(), retryMovementAttempts);
            ComMgr::Instance().printfln(EVENT_HEADER, "armPositionFail %s", sideName);
        } else {
            ComMgr::Instance().printfln(DEBUG_HEADER, "Position non atteinte sur le bras (%i-%i-%i), nouvelle tentative (n°%i)", base.getId(), elbow.getId(), wrist.getId(), retryMovementAttempts);
            retryMovementAttempts++;
            delay(10);
            setPosition(previousPositions); // on renvoie l'ordre de position!

            delay(10);
            setPosition(positions); // on renvoie l'ordre de position!
        }
    } else {
        retryMovementAttempts = 0;
    }
}

bool Arm::askPosition(XL430 &xl, const float askedPosition) {
    float value = 0.0f;
    bool valid = xl.getCurrentAngle(value);
    float value2 = 0.0f;
    bool valid2 = ask(XL430::xl430GoalAngle, xl, value2);
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

bool Arm::askSpeed(XL430 &xl) {
    int value = 0;
    bool valid = ask(XL430::xl430CurrentVelocity, xl, value);
    if(valid) {
        ComMgr::Instance().printfln(DEBUG_HEADER, "Current velocity for XL n°%i is %i", xl.getId(), value);
    } else {
        ComMgr::Instance().printfln(DEBUG_HEADER, "Invalid packet received when asking threshold");
    }
    return valid && value < VELOCITY_THRESHOLD;
}

bool Arm::askThreshold(XL430 &xl) {
    int value;
    bool valid = ask(XL430::xl430MovingThreshold, xl, value);
    if(valid) {
        ComMgr::Instance().printfln(DEBUG_HEADER, "Threshold for XL n°%i is %i", xl.getId(), value & ((1 << XL430::xl430MovingThreshold.length)-1));
    } else {
        ComMgr::Instance().printfln(DEBUG_HEADER, "Invalid packet received when asking threshold");
    }
    return valid;
}

bool Arm::askOffset(XL430 &xl) {
    int value = 0;
    bool valid = ask(XL430::xl430MovingOffset, xl, value);
    if(valid) {
        ComMgr::Instance().printfln(DEBUG_HEADER, "MovingOffset for XL n°%i is %i", xl.getId(), value);
    } else {
        ComMgr::Instance().printfln(DEBUG_HEADER, "Invalid packet received when asking MovingStatus");
    }
    return valid && (value & 0x1) != 0;
}

bool Arm::ask(const DynamixelAccessData& data, XL430& xl, float& value) {
    ComMgr::Instance().printfln(DEBUG_HEADER, "Asking for movement...");
    DynamixelPacketData* requestPacket = xl.makeReadPacket(data);
    const char* answer = manager.sendPacket(requestPacket);
    return xl.decapsulatePacket(answer, value);
}

bool Arm::ask(const DynamixelAccessData& data, XL430& xl, int& value) {
    ComMgr::Instance().printfln(DEBUG_HEADER, "Asking for movement...");
    DynamixelPacketData* requestPacket = xl.makeReadPacket(data);
    const char* answer = manager.sendPacket(requestPacket);
    return xl.decapsulatePacket(answer, value);
}

XL430* Arm::getXLlist() {
    return XLlist;
}

void Arm::fetchAngles(float *angles) {
//    setTorque(false);
    base.getCurrentAngle(angles[0]);
    elbow.getCurrentAngle(angles[1]);
    wrist.getCurrentAngle(angles[2]);
    //setTorque(true);
}

void Arm::setTorque(bool enabled) {
    syncToggleTorqueWriteData->setMotorID(0, base.getId());
    syncToggleTorqueWriteData->setMotorID(1, elbow.getId());
    syncToggleTorqueWriteData->setMotorID(2, wrist.getId());

    char toggleData[] = {enabled};
    syncToggleTorqueWriteData->setData(0, toggleData);
    syncToggleTorqueWriteData->setData(1, toggleData);
    syncToggleTorqueWriteData->setData(2, toggleData);

    syncToggleTorqueWriteData->send();
}
