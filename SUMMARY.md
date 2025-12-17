# Robi Robot Project - Implementation Summary

## Overview
Complete C implementation of hardware drivers for the Robi robot using modern Linux I2C interface, compatible with Raspberry Pi 5.

## Key Features
✅ Clean, readable code with minimal comments
✅ Uses standard Linux I2C interface (no bcm2835 dependency)
✅ Works with Raspberry Pi 5
✅ Follows the assignment structure exactly
✅ Includes multiple test programs
✅ Proper error handling

## Architecture

### Layer 1: I2C Communication (I2C.c/h)
- Direct Linux kernel I2C interface using `/dev/i2c-1`
- Functions: `i2cInit()`, `write8()`, `readU8()`, `i2cClose()`
- No external library dependencies (just Linux kernel)

### Layer 2: PWM Control (PWM.c/h)
- Controls PCA9685 PWM chip on the Motor HAT
- Functions: `PWMInit()`, `setPWMFreq()`, `setPWM()`, `setAllPWM()`
- Configures 16-channel PWM for motor control

### Layer 3: Motor HAT (MotorHat.c/h)
- DC motor control through Motor HAT
- Functions: `initHat()`, `setSpeed()`, `run()`, `setPin()`
- Supports 4 motors with FORWARD, BACKWARD, RELEASE commands

### Layer 4: Robot Movement (Robot.c/h)
- High-level robot control
- Functions: `initRobot()`, `moveForward()`, `moveBackward()`, `turnLeft()`, `turnRight()`, `stopRobot()`

## Test Programs

### 1. I2CTest
Basic I2C communication test - verifies read/write operations
```bash
sudo ./I2CTest
```

### 2. SimpleMotorTest
Tests each motor individually
```bash
sudo ./SimpleMotorTest
```

### 3. RobotTest
Full robot movement test (forward, backward, left, right)
```bash
sudo ./RobotTest
```

### 4. StopRobot
Emergency stop utility
```bash
sudo ./StopRobot
```

## Building

```bash
make clean    # Clean previous builds
make          # Build RobotTest
```

## Code Statistics
- Total lines of code: ~400 (excluding tests)
- Header files: 4
- Implementation files: 4
- Test programs: 4
- No warnings, no errors
- Clean compilation with -Wall -O2

## Differences from Assignment Template

1. **No bcm2835 library** - Uses Linux I2C instead (Pi 5 compatible)
2. **Cleaner headers** - Removed unnecessary includes
3. **Proper types** - Uses uint8_t instead of unsigned char
4. **Error handling** - Uses perror() for clear error messages
5. **Modern C** - Clean, simple, readable code

## Motor HAT Configuration
- Address: 0x60
- PWM Frequency: 1600 Hz
- Motor channels: 0-3
- Speed range: 0-255

## Safety Features
- All motors released on program exit
- Emergency stop utility included
- Clear error messages
- Validates motor IDs and speed ranges
