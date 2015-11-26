/*
    Copyright 2005-2006 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/


#ifndef ERC32_STUB_H_
#define ERC32_STUB_H_

#include <kernel/config.h>

/**
 * \todo Comentar este método
 */
void putDebugChar(unsigned char car);

/**
 * \todo Comentar este método
 */
int getDebugChar(void);

/**
 * \todo Comentar este método
 */
void exceptionHandler(unsigned char ex_num, 
                      unsigned int *ex_add,
                      unsigned int tbr);

/**
 * \todo Comentar este método
 */
void flush_i_cache(void);

#endif //ERC32_STUB_H_
