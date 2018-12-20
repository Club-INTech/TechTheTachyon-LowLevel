//
// Created by jglrxavpok on 20/12/18.
//

#include "MCSReborn.h"
#include "Motor.h"

MCS::MCS(): leftMotor(Side::LEFT), rightMotor(Side::RIGHT),
            leftSpeedPID(nullptr, nullptr, nullptr), rightSpeedPID(nullptr, nullptr, nullptr),
            translationPID(nullptr, nullptr, nullptr), rotationPID(nullptr, nullptr, nullptr) {
    // TODO: pas utiliser nullptr!!!
}