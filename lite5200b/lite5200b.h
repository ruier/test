/* lite5200b.h - Freescale Lite5200B board header */

/*
 * Copyright (c) 2003-2004, 2006-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01i,16apr08,b_m  change auxClk min and max rate.
01h,18jan07,b_m  Code clean up.
01g,03feb06,pkr  add support for Lite5200b
01f,03feb06,pkr  adapt to different frequency jumpers
01e,25jun04,bjn  fix typo with SYS_FREF (becomes SYS_FREQ)
01d,03jun04,bjn  PCI windows
01c,04mar04,bjn  Updating SDRAM configuration
                 Made IS_ICECUBE default board.
01b,04feb04,bjn  support for IS_ICECUBE_OLD
01a,25Jun03,pkr  adopted from templatePpc/config.h
*/

/*
This file contains I/O addresses and related constants for the board.
*/

#ifndef __INClite5200bh
#define __INClite5200bh

#ifdef __cplusplus
extern "C" {
#endif

#include "ppc5200.h"
#include "drv/intrCtl/m5200Intr.h"

#define MBAR_VALUE              0xF0000000

/* Clock Parameters */

#define SYS_CLK_RATE_MIN        10      /* min system clock rate    */
#define SYS_CLK_RATE_MAX        5000    /* max system clock rate    */
#define AUX_CLK_RATE_MIN        30      /* min auxiliary clock rate */
#define AUX_CLK_RATE_MAX        10000   /* max auxiliary clock rate */

/* MPC5200 PCI Memory map */

/* Slave window that makes local (60x bus) memory visible to PCI devices */

#define CPU_PCI_SLV_MEM_LOCAL_A     MBAR_VALUE
#define CPU_PCI_SLV_MEM_LOCAL_B     0x00000000  /* LOCAL_MEM_LOCAL_ADRS */

#define BUS_PCI_SLV_MEM_LOCAL_A     0x90000000
#define BUS_PCI_SLV_MEM_LOCAL_B     0xC0000000

/* Latency Timer value - 64 PCI clocks */

#undef  PCI_LAT_TIMER
#define PCI_LAT_TIMER       0x40

/* initiator 0 - used for prefetchable memory */

#define CPU_PCI_MEM_ADRS    0x40000000  /* 32 bit prefetchable memory base addres of PCI mem */
#define PCI_MEM_SIZE        0x10000000  /* 32 bit prefetchable memory size  */
#define BUS_PCI_MEM_ADRS    0x40000000  /* 32 bit prefetchable memory from PCI bus */

/* initiator 1 - used for PCI IO */
/* BUS_PCI_IO_ADRS needs to be 0 in order to support 16 bit I/O on PCI */

#define CPU_PCI_IO_ADRS     0x70000000  /* base address of PCI IO */
#define PCI_IO_SIZE         0x10000000
#define BUS_PCI_IO_ADRS     0x00000000  /* base address of PCI IO from PCI bus */

/* initiator 2 - used for non prefetchable memory */

#define CPU_PCI_NO_PRE_MEM_ADRS    0x50000000  /* 32 bit non-prefetchable memory location */
#define PCI_NO_PRE_MEM_SIZE        0x10000000  /* 32 bit non-prefetchable memory size */
#define BUS_PCI_NO_PRE_MEM_ADRS    0x50000000  /* 32 bit non-prefetchable memory from PCI bus */

/* PCI IO memory adrs to CPU (60x bus) adrs */

#define PCI_IO2LOCAL(x) \
     ((int)(x) + CPU_PCI_IO_ADRS - BUS_PCI_IO_ADRS)

#define PCI_MEMIO2LOCAL(x)  \
    ((int)(x) + CPU_PCI_NO_PRE_MEM_ADRS - BUS_PCI_NO_PRE_MEM_ADRS)

/* 60x bus adrs to PCI (non-prefetchable) memory address */

#define LOCAL2PCI_MEMIO(x) \
     ((int)(x) + PCI_SLV_MEM_LOCAL)

/* PCI device configuration definitions */

#ifndef PCI_CFG_FORCE
#    define PCI_CFG_FORCE   0x0     /* UNSUPPORTED */
#endif

#ifndef PCI_CFG_AUTO
#     define PCI_CFG_AUTO   0x1     /* IS SUPPORTED */
#endif

#ifndef PCI_CFG_NONE
#    define PCI_CFG_NONE    0x2     /* UNSUPPORTED */
#endif

#define MPC5200_PCI_BRIDGE  0

/* PCI configuration type */

#ifndef PCI_CFG_TYPE
#define PCI_CFG_TYPE        PCI_CFG_AUTO
#endif

#ifndef _ASMLANGUAGE

#ifndef PCI_IN_BYTE
#   define PCI_IN_BYTE(x)       sysPciInByte  ((UINT32)x)
    IMPORT  UINT8               sysPciInByte  (UINT32 address);
#endif

#ifndef PCI_IN_WORD
#   define PCI_IN_WORD(x)       sysPciInWord  ((UINT32)x)
    IMPORT  UINT16              sysPciInWord  (UINT32 address);
#endif

#ifndef PCI_IN_LONG
#   define PCI_IN_LONG(x)       sysPciInLong  ((UINT32)x)
    IMPORT  UINT32              sysPciInLong  (UINT32 address);
#endif

#ifndef PCI_OUT_BYTE
#   define PCI_OUT_BYTE(x,y)    sysPciOutByte ((UINT32)x,y)
    IMPORT  void                sysPciOutByte (UINT32 address, UINT8  data);
#endif

#ifndef PCI_OUT_WORD
#   define PCI_OUT_WORD(x,y)    sysPciOutWord ((UINT32)x,y)
    IMPORT  void                sysPciOutWord (UINT32 address, UINT16 data);
#endif

#ifndef PCI_OUT_LONG
#   define PCI_OUT_LONG(x,y)    sysPciOutLong ((UINT32)x,y)
    IMPORT  void                sysPciOutLong (UINT32 address, UINT32 data);
#endif

#define PCI_MAX_BUS         1
#define PCI_MIN_DEV         24
#define PCI_MAX_DEV         30
#define PCI_MAX_FUNC        8

#endif  /* _ASMLANGUAGE */

/* Cache Line Size - 32 32-bit value = 128 bytes */

#define PCI_CLINE_SZ        0x20

/* Bus Speed configuration det. by SDRAM_CONFIG0 and SDRAM_CONFIG1 */

#define SYS_FREQ            33000000L   /* 33 MHz */

#ifdef DYN_FREQ
extern int sysGetFvcoFreq();
#define SYS_PLL_FREQ        (sysGetFvcoFreq())
#else

/*
 * SDRAM_CONFIG1 - switch SYS
 * low i.e. sys_pll_cfg_0=0 -> PLLx16
 * high, i.e. sys_pll_cfg_0=1 -> PLLx12
 */

#define SYS_PLL_FREQ        (SYS_FREQ * 16)
#endif

#ifdef DYN_FREQ
extern int sysGetXlbClock();
#define XLB_CLOCK           (sysGetXlbClock())
#else

/*
 * SDRAM_CONFIG1 - switch XLB
 * low, i.e. xlb_clk_sel=0   -> VCO=%4
 * high, i.e. xlb_clk_sel=1  -> VCO=%8
 */

#define XLB_CLOCK           (SYS_PLL_FREQ / 4)
#endif

#ifdef DYN_FREQ
extern int sysGetCoreSpeed();
#define CORE_CLOCK_FREQ     (sysGetCoreSpeed())
#else

/*
 * Core speed: CONFIG0. PPC0->ppc_pll_cfg4, ..., PPC4->ppc_pll_cfg0
 * see table 5.3 MPC5200 UMan
 * PPC3 only 1 => bus:core=1:3, core:vco=1:2, bus.vco=1:6
 */

#define CORE_CLOCK_FREQ     (XLB_CLOCK * 3)
#endif

/*
 * GPS Port Configuration Register
 *
 * Note: This is part 1 of the Port Configuration. The main
 *       part is done later. The first part is here neccsary,
 *       because of the CS1 of the DDR-SDRAM.
 *
 * (Settings: CS1:     GPIO_WKUP6 as CS1 for DDR-SDRAM
 *            ALTs:    CAN1 on I2C1, CAN2 on Tmr0/1
 *            CS7:     Interrupt GPIO on PSC3_5
 *            CS6:     Interrupt GPIO on PSC3_4
 *            ATA:     ATA_CS0,1 on CS4,5
 *            USB_CLK: USB 48 MHz CLK is internally generated
 *            IrDA:    UART (without CD) is used
 *            Ether:   Ethernet 100 MBit with MD
 *            PCI_DIS: PCI Controller is enabled
 *            USB_SE:  Differential mode
 *            USB:     use as USB
 *            PSC3:    SPI with UART3 (no CD)
 *            PSC2:    UART (without CD)
 *            PSC1:    AC97 functionality)
 */

#define GPS_INIT_VALUE      0x91551C42

/* DDRAM mt46v16m16-75 @ 132 MHz on Lite5200B */

#define DDR                 1
#define SDRAM_EXTD_MODE     0x4009
#define SDRAM_MODE		    0x018D

/*
 * MC SDRAM Control Register
 *
 * (Settings: MEM_CLK_Enable:        yes
 * #ifdef IS_LITE5200B
 *	     DDR:		    yes
 * #endif
 *            Enable Refresh:        yes
 *            DDR Mode:              no
 *            HI-ADDR:               1
 *            Precharge Control Bit: MEM_A10
 *            Drive Rule:            Drive MDQ & MDQS except reads
 *            Refresh Interval:      64ms / 4K page
 *            DQS_OE:                0000)
 */

#define SDRAM_CONTROL       0x704F0F00

/*
 * MC SDRAM Config Register 1
 *
 * (Settings: Read to Read/Write/Precharge delay:   0xD
 *            Single Write to Read/Write/Precharge: 0x2
 *            Read CAS Latency:                     0x3
 *            Active to Read/Write delay:           0x2
 *            Precharge to Active delay:            0x2
 *            Refresh to Active delay:              0x8
 *            Write Latency:                        0x0)
 */

#define SDRAM_CONFIG1       0x73722930

/*
 * MC SDRAM Config Register 2
 *
 * (Settings: Burst Read to Read/Precharge delay:        0x8
 *            Burst Write to Read/Write/Precharge delay: 0xA
 *            Burst Read to Read/Write/Precharge delay:  0xD
 *            Burst Length:                              0x7)
 */

#define SDRAM_CONFIG2       0x47770000
#define SDRAM_TAP_DEL       0x10

/*
 * (Settings: WType: WaitX for Read Cycles, WaitP for Write Cycles
 *          WaitP: 0 Cycles
 *          WaitX: 4 Cycles
 *          MX:    non-multiplexed
 *          ALE:   1 IP Bus Clock (not used)
 *          AA:    active
 *          CE:    enabled
 *          AS:    24 Address Bits
 *          DS:    8 Data Bits
 *          Bank:  not used
 *          WS:    no write swapping
 *          RS:    no read swapping
 *          WO:    allow write operation
 *          RO:    allow read operation)
 */

#define LPC_CS0_BOOT_CFG_VALUE  0x00047800
#define LPC_CS1_CFG_VALUE       0x00047800
#define LPC_CS_DEAD_CTRL_VALUE  0x33333333

/*
 * IPB and PCI Frequency
 * (Write to CDM Configuration Register)
 *
 * (Setting: IPB CLK=XLB CLK/2
 *           PCI CLK=IPB CLK/2
 */

#define CDM_CFG_VALUE       0x00000101

#ifdef DYN_FREQ
extern int sysGetIpbClock();
#define IPB_CLOCK           (sysGetIpbClock())
#define PCI_CLOCK           (sysGetPciClock())
extern int sysGetPciClock();
#else
#define IPB_CLOCK           (XLB_CLOCK / 2)

/* the next is required in configNet.h */

#define IPB_CLOCK_LITERAL   66000000
#if (IPB_CLOCK != IPB_CLOCK_LITERAL)
#error
#endif

#define PCI_CLOCK           (IPB_CLOCK / 2)
#endif

/* Decrementer */

#define DEC_CLOCK_FREQ      XLB_CLOCK
#define DEC_CLK_TO_INC      4

/* Uarts (PSC1-6) */

#define UART_CLOCK          IPB_CLOCK

/*
 * Fraction Divider
 * (Write to CDM 48MHz Fraction Divider Configuration Register)
 *
 * Settings: USB:              Use internal clock
 *           Irda:             Use internal clock
 *           Fraction Divider: Enable
 *           Div Ratio:        FVCO CLK/11 (=48MHz) for PLL div 16
 *           Div Ratio:        FVCO CLK/8.25 (=48MHz) for PLL div 12
 */

#define CDM_DIV_VALUE_16    0x00015555
#define CDM_DIV_VALUE_12    0x00010001

#define CDM_ENA_VALUE       0x000FFCFF

#define XLB_ADDR_TO         0xffff
#define XLB_DATA_TO         0xffff

/*
 * This macro returns the positive difference between two unsigned ints.
 * Useful for determining delta between two successive decrementer reads.
 */

#define DELTA(a,b)          (abs((int)a - (int)b))

#define UART_REG_ADDR_INTERVAL  1   /* addr diff of adjacent regs */

/* number of Serial channels  */

#define N_SIO_CHANNELS          1       /* No. serial I/O channels   */

#define WRONG_CPU_MSG           "Unsupported processor type for this board.\n"

#define CPU_TYPE                ((vxPvrGet() >> 16) & 0xffff)
#define CPU_TYPE_5200           0x8082  /* PPC 5200 CPU */

/* General */

/* Common I/O synchronizing instructions */

#undef  EIEIO
#define EIEIO                   WRS_ASM("   eieio")

#undef  EIEIO_SYNC
#define EIEIO_SYNC              WRS_ASM("   eieio;  sync")

/* Flash is 16MB at CS0 and 16 MB at CS1 */

#define CS0_FLASH_BASE_ADRS		0xFF000000
#define CS0_FLASH_SIZE			0x01000000
#define CS1_FLASH_BASE_ADRS		0xFE000000
#define CS1_FLASH_SIZE			0x01000000

#define FLASH_SIZE              0x02000000  /* 32 MB */

/* IDE/ATA support */

#define SYS_ATA_MAX_CTRLS       1
#undef  USE_ATA_BLOCK_WRAPPER

/*
 * The ATA_DEVx_STATE determines whether the ATA driver should probe
 * for a device.
 *
 * ATA_DEV_PRESENT     = probe for the device;
 * ATA_DEV_NOT_PRESENT = don't probe for the device.
 *
 * To probe for all devices connect to both buses,
 * change all ATA_DEVx_STATE values to DEV_PRESENT.
 *
 *  ATA_DEV0_STATE = cntlr 0 / device 0
 *  ATA_DEV1_STATE = cntlr 0 / device 1
 */

/*
 * below we use the pre-processor to determine the number of ATA devices
 * to probe on each controller.  This is setup by the user in config.h
 * If the ATA_DEVx_STATE is ATA_DEV_PRESENT then the drive is probed.
 */

#define ATA_DEV_PRESENT         1   /* Drive does exist */
#define ATA_DEV_NOT_PRESENT     0   /* Drive does not exist */

#define ATA_DEV0_STATE		    ATA_DEV_PRESENT
#define ATA_DEV1_STATE		    ATA_DEV_NOT_PRESENT

#if ((ATA_DEV0_STATE == ATA_DEV_PRESENT) &&         \
         (ATA_DEV1_STATE == ATA_DEV_PRESENT))
#   define ATA_CTRL0_DRIVES 2

#elif ((ATA_DEV0_STATE == ATA_DEV_PRESENT) &&       \
           (ATA_DEV1_STATE == ATA_DEV_NOT_PRESENT))
#   define ATA_CTRL0_DRIVES 1

#elif ((ATA_DEV0_STATE == ATA_DEV_NOT_PRESENT) &&   \
           (ATA_DEV1_STATE == ATA_DEV_NOT_PRESENT))
#   define ATA_CTRL0_DRIVES 0

#endif

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#endif  /* __INClite5200bh */
