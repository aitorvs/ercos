/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT on ERC-32.
*/

/*
 * File:    $Source$
 * 
 * $Id: time.c 1269 2008-02-25 18:10:23Z cesar $
 *  
 * $Revision: 1269 $
 * $Date: 2008-02-25 19:10:23 +0100 (lun, 25 feb 2008) $
 * Last CVS commit by $Author: cesar $
 * 
 * Registro de cambios
 * 
 * $Log$
 * Revision 1.5  2005/09/28 09:24:41  aitor
 * The clock isr only marks the clock event. This allows to manipulate the
 * internal kernel data structures in mutual exclusion.
 *
 * Revision 1.4  2005/09/23 14:58:22  aitor
 * We are change the clock isr manage.
 *
 * Revision 1.3  2005/09/22 11:26:58  aitor
 * Modified the OS_TimerHalt() routine. We also use the CPU_TimerReset() routine instead
 * of ERC32_MEC... routine.
 *
 * Revision 1.2  2005/09/22 11:09:32  aitor
 * The errors in the OS_DoSleep routine are now corrected.
 *
 * Revision 1.1  2005/09/19 11:32:26  aitor
 * Restoring the project.
 *
 * Revision 1.27  2005/09/01 13:26:46  aitor
 * OS_GetTimer function is now named as OS_TimeGetTicks
 *
 * Revision 1.26  2005/09/01 11:31:00  aitor
 * The general purpose timer is now a periodic timer. The real-time clock is the one shot
 * system clock.
 *
 * Revision 1.25  2005/08/26 14:15:36  aitor
 * OS_GetClock() function debuged
 *
 * Revision 1.24  2005/08/25 14:48:32  aitor
 * Added the penalty time in doSleep() function and the efective timer
 * resolution
 *
 * Revision 1.23  2005/08/25 08:39:54  aitor
 * Errors in the timer bottom isr.
 *
 * Revision 1.22  2005/08/23 15:00:56  aitor
 * The IRQ timer has now a top half service routine and a bottom half
 * service routine. The top one register the event and the bottom one
 * deal it.
 *
 * Revision 1.21  2005/08/15 12:44:18  aitor
 * Correct an error in the OS_TimerISR() function. We only mus re-program the
 * timer in case of the time_queue is not empty
 *
 * Revision 1.20  2005/08/11 19:04:48  aitor
 * Comments.
 *
 * Revision 1.19  2005/07/21 11:04:45  aitor
 * *** empty log message ***
 *
 * Revision 1.18  2005/07/05 16:05:40  aitor
 * *** empty log message ***
 *
 * Revision 1.17  2005/07/31 17:01:19  aitor
 * The clock_tick_counter is now and unsigned 32 bit variable.
 * Correct also some errors in the OS_SoSleep() routine.
 *
 * Revision 1.16  2005/07/29 18:38:14  aitor
 * Error correction in the OS_DoSleep() routine.
 * The OS_GetClock() routine is now static.
 *
 * Revision 1.15  2005/07/29 11:27:30  aitor
 * Change the naming convention.
 *
 * Revision 1.14  2005/07/29 11:23:08  aitor
 * Change the naming convention.
 *
 * Revision 1.13  2005/07/29 10:15:31  aitor
 * Includes the treament of the real time clock in this source file.
 *
 * Revision 1.12  2005/07/29 08:46:20  aitor
 * Change the syscall.h header file location.
 *
 * Revision 1.11  2005/07/28 16:53:38  aitor
 * Modify the OS_timer_isr function.
 *
 * Revision 1.10  2005/07/28 14:29:50  aitor
 * traps.c and traps.h files are now in the independent code part.
 * We are also changed the OS_timer_isr() function to manage the ready and
 * block queues.
 * We also change the name of the OS_timer_program by OS_sleep().
 *
 * Revision 1.9  2005/07/27 15:05:52  aitor
 * Added the routine OS_timer_program() that allows to program a one shot
 * timer.
 *
 * Revision 1.8  2005/07/27 11:51:01  aitor
 * The Real Time Clock is not activated in the system startup.
 *
 * Revision 1.7  2005/07/27 10:35:58  aitor
 * Change the timer functionality form the clock functionality and vice versa.
 *
 * Revision 1.6  2005/07/27 09:38:46  aitor
 * Added the function OS_get_timer(). This routine returns the 64 bit value
 * of the system timer.
 *
 * Revision 1.5  2005/07/26 08:38:20  aitor
 * Named change.
 *
 * Revision 1.4  2005/07/22 09:20:09  aitor
 * Delete some unused routines.
 *
 * Revision 1.3  2005/07/19 14:34:44  aitor
 * Added the config.h header file. Use the TIMER_TICK label instead of a direct
 * value.
 *
 * Revision 1.2  2005/06/23 12:08:37  aitor
 * Nueva estructura de directorios.
 *
 * Revision 1.1  2005/06/21 10:28:15  aitor
 * Alta del mini sistema ERCOS-RT
 *
 */

