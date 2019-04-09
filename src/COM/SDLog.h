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
    ~SDLog();

    bool logWrite(String);
    bool logWrite(const char*);
    bool logWriteReception(const char*);

    void setStartingTime();

    explicit operator bool();

private:
    const char* headerList[4]{STD_HEADER,DEBUG_HEADER,SENSOR_HEADER,POSITION_HEADER};
    const char* fileList[6]{"/FULL.TXT","/INPUT.TXT","/STANDARD.TXT","/DEBUG.TXT","/SENSOR.TXT","/POSITION.TXT"};

    static const uint8_t fileCount = sizeof(fileList)/sizeof(char*);

    File fileStreams[fileCount];

    uint32_t startingTime;
    bool sdStatus;

    bool logToFile(const String&,uint8_t);
};


#endif //LL_SDLOG_H
