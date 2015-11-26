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
 * \file include/sparc/ERC32/spurious.h
 * \brief Spurious functions.
 */

#ifndef _SPURIOUS_H_
#define _SPURIOUS_H_

/**
 * \brief Traces a trap, sending a message through the serial line.
 * 
 * \param vec  Vector number of the trap.
 * \param l1   Value of the l1 register after the trap has occurred (i. e. the
 *             IP of the instruction that was being executed when the trap
 *             occurred).
 * \param l2   Value of the l2 register after the trap has occurred (i. e. the
 *             instruction following to the one that was being executed when the 
 *             trap occurred).
 */
void CPU_TraceTraps (unsigned long vec, int l1, int l2);

#endif
