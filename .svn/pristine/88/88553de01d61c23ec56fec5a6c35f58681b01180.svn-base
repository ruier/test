/* vxbFslDma.c - Freescale PowerPC DMA driver */

/*
 * Copyright (c) 2009-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01n,14jun13,fao  Fix Coverity Prevent warnings. (WIND00414265)
01m,14may13,y_y  use "\NOMANUAL" insead of local for vxbFslDmaMemCopy.
01l,31mar13,y_y  add basic direct and chaining link mode support,
                 and clean up the code. (WIND00405789)
01k,28mar13,syt  fix coverity error.
01j,14nov12,syt  add a macro DMA_CHAN_GROUP_OFFSET to fit for eight channels type
                 controller.
01i,12nov12,syt  add a parameter chanNum to specify the DMA channel number.
01h,28mar12,sye  fix wrong parameter when calling user callback. (WIND00288572)
01g,26mar12,sye  vxbFslDmaResourceGet() should use parameter pDev.(WIND00288799)
01f,31aug10,y_c  fix the bugs that lead the dma work incorrectly.(WIND00227382)
01e,19aug10,y_c  fix the wrong arguments of callback function in vxbFslDmaRead
                 and vxbFslDmaWrite.(WIND00227589)
01d,17aug10,y_c  add the judgement of the request minQueueDepth in function
                 vxbFslDmaResourceGet.(WIND00227590)
01c,08jul10,y_c  the function vxbFslDmaResourceGet should use the parameter
                 pReqDev.(WIND00193282)
01b,24sep09,jc0  fix bugs reported from Coverity CIDs: 6726, 6727, 6728
                 (WIND00183713)
01a,12jun09,syt  originally implemented for P2020.
*/

/*
DESCRIPTION
This is a driver for the Freescale's DMA engine. It is intended for use by
client drivers performing other high level functions.

This driver provide asynchronous and synchronous DMA transfer functions.
Basic Chaining Mode and Direct mode are supported in this version, and list
descriptor not support yet.

Both vxbFslDmaWriteAndWait and vxbFslDmaReadAndWait are synchronous routines,
support working as polling or interrupt(default) mode which can config by paramter
pDrvCtrl->polling. In interrupt mode, DMA works as basic chaining mode and
provide mutual-exclusion mechanism to guard the DMA channel. In poll mode,
DMA will work in direct mode and no mutual exclusion mechanism are invoked
(Caller provide if need).

Both vxbFslDmaWrite and vxbFslDmaRead are asynchronous routines, which implemented
by DMA basic Chaining Mode and only support interrupt mode.

Additinally, in basic Chaining Mode, the maximum transfer size depends on
numOfLinks(16 by default), currentlly, the maximum size is about 1GB(16*0x3ffffff).
The max number of link descriptors can be also set via "numOfLinks" in hwconf.c in BSP.

Note: When two or more channels are used at the same time with a extremely high
frequency, one channel may stop because "transmit done" status, which can lead
to interrupt, is not marked by DMA hardware. Furthermore, a poor transfer
performance will be achieved because the channels are vying for the same
resources such as the DRAM bandwidth and global link table, so it's recommended
to use one channel to transfer your data.

EXTERNAL INTERFACE

The driver provides the standard vxbus external interface vxbFslDmaRegister().
This function registers the driver with the vxbus subsystem, and instances will
be created as needed.  DMA is a local bus device, each device instance must be
specified in the hwconf.c file in a BSP. 

The hwconf entry must specify the following parameters:

\is

\i <regBase>
Specifies the base address where the controller's CSR registers are mapped
into the host's address space. All register offsets are computed relative
to this address.

\i <numOfLinks>
Specifies the number of DMA link descriptor nodes in link list. If left unspecified,
default value 16 will be used.

\i <chanNum>
Specifies how many channels on one DMA module. Most of Freescale QorIQ series
DMA controller module has four channels, but for some SOC, such as T4240,
it has eight. If this parameter not provided in hwconf.c, the driver will
set the channel number as four by default.

\i <polling>
Specified the synchronous DMA transfer function (vxbFslDmaWriteAndWait/vxbFslDmaReadAndWait)
use interrupt or polling mode. Also, interrupt mode implemented by DMA basic chaining
mode, polling mode implemented by DMA basic direct mode.

\ie

An example hwconf entry is shown below:

\cs
const struct hcfResource fslDma0Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,    { (void *)DMA0_REGBASE } },
};
#define fslDma0Num NELEMENTS(fslDma0Resources)
\ce

INCLUDE FILES: vxbFslDma.h
*/

/* includes */

#include <vxWorks.h>
#include <iv.h>
#include <drv/pci/pciConfigLib.h>
#include <drv/pci/pciIntLib.h>
#include <vmLib.h>
#include <private/vmLibP.h>
#include <taskLib.h>
#include <../h/hwif/util/hwMemLib.h>

#include <cacheLib.h>
#include <excLib.h>
#include <stdlib.h>
#include <string.h>

#include <logLib.h>
#include <stdio.h>

#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/vxbPciLib.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbDmaLib.h>
#include <../src/hwif/h/vxbus/vxbAccess.h>
#include <../src/hwif/h/util/vxbDmaDriverLib.h>
#include <../src/hwif/h/dmaCtlr/vxbFslDma.h>

/* defines */

#undef FSL_DMA_DEBUG

#ifdef FSL_DMA_DEBUG

#ifdef  LOCAL
#undef  LOCAL
#define LOCAL
#endif

void vxbFslDmaRegDump (VXB_DEVICE_ID);
IMPORT FUNCPTR _func_logMsg;
LOCAL int debugLevel = 0;

#   define PRINTF_DEBUG
#   ifdef PRINTF_DEBUG
#   define FSL_DMA_LOG(lvl,fmt, X1, X2, X3, X4, X5, X6)  \
    if (debugLevel >= lvl)                                 \
        printf(fmt, X1, X2, X3, X4, X5, X6)
#   else /* PRINTF_DEBUG */
#   define FSL_DMA_LOG(lvl, fmt, X1, X2, X3, X4, X5, X6) \
       if (debugLevel >= lvl)                              \
          {                                                \
          if (_func_logMsg != NULL)                        \
              _func_logMsg (fmt, (int)X1, (int)X2, (int)X3,\
               (int)X4, (int)X5, (int)X6);                 \
          }
#   endif /* PRINTF_DEBUG */
#else /* FSL_DMA_DEBUG */
#   undef  FSL_DMA_LOG
#   define FSL_DMA_LOG(lvl,fmt,a,b,c,d,e,f)
#endif  /* VXB_FSL_DMA_DEBUG */

/* forward declarations */

LOCAL void vxbFslDmaInstInit (VXB_DEVICE_ID);
LOCAL void vxbFslDmaInstInit2 (VXB_DEVICE_ID);
LOCAL void vxbFslDmaInstConnect (VXB_DEVICE_ID);
LOCAL void vxbFslDmaFreeDescriptor (VXB_DEVICE_ID, LIST *, FSL_DMA_NODE *);
LOCAL void vxbFslDmaExcJob (VXB_DEVICE_ID, FSL_DMA_CHAN *, STATUS);
LOCAL void vxbFslDmaChanIsr (FSL_DMA_DRV_CTRL *);
LOCAL STATUS vxbFslDmaResourceGet (VXB_DEVICE_ID, VXB_DEVICE_ID, 
                                   struct vxbDmaRequest *);
LOCAL STATUS vxbFslDmaResourceRelease (VXB_DEVICE_ID, VXB_DMA_RESOURCE_ID);
LOCAL STATUS vxbFslDmaInstUnlink (VXB_DEVICE_ID, void *);
LOCAL STATUS vxbFslDmaSubmitJob (VXB_DMA_RESOURCE_ID, FSL_DMA_LINK_DESC *);
LOCAL STATUS vxbFslDmaResume (VXB_DMA_RESOURCE_ID);
LOCAL STATUS vxbFslDmaSuspend (VXB_DMA_RESOURCE_ID);
LOCAL FSL_DMA_NODE * vxbFslDmaGetDescriptor (VXB_DEVICE_ID, FSL_DMA_CHAN *);

