/*
    Copyright 2006 (c) by Aitor Viana Sanchez, Pablo Parra Espada
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT on LEON3.
*/

#ifndef _GPTIMER_H_
#define _GPTIMER_H_

#include <autoconf.h>

/*
 * In this file we define the structure corresponding to the
 * Multiprocessor Interrupt Controller.
 */

#define GPTIMER_NUM_TIMERS 2

//----------------------------------------------------------------------------

struct timer_t
{
    unsigned int counter_value;
    unsigned int reload_value;
    unsigned int control;
    unsigned int dummy;
};

struct gptimer_t
{
	unsigned int scaler_value;                /* 00 */
	unsigned int scaler_reload_value;         /* 04 */
	unsigned int config_register;             /* 08 */
	unsigned int unused;                      /* 0C */
	struct timer_t timer[GPTIMER_NUM_TIMERS]; /* 10 */
};

/*  Defined in link.ld script file  */
extern volatile struct gptimer_t GPTIMER;

#endif
