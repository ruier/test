/* wrSbc85xx.h - Wind River SBC 85xx board header */

/* Copyright 2002-2004,2006-2008 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01y,23may08,syt  WIND00100828: modify AUX clock rate marcos to pass the BSTVTS.
01x,05mar07,y_w  WIND00086517: modify AUX clock rate macros to pass the BSPVTS.
01w,06feb07,d_z  Delete PCI_DEV_ID_85XX macro, add PCI_DEV_ID_8560 and
                 PCI_DEV_ID_8540 macros.
01v,17jan07,rec  reduce max aux clock rate
01u,30nov06,jmt  Update the AUX Clock macros to the correct Min, Max and
                 default values
01t,24oct06,jmt  Add define for default Aux clock rate
01s,30jul06,tor  Merge PCI support, cleanup
01r,09jun04,mil  Added VIRT_ADDR/PHYS_ADDR types conditionally.
01q,05may04,mil  Added LBCDLLSR and DDR defines and PCI ID for rev2 chips.
01p,02feb04,mil  Fixed redefine error for GNU Windows build.
01o,23oct03,mil  Cleanup.
01n,23oct03,mil  Added defines for 8560 COM2.
01m,12sep03,mil  Added defines for 8540 internal DUART.
01l,07jul03,mil  Added offsets for SCC regs.
01k,08jul02,dtr  Adding some PCI defines and macros.
*/

/* This file contains I/O addresses and related constants for the
 * Wind River SBC 85XX board. 
 */

#ifndef	INCwrSbc85xxh
#define	INCwrSbc85xxh

