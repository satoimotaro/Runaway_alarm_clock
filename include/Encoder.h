#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

class Encoder {
public:
    Encoder(uint8_t pinA, uint8_t pinB);

    void begin();
    long getCount();
    void reset();

    // ISRから呼ばれる関数
    void handleInterrupt();

    // 割り込み登録用（static）
    static void isr0();
    static void isr1();

private:
    uint8_t       _pinA;
    uint8_t       _pinB;
    volatile long _count;

    // ISRから参照するインスタンス
    static Encoder* _instance0;
    static Encoder* _instance1;
};

#endif
