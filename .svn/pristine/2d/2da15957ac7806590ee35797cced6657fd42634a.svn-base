/* p2041rdb.h - Freescale P2041RDB board header */

/*
 * Copyright (c) 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,05jul12,c_l  add related define for romInit.s (WIND00360538)
01b,05mar12,y_y  add I2C interrupt to support vxBus I2C driver.
01a,08feb12,d_l  Created from fsl_p4080_ds/p4080.h/01w 
*/

/*
This file contains I/O addresses and related constants for the
FreeScale P2041RDB board. 
*/

#ifndef __INCp2041rdbh
#define __INCp2041rdbh

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

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
#   define CAST(x) (x)
#endif  /* _ASMLANGUAGE */

/* Core set */

#define CPU_BIT(n)                      (1 << n)
#define MAX_CPUS                        4

/* Define Clock Speed and source  */

#define FREQ_10_MHZ             10000000
#define FREQ_33_MHZ             33333300
#define FREQ_40_MHZ             40000000
#define FREQ_50_MHZ             50000000
#define FREQ_66_MHZ             66666600
#define FREQ_75_MHZ             75000000
#define FREQ_83_MHZ             83333300
#define FREQ_90_MHZ             90000000
#define FREQ_100_MHZ            100000000
#define FREQ_111_MHZ            111000000
#define FREQ_125_MHZ            125000000
#define FREQ_133_MHZ            133333300
#define FREQ_166_MHZ            166666600
#define FREQ_266_MHZ            266666000
#define FREQ_400_MHZ            400000000
#define FREQ_333_MHZ            333333000
#define FREQ_533_MHZ            533333000
#define FREQ_500_MHZ            500000000
#define FREQ_600_MHZ            600000000
#define FREQ_750_MHZ            750000000

#define OSCILLATOR_FREQ         FREQ_100_MHZ

#define PLAT_RATIO_800_MHZ      20
#define PLAT_RATIO_533_MHZ      16
#define PLAT_RATIO_400_MHZ      12
#define PLAT_RATIO_333_MHZ      8 

#define DEFAULT_SYSCLKFREQ      (FREQ_750_MHZ / 2)

/* Use to force freq used with DUART/Timers etc */

#undef FORCE_DEFAULT_FREQ

/* Frequency applied to the  "RTC" pin */

#define HPCN_RTC_FREQ           14318000

/* Base Address of Memory Mapped Registers */

#define CCSBAR                  0xFE000000
#define CCSBAR_RESET            0xFE000000
#define CCSBAR_COMMIT           0x80000000
#define CCSRBARH_OFFSET         0x0
#define CCSRBARL_OFFSET         0x4
#define CCSRAR_OFFSET           0x8

/* Base Address of queue manager access window */

#define QMAN_LAW_BASE           0xF4200000

/* Base Address of buffer manager access window */

#define BMAN_LAW_BASE           0xF4000000

#define DCSR_LAW_BASE           0xFD000000

/*
 * "magic numbers" used to validate the rebootVec.  The definitions
 * are arbitrary, but should not be "common" values like all zeros,
 * all ones, 0xeeeeeeee, 0x55aa55aa, 0xdeadbeef, etc.
 */

#define MP_MAGIC_RBV            0xfeedfeed
#define MP_MAGIC_CACHE_FLUSHED  0xD07ED07E

#define ETHERNET_MAC_HANDLER

#define WR_ENET0                0x00  /* WR specific portion of MAC (MSB->LSB) */
#define WR_ENET1                0x04
#define WR_ENET2                0x9F

#define CUST_ENET3_0            0xA0  /* Customer portion of MAC address */
#define CUST_ENET3_1            0xA1
#define CUST_ENET3_2            0xA2
#define CUST_ENET3_3            0xA3
#define CUST_ENET3_4            0xA4
#define CUST_ENET3_5            0xA5
#define CUST_ENET4              0xAA
#define CUST_ENET5              0xA0

/* Two types ethernet controllers on P2041RDB, DTSEC and TGEC  */

#define MAX_MAC_DEVS            2
#define DTSEC_NUM               5
#define TGEC_NUM                1

/* default mac address */

#define MAX_MAC_ADRS            (DTSEC_NUM + TGEC_NUM)
#define MAC_ADRS_LEN            6
#define NV_MAC_ADRS_OFFSET      0x200

/* EPIC defines */

#undef  EPIC_VEC_IN_IRQ0
#define EPIC_VEC_IN_IRQ0        16

#define EPIC_DMA1_CHAN0_INT_VEC (12 + EPIC_VEC_IN_IRQ0)
#define EPIC_DMA1_CHAN1_INT_VEC (13 + EPIC_VEC_IN_IRQ0)
#define EPIC_DMA1_CHAN2_INT_VEC (14 + EPIC_VEC_IN_IRQ0)
#define EPIC_DMA1_CHAN3_INT_VEC (15 + EPIC_VEC_IN_IRQ0)

#define EPIC_DMA2_CHAN0_INT_VEC (16 + EPIC_VEC_IN_IRQ0)
#define EPIC_DMA2_CHAN1_INT_VEC (17 + EPIC_VEC_IN_IRQ0)
#define EPIC_DMA2_CHAN2_INT_VEC (18 + EPIC_VEC_IN_IRQ0)
#define EPIC_DMA2_CHAN3_INT_VEC (19 + EPIC_VEC_IN_IRQ0)

#undef EPIC_DUART_INT_VEC
#define EPIC_DUART_INT_VEC      (20 + EPIC_VEC_IN_IRQ0)
#undef EPIC_DUART2_INT_VEC
#define EPIC_DUART2_INT_VEC     (21 + EPIC_VEC_IN_IRQ0)
#undef  EPIC_I2C_INT_VEC
#define EPIC_I2C_INT_VEC        (22 + EPIC_VEC_IN_IRQ0)
#undef  EPIC_I2C2_INT_VEC
#define EPIC_I2C2_INT_VEC       (23 + EPIC_VEC_IN_IRQ0)
#define EPIC_SDHC_INT_VEC       (32 + EPIC_VEC_IN_IRQ0)
#undef  EPIC_SATA1_INT_VEC
#define EPIC_SATA1_INT_VEC      (52 + EPIC_VEC_IN_IRQ0)
#undef  EPIC_SATA2_INT_VEC
#define EPIC_SATA2_INT_VEC      (53 + EPIC_VEC_IN_IRQ0)

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

/*  p2041rdb supports three PCIe controllers */

