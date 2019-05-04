//
// Created by jglrxavpok on 07/01/19.
//

#include "Arm.h"

Arm::Arm(DynamixelManager& manager, AX12 &base, AX12 &elbow, AX12 &wrist): manager(manager), base(base), elbow(elbow), wrist(wrist) {}

void Arm::initTorque() {
    Serial.print("Toggling torque... ");
    base.toggleTorque(true);
    elbow.toggleTorque(true);
    wrist.toggleTorque(true);

    syncToggleTorqueWriteData->setMotorID(0, base.getId());
    syncToggleTorqueWriteData->setMotorID(1, elbow.getId());
    syncToggleTorqueWriteData->setMotorID(2, wrist.getId());
    syncAngleWriteData->setMotorID(0, base.getId());
    syncAngleWriteData->setMotorID(1, elbow.getId());
    syncAngleWriteData->setMotorID(2, wrist.getId());

    syncLedData->setMotorID(0, base.getId());
    syncLedData->setMotorID(1, elbow.getId());
    syncLedData->setMotorID(2, wrist.getId());

    char toggleData[] = {1};
    syncToggleTorqueWriteData->setData(0, toggleData);
    syncToggleTorqueWriteData->setData(1, toggleData);
    syncToggleTorqueWriteData->setData(2, toggleData);

    syncLedData->setData(0, toggleData);
    syncLedData->setData(1, toggleData);
    syncLedData->setData(2, toggleData);

    syncToggleTorqueWriteData->send();
    Serial.println("Done!");
}

void Arm::prepareAngleData(unsigned int motorIndex, float angle) {
    uint32_t targetAngleValue = (uint32_t)(angle/base.getAngleFromValue());
    char* parameter = &syncAngles[motorIndex*AX12::goalAngle.length];

    for(int i = 0;i<AX12::goalAngle.length;i++) {
        parameter[i] = targetAngleValue & 0xFF;
        targetAngleValue = targetAngleValue >> 8;
    }
}

void Arm::setPosition(float* positions) {
    syncLedData->send();

    prepareAngleData(0, positions[0]);
    prepareAngleData(1, positions[1]);
    prepareAngleData(2, positions[2]);
    syncAngleWriteData->setData(0, &syncAngles[0*AX12::goalAngle.length]);
    syncAngleWriteData->setData(1, &syncAngles[1*AX12::goalAngle.length]);
    syncAngleWriteData->setData(2, &syncAngles[2*AX12::goalAngle.length]);
    syncAngleWriteData->send();
}

AX12* Arm::getXLlist() {
    return XLlist;
}