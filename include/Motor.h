#ifndef MOTOR_H
#define MOTOR_H

#include "Arduino.h"
#include "Encoder.h"
class Motor
{
private:
    /* data */
    int motorspd;
    int motorPin;
    int targetdeg;
    int currentdeg;
public:
    void Init(int motorPin);
    void rotateMotor();
};

#endif