/*

CPU Addr                                  PCI Addr ( PCI1 or PCI2)
PCI_LOCAL_MEM_BUS        ------------------------- PCI_MSTR_MEM_BUS
                         -                       - 
                         -                       -
PCI_LOCAL_MEM_BUS +      ------------------------- PCI_MSTR_MEM_BUS +
PCI_LOCAL_MEM_SIZE       -                       - PCI_MSTR_MEM_SIZE
                         -                       - 
                         -                       ---- PCI Bridge (for PCI1 only)
                         -                       -     configuration regs
                         -                       - 
CPU_PCI_MEM_ADRS (PCIEX1) ------------------------ PCI_MEM_ADRS
CPU_PCI_MEM_ADRS2(PCIEX2) -                      - PCI_MEM_ADRS2
CPU_PCI_MEM_ADRS3(PCIEX3) ------------------------ PCI_MEM_ADRS3
                         -                       - 
CPU_PCI_MEMIO_ADRS       ------------------------- PCI_MEMIO_ADRS
CPU_PCI_MEMIO_ADRS2      -                       - PCI_MEMIO_ADRS2
CPU_PCI_MEMIO_ADRS3      -                       - PCI_MEMIO_ADRS3
                         -                       - 
CPU_PCI_IO_ADRS (PCIEX1) ------------------------- PCI_IO_ADRS
CPU_PCI_IO_ADRS2(PCIEX2) -                       - PCI_IO_ADRS2
CPU_PCI_IO_ADRS3(PCIEX3) -                       - PCI_IO_ADRS3
                         -                       -
CPU_PCI_IO_ADRS (PCIEX1)+------------------------- PCI_IO_ADRS +
CPU_PCI_IO_ADRS2(PCIEX2)+                          PCI_IO_ADRS2 +
CPU_PCI_IO_ADRS2(PCIEX3)                           PCI_IO_ADRS3
CPU_PCI_IO_SIZE          -                       - PCI_IO_SIZE
                         -                       -
*/

/*
 * PCI based addresses
 * Note: the LAW_BASE values have been chosen to coincide
 * with those used by U-Boot on the P2041RDB board.
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

#define PCIEX2_LAW_BASE             0xA0000000       /* PCI LAW window */
#define PCIEX2_LAW_SIZE             0x10000000       /* PCI LAW size   */

#define PCIEX2_MEM_SIZE             0x04000000        /* 64 MB */
#define PCIEX2_MEMIO_SIZE           0x04000000        /* 64 MB */
#define PCIEX2_IO_SIZE              0x04000000        /* 64 MB */
#define PCIEX2_IO32_SIZE            0x04000000        /* 64 MB */

#define PCIEX2_MEM_ADRS             (PCIEX2_LAW_BASE)                     
#define PCIEX2_MEMIO_ADRS           (PCIEX2_MEM_ADRS   + PCIEX2_MEM_SIZE)    
#define PCIEX2_IO_ADRS              (PCIEX2_MEMIO_ADRS + PCIEX2_MEMIO_SIZE)  
#define PCIEX2_IO32_ADRS            (PCIEX2_IO_ADRS    + PCIEX2_IO_SIZE)  

#define PCIEX3_LAW_BASE             0xC0000000       /* PCI LAW window */
#define PCIEX3_LAW_SIZE             0x10000000       /* PCI LAW size   */

#define PCIEX3_MEM_SIZE             0x04000000        /* 64 MB */
#define PCIEX3_MEMIO_SIZE           0x04000000        /* 64 MB */
#define PCIEX3_IO_SIZE              0x04000000        /* 64 MB */
#define PCIEX3_IO32_SIZE            0x04000000        /* 64 MB */

#define PCIEX3_MEM_ADRS             (PCIEX3_LAW_BASE)                     
#define PCIEX3_MEMIO_ADRS           (PCIEX3_MEM_ADRS   + PCIEX3_MEM_SIZE)    
#define PCIEX3_IO_ADRS              (PCIEX3_MEMIO_ADRS + PCIEX3_MEMIO_SIZE)  
#define PCIEX3_IO32_ADRS            (PCIEX3_IO_ADRS    + PCIEX3_IO_SIZE)  

#define PCI_MMU_TLB_SZ              _MMU_TLB_SZ_256M

#define PCI_AUTO_CONFIG_ADRS        0x4c00

#define PPCACR_PRKM_MASK            0XF0
#define PCI_REQUEST_LEVEL           0x3

#define CLASS_OFFSET                0xB
#define CLASS_WIDTH                 0x1
#define BRIDGE_CLASS_TYPE           0x6

#define PCICMD_ADRS                 (PCI_CFG_BASE + 0x04)  /* PCI cmd reg */
#define PCICMD_VAL                  0x00000006             /* PCI COMMAND Default value */
#define PCISTAT_ADRS                (PCI_CFG_BASE + 0x06)  /* PCI status reg */

            
#define NUM_PCIEX1_SLOTS            0x4
#define NUM_PCIEX2_SLOTS            0x4
#define NUM_PCIEX3_SLOTS            0x4
#define NUM_PCI_SLOTS               2

#define EPIC_VEC_PCIE1_INTA         40
#define EPIC_VEC_PCIE2_INTA         41
#define EPIC_VEC_PCIE3_INTA         42

#define PCIEX1_XINT1_LVL            (EPIC_VEC_PCIE1_INTA)
#define PCIEX1_XINT2_LVL            (EPIC_VEC_EXT_IRQ0 + 1)
#define PCIEX1_XINT3_LVL            (EPIC_VEC_EXT_IRQ0 + 2)
#define PCIEX1_XINT4_LVL            (EPIC_VEC_EXT_IRQ0 + 3)

#define PCIEX2_XINT1_LVL            (EPIC_VEC_PCIE2_INTA)
#define PCIEX2_XINT2_LVL            (EPIC_VEC_EXT_IRQ0 + 5)
#define PCIEX2_XINT3_LVL            (EPIC_VEC_EXT_IRQ0 + 6)
#define PCIEX2_XINT4_LVL            (EPIC_VEC_EXT_IRQ0 + 7)

#define PCIEX3_XINT1_LVL            (EPIC_VEC_PCIE3_INTA)
#define PCIEX3_XINT2_LVL            (EPIC_VEC_EXT_IRQ0 + 9)
#define PCIEX3_XINT3_LVL            (EPIC_VEC_EXT_IRQ0 + 10)
#define PCIEX3_XINT4_LVL            (EPIC_VEC_EXT_IRQ0 + 11)

#define PCI_SLOT1_DEVNO             0x11            /* PCI SLOT 1 Device no */

#define PCI_XINT1_LVL               0x0             /* PCI XINT1 routed to IRQ0  */
#define PCI_XINT2_LVL               0x1             /* PCI XINT2 routed to IRQ0 */
#define PCI_XINT3_LVL               0x2             /* PCI XINT3 routed to IRQ0 */
#define PCI_XINT4_LVL               0x3             /* PCI XINT3 routed to IRQ0 */

#define PCI_LAT_TIMER               0x40            /* latency timer value, 64 PCI clocks */

#define PCI_DEV_ID_85XX             0x000C1057      /* Id for Freescale 8555/8541 PCI 1*/

#define PCI_ARCADIA_BRIDGE_DEV_ID   0x051310E3      /* DEV ID for PCI bridge in Arcadia */

#define PCI1_DEV_ID                 0x826010E3
#define PCI2_DEV_ID                 0x826110E3
#define PCI3_DEV_ID                 0x826210E3
#define PCI_DEV_ID_82XX             0x00031057      /* Id for MPC8266ADS-PCI board - Rev1 */

#define PCI_ID_I82559               0x12298086      /* Id for Intel 82559 */
#define PCI_ID_I82559ER             0x12098086      /* Id for Intel 82559 ER */

/* Boot space translation */

#define P2041_BSTRH(base)           (CAST(VUINT32 *)((base) + 0x20))
#define P2041_BSTRL(base)           (CAST(VUINT32 *)((base) + 0x24))
#define P2041_BSTAR(base)           (CAST(VUINT32 *)((base) + 0x28))

