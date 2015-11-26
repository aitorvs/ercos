/*
    Copyright 2005-2006 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

/*
 * Formatted console output
 */

/*  Architecture specific headers   */
#include <target.h>

/*  Standard headers    */
#include <string.h>
#include <stdio.h>

/*
 * Write a formatted string
 */

int sprintf(char * buffer, const char *fmt,...)
{
	int i=0;
	char car;
	
	va_list args;
	
	va_start(args, fmt);

	/* vsprintf do really the work */
	
	vsprintf(buffer,fmt,args);
	
	/* Write string to serial port */
	
	for (i = 0; ((car=buffer[i])&&i<1024); i++);

	return i;
}

int printf(const char *fmt,...)
{
	int i=0;
	char buffer[1024],car;
	
	va_list args;
	
	va_start(args, fmt);

	/* vsprintf do really the work */
	
	vsprintf(buffer,fmt,args);
	
	/* Write string to serial port */
	
	while((car=buffer[i])&&i<1024)
	{
		CPU_SerialTx(car);

		/* Append '\r' to complete carriage return '\n' */
		 
		if (car=='\n') CPU_SerialTx('\r'); 
		
		i++;
	}

	return i;
}
