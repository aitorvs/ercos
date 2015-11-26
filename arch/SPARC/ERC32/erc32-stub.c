/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERC32mon.
*/

/*!
 * \file arch/sparc/ERC32/erc32-stub.c
 * \brief Functions of the ERC32 stub for GCC
 */

#include <arch/erc32-stub.h>


/*  Ensures that trapstack variable is 0x10 align   */
asm(
    ".data\t\n"
    ".align 0x10\t\n"
    ".globl trapstack\t\n"
    "trapstack:\t\n"
    ".word  0\t\n"
    ".text\t\n"
    ".align 0x20\t\n"
    
    );
    
//unsigned int trapstack;

void exceptionHandler(unsigned char ex_num, 
                      unsigned int *ex_add,
                      unsigned int tbr)
{
    unsigned int *t_add;
   
    t_add = (unsigned int *) ((tbr & ~0x0fff) | /*  Direcci�n de la TBR */
            ((unsigned int) ex_num << 4));      /*  Direcci�n del trap  */

    *t_add = 0xA010000F;    /* Se almacena or %o7,%g0,%l0 en t_add  */
    t_add++;
    *t_add = (0x40000000 | (((unsigned int) (ex_add-t_add)) )); /* call _ex_add */
    t_add++;
    *t_add = 0x9E100010;    /* or %l0,%g0,%o7 */
}

void flush_i_cache(void)
{
    asm volatile ("flush");
}

