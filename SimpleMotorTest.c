#include "Robot.h"
#include <stdio.h>
#include <unistd.h>

int main(void) {
    printf("Initializing robot...\n");
    initRobot();
    sleep(1);

    printf("\nTesting Motor 0...\n");
    setSpeed(0, 100);
    run(0, FORWARD);
    sleep(1);
    run(0, RELEASE);
    sleep(1);

    printf("\nTesting Motor 1...\n");
    setSpeed(1, 100);
    run(1, FORWARD);
    sleep(1);
    run(1, RELEASE);
    sleep(1);

    printf("\nTesting Motor 2...\n");
    setSpeed(2, 100);
    run(2, FORWARD);
    sleep(1);
    run(2, RELEASE);
    sleep(1);

    printf("\nTesting Motor 3...\n");
    setSpeed(3, 100);
    run(3, FORWARD);
    sleep(1);
    run(3, RELEASE);

    printf("\nAll motors tested!\n");
    i2cClose();
    return 0;
}
