/*
 * AclButton.h
 *
 *	Ref: https://github.com/JChristensen/Button
 *
 *  Created on: 2014年12月28日
 *      Author: taoyuan
 */

#ifndef BUTTON_H
#define BUTTON_H

#include "Arduino.h"

class Button {
public:
	Button() {}
	Button(uint8_t pin, bool invert = false, bool internalPullup = false);
	void begin(uint8_t pin, bool invert = false, bool internalPullup = false);
	int read();
	int update();
	void reset();
	bool isPressed();
	bool isReleased();
	bool wasPressed();
	bool wasReleased();
	bool pressedFor(uint32_t ms);
	bool releasedFor(uint32_t ms);
	uint32_t lastChange();
	uint32_t pressed();
	uint8_t clicks();

	uint32_t debounceTime;       //debounce time
	uint32_t clickTime;
	uint32_t pressTime;
private:
	uint8_t _pin;           //arduino pin number
	bool _internalPullup;      //internal pullup resistor enabled
	bool _invert; //if 0, interpret high state as pressed, else interpret low state as pressed
	int _state;         //current button state
	int _lastState;     //previous button state
	bool _changed;       //state changed since last read
	uint32_t _time;         //time of current state (all times are in ms)
	uint32_t _lastTime;     //time of previous state
	uint32_t _lastChange;   //time of last state change

	uint32_t _pressed;
	uint8_t _clickCount;
	uint8_t _clicks;
};

#if defined(BTN_BUILTIN)
extern Button BTN;
#endif

#endif /* BUTTON_H */
