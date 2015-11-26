/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/


#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <autoconf.h>

/**
 * \file This file stores all the kernel configuration and must be generated
 * by the EDROOM tool
 */
 
/**
 * The KCONFIG_MAX_THREADS is the maximum number of system threads
 */
#define KCONFIG_MAX_THREADS    32

/**
 * The KCONFIG_MAX_BIN_SEMAPHORES label is the maximum number of the binary
 * semaphores
 */
#define KCONFIG_MAX_BIN_SEMAPHORES  100

/**
 * The KCONFIG_MAX_RESOURCE_SEMAPHORES label is the maximum number of the 
 * resource semaphores
 */
#define KCONFIG_MAX_RESOURCE_SEMAPHORES 100

/** 
 * The number of the system priorities
 */
#define KCONFIG_NUM_PRIORITIES  CONFIG_NUMBER_OF_PRIORITIES

/**
 * This label defines the minimum priority. It is also the number of the 
 * priorities less one
 */
#define KCONFIG_MIN_PRIORITY    KCONFIG_NUM_PRIORITIES - 1

/**
 * This is the board system clock (MHz) speed configuration.
 */
#define KCONFIG_CLOCK_SPEED   CONFIG_CLOCK_SPEED_MHZ    /*  1 us count down */

/** Threads stack pool  */
#define KCONFIG_STACK_POOL      (1024 * 512)
#define KCONFIG_STACK_POOL_LOG2  19

/** This value is the log2 of the maximum memory chunk value    */
#define KCONFIG_MIN_CHUNK_LOG2   11

/** The idle task stack size    */
//#define KCONFIG_MAIN_STACK_SIZE (32 * 1024)
#define KCONFIG_MAIN_STACK_SIZE (5 * 1024)

/** This label is the start stack address   */
#define KCONFIG_ADDR_STACK_INIT CONFIG_STACK_BEGIN_ADDRESS

/** Tracer definitions  */
#ifdef CONFIG_ERCOS_TRACER

	/** Minimum default stack size    */
	//#define KCONFIG_MINSTACK    (32 * 2048)
	#define KCONFIG_MINSTACK    (5 * 2048)

    /** Tracer buffer size  */
    #define KCONFIG_TRACER_BUF_SIZE (CONFIG_TRACER_BUF_SIZE * 1024)
    
    /** Tracer buffer mask in order to make the module operation  */
    #define KCONFIG_TRACER_BUF_MASK (KCONFIG_TRACER_BUF_SIZE - 1UL)
    
    /** Number of levels to trace   */
    #define KCONFIG_TRACER_NLEVELS  CONFIG_TRACER_NLEVELS

#else
	/** Minimum default stack size    */
	#define KCONFIG_MINSTACK    (2 * 2048)

    /** Tracer buffer size  */
    #define KCONFIG_TRACER_BUF_SIZE (0 * 1024)
    
#endif

#endif

