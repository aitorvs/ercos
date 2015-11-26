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
 * \file include/posix/time.h
 * \brief POSIX time related functions.
 */

#ifndef _POSIX_TIME_H_
#define _POSIX_TIME_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <kernel/time.h>

//------------------------------------------------------------------------------

/*  Clock types */

/** Clock tick type */
typedef OS_Time_t clock_t;  // <sys/types.h>

//------------------------------------------------------------------------------

#define NSECS_PER_SEC 1000000000

/**
 * \brief Converts a timespec structure into clock ticks.
 * 
 * \param _timespec  The original timespec structure.
 * \param _ticks     The variable that will store the clock ticks value.
 */
#include <kernel/debug.h> /* DEBUG */
#define _POSIX_timespec_to_ticks(_timespec, _ticks)                     \
    do {                                                                \
       _ticks = (uint64) (_timespec)->tv_sec * CLICKS_PER_SEC;          \
       if ((_timespec)->tv_nsec > 1000) {                               \
           _ticks += (_timespec)->tv_nsec / 1000ul;                     \
       }                                                                \
       /*DEBUG ("ts->sec=%d ts->nsec=%d --> <%p %p> (%d)",              \
        (_timespec)->tv_sec,                                            \
        (_timespec)->tv_nsec, (long) (_ticks >> 32),                    \
        (long) (_ticks & 0xffffffff), (long) (_ticks & 0xffffffff));*/  \
    } while (0)

/**
 * \brief Converts a clock ticks value into a timespec structure.
 * 
 * \param _timespec  The timespec structure that will store the converted 
 *                   value.
 * \param _ticks     The original clock ticks value.
 */
#define _POSIX_ticks_to_timespec(_timespec, _ticks) \
    do { \
        _timespec->tv_sec = _ticks / (CLICKS_PER_SEC * 1); \
        _ticks -= _timespec->tv_sec * (CLICKS_PER_SEC * 1); \
        _timespec->tv_nsec = _ticks  * 1000000000 / (CLICKS_PER_SEC); \
    }while(0) \


/**
 * \brief Normalizes a timespec structure.
 * 
 * \param t  The timespect structure to normalize.
 * 
 * Normalizes a timespec structure, checking if the value of the nanoseconds
 * field is greater than a second.
 */
#define _POSIX_timespec_normalize(t) {\
    if ((t)->tv_nsec >= NSECS_PER_SEC) { \
     (t)->tv_nsec -= NSECS_PER_SEC; \
       (t)->tv_sec++; \
   } else if ((int)(t)->tv_nsec < 0) { \
       (t)->tv_nsec += NSECS_PER_SEC; \
       (t)->tv_sec--; \
   } \
}

/**
 * \brief Adds two timespec structures and stores the result in the first of
 *        them.
 * 
 * \param t1  Structure that will store the result of the addition and one of
 *            the addends.
 * \param t2  The other term.
 * 
 * After performing the addition, the resulting timespec structure is
 * normalized.
 */
#define _POSIX_timespec_add(t1, t2) do { \
 (t1)->tv_nsec += (t2)->tv_nsec;  \
 (t1)->tv_sec += (t2)->tv_sec; \
    _POSIX_timespec_normalize(t1);\
} while (0)

/**
 * \brief Subtracts two timespec structures and stores the difference in the
 *        first of them.
 * 
 * \param t1  Structure that will store the result of the substraction and the
 *            minuend.
 * \param t2  The subtrahend.
 * 
 * After performing the subtraction, the resulting timespec structure is
 * normalized.
 */
#define _POSIX_timespec_sub(t1, t2) do { \
  (t1)->tv_nsec -= (t2)->tv_nsec;  \
 (t1)->tv_sec -= (t2)->tv_sec; \
    _POSIX_timespec_normalize(t1);\
} while (0)

/**
 * \brief Adds to a timespec a certain value of nanoseconds.
 * 
 * \param t  The timespec structure.
 * \param n  The nanoseconds value.
 * 
 * After performing the addition, the resulting timespec structure is
 * normalized.
 */
#define _POSIX_timespec_add_ns(t,n) do { \
  (t)->tv_nsec += (n);  \
    _POSIX_timespec_normalize(t); \
} while (0)

/**
 * \brief Conditional MACRO that returns \a true if the timespec structure is 
 *        not zero.
 * 
 * \param t  The timespec structure.
 * \return   True if the timespec structure \a t is not zero.
 */
