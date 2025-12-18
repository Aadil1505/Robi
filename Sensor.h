#ifndef SENSOR_H
#define SENSOR_H

#define TRIG_PIN 23
#define ECHO_PIN 24

int initSensor(void);
int startSensorThread(void);
void stopSensorThread(void);
float getDistance(void);
float readDistance(void);
void closeSensor(void);

#endif
