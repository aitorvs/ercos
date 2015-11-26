/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez, Pablo Parra Espada
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT on LEON3.
*/

#ifndef _MCTRL_H_
#define _MCTRL_H_

#include <autoconf.h>

/* In this file we define the structure corresponding to the
 * memory control unit.
 */

//----------------------------------------------------------------------------

struct mctrl
{
	unsigned int memory_control_1;          /* 00 */
	unsigned int memory_control_2;          /* 04 */
	unsigned int memory_control_3;          /* 08 */
};

/*  Defined in link.ld script file  */
extern volatile struct mctrl MCTRL;

#endif
