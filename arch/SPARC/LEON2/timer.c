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
    int value;
    
    /*  
     * SYSCLOCK MHz -> Timeout = GPCC * (GPCS+1) / SYSCLOCK  
     */
    
    REGMAP.timer_1_counter = LEON2_TIMER_COUNTER;        /*  RTCC    */
    REGMAP.timer_1_reload = LEON2_TIMER_COUNTER;
    
    REGMAP.prescaler_counter  = KCONFIG_CLOCK_SPEED - 1;  /*  RTCS  */
    REGMAP.prescaler_reload  = KCONFIG_CLOCK_SPEED - 1;  /*  RTCS  */

    /*  Periodic configuration  */
    LEON2_Set_GP_Timer_Control(LEON2_TIMER_CONTROL_RELOAD | 
                               LEON2_TIMER_CONTROL_LOAD | 
                               LEON2_TIMER_CONTROL_ENABLE);
    CPU_EnableInterrupt(TRAP_GP_TIMER);

}

/*
 * Function:    void CPU_ClockInit(void)
 * 
 * This function configures the Real Time Clock interrupt period. 
 */
void CPU_ClockInit(uint32 _ticks)
{
    REGMAP.timer_2_reload = _ticks;

    LEON2_Set_RT_Timer_Control(LEON2_TIMER_CONTROL_LOAD |
                               LEON2_TIMER_CONTROL_ENABLE);


    CPU_EnableInterrupt(TRAP_REAL_TIME_CLOCK);
}

