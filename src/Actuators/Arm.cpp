//
// Created by jglrxavpok on 07/01/19.
//

#include "Arm.h"

Arm::Arm(DynamixelManager& manager, XL430 &base, XL430 &elbow, XL430 &wrist): manager(manager), base(base), elbow(elbow), wrist(wrist) {}

void Arm::initTorque() {
    Serial.print("Toggling torque... ");
    setTorque(true);
    Serial.println("Done!");

    Serial.print("Setting velocity limit... ");
    uint32_t velocityLimit = 50;
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
