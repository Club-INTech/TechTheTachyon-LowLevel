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
    static constexpr uint8_t fileCount = 6;
    const char* headerList[fileCount]{STD_HEADER, DEBUG_HEADER, SENSOR_HEADER, POSITION_HEADER, EVENT_HEADER, SICK_HEADER};

    const char* fileList[fileCount]{"/STD.TXT","/DEBUG.TXT","/SENSOR.TXT","/SENSOR.TXT", "/EVENT.TXT", "/SICK.TXT"};

    File fileStreams[fileCount];

    uint32_t startingTime;
    bool sdStatus;

    bool logToFile(const String&,uint8_t);
};


#endif //LL_SDLOG_H
