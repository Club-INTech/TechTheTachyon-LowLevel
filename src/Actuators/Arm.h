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
    SyncWrite* syncVelocityLimit = new SyncWrite(manager, 3, XL430::xl430VelocityLimit);
    SyncWrite* syncAngleWriteData = new SyncWrite(manager, 3, XL430::xl430GoalAngle);
    SyncWrite* syncToggleTorqueWriteData = new SyncWrite(manager, 3, XL430::xl430TorqueEnable);
    SyncWrite* syncReturnDelay = new SyncWrite(manager, 3, XL430::xl430ReturnDelay);
    SyncRead* syncMovingRead = new SyncRead(manager, 3, XL430::xl430Moving);
    SyncRead* syncMovingStatus = new SyncRead(manager, 3, XL430::xl430MovingStatus);
    SyncRead* syncHardwareError = new SyncRead(manager, 3, XL430::xl430HardwareError);
    void prepareAngleData(unsigned int motorIndex, float angle);
    bool askSpeed(XL430& xl);
    bool askOffset(XL430& xl);
    bool askThreshold(XL430& xl);
    bool askPosition(XL430& xl, const float askedPosition);
    bool ask(const DynamixelAccessData& data, XL430& xl, int& value);
    bool ask(const DynamixelAccessData& data, XL430& xl, float& value);

public:
    Arm(DynamixelManager& manager, XL430& base, XL430& elbow, XL430& wrist);
    void initTorque();
    void setPosition(const float* position);
    XL430* getXLlist();

    void fetchAngles(float angles[3]);

    void setTorque(bool enabled);

    /**
     * Attends que le bras ait fini de bouger. Si le bras n'est pas à la position demandée, cette méthode rappelle 'setPosition' pour essayer de corriger la position
     */
    void waitForStop(const float* positions);
};


#endif //TECHTHETACHYON_LOWLEVEL_ARM_H
