#ifndef MOTORHAT_H
#define MOTORHAT_H

#include <stdint.h>
#include "PWM.h"

typedef struct {
    int pwm;
    int in1;
    int in2;
} Motor;

#define FORWARD     1
#define BACKWARD    2
#define BRAKE       3
#define RELEASE     4

void initHat(void);
void setSpeed(int motorID, int speed);
void run(int motorID, int command);
void setPin(int pin, int value);

#endif
