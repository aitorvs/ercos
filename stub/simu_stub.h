#ifndef _SIMU_STUB_H_
#define _SIMU_STUB_H_

extern unsigned long long global_time;

/**
 * This MACRO enables the interrupts by setting the ET flag.
 */
#define _CPU_EnableIRQ()
/**
 * This MACRO dissables the interrupts by clear the ET flag.
 */
#define _CPU_DisableIRQ()
/**  
 * This macro dissables the interrupts by manipulating the PIL
 * The macro also returns the currenti PIL into the value variable
 */
#define CPU_DisableSched(value)
/**
 * This macro Enables the interrupts by manipulating the PIL.
 * The macro stores the saved value in the PIL field.
 */
#define CPU_EnableSched(value)
/** 
 * \todo Documentar esta MACRO
 */
#define CPU_MaskInterrupt(vector)
/** 
 * \todo Documentar esta MACRO
 */
#define CPU_MaskAllInterrupts()
/** 
 * \todo Documentar esta MACRO
 */
#define CPU_IsInterrupt(vec)    ( vec > 16 && vec < 32 )

/** 
 * \todo Documentar esta MACRO
 */
#define CPU_ClearInterrupt(vector)
/** 
 * \todo Documentar esta MACRO
 */
#define CPU_EnableInterrupt(vector)
/** 
 * \todo Documentar esta MACRO
 */
#define CPU_ForceInterrupt(vector)

/**
 * This macro Resets the general purpose timer
 */
#define CPU_TimerReset()

/**
 * This macro Resets the real time clock timer
 */
#define CPU_ClockReset()

/**
 * This macro returns the clock counter value
 */
#define CPU_GetTimer(value) (value = global_time);

#endif

