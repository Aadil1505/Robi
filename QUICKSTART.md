# Quick Start Guide

## Before You Begin
⚠️ **IMPORTANT**: Suspend the robot off the ground before running any tests!

## Step 1: Verify I2C is Working
```bash
sudo i2cdetect -y 1
```
You should see device at address 0x60

## Step 2: Test Basic I2C Communication
```bash
sudo ./I2CTest
```
Expected output: Successfully reads and writes to register

## Step 3: Test Individual Motors
```bash
sudo ./SimpleMotorTest
```
Each motor should spin for 1 second

## Step 4: Test Full Robot Movement
```bash
sudo make run
```
OR
```bash
sudo ./RobotTest
```

Robot will:
1. Move forward for 2 seconds
2. Move backward for 2 seconds
3. Turn left for 2 seconds
4. Turn right for 2 seconds
5. Stop

## Emergency Stop
If anything goes wrong:
```bash
sudo ./StopRobot
```

## Rebuild Everything
```bash
make clean
make
```

## Common Issues

**"Permission denied"**
```bash
sudo usermod -a -G i2c pi
# Then logout and login again
```

**"No such device"**
- Check I2C is enabled: `sudo raspi-config` → Interfacing → I2C
- Check motor HAT is properly connected
- Verify with: `sudo i2cdetect -y 1`

**Motors don't move**
- Ensure battery power is ON
- Check motor HAT power switch
- Verify motors are connected correctly