LOCAL DRIVER_INITIALIZATION vxbFslDmaFuncs =
    {
    vxbFslDmaInstInit,          /* devInstanceInit */
    vxbFslDmaInstInit2,         /* devInstanceInit2 */
    vxbFslDmaInstConnect        /* devConnect */
    };

LOCAL struct vxbPlbRegister fslDmaDevRegistration =
    {
        {
        NULL,                   /* pNext */
        VXB_DEVID_DEVICE,       /* devID */
        VXB_BUSID_PLB,          /* busID = PLB */
        VXB_VER_4_0_0,          /* vxbVersion */
        "fslDma",               /* drvName */
        &vxbFslDmaFuncs,        /* pDrvBusFuncs */
        NULL,                   /* pMethods */
        NULL                    /* devProbe */
        },
    };

LOCAL device_method_t   vxbFslDmaMethods[] =
    {
    DEVMETHOD(vxbDmaResourceGet,        vxbFslDmaResourceGet),
    DEVMETHOD(vxbDmaResourceRelease,    vxbFslDmaResourceRelease),
    DEVMETHOD(vxbDrvUnlink,             vxbFslDmaInstUnlink),
    {0, 0}
    };

/* imported functions supplied by the BSP */

IMPORT atomicVal_t    vxAtomicSet (atomic_t *, atomicVal_t);
IMPORT atomicVal_t    vxAtomicOr (atomic_t *, atomicVal_t);
IMPORT void           vxbUsDelay(int);

/*******************************************************************************
*
* vxbFslDmaInstInit - first pass initialization
*
* This function implements the VxBus instInit handler for a DMA controller
* device instance.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void vxbFslDmaInstInit
    (
    VXB_DEVICE_ID pDev
    )
    {
    FSL_DMA_DRV_CTRL * pDrvCtrl;
    struct hcfDevice * pHcf;

    if ((pDrvCtrl = (FSL_DMA_DRV_CTRL *)hwMemAlloc (sizeof(FSL_DMA_DRV_CTRL))) == NULL)
        return;

    pDrvCtrl->vxbDev = pDev;

    pDrvCtrl->regBase = pDev->pRegBase[0];
    vxbRegMap (pDev, 0, &pDrvCtrl->handle);

    /* fill in bus subsystem fields */

    pDev->pDrvCtrl = (void *)pDrvCtrl;
    pDev->pMethods = &vxbFslDmaMethods[0];

    pHcf = (struct hcfDevice *)hcfDeviceGet(pDev);

    /*
     * resourceDesc {
     * The chanNum resource specifies chanel number on each
     * DMA controller. }
     */

    if (devResourceGet(pHcf, "chanNum", HCF_RES_INT, (void *) &pDrvCtrl->chanNum) != OK)
        pDrvCtrl->chanNum = DMA_CHAN_NUM;

    /*
     * resourceDesc {
     * The polling resource specifies whether
     * the driver uses polling mode or not.
     * If this property is not explicitly
     * specified, the driver uses interrupt
     * by default. }
     */

    if (devResourceGet (pHcf, "polling", HCF_RES_INT,
                       (void *) &pDrvCtrl->polling) != OK)
        pDrvCtrl->polling = FALSE;

    /*
     * resourceDesc {
     * The numOfLinks resource specifies the number of
     * nodes in link descriptor list. Each link descriptor
     * can support transfer no more than 0x3ffffff bytes once.
     * If this property is not explicitly specified, the driver
     * uses 16 by default. That means the maximum transfer size
     * is about 1GB in basic Chaining mode.}
     */

    if (devResourceGet (pHcf, "numOfLinks", HCF_RES_INT,
                       (void *) &pDrvCtrl->numOfLinks) != OK)
        pDrvCtrl->numOfLinks = DMA_LINK_DESC_NUM;

    pDrvCtrl->fslDmaPhase = 1;
    }

/*******************************************************************************
*
* vxbFslDmaInstInit2 - second pass initialization
*
* This routine performs the second level initialization of the DMA controller.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void vxbFslDmaInstInit2
    (
    VXB_DEVICE_ID pDev
    )
    {
    FSL_DMA_DRV_CTRL * pDrvCtrl = pDev->pDrvCtrl;
    FSL_DMA_LINK_DESC * pDesc;
    UINT32 chan, desc;

    if (pDrvCtrl->dmaInitialized == TRUE)
        return;

    pDrvCtrl->lstMutex = semMCreate ( SEM_Q_PRIORITY | SEM_DELETE_SAFE | \
                                      SEM_INVERSION_SAFE );

    if (pDrvCtrl->lstMutex == NULL)
        return;

    pDrvCtrl->fslDmaSemId = semBCreate (SEM_Q_FIFO, SEM_EMPTY);

    if (pDrvCtrl->fslDmaSemId == NULL)
        {
        (void)semDelete (pDrvCtrl->lstMutex);
        return;
        }

    pDrvCtrl->fslDmaSyncSem = semBCreate (SEM_Q_FIFO, SEM_EMPTY);

    if (pDrvCtrl->fslDmaSyncSem == NULL)
        {
        (void)semDelete (pDrvCtrl->lstMutex);
        (void)semDelete (pDrvCtrl->fslDmaSemId);
        return;
        }

    pDrvCtrl->dmaInitialized = TRUE;

    lstInit (&pDrvCtrl->freeLink);

    pDrvCtrl->pClients = (FSL_DMA_CHAN *)hwMemAlloc (sizeof(FSL_DMA_CHAN *) *  \
                                                 pDrvCtrl->chanNum);
    if (pDrvCtrl->pClients == NULL)
        {
        (void)semDelete (pDrvCtrl->lstMutex);
        (void)semDelete (pDrvCtrl->fslDmaSemId);
        (void)semDelete (pDrvCtrl->fslDmaSyncSem);
        return;
        }

    pDesc = (FSL_DMA_LINK_DESC *)cacheDmaMalloc(sizeof (FSL_DMA_LINK_DESC) * \
                                                pDrvCtrl->numOfLinks);

    pDrvCtrl->pDesc = pDesc;

    if (pDesc == NULL)
        {
        (void)semDelete (pDrvCtrl->lstMutex);
        (void)semDelete (pDrvCtrl->fslDmaSemId);
        (void)semDelete (pDrvCtrl->fslDmaSyncSem);

#ifndef _VXBUS_BASIC_HWMEMLIB
        (void)hwMemFree ((char *)pDrvCtrl->pClients);
#endif /* _VXBUS_BASIC_HWMEMLIB */
        return;
        }

    for (desc = 0; desc < pDrvCtrl->numOfLinks; desc++, pDesc++)
        {
        FSL_DMA_NODE * pNode = malloc (sizeof(FSL_DMA_NODE));

        if (pNode == NULL)
            {
            (void)semDelete (pDrvCtrl->lstMutex);
            (void)semDelete (pDrvCtrl->fslDmaSemId);
            (void)semDelete (pDrvCtrl->fslDmaSyncSem);

#ifndef _VXBUS_BASIC_HWMEMLIB
            (void)hwMemFree ((char *)pDrvCtrl->pClients);
#endif /* _VXBUS_BASIC_HWMEMLIB */
            cacheDmaFree (pDrvCtrl->pDesc);

            return;
            }

        pNode->descriptor = pDesc;

        lstAdd (&pDrvCtrl->freeLink, (NODE*)pNode);
        }

    /* init DMA channels */

    for (chan = 0; chan < pDrvCtrl->chanNum; chan++)
        {
        pDrvCtrl->pClients[chan] = NULL;

        DMA_CHAN_REG_WRITE(pDev, chan, DMA_SR, \
                           DMA_CHAN_REG_READ(pDev, chan, DMA_SR));

        DMA_CHAN_REG_WRITE(pDev, chan, DMA_MR, DMA_MR_INIT_VALUE);
        DMA_CHAN_REG_WRITE(pDev, chan, DMA_ECLNDAR, 0x00);
        DMA_CHAN_REG_WRITE(pDev, chan, DMA_CLNDAR, 0x00);
        DMA_CHAN_REG_WRITE(pDev, chan, DMA_SATR, 0x00);
        DMA_CHAN_REG_WRITE(pDev, chan, DMA_SAR, 0x00);
        DMA_CHAN_REG_WRITE(pDev, chan, DMA_DATR, 0x00);
        DMA_CHAN_REG_WRITE(pDev, chan, DMA_DAR, 0x00);
        DMA_CHAN_REG_WRITE(pDev, chan, DMA_BCR, 0x00);
        DMA_CHAN_REG_WRITE(pDev, chan, DMA_ENLNDAR, 0x00);
        DMA_CHAN_REG_WRITE(pDev, chan, DMA_NLNDAR, 0x00);
        DMA_CHAN_REG_WRITE(pDev, chan, DMA_ECLSDAR, 0x00);
        DMA_CHAN_REG_WRITE(pDev, chan, DMA_CLSDAR, 0x00);
        DMA_CHAN_REG_WRITE(pDev, chan, DMA_ENLSDAR, 0x00);
        DMA_CHAN_REG_WRITE(pDev, chan, DMA_NLSDAR, 0x00);
        DMA_CHAN_REG_WRITE(pDev, chan, DMA_SSR, 0x00);
        DMA_CHAN_REG_WRITE(pDev, chan, DMA_DSR, 0x00);

        vxbIntConnect (pDev, chan, vxbFslDmaChanIsr, pDrvCtrl);
        vxbIntEnable (pDev, chan, vxbFslDmaChanIsr, pDrvCtrl);

        }

    pDrvCtrl->fslDmaPhase = 2;
    }

