//
// Created by jglrxavpok on 07/01/19.
//

#ifndef TECHTHETACHYON_LOWLEVEL_ARM_H
#define TECHTHETACHYON_LOWLEVEL_ARM_H


#include <XL430.h>
#include <SyncWrite.h>
#include <SyncRead.h>

class Arm {
private:
    DynamixelManager& manager;
    XL430& base;
    XL430& elbow;
    XL430& wrist;
    char* syncAngles = new char[XL430::xl430GoalAngle.length*3];
    XL430* XLlist = new XL430[3]{base,elbow,wrist};
    SyncWrite* syncVelocityLimit = new SyncWrite(manager, 3, (uint16_t ) (XL430::xl430VelocityLimit.address[0] | (XL430::xl430VelocityLimit.address[1] << 8)), XL430::xl430VelocityLimit.length);
    SyncWrite* syncAngleWriteData = new SyncWrite(manager, 3, (uint16_t ) (XL430::xl430GoalAngle.address[0] | (XL430::xl430GoalAngle.address[1] << 8)), XL430::xl430GoalAngle.length);
    SyncWrite* syncToggleTorqueWriteData = new SyncWrite(manager, 3, (uint16_t ) (XL430::xl430TorqueEnable.address[0] | (XL430::xl430TorqueEnable.address[1] << 8)), XL430::xl430TorqueEnable.length);
    SyncWrite* syncReturnDelay = new SyncWrite(manager, 3, (uint16_t ) (XL430::xl430ReturnDelay.address[0] | (XL430::xl430ReturnDelay.address[1] << 8)), XL430::xl430ReturnDelay.length);
    SyncRead* syncMovingRead = new SyncRead(manager, 3, (uint16_t ) (XL430::xl430Moving.address[0] | (XL430::xl430Moving.address[1] << 8)), XL430::xl430Moving.length);
    SyncRead* syncMovingStatus = new SyncRead(manager, 3, (uint16_t ) (XL430::xl430MovingStatus.address[0] | (XL430::xl430MovingStatus.address[1] << 8)), XL430::xl430MovingStatus.length);
    SyncRead* syncHardwareError = new SyncRead(manager, 3, (uint16_t ) (XL430::xl430HardwareError.address[0] | (XL430::xl430HardwareError.address[1] << 8)), XL430::xl430HardwareError.length);
    void prepareAngleData(unsigned int motorIndex, float angle);

public:
    Arm(DynamixelManager& manager, XL430& base, XL430& elbow, XL430& wrist);
    void initTorque();
    void setPosition(const float* position);
    XL430* getXLlist();

    void fetchAngles(float angles[3]);

    void setTorque(bool enabled);

    /**
     * Attends que le bras ait fini de bouger
     */
    void waitForStop();
};


#endif //TECHTHETACHYON_LOWLEVEL_ARM_H