#define BSTAR_EN                    0x80000000
#define BSTAR_4KB                   0x0000000b

/* Local Access Windows Regster Offsets from CCSBAR */

/* Used for DDR SDRAM */

#define  P2041_LAWBARH0(base)       (CAST(VUINT32 *)((base) + 0xc00))
#define  P2041_LAWBARL0(base)       (CAST(VUINT32 *)((base) + 0xc04))
#define  P2041_LAWAR0(base)         (CAST(VUINT32 *)((base) + 0xc08))

/* Not used */

#define  P2041_LAWBARH1(base)       (CAST(VUINT32 *)((base) + 0xc10))
#define  P2041_LAWBARL1(base)       (CAST(VUINT32 *)((base) + 0xc14))
#define  P2041_LAWAR1(base)         (CAST(VUINT32 *)((base) + 0xc18))

/* Used for LBC PIXIS   */

#define  P2041_LAWBARH2(base)       (CAST(VUINT32 *)((base) + 0xc20))
#define  P2041_LAWBARL2(base)       (CAST(VUINT32 *)((base) + 0xc24))
#define  P2041_LAWAR2(base)         (CAST(VUINT32 *)((base) + 0xc28))

/* Not used */

#define  P2041_LAWBARH3(base)       (CAST(VUINT32 *)((base) + 0xc30))
#define  P2041_LAWBARL3(base)       (CAST(VUINT32 *)((base) + 0xc34))
#define  P2041_LAWAR3(base)         (CAST(VUINT32 *)((base) + 0xc38))

/* Used for PCIEX1 */

#define  P2041_LAWBARH4(base)       (CAST(VUINT32 *)((base) + 0xc40))
#define  P2041_LAWBARL4(base)       (CAST(VUINT32 *)((base) + 0xc44))
#define  P2041_LAWAR4(base)         (CAST(VUINT32 *)((base) + 0xc48))

/* Used for PCIEX2*/

#define  P2041_LAWBARH5(base)       (CAST(VUINT32 *)((base) + 0xc50))
#define  P2041_LAWBARL5(base)       (CAST(VUINT32 *)((base) + 0xc54))
#define  P2041_LAWAR5(base)         (CAST(VUINT32 *)((base) + 0xc58))

/* Used for PCIEX3 */

#define  P2041_LAWBARH6(base)       (CAST(VUINT32 *)((base) + 0xc60))
#define  P2041_LAWBARL6(base)       (CAST(VUINT32 *)((base) + 0xc64))
#define  P2041_LAWAR6(base)         (CAST(VUINT32 *)((base) + 0xc68))

/* Used for PCI */

#define  P2041_LAWBARH7(base)       (CAST(VUINT32 *)((base) + 0xc70))
#define  P2041_LAWBARL7(base)       (CAST(VUINT32 *)((base) + 0xc74))
#define  P2041_LAWAR7(base)         (CAST(VUINT32 *)((base) + 0xc78))

/* Unused */

#define  P2041_LAWBARH8(base)       (CAST(VUINT32 *)((base) + 0xc80))
#define  P2041_LAWBARL8(base)       (CAST(VUINT32 *)((base) + 0xc84))
#define  P2041_LAWAR8(base)         (CAST(VUINT32 *)((base) + 0xc88))

/* Used for DCSR registers  */

#define  P2041_LAWBARH29(base)      (CAST(VUINT32 *)((base) + 0xDD0))
#define  P2041_LAWBARL29(base)      (CAST(VUINT32 *)((base) + 0xDD4))
#define  P2041_LAWAR29(base)        (CAST(VUINT32 *)((base) + 0xDD8))

#define  P2041_LAWBARH30(base)      (CAST(VUINT32 *)((base) + 0xDE0))
#define  P2041_LAWBARL30(base)      (CAST(VUINT32 *)((base) + 0xDE4))
#define  P2041_LAWAR30(base)        (CAST(VUINT32 *)((base) + 0xDE8))

#define  P2041_LAWBARH31(base)      (CAST(VUINT32 *)((base) + 0xDF0))
#define  P2041_LAWBARL31(base)      (CAST(VUINT32 *)((base) + 0xDF4))
#define  P2041_LAWAR31(base)        (CAST(VUINT32 *)((base) + 0xDF8))

#define  LAWBAR_ADRS_SHIFT          0
#define  LAWAR_ENABLE               0x80000000
#define  LAWAR_TGTIF_PCIEX3         0x00000000
#define  LAWAR_TGTIF_PCIEX2         0x00100000
#define  LAWAR_TGTIF_PCIEX1         0x00200000
#define  LAWAR_TGTIF_RAPIDIO        0x00800000
#define  LAWAR_TGTIF_RAPIDIO2       0x00900000
#define  LAWAR_TGTIF_DDRSDRAM       0x01000000
#define  LAWAR_TGTIF_DDRSDRAM2      0x01100000
#define  LAWAR_TGTIF_MEM_INTRL      0x01400000
#define  LAWAR_TGTIF_BMAN           0x01800000
#define  LAWAR_TGTIF_QMAN           0x01900000
#define  LAWAR_TGTIF_DCSR           0x01D00000
#define  LAWAR_TGTIF_LBC            0x01F00000
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

/* Local Bus Controller (LBC) Registers - unmentioned bits are reserved */

#define  LBC_BASE                   0x124000
#define  P2041_BR0(base)            (CAST(VUINT32 *)((base) + LBC_BASE + 0x000))
#define  P2041_OR0(base)            (CAST(VUINT32 *)((base) + LBC_BASE + 0x004))
#define  P2041_BR1(base)            (CAST(VUINT32 *)((base) + LBC_BASE + 0x008))
#define  P2041_OR1(base)            (CAST(VUINT32 *)((base) + LBC_BASE + 0x00c))
#define  P2041_BR2(base)            (CAST(VUINT32 *)((base) + LBC_BASE + 0x010))
#define  P2041_OR2(base)            (CAST(VUINT32 *)((base) + LBC_BASE + 0x014))
#define  P2041_BR3(base)            (CAST(VUINT32 *)((base) + LBC_BASE + 0x018))
#define  P2041_OR3(base)            (CAST(VUINT32 *)((base) + LBC_BASE + 0x01c))
#define  P2041_BR4(base)            (CAST(VUINT32 *)((base) + LBC_BASE + 0x020))
#define  P2041_OR4(base)            (CAST(VUINT32 *)((base) + LBC_BASE + 0x024))
#define  P2041_BR5(base)            (CAST(VUINT32 *)((base) + LBC_BASE + 0x028))
#define  P2041_OR5(base)            (CAST(VUINT32 *)((base) + LBC_BASE + 0x02C))
#define  P2041_BR6(base)            (CAST(VUINT32 *)((base) + LBC_BASE + 0x030))
#define  P2041_OR6(base)            (CAST(VUINT32 *)((base) + LBC_BASE + 0x034))
#define  P2041_BR7(base)            (CAST(VUINT32 *)((base) + LBC_BASE + 0x038))
#define  P2041_OR7(base)            (CAST(VUINT32 *)((base) + LBC_BASE + 0x03C))