/*******************************************************************************
*
* vxbFslDmaInstConnect - final initialization
*
* This routine performs the third level initialization of the DMA controller
* driver. Nothing to be done in this routine.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void vxbFslDmaInstConnect
    (
    VXB_DEVICE_ID pDev
    )
    {
    FSL_DMA_DRV_CTRL * pDrvCtrl = pDev->pDrvCtrl;

    pDrvCtrl->fslDmaPhase = 3;

    }

/*******************************************************************************
*
* vxbFslDmaRegister - register driver with vxbus
*
* RETURNS: N/A
*
* ERRNO
*/

void vxbFslDmaRegister (void)
    {
    vxbDevRegister ((struct vxbDevRegInfo *)&fslDmaDevRegistration);
    }

/*******************************************************************************
*
* vxbFslDmaMemCopy - copy memory from one address to another
*
* This routine copies user defined size bytes from source to destination
* address. The final argument is a 32bit unsigned integer used to manage or
* track the completion of vxbFslDmaMemCopy(). Possible usage for userData
* includes semaphore identifiers (SEM_ID), an array index, or as a general
* purpose unique identifier. The routine use the basic Chaining mode transmit the
* DMA data, the maximum transfer size depends on the numOfLinks. Currently,
* the maximum transfer size is about 1GB by default.
*
* RETURNS: OK if there are enough descriptors to fulfill the request
* or ERROR if alignment, address or size constraints would be violated.
*
* SEE ALSO: vxbFslDmaReserveChannel, vxbFslDmaReleaseChannel
*
* \NOMANUAL
*/

STATUS vxbFslDmaMemCopy
    (
    VXB_DMA_RESOURCE_ID dmaRes,
    char *              dst,
    char *              src,
    UINT32              size,
    pVXB_DMA_COMPLETE_FN completeFunc,
    UINT32              userData
    )
    {
    VXB_DEVICE_ID       pDev;
    FSL_DMA_DRV_CTRL *  pDrvCtrl;
    FSL_DMA_CHAN *      pChan;
    BOOL                releaseDescriptors = FALSE;
    FSL_DMA_NODE *      pNode;
    FSL_DMA_NODE *      headNode;
    UINT32              chanMode;
    int i, j;

    /* get the DMA engine vxbus instance id */

    pDev = dmaRes->dmaInst;
    pDrvCtrl = pDev->pDrvCtrl;

    /* get the associated DMA channel data */

    pChan = dmaRes->pDmaChan;

    if (size == 0)
        {
        FSL_DMA_LOG(0, "[fslDmaMemCpy]: size should not be 0!\n",
                                            0, 1, 2, 3, 4, 5);
        return ERROR;
        }

    chanMode = DMA_CHAN_REG_READ(pDev, pChan->chanId, DMA_MR);

    if (chanMode & DMA_MR_DAHE)
        {
        if (((UINT32)dst % ((chanMode & DMA_MR_DAHTS_MASK) >> 15)) != 0)
            {
            FSL_DMA_LOG(0, "[fslDmaMemCpy]: Destination address not aligned!\n",
                                            0, 1, 2, 3, 4, 5);
            return ERROR;
            }
        }

    if (chanMode & DMA_MR_SAHE)
        {
        if (((UINT32)src % ((chanMode & DMA_MR_SAHTS_MASK) >> 13)) != 0)
            {
            FSL_DMA_LOG(0, "[fslDmaMemCpy]: Destination address not aligned!\n",
                                            0, 1, 2, 3, 4, 5);
            return ERROR;
            }
        }

    i = size / DMA_TRANS_MAX_SIZE;
    if (size % DMA_TRANS_MAX_SIZE)
        i++;

    FSL_DMA_LOG(1000, "DMA %x bytes from %x to %x need %d link\r\n",
                                        size, src, dst, i, 4, 5);

    /* save the number of the descriptors assigned */

    j = i;

    /* fail if available descriptors would not satisfy the operation */

    if (CHAN_SYNC_TAKE == ERROR)
        {
        return ERROR;
        }

    if (LST_SYNC_TAKE == ERROR)
        {
        CHAN_SYNC_GIVE;
        return ERROR;
        }

    if (i > lstCount (&pDrvCtrl->freeLink))
        {
        LST_SYNC_GIVE;
        CHAN_SYNC_GIVE;
        FSL_DMA_LOG(0, "No free NODE! Increase the numOfLinks!\n",
                        1, 2, 3, 4, 5, 6);
        return ERROR;
        }

    headNode = vxbFslDmaGetDescriptor (pDev, pChan);
    pNode = headNode;

    if (pNode == NULL)
        {
        LST_SYNC_GIVE;

        CHAN_SYNC_GIVE;

        return ERROR;
        }

    headNode->callback = completeFunc;
    headNode->userData = userData;
    headNode->cookie = size;

    for (; i > 0; i--)
        {
        FSL_DMA_LINK_DESC * pDescriptor = pNode->descriptor;
        UINT32 tcr = (size >= DMA_TRANS_MAX_SIZE) ? DMA_TRANS_MAX_SIZE : size;

        pDescriptor->srcAddr = (UINT32)src;
        pDescriptor->srcAttr = FSL_DMA_READ_TYPE;
        pDescriptor->dstAddr = (UINT32)dst;
        pDescriptor->dstAttr = FSL_DMA_WRITE_TYPE;
        pDescriptor->byteCnt = tcr;
        pDescriptor->nextLinkDescExAddr = 0;
        pDescriptor->nextLinkDescAddr = NLNDAR_EOLND;
       /*logMsg("srcAddr = 0x%x..srcAttr = 0x%x..dstAddr = 0x%x..dstAttr = 0x%x,byteCnt = 0x%x\n",
                pDescriptor->srcAddr, pDescriptor->srcAttr, pDescriptor->dstAddr, pDescriptor->dstAttr,
                pDescriptor->byteCnt, 0);*/
        /* If this operation takes more than one descriptor */

        if (i > 1)
            {
            pNode = vxbFslDmaGetDescriptor (pDev, pChan);

            if (pNode == NULL)
                {
                LST_SYNC_GIVE;

                CHAN_SYNC_GIVE;

                return ERROR;
                }

            size = size - DMA_TRANS_MAX_SIZE;

            src += DMA_TRANS_MAX_SIZE;
            dst += DMA_TRANS_MAX_SIZE;

            pDescriptor->nextLinkDescAddr = (UINT32) pNode->descriptor & 0xffffffff;
            pDescriptor->nextLinkDescExAddr = 0;
            }
        else
            {
            LST_SYNC_GIVE;
            }

        FSL_DMA_LOG(1000, "Link#%d src=%8x dst=%8x CNT=%7x nxtL=%8x pDes=%8x\r\n",
                           j-i,
                           pDescriptor->srcAddr,
                           pDescriptor->dstAddr,
                           pDescriptor->byteCnt,
                           pDescriptor->nextLinkDescAddr,
                           (UINT32)pDescriptor);

        }

    /*
     * here we try to submit requirement for 2 times since hardware
     * occasionally fail to send.
     */

    if (vxbFslDmaSubmitJob (dmaRes, headNode->descriptor) == ERROR)
        if (vxbFslDmaSubmitJob (dmaRes, headNode->descriptor) == ERROR)
            releaseDescriptors = TRUE;

    if (releaseDescriptors)
        {
        for (i = 0; i < j; i++)
            {
            pNode = (void*)lstLast (&pChan->dList);
            vxbFslDmaFreeDescriptor (pDev, &pChan->dList, pNode);
            }
        CHAN_SYNC_GIVE;
        return ERROR;
        }

    CHAN_SYNC_GIVE;

    return OK;
    }

