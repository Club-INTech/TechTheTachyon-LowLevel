//
// Created by jglrxavpok on 20/12/18.
//

#ifndef TECHTHETACHYON_LOWLEVEL_SELFCONTAINEDPID_H
#define TECHTHETACHYON_LOWLEVEL_SELFCONTAINEDPID_H
#include <stdint.h>
#include "PID.hpp"

template <typename T>
class SelfContainedPID: public PID<T> {

private:
    T input;
    T output;
    T goal;

public:
    SelfContainedPID(): PID<T>(&input, &output, &goal), input(0), output(0), goal(0), active(false) {}

    bool active;

    void setGoal(T goal) {
        this->goal = goal;
    }

    T compute(T currentState) {
        this->input = currentState;
        PID<T>::compute(); // appel de la méthode de pid.h
        return this->output;
    }

    T getCurrentGoal() {
        return goal;
    }

    T getCurrentOutput() {
        return output;
    }

    T getCurrentState() {
        return input;
    }

    void fullReset() {
        PID<T>::resetErrors();
        // FIXME setGoal(0);
    }

    void resetOutput(T newValue) {
        output = newValue;
    }
};


#endif //TECHTHETACHYON_LOWLEVEL_SELFCONTAINEDPID_H
