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
 * \file arch/sparc/ERC32/spurious.c
 * \brief Definition of the trap tracing function.
 */

#include <arch/spurious.h>
#include <kernel/debug.h>

/** \brief Array containing the different trap error messages */
static const char traps[32][64] =
{
  "\n*** TRAP 00: RESET ***\n",
  "\n*** TRAP 01: INSTRUCTION ACCESS ***\n",
  "\n*** TRAP 02: ILLEGAL INSTRUCTION ***\n",
  "\n*** TRAP 03: PROVILEGED INSTRUCTION ***\n",
  "\n*** TRAP 04: FLOTATING POINT DISABLED ***\n",
  "\n*** TRAP 05: WINDOW OVERFLOW ***\n",
  "\n*** TRAP 06: WINDOW UNDERFLOW ***\n",
  "\n*** TRAP 07: MEMORY ACCESS NOT ALIGMENT ***\n",
  "\n*** TRAP 08: FLOTATING POINT EXCEPTION ***\n",
  "\n*** TRAP 09: DATA ACCESS EXCEPTION ***\n",
  "\n*** TRAP 0A: TAG OVERFLOW ***\n",
  "\n*** TRAP 0B: UNHANDLED TRAP ***\n",
  "\n*** TRAP 0C: UNHANDLED TRAP ***\n",
  "\n*** TRAP 0D: UNHANDLED TRAP ***\n",
  "\n*** TRAP 0E: UNHANDLED TRAP ***\n",
  "\n*** TRAP 0F: UNHANDLED TRAP ***\n",
  "\n*** TRAP 10: UNHANDLED TRAP ***\n",
  "\n*** TRAP 11: MASKED HARDWARE ERRORS ***\n",
  "\n*** TRAP 12: EXT INT0 ***\n",
  "\n*** TRAP 13: EXT INT1 ***\n",
  "\n*** TRAP 14: UART A DATA READY ***\n",
  "\n*** TRAP 15: UART B DATA READY ***\n",
  "\n*** TRAP 16: CORRECTABLE ERROR IN MEMORY ***\n",
  "\n*** TRAP 17: UART ERROR ***\n",
  "\n*** TRAP 18: DMA ACCESS ERROR ***\n",
  "\n*** TRAP 19: DMA TIME-OUT ***\n",
  "\n*** TRAP 1A: EXT INT2 ***\n",
  "\n*** TRAP 1B: EXT INT3 ***\n",
  "\n*** TRAP 1C: GENERAL PURPOSE TIMER ***\n",
  "\n*** TRAP 1D: REAL TIME CLOCK ***\n",
  "\n*** TRAP 1E: EXT INT4 ***\n",
  "\n*** TRAP 1F: WATCHDOG TIME-OUT ***\n"
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

