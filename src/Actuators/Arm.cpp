//
// Created by jglrxavpok on 07/01/19.
//

#include "Arm.h"

Arm::Arm(DynamixelManager& manager, XL430 &base, XL430 &elbow, XL430 &wrist): manager(manager), base(base), elbow(elbow), wrist(wrist) {

}

void Arm::prepareAngleData(unsigned int motorIndex, float angle) {
    uint32_t targetAngleValue = (uint32_t)(angle/0.088);
    char* parameter = &syncAngles[motorIndex*4];

    for(int i = 0;i<XL430::xl430GoalAngle.length;i++) {
        parameter[i] = targetAngleValue & 0xFF;
        targetAngleValue = targetAngleValue >> 8;
    }
}

void Arm::setPosition(float* positions) {
    syncWriteData->setMotorID(0, base.getId());
    syncWriteData->setMotorID(1, elbow.getId());
    syncWriteData->setMotorID(2, wrist.getId());
    prepareAngleData(0, positions[0]);
    prepareAngleData(1, positions[1]);
    prepareAngleData(2, positions[2]);
    syncWriteData->setData(0, &syncAngles[0*XL430::xl430GoalAngle.length]);
    syncWriteData->setData(1, &syncAngles[1*XL430::xl430GoalAngle.length]);
    syncWriteData->setData(2, &syncAngles[2*XL430::xl430GoalAngle.length]);
    syncWriteData->send();
}