/* xmc8540A.h - Motorola xmc8540 board header file */

/* Copyright 2004-2005, Motorola Inc. All Rights Reserved */

/*
modification history
--------------------
01k,05apr05,cak Modified Local Access Window configuration to match the
                PCI outbound window configuration.
01j,07feb05,cak Added definition for size of local board access 
		registers. 
01i,31jan05,cak VPD Support.
01h,27jan05,cak NVRAM support.
01g,18jan05,cak #define'd LOCAL_MEM_AUTOSIZE.
01f,06jan05,cak Moved the oscillator frequency definition here 
		from config.h. 
01e,17dec04,cak Added Tempe (Tsi148) support definitions.
01d,13dec04,cak PCI/PCI-X Bus Interface support.
01c,18nov04,cak Memory controller support. 
01b,09nov04,scb I2c support.
01a,11oct04,scb Initial writing.
*/

#ifndef __INCxmc8540Ah
#define __INCxmc8540Ah

/*
This file contains macros and defines for I/O addresses, chip
selects etc. for the XMC module.
*/

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

/*
 * This define must be set to the value of the resonant oscillator
 */

#define OSCILLATOR_FREQ FREQ_33_MHZ

/* 
 * Define board specific addresses and field programming values.
 * There are three different register domains which much be programmed
 * in order to access an area such a flash or IO space.  These
 * domains are:
 * 
 * MMU access -   This will be facilitated by programming TLB1 entries
 * 	          to cover the appropriate address range.  TLB1 entries
 * 	          are programmed initially inside of "romInit.s" and 
 * 	          again after the kernel comes up, using the "sysLib.c"
 * 	          resident table sysStaticTlbDesc[].
 * 
 * Local access - This is covered programming of the appropriate LAWBARn
 *                LAWARn register set.  Along with an address range, these
 * 	          registers specify the associated target interface.  This
 * 	          programming is performed inside of "romInit.s"
 * 
 * Chip select  - This is governed by BRn and ORn register programming.
 * 	          This programming is also done inside of "romInit.s".
 * 
 * All three of these register domains must be set in harmony with each
 * other to properly access flash or IO space.  The assumptions concerning 
 * flash are:
 * 
 * 1.  There may be up to two different banks of flash, the 'boot' flash
 *     and the 'alternate' flash.  The XMC modulecontains only one bank, 
 *     the 'boot' flash.
 * 
 * 2.  The boot and alternate flash banks occupy one contiguous region
 *     of memory and can thus be covered by a single TLB1 address range,
 *     and local access register set.  Separate flash banks require
 *     separate chip selects (and thus two sets of BRn/ORn registers
 *     must be programmed).  Note the we may actually have two separate
 *     TLB1 entries covering the same address range, one with a TS
 *     (Translation Space) bit set to zero, and one with TS set to 1.
 *     This will allow both the bootrom (TS = 0) and the kernel (TS = 1)
 *     to access this space.
 * 
 */

#define FLASH_BASE_ADRS            0xff000000
#define FLASH_MMU_TLB_SZ	   _MMU_TLB_SZ_16M
#define FLASH_LAWAR_SZ		   MPC8540_PCI_SIZE_ENCODE(FLASH_WINDOW_SIZE) 
#define FLASH_WINDOW_SIZE          0x01000000 
#define FLASH_BOOT_ADRS		   0xff000000
#define FLASH_BOOT_SZ	   	   0x01000000

 /* No alternate flash bank for XMC */

#define LOCAL_BOARD_ACCESS_ADRS	   	0x50000000
#define LOCAL_BOARD_ACCESS_SZ		0x08000000
#define LOCAL_BOARD_ACCESS_2_ADRS	0x58000000
#define LOCAL_BOARD_ACCESS_2_SZ	        0x08000000
#define LOCAL_BOARD_ACCESS_SIZE		0x00040000
#define LOCAL_BOARD_ACCESS_MMU_TBL_SZ	_MMU_TLB_SZ_256M
#define LOCAL_BOARD_ACCESS_LAWAR_SZ	\
	MPC8540_PCI_SIZE_ENCODE(LOCAL_BOARD_ACCESS_SZ + LOCAL_BOARD_ACCESS_2_SZ)
