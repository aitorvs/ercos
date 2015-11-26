/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

/*!
 * \file include/kernel/irq.h
 * \brief Contains all the MACRO definitions and functions related to the
 *        IRQ management service
 */

#ifndef _KERNEL_IRQ_H_
#define _KERNEL_IRQ_H_

#include <cpu.h>
#include <target.h>

/**
 * \brief This is the irq number for the real time clock.
 */
#define TIMER_TRAP  CPU_TIMER_TRAP

/**
 * \brief This is the irq number for the general purpose timer.
 */
#define CLOCK_TRAP  CPU_CLOCK_TRAP

/**
 * \brief This is the irq number for the serial device.
 */
#define SERIAL_TRAP CPU_SERIAL_TRAP

/**
 * \brief This macro return TRUE in case of nested interrupt.
 */
#define OS_IS_IN_TRAP_HANDLER()  CPU_IS_IN_TRAP_HANDLER()

/**
 * \brief This macro enables the interrupts.
 */
#define OS_EnableIRQ()  _CPU_EnableIRQ()

/**
 * \brief This macro disables the interrupts.
 */
#define OS_DisableIRQ() _CPU_DisableIRQ()

/** 
 * \brief This macro mask the interrupt vector passed as parameter.
 * 
 * \param vector  The interrupt vector
 */
#define OS_MaskInterrupt(vector)    CPU_MaskInterrupt(vector)

/**
 * \brief This macro mask all the interrutps.
 */
#define OS_MaskAllInterrupts()  CPU_MaskAllInterrupts()

/** 
 * \brief This macro verifies if the interrupt vector number passed as
 *        parameter is an interrupts.
 * 
 * \param vec  The interrupt number
 */
#define OS_IsInterrupt(vec) CPU_IsInterrupt(vec)

/** 
 * \brief This macro clears the interrupt vector passed as parameter, after the
 *        interrupt has been triggered.
 * 
 * \param vector  The interrupt vector
 */
#define OS_ClearInterrupt(vector)   CPU_ClearInterrupt(vector)

/** 
 * \brief This macro enables the interrupt vector passed as parameter.
 * 
 * \param vector  The interrupt vector
 */
#define OS_EnableInterrupt(vector)  CPU_EnableInterrupt(vector)

/** 
 * \brief This macro force the interrupt vector passed as parameter.
 * 
 * \param vector  The interrupt vector
 */
#define OS_ForceInterrupt(vector)   CPU_ForceInterrupt(vector)

#endif  // #ifndef _IRQ_H_

