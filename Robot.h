#ifndef ROBOT_H
#define ROBOT_H

#include "MotorHat.h"

void initRobot(void);
void moveForward(int speed);
void moveBackward(int speed);
void turnLeft(int speed);
void turnRight(int speed);
void stopRobot(void);

#endif

