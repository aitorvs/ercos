/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/
 
/*
 * File:    $Source$
 * 
 * $Id: cpu.h 641 2006-03-24 12:54:27 +0000 (vie, 24 mar 2006) aitor $
 *  
 * $Revision: 641 $
 * $Date: 2006-03-24 12:54:27 +0000 (vie, 24 mar 2006) $
 * Last CVS commit by $Author: aitor $
 * 
 * Registro de cambios
 * 
 * $Log$
 * Revision 1.5  2005/09/26 11:34:06  aitor
 * All the interrupt management macros are been changed to the irq.h header file.
 *
 * Revision 1.4  2005/09/23 16:40:56  aitor
 * Added the erc32.h header file.
 *
 * Revision 1.3  2005/09/23 14:53:31  aitor
 * Added some macros to manage the architecture interrupt state.
 *
 * Revision 1.2  2005/09/22 11:29:36  aitor
 * CVS Problems
 *
 * Revision 1.10  2005/08/15 12:45:46  aitor
 * *** empty log message ***
 *
 * Revision 1.9  2005/07/21 11:00:21  aitor
 * Delete the OS_context_t struture from this file. Now it is in the cputypes.h
 * header file.
 *
 * Revision 1.8  2005/07/29 11:22:52  aitor
 * Change the naming convention.
 *
 * Revision 1.7  2005/07/29 10:20:41  aitor
 * The architecture dependent routines begins with the CPU_ prefix
 *
 * Revision 1.6  2005/07/26 17:14:00  aitor
 * Delete unused macros.
 *
 * Revision 1.5  2005/07/20 13:37:11  aitor
 * Comment all the offsets for the registers used in the interrupt handler.
 *
 * Revision 1.4  2005/07/19 14:31:20  aitor
 * Added some offset to store the registers on the thread stack.
 *
 * Revision 1.3  2005/07/14 13:51:24  aitor
 * The file cpu/context.h is deleted and its structure is defined in the file
 * cpu/cpu.h.
 *
 * Revision 1.2  2005/07/12 11:00:52  aitor
 * Se cambia el nombre de la funci�n que se encarga de inicializar el
 * contexto de un hilo.
 *
 * Revision 1.1  2005/06/23 12:08:37  aitor
 * Nueva estructura de directorios.
 *
 * Revision 1.1  2005/06/23 10:55:51  aitor
 * Archivo de cabecera que contiene definiciones de registros y macros
 * dependientes de CPU
 *
 */

#ifndef _CPU_H_
#define _CPU_H_

#include <cpu/cputypes.h>


/**
 * \todo Documentar esta funci�n
 */
void CPU_ContextInit(CPU_context_t   *the_context,
                     uint8         *stack_base,
                     uint32         size,
                     uint32         new_level,
                     void           *entry_point);

/**
 * \todo Documentar esta funci�n
 */
void CPU_Init(void);


extern void _CPU_DisableIRQ();
extern void _CPU_EnableIRQ();

#endif
