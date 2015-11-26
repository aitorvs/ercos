/*
    Copyright 2006 (c) by Aitor Viana Sanchez, Pablo Parra Espada
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT on LEON3.
*/


#ifndef _LEON3_H_
#define _LEON3_H_

#include <cpu/cputypes.h>
#include <arch/regmap.h>

//------------------------------------------------------------------------------

/*  DEFINES */

#define TRAP_RESET                      0x00
#define TRAP_INSTRUCTION_ACCESS         0x01
#define TRAP_ILLEGAL_INSTRUCTION        0x02
#define TRAP_PRIVILEGED_INSTRUCTION     0x03
#define TRAP_FP_DISABLED                0x04
#define TRAP_MEMORY_NOT_ALIGNED         0x07
#define TRAP_FP_EXCEPTION               0x08
#define TRAP_DATA_ACCESS_EXCEPTION      0x09
#define TRAP_TAG_OVERFLOW               0x0a
#define TRAP_WATCHPOINT_DETECTED	0x0b
#define TRAP_CP_DISABLED		0x20
#define TRAP_CP_EXCEPTION		0x28
#define TRAP_DIVIDE_BY_ZERO		0x2a
#define TRAP_WRITE_BUFFER_ERROR		0x2b

/*  0x0B - 0x10 not handle interrupts   */

#define TRAP_AHB_ERROR 			0x11
#define TRAP_UART_B			0x12
#define TRAP_UART_A			0x13
#define TRAP_PARALLEL_IO_0		0x14
#define TRAP_PARALLEL_IO_1		0x15
#define TRAP_PARALLEL_IO_2		0x16
#define TRAP_PARALLEL_IO_3		0x17
#define TRAP_REAL_TIME_CLOCK		0x18
#define TRAP_GP_TIMER			0x19
#define TRAP_2ND_INT_CONTROLLER  	0x1a
#define TRAP_DSU_TRACE_BUFFER		0x1b
#define TRAP_ETHERNET_MAC		0x1c
#define TRAP_USER_DEFINED_1		0x1d
#define TRAP_PCI			0x1e
#define TRAP_USER_DEFINED_2		0x1f

#define TRAP_MIN_NUMBER                 0
#define TRAP_MAX_NUMBER                 64
#define TARGET_NUMBER_OF_INTERRUPTS      64

/** Bits in the timer control register  */
#define GPTIMER_CONTROL_ENABLE    		(0x1)
#define GPTIMER_CONTROL_RESTART          	(0x2)
#define GPTIMER_CONTROL_LOAD          		(0x4)
#define GPTIMER_CONTROL_IRQ_ENABLE     	(0x8)
#define GPTIMER_CONTROL_IRQ_PENDING	(0x10)

#define GPTIMER_REAL_TIME 	0
#define GPTIMER_COUNTER 	1

/****************   UART DEFINES   ***********************/

#define LEON3_UART_DR     0x01       /*  Data ready in channel A */
#define LEON3_UART_TSE    (1 << 1)   /*  No data to send */
#define LEON3_UART_THE    (1 << 2)   /*  Ready to load data  */
//Reserved
#define LEON3_UART_FE     (1 << 4)   /*  Framing error in receiver A  */
#define LEON3_UART_PE     (1 << 5)   /*  Parity error in receiver A  */
#define LEON3_UART_OE     (1 << 6)   /*  Overrun error in receiver A */
#define LEON3_UART_CU     (1 << 7)   /*  Clear UART A    */

/** LEON3 typically takes 3us to start/stop the timer  */
#define LEON3_AVG_TIMER_OVERHEAD  3

/** Timer Counter value */
#define     LEON3_TIMER_COUNTER 0x00FFFFFF

//------------------------------------------------------------------------------

/** 
 * This MACRO clears an occurred interrupt by setting the corresponding bit
 * in the interrupt clear register.
 */
#define TARGET_Clear_interrupt( _vector ) \
    do { \
        _CPU_DisableIRQ(); \
        IRQMP.irq_clear = (1 << (_vector & 0xf)); \
        _CPU_EnableIRQ(); \
    } while (0)

/** 
 * This MACRO dissables an interrupt by setting the corresponding mask bit
 */
//#define TARGET_Mask_interrupt( _vector, _cpu )
#define TARGET_Mask_interrupt( _vector ) \
    do { \
        _CPU_DisableIRQ(); \
        IRQMP.irq_mask_processor[0] &= ~(1 << (_vector & 0xf)); \
        _CPU_EnableIRQ(); \
    } while (0)

/** 
 * This MACRO enables an interrupt by clearing the corresponding mask bit
 */
// #define TARGET_Enable_interrupt( _vector, _cpu )
#define TARGET_Enable_interrupt( _vector ) \
    do { \
        _CPU_DisableIRQ(); \
        IRQMP.irq_mask_processor[0] |= (1 << (_vector & 0xf)); \
        _CPU_EnableIRQ(); \
    } while (0)

/** 
 * This macro mask all the interrupt sources
 */
//#define TARGET_Mask_all_interrupts( _cpu ) 
#define TARGET_Mask_all_interrupts() \
    do { \
        _CPU_DisableIRQ(); \
        IRQMP.irq_mask_processor[0] = 0x0; \
        _CPU_EnableIRQ(); \
    } while (0)

/** 
 * This macro force an interrupt source
 */
#define TARGET_Force_interrupt( _source ) \
  do { \
    IRQMP.irq_force = (1 << (_source)); \
  } while (0)

/** 
 * This MACRO sets the Timer1 Control register. Timer used as a general
 * purpose timer counter.
 */
#define GPTIMER_Set_Timer_Control( _num, _value ) \
    do { \
        _CPU_DisableIRQ(); \
        GPTIMER.timer[_num].control = _value; \
        _CPU_EnableIRQ(); \
    } while ( 0 )

/**
 * This MACRO returns the General Purpose Timer counter register value
 */
#define GPTIMER_Get_Timer_Counter( _num, _value ) \
    do { \
        _value = (LEON3_TIMER_COUNTER - GPTIMER.timer[_num].counter_value) - LEON3_AVG_TIMER_OVERHEAD; \
    } while(0);
    
/**
 * This MACRO verifies if the vector passed is an interrupt vector
 */
#define TARGET_Is_Interrupt( _vec ) \
    ( _vec > 16 && _vec < 32 )
    

/**
 * This MACRO saves the L1 and L2 register values
 */
#define LEON3_Save_L1L2( _reg_l1, _reg_l2) \
    do { \
        asm volatile( "mov %%l1, %0" :  "=r" (_reg_l1) : "0" (_reg_l1) ); \
        asm volatile( "mov %%l2, %0" :  "=r" (_reg_l2) : "0" (_reg_l2) ); \
    }while(0)
    
/**
 * This MACRO resotores the L1 and L1 register values
 */
#define LEON3_Restore_L1L2( _reg_l1, _reg_l2) \
    do { \
    asm volatile ( "mov  %0, %%l1 " : "=r" ((_reg_l1)) : "0" ((_reg_l1)) ); \
    asm volatile ( "mov  %0, %%l2 " : "=r" ((_reg_l2)) : "0" ((_reg_l2)) ); \
    }while(0)


#endif
