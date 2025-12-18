#include "Robot.h"
#include "Sensor.h"
#include "Servo.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#define OBSTACLE_THRESHOLD 20.0
#define SPEED 80
#define BACKUP_TIME 500000
#define TURN_TIME 505000
#define LOOP_DELAY_US 100000

static volatile int keep_running = 1;
static pthread_mutex_t state_mutex = PTHREAD_MUTEX_INITIALIZER;

void signal_handler(int sig) {
    printf("\nStopping robot...\n");
    pthread_mutex_lock(&state_mutex);
    keep_running = 0;
    pthread_mutex_unlock(&state_mutex);
}

int main(void) {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    printf("Multithreaded Obstacle Avoidance Robot\n");
    printf("=======================================\n\n");

    initRobot();

    if (initSensor() < 0) {
        fprintf(stderr, "Failed to initialize sensor\n");
        i2cClose();
        return 1;
    }

    if (initServo() < 0) {
        fprintf(stderr, "Failed to initialize servo\n");
        closeSensor();
        i2cClose();
        return 1;
    }

    printf("Starting sensor thread...\n");
    if (startSensorThread() < 0) {
        fprintf(stderr, "Failed to start sensor thread\n");
        closeSensor();
        i2cClose();
        return 1;
    }

    printf("Starting servo thread...\n");
    if (startServoThread() < 0) {
        fprintf(stderr, "Failed to start servo thread\n");
        stopSensorThread();
        closeSensor();
        i2cClose();
        return 1;
    }

    printf("Setting servo to forward position...\n");
    setServoAngle(CENTER);
    printf("Press Ctrl+C to stop\n\n");

    usleep(500000);

    while (1) {
        pthread_mutex_lock(&state_mutex);
        int running = keep_running;
        pthread_mutex_unlock(&state_mutex);

        if (!running) break;

        float distance = getDistance();

        if (distance < 0) {
            usleep(LOOP_DELAY_US);
            continue;
        }

        printf("Forward: %.2f cm\n", distance);

        if (distance > OBSTACLE_THRESHOLD) {
            moveForward(SPEED);
            usleep(LOOP_DELAY_US);
        } else {
            printf("Obstacle ahead! Scanning...\n");
            stopRobot();

            requestScan();

            ScanResult scan;
            while (!getScanResult(&scan)) {
                usleep(100000);
            }

            printf("  Left: %.2f cm\n", scan.left);
            printf("  Center: %.2f cm\n", scan.center);
            printf("  Right: %.2f cm\n", scan.right);

            if (scan.left > scan.right && scan.left > OBSTACLE_THRESHOLD) {
                printf("Turning left (%.2f cm)\n", scan.left);
                turnLeft(SPEED);
                usleep(TURN_TIME);
                stopRobot();
            } else if (scan.right > scan.left && scan.right > OBSTACLE_THRESHOLD) {
                printf("Turning right (%.2f cm)\n", scan.right);
                turnRight(SPEED);
                usleep(TURN_TIME);
                stopRobot();
            } else {
                printf("Both sides blocked - backing up\n");
                moveBackward(SPEED);
                usleep(BACKUP_TIME);
                stopRobot();
            }

            usleep(300000);
        }
    }

    printf("\nShutting down threads...\n");
    stopRobot();
    setServoAngle(CENTER);
    stopServoThread();
    stopSensorThread();
    closeSensor();
    i2cClose();
    printf("Shutdown complete\n");

    return 0;
}