/*******************************************************************************
*
* vxbFslDmaDirectMode - copy memory from one address to another via direct mode
*
* This routine copies user defined size bytes from source to destination
* address via DMA basic direct mode. Compared with vxbFslDmaMemCopy, user
* should notice that there is no DMA interrupts are invoked, polling the CB bits
* instead. Additionally, this routine don't provide task synchronization or
* mutual exclusion. If multiple tasks will access a DMA channel, that channel
* must be guarded with some mutual-exclusion mechanism
* (e.g., a mutual-exclusion semaphore).
*
* RETURNS: OK or ERROR if DMA transfer fail.
*
* SEE ALSO: vxbFslDmaReserveChannel, vxbFslDmaReleaseChannel
*
*/

LOCAL STATUS vxbFslDmaDirectMode
    (
    VXB_DMA_RESOURCE_ID dmaRes,
    char *              dst,
    char *              src,
    UINT32              size
    )
    {
    VXB_DEVICE_ID      pDev;
    FSL_DMA_CHAN *     pChan;
    UINT32             chanMode;
    UINT32             tmpSr=0;
    UINT32             bcr;
    UINT32             timeout = 0xffffffff;
#ifdef FSL_DMA_DEBUG
    UINT32             sr=0, i=0;
#endif

    /* get the DMA engine vxbus instance id */

    pDev = dmaRes->dmaInst;

    /* get the associated DMA channel data */

    pChan = dmaRes->pDmaChan;

    if (size == 0)
        {
        FSL_DMA_LOG(0, "size should not be 0!\n", 1, 2, 3, 4, 5, 6);
        return ERROR;
        } 

    /* Snoop enable */

    DMA_CHAN_REG_WRITE(pDev, 0, DMA_SATR, FSL_DMA_READ_TYPE);
    DMA_CHAN_REG_WRITE(pDev, 0, DMA_DATR, FSL_DMA_WRITE_TYPE);

    /* clear any errors */

    DMA_CHAN_REG_WRITE(pDev, 0, DMA_SR,   DMA_SR_MASK);

    chanMode = 0;

    while (size)
        {
        bcr = min (DMA_TRANS_MAX_SIZE, size);

        /* Poll the Channel state for Idle */

        tmpSr =  DMA_CHAN_REG_READ(pDev, pChan->chanId, DMA_SR) & DMA_SR_CB;

        if (tmpSr != 0)
            return ERROR;

        /* Initialize SARn, SATRn, DARn, DATRn and BCRn. */

        /* Source Address */

        DMA_CHAN_REG_WRITE(pDev, pChan->chanId, DMA_SAR, (UINT32)src);

        /* Destination Address */

        DMA_CHAN_REG_WRITE(pDev, pChan->chanId, DMA_DAR, (UINT32)dst);

        DMA_CHAN_REG_WRITE(pDev, pChan->chanId, DMA_BCR, bcr);

        /* Set MRn[CTM], to indicate direct mode. */

        chanMode |= DMA_MR_CTM;           /* Direct */

        /* Clear then set the MRn[CS], to start the DMA transfer.*/

        DMA_CHAN_REG_WRITE(pDev, pChan->chanId, DMA_MR, 0);

        chanMode |= DMA_MR_CS;

        DMA_CHAN_REG_WRITE(pDev, pChan->chanId, DMA_MR,
                           chanMode | DMA_MR_BWC_MASK);

        /* While the channel is busy, spin */

        while (timeout--)
            {
            if (!(DMA_CHAN_REG_READ(pDev, pChan->chanId, DMA_SR) & DMA_SR_CB))
                break;
            }

        /* Timed out */

        if (timeout == 0)
            return ERROR;

        /* clear MR[CS] channel start bit */

        DMA_CHAN_REG_WRITE(pDev, pChan->chanId, DMA_MR, chanMode & ~DMA_MR_CS);

        FSL_DMA_LOG(1000, "poll#%d src=%8x dst=%8x cnt=%7x sr=%x mr=%x\r\n",
                           i++, src, dst, bcr, sr, chanMode);

        size -= bcr;
        src += bcr;
        dst += bcr;
        }

    return OK;
    }

/*******************************************************************************
*
* vxbFslDmaReserveChannel - reserve an DMA channel
*
* vxbFslDmaReserveChannel() returns pointer to an FSL_DMA_CHAN structure which
* is supplied by client software to the Fsl DMA driver as a handle for
* further channel activities.  vxbFslDmaReserveChannel() is usually followed by
* vxbFslDmaMemCopy().
*
* RETURNS: FSL_DMA_CHAN *
*
* SEE ALSO: vxbFslDmaMemCopy
*
*/

LOCAL FSL_DMA_CHAN * vxbFslDmaReserveChannel
    (
    VXB_DEVICE_ID   pDev
    )
    {
    FSL_DMA_DRV_CTRL * pDrvCtrl = pDev->pDrvCtrl;
    FSL_DMA_CHAN *  pChan = NULL;
    UINT32  chan;

    for (chan = 0; chan < pDrvCtrl->chanNum; chan++)
        {
        if (pDrvCtrl->pClients[chan] == NULL &&
            !(DMA_CHAN_REG_READ(pDev, chan, DMA_SR) & DMA_SR_CB))
            {
            pChan = malloc (sizeof (FSL_DMA_CHAN));

            if (pChan != NULL)
                {
                memset (pChan, 0, sizeof (FSL_DMA_CHAN));
                lstInit (&pChan->dList);
                pChan->chanId = chan;
                pChan->mutex = semMCreate (SEM_Q_PRIORITY | SEM_DELETE_SAFE |
                                           SEM_INVERSION_SAFE);
                if (pChan->mutex == NULL)
                    break;

                DMA_CHAN_REG_WRITE(pDev, pChan->chanId, DMA_SR, \
                            DMA_CHAN_REG_READ(pDev, pChan->chanId, DMA_SR));
                DMA_CHAN_REG_WRITE(pDev, pChan->chanId, DMA_MR, \
                            DMA_MR_INIT_VALUE);

                pDrvCtrl->pClients [chan] = pChan;

                FSL_DMA_LOG(1000, "Reserved Channel[%d] for DMA transfer\r\n",
                                   chan, 2, 3, 4, 5, 6);
                }
            break;
            }
        }

    return (pChan);
    }

/*******************************************************************************
*
* vxbFslDmaReleaseChannel - release a reserved channel back to the driver
*
* vxbFslDmaReleaseChannel() releases a channel back to the FSL DMA driver.
*
* RETURNS: OK or ERROR if unable to release channel
*
* SEE ALSO: vxbFslDmaReserveChannel
*
*/

