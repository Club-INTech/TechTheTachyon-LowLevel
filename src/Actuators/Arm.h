//
// Created by jglrxavpok on 07/01/19.
//

#ifndef TECHTHETACHYON_LOWLEVEL_ARM_H
#define TECHTHETACHYON_LOWLEVEL_ARM_H

#include <SyncWrite.h>
#include <AX12.h>

class Arm {
private:
    DynamixelManager& manager;
    AX12& base;
    AX12& elbow;
    AX12& wrist;
    char* syncAngles = new char[AX12::goalAngle.length*3];
    AX12* XLlist = new AX12[3]{base,elbow,wrist};
    SyncWrite* syncAngleWriteData = new SyncWrite(manager, 3, AX12::goalAngle, 1);
    SyncWrite* syncToggleTorqueWriteData = new SyncWrite(manager, 3, AX12::torqueEnable, 1);
    SyncWrite* syncLedData = new SyncWrite(manager, 3, AX12::LED, 1);
    void prepareAngleData(unsigned int motorIndex, float angle);

public:
    Arm(DynamixelManager& manager, AX12& base, AX12& elbow, AX12& wrist);
    void initTorque();
    void setPosition(float* position);
    AX12* getXLlist();
};


#endif //TECHTHETACHYON_LOWLEVEL_ARM_H
