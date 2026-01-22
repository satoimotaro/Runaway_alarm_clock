#include "Encoder.h"

// staticメンバの実体
Encoder* Encoder::_instance0 = nullptr;
Encoder* Encoder::_instance1 = nullptr;

Encoder::Encoder(uint8_t pinA, uint8_t pinB) : _pinA(pinA), _pinB(pinB), _count(0) {}

void Encoder::begin() {
    pinMode(_pinA, INPUT_PULLUP);
    pinMode(_pinB, INPUT_PULLUP);

    // 割り込み番号で振り分け
    uint8_t intNum = digitalPinToInterrupt(_pinA);

    if (intNum == 0) { // INT0 (D2)
        _instance0 = this;
        attachInterrupt(intNum, Encoder::isr0, CHANGE);
    } else if (intNum == 1) { // INT1 (D3)
        _instance1 = this;
        attachInterrupt(intNum, Encoder::isr1, CHANGE);
    }
}

void Encoder::handleInterrupt() {
  _count += 2 * (digitalRead(_pinA) ^ digitalRead(_pinB)) - 1;
}

long Encoder::getCount() {
    noInterrupts();
    long c = _count;
    interrupts();
    return c;
}

void Encoder::reset() {
    noInterrupts();
    _count = 0;
    interrupts();
}

// ===== static ISR =====
void Encoder::isr0() {
    if (_instance0) {
        _instance0->handleInterrupt();
    }
}

void Encoder::isr1() {
    if (_instance1) {
        _instance1->handleInterrupt();
    }
}