LOCAL STATUS vxbFslDmaReleaseChannel
    (
    VXB_DEVICE_ID   pDev,
    FSL_DMA_CHAN *  pChan
    )
    {
    FSL_DMA_DRV_CTRL * pDrvCtrl = pDev->pDrvCtrl;

    if (NULL == pChan)
        return ERROR;

    /* Wait until pChan->mutex free */

    if (CHAN_SYNC_TAKE == ERROR)
        return ERROR;

    /* ensure that channel is not in use */

    if ( DMA_CHAN_REG_READ(pDev, pChan->chanId, DMA_SR) & DMA_SR_CB )
        {
        CHAN_SYNC_GIVE;
        return ERROR;
        }

    DMA_CHAN_REG_WRITE(pDev, pChan->chanId, DMA_SR, \
                       DMA_CHAN_REG_READ(pDev, pChan->chanId, DMA_SR));

    DMA_CHAN_REG_WRITE(pDev, pChan->chanId, DMA_MR, 0x00);

    pDrvCtrl->pClients [pChan->chanId] = NULL;

    /* delete mutexes */

    if (semDelete (pChan->mutex) == ERROR)
        return ERROR;

    free (pChan);

    return OK;
    }

/*******************************************************************************
*
* vxbFslDmaSubmitJob - initiate/append DMA operations with a new descriptor chain
*
* vxbFslDmaSubmitJob() initiates DMA operations with a new descriptor chain
* or appends to the descriptor chain of any currently running DMA job
* if the channel is already active.
*
* RETURNS: OK if enough descriptors are available from the descriptor
* pool and all other sanity checks clear, otherwise ERROR.
*
* SEE ALSO: vxbFslDmaMemCopy, vxbFslDmaReserveChannel
*
*/

LOCAL STATUS vxbFslDmaSubmitJob
    (
    VXB_DMA_RESOURCE_ID dmaRes,
    FSL_DMA_LINK_DESC * pJob
    )
    {
    VXB_DEVICE_ID   pDev;
    FSL_DMA_CHAN *  pChan;
    FSL_DMA_NODE *  pNode;
    volatile UINT32 dmaMode;
    volatile UINT32 dmaStatus;
    volatile UINT32 i;

    /* check for invalid arguments */

    if ((NULL == dmaRes) || (NULL == pJob))
        {
        return ERROR;
        }
    else
        {
        pDev = dmaRes->dmaInst;
        pChan = dmaRes->pDmaChan;
        }

    if (((UINT32)pJob & ~NLNDAR_NLNDA_MASK))
        {
        return ERROR;
        }

    /* check for invalid arguments */

    if (((UINT32)pJob % FSL_DMA_DESC_ALIGN) ||
        (pJob->byteCnt > DMA_TRANS_MAX_SIZE))
        {
        return ERROR;
        }

    /* check if channel is active or not */

    dmaStatus = DMA_CHAN_REG_READ(pDev, pChan->chanId, DMA_SR);

    /* append new descriptor to current active chain */

    if (dmaStatus & DMA_SR_CB)
        {
        pNode = (FSL_DMA_NODE *)lstPrevious (lstLast (&pChan->dList));

        /* suspend Dma channel*/

        if (vxbFslDmaSuspend (dmaRes) != OK)
        {
        return ERROR;
        }

        /*
         * update terminal descriptor's Next link descriptor extended
         * and Next link descriptor address
         */

        pNode->descriptor->nextLinkDescExAddr = 0;
        pNode->descriptor->nextLinkDescAddr = ((UINT32)pJob & NLNDAR_NLNDA_MASK);

        if((UINT32)pNode->descriptor ==  \
           DMA_CHAN_REG_READ(pDev, pChan->chanId, DMA_CLNDAR))
            {
            DMA_CHAN_REG_WRITE(pDev, pChan->chanId, DMA_NLNDAR,  \
                               pNode->descriptor->nextLinkDescAddr);
            DMA_CHAN_REG_WRITE(pDev, pChan->chanId, DMA_ENLNDAR,  \
                               pNode->descriptor->nextLinkDescExAddr);
            }

        /* DMA Channel continue */

        if (vxbFslDmaResume (dmaRes) != OK)
            {
            return ERROR;
            }
        }
    else
        {

        /* stop this DMA channel */

        dmaMode = DMA_CHAN_REG_READ(pDev, pChan->chanId, DMA_MR);
        DMA_CHAN_REG_WRITE(pDev, pChan->chanId, DMA_MR, (dmaMode & ~DMA_MR_CS));

        /* clear any errors */

        DMA_CHAN_REG_WRITE(pDev, 0, DMA_SR, DMA_SR_MASK);

        /* set initial descriptor address */

        DMA_CHAN_REG_WRITE(pDev, pChan->chanId,  \
                           DMA_CLNDAR, ((UINT32)pJob & NLNDAR_NLNDA_MASK));
        DMA_CHAN_REG_WRITE(pDev, pChan->chanId, DMA_ECLNDAR, 0x00);

        /* clear status register */

        DMA_CHAN_REG_WRITE(pDev, pChan->chanId, DMA_SR, DMA_SR_TE   |
                                                        DMA_SR_PE   |
                                                        DMA_SR_EOLNI|
                                                        DMA_SR_EOLSI);

        /* set the start bit and away we go */

        dmaMode = DMA_CHAN_REG_READ(pDev, pChan->chanId, DMA_MR);
        DMA_CHAN_REG_WRITE(pDev, pChan->chanId, DMA_MR, (dmaMode | DMA_MR_CS));
        }

    /* check if channel is active or not */

    pChan->csr = DMA_CHAN_REG_READ(pDev, pChan->chanId, DMA_SR);

    if (!(pChan->csr & DMA_SR_CB))
        {
        for (i = 500; i > 0; i--)
            vxbUsDelay(25);
        pChan->csr = DMA_CHAN_REG_READ (pDev, pChan->chanId, DMA_SR);
        }

    /* still busy? */

    if (!(pChan->csr & DMA_SR_CB))
        {
#ifdef FSL_DMA_DEBUG
        vxbFslDmaRegDump(pDev); /* record the reg infos */
#endif
        return ERROR;
        }
    else
        return OK;

    }

/*******************************************************************************
*
* vxbFslDmaGetDescriptor - request a descriptor from the pool
*
* vxbFslDmaGetDescriptor() pulls a descriptor from the pool and adds it to
* the FSL_DMA_NODE structure's list of active descriptors.
*
* RETURNS: FSL_DMA_NODE* if a descriptor is available, or NULL if not
*
* SEE ALSO: vxbFslDmaFreeDescriptor
*/

LOCAL FSL_DMA_NODE * vxbFslDmaGetDescriptor
    (
    VXB_DEVICE_ID   pDev,
    FSL_DMA_CHAN *  pChan
    )
    {
    FSL_DMA_DRV_CTRL * pDrvCtrl = pDev->pDrvCtrl;
    NODE * pNode;

    pNode = lstGet (&pDrvCtrl->freeLink);

    if (pNode)
        {
        lstAdd (&pChan->dList, pNode);
        }
#ifdef FSL_DMA_DEBUG
    else
        {
        FSL_DMA_LOG(0, "%s to acquire a descriptor\n",
                                            __FUNCTION__, 1, 2, 3, 4, 5);
        }
#endif  /* FSL_DMA_DEBUG */

    return (FSL_DMA_NODE *)pNode;
    }

/*******************************************************************************
*
* vxbFslDmaFreeDescriptor - return a descriptor to the pool
*
* This routine returns a descriptor to the pool and removes
* it from FSL_DMA_NODE structure's list of active descriptors.
*
* RETURNS: N/A
*
* SEE ALSO: vxbFslDmaFreeDescriptor
*/

LOCAL void vxbFslDmaFreeDescriptor
    (
    VXB_DEVICE_ID   pDev,
    LIST *          oldList,
    FSL_DMA_NODE *  pNode
    )
    {
    FSL_DMA_DRV_CTRL * pDrvCtrl = pDev->pDrvCtrl;

    pNode->descriptor->nextLinkDescAddr = 0;
    pNode->descriptor->nextLinkDescExAddr = 0;

    if (oldList != NULL)
        lstDelete (oldList, (NODE *)pNode);

    if (LST_SYNC_TAKE == ERROR)
        return;

    lstAdd (&pDrvCtrl->freeLink, (NODE *)pNode);

    LST_SYNC_GIVE;
    }

