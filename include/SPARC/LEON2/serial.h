/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez, Pablo Parra Espada
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT on LEON3.
*/

#ifndef _TARGET_SERIAL_H_
#define _TARGET_SERIAL_H_

#include <kernel/config.h>

/** This label is the mask for the UART A status register   */
#define CPU_SERIAL_STATUS_DR    LEON2_UART_DR

/**
 * This macro verifies if there is some data ready in the UART A
 */
#define CPU_SERIAL_DATA_READY() (REGMAP.uart_1_status & CPU_SERIAL_STATUS_DR)

/****************   FUNCTIONS   ***********************/

/**
 * This routine transmits a character through the serial line.
 * 
 * \param   car:    This parameter is the character to be transmited.
 */
void CPU_SerialTx (char car);

/**
 * This routine receives a character from the serial line.
 * 
 * \param   car:    This parameter is the character to be received.
 */
void CPU_SerialRx (int *car);

/**
 * \todo documentar esta función
 */
void CPU_SerialRxDebug(int _car);




#endif //   #ifndef _TARGET_SERIAL_H_

