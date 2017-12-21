/* wrSbcP5020.h - WINDRIVER SBCP5020 board header */

/*
 * Copyright (c) 2011, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,05jul12,c_l  added related define for romInit.s (WIND00360538)
01e,25jun12,y_y  changed parameter FLASH_OR0_VALUE for 133Mhz.
01d,25apr12,y_y  added SATA support.
01c,27mar12,y_y  added LED support.
01b,06mar12,y_y  add vxbus I2C support.
01a,24nov11,y_y  Created from fsl_p5020_ds 01u
*/

/*
 * This file contains I/O addresses and related constants for the
 * WINDRIVER SBCP5020 board.
*/

#ifndef __INCwrSbcP5020h
#define __INCwrSbcP5020h

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#ifndef EIEIO_SYNC
#   define EIEIO_SYNC  _WRS_ASM (" eieio; sync")
#endif        /* EIEIO_SYNC */
#ifndef EIEIO
#   define EIEIO    _WRS_ASM (" eieio")
#endif       /* EIEIO */

#ifdef  _ASMLANGUAGE
#   define CAST(x)
#else /* _ASMLANGUAGE */
typedef volatile UCHAR VCHAR;           /* shorthand for volatile UCHAR */
typedef volatile INT32 VINT32;          /* volatile unsigned word */
typedef volatile INT16 VINT16;          /* volatile unsigned halfword */
typedef volatile INT8 VINT8;            /* volatile unsigned byte */
typedef volatile UINT32 VUINT32;        /* volatile unsigned word */
typedef volatile UINT16 VUINT16;        /* volatile unsigned halfword */
typedef volatile UINT8 VUINT8;          /* volatile unsigned byte */
#   define CAST(x)  (x)
#endif  /* _ASMLANGUAGE */

/* Core set */

#define CPU_BIT(n)                  (1 << n)
#define MAX_CPUS                    sysCpuNumGet()
#define PHY_MAX_CPUS                4   /* wrSbcP3041 */

/* Define Clock Speed and source  */

#define FREQ_1_MHZ                  1000000
#define FREQ_33_MHZ                 33333334
#define FREQ_100_MHZ                100000000
#define FREQ_133_MHZ                133333334
#define FREQ_600_MHZ                600000000
#define FREQ_750_MHZ                750000000
#define FREQ_800_MHZ                800000000
#define OSCILLATOR_FREQ             FREQ_100_MHZ

/* Frequency applied to the "RTC" pin */

#define RTC_FREQ                    25000000
   
#define DEFAULT_SYSCLKFREQ          (FREQ_600_MHZ / 2)

#define FIT_CLK_RATE_MIN            (DEFAULT_SYSCLKFREQ / (1 << 21))
#define FIT_CLK_RATE_MAX            (DEFAULT_SYSCLKFREQ / (1 << 10))

/* Use to force freq used with DUART/Timers etc */

#undef FORCE_DEFAULT_FREQ

/* Base Address of Memory Mapped Registers */

#define CCSBAR                      0xFE000000
#define CCSBAR_RESET                0xFE000000
#define CCSBAR_COMMIT               0x80000000
#define CCSRBARH_OFFSET             0x0
#define CCSRBARL_OFFSET             0x4
#define CCSRAR_OFFSET               0x8

/* Base Address of queue manager access window */

#define QMAN_LAW_BASE               0xF4200000
#define QMAM_LAW_SIZE               0x100000
#define QMAM_MEM_SIZE               0x200000 

/* Base Address of buffer manager access window */

#define BMAN_LAW_BASE               0xF4000000
#define BMAM_SIZE                   0x100000

/*
 * "magic numbers" used to validate the rebootVec.  The definitions
 * are arbitrary, but should not be "common" values like all zeros,
 * all ones, 0xeeeeeeee, 0x55aa55aa, 0xdeadbeef, etc.
 */

#define MP_MAGIC_RBV                0xfeedfeed
#define MP_MAGIC_CACHE_FLUSHED      0xD07ED07E

/* Base address of debug control/status registers */

#define DCSR_LAW_BASE               0xFD000000
#define DCSR_LAW_SIZE               0x400000

#define ETHERNET_MAC_HANDLER

#define WR_ENET0                    0x00
#define WR_ENET1                    0xA0
#define WR_ENET2                    0x1E
                                    
#define CUST_ENET3_0                0xA0
#define CUST_ENET3_1                0xA1
#define CUST_ENET3_2                0xA2
#define CUST_ENET3_3                0xA3
#define CUST_ENET3_4                0xA4
#define CUST_ENET3_5                0xA5
#define CUST_ENET4                  0xAA
#define CUST_ENET5                  0xA0

/* Two types ethernet controllers on P5020, DTSEC and TGEC  */

#define MAX_MAC_DEVS                2
#define DTSEC_NUM                   5
#define TGEC_NUM                    1
                                    
/* default mac address */           
                                    
#define MAX_MAC_ADRS                (DTSEC_NUM + TGEC_NUM)
#define MAC_ADRS_LEN                6
#define NV_MAC_ADRS_OFFSET          0x200
                                    
/* MPIC VEC defines */              
                                    
#undef  EPIC_VEC_IN_IRQ0
#define EPIC_VEC_IN_IRQ0            16

