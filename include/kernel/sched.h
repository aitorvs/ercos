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
 * \file include/kernel/sched.h
 * \brief This file includes all the functions related to the real time
 * 		  scheduler.
 */

#ifndef _SCHED_H_
#define _SCHED_H_

#include <syscall.h>

//------------------------------------------------------------------------------

/**
 * \brief Scheduler function.
 * 
 * This function implements the operating system scheduler.
 */
void OS_Schedule(void);

/**
 * 
 * \brief Init the schedule structures.
 * 
 * This function initializes the schedule parameters.
 *  - Init the current thread pointed at idle thread TCB
 *  - Clear the \a need_reesched flag
 *  - Init the \a current_id identifier to 0
 */
void OS_SchedInit(void);

/**
 * 
 * \brief Adds the current thread into the sched queue grom the head.
 * 
 * The routine inserts the current thread into the head sched queue.
 */
void OS_SchedCurrentReady(void);

/**
 * \brief Adds a thread into the sched queue from the tail.
 * 
 * \param _task: The id of the thread to be inserted.
 * 
 * The routine inserts the specified thread into the tail sched queue.
 */
void OS_SchedTaskReady(tid_t _task);

/**
 * \brief Sets the priority to a certain thread.
 * 
 * \param _priority: The new priority.
 * \param _tid: The thread identifier.
 * 
 * This routine sets the new priority to the thread identified by \a _tid. The
 * priority is only changed if the new priority is minor than the old priority.
 */
void OS_SetPriority(tid_t _tid, priority_t _priority);

/**
 * \brief Sets the temporal priority to a certain thread.
 * 
 * \param _priority: The new priority.
 * \param _tid: The thread identifier.
 *
 * This routine sets the temporal priority to the thread identified by \a _tid.
 */
void OS_SetTmpPriority(tid_t _tid, priority_t _priority);

/**
 * \brief Obtains the priority of a certain thread.
 *
 * \param _tid: The thread identifier.
 * \return The priority of the thread.
 *
 * This routine obtains the priority of the thread identified by \a _tid.
 */
priority_t OS_GetPriority(tid_t _tid);

/**
 * \brief Obtains the priority of the current thread.
 *
 * \return The priority of the current thread.
 */
priority_t OS_GetCurrentPrio(void);

/**
 * \brief Restores the priority of a certain thread.
 *
 * \param _tid: The thread identifier.
 * 
 * This function restores the priority of the thread identified by \a _tid.
 */
void OS_RestorePriority(tid_t _tid);

/**
 * \brief Gets the identifier of the current thread.
 *
 * \return The identifier of the current thread.
 */
tid_t OS_GetCurrentTid(void);

/**
 * \brief Removes a certain thread from the ready queue.
 *
 * \param _tid: The id of the thread to be removed.
 */
void OS_SchedRemoveThread(tid_t _tid);

#endif

