/*
    Copyright 2006 (c) by Aitor Viana Sanchez, Pablo Parra Espada
    University of Alcala,
    Computer Engineering Department.

    For further information, please visit http://atc1.aut.uah.es

    This software is provided under the terms of the GNU General Public v2
    Licence. A full copy of the GNU GPL is provided in the file COPYING
    found in the development root of ERCOS-RT on LEON3.
*/

#ifndef _REGMAP_H_
#define _REGMAP_H_

#include <autoconf.h>

/**
 * \brief Structure used to manage the registers of the On-Chip Peripherals
 *        in C/C++ Code.
 */
struct regmap
{
	/** \brief Memory Configuration Register 1 */
	unsigned int memory_configuration_1;            /* 00 */
	/** \brief Memory Configuration Register 2 */
	unsigned int memory_configuration_2;            /* 04 */
	/** \brief Memory Configuration Register 3 */
	unsigned int memory_configuration_3;            /* 08 */
	/** \brief AHB Failing Address Register */
	unsigned int ahb_failing_address;               /* 0C */
	/** \brief AHB Status Register */
	unsigned int ahb_status;                        /* 10 */
	/** \brief Cache Control Register */
	unsigned int cache_control;                     /* 14 */
	/** \brief Power Down Register */
	unsigned int power_down;                        /* 18 */
	/** \brief Write Protection Register 1 */
	unsigned int write_protection_1;                /* 1C */
	/** \brief Write Protection Register 2 */
	unsigned int write_protection_2;                /* 20 */
	/** \brief LEON Configuration Register */
	unsigned int leon_configuration;                /* 24 */
	/** \brief Padding */
	unsigned int foo1[6];                           /* 28 */
	/** \brief Timer 1 Counter Register */ 
	unsigned int timer_1_counter;                   /* 40 */
	/** \brief Timer 1 Reload Register */
	unsigned int timer_1_reload;                    /* 44 */
	/** \brief Timer 1 Control Register */
	unsigned int timer_1_control;                   /* 48 */
	/** \brief Watchdog Register */
	unsigned int watchdog_register;                 /* 4C */
	/** \brief Timer 2 Counter Register */ 
	unsigned int timer_2_counter;                   /* 50 */
	/** \brief Timer 2 Reload Register */
	unsigned int timer_2_reload;                    /* 54 */
	/** \brief Timer 2 Control Register */
	unsigned int timer_2_control;                   /* 58 */
	/** \brief Padding */
	unsigned int foo2;                              /* 5C */
	/** \brief Prescaler Counter Register */
	unsigned int prescaler_counter;                 /* 60 */
	/** \brief Prescaler Reload Register */
	unsigned int prescaler_reload;                  /* 64 */
	/** \brief Padding */
	unsigned int foo3[2];                           /* 68 */
	/** \brief UART 1 Data Register */
	unsigned int uart_1_data;                       /* 70 */
	/** \brief UART 1 Status Register */
	unsigned int uart_1_status;                     /* 74 */
	/** \brief UART 1 Control Register */
	unsigned int uart_1_control;                    /* 78 */
	/** \brief UART 1 Scaler Register */
	unsigned int uart_1_scaler;                     /* 7C */
	/** \brief UART 2 Data Register */
	unsigned int uart_2_data;                       /* 80 */
	/** \brief UART 2 Status Register */
	unsigned int uart_2_status;                     /* 84 */
	/** \brief UART 2 Control Register */
	unsigned int uart_2_control;                    /* 88 */
	/** \brief UART 2 Scaler Register */
	unsigned int uart_2_scaler;                     /* 8C */
	/** \brief Interrupt Mask and Priority Register */
	unsigned int irq_mask_and_priority;             /* 90 */
	/** \brief Interrupt Pending Register */
	unsigned int irq_pending;                       /* 94 */
	/** \brief Interrupt Force Register */
	unsigned int irq_force;                         /* 98 */
	/** \brief Interrupt Clear Register */
	unsigned int irq_clear;                         /* 9C */
	/** \brief I/O Port input/output register */
	unsigned int ioport_io;                         /* A0 */
	/** \brief I/O Port Direction Register */
	unsigned int ioport_direction;                  /* A4 */
	/** \brief I/O Port Interrupt Config Register */
	unsigned int ioport_irq_config;                 /* A8 */
	/** \brief Padding */
	unsigned int foo4;                              /* AC */
	/** \brief Secondary Interrupt Mask Register */
	unsigned int secondary_irq_mask;                /* B0 */
	/** \brief Secondary Interrupt Pending Register */
	unsigned int secondary_irq_pending;             /* B4 */
	/** \brief Secondary Interrupt Status Register */
	unsigned int secondary_irq_status;              /* B8 */
	/** \brief Secondary Interrupt Clear Register */
	unsigned int secondary_irq_clear;               /* BC */
	/** \brief Padding */
	unsigned int foo5;                              /* C0 */
	/** \brief DSU UART Status Register */
	unsigned int dsu_uart_status;                   /* C4 */
	/** \brief DSU UART Control Register */
	unsigned int dsu_uart_control;                  /* C8 */
	/** \brief DSU UART Scaler Register */
	unsigned int dsu_uart_scaler;                   /* CC */
};

/** 
 * \brief REGMAP Structure.
 * 
 * It is located at linking time by using the link.ld script file
 */
extern volatile struct regmap REGMAP;

#endif
