//
// Created by jglrxavpok on 07/01/19.
//

#include <Utils/defines.h>
#include <COM/ComMgr.h>
#include "Arm.h"

Arm::Arm(DynamixelManager& manager, XL430 &base, XL430 &elbow, XL430 &wrist): manager(manager), base(base), elbow(elbow), wrist(wrist) {}

void Arm::initTorque() {
    Serial.print("Unlocking EEPROM by disabling torque...");
    setTorque(false);
    Serial.println("Done!");

    char returnDelay[] = {(char)0xFA};
    Serial.print("Setting XL return delay to no delay... ");
    syncReturnDelay->setMotorID(0, base.getId());
    syncReturnDelay->setMotorID(1, elbow.getId());
    syncReturnDelay->setMotorID(2, wrist.getId());
    syncReturnDelay->setData(0, returnDelay);
    syncReturnDelay->setData(1, returnDelay);
    syncReturnDelay->setData(2, returnDelay);
    syncReturnDelay->send();
    Serial.println("Done!");
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

    syncMovingRead->setMotorID(0, base.getId());
    syncMovingRead->setMotorID(1, elbow.getId());
    syncMovingRead->setMotorID(2, wrist.getId());

    syncMovingStatus->setMotorID(0, base.getId());
    syncMovingStatus->setMotorID(1, elbow.getId());
    syncMovingStatus->setMotorID(2, wrist.getId());

    syncHardwareError->setMotorID(0, base.getId());
    syncHardwareError->setMotorID(1, elbow.getId());
    syncHardwareError->setMotorID(2, wrist.getId());

    Serial.print("Setting velocity limit... ");
    char velocityLimit[] = {100, 0, 0, 0};
    syncVelocityLimit->setMotorID(0, base.getId());
    syncVelocityLimit->setMotorID(1, elbow.getId());
    syncVelocityLimit->setMotorID(2, wrist.getId());
    syncVelocityLimit->setData(0, velocityLimit);
    syncVelocityLimit->setData(1, velocityLimit);
    syncVelocityLimit->setData(2, velocityLimit);
    syncVelocityLimit->send();

    Serial.print("Configuring moving threshold... ");
    char threshold[] = {10, 0, 0, 0};
    Serial.printf("id:%i => %i\n", base.getId(), base.decapsulatePacket(manager.sendPacket(base.makeWritePacket(XL430::xl430MovingThreshold, threshold))));
    Serial.printf("id:%i => %i\n", elbow.getId(), elbow.decapsulatePacket(manager.sendPacket(elbow.makeWritePacket(XL430::xl430MovingThreshold, threshold))));
    Serial.printf("id:%i => %i\n", wrist.getId(), wrist.decapsulatePacket(manager.sendPacket(wrist.makeWritePacket(XL430::xl430MovingThreshold, threshold))));

    Serial.println("Done!");

    Serial.print("Toggling torque (and locking EEPROM)... ");
    setTorque(true);
    Serial.println("Done!");

    askThreshold(base);
    askThreshold(elbow);
    askThreshold(wrist);
}

void Arm::prepareAngleData(unsigned int motorIndex, float angle) {
    uint32_t targetAngleValue = (uint32_t)(angle/0.088);
    char* parameter = &syncAngles[motorIndex*4];

    for(int i = 0;i<XL430::xl430GoalAngle.length;i++) {
        parameter[i] = targetAngleValue & 0xFF;
        targetAngleValue = targetAngleValue >> 8;
    }
}

void Arm::setPosition(const float* positions) {
    syncAngleWriteData->setMotorID(0, base.getId());
    syncAngleWriteData->setMotorID(1, elbow.getId());
    syncAngleWriteData->setMotorID(2, wrist.getId());
    prepareAngleData(0, positions[0]);
    prepareAngleData(1, positions[1]);
    prepareAngleData(2, positions[2]);
    syncAngleWriteData->setData(0, &syncAngles[0*XL430::xl430GoalAngle.length]);
    syncAngleWriteData->setData(1, &syncAngles[1*XL430::xl430GoalAngle.length]);
    syncAngleWriteData->setData(2, &syncAngles[2*XL430::xl430GoalAngle.length]);
    syncAngleWriteData->send();
    waitForStop();
}

void Arm::waitForStop() {
    //delay(750);
    bool wristMoving;
    bool elbowMoving;
    bool baseMoving;

    bool movingStates[] = {true, true, true};
    char movingStatuses[] = {0,0,0};
    char hardwareErrors[] = {0,0,0};
    do {
        askThreshold(base);
        askThreshold(elbow);
        askThreshold(wrist);
        baseMoving = askSpeed(base);
        elbowMoving = askSpeed(elbow);
        wristMoving = askSpeed(wrist);
        /*baseMoving = askMoving(base);
        elbowMoving = askMoving(elbow);
        wristMoving = askMoving(wrist);*/
        SerialUSB.printf("Moving = {%i %i %i}\n", baseMoving, elbowMoving, wristMoving); // TODO
        /*syncMovingRead->read((char*)movingStates);
        syncMovingStatus->read((char*)movingStatuses);
        syncHardwareError->read((char*)hardwareErrors);
        baseMoving = (movingStatuses[0] & 0x2) == 0;
        elbowMoving = (movingStatuses[1] & 0x2) == 0;
        wristMoving = (movingStatuses[2] & 0x2) == 0;
        SerialUSB.printf("Current movement status = {%i %i %i}\n", movingStatuses[0], movingStatuses[1], movingStatuses[2]); // TODO
        SerialUSB.printf("Currently moving = {%i %i %i}\n", movingStates[0], movingStates[1], movingStates[2]); // TODO
        SerialUSB.printf("Errors = {%i %i %i}\n", hardwareErrors[0], hardwareErrors[1], hardwareErrors[2]); // TODO

        if(movingStates[0] & 0x08 || movingStates[1] & 0x08 || movingStates[2] & 0x08) { // 'following error' -> le XL a échoué à arriver à la position demandée
            SerialUSB.println("Echec du suivi de position");
            syncAngleWriteData->send();
            waitForStop();
            break;
        }*/
    } while (wristMoving || elbowMoving || baseMoving);
}

bool Arm::askSpeed(XL430 &xl) {
    int value;
    bool valid = ask(XL430::xl430CurrentVelocity, xl, value);
    if(valid) {
        Serial.printf("Current velocity for XL n°%i is %i\n", xl.getId(), value & ((1 << XL430::xl430CurrentVelocity.length)-1));
    } else {
        Serial.printf("Invalid packet received when asking threshold");
    }
    return valid && value < 10;
}

bool Arm::askThreshold(XL430 &xl) {
    int value;
    bool valid = ask(XL430::xl430MovingThreshold, xl, value);
    if(valid) {
        Serial.printf("Threshold for XL n°%i is %i\n", xl.getId(), value & ((1 << XL430::xl430MovingThreshold.length)-1));
    } else {
        Serial.printf("Invalid packet received when asking threshold");
    }
    return valid;
}

bool Arm::askMoving(XL430 &xl) {
    int value;
    bool valid = ask(XL430::xl430MovingStatus, xl, value);
    if(valid) {
        Serial.printf("MovingStatus for XL n°%i is %i\n", xl.getId(), value & ((1 << XL430::xl430MovingStatus.length)-1));
    } else {
        Serial.printf("Invalid packet received when asking MovingStatus");
    }
    return valid && (value & 0x1) != 0;
}

bool Arm::ask(const DynamixelAccessData& data, XL430& xl, int& value) {
    Serial.println("Asking for movement...");
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
