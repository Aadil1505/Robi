#include "Robot.h"
#include "Sensor.h"
#include "Servo.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <ncurses.h>

#define SPEED 100
#define TURN_SPEED 80

static volatile int keep_running = 1;
static pthread_mutex_t state_mutex = PTHREAD_MUTEX_INITIALIZER;

void signal_handler(int sig) {
    pthread_mutex_lock(&state_mutex);
    keep_running = 0;
    pthread_mutex_unlock(&state_mutex);
}

void draw_ui(float distance, const char *current_action) {
    clear();

    printw("========================================\n");
    printw("   ROBOT KEYBOARD CONTROL\n");
    printw("========================================\n\n");

    printw("Controls:\n");
    printw("  UP    : Move Forward\n");
    printw("  DOWN  : Move Backward\n");
    printw("  LEFT  : Turn Left\n");
    printw("  RIGHT : Turn Right\n");
    printw("  SPACE : Stop\n");
    printw("  q     : Quit\n\n");

    printw("----------------------------------------\n");
    printw("Distance: %.2f cm\n", distance);
    printw("----------------------------------------\n\n");

    printw("Robot Status: ");
    if (distance < 20.0 && distance > 0) {
        attron(A_BOLD);
        printw("OBSTACLE DETECTED!");
        attroff(A_BOLD);
    } else {
        printw("Clear");
    }
    printw("\n\n");

    printw("Current Action: %s\n\n", current_action);

    printw("Press 'q' to exit\n");

    refresh();
}

int main(void) {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

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

    if (startSensorThread() < 0) {
        fprintf(stderr, "Failed to start sensor thread\n");
        closeSensor();
        i2cClose();
        return 1;
    }

    setServoAngle(CENTER);

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    curs_set(0);
    timeout(0);

    int ch;
    int last_key = ERR;
    const char *current_action = "Stopped";

    while (1) {
        pthread_mutex_lock(&state_mutex);
        int running = keep_running;
        pthread_mutex_unlock(&state_mutex);

        if (!running) break;

        float distance = getDistance();
        draw_ui(distance, current_action);

        ch = getch();

        if (ch != ERR) {
            last_key = ch;
        }

        if (last_key == 'q' || last_key == 'Q') {
            break;
        }

        switch(last_key) {
            case KEY_UP:
                moveForward(SPEED);
                current_action = "Moving Forward";
                break;

            case KEY_DOWN:
                moveBackward(SPEED);
                current_action = "Moving Backward";
                break;

            case KEY_LEFT:
                turnLeft(TURN_SPEED);
                current_action = "Turning Left";
                break;

            case KEY_RIGHT:
                turnRight(TURN_SPEED);
                current_action = "Turning Right";
                break;

            case ' ':
                stopRobot();
                current_action = "Stopped";
                last_key = ERR;
                break;

            default:
                stopRobot();
                current_action = "Stopped";
                break;
        }

        usleep(50000);
    }

    stopRobot();
    endwin();

    printf("\nShutting down...\n");
    setServoAngle(CENTER);
    stopSensorThread();
    closeSensor();
    i2cClose();
    printf("Shutdown complete\n");

    return 0;
}