#define  P2041_MAR(base)            (CAST(VUINT32 *)((base) + LBC_BASE + 0x068))
#define  P2041_MAMR(base)           (CAST(VUINT32 *)((base) + LBC_BASE + 0x070))
#define  P2041_MBMR(base)           (CAST(VUINT32 *)((base) + LBC_BASE + 0x074))
#define  P2041_MCMR(base)           (CAST(VUINT32 *)((base) + LBC_BASE + 0x078))
#define  P2041_MRTPR(base)          (CAST(VUINT32 *)((base) + LBC_BASE + 0x084))
#define  MRTPR_PTP_MASK             0xff000000
#define  MRTPR_PTP_WRITE(x)         ((x << 24) & MRTPR_PTP_MASK)
#define  P2041_MDR(base)            (CAST(VUINT32 *)((base) + LBC_BASE + 0x088))

#define  P2041_LTESR(base)          (CAST(VUINT32 *)((base) + LBC_BASE + 0x0B0))
#define  P2041_LTEDR(base)          (CAST(VUINT32 *)((base) + LBC_BASE + 0x0B4))
#define  P2041_LTEIR(base)          (CAST(VUINT32 *)((base) + LBC_BASE + 0x0B8))
#define  P2041_LTEATR(base)         (CAST(VUINT32 *)((base) + LBC_BASE + 0x0BC))
#define  P2041_LTEAR(base)          (CAST(VUINT32 *)((base) + LBC_BASE + 0x0C0))

/* LBC Clock Configuration */

#define  P2041_LBCR(base)           (CAST(VUINT32 *)((base) + LBC_BASE + 0x0D0))
#define  P2041_LCRR(base)           (CAST(VUINT32 *)((base) + LBC_BASE + 0x0D4))

/* LBC Flash Registers */

#define  P2041_FMR(base)            (CAST(VUINT32 *)((base) + LBC_BASE + 0x0E0))
#define  P2041_FIR(base)            (CAST(VUINT32 *)((base) + LBC_BASE + 0x0E4))
#define  P2041_FCR(base)            (CAST(VUINT32 *)((base) + LBC_BASE + 0x0E8))
#define  P2041_FBAR(base)           (CAST(VUINT32 *)((base) + LBC_BASE + 0x0EC))
#define  P2041_FPAR(base)           (CAST(VUINT32 *)((base) + LBC_BASE + 0x0F0))
#define  P2041_FBCR(base)           (CAST(VUINT32 *)((base) + LBC_BASE + 0x0F4))

#define  P2041_DCR0(base)           (CAST(VUINT32 *)((base) + 0xe0f1c))
#define  P2041_DCR1(base)           (CAST(VUINT32 *)((base) + 0xe0f20))

/* ECM Registers */

#define ECM_OFFSET                  0x1000
#define ECMBA                       (CCSBAR | ECM_OFFSET)

#define EEB_ABCR(base)              (CAST(VUINT32 *)((base) + ECM_OFFSET + 0x0))
#define EEB_DBCR(base)              (CAST(VUINT32 *)((base) + ECM_OFFSET + 0x8))
#define EEB_PCR(base)               (CAST(VUINT32 *)((base) + ECM_OFFSET + 0x10))
#define EEB_REV1(base)              (CAST(VUINT32 *)((base) + ECM_OFFSET + 0xBF8))
#define EEB_REV2(base)              (CAST(VUINT32 *)((base) + ECM_OFFSET + 0xBFC))
#define EEB_EDR(base)               (CAST(VUINT32 *)((base) + ECM_OFFSET + 0xE00))
#define EEB_EER(base)               (CAST(VUINT32 *)((base) + ECM_OFFSET + 0xE08))
#define EEB_EATR(base)              (CAST(VUINT32 *)((base) + ECM_OFFSET + 0xE0C))
#define EEB_ELADR(base)             (CAST(VUINT32 *)((base) + ECM_OFFSET + 0xE10))
#define EEB_EHADR(base)             (CAST(VUINT32 *)((base) + ECM_OFFSET + 0xE14))

/* Offsets for DDR registers */

#define DDR_OFFSET                  0x8000
#define DDRBA                       (CCSBAR | DDR_OFFSET)
#define DDR_OFFSET2                 0x9000
#define DDRBA2                      (CCSBAR | DDR_OFFSET2)

#define CS0_BNDS_VALUE              ( (DDR_SDRAM_LOCAL_ADRS >> 8) | \
                                     ((DDR_SDRAM_LOCAL_END >> 24) & \
                                       0x000000FF))

#define CS1_BNDS_VALUE              0x00000000
#define CS2_BNDS_VALUE              0x00000000
#define CS3_BNDS_VALUE              0x00000000
#define CS0_CONFIG_VALUE            0x80044302
#define CS1_CONFIG_VALUE            0x80004302
#define CS2_CONFIG_VALUE            0x00000000
#define CS3_CONFIG_VALUE            0x00000000

#define CS0_BNDS_2DDR_VALUE         ((DDR_SDRAM_LOCAL_ADRS2 >> 8)    | \
                                     (DDR_SDRAM_LOCAL_END2 >> 24)    | \
                                     (DDR_SDRAM_LOCAL_ADRS2_H << 24) | \
                                     (DDR_SDRAM_LOCAL_ADRS2_H << 8))

#define CS1_BNDS_2DDR_VALUE         0x00000000
#define CS2_BNDS_2DDR_VALUE         0x00000000
#define CS3_BNDS_2DDR_VALUE         0x00000000
#define CS0_CONFIG_2DDR_VALUE       0x80014202
#define CS1_CONFIG_2DDR_VALUE       0x00000000
#define CS2_CONFIG_2DDR_VALUE       0x00000000
#define CS3_CONFIG_2DDR_VALUE       0x00000000


#define DDR_SDRAM_CLK_CTRL_VALUE    0x02800000
#define DDR_DATA_INIT_VALUE         0x00000000
#define DDR_SDRAM_MODE_CFG_2_VALUE  0x00100000
#define DDR_SDRAM_MD_CNTL_VALUE     0x00000000
#define TIMING_CFG_3_VALUE          0x01061000
#define TIMING_CFG_0_VALUE          0x50110104
#define TIMING_CFG_1_VALUE          0x98913a45
#define TIMING_CFG_2_VALUE          0x0fb8a8d4
#define TIMING_CFG_4_VALUE          0x00000001
#define TIMING_CFG_5_VALUE          0x03401400
#define DDR_ZQ_CNTL_VALUE           0x89080600
#define DDR_WRLVL_CNTL_VALUE        0x8675f607
#ifdef INCLUDE_DDR_ECC
#define DDR_SDRAM_CFG_VALUE1        0x67044000
#define DDR_SDRAM_CFG_VALUE2        0xe7044000
#else /* INCLUDE_DDR_ECC */
#define DDR_SDRAM_CFG_VALUE1        0x47044000
#define DDR_SDRAM_CFG_VALUE2        0xc7044000
#endif /* INCLUDE_DDR_ECC */
#define DDR_SDRAM_CFG_2_VALUE1      0x24401000
#define DDR_SDRAM_CFG_2_VALUE2      0x24401010
#define DDR_SDRAM_MODE_CFG_VALUE    0x00441a50
#define DDR_SDRAM_INTERVAL_VALUE    0x14500100

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