#ifdef	__cplusplus
    extern "C" {
#endif	/* __cplusplus */

#ifndef  _ASMLANGUAGE
# if (defined(_WRS_VXWORKS_MAJOR) && (_WRS_VXWORKS_MAJOR >= 6))
/* nothing needed */
# else  /* _WRS_VXWORKS_MAJOR */
typedef void * VIRT_ADDR;
typedef void * PHYS_ADDR;
# endif  /* _WRS_VXWORKS_MAJOR */
#endif  /* _ASMLANGUAGE */


#ifndef M8260ABBREVIATIONS
#define M8260ABBREVIATIONS

#ifdef  _ASMLANGUAGE
#   define	CAST(x)
#else	/* _ASMLANGUAGE */
#   define	CAST(x)	(x)
typedef	volatile UCHAR	VCHAR;
typedef	volatile INT32	VINT32;
typedef	volatile INT16	VINT16;
typedef	volatile INT8	VINT8;
typedef	volatile UINT32	VUINT32;
typedef	volatile UINT16	VUINT16;
typedef	volatile UINT8	VUINT8;
#endif	/* _ASMLANGUAGE */

#endif /* M8260ABBREVIATIONS */

#ifndef EIEIO_SYNC
#   define EIEIO_SYNC  _WRS_ASM (" eieio; sync")
#endif        /* EIEIO_SYNC */
#ifndef EIEIO
#   define EIEIO    _WRS_ASM (" eieio")
#endif        /* EIEIO */

#define INIT_SKIP_PID2	1


/* Base address of CCSRBAR */

#define	CCSBAR		0xFE000000	/* CCSRBAR runtime value */
#define	CCSBAR_RST	0xFF700000	/* CCSRBAR reset value */
#define	IVPR_VAL	0x0		/* vector location */


/* Local access window offsets */

#define	M85XX_LAWBAR0(base)	(CAST(VUINT32 *)((base) + 0xc08))
#define	M85XX_LAWAR0(base)	(CAST(VUINT32 *)((base) + 0xc10))    
#define	M85XX_LAWBAR1(base)	(CAST(VUINT32 *)((base) + 0xc28))
#define	M85XX_LAWAR1(base)	(CAST(VUINT32 *)((base) + 0xc30))
#define	M85XX_LAWBAR2(base)	(CAST(VUINT32 *)((base) + 0xc48))
#define	M85XX_LAWAR2(base)	(CAST(VUINT32 *)((base) + 0xc50))
#define	M85XX_LAWBAR3(base)	(CAST(VUINT32 *)((base) + 0xc68))
#define	M85XX_LAWAR3(base)	(CAST(VUINT32 *)((base) + 0xc70))
#define	M85XX_LAWBAR4(base)	(CAST(VUINT32 *)((base) + 0xc88))
#define	M85XX_LAWAR4(base)	(CAST(VUINT32 *)((base) + 0xc90))
#define	M85XX_LAWBAR5(base)	(CAST(VUINT32 *)((base) + 0xcA8))
#define	M85XX_LAWAR5(base)	(CAST(VUINT32 *)((base) + 0xcB0))
#define	M85XX_LAWBAR6(base)	(CAST(VUINT32 *)((base) + 0xcc8))
#define	M85XX_LAWAR6(base)	(CAST(VUINT32 *)((base) + 0xcd0))
#define	M85XX_LAWBAR7(base)	(CAST(VUINT32 *)((base) + 0xce8))
#define	M85XX_LAWAR7(base)	(CAST(VUINT32 *)((base) + 0xcf0))

#define	LAWBAR_ADRS_SHIFT	12
#define	LAWAR_ENABLE		0x80000000
#define	LAWAR_TGTIF_PCI		0x00000000
#define	LAWAR_TGTIF_LBC		0x00400000
#define	LAWAR_TGTIF_RAPIDIO	0x00c00000
#define	LAWAR_TGTIF_DDRSDRAM	0x00F00000

#define	LAWAR_SIZE_4KB		0x0000000B
#define	LAWAR_SIZE_8KB		0x0000000C
#define	LAWAR_SIZE_16KB		0x0000000D
#define	LAWAR_SIZE_32KB		0x0000000E
#define	LAWAR_SIZE_64KB		0x0000000F
#define	LAWAR_SIZE_128KB	0x00000010
#define	LAWAR_SIZE_256KB	0x00000011
#define	LAWAR_SIZE_512KB	0x00000012
#define	LAWAR_SIZE_1MB		0x00000013
#define	LAWAR_SIZE_2MB		0x00000014
#define	LAWAR_SIZE_4MB		0x00000015
#define	LAWAR_SIZE_8MB		0x00000016
#define	LAWAR_SIZE_16MB		0x00000017
#define	LAWAR_SIZE_32MB		0x00000018
#define	LAWAR_SIZE_64MB		0x00000019
#define	LAWAR_SIZE_128MB	0x0000001A
#define	LAWAR_SIZE_256MB	0x0000001B
#define	LAWAR_SIZE_512MB	0x0000001C
#define	LAWAR_SIZE_1GB		0x0000001D
#define	LAWAR_SIZE_2GB		0x0000001E


/* Local Bus Controller (LBC) Registers
 *
 * BRx 0-16 Base Address
 *     17-18 Extended Base Address 
 *     19-20 Port Size - 00 reserved
 *                     - 01 8bit
 *                     - 10 16bit
 *                     - 11 32bit
 *     21-22 Data Error Correction
 *                     - 00 reserved
 *                     - 01 Normal parity 
 *                     - 10 RMW parity generation (32-bit)
 *                     - 11 reserved
 *     23    Write Protect
 *     24-26 Machine Select = 000 GPCM
 *                          - 001->010 reserved
 *                          - 011 SDRAM
 *                          - 100->110 UPMA->UPMC
 *                          - 111 reserved
 *     28-29 Atomic Access  - 00 No atomic access
 *                          - 01 Read-after-write
 *                          - 10 Write-after-read
 *                          - 11 reserved
 *     31    Valid     
 *      
 * ORx for SDRAM
 *     0-16  Address mask 
 *     17-18 Extended address mask
 *     19-21 Column address lines - 000->111 7->14
 *     23-25 Number of row address lines - 000->110 9->15
 *                                       - 111 Reserved
 *     26    Page mode select
 *     31    External address latch delay 
 *
 * ORx for GPCM Mode
 *     0-16  Address mask 
 *     17-18 Extended address mask
 *     19    Buffer Control Disable
 *     20    Chip select negation
 *     21-22 Addres to chip select setup
 *     23    Extra Address to chip select setup
 *     24-27 Cycle length in Bus clocks - 0000->1111 0->15 wait states
 *     28    External address termination
 *     29    Timing relaxed
 *     30    Extended hold time for read access
 *     31    External address latch delay  
 */

#define	M85XX_BR0(base)		(CAST(VUINT32 *)((base) + 0x5000))
#define	M85XX_OR0(base)		(CAST(VUINT32 *)((base) + 0x5004))
#define	M85XX_BR1(base)		(CAST(VUINT32 *)((base) + 0x5008))
#define	M85XX_OR1(base)		(CAST(VUINT32 *)((base) + 0x500c))
#define	M85XX_BR2(base)		(CAST(VUINT32 *)((base) + 0x5010))
#define	M85XX_OR2(base)		(CAST(VUINT32 *)((base) + 0x5014))
#define	M85XX_BR3(base)		(CAST(VUINT32 *)((base) + 0x5018))
#define	M85XX_OR3(base)		(CAST(VUINT32 *)((base) + 0x501c))
#define	M85XX_BR4(base)		(CAST(VUINT32 *)((base) + 0x5020))
#define	M85XX_OR4(base)		(CAST(VUINT32 *)((base) + 0x5024))
#define	M85XX_BR5(base)		(CAST(VUINT32 *)((base) + 0x5028))
#define	M85XX_OR5(base)		(CAST(VUINT32 *)((base) + 0x502C))
#define	M85XX_BR6(base)		(CAST(VUINT32 *)((base) + 0x5030))
#define	M85XX_OR6(base)		(CAST(VUINT32 *)((base) + 0x5034))
#define	M85XX_BR7(base)		(CAST(VUINT32 *)((base) + 0x5038))
#define	M85XX_OR7(base)		(CAST(VUINT32 *)((base) + 0x503C))

#define	M85XX_OR_PS_32          
#define	M85XX_MAR(base)		(CAST(VUINT32 *)((base) + 0x5068))
#define	M85XX_MAMR(base)	(CAST(VUINT32 *)((base) + 0x5070))
#define	M85XX_MBMR(base)	(CAST(VUINT32 *)((base) + 0x5074))
#define	M85XX_MCMR(base)	(CAST(VUINT32 *)((base) + 0x5078))
#define	M85XX_MRTPR(base)	(CAST(VUINT32 *)((base) + 0x5084))
#define	MRTPR_PTP_MASK		0xff000000
#define	MRTPR_PTP_WRITE(x)	((x << 24) & MRTPR_PTP_MASK)
#define	M85XX_MDR(base)		(CAST(VUINT32 *)((base) + 0x5088))

#define	M85XX_LSDMR(base)	(CAST(VUINT32 *)((base) + 0x5094))
#define	LSDMR_RFEN		0x40000000	/* Refresh Enable */

/* LSDMR OP - 000 Normal operation
 *          - 001 Auto Refresh  (Initialization)
 *          - 010 Self Refresh
 *          - 011 Mode Register Write (Initialization)
 *          - 100 Precharge Bank
 *          - 101 Precharge all banks (Initialization)
 *          - 110 Activate Bank
 *          - 111 Read/Write without valid transfer
 */      

#define  LSDMR_OP_MASK  0x38000000
#define  LSDMR_OP_SHIFT(x) ((x << 27) & LSDMR_OP_MASK)

/* Bank Select Multiplexed address line - 000 lines 12:13
 *                                      - 001       13:14
 *                                      - 010       14:15
 *                                      - 011       15:16
 *                                      - 100       16:17
 *                                      - 101       17:18
 *                                      - 110       18:19
 *                                      - 111       19:20
 */

#define  LSDMR_BSMA_MASK 0x00E00000
#define  LSDMR_BSMA_SHIFT(x) ((x << 23) & LSDMR_BSMA_MASK)

/* RFCR Refresh recovery 000 - reserved
 *                       001->110 - 3->8 clocks
 *                       111 - 16 clocks
 */

#define  LSDMR_RFCR_MASK 0x00038000
#define  LSDMR_RFCR_SHIFT(x) ((x << 15) & LSDMR_RFCR_MASK)
/* other LSDMR definitions not used */

#define	M85XX_LURT(base)	(CAST(VUINT32 *)((base) + 0x50A0))
#define	M85XX_LSRT(base)	(CAST(VUINT32 *)((base) + 0x50A4))
#define	M85XX_LTESR(base)	(CAST(VUINT32 *)((base) + 0x50B0))
#define	M85XX_LTEDR(base)	(CAST(VUINT32 *)((base) + 0x50B4))
#define	M85XX_LTEIR(base)	(CAST(VUINT32 *)((base) + 0x50B8))
#define	M85XX_LTEATR(base)	(CAST(VUINT32 *)((base) + 0x50BC))
#define	M85XX_LTEAR(base)	(CAST(VUINT32 *)((base) + 0x50C0))

/* LBC clock configuration */

#define	M85XX_LBCR(base)	(CAST(VUINT32 *)((base) + 0x50D0))
#define	M85XX_LCRR(base)	(CAST(VUINT32 *)((base) + 0x50D4))

/* ECM registers */

#define	ECM_OFFSET		0x1000
#define	ECMBA			(CCSBAR | ECM_OFFSET)

/* Offsets for global utils registers */
#define M85XX_DDRDLLCR(base)	(CAST(VUINT32 *)((base) + 0xE0E10))
#define M85XX_LBCDLLCR(base)    (CAST(VUINT32 *)((base) + 0xE0E20))
#define M85XX_DEVDISR(base)     (CAST(VUINT32 *)((base) + 0xE0070))

#define	DDR_OFFSET		0x2000
#define	DDRBA			(CCSBAR | DDR_OFFSET)

#define	CS0_BNDS		0x000
#define	CS1_BNDS		0x008
#define	CS2_BNDS		0x010
#define	CS3_BNDS		0x018
#define	CS0_CONFIG		0x080
#define	CS1_CONFIG		0x084
#define	CS2_CONFIG		0x088
#define	CS3_CONFIG		0x08C
#define	TIMING_CFG_1		0x108
#define	TIMING_CFG_2		0x10C
#define	DDR_SDRAM_CFG		0x110
#define	DDR_SDRAM_MODE_CFG	0x118
#define	DDR_SDRAM_INTERVAL	0x124
#define DDR_DATA_ERR_INJECT_HI  0xe00
#define DDR_DATA_ERR_INJECT_LO  0xe04
#define DDR_ECC_ERR_INJECT      0xe08
#define DDR_CAPTURE_DATA_HI     0xe20
#define DDR_CAPTURE_DATA_LO     0xe24
#define DDR_CAPTURE_ECC         0xe28
#define DDR_ERR_DETECT          0xe40
#define DDR_ERR_DISABLE         0xe44
#define DDR_ERR_INT_EN          0xe48
#define DDR_CAPTURE_ATTRIBUTES  0xe4c
#define DDR_CAPTURE_ADDRESS     0xe50
#define	DDR_ERR_SBE		0xe58


/* Offsets for PIC registers */

#define	PIC_OFFSET		0x40000
#define	PCIBA			(CCSBAR | PIC_OFFSET)


/* Memory map for the WR SBC 85xx */

/* General */

#define DDR_SDRAM_ADRS1		0x00000000	/* 256 MB DDR RAM */
#define DDR_SDRAM_ADRS2		0x10000000	/* 256 MB DDR RAM */

/* Local bus addresses */

#define BOOT_FLASH_MAP_ADRS	0xff000000	/* tlb mapped to 4GB - 16MB */
#define BOOT_FLASH_ADRS		0xff800000	/* 8 MB in 8-bit mode CS0 */
#define USR_FLASH1_ADRS		0xe0000000	/* 64 MB in 32-bit mode CS1 */
#define USR_FLASH2_ADRS		0xe4000000	/* 64 MB in 32-bit mode CS6 */
#define LOCAL_SDRAM1_ADRS	0x20000000	/* 64 MB CS3 */
#define LOCAL_SDRAM2_ADRS	0x24000000	/* 64 MB CS4 */
#define ATM_ADRS		0x80000000	/* 8 KB in 8-bit mode CS7 */
#define UTIL_ADRS		0x28000000	/* 12 MB in 8-bit mode CS5 */

#define BOOT_FLASH_WINDOW_SIZE	0x800000	/* up to 8MB of Flash */
#define BOOT_FLASH_MAP_SIZE	0x01000000	/* 16 MB for mapping */
#define USR_FLASH_WINDOW_SIZE	0x04000000	/* up to 64MB of Flash */

#define UTIL_LED7SEG		(UTIL_ADRS + 0x0)
#define UTIL_USRSW		(UTIL_ADRS + 0x100000)
#define UTIL_SR1		(UTIL_ADRS + 0x200000)
#define UTIL_SR2		(UTIL_ADRS + 0x300000)
#define UTIL_RSTCTRL		(UTIL_ADRS + 0x400000)
#define UTIL_WPPORT		(UTIL_ADRS + 0x500000)
#define UTIL_DUART1		(UTIL_ADRS + 0x700000)
#define UTIL_DUART2		(UTIL_ADRS + 0x800000)
#define UTIL_RTC		(UTIL_ADRS + 0x900000)
#define UTIL_EEPROM		(UTIL_ADRS + 0xb00000)


/* CPU type in the PVR */

#define CPU_TYPE_8540		0x80200010
#define CPU_TYPE_8560		0x80200010	/* PVRs same */
#define BRD_SR2_8540SEL_MSK	0x10
#define BRD_SR2_MODEL_8540	0x10
#define BRD_SR2_MODEL_8560	0x00
#define SYS_MODEL_8540		"MPC8540 - Wind River SBC"
#define SYS_MODEL_8560		"MPC8560 - Wind River SBC"


/* Global function registers
 *
 * PORPLL used to detect clocking ratio for CCB/CPM, including serial.
 */

#define	M85XX_PORPLLSR(base)	(CAST(VUINT32 *)((base) + 0xE0000))
#define	M85XX_PORPLLSR_E500_RATIO_MASK	0x003f0000
#define	M85XX_PORPLLSR_PLAT_RATIO_MASK	0x0000003e
#define	M85XX_PORPLLSR_E500_RATIO(base) \
        ((*M85XX_PORPLLSR(base) & M85XX_PORPLLSR_E500_RATIO_MASK) >> 16)
#define	M85XX_PORPLLSR_PLAT_RATIO(base) \
        ((*M85XX_PORPLLSR(base) & M85XX_PORPLLSR_PLAT_RATIO_MASK) >> 1)

#define M85XX_PVR(base)		(CAST(VUINT32 *)((base) + 0xE00A0))
#define M85XX_SVR(base)		(CAST(VUINT32 *)((base) + 0xE00A4))

#define _PPC_BUCSR_FI		0x200	/* Invalidate branch cache */
#define _PPC_BUCSR_E		0x1	/* Enable branch prediction */


/* Serial port settings and offsets */

#define N_SIO_CHANNELS	 	2
#define N_UART_CHANNELS         N_SIO_CHANNELS
#define UART_REG_INTVL          1
#define DEFAULT_BAUD_RATE       9600

/* 8540 */

#define XTAL_CHIP		sysClkFreqGet()
#define COM1_CCSR_OFFSET	0x4500
#define COM2_CCSR_OFFSET	0x4600
#define COM1_CHIP_ADRS		((ULONG) CCSBAR + (ULONG) COM1_CCSR_OFFSET)
#define COM2_CHIP_ADRS		((ULONG) CCSBAR + (ULONG) COM2_CCSR_OFFSET)

/* 8560 */

#define XTAL_BRD		1843200
#define NS16550_BASE_ADRS       UTIL_DUART1
#define COM1_BRD_ADRS           NS16550_BASE_ADRS
#define COM1_INT_VEC            9
#define COM1_INT_LVL            COM1_INT_VEC
#define COM2_ADRS_OFFSET        0x100000
#define COM2_BRD_ADRS           (NS16550_BASE_ADRS + COM2_ADRS_OFFSET)
#define COM2_INT_VEC            10 
#define COM2_INT_LVL            COM2_INT_VEC


/* 7 segment LED */

#define LED_DISP_OFF		0xff
#define LED_DISP_DOT		0xfe
#define LED_DISP_A		0x11
#define LED_DISP_B		0x01
#define LED_DISP_C		0x63
#define LED_DISP_D		0x03
#define LED_DISP_E		0x61
#define LED_DISP_F		0x71
#define LED_DISP_0		(LED_DISP_D & LED_DISP_DOT)
#define LED_DISP_1		(~0x60 & LED_DISP_DOT)
#define LED_DISP_2		(~0xda & LED_DISP_DOT)
#define LED_DISP_3		(0x0d & LED_DISP_DOT)
#define LED_DISP_4		(0x99 & LED_DISP_DOT)
#define LED_DISP_5		(0x49 & LED_DISP_DOT)
#define LED_DISP_6		(0x41 & LED_DISP_DOT)
#define LED_DISP_7		(~0xe0 & LED_DISP_DOT)
#define LED_DISP_8		(LED_DISP_B & LED_DISP_DOT)
#define LED_DISP_9		(0x09 & LED_DISP_DOT)


/*
 * Timer clock divider ratios (in 2^n)
 */

#define FIT_DIVIDER_TAP_29 29
#define FIT_DIVIDER_TAP_28 28
#define FIT_DIVIDER_TAP_27 27
#define FIT_DIVIDER_TAP_26 26
#define FIT_DIVIDER_TAP_25 25
#define FIT_DIVIDER_TAP_24 24
#define FIT_DIVIDER_TAP_23 23
#define FIT_DIVIDER_TAP_22 22
#define FIT_DIVIDER_TAP_21 21
#define FIT_DIVIDER_TAP_20 20
#define FIT_DIVIDER_TAP_19 19
#define FIT_DIVIDER_TAP_18 18
#define FIT_DIVIDER_TAP_17 17
#define FIT_DIVIDER_TAP_16 16
#define FIT_DIVIDER_TAP_15 15
#define FIT_DIVIDER_TAP_14 14
#define FIT_DIVIDER_TAP_13 13
#define FIT_DIVIDER_TAP_12 12
#define FIT_DIVIDER_TAP_11 11
#define FIT_DIVIDER_TAP_10 10


/* Clock rates */

#define SYS_CLK_RATE_MIN        1       /* minimum system clock rate */
#define SYS_CLK_RATE_MAX        8000    /* maximum system clock rate */

#define AUX_CLK_RATE            ((PLAT_FREQ_DEFAULT >> 3) / (1 << FIT_DIVIDER_TAP_18))
                                                   /* apprx  95 Hz */                                                   
#define AUX_CLK_RATE_MIN        ((PLAT_FREQ_DEFAULT >> 3) / (1 << FIT_DIVIDER_TAP_21))
                                                   /* apprx   11 Hz */    
#define AUX_CLK_RATE_MAX        ((PLAT_FREQ_DEFAULT >> 3) / (1 << FIT_DIVIDER_TAP_10))
                                                 /* apprx 24414 Hz */

#define WDT_RATE_MIN            (sysTimerClkFreq / (1 << FIT_DIVIDER_TAP_29))
#define WDT_RATE_MAX            (sysTimerClkFreq / (1 << FIT_DIVIDER_TAP_21))


/* Maximum number of SCC channels to configure as SIOs. Note that this 
 * assumes sequential usage of SCCs.  SCC serial not used in wrSbc85xx.
 */

#define MAX_SCC_SIO_CHANS 2
#define M85XX_CPM_SICR(base)	(CAST(VUINT16 *)((base) + 0x90c00))
#define M85XX_CPM_SIVEC(base)	(CAST(VUINT8  *)((base) + 0x90c04))
#define M85XX_CPM_SIPNR_H(base)	(CAST(VUINT32 *)((base) + 0x90c08))
#define M85XX_CPM_SIPNR_L(base)	(CAST(VUINT32 *)((base) + 0x90c0c))
#define M85XX_CPM_SCPRR_H(base)	(CAST(VUINT32 *)((base) + 0x90c14))
#define M85XX_CPM_SCPRR_L(base)	(CAST(VUINT32 *)((base) + 0x90c18))
#define M85XX_CPM_SIMR_H(base)	(CAST(VUINT32 *)((base) + 0x90c1c))
#define M85XX_CPM_SIMR_L(base)	(CAST(VUINT32 *)((base) + 0x90c20))
#define M85XX_CPM_SIEXR(base)	(CAST(VUINT32 *)((base) + 0x90c24))
#define M85XX_CPM_SCCR(base)	(CAST(VUINT32 *)((base) + 0x90c80))


/* PCI defines and access macros */

#define CCSR_PCI_OFFSET     	0x08000
#define PCI_REGBASE		(CCSBAR | CCSR_PCI_OFFSET)

#define PCI_CLK_FREQ        	1843200

#define PCI_MEMIO2LOCAL(x) \
         (((UINT32)x  - PCI_MEMIO_ADRS) + CPU_PCI_MEMIO_ADRS)     

/* PCI IO memory adrs to CPU bus adrs */
       
#define PCI_IO2LOCAL(x) \
     (((UINT32)x  - PCI_IO_ADRS) + CPU_PCI_IO_ADRS)
	     
#define PCI_MEM2LOCAL(x) \
         (((UINT32)x  - PCI_MEM_ADRS) + CPU_PCI_MEM_ADRS)

/* CPU bus adrs to PCI (non-prefetchable) memory address */
	      
#define LOCAL2PCI_MEMIO(x) \
     ((int)(x) + PCI_MSTR_MEM_BUS)

#define PCI_CFG_ADR_REG  (CCSBAR + 0x8000)
#define PCI_CFG_DATA_REG (CCSBAR + 0x8004)
#define PCI_AUTO_CONFIG_ADRS  0x4c00

#define PPCACR_PRKM_MASK 0XF0
#define PCI_REQUEST_LEVEL 0x3

#define CLASS_OFFSET      0xB
#define CLASS_WIDTH       0x1
#define BRIDGE_CLASS_TYPE 0x6


#define PCICMD_ADRS     (PCI_CFG_BASE + 0x04)  /* PCI cmd reg */
#define PCICMD_VAL      0x00000006             /* PCI COMMAND Default value */
#define PCISTAT_ADRS    (PCI_CFG_BASE + 0x06)  /* PCI status reg */

#define NUM_PCI_SLOTS           0x5            /* 5 PCI slots: 0 to 4 - sebf */

#define PCI_INTA_IRQ            0x2            /* PCI INTA routed to IRQ2 */
#define PCI_INTB_IRQ            0x3            /* PCI INTB routed to IRQ3 */
#define PCI_INTC_IRQ            0x4            /* PCI INTC routed to IRQ4 */
#define PCI_INTD_IRQ            0x5            /* PCI INTD routed to IRQ5 */

#define PCI_SLOT1_DEVNO        0x10            /* PCI SLOT 1 Device no */

#define PCI_LAT_TIMER          0x40            /* latency timer value, 64 PCI clocks */

#define PCI1_DEV_ID      0x826010E3
#define PCI2_DEV_ID      0x826110E3
#define PCI3_DEV_ID      0x826210E3
#define PCI_DEV_ID_82XX  0x00031057            /* Id for MPC85xx - Rev1 */
#define PCI_DEV_ID_8540  0x00081057            /* Id for MPC8540 - Rev2 */
#define PCI_DEV_ID_8560  0x00091057            /* Id for MPC8560 - Rev2 */
#define PCI_ID_I82559           0x12298086     /* Id for Intel 82559 */
#define PCI_ID_I82559ER         0x12098086     /* Id for Intel 82559 ER */

#define MPC8266ADS_PCI_IRQ       08

#define PCI_XINT1_LVL           PCI_INTA_IRQ
#define PCI_XINT2_LVL           PCI_INTB_IRQ
#define PCI_XINT3_LVL           PCI_INTC_IRQ
#define PCI_XINT4_LVL           PCI_INTD_IRQ

#define PMC1_DEVNO              0x13           /* device number of the PCI-PMC slot (slot 0) */
#define PMC1_PCI_DRV            PCI_INTA_IRQ   /* default interrupt line, unused */
#define PMC2_PCI_DRV            PMC1_PCI_DRV   /* no PCI-PMC secondary slot on wrSbc8560 */
#define PCI_MSTR_MEMIO_LOCAL    PCI_MEMIO_ADRS

/* specific MPC8560 additional pci/x config header registers */
#define	PCI_CFG_PBFR   0x44
#define PCI_CFG_PBACR  0x46
#define PCI_CFG_PXNCIR 0x60
#define PCI_CFG_PXCPR  0x61
#define PCI_CFG_PXCR   0x62
#define PCI_CFG_PXSR   0x64

#ifndef _ASMLANGUAGE

#ifndef PCI_IN_BYTE
#define PCI_IN_BYTE(x)          sysPciInByte (x)
IMPORT  UINT8                   sysPciInByte  (UINT32 address);
#endif
#ifndef PCI_IN_WORD
#define PCI_IN_WORD(x)          sysPciInWord (x)
IMPORT  UINT16                  sysPciInWord  (UINT32 address);
#endif
#ifndef PCI_IN_LONG
#define PCI_IN_LONG(x)          sysPciInLong (x)
IMPORT  UINT32                  sysPciInLong  (UINT32 address);
#endif
#ifndef PCI_OUT_BYTE
#define PCI_OUT_BYTE(x,y)       sysPciOutByte (x,y)
IMPORT  void                    sysPciOutByte (UINT32 address, UINT8  data);
#endif
#ifndef PCI_OUT_WORD
#define PCI_OUT_WORD(x,y)       sysPciOutWord (x,y)
IMPORT  void                    sysPciOutWord (UINT32 address, UINT16 data);
#endif
#ifndef PCI_OUT_LONG
#define PCI_OUT_LONG(x,y)       sysPciOutLong (x,y)
IMPORT  void                    sysPciOutLong (UINT32 address, UINT32 data);
#endif

#endif  /* _ASMLANGUAGE */

#define DELTA(a,b)                 (abs((int)a - (int)b))

#define BUS	0				/* bus-less board */


/* This value is the setting for the MPTPR[PTP] Refresh timer prescaler.
 * The value is dependent on the OSCILLATOR_FREQ value.  For other values
 * a conditionally compiled term must be created here for that OSCILLATOR_FREQ
 * value.
 *
 * BRGCLK_DIV_FACTOR
 * Baud Rate Generator division factor - 0 for division by 1
 *                                       1 for division by 4
 *                                       2 for division by 16
 *                                       3 for division by 64
 */

#define DIV_FACT_4      0
#define DIV_FACT_16     1
#define DIV_FACT_64     2
#define DIV_FACT_256    3

#if (OSCILLATOR_FREQ == FREQ_66_MHZ)
#   define TPR			0x2000
#   define BRGCLK_DIV_FACTOR	DIV_FACT_16
#endif

#define M8260_BRGC_DIVISOR      BRGCLK_DIV_FACTOR


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

#ifdef __cplusplus
    }
#endif	/* __cplusplus */

#endif	/* INCwrSbc85xxh */

