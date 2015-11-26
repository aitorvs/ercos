/*
    Copyright 2006 (c) by Aitor Viana Sanchez, Pablo Parra Espada
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT on LEON3.
*/

#ifndef _IRQMP_H_
#define _IRQMP_H_

#include <autoconf.h>

/*
 * In this file we define the structure corresponding to the
 * Multiprocessor Interrupt Controller.
 */

//----------------------------------------------------------------------------

struct irqmp
{
	unsigned int irq_level;                 /* 00 */
	unsigned int irq_pending;               /* 04 */
	unsigned int irq_force;                 /* 08 */
	unsigned int irq_clear;                 /* 0C */
	unsigned int mp_status;                 /* 10 */
	unsigned int foo[11];			/* 14 */
	unsigned int irq_mask_processor[16];    /* 40 */
	unsigned int irq_force_processor[16];	/* 80 */
};

/*  Defined in link.ld script file  */
extern volatile struct irqmp IRQMP;

#endif
