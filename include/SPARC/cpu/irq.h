
#ifndef _CPU_IRQ_H_
#define _CPU_IRQ_H_
 
#include <cpu/psr.h>

#define CPU_NUMBER_OF_INTERRUPTS   TARGET_NUMBER_OF_INTERRUPTS
#define CPU_TRAP_MIN_NUMBER TRAP_MIN_NUMBER
#define CPU_TRAP_MAX_NUMBER TRAP_MAX_NUMBER

#define CPU_TIMER_TRAP  TRAP_GP_TIMER
#define CPU_CLOCK_TRAP  TRAP_REAL_TIME_CLOCK
#define CPU_SERIAL_TRAP TRAP_UART_A

extern uint32 irq_counter;

/** This macro return TRUE in case of nested interrupt. */
#define CPU_IS_IN_TRAP_HANDLER()    (0 != irq_counter)

/** 
 * \todo Documentar esta MACRO
 */
#define CPU_MaskInterrupt(vector) \
    do { \
        TARGET_Mask_interrupt(vector); \
    }while(0) \

/** 
 * \todo Documentar esta MACRO
 */
#define CPU_MaskAllInterrupts() \
    do { \
        TARGET_Mask_all_interrupts(); \
    }while(0) \

/** 
 * \todo Documentar esta MACRO
 */
#define CPU_IsInterrupt(vec) TARGET_Is_Interrupt(vec)

/** 
 * \todo Documentar esta MACRO
 */
#define CPU_ClearInterrupt(vector) \
    do { \
        TARGET_Clear_interrupt(vector); \
    }while(0) \

/** 
 * \todo Documentar esta MACRO
 */
#define CPU_EnableInterrupt(vector) \
    do { \
        TARGET_Enable_interrupt(vector); \
    }while(0) \

/** 
 * \todo Documentar esta MACRO
 */
#define CPU_ForceInterrupt(vector) \
    do { \
        TARGET_Force_interrupt(vector); \
    }while(0) \



#endif

