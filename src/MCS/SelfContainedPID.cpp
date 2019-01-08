//
// Created by jglrxavpok on 20/12/18.
//
#include "SelfContainedPID.hpp"

void SelfContainedPID::setGoal(int32_t goal) {
    this->goal = goal;
}

int32_t SelfContainedPID::compute(int32_t currentState) {
    this->input = currentState;
    PID::compute(); // appel de la méthode de pid.h
    return this->output;
}

int32_t SelfContainedPID::getCurrentGoal() {
    return goal;
}

int32_t SelfContainedPID::getCurrentOutput() {
    return output;
}

int32_t SelfContainedPID::getCurrentState() {
    return input;
}

void SelfContainedPID::fullReset() {
    resetErrors();
    setGoal(0);
}