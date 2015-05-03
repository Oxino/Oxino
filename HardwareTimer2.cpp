#include "HardwareTimer.h"

#ifdef HAVE_HWTIMER2

#ifdef __CC3200R1M1RGC__

#include "utility/timer_if.h"
#include <driverlib/timer.h>
#include <driverlib/prcm.h>

#endif

byte _t2_csb;
long _t2_cyc;
int _t2_sca;

#ifdef __CC3200R1M1RGC__
unsigned long _t2_us;
#endif

#ifdef __CC3200R1M1RGC__

void __t2_timer_handler(void) {
	//
	// Clear the timer interrupt.
	//
	Timer_IF_InterruptClear(TIMERA2_BASE);

    Timer2.isr();
}

#else
// interrupt service routine that wraps a user defined function supplied by attachInterrupt
ISR(TIMER2_OVF_vect) {
	Timer2.isr();
}
#endif

void _t2_init() {
#ifdef __CC3200R1M1RGC__
//    MAP_IntEnable(FAULT_SYSTICK);
    Timer_IF_Init(PRCM_TIMERA2, TIMERA2_BASE, TIMER_CFG_PERIODIC, TIMER_BOTH, 0);
#else
		TCCR2A = _BV(WGM21);

#endif

//	TCCR2B = 0;
}

void _t2_start() ;

long _t2_period(long us) {
#ifdef __CC3200R1M1RGC__
    _t2_us = (unsigned long) us;
    Timer_IF_ReLoad(TIMERA2_BASE, TIMER_BOTH, _t2_us);
#else
		long cycles = us * (SYSCLOCK / 1000000.0);
		if (cycles < RESOLUTION_T8) {
		_t2_csb = _BV(CS20);              				// no prescale, full xtal
		_t2_sca = 1;
		} else if ((cycles >>= 3) < RESOLUTION_T8) {
		_t2_csb = _BV(CS21);              				// prescale by /8
		_t2_sca = 8;
		} else if ((cycles >>= 2) < RESOLUTION_T8) {
		_t2_csb = _BV(CS21) | _BV(CS20);  				// prescale by /32
		_t2_sca = 32;
		} else if ((cycles >>= 1) < RESOLUTION_T8) {
		_t2_csb = _BV(CS22);              				// prescale by /64
		_t2_sca = 64;
		} else if ((cycles >>= 1) < RESOLUTION_T8) {
		_t2_csb = _BV(CS22) | _BV(CS20);  				// prescale by /128
		_t2_sca = 128;
		} else if ((cycles >>= 1) < RESOLUTION_T8) {
		_t2_csb = _BV(CS22) | _BV(CS21);  				// prescale by /256
		_t2_sca = 256;
		} else if ((cycles >>= 2) < RESOLUTION_T8) {
		_t2_csb = _BV(CS22) | _BV(CS21) | _BV(CS20);  	// prescale by /1024
		_t2_sca = 1024;
		} else {
		_t2_csb = _BV(CS22) | _BV(CS21) | _BV(CS20);  // request was out of bounds, set as maximum
		_t2_sca = 1024;
		cycles = RESOLUTION_T8 - 1;
		}
		OCR2A = _t2_cyc = cycles;
		return (cycles * _t2_sca) * (1000000.0 / SYSCLOCK);
#endif
}

void _t2_enable() {
#ifdef __CC3200R1M1RGC__
    Timer_IF_IntSetup(TIMERA2_BASE, TIMER_BOTH, __t2_timer_handler);
#else
	TIMSK2 = _BV(OCIE2A);
#endif

}

void _t2_disable() {
#ifdef __CC3200R1M1RGC__
    Timer_IF_DeInit(TIMERA2_BASE, TIMER_BOTH);
#else
	TIMSK2 &= ~_BV(OCIE2A);
#endif


}

void _t2_start() {
#ifdef __CC3200R1M1RGC__
    Timer_IF_Start(TIMERA2_BASE, TIMER_BOTH, _t2_us);
#else
		TCCR2B |= _t2_csb;
#endif

}

void _t2_stop() {
#ifdef __CC3200R1M1RGC__
    Timer_IF_Stop(TIMERA2_BASE, TIMER_BOTH);
#else
		TCCR2B &= ~(_BV(CS20) | _BV(CS21) | _BV(CS22));          // clears all clock selects bits
#endif

}

void _t2_reload() {
#ifdef __CC3200R1M1RGC__
	// Restart the timer, from the beginning of a new period.
    Timer_IF_ReLoad(TIMERA2_BASE, TIMER_BOTH, _t2_us);
#else
		TCNT2 = 0;
#endif

}


hwt_callbacks _t2_callbacks = {_t2_init, _t2_period, _t2_enable, _t2_disable, _t2_start, _t2_stop, _t2_reload};

HardwareTimer Timer2(_t2_callbacks);

// interrupt service routine that wraps a user defined function supplied by attachInterrupt


#endif
