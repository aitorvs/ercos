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
 * \file include/posix/semaphore.h
 * \brief POSIX semaphore management functions
 */

#ifndef _POSIX_SEMAPHORE_H_
#define _POSIX_SEMAPHORE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>

//------------------------------------------------------------------------------

/**
 * \brief Initializes the semaphore object pointed to by \a sem. 
 * 
 * \param sem     The semaphore to be initialized.
 * \param pshared Unused.
 * \param value   The semaphore counter value.
 * \return        Returns 0 on success or -1 on error.
 * 
 * The count associated with the semaphore is set initially to \a value.
 */
int sem_init(sem_t *sem, int pshared, unsigned int value);

/**
 * \brief Destroys the semaphore object pointed to by \a sem.
 * 
 * \param sem  The semaphore to be destroyed.
 * \return     Upon successful completion, a value of zero is returned. 
 *             Otherwise returns a value of -1.
 */
int sem_destroy(sem_t * sem);

/**
 * \brief Suspends the calling thread until the semaphore pointed to by \a sem
 *        has non-zero count.
 * 
 * \param sem  This parameter is the semaphore.
 * \return     Allways returns 0.
 * 
 * It then atomically decreases the semaphore count.
 */
int sem_wait(sem_t * sem);

/**
 * \brief Is a non-blocking variant of \a sem_wait(). 
 * 
 * \param sem  This parameter is the semaphore to be decremented.
 * \return     Returns 0 on success or -1 on error.
 * 
 * If the semaphore pointed to by \a sem has non-zero count, the count is 
 * atomically decreased and \a sem_trywait() immediately returns 0.  
 * If the semaphore count is zero,  \a sem_trywait() immediately returns with
 * error.
 */
int sem_trywait(sem_t * sem);

/**
 * \brief Atomically increases the count of the semaphore pointed to by \a sem. 
 * 
 * \param sem  This parameter is the semaphore to be decremented.
 * \return     If successful, the \a sem_post() function returns zero;
 *             otherwise, the function returns -1.
 * 
 * This function never blocks and can safely be used in asynchronous signal 
 * handlers.
 */
int sem_post(sem_t * sem);

/**
 * \brief Locks a semaphore specifying a maximum waiting timeout.
 * 
 * \param sem          Specifies the semaphore to be locked.
 * \param abs_timeout  Specifies how long to wait for the semaphore to become 
 *                     unlocked by another thread or process.
 * \return             When successful, the \a sem_timedwait() function returns
 *                     zero. Otherwise, the state of the semaphore is
 *                     unchanged, the function returns -1.
 * 
 * The \a sem_timedwait() function locks the specified semaphore as in the 
 * \a sem_wait() function. However, if the specified semaphore needs to be 
 * unlocked by another thread or process (with \a sem_post()) before it can be
 * locked, the \a sem_timedwait() function only waits for this unlock until the
 * timeout expires.
 * 
 * If the semaphore can be locked immediately, the \a abs_timeout argument is 
 * ignored.
 * 
 * Derived from IEEE Std 1003.1d-1999.
 */
int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout);

#ifdef __cplusplus
}
#endif


#endif

