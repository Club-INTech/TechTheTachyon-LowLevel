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
    headerStack[writePointer][0] = header[0];
    headerStack[writePointer][1] = header[1];
    stack[writePointer] = str;
    writePointer++;
    writePointer %= INTERRUPT_PRINT_STACK_MAX_SIZE;
}

void InterruptStackPrint::print()
{
    if( com_options & ETHERNET_RW )
    {
        /*if( ComMgr::Instance().connectedEthernet() )
        {
            doPrint();
        }*/
    }
    else
    {
        doPrint();
    }
}

inline void InterruptStackPrint::doPrint() {
    while(readPointer != writePointer) {
        ComMgr::Instance().printfln(headerStack[readPointer], "%s", stack[readPointer].c_str());
        ComMgr::Instance().printfln(DEBUG_HEADER, "StackPrint n°%i/%i", readPointer, writePointer);

        readPointer++;
        readPointer %= INTERRUPT_PRINT_STACK_MAX_SIZE;
    }
}