#define EPIC_DMA1_CHAN0_INT_VEC     (EPIC_VEC_IN_IRQ0 + 12 )
#define EPIC_DMA1_CHAN1_INT_VEC     (EPIC_VEC_IN_IRQ0 + 13 )
#define EPIC_DMA1_CHAN2_INT_VEC     (EPIC_VEC_IN_IRQ0 + 14 )
#define EPIC_DMA1_CHAN3_INT_VEC     (EPIC_VEC_IN_IRQ0 + 15 )
#define EPIC_DMA2_CHAN0_INT_VEC     (EPIC_VEC_IN_IRQ0 + 16 )
#define EPIC_DMA2_CHAN1_INT_VEC     (EPIC_VEC_IN_IRQ0 + 17 )
#define EPIC_DMA2_CHAN2_INT_VEC     (EPIC_VEC_IN_IRQ0 + 18 )
#define EPIC_DMA2_CHAN3_INT_VEC     (EPIC_VEC_IN_IRQ0 + 19 )
#undef  EPIC_DUART_INT_VEC
#define EPIC_DUART_INT_VEC          (EPIC_VEC_IN_IRQ0 + 20)
#undef  EPIC_DUART2_INT_VEC
#define EPIC_DUART2_INT_VEC         (EPIC_VEC_IN_IRQ0 + 21)
#undef  EPIC_I2C_INT_VEC
#define EPIC_I2C_INT_VEC            (EPIC_VEC_IN_IRQ0 + 22)
#undef  EPIC_I2C2_INT_VEC
#define EPIC_I2C2_INT_VEC           (EPIC_VEC_IN_IRQ0 + 23)
#define EPIC_USB1_INT_VEC           (EPIC_VEC_IN_IRQ0 + 28)
#define EPIC_USB2_INT_VEC           (EPIC_VEC_IN_IRQ0 + 29)
#define EPIC_SDHC_INT_VEC           (EPIC_VEC_IN_IRQ0 + 32)
#define EPIC_SATA1_INT_VEC          (EPIC_VEC_IN_IRQ0 + 52)
#define EPIC_SATA2_INT_VEC          (EPIC_VEC_IN_IRQ0 + 53)

#define EPIC_QMAN_PORTAL0_INT_VEC   104
#define EPIC_QMAN_PORTAL1_INT_VEC   106
#define EPIC_QMAN_PORTAL2_INT_VEC   108
#define EPIC_QMAN_PORTAL3_INT_VEC   110
#define EPIC_QMAN_PORTAL4_INT_VEC   112
#define EPIC_QMAN_PORTAL5_INT_VEC   114
#define EPIC_QMAN_PORTAL6_INT_VEC   116
#define EPIC_QMAN_PORTAL7_INT_VEC   118
#define EPIC_QMAN_PORTAL8_INT_VEC   120
#define EPIC_QMAN_PORTAL9_INT_VEC   122

#define EPIC_BMAN_PORTAL0_INT_VEC   105
#define EPIC_BMAN_PORTAL1_INT_VEC   107
#define EPIC_BMAN_PORTAL2_INT_VEC   109
#define EPIC_BMAN_PORTAL3_INT_VEC   111
#define EPIC_BMAN_PORTAL4_INT_VEC   113
#define EPIC_BMAN_PORTAL5_INT_VEC   115
#define EPIC_BMAN_PORTAL6_INT_VEC   117
#define EPIC_BMAN_PORTAL7_INT_VEC   119
#define EPIC_BMAN_PORTAL8_INT_VEC   121
#define EPIC_BMAN_PORTAL9_INT_VEC   123

/* OpenPIC timer vectors */

#define OPENPIC_TIMERA0_INT_VEC     200
#define OPENPIC_TIMERB0_INT_VEC     204

/* GPIO vector */

#define GPIO_INT_VEC                55

/* PCI defines */

/*
 * PCI based addresses
 * Note: the LAW_BASE values have been chosen to coincide
 * with those used by U-Boot on the P5020DS board.
 */

#define PCIEX1_LAW_BASE             0x80000000       /* PCI LAW window */
#define PCIEX1_LAW_SIZE             0x10000000       /* PCI LAW size   */

#define PCIEX1_MEM_SIZE             0x04000000        /* 64 MB */
#define PCIEX1_MEMIO_SIZE           0x04000000        /* 64 MB */
#define PCIEX1_IO_SIZE              0x04000000        /* 64 MB */
#define PCIEX1_IO32_SIZE            0x04000000        /* 64 MB */

#define PCIEX1_MEM_ADRS             (PCIEX1_LAW_BASE)
#define PCIEX1_MEMIO_ADRS           (PCIEX1_MEM_ADRS   + PCIEX1_MEM_SIZE)
#define PCIEX1_IO_ADRS              (PCIEX1_MEMIO_ADRS + PCIEX1_MEMIO_SIZE)
#define PCIEX1_IO32_ADRS            (PCIEX1_IO_ADRS    + PCIEX1_IO_SIZE)

#define PCI_MMU_TLB_SZ              _MMU_TLB_SZ_256M

/* latency timer value, 64 PCI clocks */

#define PCI_LAT_TIMER               0x40  

#define NUM_PCIEX1_SLOTS            0x4

/*
 * The wrSbcP5020 processor has one PCIe slot, each bridges's interrupt supports
 * two mechanisms, INTx emulation and Message Signaled Interrupt (MSI) mode.
 * The bridge provide four Virtual Wires(INTA, INTB, INTC and INTD) for
 * signaling interrupts across a Link. When the bridge works on INTx emulation
 * mode, the virtual Wires INTA are routed to dedicated internal
 * interrupts (24, 25, 26, and 27) in the MPIC, the others INTx are logically
 * combined with the IRQn signals so that they share the external interrupts.
 * If the bridge interrupts works on MSI mode, interrupts generated from
 * the EP device will be routed to the MSIIR register in the PIC, virtual Wires
 * INTx will not be triggered.
 */

#define EPIC_VEC_PCIE1_INTA         (EPIC_VEC_IN_IRQ0 + 24)

#define PCIEX1_XINT1_LVL            (EPIC_VEC_PCIE1_INTA)
#define PCIEX1_XINT2_LVL            (EPIC_VEC_EXT_IRQ0 + 1)
#define PCIEX1_XINT3_LVL            (EPIC_VEC_EXT_IRQ0 + 2)
#define PCIEX1_XINT4_LVL            (EPIC_VEC_EXT_IRQ0 + 3)

