/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

/*
 * File:    $Source$
 * 
 * $Id: sched.c 1269 2008-02-25 18:10:23Z cesar $
 *  
 * $Revision: 1269 $
 * $Date: 2008-02-25 19:10:23 +0100 (lun, 25 feb 2008) $
 * Last CVS commit by $Author: cesar $
 * 
 * Registro de cambios
 * 
 * $Log$
 * Revision 1.2  2005/10/06 08:54:58  aitor
 * Change the schedluing policy. Now the schedluer and the dispatcher are separated.
 *
 * Revision 1.1  2005/09/19 11:32:26  aitor
 * Restoring the project.
 *
 * Revision 1.20  2005/09/01 11:33:29  aitor
 * CPU_EnableIRQ and CPU_DisableIRQ are named now as _CPU_EnableIRQ and
 * _CPU_DisableIRQ
 *
 * Revision 1.19  2005/08/25 08:38:03  aitor
 * Delete some commented code.
 *
 * Revision 1.18  2005/08/23 15:01:32  aitor
 * Ensure the irq disable in the scheduler.
 *
 * Revision 1.17  2005/08/18 14:59:13  aitor
 * Correct some errors in the OS_SetPriority() routine.
 *
 * Revision 1.16  2005/08/15 12:44:35  aitor
 * *** empty log message ***
 *
 * Revision 1.15  2005/08/11 19:01:02  aitor
 * Added the function OS_SetTmpPriority(...) to change the thread priority.
 * We also modify the OS_SetPriority(...) to change the base thread priority.
 *
 * Revision 1.14  2005/08/08 15:29:06  aitor
 * Comments.
 *
 * Revision 1.13  2005/08/03 10:40:30  aitor
 * Comments.
 *
 * Revision 1.12  2005/07/21 11:04:01  aitor
 * Implement the OS_DoGetPriority() and OS_DoSetPriority() routines.
 * The OS_SchedADDQueueHead() and the OS_SchedADDQueueTail() routines
 * are now static.
 * Adde the OS_SchedTaskReady() routine. This routine adds a task into the
 * sched queue from the tail.
 *
 * Revision 1.11  2005/07/29 19:37:05  aitor
 * *** empty log message ***
 *
 * Revision 1.10  2005/07/29 19:08:28  aitor
 * Modify the OS_Schedule routine in order to execute the idle task in case of
 * need.
 *
 * Revision 1.9  2005/07/29 18:36:34  aitor
 * *** empty log message ***
 *
 * Revision 1.8  2005/07/29 11:27:30  aitor
 * Change the naming convention.
 *
 * Revision 1.7  2005/07/29 11:23:08  aitor
 * Change the naming convention.
 *
 * Revision 1.6  2005/07/29 08:45:18  aitor
 * Delete all the need_resched variable treatment.
 *
 * Revision 1.5  2005/07/28 16:54:14  aitor
 * The variable need_resched is now not necessary.
 *
 * Revision 1.4  2005/07/28 14:28:26  aitor
 * Added functions to insert task in the ready queue namend sched_queue.
 * Also we change the schedule in order to take into acount the ready and
 * block queue.
 *
 * Revision 1.3  2005/07/27 15:05:15  aitor
 * In the OS_schedule() function we need activate the ET flag when a context
 * switch is not necessary.
 *
 * Revision 1.2  2005/07/26 08:38:20  aitor
 * Named change.
 *
 * Revision 1.1  2005/07/19 14:36:05  aitor
 * Added the system schedule. This schedule is not the definitive.
 *
 */

#include <kernel/sched.h>
#include <kernel/debug.h>
#include <sys/types.h>
#include <kernel/config.h>
#include <kernel/context.h>
#include <kernel/list.h>
#include <kernel/task.h>
#include <tracer/systracer.h>
#include <tracer/ktracer.h>
#include <target.h>

//------------------------------------------------------------------------------

extern volatile uint32 schedule_active;

/**
 * This pointer is the current thread
 * 
 * The current thread is initialized to the idle thread.
 */
OS_task_t *current;
OS_task_t *old_current;

/**
 * This is the sched queue.
 * 
 * In this queue the task is been ready. We have one queue for each priority.
 * The task is store in the queue that match with its priority.
 */
#ifdef CONFIG_ERCOS_SIMU
    OS_List_t sched_list[KCONFIG_NUM_PRIORITIES];
#else
    static OS_List_t sched_list[KCONFIG_NUM_PRIORITIES];
#endif

/**
 * The sched queue index.
 * 
 * This index points to the mayor priority and non empty queue.
 */
static uint16 sched_queue_i;

/** This macro verifies if a task y already ready   */
#define IS_TASK_READY(task) (TS_READY == task->status)

//------------------------------------------------------------------------------

/**
 * \todo Documentar esta funcion
 */
static void OS_SchedAddQueueHead(OS_task_t *_task);

/**
 * Inserts a task into the sched queue from the tail.
 * 
 * \param _task:    This is a pointer to the TCB to be inserted.
 */
static void OS_SchedAddQueueTail(OS_task_t *_task);

//------------------------------------------------------------------------------


/*
 * Function: void OS_SchedInit(void)
 * 
 * This function initializes the schedule parameters
 */
