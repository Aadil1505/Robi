#include "Sensor.h"
#include <lgpio.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

#define TIMEOUT_US 38000

static int h = -1;

static uint64_t getMicros(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint64_t)(tv.tv_sec * 1000000ULL + tv.tv_usec);
}

int initSensor(void) {
    h = lgGpiochipOpen(0);
    if (h < 0) {
        fprintf(stderr, "Failed to open gpiochip0: %d\n", h);
        return -1;
    }

    int result = lgGpioClaimOutput(h, 0, TRIG_PIN, 0);
    if (result < 0) {
        fprintf(stderr, "Failed to claim TRIG pin %d: %d\n", TRIG_PIN, result);
        lgGpiochipClose(h);
        return -1;
    }

    result = lgGpioClaimInput(h, 0, ECHO_PIN);
    if (result < 0) {
        fprintf(stderr, "Failed to claim ECHO pin %d: %d\n", ECHO_PIN, result);
        lgGpioFree(h, TRIG_PIN);
        lgGpiochipClose(h);
        return -1;
    }

    lgGpioWrite(h, TRIG_PIN, 0);
    printf("Waiting for sensor to settle...\n");
    sleep(2);
    printf("Sensor ready\n");

    return 0;
}

float getDistance(void) {
    uint64_t pulse_start, pulse_end, timeout_start;
    int echo_value;

    lgGpioWrite(h, TRIG_PIN, 1);
    usleep(10);
    lgGpioWrite(h, TRIG_PIN, 0);

    timeout_start = getMicros();
    while (1) {
        echo_value = lgGpioRead(h, ECHO_PIN);
        if (echo_value < 0) return -1.0;
        if (echo_value == 1) {
            pulse_start = getMicros();
            break;
        }
        if ((getMicros() - timeout_start) > TIMEOUT_US) {
            fprintf(stderr, "Sensor timeout\n");
            return -1.0;
        }
    }

    timeout_start = getMicros();
    while (1) {
        echo_value = lgGpioRead(h, ECHO_PIN);
        if (echo_value == 0) {
            pulse_end = getMicros();
            break;
        }
        if ((getMicros() - timeout_start) > TIMEOUT_US) {
            fprintf(stderr, "Sensor timeout\n");
            return -1.0;
        }
    }

    uint64_t pulse_duration = pulse_end - pulse_start;
    return pulse_duration * 0.017150;
}

void closeSensor(void) {
    if (h >= 0) {
        lgGpioWrite(h, TRIG_PIN, 0);
        lgGpioFree(h, TRIG_PIN);
        lgGpioFree(h, ECHO_PIN);
        lgGpiochipClose(h);
        h = -1;
    }
}
