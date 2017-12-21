/* vxbM548xDma.c - Coldfire MCD slave DMA VxBus driver */

/*
 * Copyright (c) 2007-2008 Wind River Systems, Inc. All rights are reserved.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,05may08,tor  update version
01c,20sep07,tor  VXB_VERSION_3
01b,13jun07,tor  remove VIRT_ADDR
01a,26jan07,wap  written
*/

/*
DESCRIPTION
This module implements a VxBus slave DMA driver front-end for the
Coldfire Multi-Channel DMA controller. The DMA controller is needed
in order to support the on-board FEC ethernet ports.

The MCD controller is driven by microcode, which is loaded from and
manipulated using an API library provided by Freescale. The library
provides the ability to start, stop, pause and resume DMA operations.
For the FEC, once a DMA channel is started, it continues to run until
the DMA task is killed. The MCD controller has a single interrupt
dedicated to it, which fires whenever a transfer completes. The
dma_utils module determines which channel triggered the interrupt
and invokes the callback which has been associated with that
channel. Note that callbacks occur in interrupt context, and as a
resume, so do the DMA completion routines registered via this module.

BOARD LAYOUT
The Multi-Channel DMA controller is internal to the Coldfire CPU.

EXTERNAL INTERFACE

SEE ALSO: vxBus
*/

#include <vxWorks.h>
#include <string.h>
#include <stdlib.h>
#include <intLib.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/util/vxbDmaLib.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/util/vxbParamSys.h>

#define M8641DMA_DEBUG
#undef M8641DMA_DEBUG
#ifdef M8641DMA_DEBUG
char sprintBuf[100];
#define DMA_DEBUG_MSG(msg,a,b,c,d,e,f)	sprintf(sprintBuf,msg,a,b,c,d,e,f);ns16550Pollprintf(sprintBuf)
#else
#define DMA_DEBUG_MSG(msg,a,b,c,d,e,f)
#endif

LOCAL void m8641DmaInstInit (VXB_DEVICE_ID);
LOCAL void m8641DmaInstInit2 (VXB_DEVICE_ID);
LOCAL void m8641DmaInstConnect (VXB_DEVICE_ID);
#if 0
LOCAL STATUS m8641DmaInstUnlink (VXB_DEVICE_ID, void *);

//LOCAL STATUS m8641ResourceGet (VXB_DEVICE_ID, VXB_DEVICE_ID,struct vxbDmaRequest *);
//LOCAL STATUS m8641ResourceRelease (VXB_DEVICE_ID, struct vxbDmaRequest *);


LOCAL STATUS m8641DmaRead (VXB_DMA_RESOURCE_ID, char *, char *,
                         int, int, UINT32, pVXB_DMA_COMPLETE_FN, void *);
LOCAL STATUS m8641DmaWrite (VXB_DMA_RESOURCE_ID, char *, char *,
                          int, int, UINT32, pVXB_DMA_COMPLETE_FN, void *);
LOCAL STATUS m8641DmaSyncRead (VXB_DMA_RESOURCE_ID, char *, char *,
                             int *, int, UINT32);
LOCAL STATUS m8641DmaSyncWrite (VXB_DMA_RESOURCE_ID, char *, char *,
                              int *, int, UINT32);
LOCAL STATUS m8641DmaCancel (VXB_DMA_RESOURCE_ID);
LOCAL STATUS m8641DmaPause (VXB_DMA_RESOURCE_ID);
LOCAL STATUS m8641DmaResume (VXB_DMA_RESOURCE_ID);
LOCAL int m8641DmaStatus (VXB_DMA_RESOURCE_ID);
#endif

LOCAL struct drvBusFuncs m8641DmaFuncs =
    {
    m8641DmaInstInit,        /* devInstanceInit */
    m8641DmaInstInit2,       /* devInstanceInit2 */
    m8641DmaInstConnect      /* devConnect */
    };
#if 0
LOCAL device_method_t   m8641DmaMethods[] = 
    {
    //DEVMETHOD(vxbDmaResourceGet, m8641ResourceGet),
    DEVMETHOD(vxbDmaResourceRelease, m8641ResourceRelease),
    DEVMETHOD(vxbDrvUnlink, m8641DmaInstUnlink),
    {0,0}
    };
#endif

/* structure used for registration with vxbus */

LOCAL struct vxbDevRegInfo m8641Registration =
    {
    NULL,                       /* pNext */
    VXB_DEVID_DEVICE,           /* devID */
    VXB_BUSID_PLB,              /* busID = PLB */
    VXB_VER_4_0_0,            /* vxbVersion */
    "dma",                      /* drvName */
    &m8641DmaFuncs,                  /* pDrvBusFuncs */
   // m8641DmaMethods,                 /* pMethods */
    NULL,
    NULL                        /* devProbe */
    };

