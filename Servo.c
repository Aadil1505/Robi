#include "Servo.h"
#include "Sensor.h"
#include "PWM.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

#define LED0_ON_L 0x06

static int servo_fd = -1;

static void servoWrite8(uint8_t reg, uint8_t value) {
    uint8_t buffer[2] = {reg, value};
    if (write(servo_fd, buffer, 2) != 2) {
        perror("Servo I2C write failed");
    }
}

static uint8_t servoReadU8(uint8_t reg) {
    uint8_t value = 0;
    if (write(servo_fd, &reg, 1) != 1) {
        perror("Servo I2C write register failed");
        return 0;
    }
    if (read(servo_fd, &value, 1) != 1) {
        perror("Servo I2C read failed");
        return 0;
    }
    return value;
}

int initServo(void) {
    servo_fd = open("/dev/i2c-1", O_RDWR);
    if (servo_fd < 0) {
        perror("Failed to open I2C bus for servo");
        return -1;
    }

    if (ioctl(servo_fd, I2C_SLAVE, 0x40) < 0) {
        perror("Failed to set I2C address for servo");
        close(servo_fd);
        return -1;
    }

    servoWrite8(0xFA, 0);
    servoWrite8(0xFB, 0);
    servoWrite8(0xFC, 0);
    servoWrite8(0xFD, 0);
    servoWrite8(MODE2, OUTDRV);
    servoWrite8(MODE1, ALLCALL);
    usleep(5000);

    uint8_t mode1 = servoReadU8(MODE1);
    mode1 = mode1 & ~SLEEP;
    servoWrite8(MODE1, mode1);
    usleep(5000);

    float prescaleval = 25000000.0 / 4096.0 / 60.0 - 1.0;
    uint8_t prescale = (uint8_t)(prescaleval + 0.5);
    uint8_t oldmode = servoReadU8(MODE1);
    uint8_t newmode = (oldmode & 0x7F) | SLEEP;
    servoWrite8(MODE1, newmode);
    servoWrite8(PRESCALE, prescale);
    servoWrite8(MODE1, oldmode);
    usleep(5000);
    servoWrite8(MODE1, oldmode | RESTART);

    setServoAngle(CENTER);
    usleep(500000);

    printf("Servo initialized at 0x40\n");
    return 0;
}

void setServoAngle(int angle) {
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;

    int pulse = SERVO_MIN + ((SERVO_MAX - SERVO_MIN) * angle) / 180;

    uint8_t reg_base = LED0_ON_L + 4 * SERVO_CHANNEL;
    servoWrite8(reg_base, 0);
    servoWrite8(reg_base + 1, 0);
    servoWrite8(reg_base + 2, pulse & 0xFF);
    servoWrite8(reg_base + 3, pulse >> 8);

    usleep(300000);
}

int findBestDirection(float *left_dist, float *center_dist, float *right_dist) {
    printf("Scanning for obstacles...\n");

    setServoAngle(LEFT);
    usleep(500000);
    *left_dist = getDistance();
    printf("  Left (0°): %.2f cm\n", *left_dist);

    setServoAngle(CENTER);
    usleep(500000);
    *center_dist = getDistance();
    printf("  Center (90°): %.2f cm\n", *center_dist);

    setServoAngle(RIGHT);
    usleep(500000);
    *right_dist = getDistance();
    printf("  Right (180°): %.2f cm\n", *right_dist);

    setServoAngle(CENTER);

    float max_dist = *left_dist;
    int best_direction = LEFT;

    if (*center_dist > max_dist) {
        max_dist = *center_dist;
        best_direction = CENTER;
    }

    if (*right_dist > max_dist) {
        max_dist = *right_dist;
        best_direction = RIGHT;
    }

    return best_direction;
}
