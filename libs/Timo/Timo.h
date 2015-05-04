//
// Created by 陶源 on 15/5/4.
//

#ifndef TIMO_H
#define TIMO_H

#include <Arduino.h>

class Timo {
public:
    Timo() {
        _timeout_end = 0;
    }

    Timo(unsigned long ms) {
        delay(ms);
    }

    bool expired() {
        return (_timeout_end > 0L) && (millis() >= _timeout_end);
    }

    void reset() {
        _timeout_end = millis() + _timeout;
    }

    void delay(unsigned long ms) {
        _timeout = ms;
        reset();
    }

    void delay_sec(int seconds) {
        delay((unsigned long) seconds * 1000L);
    }

    unsigned long left() {
        return _timeout_end - millis();
    }

private:
    unsigned long _timeout_end;
    unsigned long _timeout;
};

#endif //TIMO_H
