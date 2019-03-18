//
// Created by asphox on 05/02/19.
//

#ifndef TECHTHETACHYON_LOWLEVEL_INTERRUPTSTACKPRINT_H
#define TECHTHETACHYON_LOWLEVEL_INTERRUPTSTACKPRINT_H

#include <cstdint>
#include <Arduino.h>
#include "../Utils/Singleton.hpp"
#include "ComMgr.h"

class InterruptStackPrint : public Singleton<InterruptStackPrint>
{
private:
    static constexpr uint8_t MAX_STACK_SIZE = 100;
    const char* stack[MAX_STACK_SIZE];
    uint8_t current_size = 0;
public:

    void push(const char* str)
    {
        if( current_size == MAX_STACK_SIZE )
            return;
        stack[current_size] = str;
        current_size++;
    }

    void print()
    {
        for( uint8_t i=0 ; i < current_size ; i++)
        {
            Serial.println(stack[i]);
        }
        current_size=0;
    }

};


#endif //TECHTHETACHYON_LOWLEVEL_INTERRUPTSTACKPRINT_H
