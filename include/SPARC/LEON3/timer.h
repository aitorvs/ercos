/*
    Copyright 2006 (c) by Aitor Viana Sanchez, Pablo Parra Espada
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT on LEON3.
*/

#ifndef _HW_TIMER_H_
#define _HW_TIMER_H_

#include <cpu/cputypes.h>
#include <arch/platform.h>

//------------------------------------------------------------------------------

#define CPU_TIMER_COUNTER   LEON3_TIMER_COUNTER

/**
 * General Purpose Timer resolution
 */
#define CPU_TIMER_RESOLUTION    2

/**
 * This macro Resets the general purpose timer
 */
#define CPU_TimerReset() \
    do { \
        GPTIMER_Set_Timer_Control(GPTIMER_COUNTER,0); \
    }while(0) \

/**
 * This macro Resets the real time clock timer
 */
#define CPU_ClockReset() \
    do { \
        GPTIMER_Set_Timer_Control(GPTIMER_REAL_TIME,0); \
    }while(0) \

/**
 * This macro returns the clock counter value
 */
#define CPU_GetTimer(value) \
    do { \
        GPTIMER_Get_Timer_Counter(GPTIMER_COUNTER,value); \
    }while(0)

/**
 * Configure the General Purpose Timer tick slice.
 * 
 * \param _ticks:  This parameter is the one shot interval ticks
 * 
 * This function configures the General Purpose Timer interrupt period. With the 
 * current timer configuration it is possible to interrupt with 4 min of maximun
 * period
 */
void CPU_TimerInit(void);

/**
 * Configure the Real Time Clock one shot interrupt period.
 * 
 * This routine configure the Real Time Clock to 125ns (2/SYSCLOCK) 
 * hardware tick resolution.
 */
void CPU_ClockInit(uint32 _ticks);

#endif //_GPTIMER_H_