/* Local Access Windows Regster Offsets from CCSBAR */

/* Used for Flash ROM */

#define  P5020_LAWBARH0(base)       (CAST(VUINT32 *)((base) + 0xc00))
#define  P5020_LAWBARL0(base)       (CAST(VUINT32 *)((base) + 0xc04))
#define  P5020_LAWAR0(base)         (CAST(VUINT32 *)((base) + 0xc08))

/* Used for DDR controller 1 */

#define  P5020_LAWBARH1(base)       (CAST(VUINT32 *)((base) + 0xc10))
#define  P5020_LAWBARL1(base)       (CAST(VUINT32 *)((base) + 0xc14))
#define  P5020_LAWAR1(base)         (CAST(VUINT32 *)((base) + 0xc18))

/* Used for DDR controller 2 */

#define  P5020_LAWBARH2(base)       (CAST(VUINT32 *)((base) + 0xc20))
#define  P5020_LAWBARL2(base)       (CAST(VUINT32 *)((base) + 0xc24))
#define  P5020_LAWAR2(base)         (CAST(VUINT32 *)((base) + 0xc28))

/* Used for LBC PIXIS */

#define  P5020_LAWBARH3(base)       (CAST(VUINT32 *)((base) + 0xc30))
#define  P5020_LAWBARL3(base)       (CAST(VUINT32 *)((base) + 0xc34))
#define  P5020_LAWAR3(base)         (CAST(VUINT32 *)((base) + 0xc38))

/* Used for EEPROM */

#define  P5020_LAWBARH4(base)       (CAST(VUINT32 *)((base) + 0xc40))
#define  P5020_LAWBARL4(base)       (CAST(VUINT32 *)((base) + 0xc44))
#define  P5020_LAWAR4(base)         (CAST(VUINT32 *)((base) + 0xc48))

/* Used for LED */

#define  P5020_LAWBARH5(base)       (CAST(VUINT32 *)((base) + 0xc50))
#define  P5020_LAWBARL5(base)       (CAST(VUINT32 *)((base) + 0xc54))
#define  P5020_LAWAR5(base)         (CAST(VUINT32 *)((base) + 0xc58))

/* Used for LCD */

#define  P5020_LAWBARH6(base)       (CAST(VUINT32 *)((base) + 0xc60))
#define  P5020_LAWBARL6(base)       (CAST(VUINT32 *)((base) + 0xc64))
#define  P5020_LAWAR6(base)         (CAST(VUINT32 *)((base) + 0xc68))

/* Used for TFFS */

#define  P5020_LAWBARH7(base)       (CAST(VUINT32 *)((base) + 0xc70))
#define  P5020_LAWBARL7(base)       (CAST(VUINT32 *)((base) + 0xc74))
#define  P5020_LAWAR7(base)         (CAST(VUINT32 *)((base) + 0xc78))

/* Unused */

#define  P5020_LAWBARH8(base)       (CAST(VUINT32 *)((base) + 0xc80))
#define  P5020_LAWBARL8(base)       (CAST(VUINT32 *)((base) + 0xc84))
#define  P5020_LAWAR8(base)         (CAST(VUINT32 *)((base) + 0xc88))

#define  P5020_LAWBARH29(base)      (CAST(VUINT32 *)((base) + 0xDD0))
#define  P5020_LAWBARL29(base)      (CAST(VUINT32 *)((base) + 0xDD4))
#define  P5020_LAWAR29(base)        (CAST(VUINT32 *)((base) + 0xDD8))

#define  P5020_LAWBARH30(base)      (CAST(VUINT32 *)((base) + 0xDE0))
#define  P5020_LAWBARL30(base)      (CAST(VUINT32 *)((base) + 0xDE4))
#define  P5020_LAWAR30(base)        (CAST(VUINT32 *)((base) + 0xDE8))

#define  LAWBAR_ADRS_SHIFT          0
#define  LAWAR_ENABLE               0x80000000
#define  LAWAR_TGTIF_PCIEX1         0x00000000
#define  LAWAR_TGTIF_PCIEX2         0x00100000
#define  LAWAR_TGTIF_PCIEX3         0x00200000
#define  LAWAR_TGTIF_PCIEX4         0x00300000
#define  LAWAR_TGTIF_RAPIDIO        0x00800000
#define  LAWAR_TGTIF_RAPIDIO2       0x00900000
#define  LAWAR_TGTIF_DDRSDRAM       0x01000000
#define  LAWAR_TGTIF_DDRSDRAM2      0x01100000
#define  LAWAR_TGTIF_MEM_INTRL      0x01400000
#define  LAWAR_TGTIF_BMAN           0x01800000
#define  LAWAR_TGTIF_QMAN           0x01900000
#define  LAWAR_TGTIF_DCSR           0x01D00000
#define  LAWAR_TGTIF_LBC            0x01f00000
#define  LAWAR_TGTIF_RESERVE        0x01E00000

/* LAWAR SIZE Settings */

#define  LAWAR_SIZE_4KB             0x0000000B
#define  LAWAR_SIZE_8KB             0x0000000C
#define  LAWAR_SIZE_16KB            0x0000000D
#define  LAWAR_SIZE_32KB            0x0000000E
#define  LAWAR_SIZE_64KB            0x0000000F
#define  LAWAR_SIZE_128KB           0x00000010
#define  LAWAR_SIZE_256KB           0x00000011
#define  LAWAR_SIZE_512KB           0x00000012
#define  LAWAR_SIZE_1MB             0x00000013
#define  LAWAR_SIZE_2MB             0x00000014
#define  LAWAR_SIZE_4MB             0x00000015
#define  LAWAR_SIZE_8MB             0x00000016
#define  LAWAR_SIZE_16MB            0x00000017
#define  LAWAR_SIZE_32MB            0x00000018
#define  LAWAR_SIZE_64MB            0x00000019
#define  LAWAR_SIZE_128MB           0x0000001A
#define  LAWAR_SIZE_256MB           0x0000001B
#define  LAWAR_SIZE_512MB           0x0000001C
#define  LAWAR_SIZE_1GB             0x0000001D
#define  LAWAR_SIZE_2GB             0x0000001E

