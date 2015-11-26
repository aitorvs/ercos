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
 * \file include/kernel/serial.h
 * \brief Functions related to the serial communications line.
 */

#ifndef _KERNEL_SERIAL_H_
#define _KERNEL_SERIAL_H_

#include <kernel/irq.h>
#include <target.h>

//------------------------------------------------------------------------------

/**
 * \brief This macro verifies if there is any data ready in the serial line
 *        device
 */
#define IS_SERIAL_DATA_READY()  CPU_SERIAL_DATA_READY()


//------------------------------------------------------------------------------

/**
 * \brief Initializes the serial line
 */
void OS_InitSerialLine();

#endif  //#ifndef _SERIAL_H_

