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

typedef struct {
    float left;
    float center;
    float right;
    int ready;
} ScanResult;

int initServo(void);
int startServoThread(void);
void stopServoThread(void);
void setServoAngle(int angle);
void setServoAngleAsync(int angle);
void requestScan(void);
int getScanResult(ScanResult *result);
int findBestDirection(float *left_dist, float *center_dist, float *right_dist);

#endif
