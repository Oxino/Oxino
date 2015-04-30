/*
 * AclButton.cpp
 *
 *  Created on: 2014年12月28日
 *      Author: taoyuan
 */

#include "Button.h"

#if defined(BTN_BUILTIN)
Button BTN(BTN_BUILTIN);
#endif

/*------------------------------------------------------------------------------*
 * Button(pin, puEnable, invert) instantiates a button object.  		*
 * pin      		Is the Arduino pin the button is connected to.             	*
 * invert   		If invert == 0, interprets a high state as pressed, low as  *
 *          		released. If invert != 0, interprets a high state as        *
 *          		released, low as pressed  (can also use true or false).     *
 * internalPullup 	Enables the AVR internal pullup resistor if != 0 (can also  *
 *          		use true or false).  						           		*
 *                                                                      		*
 * (Note that invert cannot be implied from puEnable since an external  		*
 *  pullup could be used.)                                              		*
 *--------------------------------------------------------------------------*/
Button::Button(uint8_t pin, bool invert, bool internalPullup) {
	_pin = pin;
	_internalPullup = internalPullup;
	_invert = invert;
	pinMode(_pin, INPUT);
	if (_internalPullup != 0)
		digitalWrite(_pin, HIGH);       //enable pullup resistor
	_state = digitalRead(_pin);
	if (_invert != 0)
		_state = !_state;
	_time = millis();
	_lastState = _state;
	_changed = 0;
	_lastTime = _time;
	_lastChange = _time;
	_clicks = 0;
	_clickCount = 0;
	_pressed = 0;

	debounceTime = 20;
	clickTime = 250;
	pressTime = 1000;
}

/*----------------------------------------------------------------------*
 * read() returns the state of the button, 1==pressed, 0==released,     *
 * does debouncing, captures and maintains times, previous states, etc. *
 *----------------------------------------------------------------------*/
uint8_t Button::read() {
	return update();
}

/*----------------------------------------------------------------------*
 * read() returns the state of the button, 1==pressed, 0==released,     *
 * does debouncing, captures and maintains times, previous states, etc. *
 *----------------------------------------------------------------------*/
uint8_t Button::update() {
	static uint32_t now;
	static int curr;

	now = millis();
	curr = digitalRead(_pin);
	if (_invert != 0)
		curr = !curr;

	_lastTime = _time;
	_time = now;
	_changed = 0;

	if (now - _lastChange > debounceTime) {
		_lastState = _state;
		_state = curr;
		_changed = _state != _lastState;
		if (_changed) {
			_lastChange = now;
		}
	}

	// debounce the button (Check if a stable, changed state has occured)
	if (_changed && _state) {
		_pressed = 0;
		_clickCount++;
	}

	// If the button released state is stable, report nr of clicks and start new cycle
	if (!_state && (now - _lastChange) > clickTime) {
		// positive count for released buttons
		_clicks = _clickCount;
		_clickCount = 0;
	}

	// clear click count if long press
	if (_state && (now - _lastChange) > pressTime) {
		_clicks = 0;
		_clickCount = 0;
	}

	if (_state) {
		_pressed += (_time - _lastTime);
	}

	return _state;

}

void Button::reset() {
	_pressed = 0;
	_clicks = 0;
	_clickCount = 0;
}

/*----------------------------------------------------------------------*
 * isPressed() and isReleased() check the button state when it was last *
 * read, and return false (0) or true (!=0) accordingly.                *
 * These functions do not cause the button to be read.                  *
 *----------------------------------------------------------------------*/
uint8_t Button::isPressed() {
	return _state == 0 ? 0 : 1;
}

uint8_t Button::isReleased() {
	return _state == 0 ? 1 : 0;
}

/*----------------------------------------------------------------------*
 * wasPressed() and wasReleased() check the button state to see if it   *
 * changed between the last two reads and return false (0) or           *
 * true (!=0) accordingly.                                              *
 * These functions do not cause the button to be read.                  *
 *----------------------------------------------------------------------*/
uint8_t Button::wasPressed() {
	return _state && _changed;
}

uint8_t Button::wasReleased() {
	return !_state && _changed;
}

/*----------------------------------------------------------------------*
 * pressedFor(ms) and releasedFor(ms) check to see if the button is     *
 * pressed (or released), and has been in that state for the specified  *
 * time in milliseconds. Returns false (0) or true (1) accordingly.     *
 * These functions do not cause the button to be read.                  *
 *----------------------------------------------------------------------*/
uint8_t Button::pressedFor(uint32_t ms) {
	return (_state == 1 && _time - _lastChange >= ms) ? 1 : 0;
}

uint8_t Button::releasedFor(uint32_t ms) {
	return (_state == 0 && _time - _lastChange >= ms) ? 1 : 0;
}

/*----------------------------------------------------------------------*
 * lastChange() returns the time the button last changed state,         *
 * in milliseconds.                                                     *
 *----------------------------------------------------------------------*/
uint32_t Button::lastChange() {
	return _lastChange;
}

uint8_t Button::clicks() {
	return _clicks;
}

uint32_t Button::pressed() {
	return _pressed;
}