void OS_SchedInit(void)
{
    int i;
    
    DEBUG("Scheduling init...");

    /*  Current task points to the first entry in the task table. After calling
     *  to OS_Schedule, the current task will be the main task (the second
     *  entry) and the old_current will become this task. In fact, this slot is
     *  never used (really?) */
    current = (task_table + 0);
    
    /*  The sched queue index points to the min priority queue  */
    sched_queue_i = KCONFIG_MIN_PRIORITY;
    
    for (i = 0; i < KCONFIG_NUM_PRIORITIES; i++)
        OS_ListReset(&sched_list[i]);
}

/*
 * Function:    void OS_SchedCurrentReady(void)
 */
void OS_SchedCurrentReady(void)
{
    OS_SchedAddQueueHead(current);
}

/*
 * Function:    void OS_SchedTaskReady(tid_t _task)
 */
void OS_SchedTaskReady(tid_t _task)
{
        /* DEBUG ("(_task=%d); priority=%d", _task, task_table[_task].priority); */
        OS_SchedAddQueueTail(task_table + _task);
}

/*
 * Function: void OS_Schedule(void)
 * 
 * This function implements the system scheduler
 */
void OS_Schedule(void)
{
    OS_task_t *current_aux;
    OS_Node_t *n;
    
    /*  
     * It is necessary to execute the OS_Schedule function without interrupts so
     * to ensure it we disable the interrupts
     */
    OS_DisableIRQ();
    
    KTRACE_EVENT(KEVENT_SCHEDULE_IN);
    
    SYSTRACE_TAM16(SYSTRACE_SCHEDULE, 0);

    while(OS_ListGetNum(&(sched_list[sched_queue_i])) == 0) {
        /*  Increment the queue index in order to obtain the next thread    */
//        sched_queue_i = (sched_queue_i + 1) & (KCONFIG_NUM_PRIORITIES - 1);
        sched_queue_i++;
        sched_queue_i %= (KCONFIG_NUM_PRIORITIES);
    }
    
    current_aux = current;
    
    /*  Extract the new current task    */
    n = OS_ListGetNode(&sched_list[sched_queue_i]);
    current = GET_ELEMENT(OS_task_t, n, node);
    
    current->status = TS_RUNNING;

    old_current = current_aux;

    /* DEBUG ("Selected %d as current task (prio %d); leaving %d as old_current",
        current - task_table, current->priority, old_current - task_table); */

    KTRACE_EVENT(KEVENT_SCHEDULE_OUT);

    schedule_active = 0;
}

void OS_SetPriority(tid_t _tid, priority_t _priority)
{
    ASSERT (_priority <= KCONFIG_MIN_PRIORITY);

    task_table[_tid].base_priority = _priority;
    
    if((task_table[_tid].pmutex) && (_priority < task_table[_tid].pmutex->prio))
        task_table[_tid].priority = task_table[_tid].pmutex->prio;
    else
        task_table[_tid].priority = _priority;

}

void OS_SetTmpPriority(tid_t _tid, priority_t _priority)
{
    ASSERT (_priority <= KCONFIG_MIN_PRIORITY);

    if (_priority > KCONFIG_MIN_PRIORITY)
        task_table[_tid].priority = KCONFIG_MIN_PRIORITY;
    else
        task_table[_tid].priority = _priority;
        
}

tid_t OS_GetCurrentTid(void)
{
    return current->tid;
}

void OS_SchedRemoveThread(tid_t _tid)
{
    OS_task_t *task = task_table + _tid;
    
    /* DEBUG ("Removing task %d from prq. %d", _tid, task->priority); */
    OS_ListExtract(&sched_list[task->priority], &(task->node));
    
}

priority_t OS_GetPriority(tid_t _tid)
{
    return task_table[_tid].priority;
}

void OS_RestorePriority(tid_t _tid)
{
    task_table[_tid].priority = task_table[_tid].base_priority;
}

priority_t OS_GetCurrentPrio(void)
{
    return current->priority;
}

//------------------------------------------------------------------------------

static void OS_SchedAddQueueTail(OS_task_t *_task)
{
    ASSERT (_task->priority <= KCONFIG_MIN_PRIORITY);

    /*  Task ready  */
    _task->status = TS_READY;

    /*  The task priority is greater than the queue index   */
    if (_task->priority < sched_queue_i) {
        sched_queue_i = _task->priority;
    }
        
    /*  Add task from the tail of the list  */
    /* DEBUG ("Appending task %d to the tail of prq. %d", _task - task_table, _task->priority); */
    OS_ListAppend(&sched_list[_task->priority], &(_task->node));
}

/*
 * Function:    void OS_SchedAddQueueHead(OS_task_t *_task)
 */
static void OS_SchedAddQueueHead(OS_task_t *_task)
{
    ASSERT (_task->priority <= KCONFIG_MIN_PRIORITY);

    /*  Task ready  */
    _task->status = TS_READY;
    
    /*  The task priority is greater than the queue index   */
    if (_task->priority < sched_queue_i)
        sched_queue_i = _task->priority;
        
    /*  Add task from the head of the list  */
    /* DEBUG ("Appending task %d to the head of prq. %d", _task - task_table, _task->priority); */
    OS_ListInsert(&sched_list[_task->priority], &(_task->node));
}

// vim:ts=4:sw=4:et:

