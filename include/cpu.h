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
 * \file include/cpu.h
 * \brief This file contains all the header files related with the CPU
 */

#ifndef _CPU_INCLUDES_
#define _CPU_INCLUDES_


#ifdef CONFIG_ERCOS_SIMU

#include <simu_stub.h>

#define CPU_TIMER_TRAP  0               //  in cpu/irq.h
#define CPU_CLOCK_TRAP  0               //  in cpu/irq.h
#define CPU_TIMER_RESOLUTION    0       //  in timer.h
#define CPU_TIMER_COUNTER   0           //  in timer.h
#define CPU_NUMBER_OF_INTERRUPTS   0    //  in irq.h
#define CPU_TRAP_MIN_NUMBER 0           //  in irq.h
#define CPU_TRAP_MAX_NUMBER 0           //  in irq.h
#define CPU_SERIAL_TRAP     0           //  in 

#include <cpu/asm.h>
#include <cpu/cpu.h>
#include <cpu/cputypes.h>
#include <cpu/psr.h>

#else 

#include <cpu/arch.h>

#endif



#endif  // _CPU_INCLUDES_

