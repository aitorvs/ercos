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
 * \file include/sparc/ERC32/platform.h
 * \brief ERC32-specific includes. This file contains all the necessary include
 *        files for this platform.
 */
 
#ifndef __ERC32_PLATFORM_
#define __ERC32_PLATFORM_

#include "asm.h"
#include "erc32.h"
#include "mec.h"
#include "erc32-stub.h"
#include "serial.h"

#ifdef CONFIG_DEBUG
#include "spurious.h"
#endif 

#ifndef CONFIG_ERCOS_SIMU
#include "timer.h"
#endif

#endif