/* CPLD Registers - see P2041RDB board CPLD Specification V2.2.doc */

#define CPLD_BASE                   0xEFDF0000
#define CPLD_SIZE                   0x00001000

#define CPLD_VER(base)              (CAST(VUINT8*)(base + 0x00))
#define CPLD_VERSUB(base)           (CAST(VUINT8*)(base + 0x01))
#define CPLD_PCBAVER(base)          (CAST(VUINT8*)(base + 0x02))
#define CPLD_OVSYSRST(base)         (CAST(VUINT8*)(base + 0x03))
/* offset 0x04 is reserved */
#define CPLD_SWON(base)             (CAST(VUINT8*)(base + 0x05))
#define CPLD_CFGRCWSRC(base)        (CAST(VUINT8*)(base + 0x06))
#define CPLD_SWSTRB(base)           (CAST(VUINT8*)(base + 0x07))
#define CPLD_AURORAJTAGSEL(base)    (CAST(VUINT8*)(base + 0x08))
#define CPLD_SDBANK1CLK(base)       (CAST(VUINT8*)(base + 0x09))
#define CPLD_SDBANK2CLK(base)       (CAST(VUINT8*)(base + 0x0A))
#define CPLD_VBANK(base)            (CAST(VUINT8*)(base + 0x0B))
#define CPLD_PCIESEL(base)          (CAST(VUINT8*)(base + 0x0C))
#define CPLD_SW2STATUS(base)        (CAST(VUINT8*)(base + 0x0D))
#define CPLD_GLOBALRST(base)        (CAST(VUINT8*)(base + 0x0E))
#define CPLD_SWSYSCLK(base)         (CAST(VUINT8*)(base + 0x0F))

#define CPLD_SYSCLK_66_MHZ          0
#define CPLD_SYSCLK_83_MHZ          1
#define CPLD_SYSCLK_100_MHZ         2

/* PIC Base Address */

#define PIC_OFFSET                  0x40000
#define PCIBA                       (CCSBAR | PIC_OFFSET)

/* Global Function Registers */
 
#define P2041_DEVDISR1(base)        (CAST(VUINT32 *)((base) + 0xE0070))
#define P2041_DEVDISR1_PEX1         0x80000000
#define P2041_DEVDISR1_PEX2         0x40000000
#define P2041_DEVDISR1_PEX3         0x20000000
#define P2041_DEVDISR1_ERMU         0x08000000
#define P2041_DEVDISR1_SRIO1        0x04000000
#define P2041_DEVDISR1_SRIO2        0x02000000
#define P2041_DEVDISR1_DMA1         0x00400000
#define P2041_DEVDISR1_DMA2         0x00200000
#define P2041_DEVDISR1_DDR1         0x00100000
#define P2041_DEVDISR1_DDR2         0x00080000
#define P2041_DEVDISR1_DBG          0x00010000
#define P2041_DEVDISR1_NAL          0x00008000
#define P2041_DEVDISR1_ELBC         0x00001000
#define P2041_DEVDISR1_USB1         0x00000800
#define P2041_DEVDISR1_USB2         0x00000400
#define P2041_DEVDISR1_ESDHC        0x00000100
#define P2041_DEVDISR1_GPIO         0x00000080
#define P2041_DEVDISR1_ESPI         0x00000040
#define P2041_DEVDISR1_I2C1         0x00000020
#define P2041_DEVDISR1_I2C2         0x00000010
#define P2041_DEVDISR1_DUART1       0x00000002
#define P2041_DEVDISR1_DUART2       0x00000001
 
#define P2041_DEVDISR2(base)        (CAST(VUINT32 *)((base) + 0xE0074))
#define P2041_DEVDISR2_PME          0x80000000
#define P2041_DEVDISR2_SEC          0x40000000
#define P2041_DEVDISR2_QMBM         0x08000000
#define P2041_DEVDISR2_FM1          0x02000000
#define P2041_DEVDISR2_XAUI1        0x01000000
#define P2041_DEVDISR2_DTSEC1_1     0x00800000
#define P2041_DEVDISR2_DTSEC1_2     0x00400000
#define P2041_DEVDISR2_DTSEC1_3     0x00200000
#define P2041_DEVDISR2_DTSEC1_4     0x00100000
#define P2041_DEVDISR2_FM2          0x00020000
#define P2041_DEVDISR2_XAUI2        0x00010000
#define P2041_DEVDISR2_DTSEC2_1     0x00008000
#define P2041_DEVDISR2_DTSEC2_2     0x00004000
#define P2041_DEVDISR2_DTSEC2_3     0x00002000
#define P2041_DEVDISR2_DTSEC2_4     0x00001000

#define P2041_POWMGTCSR(base)       (CAST(VUINT32 *)((base) + 0xE2040))
#define P2041_POWMGTCSR_LYNX_PD     0x00040000
#define P2041_POWMGTCSR_SLP         0x00020000
#define P2041_POWMGTCSR_SLPING      0x00000002
#define P2041_POWMGTCSR_PSLPING     0x00000001

/* CPU Doze/Nap status and control registers - bits are 1 << CPU index */

#define P2041_CDOZSRL(base)         (CAST(VUINT32 *)((base) + 0xE2004))
#define P2041_CDOZCRL(base)         (CAST(VUINT32 *)((base) + 0xE200C))
#define P2041_CNAPSRL(base)         (CAST(VUINT32 *)((base) + 0xE2014))
#define P2041_CNAPCRL(base)         (CAST(VUINT32 *)((base) + 0xE201C))

/* Processor reset register */

#define P2041_EPICPRR(base)         (CAST(VUINT32 *)((base) + 0x41098))

/* Processor initialization register */

#define P2041_EPICPIR(base)         (CAST(VUINT32 *)((base) + 0x41090))

#define P2041_EPICPIR_HRST_CORE0    0x00000001
#define P2041_EPICPIR_HRST_CORE1    0x00000002
#define P2041_EPICPIR_HRST_CORE2    0x00000004
#define P2041_EPICPIR_HRST_CORE3    0x00000008
#define P2041_EPICPIR_HRST_CORE4    0x00000010
#define P2041_EPICPIR_HRST_CORE5    0x00000020
#define P2041_EPICPIR_HRST_CORE6    0x00000040
#define P2041_EPICPIR_HRST_CORE7    0x00000080
#define P2041_EPICPIR_HRST_COREn0   0x000000fe

#define P2041_EPICWHOAMI(base)      (CAST(VUINT32 *)((base) + 0x40090))
#define P5020_EPICWHOAMI            P2041_EPICWHOAMI

/* 
 * CPU-private current task priority and end-of-interrupt registers 
 * (used for CPU reset)
 */

#define P2041_EPICCTPRn(base,n)     (CAST(VUINT32 *)((base) + 0x60080 + (n)*0x1000))    
#define P2041_EPICEOIn(base,n)      (CAST(VUINT32 *)((base) + 0x600b0 + (n)*0x1000))    
#define EPIC_CTPR_MAX               15  /* maximum priority (mask) */
#define RESET_EOI_NUM               15  /* EOIs to send RM:31.5.1.3 */

