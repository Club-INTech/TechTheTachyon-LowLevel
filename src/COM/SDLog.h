//
// Created by Teo-CD on 22/04/18.
//

#ifndef LL_SDLOG_H
#define LL_SDLOG_H

#include "SD.h"
#include "Config/defines.h"

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
    static constexpr uint8_t headerCount = 6;
    static constexpr uint8_t fileCount = 8;
    const char* headerList[headerCount]{STD_HEADER, DEBUG_HEADER, SENSOR_HEADER, POSITION_HEADER, EVENT_HEADER, SICK_HEADER};

    const char* fileList[fileCount]{"/FULL.TXT", "/INPUT.TXT", "/STD.TXT","/DEBUG.TXT","/SENSOR.TXT","/SENSOR.TXT", "/EVENT.TXT", "/SICK.TXT"};

    File fileStreams[fileCount];

    uint32_t startingTime;
    bool sdStatus = false;

    bool logToFile(const String&,uint8_t);
};


#endif //LL_SDLOG_H