/*******************************************************************************
*
* vxbFslDmaSuspend - suspend a DMA channel
*
* Suspend a DMA channel by setting the suspend bit in the Channel
* Control Register.
*
* RETURNS: OK if successful, ERROR if the suspend request failed
*
* SEE ALSO: vxbFslDmaResume, vxbFslDmaCancel
*
*/

LOCAL STATUS vxbFslDmaSuspend
    (
    VXB_DMA_RESOURCE_ID dmaRes
    )
    {
    VXB_DEVICE_ID  pDev = dmaRes->dmaInst;
    FSL_DMA_CHAN * pChan = dmaRes->pDmaChan;
    UINT32  timeOut = 0;
    volatile UINT32  regValue;

    regValue = DMA_CHAN_REG_READ(pDev, pChan->chanId, DMA_MR);

    DMA_CHAN_REG_WRITE(pDev, pChan->chanId, DMA_MR, (regValue & ~DMA_MR_CS));

    do
        {
        regValue = DMA_CHAN_REG_READ(pDev, pChan->chanId, DMA_SR);

        vxbUsDelay(1);

        timeOut++;
        }
    while (!((regValue & DMA_SR_CH) || (timeOut > 10)));

    if (timeOut > 10)
        {
        FSL_DMA_LOG(0, "DMA channel suspend failure!\r\n",
                                            0, 1, 2, 3, 4, 5);
        return ERROR;
        }

    return OK;
    }

/*******************************************************************************
*
* vxbFslDmaCancel - cancel a DMA operation on a given channel
*
* This function will cancel a read or write operation that was previously
* started on a given channel. This will prevent any further I/O from
* occuring on the channel until a new read or write operation is started.
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/

LOCAL STATUS vxbFslDmaCancel
    (
    VXB_DMA_RESOURCE_ID dmaRes
    )
    {
    VXB_DEVICE_ID  pDev = dmaRes->dmaInst;
    FSL_DMA_CHAN * pChan = dmaRes->pDmaChan;
    UINT32         regValue;

    regValue = DMA_CHAN_REG_READ(pDev, pChan->chanId, DMA_MR);
    regValue |= DMA_MR_CA;
    DMA_CHAN_REG_WRITE(pDev, pChan->chanId, DMA_MR, regValue);

    if (DMA_CHAN_REG_READ(pDev, pChan->chanId, DMA_SR) & DMA_SR_CB)
        return ERROR;
    else
        return OK;
    }

/*******************************************************************************
*
* vxbFslDmaResume - resume a DMA channel
*
* Resume a DMA channel previously suspended by vxbFslDmaSuspend.
*
* RETURNS: OK if successful, ERROR if the resume request failed
*
* SEE ALSO: vxbFslDmaSuspend, vxbFslDmaCancel
*
*/

LOCAL STATUS vxbFslDmaResume
    (
    VXB_DMA_RESOURCE_ID dmaRes
    )
    {
    VXB_DEVICE_ID  pDev = dmaRes->dmaInst;
    FSL_DMA_CHAN * pChan = dmaRes->pDmaChan;
    volatile UINT32 dmaSr;
    volatile UINT32 dmaMr;
    volatile UINT32 byteCount;

    dmaSr = DMA_CHAN_REG_READ(pDev, pChan->chanId, DMA_SR);
    dmaMr = DMA_CHAN_REG_READ(pDev, pChan->chanId, DMA_MR);
    byteCount = DMA_CHAN_REG_READ(pDev, pChan->chanId, DMA_BCR);

    if (!(dmaMr & DMA_MR_CS) && (dmaSr & DMA_SR_CH) && (byteCount > 0))
        {

        /*
         * If the channel is halted and there are bytes left
         * to transfer then resume the transfer.
         */

        DMA_CHAN_REG_WRITE(pDev, pChan->chanId, DMA_MR, (dmaMr | DMA_MR_CS));

        return OK;

        }
    else
        return ERROR;
    }

/*******************************************************************************
*
* vxbFslDmaExcJob - maintenance function called after DMA events
*
* vxbFslDmaExcJob() performs descriptor maintenance and calls client software
* callback functions whenever an interrupt occurs.
*
* RETURNS: N/A
*
*/

LOCAL void vxbFslDmaExcJob
    (
    VXB_DEVICE_ID  pDev,
    FSL_DMA_CHAN * pChan,
    STATUS         jobStatus
    )
    {
    FSL_DMA_NODE * pNode;
    UINT32  regValue;
    int nextNode;

    if (pChan == NULL)
        {
        return;
        }

    if (CHAN_SYNC_TAKE == ERROR)
        return;

    pNode = (FSL_DMA_NODE *)lstFirst (&pChan->dList);

    if (pNode != NULL)
        {
        if (pNode->callback != NULL)
            {
            if (jobStatus == OK)
                pNode->callback((void *)pNode->userData,
                                        pNode->cookie, DMA_STATUS_OK);
            else
                pNode->callback((void *)pNode->userData, 0, DMA_STATUS_ERROR);
            }
        }

    /* Free the node in Link list */

    while (pNode != NULL)
        {
        nextNode = 0;

        /* check if we have other nodes to release */

        if(pNode->descriptor->nextLinkDescAddr != 1)
            nextNode = 1;

        /* return this node to the free list */

        vxbFslDmaFreeDescriptor (pDev, &pChan->dList, pNode);

        if (nextNode)
            pNode = (FSL_DMA_NODE *)lstFirst (&pChan->dList);
        else
            pNode = NULL;
        };

    /* if this DMA channel is not busy, then suspend it */

    if (!(DMA_CHAN_REG_READ(pDev, pChan->chanId, DMA_SR) & DMA_SR_CB))
        {
        regValue = DMA_CHAN_REG_READ(pDev, pChan->chanId, DMA_MR);

        regValue &= ~DMA_MR_CS;

        DMA_CHAN_REG_WRITE(pDev, pChan->chanId, DMA_MR, regValue);

        }

    CHAN_SYNC_GIVE;
    }

/*******************************************************************************
*
* vxbFslDmaExcEntry - entry function of DMA main execute task
*
* vxbFslDmaExcEntry() always waits for the message sent by the interrupt
* service routine, and subsequently invokes vxbFslDmaExcJob() to maintain
* the descriptor.
*
* RETURNS: N/A
*
*/

LOCAL void vxbFslDmaExcEntry
    (
    VXB_DEVICE_ID   pDev
    )
    {
    FSL_DMA_CHAN *      pChan;
    FSL_DMA_DRV_CTRL *  pDrvCtrl;
    atomicVal_t         events;
    UINT32              i;

    pDrvCtrl = pDev->pDrvCtrl;

    while (TRUE)
        {
        /* returns OK as fslDmaSemId is valid and wait forever */

        (void)semTake (pDrvCtrl->fslDmaSemId, WAIT_FOREVER);

        if ((events = vxAtomicSet (&(pDrvCtrl->fslDmaEvents), 0)) != 0)
            {
            for (i = 0; i < pDrvCtrl->chanNum; i++)
                {
                if (events & (1 << i))
                    {
                    pChan = pDrvCtrl->pClients [i];
                    vxbFslDmaExcJob (pDev, pChan, OK);
                    }
                }
            for (i = DMA_EVENTS_ERROR_BIT_OFFSET; 
                 i < DMA_EVENTS_ERROR_BIT_OFFSET + pDrvCtrl->chanNum; i++)
                {
                if (events & (1 << i))
                    {
                    pChan = pDrvCtrl->pClients [i-DMA_EVENTS_ERROR_BIT_OFFSET];
                    vxbFslDmaExcJob (pDev, pChan, ERROR);
                    }
                }
            }
        }
    }

/*******************************************************************************
*
* vxbFslDmaChanIsr - DMA driver channel interrupt service routine
*
* vxbFslDmaChanIsr() is called from interrupt context and determines if
* a DMA channel needs attention. If it does, it sends a message to inform
* the main service task. vxbFslDmaChanIsr() is called if a channel's interrupt
* enable bits are set in the DCR field for the current DMA descriptor.
*
* RETURNS: N/A
*
* SEE ALSO: vxbFslDmaMemCopy, vxbFslDmaSubmitJob
*
*/

