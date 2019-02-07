//===========================================================================
//  HARDWAREENCODER
//
//  HardwareEncoder library for teensy 3.x using Flexible Timer from µC.
//  Can be used to decode quadrature signal from rotary encoder.
//
//  Author  : SOTON "Asphox" Dylan
//  From    : Intech, robotic club of Telecom SudParis
//  Email   : dylan.soton@telecom-sudparis.eu
//  Date    : Feb 7 2019
//  Version : 1.5
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
    PULLDOWN =      0b100,
    INVERSED =      0b1000
};

static ENCODERMODE operator|(const ENCODERMODE& e1 , const ENCODERMODE& e2)
{
    return (ENCODERMODE)((int)e1|(int)e2);
}

template< uint8_t N >
class HardwareEncoder;

class AbsHardwareEncoder
{
public:
    virtual void ftm_isr() = 0;
};

static AbsHardwareEncoder* __PTR__encoder1 = nullptr;
static AbsHardwareEncoder* __PTR__encoder2 = nullptr;

template< uint8_t N >
class HardwareEncoder : public AbsHardwareEncoder
{
private:
    constexpr volatile uint32_t& SC()     { return (N<2) ? FTM1_SC : FTM2_SC; }
    constexpr volatile uint32_t& CNT()    { return (N<2) ? FTM1_CNT : FTM2_CNT; }
    constexpr volatile uint32_t& MOD()    { return (N<2) ? FTM1_MOD : FTM2_MOD; }
    constexpr volatile uint32_t& C0SC()   { return (N<2) ? FTM1_C0SC : FTM2_C0SC; }
    constexpr volatile uint32_t& C0V()    { return (N<2) ? FTM1_C0V : FTM2_C0V; }
    constexpr volatile uint32_t& C1SC()   { return (N<2) ? FTM1_C1SC : FTM2_C1SC; }
    constexpr volatile uint32_t& CNTIN()  { return (N<2) ? FTM1_CNTIN : FTM2_CNTIN; }
    constexpr volatile uint32_t& MODE()   { return (N<2) ? FTM1_MODE : FTM2_MODE; }
    constexpr volatile uint32_t& COMBINE(){ return (N<2) ? FTM1_COMBINE : FTM2_COMBINE; }
    constexpr volatile uint32_t& FMS()    { return (N<2) ? FTM1_FMS : FTM2_FMS; }
    constexpr volatile uint32_t& FILTER() { return (N<2) ? FTM1_FILTER : FTM2_FILTER; }
    constexpr volatile uint32_t& QDCTRL() { return (N<2) ? FTM1_QDCTRL : FTM2_QDCTRL; }
    constexpr volatile uint32_t& PORT_A()  { return (N<2) ? PORTA_PCR12 : PORTB_PCR18; }
    constexpr volatile uint32_t& PORT_B()  { return (N<2) ? PORTA_PCR13 : PORTB_PCR19; }
    constexpr volatile uint32_t  IRQ_FTM(){ return (N<2) ? IRQ_FTM1 : IRQ_FTM2; }

    volatile uint32_t blackHole;
    volatile int32_t cnt_H;
    bool             m_inverseRotation = false;

    inline void DISABLE_PROTECTION() { FORCE_READ(FMS()); MODE() |= FTM_MODE_WPDIS; }
    inline void ENABLE_PROTECTION() { FMS() |= FTM_FMS_WPEN; }
    inline void FORCE_READ(volatile uint32_t& var){  blackHole=var; }

public:

    HardwareEncoder()
    {
        static_assert( N == 1 || N == 2 , "");
        if(N == 1)
        {
            __PTR__encoder1 = this;
        }
        else
        {
            __PTR__encoder2 = this;
        }
    }

