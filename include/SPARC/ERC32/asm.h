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
 * \file include/sparc/ERC32/asm.h
 * \brief Several ASM ERC32-specific definitions.
 */

#ifndef _ARCH_ASM_H_
#define _ARCH_ASM_H_

#include <autoconf.h>

//------------------------------------------------------------------------------

/*******************************************************************************
 * MEC REGISTERS OFFSETS
 ******************************************************************************/
 
/** \brief MEC Control Register Offset */
#define MEC_CONTROL_REGISTER         0x00
/** \brief MEC Software Reset Register Offset */
#define MEC_SOFTWARE_RESET           0x04
/** \brief MEC Power Down Register Offset */
#define MEC_POWER_DOWN               0x08
/** \brief MEC Memory Configuration Register Offset */
#define MEC_MEMORY_CONFIGURATION     0x10
/** \brief MEC I/O Configuration Register Offset */
#define MEC_IO_CONFIGURATION         0x14
/** \brief MEC Waitstate Configuration Register Offset */
#define MEC_WAITSTATE_CONFIGURATION  0x18
/** \brief MEC Access Protection Segment 1 Base Register Offset */
#define MEC_AP_SEGMENT1_BASE         0x20
/** \brief MEC Access Protection Segment 1 End Register Offset */
#define MEC_AP_SEGMENT1_END          0x24
/** \brief MEC Access Protection Segment 2 Base Register Offset */
#define MEC_AP_SEGMENT2_BASE         0x28
/** \brief MEC Access Protection Segment 2 End Register Offset */
#define MEC_AP_SEGMENT2_END          0x2C
/** \brief MEC Interrupt Shape Register Offset */
#define MEC_INTERRUPT_SHAPE          0x44
/** \brief MEC Interrupt Pending Register Offset */
#define MEC_INTERRUPT_PENDING        0x48
/** \brief MEC Interrupt Mask Register Offset */
#define MEC_INTERRUPT_MASK           0x4C
/** \brief MEC Interrupt Clear Register Offset */
#define MEC_INTERRUPT_CLEAR          0x50
/** \brief MEC Interrupt Force Register Offset */
#define MEC_INTERRUPT_FORCE          0x54
/** \brief MEC Watchdog Register Offset */
#define MEC_WATCHDOG_REGISTER        0x60
/** \brief MEC Watchdog Trap Door Register Offset */
#define MEC_WATCHDOG_TRAP_DOOR       0x64
/** \brief MEC Real-Time Clock Counter Register Offset */
#define MEC_RT_CLOCK_COUNTER         0x80
/** \brief MEC Real-Time Clock Scaler Register Offset */
#define MEC_RT_CLOCK_SCALER          0x84
/** \brief MEC General Purpose Counter Register Offset */
#define MEC_GP_TIMER_COUNTER         0x88
/** \brief MEC General Purpose Scaler Register Offset */
#define MEC_GP_TIMER_SCALER          0x8C
/** \brief MEC Timer Control Register Offset */
#define MEC_TIMER_CONTROL            0x98
/** \brief MEC System Fault Status Register Offset */
#define MEC_SYSTEM_FAULT_STATUS      0xA0
/** \brief MEC Failing Address Register Offset */
#define MEC_FAILING_ADDRESS          0xA4
/** \brief MEC Error and Reset Status Register Offset */
#define MEC_ERROR_AND_RESET_STATUS   0xB0
/** \brief MEC Test Control Register Offset */
#define MEC_TEST_CONTROL             0xD0
/** \brief MEC UART Channel 1 Register Offset */
#define MEC_UART_CHANNEL1            0xE0
/** \brief MEC UART Channel 2 Register Offset */
#define MEC_UART_CHANNEL2            0xE4
/** \brief MEC UART Status Register Offset */
#define MEC_UART_STATUS              0xE8


/*******************************************************************************
 * PSR REGISTER MASKS
 ******************************************************************************/

/* The SPARC PSR fields are laid out as the following:
 *
 *  ------------------------------------------------------------------------
 *  | impl  | vers  | icc   | resv  | EC | EF | PIL  | S | PS | ET |  CWP  |
 *  | 31-28 | 27-24 | 23-20 | 19-14 | 13 | 12 | 11-8 | 7 | 6  | 5  |  4-0  |
 *  ------------------------------------------------------------------------
 */
#define PSR_CWP     0x0000001f         /**< \brief Current window pointer     */
#define PSR_ET      0x00000020         /**< \brief Enable traps field         */
#define PSR_PS      0x00000040         /**< \brief Previous privilege level   */
#define PSR_S       0x00000080         /**< \brief Current privilege level    */
#define PSR_PIL     0x00000f00         /**< \brief Processor interrupt level  */
#define PSR_EF      0x00001000         /**< \brief Enable floating point      */
#define PSR_EC      0x00002000         /**< \brief Enable co-processor        */
#define PSR_LE      0x00008000         /**< \brief SuperSparcII little-endian */
#define PSR_ICC     0x00f00000         /**< \brief Integer condition codes    */
#define PSR_C       0x00100000         /**< \brief Carry bit                  */
#define PSR_V       0x00200000         /**< \brief Overflow bit               */
#define PSR_Z       0x00400000         /**< \brief Zero bit                   */
#define PSR_N       0x00800000         /**< \brief Negative bit               */
#define PSR_VERS    0x0f000000         /**< \brief CPU-version field          */
#define PSR_IMPL    0xf0000000         /**< \brief CPU-implementation field   */


#endif
