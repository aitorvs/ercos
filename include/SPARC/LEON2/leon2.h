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
#include <cpu/cpu.h>
#include <arch/regmap.h>

//------------------------------------------------------------------------------

/*  DEFINES */

/** \brief Reset Trap Vector */
#define TRAP_RESET                      0x00
/** \brief Instruction Access Trap Vector */
#define TRAP_INSTRUCTION_ACCESS         0x01
/** \brief Illegal Instruction Trap Vector */
#define TRAP_ILLEGAL_INSTRUCTION        0x02
/** \brief Privileged Instruction Trap Vector */
#define TRAP_PRIVILEGED_INSTRUCTION     0x03
/** \brief Floating-Point Unit Disabled Trap Vector */
#define TRAP_FP_DISABLED                0x04
/** \brief Memory Access not Alligned Trap Vector */
#define TRAP_MEM_ADDRESS_NOT_ALIGNED    0x07
/** \brief Floating-Point Unit Exception Trap Vector */
#define TRAP_FP_EXCEPTION               0x08
/** \brief Data Access Exception Trap Vector */
#define TRAP_DATA_ACCESS_EXCEPTION      0x09
/** \brief Tag Overflow Trap Vector */
#define TRAP_TAG_OVERFLOW               0x0A
/** \brief Watchpoint Detected Trap Vector */
#define TRAP_WATCHPOINT_DETECTED        0x0B
/** \brief Register Hardware Error Trap Vector */
#define TRAP_REGISTER_HW_ERROR          0x20
/** \brief Coprocessor Unit Disabled Trap Vector */
#define TRAP_CP_DISABLED                0x24
/** \brief Divide Exception Trap Vector */
#define TRAP_DIVIDE_EXCEPTION           0x2A
/** \brief Write Buffer Error */
#define TRAP_WRITE_ERROR                0x2B

/*  0x0B - 0x10 not handle interrupts   */

#define TRAP_AHB_ERROR 			0x11
#define TRAP_UART_B			0x12
#define TRAP_UART_A			0x13
#define TRAP_PARALLEL_IO_0		0x14
#define TRAP_PARALLEL_IO_1		0x15
#define TRAP_PARALLEL_IO_2		0x16
#define TRAP_PARALLEL_IO_3		0x17
#define TRAP_GP_TIMER			0x18
#define TRAP_REAL_TIME_CLOCK		0x19
#define TRAP_2ND_INT_CONTROLLER  	0x1a
#define TRAP_DSU_TRACE_BUFFER		0x1b
#define TRAP_ETHERNET_MAC		0x1c
#define TRAP_USER_DEFINED_1		0x1d
#define TRAP_PCI			0x1e
#define TRAP_USER_DEFINED_2		0x1f

/** \brief Minimum Trap Vector Number */
#define TRAP_MIN_NUMBER                 0
/** \brief Maximum Trap Vector Number */
#define TRAP_MAX_NUMBER                 32 
/** \brief Target Number of IRQs */
#define TARGET_NUMBER_OF_INTERRUPTS     32

/** Bits in the timer control register  */
#define LEON2_TIMER_CONTROL_ENABLE      (0x1)
#define LEON2_TIMER_CONTROL_RELOAD      (0x2)
#define LEON2_TIMER_CONTROL_LOAD        (0x4)

/****************   UART DEFINES   ***********************/

#define LEON2_UART_DR     (0x01)     /*  Data ready in channel A */
#define LEON2_UART_TSE    (1 << 1)   /*  No data to send */
#define LEON2_UART_THE    (1 << 2)   /*  Ready to load data  */
#define LEON2_UART_BR     (1 << 3)   /*  Break Received */
#define LEON2_UART_OE     (1 << 4)   /*  Overrun error in receiver */
#define LEON2_UART_PE     (1 << 5)   /*  Parity error in receiver */
#define LEON2_UART_FE     (1 << 6)   /*  Framing error in receiver */

