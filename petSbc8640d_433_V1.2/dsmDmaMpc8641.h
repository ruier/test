/* Includes */
#include "wrSbc8641.h"
#ifndef _DSM_DMA_MPC8641_H
#define _DSM_DMA_MPC8641_H

/* Macros & Constants */
#define DMA8641_CHAN_CNT    4

/* ------------------------------------------------- */
/* Channel control register related macros           */
/* ------------------------------------------------- */
/* Bandwidth/Pause control */
#define DMA8641_MR_BWC_MASK         (0xF<<24)
#define DMA8641_MR_BWC_1BYTE        (0x0<<24)
#define DMA8641_MR_BWC_2BYTES       (0x1<<24)
#define DMA8641_MR_BWC_4BYTES       (0x2<<24)
#define DMA8641_MR_BWC_8BYTES       (0x3<<24)
#define DMA8641_MR_BWC_16BYTES      (0x4<<24)
#define DMA8641_MR_BWC_32BYTES      (0x5<<24)
#define DMA8641_MR_BWC_64BYTES      (0x6<<24)
#define DMA8641_MR_BWC_128BYTES     (0x7<<24)
#define DMA8641_MR_BWC_256BYTES     (0x8<<24)
#define DMA8641_MR_BWC_512BYTES     (0x9<<24)
#define DMA8641_MR_BWC_1024BYTES    (0xA<<24)
#define DMA8641_MR_BWC_DISABLE      (0xF<<24)

/* External master pause enable/disable */
#define DMA8641_MR_EMP_EN_MASK      (0x1<<21)
#define DMA8641_MR_EMP_EN_DISABLE   (0x0<<21)
#define DMA8641_MR_EMP_EN_ENABLE    (0x1<<21)

/* External master start enable */
#define DMA8641_MR_EMS_EN_MASK      (0x1<<18)
#define DMA8641_MR_EMS_EN_DISABLE   (0x0<<18)
#define DMA8641_MR_EMS_EN_ENABLE    (0x1<<18)

/* Destination address hold transfer size */
#define DMA8641_MR_DAHTS_MASK       (0x3<<16)
#define DMA8641_MR_DAHTS_1BYTE      (0x0<<16)
#define DMA8641_MR_DAHTS_2BYTES     (0x1<<16)
#define DMA8641_MR_DAHTS_4BYTES     (0x2<<16)
#define DMA8641_MR_DAHTS_8BYTES     (0x3<<16)

/* Source address hold transfer size */
#define DMA8641_MR_SAHTS_MASK       (0x3<<14)
#define DMA8641_MR_SAHTS_1BYTE      (0x0<<14)
#define DMA8641_MR_SAHTS_2BYTES     (0x1<<14)
#define DMA8641_MR_SAHTS_4BYTES     (0x2<<14)
#define DMA8641_MR_SAHTS_8BYTES     (0x3<<14)

/* Destination address hold enable/disable */
#define DMA8641_MR_DAHE_MASK        (0x1<<13)
#define DMA8641_MR_DAHE_DISABLE     (0x0<<13)
#define DMA8641_MR_DAHE_ENABLE      (0x1<<13)

/* Source Address hold enable/disable */
#define DMA8641_MR_SAHE_MASK        (0x1<<12)
#define DMA8641_MR_SAHE_DISABLE     (0x0<<12)
#define DMA8641_MR_SAHE_ENABLE      (0x1<<12)

/* Single Register write (direct mode only) */
#define DMA8641_MR_SRW_MASK         (0x1<<10)
#define DMA8641_MR_SRW_DISABLE      (0x0<<10)
#define DMA8641_MR_SRW_ENABLE       (0x1<<10)

/* End of segment interrupt enable/disable */
#define DMA8641_MR_EOSIE_MASK       (0x1<<9)
#define DMA8641_MR_EOSIE_DISABLE    (0x0<<9)
#define DMA8641_MR_EOSIE_ENABLE     (0x1<<9)

/* End of links interrupt enable/disable */
#define DMA8641_MR_EOLNIE_MASK      (0x1<<8)
#define DMA8641_MR_EOLNIE_DISABLE   (0x0<<8)
#define DMA8641_MR_EOLNIE_ENABLE    (0x1<<8)

