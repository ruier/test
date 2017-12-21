/* ads827x.h - Motorola MPC827x ADS board header */

/*
 * Copyright (c) 2003-2005, 2007, 2009, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01h,30may13,d_l  Fix prevent error. (WIND00419006)
01g,10jun09,wqi  Correct MPC827x's interrupt numbers. (WIND00086544)
01f,16feb07,rec  vxWorks 6.5 update
01e,08sep05,dtr  Support storage of MAC in pseudoi NVRAM.
01d,30mar05,dtr  Backward compatible defines.
01c,28jan04,dtr  Adding defines for DPRAM.
01b,08jan04,dtr  Adding in SEC registers.
01a,18dec03,dtr  adapted from ads826x.h
*/

/*
This file contains I/O addresses and related constants for the
Motorola MPC8272 ADS board.
*/

#ifndef	INCads827xh
#define	INCads827xh

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef  _ASMLANGUAGE
# if (defined(_WRS_VXWORKS_MAJOR) && (_WRS_VXWORKS_MAJOR >= 6))
/* nothing needed */
# else  /* _WRS_VXWORKS_MAJOR */
typedef void * VIRT_ADDR;
typedef void * PHYS_ADDR;
# endif  /* _WRS_VXWORKS_MAJOR */
#endif  /* _ASMLANGUAGE */

#include "drv/intrCtl/m8260IntrCtl.h"
#include "m827xInNu.h"

#define EIEIO_SYNC WRS_ASM (" eieio; sync")

#define M82XX_STANDARD_DPRAM_START 0x100
#define M82XX_STANDARD_DPRAM_LIMIT 0x1000
#define M82XX_FCC_DPRAM_START 0x1000
#define M82XX_FCC_DPRAM_LIMIT 0x2000

#define SPD_DATA_SIZE         128
#undef  BOOT_LINE_SIZE
#define BOOT_LINE_SIZE        256
#undef  NV_RAM_SIZE
#define NV_RAM_SIZE           4096  
#define NV_RAM_SIZE_WRITEABLE NV_RAM_SIZE  /* force VTS to only use 128 bytes */
#define NV_RAM_READ(x)        sysNvRead (x)
#define NV_RAM_WRITE(x,y)     sysNvWrite (x,y)

/* add PCI access macros */
#define PCI_MEMIO2LOCAL(x) \
    (uint32_t)(((VUINT32)(x)  - PCI_MEMIO_ADRS) + CPU_PCI_MEMIO_ADRS)


/* PCI IO memory adrs to CPU (60x bus) adrs */
#define PCI_IO2LOCAL(x) \
    (uint32_t)(((VUINT32)x  - PCI_IO_ADRS) + CPU_PCI_IO_ADRS)

#define PCI_MEM2LOCAL(x) \
    (uint32_t)(((VUINT32)x  - PCI_MEM_ADRS) + CPU_PCI_MEM_ADRS)


/* 60x bus adrs to PCI (non-prefetchable) memory address */

#define LOCAL2PCI_MEMIO(x) \
     (uint32_t)((VUINT32)(x) + PCI_MSTR_MEM_BUS)


/* PCI defines begin */

#define PCI_BRIDGE_INTR_CTL_BA      0x4730000
#define PCI_BRIDGE_INTR_CTL_SIZE    0x10000

#define PCI_AGENT_IMMR_BA   0x4800000
#define PCI_AGENT_IMMR_SIZE 0x10000


#define PCI_CFG_ADR_REG  0x10900
#define PCI_CFG_DATA_REG 0x10904
#define PCI_AUTO_CONFIG_ADRS  0x4c00

#define PPCACR_PRKM_MASK 0XF0
#define PCI_REQUEST_LEVEL 0x3

#define CLASS_OFFSET      0xB
#define CLASS_WIDTH       0x1
#define BRIDGE_CLASS_TYPE 0x6


#define PCICMD_ADRS     (PCI_CFG_BASE + 0x04)  /* PCI cmd reg */
#define PCICMD_VAL      0x00000006             /* PCI COMMAND Default value */
#define PCISTAT_ADRS    (PCI_CFG_BASE + 0x06)  /* PCI status reg */

#define NUM_PCI_SLOTS		0x3          /* 3 PCI slots: 0 to 2 */

#define PCI_XINT1_LVL		0x0
#define PCI_XINT2_LVL		0x1
#define PCI_XINT3_LVL		0x2
#define PCI_XINT4_LVL		0x3

#define PCI_SLOT1_DEVNO		0x16         /* PCI SLOT 1 Device no */
#define PCI_LAT_TIMER		0x40         /* latency timer value, 64 PCI clocks */

