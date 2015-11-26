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
 * \file include/posix/pthread.h
 * \brief POSIX threads management functions
 */

#ifndef _POSIX_PTHREAD_H_
#define _POSIX_PTHREAD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>

//------------------------------------------------------------------------------

/**
 * \brief Identifies the inherit policy in the mutex
 */
#define PTHREAD_PRIO_NORMAL    MUTEX_PRIO_NORMAL

/**
 * \brief Identifies the inherit policy in the mutex
 */
#define PTHREAD_PRIO_INHERIT    MUTEX_PRIO_INHERIT

/**
 * \brief Identifies the priority ceiling policy in the mutex
 */
#define PTHREAD_PRIO_PROTECT    MUTEX_PRIO_CEILING

/**
 * \brief Identifies the normal mutex semaphore type
 */
#define PTHREAD_MUTEX_NORMAL    0

//------------------------------------------------------------------------------

/**
 * \brief Initializes the thread attributes object.
 * 
 * \param attr  The structure to initialize.
 * \return      Upon successful completion, returns a value of 0; otherwise, 
 *              an error number shall be returned to indicate the error.
 */
int pthread_attr_init(pthread_attr_t *attr);

/**
 * \brief Creates a new thread.
 * 
 * \param thread         ID of the created thread.
 * \param attr           Attributes of the new thread.
 * \param start_routine  Starting function of the new thread.
 * \param arg            Argument passed to the start routine.
 * \return               If successful, the pthread_create() function returns
 *                       zero; otherwise, an error number shall be returned to
 *                       indicate the error.
 */
int pthread_create(pthread_t  *thread,                  const pthread_attr_t *attr, 
                   void       *(*start_routine)(void*), void  *arg);

/**
 * \brief Set stacksize attribute.
 * 
 * \param attr        This parameter is the attr object where the stack size is
 *                    initialized
 * \param stacksize:  This parameter is the thread stack size to be initalized
 *                    with.
 * \return            Upon successful completion, 
 *                    \a pthread_attr_setstacksize() returns a value of 0. 
 *                    Otherwise, an error number is returned to indicate the
 *                    error.
 * 
 * This function sets the thread creation stacksize attribute in the \a attr 
 * object.
 * Derived from the POSIX Threads Extension (1003.1c-1995)
 */
int pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);

/**
 * \brief Set stackaddr attribute.
 * 
 * \param attr       This parameter is the attr object where the stack size is
 * initialized
 * \param stackaddr  This parameter specifies the location of storage to be 
 *                   used for the created thread's stack. The size of the 
 *                   storage is at least PTHREAD_STACK_MIN.
 * \return           Upon successful completion, \a pthread_attr_setstackaddr()
 *                   returns a value of 0. Otherwise, an error number is 
 *                   returned to indicate the error.
 * 
 * This routine sets the thread creation stackaddr attribute in the \a attr 
 * object.
 * Derived from the POSIX Threads Extension (1003.1c-1995)
 */
int pthread_attr_setstackaddr(pthread_attr_t *attr, void *stackaddr);

/**
 * \brief Set schedparam attribute.
 * 
 * \param attr   The attr argument.
 * \param param  The contents of the param structure are defined in <sched.h>.
 * \return       If successful, the \a pthread_attr_setschedparam() functions 
 *               returns zero. Otherwise, an error number is returned to 
 *               indicate the error. 
 * 
 * This routine sets the scheduling parameter attributes in the \a attr 
 * argument.
 */
int pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param);

/**
 * \brief Retrieves the scheduling policy and scheduling parameters of a given
 *        thread.
 * 
 * \param thread  ID of the thread.
 * \param policy  Stores the scheduling policy of the thread.
 * \param param   Stores the scheduling parameters of the thread.
 * \return        If  successful, \a pthread_getschedparam() returns zero; 
 *                otherwise, an error number shall be returned to indicate the
 *                error.
 * 
 * The contents of the param structure are defined in <sched.h>.
 */
int pthread_getschedparam(pthread_t thread, int *policy, struct sched_param *param);

