#include "Robot.h"
#include "Sensor.h"
#include "Servo.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#define OBSTACLE_THRESHOLD 20.0
#define SPEED 80
#define BACKUP_TIME 500000
#define TURN_TIME 505000        // Calibrated 90-degree turn time
#define SENSOR_DELAY 100000

volatile int keep_running = 1;

void signal_handler(int sig) {
    printf("\nStopping robot...\n");
    keep_running = 0;
}

int main(void) {
    float distance, left_dist, right_dist;

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    printf("Simple Obstacle Avoidance Robot\n");
    printf("================================\n\n");

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

    // Set servo to forward position at start
    printf("Setting servo to forward position...\n");
    setServoAngle(CENTER);
    printf("Press Ctrl+C to stop\n\n");

    while (keep_running) {
        distance = getDistance();

        if (distance < 0) {
            printf("Sensor error\n");
            usleep(SENSOR_DELAY);
            continue;
        }

        printf("Forward: %.2f cm\n", distance);

        // Path is clear, keep moving forward
        if (distance > OBSTACLE_THRESHOLD) {
            moveForward(SPEED);
            usleep(SENSOR_DELAY);
        }
        // Obstacle detected - scan left and right
        else {
            printf("Obstacle ahead! Scanning left and right...\n");
            stopRobot();
            usleep(300000);

            // Scan LEFT 
            setServoAngle(LEFT);
            usleep(300000);
            left_dist = getDistance();
            printf("  Left: %.2f cm\n", left_dist);

            // Scan RIGHT 
            setServoAngle(RIGHT);
            usleep(300000);
            right_dist = getDistance();
            printf("  Right: %.2f cm\n", right_dist);

            // Reset servo to forward
            setServoAngle(CENTER);

            // Decide which way to turn
            if (left_dist > right_dist && left_dist > OBSTACLE_THRESHOLD) {
                printf("Turning left (%.2f cm)\n", left_dist);
                turnLeft(SPEED);
                usleep(TURN_TIME);
                stopRobot();
            }
            else if (right_dist > left_dist && right_dist > OBSTACLE_THRESHOLD) {
                printf("Turning right (%.2f cm)\n", right_dist);
                turnRight(SPEED);
                usleep(TURN_TIME);
                stopRobot();
            }
            // Both sides blocked - backup and try again
            else {
                printf("Both sides blocked - backing up\n");
                moveBackward(SPEED);
                usleep(BACKUP_TIME);
                stopRobot();
            }

            usleep(300000);
        }
    }

    stopRobot();
    setServoAngle(CENTER);
    closeSensor();
    i2cClose();
    printf("Shutdown complete\n");

    return 0;
}