/** LEON3 typically takes 3us to start/stop the timer  */
#define LEON2_AVG_TIMER_OVERHEAD  3

/** 
 * Timer Counter value. The timer counter in LEON2 is a 24 bit timer counter
 */
#define LEON2_TIMER_COUNTER 0x00FFFFFFULL

//------------------------------------------------------------------------------

/** 
 * This MACRO clears an occurred interrupt by setting the corresponding bit
 * in the interrupt clear register.
 */
#define TARGET_Clear_interrupt( _vector ) \
    do { \
        _CPU_DisableIRQ(); \
        REGMAP.irq_clear = (1 << (_vector & 0xf)); \
        _CPU_EnableIRQ(); \
    } while (0)

/** 
 * This MACRO dissables an interrupt by clearing the corresponding mask bit
 */
#define TARGET_Mask_interrupt( _vector ) \
    do { \
        _CPU_DisableIRQ(); \
        REGMAP.irq_mask_and_priority &= ~(1 << (_vector & 0xf)); \
        _CPU_EnableIRQ(); \
    } while (0)

/** 
 * This MACRO enables an interrupt by setting the corresponding mask bit
 */
#define TARGET_Enable_interrupt( _vector ) \
    do { \
        _CPU_DisableIRQ(); \
        REGMAP.irq_mask_and_priority |= (1 << (_vector & 0xf)); \
        _CPU_EnableIRQ(); \
    } while (0)

/** 
 * This macro mask all the interrupt sources
 */
#define TARGET_Mask_all_interrupts() \
    do { \
        _CPU_DisableIRQ(); \
        REGMAP.irq_mask_and_priority = 0x0; \
        _CPU_EnableIRQ(); \
    } while (0)

/** 
 * This macro force an interrupt source
 */
#define TARGET_Force_interrupt( _source ) \
  do { \
    REGMAP.irq_force = (1 << (_source)); \
  } while (0)

/** 
 * This MACRO sets the Timer1 Control register. Timer used as a general
 * purpose timer counter.
 */
#define LEON2_Set_GP_Timer_Control( _value ) \
    do { \
        _CPU_DisableIRQ(); \
        REGMAP.timer_1_control = _value; \
        _CPU_EnableIRQ(); \
    } while ( 0 )

/**
 * This MACRO returns the General Purpose Timer counter register value
 */
#define LEON2_Get_GP_Timer_Counter( _value ) \
    do { \
        _value = (LEON2_TIMER_COUNTER - REGMAP.timer_1_counter) - LEON2_AVG_TIMER_OVERHEAD; \
    } while(0);

/** 
 * \brief Sets the Real Time Clock Control register.
 * 
 * \param _value  New value of the Real Time Clock Control Register.
 * 
 */
#define LEON2_Set_RT_Timer_Control( _value ) \
    do { \
        _CPU_DisableIRQ(); \
        REGMAP.timer_2_control = _value; \
        _CPU_EnableIRQ(); \
    } while ( 0 )
    
/**
 * This MACRO verifies if the vector passed is an interrupt vector
 */
#define TARGET_Is_Interrupt( _vec ) \
    ( _vec > 16 && _vec < 32 )
    

/**
 * This MACRO saves the L1 and L2 register values
 */
#define LEON2_Save_L1L2( _reg_l1, _reg_l2) \
    do { \
        asm volatile( "mov %%l1, %0" :  "=r" (_reg_l1) : "0" (_reg_l1) ); \
        asm volatile( "mov %%l2, %0" :  "=r" (_reg_l2) : "0" (_reg_l2) ); \
    }while(0)
    
/**
 * This MACRO resotores the L1 and L1 register values
 */
#define LEON2_Restore_L1L2( _reg_l1, _reg_l2) \
    do { \
    asm volatile ( "mov  %0, %%l1 " : "=r" ((_reg_l1)) : "0" ((_reg_l1)) ); \
    asm volatile ( "mov  %0, %%l2 " : "=r" ((_reg_l2)) : "0" ((_reg_l2)) ); \
    }while(0)


#endif