#define PCI1_DEV_ID			0x826010E3
#define PCI2_DEV_ID			0x826110E3
#define PCI3_DEV_ID			0x826210E3
#define PCI_DEV_ID_8266		0x18C11057   /* Vendor & Dev Id for MPC8266ADS-PCI board */
#define PCI_ID_I82559		0x12298086   /* Id for Intel 82559 */
#define PCI_ID_I82559ER		0x12098086   /* Id for Intel 82559 ER */
#define PCI_ID_I82559_IB	0x10308086   /* Id for In-business card */

#define MPC8266ADS_PCI_IRQ       19

#define PCI_INTA_IRQ     MPC8266ADS_PCI_IRQ
#define PCI_INTB_IRQ     MPC8266ADS_PCI_IRQ
#define PCI_INTC_IRQ     MPC8266ADS_PCI_IRQ

#define PCI_ADDRESS_REGISTER    0x30000290     /* PCI Address Register */
#define PCI_DATA_REGISTER       0x30000294     /* PCI Data Register */

#define PCI_SIZE_MASK_4K   0xfffff
#define PCI_SIZE_MASK_8K   0xffffe
#define PCI_SIZE_MASK_16K  0xffffc
#define PCI_SIZE_MASK_32K  0xffff8
#define PCI_SIZE_MASK_64K  0xffff0
#define PCI_SIZE_MASK_128K 0xfffe0
#define PCI_SIZE_MASK_256K 0xfffc0
#define PCI_SIZE_MASK_512K 0xfff80
#define PCI_SIZE_MASK_1M   0xfff00
#define PCI_SIZE_MASK_2M   0xffe00
#define PCI_SIZE_MASK_4M   0xffc00
#define PCI_SIZE_MASK_8M   0xff800
#define PCI_SIZE_MASK_16M  0xff000
#define PCI_SIZE_MASK_32M  0xfe000
#define PCI_SIZE_MASK_64M  0xfc000
#define PCI_SIZE_MASK_128M 0xf8000
#define PCI_SIZE_MASK_256M 0xe0000
#define PCI_SIZE_MASK_512M 0xc0000
#define PCI_SIZE_MASK_1G   0x80000
#define PCI_SIZE_MASK_2G   0x00000


#define DELTA(a,b)		(sysAbs((int)a - (int)b))

#define BUS	0				/* bus-less board */

#undef  CPU
#define CPU	PPC603				/* CPU type */

#define N_SIO_CHANNELS	 	2		/* No. serial I/O channels */


/* define the decrementer input clock frequency */

#define DEC_CLOCK_FREQ	OSCILLATOR_FREQ

/*
 * Internal Memory Map base Address calculation
 *
 * If config.h has IMMAP_REMAP defined, then the internal mamory map
 * is defined to be 0x04700000, as defined in the ADS8266-PCI manual.
 *
 * Otherwise the memory map is defined by the Hard Reset Configuration Word
 * and must be determined by looking at bits 13:15 of the configuration word.
 * Bits 13:15 are bits 5:7 of the second byte of the config word: HRCW_BYTE_1
 */

#if   ((HRCW_BYTE_1 & 0x07) == 0)
#	define	RST_INTERNAL_MEM_MAP_ADDR	0x00000000
#elif ((HRCW_BYTE_1 & 0x07) == 1)
#	define	RST_INTERNAL_MEM_MAP_ADDR	0x00F00000
#elif ((HRCW_BYTE_1 & 0x07) == 2)
#	define	RST_INTERNAL_MEM_MAP_ADDR	0x0F000000
#elif ((HRCW_BYTE_1 & 0x07) == 3)
#	define	RST_INTERNAL_MEM_MAP_ADDR	0x0FF00000
#elif ((HRCW_BYTE_1 & 0x07) == 4)
#	define	RST_INTERNAL_MEM_MAP_ADDR	0xF0000000
#elif ((HRCW_BYTE_1 & 0x07) == 5)
#	define	RST_INTERNAL_MEM_MAP_ADDR	0xF0F00000
#elif ((HRCW_BYTE_1 & 0x07) == 6)
#	define	RST_INTERNAL_MEM_MAP_ADDR	0xFF000000
#elif ((HRCW_BYTE_1 & 0x07) == 7)
#	define	RST_INTERNAL_MEM_MAP_ADDR	0xFFF00000
#endif

#if defined (IMMAP_REMAP)
#	define INTERNAL_MEM_MAP_ADDR	0x04700000	/* defined by bsp */
#else
#	define INTERNAL_MEM_MAP_ADDR	RST_INTERNAL_MEM_MAP_ADDR
#endif