#define _POSIX_timespec_nz(t) ((t)->tv_sec != 0 || (t)->tv_nsec != 0)


/**
 * \brief Conditional MACRO that returns \a true if the first timespec
 *        structure is lesser than the second.
 * 
 * \param t1  First timespec structure.
 * \param t2  Second timespec structure.
 * \return    True if \a t1 is lesser than \a t2.
 */
#define _POSIX_timespec_lt(t1, t2) ((t1)->tv_sec < (t2)->tv_sec || ((t1)->tv_sec == (t2)->tv_sec && (t1)->tv_nsec < (t2)->tv_nsec))

/**
 * \brief Conditional MACRO that returns \a true if the first timespec
 *        structure is greater than the second.
 * 
 * \param t1  First timespec structure.
 * \param t2  Second timespec structure.
 * \return    True if \a t1 is greater than \a t2.
 */
#define _POSIX_timespec_gt(t1, t2) (_POSIX_timespec_lt(t2, t1))

/**
 * \brief Conditional MACRO that returns \a true if the first timespec
 *        structure is greater than or equal to the second.
 * 
 * \param t1  First timespec structure.
 * \param t2  Second timespec structure.
 * \return    True if \a t1 is greater than or equal to \a t2.
 */
#define _POSIX_timespec_ge(t1, t2) (!_POSIX_timespec_lt(t1, t2))

/**
 * \brief Conditional MACRO that returns \a true if the first timespec
 *        structure is lesser than or equal to the second.
 * 
 * \param t1  First timespec structure.
 * \param t2  Second timespec structure.
 * \return    True if \a t1 is lesser than or equal to \a t2.
 */
#define _POSIX_timespec_le(t1, t2) (!_POSIX_timespec_gt(t1, t2))

/**
 * \brief Conditional MACRO that returns \a true if the two timespec
 *        structures are equal.
 * 
 * \param t1  First timespec structure.
 * \param t2  Second timespec structure.
 * \return    True if \a t1 is equal to \a t2.
 */
#define _POSIX_timespec_eq(t1, t2) ((t1)->tv_sec == (t2)->tv_sec && (t1)->tv_nsec == (t2)->tv_nsec)


//------------------------------------------------------------------------------

/* for clock_nanosleep */
#define TIMER_ABSTIME		0x00000001

/**
 * \brief Delays the execution of the program for at least the time specified
 *        in \a req.
 * 
 * \param req  This parameter stores the delay time.
 * \param rem  writes the remaining time into the structure pointed to by 
 *             \a rem unless \a rem is \a NULL.
 * \return     In case of an error or exception, the \a nanosleep() system call
 *             returns -1 instead of 0.
 * 
 * Conforming to: POSIX.1b (formerly POSIX.4).
 * The function can return earlier if a signal has been delivered to the process.
 */
int nanosleep(const struct timespec *req, struct timespec *rem);

/**
 * \brief This function can perform both a relative 
 * mark specified in \a req.
 * 
 * \param clk_id	The identifier of the particular clock on which to act.
 *			Currently only CLOCK_REALTIME is implemented.
 * \param flags		Flags modifier. If TIMER_ABSTIME is set, the \a \req
 *			defines an absolute time mark, else a relative time.
 * \param req		This parameter stores the requested time. It may be
 *			relative (like in nanosleep) if TIMER_ABSTIME is not
 *			set or it may point to a time mark until which the
 *			thread has to sleep, if TIMER_ABSTIME bit is set.
 * \param rem		The remaining time not consumed in the call. Currently
 *			this value is never updated.
 * \return		A 0 value is always returned. The function returns -1
 *			only if a clock identifier different from
 *			CLOCK_REALTIME is requested.
 */
int clock_nanosleep (clockid_t clk_id, int flags, const struct timespec *req,
		struct timespec *rem);

/**
 * \brief Retrieves the time of the specified clock \a clk_id.
 * 
 * \param res     In this parameter the routin stores the current time.
 * \param clk_id  The identifier of the particular clock on which to act.
 * \return        Return 0 for success, or -1 for failure.
 * 
 * Conforming to: SUSv2, POSIX 1003.1-2001.
 */
int clock_gettime(clockid_t clk_id, struct timespec *res);

#ifdef __cplusplus
}
#endif

#endif

