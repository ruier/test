/* tsi108.h - Tsi108 Host Bridge Definitions */

/* Copyright 2005 Tundra Semiconductor Corp. */

/*
modification history
--------------------
01c,20sep07,pmr  fix apigen errors
01b,06dec05,avb  clen up to meet vxWorks coding style requirements
01a,21jul05,avb  written (based on Tsi108 Host Bridge specification)
*/

/*
This file contains definitions for Tsi108 host bridge device.
*/

#ifndef INCtsi108h
#define INCtsi108h

#define TSI108_CSR_BASE_RST	(0xc0000000)

#ifndef TSI108_CSR_BASE
#define TSI108_CSR_BASE		TSI108_CSR_BASE_RST
#endif


/*
 * Functional blocks offsets within Tsi108 register space
 */

#define TSI108_HLP_OFFSET	(0x0000)    /* HLP Interface Block */
#define TSI108_PCI_OFFSET	(0x1000)    /* PCI/PCI-X Interface Block */
#define TSI108_CG_OFFSET	(0x2000)    /* Clock Generator Block */
#define TSI108_PB_OFFSET	(0x3000)    /* Processor Bus Interface Block */
#define TSI108_SDC_OFFSET	(0x4000)    /* SDRAM Memory Controller Block */
#define TSI108_DMA_OFFSET	(0x5000)    /* DMA/XOR Block */
#define TSI108_GIGE_OFFSET	(0x6000)    /* GIGE Controller Block */
#define TSI108_I2C_OFFSET	(0x7000)    /* I2C Interface Block */
#define TSI108_MPIC_OFFSET	(0x7400)    /* Interrupt Controller */
#define TSI108_UART0_OFFSET	(0x7800)    /* UART 0 */
#define TSI108_GPIO_OFFSET	(0x7A00)    /* GPIO Port */
#define TSI108_UART1_OFFSET	(0x7C00)    /* UART 1 */

/*
 * Block identifications used by Tsi108 resisters access macro (defined below)
 */

#define BL_HLP		TSI108_HLP_OFFSET
#define BL_PCI		TSI108_PCI_OFFSET
#define BL_CLKG		TSI108_CG_OFFSET
#define BL_PB		TSI108_PB_OFFSET
#define BL_SDC		TSI108_SDC_OFFSET
#define BL_DMA(x)	(TSI108_DMA_OFFSET + (x)*0x100)
#define BL_GIGE(x)	(TSI108_GIGE_OFFSET + (x)*0x400)
#define BL_I2C		TSI108_I2C_OFFSET
#define BL_MPIC		TSI108_MPIC_OFFSET
#define BL_UART(x)	(TSI108_UART0_OFFSET + (x)*0x400)
#define BL_GPIO		TSI108_GPIO_OFFSET

/*
 * Host Local Port (HLP) Registers (BL_HLP) 
 */

#define HLP_MAX_PORTS			4

/*
 * Registers offsets from the block base
 */

#define HLP_BX_ADDR(x)			(0x0 + 0x10*x)
#define HLP_BX_MASK(x)			(0x4 + 0x10*x)
#define HLP_BX_CTRL0(x)			(0x8 + 0x10*x)
#define HLP_BX_CTRL1(x)			(0xC + 0x10*x)

#define HLP_DATA_SWAP_CTRL		(0x00000040)
#define HLP_ERR				(0x00000044)

/*
 * Register bits and fields
 */

#define HLP_BX_ADDR_A			(0xffffff00)	/* Base Address */

#define HLP_BX_MASK_M			(0xffffff00)	/* Address Mask */

#define HLP_BX_CTRL0_WIDTH		(0x00000003)
#define HLP_BX_CTRL0_FWT		(0x000000fc)
#define HLP_BX_CTRL0_THWR		(0x00000f00)
#define HLP_BX_CTRL0_THRD		(0x0000f000)
#define HLP_BX_CTRL0_WEON		(0x000c0000)
#define HLP_BX_CTRL0_OEON		(0x00300000)
#define HLP_BX_CTRL0_CSON		(0x00c00000)
#define HLP_BX_CTRL0_WAIT		(0x3f000000)
#define HLP_BX_CTRL0_FWE		(0x40000000)
#define HLP_BX_CTRL0_BM			(0x80000000)

