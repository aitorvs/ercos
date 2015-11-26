/*
    Copyright 2006 (c) by Aitor Viana Sanchez, Pablo Parra Espada
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT on LEON3.
*/

#ifndef _REGMAP_H_
#define _REGMAP_H_

#include <autoconf.h>

//----------------------------------------------------------------------------
/* Here we include the definition of the structures of the components
 * that are included in the system */

/* APB UARTA */
#include <arch/apbuart.h>

/* MCTRL Memory Controller */
#include <arch/mctrl.h>

/* IRQMP IRQ MP Controller */
#include <arch/irqmp.h>

/* GPTIMER General Purpose Timers */
#include <arch/gptimer.h>

#endif
