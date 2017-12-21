/* bspBridgeDma8641.h */

#ifndef _bspBridgeDma8641_h_H
#define _bspBridgeDma8641_h_H

#ifdef __cplusplus
extern "C" {
#endif

#include "dsmDmaMpc8641.h"


#define DMA_MODULE_OFFSET 0x21000

#define  MPC8641_DMA_MR(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x100))
#define  MPC8641_DMA_SR(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x104))

#define  MPC8641_DMA_ECLNDAR(channel)       (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x108))
#define  MPC8641_DMA_CLNDAR(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x10C))
#define  MPC8641_DMA_SATR(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x110))
#define  MPC8641_DMA_SAR(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x114))
#define  MPC8641_DMA_DATR(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x118))
#define  MPC8641_DMA_DAR(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x11C))
#define  MPC8641_DMA_BCR(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x120))
#define  MPC8641_DMA_ENLNDAR(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x124))
#define  MPC8641_DMA_NLNDAR(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x128))
#define  MPC8641_DMA_ECLSDAR(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x130))
#define  MPC8641_DMA_CLSDAR(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x134))
#define  MPC8641_DMA_ENLSDAR(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x138))
#define  MPC8641_DMA_NLSDAR(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x13C))
#define  MPC8641_DMA_SSR(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x140))
#define  MPC8641_DMA_DSR(channel)        (CAST(VUINT32 *)((CCSBAR)+DMA_MODULE_OFFSET+(channel)*0x80 + 0x144))



/* defines */
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

/* Bridge DMA Control user options */
/*---------------------------------*/

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

/* DMA default options */
#define MPC8641_DMA_DEF_OPTIONS      (MPC8641_DMA_BWC_DISABLE    | \
                                      MPC8641_DMA_DAHTS_DISABLE  | \
                                      MPC8641_DMA_SAHTS_DISABLE)

/* Source attributes for SRIO transfers */
/*----------------------------------------*/
/* RapidIO transaction flow level (SRIO only) */
#define MPC8641_DMA_STFLOWLVL_LOW      ((0x0<<26) | (0x1<<29))
#define MPC8641_DMA_STFLOWLVL_MED      ((0x1<<26) | (0x1<<29))
#define MPC8641_DMA_STFLOWLVL_HIGH     ((0x2<<26) | (0x1<<29))

/* Follow PCI transaction ordering rules (Applicable while SPBATMU is set) */
#define MPC8641_DMA_SPCIORDER_DISABLE  ((0x0<<25) | (0x1<<29))
#define MPC8641_DMA_SPCIORDER_ENABLE   ((0x1<<25) | (0x1<<29))

/* Source Transaction Interface (SRIO only, applicable while SPBATMU is set)*/
#define MPC8641_DMA_STRANSIT_PCIE1     ((0x0<<20) | (0x1<<29))
#define MPC8641_DMA_STRANSIT_PCIE2     ((0x1<<20) | (0x1<<29))
#define MPC8641_DMA_STRANSIT_SRIO      ((0xC<<20) | (0x1<<29))

/* DMA Source transaction type */
#define MPC8641_DMA_SREADTTYPE_ATMU_IORD   ((0x2<<16) | (0x1<<29))
#define MPC8641_DMA_SREADTTYPE_ATMU_NRD    ((0x4<<16) | (0x1<<29))
#define MPC8641_DMA_SREADTTYPE_ATMU_MRD    ((0x7<<16) | (0x1<<29))

/* Source attributes for normal transfers */
/*----------------------------------------*/
/* DMA Source transaction type */
#define MPC8641_DMA_SREADTTYPE_RD_NOSNOOP  ((0x4<<16) | (0x0<<29))
#define MPC8641_DMA_SREADTTYPE_RD_SNOOP    ((0x5<<16) | (0x0<<29))


/* Destination attributes for SRIO transfers */
/*-------------------------------------------*/
/* RapidIO transaction flow level (SRIO only) */
#define MPC8641_DMA_DTFLOWLVL_LOW      ((0x0<<26) | (0x1<<29))
#define MPC8641_DMA_DTFLOWLVL_MED      ((0x1<<26) | (0x1<<29))
#define MPC8641_DMA_DTFLOWLVL_HIGH     ((0x2<<26) | (0x1<<29))

/* Follow PCI transaction ordering rules (Applicable while DPBATMU is set) */
#define MPC8641_DMA_DPCIORDER_DISABLE  ((0x0<<25) | (0x1<<29))
#define MPC8641_DMA_DPCIORDER_ENABLE   ((0x1<<25) | (0x1<<29))