#include <kernel/time.h>

#include <tracer/systracer.h>
#include <kernel/sched.h>
#include <kernel/traps.h>
#include <kernel/irqev.h>
#include <kernel/list.h>
#include <kernel/sem.h>
#include <kernel/task.h>
#include <kernel/debug.h>
#include <syscall.h>
//----------------------------------------------------------------------------
static uint32 __div64_32(uint64 *n, uint32 base);

/** This macro return the firs thread in the time queue */
#define FIRST_TIME_THREAD   GET_ELEMENT(OS_task_t, time_list.first, time_node)

#define GET_TIME_THREAD(node)   GET_ELEMENT(OS_task_t, node, time_node)

/**
 * This variable stores the timer ticks
 */
volatile static OS_Time_t timer_tick_counter;

/** Time queue  */
#ifdef CONFIG_ERCOS_SIMU
OS_List_t time_list;
#else
static OS_List_t time_list;
#endif

/** Microseconds per second */
#define USECS_PER_SEC   1000000

/** \todo comentar esta etiqueta    */
#define OVERHEAD_CLICKS (CPU_TIMER_COUNTER + 1)

/**
 * This MACRO normalizes the Timeval system structure.
 * FIXME -- the "else if" clause is really needed ?
 */
#define _OS_Timeval_normalize(t) {\
    if ((t)->useconds >= USECS_PER_SEC) { \
     (t)->useconds -= USECS_PER_SEC; \
       (t)->seconds++; \
   } else if ((int)(t)->useconds < 0) { \
       (t)->useconds += USECS_PER_SEC; \
       (t)->seconds--; \
   } \
}

static uint32 oneshot_programed = 0;

#define ONESHOT_TIMER_NOT_PROGRAMMED()  (oneshot_programed = 0)

#define ONESHOT_TIMER_PROGRAMMED()      (oneshot_programed = 1)

#define IS_ONESHOT_TIMER_NOT_PROGRAMMED()   (0 == oneshot_programed)

#define OS_ProgramClock(_value)  \
do {    \
    ONESHOT_TIMER_PROGRAMMED();  \
    CPU_ClockInit(_value);   \
}while(0)


/** This structure stores the system time in seconds and microseconds   */
volatile static OS_Timeval_t OS_Time;

static uint32 seconds_per_timer_tick = 0;

static uint32 useconds_per_timer_tick = 0;

//----------------------------------------------------------------------------

/**
 * Init the General Purpose Timer.
 * 
 * This routine initialize the General Purpose Timer by registering its
 * interrupt service routin but not activate the timer count.
 */
static void OS_TimerInit(void);

/**
 * Timer Interrupt Service Routine.
 * 
 * \param   level:  This parameter is the interrupt level.
 * 
 * This routine manage the timer interrupt. This timer is programmed when a task
 * need to sleep during a certain time interval.
 */
static void OS_TimerISR(int level);

