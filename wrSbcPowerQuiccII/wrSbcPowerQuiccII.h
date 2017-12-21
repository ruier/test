/* wrSbcPowerQuiccII.h - board header */

/*
 * Copyright (c) 2003, 2004, 2006, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01l,15nov12,ylu  add STATUS_MODCK_H0/H1/H2/H3 macros.(WIND00057961) 
01k,16jun06,pch  remove definition of CPU
01j,10nov04,j_b  project support for boot Flash selection (SPR 103800)
01i,09feb04,j_b  remove M48T59_WRITE_EN,M48T59_WRITE_DIS macros
01h,12nov03,rec  add sysCpuPwrEnable prototype
01g,23jun03,j_b  add RTC support
01f,29may03,j_b  remove ATM macros
01e,09may03,scm  remove BUS_TYPE
01d,02may03,j_b  remove CPU_ID_MASK
01c,30apr03,j_b  add input frequencies; replace INIT_ register names with PQII_
01b,17apr03,j_b  add CPU ID and Status Register support
01a,01apr03,j_b  adapted from wrSbc8260Atm (ver 01b)
*/

#ifndef	__INCwrSbcPowerQuiccIIh
#define	__INCwrSbcPowerQuiccIIh

#ifdef __cplusplus
extern "C" {
#endif

/* Include files */

#include "drv/mem/memDev.h"
#include "drv/mem/flashDev.h"
#include "drv/intrCtl/m8260IntrCtl.h"

/* Defintion for Bit macro */
#ifndef BIT
#define BIT(bitNumber)		(1 << (bitNumber))
#endif /* BIT */

#define STRING_MATCH  0

/* On board device I/O addresses */

#define LOCAL_BUS_SDRAM_ADRS      0x20000000 /* Local Bus SDRAM base adrs */
#define LOCAL_BUS_SDRAM_SIZE      0x01000000 /* 16MB Local Bus SDRAM */
#define EEPROM_ADRS               0x22000000 /* EEPROM base address */
#define EEPROM_SIZE               0x2000     /* 8KB EEPROM */
#define INTERNAL_MEM_MAP_ADDR     0xF0000000 /* Internal Memory Map base Address  */
#define INTERNAL_MEM_MAP_SIZE     0x00020000 /* 128 K bytes      */
#define BSCR_USER_SWITCH          0x22004000 /* Read Only */
#define BSCR_STATUS               0x22006000 /* Read/Write */
#define BSCR_LED_REGISTER         0x2200A000 /* Write Only */
#define BSCR_REG_SIZE             0x2000     /* Board Ctrl/Status register size */
#define RTC_ADRS                  0x80000000 /* Real-Time Clock & SRAM */
#define RTC_SIZE                  0x2000     /* Real-Time Clock SRAM + reg size */

#define FLASH_SIZE_2MB       0x00200000
#define FLASH_SIZE_32MB	     0x02000000
#define FLASH_SIZE_64MB	     0x04000000

/* Flash address definitions, depending on boot configuration */
#ifdef INCLUDE_ON_BOARD_BOOT_FLASH
#  undef BOOT_FLASH
#  define BOOT_FLASH         ON_BOARD_FLASH
#elif defined INCLUDE_SIMM_BOOT_FLASH
#  undef BOOT_FLASH
#  define BOOT_FLASH         SIMM_FLASH
#endif

#if (BOOT_FLASH == ON_BOARD_FLASH)
#  define CS0_FLASH_ADRS     0xFE000000
#  define CS0_FLASH_SIZE     FLASH_SIZE_2MB  /*  2MB */
#  define CS6_FLASH_ADRS     0xE0000000
#  define CS6_FLASH_SIZE     FLASH_SIZE_64MB /* 64MB */
#  define CS1_FLASH_ADRS     (CS6_FLASH_ADRS + FLASH_SIZE_64MB)
#  define CS1_FLASH_SIZE     0x0 /* remaining SIMM */
#elif (BOOT_FLASH == SIMM_FLASH)
#  define CS0_FLASH_ADRS     0xFC000000
#  define CS0_FLASH_SIZE     FLASH_SIZE_64MB /* 64MB */
#  define CS6_FLASH_ADRS     0xE0000000
#  define CS6_FLASH_SIZE     FLASH_SIZE_2MB  /*  2MB */
#  define CS1_FLASH_ADRS     (CS6_FLASH_ADRS + FLASH_SIZE_64MB)
#  define CS1_FLASH_SIZE     0x0 /* remaining SIMM */
#else
#  error Boot Flash must be selected
#endif

/* Reset Configuration Locations */

#define HRDW_CONFIG_BYTE1         0xfe000000 /* Most Significant Byte */
#define HRDW_CONFIG_BYTE2         0xfe000008
#define HRDW_CONFIG_BYTE3         0xfe000010
#define HRDW_CONFIG_BYTE4         0xfe000018

#define MODCK_H_MASK              0x0F       /* MODCK_H in HRDW_CONFIG_BYTE4 */

/* Decrementer rate determination */

#define  DEC_CLOCK_FREQ           OSCILLATOR_FREQ
#define  DEC_ADJUSTMENT           4
#define  DEC_CLK_TO_INC	          DEC_ADJUSTMENT

/* Cache Commands  */

#define CACHE_DISABLE_CMD         0x0400
#define CACHE_ENABLE              0x0200
#define CACHE_UNLOCK_ALL          0x0A00
#define CACHE_INVALIDATE_ALL      0x0C00

/* Internal Memory Map Register */
#define IMMR_ISB_MASK             0xFFFE0000 /* Internal Space Base mask */
#define IMMR_PARTNUM_MASK         0x0000FF00 /* Part Number mask */
#define IMMR_MASKNUM_MASK         0x000000FF /* Mask Number mask */

/* Clock Parameters */

#define	SYS_CLK_RATE_MIN          1    /* min system clock rate    */
#define	SYS_CLK_RATE_MAX          8000 /* max system clock rate    */
#define	AUX_CLK_RATE_MIN          1    /* min auxiliary clock rate */
#define	AUX_CLK_RATE_MAX          8000 /* max auxiliary clock rate */

/* Define number of Serial channels  */

#define BUS                       0      /* bus-less board            */
#define N_SIO_CHANNELS            2      /* No. serial I/O channels   */

/* create a single macro INCLUDE_MMU */

#if defined(INCLUDE_MMU_BASIC) || defined(INCLUDE_MMU_FULL)
#define INCLUDE_MMU
#endif

/* Only one can be selected, FULL overrides BASIC */

#ifdef INCLUDE_MMU_FULL
#   undef INCLUDE_MMU_BASIC
#endif

/* PCI/MODCK/FLASH Status Register Fields */
#define STATUS_BUILD_MASK	0x8000  /* Build Type: ATM=0, PCMCIA=1 */
#define STATUS_MODCK13_MASK	0x7000  /* MODCK[1-3] */
#define STATUS_FPD31_MASK	0x0E00  /* F_PD[3-1] from Flash SIMM */
#define STATUS_MODCKH_MASK	0x00F0  /* MODCK_H[3-0] valid if PCIMODE = 0 */
#define STATUS_DLLEN_MASK	0x0008  /* Internal DLL in PCIMODE: Enable=1 */
#define STATUS_ARBEN_MASK	0x0004  /* Internal PCI Arbiter: Enable=0 */
#define STATUS_HOSTEN_MASK	0x0002  /* PCI Interface Mode: Host=0,Agent=1 */
#define STATUS_PCIMODE_MASK	0x0001  /* PCI Mode: Enable=0, Disable=1  */

#define STATUS_MODCKH0_MASK	0x0001  /* MODCK_H[0] valid if PCIMODE = 0 */
#define STATUS_MODCKH1_MASK	0x0002  /* MODCK_H[1] valid if PCIMODE = 0 */
#define STATUS_MODCKH2_MASK	0x0004  /* MODCK_H[2] valid if PCIMODE = 0 */
#define STATUS_MODCKH3_MASK	0x0008  /* MODCK_H[3] valid if PCIMODE = 0 */

/* PowerQUICC II Internal Registers */

#define PQII_REG_BASE	0x10000  /* Internal Register Base Offset */

/* Individual register addresses below are in addition to base offset above */

/* SIU Group */

#define PQII_IMMR       0x01A8
#define PQII_SIUMCR     0x0000
#define PQII_SYPCR      0x0004
#define PQII_SWT        0x0008
#define PQII_SWSR       0x000E
#define PQII_BCR        0x0024
#define PQII_PPC_ACR    0x0028
#define PQII_PPC_ALRH   0x002C
#define PQII_PPC_ALRL   0x0030
#define PQII_LCL_ACR    0x0034
#define PQII_LCL_ALRH   0x0038
#define PQII_LCL_ALRL   0x003C
#define PQII_TESCR1     0x0040
#define PQII_TESCR2     0x0044
#define PQII_LTESCR1    0x0048
#define PQII_LTESCR2    0x004C
#define PQII_PDTEA      0x0050
#define PQII_PDTEM      0x0054
#define PQII_LDTEA      0x0058
#define PQII_LDTEM      0x005C

/* MEMC Group */

#define PQII_MAR        0x0168
#define PQII_MAMR       0x0170
#define PQII_MBMR       0x0174
#define PQII_MCMR       0x0178
#define PQII_MPTPR      0x0184
#define PQII_MDR        0x0188
#define PQII_PSDMR      0x0190
#define PQII_LSDMR      0x0194
#define PQII_PURT       0x0198
#define PQII_PSRT       0x019C
#define PQII_LURT       0x01A0
#define PQII_LSRT       0x01A4
#define PQII_PCIBR0     0x01AC
#define PQII_PCIBR1     0x01B0
#define PQII_PCIMSK0    0x01C4
#define PQII_PCIMSK1    0x01C8
#define PQII_RMR        0x0C94

/* Clock Group */

#define PQII_SCCR       0x0c80
#define PQII_SCMR       0x0c88

    /* SCCR fields */
#define SCCR_PCIMODE_MASK  0x00000100
#define SCCR_PCIMODCK_MASK 0x00000080
#define SCCR_PCIDF_MASK    0x00000078

    /* SCMR fields */
#define SCMR_CORECNF_MASK    0x1f000000
#define SCMR_CPMDF       1	/* MPC82xx CPM division factor is always 1 */
#define PQII_CPMDF       (SCMR_CPMDF + 1)

        /* PLL fields in SCMR - different for new CPU generation */
#define SCMR_PLLDF_MASK_HIP4 0x00001000  /* use with 825x and 826x */
#define SCMR_PLLMF_MASK_HIP4 0x00000fff  /* use with 825x and 826x */
#define SCMR_PLLMF_MASK_HIP7 0x0000000f  /* use with 827x amd 828x */

/* Chip Select Group */

#define PQII_OR0        0x0104
#define PQII_BR0        0x0100
#define PQII_OR1        0x010C
#define PQII_BR1        0x0108
#define PQII_OR2        0x0114
#define PQII_BR2        0x0110
#define PQII_OR3        0x011C
#define PQII_BR3        0x0118
#define PQII_OR4        0x0124
#define PQII_BR4        0x0120
#define PQII_OR5        0x012C
#define PQII_BR5        0x0128
#define PQII_OR6        0x0134
#define PQII_BR6        0x0130
#define PQII_OR7        0x013C
#define PQII_BR7        0x0138
#define PQII_OR8        0x0144
#define PQII_BR8        0x0140
#define PQII_OR9        0x014C
#define PQII_BR9        0x0148
#define PQII_OR10       0x0154
#define PQII_BR10       0x0150
#define PQII_OR11       0x015C
#define PQII_BR11       0x0158

    /* BR fields */
#define BR_VALID          0x00000001
#define BR_PORT_SZ_MASK   0x00001800
#define BR_PORT_SZ_8BIT   0x00000800
#define BR_PORT_SZ_16BIT  0x00001000
#define BR_PORT_SZ_32BIT  0x00001800
#define BR_PORT_SZ_64BIT  0x00000000  /* 60x bus only */

/* 60x Bus Group */

#define PQII_PSDMR1    0x0190
#define PQII_MEM0      0x0000
#define PQII_PSDMR2    0x0190
#define PQII_MEM1      0x0001
#define PQII_MEM2      0x0002
#define PQII_MEM3      0x0003
#define PQII_MEM4      0x0004
#define PQII_MEM5      0x0005
#define PQII_MEM6      0x0006
#define PQII_MEM7      0x0007
#define PQII_MEM8      0x0008
#define PQII_PSDMR3    0x0190
#define PQII_MEM9      0x0000
#define PQII_PSDMR4    0x0190

/* Local Bus Group */

#define PQII_LSDMR1    0x0194
#define PQII_LSDMR2    0x0194
#define PQII_LSDMR3    0x0194
#define PQII_LSDMR4    0x0194

/* Ports A, B, C and D */

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

/* Real-Time Clock, required macros */
#define M48T59_BASE      (RTC_ADRS)  /* RTC Base Address */
#define M48T59_INTERVAL  1           /* Addressing interval */
#undef M48T59_WRITE_EN   /* no special write enable code required */
#undef M48T59_WRITE_DIS  /* no special write disable code required */

/* General */

#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif

#undef  EIEIO
#define EIEIO      WRS_ASM("	eieio")

#undef  EIEIO_SYNC
#define EIEIO_SYNC WRS_ASM("	eieio;	sync")

#ifndef _ASMLANGUAGE
#ifdef INCLUDE_POWER_MGMT_CPU_BSP_SUPPORT
IMPORT void sysCpuPwrEnable (void);
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif	/* __INCwrSbcPowerQuiccIIh */

