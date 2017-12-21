/* hpc2.h - HPC-II board header file */

/* Copyright (c) 2006 Wind River Systems, Inc. */
/* Copyright 2005 Tundra Semiconductor Corp. */

/*
modification history
--------------------
01c,07feb06,avb  Added SNOOP_ENABLE to setup DISABLE_PBM option
01b,10jan06,dtr  Cleanup for WindRiver release.
01a,21jul05,avb  written (based on templatePpc/template.h)
*/

/*
This file contains I/O addresses and related constants for the HPC2 BSP.
*/

#ifndef	INChpc2h
#define	INChpc2h

#ifdef __cplusplus
extern "C" {
#endif

#define N_SIO_CHANNELS		2	/* Number of serial I/O channels */

/* Local I/O address map */

/* timer constants */

#define	SYS_CLK_RATE_MIN  	3	/* minimum system clock rate */
#define	SYS_CLK_RATE_MAX  	5000	/* maximum system clock rate */
#define	AUX_CLK_RATE_MIN  	3	/* minimum auxiliary clock rate */
#define	AUX_CLK_RATE_MAX  	5000	/* maximum auxiliary clock rate */


#ifdef INCLUDE_PCI

#ifndef INCLUDE_PCI_AUTOCONF
#define INCLUDE_PCI_AUTOCONF	/* enforce PCI autoconfig */
#endif

#ifndef PCI_CFG_TYPE
#define PCI_CFG_TYPE	PCI_CFG_AUTO
#endif /* PCI_CFG_TYPE */


#define HOST_BRIDGE_IDSEL	0
#define PCI_SLOT1_DEVNO		1	/* PCI SLOT 1 Device no */

/* Translate PCI addresses to virtual addresses (master windows) */

#define PCI_MEM2LOCAL(x) \
	    ((void *)((UINT)(x) - PCI_MSTR_MEM_BUS + PCI_MSTR_MEM_LOCAL))

#define PCI_IO2LOCAL(x) \
	    ((void *)((UINT)(x) - PCI_MSTR_IO_BUS + PCI_MSTR_IO_LOCAL))

#define PCI_MEMIO2LOCAL(x) \
	    ((void *)((UINT)(x) - PCI_MSTR_MEMIO_BUS + PCI_MSTR_MEMIO_LOCAL))

/* Translate local memory address to PCI address (slave window) */

#define LOCAL2PCI_MEM(x) \
	    ((void *)((UINT)(x) - PCI_SLV_MEM_LOCAL + PCI_SLV_MEM_BUS))
#endif /* INCLUDE_PCI */

/*
 * Miscellaneous definitions go here. For example, macro definitions
 * for various devices.
 */

#ifndef CPU
#error "CPU not defined"
#endif

#if (CPU == PPC603)
#   error "This BSP will not run on PPC603!\n";
#else
#   define WRONG_CPU_MSG "ERROR: Unknown CPU type!\n";
#endif

#define CPU_TYPE        ((vxPvrGet() >> 16) & 0xffff)
#define CPU_TYPE_7457   0x8002  /* MPC 7457 CPU */
#define CPU_TYPE_7447A  0x8003  /* MPC 7447A CPU */
#define CPU_TYPE_7448   0x8004  /* MPC 7448 CPU */
    
/* C PPC syncronization macro */

#define PPC_EIEIO_SYNC	WRS_ASM (" eieio; sync")


/*******************************************************/
/* Resources on the board :                            */
/*******************************************************/

#define TSI108_CSR_BASE		TSI108_CSR_BASE_RST
#define TSI108_CSR_SIZE		0x10000 /* 64 KB */

/* 
 * PCI Mapping Section.
 */

/* PCI CFG master window */

#define	PCI_MSTR_CFG_LOCAL	0xfe000000
#define	PCI_MSTR_CFG_SIZE	0x01000000


/* PCI I/O master window */

#define	PCI_MSTR_IO_LOCAL	0xfd000000
#define	PCI_MSTR_IO_BUS		0xfd000000
#define	PCI_MSTR_IO_SIZE	0x00010000

/* PCI MEMIO master (non-prefetch memory) window */

#define	PCI_MSTR_MEMIO_LOCAL	0xe0000000
#define	PCI_MSTR_MEMIO_BUS	0xe0000000
#define	PCI_MSTR_MEMIO_SIZE	0x08000000

/* PCI MEM master (prefetchable memory) window */

#define	PCI_MSTR_MEM_LOCAL	0xe8000000
#define	PCI_MSTR_MEM_BUS	0xe8000000
#define	PCI_MSTR_MEM_SIZE	0x08000000

#define DEC_CLOCK_FREQ	sysPbClock       /* 200MHz default */

#define PCI_LAT_TIMER		0xff	/* default latency timer value */
#define PCI_CFG_TYPE		PCI_CFG_AUTO 

#undef DISABLE_PBM

#ifndef SNOOP_ENABLE
#define DISABLE_PBM	/* disable snooping path */
#endif

#undef SDC_HARDCODED_INIT
#undef ENABLE_SDRAM_ECC
#undef SDC_AUTOPRECH_EN
#undef CFG_BUS_CLK
#undef CFG_CLK_SPREAD
#undef ENABLE_PCI_CSR_BAR

#ifdef SDC_HARDCODED_INIT

/*===========================================================================
 * Hardcoded SDC settings
 */

/* Micron MT9HTF6472AY-40EA1 : Unbuffered,512MB,400,CL3, Single Rank */
#define VAL_SD_REFRESH	(0x61A)
#define VAL_SD_TIMING	(0x0308334b)
#define VAL_SD_D0_CTRL	(0x00100021) /* auto-precharge disabled */
#define VAL_SD_D0_BAR	(0x0FE00000) /* 512MB @ 0x00000000 */
#define VAL_SD_D1_CTRL	(0x00100020) /* auto-precharge disabled */
#define VAL_SD_D1_BAR	(0x0FE00200) /* 512MB @ 0x20000000 */

#else /* !SDC_HARDCODED_INIT */

/*===========================================================================
 * SDRAM SPD Support Definitions and Macros
 */

#define SPD_DIMM0		(0x00000100)
#define SPD_DIMM1		(0x00000200)

#define SPD_RDIMM		(0x01)
#define SPD_UDIMM		(0x02)

#define SPD_CAS_3		0x8
#define SPD_CAS_4		0x10
#define SPD_CAS_5		0x20

#define ERR_NO_DIMM_FOUND	(0xdb0)
#define ERR_TRAS_FAIL		(0xdb1)
#define ERR_TRCD_FAIL		(0xdb2)
#define ERR_TRP_FAIL		(0xdb3)
#define ERR_TWR_FAIL		(0xdb4)
#define ERR_UNKNOWN_PART	(0xdb5)
#define ERR_NRANK_INVALID	(0xdb6)
#define ERR_DIMM_SIZE		(0xdb7)
#define ERR_ADDR_MODE		(0xdb8)
#define ERR_RFRSH_RATE		(0xdb9)
#define ERR_DIMM_TYPE		(0xdba)
#define ERR_CL_VALUE		(0xdbb)
#define ERR_TRFC_FAIL		(0xdbc)

#endif /* SDC_HARDCODED_INIT */

/* PHY address definitions for TSI108 GIGE ports 0 & 1 */
#define TSI108_PHY0_ADDR	(8)
#define TSI108_PHY1_ADDR	(9)
#define TSI108_PHY_MV88E10XX

#ifndef EIEIO_SYNC
#   define EIEIO_SYNC	WRS_ASM ("eieio; sync")
#endif  /* EIEIO_SYNC */

#ifndef EIEIO
#   define EIEIO	WRS_ASM ("eieio")
#endif	/* EIEIO */

#ifndef SYNC
#   define SYNC		WRS_ASM ("sync") 
#endif  /* SYNC */

/* serial ports (COM1,COM2) */

#define N_SIO_CHANNELS		2 /* Number of serial I/O channels */
#define N_UART_CHANNELS		N_SIO_CHANNELS
#define COM1_BASE_ADR		(TSI108_CSR_BASE + TSI108_UART0_OFFSET + 8)
#define COM2_BASE_ADR		(TSI108_CSR_BASE + TSI108_UART1_OFFSET + 8)
#define UART_REG_ADDR_INTERVAL	1
#define UART_XTAL_FREQ		133000000
#define DEFAULT_BAUD_RATE	115200

#ifndef _ASMLANGUAGE

/* PCI I/O function defines */

#ifndef PCI_IN_BYTE
#define PCI_IN_BYTE(x)		sysPciInByte (x)
#endif
IMPORT UINT8			sysPciInByte  (UINT32 address);
#ifndef PCI_IN_WORD
#define PCI_IN_WORD(x)		sysPciInWord (x)
#endif
IMPORT UINT16			sysPciInWord (UINT32 address);
#ifndef PCI_IN_LONG
#define PCI_IN_LONG(x)		sysPciInLong (x)
#endif
IMPORT UINT32			sysPciInLong  (UINT32 address);

#ifndef PCI_OUT_BYTE
#define PCI_OUT_BYTE(x,y)	sysPciOutByte (x,y)
#endif
IMPORT void			sysPciOutByte (UINT32 address, UINT8  data);
#ifndef PCI_OUT_WORD
#define PCI_OUT_WORD(x,y)	sysPciOutWord (x,y)
#endif
IMPORT void			sysPciOutWord (UINT32 address, UINT16 data);
#ifndef PCI_OUT_LONG
#define PCI_OUT_LONG(x,y)	sysPciOutLong (x,y)
#endif
IMPORT void			sysPciOutLong (UINT32 address, UINT32 data);

#endif  /* _ASMLANGUAGE */

/* NVRAM */
#if defined (INCLUDE_NVRAM)
#   define NV_RAM_BASE_PHYS	0xfc000000
#   define NV_RAM_SIZE_PHYS	0x8000
#   define NV_RAM_RESERVED	0x4000	    /* Reserved for U-Boot */
#   define NV_RAM_SIZE		(NV_RAM_SIZE_PHYS - NV_RAM_RESERVED)
#   define NV_RAM_ADRS		(NV_RAM_BASE_PHYS + NV_RAM_RESERVED)
#   define NV_RAM_INTRVL	0x00000001
#   define NV_RAM_READ(x)	sysNvRead(x)	/* sysLib.c */
#   define NV_RAM_WRITE(x,y)	sysNvWrite(x,y)	/* sysLib.c */
#else
#   define NV_RAM_SIZE		0
#   define NV_RAM_ADRS		00000000
#   define NV_RAM_INTRVL	00000000
#endif /* INCLUDE_NVRAM */

/* Flash Support */

#ifdef INCLUDE_FLASH

/*
 * 29LV640D flash is not supported in flashMem.c directly but is similar
 * in procedures with 29LV160B
 */

#define FLASH_29LV640D	FLASH_29LV160B

#undef  SYS_FLASH_TYPE
#define SYS_FLASH_TYPE		FLASH_29LV640D
#undef  FLASH_WIDTH
#define FLASH_WIDTH		4		/* 4 byte wide */
#undef  FLASH_CHIP_WIDTH
#define FLASH_CHIP_WIDTH	2		/* 2 chip */
#undef  FLASH_SIZE
#define FLASH_SIZE		0x01000000	/* 16 Megs */

#define FLASH_ADRS		0xff000000	/* Flash device on Bank 0 */
#define FLASH_SIZE_WRITEABLE	FLASH_SIZE	/* write all of flash */
#define SYS_FLASH_WRITE				/* enable writting to flash */
#define FLASH_NO_OVERLAY			/* blow away old flash data */
#define SYS_FLASH_BOARD_DELAY			/* sysLib.c uSec delay rtn. */

#endif /* INCLUDE_FLASH */

#ifdef __cplusplus
}
#endif

#endif	/* INChpc2h */
