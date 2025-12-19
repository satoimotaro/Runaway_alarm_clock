#include "SerialComm.h"

void SerialComm::begin(int baud) {
    Serial.begin(baud);
}

void SerialComm::sendData() {
    Serial.print(up);
    Serial.print(",");
    Serial.print(down);
    Serial.print(",");
    Serial.print(left);
    Serial.print(",");
    Serial.println(right);
}

void SerialComm::readData() {
    if (Serial.available()) {
        String line = Serial.readStringUntil('\n');

        int comma1 = line.indexOf(',');
        int comma2 = line.indexOf(',', comma1 + 1);

        if (comma1 > 0 && comma2 > 0) {
            x = line.substring(0, comma1).toInt();
            y = line.substring(comma1 + 1, comma2).toInt();
            speed = line.substring(comma2 + 1).toInt();

            // デバッグ出力
            // Serial.print("x = ");
            // Serial.println(x);
            // Serial.print("y = ");
            // Serial.println(y);
            // Serial.print("speed = ");
            // Serial.println(speed);
        }
    }
}

void SerialComm::setUp(int value) {
    up = value;
}

void SerialComm::setDown(int value) {
    down = value;
}

void SerialComm::setLeft(int value) {
    left = value;
}

void SerialComm::setRight(int value) {
    right = value;
}

int SerialComm::getX() {
    return x;
}

int SerialComm::getY() {
    return y;
}

int SerialComm::getSpeed() {
    return speed;
}