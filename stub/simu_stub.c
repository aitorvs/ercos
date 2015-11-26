#include <sys/types.h>
#include <cpu.h>


/*
 * This file defines all the architecture dependent routines as dummy routines
 * in order to carry out an architecture independent simuations of the traced
 * system
 */

uint32 __ERC32_MEC_Timer_Control_Mirror;
uint32 irq_counter;
OS_Time_t global_time;

void CPU_ContextInit(CPU_context_t   *the_context,
                     uint8         *stack_base,
                     uint32         size,
                     uint32         new_level,
                     void           *entry_point)
{
    /*  Do nothing  */
}
 
void CPU_Init(void)
{
    /*  Do nothing  */
}

void CPU_TimerInit(void)
{
    /*  Do nothing  */
}

void CPU_ClockInit(uint32 _ticks)
{
    /*  Do nothing  */
}

void CPU_TraceTraps (unsigned long vec, int l1, int l2)
{
    /*  Do nothing  */
}

void CPU_Schedule(void)
{
    OS_Schedule();
    /*  Do nothing  */
}

void CPU_SerialTx (char car)
{
    /*  Do nothing  */
}

void CPU_SerialRx (int *car)
{
    /*  Do nothing  */
}

void CPU_PowerDown(void)
{
    /*  Do nothing  */
}

void CPU_SerialRxDebug(uint8 _car)
{
    /*  Do nothing  */
}
