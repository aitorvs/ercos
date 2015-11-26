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
 * $Id: string.h 605 2006-03-21 15:27:22 +0000 (mar, 21 mar 2006) aitor $
 *  
 * $Revision: 605 $
 * $Date: 2006-03-21 15:27:22 +0000 (mar, 21 mar 2006) $
 * Last CVS commit by $Author: aitor $
 * 
 * Registro de cambios
 * 
 * $Log$
 * Revision 1.1  2005/09/19 11:38:04  aitor
 * Restoring the project.
 *
 * Revision 1.1  2005/06/21 10:28:15  aitor
 * Alta del mini sistema ERCOS-RT
 *
 * Revision 1.1  2005/03/03 14:57:18  aitor
 * Alta en el repositorio del n�cleo m�nimo para ERC-32
 *
 */

#ifndef _STRING_H_
#define _STRING_H_

#include <sys/types.h>

void *memset(void *p,int c,unsigned int n);
void *memcpy(void *dest,void *src,unsigned int n);
int memcmp(const void *s1,const void *s2,unsigned int n);
char *strcpy(char *dest,const char *src);
char *strncpy(char *dest,const char *src, unsigned int n);
int strlen(const char *s);
int strnlen(const char *s, unsigned int n);

#endif
