#include "Encoder.h"
#include "SerialComm.h"
#include <Arduino.h>
#include <TimerOne.h>

SerialComm comm;
Encoder    enc1{ 2, 4 };
Encoder    enc2{ 3, 9 };


//Pin definitions
#define MOTOR1_PIN1 A0
#define MOTOR1_PIN2 A1
#define MOTOR1_PWM  5
#define MOTOR2_PIN1 A2
#define MOTOR2_PIN2 A3
#define MOTOR2_PWM   6
#define Switch_PIN  A7

//PID parameters
#define KP 1.5
#define KI 0.1
#define KD 0.3
#define DT_us 2000 // 200Hz
#define CONVERJENCE_THRESHOLD 10

#define mm_cnt 8.1

int currentx = 0;
int currenty = 0;
int targetx = 0;
int targety = 0;
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
int D_term1 = 0;
int D_term2 = 0;
void PIDcontrol() {
    if(!comm.getMoveFlag()){
        analogWrite(MOTOR1_PWM, 0);
        analogWrite(MOTOR2_PWM, 0);
        return;
    }
    // ここにPID
    currentcnt1 = enc1.getCount();
    currentcnt2 = enc2.getCount();
    currentx = (currentcnt1 + currentcnt2) / (2 * mm_cnt);
    currenty = (currentcnt2 - currentcnt1) / (2 * mm_cnt);
    int P1 = targetcnt1 - currentcnt1;
    int P2 = targetcnt2 - currentcnt2;
    float alpha = 0.2;
    alpha = alpha/(1 + alpha);
    I1 += P1*(DT_us/100000);
    I2 += P2*(DT_us/100000);
    int D1 = (P1 - PrevP1)*(100000/DT_us);
    int D2 = (P2 - PrevP2)*(100000/DT_us);
    D_term1 = (1 - alpha)*D_term1 + alpha*D1;
    D_term2 = (1 - alpha)*D_term2 + alpha*D2;
    PrevP1 = P1;
    PrevP2 = P2;
    int output1 = KP * P1 + KI * I1 + KD * D_term1;
    int output2 = KP * P2 + KI * I2 + KD * D_term2;
    if(abs(P1) < CONVERJENCE_THRESHOLD && abs(P2) < CONVERJENCE_THRESHOLD){
        // 到達判定
        output1 = 0;
        output2 = 0;
    }
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

void moveXY(int tarx, int tary) {
    // ここにXY移動のためのモーター制御
    if(targetx == tarx && targety == tary)return;
    targetcnt1 = (int)((tarx - tary) * mm_cnt);
    targetcnt2 = (int)((tarx + tary) * mm_cnt);
    targetx = tarx;
    targety = tary;
    PrevP1 = 0;
    PrevP2 = 0;
    I1 = 0;
    I2 = 0;
}

void setup() {
    Timer1.initialize(DT_us); // ここに割り込みの周期を設定　1000000=1s
    Timer1.attachInterrupt(PIDcontrol);

    comm.begin(115200);

    enc1.begin();
    enc2.begin();

    pinMode(MOTOR1_PIN1, OUTPUT);
    pinMode(MOTOR1_PIN2, OUTPUT);
    pinMode(MOTOR1_PWM, OUTPUT);
    pinMode(MOTOR2_PIN1, OUTPUT);
    pinMode(MOTOR2_PIN2, OUTPUT);
    pinMode(MOTOR2_PWM, OUTPUT);
    pinMode(Switch_PIN, INPUT);

    targetcnt1 = 0;
    targetcnt2 = 0;

    comm.setMoveFlag(true);

    speed = 100;
    moveXY(50,50);
    delay(2000);
    enc1.reset();
    enc2.reset();
    moveXY(-50,-50);
    delay(2000);
    enc1.reset();
    enc2.reset();
}

void loop() {
    if(comm.getMoveFlag() == true){
        comm.readData();
        comm.readFromSoftwareSerial();
        speed = comm.getSpeed();
        moveXY(comm.getX(), comm.getY());
        comm.setCurrentPosition(currentx, currenty);
        comm.sendData();
    }
    if(analogRead(Switch_PIN) < 300 && comm.getMoveFlag() == true){
        // リセット処理
        comm.setMoveFlag(false);
        Serial.println("Button Pressed! Stopping...");
    }
    delay(20);
}
