//
// Created by serandour on 17/11/2019.
//

#ifndef TECHTHETACHYON_LOWLEVEL_METRO_H
#define TECHTHETACHYON_LOWLEVEL_METRO_H


class Metro
{

public:
    Metro();
    Metro(unsigned long interval_millis);
    void interval(unsigned long interval_millis);
    bool check();
    bool checkCatchUp();
    bool expired() const;
    bool running() const;
    void reset();
    long time_remaining() const;

public:
    unsigned long  previous_millis, interval_millis;

};

#endif //TECHTHETACHYON_LOWLEVEL_METRO_H
