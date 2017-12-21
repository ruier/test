#ifndef __VXB_M8641_DMA_H
#define __VXB_M8641_DMA_H


/***********************************new dma**********************/
#define DMA_CHAIN_DEMO
#ifdef DMA_CHAIN_DEMO
/* Local definitions */
#define MAX_BYTES_PER_LINE       16
#define TRANSFER_SIZE   0x100000                /* entire transfer size */
#define BUFFER_SIZE     (3*TRANSFER_SIZE)       /* more buffer than required for demo*/
#define DESC_CNT        10                      /* number of decriptors in pool */
#define SRC_ATTRIB      DMA8641_SATR_SREADTTYPE_RD_SNOOP
#define DST_ATTRIB      DMA8641_DATR_DWRITETTYPE_WR_SNOOP
#endif

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

typedef struct
{
    UINT32  signature;                          /* Signature used to validate this structure         */
    char*   pMemPool;                           /* Pointer to uncached memory buffer                 */
    UINT32  memPoolSize;                        /* Memory pool size in bytes                         */
    UINT32  maxDescriptor;                      /* Maximum no of descriptors in memory pool          */
    UINT32  descSize;                           /* Size for one descriptor                           */
    UINT32  descAlign;                          /* Alignment of descriptor                           */
    sDSM_DMA_MPC8641_LINK_DESC*   pLastLinkDesc;/* Pointer to the last descriptor added to the chain */
    sDSM_DMA_MPC8641_LIST_DESC*   pLastListDesc;/* Pointer to the last descriptor added to the chain */
    UINT32  firstDescType;                      /* List or Link descriptor                           */
    UINT32  lastDescType;                       /* List or Link descriptor                           */
    UINT32  listDescCnt;                        /* List descriptor count in chain                    */
    UINT32  linkDescCnt;                        /* Link descriptor count in chain                    */
    BOOL    valid;                              /* Chain valid and completed flag (True or False)    */
    BOOL    inUse;                              /* Chain owned by the DMA engine                     */
    UINT32  descFreeHead;                       /* Index of first free descriptor in memory buffer   */
    SEM_ID  dmaSem;                             /* Semaphore for pool descriptor changes             */
}  sBSP_DMA_BRIDGE_CHAIN_MNGT_BLK;

typedef sBSP_DMA_BRIDGE_CHAIN_MNGT_BLK* BSP_DMA_BRIDGE_POOL_ID;
typedef UINT32                          BSP_BRIDGE_DMA_CHAN_ID;

/* DMA status reported */
typedef enum
{
    BSP_DMA_STAT_IDLE,
    BSP_DMA_STAT_RUNNING,
    BSP_DMA_STAT_XFER_ERROR,
    BSP_DMA_STAT_PROG_ERROR,
    BSP_DMA_STAT_PAUSED
} eBSP_DMA_STATUS;

#define DMA_MODULE_OFFSET 0x21000
typedef sBSP_DMA_BRIDGE_CHAIN_MNGT_BLK* BSP_DMA_BRIDGE_POOL_ID;

#define MPC8641_DMA_BWC_1BYTE        (0x0<<24)
#define MPC8641_DMA_BWC_2BYTES       (0x1<<24)
#define MPC8641_DMA_BWC_4BYTES       (0x2<<24)
#define MPC8641_DMA_BWC_8BYTES       (0x3<<24)
#define MPC8641_DMA_BWC_16BYTES      (0x4<<24)
#define MPC8641_DMA_BWC_32BYTES      (0x5<<24)
#define MPC8641_DMA_BWC_64BYTES      (0x6<<24)
#define MPC8641_DMA_BWC_128BYTES     (0x7<<24)
#define MPC8641_DMA_BWC_256BYTES     (0x8<<24)
#define MPC8641_DMA_BWC_512BYTES     (0x9<<24)
#define MPC8641_DMA_BWC_1024BYTES    (0xA<<24)
#define MPC8641_DMA_BWC_DISABLE      (0xF<<24)

/* Destination address hold transfer size */
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

#define DESC_TYPE_UNDEFINED  0
#define DESC_TYPE_LINK       1
#define DESC_TYPE_LIST       2

#define MPC8641_DMA_LINK_DESC_SIZE  0x20
#define MPC8641_DMA_LIST_DESC_SIZE  0x20
#define MPC8641_DMA_DESC_ALIGNMENT  0x20

#define NO_LINK_HI      0x0
#define NO_LINK_LO      0x0
#define END_OF_LIST_HI  0x0
#define END_OF_LIST_LO  0x1         /*  Does not include interrupt enables  */
#define END_OF_LINK_HI  0x0
#define END_OF_LINK_LO  0x1         /*  Does include interrupt enables      */
#define END_OF_POOL_INT 0x8
#define STRIDE_BIT_MAX  0xFFF
#define DESC_ADDR_LO_MASK   0xFFFFFFE0
#define ADDR_LO_MASK    0xFFFFFFFF
#define ADDR_HI_MASK    ((UINT64)0x0000000F00000000ULL)

#define POOL_SIGNATURE  0xB001B001

#define DMA_OK          1
#define DMA_ERROR       0

typedef UINT32                          BSP_BRIDGE_DMA_CHAN_ID;
#define  MPC8641_DMA_MREG(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x100))
#define  MPC8641_DMA_SREG(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x104))
#define  MPC8641_DMA_ECLNDAREG(channel)       (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x108))
#define  MPC8641_DMA_CLNDAREG(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x10C))
#define  MPC8641_DMA_SATREG(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x110))
#define  MPC8641_DMA_SAREG(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x114))
#define  MPC8641_DMA_DATREG(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x118))
#define  MPC8641_DMA_DAREG(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x11C))
#define  MPC8641_DMA_BCREG(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x120))
#define  MPC8641_DMA_ENLNDAREG(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x124))
#define  MPC8641_DMA_NLNDAREG(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x128))
#define  MPC8641_DMA_ECLSDAREG(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x130))
#define  MPC8641_DMA_CLSDAREG(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x134))
#define  MPC8641_DMA_ENLSDAREG(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x138))
#define  MPC8641_DMA_NLSDAREG(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x13C))
#define  MPC8641_DMA_SSREG(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x140))
#define  MPC8641_DMA_DSREG(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x144))

/* Extended feature enable/disable */
#define DMA8641_MR_XFE_MASK         (0x1<<5)
#define DMA8641_MR_XFE_DISABLE      (0x0<<5)
#define DMA8641_MR_XFE_ENABLE       (0x1<<5)
/* Source Stride mode enable/disable */
#define DMA8641_SATR_SSME_MASK          (0x1<<24)
#define DMA8641_SATR_SSME_DISABLE       (0x0<<24)
#define DMA8641_SATR_SSME_ENABLE        (0x1<<24)
/* Destination Stride mode enable/disable */
#define DMA8641_DATR_DSME_MASK          (0x1<<24)
#define DMA8641_DATR_DSME_DISABLE       (0x0<<24)
#define DMA8641_DATR_DSME_ENABLE        (0x1<<24)
/* End of links interrupt enable/disable */
#define DMA8641_MR_EOLNIE_MASK      (0x1<<8)
#define DMA8641_MR_EOLNIE_DISABLE   (0x0<<8)
#define DMA8641_MR_EOLNIE_ENABLE    (0x1<<8)
/* End of lists interrupt enable/disable */
#define DMA8641_MR_EOLSIE_MASK      (0x1<<7)
#define DMA8641_MR_EOLSIE_DISABLE   (0x0<<7)
#define DMA8641_MR_EOLSIE_ENABLE    (0x1<<7)

/*********************************end ****************************/
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

#endif

