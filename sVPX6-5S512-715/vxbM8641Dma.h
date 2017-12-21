#ifndef __VXB_M8641_DMA_H
#define __VXB_M8641_DMA_H

#define DMA8641_MAX_CHAN                4
#define DMA8641_SATR_SBPATMU_MASK       (0x1<<29)
#define DMA8641_SATR_SBPATMU_NORMAL     (0x0<<29)
#define DMA8641_SATR_SBPATMU_BYPASS     (0x1<<29)


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

#define DMA8641_DATR_DBPATMU_MASK       (0x1<<29)
#define DMA8641_DATR_DBPATMU_NORMAL     (0x0<<29)
#define DMA8641_DATR_DBPATMU_BYPASS     (0x1<<29)


#define DMA8641_DATR_DWRITETTYPE_MASK           ((0xF<<16) | DMA8641_DATR_DBPATMU_MASK)
#define DMA8641_DATR_DWRITETTYPE_ATMU_FLUSH     ((0x1<<16) | DMA8641_DATR_DBPATMU_BYPASS)
#define DMA8641_DATR_DWRITETTYPE_ATMU_SWRITE    ((0x3<<16) | DMA8641_DATR_DBPATMU_BYPASS)
#define DMA8641_DATR_DWRITETTYPE_ATMU_NWRITE    ((0x4<<16) | DMA8641_DATR_DBPATMU_BYPASS)
#define DMA8641_DATR_DWRITETTYPE_ATMU_NWRITE_R  ((0x5<<16) | DMA8641_DATR_DBPATMU_BYPASS)
#define DMA8641_DATR_DWRITETTYPE_ATMU_MSG_SMALL ((0x6<<16) | DMA8641_DATR_DBPATMU_BYPASS)
#define DMA8641_DATR_DWRITETTYPE_ATMU_MWRITE    ((0x7<<16) | DMA8641_DATR_DBPATMU_BYPASS)
#define DMA8641_DATR_DWRITETTYPE_WR_NOSNOOP     ((0x4<<16) | DMA8641_DATR_DBPATMU_NORMAL)
#define DMA8641_DATR_DWRITETTYPE_WR_SNOOP       ((0x5<<16) | DMA8641_DATR_DBPATMU_NORMAL)


#define lOCAL_SRC_ATTRIB_NOSNOOP      (DMA8641_SATR_SREADTTYPE_RD_NOSNOOP)
#define lOCAL_SRC_ATTRIB_SNOOP      (DMA8641_SATR_SREADTTYPE_RD_SNOOP)

#define lOCAL_DST_ATTRIB_NOSNOOP       (DMA8641_DATR_DWRITETTYPE_WR_NOSNOOP)
#define lOCAL_DST_ATTRIB_SNOOP       (DMA8641_DATR_DWRITETTYPE_WR_SNOOP)

/* RapidIO transaction flow level (SRIO only) */
#define DMA8641_SATR_STFLOWLVL_MASK     ((0x3<<26) | DMA8641_SATR_SBPATMU_MASK)
#define DMA8641_SATR_STFLOWLVL_LOW      ((0x0<<26) | DMA8641_SATR_SBPATMU_BYPASS)
#define DMA8641_SATR_STFLOWLVL_MED      ((0x1<<26) | DMA8641_SATR_SBPATMU_BYPASS)
#define DMA8641_SATR_STFLOWLVL_HIGH     ((0x2<<26) | DMA8641_SATR_SBPATMU_BYPASS)
/* RapidIO transaction flow level (SRIO only) */
#define DMA8641_DATR_DTFLOWLVL_MASK     ((0x3<<26) | DMA8641_DATR_DBPATMU_MASK)
#define DMA8641_DATR_DTFLOWLVL_LOW      ((0x0<<26) | DMA8641_DATR_DBPATMU_BYPASS)
#define DMA8641_DATR_DTFLOWLVL_MED      ((0x1<<26) | DMA8641_DATR_DBPATMU_BYPASS)
#define DMA8641_DATR_DTFLOWLVL_HIGH     ((0x2<<26) | DMA8641_DATR_DBPATMU_BYPASS)
/* Destination Transaction Interface (SRIO only, applicable while SPBATMU is set)*/
#define DMA8641_DATR_DTRANSIT_MASK      ((0xF<<20) | DMA8641_DATR_DBPATMU_MASK)
#define DMA8641_DATR_DTRANSIT_PCIE1     ((0x0<<20) | DMA8641_DATR_DBPATMU_BYPASS)
#define DMA8641_DATR_DTRANSIT_PCIE2     ((0x1<<20) | DMA8641_DATR_DBPATMU_BYPASS)
#define DMA8641_DATR_DTRANSIT_SRIO      ((0xC<<20) | DMA8641_DATR_DBPATMU_BYPASS)

/* SRIO specific target ID */
#define DMA8641_SATR_SRIO_ID_MASK       ((0xFF<<2) | DMA8641_SATR_SBPATMU_MASK)
#define DMA8641_SATR_SRIO_ID(id)        (((id<<2) & (0xFF<<2)) | DMA8641_SATR_SBPATMU_BYPASS)

/* SRIO specific target ID */
#define DMA8641_DATR_SRIO_ID_MASK       ((0xFF<<2) | DMA8641_DATR_DBPATMU_MASK)
#define DMA8641_DATR_SRIO_ID(id)        (((id<<2) & (0xFF<<2)) | DMA8641_DATR_DBPATMU_BYPASS)


