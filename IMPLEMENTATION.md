# Implementation Details

## Assignment Compliance

This implementation follows the CSC155 Final Project requirements:

### ✅ Required Files (All Implemented)
- **I2C.h / I2C.c** - I2C communication layer
- **PWM.h / PWM.c** - PWM servo driver
- **MotorHat.h / MotorHat.c** - Motor HAT control
- **Robot.h / Robot.c** - Robot movement functions
- **Test programs** - RobotTest.c (and extras)

### ✅ Assignment Requirements Met

1. **I2C Communication** ✓
   - Uses Linux I2C device driver instead of bcm2835 (Pi 5 compatible)
   - `i2cInit()` - Initialize I2C bus with device address
   - `write8()` - Write 8-bit value to register
   - `readU8()` - Read 8-bit value from register

2. **PWM Driver** ✓
   - `PWMInit()` - Initialize PCA9685 chip
   - `setPWMFreq()` - Set PWM frequency (1600 Hz)
   - `setPWM()` - Set individual channel PWM
   - `setAllPWM()` - Set all channels at once

3. **Motor HAT Driver** ✓
   - `initHat()` - Initialize motor array and PWM
   - `setSpeed()` - Set motor speed (0-255)
   - `run()` - Control motor direction (FORWARD/BACKWARD/RELEASE)
   - `setPin()` - Control individual motor pins

4. **Robot Movement** ✓
   - `initRobot()` - Initialize entire system
   - `moveForward()` - All motors forward
   - `moveBackward()` - All motors backward
   - `turnLeft()` - Left side backward, right side forward
   - `turnRight()` - Right side backward, left side forward
   - `stopRobot()` - Release all motors

## Key Implementation Decisions

### Linux I2C Instead of BCM2835
**Why?**
- BCM2835 library doesn't work on Raspberry Pi 5
- Linux I2C is the standard, portable approach
- No external dependencies needed
- Works on all Pi models

**How?**
```c
// Open I2C device
int fd = open("/dev/i2c-1", O_RDWR);

// Set slave address
ioctl(fd, I2C_SLAVE, address);

// Write: send register + value
uint8_t buffer[2] = {reg, value};
write(fd, buffer, 2);

// Read: write register, then read value
write(fd, &reg, 1);
read(fd, &value, 1);
```

### Motor Configuration
Based on Python reference implementation:
```c
Motor 0: PWM=8,  IN1=10, IN2=9
Motor 1: PWM=13, IN1=11, IN2=12
Motor 2: PWM=2,  IN1=4,  IN2=3
Motor 3: PWM=7,  IN1=5,  IN2=6
```

### PWM Frequency Calculation
```c
prescaleval = 25000000.0 / 4096.0 / freq - 1.0
prescale = floor(prescaleval + 0.5)
```
Set to 1600 Hz for motor control.

### Speed Control
Speed range: 0-255 (maps to PWM 0-4095)
```c
pwm_value = speed * 16
```

## Code Style Guidelines Followed

1. **Minimal Comments** - Code is self-documenting
2. **Clear Naming** - Function and variable names are descriptive
3. **Simple Logic** - No complex abstractions
4. **Consistent Style** - Same formatting throughout
5. **Error Handling** - Uses perror() for clear messages
6. **Type Safety** - Uses uint8_t for 8-bit values

## Testing Approach

### Progressive Testing
1. **I2CTest** - Verify basic communication
2. **SimpleMotorTest** - Test each motor individually
3. **RobotTest** - Test coordinated movement
4. **StopRobot** - Emergency stop utility

### Safety Features
- All tests print what they're doing
- Motors are released after each test
- Emergency stop utility available
- Clear warnings about suspending robot

## Compilation

Clean compilation with strict warnings:
```bash
gcc -Wall -O2 -c file.c
```

No warnings, no errors, optimized code.

## File Organization

```
CLibrary/
├── Core Implementation
│   ├── I2C.c / I2C.h           (56 lines)
│   ├── PWM.c / PWM.h           (83 lines)
│   ├── MotorHat.c / MotorHat.h (84 lines)
│   └── Robot.c / Robot.h       (74 lines)
├── Test Programs
│   ├── I2CTest.c               (25 lines)
│   ├── SimpleMotorTest.c       (40 lines)
│   ├── RobotTest.c             (43 lines)
│   └── StopRobot.c             (11 lines)
├── Build System
│   └── Makefile
└── Documentation
    ├── README.md
    ├── QUICKSTART.md
    ├── SUMMARY.md
    └── IMPLEMENTATION.md
```

Total: ~430 lines of clean, readable code.