/**
 * Init the system real time clock.
 * 
 * The routine register a interrupt service routine for the real time clock,
 * configures the clock tick interval and reset the clock tick counter.
 */
static void OS_ClockInit(void);

/**
 * Interrupt service routine for the real time clock.
 * 
 * \param   level:  This parameter is the interrupt level.
 */
#ifndef CONFIG_ERCOS_SIMU

static void OS_ClockISR(int level);

#endif


//----------------------------------------------------------------------------

/*
 * Function:    void OS_TimeInit(void)
 * 
 * This routine init the system timming. The routine init the Real Time Clock
 * and the General Purpose Timer.
 */
void OS_TimeInit(void)
{
    /*  Init the Real Time Clock    */
    OS_ClockInit();
    /*  Init the General Purpose Timer  */
    OS_TimerInit();
}


/******************************************************************************
 ******************************** GP TIMER ************************************
 * ***************************************************************************/

/*
 * Function:    static void OS_TimerInit(void)
 * 
 * This function initializes the General Purpose Timer. It basically registers
 * a timer interrupt service routine.
 */
static void OS_TimerInit(void)
{
    uint64 clicks = OVERHEAD_CLICKS;
    
    DEBUG ("Timer init...");
    
    /*  Register the General Purpose Timer ISR  */
    OS_RegisterIRQ(TIMER_TRAP, OS_TimerISR);

    /*  10ms real time clock tick configuration   */
    CPU_TimerInit();
    
    /*  Reset the time queue    */
    OS_ListReset(&time_list);

    /*  Reset the counter   */
    timer_tick_counter = 0;
    
    /*  Init the system time structure  */
    OS_Time.seconds = OS_Time.useconds = 0;
    
    useconds_per_timer_tick = __div64_32(&clicks, CLICKS_PER_SEC) * CLICKS_PER_USEC;
    seconds_per_timer_tick = (uint32)clicks;
}

/*
 * Function:    static void OS_TimerISR(int level)
 * 
 * This function is the timer interrupt routine
 */
#ifdef CONFIG_ERCOS_SIMU
void OS_ClockISR(int _level)
#else
static void OS_ClockISR(int _level)
#endif
{

    ONESHOT_TIMER_NOT_PROGRAMMED();
    
    SYSTRACE_TAM16(SYSTRACE_CLOCK_EVENT, 0);
         
    /*  Mark the interrupt clock event    */
    OS_IRQMarkClockEvent();

}