/**
 * \brief Sets the scheduling policy and scheduling parameters of a given
 *        thread.
 * 
 * \param thread  ID of the thread.
 * \param policy  The new scheduling policy of the thread.
 * \param param   The new scheduling parameters of the thread.
 * \return        If  successful, \a pthread_getschedparam() returns zero;
 *                otherwise, an error number shall be returned to indicate the
 *                error.
 * 
 * The contents of the param structure are defined in <sched.h>.
 */
int pthread_setschedparam(pthread_t thread, int policy, const struct sched_param *param);


/**
 * \brief Initialize the mutex attributes object with the default values.
 * 
 * \param attr  mutex attributes object reference.
 * \return      Upon successful completion, pthread_mutexattr_init() returns
 *              zero; otherwise, an error number is returned to indicate the
 *              error.
 */
int pthread_mutexattr_init(pthread_mutexattr_t *attr);

/**
 * \brief Sets the mutex type attribute.
 * 
 * \param attr  Attribute object to be modified.
 * \param kind  New type for the mutex.
 * \return      If successful, the pthread_mutexattr_settype() function returns
 *              zero; otherwise, an error number is returned to indicate the 
 *              error.
 */
int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int kind);

/**
 * \brief Sets the mutex prolotocol attribute.
 * 
 * \param attr  Attribute object to be modified.
 * \param kind  New protocol for the mutex.
 * \return      If successful, the \a pthread_mutexattr_setprotocol() function
 *              returns zero; otherwise, an error number is returned to
 *              indicate the error.
 * 
 * ERCOS-RT defines the following protocols for the mutexes:
 * - PTHREAD_PRIO_NORMAL: The priority and scheduling is not  affected  by  the
 *     mutex ownership.
 * - PTHREAD_PRIO_INHERIT: Inherit priority scheduling policy.
 * - PTHREAD_PRIO_PROTEC: Priority ceiling scheduling policy.
 */
int pthread_mutexattr_setprotocol(pthread_mutexattr_t *attr, int kind);

/**
 * \brief Initializes a mutex semaphore.
 * 
 * \param mutex      Reference to the mutex semaphore.
 * \param mutexattr  Attributes of the semaphore.
 * \return           If successful, returns zero; otherwise, an error number 
 *                   shall be returned to indicate the error.
 */
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *mutexattr);

/**
 * \brief Locks a mutex semaphore.
 * 
 * \param mutex  Reference to the mutex to lock.
 * \return       If successful, the \a pthread_mutex_lock() function returns
 *               zero; otherwise, an error number is returned to indicate the
 *               error.
 */
int pthread_mutex_lock(pthread_mutex_t *mutex);

/**
 * \brief Unblockable version of \a pthread_mutex_lock().
 * 
 * \param mutex  Reference to the mutex to lock.
 * \return       Returns zero if a lock on the mutex semaphore is acquired. 
 *               Otherwise, an error number is returned to indicate the error.
 * 
 *  Equivalent to \a pthread_mutex_lock(), except that if the mutex object 
 *  referenced by mutex is currently locked (by any thread, including the 
 *  current thread), the call returns immediately.
 */
int pthread_mutex_trylock(pthread_mutex_t *mutex);

/**
 * \brief Unlocks a mutex semaphore.
 * 
 * \param mutex  Reference to the mutex to unlock.
 * \return       If successful, the \a pthread_mutex_unlock() function returns
 *               zero; otherwise, an error number is returned to indicate the 
 *               error.
 */
int pthread_mutex_unlock(pthread_mutex_t *mutex);

/**
 * \brief Destroys the mutex object referenced by mutex.
 * 
 * \param mutex  Reference to the mutex to destroy.
 * \return       If successful, returns zero; otherwise, an error number is
 *               returned to indicate the error.
 */
int pthread_mutex_destroy(pthread_mutex_t *mutex);

