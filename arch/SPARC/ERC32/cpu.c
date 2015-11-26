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
 * \file arch/sparc/ERC32/cpu.c
 * \brief CPU Initialization function definition.
 */

//------------------------------------------------------------------------------

#include <target.h>

//------------------------------------------------------------------------------

extern uint32 __ERC32_MEC_Timer_Control_Mirror;
extern uint32 ERC32_RAM_START[];
extern uint32 CODE_END[];

//------------------------------------------------------------------------------

/*
 * Function:    void CPU_Init(void)
 */
void CPU_Init(void)
{
    __ERC32_MEC_Timer_Control_Mirror = 0;
    ERC32_MEC.timer_control = 0;
    /* Enable write protection in user and supervisor mode */
    ERC32_MEC.ap_segment1_base = 0x1800400;
    ERC32_MEC.ap_segment1_end = ((uint32)((CODE_END - ERC32_RAM_START) >> 2));
}

