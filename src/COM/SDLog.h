//
// Created by Teo-CD on 22/04/18.
//

#ifndef LL_SDLOG_H
#define LL_SDLOG_H

#include "SD.h"
#include "Utils/defines.h"

class SDLog {

public:
    SDLog();

    bool logWrite(String);
    bool logWrite(const char*);
    bool logWriteReception(const char*);

    void setStartingTime();

    explicit operator bool();

private:
    const char* headerList[4]{STD_HEADER,DEBUG_HEADER,SENSOR_HEADER,POSITION_HEADER};
    const char* fileList[6]{"/full.txt","/input.txt","/standard.txt","/debug.txt","/sensor.txt","/position.txt"};

    uint32_t startingTime;
    bool sdStatus;

    bool logToFile(const String&,const char*);
};


#endif //LL_SDLOG_H
