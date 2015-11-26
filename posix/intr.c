/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT project.
*/

/*
 * File:    $Source$
 * 
 * $Id: intr.c 1252 2007-11-28 16:20:19Z cesar $
 *  
 * $Revision: 1252 $
 * $Date: 2007-11-28 17:20:19 +0100 (mié, 28 nov 2007) $
 * Last CVS commit by $Author: cesar $
 * 
 * Registro de cambios
 * 
 * $Log$
 * Revision 1.1  2005/09/26 11:40:40  aitor
 * Added to the repository the POSIX interrupt managemen. Not all the routines all POSIX compliant.
 *
 */

#include <posix/intr.h>

#include <syscall.h>

//----------------------------------------------------------------------------

int intr_lock(intr_t  intr)
{
    SYS_IntrLock(intr);
    
    return 0;
}

int intr_unlock(intr_t  intr)
{
    SYS_IntrUnlock(intr);
    
    return 0;
}

int intr_capture(void (*intr_handler)(int _level), int level)
{
    return SYS_RegisterIRQ(level, intr_handler);
}


int intr_restore(int level)
{
    return SYS_RestoreIRQ(level);
}

int intr_force(int level)
{
    SYS_ForceIRQ(level);
    return 0;
}