#define PQII_REG_BASE_OFF		0x10000
#define PQII_REG_BASE			(INTERNAL_MEM_MAP_ADDR + PQII_REG_BASE_OFF)
#define INTERNAL_MEM_MAP_SIZE	0x00020000	/* 128 K bytes */
#define IMMR_OFFSET				0x01a8		/* offset from SIU base */
#define IMMR_ISB_MASK			0xfffe0000	/* Internal Space Base mask */
#define IMMR_PARTNUM_MASK		0x0000ff00	/* Part Number mask */
#define IMMR_MASKNUM_MASK		0x000000ff	/* Mask Number mask */
#define DPRAM1_SIZE				0x00004000	/* 16K bytes of DPRAM in bank 1 */


/* Board Status and Control Registers - unique to ADS */

#define	BCSR_BASE_ADRS	0x04500000			/* BCSR base address */
#define BCSRS_SIZE      0x00008000  		/* 32K of address space */
#define BCSRS_MASK      ~(BCSRS_SIZE - 1)	/* set 32K mask for BCSRs */

#ifdef _ASMLANGUAGE
#	define BCSR0		BCSR_BASE_ADRS 			/* Register 0 */
#	define BCSR1		BCSR_BASE_ADRS + 0x04	/* Register 1 */
#	define BCSR2		BCSR_BASE_ADRS + 0x08	/* Register 2 */
#	define BCSR3		BCSR_BASE_ADRS + 0x0c	/* Register 3 */
#	define	BCSR2_BREVN_MASK	0x00000f00		/* Board revision number */
#else
#	define BCSR0		((uint32_t *) (BCSR_BASE_ADRS))			/* Register 0 */
#	define BCSR1		((uint32_t *) (BCSR_BASE_ADRS + 0x04))	/* Register 1 */
#	define BCSR2		((uint32_t *) (BCSR_BASE_ADRS + 0x08))	/* Register 2 */
#	define BCSR3		((uint32_t *) (BCSR_BASE_ADRS + 0x0c))	/* Register 3 */
#endif	/* _ASMLANGUAGE */


/* BCSR0 bit definition (active at low level when _L) */
/*
0 1 2 3 4 5 6 7 8-31
| | | | | | | |  `-- 0x00800000 through 0x00000001   reserved
| | | | | | | `----- 0x01000000 BCSR0_SIG_LED1_L     Signal LED 1 (red)
| | | | | | `------- 0x02000000 BCSR0_SIG_LED0_L     Signal LED 0 (green)
| | | | | `--------- 0x04000000 Reserved
| | | | `----------- 0x08000000 Reserved
| | | `------------- 0x10000000 Reserved
| | `--------------- 0x20000000 Reserved
| `----------------- 0x40000000 Reserved
`------------------- 0x80000000 Reserved

*/

#define BCSR0_SIG_LED1_L	0x01000000		/* LED 1 */
#define BCSR0_LED_RED		BCSR0_SIG_LED1_L
#define BCSR0_SIG_LED0_L	0x02000000		/* LED 0 */
#define BCSR0_LED_GREEN		BCSR0_SIG_LED0_L
#define BCSR0_LED_ON		0x0
#define BCSR0_LED_OFF		0x1


/* BCSR1 bit definition (active at low level when _L) */
/*
0 1 2 3 4 5 6 7 8-31
| | | | | | | |  `-- 0x00800000 through 0x00000001   reserved
| | | | | | | `----- 0x01000000 BCSR1_RS232EN_2_L  RS232 Port 2 Enable
| | | | | | `------- 0x02000000 BCSR1_RS232EN_1_L  RS232 Port 1 Enable
| | | | | `--------- 0x04000000 BCSR1_FETH_RST_L   Fast Ethernet Port Reset
| | | | `----------- 0x08000000 BCSR1_FETH_IEN_L   Fast Ethernet Port Initial
| | | |                                              Enable
| | | `------------- 0x10000000 BCSR1_ATM_RST_L    ATM Port Reset
| | `--------------- 0x20000000 BCSR1_ATM_EN_L     ATM Port Enable
| '----------------- 0x40000000 Flash Chip select CS0 = 0 CS4 = 1
-------------------- 0x80000000 Conf Word        BCSR = 0 Flash = 1
 */

#define BCSR1_RS232EN_2_L	0x01000000		/* RS232 port 2 enable */
#define BCSR1_RS232EN_1_L	0x02000000		/* RS232 port 1 enable */
#define BCSR1_FETH_RST_L	0x04000000		/* Fast Ethernet Port Reset */
#define BCSR1_FETH_IEN_L	0x08000000   	/* Fast Enet Port Initial Enable */
#define BCSR1_ATM_RST_L		0x10000000  	/* ATM Port Reset */
#define BCSR1_ATM_EN_L		0x20000000  	/* ATM Port Enable */
#define BCSR1_FLASH_CS0         0x40000000 
#define BCSR1_CONF_WORD         0x80000000

/* BCSR2 bit definition */

