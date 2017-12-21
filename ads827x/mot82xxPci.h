/* mot82xxPci.h - Motorola ads 82xx PCI Bridge setup header file */

/* Copyright 1984-2003 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
modification history
--------------------
01c,01oct03,dee  fix SPR #89964
01b,08jan03,dtr  Added defines for PCI DMA and Error Handling.
01a,04jul02,dtr  File created.
*/

#ifndef _INCmot82xxPcih
#define _INCmot82xxPcih

#ifdef __cplusplus
    extern "C" {
#endif /* __cplusplus */

#define MAX_NUM_VECTORS 3

#define  PCIBR_REG0(x)   ( x | 0x101ac)
#define  PCIBR_REG1(x)   ( x | 0x101b0)
#define  PCIMSK_REG0(x)   ( x | 0x101c4)
#define  PCIMSK_REG1(x)   ( x | 0x101c8)

#define M8260_SCCR_PCI_MODE_BIT 0x80  
#define M8260_PPCACR(x)    ((UINT32)x | 0x10028)

#define PIMMR_OFFSET_BASE_ADRS          0x10430
#define OUTBOUND_INT_STAT_REG_OFFSET    0x0
#define OUTBOUND_INT_MASK_REG_OFFSET    0x4
#define INBOUND_FIF0_QUEUE_PORT_REG     0x10
#define OUTBOUND_FIF0_QUEUE_PORT_REG    0x14
#define INBOUND_MSG_REG0                0x20
#define OUTBOUND_MSG_REG0               0x24
#define INBOUND_MSG_REG1                0x28
#define OUTBOUND_MSG_REG1               0x2c

/* TBD ALL the registers for PCI Bridge */

/* Outbound translation registers */
#define PCI_OUTBOUND_TRANS_ADRS_REG0(x)     ((UINT32)x | 0x10800)
#define PCI_OUTBOUND_TRANS_ADRS_REG1(x)     ((UINT32)x | 0x10818)
#define PCI_OUTBOUND_TRANS_ADRS_REG2(x)     ((UINT32)x | 0x10830)
#define PCI_OUTBOUND_BASE_ADRS_REG0(x)      ((UINT32)x | 0x10808)
#define PCI_OUTBOUND_BASE_ADRS_REG1(x)      ((UINT32)x | 0x10820)
#define PCI_OUTBOUND_BASE_ADRS_REG2(x)      ((UINT32)x | 0x10838)
#define PCI_OUTBOUND_COMP_MASK_REG0(x)      ((UINT32)x | 0x10810)
#define PCI_OUTBOUND_COMP_MASK_REG1(x)      ((UINT32)x | 0x10828)
#define PCI_OUTBOUND_COMP_MASK_REG2(x)      ((UINT32)x | 0x10840)


/* Soft PCI reset */
#define PCI_GCR_REG                      0x10880

/* Inbound translation registers */
#define PCI_INBOUND_TRANS_ADRS_REG1      0x108D0
#define PCI_INBOUND_BASE_ADRS_REG1       0x108D8
#define PCI_INBOUND_COMP_MASK_REG1       0x108E0
#define PCI_INBOUND_TRANS_ADRS_REG0      0x108E8
#define PCI_INBOUND_BASE_ADRS_REG0       0x108F0
#define PCI_INBOUND_COMP_MASK_REG0       0x108F8

/* PCI error Registers */
#define PCI_ERROR_ADRS_CAPTURE_REG       0x10890
#define PCI_ERROR_DATA_CAPTURE_REG       0x10898
#define PCI_ERROR_CTRL_CAPTURE_REG       0x108A0


/* Setting required for IRQ6 to be configured for PCI*/
#define SIUMCR_PCI_SETTING 0x0e600000

/* Bus parking decides where the bus control sits when idle */
/* If modifying memory controllers for PCI park on the core */
#define PPC_ACR_BUS_PARK_CORE 0x6
#define PPC_ACR_BUS_PARK_PCI  0x3

/* PCI Interrupt registers are at IMMR_BASE + 0x30000 */
/* uses GPCM memory controller*/ 
#define PCI_INT_MEM_BR8_SETTING 0x04731801
/* Smallest window size used and interrupts registers just wrap round */
#define PCI_INT_MEM_OR8_SETTING 0xFFFF8010

/* These windows are ignored if an outbound translation register maps
 * to the same address but are set to the memory mapping on the 826x
 * with PCI.
 */
#define PCIBR0_SETTING 0x80000001
#define PCIBR1_SETTING 0x4800001
#define PCIMSK0_SETTING 0xc0000000
#define PCIMSK1_SETTING 0xff800000
#define PCIBR_ENABLE 0x1

#define PCI_INTERRUPT_STATUS_REG 0x4730000
#define PCI_INTERRUPT_MASK_REG   0x4730004

/* configuration space  register offsets */
#define PIMMR_BASE_ADRS_REGISTER_OFFSET 0x10
#define PIMMR_BASE_ADRS_REGISTER_WIDTH  0x4
#define COMMAND_REGISTER_OFFSET         0x4
#define COMMAND_REGISTER_WIDTH		0x2	       

/* Outbound/Inbound Comparison mask register defines */
#define PCI_WINDOW_ENABLE_BIT   0x80000000
#define PCI_IO_MAP_BIT          0x40000000

#define PCI_SNOOP_ENABLE        0x00000000
#define PCI_SNOOP_DISABLE       0x40000000
#define PCI_PREFETCHABLE        0x20000000

/* Comand status register defines */
#define BUS_MASTER_ENABLE_BIT   0x4
#define MEMORY_SPACE_ACCESS_ENABLE_BIT 0x2

/* PCI_GCR register defines */
#define EXTERNAL_PCI_BUS_ENABLE 0x1

/* PCI Error Handling defines */

#define PCI_ESR_INT_SET         *(VUINT32*)PCI_ESR(INTERNAL_MEM_MAP_ADDR)    
#define PCIERR_MSK_NMI         0x00000800
#define PCIERR_MSK_IRA         0x00000400
#define PCIERR_MSK_I2O_IPQO    0x00000200
#define PCIERR_MSK_I2O_OFQO    0x00000100
#define PCIERR_MSK_PERR_WR     0x00000080
#define PCIERR_MSK_PERR_RD     0x00000040
#define PCIERR_MSK_SERR        0x00000020
#define PCIERR_MSK_TAR_ABT     0x00000010
#define PCIERR_MSK_NO_RSP      0x00000008
#define PCIERR_MSK_DATA_PAR_RD 0x00000004
#define PCIERR_MSK_DATA_PAR_WR 0x00000002
#define PCIERR_MSK_ADDR_PAR    0x00000001

#define PCI_ESR(x)                       (0x10884 | x)
#define PCI_EMR(x)                       (0x10888 | x)
#define PCI_ECR(x)                       (0x1088c | x)

/* PCI DMA engine defines */ 
#define PCI_DMA_SOURCE_MASK 0xffffffc0
#define PCI_DMA_DEST_MASK   0xffffffc0
#define PCI_DMA_MAX_BYTE_COUNT_MASK 0x00ffffff

/* Mode Register */
#define PCI_DMAMR(x,y)                    (UINT32*)((0x10500 | x) + ( y * 128 ))     
/* Status Register */
#define PCI_DMASR(x,y)                    (UINT32*)((0x10504 | x) + ( y * 128 ))
     /* Current Descriptor Address */
#define PCI_DMACDAR(x,y)                  (UINT32*)((0x10508 | x) + ( y * 128 ))
     /* Source Address */
#define PCI_DMASAR(x,y)                   (UINT32*)((0x10510 | x) + ( y * 128 ))
     /*Destination Address */
#define PCI_DMADAR(x,y)                   (UINT32*)((0x10518 | x) + ( y * 128 ))
     /*Byte Count Register */
#define PCI_DMABCR(x,y)                   (UINT32*)((0x10520 | x) + ( y * 128 ))

#define NO_OF_DMA_CHANNELS      4

#define DMAMR_BWC_01_CACHE_LINES 0x0
#define DMAMR_BWC_16_CACHE_LINES (0x100 <<21)
#define DMAMR_SEN                (1<<20)
#define DMAMR_IRQ_CORE           0x00000000
#define DMAMR_IRQ_INTA           (1<<19)
#define DMAMR_EOTIE              (1<<7)
#define DMAMR_CTM_DIRECT         (1<<2)
#define DMAMR_CS                 0x1 
#define DMASR_CB                 (1<<2)
#define DMAMR_CS_MASK            0xfffffffe
#define PCI_ERROR_INT_ENABLED       0x1
#define DMA_INT_ENABLED             0x2
#define MESSAGE_INT_ENABLED         0x4
#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* _INCmot82xxPcih */