/**
 * \brief Thread-specific data key creation
 * 
 * \param key         Stores the thread-specific data
 * \param destructor  Optional function that can be called when destroying
 *                    the thread.
 * \return            If successful, the \a pthread_key_create() function
 *                    stores the newly created key value at \a key and returns
 *                    zero. Otherwise, an error number is returned to indicate
 *                    the error.
 */
int pthread_key_create(pthread_key_t *key, void (*destructor)(void *));

/**
 * \brief Associates a thread-specific value with a key obtained via a previous
 *        call to \a pthread_key_create()
 * 
 * \param key    Specifies the key to which the value is bound.
 * \param value  Specifies the new thread-specific value.
 * \return       If successful, the \a pthread_setspecific() function returns 
 *               zero; otherwise, an error number is returned to indicate the
 *               error.
 * 
 * The \a pthread_setspecific() function associates a thread-specific value 
 * with a key obtained via a previous call to \a pthread_key_create(). 
 * Different threads may bind different values to the same key. 
 * These values are typically pointers to blocks of dynamically allocated 
 * memory that have been reserved for use by the calling thread.
 */
int pthread_setspecific(pthread_key_t key, const void *value);

/**
 * \brief Return the value currently bound to the specified key on behalf of
 *        the calling thread.
 * 
 * \param key  Specifies the key to which the value is bound.
 * \return     Returns the thread-specific data value associated with the given
 *             key. If no thread-specific data value is associated with key, 
 *             then the value \a NULL is returned.
 * 
 * The \a pthread_getspecific() function returns the value currently bound
 * to the specified key on behalf of the calling thread.
 */
void *pthread_getspecific(pthread_key_t key);

/**
 * \brief Thread-specific data key deletion
 * 
 * \param key  Specifies the key to delete.
 * \return     If successful, the pthread_key_delete function returns zero.
 *             Otherwise, an error number is returned to indicate the error.
 * 
 * The \a pthread_key_delete() subroutine deletes the thread-specific data key,
 * previously created with the \a pthread_key_create() subroutine. The
 * application must ensure that no thread-specific data is associated with the
 * key. No destructor routine is called.
 */
int pthread_key_delete(pthread_key_t key);

/**
 * \brief Sets the prioceiling attribute of the mutex attributes object.
 * 
 * \param attr         Specifies the mutex attributes object.
 * \param prioceiling  Specifies the priority ceiling to set. It must be in 
 *                     the range from 1 to 127, where 1 is the least favored
 *                     priority, and 127 the most favored; it should be the
 *                     priority of the highest priority thread that can lock
 *                     the mutex lock, or higher. Setting a lower value may
 *                     cause priority inversions.
 * \return             Upon successful completion, 0 is returned. Otherwise,
 *                     an error code is returned.
 * 
 * The \a pthread_mutexattr_setprioceiling() subroutine sets the value of
 * the prioceiling attribute of the mutex attributes object \a attr. This
 * attribute specifies the priority ceiling of the mutex created with
 * this attributes object, used in the priority protection protocol to
 * prevent priority inversions. It has no meaning if the mutex uses another
 * protocol.
 */
int pthread_mutexattr_setprioceiling (pthread_mutexattr_t *attr, int prioceiling);

/**
 * \brief Gets the prioceiling attribute of the mutex attributes object.
 * 
 * \param attr         Specifies the mutex attributes object.
 * \param prioceiling  Points to where the prioceiling attribute value will be
 *                     stored.
 * \return             Upon successful completion, the value of the prioceiling
 *                     attribute is returned via the prioceiling parameter, and
 *                     0 is returned. Otherwise, an error code is returned.
 * 
 * The \a pthread_mutexattr_getprioceiling() subroutine returns the value
 * of the prioceiling attribute of the mutex attributes object \a attr.
 * This attribute specifies the priority ceiling of the mutex created
 * with this attributes object, used in the priority protection protocol
 * to prevent priority inversions. It has no meaning if the mutex uses
 * another protocol.
 */
int pthread_mutexattr_getprioceiling (pthread_mutexattr_t *attr, int *prioceiling);

#ifdef __cplusplus
}
#endif

#endif