#define LED_ADRS		   	(LOCAL_BOARD_ACCESS_ADRS)

/*
 * Define programming values for the 8 sets of local access window 
 *  registers (LAWBARn/LAWARn) 
 */

/* Access to local DRAM */

#define CCSR_LAWBAR0_PROG (CCSR_LAWBAR_BASE_ADDR_ENCODE(LOCAL_MEM_LOCAL_ADRS))
#define CCSR_LAWAR0_PROG  (CCSR_LAWAR_EN_VAL_ENABLED | \
			   CCSR_LAWAR_TGTIF_VAL_DDRSDRAM | \
			   MPC8540_PCI_SIZE_ENCODE(0x40000000))

/* Access to flash (covers chip select 0 (BR0/OR0)) */

#define CCSR_LAWBAR1_PROG (CCSR_LAWBAR_BASE_ADDR_ENCODE(FLASH_BASE_ADRS))
#define CCSR_LAWAR1_PROG  (CCSR_LAWAR_EN_VAL_ENABLED | \
                           CCSR_LAWAR_TGTIF_VAL_LBC | \
			   FLASH_LAWAR_SZ)

/*
 * Access to local I/O (covers chip BOTH selects 1 and 2 
 * (BR1/OR1 and BR2/or2)) 
 */ 

#define CCSR_LAWBAR2_PROG  \
	(CCSR_LAWBAR_BASE_ADDR_ENCODE(LOCAL_BOARD_ACCESS_ADRS))
#define CCSR_LAWAR2_PROG  (CCSR_LAWAR_EN_VAL_ENABLED | \
                           CCSR_LAWAR_TGTIF_VAL_LBC | \
			   LOCAL_BOARD_ACCESS_LAWAR_SZ)

/*
 * Now let's cover PCI space.  We must configure one local access
 * window for each of the configured outbound PCI windows.  There
 * will be a total of four windows configured for PCI space:
 *
 * PCI_MSTR_MEMIO_LOCAL
 * PCI_MSTR_MEM_LOCAL
 * ISA_MSTR_IO_LOCAL/PCI_MSTR_IO_LOCAL
 * VME_MEM_LOCAL_START
 */

#define CCSR_LAWBAR3_PROG  \
        (CCSR_LAWBAR_BASE_ADDR_ENCODE(PCI_MSTR_MEMIO_LOCAL))
#define CCSR_LAWAR3_PROG  (CCSR_LAWAR_EN_VAL_ENABLED | \
                           CCSR_LAWAR_TGTIF_VAL_PCIX | \
                           MPC8540_PCI_SIZE_ENCODE(PCI_MSTR_MEMIO_SIZE))

#define CCSR_LAWBAR4_PROG \
        (CCSR_LAWBAR_BASE_ADDR_ENCODE(PCI_MSTR_MEM_LOCAL))
#define CCSR_LAWAR4_PROG (CCSR_LAWAR_EN_VAL_ENABLED | \
                           CCSR_LAWAR_TGTIF_VAL_PCIX | \
                           MPC8540_PCI_SIZE_ENCODE(PCI_MSTR_MEM_SIZE))

#define CCSR_LAWBAR5_PROG \
        (CCSR_LAWBAR_BASE_ADDR_ENCODE(ISA_MSTR_IO_LOCAL))
#define CCSR_LAWAR5_PROG (CCSR_LAWAR_EN_VAL_ENABLED | \
                           CCSR_LAWAR_TGTIF_VAL_PCIX | \
                           MPC8540_PCI_SIZE_ENCODE(PCI_IO_SIZE))

#define CCSR_LAWBAR6_PROG  \
        (CCSR_LAWBAR_BASE_ADDR_ENCODE(VME_MEM_LOCAL_START))
#define CCSR_LAWAR6_PROG  (CCSR_LAWAR_EN_VAL_ENABLED | \
                           CCSR_LAWAR_TGTIF_VAL_PCIX | \
                           MPC8540_PCI_SIZE_ENCODE(VME_MEM_SIZE))

