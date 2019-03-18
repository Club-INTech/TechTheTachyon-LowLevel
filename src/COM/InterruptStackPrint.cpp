//
// Created by asphox on 05/02/19.
//

#include "InterruptStackPrint.h"

void InterruptStackPrint::push(const String& str)
{
    if( current_size == INTERRUPT_PRINT_STACK_MAX_SIZE )
        return;
    stack[current_size] = str;
    current_size++;
}

void InterruptStackPrint::print()
{
        for( uint8_t i=0 ; i < current_size ; i++)
        {
            ComMgr::Instance().printf(DEBUG_HEADER,stack[i].c_str());
        }
        current_size=0;
}