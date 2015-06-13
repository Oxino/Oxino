/*
 * Led.h
 *
 * Ref: https://github.com/harrisonhjones/LEDEffect
 *
 */

#ifndef LED_H
#define LED_H

#include <Arduino.h>

#define USE_SPWM

#ifdef USE_SPWM
#include "SPWM.h"
#endif

//RGB Basic Colors
#define RGB_COLOR_RED       0xFF0000
#define RGB_COLOR_GREEN     0x00FF00
#define RGB_COLOR_BLUE      0x0000FF
#define RGB_COLOR_YELLOW    0xFFFF00
#define RGB_COLOR_CYAN      0x00FFFF
#define RGB_COLOR_MAGENTA   0xFF00FF
#define RGB_COLOR_WHITE     0xFFFFFF
#define RGB_COLOR_ORANGE    0xFF6000

#define DEFAULT_FADE_INTERVAL   10
#define DEFAULT_BLINK_INTERVAL  250

class BaseLED {
public:
    BaseLED(bool invert = false);

    void off();

    void on();

    void light(uint32_t color);

    void update();

    void breath(uint32_t color, int interval);

    void breath(uint32_t color);

    void fadeDown(uint32_t color, int interval);

    void fadeDown(uint32_t color);

    void fadeUp(uint32_t color, int interval);

    void fadeUp(uint32_t color);

    void blink(uint32_t color, unsigned long interval);

    void blink(uint32_t color);

    void brightness(byte value);

    void interval(int newInterval);

private:
    bool _invert;
    int _brightness;
    int _step;
    int _direction;
    byte _mode; // 0 = off, 1 = on, 2 = breath, 3 = fade down, 4 = fade up, 5 = blink
    unsigned long _interval; // in ms
    unsigned long _time;

    void decode(uint32_t color, byte *buf, int size);

    void setColor(uint32_t value);

protected:
    virtual int count() = 0;

    virtual byte *color() = 0;

    virtual byte *current() = 0;

    virtual void write(byte index, int value) = 0;

    uint8_t scale(uint8_t value);

    void scaleFade(uint8_t step, byte *target);
};

template<size_t NUM = 1>
class CustomLED : public BaseLED {
public:
    CustomLED(int *pins, bool invert = false) : BaseLED(invert) {
        setPins(pins);
    }

    CustomLED(bool invert = false) : BaseLED(invert) {
    }

private:
    int _pins[NUM];
    byte _color[NUM];
    byte _current[NUM];
protected:
    int count() {
        return NUM;
    };

    byte *color() {
        return _color;
    }

    byte *current() {
        return _current;
    }

    void write(byte index, int value) {
        if (index >= NUM) return;
        int pin = _pins[index];
        if (pin < 0) return;
        #ifdef USE_SPWM
        SPWM.write(pin, (uint8_t) value);
        #else
        analogWrite(pin, value);
        #endif
    }

    void setPins(int *pins) {
        memcpy(_pins, pins, NUM * sizeof(int));
        for (size_t i = 0; i < NUM; i++) {
            if (_pins[i] >= 0) pinMode((uint8_t) _pins[i], OUTPUT);
        }
        off();
        update(); // turns off current led
    }
};

typedef CustomLED<1> LED_1;
typedef CustomLED<3> LED_3;

class SingleLED : public LED_1 {
public:
    SingleLED(int pin, bool invert = false) : LED_1(invert) {
        int pins[] = {pin};
        setPins(pins);
    }

};

class RGBLED : public LED_3 {
public:
    RGBLED(int redPin, int greenPin, int bluePin, bool invert = false) : LED_3(invert) {
        int pins[] = {redPin, greenPin, bluePin};
        setPins(pins);
    }

};

#endif /* LED_H */
