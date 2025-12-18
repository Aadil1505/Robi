#include "Robot.h"

// Speed offset to compensate for slower left wheels
#define LEFT_WHEEL_OFFSET 80

void initRobot(void) {
    initHat();
}

void moveForward(int speed) {
    setSpeed(0, speed);
    setSpeed(1, speed);
    setSpeed(2, speed);
    setSpeed(3, speed);

    run(0, BACKWARD);
    run(1, FORWARD);
    run(2, BACKWARD);
    run(3, FORWARD);
}

void moveBackward(int speed) {
    setSpeed(0, speed);
    setSpeed(1, speed);
    setSpeed(2, speed);
    setSpeed(3, speed);

    run(0, FORWARD);
    run(1, BACKWARD);
    run(2, FORWARD);
    run(3, BACKWARD);
}

void turnRight(int speed) {
    setSpeed(0, speed);
    setSpeed(1, speed);
    setSpeed(2, speed);
    setSpeed(3, speed);

    run(0, FORWARD);
    run(1, FORWARD);
    run(2, FORWARD);
    run(3, FORWARD);
}

void turnLeft(int speed) {
    setSpeed(0, speed);
    setSpeed(1, speed);
    setSpeed(2, speed);
    setSpeed(3, speed);

    run(0, BACKWARD);
    run(1, BACKWARD);
    run(2, BACKWARD);
    run(3, BACKWARD);
}

void stopRobot(void) {
    run(0, RELEASE);
    run(1, RELEASE);
    run(2, RELEASE);
    run(3, RELEASE);
}
