//
// Created by jglrxavpok on 20/12/18.
//

#ifndef TECHTHETACHYON_LOWLEVEL_SELFCONTAINEDPID_H
#define TECHTHETACHYON_LOWLEVEL_SELFCONTAINEDPID_H
#include <stdint.h>
#include "pid.hpp"

class SelfContainedPID: public PID {

public:
    SelfContainedPID(): PID(&input, &output, &goal), input(0), output(0), goal(0) {}

    bool active;

    void setGoal(int32_t goal);
    int32_t compute(int32_t currentState);
    int32_t getCurrentGoal();
    int32_t getCurrentState();
    int32_t getCurrentOutput();
    /**
     * Reset complet (erreurs et 'goal')
     */
    void fullReset();

private:
    int32_t input;
    int32_t output;
    int32_t goal;
};


#endif //TECHTHETACHYON_LOWLEVEL_SELFCONTAINEDPID_H
