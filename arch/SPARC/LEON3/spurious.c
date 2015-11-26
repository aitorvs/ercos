/*
    Copyright 2006 (c) by Aitor Viana Sanchez, Pablo Parra Espada
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

/*!
 * \file arch/sparc/LEON3/spurious.c
 * \brief Definition of the trap tracing function.
 */

#include <arch/spurious.h>
#include <kernel/debug.h>

/** \brief Array containing the different trap error messages */
static const char traps[64][64] =
{
  "\n*** TRAP 00: RESET ***\n",
  "\n*** TRAP 01: INSTRUCTION ACCESS ***\n",
  "\n*** TRAP 02: ILLEGAL INSTRUCTION ***\n",
  "\n*** TRAP 03: PRIVILEGED INSTRUCTION ***\n",
  "\n*** TRAP 04: FLOATING POINT DISABLED ***\n",
  "\n*** TRAP 05: WINDOW OVERFLOW ***\n",
  "\n*** TRAP 06: WINDOW UNDERFLOW ***\n",
  "\n*** TRAP 07: MEMORY ACCESS NOT ALIGMENT ***\n",
  "\n*** TRAP 08: FLOATING POINT EXCEPTION ***\n",
  "\n*** TRAP 09: DATA ACCESS EXCEPTION ***\n",
  "\n*** TRAP 0A: TAG OVERFLOW ***\n",
  "\n*** TRAP 0B: WATCHPOINT DETECTED ***\n",
  "\n*** TRAP 0C: UNHANDLED TRAP ***\n",
  "\n*** TRAP 0D: UNHANDLED TRAP ***\n",
  "\n*** TRAP 0E: UNHANDLED TRAP ***\n",
  "\n*** TRAP 0F: UNHANDLED TRAP ***\n",
  "\n*** TRAP 10: UNHANDLED TRAP ***\n",
  "\n*** TRAP 11: ASYNCHRONOUS IRQ 1 ***\n",
  "\n*** TRAP 12: ASYNCHRONOUS IRQ 2 ***\n",
  "\n*** TRAP 13: ASYNCHRONOUS IRQ 3 ***\n",
  "\n*** TRAP 14: ASYNCHRONOUS IRQ 4 ***\n",
  "\n*** TRAP 15: ASYNCHRONOUS IRQ 5 ***\n",
  "\n*** TRAP 16: ASYNCHRONOUS IRQ 6 ***\n",
  "\n*** TRAP 17: ASYNCHRONOUS IRQ 7 ***\n",
  "\n*** TRAP 18: ASYNCHRONOUS IRQ 8 ***\n",
  "\n*** TRAP 19: ASYNCHRONOUS IRQ 9 ***\n",
  "\n*** TRAP 1A: ASYNCHRONOUS IRQ 10 ***\n",
  "\n*** TRAP 1B: ASYNCHRONOUS IRQ 11 ***\n",
  "\n*** TRAP 1C: ASYNCHRONOUS IRQ 12 ***\n",
  "\n*** TRAP 1D: ASYNCHRONOUS IRQ 13 ***\n",
  "\n*** TRAP 1E: ASYNCHRONOUS IRQ 14 ***\n",
  "\n*** TRAP 1F: ASYNCHRONOUS IRQ 15 ***\n",
  "\n*** TRAP 20: UNHANDLED TRAP ***\n",
  "\n*** TRAP 21: UNHANDLED TRAP ***\n",
  "\n*** TRAP 22: UNHANDLED TRAP ***\n",
  "\n*** TRAP 23: UNHANDLED TRAP ***\n",
  "\n*** TRAP 24: COPROCESSOR DISABLED ***\n",
  "\n*** TRAP 25: UNHANDLED TRAP ***\n",
  "\n*** TRAP 26: UNHANDLED TRAP ***\n",
  "\n*** TRAP 27: UNHANDLED TRAP ***\n",
  "\n*** TRAP 28: COPROCESSOR EXCEPTION ***\n",
  "\n*** TRAP 29: UNHANDLED TRAP ***\n",
  "\n*** TRAP 2A: DIVIDE EXCEPTION ***\n",
  "\n*** TRAP 2B: WRITE BUFFER ERROR ***\n",
};

/*
 * Function: void trace_trap (unsigned long vec, unsigned long pc)
 * 
 * Traces an empty trap.
 */
void CPU_TraceTraps (unsigned long vec, int l1, int l2)
{
    DEBUG ("\nTrap Ocurred at L1: 0x%08x and L2: 0x%08x\n", l1, l2);
    DEBUG ("%s", traps[vec]);
}