/*
 * Local Bus Controller (LBC) Registers - unmentioned bits are reserved
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
 *     21-22 Address to chip select setup
 *     23    Extra Address to chip select setup
 *     24-27 Cycle length in Bus clocks - 0000->1111 0->15 wait states
 *     28    External address termination
 *     29    Timing relaxed
 *     30    Extended hold time for read access
 *     31    External address latch delay
 */

#define  LBC_BASE                   0x124000
#define  P5020_BR0(base)            (CAST(VUINT32 *)(base + LBC_BASE + 0x000))
#define  P5020_OR0(base)            (CAST(VUINT32 *)(base + LBC_BASE + 0x004))
#define  P5020_BR1(base)            (CAST(VUINT32 *)(base + LBC_BASE + 0x008))
#define  P5020_OR1(base)            (CAST(VUINT32 *)(base + LBC_BASE + 0x00c))
#define  P5020_BR2(base)            (CAST(VUINT32 *)(base + LBC_BASE + 0x010))
#define  P5020_OR2(base)            (CAST(VUINT32 *)(base + LBC_BASE + 0x014))
#define  P5020_BR3(base)            (CAST(VUINT32 *)(base + LBC_BASE + 0x018))
#define  P5020_OR3(base)            (CAST(VUINT32 *)(base + LBC_BASE + 0x01c))
#define  P5020_BR4(base)            (CAST(VUINT32 *)(base + LBC_BASE + 0x020))
#define  P5020_OR4(base)            (CAST(VUINT32 *)(base + LBC_BASE + 0x024))
#define  P5020_BR5(base)            (CAST(VUINT32 *)(base + LBC_BASE + 0x028))
#define  P5020_OR5(base)            (CAST(VUINT32 *)(base + LBC_BASE + 0x02C))
#define  P5020_BR6(base)            (CAST(VUINT32 *)(base + LBC_BASE + 0x030))
#define  P5020_OR6(base)            (CAST(VUINT32 *)(base + LBC_BASE + 0x034))
#define  P5020_BR7(base)            (CAST(VUINT32 *)(base + LBC_BASE + 0x038))
#define  P5020_OR7(base)            (CAST(VUINT32 *)(base + LBC_BASE + 0x03C))

#define  P5020_MAR(base)            (CAST(VUINT32 *)(base + LBC_BASE + 0x068))
#define  P5020_MAMR(base)           (CAST(VUINT32 *)(base + LBC_BASE + 0x070))
#define  P5020_MBMR(base)           (CAST(VUINT32 *)(base + LBC_BASE + 0x074))
#define  P5020_MCMR(base)           (CAST(VUINT32 *)(base + LBC_BASE + 0x078))
#define  P5020_MRTPR(base)          (CAST(VUINT32 *)(base + LBC_BASE + 0x084))
#define  MRTPR_PTP_MASK             0xff000000
#define  MRTPR_PTP_WRITE(x)         ((x << 24) & MRTPR_PTP_MASK)
#define  P5020_MDR(base)            (CAST(VUINT32 *)(base + LBC_BASE + 0x088))

#define  P5020_LTESR(base)          (CAST(VUINT32 *)(base + LBC_BASE + 0x0B0))
#define  P5020_LTEDR(base)          (CAST(VUINT32 *)(base + LBC_BASE + 0x0B4))
#define  P5020_LTEIR(base)          (CAST(VUINT32 *)(base + LBC_BASE + 0x0B8))
#define  P5020_LTEATR(base)         (CAST(VUINT32 *)(base + LBC_BASE + 0x0BC))
#define  P5020_LTEAR(base)          (CAST(VUINT32 *)(base + LBC_BASE + 0x0C0))

/* LBC Clock Configuration */

#define  P5020_LBCR(base)           (CAST(VUINT32 *)(base + LBC_BASE + 0x0D0))
#define  P5020_LCRR(base)           (CAST(VUINT32 *)(base + LBC_BASE + 0x0D4))

/* LBC Flash Registers */

#define  P5020_FMR(base)            (CAST(VUINT32 *)(base + LBC_BASE + 0x0E0))
#define  P5020_FIR(base)            (CAST(VUINT32 *)(base + LBC_BASE + 0x0E4))
#define  P5020_FCR(base)            (CAST(VUINT32 *)(base + LBC_BASE + 0x0E8))
#define  P5020_FBAR(base)           (CAST(VUINT32 *)(base + LBC_BASE + 0x0EC))
#define  P5020_FPAR(base)           (CAST(VUINT32 *)(base + LBC_BASE + 0x0F0))
#define  P5020_FBCR(base)           (CAST(VUINT32 *)(base + LBC_BASE + 0x0F4))

#define  P5020_DCR0(base)           (CAST(VUINT32 *)(base + 0xe0f1c))
#define  P5020_DCR1(base)           (CAST(VUINT32 *)(base + 0xe0f20))

/* Offsets for DDR registers */

