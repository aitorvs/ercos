/*
    Copyright 2007-2008 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT on DUMMY platform.
*/


#ifndef _SPARC_TYPES_H_
#define _SPARC_TYPES_H_

/*  CPU types   */

/** unsigned byte   */
typedef unsigned char	uint8;
/** unsigned double byte    */
typedef unsigned short	uint16;
/** unsigned 32 bit word    */
typedef unsigned int	uint32;
/** unsidned 64 bit word    */
typedef unsigned long long	uint64;

/** Signed byte */
typedef signed char	sint8;
/** Signed 16 bit word  */
typedef signed short	sint16;
/** Signed 32 bit word  */
typedef signed int	sint32;
/** Signed 64 bit word  */
typedef signed long	long sint64;

#ifndef NULL
#define NULL      0x0
#endif

#define TRUE    1
#define FALSE   0

/**
 * Contexts
 *
 *  Generally there are 2 types of context to save.
 *     1. Interrupt registers to save
 *     2. Task level registers to save
 *
 *  This means we have the following 3 context items:
 *     1. task level context stuff::  Context_Control
 *     2. floating point task stuff:: Context_Control_fp
 *     3. special interrupt level context :: Context_Control_interrupt
 *
 */

typedef struct context
{

}CPU_context_t;


#endif

