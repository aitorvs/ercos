/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT on ERC-32.
*/

/*!
 * \file arch/sparc/ERC32/halt.c
 * \brief ERC32 CPU reset and hatling functions
 */
 
 /** \brief Halts the system by writing on a special MEC register. */
void CPU_PowerDown(void)
{
    *((volatile unsigned int *) 0x01f80008) = 0;
}

/** \brief Resets the system by writing on a special MEC register */
void CPU_Reset(void)
{
   volatile unsigned int* dirmec = (unsigned int*)0x01f80000;
    
    dirmec[0x04 / 4] = 0xffffffff;
}
