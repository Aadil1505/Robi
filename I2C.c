#include "I2C.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <string.h>
#include <errno.h>

static int i2c_fd = -1;
static pthread_mutex_t i2c_mutex = PTHREAD_MUTEX_INITIALIZER;

int i2cInit(int address) {
    i2c_fd = open("/dev/i2c-1", O_RDWR);
    if (i2c_fd < 0) {
        perror("Failed to open I2C bus");
        return -1;
    }

    if (ioctl(i2c_fd, I2C_SLAVE, address) < 0) {
        perror("Failed to set I2C slave address");
        close(i2c_fd);
        return -1;
    }

    return 0;
}

void write8(uint8_t reg, uint8_t value) {
    uint8_t buffer[2] = {reg, value};

    pthread_mutex_lock(&i2c_mutex);
    if (write(i2c_fd, buffer, 2) != 2) {
        perror("I2C write failed");
    }
    pthread_mutex_unlock(&i2c_mutex);
}

uint8_t readU8(uint8_t reg) {
    uint8_t value = 0;

    pthread_mutex_lock(&i2c_mutex);
    if (write(i2c_fd, &reg, 1) != 1) {
        perror("I2C write register failed");
        pthread_mutex_unlock(&i2c_mutex);
        return 0;
    }

    if (read(i2c_fd, &value, 1) != 1) {
        perror("I2C read failed");
        pthread_mutex_unlock(&i2c_mutex);
        return 0;
    }
    pthread_mutex_unlock(&i2c_mutex);

    return value;
}

void i2cLock(void) {
    pthread_mutex_lock(&i2c_mutex);
}

void i2cUnlock(void) {
    pthread_mutex_unlock(&i2c_mutex);
}

void i2cClose(void) {
    pthread_mutex_lock(&i2c_mutex);
    if (i2c_fd >= 0) {
        close(i2c_fd);
        i2c_fd = -1;
    }
    pthread_mutex_unlock(&i2c_mutex);
}