#define BCSR2_TOOL_STAT_MASK	0xff000000	/* Tool Status mask */
#define BCSR2_TOOL_REV_MASK	0x00f00000	/* Tool Revision mask */
#define BCSR2_EXT_TOOL_ID_MASK 	0x000f0000	/* External Tool ID */
#define BCSR2_SWOPT_0_MASK     	0x00008000	/* S/W opt bit 0 mask */
#define BCSR2_SWOPT_1_MASK     	0x00004000	/* S/W opt bit 1 mask */
#define BCSR2_L2CSIZE_MASK     	0x00003000	/* L2 Cache Size mask */
#define BCSR2_BREVN_MASK       	0x00000f00	/* Board rev number */
#define BCSR2_SWOPT_2_MASK     	0x00000080	/* S/W opt bit 2 mask */
#define BCSR2_FLASH_PD_75_MASK 	0x00000070	/* Flash Presence Detect[7:5] mask */
#define BCSR2_FLASH_PD_41_MASK 	0x0000007f	/* Flash Presence Detect[4:1] mask */


/* BCSR3 bit definition */

#define BCSR3_USB_EN          0x80000000
#define BCSR3_USB_HI_SPEED    0x40000000
#define BCSR3_USB_VCCO        0x20000000
#define BCSR3_FETHIEN2        0x10000000
#define BCSR3_FETH2_RST       0x08000000
#define BCSR3_ATM16           0x04000000
#define BCSR3_ATM_SINGLE_PHY  0x02000000
#define BCSR3_PCI_MODE        0x01000000



/* BCSR4 bit definition */

#define BCSR4_PCI0_PRESENT   0xc0000000
#define BCSR4_PCI1_PRESENT   0x30000000
#define BCSR4_PCI2_PRESENT   0x0c000000
#define BCSR4_M66EN          0x02000000
#define BCSR4_PCI_MODCK      0x01000000

#define BCSR1_RESET_VAL	(BCSR1_RS232EN_2_L | BCSR1_RS232EN_1_L | \
						 BCSR1_FETH_RST_L  | BCSR1_FETH_IEN_L  | \
						 BCSR1_ATM_RST_L   | BCSR1_ATM_EN_L)

/* Security IMMR registers . Initialised in romInit.s */
#define M82XX_SECBR(base) (CAST(VUINT32 *)((base) + 0x101B4)) 
#define M82XX_SECMR(base) (CAST(VUINT32 *)((base) + 0x101BC))

#define SEC_ENG_BASE_ADRS (INTERNAL_MEM_MAP_ADDR + 0x40000)
#define SEC_ENG_SIZE      0x20000
#define SEC_ENG_SIZE_MASK 0xfffe0000

/* CPU type in the PVR */

#define CPU_TYPE_8260			0xAAAA		/* value for PPC8260 */
#define CPU_TYPE_8266			0xBBBB		/* value for PPC8266 */
#define	CPU_REV_A1_MASK_NUM		0x0010		/* revision mask num */
#define HIP4_ID			       	0x80810000  /* device ID via PVR */
#define HIP4_MASK	       		0xFFFF0000  /* mask upper word   */
#define HIP7_ID				0x80820000

/*
 * Maximum SCC Number used.
*/
#define MAX_SCC_SIO_CHANS 4

/*
 * CPM Parameter RAM definitions.  From Table 13-10, page 13-18 in
 *  MPC8260 PowerQUICC-II User's Manual
 */
#define SCC1_PRAM	(INTERNAL_MEM_MAP_ADDR + 0x8000)
#define SCC2_PRAM	(INTERNAL_MEM_MAP_ADDR + 0x8100)
#define SCC3_PRAM	(INTERNAL_MEM_MAP_ADDR + 0x8200)
#define SCC4_PRAM	(INTERNAL_MEM_MAP_ADDR + 0x8300)
#define FCC1_PRAM	(INTERNAL_MEM_MAP_ADDR + 0x8400)
#define FCC2_PRAM	(INTERNAL_MEM_MAP_ADDR + 0x8500)
#define FCC3_PRAM	(INTERNAL_MEM_MAP_ADDR + 0x8600)
#define MCC1_PRAM	(INTERNAL_MEM_MAP_ADDR + 0x8700)
#define SMC1_BASE	(INTERNAL_MEM_MAP_ADDR + 0x87FC)
#define IDMA1_BASE	(INTERNAL_MEM_MAP_ADDR + 0x87FE)
#define MCC2_PRAM	(INTERNAL_MEM_MAP_ADDR + 0x8800)
#define SMC2_BASE	(INTERNAL_MEM_MAP_ADDR + 0x88FC)
#define IDMA2_BASE	(INTERNAL_MEM_MAP_ADDR + 0x88FE)
#define SPI_BASE	(INTERNAL_MEM_MAP_ADDR + 0x89FC)
#define IDMA3_BASE	(INTERNAL_MEM_MAP_ADDR + 0x89FE)
#define RISC_TMR	(INTERNAL_MEM_MAP_ADDR + 0x8AE0)
#define REV_NUM		(INTERNAL_MEM_MAP_ADDR + 0x8AF0)
#define RAND		(INTERNAL_MEM_MAP_ADDR + 0x8AF8)
#define I2C_BASE	(INTERNAL_MEM_MAP_ADDR + 0x8AFC)
#define IDMA4_BASE	(INTERNAL_MEM_MAP_ADDR + 0x8AFE)

