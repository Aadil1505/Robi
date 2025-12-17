#include "Servo.h"
#include "Sensor.h"
#include "I2C.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    printf("Quick RIGHT Direction Test\n");
    printf("===========================\n\n");

    if (initSensor() < 0) {
        fprintf(stderr, "Failed to initialize sensor\n");
        return 1;
    }

    if (initServo() < 0) {
        fprintf(stderr, "Failed to initialize servo\n");
        closeSensor();
        return 1;
    }

    printf("Starting at 0° (forward)...\n");
    setServoAngle(0);
    sleep(2);

    // Try sweeping in the opposite direction
    printf("\nTrying negative angles to find RIGHT:\n");

    int angles[] = {-30, -45, -60, -90};
    for (int i = 0; i < 4; i++) {
        printf("\nSetting to %d°...\n", angles[i]);
        setServoAngle(angles[i]);
        sleep(2);
        printf("  Direction (forward/right/backward/same): ");
        char dir[20];
        scanf("%s", dir);

        if (strcmp(dir, "right") == 0) {
            printf("\n*** FOUND IT! RIGHT = %d° ***\n", angles[i]);
            break;
        }
    }

    // Reset to center
    printf("\nResetting to 0° (center)...\n");
    setServoAngle(0);
    sleep(1);

    closeSensor();
    i2cClose();

    return 0;
}