#define HLP_BX_CTRL1_PAGE		(0x0000e000)
#define HLP_BX_CTRL1_WEOFF		(0x000f0000)
#define HLP_BX_CTRL1_ARE		(0x01000000)
#define HLP_BX_CTRL1_RE			(0x02000000)
#define HLP_BX_CTRL1_LEHD		(0x0c000000)
#define HLP_BX_CTRL1_LE			(0x30000000)
#define HLP_BX_CTRL1_ENABLE		(0x40000000)
#define HLP_BX_CTRL1_MODE		(0x80000000)
#define HLP_BX_CTRL1_RESERVED		(0x00f01fff)

#define HLP_DATA_SWAP_CTRL_DATA_SWAP	(0x00000003)	/* Swap Ctrl Field */

#define HLP_DATA_SWAP_CTRL_DATA_SWAP_NO	(0x00000000)	/* No Swapping */
#define HLP_DATA_SWAP_CTRL_DATA_SWAP_B	(0x00000001)	/* Byte Swapping */
#define HLP_DATA_SWAP_CTRL_DATA_SWAP_W	(0x00000002)	/* Word Swapping */
#define HLP_DATA_SWAP_CTRL_DATA_SWAP_BW	(0x00000003)	/* Byte-Word Swap */

#define HLP_ERR_TEA			(0x00000001)
#define HLP_ERR_INT_EN			(0x00000002)
#define HLP_ERR_ERR_MSG			(0x0000ff00)
#define HLP_ERR_RESERVED		(0xffff00fc)

/*
 * PCI/PCI-X Bus Block Registers (BL_PCI)
 */

/*
 * Registers offsets from the block base
 */

/* PCI/PCI-X General Configuration Space Registers */
#define PCI_PE_ID			(0x00000000)
#define PCI_PE_CSR			(0x00000004)
#define PCI_PE_CLASS			(0x00000008)
#define PCI_PE_MISC0			(0x0000000c)
#define PCI_P2O_BAR0			(0x00000010)
#define PCI_P2O_BAR0_UPPER		(0x00000014)
#define PCI_P2O_BAR2			(0x00000018)
#define PCI_P2O_BAR2_UPPER		(0x0000001c)
#define PCI_P2O_BAR3			(0x00000020)
#define PCI_P2O_BAR3_UPPER		(0x00000024)
#define PCI_PE_SID			(0x0000002c)
#define PCI_PE_ROM			(0x00000030)
#define PCI_PE_CAP			(0x00000034)
#define PCI_PE_MISC2			(0x0000003c)
#define PCI_MISC_CSR			(0x00000040)
#define PCI_SERR_DIS			(0x00000044)
#define PCI_SERR_STAT			(0x00000048)
#define PCI_P2O_PAGE_SIZES		(0x0000004c)
#define PCI_P_VPD_CSR			(0x00000050)
#define PCI_P_SLOT_ID			(0x000000c8)
#define PCI_P_PMC			(0x000000cc)
#define PCI_P_PMCS			(0x000000d0)
#define PCI_P_HS_CSR			(0x000000d4)
#define PCI_P_VPDC			(0x000000d8)
#define PCI_P_VPDD			(0x000000dc)
#define PCI_P_MSIC			(0x000000e0)
#define PCI_P_MSIA			(0x000000e4)
#define PCI_P_MSIA_UPPER		(0x000000e8)
#define PCI_P_MSID			(0x000000ec)
#define PCI_PE_PCIX_C			(0x000000f0)
#define PCI_PE_PCIX_S			(0x000000f4)

/* device specific */
#define PCI_ARB_CTRL			(0x0000010c)
#define PCI_IRP_CFG_CTL			(0x00000180)
#define PCI_IRP_STAT			(0x00000184)
#define PCI_IRP_ENABLE			(0x00000188)
#define PCI_IRP_INTAD			(0x0000018c)
#define PCI_P_VID_OVERRIDE		(0x0000019c)
#define PCI_P_CLASS_OVERRIDE		(0x000001a0)
#define PCI_PE_SID_OVERRIDE		(0x000001a4)
#define PCI_P_SLOT_ID_OVERRIDE		(0x000001a8)

