#include "Encoder.h"
#include "Sensor.h"
#include "SerialComm.h"
#include <Arduino.h>
#include <TimerOne.h>

SerialComm comm;
Sensor     senU{ 8, 7 }; // up
Sensor     senD{ 8, 7 }; // down
Sensor     senL{ 8, 7 }; // left
Sensor     senR{ 8, 7 }; // right
Encoder    enc1{ 2, 3 };
Encoder    enc2{ 2, 3 };



int x = 0;
int y = 0;
int speed = 0;
int distance = 0;

void PIDcontrol() {
    // ここにPID
}

void setup() {
    Timer1.initialize(100000); // ここに割り込みの周期を設定　1000000=1s
    Timer1.attachInterrupt(PIDcontrol);

    comm.begin(9600);

    senU.begin();
    senD.begin();
    senL.begin();
    senR.begin();
    enc1.begin();
    enc2.begin();
}

void loop() {
    senU.read();
    senD.read();
    senL.read();
    senR.read();
    comm.setUp(senU.getDistance());
    comm.setUp(senD.getDistance());
    comm.setUp(senL.getDistance());
    comm.setUp(senR.getDistance());
    comm.sendData();

    comm.readData();
    x = comm.getX();
    y = comm.getY();
    speed = comm.getSpeed();

    delay(50);
}
