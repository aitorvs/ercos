/*
    Copyright 2004-2005 (c) by Aitor Viana Sanchez, Pablo Parra Espada
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
 * ON-CHIP PERIPHERALS REGISTERS
 ******************************************************************************/

#define REGMAP_MEMORY_CONFIGURATION_1   0x00
#define REGMAP_MEMORY_CONFIGURATION_2   0x04
#define REGMAP_MEMORY_CONFIGURATION_3   0x08
#define REGMAP_AHB_FAILING_ADDRESS      0x0C
#define REGMAP_AHB_STATUS               0x10
#define REGMAP_CACHE_CONTROL            0x14
#define REGMAP_POWER_DOWN               0x18
#define REGMAP_WRITE_PROTECTION_1       0x1C
#define REGMAP_WRITE_PROTECTION_2       0x20
#define REGMAP_LEON_CONFIGURATION       0x28
#define REGMAP_TIMER_1_COUNTER          0x40
#define REGMAP_TIMER_1_RELOAD           0x44
#define REGMAP_TIMER_1_CONTROL          0x48
#define REGMAP_WATCHDOG                 0x4C
#define REGMAP_TIMER_2_COUNTER          0x50
#define REGMAP_TIMER_2_RELOAD           0x54
#define REGMAP_TIMER_2_CONTROL          0x58
#define REGMAP_PRESCALER_COUNTER        0x60
#define REGMAP_PRESCALER_RELOAD         0x64
#define REGMAP_UART_1_DATA              0x70
#define REGMAP_UART_1_STATUS            0x74
#define REGMAP_UART_1_CONTROL           0x78
#define REGMAP_UART_1_SCALER            0x7C
#define REGMAP_UART_2_DATA              0x80
#define REGMAP_UART_2_STATUS            0x84
#define REGMAP_UART_2_CONTROL           0x88
#define REGMAP_UART_2_SCALER            0x8C
#define REGMAP_IRQ_MASK_AND_PRIORITY    0x90
#define REGMAP_IRQ_PENDING              0x94
#define REGMAP_IRQ_FORCE                0x98
#define REGMAP_IRQ_CLEAR                0x9C
#define REGMAP_IOPORT_IO                0xA0
#define REGMAP_IOPORT_DIRECTION         0xA4
#define REGMAP_IOPORT_IRQ_CONFIG        0xA8
#define REGMAP_SECONDARY_IRQ_MASK       0xB0
#define REGMAP_SECONDARY_IRQ_PENDING    0xB4
#define REGMAP_SECONDARY_IRQ_STATUS     0xB8
#define REGMAP_SECONDARY_IRQ_CLEAR      0xBC
#define REGMAP_DSU_UART_STATUS          0xC4
#define REGMAP_DSU_UART_CONTROL         0xC8
#define REGMAP_DSU_UART_SCALER          0xCC


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
