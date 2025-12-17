#include "Servo.h"
#include "Sensor.h"
#include "I2C.h"
#include <stdio.h>
#include <unistd.h>

int main(void) {
    float left_dist, center_dist, right_dist;

    printf("Servo and Sensor Test\n");
    printf("=====================\n\n");

    if (initSensor() < 0) {
        fprintf(stderr, "Failed to initialize sensor\n");
        return 1;
    }

    if (initServo() < 0) {
        fprintf(stderr, "Failed to initialize servo\n");
        closeSensor();
        return 1;
    }

    printf("Performing scan...\n\n");

    int best = findBestDirection(&left_dist, &center_dist, &right_dist);

    printf("\nBest direction: ");
    switch(best) {
        case LEFT:
            printf("LEFT (0°) - %.2f cm\n", left_dist);
            break;
        case CENTER:
            printf("CENTER (90°) - %.2f cm\n", center_dist);
            break;
        case RIGHT:
            printf("RIGHT (180°) - %.2f cm\n", right_dist);
            break;
    }

    printf("\nSweeping servo...\n");
    for (int angle = 30; angle <= 150; angle += 10) {
        printf("Angle %d°...\n", angle);
        setServoAngle(angle);
        usleep(500000);
    }

    printf("\nLOOK CENTER\n");
    setServoAngle(CENTER);
    usleep(500000);
    printf("\nLOOK RIGHT\n");
    setServoAngle(RIGHT);
    usleep(500000);
    printf("\nLOOK LEFT\n");
    setServoAngle(LEFT);
    usleep(500000);

    setServoAngle(CENTER);
    printf("\nTest complete\n");

    closeSensor();
    i2cClose();

    return 0;
}