#define RAPIDIO_SRC_ATTRIB      (DMA8641_SATR_SBPATMU_BYPASS | \
                                 DMA8641_SATR_STFLOWLVL_HIGH | \
                                 DMA8641_SATR_STRANSIT_SRIO | \
                                 DMA8641_SATR_SREADTTYPE_ATMU_NRD)
#define RAPIDIO_DST_ATTRIB      (DMA8641_DATR_DBPATMU_BYPASS | \
                                 DMA8641_DATR_DTFLOWLVL_HIGH | \
                                 DMA8641_DATR_DTRANSIT_SRIO |\
                                 DMA8641_DATR_DWRITETTYPE_ATMU_NWRITE)




#define MPC8641_DMA_BWC_DISABLE      (0xF<<24)

#define MPC8641_DMA_DAHTS_DISABLE    (0x0<<13)
#define MPC8641_DMA_DAHTS_1BYTE      ((0x0<<16) | (0x1<<13))
#define MPC8641_DMA_DAHTS_2BYTES     ((0x1<<16) | (0x1<<13))
#define MPC8641_DMA_DAHTS_4BYTES     ((0x2<<16) | (0x1<<13))
#define MPC8641_DMA_DAHTS_8BYTES     ((0x3<<16) | (0x1<<13))

/* Source address hold transfer size */
#define MPC8641_DMA_SAHTS_DISABLE    (0x0<<12)
#define MPC8641_DMA_SAHTS_1BYTE      ((0x0<<14) | (0x1<<12))
#define MPC8641_DMA_SAHTS_2BYTES     ((0x1<<14) | (0x1<<12))
#define MPC8641_DMA_SAHTS_4BYTES     ((0x2<<14) | (0x1<<12))
#define MPC8641_DMA_SAHTS_8BYTES     ((0x3<<14) | (0x1<<12))


#define MPC8641_DMA_DEF_OPTIONS      (MPC8641_DMA_BWC_DISABLE    | \
                                      MPC8641_DMA_DAHTS_DISABLE  | \
                                      MPC8641_DMA_SAHTS_DISABLE)
                                      

#define DMA_WAIT_TIMEOUT  sysClkRateGet() /*tick*/
#define DMA_MODULE_OFFSET 0x21000
#define DMA8641_MR_CTM_MASK         (0x1<<2)
#define DMA8641_MR_CTM_CHAINING     (0x0<<2)
#define DMA8641_MR_CTM_DIRECT       (0x1<<2)

/* Channel Continue */
#define DMA8641_MR_CC_MASK          (0x1<<1)
#define DMA8641_MR_CC_NO            (0x0<<1)
#define DMA8641_MR_CC_YES           (0x1<<1)

#define DMA8641_MR_CS		0x00000001
#define DMA8641_MR_CC		0x00000002
#define DMA8641_MR_CA		0x00000008
#define DMA8641_MR_EMP_EN	0x00200000


/* Channel halt/start */
#define DMA8641_MR_CS_MASK          (0x1<<0)
#define DMA8641_MR_CS_HALT          (0x0<<0)
#define DMA8641_MR_CS_START         (0x1<<0)

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

#define DMA8641_SR_TE_MASK          (0x1<<7)
#define DMA8641_SR_TE_NOERROR       (0x0<<7)
#define DMA8641_SR_TE_ERROR         (0x1<<7)


#define  MPC8641_DMA_MR              0x00
#define  MPC8641_DMA_SR              0x04
#define  MPC8641_DMA_SATR            0x10
#define  MPC8641_DMA_SAR             0x14
#define  MPC8641_DMA_DATR            0x18
#define  MPC8641_DMA_DAR             0x1C
#define  MPC8641_DMA_BCR             0x20

#define DMA_CSR_READ_4(pDev, addr)					\
	*(volatile UINT32 *)((UINT32)pDev->pRegBase[0] + addr)

#define DMA_CSR_WRITE_4(pDev, addr, data)				\
	do {							\
	    volatile UINT32 *pReg =				\
	        (UINT32 *)((UINT32)pDev->pRegBase[0] + addr);	\
	    *(pReg) = (UINT32)(data);				\
	    WRS_ASM("eieio");					\
	} while ((0))
	

#define DMA_M8641_MAGIC   0x87340629

typedef struct tagDMA_M8641_INFO_S
{
	UINT32      uiFlag;
	UINT32      uiUnit;
	UINT32      uiChan;
	UINT32      uiPriority;
	UINT32      uiSrcAttr;
	UINT32      uiDestAttr;
	UINT32      uiOption;
	
	SEM_ID      semDmaSync;
	SEM_ID      semDmaSrc;
	
	pVXB_DMA_COMPLETE_FN   pfIsr;
	VOID       *pvArg;
	UINT32      uiSize;
} DMA_M8641_INFO_S;

typedef struct tagDMA_M8641_DEDICATED_S
{
    UINT32      uiMagic;
    UINT32      uiUnit;
    UINT32      uiPriority;
    UINT32      uiSrcAttr;
    UINT32      uiDestAttr;
    UINT32      uiOption;
}DMA_M8641_DEDICATED_S;


#define DMA_ERROR_TRAN     -1
#define DMA_ERROR_PROM     -2
#define DMA_ERROR_PARM     -3
#define DMA_ERROR_OTHER    -4
#define DMA_ERROR_TIMEOUT  -5
#define DMA_ERROR_START    -6
#define DMA_ERROR_CHAN     -7
#define DMA_ERROR_WAIT     -8
#define DMA_ERROR_ADDR     -9



#endif

