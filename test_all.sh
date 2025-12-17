#!/bin/bash

echo "================================================"
echo "Robi Robot - Complete Test Suite"
echo "================================================"
echo ""

echo "Step 1: Checking I2C bus..."
sudo i2cdetect -y 1 | grep -q "60"
if [ $? -eq 0 ]; then
    echo "✓ Motor HAT detected at address 0x60"
else
    echo "✗ Motor HAT not found!"
    exit 1
fi
echo ""

echo "Step 2: Testing I2C communication..."
sudo ./I2CTest
if [ $? -eq 0 ]; then
    echo "✓ I2C test passed"
else
    echo "✗ I2C test failed"
    exit 1
fi
echo ""

echo "Step 3: Ready to test motors"
echo "⚠️  WARNING: Make sure robot is suspended off the ground!"
echo ""
read -p "Press Enter to continue or Ctrl+C to cancel..."
echo ""

echo "Testing individual motors..."
sudo ./SimpleMotorTest
echo ""

echo "Testing full robot movement..."
sudo ./RobotTest
echo ""

echo "================================================"
echo "All tests complete!"
echo "================================================"