/* End of lists interrupt enable/disable */
#define DMA8641_MR_EOLSIE_MASK      (0x1<<7)
#define DMA8641_MR_EOLSIE_DISABLE   (0x0<<7)
#define DMA8641_MR_EOLSIE_ENABLE    (0x1<<7)

/* Error interrupt enable/disable */
#define DMA8641_MR_EIE_MASK         (0x1<<6)
#define DMA8641_MR_EIE_DISABLE      (0x0<<6)
#define DMA8641_MR_EIE_ENABLE       (0x1<<6)

/* Extended feature enable/disable */
#define DMA8641_MR_XFE_MASK         (0x1<<5)
#define DMA8641_MR_XFE_DISABLE      (0x0<<5)
#define DMA8641_MR_XFE_ENABLE       (0x1<<5)

/* Current Descriptor Start Mode (Chaining only)*/
#define DMA8641_MR_CDSM_MASK        (0x1<<4)
#define DMA8641_MR_CDSM_DISABLE     (0x0<<4)
#define DMA8641_MR_CDSM_ENABLE      (0x1<<4)

/* Single Write Start Mode (Direct mode only) */
#define DMA8641_MR_SWSM_MASK        (0x1<<4)
#define DMA8641_MR_SWSM_DST         (0x0<<4)
#define DMA8641_MR_SWSM_SRC         (0x1<<4)

/* Channel Abort */
#define DMA8641_MR_CA_MASK          (0x1<<3)
#define DMA8641_MR_CA_NO            (0x0<<3)
#define DMA8641_MR_CA_YES           (0x1<<3)

/* Channel transfer mode */
#define DMA8641_MR_CTM_MASK         (0x1<<2)
#define DMA8641_MR_CTM_CHAINING     (0x0<<2)
#define DMA8641_MR_CTM_DIRECT       (0x1<<2)

/* Channel Continue */
#define DMA8641_MR_CC_MASK          (0x1<<1)
#define DMA8641_MR_CC_NO            (0x0<<1)
#define DMA8641_MR_CC_YES           (0x1<<1)

/* Channel halt/start */
#define DMA8641_MR_CS_MASK          (0x1<<0)
#define DMA8641_MR_CS_HALT          (0x0<<0)
#define DMA8641_MR_CS_START         (0x1<<0)

/* ------------------------------------------------- */
/*  Channel status register related macros           */
/* ------------------------------------------------- */
/* Transfer error (Write 1 to reset bit) */
#define DMA8641_SR_TE_MASK          (0x1<<7)
#define DMA8641_SR_TE_NOERROR       (0x0<<7)
#define DMA8641_SR_TE_ERROR         (0x1<<7)

/* Channel Halted */
#define DMA8641_SR_CH_MASK          (0x1<<5)
#define DMA8641_SR_CH_NOTHALTED     (0x0<<5)
#define DMA8641_SR_CH_HALTED        (0x1<<5)

/* Programming error (Write 1 to reset bit) */
#define DMA8641_SR_PE_MASK          (0x1<<4)
#define DMA8641_SR_PE_NOERROR       (0x0<<4)
#define DMA8641_SR_PE_ERROR         (0x1<<4)

/* End-of-Links interrupt (Write 1 to reset bit) */
#define DMA8641_SR_EOLNI_MASK       (0x1<<3)
#define DMA8641_SR_EOLNI_NO         (0x0<<3)
#define DMA8641_SR_EOLNI_ACTIVE     (0x1<<3)

/* Channel busy status  */
#define DMA8641_SR_CB_MASK          (0x1<<2)
#define DMA8641_SR_CB_IDLE          (0x0<<2)
#define DMA8641_SR_CB_BUSY          (0x1<<2)

/* End-of-Segment interrupt (Write 1 to reset bit) */
#define DMA8641_SR_EOSI_MASK        (0x1<<1)
#define DMA8641_SR_EOSI_NO          (0x0<<1)
#define DMA8641_SR_EOSI_ACTIVE      (0x1<<1)

/* End-of-List interrupt (Write 1 to reset bit) */
#define DMA8641_SR_EOLSI_MASK        (0x1<<0)
#define DMA8641_SR_EOLSI_NO          (0x0<<0)
#define DMA8641_SR_EOLSI_ACTIVE      (0x1<<0)

