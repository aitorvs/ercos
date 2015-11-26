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
 * \file arch/sparc/LEON3/cpu.c
 * \brief CPU Initialization function definition.
 */

//------------------------------------------------------------------------------

#include <target.h>

//------------------------------------------------------------------------------

/*
 * Function:    void CPU_Init(void)
 */
void CPU_Init(void)
{
    GPTIMER.timer[GPTIMER_COUNTER].control = 0;
    GPTIMER.timer[GPTIMER_REAL_TIME].control = 0;
}
