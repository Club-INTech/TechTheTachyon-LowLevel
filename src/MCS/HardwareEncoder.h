//===========================================================================
//  HARDWAREENCODER
//
//  HardwareEncoder library for teensy 3.x using Flexible Timer from µC.
//  Can be used to decode quadrature signal from rotary encoder.
//
//  Author  : SOTON "Asphox" Dylan
//  From    : Intech, robotic club of Telecom SudParis
//  Email   : dylan.soton@telecom-sudparis.eu
//  Date    : Nov 24 2018
//  Version : 1.0
//  Licence : X11/MIT
//==========================================================================
//  Permission is hereby granted, free of charge, to any person obtaining a copy of this software
//  and associated documentation files (the “Software”), to deal in the Software without restriction,
//  including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
//  subject to the following conditions:
//  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//  The Software is provided “as is”, without warranty of any kind, express or implied,
//  including but not limited to the warranties of merchantability, fitness for a particular purpose and noninfringement.
//  In no event shall the authors or copyright holders be liable for any claim, damages or other liability, whether in an action of contract,
//  tort or otherwise, arising from, out of or in connection with the software or the use or other dealings in the Software.
//==========================================================================

#ifndef HARDWAREENCODER_H
#define HARDWAREENCODER_H

#include "mk20dx128.h"
#include "core_pins.h"
#include <Arduino.h>

enum class ENCODERMODE
{
    NONE =          0,
    FILTER_OFF =    0b1,
    PULLUP =        0b10,
    PULLDOWN =      0b100
};


template< uint8_t N >
class HardwareEncoder
{
protected:
    volatile uint32_t& SC         =       *reinterpret_cast<volatile uint32_t*>((N<2) ? &FTM1_SC:&FTM2_SC);
    volatile uint32_t& CNT        =       *reinterpret_cast<volatile uint32_t*>((N<2) ? &FTM1_CNT:&FTM2_CNT);
    volatile uint32_t& MOD        =       *reinterpret_cast<volatile uint32_t*>((N<2) ? &FTM1_MOD:&FTM2_MOD);
    volatile uint32_t& C0SC       =       *reinterpret_cast<volatile uint32_t*>((N<2) ? &FTM1_C0SC:&FTM2_C0SC);
    volatile uint32_t& C0V        =       *reinterpret_cast<volatile uint32_t*>((N<2) ? &FTM1_C0V:&FTM2_C0V);
    volatile uint32_t& C1SC       =       *reinterpret_cast<volatile uint32_t*>((N<2) ? &FTM1_C1SC:&FTM2_C1SC);
    volatile uint32_t& CNTIN      =       *reinterpret_cast<volatile uint32_t*>((N<2) ? &FTM1_CNTIN:&FTM2_CNTIN);
    volatile uint32_t& MODE       =       *reinterpret_cast<volatile uint32_t*>((N<2) ? &FTM1_MODE:&FTM2_MODE);
    volatile uint32_t& COMBINE    =       *reinterpret_cast<volatile uint32_t*>((N<2) ? &FTM1_COMBINE:&FTM2_COMBINE);
    volatile uint32_t& FMS        =       *reinterpret_cast<volatile uint32_t*>((N<2) ? &FTM1_FMS:&FTM2_FMS);
    volatile uint32_t& FILTER     =       *reinterpret_cast<volatile uint32_t*>((N<2) ? &FTM1_FILTER:&FTM2_FILTER);
    volatile uint32_t& QDCTRL     =       *reinterpret_cast<volatile uint32_t*>((N<2) ? &FTM1_QDCTRL:&FTM2_QDCTRL);
    volatile uint32_t& PORT_A     =       *reinterpret_cast<volatile uint32_t*>((N<2) ? &PORTA_PCR12:&PORTB_PCR18);
    volatile uint32_t& PORT_B     =       *reinterpret_cast<volatile uint32_t*>((N<2) ? &PORTA_PCR13:&PORTB_PCR19);
    volatile uint32_t  IRQ_FTM    =        (N<2) ? IRQ_FTM1:IRQ_FTM2;

    volatile int32_t  cnt_H = 0;
    volatile uint32_t blackHole;

    inline void DISABLE_PROTECTION() { FORCE_READ(FMS); MODE |= FTM_MODE_WPDIS; }
    inline void ENABLE_PROTECTION() { FMS |= FTM_FMS_WPEN; }
    inline void FORCE_READ(volatile uint32_t& var){  blackHole=var; }

public:

    HardwareEncoder()
    {
        static_assert( N == 1 || N == 2 , "");
    }