#define DDRBASE                     (CCSBAR + 0x8000)
#define DDRBASE2                    (CCSBAR + 0x9000)
#define CS0_BNDS                    0x000
#define CS1_BNDS                    0x008
#define CS2_BNDS                    0x010
#define CS3_BNDS                    0x018
#define CS0_CONFIG                  0x080
#define CS1_CONFIG                  0x084
#define CS2_CONFIG                  0x088
#define CS3_CONFIG                  0x08C
#define TIMING_CFG_3                0x100
#define TIMING_CFG_0                0x104
#define TIMING_CFG_1                0x108
#define TIMING_CFG_2                0x10C
#define DDR_SDRAM_CFG               0x110
#define DDR_SDRAM_CFG_2             0x114
#define DDR_SDRAM_MODE_CFG          0x118
#define DDR_SDRAM_MODE_CFG_2        0x11c
#define DDR_SDRAM_MD_CNTL           0x120
#define DDR_SDRAM_INTERVAL          0x124
#define DDR_DATA_INIT               0x128
#define DDR_SDRAM_CLK_CTRL          0x130
#define TIMING_CFG_4                0x160
#define TIMING_CFG_5                0x164
#define DDR_ZQ_CNTL                 0x170
#define DDR_WRLVL_CNTL              0x174
#define DDR_SR_CNTR                 0x17C
#define DDR_WRLVL_CNTL_2            0x190
#define DDR_WRLVL_CNTL_3            0x194
#define DDRCDR_1                    0xB28
#define DDRCDR_2                    0xB2C

#define DDR_DATA_ERR_INJECT_HI      0xe00
#define DDR_DATA_ERR_INJECT_LO      0xe04
#define DDR_ECC_ERR_INJECT          0xe08
#define DDR_CAPTURE_DATA_HI         0xe20
#define DDR_CAPTURE_DATA_LO         0xe24
#define DDR_CAPTURE_ECC             0xe28
#define DDR_ERR_DETECT              0xe40
#define DDR_ERR_DISABLE             0xe44
#define DDR_ERR_INT_EN              0xe48
#define DDR_CAPTURE_ATTRIBUTES      0xe4c
#define DDR_CAPTURE_ADDRESS         0xe50
#define DDR_ERR_SBE                 0xe58
#define DDR_IO_OVCR                 0x000E0F24

/* EEPROM */

#define EEPROM_BASE                 0xF0000000
#define EEPROM_SIZE                 0x00004000 /* 16KB TLB entry */
#define EEPROM_SIZE_MASK            0xFFFFC000

#define LBC_CS3_LOCAL_ADRS          0xEFDF0000
#define LBC_CS3_LOCAL_SIZE_MASK     0xfff00000
#define LBC_CS3_SIZE                0x00001000

#define PX_ID(base)                 (CAST(VUINT8*)(base + 0x00))
#define PX_ARCH(base)               (CAST(VUINT8*)(base + 0x01))
#define PX_SCVER(base)              (CAST(VUINT8*)(base + 0x02))
#define PX_CSR(base)                (CAST(VUINT8*)(base + 0x03))
#define PX_RST(base)                (CAST(VUINT8*)(base + 0x04))

/* offset 0x05 is reserved */

#define PX_AUX(base)                (CAST(VUINT8*)(base + 0x06))
#define PX_SPD(base)                (CAST(VUINT8*)(base + 0x07))
#define PX_BRDCFG0(base)            (CAST(VUINT8*)(base + 0x08))
#define PX_BRDCFG1(base)            (CAST(VUINT8*)(base + 0x09))
#define PX_ADDR(base)               (CAST(VUINT8*)(base + 0x0A))
#define PX_BRDCFG2(base)            (CAST(VUINT8*)(base + 0x0B))

/* offsets 0x0B-0x0C are reserved */

#define PX_DATA(base)               (CAST(VUINT8*)(base + 0x0D))
#define PX_LED(base)                (CAST(VUINT8*)(base + 0x0E))

/* offset 0x0F is reserved */

#define PX_VCTL(base)               (CAST(VUINT8*)(base + 0x10))
#define PX_VSTAT(base)              (CAST(VUINT8*)(base + 0x11))
#define PX_VCFGEN0(base)            (CAST(VUINT8*)(base + 0x12))

/* offset 0x13 is reserved */

#define PX_OCMCSR(base)             (CAST(VUINT8*)(base + 0x14))
#define PX_OCMMSG(base)             (CAST(VUINT8*)(base + 0x15))
#define PX_GMDBG(base)              (CAST(VUINT8*)(base + 0x16))

/* offsets 0x17-0x18 are reserved */

#define PX_SCLK0(base)              (CAST(VUINT8*)(base + 0x19))
#define PX_SCLK1(base)              (CAST(VUINT8*)(base + 0x1A))
#define PX_SCLK2(base)              (CAST(VUINT8*)(base + 0x1B))
#define PX_WATCH(base)              (CAST(VUINT8*)(base + 0x1F))
#define PX_SW(base,n)               (CAST(VUINT8*)(base + 0x20 + ((n)*2)))
#define PX_EN(base,n)               (CAST(VUINT8*)(base + 0x21 + ((n)*2)))

#define PX_SYSCLK_67_MHZ            0
#define PX_SYSCLK_83_MHZ            1
#define PX_SYSCLK_100_MHZ           2
#define PX_SYSCLK_125_MHZ           3
#define PX_SYSCLK_133_MHZ           4
#define PX_SYSCLK_150_MHZ           5
#define PX_SYSCLK_160_MHZ           6
#define PX_SYSCLK_167_MHZ           7
#define PX_CFG1_EMI1_SEL_MASK       0x8f
#define PX_CFG1_EMI1_SEL_SHIFT      4
#define PX_CFG1_EMI1_SEL_EN         0x08
#define PX_CFG1_EMI2_SEL_MASK       0xF9
#define PX_CFG1_EMI2_SEL_SHIFT      1
#define PX_CFG2_MII_SEL_FPGA        0x20

/* PIC Base Address */

#define PIC_OFFSET                  0x40000
#define PCIBA                       (CCSBAR | PIC_OFFSET)

/* CPU Doze/Nap status and control registers - bits are 1 << CPU index */

