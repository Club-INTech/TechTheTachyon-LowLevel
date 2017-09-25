#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

#define MIN(x,y) (((x)<(y))?(x):(y))
#define MAX(x,y) (((x)>(y))?(x):(y))
#define ABS(x) (((x) > 0) ? (x) : -(x))

#ifdef __cplusplus

extern "C" {
#endif

int modulo(int nombre, int modulo);

int parseInt(char*);

double parseFloat(char*);

#ifdef __cplusplus
}
#endif

#endif