LOCAL void vxbFslDmaChanIsr
    (
    FSL_DMA_DRV_CTRL * pDrvCtrl
    )
{
    VXB_DEVICE_ID pDev;
    UINT32 dmaSr;
    UINT32 chan;

    pDev = pDrvCtrl->vxbDev;

    for (chan = 0; chan < pDrvCtrl->chanNum; chan++)
    {
    /* check interrupt pending status */

    dmaSr = DMA_CHAN_REG_READ(pDev, chan, DMA_SR);

    dmaSr &= ~(DMA_SR_CB | DMA_SR_CH);

    if (dmaSr & (DMA_SR_TE| DMA_SR_PE | DMA_SR_EOLNI | DMA_SR_EOSI))
        {
        DMA_CHAN_REG_WRITE(pDev, chan, DMA_SR, dmaSr);

        if (dmaSr & (DMA_SR_TE | DMA_SR_PE))
            vxAtomicOr (&(pDrvCtrl->fslDmaEvents), 
                        (1 << (chan + DMA_EVENTS_ERROR_BIT_OFFSET)));
        else
            vxAtomicOr (&(pDrvCtrl->fslDmaEvents), (1 << chan));
        }
    }

    if (pDrvCtrl->fslDmaEvents != 0)
    {
        semGive (pDrvCtrl->fslDmaSemId);
    }
}

/*******************************************************************************
*
* vxbFslDmaRead - initiate a DMA read transaction on a channel
*
* This routine initiates a DMA read operation on a given DMA channel.
* The DMA will be started, and when it completes, the supplied completion
* routine will be invoked.
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/

LOCAL STATUS vxbFslDmaRead
    (
    VXB_DMA_RESOURCE_ID     dmaRes,
    char *                  src,
    char *                  dest,
    int                     transferSize,
    int                     unitSize,
    UINT32                  flags,
    pVXB_DMA_COMPLETE_FN    completeFunc,
    void *                  pArg
    )
    {
    return (vxbFslDmaMemCopy (dmaRes, dest, src, transferSize,
                              completeFunc, (UINT32)pArg));
    }

/*******************************************************************************
*
* vxbFslDmaSyncCallBack - callback when DMA transfer done
*
* This routine is the callback function when DMA transfer is done.
*
* RETURNS: N/A 
*
* ERRNO: N/A
*/

LOCAL void vxbFslDmaSyncCallBack
    (
    VXB_DEVICE_ID  pDev
    )
    {
    FSL_DMA_DRV_CTRL * pDrvCtrl;

    pDrvCtrl = pDev->pDrvCtrl;

    (void)semGive (pDrvCtrl->fslDmaSyncSem);

    FSL_DMA_LOG(1000, "ISR: Sync Semaphore SemGive!\n", 1, 2, 3, 4, 5, 6);
    }

/*******************************************************************************
*
* vxbFslDmaSyncRead - initiate a synchronous DMA read transaction on a channel
*
* This routine initiates a synchronous read operation on a given DMA channel.
* The function will block until the DMA completes.
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/

LOCAL STATUS vxbFslDmaSyncRead
    (
    VXB_DMA_RESOURCE_ID dmaRes,
    char *              src,
    char *              dest,
    int *               transferSize,
    int                 unitSize,
    UINT32              flags
    )
    {
    VXB_DEVICE_ID  pDev;
    FSL_DMA_DRV_CTRL * pDrvCtrl;

    /* get the DMA engine vxbus instance id */

    pDev = dmaRes->dmaInst;

    pDrvCtrl = pDev->pDrvCtrl;

    
    if ((pDrvCtrl->polling == FALSE) && (pDrvCtrl->fslDmaPhase >= 3))
        {
        
        if (vxbFslDmaMemCopy (dmaRes, dest, src,
                              *transferSize,
                             (pVXB_DMA_COMPLETE_FN)vxbFslDmaSyncCallBack,
                             (UINT32)pDev) != OK)
            return ERROR;
        
        if (semTake (pDrvCtrl->fslDmaSyncSem, WAIT_FOREVER) != OK)
            {
            FSL_DMA_LOG(0, "semTake fslDmaSyncSem error!\n", 1, 2, 3, 4, 5, 6);
            return ERROR;
            }
        }
    else
        if (vxbFslDmaDirectMode (dmaRes, dest, src, *transferSize) != OK)
            return ERROR;
    
    return OK;
    }

/*******************************************************************************
*
* vxbFslDmaDmaWrite - initiate a DMA write transaction on a channel
*
* This routine initiates a DMA write operation on a given DMA channel.
* The DMA will be started, and when it completes, the supplied completion
* routine will be invoked.
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/

LOCAL STATUS vxbFslDmaWrite
    (
    VXB_DMA_RESOURCE_ID     dmaRes,
    char *                  src,
    char *                  dest,
    int                     transferSize,
    int                     unitSize,
    UINT32                  flags,
    pVXB_DMA_COMPLETE_FN    completeFunc,
    void *                  pArg
    )
    {
    return (vxbFslDmaMemCopy (dmaRes, dest, src, transferSize,
                              completeFunc, (UINT32)pArg));
    }

/*******************************************************************************
*
* vxbFslDmaSyncWrite - initiate a synchronous DMA write transaction on a channel
*
* This routine initiates a synchronous write operation on a given DMA channel.
* The function will block until the DMA completes.
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/

LOCAL STATUS vxbFslDmaSyncWrite
    (
    VXB_DMA_RESOURCE_ID dmaRes,
    char *              src,
    char *              dest,
    int *               transferSize,
    int                 unitSize,
    UINT32              flags
    )
    {
    VXB_DEVICE_ID  pDev;
    FSL_DMA_DRV_CTRL * pDrvCtrl;

    /* get the DMA engine vxbus instance id */

    pDev = dmaRes->dmaInst;

    pDrvCtrl = pDev->pDrvCtrl;
    
    if ((pDrvCtrl->polling == FALSE) && (pDrvCtrl->fslDmaPhase >= 3))
    {

        if (vxbFslDmaMemCopy (dmaRes, dest, src,*transferSize,
                             (pVXB_DMA_COMPLETE_FN)vxbFslDmaSyncCallBack,
                              (UINT32)pDev) != OK)
            return ERROR;

        printf("yyyyy");
        if (semTake (pDrvCtrl->fslDmaSyncSem, WAIT_FOREVER) != OK)
            {
            FSL_DMA_LOG(0, "semTake fslDmaSyncSem error!\n", 1, 2, 3, 4, 5, 6);
            return ERROR;
            }
    }
    else
        if (vxbFslDmaDirectMode (dmaRes, dest, src, *transferSize) != OK)
            return ERROR;
    
    return OK;
    }

/*******************************************************************************
*
* vxbFslDmaStatus - return DMA channel status
*
* This routine returns the status of a current DMA channel. A channel
* may be paused, idle, running or have no DMA operation in progress.
*
* RETURNS: the status of a current DMA channel
*
* ERRNO
*/

LOCAL int vxbFslDmaStatus
    (
    VXB_DMA_RESOURCE_ID dmaRes
    )
    {
    VXB_DEVICE_ID  pDev;
    FSL_DMA_CHAN * pChan;
    UINT32         status = 0;

    /* get the DMA engine vxbus instance id */

    pDev = dmaRes->dmaInst;

    /* get the associated DMA channel data */

    pChan = dmaRes->pDmaChan;

    status = DMA_CHAN_REG_READ(pDev, pChan->chanId, DMA_SR);

    return status;
    }

/*******************************************************************************
*
* vxbFslDmaInstUnlink - VxBus unlink method
*
* This is a cleanup routine invoked by VxBus when this device instance is
* destroyed. We release our allocated resources here.
*
* RETURNS: OK, or ERROR
*
* ERRNO
*/

