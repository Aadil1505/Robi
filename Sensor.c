#include "Sensor.h"
#include <lgpio.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>

#define TIMEOUT_US 38000
#define POLL_INTERVAL_US 50000

static int h = -1;
static pthread_t sensor_thread;
static pthread_mutex_t distance_mutex = PTHREAD_MUTEX_INITIALIZER;
static volatile int thread_running = 0;
static float latest_distance = -1.0;

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

float readDistance(void) {
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
            return -1.0;
        }
    }

    uint64_t pulse_duration = pulse_end - pulse_start;
    return pulse_duration * 0.017150;
}

static void* sensor_thread_func(void* arg) {
    while (thread_running) {
        float distance = readDistance();

        pthread_mutex_lock(&distance_mutex);
        latest_distance = distance;
        pthread_mutex_unlock(&distance_mutex);

        usleep(POLL_INTERVAL_US);
    }
    return NULL;
}

int startSensorThread(void) {
    thread_running = 1;
    if (pthread_create(&sensor_thread, NULL, sensor_thread_func, NULL) != 0) {
        perror("Failed to create sensor thread");
        thread_running = 0;
        return -1;
    }
    return 0;
}

void stopSensorThread(void) {
    if (thread_running) {
        thread_running = 0;
        pthread_join(sensor_thread, NULL);
    }
}

float getDistance(void) {
    float distance;
    pthread_mutex_lock(&distance_mutex);
    distance = latest_distance;
    pthread_mutex_unlock(&distance_mutex);
    return distance;
}

void closeSensor(void) {
    stopSensorThread();
    if (h >= 0) {
        lgGpioWrite(h, TRIG_PIN, 0);
        lgGpioFree(h, TRIG_PIN);
        lgGpioFree(h, ECHO_PIN);
        lgGpiochipClose(h);
        h = -1;
    }
}
