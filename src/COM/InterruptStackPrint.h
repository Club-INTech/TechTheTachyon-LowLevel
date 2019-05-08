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
    char headerStack[INTERRUPT_PRINT_STACK_MAX_SIZE][HEADER_LENGTH];
    uint8_t writePointer = 0;
    uint8_t readPointer = 0;
public:

    InterruptStackPrint() = default;

    void push(const String& str);
    void push(Header header, const String& str);

    void print();

    ~InterruptStackPrint() = default;

    void doPrint();
};


#endif //TECHTHETACHYON_LOWLEVEL_INTERRUPTSTACKPRINT_H
