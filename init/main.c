/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT on ERC-32.
*/

#include <kernel/config.h>
#include <kernel/task.h>
#include <kernel/traps.h>
#include <kernel/mm.h>
#include <kernel/debug.h>
#include <lib/stdio.h>
#include <syscall.h>

#ifndef CONFIG_ERCOS_SIMU
/* This function is defined in kernel/cppinit.c. There is no header file
 * exporting this symbol definition, so we must to do it by hand.
 */
void OS_CPPInit(void);
#endif

#ifdef CONFIG_GDB_MONITOR
void gdb_monitor(void);
#endif

//------------------------------------------------------------------------------


/*
 * Kernel start entry point
 */
void OS_Start(void)
{

    /* If the GDB Monitor is active, enter in debug mode */
    #ifdef CONFIG_GDB_MONITOR
    	int car;
    	printf("Run debugger? (s/n): ");
        CPU_SerialRx(&car);
        printf("%c\n", car);
        if (car == 's')
    	    gdb_monitor();
    #endif
    
    #ifdef CONFIG_ERCOS_TRACER
        OS_TracerInit();
    #endif
    /*  
     * Start the architecture configuration:
     */
    CPU_Init();
    
    /*
     * Init the Trap Table
     */
    OS_TrapInit();

    /*
     * Init the stack memory pool
     */
    OS_MMInitMemoryPool(KCONFIG_STACK_POOL_LOG2, KCONFIG_MIN_CHUNK_LOG2, (char *)KCONFIG_ADDR_STACK_INIT);
    
    /*
     * init the Schedule
     */
    OS_SchedInit();
    
    /** 
     * Init the serial line device
     */
//    OS_InitSerialLine();
    
    /*
     * Init the General Purpose Timer and the Real Time Clock
     */
    OS_TimeInit();
    
    /*
     * Init all the Thread Control Blocks
     * 
     * The routine also init the idle task
     */

    OS_TCBInit();
    
    OS_IRQInitEvents();
    
    /*
     * Init all the C++ Constructors in case of need
     */
#ifndef CONFIG_ERCOS_SIMU
    OS_CPPInit();
#endif
    
    /*  First Schedule  */
    OS_Schedule();
    SYS_Schedule();
    
    /*  NEVER REACH HERE    */
}

