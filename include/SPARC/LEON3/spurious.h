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
 * $Id: spurious.h 622 2006-03-23 16:17:45 +0100 (jue, 23 mar 2006) aitor $
 *  
 * $Revision: 622 $
 * $Date: 2006-03-23 16:17:45 +0100 (jue, 23 mar 2006) $
 * Last CVS commit by $Author: aitor $
 * 
 * Registro de cambios
 * 
 * $Log$
 * Revision 1.2  2005/09/22 11:31:57  aitor
 * CVS Problems
 *
 * Revision 1.1  2005/06/23 12:08:37  aitor
 * Nueva estructura de directorios.
 *
 * Revision 1.1  2005/06/23 10:55:17  aitor
 * Archivo de cabecera que declara la función CPU_TraceTraps para acciones de
 * depuración.
 *
 */

#ifndef _SPURIOUS_H_
#define _SPURIOUS_H_

void CPU_TraceTraps (unsigned long vec, int l1, int l2);

#endif
