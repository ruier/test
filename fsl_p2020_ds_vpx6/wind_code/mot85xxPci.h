/* mot85xxPci.h - FreeScale p2020 PCI Bridge setup header file */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,04jul02,syt derived from version 01c of ads8572/mot85xxPci.h
*/

#ifndef __INCmot85xxPcih
#define __INCmot85xxPcih

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */


#define COMMAND_REGISTER_OFFSET     0x4
#define COMMAND_REGISTER_WIDTH      0x2
#define BRIDGE_BAR0_OFFSET          0x10
#define BRIDGE_BAR0_WIDTH           0x4

/* PCIEX slots number */

#define NUM_PCIEX1_SLOTS            0x1
#define NUM_PCIEX2_SLOTS            0x1
#define NUM_PCIEX3_SLOTS            0x1

/* PCIEX interrupt level */

#define PCIEX1_XINT1_LVL            0
#define PCIEX1_XINT2_LVL            1
#define PCIEX1_XINT3_LVL            2
#define PCIEX1_XINT4_LVL            3

#define PCIEX2_XINT1_LVL            4
#define PCIEX2_XINT2_LVL            5
#define PCIEX2_XINT3_LVL            6
#define PCIEX2_XINT4_LVL            7

#define PCIEX3_XINT1_LVL            8
#define PCIEX3_XINT2_LVL            9
#define PCIEX3_XINT3_LVL            10
#define PCIEX3_XINT4_LVL            11

#define PCI_LAT_TIMER               0x40  /* latency timer value, 64 PCI clocks */

/* PCIEX configuration space reg and int ack */

#define PCIEX1_REGBASE              (CCSBAR + 0xa000)   /* Slot2 */
#define PCIEX2_REGBASE              (CCSBAR + 0x9000)   /* M1575 */
#define PCIEX3_REGBASE              (CCSBAR + 0x8000)   /* Slot1 */

/* PCIEX based addresses */

#define PCIEX1_LAW_BASE             0x80000000          /* PCI LAW window */
#define PCIEX1_LAW_SIZE             0x10000000          /* PCI LAW size   */
#define PCIEX1_MEM_SIZE             0x04000000          /* 64 MB */
#define PCIEX1_MEMIO_SIZE           0x04000000          /* 64 MB */
#define PCIEX1_IO_SIZE              0x04000000          /* 64 MB */
#define PCIEX1_MEM_ADRS             (PCIEX1_LAW_BASE)
#define PCIEX1_MEMIO_ADRS           (PCIEX1_MEM_ADRS   + PCIEX1_MEM_SIZE)
#define PCIEX1_IO_ADRS              (PCIEX1_MEMIO_ADRS + PCIEX1_MEMIO_SIZE)

#define PCIEX2_LAW_BASE             0x90000000          /* PCI LAW window */
#define PCIEX2_LAW_SIZE             0x10000000          /* PCI LAW size   */
#define PCIEX2_MEM_SIZE             0x04000000          /* 64 MB */
#define PCIEX2_MEMIO_SIZE           0x04000000          /* 64 MB */
#define PCIEX2_IO_SIZE              0x04000000          /* 64 MB */
#define PCIEX2_MEM_ADRS             (PCIEX2_LAW_BASE)
#define PCIEX2_MEMIO_ADRS           (PCIEX2_MEM_ADRS   + PCIEX2_MEM_SIZE)
#define PCIEX2_IO_ADRS              (PCIEX2_MEMIO_ADRS + PCIEX2_MEMIO_SIZE)

#define PCIEX3_LAW_BASE             0xa0000000          /* PCI LAW window */
#define PCIEX3_LAW_SIZE             0x10000000          /* PCI LAW size   */
#define PCIEX3_MEM_SIZE             0x04000000          /* 64 MB */
#define PCIEX3_MEMIO_SIZE           0x04000000          /* 64 MB */
#define PCIEX3_IO_SIZE              0x04000000          /* 64 MB */
#define PCIEX3_MEM_ADRS             (PCIEX3_LAW_BASE)
#define PCIEX3_MEMIO_ADRS           (PCIEX3_MEM_ADRS   + PCIEX3_MEM_SIZE)
#define PCIEX3_IO_ADRS              (PCIEX3_MEMIO_ADRS + PCIEX3_MEMIO_SIZE)