/* ------------------------------------------------- */
/* Current Link Descriptor Address Register (Ext.)   */
/* ------------------------------------------------- */
/* Extended address upper 4 bits*/
#define DMA8641_ECLNDAR_ECLNDA_MASK    (0x0000000F)

/* ------------------------------------------------- */
/* Current Link Descriptor Address Register          */
/* ------------------------------------------------- */
/* Address bits 31-5 */
#define DMA8641_CLNDAR_CLNDA_MASK      (0xFFFFFFE0)
/* End of segment interrupt enable */
#define DMA8641_CLNDAR_EOSIE_MASK       (0x1<<2)
#define DMA8641_CLNDAR_EOSIE_DISABLE    (0x0<<2)
#define DMA8641_CLNDAR_EOSIE_ENABLE     (0x1<<2)

/* ------------------------------------------------- */
/* Source Attributes Register                        */
/* ------------------------------------------------- */

/* ATMU Bypass (SRIO only) */
#define DMA8641_SATR_SBPATMU_MASK       (0x1<<29)
#define DMA8641_SATR_SBPATMU_NORMAL     (0x0<<29)
#define DMA8641_SATR_SBPATMU_BYPASS     (0x1<<29)

/* RapidIO transaction flow level (SRIO only) */
#define DMA8641_SATR_STFLOWLVL_MASK     ((0x3<<26) | DMA8641_SATR_SBPATMU_MASK)
#define DMA8641_SATR_STFLOWLVL_LOW      ((0x0<<26) | DMA8641_SATR_SBPATMU_BYPASS)
#define DMA8641_SATR_STFLOWLVL_MED      ((0x1<<26) | DMA8641_SATR_SBPATMU_BYPASS)
#define DMA8641_SATR_STFLOWLVL_HIGH     ((0x2<<26) | DMA8641_SATR_SBPATMU_BYPASS)

/* Follow PCI transaction ordering rules (Applicable while SPBATMU is set) */
#define DMA8641_SATR_SPCIORDER_MASK     ((0x1<<25) | DMA8641_SATR_SBPATMU_MASK)
#define DMA8641_SATR_SPCIORDER_DISABLE  ((0x0<<25) | DMA8641_SATR_SBPATMU_BYPASS)
#define DMA8641_SATR_SPCIORDER_ENABLE   ((0x1<<25) | DMA8641_SATR_SBPATMU_BYPASS)

/* Source Stride mode enable/disable */
#define DMA8641_SATR_SSME_MASK          (0x1<<24)
#define DMA8641_SATR_SSME_DISABLE       (0x0<<24)
#define DMA8641_SATR_SSME_ENABLE        (0x1<<24)

/* Source Transaction Interface (SRIO only, applicable while SPBATMU is set)*/
#define DMA8641_SATR_STRANSIT_MASK      ((0xF<<20) | DMA8641_SATR_SBPATMU_MASK)
#define DMA8641_SATR_STRANSIT_PCIE1     ((0x0<<20) | DMA8641_SATR_SBPATMU_BYPASS)
#define DMA8641_SATR_STRANSIT_PCIE2     ((0x1<<20) | DMA8641_SATR_SBPATMU_BYPASS)
#define DMA8641_SATR_STRANSIT_SRIO      ((0xC<<20) | DMA8641_SATR_SBPATMU_BYPASS)

/* DMA Source transaction type */
#define DMA8641_SATR_SREADTTYPE_MASK        ((0xF<<16) | DMA8641_SATR_SBPATMU_MASK)
#define DMA8641_SATR_SREADTTYPE_ATMU_IORD   ((0x2<<16) | DMA8641_SATR_SBPATMU_BYPASS)
#define DMA8641_SATR_SREADTTYPE_ATMU_NRD    ((0x4<<16) | DMA8641_SATR_SBPATMU_BYPASS)
#define DMA8641_SATR_SREADTTYPE_ATMU_MRD    ((0x7<<16) | DMA8641_SATR_SBPATMU_BYPASS)
#define DMA8641_SATR_SREADTTYPE_RD_NOSNOOP  ((0x4<<16) | DMA8641_SATR_SBPATMU_NORMAL)
#define DMA8641_SATR_SREADTTYPE_RD_SNOOP    ((0x5<<16) | DMA8641_SATR_SBPATMU_NORMAL)

/* Extended Source address (bit 0-9)*/
#define DMA8641_SATR_ESAD_MASK              (0x000003FF<<0)