    void setup(ENCODERMODE option = ENCODERMODE::NONE)
    {
        PORT_A = 0;
        PORT_B = 0;

        /* configure pins to enable pullup mode */
        if( (uint8_t)option & (uint8_t)ENCODERMODE::PULLUP )
        {
            PORT_A |= PORT_PCR_PE|PORT_PCR_PS;
            PORT_B |= PORT_PCR_PE|PORT_PCR_PS;
        }

        /* configure pins to enable pulldown mode */
        else if( (uint8_t)option & (uint8_t)ENCODERMODE::PULLDOWN )
        {
            PORT_A |= PORT_PCR_PE;
            PORT_B |= PORT_PCR_PE;
        }

        /* configure pins to enable the filter */
        if( !( (uint8_t)option & (uint8_t)ENCODERMODE::FILTER_OFF) )
        {
            PORT_A |= PORT_PCR_PFE;
            PORT_B |= PORT_PCR_PFE;
        }

        /* set muxing slots for pins, depending on FTM selected */
        if ( N==1 )
        {
            PORT_A |= PORT_PCR_MUX(7);
            PORT_B |= PORT_PCR_MUX(7);
        }
        else
        {
            PORT_A |= PORT_PCR_MUX(6);
            PORT_B |= PORT_PCR_MUX(6);
        }

        /* Disables write protection on registers */
        DISABLE_PROTECTION();

        /* Activates FTM mode and lets write protected disabled */
        MODE = FTM_MODE_WPDIS|FTM_MODE_FTMEN;

        /* Resets all status flags for FTM counters */
        SC = 0;

        /* Configures input filter */
        FILTER=0x22;

        /* Lowest value before hardware underflow on register CNT */
        CNTIN=0;

        /* Highest value before hardware overflow on register CNT */
        MOD=0xFFFF;

        /* Resets CNT to CNTIN value */
        CNT=0;

        /* Resets all configurations for linked channels to be sure */
        COMBINE=0;

        C0SC=0x10;
        /* Sets value for software  "over/under-flow interrupt" check*/
        C0V=0x0000;

        /*Enables inputs filters on pinA and pinB, activates quadrature decoder mode*/
        QDCTRL=FTM_QDCTRL_PHAFLTREN|FTM_QDCTRL_PHBFLTREN|FTM_QDCTRL_QUADEN;

        /* Enables write protections on registers */
        ENABLE_PROTECTION();
    }

    void start()
    {
        reset();

        /* Enables over/under-flow interruptions for FTM */
        NVIC_ENABLE_IRQ(IRQ_FTM);
    }

    void stop()
    {
        DISABLE_PROTECTION();
        noInterrupts();
        QDCTRL=FTM_QDCTRL_PHAFLTREN|FTM_QDCTRL_PHBFLTREN;
        interrupts();
        NVIC_DISABLE_IRQ(IRQ_FTM);
        ENABLE_PROTECTION();
    }

    void ofi()
    {
        /* If hardware over/under-flow is detected */
        if( SC == (FTM_SC_TOIE|FTM_SC_TOF) )
        {
            /* Force reads SC to enable modification*/
            FORCE_READ(SC);

            /* Checks if it was an overflow or underflow, then increment or decrement the high register */
            if( QDCTRL & FTM_QDCTRL_TOFDIR )
                cnt_H++;
            else
                cnt_H--;
        }
        /* If software over/under-flow is detected (to prevent ghost ticks) */
        else
        {
            /* If down ghost tick detected, reset CNT */
            if( CNT == 0xFFFF )CNT = 0;

            /* Resets software check value to 0 */
            C0V= 0x0000;
        }

        /* Enables channel interrupts again */
        FORCE_READ(C0SC);
        C0SC=0x50;

        /* Clears over/under-flow flag */
        SC=FTM_SC_TOIE;
    }

    void reset()
    {
        /* Disables write protection on registers*/
        DISABLE_PROTECTION();

        /* Desactivates interruptions to avoid changes of flags on QDCTRL */
        noInterrupts();
        /* Desactivates quadrature decoder mode */
        QDCTRL=FTM_QDCTRL_PHAFLTREN|FTM_QDCTRL_PHBFLTREN;

        /* NoInterrupts no more needed because quadrature mode is disabled */
        interrupts();

        /* Forces read SC register to enable reset, then reset */
        FORCE_READ(SC);
        SC=0;

        /* Forces read C0SC register to enable resert, then reset */
        FORCE_READ(C0SC);
        C0SC=0x10;

        /* Sets value for software  "over/under-flow interrupt" on 0xFFFF to avoid ghost tick on startup */
        C0V = 0xFFFF;

        /* Resets count register */
        CNT=0;
        cnt_H=0;

        /* Enables quadrature mode */
        QDCTRL=FTM_QDCTRL_PHAFLTREN|FTM_QDCTRL_PHBFLTREN|FTM_QDCTRL_QUADEN;;

        /* Activates channel interrupts */
        C0SC=0x50;

        /* Enables write protection on registers */
        ENABLE_PROTECTION();
    }

    inline int32_t count ()
    {
        return cnt_H*0x10000+CNT;
    }
};

/* Encoder1 "arduino" pins : A:3	B:4 */
static HardwareEncoder<1> Encoder1;

/* Encoder2 "arduino" pins : A:32   B:25 */
static HardwareEncoder<2> Encoder2;

/* Wrapper for over/under-flow interrupt on register CNT for FTM1 */
extern void ftm1_isr();

/* Wrapper for over/under-flow interrupt on register CNT for FTM2*/
extern void ftm2_isr();

#endif //HARDWAREENCODER_H
