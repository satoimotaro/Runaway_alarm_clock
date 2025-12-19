#ifndef SERIAL_COMM_H
#define SERIAL_COMM_H

#include <Arduino.h>

class SerialComm {
public:
    void begin(int baud);
    void sendData();
    void readData();
    void setUp(int value);
    void setDown(int value);
    void setLeft(int value);
    void setRight(int value);
    int  getX();
    int  getY();
    int  getSpeed();

private:
    int up = 0;
    int down = 0;
    int left = 0;
    int right = 0;

    int x = 0;
    int y = 0;
    int speed = 0;
};

#endif
