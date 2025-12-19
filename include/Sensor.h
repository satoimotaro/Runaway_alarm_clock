#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

class Sensor {
public:
    Sensor(uint8_t echoPin, uint8_t triPin);
    void begin();
    void read();
    int  getDistance();

private:
    int echoPin;
    int trigPin;
    int duration = 0;
    int distance = 0;
};

#endif
