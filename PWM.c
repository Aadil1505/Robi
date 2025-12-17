#include "PWM.h"
#include <unistd.h>
#include <math.h>

void PWMInit(int address) {
    if (i2cInit(address) < 0) {
        return;
    }

    setAllPWM(0, 0);
    write8(MODE2, OUTDRV);
    write8(MODE1, ALLCALL);
    usleep(5000);

    uint8_t mode1 = readU8(MODE1);
    mode1 = mode1 & ~SLEEP;
    write8(MODE1, mode1);
    usleep(5000);
}

void setPWMFreq(int freq) {
    float prescaleval = 25000000.0;
    prescaleval /= 4096.0;
    prescaleval /= (float)freq;
    prescaleval -= 1.0;

    uint8_t prescale = (uint8_t)floor(prescaleval + 0.5);

    uint8_t oldmode = readU8(MODE1);
    uint8_t newmode = (oldmode & 0x7F) | SLEEP;
    write8(MODE1, newmode);
    write8(PRESCALE, prescale);
    write8(MODE1, oldmode);
    usleep(5000);
    write8(MODE1, oldmode | RESTART);
}

void setPWM(int channel, int on, int off) {
    write8(LED0_ON_L + 4*channel, on & 0xFF);
    write8(LED0_ON_H + 4*channel, on >> 8);
    write8(LED0_OFF_L + 4*channel, off & 0xFF);
    write8(LED0_OFF_H + 4*channel, off >> 8);
}

void setAllPWM(int on, int off) {
    write8(ALL_LED_ON_L, on & 0xFF);
    write8(ALL_LED_ON_H, on >> 8);
    write8(ALL_LED_OFF_L, off & 0xFF);
    write8(ALL_LED_OFF_H, off >> 8);
}