/* virt to phys conversion is required for source address */
#define DMA8641_SATR_PHYS_ADRS     		   (0x0<<30)
#define DMA8641_SATR_VIRT_ADRS     		   (0x1<<30) 

/* SRIO specific target ID */
#define DMA8641_SATR_SRIO_ID_MASK       ((0xFF<<2) | DMA8641_SATR_SBPATMU_MASK)
#define DMA8641_SATR_SRIO_ID(id)        (((id<<2) & (0xFF<<2)) | DMA8641_SATR_SBPATMU_BYPASS)


#define DMA8621_SATR_DEFAULT                0
/* ------------------------------------------------- */
/* Source Address Register                           */
/* ------------------------------------------------- */
/* Source Address */
#define DMA8641_SAR_SAD_MASK                (0xFFFFFFFF)

/* ------------------------------------------------- */
/* Source Address Register (SRIO Maintenance Read)   */
/* ------------------------------------------------- */
/* word offset*/
#define DMA8641_SAR_CONFIG_OFFSET_MASK      (0xFFFFFC<<2)
/*hop count */
#define DMA8641_SAR_HOP_COUNT_MASK          (0xFF<<24)

/* ------------------------------------------------- */
/* Destination Attributes Register                   */
/* ------------------------------------------------- */
/* ATMU Bypass (SRIO only) */
#define DMA8641_DATR_DBPATMU_MASK       (0x1<<29)
#define DMA8641_DATR_DBPATMU_NORMAL     (0x0<<29)
#define DMA8641_DATR_DBPATMU_BYPASS     (0x1<<29)

/* RapidIO transaction flow level (SRIO only) */
#define DMA8641_DATR_DTFLOWLVL_MASK     ((0x3<<26) | DMA8641_DATR_DBPATMU_MASK)
#define DMA8641_DATR_DTFLOWLVL_LOW      ((0x0<<26) | DMA8641_DATR_DBPATMU_BYPASS)
#define DMA8641_DATR_DTFLOWLVL_MED      ((0x1<<26) | DMA8641_DATR_DBPATMU_BYPASS)
#define DMA8641_DATR_DTFLOWLVL_HIGH     ((0x2<<26) | DMA8641_DATR_DBPATMU_BYPASS)

/* Follow PCI transaction ordering rules (Applicable while DPBATMU is set) */
#define DMA8641_DATR_DPCIORDER_MASK     ((0x1<<25) | DMA8641_DATR_DBPATMU_MASK)
#define DMA8641_DATR_DPCIORDER_DISABLE  ((0x0<<25) | DMA8641_DATR_DBPATMU_BYPASS)
#define DMA8641_DATR_DPCIORDER_ENABLE   ((0x1<<25) | DMA8641_DATR_DBPATMU_BYPASS)

/* Destination Stride mode enable/disable */
#define DMA8641_DATR_DSME_MASK          (0x1<<24)
#define DMA8641_DATR_DSME_DISABLE       (0x0<<24)
#define DMA8641_DATR_DSME_ENABLE        (0x1<<24)

/* Destination Transaction Interface (SRIO only, applicable while SPBATMU is set)*/
#define DMA8641_DATR_DTRANSIT_MASK      ((0xF<<20) | DMA8641_DATR_DBPATMU_MASK)
#define DMA8641_DATR_DTRANSIT_PCIE1     ((0x0<<20) | DMA8641_DATR_DBPATMU_BYPASS)
#define DMA8641_DATR_DTRANSIT_PCIE2     ((0x1<<20) | DMA8641_DATR_DBPATMU_BYPASS)
#define DMA8641_DATR_DTRANSIT_SRIO      ((0xC<<20) | DMA8641_DATR_DBPATMU_BYPASS)