/*
* Dual Ported RAM definitions
*
* This is the area where Buffer Descriptor tables are setup for all the
* CPM controlled devices.  This bsp defines areas for the SCC's, IDMA's
* and the I2C controllers.  The FCC DPRAM areas are defined in the FCC
* driver code, and start at INTERNAL_MEM_MAP_ADDR + 0x8400
*/
#define I2C_PARAM_OFF	0x200	/* offset from IMM */

/* Add in IDMA registers for PCI bridge register read errata fix*/

#define IDSR1   		(PQII_REG_BASE | 0x1020)
#define IDMR1   		(PQII_REG_BASE | 0x1024)
#define IDSR2   		(PQII_REG_BASE | 0x1028)
#define IDMR2   		(PQII_REG_BASE | 0x102c)
#define IDSR3   		(PQII_REG_BASE | 0x1030)
#define IDMR3   		(PQII_REG_BASE | 0x1034)
#define IDSR4   		(PQII_REG_BASE | 0x1038)
#define IDMR5   		(PQII_REG_BASE | 0x103c)

/* I2C register and buffer definitions */

#define I2C_MAX_RXDATA		32		/* limit receives to 32 bytes */
#define I2C_MAX_TXDATA		32		/* limit transmits to 32 bytes */
#define I2C_MAX_CLK			70000	/* limit max data rate to 70Kbps */
#define I2C_FUNC_CODE		0x10	/* no snooping, big-endian, 60x bus */
#define I2C_RDYLST_TXBD		0x8C00  /* full, last data, start, interrupt */
#define I2C_RDY_TXBD		0x8400  /* full, start, interrupt */
#define I2C_INIT_RXBD		0xA000  /* empty, Last BD */
#define I2C_BD_CPMOWN		0x8000  /* cpm owns this BD */
#define I2C_MASTER_ADDR		0x55	/* some default address */
#define I2C_DRAM_ADDR		(0x50 | I2C_SW2_SETTING)	  /* SDRAM DIMM 7 bit addr */
#define I2C_READ			1
#define I2C_WRITE			0
#define I2C_BD_WRAP			0x2000	/* buffer desc table wrap */
#define I2C_GEN_INT			0x1000  /* set interrupt bits in event register */

#define	I2C_MOD	(uint8_t *)(INTERNAL_MEM_MAP_ADDR + 0x11860)	/* mode register */
#	define	REVD	0x20	/* Reverse Data */
#	define	GCD		0x10	/* General Call Disable */
#	define	FLT		0x08	/* Clock Filter */
#	define	PDIV32	0x00	/* predivide by 32 */
#	define	PDIV16	0x02	/* predivide by 16 */
#	define	PDIV8	0x04	/* predivide by  8 */
#	define	PDIV4	0x06	/* predivide by  4 */
#	define	I2C_EN 	0x01	/* enable I2C */
#define	I2C_ADD	(uint8_t *)(INTERNAL_MEM_MAP_ADDR + 0x11864)	/* address reg */
#define	I2C_BRG	(uint8_t *)(INTERNAL_MEM_MAP_ADDR + 0x11868)	/* baud rate reg */
#	define	DIV		6		/* always use this divisor */
#define	I2C_COM	(uint8_t *)(INTERNAL_MEM_MAP_ADDR + 0x1186C)	/* command reg */
#	define I2C_MASTER 	0x01
#	define I2C_TRANSMIT 0x80
#define	I2C_ER	(uint8_t *)(INTERNAL_MEM_MAP_ADDR + 0x11870)	/* event reg */
#define	I2C_MR	(uint8_t *)(INTERNAL_MEM_MAP_ADDR + 0x11874)	/* event mask reg */
#	define I2C_TXE	0x10				/* TX Error, set when an error */
#	define I2C_BSY	0x04				/* Busy, set after rx overrun */
#	define I2C_TXB	0x02				/* TX buffer empty */
#	define I2C_RXB	0x01				/* RX buffer available */
#	define I2C_ERR  (I2C_TXE + I2C_BSY)	/* any I2C error */


