#ifndef SERIAL_COMM_H
#define SERIAL_COMM_H

#include <Arduino.h>
#include <SoftwareSerial.h>

class SerialComm {
public:
    void begin(uint32_t baud);
    void sendData();
    void readData();
    void readFromSoftwareSerial();
    void setCurrentPosition(int x, int y);
    void setMoveFlag(bool flag);
    int  getX();
    int  getY();
    int  getSpeed();
    bool getMoveFlag();

private:
    int sensor1 = 0;
    int sensor2 = 0;
    int sensor3 = 0;
    int sensor4 = 0;

    int targetx = 0;
    int targety = 0;
    int speed = 0;
    int currentx = 0;
    int currenty = 0;
    bool move_flag = false;
    SoftwareSerial mySerial{8, 7}; // RX, TX
};

#endif
