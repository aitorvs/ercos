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
 * $Id: psr.h 956 2006-06-08 08:47:22 +0000 (jue, 08 jun 2006) aitor $
 *  
 * $Revision: 956 $
 * $Date: 2006-06-08 08:47:22 +0000 (jue, 08 jun 2006) $
 * Last CVS commit by $Author: aitor $
 * 
 * Registro de cambios
 * 
 * $Log$
 * Revision 1.2  2005/09/22 11:30:22  aitor
 * CVS Problems
 *
 * Revision 1.5  2005/07/21 11:00:35  aitor
 * Delete the types.h header file.
 *
 * Revision 1.4  2005/07/28 16:52:09  aitor
 * Delete some macros.
 *
 * Revision 1.3  2005/07/27 15:01:45  aitor
 * Added a macro that is necessary to set the ET flag.
 *
 * Revision 1.2  2005/07/19 14:31:40  aitor
 * Added ASM labels on the code to ensure the good compilation.
 *
 * Revision 1.1  2005/06/23 12:08:37  aitor
 * Nueva estructura de directorios.
 *
 * Revision 1.1  2005/06/21 10:28:15  aitor
 * Alta del mini sistema ERCOS-RT
 *
 */

#ifndef __LINUX_SPARC_PSR_H
#define __LINUX_SPARC_PSR_H

#include <autoconf.h>

/****************************** Only for C code ******************************/

#ifndef ASM
/*
 *  Standard nop MACRO
 */
#define nop() \
  do { \
    asm volatile ( "nop" ); \
  } while ( 0 )


/**
 * \todo Documentar esta MACRO
 */
#define sparc_get_psr( _psr ) \
  do { \
     (_psr) = 0; \
     asm volatile( "rd %%psr, %0" :  "=r" (_psr) : "0" (_psr) ); \
  } while ( 0 )

/**
 * \todo Documentar esta MACRO
 */
#define sparc_set_psr( _psr ) \
  do { \
    asm volatile ( "mov  %0, %%psr " : "=r" ((_psr)) : "0" ((_psr)) ); \
    nop(); \
    nop(); \
    nop(); \
  } while ( 0 )
  
/**
 * Disables the interrupts by manipulating the interrupt level
 */
#define sparc_disable_interrupts( _level ) \
  do { \
    register unsigned int _newlevel; \
    \
    sparc_get_psr( _level ); \
    (_newlevel) = (_level) | PSR_PIL; \
    sparc_set_psr( _newlevel ); \
  } while ( 0 )

/**
 * Enables the interrupts by manipulating the interrupt level
 */
#define sparc_enable_interrupts( _level ) \
  do { \
    unsigned int _tmp; \
    \
    sparc_get_psr( _tmp ); \
    _tmp &= ~PSR_PIL; \
    _tmp |= (_level) & PSR_PIL; \
    sparc_set_psr( _tmp ); \
  } while ( 0 ) 


#endif  // ASM

#endif /* !(__LINUX_SPARC_PSR_H) */


