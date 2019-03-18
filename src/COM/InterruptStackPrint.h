//
// Created by asphox on 05/02/19.
//

#ifndef TECHTHETACHYON_LOWLEVEL_INTERRUPTSTACKPRINT_H
#define TECHTHETACHYON_LOWLEVEL_INTERRUPTSTACKPRINT_H

#include <cstdint>
#include <Arduino.h>
#include "../Utils/defines.h"
#include "../Utils/Singleton.hpp"
#include "ComMgr.h"

class InterruptStackPrint : public Singleton<InterruptStackPrint>
{
private:
    String stack[INTERRUPT_PRINT_STACK_MAX_SIZE];
    uint8_t current_size = 0;
public:

    InterruptStackPrint() = default;
    
    void push(const String& str);

    void print();

    ~InterruptStackPrint() = default;
};


#endif //TECHTHETACHYON_LOWLEVEL_INTERRUPTSTACKPRINT_H
