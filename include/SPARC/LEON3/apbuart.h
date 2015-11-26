/*
    Copyright 2006 (c) by Aitor Viana Sanchez, Pablo Parra Espada
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT on LEON3.
*/

#ifndef _APBUART_H_
#define _APBUART_H_

#include <autoconf.h>

/*
 * In this file we define the structure corresponding to the
 * UART with the APB (Advanced Peripheral Bus) Interface.
 */

//----------------------------------------------------------------------------

struct apbuart
{
	unsigned int data;                      /* 00 */
	unsigned int status;                    /* 04 */
	unsigned int control;                   /* 08 */
	unsigned int scaler;                    /* 0C */
};

/*  Defined in link.ld script file  */
extern volatile struct apbuart UARTA;

#endif
