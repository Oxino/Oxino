/*
 * HardwareTimer.h
 *
 *  Created on: 2015年1月24日
 *      Author: taoyuan
 */
#ifndef HARDWARETIMER_H_
#define HARDWARETIMER_H_

#ifdef ARDUINO

#include <Arduino.h>

#endif

#ifndef __CC3200R1M1RGC__
#include <avr/io.h>
#include <avr/interrupt.h>
#endif


#define RESOLUTION_T8     256
#define RESOLUTION_T16    65536

#ifndef SYSCLK
#ifdef F_CPU
#define SYSCLK F_CPU     // main cpu clock
#else
#define SYSCLOCK 16000000  // main Arduino clock
#endif // F_CPU
#endif // SYSCLK

struct hwt_callbacks {
    void (*init)();

    long (*period)(long us);

    void (*enable)();

    void (*disable)();

    void (*start)();

    void (*stop)();

    void (*reload)();

};


typedef void (*ISRCallback)(void *params);

class HardwareTimer {
public:
    HardwareTimer(hwt_callbacks &callbacks) : _callbacks(callbacks), _isr(0) {
    }

    void init(unsigned long us = 1000000);

    void start(long us = 0);

    void stop();

    void attachInterrupt(ISRCallback isr, void *params = NULL);

    void enable();

    void disable();

    long reload(long us = 0);

    void isr();

protected:
    hwt_callbacks &_callbacks;
    ISRCallback _isr;
    void *_params;
};

#if defined(TIMERA0A)
extern HardwareTimer Timer0;
#define HAVE_HWTIMER0
#endif


#if defined(TCCR1A) || defined(TIMERA1A)
extern HardwareTimer Timer1;
#define HAVE_HWTIMER1
#endif

#if defined(TCCR2A) || defined(TIMERA2A)
extern HardwareTimer Timer2;
#define HAVE_HWTIMER2
#endif

#if defined(TCCR3A) || defined(TIMERA3A)
extern HardwareTimer Timer3;
#define HAVE_HWTIMER3
#endif

#endif /* HARDWARETIMER_H_ */
