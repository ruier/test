/* mpc8540Dma.h - MPC8540 DMA Controller Header File */

/* Copyright 2005 Motorola, Inc., All Rights Reserved */

/*
modification history
--------------------
01a,06mar05,cak  New. Base on mv64360Dma.h features (01c,03nov03,mv6100).
*/

/*
DESCRIPTION
This file contains the MPC8540 DMA register bit definitions, masks, and 
typical values. Register offsets are defined in the main MPC8540 header file.
*/

#ifndef	INCmpc8540Dmah
#define	INCmpc8540Dmah

#ifdef __cplusplus
   extern "C" {
#endif /* __cplusplus */

/* includes */

/* Maximum counts */

#define DMA_CHANNEL_COUNT        4

/* Interrupt vectors for each DMA channel */

#define DMA_CHAN0_COMPLETION_INT_VEC	EPIC_DMA0_INT_VEC 
#define DMA_CHAN1_COMPLETION_INT_VEC	EPIC_DMA1_INT_VEC	
#define DMA_CHAN2_COMPLETION_INT_VEC	EPIC_DMA2_INT_VEC	
#define DMA_CHAN3_COMPLETION_INT_VEC	EPIC_DMA3_INT_VEC 

/* Mode Register */

#define DMA_MRn_BWC_BIT		24
#define DMA_MRn_BWC_MASK	(0xF << DMA_MRn_BWC_BIT)
#define DMA_MRn_EMP_EN_BIT	21	
#define DMA_MRn_EMP_EN_MASK	(1 << DMA_MRn_EMP_EN_BIT)
#define DMA_MRn_EMS_EN_BIT	18
#define DMA_MRn_EMS_EN_MASK	(1 << DMA_MRn_EMS_EN_BIT)
#define DMA_MRn_DAHTS_BIT	16
#define DMA_MRn_DAHTS_MASK	(0x3 << DMA_MRn_DAHTS_BIT)
#define DMA_MRn_SAHTS_BIT	14
#define DMA_MRn_SAHTS_MASK	(0x3 << DMA_MRn_SAHTS_BIT)
#define DMA_MRn_DAHE_BIT	13
#define DMA_MRn_DAHE_MASK	(1 << DMA_MRn_DAHE_BIT)
#define DMA_MRn_SAHE_BIT	12
#define DMA_MRn_SAHE_MASK	(1 << DMA_MRn_SAHE_BIT)
#define DMA_MRn_SRW_BIT		10
#define DMA_MRn_SRW_MASK	(1 << DMA_MRn_SRW_BIT)
#define DMA_MRn_EOSIE_BIT	9
#define DMA_MRn_EOSIE_MASK	(1 << DMA_MRn_EOSIE_BIT)
#define DMA_MRn_EOLNIE_BIT	8
#define DMA_MRn_EOLNIE_MASK	(1 << DMA_MRn_EOLNIE_BIT)
#define DMA_MRn_EOLSIE_BIT	7
#define DMA_MRn_EOLSIE_MASK	(1 << DMA_MRn_EOLSIE_BIT)
#define DMA_MRn_EIE_BIT		6
#define DMA_MRn_EIE_MASK	(1 << DMA_MRn_EIE_BIT)
#define DMA_MRn_XFE_BIT		5
#define DMA_MRn_XFE_MASK	(1 << DMA_MRn_XFE_BIT)
#define DMA_MRn_CDSM_SWSM_BIT	4
#define DMA_MRn_CDSM_SWSM_MASK	(1 << DMA_MRn_CDSM_SWSM_BIT)
#define DMA_MRn_CA_BIT		3
#define DMA_MRn_CA_MASK		(1 << DMA_MRn_CA_BIT)
#define DMA_MRn_CTM_BIT		2
#define DMA_MRn_CTM_MASK	(1 << DMA_MRn_CTM_BIT)
#define DMA_MRn_CC_BIT		1
#define DMA_MRn_CC_MASK		(1 << DMA_MRn_CC_BIT)
#define DMA_MRn_CS_BIT		0
#define DMA_MRn_CS_MASK		(1 << DMA_MRn_CS_BIT)

/* Status Register */

#define DMA_SRn_TE_BIT		7
#define DMA_SRn_TE_MASK		(1 << DMA_SRn_TE_BIT)
#define DMA_SRn_CH_BIT		5
#define DMA_SRn_CH_MASK		(1 << DMA_SRn_CH_BIT)
#define DMA_SRn_PE_BIT		4
#define DMA_SRn_PE_MASK		(1 << DMA_SRn_PE_BIT)
#define DMA_SRn_EOLNI_BIT	3
#define DMA_SRn_EOLNI_MASK	(1 << DMA_SRn_EOLNI_BIT)
#define DMA_SRn_CB_BIT		2
#define DMA_SRn_CB_MASK		(1 << DMA_SRn_CB_BIT)
#define DMA_SRn_EOSI_BIT	1
#define DMA_SRn_EOSI_MASK	(1 << DMA_SRn_EOSI_BIT)
#define DMA_SRn_EOLSI_BIT	0
#define DMA_SRn_EOLSI_MASK	(1 << DMA_SRn_EOLSI_BIT)

/* Current Link Descriptor Address Register */

#define DMA_CLNDARn_CLNDA_BIT		5
#define DMA_CLNDARn_CLNDA_MASK		(0x7FFFFFF  << DMA_CLNDARn_CLNDA_BIT)
#define DMA_CLNDARn_EOSIE_BIT		3
#define DMA_CLNDARn_EOSIE_MASK		(1 << DMA_CLNDARn_EOSIE_BIT)

/* Source Attributes Register */

#define DMA_SATRn_SBPATMU_BIT		29
#define DMA_SATRn_SBPATMU_MASK		(1 << DMA_SATRn_SBPATMU_BIT)
#define DMA_SATRn_STFLOWLVL_BIT		26
#define DMA_SATRn_STFLOWLVL_MASK	(0x3 << DMA_SATRn_STFLOWLVL_BIT)
#define DMA_SATRn_SPCIORDER_BIT		25
#define DMA_SATRn_SPCIORDER_MASK	(1 << DMA_SATRn_SPCIORDER_BIT)
#define DMA_SATRn_SSME_BIT		24
#define DMA_SATRn_SSME_MASK		(1 << DMA_SATRn_SSME_BIT)
#define DMA_SATRn_STRANSINT_BIT		20
#define DMA_SATRn_STRANSINT_MASK	(0xF << DMA_SATRn_STRANSINT_BIT)
#define DMA_SATRn_SREADTTYPE_BIT	16
#define DMA_SATRn_SREADTTYPE_MASK	(0xF << DMA_SATRn_SREADTTYPE_BIT)
#define DMA_SATRn_ESAD_BIT		0
#define DMA_SATRn_ESAD_MASK		(0x3FF << DMA_SATRn_ESAD_BIT)

/* Source Address Register */

#define DMA_SARn_SAD_BIT		0
#define DMA_SARn_SAD_MASK		(0xFFFFFFFF << DMA_SARn_SAD_BIT)

/* Destination Attributes Register */

#define DMA_DATRn_DBPATMU_BIT		29
#define DMA_DATRn_DBPATMU_MASK		(1 << DMA_DATRn_DBPATMU_BIT)
#define DMA_DATRn_DTFLOWLVL_BIT		26
#define DMA_DATRn_DTFLOWLVL_MASK	(0x3 << DMA_DATRn_DTFLOWLVL_BIT)
#define DMA_DATRn_DPCIORDER_BIT		25	
#define DMA_DATRn_DPCIORDER_MASK	(1 << DMA_DATRn_DPCIORDER_BIT)
#define DMA_DATRn_DSME_BIT		24
#define DMA_DATRn_DSME_MASK		(1 << DMA_DATRn_DSME_BIT)
#define DMA_DATRn_DTRANSINT_BIT		20
#define DMA_DATRn_DTRANSINT_MASK	(0xF << DMA_DATRn_DTRANSINT_BIT)
#define DMA_DATRn_DWRITETYPE_BIT	16
#define DMA_DATRn_DWRITETYPE_MASK	(0xF << DMA_DATRn_DWRITETYPE_BIT)
#define DMA_DATRn_EDAD_BIT		0
#define DMA_DATRn_EDAD_MASK		(0x3FF << DMA_DATRn_EDAD_BIT)

/* Destination Address Register */

#define DMA_DARn_DAD_BIT		0
#define DMA_DARn_DAD_MASK		(0xFFFFFFFF << DMA_DARn_DAD_BIT)

/* Byte Count Register */

#define DMA_BYTE_COUNT_MASK	0x03FFFFFF

/* Next Link Descriptor Address Register */

#define DMA_NLNDARn_NLNDA_BIT		5
#define DMA_NLNDARn_NDEOSIE_BIT		3
#define DMA_NLNDARn_NDEOSIE_MASK	(1 << DMA_NLNDARn_NDEOSIE_BIT)
#define DMA_NLNDARn_EOLND_BIT		0
#define DMA_NLNDARn_EOLND_MASK		(1 << DMA_NLNDARn_EOLND_BIT)

/* Next List Descriptor Address Register */

#define DMA_NLSDARn_NLSDA_BIT		5
#define DMA_NLSDARn_EOLSD_BIT		0
#define DMA_NLSDARn_EOLSD_MASK		(1 << DMA_NLSDARn_EOLSD_BIT)

/* Source Stride Register */

#define DMA_SSRn_SSS_BIT		12
#define DMA_SSRn_SSS_MASK		(0xFFF << DMA_SSRn_SSS_BIT)
#define DMA_SSRn_SSD_BIT		0
#define DMA_SSRn_SSD_MASK		(0xFFF << DMA_SSRn_SSD_BIT)

/* Destination Stride Register */

#define DMA_DSRn_DSS_BIT		12
#define DMA_DSRn_DSS_MASK		(0xFFF << DMA_DSRn_DSS_BIT)
#define DMA_DSRn_DSD_BIT		0
#define DMA_DSRn_DSD_MASK		(0xFFF << DMA_DSRn_DSD_BIT)

/* General Status Register */

#define	DMA_DGSR_TE_BIT			31	
#define DMA_DGSR_TE_MASK		(1 << DMA_DGSR_TE_BIT)
#define DMA_DGSR_CH_BIT			29	
#define DMA_DGSR_CH_MASK		(1 << DMA_DGSR_CH_BIT)
#define DMA_DGSR_PE_BIT			28	
#define DMA_DGSR_PE_MASK		(1 << DMA_DGSR_PE_BIT)
#define DMA_DGSR_EOLNI_BIT		27	
#define DMA_DGSR_EOLNI_MASK		(1 << DMA_DGSR_EOLNI_BIT)
#define DMA_DGSR_CB_BIT			26	
#define DMA_DGSR_CB_MASK		(1 << DMA_DGSR_CB_BIT)
#define DMA_DGSR_EOSI_BIT		25	
#define DMA_DGSR_EOSI_MASK		(1 << DMA_DGSR_EOSI_BIT)
#define DMA_DGSR_EOLSI_BIT		24	
#define DMA_DGSR_EOLSI_MASK		(1 << DMA_DGSR_EOLSI_BIT)
#define DMA_DGSR_MASK			(0xFF000000)

/*
 * The following structure defines the DMA link descriptor.
 */

typedef struct sysDmaLinkDesc
    {
    UINT32 sourceAttr;		/* Source Attributes */
    UINT32 sourceAddr;		/* Source Address */
    UINT32 destinAttr;		/* Destination Attributes */
    UINT32 destinAddr;		/* Destination Address */
    UINT32 reserved1;		/* Reserved */
    UINT32 nextDescPtrAddr;	/* Next Descriptor Pointer Address */
    UINT32 byteCount;		/* Number of bytes to transfer */
    UINT32 reserved2;		/* Reserved */
    } DMA_LINK_DESCRIPTOR;

/*
 * The following structure defines the DMA list descriptor.
 */

typedef struct sysDmaListDesc
    {
    UINT32 reserved1;		/* Reserved */
    UINT32 nextListDescPtrAddr;	/* Next List Descriptor Pointer Address */
    UINT32 reserved2;		/* Reserved */
    UINT32 firstLinkDescPrtAddr;/* First Link Descriptor Pointer Address */
    UINT32 sourceStride;	/* Source Stride */
    UINT32 destStride;		/* Destination Stride */
    UINT32 reserved3;		/* Reserved */
    UINT32 reserved4;		/* Reserved */
    } DMA_LIST_DESCRIPTOR;

/*
 * The following structure defines user controlled attributes for a
 * given DMA transfer. 
 */

typedef void (*HANDLER)(UINT32 chan);
typedef struct sysDmaAttrib
    {
    UINT32 sourceStride;	/* Source stride */
    UINT32 destStride;		/* Destination Stride */
    HANDLER userEosiHandler;	/* User defined EOSI routine */
    HANDLER userEolniHandler;	/* User defined EOLNI routine */
    HANDLER userEolsiHandler;	/* User defined EOLSI routine */
    HANDLER userErrorHandler;	/* User defined error handling routine */
    BOOL waitForCompletion;	/* FLAG to wait for DMA completion */
    BOOL holdSourceAddr;	/* Do not increment the source address */
    UCHAR holdSourceAddrSize;	/* source address hold transfer size */
    BOOL holdDestAddr;		/* Do not increment the destination address */
    UCHAR holdDestAddrSize;	/* destination address hold transfer size */
    } DMA_ATTRIBUTES;

/*
 * The following structure defines DMA result information for the last
 * successful DMA transfer or the last error for a particular channel.
 * Provides a snapshot of the registers for a given DMA channel. 
 */
 
typedef struct mpc8540DmaStatus
    {
    UINT32 modeReg;		/* Mode Register */
    UINT32 statusReg;		/* Status Register */
    UINT32 currLinkDescAddrReg;	/* current link descriptor address register */
    UINT32 srcAttrReg;		/* source attributes register */
    UINT32 srcAddrReg;		/* source address register */
    UINT32 destAttrReg;		/* destination attributes register */
    UINT32 destAddrReg;		/* destination address register */
    UINT32 byteCountReg;	/* byte count register */
    UINT32 nextLinkDescAddrReg;	/* next link descriptor address register */
    UINT32 currListDescAddrReg;	/* current list descriptor address register */
    UINT32 nextListDescAddrReg;	/* next list descriptor address register */
    UINT32 srcStrideReg;	/* source stride register */
    UINT32 destStrideReg;	/* destination stride register */ 
    STATUS dmaStatus;		/* OK when DMA completes successfully */
    } DMA_STATUS;

STATUS sysMpc8540ChannelContinueSet (UINT32 chan);
STATUS sysMpc8540DmaSetBandwidth (UINT32 chan, UCHAR bandwidth);
STATUS sysMpc8540DmaInit (void);
STATUS sysMpc8540DmaStart (UINT32 chan, DMA_LINK_DESCRIPTOR *dmaLinkDesc, 
			   DMA_LIST_DESCRIPTOR *dmaListDesc,
                           DMA_ATTRIBUTES *dmaAttrib, DMA_STATUS *dmaStatus);
STATUS sysMpc8540DmaStatus (UINT32 chan, DMA_STATUS *dmaStatus);
STATUS sysMpc8540DmaAbort (UINT32 chan);
STATUS sysMpc8540DmaPause (UINT32 chan);
STATUS sysMpc8540DmaResume (UINT32 chan);

#ifdef INCLUDE_MPC8540_DMA_TEST
STATUS strideDataCompare (UINT32 destStrideSize, UINT32 destStrideDist,
			  UINT32 srcStrideSize, UINT32 srcStrideDist,
			  UINT32 byteCount, UCHAR * srcBuffer,
			  UCHAR * destBuffer);
STATUS holdDataCompare (BOOL holdSrcAddr, BOOL holdDestAddr,
			UCHAR holdSrcAddrSize, UCHAR holdDestAddrSize,
			UINT32 byteCount, UCHAR * srcBuffer,
			UCHAR * destBuffer);
STATUS dataCompare (UCHAR *expAddr, UCHAR *compAddr, UINT32 count);
void dataFill (UCHAR *buffAddr, UINT32 count);
void displayStatus (DMA_STATUS * dmaStatus);
STATUS sysMpc8540DmaTest (void);
LOCAL void userErrorHandleInt (int chan);
LOCAL void userEolsiHandleInt (int chan);
LOCAL void userEolniHandleInt (int chan);
LOCAL void userEosiHandleInt (int chan);
#endif /* INCLUDE_MPC8540_DMA_TEST */

#ifdef __cplusplus
    }
#endif /* __cplusplus */

#endif /* INCmpc8540Dmah */
