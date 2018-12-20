//
// Created by jglrxavpok on 20/12/18.
//

#include "HardwareEncoder.h"

/* Wrapper for over/under-flow interrupt on register CNT for FTM1 */
void ftm1_isr()
{
    Encoder1.ofi();
}

/* Wrapper for over/under-flow interrupt on register CNT for FTM2*/
void ftm2_isr()
{
    Encoder2.ofi();
}