/******************************************************************************
*
* m548xDmaDrvRegister - register coldFire DMA driver
*
* This routine registers the coldFire DMA driver with the vxBus subsystem.
*
* RETURNS: N/A
*
* ERRNO
*/

void m8641DmaDrvRegister (void)
    {
    DMA_DEBUG_MSG("run into m8641DmaDrvRegister\n" ,1,2,3,4,5,6);
    vxbDevRegister (&m8641Registration);
    }

/******************************************************************************
*
* mcdInstInit - first level initialization routine of DMA device
*
* This is the function called to perform the first level initialization of
* the coldFire DMA device.
*
* NOTE:
*
* This routine is called early during system initialization, and
* *MUST NOT* make calls to OS facilities such as memory allocation
* and I/O.
*
* RETURNS: N/A
*
* ERRNO
*/

#define INTERNAL_SRAM_BASE     0x10000

LOCAL void m8641DmaInstInit
    (
    VXB_DEVICE_ID pDev
    )
    {
    DMA_DEBUG_MSG("run into m8641DmaInstInit\n" ,1,2,3,4,5,6);
    return;
    }

/******************************************************************************
*
* mcdInstInit2 - second level initialization routine of DMA device
*
* This routine performs the second level initialization of the DMA device.
*
* This routine is called later during system initialization.  OS features
* such as memory allocation are available at this time.
*
* RETURNS: N/A
*
* ERRNO
*/


#if 0 /*hdz modify*/
FUNCPTR pISRDMA=NULL; 
int dmaParam;
void dma_interrupt_handler(VXB_DEVICE_ID pDev)
{
    unsigned int srVal;
	/*DMA_DEBUG_MSG("congraduation: into dma_interrupt_handler\n" ,1,2,3,4,5,6);*/
    srVal = *(unsigned int*)(CCSBAR+0x21104);
	/*DMA_DEBUG_MSG("srVal is 0x%x\n" ,srVal,2,3,4,5,6);*/
	*(unsigned int*)(CCSBAR+0x21104 = srVal;
    if (pISRDMA!=NULL)
		pISRDMA(dmaParam);
}
#else
FUNCPTR pISRDMA[4]={NULL,NULL,NULL,NULL}; 
int dmaParam[4];
void dma_interrupt_handler(VXB_DEVICE_ID pDev)
{
    unsigned int srVal;
#if 0   
    static count=0;
    *(UINT32*)0xe0000214 =count+((pDev->unitNumber)<<16);
    count++;
#endif 
	/*DMA_DEBUG_MSG("congraduation: into dma_interrupt_handler\n" ,1,2,3,4,5,6);*/
    srVal = *(unsigned int*)(CCSBAR+0x21104+(pDev->unitNumber)*0x80);
	/*DMA_DEBUG_MSG("srVal is 0x%x\n" ,srVal,2,3,4,5,6);*/
    *(unsigned int*)(CCSBAR+0x21104+(pDev->unitNumber)*0x80) = srVal;
    if (pISRDMA[pDev->unitNumber]!=NULL)
    {
    	/**(UINT32*)0xe0000218=0x1111;*/
    	DMA_DEBUG_MSG("srVal1 is 0x%x\n" ,srVal,2,3,4,5,6);
		(pISRDMA[pDev->unitNumber])(dmaParam[pDev->unitNumber]);
    	/*(pISRDMA)(dmaParam[pDev->unitNumber]);*/
    }
}


#endif


LOCAL void m8641DmaInstInit2
    (
    VXB_DEVICE_ID pDev
    )
    {
    DMA_DEBUG_MSG("run into m8641DmaInstInit2\n" ,1,2,3,4,5,6);
    vxbIntConnect (pDev, 0, dma_interrupt_handler, pDev);
    vxbIntEnable (pDev, 0, dma_interrupt_handler, pDev);
/*    dma_irq_enable (pDev, 0, 0); */

    return;
    }

/******************************************************************************
*
* mcdInstConnect - third level initialization routine of DMA device
*
* This is the function called to perform the third level initialization of
* the DMA device.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void m8641DmaInstConnect
    (
    VXB_DEVICE_ID       pDev
    )
    {
    /* nothing is done here */
	DMA_DEBUG_MSG("run into m8641DmaInstConnect\n" ,1,2,3,4,5,6);
    return;
    }
#if 0
/******************************************************************************
*
* mcdInstUnlink - VxBus unlink method
*
* This is a cleanup routine invoked by VxBus when this device instance is
* destroyed. We release our allocated resources here.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL STATUS m8641DmaInstUnlink
    (
    VXB_DEVICE_ID pDev,
    void * unused
    )
    { 
    vxbIntDisconnect (pDev, 0, dma_interrupt_handler, pDev);
    vxbIntDisable (pDev, 0, dma_interrupt_handler, pDev);
/*    dma_irq_disable (pDev);*/
    return (OK);
    }

