/*
 * Led.h
 *
 * Ref: https://github.com/harrisonhjones/LEDEffect
 *
 */

#ifndef LED_H
#define LED_H

#include <Arduino.h>

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
    CustomLED(byte *pins, bool invert = false) : BaseLED(invert) {
        setPins(pins);
    }

    CustomLED(bool invert = false) :
            BaseLED(invert) {
    }

private:
    byte _pins[NUM];
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
        uint8_t pin = _pins[index];
        uint8_t timer = digitalPinToTimer(pin);
        if (timer == NOT_ON_TIMER) {
            digitalWrite(pin, value > 0 ? HIGH : LOW);
        } else {
            analogWrite(pin, value);
        }
    }

    void setPins(byte *pins) {
        memcpy(_pins, pins, NUM);
        for (size_t i = 0; i < NUM; i++) {
            pinMode(_pins[i], OUTPUT);
        }
        off();
        update(); // turns off current led
    }
};

typedef CustomLED<1> LED_1;
typedef CustomLED<3> LED_3;

class SolidLED : public LED_1 {
public:
    SolidLED(byte pin, bool invert = false) : LED_1(invert) {
        byte pins[] = {pin};
        setPins(pins);
    }

};

class RGBLED : public LED_3 {
public:
    RGBLED(byte redPin, byte greenPin, byte bluePin, bool invert = false) : LED_3(invert) {
        byte pins[] = {redPin, greenPin, bluePin};
        setPins(pins);
    }

};

#if !defined(LED_ENABLED) && defined(LED_BUILTIN)
#define LED_ENABLED
extern SolidLED LED;
#endif

#if !defined(RGB_ENABLED) && defined(RGB_BUILTIN_R) && defined(RGB_BUILTIN_G) && defined(RGB_BUILTIN_B)
#define RGB_ENABLED
extern RGBLED RGB;
#endif

#endif /* LED_H */
