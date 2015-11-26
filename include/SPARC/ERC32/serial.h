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
 * \file include/sparc/ERC32/serial.h
 * \brief Contains all the necessary functions to manage the serial
 *        transmission.
 */

#ifndef _TARGET_SERIAL_H_
#define _TARGET_SERIAL_H_

#include <kernel/config.h>

/** \brief Mask for the UART A status register   */
#define CPU_SERIAL_STATUS_DR    ERC32_MEC_UART_DRA


/**
 * \brief Verifies if there is some data ready in the UART A.
 */
#define CPU_SERIAL_DATA_READY() (ERC32_MEC.uart_status & CPU_SERIAL_STATUS_DR)

/****************   FUNCTIONS   ***********************/

/**
 * \brief Transmits a character through the serial line.
 * 
 * \param car  This parameter is the character to be transmited.
 */
void CPU_SerialTx (char car);

/**
 * \brief Receives a character from the serial line.
 * 
 * \param car  This parameter is the character to be received.
 */
void CPU_SerialRx (int *car);

/**
 * \brief Receives a serial character through the serial debug line.
 */
void CPU_SerialRxDebug(int _car);




#endif //   #ifndef _TARGET_SERIAL_H_