LOCAL STATUS vxbFslDmaInstUnlink
    (
    VXB_DEVICE_ID   pDev,
    void *          unused
    )
    {
    FSL_DMA_DRV_CTRL * pDrvCtrl = pDev->pDrvCtrl;
    UINT32 chan;

    if(pDrvCtrl->fslDmaExcId != 0)
        taskDelete(pDrvCtrl->fslDmaExcId);

    pDrvCtrl->fslDmaExcId = 0;

    if (semDelete (pDrvCtrl->lstMutex) != OK)
        return ERROR;

    pDrvCtrl->lstMutex = NULL;

    if (semDelete (pDrvCtrl->fslDmaSemId) != OK)
        return ERROR;

    pDrvCtrl->fslDmaSemId = NULL;

    if (semDelete (pDrvCtrl->fslDmaSyncSem) != OK)
        return ERROR;

    pDrvCtrl->fslDmaSyncSem = NULL;

    for (chan = 0; chan < pDrvCtrl->chanNum; chan++)
        {
        vxbIntDisconnect (pDev, chan, vxbFslDmaChanIsr, pDev);
        vxbIntDisable (pDev, chan, vxbFslDmaChanIsr, pDev);

        DMA_CHAN_REG_WRITE(pDev, chan, DMA_MR, 0x00);
        DMA_CHAN_REG_WRITE(pDev, chan, DMA_BCR, 0x00);

        }

    cacheDmaFree (pDrvCtrl->pDesc);

    pDrvCtrl->fslDmaPhase = -1;

    return OK;
    }

/*******************************************************************************
*
* vxbFslDmaResourceGet - method to get the dma channel resources
*
* This function is the driver method used to get the dma channel resources.
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/

LOCAL STATUS vxbFslDmaResourceGet
    (
    VXB_DEVICE_ID           pDev,
    VXB_DEVICE_ID           pReqDev,
    struct vxbDmaRequest *  pReq
    )
    {
    VXB_DMA_RESOURCE_ID dmaRes;
    FSL_DMA_DRV_CTRL *  pDrvCtrl = pDev->pDrvCtrl;
    FSL_DMA_CHAN *      pChan;

    pChan = vxbFslDmaReserveChannel (pDev);

    if (pChan == NULL)
        {
        FSL_DMA_LOG(0, "can not get a free DMA channel!\r\n",
                                            0, 1, 2, 3, 4, 5);
        return ERROR;
        }

    if (pReq->minQueueDepth > pDrvCtrl->numOfLinks)
        {
        vxbFslDmaReleaseChannel (pDev, pChan);
        FSL_DMA_LOG(0, "the request QueueDepth is error!\r\n",
                                            0, 1, 2, 3, 4, 5);
        return ERROR;
        }

    /* get DMA resource data */

    dmaRes = pReq->pChan;

    /* update the function pointers */

    dmaRes->dmaFuncs.dmaRead         = vxbFslDmaRead;
    dmaRes->dmaFuncs.dmaReadAndWait  = vxbFslDmaSyncRead;
    dmaRes->dmaFuncs.dmaWrite        = vxbFslDmaWrite;
    dmaRes->dmaFuncs.dmaWriteAndWait = vxbFslDmaSyncWrite;
    dmaRes->dmaFuncs.dmaCancel       = vxbFslDmaCancel;
    dmaRes->dmaFuncs.dmaPause        = vxbFslDmaSuspend;
    dmaRes->dmaFuncs.dmaResume       = vxbFslDmaResume;
    dmaRes->dmaFuncs.dmaStatus       = vxbFslDmaStatus;

    /* save DMA controller vxbus instance id */

    dmaRes->dmaInst = pDev;

    /* save DMA per channel data to DMA resource */

    dmaRes->pDmaChan = pChan;

    if(pDrvCtrl->fslDmaExcId == 0)
        {
        pDrvCtrl->fslDmaExcId = taskSpawn ("tFslDmaExc", 100, 0, 4096,
                                           (FUNCPTR) vxbFslDmaExcEntry,
                                           (_Vx_usr_arg_t)pDev,
                                           0, 0, 0, 0, 0, 0, 0, 0, 0);
        if(pDrvCtrl->fslDmaExcId == (TASK_ID)ERROR)
            {
            FSL_DMA_LOG(0, "Unable to create DMA execute task\n",
                              0, 1, 2, 3, 4, 5);
            return ERROR;
            }
        }

    return OK;
    }

/*******************************************************************************
*
* vxbFslDmaResourceRelease - method to release DMA resources
*
* This function is the driver method for releasing DMA resources when
* a channel is deallocated.
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/

LOCAL STATUS vxbFslDmaResourceRelease
    (
    VXB_DEVICE_ID           pDev,
    VXB_DMA_RESOURCE_ID     dmaRes
    )
    {
    FSL_DMA_CHAN *  pChan;
    FSL_DMA_DRV_CTRL *  pDrvCtrl = pDev->pDrvCtrl;
    UINT32  chan;
    UINT32  deleteTask = 1;

    pChan = dmaRes->pDmaChan;

    /* free a DMA channel to DMA engine */

    if (vxbFslDmaReleaseChannel (pDev, pChan) != OK)
        return ERROR;

    for(chan = 0; chan < pDrvCtrl->chanNum; chan++)
        {
        if(pDrvCtrl->pClients[chan] != NULL)
            {
            deleteTask = 0;
            break;
            }
        }

    if (deleteTask)
        {
        taskDelete (pDrvCtrl->fslDmaExcId);
        pDrvCtrl->fslDmaExcId = 0;
        }

    return OK;
    }

#ifdef FSL_DMA_DEBUG

/*******************************************************************************
*
* vxbFslDmaRegDump - dump the DMA registers
*
* This function dump the DMA registers.
*
* RETURNS: NULL
*
* ERRNO
*/

void vxbFslDmaRegDump
    (
    VXB_DEVICE_ID pDev
    )
    {
    int chan;

    for (chan = 0; chan < DMA_CHAN_NUM; chan++)
        {
        printf("\r\n"); 
        printf("DMA channel %d registers (0x%08X)\r\n", chan + 1,
                (UINT32)DMA_BAR(pDev) + (chan * DMA_CHAN_OFFSET));
        printf("-------------------------------------------\r\n");
        printf("mr 0x%08x\r\n", DMA_CHAN_REG_READ(pDev, chan, DMA_MR));
        printf("sr 0x%08x\r\n", DMA_CHAN_REG_READ(pDev, chan, DMA_SR));
        printf("eclndar 0x%08x\r\n", DMA_CHAN_REG_READ(pDev, chan, DMA_ECLNDAR));
        printf("clndar 0x%08x\r\n", DMA_CHAN_REG_READ(pDev, chan, DMA_CLNDAR));
        printf("satr 0x%08x\r\n", DMA_CHAN_REG_READ(pDev, chan, DMA_SATR));
        printf("sar 0x%08x\r\n", DMA_CHAN_REG_READ(pDev, chan, DMA_SAR));
        printf("datr 0x%08x\r\n", DMA_CHAN_REG_READ(pDev, chan, DMA_DATR));
        printf("dar 0x%08x\r\n", DMA_CHAN_REG_READ(pDev, chan, DMA_DAR));
        printf("bcr 0x%08x\r\n", DMA_CHAN_REG_READ(pDev, chan, DMA_BCR));
        printf("enlndar 0x%08x\r\n", DMA_CHAN_REG_READ(pDev, chan, DMA_ENLNDAR));
        printf("nlndar 0x%08x\r\n", DMA_CHAN_REG_READ(pDev, chan, DMA_NLNDAR));
        printf("eclsdar 0x%08x\r\n",DMA_CHAN_REG_READ(pDev, chan, DMA_ECLSDAR));
        printf("clsdar 0x%08x\r\n", DMA_CHAN_REG_READ(pDev, chan, DMA_CLSDAR));
        printf("enlsdar 0x%08x\r\n", DMA_CHAN_REG_READ(pDev, chan, DMA_ENLSDAR));
        printf("nlsdar 0x%08x\r\n", DMA_CHAN_REG_READ(pDev, chan, DMA_NLSDAR));
        printf("ssr 0x%08x\r\n", DMA_CHAN_REG_READ(pDev, chan, DMA_SSR));
        printf("dsr 0x%08x\r\n", DMA_CHAN_REG_READ(pDev, chan, DMA_DSR));
        }
    printf("\r\n"); 
    }
#endif /* FSL_DMA_DEBUG */
