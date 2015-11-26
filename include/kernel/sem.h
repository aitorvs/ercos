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
 * \file include/kernel/sem.h
 * \brief Kernel layer MACROS and functions related to the management of the
 *        binary semaphores.
 */

#ifndef _SEM_H_
#define _SEM_H_

#include <sys/types.h>

//------------------------------------------------------------------------------

/**
 * \brief Identifies the semaphore value when it is free.
 */
#define SEM_FREE    255

//------------------------------------------------------------------------------

/**
 * \brief Initializes a certain binary semaphore.
 * 
 * \param _sem    Reference to the semaphore that is being initialized.
 * \param _value  Initial value of the semaphore.
 * \return        Always returns zero.
 */
int OS_SemInit(OS_SemID_t *_sem, uint32 _value);

/**
 * \brief Resets a certain binary semaphore.
 * 
 * \param _sem  Reference to the semaphore.
 * return       Always returns zero.
 */
int OS_SemDestroy(OS_SemID_t *_sem);

/**
 * \brief Performs a post (unlock) to a certain binary semaphore.
 * 
 * \param _sem  The semaphore to unlock.
 * \return      Always returns zero.
 */
int OS_SemPost(OS_SemID_t _sem);

/**
 * \brief Performs a wait (lock) to a certain binary semaphore.
 * 
 * \param _sem  The semaphore to lock.
 * \return      Returns zero if the semaphore was free, -1 otherwise.
 * 
 * In this case, if the semaphore was already catched, the function inserts
 * the current thread into the semaphore's waiting queue.
 */
int OS_SemWait(OS_SemID_t _sem);

/**
 * \brief Performs a non-blocking wait to a certain binary semaphore.
 * 
 * \param _sem  The semaphore to lock.
 * \return      Returns zero if the thread obtains the semaphore, -1
 *              otherwise.
 * 
 * In this case, the current thread is never blocked, even if the semaphore was
 * already catched.
 */
int OS_SemTryWait(OS_SemID_t _sem);

/**
 * \brief Checks if the semaphore has already been catched.
 * 
 * \param _sem  The semaphore to check.
 * \return      True if the semaphore was catched, false if not.
 */
int OS_SemIsCatched(OS_SemID_t _sem);

void OS_SemExtracThreadFromList(OS_SemID_t _sem, OS_task_t *task);

#endif

