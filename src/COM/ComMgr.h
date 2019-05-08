//
// Created by asphox on 24/04/18.
//

#ifndef TECHTHETOWN_LOWLEVEL_COMMGR_H
#define TECHTHETOWN_LOWLEVEL_COMMGR_H

#include "Interfaces/SerialInterface.h"
#include "Interfaces/EthernetInterface.h"
#include "../Utils/Singleton.hpp"
#include "../Utils/define_com_options.h"
#include "../Utils/Median.h"
#include <map>
#include <array>

class ComMgr : public Singleton<ComMgr>
{
public:

    ComMgr();
    ~ComMgr();

    template< typename T >
    bool read(T data){
        static bool r1=false,r2=false;
        if( com_options & COM_OPTIONS::ETHERNET_R )
            r1 = ethernet->read(data);
        if( com_options & COM_OPTIONS::SERIAL_R && !r1 )
            r2 = serial->read(data);
        return r1||r2;
    }

    /* ENVOI */
    void sendPosition(const float*);

    void printfln(Header header,const char*, ...) __attribute__((format(printf, 3, 4)));
    void printf(Header header,const char*,...) __attribute__((format(printf, 3, 4)));
    void printOnSerial(const char*);

    void startMatch();

    void resetEth();

    bool connectedEthernet();

private:
    AbstractComInterface*    ethernet = nullptr;
    AbstractComInterface*      serial = nullptr;
    SDLog* sdlog = nullptr;

};


#endif //TECHTHETOWN_LOWLEVEL_COMMGR_H
