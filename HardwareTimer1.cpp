#include "HardwareTimer.h"

#ifdef HAVE_HWTIMER1

#ifdef __CC3200R1M1RGC__

#include "utility/timer_if.h"
#include <driverlib/timer.h>
#include <driverlib/prcm.h>

#endif

byte _t1_csb;
long _t1_cyc;
int _t1_sca;

#ifdef __CC3200R1M1RGC__
unsigned long _t1_us;
#endif

#ifdef __CC3200R1M1RGC__

void __t1_timer_handler(void) {
    //
    // Clear the timer interrupt.
    //
    Timer_IF_InterruptClear(TIMERA1_BASE);

    Timer1.isr();
}

#else
// interrupt service routine that wraps a user defined function supplied by attachInterrupt
ISR(TIMER1_OVF_vect) {
	Timer1.isr();
}
#endif

void _t1_init() {
#ifdef __CC3200R1M1RGC__
//    MAP_IntEnable(FAULT_SYSTICK);
	Timer_IF_Init(PRCM_TIMERA1, TIMERA1_BASE, TIMER_CFG_PERIODIC, TIMER_BOTH, 0);
#else
	TCCR1A = 0;                 // clear control register A
	TCCR1B = _BV(WGM13);        // set mode as phase and frequency correct pwm, stop the timer

#endif

}

void _t1_start() ;

long _t1_period(long us) {
#ifdef __CC3200R1M1RGC__
	_t1_us = (unsigned long) us;
    Timer_IF_ReLoad(TIMERA1_BASE, TIMER_BOTH, _t1_us);
#else
		long cycles = us * (SYSCLOCK / 2000000.0); // the counter runs backwards after TOP, interrupt is at BOTTOM so divide us by 2
		if (cycles < RESOLUTION_T16) {					// no prescale, full xtal
		_t1_sca = 1;
		_t1_csb = _BV(CS10);
		} else if ((cycles >>= 3) < RESOLUTION_T16) {	// prescale by /8
		_t1_sca = 8;
		_t1_csb = _BV(CS11);
		} else if ((cycles >>= 3) < RESOLUTION_T16) {	// prescale by /64
		_t1_sca = 64;
		_t1_csb = _BV(CS11) | _BV(CS10);
		} else if ((cycles >>= 2) < RESOLUTION_T16) {	// prescale by /256
		_t1_sca = 256;
		_t1_csb = _BV(CS12);
		} else if ((cycles >>= 2) < RESOLUTION_T16) {	// prescale by /1024
		_t1_sca = 1024;
		_t1_csb = _BV(CS12) | _BV(CS10);
		} else {
		_t1_sca = 1024;
		_t1_csb = _BV(CS12) | _BV(CS10);  // request was out of bounds, set as maximum
		cycles = RESOLUTION_T16 - 1;
		}

		ICR1 = _t1_cyc = cycles;
		return (cycles * _t1_sca) * (2000000.0 / SYSCLOCK);
#endif
}

void _t1_enable() {
#ifdef __CC3200R1M1RGC__
    Timer_IF_IntSetup(TIMERA1_BASE, TIMER_BOTH, __t1_timer_handler);
#else
		TIMSK1 = _BV(TOIE1);
#endif

}

void _t1_disable() {
#ifdef __CC3200R1M1RGC__
    Timer_IF_DeInit(TIMERA1_BASE, TIMER_BOTH);
#else
		TIMSK1 &= ~_BV(TOIE1);
#endif

}

void _t1_start() {
#ifdef __CC3200R1M1RGC__
	Timer_IF_Start(TIMERA1_BASE, TIMER_BOTH, _t1_us);
#else
		TCCR1B |= _t1_csb;
#endif


}

void _t1_stop() {
#ifdef __CC3200R1M1RGC__
    Timer_IF_Stop(TIMERA1_BASE, TIMER_BOTH);
#else
		TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
#endif
    //          // clears all clock selects bits
}

void _t1_reload() {
#ifdef __CC3200R1M1RGC__
	// Restart the timer, from the beginning of a new period.
    Timer_IF_ReLoad(TIMERA1_BASE, TIMER_BOTH, _t1_us);
#else
		TCNT1 = 0;
#endif

}

hwt_callbacks _t1_callbacks = {_t1_init, _t1_period, _t1_enable, _t1_disable, _t1_start, _t1_stop, _t1_reload};

HardwareTimer Timer1(_t1_callbacks);

#endif
