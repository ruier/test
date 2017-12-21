/* sysTsi108Dma.h - Tundra TSI108 DMA Engine definitions */

/* Copyright 2005 Tundra Semiconductor Corp. */

/*
modification history
--------------------
01c,20sep07,pmr  fix apigen errors
01b,05dec05,avb  clean up to meet vxWorks coding style requirements
01a,12aug05,avb  written based on Tsi108 specification
*/

/*
Description:

Contains definitions for the Tundra Tsi108 DMA Engine library
*/


#ifndef	__INCsysTsi108Dma
#define	__INCsysTsi108Dma

#ifdef __cplusplus
extern "C" {
#endif

/* includes */

/* structures */

#ifndef	_ASMLANGUAGE

typedef struct _tsi108DmaDesc
    {
    UINT32 srcAddressLow;	/* Source Address (low 32 bits) */
    UINT32 srcAddressHigh;	/* Source Address (high 32 bits) */
    UINT32 dstAddressLow;	/* Destination Address (low 32 bits) */
    UINT32 dstAddressHigh;	/* Destination Address (high 32 bits) */
    UINT32 tcr1;		/* TCR1 register image */
    UINT32 tcr2;		/* TCR2 register image */
    UINT32 nextDescAddressLow;	/* Next Descriptor Address (low 32 bits) */
    UINT32 nextDescAddressHigh;	/* Next Descriptor Address (high 32 bits) */
    UINT32 nextDescTCR;		/* Next Descriptor TCR */
    UINT32 crcAddressLow;	/* CRC Address Low 32 bits */
    UINT32 crcAddressHigh;	/* CRC Address High 32 bits */
    } TSI108_DMA_DESCRIPTOR;

#define TSI108_DMA_DESC_ALIGN	64  /* DMA descriptor aligned to 64 bytes */

#endif	/* _ASMLANGUAGE */

/*
 * Tsi108 Port Numbers
 * NOTE: Only the Tsi108 ports that can be used for DMA data transfers 
 *       are defined below. 
 */
#define TSI108_PORT_HLP		0
#define TSI108_PORT_PCI		1
#define TSI108_PORT_PBM		2
#define TSI108_PORT_SDC		4

/*
 * Tsi108 DMA Operation Commands
 */
#define TSI108_OP_DMA		0
#define TSI108_OP_XOR		1
#define TSI108_OP_MFILL		2
#define TSI108_OP_ZERO		3


#define MAX_DMA_PORTS		4

/*
 * DMA/XOR Block Registers ( BL_DMA(x) )
 */

#define DMA_CHX_SRC_ADDR_M		(0x000)
#define DMA_CHX_SRC_ADDR_L		(0x004)
#define DMA_CHX_DST_ADDR_M		(0x008)
#define DMA_CHX_DST_ADDR_L		(0x00c)
#define DMA_CHX_TCR1			(0x010)
#define DMA_CHX_TCR2			(0x014)
#define DMA_CHX_ND_ADDR_M		(0x018)
#define DMA_CHX_ND_ADDR_L		(0x01c)
#define DMA_CHX_ND_TCR			(0x020)
#define DMA_CHX_GCSR			(0x024)
#define DMA_CHX_CRC_ADDR_M		(0x028)
#define DMA_CHX_CRC_ADDR_L		(0x02c)
#define DMA_CHX_CRC			(0x030)

/*
 * XOR source address registers (x = dma chan num; y=SRC num)
 * NOTE: first source register index for XOR is 1 not 0
 */

#define MAX_SRC				15
#define DMA_CHX_SRCX_ADDR_M(y)		(0x040 + ((y-1)*8))
#define DMA_CHX_SRCX_ADDR_L(y)		(0x044 + ((y-1)*8))

#define DMA_DMA_OCN_SOFT_RST		(0x00000400)

/*
 * Register bits and fields
 */

#define DMA_CHX_TCR1_DATA_BLOCKS	(0x0000000f)
#define DMA_CHX_TCR1_DIRECT_FILL	(0x00000020)
#define DMA_CHX_TCR1_DST_PORT		(0x00003f00)
#define DMA_CHX_TCR1_DIS_DMA_WR		(0x00008000)
#define DMA_CHX_TCR1_SRC_PORT		(0x003f0000)
#define DMA_CHX_TCR1_READ_CRC_SEED	(0x00400000)
#define DMA_CHX_TCR1_CRC_WR_EN		(0x00800000)
#define DMA_CHX_TCR1_CRC_PORT		(0x3f000000)
#define DMA_CHX_TCR1_CRC_EN		(0x40000000)
#define DMA_CHX_TCR1_XOR_WR_EN		(0x80000000)

#define DMA_CHX_TCR2_BC			(0x00ffffff)
#define DMA_CHX_TCR2_SWAP		(0x03000000)
#define DMA_CHX_TCR2_BURST_SIZE		(0x0c000000)
#define DMA_CHX_TCR2_CRC_SWAP		(0x30000000)
#define DMA_CHX_TCR2_CRC_ORIENT		(0x80000000)

#define DMA_CHX_ND_ADDR_L_LAST		(0x00000001)
#define DMA_CHX_ND_ADDR_L_ND_ADDR_LB	(0xffffffc0)

#define DMA_CHX_ND_TCR_RESET_VALUE	(0x00030000)

#define DMA_CHX_ND_TCR_ND_DATA_BLOCKS	(0x000f0000)
#define DMA_CHX_ND_TCR_ND_PORT		(0x3f000000)
#define DMA_CHX_ND_TCR_ND_SWAP		(0xc0000000)

#define DMA_CHX_GCSR_DONE_EN		(0x00000001)
#define DMA_CHX_GCSR_HALT_EN		(0x00000002)
#define DMA_CHX_GCSR_STOP_EN		(0x00000004)
#define DMA_CHX_GCSR_EX_PKT_EN		(0x00000008)
#define DMA_CHX_GCSR_ERR_EN		(0x00000010)
#define DMA_CHX_GCSR_PERR_EN		(0x00000020)
#define DMA_CHX_GCSR_PERR		(0x00000040)
#define DMA_CHX_GCSR_EX_PKT_ERR		(0x00000080)
#define DMA_CHX_GCSR_DONE		(0x00000100)
#define DMA_CHX_GCSR_HALT		(0x00000200)
#define DMA_CHX_GCSR_STOP		(0x00000400)
#define DMA_CHX_GCSR_RESUME		(0x00000800)
#define DMA_CHX_GCSR_ERR_OUT		(0x00001000)
#define DMA_CHX_GCSR_ERR_IN		(0x00002000)
#define DMA_CHX_GCSR_DACT		(0x00800000)
#define DMA_CHX_GCSR_SOFT_RST		(0x01000000)
#define DMA_CHX_GCSR_HALT_REQ		(0x02000000)
#define DMA_CHX_GCSR_STOP_REQ		(0x04000000)
#define DMA_CHX_GCSR_OP_CMD		(0x30000000)
#define DMA_CHX_GCSR_CHAIN		(0x40000000)
#define DMA_CHX_GCSR_GO			(0x80000000)

#define DMA_CHX_SRCX_ADDR_L_SRC_ADDR_LB_R   (0x00000007)
#define DMA_CHX_SRCX_ADDR_L_SRC_ADDR_LB	    (0xfffffff8)

#define DMA_DMA_OCN_SOFT_RST_OCN_SOFT_RST   (0x00000001)

#ifdef __cplusplus
}
#endif

#endif	/* __INCsysTsi108Dma */

