#include "SerialComm.h"

void SerialComm::begin(uint32_t baud) {
    Serial.begin(baud);
    mySerial.begin(baud);
    currentx = 0;
    currenty = 0;
    sensor1 = 99;
    sensor2 = 99;
    sensor3 = 99;
    sensor4 = 99;
    move_flag = false;
}

void SerialComm::sendData() {
    Serial.print(currentx);
    Serial.print(",");
    Serial.print(currenty);
    Serial.print(",");
    Serial.print(sensor1);
    Serial.print(",");
    Serial.print(sensor2);
    Serial.print(",");
    Serial.print(sensor3);
    Serial.print(",");
    Serial.print(sensor4);
    Serial.print(",");
    Serial.println(move_flag ? 1 : 0);
}

void SerialComm::readData() {
    if (Serial.available()) {
        String line = Serial.readStringUntil('\n');

        int comma1 = line.indexOf(',');
        int comma2 = line.indexOf(',', comma1 + 1);

        if (comma1 > 0 && comma2 > 0) {
            targetx = line.substring(0, comma1).toInt();
            targety = line.substring(comma1 + 1, comma2).toInt();
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

void SerialComm::readFromSoftwareSerial() {
    if (mySerial.available()) {
        String line = mySerial.readStringUntil('\n');

        int comma1 = line.indexOf(',');
        int comma2 = line.indexOf(',', comma1 + 1);
        int comma3 = line.indexOf(',', comma2 + 1);

        if (comma1 > 0 && comma2 > 0) {
            sensor1 = line.substring(0, comma1).toInt();
            sensor2 = line.substring(comma1 + 1, comma2).toInt();
            sensor3 = line.substring(comma2 + 1, comma3).toInt();
            sensor4 = line.substring(comma3 + 1).toInt();
        }
    }
}

void SerialComm::setMoveFlag(bool flag) {
    move_flag = flag;
}

void SerialComm::setCurrentPosition(int x, int y) {
    currentx = x;
    currenty = y;
}

int SerialComm::getX() {
    return targetx;
}

int SerialComm::getY() {
    return targety;
}

int SerialComm::getSpeed() {
    return speed;
}

bool SerialComm::getMoveFlag() {
    return move_flag;
}