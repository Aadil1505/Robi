#include "Sensor.h"
#include <stdio.h>
#include <unistd.h>

int main(void) {
    printf("HC-SR04 Sensor Test\n");
    printf("===================\n\n");

    if (initSensor() < 0) {
        fprintf(stderr, "Failed to initialize sensor\n");
        return 1;
    }

    printf("Taking 10 measurements...\n\n");

    for (int i = 0; i < 10; i++) {
        float distance = getDistance();

        if (distance < 0) {
            printf("Measurement %d: Error\n", i + 1);
        } else {
            printf("Measurement %d: %.2f cm\n", i + 1, distance);
        }

        usleep(500000);
    }

    printf("\nTest complete\n");
    closeSensor();
    return 0;
}