/* DMA Destination Transaction type */
#define DMA8641_DATR_DWRITETTYPE_MASK           ((0xF<<16) | DMA8641_DATR_DBPATMU_MASK)
#define DMA8641_DATR_DWRITETTYPE_ATMU_FLUSH     ((0x1<<16) | DMA8641_DATR_DBPATMU_BYPASS)
#define DMA8641_DATR_DWRITETTYPE_ATMU_SWRITE    ((0x3<<16) | DMA8641_DATR_DBPATMU_BYPASS)
#define DMA8641_DATR_DWRITETTYPE_ATMU_NWRITE    ((0x4<<16) | DMA8641_DATR_DBPATMU_BYPASS)
#define DMA8641_DATR_DWRITETTYPE_ATMU_NWRITE_R  ((0x5<<16) | DMA8641_DATR_DBPATMU_BYPASS)
#define DMA8641_DATR_DWRITETTYPE_ATMU_MSG_SMALL ((0x6<<16) | DMA8641_DATR_DBPATMU_BYPASS)
#define DMA8641_DATR_DWRITETTYPE_ATMU_MWRITE    ((0x7<<16) | DMA8641_DATR_DBPATMU_BYPASS)
#define DMA8641_DATR_DWRITETTYPE_WR_NOSNOOP     ((0x4<<16) | DMA8641_DATR_DBPATMU_NORMAL)
#define DMA8641_DATR_DWRITETTYPE_WR_SNOOP       ((0x5<<16) | DMA8641_DATR_DBPATMU_NORMAL)

/* Extended Destination Address (bit 0-9)*/
#define DMA8641_DATR_EDAD_MASK                  (0x3FF<<0)

/* virt to phys conversion is required for destination address */
#define DMA8641_DATR_PHYS_ADRS     			   (0x0<<30)
#define DMA8641_DATR_VIRT_ADRS     			   (0x1<<30) 

/* SRIO specific target ID */
#define DMA8641_DATR_SRIO_ID_MASK       ((0xFF<<2) | DMA8641_DATR_DBPATMU_MASK)
#define DMA8641_DATR_SRIO_ID(id)        (((id<<2) & (0xFF<<2)) | DMA8641_DATR_DBPATMU_BYPASS)

#define DMA8641_DATR_DEFAULT                    0

/* --------------------------------------------------*/
/* Destination Address Register                      */
/* --------------------------------------------------*/
/* Destination Address */
#define DMA8641_DAR_DAD_MASK                    (0xFFFFFFFF)

/* --------------------------------------------------*/
/* Destination Address Register (SRIO Maint. Write)  */
/* --------------------------------------------------*/
/* word offset*/
#define DMA8641_DAR_CONFIG_OFFSET_MASK          (0xFFFFFC<<2)
/*hop count */
#define DMA8641_DAR_HOP_COUNT_MASK              (0xFF<<24)

/* ------------------------------------------------- */
/* Byte count register                               */
/* ------------------------------------------------- */
/* Byte count */
#define DMA8641_BCR_BC_MASK                     (0x03FFFFFF)
#define DMA8641_BCR_BC_MAX_CNT                  (0x03FFFFFF)

/* ------------------------------------------------- */
/* Next Link Descriptor Address Register (Extended)  */
/* --------------------------------------------------*/
/* Extended address upper 4 bits*/
#define DMA8641_ENLNDAR_ENLNDA_MASK             (0x0000000F)

/* ------------------------------------------------- */
/* Next Link Descriptor Address Register             */
/* ------------------------------------------------- */
/*  address */
#define DMA8641_NLNDAR_NLNDA_MASK               (0xFFFFFFE0)
/* next descriptor end of segment interrupt enable/disable */
#define DMA8641_NLNDAR_NDEOSIE_MASK             (0x1<<3)
#define DMA8641_NLNDAR_NDEOSIE_DISABLE          (0x0<<3)
#define DMA8641_NLNDAR_NDEOSIE_ENABLE           (0x1<<3)
/* end-of-links descriptor flag */
#define DMA8641_NLNDAR_EOLND_MASK               (0x1<<0)
#define DMA8641_NLNDAR_EOLND_NOTLAST            (0x0<<0)
#define DMA8641_NLNDAR_EOLND_LAST               (0x1<<0)

/* ------------------------------------------------- */
/* Current List Descriptor Address Register (Ext)    */
/* ------------------------------------------------- */
/* Extended address upper 4 bits*/
#define DMA8641_ECLSDAR_ECLSDA_MASK         (0x0000000F)

/* ------------------------------------------------- */
/* Current List Descriptor Address Register          */
/* ------------------------------------------------- */
/*  address */
#define DMA8641_CLSDAR_CLSDA_MASK           (0xFFFFFFE0)

/* ------------------------------------------------- */
/* Next List Descriptor Address Register (Ext.)      */
/* ------------------------------------------------- */
/* Extended address upper 4 bits*/
#define DMA8641_ENLSDAR_ENLSDA_MASK         (0x0000000F)

