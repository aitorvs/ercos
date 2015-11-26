/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT on ERC-32.
*/


#ifndef _STDIO_H_
#define _STDIO_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <lib/stdarg.h>

int printf(const char *fmt,...);

int sprintf(char * buffer, const char *fmt,...);

int putchar(int __c);

int vsprintf(char *buff,const char *fmt,va_list args);

#ifdef __cplusplus
}
#endif

#endif