#define P5020_CDOZSRL(base)         (CAST(VUINT32 *)(base + 0xE2004))
#define P5020_CDOZCRL(base)         (CAST(VUINT32 *)(base + 0xE200C))
#define P5020_CNAPSRL(base)         (CAST(VUINT32 *)(base + 0xE2014))
#define P5020_CNAPCRL(base)         (CAST(VUINT32 *)(base + 0xE201C))

/* Processor reset register */

#define P5020_EPICPRR(base)         (CAST(VUINT32 *)(base + 0x41098))

/* Feature Reporting Register (FRR) */

#define P5020_EPICFRR(base)         (CAST(VUINT32 *)(base + 0x41000))
#define CPU_NUM                     1 + ((*P5020_EPICFRR(CCSBAR) >> 8) & 0x1f)

/* Processor initialization register */

#define P5020_EPICPIR(base)         (CAST(VUINT32 *)(base + 0x41090))
#define P5020_EPICWHOAMI(base)      (CAST(VUINT32 *)((base) + 0x40090))

/*
 * CPU-private current task priority and end-of-interrupt registers
 * (used for CPU reset)
 */

#define P5020_EPICCTPRn(base,n)     (CAST(VUINT32 *)((base) + 0x60080 + (n)*0x1000))
#define P5020_EPICEOIn(base,n)      (CAST(VUINT32 *)((base) + 0x600b0 + (n)*0x1000))
#define EPIC_CTPR_MAX               15  /* maximum priority (mask) */
#define RESET_EOI_NUM               15  /* EOIs to send RM:31.5.1.3 */

#define P5020_PVR(base)             (CAST(VUINT32 *)((base) + 0xE00A0))
#define P5020_SVR(base)             (CAST(VUINT32 *)((base) + 0xE00A4))

#define P5020_BRR(base)             (CAST(VUINT32 *)((base) + 0xE00E4))

/* Core reset status register */

#define P5020_CRSTSR(base,n)        (CAST(VUINT32 *)((base) + 0xE0400 + (n)*4))
#define P5020_CRSTSR_RST_WRT        0x08000000
#define P5020_CRSTSR_RST_MPIC       0x00800000
#define P5020_CRSTSR_RST_CORE       0x00080000
#define P5020_CRSTSR_READY          0x00008000
#define P5020_CRSTSR_RST_HRST       0x00000002
#define P5020_CRSTSR_RST_PORST      0x00000001

/* timebase enable register */

#define P5020_RCWSR(base,n)        (CAST(VUINT32 *)((base) + 0xE0100 + (n)*4))
#define P5020_MEM_PLL_RAT_MSK       0x003E0000
#define P5020_MEM_PLL_RAT_SFT       17
#define P5020_MEM_PLL_RAT(base)     \
        ((*P5020_RCWSR(base,0) & P5020_MEM_PLL_RAT_MSK) >> P5020_MEM_PLL_RAT_SFT)

#define P5020_SYS_PLL_RAT_MSK       0x3E000000
#define P5020_SYS_PLL_RAT_SFT       25
#define P5020_SYS_PLL_RAT(base)     \
        ((*P5020_RCWSR(base,0) & P5020_SYS_PLL_RAT_MSK) >> P5020_SYS_PLL_RAT_SFT)

#define P5020_CCN_PLL_RAT_MSK(n)    (0x3E000000 >> (8 * n))
#define P5020_CCN_PLL_RAT_SFT(n)    (25 - (8 * n))
#define P5020_CCN_PLL_RAT(base,n)   \
        ((*P5020_RCWSR(base,2) & P5020_CCN_PLL_RAT_MSK(n)) >> P5020_CCN_PLL_RAT_SFT(n))

#define P5020_CN_PLL_SEL_MSK(n)     (0xF0000000 >> (4 * n))
#define P5020_CN_PLL_SEL_SFT(n)     (28 - (4 * n))
#define P5020_CN_PLL_SEL(base,n)    \
        ((*P5020_RCWSR(base,3) & P5020_CN_PLL_SEL_MSK(n)) >> P5020_CN_PLL_SEL_SFT(n))

#define P5020_SRDS_PRTCL(base)    ((*P5020_RCWSR(base,4) & 0xfc000000) >> 26)

/* Timebase enable register */

#define P5020_CTBENR(base)          (CAST(VUINT32 *)((base) + 0xE2084))

/* Branch Unit Control and Status Register (BUCSR) bit defines */

#define _PPC_BUCSR_FI               0x200       /* Invalidate branch cache */
#define _PPC_BUCSR_E                0x1         /* Enable branch prediction */

/* DMA registers base address and offsets */

#define DMA1_REGBASE                (CCSBAR + 0x100100)
#define DMA2_REGBASE                (CCSBAR + 0x101100)

#define DMA_CHAN_OFFSET             0x80
#define DMA_CHAN_NUM                4

/* I2C */

#define I2C_BUS1_BASE               (CCSBAR + 0x118000)
#define I2C_BUS2_BASE               (CCSBAR + 0x118100)
#define I2C_BUS3_BASE               (CCSBAR + 0x119000)
#define I2C_BUS4_BASE               (CCSBAR + 0x119100)
#define I2C_RESETWORD_LEN           0x60

/* SATA */

#define SATA0_BASE                  (CCSBAR + 0x220000)
#define SATA1_BASE                  (CCSBAR + 0x221000)

/* Base address defines */

