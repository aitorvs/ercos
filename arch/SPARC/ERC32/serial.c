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
 * \file arch/sparc/ERC32/serial.c
 * \brief Definition of the serial input/output functions
 */

#include <arch/serial.h>
#include <arch/mec.h>
#include <arch/erc32.h>

//------------------------------------------------------------------------------

void CPU_SerialTx (char car)
{
    /* 
     * This variable is necessary in order to manipulate the ERC32_MEC.uart_status
     * in a local register instead of a global one....pa molar
     */
    volatile uint32 uart_status;

    /*  Waiting fot the channelis ready */
    while(!((uart_status = ERC32_MEC.uart_status) & ERC32_MEC_UART_THEA));
    
    ERC32_MEC.uart_channel1 = car;
    
    /*  Wait for the correct character transmision  */
    while(!((uart_status = ERC32_MEC.uart_status) & ERC32_MEC_UART_THEA));
}

void CPU_SerialRx (int *car)
{
    /* 
     * This variable is necessary in order to manipulate the ERC32_MEC.uart_status
     * in a local register instead of a global one....pa molar
     */
    volatile uint32 uart_status;

    /*  Waiting for ready data in the a channel */
    while(!((uart_status = ERC32_MEC.uart_status) & ERC32_MEC_UART_DRA));
    *car = ERC32_MEC.uart_channel1;
}

/*
 * 
 */
void CPU_SerialRxDebug(int _car)
{
    /* 
     * This variable is necessary in order to manipulate the ERC32_MEC.uart_status
     * in a local register instead of a global one....pa molar
     */
    volatile uint32 uart_status;

    /*  Waiting fot the channelis ready */
    while(!((uart_status = ERC32_MEC.uart_status) & ERC32_MEC_UART_THEB));
    
    ERC32_MEC.uart_channel2 = _car;
    
    /*  Wait for the correct character transmision  */
    while(!((uart_status = ERC32_MEC.uart_status) & ERC32_MEC_UART_THEB));
}

