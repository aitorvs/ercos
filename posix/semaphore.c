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
 * $Id: semaphore.c 1273 2008-03-13 17:29:27Z cesar $
 *  
 * $Revision: 1273 $
 * $Date: 2008-03-13 18:29:27 +0100 (jue, 13 mar 2008) $
 * Last CVS commit by $Author: cesar $
 * 
 * Registro de cambios
 * 
 * $Log$
 * Revision 1.2  2005/09/23 14:56:31  aitor
 * Use the _POSIX_Timespec_to_ticks() macro.
 *
 * Revision 1.1  2005/09/19 11:38:05  aitor
 * Restoring the project.
 *
 * Revision 1.2  2005/08/23 17:38:00  aitor
 * sem_timedwait() function implemented.
 *
 * Revision 1.1  2005/08/03 10:39:07  aitor
 * Added the POSIX semaphore interface.
 *
 */

#include <posix/semaphore.h>
#include <posix/time.h>
#include <kernel/irq.h>
#include <kernel/debug.h>
#include <syscall.h>

//------------------------------------------------------------------------------

int sem_init(sem_t *sem, int pshared, unsigned int value)
{
    return SYS_SemInit(sem, value);
}

int sem_destroy(sem_t * sem)
{
    return SYS_SemDestroy(sem);
}

int sem_wait(sem_t * sem)
{
    return SYS_SemWait(*sem);
}

int sem_trywait(sem_t * sem)
{
    return SYS_SemTryWait(*sem);
}

int sem_post(sem_t * sem)
{
    if(OS_IS_IN_TRAP_HANDLER()) {
        SYS_IRQMarkEvent(*sem);
    }
    else
        return SYS_SemPost(*sem);
    
    return 0;
}

int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout)
{
    OS_Time_t ticks;

    _POSIX_timespec_to_ticks(abs_timeout, ticks);
    return SYS_SemTimedWait(*sem, ticks);
}