/* No more access windows required so we set the last one to 'invalid' */

#define CCSR_LAWBAR7_PROG  CCSR_LAWBARn_INVALID
#define CCSR_LAWAR7_PROG   CCSR_LAWARn_INVALID

/* 
 * Set up the 'chip select' programming for the 8 sets of chip select
 * registers (BRn/ORn). 
 */

/*
 * ----- Chip select 0
 *
 * BR0 to be set as follows (flash boot access):
 *   bit 0       = 1    ->  V: valid
 *   bit 2:3     = 00   ->  ATOM: Not used for atomic operations
 *   bit 5:7     = 000  ->  MSEL: Machine select = GPCM
 *   bit 8       = 0    ->  WP: Read and write accesses allowed
 *   bit 10:9    = 00   ->  DECC: Err check disable, parity generated
 *   bit 12:11   = 11   ->  PS: Port size = 32-bit
 *   bit 31:16   = xx   ->  BA: FLASH_BOOT_ADRS (base address)
 */

#define CCSR_BR0_PROG	(FLASH_BOOT_ADRS | 0x1801)

/*
 * OR0 to be set as follows:
 *   bit 0       = 1    -> EAD: Extra bus clock cycles are added (LALE
 *                           is asserted for the number of bus clock
 *                           cycles specified by LCRR[EADC].
 *   bit 1       = 0    -> EHTR: Normal timing (no additional cycles).
 *   bit 2       = 0    -> TRLX: Normal timing generated by GPCM
 *   bit 3       = 0    -> SETA: Access teminated internaly by memory
 *                           controller.
 *   bit 7:4     = 1001 -> SCY: 9 bus clock cycle wait states.
 *   bit 8       = 0    -> XACS: Address to chip select setup 
 *                           determined by ORx[ACS] & LCRR[CLKDIV].
 *   bit 10:9    = 00   -> ACS: LCSn# is output at the same time as
 *                           the address lines.
 *   bit 11      = 1    -> CSNT: LCSn# and LWE# are negated earlier 
 *                           depending on the value of LCRR[CLKDIV].
 *   bit 12      = 0    -> BCTLD: LBCTL is asserted upon access to 
 *                           the current memory bank.
 *   bit 31:16   = xxxx -> AM: GPCM address mask.
 *
 */

# define CCSR_OR0_PROG	(CCSR_ORn_AM_VAL(FLASH_BOOT_SZ) | 0x891)

/*
 * ----- Chip select 1
 *
 * BR1 to be set as follows (On board peripherals, LEDs):
 * bit  0     = 1    ->  V: valid
 * bits 2:3   = 00b  ->  ATOM: Not used for atomic operations
 * bits 5:7   = 000b ->  MSEL: Machine select = GPCM
 * bit  8     = 0    ->  WP: Read and write accesses allowed
 * bits 10:9  = 00b  ->  DECC: Err check disable, parity generated
 * bits 12:11 = 01b  ->  PS: Port size = 8-bit
 * bits 31:16 = xx   ->  BA: LOCAL_BOARD_ACCESS_BASE_ADRS (base address)
 */

#define CCSR_BR1_PROG	(LOCAL_BOARD_ACCESS_ADRS | 0x801)
	
/*
 * OR1 to be set as follows:
 *   bit 0       = 1    -> EAD: Extra bus clock cycles are added (LALE
 *                           is asserted for the number of bus clock
 *                           cycles specified by LCRR[EADC].
 *   bit 1       = 0    -> EHTR: Normal timing (no additional cycles).
 *   bit 2       = 0    -> TRLX: Normal timing generated by GPCM
 *   bit 3       = 0    -> SETA: Access teminated internaly by memory
 *                           controller.
 *   bit 7:4     = 0101 -> SCY: 5 bus clock cycle wait states.
 *   bit 8       = 0    -> XACS: Address to chip select setup 
 *                           determined by ORx[ACS] & LCRR[CLKDIV].
 *   bit 10:9    = 00   -> ACS: LCSn# is output at the same time as
 *                           the address lines.
 *   bit 11      = 1    -> CSNT: LCSn# and LWE# are negated earlier 
 *                           depending on the value of LCRR[CLKDIV].
 *   bit 12      = 0    -> BCTLD: LBCTL is asserted upon access to 
 *                           the current memory bank.
 *   bit 31:16   = xxxx -> AM: GPCM address mask.
 *
 */

