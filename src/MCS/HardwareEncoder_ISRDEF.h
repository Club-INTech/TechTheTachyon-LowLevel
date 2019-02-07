//
// Created by asphox on 07/02/19.
//

#ifndef TECHTHETACHYON_LOWLEVEL_HARDWAREENCODER_DEF_H
#define TECHTHETACHYON_LOWLEVEL_HARDWAREENCODER_DEF_H

#include "HardwareEncoder.h"

extern "C" void ftm1_isr()
{
    __PTR__encoder1->ftm_isr();
}

extern "C" void ftm2_isr()
{
    __PTR__encoder2->ftm_isr();
}

#endif //TECHTHETACHYON_LOWLEVEL_HARDWAREENCODER_DEF_H
