#include "Sensor.h"

Sensor::Sensor(uint8_t echoPin, uint8_t trigPin) : echoPin(echoPin), trigPin(trigPin) {}

void Sensor::begin() {
    pinMode(echoPin, INPUT);
    pinMode(trigPin, OUTPUT);
}

void Sensor::read() {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    if (duration > 0) {
        duration = duration / 2;
        distance = duration / 2 / 29.1; // 音速341m/s 要検討
    }
}

int Sensor::getDistance() {
    return distance;
}