#define PCI_MMU_TLB_SZ              _MMU_TLB_SZ_256M

#define NUM_PCIEX_SLOTS             2

/* CPU address */

#define PCI_MSTR_MEM_BUS            0x00000000
#define PCI_BRIDGE_PIMMR_BASE_ADRS  0xc0000000

/* PCI Express Outbound translation registers */

#undef CVPUINT32
#define CVPUINT32(base,addr)        (CAST(VUINT32 *)((base) + (addr)))

#define PCIEX_OUTBOUND_TRANS_ADRS_REG0(base)     CVPUINT32(base, 0xac00)
#define PCIEX_OUTBOUND_TRANS_ADRS_REG1(base)     CVPUINT32(base, 0xac20)
#define PCIEX_OUTBOUND_TRANS_ADRS_REG2(base)     CVPUINT32(base, 0xac40)
#define PCIEX_OUTBOUND_TRANS_ADRS_REG3(base)     CVPUINT32(base, 0xac60)
#define PCIEX_OUTBOUND_TRANS_ADRS_REG4(base)     CVPUINT32(base, 0xac80)
#define PCIEX_OUTBOUND_TRANS_EXT_ADRS_REG0(base) CVPUINT32(base, 0xac04)
#define PCIEX_OUTBOUND_TRANS_EXT_ADRS_REG1(base) CVPUINT32(base, 0xac24)
#define PCIEX_OUTBOUND_TRANS_EXT_ADRS_REG2(base) CVPUINT32(base, 0xac44)
#define PCIEX_OUTBOUND_TRANS_EXT_ADRS_REG3(base) CVPUINT32(base, 0xac64)
#define PCIEX_OUTBOUND_TRANS_EXT_ADRS_REG4(base) CVPUINT32(base, 0xac84)
#define PCIEX_OUTBOUND_BASE_ADRS_REG0(base)      CVPUINT32(base, 0xac08)
#define PCIEX_OUTBOUND_BASE_ADRS_REG1(base)      CVPUINT32(base, 0xac28)
#define PCIEX_OUTBOUND_BASE_ADRS_REG2(base)      CVPUINT32(base, 0xac48)
#define PCIEX_OUTBOUND_BASE_ADRS_REG3(base)      CVPUINT32(base, 0xac68)
#define PCIEX_OUTBOUND_BASE_ADRS_REG4(base)      CVPUINT32(base, 0xac88)

/* Outbound attributes register definitions */

#define PCIEX_OUTBOUND_ATTR_REG0(base)           CVPUINT32(base, 0xac10)
#define PCIEX_OUTBOUND_ATTR_REG1(base)           CVPUINT32(base, 0xac30)
#define PCIEX_OUTBOUND_ATTR_REG2(base)           CVPUINT32(base, 0xac50)
#define PCIEX_OUTBOUND_ATTR_REG3(base)           CVPUINT32(base, 0xac70)
#define PCIEX_OUTBOUND_ATTR_REG4(base)           CVPUINT32(base, 0xac90)

/* Outbound/Inbound Comparison mask register defines */

