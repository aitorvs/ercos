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
 * \file arch/sparc/LEON3/halt.c
 * \brief LEON3 CPU reset and hatling functions
 */

#include <cpu.h>

/** \brief Halts the system by writing on a special ancillary register. */
void CPU_PowerDown(void)
{
    //    BUG: We can't disable the interrupts because otherwise we the
    //    processor will never exit the power-down mode and hang infinitely
//    _CPU_DisableIRQ();
    asm volatile( "wr %%g0, %%asr19" ::);
}

/** \brief Resets the system. Currently UNIMPLEMENTED */
void CPU_Reset(void)
{
    /* UNIMPLEMENTED */
}