/******************************************************************************
*
* mcdResourceGet - method to get the dma resources
*
* This function is the driver method used to get the dma resources.
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/

LOCAL STATUS m8641ResourceGet
    (
    VXB_DEVICE_ID pDev,
    VXB_DEVICE_ID pReqDev,
    struct vxbDmaRequest * pReq
    )
    {
#if 0
    VXB_DMA_RESOURCE_ID pChan;
    MCD_CHAN_INFO * pInfo;
    MCD_DEDICATED_INFO * pReqInfo;

    pReqInfo = pReq->pDedicatedChanInfo;

    if (pReqInfo->mcd_magic != MCD_MAGIC)
        return (ERROR);

    pChan = pReq->pChan;

    pChan->dmaInst = pDev;

    /* update the function pointers */

    pChan->dmaFuncs.dmaRead = mcdDmaRead;
    pChan->dmaFuncs.dmaReadAndWait = mcdDmaSyncRead;
    pChan->dmaFuncs.dmaWrite = mcdDmaWrite;
    pChan->dmaFuncs.dmaWriteAndWait = mcdDmaSyncWrite;
    pChan->dmaFuncs.dmaCancel = mcdDmaCancel;
    pChan->dmaFuncs.dmaPause = mcdDmaPause;
    pChan->dmaFuncs.dmaResume = mcdDmaResume;
    pChan->dmaFuncs.dmaStatus = mcdDmaStatus;

    if (dma_set_initiator (pDev, pReqInfo->mcd_chanId) != OK)
        return (ERROR);
    
    pInfo = malloc (sizeof (MCD_CHAN_INFO));

    pInfo->mcd_initiator = dma_get_initiator (pReqInfo->mcd_chanId);
    pInfo->mcd_priority = pReqInfo->mcd_priority;
    pInfo->mcd_chanId = pReqInfo->mcd_chanId;

    pChan->pDmaChan = pInfo;
#endif    
    return (OK);
    }