/* SF device specific */
#define PCI_PFAB_CSR			(0x00000200)
#define PCI_PFAB_BAR0			(0x00000204)
#define PCI_PFAB_BAR0_UPPER		(0x00000208)
#define PCI_PFAB_IO			(0x0000020c)
#define PCI_PFAB_IO_UPPER		(0x00000210)
#define PCI_PFAB_MEM32			(0x00000214)
#define PCI_PFAB_MEM32_REMAP		(0x00000218)
#define PCI_PFAB_MEM32_MASK		(0x0000021c)
#define PCI_PFAB_PFM3			(0x00000220)
#define PCI_PFAB_PFM3_REMAP_UPPER	(0x00000224)
#define PCI_PFAB_PFM3_REMAP_LOWER	(0x00000228)
#define PCI_PFAB_PFM3_MASK		(0x0000022c)
#define PCI_PFAB_PFM4			(0x00000230)
#define PCI_PFAB_PFM4_REMAP_UPPER	(0x00000234)
#define PCI_PFAB_PFM4_REMAP_LOWER	(0x00000238)
#define PCI_PFAB_PFM4_MASK		(0x0000023c)
#define PCI_PFAB_SYNC_BAR		(0x00000240)
#define PCI_PFAB_SYNC_BAR_UPPER		(0x00000244)
#define PCI_EROM_MAP			(0x000002fc)

#define MAX_PCI_LUTS			32

#define PCI_P2O_FAB_BAR2_LUTX(x)	(0x00000500 + (x)*8)
#define PCI_P2O_FAB_BAR2_LUT_UPPERX(x)	(0x00000504 + (x)*8)

#define PCI_P2O_FAB_BAR3_LUTX(x)	(0x00000600 + (x)*8)
#define PCI_P2O_FAB_BAR3_LUT_UPPERX(x)	(0x00000604 + (x)*8)

/*
 * Register bits and fields
 */

/* PCI_IRP_CFG_CTL : Register Bits Masks Definitions */
#define PCI_IRP_CFG_CTL_INTA_DIR	(0x00000001)
#define PCI_IRP_CFG_CTL_INTB_DIR	(0x00000002)
#define PCI_IRP_CFG_CTL_INTC_DIR	(0x00000004)
#define PCI_IRP_CFG_CTL_INTD_DIR	(0x00000008)
#define PCI_IRP_CFG_CTL_LOC_INT_DEST	(0x00000030)
#define PCI_IRP_CFG_CTL_INTA_TYPE	(0x00000300)
#define PCI_IRP_CFG_CTL_INTB_TYPE	(0x00000c00)
#define PCI_IRP_CFG_CTL_INTC_TYPE	(0x00003000)
#define PCI_IRP_CFG_CTL_INTD_TYPE	(0x0000c000)

/* PCI_IRP_STAT : Register Bits Masks Definitions */
#define PCI_IRP_STAT_SPR_TMO		(0x00010000)
#define PCI_IRP_STAT_FAB		(0x00020000)
#define PCI_IRP_STAT_HS_CSR		(0x00100000)
#define PCI_IRP_STAT_P_INT		(0x00400000)
#define PCI_IRP_STAT_P_CSR		(0x00800000)
#define PCI_IRP_STAT_APE		(0x01000000)
#define PCI_IRP_STAT_PW_DPE		(0x02000000)
#define PCI_IRP_STAT_PW_RETRY		(0x04000000)
#define PCI_IRP_STAT_PW_TA		(0x08000000)
#define PCI_IRP_STAT_PW_MA		(0x10000000)
#define PCI_IRP_STAT_DW_ND		(0x20000000)
#define PCI_IRP_STAT_DR_ND		(0x40000000)

/* PCI_IRP_ENABLE : Register Bits Masks Definitions */
#define PCI_IRP_ENABLE_SPR_TMO		(0x00010000)
#define PCI_IRP_ENABLE_FAB		(0x00020000)
#define PCI_IRP_ENABLE_HS_CSR		(0x00100000)
#define PCI_IRP_ENABLE_P_INT		(0x00400000)
#define PCI_IRP_ENABLE_P_CSR		(0x00800000)
#define PCI_IRP_ENABLE_APE		(0x01000000)
#define PCI_IRP_ENABLE_PW_DPE		(0x02000000)
#define PCI_IRP_ENABLE_PW_RETRY		(0x04000000)
#define PCI_IRP_ENABLE_PW_TA		(0x08000000)
#define PCI_IRP_ENABLE_PW_MA		(0x10000000)
#define PCI_IRP_ENABLE_DW_ND		(0x20000000)
#define PCI_IRP_ENABLE_DR_ND		(0x40000000)

