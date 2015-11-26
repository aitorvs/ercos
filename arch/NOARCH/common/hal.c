#include <sys/types.h>
#include <cpu.h>

/*****************************************************************************
                THIS SHALL BE DEFINED IN THE LINK.LD SCTIPT
*****************************************************************************/
// helper type
typedef void (*func_ptr) (void);

// zero-terminated constructor tables for each group
func_ptr __ctors_GLOBAL__[];
void * _end_ctors;


/*****************************************************************************
                All these routines shall be implemented by a new
                architecture. These routines can be distributed
                within the common andd NOPLATFORM directories 
                depending whether they are common for the architecture
                or specific for each platform architecture.
                Also, the common and NOPLATFORM architecture separation
                can be modified creating one unique directory.
*****************************************************************************/

/*****************************************************************************
                        GENERIC CALLS
*****************************************************************************/
void CPU_ContextInit(CPU_context_t   *the_context,
                     uint8         *stack_base,
                     uint32         size,
                     uint32         new_level,
                     void           *entry_point)
{
    /*  Do nothing  */
}

void CPU_Init(void) {}

void CPU_PowerDown(void) {}

/*****************************************************************************
                        SYSTEM CALLS
*****************************************************************************/

/** 
 * This routine shall invoke the system call that performs the scheduling.
 */
void _SYS_Schedule(void) {}

/**
 * This routine shall perform the system call to perform the enabling of
 * the IRQs
 */
void _CPU_EnableIRQ() {}
/**
 * This routine shall perform the system call to perform the disabling of
 * the IRQs
 */
void _CPU_DisableIRQ() {}

/*****************************************************************************
                        TIMING CALLS
*****************************************************************************/

/** This global variable stores the kernel timing   */
OS_Time_t global_time;

/** This routine starts the general purpose timer   */
void CPU_TimerInit(void) {}

/** This routine starts the real-time clock   */
void CPU_ClockInit(uint32 _ticks) {}

/** This routine Resets the general purpose timer */
void CPU_TimerReset() {}

/** This routine Resets the real-time clock */
void CPU_ClockReset() {}


/*****************************************************************************
                        INTERRUPT RELATED CALLS
*****************************************************************************/

/** 
 * This routine shall mask a given interrupt
 */
void CPU_MaskInterrupt(uint32 vector) {}

/** 
 * This routine shall mask all interrupts (only the NMI shall be un-masked)
 */
void CPU_MaskAllInterrupts() {}

/** 
 * This routine shall return TRUE in case the given vector is a valid
 * interrupt vector, and FALSE in any other case. 
 */
bool CPU_IsInterrupt(uint32 vec) {}

/** 
 * This routine shall clear the given interrupt.
 */
void CPU_ClearInterrupt(uint32 vector) {}
/** 
 * This routine shall enable the given interrupt
 */
void CPU_EnableInterrupt(uint32 vector) {}
/** 
 * This routine shall force the given interrupt if possible. 
 * (just for debug purposes)
 */
void CPU_ForceInterrupt(uint32 vector) {}


/*****************************************************************************
                        DEBUG RELATED CALLS
*****************************************************************************/

void CPU_TraceTraps (unsigned long vec, int l1, int l2) {}

void CPU_SerialTx (char car) {}

void CPU_SerialRx (int *car) {}

void CPU_SerialRxDebug(uint8 _car) {}

/*****************************************************************************
                        SCHEDULING CALLS
*****************************************************************************/

/** This routine shall disable the schedule   */  
void CPU_DisableSched(uint32 value) {}

/** This routine shall enable the schedule    */
void CPU_EnableSched(uint32 value) {}






