/* mpc834xDma.h - Freescale mpc834x Message passing/doorbell/DMA header file */

/*
 * Copyright 2007 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
  modification history
  --------------------
  01b,18sep07,l_g modified for fsl_rdb8313
  01a,10apr07,vf  created
*/


#ifndef INCmpc834XDmah
#define INCmpc834XDmah

#ifdef __cplusplus
extern "C" {
#endif

#include <semLib.h>
#include <lstLib.h>
#include "fsl_rdb8313.h"

#define MPC834XDMA_NUM_CHANNELS              0x04
#define MPC834XDMA_DESCRIPTOR_ALIGNMENT      0x20
#define MPC834XDMA_NUM_DESCRIPTORS           0x01  /* Only one client at a time */


/* DRCNT=2, BWC=2, DMSEN=1, IRQS=0, EMSEN=0, DAHTS=0, SAHTS=0, DAHE=0, SAHE=0, PRC=PCI Read, EOTIE=1, TEM=1, CTM=1, CC=0, CS=0: */
/* #define MPC83XX_DEFAULT_DMAMR                0x0720008C */
#define MPC83XX_DEFAULT_DMAMR                0x0630008C

/* mpc834XDma registers, fields and masks */

/* special constants */

/* base address offset from CCS_BASE */
#ifndef CCS_BASE
#define CCS_BASE CCSBAR
#endif

#define DMAMSG_REG_BA                        0x8000



/* DMA & Messaging unit registers */

/* Message & doorbell registers: */
#define M83XX_OMISR(base)      (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0030))
#define M83XX_OMIMR(base)      (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0034))
#define M83XX_IMR0(base)       (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0050))
#define M83XX_IMR1(base)       (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0054))
#define M83XX_OMR0(base)       (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0058))
#define M83XX_OMR1(base)       (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x005C))
#define M83XX_ODR(base)        (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0060))
#define M83XX_IDR(base)        (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0068))
#define M83XX_IMISR(base)      (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0080))
#define M83XX_IMIMR(base)      (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0084))

/* General DMA register access based on channel number: */
#define M83XX_DMAMR(base)      (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0100))
#define M83XX_DMASR(base)      (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0104))
#define M83XX_DMACDAR(base)    (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0108))
#define M83XX_DMASAR(base)     (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0110))
#define M83XX_DMADAR(base)     (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0118))
#define M83XX_DMABCR(base)     (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0120))
#define M83XX_DMANDAR(base)    (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0124))

/* DMA offsets */
#define M83XX_DMAMR_OFFSET     (CAST(VUINT32)(0x0000))
#define M83XX_DMASR_OFFSET     (CAST(VUINT32)(0x0004))
#define M83XX_DMACDAR_OFFSET   (CAST(VUINT32)(0x0008))
#define M83XX_DMASAR_OFFSET    (CAST(VUINT32)(0x0010))
#define M83XX_DMADAR_OFFSET    (CAST(VUINT32)(0x0018))
#define M83XX_DMABCR_OFFSET    (CAST(VUINT32)(0x0020))
#define M83XX_DMANDAR_OFFSET   (CAST(VUINT32)(0x0024))

/* DMA0 registers */
#define M83XX_DMAMR0(base)     (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0100))
#define M83XX_DMASR0(base)     (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0104))
#define M83XX_DMACDAR0(base)   (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0108))
#define M83XX_DMASAR0(base)    (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0110))
#define M83XX_DMADAR0(base)    (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0118))
#define M83XX_DMABCR0(base)    (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0120))
#define M83XX_DMANDAR0(base)   (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0124))

/* DMA1 registers */
#define M83XX_DMAMR1(base)     (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0180))
#define M83XX_DMASR1(base)     (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0184))
#define M83XX_DMACDAR1(base)   (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0188))
#define M83XX_DMASAR1(base)    (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0190))
#define M83XX_DMADAR1(base)    (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0198))
#define M83XX_DMABCR1(base)    (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x01A0))
#define M83XX_DMANDAR1(base)   (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x01A4))

/* DMA2 registers */
#define M83XX_DMAMR2(base)     (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0200))
#define M83XX_DMASR2(base)     (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0204))
#define M83XX_DMACDAR2(base)   (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0208))
#define M83XX_DMASAR2(base)    (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0210))
#define M83XX_DMADAR2(base)    (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0218))
#define M83XX_DMABCR2(base)    (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0220))
#define M83XX_DMANDAR2(base)   (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0224))

/* DMA3 registers */
#define M83XX_DMAMR3(base)     (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0280))
#define M83XX_DMASR3(base)     (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0284))
#define M83XX_DMACDAR3(base)   (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0288))
#define M83XX_DMASAR3(base)    (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0290))
#define M83XX_DMADAR3(base)    (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x0298))
#define M83XX_DMABCR3(base)    (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x02A0))
#define M83XX_DMANDAR3(base)   (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x02A4))

/* DMA general status register */
#define M83XX_DMAGSR(base)     (CAST(VUINT32 *)((base) + DMAMSG_REG_BA + 0x02A8))


/* Bit definitions */

#define M83XX_MSG_REG_MGS0        0x01  /* Message register 0 interrupt pending */
#define M83XX_MSG_REG_MGS1        0x02  /* Message register 1 interrupt pending */
#define M83XX_MSG_REG_DOORBELL    0x08  /* Inbound doorbell interrupt pending */

/* DMA mode register */
#define M83XX_DMAMR_CS            0x01  /* Start/Stop DMA bit */

/* DMA status register */
#define M83XX_DMASR_TE            0x80  /* Transfer Error bit */
#define M83XX_DMASR_CB            0x04  /* Channel busy bit */
#define M83XX_DMASR_EOSI          0x02  /* End of Segment Interrupt bit */
#define M83XX_DMASR_EOCDI         0x01  /* End of Chain/Direct Interrupt bit */


/* types */

typedef enum  /* two message registers available, select which one with this type */
    {
    MPC834XDMA_MSG_REG0,
    MPC834XDMA_MSG_REG1
    } MPC834XDMA_MSG_REG;


typedef struct mpc834XDma_chan  /* describes a DMA channel */
    {
    UINT32   chanId;    /* channel 0-3 */
    VUINT32  *base;     /* base address of DMA register set for this channel */
    UINT32   dmamr;     /* DMA mode register value */
    SEM_ID   mutex;     /* exclusive access to channel */
    FUNCPTR  callback;  /* notification routine */
    BOOL     available; /* True if channel has no active client */
    UINT32   user_data; /* 32-bit field for client data usage, passed to callback routine */
    } MPC834XDMA_CHAN;


typedef struct mpc834XDma_descriptor  /* DMA descriptor from 8349 manual */
    {
    UINT32 sourceAddr;   /* source DMA address */
    UINT32 reserved1;    /* reserved on mpc834x */
    UINT32 destAddr;     /* destination DMA address */
    UINT32 reserved2;    /* reserved on mpc834x */
    UINT32 nextAddr;     /* next in chain, not used by this library */
    UINT32 reserved3;    /* reserved on mpc834x */
    UINT32 byteCount;    /* byte count for DMA transfer */
    UINT32 reserved4;    /* reserved on mpc834x */
    } MPC834XDMA_DESCRIPTOR;


/* public declarations */

/* DMA APIs */
IMPORT STATUS mpc834xDmaInit(UINT32 immrBar, UINT32 dmaINum, UINT32 msgINum);
IMPORT STATUS mpc834xDmaMemCopy(MPC834XDMA_CHAN *pChan, UINT32 dmamr, PHYS_ADDR dst, PHYS_ADDR src, UINT32 size, UINT32 user_data);
IMPORT MPC834XDMA_CHAN *mpc834xDmaReserveChannel(void);
IMPORT STATUS mpc834xDmaReleaseChannel(MPC834XDMA_CHAN *pChannel);
IMPORT STATUS mpc834xDmaCallbackAdd(MPC834XDMA_CHAN *pChan, FUNCPTR pCallback);
IMPORT STATUS mpc834xDmaCallbackDelete(MPC834XDMA_CHAN *pChan);
IMPORT STATUS mpc834xDmaSuspend(MPC834XDMA_CHAN *pChan);
IMPORT STATUS mpc834xDmaResume(MPC834XDMA_CHAN *pChan);

/* Message passing APIs */
IMPORT STATUS mpc834xMsgRegRead(MPC834XDMA_MSG_REG whichMsgReg, UINT32 *message);
IMPORT STATUS mpc834xMsgRegWrite(MPC834XDMA_MSG_REG whichMsgReg, UINT32 msg);

/* Doorbell register APIs */
IMPORT UINT32 mpc834xDoorbellRead(void);
IMPORT void   mpc834xDoorbellWrite(UINT32 doorbellValue);

IMPORT STATUS mpc834xMsgDoorbellCallbackAdd(FUNCPTR inMsgHandler, UINT32 inMsgUserArg, FUNCPTR outMsgHandler, UINT32 outMsgUserArg, FUNCPTR inDoorbellHandler, UINT32 inDoorbellUserArg, FUNCPTR outDoorbellHandler, UINT32 outDoorbellUserArg);


#ifdef __cplusplus
}
#endif

#endif  /* INCmpc834XDmah */
