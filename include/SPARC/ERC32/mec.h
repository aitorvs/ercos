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
 * \file include/sparc/ERC32/mec.h
 * \brief Defines the structure mec used to manage the registers of the Memory
 *        Controller Unit.
 */

#ifndef _MEC_H_
#define _MEC_H_

#include <autoconf.h>

//----------------------------------------------------------------------------

/**
 * \brief Structure used to manage the registers of the Memory Controller
 *        Unit in C/C++ Code.
 */
struct mec
{
	/** \brief Control Register */
	unsigned int control_register;          /* 00 */
	/** \brief Software Reset Register */
	unsigned int software_reset;            /* 04 */
	/** \brief Power Down Register */
	unsigned int power_down;                /* 08 */
	/** \brief Padding */
	unsigned int foo1;                      /* 0C */
	/** \brief Memory Configuration Register */
	unsigned int memory_configuration;      /* 10 */
	/** \brief I/O Configuration Register */
	unsigned int io_configuration;          /* 14 */
	/** \brief Waitstate Configuration Register */
	unsigned int waitstate_configuration;   /* 18 */
	/** \brief Padding */
	unsigned int foo2;                      /* 1C */
	/** \brief Access Protection Segment 1 Base Register */
	unsigned int ap_segment1_base;          /* 20 */
	/** \brief Access Protection Segment 1 End Register */
	unsigned int ap_segment1_end;           /* 24 */
	/** \brief Access Protection Segment 2 Base Register */
	unsigned int ap_segment2_base;          /* 28 */
	/** \brief Access Protection Segment 2 End Register */
	unsigned int ap_segment2_end;           /* 2C */
	/** \brief Padding */
	unsigned int foo3[5];                   /* 30-40 */
	/** \brief Interrupt Shape Register */
	unsigned int interrupt_shape;           /* 44 */
	/** \brief Interrupt Pending Register */
	unsigned int interrupt_pending;         /* 48 */
	/** \brief Interrupt Mask Register */
	unsigned int interrupt_mask;            /* 4C */
	/** \brief Interrupt Clear Register */
	unsigned int interrupt_clear;           /* 50 */
	/** \brief Interrupt Force Register */
	unsigned int interrupt_force;           /* 54 */
	/** \brief Padding */
	unsigned int foo4[2];                   /* 58-5C */
	/** \brief Watchdog Register */
	unsigned int watchdog_register;         /* 60 */
	/** \brief Watchdog Trap Door Register */
	unsigned int watchdog_trap_door;        /* 64 */
	/** \brief Padding */
	unsigned int foo5[6];                   /* 68-7C */
	/** \brief Real Time Clock Counter Register */
	unsigned int rt_clock_counter;          /* 80 */
	/** \brief Real Time Clock Scaler Register */
	unsigned int rt_clock_scaler;           /* 84 */
	/** \brief General Purpose Timer Counter Register */
	unsigned int gp_timer_counter;          /* 88 */
	/** \brief General Purpose Timer Scaler Register */
	unsigned int gp_timer_scaler;           /* 8C */
	/** \brief Padding */
	unsigned int foo6[2];                   /* 90-94 */
	/** \brief Timer Control Register */
	unsigned int timer_control;             /* 98 */
	/** \brief Padding */
	unsigned int foo7;                      /* 9C */
	/** \brief System Fault Status Register */
	unsigned int system_fault_status;       /* A0 */
	/** \brief Failing address Register */
	unsigned int failing_address;           /* A4 */
	/** \brief Padding */
	unsigned int foo8[2];                   /* A8-AC */
	/** \brief Error and Reset Status Register */
	unsigned int error_and_reset_status;    /* B0 */
	/** \brief Padding */
	unsigned int foo9[7];                   /* B4-CC */
	/** \brief Test Control Register */
	unsigned int test_control;              /* D0 */
	/** \brief Padding */
	unsigned int foo0[3];                   /* D4-DC */
	/** \brief UART Channel 1 Register */
	unsigned int uart_channel1;             /* E0 */
	/** \brief UART Channel 2 Register */
	unsigned int uart_channel2;             /* E4 */
	/** \brief UART Status Register */
	unsigned int uart_status;               /* E8 */
};

/** 
 * \brief MEC Structure.
 * 
 * It is located at linking time by using the link.ld script file
 */
extern volatile struct mec ERC32_MEC;

#endif