#define PCI_WINDOW_ENABLE_BIT                               0x80000000
#define PCI_ATTR_BS_BIT                                     0x40000000
#define PCI_OUT_ATTR_RTT_MEM                                0x00040000
#define PCI_OUT_ATTR_RTT_IO                                 0x00080000
#define PCI_OUT_ATTR_WTT_MEM                                0x00004000
#define PCI_OUT_ATTR_WTT_IO                                 0x00008000
#define PCI_ATTR_WS_4K                                      0x0000000B
#define PCI_ATTR_WS_8K                                      0x0000000c
#define PCI_ATTR_WS_16K                                     0x0000000D
#define PCI_ATTR_WS_32K                                     0x0000000E
#define PCI_ATTR_WS_64K                                     0x0000000F
#define PCI_ATTR_WS_128K                                    0x00000010
#define PCI_ATTR_WS_256K                                    0x00000011
#define PCI_ATTR_WS_512K                                    0x00000012
#define PCI_ATTR_WS_1M                                      0x00000013
#define PCI_ATTR_WS_2M                                      0x00000014
#define PCI_ATTR_WS_4M                                      0x00000015
#define PCI_ATTR_WS_8M                                      0x00000016
#define PCI_ATTR_WS_16M                                     0x00000017
#define PCI_ATTR_WS_32M                                     0x00000018
#define PCI_ATTR_WS_64M                                     0x00000019
#define PCI_ATTR_WS_128M                                    0x0000001a
#define PCI_ATTR_WS_256M                                    0x0000001b
#define PCI_ATTR_WS_512M                                    0x0000001c
#define PCI_ATTR_WS_1G                                      0x0000001d
#define PCI_ATTR_WS_2G                                      0x0000001e
#define PCI_ATTR_WS_4G                                      0x0000001f
#define PCI_IN_ATTR_TGI_LM                                  0x00f00000
#define PCI_IN_ATTR_TGI_RIO                                 0x00c00000
#define PCI_IN_ATTR_RTT_RIO_READ                            0x00040000
#define PCI_IN_ATTR_RTT_LM_READ_NO_SNOOP                    0x00040000
#define PCI_IN_ATTR_RTT_LM_READ_SNOOP                       0x00050000
#define PCI_IN_ATTR_RTT_LM_READ_UNLOCK_L2_CACHE_LINE        0x00070000
#define PCI_IN_ATTR_RTT_LM_WRITE_NO_SNOOP                   0x00004000
#define PCI_IN_ATTR_RTT_LM_WRITE_SNOOP                      0x00005000
#define PCI_IN_ATTR_RTT_LM_WRITE_ALLOC_L2_CACHE_LINE        0x00006000
#define PCI_IN_ATTR_RTT_LM_WRITE_ALLOC_LOCK_L2_CACHE_LINE   0x00007000

#define PCI_SNOOP_ENABLE                                    0x40000000
#define PCI_PREFETCHABLE                                    0x20000000


/* PCI Express Inbound translation registers */

#define PCIEX_INBOUND_TRANS_ADRS_REG3(base)     CVPUINT32(base, 0xaDA0)
#define PCIEX_INBOUND_TRANS_EXT_ADRS_REG3(base) CVPUINT32(base, 0xaDA4)
#define PCIEX_INBOUND_BASE_ADRS_REG3(base)      CVPUINT32(base, 0xaDA8)
#define PCIEX_INBOUND_ATTR_REG3(base)           CVPUINT32(base, 0xaDB0)
#define PCIEX_INBOUND_TRANS_ADRS_REG2(base)     CVPUINT32(base, 0xaDC0)
#define PCIEX_INBOUND_BASE_ADRS_REG2(base)      CVPUINT32(base, 0xaDC8)
#define PCIEX_INBOUND_ATTR_REG2(base)           CVPUINT32(base, 0xaDD0)
#define PCIEX_INBOUND_TRANS_ADRS_REG1(base)     CVPUINT32(base, 0xaDE0)
#define PCIEX_INBOUND_BASE_ADRS_REG1(base)      CVPUINT32(base, 0xaDE8)
#define PCIEX_INBOUND_ATTR_REG1(base)           CVPUINT32(base, 0xaDF0)

/* PCI 1 error Registers */

#define PCI_ERROR_DETECT_REG                                0x8e00
#define PCI_ERROR_CAPTURE_DISABLE_REG                       0x8e04
#define PCI_ERROR_ENABLE_REG                                0x8e08
#define PCI_ERROR_ATTR_CAPTURE_REG                          0x8e0c
#define PCI_ERROR_ADRS_CAPTURE_REG                          0x8e10
#define PCI_ERROR_EXT_ADRS_CAPTURE_REG                      0x8e14
#define PCI_ERROR_DATA_LOW_CAPTURE_REG                      0x8e18
#define PCI_ERROR_DATA_HIGH_CAPTURE_REG                     0x8e1c
#define PCI_ERROR_GASKET_TIMER_REG                          0x8e20

/* Command status register defines */

#define BUS_MASTER_ENABLE_BIT                               0x4
#define MEMORY_SPACE_ACCESS_ENABLE_BIT                      0x2

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* __INCmot85xxPcih */

