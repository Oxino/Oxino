#include "HardwareTimer.h"

#ifdef HAVE_HWTIMER0

#ifdef __CC3200R1M1RGC__

#include <driverlib/timer.h>
#include <inc/hw_ints.h>
#include <driverlib/prcm.h>

#endif

byte _t0_csb;
long _t0_cyc;
int _t0_sca;

#ifdef __CC3200R1M1RGC__
unsigned long _t0_ticks;
#endif

#ifdef __CC3200R1M1RGC__

void __t0_timer_handler(void) {
    unsigned long ulInts;
    ulInts = MAP_TimerIntStatus(TIMERA0_BASE, 1);
    //
    // Clear the timer interrupt.
    //
    MAP_TimerIntClear(TIMERA0_BASE, ulInts);
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
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);
    PRCMCC3200MCUInit();
    MAP_PRCMPeripheralClkEnable(PRCM_TIMERA0, PRCM_RUN_MODE_CLK);
    MAP_PRCMPeripheralReset(PRCM_TIMERA0);
    MAP_TimerConfigure(TIMERA0_BASE, TIMER_CFG_PERIODIC);
    MAP_TimerPrescaleSet(TIMERA0_BASE, TIMER_BOTH, 0);
#endif

}

void _t0_start() ;

long _t0_period(long us) {
#ifdef __CC3200R1M1RGC__
    _t0_ticks = US_TO_TICKS(us);
    _t0_start();
#endif
}

void _t0_enable() {
#ifdef __CC3200R1M1RGC__
    MAP_TimerIntRegister(TIMERA0_BASE, TIMER_BOTH, __t0_timer_handler);
    MAP_IntPrioritySet(INT_TIMERA0A, INT_PRIORITY_LVL_1);
    MAP_TimerIntEnable(TIMERA0_BASE, TIMER_TIMA_TIMEOUT | TIMER_TIMB_TIMEOUT);
#endif

}

void _t0_disable() {
#ifdef __CC3200R1M1RGC__
    MAP_TimerIntDisable(TIMERA0_BASE, TIMER_BOTH);
    //
    // Unregister the timer interrupt
    //
    MAP_TimerIntUnregister(TIMERA0_BASE, TIMER_BOTH);
#endif

}

void _t0_start() {
#ifdef __CC3200R1M1RGC__
    MAP_TimerLoadSet(TIMERA0_BASE, TIMER_BOTH, _t0_ticks);
    MAP_TimerEnable(TIMERA0_BASE, TIMER_BOTH);
#endif


}

void _t0_stop() {
#ifdef __CC3200R1M1RGC__
    MAP_TimerDisable(TIMERA0_BASE, TIMER_BOTH);
#endif
    //          // clears all clock selects bits
}

void _t0_restart() {
#ifdef __CC3200R1M1RGC__
	// Restart the timer, from the beginning of a new period.
    MAP_TimerLoadSet(TIMERA0_BASE, TIMER_BOTH, _t0_ticks);
#endif
}

hwt_callbacks TIMER0_CALLBACKS = {_t0_init, _t0_period, _t0_enable, _t0_disable, _t0_start, _t0_stop, _t0_restart};

HardwareTimer Timer0(TIMER0_CALLBACKS);

#endif
