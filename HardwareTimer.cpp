/*
 * HardwareTimer.cpp
 *
 *  Created on: 2015年1月24日
 *      Author: taoyuan
 */

#include "HardwareTimer.h"

void HardwareTimer::init(unsigned long us) {
    _callbacks.init();
    reload(us);
}


long HardwareTimer::reload(long us) {
    if (us > 0) {
        _callbacks.period(us);
    }
    _callbacks.reload();
}

void HardwareTimer::attachInterrupt(ISRCallback isr, void *params) {
    _isr = isr;        // register the user's callback with the real ISR
    _params = params;
    _callbacks.enable();    // sets the timer overflow interrupt enable bit
#ifdef __CC3200R1M1RGC__
#else
	sei();	
#endif				// ensures that interrupts are globally enabled
    start();
}

void HardwareTimer::enable() {
    _callbacks.enable();
}

void HardwareTimer::disable() {
    _callbacks.disable();    // clears the timer overflow interrupt enable bit
}

void HardwareTimer::start(long us) {
    if (us > 0) {
        reload(us);
    }
    _callbacks.start();
}

void HardwareTimer::stop() {
    _callbacks.stop();          // clears all clock selects bits
}


void HardwareTimer::isr() {
    if (_isr) {
        _isr(_params);
    }
}
