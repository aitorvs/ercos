/*
    Copyright 2004-2005 (c) by Óscar García Población,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT on M68K.
*/
/**
 * $Revision$ $Date$
 * Last CVS commit by $Author$
 * $Log$
 */

#ifndef _M68K_CPU_CPUTYPES_H_
#define _M68K_CPU_CPUTYPES_H_

/** unsigned word */
typedef unsigned short		word16;

/** unsigned byte   */
typedef unsigned char		uint8;
/** unsigned double byte    */
typedef unsigned short		uint16;
/** unsigned 32 bit word    */
typedef unsigned long int	uint32;
/** unsidned 64 bit word    */
typedef unsigned long long	uint64;

/** Signed byte */
typedef signed char		sint8;
/** Signed 16 bit word  */
typedef signed short		sint16;
/** Signed 32 bit word  */
typedef signed int		sint32;
/** Signed 64 bit word  */
typedef signed long long 	sint64;

#ifndef NULL
#define NULL      ((void *) 0x0)
#endif

#define TRUE    1
#define FALSE   0

typedef struct m68k_context {
	uint32	usp;

	uint32	a6;
	uint32	a5;
	uint32	a4;
	uint32	a3;
	uint32	a2;
	uint32	a1;
	uint32	a0;

	uint32	d7;
	uint32	d6;
	uint32	d5;
	uint32	d4;
	uint32	d3;
	uint32	d2;
	uint32	d1;
	uint32	d0;

	uint16	sr;
	uint32	pc;
} m68k_context_t;

typedef m68k_context_t CPU_context_t;

#endif