    void setup(ENCODERMODE option = ENCODERMODE::NONE)
    {
        PORT_A() = 0;
        PORT_B() = 0;

        m_inverseRotation = (int)option & (int)ENCODERMODE::INVERSED;

        /* configure pins to enable pullup mode */
        if( (int)option & (int)ENCODERMODE::PULLUP )
        {
            PORT_A() |= PORT_PCR_PE|PORT_PCR_PS;
            PORT_B() |= PORT_PCR_PE|PORT_PCR_PS;
        }

            /* configure pins to enable pulldown mode */
        else if( (int)option & (int)ENCODERMODE::PULLDOWN )
        {
            PORT_A() |= PORT_PCR_PE;
            PORT_B() |= PORT_PCR_PE;
        }

        /* configure pins to enable the filter */
        if( !((int)option & (int)ENCODERMODE::FILTER_OFF) )
        {
            PORT_A() |= PORT_PCR_PFE;
            PORT_B() |= PORT_PCR_PFE;
        }

        /* set muxing slots for pins, depending on FTM selected */
        if ( N==1 )
        {
            PORT_A() |= PORT_PCR_MUX(7);
            PORT_B() |= PORT_PCR_MUX(7);
        }
        else
        {
            PORT_A() |= PORT_PCR_MUX(6);
            PORT_B() |= PORT_PCR_MUX(6);
        }

        /* Disables write protection on registers */
        DISABLE_PROTECTION();

        /* Activates FTM mode and lets write protected disabled */
        MODE() = FTM_MODE_WPDIS|FTM_MODE_FTMEN;

        /* Resets all status flags for FTM counters */
        SC() = 0;

        /* Configures input filter */
        FILTER() = 0x22;

        /* Lowest value before hardware underflow on register CNT */
        CNTIN() = 0;

        /* Highest value before hardware overflow on register CNT */
        MOD() = 0xFFFF;

        /* Resets CNT to CNTIN value */
        CNT() = 0;

        /* Resets all configurations for linked channels to be sure */
        COMBINE() = 0;

        C0SC() = 0x10;
        C1SC() = 0;
        /* Sets value for software  "over/under-flow interrupt" check*/
        C0V() = 0x0000;

        /*Enables inputs filters on pinA and pinB, activates quadrature decoder mode*/
        QDCTRL() = FTM_QDCTRL_PHAFLTREN|FTM_QDCTRL_PHBFLTREN|FTM_QDCTRL_QUADEN;

        /* Enables write protections on registers */
        ENABLE_PROTECTION();

    }

    void start()
    {
        reset();

        /* Enables over/under-flow interruptions for FTM */
        NVIC_ENABLE_IRQ(IRQ_FTM());
    }

    void stop()
    {
        DISABLE_PROTECTION();
        noInterrupts();
        QDCTRL() = FTM_QDCTRL_PHAFLTREN|FTM_QDCTRL_PHBFLTREN;
        interrupts();
        NVIC_DISABLE_IRQ(IRQ_FTM());
        ENABLE_PROTECTION();
    }

    void ftm_isr()
    {
        /* If hardware over/under-flow is detected */
        if( SC() == (FTM_SC_TOIE|FTM_SC_TOF) )
        {
            /* Force reads SC to enable modification*/
            FORCE_READ(SC());
            /* Checks if it was an overflow or underflow, then increment or decrement the high register */
            if( QDCTRL() & FTM_QDCTRL_TOFDIR )
                cnt_H++;
            else
                cnt_H--;

        }
        /* If software over/under-flow is detected (to prevent ghost ticks) */
        else
        {
            /* If down ghost tick detected, reset CNT */
            if( CNT() == 0xFFFF ) CNT() = 0;

            /* Resets software check value to 0 */
            C0V() = 0x0000;
        }

        /* Enables channel interrupts again */
        FORCE_READ(C0SC());
        C0SC() = 0x50;

        /* wait for clear on over/under-flow flag */
        while( SC() != FTM_SC_TOIE )
        {
            FORCE_READ(SC());
            SC() = FTM_SC_TOIE;
        }
    }

    void reset()
    {
        /* Disables write protection on registers*/
        DISABLE_PROTECTION();

        /* Desactivates interruptions to avoid changes of flags on QDCTRL */
        noInterrupts();
        /* Desactivates quadrature decoder mode */
        QDCTRL() = FTM_QDCTRL_PHAFLTREN|FTM_QDCTRL_PHBFLTREN;

        /* NoInterrupts no more needed because quadrature mode is disabled */
        interrupts();

        /* Forces read SC register to enable reset, then reset */
        FORCE_READ(SC());
        SC() = 0;

        /* Forces read C0SC register to enable resert, then reset */
        FORCE_READ(C0SC());
        C0SC() = 0x10;

        /* Sets value for software  "over/under-flow interrupt" on 0xFFFF to avoid ghost tick on startup */
        C0V() = 0xFFFF;

        /* Resets count register */
        CNT() =0;
        cnt_H = 0;

        /* Enables quadrature mode */
        QDCTRL() = FTM_QDCTRL_PHAFLTREN|FTM_QDCTRL_PHBFLTREN|FTM_QDCTRL_QUADEN;;

        /* Activates channel interrupts */
        C0SC() = 0x50;

        /* Enables write protection on registers */
        ENABLE_PROTECTION();
    }

    int32_t count ()
    {
        noInterrupts();
        int32_t cnt_L = CNT();
        interrupts();
        if(m_inverseRotation)
            return -(cnt_H<<16)-cnt_L;
        else
            return (cnt_H<<16)+cnt_L;
    }
};

/* Encoder1 teensy35 pins : A:3	B:4 */
static HardwareEncoder<1> Encoder1;

/* Encoder2 teensy32 pins : A:25   B:32 */
/* Encoder2 teensy35 pins : A:29   B:30 */
static HardwareEncoder<2> Encoder2;

#endif //HARDWAREENCODER_H