#define GPIO_BASE                   (CCSBAR + 0x130000)
#define DTSEC0_BASE                 (CCSBAR + 0x4E0000)
#define DTSEC1_BASE                 (CCSBAR + 0x4E2000)
#define DTSEC2_BASE                 (CCSBAR + 0x4E4000)
#define DTSEC3_BASE                 (CCSBAR + 0x4E6000)
#define DTSEC4_BASE                 (CCSBAR + 0x4E8000)
#define DTSEC_MDIO_BASE             (CCSBAR + 0x4E1000)
#define TGEC_BASE                   (CCSBAR + 0x4F0000)
#define TGEC_MDIO_BASE              (CCSBAR + 0x4F1000)
#define PCI_BUS_BASE                (CCSBAR + 0x200000)
#define LAW_BASE                    (CCSBAR + 0xC00)
#define FMAN_BASE                   (CCSBAR + 0x400000)
#define QMAN_BASE                   (CCSBAR + 0x318000)
#define BMAN_BASE                   (CCSBAR + 0x31A000)
#define UART0_BASE                  (CCSBAR + 0x11c500)
#define UART1_BASE                  (CCSBAR + 0x11c600)
#define OPIC_TIMER0_BASE            (CCSBAR + 0x410F0)
#define OPIC_TIMER1_BASE            (CCSBAR + 0x420F0)

#define CPLD_BASE                   0xEFDF0000
#define CPLD_SIZE                   0x00010000
#define CPLD_SIZE_MASK              0xFFFF0000

#define EEPROM_BASE                 0xF0000000
#define EEPROM_SIZE                 0x00004000 /* 16KB TLB entry */
#define EEPROM_SIZE_MASK            0xFFFFC000
                                    
#define LCD_BASE_ADRS               0xF1000000
#define LCD_CNTRST_DWN_ADRS         LCD_BASE_ADRS
#define LCD_SIZE                    0x00010000
#define LCD_SIZE_MASK               0xFFFF0000

#define NVRAM_OR4_VALUE             0x0e65
#define NVRAM_BR4_VALUE             0x0801
#define LED_OR5_VALUE               NVRAM_OR4_VALUE
#define LED_BR5_VALUE               NVRAM_BR4_VALUE
#define LCD_OR6_VALUE               NVRAM_OR4_VALUE
#define LCD_BR6_VALUE               NVRAM_BR4_VALUE
#define TFFS_OR7_VALUE              0xFF000E65
#define TFFS_BR7_VALUE              0xF2001001
#define FCM_BR_BASE_MASK            0xFFFF0000

/* These address offsets are determined by the EPLD */

#define LCD_CNTRST_UP_ADRS          (LCD_BASE_ADRS + 0x1)
#define LCD_INSTR_ADRS              (LCD_BASE_ADRS + 0x2)
#define LCD_DATA_ADRS               (LCD_BASE_ADRS + 0x3)

/* USB */

#define USB1_BASE                   (CCSBAR + 0x210000)
#define USB2_BASE                   (CCSBAR + 0x211000)

/* EHCI registers */

#define EHCI_CAPLENGTH(base)        ((base) + 0x00100)
#define EHCI_BURSTSIZE(base)        ((base) + 0x00160)
#define EHCI_TXTTFILLTUNING(base)   ((base) + 0x00164)
#define EHCI_CONFIGFLAG(base)       ((base) + 0x00180)
#define EHCI_PORTSC(base)           ((base) + 0x00184)
#define EHCI_USBMODE(base)          ((base) + 0x001a8)
#define EHCI_SNOOP1(base)           ((base) + 0x00400)
#define EHCI_SNOOP2(base)           ((base) + 0x00404)
#define EHCI_AGE_CNT_THRESH(base)   ((base) + 0x00408)
#define EHCI_PRI_CTRL(base)         ((base) + 0x0040c)
#define EHCI_SI_CTRL(base)          ((base) + 0x00410)
#define EHCI_CONTROL(base)          ((base) + 0x00500)
#define EHCI_USBMODE_IDLE           0x00000000
#define EHCI_USBMODE_DEVICE         0x00000002
#define EHCI_USBMODE_HOST           0x00000003
#define EHCI_PORTSC_UTMI            0x00000000
#define EHCI_SNOOP_SIZE_4K          0x0000000B
#define EHCI_SNOOP_SIZE_8K          0x0000000C
#define EHCI_SNOOP_SIZE_16K         0x0000000D
#define EHCI_SNOOP_SIZE_32K         0x0000000E
#define EHCI_SNOOP_SIZE_64K         0x0000000F
#define EHCI_SNOOP_SIZE_128K        0x00000010
#define EHCI_SNOOP_SIZE_256K        0x00000011
#define EHCI_SNOOP_SIZE_512K        0x00000012
#define EHCI_SNOOP_SIZE_1M          0x00000013
#define EHCI_SNOOP_SIZE_2M          0x00000014
#define EHCI_SNOOP_SIZE_4M          0x00000015
#define EHCI_SNOOP_SIZE_8M          0x00000016
#define EHCI_SNOOP_SIZE_16M         0x00000017
#define EHCI_SNOOP_SIZE_32M         0x00000018
#define EHCI_SNOOP_SIZE_64M         0x00000019
#define EHCI_SNOOP_SIZE_128M        0x0000001A
#define EHCI_SNOOP_SIZE_256M        0x0000001B
#define EHCI_SNOOP_SIZE_512M        0x0000001C
#define EHCI_SNOOP_SIZE_1G          0x0000001D
#define EHCI_SNOOP_SIZE_2G          0x0000001E
#define EHCI_PRI_EN                 0x00000008
#define EHCI_CONTROL_UTMI_PHY_EN    0x00000200
#define EHCI_CONTROL_USB_EN         0x00000004

/* USB PHY */

#define USB1_PHY_BASE               (CCSBAR + 0x214000)
#define USB2_PHY_BASE               (CCSBAR + 0x214100)
#define USB_PHY_EOR(base)           ((base) + 0x18)
#define USB_PHY_EN_OVERRIDE         0x00000001

/* Debug control/status registers */

#define DCSR_RCW0                   0x20100
#define DCSR_RCW1                   0x20104
#define DCSR_RCW2                   0x20108
#define DCSR_RCW3                   0x2010C
#define DCSR_RCW4                   0x20110
#define DCSR_RCW5                   0x20114
#define DCSR_RCW6                   0x20118
#define DCSR_RCW7                   0x2011C