void OS_ClockBottomISR(void)
{
    OS_task_t *task;
    OS_Node_t *pos;
    OS_Time_t current_time;
    OS_Time_t thread_time;
    OS_Time_t next_clock_val;         // the next clock value to be programmed
    
    if(OS_ListGetNum(&time_list) == 0) 
    {

        OS_ListReset(&time_list);
        DEBUG ("Reset de la lista de tiempos");

        return;
    }
    
    /*  This is the timeout of the first thread in the list */
    thread_time = FIRST_TIME_THREAD->abs_timeout;
    
    /*  Obtain the current system time  */
    current_time = OS_TimeGetTicks();
    
    if(thread_time > current_time)
    {
    
        next_clock_val = thread_time - current_time;
        
        if(next_clock_val < (OS_Time_t)OVERHEAD_CLICKS)
        {
            /*  
             * If the remainder timeout of the first thread is less than 2^32
             * clock ticks, we program the remainder value and the thread is 
             * left in the time queue, waiting for the next clock interrupt
             */
	    /* DEBUG ("Reprogramming clock with %dus; the first thread's abs_timeout (%d) is future to the current time (%d)",
	    	(uint32) next_clock_val, (uint32) thread_time, (uint32) current_time); */
            OS_ProgramClock((uint32)next_clock_val);
        }

        return;
    }
    else 
    {
        list_for_each(pos, &time_list) 
        {
            if((FIRST_TIME_THREAD->abs_timeout) <= (TIME_PENALTY + OS_TimeGetTicks()))
            {
                /*  Outs the blocked task   */
                task = GET_ELEMENT(OS_task_t, OS_ListGetNode(&time_list), time_node);

                /*  
                 * If the task is blocked in a semaphore due to a TimedWait 
                 * system call, we outs the thread from the semaphore queue
                 */
                if(task->status | TS_SEM_BLOCKED)
                {
                    OS_SemExtracThreadFromList(task->wait_sem, task);
                }
        
                /*  Inserts the waked task in the ready queue   */
                OS_SchedTaskReady(task->tid);
            }
            else 
                break;
        }
    }

    /*  Activate the one shot timer */
    if(OS_ListGetNum(&time_list) != 0) 
    {
        OS_Time_t current_time;
        OS_Time_t clock_program_value;
        
        //	current_time = OS_TimeGetTicks() + TIME_PENALTY;
        current_time = OS_TimeGetTicks();
        clock_program_value = FIRST_TIME_THREAD->abs_timeout - current_time -
            TIME_PENALTY;
        
        if((clock_program_value < OVERHEAD_CLICKS) &&
           (FIRST_TIME_THREAD->abs_timeout > current_time))
        {
            /*  
             * Program the clock in case the value to be programmed was
             * correct
             */
	    /* DEBUG ("Reprogramming clock with %dus due to changes in the sleep "
		"queue; the first thread's abs_timeout (%d) is future to the "
		"current time (%d)", (uint32) clock_program_value,
		(uint32) FIRST_TIME_THREAD->abs_timeout, (uint32) current_time); */
            OS_ProgramClock((uint32)clock_program_value);
        }
        /*  
         * In case of a thread timeout was expired during the routine
         * execution, the routine marks a timer event
         */
        else if((FIRST_TIME_THREAD->abs_timeout <= current_time))
            OS_IRQMarkClockEvent();
    }
    else 
        OS_ListReset(&time_list);
}

void OS_TimeRemoveFromQueue(OS_task_t *_task)
{
    _task->status &= ~(TS_TIME_BLOCKED);
    
    /*  Remove the thread from the time_queue   */
    OS_ListExtract(&time_list, &(_task->time_node));
    
}

/*
 * Function:    void OS_DoSleep(uint32 _ticks)
 * 
 * In this sleep system call implementation the current task is been blocked. 
 * For this reason we not insert the current task in the ready queue.
 */

int OS_DoSleep(OS_Time_t _ticks)
{
    OS_Node_t *pos;
    int inserted = 0;
    int retval;
    volatile OS_Time_t current_time = 0;
    OS_Time_t clock_program_value = 0;

    
    if(_ticks < OS_TimeGetTicks() + TIME_PENALTY)
    {
        retval = 0;
    }
    else {

        /*  Init the absolute clock */
        current->abs_timeout = _ticks;
    
        // DEBUG2 ("Task #%d; until %x %d\n", current - task_table, (uint32)(_ticks >> 32), (uint32)_ticks);
        
        /*  Change the task status  */
        current->status |= TS_TIME_BLOCKED;
        
        if(OS_ListGetNum(&time_list) == 0)
        	OS_ListInsert(&time_list, &(current->time_node));
    	else {
	        list_for_each(pos, &time_list)
	        {
	            
	            if(GET_TIME_THREAD(pos)->abs_timeout > current->abs_timeout) 
                {
	                if(pos->prev != NULL){
	                    OS_ListAdd(&time_list, &(current->time_node), pos->prev);
	                }
	                else {
	                    OS_ListInsert(&time_list, &(current->time_node));
	                }
	    
	                inserted = 1;
	                break;
	            }
	        }

	        if(!inserted) {
	            OS_ListAppend(&time_list, &(current->time_node));
	        }
    	}
        

	current_time = OS_TimeGetTicks() + TIME_PENALTY;
        clock_program_value = FIRST_TIME_THREAD->abs_timeout - current_time;

        if(clock_program_value < OVERHEAD_CLICKS &&
                (FIRST_TIME_THREAD->abs_timeout > current_time))
        {
            OS_ProgramClock((uint32)clock_program_value);
        }
        else if(FIRST_TIME_THREAD->abs_timeout <= current_time) 
        {
            OS_IRQMarkClockEvent();
        }

        retval = -1;
    }
      
    return retval;
    
}

