/* mot82xxPci.h - Motorola wrSbcPowerQuiccII 82xx PCI Bridge setup header file */

/* Copyright 2003 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
modification history
--------------------
01c,29may03,scm  adjust header...
01b,09may03,scm  remove duplicate var names...
01a,24apr03,scm  File created.
*/

#ifndef _INCmot82xxPcih
#define _INCmot82xxPcih

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

/* IMMR Base Address    CORE|UINT32 */
#define IMMR_BASE_ADDR              0xF0000000

/* Register Offsets */
#define PQII_OUT_INT_STAT           (IMMR_BASE_ADDR + 0x10430)
#define PQII_OUT_INT_MSK            (IMMR_BASE_ADDR + 0x10434)
#define PQII_IN_FIFO_Q_PORT         (IMMR_BASE_ADDR + 0x10440)
#define PQII_OUT_FIFO_Q_PORT        (IMMR_BASE_ADDR + 0x10444)
#define PQII_IN_MSG_REG_0           (IMMR_BASE_ADDR + 0x10450)
#define PQII_IN_MSG_REG_1           (IMMR_BASE_ADDR + 0x10454)
#define PQII_OUT_MSG_REG_0          (IMMR_BASE_ADDR + 0x10458)
#define PQII_OUT_MSG_REG_1          (IMMR_BASE_ADDR + 0x1045C)
#define PQII_OUT_DOORBELL           (IMMR_BASE_ADDR + 0x10460)
#define PQII_IN_DOORBELL            (IMMR_BASE_ADDR + 0x10468)
#define PQII_IN_MSG_INT_STAT        (IMMR_BASE_ADDR + 0x10480)
#define PQII_IN_MSG_INT_MSK         (IMMR_BASE_ADDR + 0x10484)
#define PQII_IN_FREE_FIFO_HD_PTR    (IMMR_BASE_ADDR + 0x104A0)
#define PQII_IN_FREE_FIFO_TL_PTR    (IMMR_BASE_ADDR + 0x104A8)
#define PQII_IN_POST_FIFO_HD_PTR    (IMMR_BASE_ADDR + 0x104B0)
#define PQII_IN_POST_FIFO_TL_PTR    (IMMR_BASE_ADDR + 0x104B8)
#define PQII_OUT_FREE_FIFO_HD_PTR   (IMMR_BASE_ADDR + 0x104C0)
#define PQII_OUT_FREE_FIFO_TL_PTR   (IMMR_BASE_ADDR + 0x104C8)
#define PQII_OUT_POST_FIFO_HD_PTR   (IMMR_BASE_ADDR + 0x104D0)
#define PQII_OUT_POST_FIFO_TL_PTR   (IMMR_BASE_ADDR + 0x104D8)
#define PQII_MSG_UNIT_CTRL          (IMMR_BASE_ADDR + 0x104E4)
#define PQII_Q_BASE_ADRS            (IMMR_BASE_ADDR + 0x104F0)
#define PQII_DMA0_MODE              (IMMR_BASE_ADDR + 0x10500)
#define PQII_DMA0_STATUS            (IMMR_BASE_ADDR + 0x10504)
#define PQII_DMA0_CUR_DSC_ADRS      (IMMR_BASE_ADDR + 0x10508)
#define PQII_DMA0_SRC_ADRS          (IMMR_BASE_ADDR + 0x10510)
#define PQII_DMA0_DEST_ADRS         (IMMR_BASE_ADDR + 0x10518)
#define PQII_DMA0_BYTE_CNT          (IMMR_BASE_ADDR + 0x10520)
#define PQII_DMA0_NXT_DESC_ADRS     (IMMR_BASE_ADDR + 0x10524)
#define PQII_DMA1_MODE              (IMMR_BASE_ADDR + 0x10580)
#define PQII_DMA1_STATUS            (IMMR_BASE_ADDR + 0x10584)
#define PQII_DMA1_CUR_DSC_ADRS      (IMMR_BASE_ADDR + 0x10588)
#define PQII_DMA1_SRC_ADRS          (IMMR_BASE_ADDR + 0x10590)
#define PQII_DMA1_DEST_ADRS         (IMMR_BASE_ADDR + 0x10598)
#define PQII_DMA1_BYTE_CNT          (IMMR_BASE_ADDR + 0x105A0)
#define PQII_DMA1_NXT_DESC_ADRS     (IMMR_BASE_ADDR + 0x105A4)
#define PQII_DMA2_MODE              (IMMR_BASE_ADDR + 0x10600)
#define PQII_DMA2_STATUS            (IMMR_BASE_ADDR + 0x10604)
#define PQII_DMA2_CUR_DSC_ADRS      (IMMR_BASE_ADDR + 0x10608)
#define PQII_DMA2_SRC_ADRS          (IMMR_BASE_ADDR + 0x10610)
#define PQII_DMA2_DEST_ADRS         (IMMR_BASE_ADDR + 0x10618)
#define PQII_DMA2_BYTE_CNT          (IMMR_BASE_ADDR + 0x10620)
#define PQII_DMA2_NXT_DESC_ADRS     (IMMR_BASE_ADDR + 0x10624)
#define PQII_DMA3_MODE              (IMMR_BASE_ADDR + 0x10680)
#define PQII_DMA3_STATUS            (IMMR_BASE_ADDR + 0x10684)
#define PQII_DMA3_CUR_DSC_ADRS      (IMMR_BASE_ADDR + 0x10688)
#define PQII_DMA3_SRC_ADRS          (IMMR_BASE_ADDR + 0x10690)
#define PQII_DMA3_DEST_ADRS         (IMMR_BASE_ADDR + 0x10698)
#define PQII_DMA3_BYTE_CNT          (IMMR_BASE_ADDR + 0x106A0)
#define PQII_DMA3_NXT_DESC_ADRS     (IMMR_BASE_ADDR + 0x106A4)
#define PQII_PCI_OUT_TRANS_ADRS0    (IMMR_BASE_ADDR + 0x10800)
#define PQII_PCI_OUT_BASE_ADRS0     (IMMR_BASE_ADDR + 0x10808)
#define PQII_PCI_OUT_COMP_MSK0      (IMMR_BASE_ADDR + 0x10810)
#define PQII_PCI_OUT_TRANS_ADRS1    (IMMR_BASE_ADDR + 0x10818)
#define PQII_PCI_OUT_BASE_ADRS1     (IMMR_BASE_ADDR + 0x10820)
#define PQII_PCI_OUT_COMP_MSK1      (IMMR_BASE_ADDR + 0x10828)
#define PQII_PCI_OUT_TRANS_ADRS2    (IMMR_BASE_ADDR + 0x10830)
#define PQII_PCI_OUT_BASE_ADRS2     (IMMR_BASE_ADDR + 0x10838)
#define PQII_PCI_OUT_COMP_MSK2      (IMMR_BASE_ADDR + 0x10840)
#define PQII_PWR_MNGMNT_CTRL        (IMMR_BASE_ADDR + 0x10870)
#define PQII_DISCARD_TIMER_CTRL     (IMMR_BASE_ADDR + 0x10878)
#define PQII_GEN_PURPOSE_CTRL       (IMMR_BASE_ADDR + 0x1087C)
#define PQII_PCI_GEN_CTRL           (IMMR_BASE_ADDR + 0x10880)
#define PQII_ERR_STAT               (IMMR_BASE_ADDR + 0x10884)
#define PQII_ERR_MSK                (IMMR_BASE_ADDR + 0x10888)
#define PQII_ERR_CTRL               (IMMR_BASE_ADDR + 0x1088C)
#define PQII_PCI_ERR_ADRS_CAPT      (IMMR_BASE_ADDR + 0x10890)
#define PQII_PCI_ERR_DATA_CAPT      (IMMR_BASE_ADDR + 0x10898)
#define PQII_PCI_ERR_CTRL_CAPT      (IMMR_BASE_ADDR + 0x108A0)
#define PQII_PCI_IN_TRANS_ADRS1     (IMMR_BASE_ADDR + 0x108D0)
#define PQII_PCI_IN_BASE_ADRS1      (IMMR_BASE_ADDR + 0x108D8)
#define PQII_PCI_IN_COMP_MSK1       (IMMR_BASE_ADDR + 0x108E0)
#define PQII_PCI_IN_TRANS_ADRS0     (IMMR_BASE_ADDR + 0x108E8)
#define PQII_PCI_IN_BASE_ADRS0      (IMMR_BASE_ADDR + 0x108F0)
#define PQII_PCI_IN_COMP_MSK0       (IMMR_BASE_ADDR + 0x108F8)
#define PQII_PCI_CFG_ADDR           (IMMR_BASE_ADDR + 0x10900)
#define PQII_PCI_CFG_DATA           (IMMR_BASE_ADDR + 0x10904)
#define PQII_PCI_INT_ACK            (IMMR_BASE_ADDR + 0x10908)

