#ifndef SENSOR_H
#define SENSOR_H

#define TRIG_PIN 23
#define ECHO_PIN 24

int initSensor(void);
float getDistance(void);
void closeSensor(void);

#endif