/*
 * Function:    void OS_TimerHalt(void)
 * 
 * Halt and unregister the timer
 */
void OS_TimerHalt(void)
{
    CPU_TimerReset();
    
    OS_FreeIRQ(CPU_CLOCK_TRAP, OS_TimerISR);

}


/******************************************************************************
 ***************************** REAL TIME CLOCK*********************************
 * ***************************************************************************/

/*
 * Function: static void OS_ClockInit(void)
 * 
 * This function initializes the system Real Time Clock
 */
static void OS_ClockInit(void)
{
    DEBUG ("Real Time Clock init...");
    
    /*  Register IRQ for the Real Time Clock    */
    OS_RegisterIRQ(CLOCK_TRAP, OS_ClockISR);
    
}

/*
 * Function:    static uint32 OS_GetClock(void)
 * 
 * Return the timer ticks.
 */
OS_Time_t OS_TimeGetTicks(void)
{
    OS_Time_t counter = 0;
    OS_Time_t the_counter = 0;

    
    /*  Get the General Purpose Timer Counter   */
    CPU_GetTimer(the_counter);
    
    //counter = (OS_Time_t)(timer_tick_counter | (OS_Time_t)the_counter);
    counter = (OS_Time_t)(timer_tick_counter + (OS_Time_t)the_counter);

//    if(counter < CPU_TIMER_RESOLUTION)
//        return 0;
    
    return (counter);
}

void OS_TimeGetTime(OS_Timeval_t *_system_time)
{
    OS_Time_t the_counter;
    
    /*  Get the General Purpose Timer Counter   */
    CPU_GetTimer(the_counter);
    
    _system_time->seconds = OS_Time.seconds + (uint32)((uint32)the_counter / 1000000);
    _system_time->useconds = OS_Time.useconds + ((uint32)the_counter % 1000000);
    
    _OS_Timeval_normalize(_system_time);
}

/*
 * Function: static void OS_ClockISR(int level)
 * 
 * This function implements the Real Time Clock service routine
 */
static void OS_TimerISR(int level)
{
    /*  Increment the real time clock counter ticks */
    timer_tick_counter += (OS_Time_t)((OS_Time_t)CPU_TIMER_COUNTER + 1);
    
    OS_Time.seconds += seconds_per_timer_tick;
    OS_Time.useconds += useconds_per_timer_tick;
    
    _OS_Timeval_normalize(&OS_Time);
    
    /*
     * If the one shot timer is not programmed means that it is possible that 
     * the next thread to be programmed has the abs_timeout greater than the
     * timer overflow value. So we must call the OS_ClockBottomISR routine in 
     * order to try a new one shot programation
     */ 
    if(IS_ONESHOT_TIMER_NOT_PROGRAMMED() && 
       OS_ListGetNum(&time_list) != 0)
    {
        OS_ClockBottomISR();
    }
    
}

static uint32 __div64_32(uint64 *n, uint32 base)
{
    uint64 rem = *n;
    uint64 b = base;
    uint64 res, d = 1;
    uint32 high = rem >> 32;

    /* Reduce the thing a bit first */
    res = 0;
    if (high >= base) {
        high /= base;
        res = (uint64) high << 32;
        rem -= (uint64) (high*base) << 32;
    }
    while ((uint64)b > 0 && b < rem) {
        b = b+b;
        d = d+d;
    }
    do {
        if (rem >= b) {
            rem -= b;
            res += d;
        }
        b >>= 1;
        d >>= 1;
    } while (d);
    *n = res;
    return rem;
}