#define P2041_PVR(base)             (CAST(VUINT32 *)((base) + 0xE00A0))
#define P2041_SVR(base)             (CAST(VUINT32 *)((base) + 0xE00A4))

#define P2041_BRR(base)             (CAST(VUINT32 *)((base) + 0xE00E4)) 

/* Core reset status register */

#define P2041_CRSTSR(base,n)        (CAST(VUINT32 *)((base) + 0xE0400 + (n)*4)) 
#define P2041_CRSTSR_RST_WRT        0x08000000
#define P2041_CRSTSR_RST_MPIC       0x00800000
#define P2041_CRSTSR_RST_CORE       0x00080000
#define P2041_CRSTSR_READY          0x00008000
#define P2041_CRSTSR_RST_HRST       0x00000002
#define P2041_CRSTSR_RST_PORST      0x00000001

/* reset configuration word status registers (0 <= n < 16) */

#define P2041_RCWSR(base,n)         (CAST(VUINT32 *)((base) + 0xE0100 + (n)*4))  
#define P2041_MEM_PLL_RAT_MSK       0x003E0000
#define P2041_MEM_PLL_RAT_SFT       17
#define P2041_MEM_PLL_RAT(base)     ((*P2041_RCWSR(base,0) & P2041_MEM_PLL_RAT_MSK) >> P2041_MEM_PLL_RAT_SFT)

#define P2041_SYS_PLL_RAT_MSK       0x3E000000
#define P2041_SYS_PLL_RAT_SFT       25
#define P2041_SYS_PLL_RAT(base)     ((*P2041_RCWSR(base,0) & P2041_SYS_PLL_RAT_MSK) >> P2041_SYS_PLL_RAT_SFT)

#define P2041_CCN_PLL_RAT_MSK(n)    (0x3E000000 >> (8 * n))
#define P2041_CCN_PLL_RAT_SFT(n)    (25 - (8 * n))
#define P2041_CCN_PLL_RAT(base,n)   ((*P2041_RCWSR(base,2) & P2041_CCN_PLL_RAT_MSK(n)) >> P2041_CCN_PLL_RAT_SFT(n))

#define P2041_CN_PLL_SEL_MSK(n)     (0xF0000000 >> (4 * n))
#define P2041_CN_PLL_SEL_SFT(n)     (28 - (4 * n))          
#define P2041_CN_PLL_SEL(base,n)    ((*P2041_RCWSR(base,3) & P2041_CN_PLL_SEL_MSK(n)) >> P2041_CN_PLL_SEL_SFT(n))

/* Timebase enable register */

#define P2041_CTBENR(base)          (CAST(VUINT32 *)((base) + 0xE2084))

#define _PPC_BUCSR_FI               0x200       /* Invalidate branch cache */
#define _PPC_BUCSR_E                0x1         /* Enable branch prediction */

/* DMA registers base address and offsets */

#define DMA1_REGBASE                (CCSBAR + 0x100100)
#define DMA2_REGBASE                (CCSBAR + 0x101100)

#define DMA_CHAN_OFFSET             0x80
#define DMA_CHAN_NUM                4

/* SPI defines */

#define SPI_REG_BASE                (CCSBAR + 0x110000)

/* I2C */

#define I2C_BUS1_BASE               (CCSBAR + 0x118000)
#define I2C_BUS2_BASE               (CCSBAR + 0x118100)
#define I2C_BUS3_BASE               (CCSBAR + 0x119000)
#define I2C_BUS4_BASE               (CCSBAR + 0x119100)

/* USB */

#define USB1_BASE                   (CCSBAR + 0x210000)
#define USB2_BASE                   (CCSBAR + 0x211000)

#define EPIC_USB1_INT_VEC           (28 + EPIC_VEC_IN_IRQ0)
#define EPIC_USB2_INT_VEC           (29 + EPIC_VEC_IN_IRQ0)

/* EHCI registers */

#define EHCI_CAPLENGTH(base)        ((base) + 0x00100)

#define EHCI_BURSTSIZE(base)        ((base) + 0x00160)
#define EHCI_TXTTFILLTUNING_L(base) ((base) + 0x00164)
#define EHCI_TXTTFILLTUNING_H(base) ((base) + 0x00168)
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

#define EHCI_PORTSC_ULPI            0x80000000
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

/* SERDES control register definitions */

#define SRDS_BASE                   (CCSBAR + 0xEA000)

#define SRDS_MAX_LANES              18
#define SRDS_MAX_BANKS              3
#define SRDS_MAX_CFGS               0x26

#define RCWSR4_SRDS_PRTCL           0xFC000000

#define PCIE1                       0x00
#define PCIE2                       0x01
#define PCIE3                       0x02
#define XAUI_FM1                    0x03
#define XAUI_FM2                    0x04
#define SGMII_FM1                   0x05
#define SGMII_FM2                   0x06
#define SRIO1                       0x07
#define SRIO2                       0x08
#define AURORA                      0x09
#define NOPROTO                     0xFF

#define SRDS_BANK1                  0x000
#define SRDS_BANK2                  0x020
#define SRDS_BANK3                  0x040

/* Three sets of these, per bank */

#define SRDS_BANK_RCR               0x000 /* Reset control */
#define SRDS_BANK_PCR0              0x004 /* PLL control 0 */
#define SRDS_BANK_PCR1              0x008 /* PLL control 1 */

#define SDRS_CALCTL_TX              0x090 /* TX calibration control */
#define SDRS_CALCTL_RX              0x0A0 /* RX calibration control */

#define SRDS_GR0                    0x0B0 /* General register */
#define SRDS_PCONV_CFG0             0x0E0 /* Protocol converter cfg 0 */
#define SRDS_PCONV_CFG1             0x0E4 /* Protocol converter cfg 1 */
#define SRDS_PCONV_CFG2             0x0E8 /* Protocol converter cfg 2 */

#define SRDS_BANK3_ALIGN_STS        0x250 /* WAR 4 */
#define SRDS_BANK2_ALIGN_STS        0x260 /* WAR 4 */

/* Reset control */

#define SRDS_RCR_RSTRQ              0x80000000 /* Start reset */
#define SRDS_RCR_RSTDONE            0x40000000 /* Reset complete */
#define SRDS_RCR_RSTERR             0x20000000 /* Reset error */
#define SRDS_RCR_PSCL               0x0E000000 /* Plat cycles per tick */
#define SRDS_RCR_CNT1               0x00F00000 /* cnt1 exit state ticks */
#define SRDS_RCR_CNT2               0x000F0000 /* cnt2 exit state ticks */
#define SRDS_RCR_CNT3               0x0000F000 /* cnt3 exit state ticks */
#define SRDS_RCR_CNT4               0x00000F00 /* cnt4 exit state ticks */
#define SRDS_RCR_PLLRST             0x00000080 /* PLL master reset */
#define SRDS_RCR_SDRST              0x00000040 /* Serdes master reset */
#define SRDS_RCR_SDPD               0x00000020 /* Serdes power down */
#define SRDS_RCR_RSMCS              0x00000007 /* current PLL state */

#define SRDS_PSCL_200MHZ            0x00000000
#define SRDS_PSCL_400MHZ            0x01000000
#define SRDS_PSCL_600MHZ            0x02000000
#define SRDS_PSCL_800MHZ            0x03000000
#define SRDS_PSCL_1000MHZ           0x04000000

