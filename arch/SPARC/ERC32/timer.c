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
 * \file arch/sparc/ERC32/timer.c
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
    ERC32_MEC.gp_timer_counter = ERC32_TIMER_COUNTER;        /*  RTCC    */
    
    ERC32_MEC.gp_timer_scaler  = KCONFIG_CLOCK_SPEED - 1;  /*  RTCS  */

    /*  Periodic configuration  */
    ERC32_MEC_Set_GP_Timer_Control(ERC32_MEC_TIMER_COUNTER_RELOAD | 
                                   ERC32_MEC_TIMER_COUNTER_LOAD | 
                                   ERC32_MEC_TIMER_COUNTER_ENABLE);
    
    CPU_EnableInterrupt(TRAP_GP_TIMER);
    
    /*  Time spent to program the fucking timer */
    do {
        ERC32_MEC_Get_GP_Timer_Counter(value);
    }while(value < 0);
}

/*
 * Function:    void CPU_ClockInit(void)
 * 
 * This function configures the Real Time Clock interrupt period. 
 */
void CPU_ClockInit(uint32 _ticks)
{
    /*  
     * SYSCLOCK MHz -> Timeout = RTCC * (RTCS+1) / SYSCLOCK  
     */
    ERC32_MEC.rt_clock_counter = _ticks;
    ERC32_MEC.rt_clock_scaler  = KCONFIG_CLOCK_SPEED - 1;

    /*  One shot Configuration  */
    ERC32_MEC_Set_RT_Timer_Control(ERC32_MEC_TIMER_COUNTER_LOAD |
                                   ERC32_MEC_TIMER_COUNTER_ENABLE);


    CPU_EnableInterrupt(TRAP_REAL_TIME_CLOCK);
}

