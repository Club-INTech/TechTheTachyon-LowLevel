//
// Created by jglrxavpok on 07/01/19.
//

#ifndef TECHTHETACHYON_LOWLEVEL_ARM_H
#define TECHTHETACHYON_LOWLEVEL_ARM_H


#include <XL430.h>
#include <SyncWrite.h>

class Arm {
private:
    DynamixelManager& manager;
    XL430& base;
    XL430& elbow;
    XL430& wrist;
    char* syncAngles = new char[XL430::xl430GoalAngle.length*3];
    XL430* XLlist = new XL430[3]{base,elbow,wrist};
    SyncWrite* syncAngleWriteData = new SyncWrite(manager, 3, (uint16_t ) (XL430::xl430GoalAngle.address[0] | (XL430::xl430GoalAngle.address[1] << 8)), XL430::xl430GoalAngle.length);
    SyncWrite* syncToggleTorqueWriteData = new SyncWrite(manager, 3, (uint16_t ) (XL430::xl430TorqueEnable.address[0] | (XL430::xl430TorqueEnable.address[1] << 8)), XL430::xl430TorqueEnable.length);
    void prepareAngleData(unsigned int motorIndex, float angle);

public:
    Arm(DynamixelManager& manager, XL430& base, XL430& elbow, XL430& wrist);
    void initTorque();
    void setPosition(float* position);
    XL430* getXLlist();
};


#endif //TECHTHETACHYON_LOWLEVEL_ARM_H
