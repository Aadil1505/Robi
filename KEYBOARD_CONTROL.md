# Keyboard Control Guide

## Quick Start

Run the keyboard control program:
```bash
cd /home/pi/CLibrary
sudo ./KeyboardControl
```

Or use the makefile shortcut:
```bash
make run-keyboard
```

## Controls

**Arrow Keys:**
- **↑ UP**    : Move Forward
- **↓ DOWN**  : Move Backward
- **← LEFT**  : Turn Left
- **→ RIGHT** : Turn Right

**Other Keys:**
- **SPACE** : Stop robot
- **q**     : Quit program

## Features

### Real-Time Distance Display
The screen shows live distance readings from the ultrasonic sensor, updated continuously by the background sensor thread.

### Automatic Stop
When you release a key, the robot automatically stops. This prevents runaway motion.

### Obstacle Warning
If an object is detected within 20cm, the screen displays "OBSTACLE DETECTED!" in bold.

### Clean Interface
The ncurses-based UI provides a clean, full-screen control interface with:
- Current distance reading
- Control instructions
- Robot status

## How It Works

The keyboard control uses multithreading:
- **Sensor Thread**: Continuously monitors distance (50ms intervals)
- **Main Thread**: Handles keyboard input and motor control
- **Non-blocking Input**: UI updates in real-time while waiting for keys

## Safety Tips

1. **Clear Space**: Ensure the robot has space to move
2. **Watch Distance**: Keep an eye on the distance reading
3. **Emergency Stop**: Press SPACE or 'q' to stop immediately
4. **Ctrl+C**: Also works as emergency stop

## Technical Details

- Uses `ncurses` library for terminal control
- Non-blocking keyboard input with 50ms refresh
- Thread-safe sensor readings
- Automatic motor stop on key release (ERR state)

## Troubleshooting

**Robot doesn't move:**
- Check that hardware is connected
- Verify I2C devices are detected: `i2cdetect -y 1`
- Ensure you're running with `sudo`

**Keys don't respond:**
- Make sure terminal window has focus
- Try pressing keys firmly
- Check that ncurses is installed: `dpkg -l | grep ncurses`

**Screen doesn't clear properly:**
- Terminal may not support ncurses features
- Try running in a different terminal emulator
