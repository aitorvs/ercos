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
 * \file include/sparc/ERC32/erc32.h
 * \brief ERC32-specific MACROS and definitions.
 */

#ifndef _ERC32_H_
#define _ERC32_H_

#include <cpu/cputypes.h>
#include <arch/mec.h>

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
#define TRAP_MEMORY_NOT_ALIGMENT        0x07
/** \brief Floating-Point Unit Exception Trap Vector */
#define TRAP_FP_EXCEPTION               0x08
/** \brief Data Access Exception Trap Vector */
#define TRAP_DATA_ACCESS_EXCEPTION      0x09
/** \brief Tag Overflow Trap Vector */
#define TRAP_TAG_OVERFLOW               0x0a

/*  0x0B - 0x10 unhandled interrupts   */

/** \brief Maskarable HW Errors Trap Vector */
#define TRAP_MASK_HARDWARE_ERRORS       0x11
/** \brief External IRQ 0 Trap Vector */
#define TRAP_EXTERNAL_INT0              0x12
/** \brief External IRQ 1 Trap Vector */
#define TRAP_EXTERNAL_INT1              0x13
/** \brief UART A Data Ready Trap Vector */
#define TRAP_UART_A                     0x14
/** \brief UART B Data Ready Trap Vector */
#define TRAP_UART_B                     0x15
/** \brief Memory Error Trap Vector */
#define TRAP_MEMORY_ERROR               0x16
/** \brief UART Error Trap Vector */
#define TRAP_UART_ERROR                 0x17
/** \brief DMA Access Error Trap Vector */
#define TRAP_DMA_ACCESS_ERROR           0x18
/** \brief DMA Timeout Trap Vector */
#define TRAP_DMA_TIMEOUT                0x19
/** \brief External IRQ 2 Trap Vector */
#define TRAP_EXTERNAL_INT2              0x1a
/** \brief External IRQ 3 Trap Vector */
#define TRAP_EXTERNAL_INT3              0x1b
/** \brief General Purpose Timer Overflow Trap Vector */
#define TRAP_GP_TIMER                   0x1c
/** \brief Real Time Clock Overflow Trap Vector */
#define TRAP_REAL_TIME_CLOCK            0x1d
/** \brief External IRQ 4 Trap Vector */
#define TRAP_EXTERNAL_INT4              0x1e
/** \brief Watchdog Trap Vector */
#define TRAP_WATCHDOG                   0x1f

/** \brief Minimum Trap Vector Number */
#define TRAP_MIN_NUMBER                 0
/** \brief Maximum Trap Vector Number */
#define TRAP_MAX_NUMBER                 32
/** \brief Target Number of IRQs */
#define TARGET_NUMBER_OF_INTERRUPTS      32


/* Timer control register Masks  */

/** \brief MEC Timer Control Register Timer Counter Reload Bitmask */
#define ERC32_MEC_TIMER_COUNTER_RELOAD          (0x1)
/** \brief MEC Timer Control Register Timer Counter Load Bitmask */
#define ERC32_MEC_TIMER_COUNTER_LOAD            (0x2)
/** \brief MEC Timer Control Register Timer Counter Enable Bitmask */
#define ERC32_MEC_TIMER_COUNTER_ENABLE          (0x4)
/** \brief MEC Timer Control Register Timer Counter Scaler Load Bitmask */
#define ERC32_MEC_TIMER_COUNTER_SCALER_LOAD     (0x8)
/** \brief MEC Timer Control Register Counter Defined Mask */
#define ERC32_MEC_TIMER_COUNTER_DEFINED_MASK       0x0000000F
/** \brief MEC Timer Control Register Current Mode Mask */
#define ERC32_MEC_TIMER_COUNTER_CURRENT_MODE_MASK  0x00000005


/****************   UART DEFINES   ***********************/

#define ERC32_MEC_UART_DRA     0x01       /**<  Data ready in channel A      */
#define ERC32_MEC_UART_TSEA    (1 << 1)   /**<  No data to send              */
#define ERC32_MEC_UART_THEA    (1 << 2)   /**<  Ready to load data           */
//Reserved
#define ERC32_MEC_UART_FEA     (1 << 4)   /**<  Framing error in receiver A  */
#define ERC32_MEC_UART_PEA     (1 << 5)   /**<  Parity error in receiver A   */
#define ERC32_MEC_UART_OEA     (1 << 6)   /**<  Overrun error in receiver A  */
#define ERC32_MEC_UART_CUA     (1 << 7)   /**<  Clear UART A                 */
//Reserved 8-15
#define ERC32_MEC_UART_DRB     (1 << 16)  /**<  Data ready in channel B      */
#define ERC32_MEC_UART_TSEB    (1 << 17)  /**<  No data to send              */
#define ERC32_MEC_UART_THEB    (1 << 18)  /**<  Ready to load data           */
//Reserved
#define ERC32_MEC_UART_FEB     (1 << 20)  /**<  Framing error in receiver B  */
#define ERC32_MEC_UART_PEB     (1 << 21)  /**<  Parity error in receiver B   */
#define ERC32_MEC_UART_OEB     (1 << 22)  /**<  Overrun error in receiver B  */
#define ERC32_MEC_UART_CUB     (1 << 23)  /**<  Celar UART  B                */
//Reserved 24-31



/** ERC-32 typically takes 3us to start/stop the timer  */
#define ERC32_AVG_TIMER_OVERHEAD  3

/** 
 * \brief General Purpose Timer Counter Reload Value */
#define     ERC32_TIMER_COUNTER 0xFFFFFFFFULL

//------------------------------------------------------------------------------

/** 
 * \brief Clears an occurred interrupt by setting the corresponding bit in the
 *        interrupt clear register.
 * 
 * \param _vector  Vector of the trap to clear.
 */