/******************************************************************************
*
* mcdResourceRelease - method to release DMA resources
*
* This function is the driver method for releasing DMA resources when
* a channel is deallocated.
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/


LOCAL STATUS m8641ResourceRelease
    (
    VXB_DEVICE_ID pDev,
    struct vxbDmaRequest * pReq
    )
    {
#if 0
    MCD_CHAN_INFO * pInfo;
    int s;

    pInfo = pReq->pChan->pDmaChan;

    s = intLock ();
    dma_free_initiator (pInfo->mcd_initiator);
    dma_free_channel (pInfo->mcd_channel);
    intUnlock (s);

    free (pInfo);
#endif
    return (OK);
    }

/******************************************************************************
*
* mcdDmaRead - initiate a DMA read transaction on a channel
*
* This routine initiates a DMA read operation on a given DMA channel.
* The DMA will be started, and when it completes, the supplied completion
* routine will be invoked.
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/

LOCAL STATUS m8641DmaRead
    (
    VXB_DMA_RESOURCE_ID pChan,
    char * src,
    char * dest,
    int transferSize,
    int unitSize,
    UINT32 flags,
    pVXB_DMA_COMPLETE_FN completeFunc,
    void * pArg
    )
    {

#if 0
    MCD_CHAN_INFO * pInfo;
    int rval;
    int s;

    pInfo = pChan->pDmaChan;

    s = intLock ();

    pInfo->mcd_channel = dma_set_channel (pInfo->mcd_chanId,
        (VOIDFUNCPTR)completeFunc, pArg);

    if (pInfo->mcd_channel == -1)
        {
        intUnlock (s);
        return (ERROR);
        }

    MCD_killDma (pInfo->mcd_channel);

    rval = MCD_startDma (pInfo->mcd_channel, (char *)dest, 0, (char *)src, 0,
        transferSize, unitSize, pInfo->mcd_initiator, pInfo->mcd_priority,
        flags, MCD_NO_CSUM|MCD_NO_BYTE_SWAP);

    intUnlock (s);

    if (rval == MCD_OK)
        return (OK);
#endif
    return (ERROR);
    }

/******************************************************************************
*
* mcdDmaWrite - initiate a DMA write transaction on a channel
*
* This routine initiates a DMA write operation on a given DMA channel.
* The DMA will be started, and when it completes, the supplied completion
* routine will be invoked.
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/

LOCAL STATUS m8641DmaWrite
    (
    VXB_DMA_RESOURCE_ID pChan,
    char * src,
    char * dest,
    int transferSize,
    int unitSize,
    UINT32 flags,
    pVXB_DMA_COMPLETE_FN completeFunc,
    void * pArg
    )
    {
#if 0
    MCD_CHAN_INFO * pInfo;
    int rval;
    int s;

    pInfo = pChan->pDmaChan;

    s = intLock ();

    pInfo->mcd_channel = dma_set_channel (pInfo->mcd_chanId,
        (VOIDFUNCPTR)completeFunc, pArg);

    if (pInfo->mcd_channel == -1)
        {
        intUnlock (s);
        return (ERROR);
        }

    MCD_killDma (pInfo->mcd_channel);

    rval = MCD_startDma (pInfo->mcd_channel, (char *)dest, 0, (char *)src, 0,
        transferSize, unitSize, pInfo->mcd_initiator, pInfo->mcd_priority,
        flags, MCD_NO_CSUM|MCD_NO_BYTE_SWAP);

    intUnlock (s);

    if (rval == MCD_OK)
        return (OK);
#endif
    return (ERROR);
    }

/******************************************************************************
*
* mcdDmaSyncRead - initiate a synchronous DMA read transaction on a channel
*
* This routine initiates a synchronous read operation on a given DMA channel.
* The function will block until the DMA completes.
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/

LOCAL STATUS m8641DmaSyncRead
    (
    VXB_DMA_RESOURCE_ID pChan,
    char * src,
    char * dest,
    int * transferSize,
    int unitSize,
    UINT32 flags
    )
    {
    return (ERROR);
    }

/******************************************************************************
*
* mcdDmaSyncWrite - initiate a synchronous DMA write transaction on a channel
*
* This routine initiates a synchronous write operation on a given DMA channel.
* The function will block until the DMA completes.
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/

LOCAL STATUS m8641DmaSyncWrite
    (
    VXB_DMA_RESOURCE_ID pChan,
    char * src,
    char * dest,
    int * transferSize,
    int unitSize,
    UINT32 flags
    )
    {
    return (ERROR);
    }

/******************************************************************************
*
* mcdDmaCancel - cancel a DMA operation on a given channel
*
* This function will cancel a read or write operation that was previously
* started on a given channel. This will prevent any further I/O from
* occuring on the channel until a new read or write operation is started.
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/

LOCAL STATUS m8641DmaCancel
    (
    VXB_DMA_RESOURCE_ID pChan
    )
    {
#if 0
    MCD_CHAN_INFO * pInfo;
    int s, r;
    pInfo = pChan->pDmaChan;

    s = intLock ();
    r = MCD_killDma (pInfo->mcd_channel);
    intUnlock (s);

    return (r);
#endif
    return (OK);
    }

/******************************************************************************
*
* mcdDmaPause - pause DMA operation on a given channel
*
* This function will pause a DMA channel that has been previously started.
* Pausing a channel allows the caller to safely manipulate any underlying
* DMA descriptor or buffer structures associated with a channel without
* cancelling the DMA operation completely. A paused channel can be resumed
* with the resume method below.
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/

LOCAL STATUS m8641DmaPause
    (
    VXB_DMA_RESOURCE_ID pChan
    )
    {
#if 0
    MCD_CHAN_INFO * pInfo;
    int s, r;
    pInfo = pChan->pDmaChan;

    s = intLock ();
    r = MCD_pauseDma (pInfo->mcd_channel);
    intUnlock (s);

    if (r != MCD_OK)
        return (ERROR);
#endif
    return (OK);
    }

/******************************************************************************
*
* mcdDmaResume - resume a DMA channel that has paused
*
* This function will resume DMA on a channel that has been paused, or which
* has gone idle. A channel may enter the idle state if all DMA descriptors
* have been consumed and not yet replenished. It is safe to call the resume
* method at any time, even if the channel is not currently paused.
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/

LOCAL STATUS m8641DmaResume
    (
    VXB_DMA_RESOURCE_ID pChan
    )
    {
#if 0
    MCD_CHAN_INFO * pInfo;
    int s, r, i;

    pInfo = pChan->pDmaChan;

    s = intLock ();
    r = MCD_dmaStatus(pInfo->mcd_channel);
    if (r == MCD_PAUSED)
        r = MCD_resumeDma (pInfo->mcd_channel);
    else
        r = MCD_continDma (pInfo->mcd_channel);
    intUnlock (s);

    if (r != MCD_OK)
        return (ERROR);
#endif
    return (OK);
    }

/******************************************************************************
*
* mcdDmaStatus - return DMA channel status
*
* This routine returns the status of a current DMA channel. A channel
* may be paused, idle, running or have no DMA operation in progress.
*
* RETURNS: MCD-specific status code
*
* ERRNO
*/

LOCAL int m8641DmaStatus
    (
    VXB_DMA_RESOURCE_ID pChan
    )
    {
#if 0
    MCD_CHAN_INFO * pInfo;
    int s, r;

    pInfo = pChan->pDmaChan;

    s = intLock ();
    r = MCD_dmaStatus (pInfo->mcd_channel);
    intUnlock (s);
    return (r);
#endif
    return (OK);
    }
#endif
