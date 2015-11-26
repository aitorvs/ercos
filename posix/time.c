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
 * $Id: time.c 1269 2008-02-25 18:10:23Z cesar $
 *  
 * $Revision: 1269 $
 * $Date: 2008-02-25 19:10:23 +0100 (lun, 25 feb 2008) $
 * Last CVS commit by $Author: cesar $
 * 
 * Registro de cambios
 * 
 * $Log$
 * Revision 1.2  2005/09/23 14:57:58  aitor
 * _POSIX_Timespec_to_ticks() macro is used instead of direct conversion.
 *
 * Revision 1.1  2005/09/19 11:38:05  aitor
 * Restoring the project.
 *
 * Revision 1.7  2005/09/01 13:26:46  aitor
 * OS_GetTimer function is now named as OS_TimeGetTicks
 *
 * Revision 1.6  2005/09/01 11:31:15  aitor
 * The general purpose timer is now a periodic timer. The real-time clock is the one shot
 * system clock.
 *
 * Revision 1.5  2005/08/25 14:49:00  aitor
 * Modify the nanosleep() function in order to increase the performance.
 *
 * Revision 1.4  2005/08/25 08:39:39  aitor
 * Delete the clock function.
 *
 * Revision 1.3  2005/07/05 16:04:53  aitor
 * Implement the clock() and the clock_gettime() routines.
 *
 * Revision 1.2  2005/07/04 16:00:17  aitor
 * Added the nanosleep() implementation.
 *
 */

#include <posix/time.h>

#include <syscall.h>

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------

/*
 * Function:    int nanosleep(const struct timespec *req, struct timespec *rem)
 */
int nanosleep(const struct timespec *req, struct timespec *rem)
{
    uint16 ret_val = 0;
    clock_t clicks;
       
    _POSIX_timespec_to_ticks(req, clicks);
       
    SYS_Sleep(clicks);
    
    
    return ret_val;
}

int clock_gettime(clockid_t clk_id, struct timespec *res)
{
    OS_Timeval_t time;

    SYS_TimeGetTime(&time);
    
    switch(clk_id) {
        case(CLOCK_REALTIME):

            res->tv_nsec = time.useconds * 1000;
            res->tv_sec = time.seconds;

            return 0;
    }

    return -1;
}

#include <kernel/debug.h>

int clock_nanosleep (clockid_t clk_id, int flags, const struct timespec *req,
		struct timespec *rem) {
	clock_t us;

	/* only CLOCK_REALTIME clock is supported */
	if (clk_id != CLOCK_REALTIME) return -1;

	/* translate the requested time to ticks (us) */
	_POSIX_timespec_to_ticks (req, us);

	/* absolute sleep ? */
	if (flags & TIMER_ABSTIME) {
		SYS_AbsSleep (us);
		return 0;
	}

	/* relative sleep */
	SYS_Sleep (us);
	return 0;
}