/* PCI_IRP_INTAD : Register Bits Masks Definitions */
#define PCI_IRP_INTAD_INTA		(0x00000001)
#define PCI_IRP_INTAD_INTB		(0x00000002)
#define PCI_IRP_INTAD_INTC		(0x00000004)
#define PCI_IRP_INTAD_INTD		(0x00000008)
#define PCI_IRP_INTAD_PME		(0x00000010)
#define PCI_IRP_INTAD_INTA_EN		(0x00010000)
#define PCI_IRP_INTAD_INTB_EN		(0x00020000)
#define PCI_IRP_INTAD_INTC_EN		(0x00040000)
#define PCI_IRP_INTAD_INTD_EN		(0x00080000)

/*
 * Clock Generator Registers (BL_CLKG)
 */

/*
 * Registers offsets from the block base
 */

#define CG_INT_STATUS			(0x00000200)
#define CG_INT_SET			(0x00000204)
#define CG_INT_CTRL			(0x00000208)
#define CG_CLK_CTRL			(0x0000020c)
#define CG_PLL0_CTRL0			(0x00000210)
#define CG_PLL0_CTRL1			(0x00000214)
#define CG_PLL1_CTRL0			(0x00000220)
#define CG_PLL1_CTRL1			(0x00000224)
#define CG_IO_CTRL			(0x00000230)
#define CG_PWRUP_STATUS			(0x00000234)

/*
 * Register bits and fields
 */

#define CG_PWRUP_STATUS_CG_PB_SELECT	(0x00070000)
#define CG_PWRUP_STATUS_CG_SD_SELECT	(0x00700000)

/*
 * Processor Bus Block Registers (BL_PB)
 */

/*
 * Registers offsets from the block base
 */

#define PB_ID				(0x00000000)
#define PB_RSR				(0x00000004)
#define PB_BUS_MS_SELECT		(0x00000008)
#define PB_ISR				(0x0000000c)
#define PB_IER				(0x00000010)
#define PB_INT_SET			(0x00000014)
#define PB_ARB_CTRL			(0x00000018)
#define PB_SCRATCH_PAD			(0x00000020)
#define PB_PVT_CTRL1			(0x00000030)
#define PB_PVT_CTRL2			(0x00000034)
#define PB_PVT_S			(0x00000038)
#define PB_SCR				(0x00000400)
#define PB_ERRCS			(0x00000404)
#define PB_AERR				(0x00000408)
#define PB_REG_BAR			(0x00000410)
#define PB_OCN_BAR1			(0x00000414)
#define PB_OCN_BAR2			(0x00000418)
#define PB_SDRAM_BAR1			(0x0000041c)
#define PB_SDRAM_BAR2			(0x00000420)
#define PB_OCN_SYNC_DATA		(0x00000424)
#define PB_OCN_SYNC_GEN			(0x00000428)

#define PB_BARX_LOWER_LUT_ADDRY(x,y)	(0x00000800 + ((x-1)*0x100) + (y*8))
#define PB_BARX_UPPER_LUT_ADDRY(x,y)	(0x00000804 + ((x-1)*0x100) + (y*8))

#define PB_MCR				(0x00000c00)
#define PB_MCMD				(0x00000c04)

/*
 * Register bits and fields
 */

#define PB_ERRCS_ES			(0x00000002)
#define PB_ISR_PBS_RD_ERR		(0x00000100)

/*
 * SDRAM Controller Block Registers (BL_SDC)
 */

/*
 * Registers offsets from the block base
 */

#define SD_CTRL				(0x00000000)
#define SD_STATUS			(0x00000004)
#define SD_TIMING			(0x00000008)
#define SD_REFRESH			(0x0000000c)
#define SD_INT_STATUS			(0x00000010)
#define SD_INT_ENABLE			(0x00000014)
#define SD_INT_SET			(0x00000018)

#define MAX_SD_DX			2
#define SD_DX_CTRL(x)			(0x20 + x*0x4)
#define SD_DX_BAR(x)			(0x28 + x*0x4)

