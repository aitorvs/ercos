/*
    Copyright 2005-2006 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

#include <arch/serial.h>
#include <arch/mec.h>
#include <arch/erc32.h>
//#include <arch/traps.h>

void putDebugChar(unsigned char car) {

    /*  Waiting fot the channelis ready */
    while(!(ERC32_MEC.uart_status & ERC32_MEC_UART_THEB));    //Ready to load data
    
    ERC32_MEC.uart_channel2 = car;
    
    /*  Wait for the correct character transmision  */
    while(!(ERC32_MEC.uart_status & ERC32_MEC_UART_THEB));  //  No data to send
}

int getDebugChar(void) {
    int car;
    
    /*  Waiting for ready data in the a channel */
    while(!(ERC32_MEC.uart_status & ERC32_MEC_UART_DRB));
    car = ERC32_MEC.uart_channel2;
    
    return car;
    
}

