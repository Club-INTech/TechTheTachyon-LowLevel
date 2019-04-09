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
    headerStack[current_size][0] = header[0];
    headerStack[current_size][1] = header[1];
    stack[current_size] = str;
    current_size++;
}

void InterruptStackPrint::print()
{
    if( com_options & ETHERNET_RW )
    {
        if( ComMgr::Instance().connectedEthernet() )
        {
            for( uint8_t i=0 ; i < current_size ; i++)
            {
                ComMgr::Instance().printfln(headerStack[i], "%s", stack[i].c_str());
            }
            current_size=0;
        }
    }
    else
    {
            for( uint8_t i=0 ; i < current_size ; i++)
            {
                ComMgr::Instance().printfln(headerStack[i], "%s", stack[i].c_str());
            }
            current_size=0;
    }
}