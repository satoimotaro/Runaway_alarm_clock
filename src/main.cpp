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

//Pin definitions
#define MOTOR1_PIN1 5
#define MOTOR1_PIN2 6
#define MOTOR2_PIN1 9
#define MOTOR2_PIN2 10

//PID parameters
#define KP 2.0
#define KI 0.5
#define KD 1.0
#define DT_MS 100

int x = 0;
int y = 0;
int speed = 0;
int distance = 0;

// PID制御用変数
int targetcnt = 0;
int targetcnt2 = 0;
int currentcnt1 = 0;
int currentcnt2 = 0;
int PrevP1 = 0;
int PrevP2 = 0;
int I1 = 0;
int I2 = 0;
void PIDcontrol() {
    // ここにPID
    currentcnt1 = enc1.getCount();
    currentcnt2 = enc2.getCount();
    int P1 = targetcnt - currentcnt1;
    int P2 = targetcnt2 - currentcnt2;
    I1 += P1/(1000/DT_MS);
    I2 += P2/(1000/DT_MS);
    int D1 = (P1 - PrevP1)/(1000/DT_MS);
    int D2 = (P2 - PrevP2)/(1000/DT_MS);
    PrevP1 = P1;
    PrevP2 = P2;
    int output1 = KP * P1 + KI * I1 + KD * D1;
    int output2 = KP * P2 + KI * I2 + KD * D2;
}

void setup() {
    Timer1.initialize(DT_MS * 1000); // ここに割り込みの周期を設定　1000000=1s
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
