/* vxbQorIQRman.h - QorIQ Rman header */

/*
 * Copyright (c) 2012, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
DESCRIPTION
This file contains the definitons for QorIQ Rman module.
*/

/*
modification history
--------------------
01c,08mar13,j_z  add T4240 Rman support.
01b,24nov12,j_z  added scatter-gather buffer pool and statistics counters.
01a,03sep12,j_z  written
*/

#ifndef __INCvxbQorIQRmanH
#define __INCvxbQorIQRmanH

#ifdef __cplusplus
extern "C" {
#endif

#include <../src/hwif/h/resource/vxbQorIQBmanPortal.h>
#include <../src/hwif/h/resource/vxbQorIQQmanPortal.h>
/*#include <hwif/util/vxbMsgSupport.h>*/
#include <../src/hwif/h/vxbus/vxbRapidIO.h>
#include "vxbMsgSupport.h"


#define QORIQ_RMAN_NAME          "QorIQRman"

/* RMan Global Registers */

#define RMAN_IPBRR0               0xBF8
#define RMAN_IPBRR1               0xBFC
#define RMAN_MMMR                 0xF00
#define RMAN_MMSR                 0xF04
#define RMAN_MMT8FQAR             0xF10
#define RMAN_MMT9FQAR             0xF14
#define RMAN_MMT10FQAR            0xF18
#define RMAN_MMT11FQAR            0xF1C
#define RMAN_MMIER                0xF20
#define RMAN_MMEDR                0xF24

/* Inbound Block TypeX Classification Registers */

/* 0xmn00 - Mode Register */

#define RMAN_IBCU_MR(b,n)         (0x1000*(b) + 0x100*(n) + 0x00)

/* 0xmn08 - Frame Queue Register */

#define RMAN_IBCU_FQR(b,n)        (0x1000*(b) + 0x100*(n) + 0x08)

 /* 0xmn10 - Rule Value Register 0/1 */

#define RMAN_IBCU_RVR0(b,n)       (0x1000*(b) + 0x100*(n) + 0x10)
#define RMAN_IBCU_RVR1(b,n)       (0x1000*(b) + 0x100*(n) + 0x14)

/* 0xmn18 - Rule Mask Register 0/1 */

#define RMAN_IBCU_RMR0(b,n)       (0x1000*(b) + 0x100*(n) + 0x18)
#define RMAN_IBCU_RMR1(b,n)       (0x1000*(b) + 0x100*(n) + 0x1c)

/* 0xmn20 - Type9 Flow Control Destination Register */

#define RMAN_IBCU_T9FCDR(b,n)     (0x1000*(b) + 0x100*(n) + 0x20)

/* 0xmn30 - Data Buffer Pool Register */

#define RMAN_IBCU_DBPR(b,n)       (0x1000*(b) + 0x100*(n) + 0x30)

/* 0xmn34 - Data Offset Register */

#define RMAN_IBCU_DOR(b,n)        (0x1000*(b) + 0x100*(n) + 0x34)

 /* 0xmn38 - Type9 Scatter/Gather Buffer Poll Register */

#define RMAN_IBCU_T9SGBPR(b,n)    (0x1000*(b) + 0x100*(n) + 0x38)

#define RMAN_IBCU_ENABLE           0x80000000
#define RMAN_IBCU_FQMODE_SHIT      28
#define RMAN_IBCU_TRANTYPE_SHIT    24
#define RMAN_IBCU_PORT_SHIT        24
#define RMAN_IBCU_FLVMASK_SHIT     28

#define RMAN_RESET                 0x00000001
#define RMAN_EFQ                   0x08000000
#define RMAN_MMQ                   0x20000000

#define RMAN_IB_INDEX_OFFSET       12
#define RMAN_CU_OFFSET             0x100
#define MMIER_ENABLE_ALL           0x800000FF
#define MMEDR_CLEAR                0x800000FF

#define RM_FD_SIZE                 0x20
#define RM_DATA_OFFSET             RM_FD_SIZE

#define RMAN_ADDR_LO(y)          ((UINT64)((UINT64) (y)) & 0xFFFFFFFF)
#define RMAN_ADDR_HI(y)          (((UINT64)((UINT64) (y)) >> 32) & 0xFFFFFFFF)

#define RMAN_OB_SINGLE            0x0
#define RMAN_OB_GATHER            0x80000000
#define RMAN_IBCU_NUM             4
#define RMAN_CU_NUM_PER_IB        8
#define RMAN_MBOX_SEG_NUM         15

#define RMAN_MAXFRAG              8
#define RMAN_TX_DESC_CNT          128
#define RMAN_BMANBUF_NUM          2048
#define RMAN_BMANBUF_PERSIZE      (4096 + 0x100) /* maximum 64K */

/* one sgtable entry has 16 bytes */

#define RMAN_SGTABLE_PERSIZE      (RMAN_MAXFRAG * 16)

#define RMAN_SC_F_ENTRY           0x40000000
#define RMAN_SC_E_ENTRY           0x80000000

#define RMAN_MAX_PORT             2

#define RMAN_TYPE9_FQID_OFFSET    0
#define RMAN_TYPE10_FQID_OFFSET   1
#define RMAN_TYPE11_FQID_OFFSET   2

/*
 * Note: this buffer pool id should be unique within the system.
 * The value RMAN_BPID is used as a bias just so that the RMAN buffer
 * pool numbers won't collide with the TGEC/DTSEC buffer pool numbers.
 * !!!!!
 */

#define RMAN_FQID_PORT0_POOL      0x70
#define RMAN_FQID_PORT1_POOL      0x80
#define RMAN_BPID                 0x30
#define RMAN_QMAN_CHANNEL         0x62
#define RMAN_QMAN_CHANNEL_T4240   0x880

typedef UINT64 (*rmanVirtToPhysFunc)(void *);
typedef void * (*rmanPhysToVirtFunc)(UINT64);

#define RMAN_INC_DESC(x, y)       (x) = ((x + 1) & (y - 1))

enum FQ_MODE
    {
    DIRECT = 0,
    ALGORITHMIC
    };

struct rio_tran
    {
    UINT8 type;
    UINT8 flowlvl;
    UINT8 flowlvlMask;
    struct mboxAttr
        {
        UINT8 mbox;
        UINT8 mboxMask;
        UINT8 ltr;
        UINT8 ltrMask;
        UINT8 msglen;
        UINT8 msglenMask;
        } mbox;
    struct dstrAttr
        {
        UINT16 streamid;
        UINT16 streamidMask;
        UINT8  cos;
        UINT8  cosMask;
        } dstr;
    };

typedef struct ibcu_cfg
    {
    UINT8           ibcu;
    UINT8           port;
    UINT8           portMask;
    UINT16          sid;
    UINT16          sidMask;
    UINT16          did;
    UINT16          didMask;
    UINT32          fqid;
    UINT8           bpid;
    UINT8           sgbpid;
    UINT32          msgsize;
    UINT32          sgsize;
    UINT32          dataoffset;
    enum FQ_MODE    fqMode;
    struct rio_tran tran;
    } IBCU_CONFIG;

/* RMAN TypeX Memory Outbound Descriptor */

struct rmanObMd
    {
    /* word0 */

    volatile UINT8  ftype:4; /* Descriptor type select */
    volatile UINT8  br:1;    /* Buffer release enable */
    volatile UINT8  so:1;    /* Strict ordering */
    volatile UINT8  cs:1;    /* Completion status */
    volatile UINT8  es:1;    /* Error status */
    volatile UINT8  reserved0[2];

   /* retry or hopcount or extend address */

    volatile UINT8  reTryHopCntAdrs;

    /* word1 */

    volatile UINT32 address; /* word1 */

    /* word2 */

    volatile UINT32 reserved1:8;
    volatile UINT32 statusFqid:24;

    /* word3 */

    volatile UINT16 did;
    volatile UINT16 others;

    /* word4 */

    volatile UINT8  reserved2:3;
    volatile UINT8  flowlvl:3;
    volatile UINT8  reserved3:2;
    volatile UINT8  tint:4;
    volatile UINT8  reserved4:4;
    volatile UINT16 otherAttr;

    /* word5 */

    volatile UINT32 messageGroup;

    /* word6 */

    volatile UINT32 messageList;

    /* word7 */

    volatile UINT32 count;
    };

/* RMAN TypeX Memory Inbound  Descriptor */

struct rmanIbMd {
    /* word0 */

    volatile UINT8 ftype:4; /* rio type */
    volatile UINT8 reserved0:4;
    volatile UINT8 reserved1[3];

    /* word1 */

    volatile UINT32 reserved2;

    /* word2 */

    volatile UINT32 reserved3;

    /* word3 */

    volatile UINT16 sid;
    volatile UINT16 src;

    /* word4 */

    volatile UINT8 reserved4:3;
    volatile UINT8 flowlvl:3;
    volatile UINT8 reserved5:2;
    volatile UINT8 sint:4;
    volatile UINT8 reserved6:4;
    volatile UINT16 otherAttr;

    /* word5 */
    volatile UINT16 did;
    volatile UINT16 dest;

    /* word6 */

    volatile UINT32 reserved7;

    /* word7 */

    volatile UINT32 count;
    };

typedef struct rmanCallbackArg
    {
    FUNCPTR pRxCallBack;
    FUNCPTR pTxCallBack;
    void *  pRxArg;
    void *  pTxArg;
    } RMAN_CLBACK_ARG;

typedef struct rman_tx_desc
    {
    QPORTAL_SGLIST  rman_frag [RMAN_MAXFRAG + 1];
    struct rmanObMd md;
    } RMAN_TX_DESC;

typedef struct rmanScatterGatherTable
    {
    volatile UINT32 addrHi;
    volatile UINT32 addrLo;
    volatile UINT32 efLen;
    volatile UINT32 bpidOffset;
    } RMAN_SC_TABLE;

typedef struct rmanDrvCtrl
    {
    VXB_DEVICE_ID       pDev;
    void *              rmanHandle;
    SEM_ID              rmanSem;
    UINT32              rmanCpu;
    UINT32              rmanPort;
    UINT32              rmanIbNum;
    UINT32              rmanBpid;
    UINT32              rmanWqChan;
    BPORTAL *           rmanBportal;
    QPORTAL *           rmanQportal;
    UINT32              rmanTxCtx;
    UINT32              rmanRxCtx[RIO_TYPE_NUM];
    UINT32              rmanTxDoneCtx;
    UINT32              rmanSgBpid;
    UINT32              rmanRxFqId;
    UINT32              rmanTxFqId;
    UINT32              rmanTxDoneFqId;
    rmanVirtToPhysFunc  rmanVirtToPhys;
    rmanPhysToVirtFunc  rmanPhysToVirt;
    RMAN_CLBACK_ARG     typeXCallBackArg[RIO_TYPE_NUM];
    RMAN_TX_DESC *      pRmanSCdescMem;
    UINT32              rmanTxProd;
    UINT32              rmanTxFree;
    UINT32              rmanTxErrPkts;
    UINT64              rmanTxBytes;
    UINT32              rmanRxErrPkts;
    UINT64              rmanRxBytes;
    UINT32              rmanBufNum;
    UINT32              rmanSizePerBuf;
    } RMAN_DRV_CTRL;




#define RMAN_BAR(p)      (VXB_DEVICE_ID)(p)->pRegBase[0]
#define RMAN_HANDLE(p)   ((RMAN_DRV_CTRL *)(p)->pDrvCtrl)->rmanHandle

#undef CSR_READ_4
#define CSR_READ_4(pDev, addr)                                  \
    vxbRead32 (RMAN_HANDLE(pDev), (UINT32 *)((UINT32)RMAN_BAR(pDev)+((UINT32)(addr))))

#undef CSR_WRITE_4
#define CSR_WRITE_4(pDev, addr, data)                           \
    vxbWrite32 (RMAN_HANDLE(pDev),                             \
        (UINT32 *)((UINT32)RMAN_BAR(pDev)+((UINT32)(addr))), data)

#define RMAN_VTOP(p, x) \
        p->rmanVirtToPhys == NULL ? (ULONG)(x) : (p->rmanVirtToPhys)(x)

#define RMAN_PTOV(p, x) \
        p->rmanPhysToVirt == NULL ? (void *)(ULONG)(x) : (p->rmanPhysToVirt)(x)

#ifdef __cplusplus
}
#endif

#endif /* __INCvxbQorIQRmanH */

