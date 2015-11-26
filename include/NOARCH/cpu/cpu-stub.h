#ifndef CPUSTUB_H_
#define CPUSTUB_H_

/** This value shall be the trap number of the general purpose timer    */
#define CPU_TIMER_TRAP  0x0
#define CPU_CLOCK_TRAP  0x0

#define CPU_NUMBER_OF_INTERRUPTS   0x0
#define CPU_TRAP_MIN_NUMBER 0x0
#define CPU_TRAP_MAX_NUMBER 0x0


//--------------------------------
extern unsigned long long global_time;


/**
 * This macro returns the clock counter value
 */
#define CPU_GetTimer(value) (value = global_time);

/** 
 * This macro returns TRUE if the kernel is currently inside a trap handler.
 * Otherwise returns FALSE
 */
#define CPU_IS_IN_TRAP_HANDLER  (1 == 1)

#endif /*CPUSTUB_H_*/
