#include "Robot.h"
#include <stdio.h>

int main(void) {
    printf("Emergency stop - releasing all motors...\n");
    initRobot();
    stopRobot();
    i2cClose();
    printf("All motors stopped.\n");
    return 0;
}
