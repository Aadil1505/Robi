#ifndef SERVO_H
#define SERVO_H

#define SERVO_CHANNEL 0
#define SERVO_MIN 150
#define SERVO_MAX 600

typedef enum {
    LEFT = 180,
    CENTER = 90,
    RIGHT = 0
} ServoPosition;

int initServo(void);
void setServoAngle(int angle);
int findBestDirection(float *left_dist, float *center_dist, float *right_dist);

#endif