/* ------------------------------------------------- */
/* Current List Descriptor Address Register          */
/* ------------------------------------------------- */
/* address */
#define DMA8641_NLSDAR_NLSDA_MASK           (0xFFFFFFE0)
/* End of List descriptor flag*/
#define DMA8641_NLSDAR_EOLSD_MASK           (0x1<<0)
#define DMA8641_NLSDAR_EOLSD_NOTLAST        (0x0<<0)
#define DMA8641_NLSDAR_EOLSD_LAST           (0x1<<0)

/* ------------------------------------------------- */
/* Source Stride Register                            */
/* ------------------------------------------------- */
/* Stride size */
#define DMA8641_SSR_SSS_MASK                (0xFFF<<12)
/* Stride distance */
#define DMA8641_SSR_SSD_MASK                (0xFFF<<0)

/* ------------------------------------------------- */
/* Destination Stride Register                       */
/* ------------------------------------------------- */
/* Stride size */
#define DMA8641_DSR_DSS_MASK                (0xFFF<<12)
/* Stride distance */
#define DMA8641_DSR_DSD_MASK                (0xFFF<<0)

/* ------------------------------------------------- */
/* General Status Register                           */
/* ------------------------------------------------- */
#define DMA8641_DGSR_CHAN_DATA_GET(dgsr,channel) ((dgsr >> ((3-channel)*8)) & 0xFF)
/* Transfer error */
#define DMA8641_DGSR_TE_MASK                (0x1<<7)
#define DMA8641_DGSR_TE_NOERROR             (0x0<<7)
#define DMA8641_DGSR_TE_ERROR               (0x1<<7)
/* Channel halted */
#define DMA8641_DGSR_CH_MASK                (0x1<<5)
#define DMA8641_DGSR_CH_NOTHALTED           (0x0<<5)
#define DMA8641_DGSR_CH_HALTED              (0x1<<5)
/* Programming error */
#define DMA8641_DGSR_PE_MASK                (0x1<<4)
#define DMA8641_DGSR_PE_NOERROR             (0x0<<4)
#define DMA8641_DGSR_PE_ERROR               (0x1<<4)
/* End-of-Link interrupt */
#define DMA8641_DGSR_E0LNI_MASK             (0x1<<3)
#define DMA8641_DGSR_E0LNI_NOTACTIVE        (0x0<<3)
#define DMA8641_DGSR_E0LNI_ACTIVE           (0x1<<3)
/* Channel busy interrupt */
#define DMA8641_DGSR_CB_MASK                (0x1<<2)
#define DMA8641_DGSR_CB_IDLE                (0x0<<2)
#define DMA8641_DGSR_CB_BUSY                (0x1<<2)
/* End-of-Segment interrupt */
#define DMA8641_DGSR_E0SI_MASK              (0x1<<1)
#define DMA8641_DGSR_E0SI_NOTACTIVE         (0x0<<1)
#define DMA8641_DGSR_E0SI_ACTIVE            (0x1<<1)
/* End-of-List/direct interrupt */
#define DMA8641_DGSR_E0LSI_MASK             (0x1<<0)
#define DMA8641_DGSR_E0LSI_NOTACTIVE        (0x0<<0)
#define DMA8641_DGSR_E0LSI_ACTIVE           (0x1<<0)

/* ------------------------------------------------- */
/*       DMA Control Hardware Specific structures    */
/* ------------------------------------------------- */


/* MPC8641 List descriptor definition */
typedef struct
{
    UINT32 nextListHi;
    UINT32 nextListLo;
    UINT32 firstLinkHi;
    UINT32 firstLinkLo;
    UINT32 srcStride;
    UINT32 dstStride;
} sDSM_DMA_MPC8641_LIST_DESC;

/* MPC8641 Link descriptor definition */
typedef struct
{
    UINT32 srcAttrib;
    UINT32 srcAdrsLo;
    UINT32 dstAttrib;
    UINT32 dstAdrsLo;
    UINT32 nextLinkHi;
    UINT32 nextLinkLo;
    UINT32 byteCnt;
} sDSM_DMA_MPC8641_LINK_DESC;

/*********************************************************/
/* externs */

#endif /* _DSM_UART_MPC8641_H */
