#include "MotorHat.h"

static Motor motors[4];

void initHat(void) {
    PWMInit(0x60);
    setPWMFreq(1600);

    motors[0].pwm = 8;
    motors[0].in2 = 9;
    motors[0].in1 = 10;

    motors[1].pwm = 13;
    motors[1].in2 = 12;
    motors[1].in1 = 11;

    motors[2].pwm = 2;
    motors[2].in2 = 3;
    motors[2].in1 = 4;

    motors[3].pwm = 7;
    motors[3].in2 = 6;
    motors[3].in1 = 5;
}

void setPin(int pin, int value) {
    if (value == 0) {
        setPWM(pin, 0, 4096);
    } else if (value == 1) {
        setPWM(pin, 4096, 0);
    }
}

void setSpeed(int motorID, int speed) {
    if (motorID < 0 || motorID > 3) return;
    if (speed < 0) speed = 0;
    if (speed > 255) speed = 255;

    setPWM(motors[motorID].pwm, 0, speed * 16);
}

void run(int motorID, int command) {
    if (motorID < 0 || motorID > 3) return;

    Motor *m = &motors[motorID];

    switch(command) {
        case FORWARD:
            setPin(m->in2, 0);
            setPin(m->in1, 1);
            break;
        case BACKWARD:
            setPin(m->in1, 0);
            setPin(m->in2, 1);
            break;
        case RELEASE:
            setPin(m->in1, 0);
            setPin(m->in2, 0);
            break;
    }
}
