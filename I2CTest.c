#include "I2C.h"
#include <stdio.h>

int main(void) {
    printf("Testing I2C communication...\n");

    if (i2cInit(0x60) < 0) {
        printf("Failed to initialize I2C\n");
        return 1;
    }

    printf("I2C initialized successfully at address 0x60\n");

    printf("Writing 0x04 to register 0x01...\n");
    write8(0x01, 0x04);

    printf("Reading from register 0x01...\n");
    uint8_t value = readU8(0x01);
    printf("Read value: 0x%02X\n", value);

    i2cClose();
    printf("Test complete!\n");

    return 0;
}
