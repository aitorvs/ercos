/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT on ERC-32.
*/

/*!
 * \file include/sparc/ERC32/timer.h
 * \brief CPU Layer timer managemente functions.
 */

#ifndef _HW_TIMER_H_
#define _HW_TIMER_H_

#include <cpu/cputypes.h>
#include <arch/erc32.h>

//------------------------------------------------------------------------------

/** \brief Timer Counter Reload Value. */
#define CPU_TIMER_COUNTER   ERC32_TIMER_COUNTER

/** \brief General Purpose Timer resolution. */
#define CPU_TIMER_RESOLUTION    2

/** \brief Resets the general purpose timer. */
#define CPU_TimerReset() \
    do { \
        ERC32_MEC_Set_GP_Timer_Control(0); \
    }while(0) \

/** \brief Resets the real time clock timer. */
#define CPU_ClockReset() \
    do { \
        ERC32_MEC_Set_RT_Timer_Control(0); \
    }while(0) \

/**
 * \brief Returns the clock counter value.
 * 
 * \param value  Stores the value of the General Purpose Timer Counter 
 *               Register.
 */
#define CPU_GetTimer(value) \
    do { \
        ERC32_MEC_Get_GP_Timer_Counter(value); \
    }while(0)

/**
 * \brief Configures the General Purpose Timer tick slice.
 * 
 * This function configures the General Purpose Timer interrupt period. With the 
 * current timer configuration it is possible to interrupt with 4 min of maximun
 * period
 */
void CPU_TimerInit(void);

/**
 * \brief Configures the Real Time Clock one shot interrupt period.
 * 
 * \param ticks  Load value of the Real Time Clock.
 * 
 * This routine configure the Real Time Clock to 125ns (2/SYSCLOCK) 
 * hardware tick resolution.
 */
void CPU_ClockInit(uint32 _ticks);

#endif //_GPTIMER_H_

