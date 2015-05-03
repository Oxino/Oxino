/*
 * leds.cpp
 *
 *  Created on: 2014年12月27日
 *      Author: taoyuan
 */

#include "Led.h"

#define NUM_LED_FADE_STEPS	100

#if defined(LED_BUILTIN)
SolidLED LED(LED_BUILTIN);
#endif

#if defined(RGB_BUILTIN_R) && defined(RGB_BUILTIN_G) && defined(RGB_BUILTIN_B)
RGBLED RGB(RGB_BUILTIN_R, RGB_BUILTIN_G, RGB_BUILTIN_B, true);
#endif

BaseLED::BaseLED(bool invert) {
	_invert = invert;
	_time = millis();
	_brightness = 255;
	_step = 0;
	_direction = 0;
	_mode = 0; // 0 = off, 1 = on, 2 = breath, 3 = fade down, 4 = fade up, 5 = blink
	_interval = 30; // in ms
}

void BaseLED::update() {
	byte *current = this->current();
	if ((millis() - _time) > _interval) {
		_time = millis();
		byte *c = this->color();
		int num = this->count();
		if (_mode == 0) {
			memset(current, 0, num);
		} else if (_mode == 1) {
			memset(current, 255, num);
		} else if (_mode == 2) {
			/* Update position in fade. */
			if (_step == 0)
				_direction = 1; /* Switch to fade growing. */
			else if (_step == NUM_LED_FADE_STEPS - 1)
				_direction = -1; /* Switch to fade falling. */

			_step += _direction;

			scaleFade(_step, current);
		} else if (_mode == 3) { // Fade down
			/* Update position in fade. */
			if (_step == 0)
				_step = NUM_LED_FADE_STEPS - 1; /* Switch to fade growing. */

			scaleFade(_step--, current);

		} else if (_mode == 4) { // Fade up
			/* Update position in fade. */
			if (_step == NUM_LED_FADE_STEPS - 1)
				_step = 0; /* Switch to fade falling. */

			scaleFade(_step++, current);
		} else if (_mode == 5) {
			if (_direction > 0) {
				_direction = -1;
				memcpy(current, c, num);
			} else {
				_direction = 1;
				memset(current, 0, num);
			}
		} else if (_mode == 6) {
		}
	}

	for (int i = 0; i < count(); i++) {
		if (_invert) {
			write(i, 255 - scale(current[i]));
		} else {
			write(i, scale(current[i]));
		}
	}
}

void BaseLED::off() {
	_mode = 0;
	_interval = 10;
}

void BaseLED::on() {
	_mode = 1;
	_interval = 10;
}

void BaseLED::breath(uint32_t color, int interval) {
	setColor(color);
	_mode = 2;
	_interval = interval;
}

void BaseLED::breath(uint32_t color) {
	breath(color, DEFAULT_FADE_INTERVAL);
}

void BaseLED::fadeDown(uint32_t color, int interval) {
	setColor(color);
	_mode = 3;
	_interval = interval;
}

void BaseLED::fadeDown(uint32_t color) {
	fadeDown(color, DEFAULT_FADE_INTERVAL);
}

void BaseLED::fadeUp(uint32_t color, int interval) {
	setColor(color);
	_mode = 4;
	_interval = interval;
}

void BaseLED::fadeUp(uint32_t color) {
	fadeUp(color, DEFAULT_FADE_INTERVAL);
}

void BaseLED::blink(uint32_t color, unsigned long interval) {
	setColor(color);
	_mode = 5;
	_interval = interval;
}

void BaseLED::blink(uint32_t color) {
	blink(color, DEFAULT_BLINK_INTERVAL);
}

void BaseLED::light(uint32_t color) {
	setColor(color);
	memcpy(this->current(), this->color(), this->count());
	_mode = 6;
	_interval = 1000;   // Not really required.
}

void BaseLED::brightness(byte value) {
	_brightness = value;
}

void BaseLED::interval(int newInterval) {
	_interval = newInterval;
}

uint8_t BaseLED::scale(uint8_t value) {
	return (uint8_t) ((((uint16_t) (value)) * _brightness / 255));
}

void BaseLED::setColor(uint32_t value) {
	decode(value, this->color(), this->count());
}

void BaseLED::decode(uint32_t color, byte *buf, int size) {
	int len = sizeof(color);
	int num = min(size, len);
	for (int i = 0; i < size; i++) {
		if (i >= len) {
			buf[i] = 0;
		} else {
			buf[i] = (byte) (color >> ((num - i - 1) * 8));
		}
	}
}

void BaseLED::scaleFade(uint8_t step, byte *target) {
	byte *color = this->color();
	for (int i = 0; i < count(); i++) {
		target[i] = (byte) ((((uint32_t) color[i]) * step)
				/ (NUM_LED_FADE_STEPS - 1));
	}
}