#define SD_ECC_CTRL			(0x00000040)
#define SD_ECC_STATUS			(0x00000044)
#define SD_ECC_ADDR_LO			(0x00000048)
#define SD_ECC_ADDR_HI			(0x0000004c)
#define SD_PB_ADDR_ERR_LO		(0x00000050)
#define SD_PB_ADDR_ERR_HI		(0x00000054)
#define SD_OCN_ADDR_ERR_LO		(0x00000058)
#define SD_OCN_ADDR_ERR_HI		(0x0000005c)
#define SD_ODT_CFG			(0x00000080)
#define SD_ODT_WRITE_PATTERN		(0x00000090)
#define SD_ODT_READ_PATTERN		(0x00000098)
#define SD_OCD_IA_CTRL			(0x000000a0)
#define SD_OCD_IA_STATUS		(0x000000a4)
#define SD_SINT_STATUS			(0x0000010c)
#define SD_SECC_STATUS			(0x00000144)
#define SD_SECC_ADDR_LO			(0x00000148)
#define SD_SECC_ADDR_HI			(0x0000014c)

#define MAX_SD_DQS			18
#define SD_DQSX_DELAY(x)		(0x200+ x*0x4)

#define SD_FB_DELAY			(0x00000248)
#define SD_ADV_CLK_DELAY		(0x0000024c)
#define SD_DLL_STATUS			(0x00000250)
#define SD_I2C_CTRL1			(0x00000400)
#define SD_I2C_CTRL2			(0x00000404)
#define SD_I2C_RD_DATA			(0x00000408)
#define SD_I2C_WRT_DATA			(0x0000040c)

/*
 * I2C Interface Registers (BL_I2C and within SDC)
 */

/*
 * Registers offsets from the block base
 */

#define I2C_CNTRL1			(0x00000000)
#define I2C_CNTRL2			(0x00000004)
#define I2C_RD_DATA			(0x00000008)
#define I2C_TX_DATA			(0x0000000c)

/*
 * Register bits and fields
 */

#define I2C_CNTRL1_DEVCODE		(0x0000000f)
#define I2C_CNTRL1_PAGE			(0x00000700)
#define I2C_CNTRL1_BYTADDR		(0x00ff0000)
#define I2C_CNTRL1_I2CWRITE		(0x01000000)

#define I2C_CNTRL2_SIZE			(0x00000003)
#define I2C_CNTRL2_LANE			(0x0000000c)
#define I2C_CNTRL2_MULTIBYTE		(0x00000010)
#define I2C_CNTRL2_START		(0x00000100)
#define I2C_CNTRL2_WR_STATUS		(0x00010000)
#define I2C_CNTRL2_RD_STATUS		(0x00020000)
#define I2C_CNTRL2_I2C_TO_ERR		(0x04000000)
#define I2C_CNTRL2_I2C_CFGERR		(0x08000000)
#define I2C_CNTRL2_I2C_CMPLT		(0x10000000)



#ifndef	_ASMLANGUAGE

/*
 * Macros to read/write from/to Tsi108 registers.
 * 
 * Definitions of individual block specific register blocks are provided above
 *
 * Examples:
 *   1. Read status register of DMA channel 1:
 *         reg_value = *TSI108_REG_PTR(BL_DMA(1), DMA_CHX_GCSR);
 *
 *   2. Set all four dorbell bits in MPICs doorbell activation register:
 *         TSI108_REG_WR(BL_MPIC, MPIC_INT_DAR, 0x000F);
 *
 */

#define TSI108_REG_PTR(blk,reg) \
		((volatile unsigned long *)(TSI108_CSR_BASE + (blk) + (reg)))

#define TSI108_REG_RD(blk,x) \
		*((volatile UINT32*)(TSI108_CSR_BASE + (blk) + (x)))

#define TSI108_REG_WR(blk,x,val) \
		do { \
		*((volatile UINT32*)(TSI108_CSR_BASE + (blk) + (x))) = val; \
		EIEIO; \
		} while((0))

#ifndef EIEIO_SYNC
#   define EIEIO_SYNC	WRS_ASM ("eieio; sync")
#endif  /* EIEIO_SYNC */

#ifndef EIEIO
#   define EIEIO	WRS_ASM ("eieio")
#endif	/* EIEIO */

#ifndef SYNC
#   define SYNC		WRS_ASM ("sync") 
#endif  /* SYNC */

#endif /* _ASMLANGUAGE */

#endif  /* INCtsi108h */

