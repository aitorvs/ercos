/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT on ERC-32.
*/

/*
 * File:    $Source$
 * 
 * $Id: cputypes.h 1025 2006-06-19 14:19:24 +0000 (lun, 19 jun 2006) aitor $
 *  
 * $Revision: 1025 $
 * $Date: 2006-06-19 14:19:24 +0000 (lun, 19 jun 2006) $
 * Last CVS commit by $Author: aitor $
 * 
 * Registro de cambios
 * 
 * $Log$
 * Revision 1.3  2005/10/10 12:56:59  aitor
 * NULL value redefinition
 *
 * Revision 1.2  2005/09/22 11:30:22  aitor
 * CVS Problems
 *
 * Revision 1.7  2005/08/15 12:45:29  aitor
 * Change the OS_context_t name by CPU_context_t
 *
 * Revision 1.6  2005/07/21 10:59:46  aitor
 * Added the OS_context_t struture in this file.
 *
 * Revision 1.5  2005/07/05 16:06:29  aitor
 * Added some comments and other kernel types.
 *
 * Revision 1.4  2005/07/22 09:00:55  aitor
 * Delete the BIG_ENDIAN label.
 *
 * Revision 1.3  2005/07/19 07:58:44  aitor
 * Define the NULL value.
 *
 * Revision 1.2  2005/07/14 15:33:55  aitor
 * Added the TRUE and FALSE types
 *
 * Revision 1.1  2005/07/12 13:43:57  aitor
 * Se cambia de nombre el archivo fuente.
 *
 * Revision 1.1  2005/06/23 12:08:37  aitor
 * Nueva estructura de directorios.
 *
 * Revision 1.2  2005/06/23 10:54:33  aitor
 * Correcci�n de los tipos de 64 bits.
 *
 * Revision 1.1  2005/06/21 10:28:15  aitor
 * Alta del mini sistema ERCOS-RT
 *
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
 *  On the SPARC, we are relatively conservative in that we save most
 *  of the CPU state in the context area.  The ET (enable trap) bit and
 *  the CWP (current window pointer) fields of the PSR are considered
 *  system wide resources and are not maintained on a per-thread basis.
 */

typedef struct context
{
    /*
     *  Using a double g0_g1 will put everything in this structure on a 
     *  double word boundary which allows us to use double word loads
     *  and stores safely in the context switch.
     */
    uint64     g0_g1;
    uint32 g2;
    uint32 g3;
    uint32 g4;
    uint32 g5;
    uint32 g6;
    uint32 g7;

    uint32 l0;
    uint32 l1;
    uint32 l2;
    uint32 l3;
    uint32 l4;
    uint32 l5;
    uint32 l6;
    uint32 l7;

    uint32 i0;
    uint32 i1;
    uint32 i2;
    uint32 i3;
    uint32 i4;
    uint32 i5;
    uint32 i6_fp;
    uint32 i7;

    uint32 o0;
    uint32 o1;
    uint32 o2;
    uint32 o3;
    uint32 o4;
    uint32 o5;
    uint32 o6_sp;
    uint32 o7;

    uint32 psr;
    uint32 y;
}CPU_context_t;


#endif