/* The parallel port needs some setup for I2C to work
* in particular Bits 14 and 15 of the parallel port D registers need to
* be setup.
*/
#define BIT14		0x0002	/* for word access to PPxx ports */
#define BIT15		0x0001	/* for word access to PPxx ports */

#define PODRD		(INTERNAL_MEM_MAP_ADDR + 0x10D6C)
#define PDIRD		(INTERNAL_MEM_MAP_ADDR + 0x10D60)
#define PPARD		(INTERNAL_MEM_MAP_ADDR + 0x10D64)
#define PSORD		(INTERNAL_MEM_MAP_ADDR + 0x10D68)


#define DCM_DUAL_ADDR 		0x0000
#define DCM_SINGLE_ADDR 	0x1000
#define DCM_TRANS_BUF_64 	0x0000
#define DCM_TRANS_BUF_128 	0x0040
#define DCM_TRANS_BUF_256	0x0080
#define DCM_TRANS_BUF_512	0x00c0
#define DCM_TRANS_BUF_1024	0x0100
#define DCM_TRANS_BUF_2048	0x0180
#define DCM_EXT_REQ			0x0008
#define DCM_MEM_MEM			0x0000
#define DCM_PER_MEM			0x0002
#define DCM_MEM_PER			0x0001
#define DCM_TC2				0x0400
#define DCM_DONE			0x0004
#define DCM_SINC			0x0020
#define DCM_DINC			0x0010
#define IDMABD_VALID 		0x80000000	/*  Bit 0 Valid */
#define IDMABD_WRAP			0x20000000	/* Bit 2 Wrap */
#define IDMABD_INT_ENABLE	0x10000000 /*  Bit 3 Interrupt */
#define IDMABD_LAST			0x08000000 /*  Bit 4 Last */
#define IDMABD_AUTO_BUF				0x02000000  /* Bit 6 Auto Buffer Mode */
#define IDMABD_SRC_DONE				0x00400000	/* Bit 9 Source Done */
#define IDMABD_DST_DONE				0x00200000  /* Bit 10 Destination Done */
#define IDMABD_DST_DGBL				0x00100000	/* Bit 11 Destination Global (Snooping Activated) */
#define IDMABD_DBO_BIG_ENDIAN		0x00080000 /*  Bit 12-13 Dest. Byte Ordering */
#define IDMABD_DBO_LITTLE_ENDIAN	0x00060000  /* Bit 12-13 Dest. Byte Ordering */
#define IDMABD_DST_LOCAL			0x00010000  /* Bit 15 Dest data Bus is Local */
#define IDMABD_SRC_SGBL				0x00002000	/* Bit 2 Source Global (Snooping Activated) */
#define IDMABD_SBO_BIG_ENDIAN		0x00001000	/* Bit 3-4 Src Byte Ordering  */
#define IDMABD_SBO_LITTLE_ENDIAN	0x00000800  /* Bit 3-4 Src Byte Ordering */
#define IDMABD_SRC_LOCAL			0x00000200  /* bit 6 Source data bus is local */

#define IDMADEST				(INTERNAL_MEM_MAP_ADDR + 0xB000)
#define NO_OF_BDS 				1   	/* The number of desired buffer descriptors */
#define IDMASIZE_BYTES			4   	/* Number of bytes in source buffer */
#define IDMA2_PAGE_SUBBLOCK		0x22A00000     /* page 8, code = 21 */
#define CPCR_REG 				(INTERNAL_MEM_MAP_ADDR + 0x119c0)
#define CPCR_START_IDMA			0x00000009     /* Opcode 9 */
#define CPCR_STOP_IDMA			0x0000000B     /* Opcode 11 */
#define CPCR_FLG				0x00010000
#define DST_ADDR_LOCAL			0x04000000
#define DPR_BASE				INTERNAL_MEM_MAP_ADDR
#define I2C_CPM_INIT			0x29610000		/* init rx and tx params */

#define MEMC			(INTERNAL_MEM_MAP_ADDR + 0x10100)	/* mem controller */
#define SCCR			(INTERNAL_MEM_MAP_ADDR + 0x10C80)	/* address of SCMR */
#define SCMR			(INTERNAL_MEM_MAP_ADDR + 0x10C88)	/* address of SCMR */
#define SCMR_PLLDF(val)	((val & 0x00000000) >> 12)			/* get PLLDF */
#define SCMR_PLLMF(val)	 (val & 0x000000FF)					/* get PLLMF */
#define SCMR_CPMDF(val)	((val & 0x000F0000) >> 16)			/* get CPMDF */
#define SCMR_BUSDF(val)	((val & 0x00F00000) >> 20)			/* get BusDF */
#define SCMR_DFBRG_MASK	0x00000003				/* mask low 2 bits */

