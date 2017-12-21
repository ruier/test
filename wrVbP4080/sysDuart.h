/* sysDuart.h - wrVbP4080 duart header */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,14dec09,to   created
*/

/*
This file contains I/O addresses and related constants for the
wrVbP4080 Duart device.
*/

#ifndef	__INCsysDuarth
#define	__INCsysDuarth

#ifdef __cplusplus
extern "C" {
#endif

/* Duart Port Definitions */

#define	DUART1_BLOCK_BASE	0x11c000
#define	DUART2_BLOCK_BASE	0x11d000

#define	UART1_REG_OFFSET	0x500
#define	UART2_REG_OFFSET	0x600
#define	UART3_REG_OFFSET	0x500
#define	UART4_REG_OFFSET	0x600

#define	DUART1_BASE		((ULONG)CCSBAR + (ULONG)DUART1_BLOCK_BASE)
#define	DUART2_BASE		((ULONG)CCSBAR + (ULONG)DUART2_BLOCK_BASE)

#define	UART1_BASE		(DUART1_BASE + (ULONG)UART1_REG_OFFSET)
#define	UART2_BASE		(DUART1_BASE + (ULONG)UART2_REG_OFFSET)
#define	UART3_BASE		(DUART2_BASE + (ULONG)UART3_REG_OFFSET)
#define	UART4_BASE		(DUART2_BASE + (ULONG)UART4_REG_OFFSET)

#define	DUART_REG_ADDR_INTERVAL	1

#ifdef __cplusplus
}
#endif

#endif  /* __INCsysDuarth */