#define TARGET_Clear_interrupt( _vector ) \
    do { \
        _CPU_DisableIRQ(); \
        ERC32_MEC.interrupt_clear = (1 << (_vector & 0xf)); \
        _CPU_EnableIRQ(); \
    } while (0)

/** 
 * \brief Disables an interrupt by setting the corresponding mask bit.
 * 
 * \param _vector  Vector of the trap to disable
 */
#define TARGET_Mask_interrupt( _vector ) \
    do { \
        _CPU_DisableIRQ(); \
        ERC32_MEC.interrupt_mask |= (1 << (_vector & 0xf)); \
        _CPU_EnableIRQ(); \
    } while (0)

/** 
 * \brief Enables an interrupt by clearing the corresponding mask bit.
 * 
 * \param _vector  Vector of the trap to enable.
 */
#define TARGET_Enable_interrupt( _vector ) \
    do { \
        _CPU_DisableIRQ(); \
        ERC32_MEC.interrupt_mask &= ~(1 << (_vector & 0xf)); \
        _CPU_EnableIRQ(); \
    } while (0)

/** 
 * \brief Masks all the interrupt sources.
 */
#define TARGET_Mask_all_interrupts() \
    do { \
        _CPU_DisableIRQ(); \
        ERC32_MEC.interrupt_mask = 0x7FFE; \
        _CPU_EnableIRQ(); \
    } while (0)

/** 
 * \brief Forces an external interrupt source.
 * 
 * \param _source  External IRQ source to force.
 */
#define TARGET_Force_interrupt( _source ) \
  do { \
    ERC32_MEC.test_control = ERC32_MEC.test_control | 0x80000; \
    ERC32_MEC.interrupt_force = (1 << (_source)); \
  } while (0)

extern uint32 __ERC32_MEC_Timer_Control_Mirror;

/** 
 * \brief Sets the General Purpose Timer Control register. 
 * 
 * \param _value  New value of the General Purpose Timer Control Register.
 * 
 * It stores the previous value in the __ERC32_MEC_Timer_Control_Mirror
 * variable defined in crt0.S file.
 */
#define ERC32_MEC_Set_GP_Timer_Control( _value ) \
    do { \
        uint32 _control; \
        uint32 __value; \
        \
        __value = ((_value) & 0x0f); \
        _CPU_DisableIRQ(); \
        _control = __ERC32_MEC_Timer_Control_Mirror; \
        _control &= ERC32_MEC_TIMER_COUNTER_DEFINED_MASK << 8; \
        __ERC32_MEC_Timer_Control_Mirror = _control | _value; \
        _control &= (ERC32_MEC_TIMER_COUNTER_CURRENT_MODE_MASK << 8); \
        _control |= __value; \
        ERC32_MEC.timer_control = _control; \
        _CPU_EnableIRQ(); \
    } while ( 0 )


/** 
 * \brief Sets the Real Time Clock Control register.
 * 
 * \param _value  New value of the Real Time Clock Control Register.
 * 
 * It stores the previous value in the __ERC32_MEC_Timer_Control_Mirror
 * variable defined in crt0.S file.
 */
#define ERC32_MEC_Set_RT_Timer_Control( _value ) \
    do { \
        uint32 _control; \
        uint32 __value; \
        \
        __value = ((_value) & 0x0f) << 8; \
        _CPU_DisableIRQ(); \
        _control = __ERC32_MEC_Timer_Control_Mirror; \
        _control &= ERC32_MEC_TIMER_COUNTER_DEFINED_MASK; \
        __ERC32_MEC_Timer_Control_Mirror = _control | __value; \
        _control &= ERC32_MEC_TIMER_COUNTER_CURRENT_MODE_MASK; \
        _control |= __value; \
        ERC32_MEC.timer_control = _control; \
        _CPU_EnableIRQ(); \
    } while ( 0 )

/**
 * \brief Returns the General Purpose Timer Counter Register value.
 * 
 * \param _value  Stores the General Purpose Timer Counter Register.
 */
#define ERC32_MEC_Get_GP_Timer_Counter( _value ) \
    do { \
        _value = (ERC32_TIMER_COUNTER - ERC32_MEC.gp_timer_counter) - ERC32_AVG_TIMER_OVERHEAD; \
    } while(0);
    
/**
 * \brief Verifies if the vector passed is an interrupt vector
 * 
 * \param _vec  Vector to check.
 */
#define TARGET_Is_Interrupt( _vec ) \
    ( _vec > 16 && _vec < 32 )
    

/**
 * \brief aves the L1 and L2 registers values.
 * 
 * \param _reg_l1  Stores the value of the L1 Register.
 * \param _reg_l2  Stores the value of the L2 Register.
 */
#define TARGET_Save_L1L2( _reg_l1, _reg_l2) \
    do { \
        asm volatile( "mov %%l1, %0" :  "=r" (_reg_l1) : "0" (_reg_l1) ); \
        asm volatile( "mov %%l2, %0" :  "=r" (_reg_l2) : "0" (_reg_l2) ); \
    }while(0)
    
/**
 * \brief Restores the L1 and L1 Register values
 * 
 * \param _reg_l1  New value of the L1 Register.
 * \param _reg_l2  New value of the L2 Register.
 */
#define TARGET_Restore_L1L2( _reg_l1, _reg_l2) \
    do { \
    asm volatile ( "mov  %0, %%l1 " : "=r" ((_reg_l1)) : "0" ((_reg_l1)) ); \
    asm volatile ( "mov  %0, %%l2 " : "=r" ((_reg_l2)) : "0" ((_reg_l2)) ); \
    }while(0)
    
    
#endif