#define NORMAL_REFRESH_PS 15625000       /* normal refresh period in ps */

#ifndef _ASMLANGUAGE

/*
* global variable declaration
*/


	typedef struct idmaBufferDescriptor
		{
		volatile uint32_t cstatus;	/* BD control and status   */
		uint32_t 		  length;	/* BD Data length transfer   */
		uint32_t 		  srcptr;	/* BD Source Data Buffer Pointer   */
		uint32_t 		  dstptr;	/* BD Destination Data Buffer Pointer   */
		}BD_DMA;

	struct i2cBufferDescriptor
		{
		volatile uint16_t cstatus;	/* BD control and status   */
		uint16_t length;	/* BD Data length transfer   */
		uint8_t  *dataptr;	/* BD I2C Data Buffer Pointer   */
		};

	typedef struct i2cBufferDescriptor I2C_TXBD;	/* I2C transmit buffer descriptor */
	typedef struct i2cBufferDescriptor I2C_RXBD;	/* I2C receive buffer descriptor */


	typedef  struct
		{
		uint16_t ibase;
		uint16_t dcm;
		uint16_t ibdptr;
		uint16_t dpr_buf;
		uint16_t buf_inv;
		uint16_t ss_max;
		uint16_t dpr_in_ptr;
		uint16_t sts;
		uint16_t dpr_out_ptr;
		uint16_t seob;
		uint16_t deob;
		uint16_t dts;
		uint16_t ret_add;
		uint16_t rsvd1;
		uint32_t bd_cnt;
		uint32_t s_ptr;
		uint32_t d_ptr;
		uint32_t istate;
		}IDMA_PARAM ;

	typedef  struct
		{
		uint16_t rbase;
		uint16_t tbase;
		uint8_t  rfcr;
		uint8_t  tfcr;
		uint16_t mlblr;
		uint32_t rstate;
		uint32_t rptr;
		uint16_t rbptr;
		uint16_t rcount;
		uint32_t rtemp;
		uint32_t tstate;
		uint32_t tptr;
		uint16_t tbptr;
		uint16_t tcount;
		uint32_t ttemp;
		uint32_t fill[10];
		}I2C_PARAM ;

/*
* The following defines the complete I2C data structure.
* It consists of the Parameter Ram, 1 Tx buffer descriptor
* 1 Rx buffer descriptor, the rx and tx data buffers.
*
* This structure must be located on a 64 byte boundary in
* Dual ported RAM of the 826x.  In this case as defined by
*   I2C_PARAMRAM above
*
* This I2C support has fixed buffer descriptors.  It allows
* a single transmit operation of a single byte multiple
* bytes up to I2C_MAX_TXDATA.
*
* For a transmit only operation a single TX BD will be used
* For a receive operation, 2 TX BD's are used.
*/
	typedef	struct i2cMapping
		{
		I2C_PARAM	i2cParams;
		I2C_RXBD	i2cRxBd;
		I2C_TXBD	i2cTxBd1;
		I2C_TXBD	i2cTxBd2;
		uint8_t		i2cRxData[I2C_MAX_RXDATA];
		uint8_t		i2cTxData1[I2C_MAX_TXDATA];
		uint8_t		i2cTxData2[I2C_MAX_TXDATA];
		} I2C_CTRL;

/* Memory controller registers.
*/
typedef struct  mem_ctlr {
        uint32_t    memc_br0;
        uint32_t    memc_or0;
        uint32_t    memc_br1;
        uint32_t    memc_or1;
        uint32_t    memc_br2;
        uint32_t    memc_or2;
        uint32_t    memc_br3;
        uint32_t    memc_or3;
        uint32_t    memc_br4;
        uint32_t    memc_or4;
        uint32_t    memc_br5;
        uint32_t    memc_or5;
        uint32_t    memc_br6;
        uint32_t    memc_or6;
        uint32_t    memc_br7;
        uint32_t    memc_or7;
        uint32_t    memc_br8;
        uint32_t    memc_or8;
        uint32_t    memc_br9;
        uint32_t    memc_or9;
        uint32_t    memc_br10;
        uint32_t    memc_or10;
        uint32_t    memc_br11;
        uint32_t    memc_or11;
        char        res1[8];
        uint32_t    memc_mar;
        char        res2[4];
        uint32_t    memc_mamr;
        uint32_t    memc_mbmr;
        uint32_t    memc_mcmr;
        char        res3[8];		/* manual wrong, says 48 bits */
        uint16_t    memc_mptpr;
        char        res4[2];
        uint32_t    memc_mdr;
        char        res5[4];
        uint32_t    memc_psdmr;
        uint32_t    memc_lsdmr;
        uint8_t     memc_purt;
        char        res6[3];
        uint8_t     memc_psrt;
        char        res7[3];
        uint8_t     memc_lurt;
        char        res8[3];
        uint8_t     memc_lsrt;
        char        res9[3];
        uint32_t    memc_immr;
        char        res10[84];
} memctl8260_t;