/* Destination Transaction Interface (SRIO only, applicable while SPBATMU is set)*/
#define MPC8641_DMA_DTRANSIT_PCIE1     ((0x0<<20) | (0x1<<29))
#define MPC8641_DMA_DTRANSIT_PCIE2     ((0x1<<20) | (0x1<<29))
#define MPC8641_DMA_DTRANSIT_SRIO      ((0xC<<20) | (0x1<<29))

/* DMA Destination Transaction type */
#define MPC8641_DMA_DWRITETTYPE_ATMU_FLUSH     ((0x1<<16) | (0x1<<29))
#define MPC8641_DMA_DWRITETTYPE_ATMU_SWRITE    ((0x3<<16) | (0x1<<29))
#define MPC8641_DMA_DWRITETTYPE_ATMU_NWRITE    ((0x4<<16) | (0x1<<29))
#define MPC8641_DMA_DWRITETTYPE_ATMU_NWRITE_R  ((0x5<<16) | (0x1<<29))
#define MPC8641_DMA_DWRITETTYPE_ATMU_MSG_SMALL ((0x6<<16) | (0x1<<29))
#define MPC8641_DMA_DWRITETTYPE_ATMU_MWRITE    ((0x7<<16) | (0x1<<29))

/* SRIO specific target ID */
#define MPC8641_DMA_SRIO_ID(id)        (((id<<2) & (0xFF<<2)) | (0x1<<29))

/* Destination attributes for normal transfers */
/*---------------------------------------------*/
/* DMA Destination Transaction type */
#define MPC8641_DMA_DWRITETTYPE_WR_NOSNOOP     ((0x4<<16) | (0x0<<29))
#define MPC8641_DMA_DWRITETTYPE_WR_SNOOP       ((0x5<<16) | (0x0<<29))


 /*Bridge DMA Descriptor Pool management data structure */
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


void bspBridgeDmaInit(void);

BSP_BRIDGE_DMA_CHAN_ID bspBridgeDmaChannelReserve(void);

STATUS bspBridgeDmaChannelRelease(BSP_BRIDGE_DMA_CHAN_ID channel);

STATUS bspBridgeDmaDirectXfer(BSP_BRIDGE_DMA_CHAN_ID channel,
                              UINT32  options,
                              UINT64  src, 
                              UINT32  srcStrideSize,
                              UINT32  srcStrideDist,
                              UINT32  srcAttrib,
                              UINT64  dst,
                              UINT32  dstStrideSize,
                              UINT32  dstStrideDist,
                              UINT32  dstAttrib,
                              UINT32  size,
                              VOIDFUNCPTR  callback,
                              void*   param
                             );

STATUS bspBridgeDmaChainedXfer(BSP_BRIDGE_DMA_CHAN_ID  channel,
                               UINT32                  options,
                               BSP_DMA_BRIDGE_POOL_ID  poolId,
                               VOIDFUNCPTR             callback,
                               void*                   param
                              );

STATUS bspBridgeDmaAbort(BSP_BRIDGE_DMA_CHAN_ID channel);

STATUS bspBridgeDmaPause(BSP_BRIDGE_DMA_CHAN_ID channel);

STATUS bspBridgeDmaResume(BSP_BRIDGE_DMA_CHAN_ID channel);

eBSP_DMA_STATUS  bspBridgeDmaStatusGet(BSP_BRIDGE_DMA_CHAN_ID channel);

BSP_DMA_BRIDGE_POOL_ID bspBridgeDmaDescPoolCreate(UINT32 descCnt);

STATUS bspBridgeDmaDescPoolReset(BSP_DMA_BRIDGE_POOL_ID  poolId);

STATUS bspBridgeDmaListDescAdd(BSP_DMA_BRIDGE_POOL_ID  poolId,
                               UINT32                  srcStrideDist,
                               UINT32                  srcStrideSize,
                               UINT32                  dstStrideDist,
                               UINT32                  dstStrideSize
                              );

STATUS bspBridgeDmaLinkDescAdd(BSP_DMA_BRIDGE_POOL_ID  poolId,
                               UINT64                  src,
                               UINT32                  srcAttrib,
                               UINT64                  dst,
                               UINT32                  dstAttrib,
                               UINT32                  size
                              );

STATUS bspBridgeDmaDescPoolFree(BSP_DMA_BRIDGE_POOL_ID  poolId);

STATUS bspBridgeDmaDescPoolShow(BSP_DMA_BRIDGE_POOL_ID  poolId);

#ifdef __cplusplus
}
#endif

#endif /* bspBridgeDma8641_h_H */