#define PQII_SIPNR_H                (IMMR_BASE_ADDR + 0x10C08)
#define PQII_SIPNR_L                (IMMR_BASE_ADDR + 0x10C0C)

/* PCI Outbound Translation Registers */
#define PQII_PCI_BR0                (IMMR_BASE_ADDR + 0x101AC)
#define PQII_PCI_MSK0               (IMMR_BASE_ADDR + 0x101C4)
#define PQII_PCI_BR1                (IMMR_BASE_ADDR + 0x101B0)
#define PQII_PCI_MSK1               (IMMR_BASE_ADDR + 0x101C8)

#define PCI_ADDRESS_REGISTER        (PQII_PCI_CFG_ADDR) /* PCI Address Register */
#define PCI_DATA_REGISTER           (PQII_PCI_CFG_DATA) /* PCI Data Register */

/* PCI Configuration Registers Offsets */
#define PQII_VEND_ID                0x00
#define PQII_DEV_ID                 0x02
#define PQII_PCI_CMD                0x04
#define PQII_PCI_STAT               0x06
#define PQII_REV_ID                 0x08
#define PQII_STD_PROG_IFACE         0x09
#define PQII_SUBCLASS_CODE          0x0A
#define PQII_CLASS_CODE             0x0B
#define PQII_CACHE_LINE_SZ          0x0C
#define PQII_LATENCY_TIMER          0x0D
#define PQII_HEADER_TYPE            0x0E
#define PQII_BIST_CTRL              0x0F
#define PQII_PIMMR_BASE_ADRS        0x10
#define PQII_GPL_BASE_ADRS0         0x14
#define PQII_GPL_BASE_ADRS1         0x18
#define PQII_SUBSYS_VEND_ID         0x2C
#define PQII_SUBSYS_DEV_ID          0x2E
#define PQII_CAPABILITIES_PTR       0x34
#define PQII_INT_LINE               0x3C
#define PQII_INT_PIN                0x3D
#define PQII_MIN_GNT                0x3E
#define PQII_MAX_LAT                0x3F
#define PQII_PCI_FUNC               0x44
#define PQII_PCI_ARB_CTRL           0x46
#define PQII_HOT_SWAP_BLK           0x48

/* TBD ALL the registers for PCI Bridge */

/* These windows are ignored if an outbound translation register maps
 * to the same address but are set to the memory mapping on the 826x
 * with PCI.
 */
#define PCIBR0_SETTING              (PCI_MEM_BASE | 0x1)
#define PCIBR1_SETTING              (PCI_IO_BASE | 0x1)

#define PCIMSK0_SETTING             0xF0000000
#define PCIMSK1_SETTING             0xF0000000

/* configuration space register offsets */
#define COMMAND_REGISTER_OFFSET     0x4
#define COMMAND_REGISTER_WIDTH      0x2

/* Comand status register defines */
#define BUS_MASTER_ENABLE_BIT           0x4
#define MEMORY_SPACE_ACCESS_ENABLE_BIT  0x2
#define IO_SPACE_ACCESS_ENABLE_BIT      0x1

/* PCI_GCR register defines */
#define EXTERNAL_PCI_BUS_ENABLE     0x1

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* _INCmot82xxPcih */