/* Port A, B, C and D Defines */

#define PA31    (0x00000001)
#define PA30    (0x00000002)
#define PA29    (0x00000004)
#define PA28    (0x00000008)
#define PA27    (0x00000010)
#define PA26    (0x00000020)
#define PA25    (0x00000040)
#define PA24    (0x00000080)
#define PA23    (0x00000100)
#define PA22    (0x00000200)
#define PA21    (0x00000400)
#define PA20    (0x00000800)
#define PA19    (0x00001000)
#define PA18    (0x00002000)
#define PA17    (0x00004000)
#define PA16    (0x00008000)
#define PA15    (0x00010000)
#define PA14    (0x00020000)
#define PA13    (0x00040000)
#define PA12    (0x00080000)
#define PA11    (0x00100000)
#define PA10    (0x00200000)
#define PA9     (0x00400000)
#define PA8     (0x00800000)
#define PA7     (0x01000000)
#define PA6     (0x02000000)
#define PA5     (0x04000000)
#define PA4     (0x08000000)
#define PA3     (0x10000000)
#define PA2     (0x20000000)
#define PA1     (0x40000000)
#define PA0     (0x80000000)

#define PB31    (0x00000001)
#define PB30    (0x00000002)
#define PB29    (0x00000004)
#define PB28    (0x00000008)
#define PB27    (0x00000010)
#define PB26    (0x00000020)
#define PB25    (0x00000040)
#define PB24    (0x00000080)
#define PB23    (0x00000100)
#define PB22    (0x00000200)
#define PB21    (0x00000400)
#define PB20    (0x00000800)
#define PB19    (0x00001000)
#define PB18    (0x00002000)
#define PB17    (0x00004000)
#define PB16    (0x00008000)
#define PB15    (0x00010000)
#define PB14    (0x00020000)
#define PB13    (0x00040000)
#define PB12    (0x00080000)
#define PB11    (0x00100000)
#define PB10    (0x00200000)
#define PB9     (0x00400000)
#define PB8     (0x00800000)
#define PB7     (0x01000000)
#define PB6     (0x02000000)
#define PB5     (0x04000000)
#define PB4     (0x08000000)

#define PC31    (0x00000001)
#define PC30    (0x00000002)
#define PC29    (0x00000004)
#define PC28    (0x00000008)
#define PC27    (0x00000010)
#define PC26    (0x00000020)
#define PC25    (0x00000040)
#define PC24    (0x00000080)
#define PC23    (0x00000100)
#define PC22    (0x00000200)
#define PC21    (0x00000400)
#define PC20    (0x00000800)
#define PC19    (0x00001000)
#define PC18    (0x00002000)
#define PC17    (0x00004000)
#define PC16    (0x00008000)
#define PC15    (0x00010000)
#define PC14    (0x00020000)
#define PC13    (0x00040000)
#define PC12    (0x00080000)
#define PC11    (0x00100000)
#define PC10    (0x00200000)
#define PC9     (0x00400000)
#define PC8     (0x00800000)
#define PC7     (0x01000000)
#define PC6     (0x02000000)
#define PC5     (0x04000000)
#define PC4     (0x08000000)
#define PC3     (0x10000000)
#define PC2     (0x20000000)
#define PC1     (0x40000000)
#define PC0     (0x80000000)

#define PD31    (0x00000001)
#define PD30    (0x00000002)
#define PD29    (0x00000004)
#define PD28    (0x00000008)
#define PD27    (0x00000010)
#define PD26    (0x00000020)
#define PD25    (0x00000040)
#define PD24    (0x00000080)
#define PD23    (0x00000100)
#define PD22    (0x00000200)
#define PD21    (0x00000400)
#define PD20    (0x00000800)
#define PD19    (0x00001000)
#define PD18    (0x00002000)
#define PD17    (0x00004000)
#define PD16    (0x00008000)
#define PD15    (0x00010000)
#define PD14    (0x00020000)
#define PD13    (0x00040000)
#define PD12    (0x00080000)
#define PD11    (0x00100000)
#define PD10    (0x00200000)
#define PD9     (0x00400000)
#define PD8     (0x00800000)
#define PD7     (0x01000000)
#define PD6     (0x02000000)
#define PD5     (0x04000000)
#define PD4     (0x08000000)

/* function declarations */
extern uint32_t sysBaudClkFreq(void);
extern void   sysMsDelay(uint32_t);
extern void   sysDelay(void);
extern uint32_t sysAbs(int);
extern uint32_t sysDecGet(void);

#endif /* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* INCads827xh */
