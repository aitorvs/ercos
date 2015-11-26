/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez,
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT.
*/

#ifndef _ARCH_ASM_H_
#define _ARCH_ASM_H_

#include <autoconf.h>

//------------------------------------------------------------------------------

/*******************************************************************************
 * MEC REGISTERS
 ******************************************************************************/

#define MEC_CONTROL_REGISTER         0x00
#define MEC_SOFTWARE_RESET           0x04
#define MEC_POWER_DOWN               0x08
#define MEC_MEMORY_CONFIGURATION     0x10
#define MEC_IO_CONFIGURATION         0x14
#define MEC_WAITSTATE_CONFIGURATION  0x18
#define MEC_AP_SEGMENT1_BASE         0x20
#define MEC_AP_SEGMENT1_END          0x24
#define MEC_AP_SEGMENT2_BASE         0x28
#define MEC_AP_SEGMENT2_END          0x2C
#define MEC_INTERRUPT_SHAPE          0x44
#define MEC_INTERRUPT_PENDING        0x48
#define MEC_INTERRUPT_MASK           0x4C
#define MEC_INTERRUPT_CLEAR          0x50
#define MEC_INTERRUPT_FORCE          0x54
#define MEC_WATCHDOG_REGISTER        0x60
#define MEC_WATCHDOG_TRAP_DOOR       0x64
#define MEC_RT_CLOCK_COUNTER         0x80
#define MEC_RT_CLOCK_SCALER          0x84
#define MEC_GP_TIMER_COUNTER         0x88
#define MEC_GP_TIMER_SCALER          0x8C
#define MEC_TIMER_CONTROL            0x98
#define MEC_SYSTEM_FAULT_STATUS      0xA0
#define MEC_FAILING_ADDRESS          0xA4
#define MEC_ERROR_AND_RESET_STATUS   0xB0
#define MEC_TEST_CONTROL             0xD0
#define MEC_UART_CHANNEL1            0xE0
#define MEC_UART_CHANNEL2            0xE4
#define MEC_UART_STATUS              0xE8


/*******************************************************************************
 * PSR REGISTERS
 ******************************************************************************/

/* The Sparc PSR fields are laid out as the following:
 *
 *  ------------------------------------------------------------------------
 *  | impl  | vers  | icc   | resv  | EC | EF | PIL  | S | PS | ET |  CWP  |
 *  | 31-28 | 27-24 | 23-20 | 19-14 | 13 | 12 | 11-8 | 7 | 6  | 5  |  4-0  |
 *  ------------------------------------------------------------------------
 */
#define PSR_CWP     0x0000001f         /* current window pointer     */
#define PSR_ET      0x00000020         /* enable traps field         */
#define PSR_PS      0x00000040         /* previous privilege level   */
#define PSR_S       0x00000080         /* current privilege level    */
#define PSR_PIL     0x00000f00         /* processor interrupt level  */
#define PSR_EF      0x00001000         /* enable floating point      */
#define PSR_EC      0x00002000         /* enable co-processor        */
#define PSR_LE      0x00008000         /* SuperSparcII little-endian */
#define PSR_ICC     0x00f00000         /* integer condition codes    */
#define PSR_C       0x00100000         /* carry bit                  */
#define PSR_V       0x00200000         /* overflow bit               */
#define PSR_Z       0x00400000         /* zero bit                   */
#define PSR_N       0x00800000         /* negative bit               */
#define PSR_VERS    0x0f000000         /* cpu-version field          */
#define PSR_IMPL    0xf0000000         /* cpu-implementation field   */


#endif