# define CCSR_OR1_PROG	(CCSR_ORn_AM_VAL(LOCAL_BOARD_ACCESS_SZ) | 0x851)

/*
 * ----- Chip select 2
 *
 * BR2 to be set as follows (on board peripherals, flash ctl reg):
 * bit  0     = 1    ->  V: valid
 * bits 2:3   = 00b  ->  ATOM: Not used for atomic operations
 * bits 5:7   = 000b ->  MSEL: Machine select = GPCM
 * bit  8     = 0    ->  WP: Read and write accesses allowed
 * bits 10:9  = 00b  ->  DECC: Err check disable, parity generated
 * bits 12:11 = 11b  ->  PS: Port size = 32-bit
 * bits 31:16 = xx   ->  BA: LOCAL_BOARD_ACCESS_2_ADRS (base address)
 */

#define CCSR_BR2_PROG	(LOCAL_BOARD_ACCESS_2_ADRS | 0x1801)

/*
 * OR2 to be set as follows:
 *   bit 0       = 1    -> EAD: Extra bus clock cycles are added (LALE
 *                           is asserted for the number of bus clock
 *                           cycles specified by LCRR[EADC].
 *   bit 1       = 0    -> EHTR: Normal timing (no additional cycles).
 *   bit 2       = 0    -> TRLX: Normal timing generated by GPCM
 *   bit 3       = 0    -> SETA: Access teminated internaly by memory
 *                           controller.
 *   bit 7:4     = 0101 -> SCY: 5 bus clock cycle wait states.
 *   bit 8       = 0    -> XACS: Address to chip select setup 
 *                           determined by ORx[ACS] & LCRR[CLKDIV].
 *   bit 10:9    = 00   -> ACS: LCSn# is output at the same time as
 *                           the address lines.
 *   bit 11      = 1    -> CSNT: LCSn# and LWE# are negated earlier 
 *                           depending on the value of LCRR[CLKDIV].
 *   bit 12      = 0    -> BCTLD: LBCTL is asserted upon access to 
 *                           the current memory bank.
 *   bit 31:16   = xxxx -> AM: GPCM address mask.
 *
 */

#define CCSR_OR2_PROG	(CCSR_ORn_AM_VAL(LOCAL_BOARD_ACCESS_2_SZ) | 0x851)

/* The rest are invalid */

#define CCSR_BR3_PROG	(CCSR_BRn_INVALID)
#define CCSR_OR3_PROG	(CCSR_ORn_INVALID)

#define CCSR_BR4_PROG	(CCSR_BRn_INVALID)
#define CCSR_OR4_PROG	(CCSR_ORn_INVALID)

#define CCSR_BR5_PROG	(CCSR_BRn_INVALID)
#define CCSR_OR5_PROG	(CCSR_ORn_INVALID)

#define CCSR_BR6_PROG	(CCSR_BRn_INVALID)
#define CCSR_OR6_PROG	(CCSR_ORn_INVALID)

#define CCSR_BR7_PROG	(CCSR_BRn_INVALID)
#define CCSR_OR7_PROG	(CCSR_ORn_INVALID)

/* I2C related */

#define USR_BRD_EEPROM_256B_ADDR 	(0xae)
#define USR_BRD_EEPROM_32KB_ADDR 	(0xa4)
#define USR_BRD_MAX6900_RTC_ADDR	(0xa0)

#define VPD_BRD_EEPROM_I2C_ADDR		USR_BRD_EEPROM_256B_ADDR
#define VPD_BYTE_ADDR_SZ		1

#define VALID_I2C_WRT_ADDR	USR_BRD_EEPROM_32KB_ADDR, \
				0xffffffff

#define I2C_WRITE_ENABLE	{}