/* PLL control 0 */

#define SRDS_PCR0_RFCK_SEL          0x30000000 /* refrerence clock freq */
#define SRDS_PCR0_RCFK_EN           0x01000000 /* ref clock enable */
#define SRDS_PCR0_PLL_LCK           0x00800000 /* PLL has locked */
#define SRDS_PCR0_FRATE_SEL         0x00030000 /* PLL VCO freq */

#define SRDS_PCR0_RFCK_100MHZ       0x00000000
#define SRDS_PCR0_RFCK_125MHZ       0x10000000
#define SRDS_PCR0_RFCK_156MHZ       0x20000000

#define SRDS_PCR0_FRATE_5GHZ        0x00010000
#define SRDS_PCR0_FRATE_6_25GHZ     0x00010000

/* PLL control 1 */

#define SRDS_PCR1_PLLBW_SEL         0x08000000 /* PLL bandwidth */
#define SRDS_PCR1_RFCK_TRM          0x00000300 /* RX termination common mode */

#define SRCS_PCR1_PLLBW_NOMINAL     0x00000000
#define SRCS_PCR1_PLLBW_PCIE        0x08000000

/* TX calibration control */

#define SRDS_CALCTL_TX_RST          0x08000000 /* Reset */

/* TX calibration control */

#define SRDS_CALCTL_RX_RST          0x08000000 /* Reset */

/* General register */

#define SRDS_GR_LID                 0x03FF0000 /* block ID register */
#define SRDS_GR_DDR_SEL             0x00004000 /* DDR value */

#define SRDS_DDR_1_8VDD             0x00000000
#define SRDS_DDR_1_5VDD             0x00004000

/* Protcol converter configuration 0 */

#define SRDS_PCONVCFG0_PEXCFG1      0x30000000
#define SRDS_PCONVCFG0_PEXCFG2      0x03000000
#define SRDS_PCONVCFG0_PEXCFG3      0x00300000
#define SRDS_PCONVCFG0_SRIOCFG1     0x00003000  
#define SRDS_PCONVCFG0_SRIOCFG2     0x00000300

#define SRDS_PEXCFG1_DISABLED       0x00000000
#define SRDS_PEXCFG1_X2             0x10000000
#define SRDS_PEXCFG1_X4             0x20000000
#define SRDS_PEXCFG1_X8             0x30000000

#define SRDS_PEXCFG2_DISABLED       0x00000000
#define SRDS_PEXCFG2_X2             0x01000000
#define SRDS_PEXCFG2_X4             0x02000000
#define SRDS_PEXCFG2_X8             0x03000000

#define SRDS_PEXCFG3_DISABLED       0x00000000
#define SRDS_PEXCFG3_X2             0x00100000
#define SRDS_PEXCFG3_X4             0x00200000
#define SRDS_PEXCFG3_X8             0x00300000

/* Protocol converter configuration 1 */

#define SRDS_PCONVCFG1_SGMII1       0x40000000
#define SRDS_PCONVCFG1_SGMII2       0x10000000
#define SRDS_PCONVCFG1_SGMII3       0x04000000
#define SRDS_PCONVCFG1_SGMII4       0x01000000
#define SRDS_PCONVCFG1_SGMII5       0x00C00000
#define SRDS_PCONVCFG1_SGMII6       0x00300000
#define SRDS_PCONVCFG1_SGMII7       0x000C0000
#define SRDS_PCONVCFG1_SGMII8       0x00030000
#define SRDS_PCONVCFG1_XGMII1       0x00004000
#define SRDS_PCONVCFG1_XGMII2       0x00001000
#define SRDS_PCONVCFG1_AURORA       0x00000030

#define SRDS_SGMII1_DISABLED        0x00000000
#define SRDS_SGMII1_X1              0x40000000

#define SRDS_SGMII2_DISABLED        0x00000000
#define SRDS_SGMII2_X1              0x10000000

#define SRDS_SGMII3_DISABLED        0x00000000
#define SRDS_SGMII3_X1              0x04000000

#define SRDS_SGMII4_DISABLED        0x00000000
#define SRDS_SGMII4_X1              0x01000000

#define SRDS_SGMII5_DISABLED        0x00000000
#define SRDS_SGMII5_X1_B1           0x00400000
#define SRDS_SGMII5_X1_B2           0x00800000

#define SRDS_SGMII6_DISABLED        0x00000000
#define SRDS_SGMII6_X1_B1           0x00100000
#define SRDS_SGMII6_X1_B2           0x00200000

#define SRDS_SGMII7_DISABLED        0x00000000
#define SRDS_SGMII7_X1_B1           0x00040000
#define SRDS_SGMII7_X1_B2           0x00080000

#define SRDS_SGMII8_DISABLED        0x00000000
#define SRDS_SGMII8_X1_B1           0x00010000
#define SRDS_SGMII8_X1_B2           0x00020000

#define SRDS_XGMII1_DISABLED        0x00000000
#define SRDS_XGMII1_X4              0x00004000

#define SRDS_XGMII2_DISABLED        0x00000000
#define SRDS_XGMII2_X4              0x00001000

/* Protocol converter configuration 2 */

#define SRDS_PCONVCFG2_PEX1_RST     0x80000000
#define SRDS_PCONVCFG2_PEX2_RST     0x40000000
#define SRDS_PCONVCFG2_PEX3_RST     0x20000000
#define SRDS_PCONVCFG2_SRIO1RST     0x08000000
#define SRDS_PCONVCFG2_SRIO2RST     0x04000000
#define SRDS_PCONVCFG2_XGM1_RST     0x00800000
#define SRDS_PCONVCFG2_XGM2_RST     0x00400000
#define SRDS_PCONVCFG2_SGM1_RST     0x00008000
#define SRDS_PCONVCFG2_SGM2_RST     0x00004000
#define SRDS_PCONVCFG2_SGM3_RST     0x00002000
#define SRDS_PCONVCFG2_SGM4_RST     0x00001000
#define SRDS_PCONVCFG2_SGM5_RST     0x00000800
#define SRDS_PCONVCFG2_SGM6_RST     0x00000400
#define SRDS_PCONVCFG2_SGM7_RST     0x00000200
#define SRDS_PCONVCFG2_SGM8_RST     0x00000100

#define SRDS_LANE_BASE              0x400

#define SRDS_B1_LANEA               0x000
#define SRDS_B1_LANEB               0x040
#define SRDS_B1_LANEC               0x080
#define SRDS_B1_LANED               0x0C0
#define SRDS_B1_LANEE               0x100
#define SRDS_B1_LANEF               0x140
#define SRDS_B1_LANEG               0x180
#define SRDS_B1_LANEH               0x1C0
#define SRDS_B1_LANEI               0x200
#define SRDS_B1_LANEJ               0x240 
#define SRDS_B2_LANEA               0x400
#define SRDS_B2_LANEB               0x440
#define SRDS_B2_LANEC               0x480
#define SRDS_B2_LANED               0x4C0
#define SRDS_B3_LANEA               0x500
#define SRDS_B3_LANEB               0x540
#define SRDS_B3_LANEC               0x580
#define SRDS_B3_LANED               0x5C0

