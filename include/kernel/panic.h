/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT on ERC-32.
*/

/*!
 * \file include/panic.h
 * \brief Kernel panic function.
 */

#ifndef _PANIC_H_
#define _PANIC_H_

/**
 * \brief Kernel panic function.
 * 
 * \param string  String to show as a warning message.
 * 
 * This function outputs the given message string and hangs the system as a
 * result of an irrecoverable error.
 */
void OS_KernelPanic (char *string);

#endif //_PANIC_H_

