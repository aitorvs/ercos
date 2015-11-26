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
 * \file include/kernel/irqev.h
 * \brief Kernel level MACROS and functions related to the IRQ event services.
 */

#ifndef _IRQ_H_
#define _IRQ_H_

#include <sys/types.h>

/**
 * \brief TIMER interrupt event number.
 */
#define CLOCK_EVENT TRAP_REAL_TIME_CLOCK

/**
 * \brief Events mask.
 */
#define EVENT_MASK  0xFF00

/**
 * \brief Event type.
 */
typedef uint16 event_t;

/**
 * \brief Init the IRQ event queue.
 */
void OS_IRQInitEvents(void);

/**
 * \brief Handle all pending IRQ events.
 */
void OS_IRQHandleEvent(void);

/**
 * \brief Marks the occurrence of an IRQ.
 * 
 * \param _sem  This parameter is the semaphore corresponding to the event
 *              that has occurred.
 */
void OS_IRQMarkEvent(OS_SemID_t _sem);

/**
 * \brief Marks the occurrence of the IRQ corresponding to the real time clock.
 */
void OS_IRQMarkClockEvent(void);
#endif


