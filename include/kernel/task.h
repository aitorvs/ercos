/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

/*!
 * \file include/kernel/task.h
 * \brief Kernel level thread management functions.
 */

#ifndef _THREAD_H_
#define _THREAD_H_

#include <sys/types.h>

//------------------------------------------------------------------------------
/* This table stores the Task Control Block for all the running tasks */
extern OS_task_t task_table[KCONFIG_MAX_THREADS];

/* This variable points to the task currently running */
extern OS_task_t *current;

/* This variable points to the task that was running before the current task
 * was updated (see OS_Schedule) */
extern OS_task_t *old_current;


//------------------------------------------------------------------------------

/**
 * \brief This function initializes all the system TCBs.
 * 
 * The routine also init the idle thread TCB.
 */
void OS_TCBInit(void);

/**
 * \brief Returns the \a status parameter of a given thread.
 * 
 * \param _tid  ID of the thread.
 * \return      \a Status word.
 * 
 * The \a status parameter can take the following values:
 * - TS_READY: The thread is ready to get the processor and is enqueued in the
 *             ready queue.
 * - TS_RUNNING: The thread is currently running and not present in the ready
 *               queue.
 * - TS_SEM_BLOCKED: The thread is currently blocked in a semaphore queue.
 * - TS_TIME_BLOCKED: The thread is currently blocked in the time queue.
 */
int OS_TaskGetStatus(tid_t _tid);

/**
 * \brief Frees all the TCB entries of the current thread and erases all its
 *        information from the system.
 */
void OS_ThreadExit(void);

/**
 * Creates a thread.
 * 
 * \param _stack_size  This parameter is the thread stack size.
 * \param _ip          This parameter is the thread entry point.
 * \param _priority    This parameter is the thread priority.
 * \return             The function returns the thread identifier.
 * 
 * The routine init the thread TCB:
 *  - init the thread hardware context.
 *  - stablish the thread priority.
 *  - stablish the thread identifier.
 *  - stablish the thread status to TS_READY.
 *
 */
tid_t OS_TaskCreate(uint32 *_stackaddr, uint32  _stacksize, void *_ip, void *_arg, uint8 _priority);

/**
 * \brief Obtains the stack parameters (address and size) of the current
 *        thread.
 * 
 * \param _stackaddr  Initial stack address.
 * \param _stacksize  Size of the stack.
 */
void OS_TaskGetStackParameters(uint32 *_stackaddr, uint32 *_stacksize);

/**
 * \brief Returns true if the task specified in the parameter is blocked.
 * 
 * \param _tid  This parameter is the thread identifier.
 * \return      The routine returns true in case the thread is blocked and
 *              false in any other case.
 */
int OS_TaskIsBlocked(tid_t _tid);

#endif
