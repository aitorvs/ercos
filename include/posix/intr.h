/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT project.
*/

/*!
 * \file include/posix/intr.h
 * \brief POSIX functions to handle interruptions
 */
 
#ifndef _POSIX_INTR_H_
#define _POSIX_INTR_H_

#ifdef __cplusplus
extern "C" {
#endif


#include <sys/types.h>

//----------------------------------------------------------------------------

typedef uint32 intr_t;

//----------------------------------------------------------------------------

/**
 * \brief This function mask a hardware interrupt.
 * 
 * \param intr  This parameter is the hardware interrupt vector number.
 * \return      The funcion always returns 0.
 * 
 * This function is POSIX compliant with the P1003.4b/D8, p. 76.
 */
int intr_lock(intr_t  intr);

/**
 * \brief This function unmask a hardware interrupt.
 * 
 * \param intr  This parameter is the hardware interrupt vector number.
 * \return      The funcion always returns 0.
 * 
 * This function is POSIX compliant with the P1003.4b/D8, p. 74.
 */
int intr_unlock(intr_t  intr);

/**
 * \brief Registers a handler routine for a determined HW IRQ.
 * 
 * \param intr_handler  Handler routine.
 * \param level         HW IRQ level.
 * \return              Returns 0 on success, -1 if not.
 */
int intr_capture(void (*intr_handler)(int _level), int level);

/**
 * \brief Restores the previous interrupt handler.
 * 
 * \param level  HW IRQ level.
 * \return       Returns 0 on success, -1 if not.
 */
int intr_restore(int level);

/**
 * \brief Forces an interrupt given as a parameter.
 * 
 * \param level  HW IRQ level.
 * \return       Always return 0.
 */
int intr_force(int level);

#ifdef __cplusplus
}
#endif



#endif

