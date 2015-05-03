#include "HardwareTimer.h"

#ifdef HAVE_HWTIMER3

#ifdef __CC3200R1M1RGC__

#include "utility/timer_if.h"
#include <driverlib/timer.h>
#include <driverlib/prcm.h>

#endif

byte _t3_csb;
long _t3_cyc;
int _t3_sca;

#ifdef __CC3200R1M1RGC__
unsigned long _t3_us;
#endif

#ifdef __CC3200R1M1RGC__

void __t3_timer_handler(void) {
	//
	// Clear the timer interrupt.
	//
	Timer_IF_InterruptClear(TIMERA3_BASE);

    Timer3.isr();
}

#else
// interrupt service routine that wraps a user defined function supplied by attachInterrupt
ISR(TIMER3_OVF_vect) {
	Timer3.isr();
}
#endif

void _t3_init() {
#ifdef __CC3200R1M1RGC__
//    MAP_IntEnable(FAULT_SYSTICK);
	Timer_IF_Init(PRCM_TIMERA3, TIMERA3_BASE, TIMER_CFG_PERIODIC, TIMER_BOTH, 0);
#else
			TCCR3A = 0;                 // clear control register A
			TCCR3B = _BV(WGM33);        // set mode as phase and frequency correct pwm, stop the timer
	
#endif


}

void _t3_start() ;

long _t3_period(long us) {
#ifdef __CC3200R1M1RGC__
	_t3_us = (unsigned long) us;
	Timer_IF_ReLoad(TIMERA3_BASE, TIMER_BOTH, _t3_us);
#else
		long cycles = us * (SYSCLOCK / 2000000.0); // the counter runs backwards after TOP, interrupt is at BOTTOM so divide us by 2
		if (cycles < RESOLUTION_T16) {					// no prescale, full xtal
		_t3_sca = 1;
		_t3_csb = _BV(CS10);
		} else if ((cycles >>= 3) < RESOLUTION_T16) {	// prescale by /8
		_t3_sca = 8;
		_t3_csb = _BV(CS11);
		} else if ((cycles >>= 3) < RESOLUTION_T16) {	// prescale by /64
		_t3_sca = 64;
		_t3_csb = _BV(CS11) | _BV(CS10);
		} else if ((cycles >>= 2) < RESOLUTION_T16) {	// prescale by /256
		_t3_sca = 256;
		_t3_csb = _BV(CS12);
		} else if ((cycles >>= 2) < RESOLUTION_T16) {	// prescale by /1024
		_t3_sca = 1024;
		_t3_csb = _BV(CS12) | _BV(CS10);
		} else {
		_t3_sca = 1024;
		_t3_csb = _BV(CS12) | _BV(CS10);  // request was out of bounds, set as maximum
		cycles = RESOLUTION_T16 - 1;
		}

		ICR3 = _t3_cyc = cycles;
		return (cycles * _t3_sca) * (2000000.0 / SYSCLOCK);
#endif
}

void _t3_enable() {

#ifdef __CC3200R1M1RGC__
	Timer_IF_IntSetup(TIMERA3_BASE, TIMER_BOTH, __t3_timer_handler);

#else
			TIMSK3 = _BV(TOIE3);
#endif

}

void _t3_disable() {
#ifdef __CC3200R1M1RGC__
	Timer_IF_DeInit(TIMERA3_BASE, TIMER_BOTH);
#else
		TIMSK3 &= ~_BV(TOIE3);
#endif

}

void _t3_start() {
#ifdef __CC3200R1M1RGC__
	Timer_IF_Start(TIMERA3_BASE, TIMER_BOTH, _t3_us);
#else
		TCCR3B |= _t3_csb;
#endif


}

void _t3_stop() {
#ifdef __CC3200R1M1RGC__
	Timer_IF_Stop(TIMERA3_BASE, TIMER_BOTH);
#else
		TCCR3B &= ~(_BV(CS30) | _BV(CS31) | _BV(CS32));          // clears all clock selects bits
#endif

}

void _t3_reload() {
#ifdef __CC3200R1M1RGC__
	Timer_IF_ReLoad(TIMERA3_BASE, TIMER_BOTH, _t3_us);
#else
		TCNT3 = 0;
#endif

}


hwt_callbacks _t3_callbacks = {_t3_init, _t3_period, _t3_enable, _t3_disable, _t3_start, _t3_stop, _t3_reload};

HardwareTimer Timer3(_t3_callbacks);


#endif
