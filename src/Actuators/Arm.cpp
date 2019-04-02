//
// Created by jglrxavpok on 07/01/19.
//

#include <Utils/defines.h>
#include <COM/ComMgr.h>
#include "Arm.h"

Arm::Arm(DynamixelManager& manager, XL430 &base, XL430 &elbow, XL430 &wrist): manager(manager), base(base), elbow(elbow), wrist(wrist) {}

void Arm::initTorque() {
    uint8_t returnDelay = 250;

    Serial.print("Setting XL return delay to no delay... ");
    syncReturnDelay->setMotorID(0, base.getId());
    syncReturnDelay->setMotorID(1, elbow.getId());
    syncReturnDelay->setMotorID(2, wrist.getId());
    syncReturnDelay->setData(0, (char*)&returnDelay);
    syncReturnDelay->setData(1, (char*)&returnDelay);
    syncReturnDelay->setData(2, (char*)&returnDelay);
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

    Serial.print("Toggling torque... ");
    setTorque(true);
    Serial.println("Done!");

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
    uint32_t velocityLimit = 100;
    syncVelocityLimit->setMotorID(0, base.getId());
    syncVelocityLimit->setMotorID(1, elbow.getId());
    syncVelocityLimit->setMotorID(2, wrist.getId());
    syncVelocityLimit->setData(0, (char*)&velocityLimit);
    syncVelocityLimit->setData(1, (char*)&velocityLimit);
    syncVelocityLimit->setData(2, (char*)&velocityLimit);
    syncVelocityLimit->send();

    Serial.println("Done!");
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
    bool wristMoving;
    bool elbowMoving;
    bool baseMoving;

    bool movingStates[] = {true, true, true};
    char movingStatuses[] = {0,0,0};
    char hardwareErrors[] = {0,0,0};
    do {
        syncMovingRead->read((char*)movingStates);
//        syncMovingStatus->read((char*)movingStatuses);
    //    syncHardwareError->read((char*)hardwareErrors);
        baseMoving = (movingStatuses[0] & 0x1) == 0;
        elbowMoving = (movingStatuses[1] & 0x1) == 0;
        wristMoving = (movingStatuses[2] & 0x1) == 0;
        SerialUSB.printf("Current movement status = {%i %i %i}\n", movingStatuses[0], movingStatuses[1], movingStatuses[2]); // TODO
        SerialUSB.printf("Currently moving = {%i %i %i}\n", movingStates[0], movingStates[1], movingStates[2]); // TODO
        SerialUSB.printf("Errors = {%i %i %i}\n", hardwareErrors[0], hardwareErrors[1], hardwareErrors[2]); // TODO
        SerialUSB.printf("Moving = {%i %i %i}\n", baseMoving, elbowMoving, wristMoving); // TODO
        if(movingStates[0] & 0x08 || movingStates[1] & 0x08 || movingStates[2] & 0x08) { // 'following error' -> le XL a échoué à arriver à la position demandée
            SerialUSB.println("Echec du suivi de position");
            syncAngleWriteData->send();
            waitForStop();
            break;
        }
    } while (wristMoving || elbowMoving || baseMoving);
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
