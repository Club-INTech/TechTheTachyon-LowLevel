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
    Serial.println("Setting torque...");
    syncToggleCoupleWriteData->setMotorID(0, base.getId());
    syncToggleCoupleWriteData->setMotorID(1, elbow.getId());
    syncToggleCoupleWriteData->setMotorID(2, wrist.getId());

    char toggleData[] = {1};
    syncToggleCoupleWriteData->setData(0, toggleData);
    syncToggleCoupleWriteData->setData(1, toggleData);
    syncToggleCoupleWriteData->setData(2, toggleData);

    syncToggleCoupleWriteData->send();


    Serial.println("Setting torque... Finished!");
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