CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lm -llgpio

COMMON_OBJS = I2C.o PWM.o MotorHat.o Robot.o
SENSOR_OBJS = Sensor.o
SERVO_OBJS = Servo.o

all: RobotTest SensorTest ServoTest ObstacleAvoidance

RobotTest: $(COMMON_OBJS) RobotTest.o
	$(CC) $^ -o $@ $(LDFLAGS)

SensorTest: $(SENSOR_OBJS) SensorTest.o
	$(CC) $^ -o $@ $(LDFLAGS)

ServoTest: $(COMMON_OBJS) $(SENSOR_OBJS) $(SERVO_OBJS) ServoTest.o
	$(CC) $^ -o $@ $(LDFLAGS)

ServoAngleTest: $(COMMON_OBJS) $(SENSOR_OBJS) $(SERVO_OBJS) ServoAngleTest.o
	$(CC) $^ -o $@ $(LDFLAGS)

ObstacleAvoidance: $(COMMON_OBJS) $(SENSOR_OBJS) $(SERVO_OBJS) ObstacleAvoidance.o
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o RobotTest SensorTest ServoTest ServoAngleTest ObstacleAvoidance

run-robot: RobotTest
	sudo ./RobotTest

run-sensor: SensorTest
	sudo ./SensorTest

run-servo: ServoTest
	sudo ./ServoTest

run-avoidance: ObstacleAvoidance
	sudo ./ObstacleAvoidance

.PHONY: all clean run-robot run-sensor run-servo run-avoidance