/*
 * Non-volatile (NVRAM) defines.
 *
 * The XMC8540 board does not have a true NVRAM chip.  This
 * functionality is implemented via a 32KB EEPROM.  
 *
 * The first 0x100 (256 bytes) are used to store the VxWorks 
 * boot parameters.
 */

#undef  NV_BOOT_OFFSET
#define NV_BOOT_OFFSET          0x0 
#define NV_RAM_SIZE             0x8000  /* 32KB Total */
#define NV_RAM_INTRVL           1
#define NV_RAM_I2C_ADDR         USR_BRD_EEPROM_32KB_ADDR 
#define NV_RAM_READ(x)          sysNvRead (x,NV_RAM_I2C_ADDR)
#define NV_RAM_WRITE(x,y)       sysNvWrite (x,y,NV_RAM_I2C_ADDR)

/* DDR SDRAM Memory Controller Configuration Parameters */

#define DDR_SDRAM_CS_0_MODE		DDR_SDRAM_PAGE_MODE
#define DDR_SDRAM_CS_1_MODE		DDR_SDRAM_PAGE_MODE
#define DDR_SDRAM_CS_2_MODE		DDR_SDRAM_PAGE_MODE
#define DDR_SDRAM_CS_3_MODE		DDR_SDRAM_PAGE_MODE
#define DDR_SDRAM_BSTOPRE_VAL		0x100
#define MPC8540_DDR_SDRAM_WRREC_VAL	3
#define MPC8540_DDR_SDRAM_WRTORD_VAL	1
#define DDR_SDRAM_WR_DATA_DELAY_VAL	0x2
#define DDR_SDRAM_ACSM_VAL		0
#define DDR_SDRAM_CPO_VAL		0x0
#define DDR_SDRAM_SDMODE_VAL		0x0063
#define DDR_SDRAM_ESDMODE_VAL		0x0000
#define DDR_SDRAM_2T_EN_VAL		0
#define DDR_SDRAM_NCAP_VAL		0
#define DDR_SDRAM_DYN_PWR_VAL		0
#define DDR_SDRAM_SREN_VAL		1
#define DDR_SDRAM_SBET_VAL		0xFF
#undef	INCLUDE_ECC
#define	LOCAL_MEM_AUTOSIZE
#define LOCAL_MEM_SIZE			0x01000000

#define	MUSHKIN_3726966		(1)
#define	MICRON_MT9VDDT6472HG	(2)
#define SODIMM_TYPE	MUSHKIN_3726966	

/* External Interrupt Sources */

#define TEMPE_INT_LVL	(EPIC_VEC_EXT_IRQ3)
#define PCI_VME_LINT0   (EPIC_VEC_EXT_IRQ3)
#define PCI_VME_LINT1   (EPIC_VEC_EXT_IRQ4)
#define PCI_VME_LINT2   (EPIC_VEC_EXT_IRQ5)
#define PCI_VME_LINT3   (EPIC_VEC_EXT_IRQ6)

/* Tempe Support - Tempe Prove Out Board */

#define TEMPE_PCI_BUS           0         /* PCI bus of Tempe PCI-VME bridge */
#define TEMPE_PCI_DEV           0x15      /* Device number of Tempe */
#define TEMPE_PCI_FCN           0         /* Function number of Tempe */

/* PCI interrupt assignments */

#define INT_LINES \
    { \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { PCI_VME_LINT0, PCI_VME_LINT1,    /* Tsi148 on prove-out */ \
	  PCI_VME_LINT2, PCI_VME_LINT3 },  /* board */ \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff }, \
        { 0xff,0xff,0xff,0xff } \
    }

#define EEBACR_STREAM_DIS      0x0000000b
#define EEBPCR_CPU_ENB_HIGHEST 0x01000002

#define PHY_ADDR_ENET_PORT0 1
#define PHY_ADDR_ENET_PORT1 2
#define PHY_ADDR_ENET_PORT2 3

#define MAX_ENET_PORTS 2

/* Transmit descriptors per queue per port */

#define ETHER_TX_DESC_NUM      0x80

/* Receive descriptors per queue per port */

#define ETHER_RX_DESC_NUM      0x80

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* __INCxmc8540Ah */
