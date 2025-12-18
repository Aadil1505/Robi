#include "Robot.h"
#include "Sensor.h"
#include "Servo.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>

static volatile int keep_running = 1;

void signal_handler(int sig) {
    keep_running = 0;
}

void print_timestamp(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    printf("[%ld.%03ld] ", ts.tv_sec % 100, ts.tv_nsec / 1000000);
}

int main(void) {
    signal(SIGINT, signal_handler);

    printf("=================================================\n");
    printf("Multithreading Demo - Watch the threads work!\n");
    printf("=================================================\n\n");

    initRobot();

    if (initSensor() < 0) {
        fprintf(stderr, "Failed to initialize sensor\n");
        return 1;
    }

    if (initServo() < 0) {
        fprintf(stderr, "Failed to initialize servo\n");
        closeSensor();
        return 1;
    }

    printf("\n--- WITHOUT THREADING (Old Way) ---\n");
    printf("Each operation blocks the next one:\n\n");

    print_timestamp();
    printf("Reading sensor...\n");
    float dist1 = readDistance();

    print_timestamp();
    printf("  Result: %.2f cm\n", dist1);

    print_timestamp();
    printf("Moving servo to left...\n");
    setServoAngle(LEFT);

    print_timestamp();
    printf("  Servo moved\n");

    print_timestamp();
    printf("Reading sensor again...\n");
    float dist2 = readDistance();

    print_timestamp();
    printf("  Result: %.2f cm\n\n", dist2);

    printf("Notice: Each step waits for the previous to complete\n");
    printf("Total time: ~1 second of blocking operations\n\n");

    sleep(2);

    printf("\n--- WITH THREADING (New Way) ---\n");
    printf("Operations run concurrently:\n\n");

    print_timestamp();
    printf("Starting sensor thread (continuous background polling)...\n");
    startSensorThread();

    print_timestamp();
    printf("Starting servo thread (async scanning)...\n");
    startServoThread();

    usleep(100000);

    print_timestamp();
    printf("Main thread: Getting distance (non-blocking!)...\n");
    float dist3 = getDistance();

    print_timestamp();
    printf("  Instant result: %.2f cm (from background thread)\n", dist3);

    print_timestamp();
    printf("Main thread: Requesting scan (non-blocking!)...\n");
    requestScan();

    print_timestamp();
    printf("Main thread: I can do other things while scan runs!\n");

    print_timestamp();
    printf("Main thread: Like moving the robot...\n");
    moveForward(60);
    usleep(200000);
    stopRobot();

    print_timestamp();
    printf("Main thread: Or reading sensor again...\n");
    printf("  Distance: %.2f cm\n", getDistance());

    print_timestamp();
    printf("Main thread: Now checking if scan is ready...\n");

    ScanResult scan;
    while (!getScanResult(&scan)) {
        print_timestamp();
        printf("  Scan still in progress, waiting...\n");
        usleep(200000);
    }

    print_timestamp();
    printf("  Scan complete! L:%.1f C:%.1f R:%.1f\n\n",
           scan.left, scan.center, scan.right);

    printf("=================================================\n");
    printf("BENEFITS OF MULTITHREADING:\n");
    printf("=================================================\n");
    printf("1. NON-BLOCKING: Main loop never freezes\n");
    printf("2. CONTINUOUS SENSING: Always getting fresh data\n");
    printf("3. PARALLEL WORK: Scan while moving/deciding\n");
    printf("4. RESPONSIVE: Can react to Ctrl+C instantly\n");
    printf("5. EFFICIENT: CPU does useful work while waiting\n");
    printf("=================================================\n\n");

    printf("Press Ctrl+C to exit\n");

    while (keep_running) {
        print_timestamp();
        printf("Distance: %.2f cm (updated every 50ms by thread)\n", getDistance());
        sleep(1);
    }

    printf("\nCleaning up...\n");
    stopRobot();
    stopServoThread();
    stopSensorThread();
    closeSensor();
    i2cClose();

    return 0;
}
