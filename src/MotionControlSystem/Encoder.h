//
// Created by serandour on 03/10/2019.
//

#ifndef TECHTHETACHYON_LOWLEVEL_ENCODER_H
#define TECHTHETACHYON_LOWLEVEL_ENCODER_H
#include <Arduino.h>

class Encoder {
public:
    Encoder(int pin1, int pin2);
    int32_t read();
    void write(int val_reset);

};


#endif //TECHTHETACHYON_LOWLEVEL_ENCODER_H