#define DCSR_READ(offset) \
    vxbRead32 (VXB_HANDLE_MEM, (UINT32 *)(offset + DCSR_LAW_BASE))

#define DCSR_WRITE(offset, val) \
    vxbWrite32 (VXB_HANDLE_MEM, (UINT32 *)(offset + DCSR_LAW_BASE), val)

/* BOARD SPECIFIC VALUES: */

#define FLASH_OR0_VALUE             0x35
#define LCRR_CLOCK_DIV              2
#define LAWBARL2_VALUE              ((DDR_SDRAM_LOCAL_ADRS2) >> (LAWBAR_ADRS_SHIFT))

#define CS0_BNDS_VALUE              0x0000003F
#define CS1_BNDS_VALUE              0x0040007F
#define CS2_BNDS_VALUE              0x00000000
#define CS3_BNDS_VALUE              0x00000000
#define CS0_CONFIG_VALUE            0x80014202
#define CS1_CONFIG_VALUE            0x80014202
#define CS2_CONFIG_VALUE            0x00000000
#define CS3_CONFIG_VALUE            0x00000000
#define CS0_BNDS_2DDR_VALUE         ((DDR_SDRAM_LOCAL_ADRS >> 8) | \
                                     (DDR_SDRAM_LOCAL_END >> 24) | \
                                     (DDR_SDRAM_LOCAL_ADRS2_H << 24) | \
                                     (DDR_SDRAM_LOCAL_ADRS2_H << 8))
#define CS1_BNDS_2DDR_VALUE         ((DDR_SDRAM_LOCAL_ADRS2 >> 8) | \
                                     (DDR_SDRAM_LOCAL_END2 >> 24) | \
                                     (DDR_SDRAM_LOCAL_ADRS2_H << 24) | \
                                     (DDR_SDRAM_LOCAL_ADRS2_H << 8))
#define CS2_BNDS_2DDR_VALUE         0x00000000
#define CS3_BNDS_2DDR_VALUE         0x00000000
#define CS0_CONFIG_2DDR_VALUE       0x80014202
#define CS1_CONFIG_2DDR_VALUE       0x80014202
#define CS2_CONFIG_2DDR_VALUE       0x00000000
#define CS3_CONFIG_2DDR_VALUE       0x00000000
#define DDR_SDRAM_CLK_CTRL_VALUE    0x02800000
#define DDR_DATA_INIT_VALUE         0x00000000
#define DDR_SDRAM_MODE_CFG_2_VALUE  0x00100000
#define DDR_SDRAM_MD_CNTL_VALUE     0x00000000
#define TIMING_CFG_3_VALUE          0x01041000
#define TIMING_CFG_0_VALUE          0x55550804
#define TIMING_CFG_1_VALUE          0x98912C47
#define TIMING_CFG_2_VALUE          0x0FB8E914
#define TIMING_CFG_4_VALUE          0x00440001
#define TIMING_CFG_5_VALUE          0x03402400
#define DDR_ZQ_CNTL_VALUE           0x89080600
#define DDR_WRLVL_CNTL_VALUE        0x8675C507
#define DDR_SDRAM_CFG_VALUE1        0x47000000
#define DDR_SDRAM_CFG_VALUE2        0xc7000000
#define DDR_SDRAM_CFG_2_VALUE1      0x24401040
#define DDR_SDRAM_CFG_2_VALUE2      0x24401050
#define DDR_SDRAM_MODE_CFG_VALUE    0x00421a41
#define DDR_SDRAM_INTERVAL_VALUE    0x12480100

/* TO MATCH THE P4080 to facilitate sharing romInit.s: */

/*
 * Errata: A-003474 (Internal DDR calibration circuit is not supported) 
 * This workaround is implemented in bootrom (romInit.s).
 */

#define FSL_ERRATA_DDR_A003474

/*
 * Errata: A-003999 (Running Floating Point instructions requires special 
 * initialization), This workaround is implemented in boortom(romInit.s) 
 * and vxWorks image(sysLib.c).
 */

#define FSL_ERRATA_FPU_A003999

#define MMU_RAM_ACCESS              MMU_CACHED  /* see romInit.s */ 
#define DDR_SDRAM_LAWAR_TGTIF       LAWAR_TGTIF_DDRSDRAM
#undef  DDR_SDRAM_LAWAR_TGTIF2      /* LAWAR_TGTIF_DDRSDRAM2 */

#define P4080_CTBENR                P5020_CTBENR
#define P4080_LAWAR0                P5020_LAWAR0
#define P4080_LAWAR1                P5020_LAWAR1
#define P4080_LAWAR3                P5020_LAWAR3
#define P4080_LAWAR8                P5020_LAWAR8
#define P4080_LAWARL8               P5020_LAWARL8
#define P4080_LAWBARL0              P5020_LAWBARL0
#define P4080_LAWBARL1              P5020_LAWBARL1
#define P4080_LAWBARH1              P5020_LAWBARH1
#define P4080_LAWBARL3              P5020_LAWBARL3
#define P4080_LAWBARL8              P5020_LAWBARL8
#define P4080_LAWBARH29             P5020_LAWBARH29
#define P4080_LAWBARL29             P5020_LAWBARL29
#define P4080_LAWAR29               P5020_LAWAR29
#define P4080_LAWBARH30             P5020_LAWBARH30
#define P4080_LAWBARL30             P5020_LAWBARL30
#define P4080_LAWAR30               P5020_LAWAR30
#define P4080_OR0                   P5020_OR0
#define P4080_BR0                   P5020_BR0
#define P4080_OR3                   P5020_OR3
#define P4080_BR3                   P5020_BR3
#define P4080_LBCR                  P5020_LBCR
#define P4080_LCRR                  P5020_LCRR
#define DDRBA                       DDRBASE
#define DDRBA2                      DDRBASE2

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* __INCwrSbcP5020h */
