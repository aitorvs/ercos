/*
    Copyright 2006 (c) by Aitor Viana Sanchez, Pablo Parra Espada
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT on ERC-32.
*/

/*!
 * \file arch/sparc/LEON3/timer.c
 * \brief Definition of the CPU-layer timer management functions.
 */

#include <cpu.h>
#include <target.h>
#include <kernel/config.h>

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------

/*
 * Function:    void CPU_TimerInit(uint32 _ticks)
 * 
 * This function configures the General Purpose Timer tick resolution.
 * This is a ONE SHOT timer.
 */

void CPU_TimerInit(void)
{
    /*  
     * SYSCLOCK MHz -> Timeout = GPCC * (GPCS+1) / SYSCLOCK  
     */
    
    GPTIMER.timer[GPTIMER_COUNTER].counter_value = LEON3_TIMER_COUNTER;        /*  RTCC    */
    GPTIMER.timer[GPTIMER_COUNTER].reload_value = LEON3_TIMER_COUNTER;
    
    GPTIMER.scaler_value  = KCONFIG_CLOCK_SPEED - 1;  /*  RTCS  */
    GPTIMER.scaler_reload_value  = KCONFIG_CLOCK_SPEED - 1;  /*  RTCS  */

    /*  Periodic configuration  */
    GPTIMER_Set_Timer_Control(GPTIMER_COUNTER, GPTIMER_CONTROL_RESTART |
	   			   GPTIMER_CONTROL_IRQ_ENABLE | 
                                   GPTIMER_CONTROL_ENABLE);

}

/*
 * Function:    void CPU_ClockInit(void)
 * 
 * This function configures the Real Time Clock interrupt period. 
 */
void CPU_ClockInit(uint32 _ticks)
{
    GPTIMER.timer[GPTIMER_REAL_TIME].counter_value = _ticks;
    GPTIMER_Set_Timer_Control(GPTIMER_REAL_TIME, GPTIMER_CONTROL_IRQ_ENABLE |
                                   GPTIMER_CONTROL_ENABLE);
}

