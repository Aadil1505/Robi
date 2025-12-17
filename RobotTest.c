#include "Robot.h"
#include <stdio.h>
#include <unistd.h>

// Tunable parameters
#define SPEED 80
#define MOVE_TIME 2000000       // 2 seconds for forward/backward (microseconds)
#define TURN_TIME_90 505000     // 0.8 seconds for 90-degree turn (adjust as needed)
#define PAUSE_TIME 1000000      // 1 second pause between actions

int main(void) {
    printf("Initializing robot...\n");
    initRobot();

    printf("Moving forward...\n");
    moveForward(SPEED);
    usleep(MOVE_TIME);

    printf("Moving backward...\n");
    moveBackward(SPEED);
    usleep(MOVE_TIME);

    printf("Stopping...\n");
    stopRobot();
    usleep(PAUSE_TIME);

    printf("Turning left (90 degrees)...\n");
    turnLeft(SPEED);
    usleep(TURN_TIME_90);

    printf("Stopping...\n");
    stopRobot();
    usleep(PAUSE_TIME);

    printf("Turning right (90 degrees)...\n");
    turnRight(SPEED);
    usleep(TURN_TIME_90);

    printf("Stopping...\n");
    stopRobot();

    printf("Test complete!\n");
    i2cClose();
    return 0;
}