#define SRDS_GCR0                   0x000 /* General control 0 */
#define SRDS_GCR1                   0x004 /* General control 0 */
#define SRDS_RSVD0                  0x008
#define SRDS_RSVD1                  0x00C
#define SRDS_RSVD2                  0x010 /* WAR 2 */
#define SRDS_LOOPBACK2              0x010 /* WAR 2 */
#define SRDS_RSVD3                  0x014
#define SRDS_TXEQ                   0x018 /* TX equalization */
#define SRDS_RSVD5                  0x01C
#define SRDS_TTLCTL                 0x020 /* TTL control */
#define SRDS_RSVD6                  0x024
#define SRDS_RSVD7                  0x028
#define SRDS_RSVD8                  0x02C
#define SRDS_RSVD9                  0x030
#define SRDS_RSVD10                 0x034 /* WAR 1 */
#define SRDS_LOOPBACK1              0x034 /* WAR 1 */
#define SRDS_RSVD11                 0x038
#define SRDS_RSVD12                 0x03C /* WAR 3 */
#define SRDS_LOOPBACK3              0x03C /* WAR 3 */

/* General control register 0 */

#define SRDS_GCR0_RRAT_SEL          0x30000000 /* RX speed */
#define SRDS_GCR0_TRAT_SEL          0x03000000 /* TX speed */
#define SRDS_GCR0_HALT_RCLK         0x00800000 /* halt RX clock */
#define SRDS_GCR0_RRST              0x00400000 /* Reset receiver */
#define SRDS_GCR0_TRST              0x00200000 /* Reset xmitter */
#define SRDS_GCR0_PD                0x00080000 /* Lane power down */
#define SRDS_GCR0_X3S               0x00040000 /* Lane tri-state */
#define SRDS_GCR0_IACC_EN           0x00020000 /* AC coupling enb */
#define SRDS_GCR0_1STLANE           0x00010000 /* 1st lane in group */
#define SRDS_GCR0_TTRM_VM_SEL       0x00003000 /* RX term common mode */
#define SRDS_GCR0_PROTS             0x00000F00 /* Protocol selection */

#define SRDS_PROTO_PCIE             0x00000000
#define SRDS_PROTO_SGMII            0x00000100
#define SRDS_PROTO_SRIO             0x00000300
#define SRDS_PROTO_XGMII            0x00000400
#define SRDS_PROTO_AURORA           0x00000500

/* General control register 1 */

#define SRDS_GCR1_TDET              0x20000000 /* RX detection */
#define SRDS_GCR1_REIDL             0x10000000 /* RX elect. idle detect */
#define SRDS_GCR1_OPAD              0x04000000 /* TX output pad */
#define SRDS_GCR1_TDET_EN           0x02000000 /* Enable RX det */
#define SRDS_GCR1_REIDL_EN          0x01000000 /* Enable RX idle */
#define SRDS_GCR1_REIDL_CTL         0x001F0000 /* RX idle filter select */

#define SRDS_GCR1_REIDL_SRIO        0x00000000
#define SRDS_GCR1_REIDL_SGMII       0x00040000
#define SRDS_GCR1_REIDL_PEX         0x00100000

/* TX equalization register */

#define SRDS_TXEQ_TYPE              0x30000000 /* equalization type */

#define SRDS_TXEQTYPE_NONE          0x00000000
#define SRDS_TXEQTYPE_2LVL          0x10000000
#define SRDS_TXEQTYPE_3LVL          0x20000000
#define SRDS_TXEQTYPE_4LVL          0x30000000

#define SRDS_READ(offset) \
    vxbRead32 (VXB_HANDLE_MEM, (UINT32 *)(offset + SRDS_BASE))

#define SRDS_WRITE(offset, val) \
    vxbWrite32 (VXB_HANDLE_MEM, (UINT32 *)(offset + SRDS_BASE), val)

#define SRDS_SETBIT(offset, val)          \
    SRDS_WRITE(offset, SRDS_READ(offset) | (val))

#define SRDS_CLRBIT(offset, val)          \
    SRDS_WRITE(offset, SRDS_READ(offset) & ~(val))

#define SRDS_LN_READ(lane, offset) \
    SRDS_READ(lane + SRDS_LANE_BASE + offset)

#define SRDS_LN_WRITE(lane, offset, val) \
    SRDS_WRITE(lane + SRDS_LANE_BASE + offset, val)

#define SRDS_LN_CLRBIT(lane, offset, val) \
    SRDS_CLRBIT(lane + SRDS_LANE_BASE + offset, val)

#define SRDS_LN_SETBIT(lane, offset, val) \
    SRDS_SETBIT(lane + SRDS_LANE_BASE + offset, val)

#define SRDS_BK_READ(lane, offset) \
    SRDS_READ(lane + offset)

#define SRDS_BK_WRITE(lane, offset, val) \
    SRDS_WRITE(lane + offset, val)

#define SRDS_BK_CLRBIT(lane, offset, val) \
    SRDS_CLRBIT(lane + offset, val)

#define SRDS_BK_SETBIT(lane, offset, val) \
    SRDS_SETBIT(lane + offset, val)

/* BOARD SPECIFIC VALUES: */

#define  FLASH_OR0_VALUE            0x0064
#define  LCRR_CLOCK_DIV             2
#define  LAWBARL2_VALUE             ((DDR_SDRAM_LOCAL_ADRS2) >> (LAWBAR_ADRS_SHIFT))

/* TO MATCH THE P4080 to facilitate sharing romInit.s: */

#define MMU_RAM_ACCESS              MMU_CACHED  /* WIMGE bits for RAM */
#define DDR_SDRAM_LAWAR_TGTIF       LAWAR_TGTIF_DDRSDRAM
#undef  DDR_SDRAM_LAWAR_TGTIF2                  /* LAWAR_TGTIF_DDRSDRAM2 */

#define P4080_CTBENR                P2041_CTBENR
#define P4080_LAWAR0                P2041_LAWAR0
#define P4080_LAWAR1                P2041_LAWAR1
#define P4080_LAWAR3                P2041_LAWAR3
#define P4080_LAWAR8                P2041_LAWAR8
#define P4080_LAWARL8               P2041_LAWARL8
#define P4080_LAWBARL0              P2041_LAWBARL0
#define P4080_LAWBARL1              P2041_LAWBARL1
#define P4080_LAWBARH1              P2041_LAWBARH1
#define P4080_LAWBARL3              P2041_LAWBARL3
#define P4080_LAWBARL8              P2041_LAWBARL8
#define P4080_LAWBARH29             P2041_LAWBARH29
#define P4080_LAWBARL29             P2041_LAWBARL29
#define P4080_LAWAR29               P2041_LAWAR29
#define P4080_LAWBARH30             P2041_LAWBARH30
#define P4080_LAWBARL30             P2041_LAWBARL30
#define P4080_LAWAR30               P2041_LAWAR30
#define P4080_OR0                   P2041_OR0
#define P4080_BR0                   P2041_BR0
#define P4080_OR3                   P2041_OR3
#define P4080_BR3                   P2041_BR3
#define P4080_LBCR                  P2041_LBCR
#define P4080_LCRR                  P2041_LCRR

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* __INCp2041rdbh */
