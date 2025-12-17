# Robi Robot C Library - Obstacle Avoidance

This project implements hardware drivers for the Robi robot with obstacle avoidance capabilities using an HC-SR04 ultrasonic sensor.

## Hardware Configuration

### Motor HAT
- I2C Address: 0x60
- Connected to /dev/i2c-1

### HC-SR04 Ultrasonic Sensor
- TRIG Pin: GPIO 23 (BCM)
- ECHO Pin: GPIO 24 (BCM)
- Range: 2cm - 400cm

## Files

### Header Files
- `I2C.h` - I2C communication interface
- `PWM.h` - PWM control for motor HAT
- `MotorHat.h` - Motor HAT driver
- `Robot.h` - High-level robot movement functions
- `Sensor.h` - HC-SR04 ultrasonic sensor driver

### Implementation Files
- `I2C.c` - I2C implementation using Linux i2c-dev
- `PWM.c` - PWM implementation
- `MotorHat.c` - Motor control implementation
- `Robot.c` - Robot movement logic (forward, backward, turn, stop)
- `Sensor.c` - HC-SR04 sensor implementation using lgpio

### Test Programs
- `RobotTest.c` - Test basic robot movements
- `SensorTest.c` - Test HC-SR04 sensor readings
- `ObstacleAvoidance.c` - **Main autonomous navigation program**

## Building

### Build All Programs
```bash
cd /home/pi/CLibrary
make clean
make all
```

### Build Individual Programs
```bash
make RobotTest          # Build robot movement test only
make SensorTest         # Build sensor test only
make ObstacleAvoidance  # Build obstacle avoidance program only
```

## Running the Programs

**IMPORTANT:** All programs must be run with `sudo` because they require GPIO access.

### 1. Test Robot Movement
Tests basic robot movements (forward, backward, left, right).
```bash
make run-robot
# OR
sudo ./RobotTest
```

### 2. Test Sensor
Tests the HC-SR04 sensor and takes 10 distance measurements.
```bash
make run-sensor
# OR
sudo ./SensorTest
```

### 3. Run Obstacle Avoidance (Main Program)
Autonomous navigation with obstacle avoidance.
```bash
make run-avoidance
# OR
sudo ./ObstacleAvoidance
```

**Make sure Robi is suspended off the table or has enough space to move around!**

## Obstacle Avoidance Behavior

The `ObstacleAvoidance` program implements the following logic:

1. **Continuous Distance Monitoring**: Reads distance from HC-SR04 sensor
2. **Clear Path (distance > 20cm)**: Moves forward at moderate speed
3. **Obstacle Detected (distance ≤ 20cm)**:
   - Stops immediately
   - Activates servo scanning: sweeps sensor LEFT → CENTER → RIGHT
   - Analyzes which direction has the most open space
   - **If LEFT has most space**: Turns left
   - **If RIGHT has most space**: Turns right
   - **If CENTER has most space**: Continues forward
   - **If ALL directions blocked** (all < 20cm):
     - Turns around 180°
     - Scans again in opposite direction (behind LEFT → CENTER → RIGHT)
     - Moves toward direction with most space
     - If completely surrounded: backs up and retries

4. **Graceful Shutdown**: Press `Ctrl+C` to stop the program safely

## Key Parameters (ObstacleAvoidance.c)

You can modify these constants to tune behavior:

- `OBSTACLE_THRESHOLD`: Distance threshold in cm (default: 20.0)
- `FORWARD_SPEED`: Speed when moving forward (default: 100)
- `TURN_SPEED`: Speed when turning (default: 120)
- `BACKUP_TIME`: How long to back up in microseconds (default: 500000 = 0.5s)
- `TURN_TIME`: How long to turn in microseconds (default: 800000 = 0.8s)

## Troubleshooting

### Sensor Not Working ("Timeout waiting for ECHO" errors)

The HC-SR04 sensor requires proper wiring and power. If you see timeout errors:

1. **Check HC-SR04 Power**:
   - VCC must be connected to 5V power
   - GND must be connected to ground
   - The sensor will not work without 5V power!

2. **Check Wiring**:
   - TRIG → GPIO 23 (physical pin 16)
   - ECHO → GPIO 24 (physical pin 18)
   - **IMPORTANT**: The ECHO pin outputs 5V, but Raspberry Pi GPIO pins are 3.3V tolerant
   - You MUST use a voltage divider (two resistors: 1kΩ and 2kΩ) on the ECHO line to step down 5V to 3.3V
   - Or use a level shifter module

3. **Verify GPIO permissions**:
   ```bash
   sudo usermod -a -G gpio pi
   # Log out and back in after this command
   ```

4. **Check GPIO assignment**:
   ```bash
   gpioinfo | grep -E "GPIO23|GPIO24"
   ```
   Should show both pins as available (unused or claimed by your program)

5. **Test with multimeter** (if available):
   - Measure voltage on sensor VCC pin (should be ~5V)
   - Check if TRIG pin toggles when program runs

6. **Run sensor test with debug output**:
   ```bash
   sudo ./SensorTest
   ```

### Robot Not Moving
1. Verify motor HAT power is ON
2. Check I2C connection: `sudo i2cdetect -y 1` (should show 0x60 and 0x70)
3. Run robot test: `sudo ./RobotTest`
4. Make sure robot is suspended off the ground for testing!

### Compilation Errors
Make sure lgpio development library is installed (for Raspberry Pi 5):
```bash
sudo apt-get update
sudo apt-get install liblgpio-dev
```

### HC-SR04 Wiring Diagram
```
HC-SR04          Raspberry Pi 5
--------         --------------
VCC     -------> 5V (Pin 2 or 4)
TRIG    -------> GPIO 23 (Pin 16)
ECHO    -------> [Voltage Divider] -------> GPIO 24 (Pin 18)
GND     -------> GND (Pin 6, 9, 14, 20, 25, 30, 34, or 39)

Voltage Divider for ECHO pin:
ECHO (5V) ---[1kΩ resistor]--- GPIO24 (3.3V) ---[2kΩ resistor]--- GND
```

## Credits

Based on CSC155 Final Project specifications by Prof. Thomas G. Re, Nassau Community College.

HC-SR04 sensor protocol adapted from Python reference implementation in `/home/pi/sensor.py`.
