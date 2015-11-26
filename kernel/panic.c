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
 * $Id: panic.c 1269 2008-02-25 18:10:23Z cesar $
 *  
 * $Revision: 1269 $
 * $Date: 2008-02-25 19:10:23 +0100 (lun, 25 feb 2008) $
 * Last CVS commit by $Author: cesar $
 * 
 * Registro de cambios
 * 
 * $Log$
 * Revision 1.1  2005/09/19 11:35:29  aitor
 * Restoring the project.
 *
 * Revision 1.2  2005/07/28 14:31:58  aitor
 * *** empty log message ***
 *
 * Revision 1.1  2005/06/21 10:28:15  aitor
 * Alta del mini sistema ERCOS-RT
 *
 */

#include <kernel/config.h>
#include <kernel/debug.h>
#include <kernel/panic.h>
#include <kernel/task.h>
#include <lib/stdio.h>

#include <sys/types.h>

void OS_KernelPanic (char *string)
{
    DEBUG ("AIEEE!! kernel panic: %s", string);

    DEBUG ("current 0x%x", current - task_table);
    DEBUG ("old_current 0x%x", old_current - task_table);
    DEBUG ("abs_timeout %x %d", (uint32)(current->abs_timeout >> 32), (uint32)current->abs_timeout);
    DEBUG ("abs_timeout dir 0x%x", &(current->abs_timeout));
    while(1);
}

