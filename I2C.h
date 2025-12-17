#ifndef I2C_H
#define I2C_H

#include <stdio.h>
#include <stdint.h>

int i2cInit(int address);
void write8(uint8_t reg, uint8_t value);
uint8_t readU8(uint8_t reg);
void i2cClose(void);

#endif
