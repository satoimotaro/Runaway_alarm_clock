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
Encoder    enc2{ 4, 12 };

//Pin definitions
#define MOTOR1_PIN1 5
#define MOTOR1_PIN2 6
#define MOTOR1_PWM   11
#define MOTOR2_PIN1 9
#define MOTOR2_PIN2 10
#define MOTOR2_PWM   4

//PID parameters
#define KP 2.0
#define KI 0.0
#define KD 0.0
#define DT_us 100000

int x = 0;
int y = 0;
int speed = 200;
int distance = 0;

// PID制御用変数
int targetcnt1 = 0;
int targetcnt2 = 0;
int currentcnt1 = 0;
int currentcnt2 = 0;
int PrevP1 = 0;
int PrevP2 = 0;
int I1 = 0;
int I2 = 0;
void PIDcontrol() {
    // ここにPID
    Serial.print(currentcnt1);
    Serial.print(","); 
    Serial.println(currentcnt2);
    currentcnt1 = enc1.getCount();
    // currentcnt2 = enc2.getCount();
    int P1 = targetcnt1 - currentcnt1;
    int P2 = targetcnt2 - currentcnt2;
    I1 += P1*(DT_us/100000);
    I2 += P2*(DT_us/100000);
    int D1 = (P1 - PrevP1)*(100000/DT_us);
    int D2 = (P2 - PrevP2)*(100000/DT_us);
    PrevP1 = P1;
    PrevP2 = P2;
    int output1 = KP * P1 + KI * I1 + KD * D1;
    int output2 = KP * P2 + KI * I2 + KD * D2;
    // モーター制御
    if (output1 >= 0) {
        analogWrite(MOTOR1_PWM, min(output1, speed));
        digitalWrite(MOTOR1_PIN1, HIGH);
        digitalWrite(MOTOR1_PIN2, LOW);
    } else {
        analogWrite(MOTOR1_PWM, min(-output1, speed));
        digitalWrite(MOTOR1_PIN1, LOW);
        digitalWrite(MOTOR1_PIN2, HIGH);
    }
    if (output2 >= 0) {
        analogWrite(MOTOR2_PWM, min(output2, speed));
        digitalWrite(MOTOR2_PIN1, HIGH);
        digitalWrite(MOTOR2_PIN2, LOW);
    } else {
        analogWrite(MOTOR2_PWM, min(-output2, speed));
        digitalWrite(MOTOR2_PIN1, LOW);
        digitalWrite(MOTOR2_PIN2, HIGH);
    }
}

void setup() {
    Timer1.initialize(DT_us); // ここに割り込みの周期を設定　1000000=1s
    Timer1.attachInterrupt(PIDcontrol);

    // comm.begin(9600);

    // senU.begin();
    // senD.begin();
    // senL.begin();
    // senR.begin();
    enc1.begin();
    // enc2.begin();

    Serial.begin(9600);
    targetcnt1 = 500;
    targetcnt2 = 100;
}

void loop() {
//     senU.read();
//     senD.read();
//     senL.read();
//     senR.read();
//     comm.setUp(senU.getDistance());
//     comm.setUp(senD.getDistance());
//     comm.setUp(senL.getDistance());
//     comm.setUp(senR.getDistance());
//     comm.sendData();

    // comm.readData();
    // x = comm.getX();
    // y = comm.getY();
    // speed = comm.getSpeed();

    delay(100);
}
