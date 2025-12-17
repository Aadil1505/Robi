#include <lgpio.h>
#include <stdio.h>
#include <unistd.h>

#define TRIG_PIN 23
#define ECHO_PIN 24

int main(void) {
    int h, result, value;

    printf("Simple GPIO Test\n");
    printf("================\n\n");

    // Open GPIO chip
    h = lgGpiochipOpen(0);
    if (h < 0) {
        fprintf(stderr, "Failed to open GPIO chip\n");
        return 1;
    }
    printf("Opened GPIO chip, handle: %d\n", h);

    // Claim both pins as outputs for testing
    result = lgGpioClaimOutput(h, 0, TRIG_PIN, 0);
    if (result < 0) {
        fprintf(stderr, "Failed to claim TRIG pin: %d\n", result);
        lgGpiochipClose(h);
        return 1;
    }
    printf("Claimed GPIO%d (TRIG) as output\n", TRIG_PIN);

    result = lgGpioClaimOutput(h, 0, ECHO_PIN, 0);
    if (result < 0) {
        fprintf(stderr, "Failed to claim ECHO pin: %d\n", result);
        lgGpioFree(h, TRIG_PIN);
        lgGpiochipClose(h);
        return 1;
    }
    printf("Claimed GPIO%d (ECHO) as output\n", ECHO_PIN);

    // Toggle TRIG pin
    printf("\nToggling TRIG pin (GPIO%d):\n", TRIG_PIN);
    for (int i = 0; i < 5; i++) {
        lgGpioWrite(h, TRIG_PIN, 1);
        printf("  Set HIGH\n");
        usleep(500000);
        lgGpioWrite(h, TRIG_PIN, 0);
        printf("  Set LOW\n");
        usleep(500000);
    }

    // Toggle ECHO pin
    printf("\nToggling ECHO pin (GPIO%d):\n", ECHO_PIN);
    for (int i = 0; i < 5; i++) {
        lgGpioWrite(h, ECHO_PIN, 1);
        printf("  Set HIGH\n");
        usleep(500000);
        lgGpioWrite(h, ECHO_PIN, 0);
        printf("  Set LOW\n");
        usleep(500000);
    }

    // Cleanup
    printf("\nCleaning up...\n");
    lgGpioFree(h, TRIG_PIN);
    lgGpioFree(h, ECHO_PIN);
    lgGpiochipClose(h);

    printf("Test complete. You should have seen both LEDs blinking if connected.\n");
    return 0;
}
