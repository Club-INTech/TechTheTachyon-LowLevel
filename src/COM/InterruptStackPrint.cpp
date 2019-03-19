//
// Created by asphox on 05/02/19.
//

#include "InterruptStackPrint.h"

void InterruptStackPrint::push(const String& str)
{
    push(DEBUG_HEADER, str);
}

void InterruptStackPrint::push(const Header header, const String& str)
{
    if( current_size == INTERRUPT_PRINT_STACK_MAX_SIZE )
        return;
    stack[current_size].header[0] = header[0];
    stack[current_size].header[1] = header[1];
    stack[current_size].str = &str;
    current_size++;
}

void InterruptStackPrint::print()
{
        for( uint8_t i=0 ; i < current_size ; i++)
        {
            ComMgr::Instance().printfln(stack[i].header, "%s", stack[i].str->c_str());
        }
        current_size=0;
}