#include "HardwareTimer.h"

#ifdef HAVE_HWTIMER0

#ifdef __CC3200R1M1RGC__

#include "utility/timer_if.h"
#include <driverlib/timer.h>
#include <driverlib/prcm.h>

#endif

byte _t0_csb;
long _t0_cyc;
int _t0_sca;

#ifdef __CC3200R1M1RGC__
unsigned long _t0_us;
#endif

#ifdef __CC3200R1M1RGC__

void __t0_timer_handler(void) {
    //
    // Clear the timer interrupt.
    //
    Timer_IF_InterruptClear(TIMERA0_BASE);

    Timer0.isr();
}

#else
// interrupt service routine that wraps a user defined function supplied by attachInterrupt
ISR(TIMER0_OVF_vect) {
	Timer0.isr();
}
#endif

void _t0_init() {
#ifdef __CC3200R1M1RGC__
//    MAP_IntEnable(FAULT_SYSTICK);
    Timer_IF_Init(PRCM_TIMERA0, TIMERA0_BASE, TIMER_CFG_PERIODIC, TIMER_BOTH, 0);
#endif

}

void _t0_start() ;

long _t0_period(long us) {
#ifdef __CC3200R1M1RGC__
    _t0_us = (unsigned long) us;
    Timer_IF_ReLoad(TIMERA0_BASE, TIMER_BOTH, _t0_us);
#endif
}

void _t0_enable() {
#ifdef __CC3200R1M1RGC__
    Timer_IF_IntSetup(TIMERA0_BASE, TIMER_BOTH, __t0_timer_handler);
#endif

}

void _t0_disable() {
#ifdef __CC3200R1M1RGC__
    Timer_IF_DeInit(TIMERA0_BASE, TIMER_BOTH);
#endif

}

void _t0_start() {
#ifdef __CC3200R1M1RGC__
    Timer_IF_Start(TIMERA0_BASE, TIMER_BOTH, _t0_us);
#endif


}

void _t0_stop() {
#ifdef __CC3200R1M1RGC__
    Timer_IF_Stop(TIMERA0_BASE, TIMER_BOTH);
#endif
    //          // clears all clock selects bits
}

void _t0_reload() {
#ifdef __CC3200R1M1RGC__
    // Restart the timer, from the beginning of a new period.
    Timer_IF_ReLoad(TIMERA0_BASE, TIMER_BOTH, _t0_us);
#endif
}

hwt_callbacks _t0_callbacks = {_t0_init, _t0_period, _t0_enable, _t0_disable, _t0_start, _t0_stop, _t0_reload};

HardwareTimer Timer0(_t0_callbacks);

#endif
