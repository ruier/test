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
m8641_utils module determines which channel triggered the interrupt
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
/*#include <arch/coldfire/mcf5475.h>*/
#include <hwif/vxbus/vxBus.h>
#include <hwif/util/vxbDmaLib.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/util/vxbParamSys.h>
#include <semLib.h>
#include <../src/hwif/h/util/vxbDmaDriverLib.h>
#include "vxbM8641Dma.h"


METHOD_DECL(vxbDmaResourceGet);
METHOD_DECL(vxbDmaResourceRelease);

LOCAL void m8641InstInit (VXB_DEVICE_ID);
LOCAL void m8641InstInit2 (VXB_DEVICE_ID);
LOCAL void m8641InstConnect (VXB_DEVICE_ID);
LOCAL STATUS m8641InstUnlink (VXB_DEVICE_ID, void *);

LOCAL STATUS m8641ResourceGet (VXB_DEVICE_ID, VXB_DEVICE_ID,
                             struct vxbDmaRequest *);
LOCAL STATUS m8641ResourceRelease (VXB_DEVICE_ID, struct vxbDmaRequest *);
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

LOCAL void m8641DmaHandle(VXB_DEVICE_ID pDev);

LOCAL STATUS m8641DmaStart(VXB_DEVICE_ID pDev,UINT32 uiChan,UINT32 uiOption,
                           UINT32 uiSrc,UINT32 uiSrcAttr,UINT32 uiDest,
                           UINT32 uiDestAttr,int uiSize);
                           
DMA_M8641_INFO_S   *gapstDmaInfo[DMA8641_MAX_CHAN] = {NULL};


LOCAL struct drvBusFuncs m8641Funcs =
    {
        m8641InstInit,        /* devInstanceInit */
        m8641InstInit2,       /* devInstanceInit2 */
        m8641InstConnect      /* devConnect */
    };

LOCAL device_method_t   m8641Methods[] = 
    {
    DEVMETHOD(vxbDmaResourceGet, m8641ResourceGet),
    DEVMETHOD(vxbDmaResourceRelease, m8641ResourceRelease),
    DEVMETHOD(vxbDrvUnlink, m8641InstUnlink),
    {0,0}
    };

/* structure used for registration with vxbus */

LOCAL struct vxbDevRegInfo m8641Registration =
{
    NULL,                       /* pNext */
    VXB_DEVID_DEVICE,           /* devID */
    VXB_BUSID_PLB,              /* busID = PLB */
    VXB_VER_4_0_0,            /* vxbVersion */
    "dma",                      /* drvName */
    &m8641Funcs,                  /* pDrvBusFuncs */
    m8641Methods,                 /* pMethods */
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
    vxbDevRegister (&m8641Registration);
}

/******************************************************************************
*
* m8641InstInit - first level initialization routine of DMA device
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

LOCAL void m8641InstInit(VXB_DEVICE_ID pDev)
{
    /*DMA_CSR_WRITE_4(pDev,MPC8641_DMA_MR,0x8);*/
    return;
}

/******************************************************************************
*
* m8641InstInit2 - second level initialization routine of DMA device
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

LOCAL void m8641InstInit2
    (
    VXB_DEVICE_ID pDev
    )
{
	DMA_M8641_INFO_S *pstDmaInfo = NULL;

	pstDmaInfo = malloc(sizeof(DMA_M8641_INFO_S));
	if(NULL == pstDmaInfo)
	{
		logMsg("m8641Resource malloc failed\n",0,0,0,0,0,0);
		return ;
	}
	
	if(pDev->unitNumber > 3)
	{
		logMsg("dma unit %d overflow\n",pDev->unitNumber,0,0,0,0,0);
		return ;
	}

	gapstDmaInfo[pDev->unitNumber] = pstDmaInfo;
	memset(pstDmaInfo,0,sizeof(DMA_M8641_INFO_S));
	pstDmaInfo->semDmaSync = semBCreate(SEM_Q_FIFO,SEM_EMPTY);
	pstDmaInfo->semDmaSrc = semBCreate(SEM_Q_FIFO,SEM_FULL);
	return;
}

/******************************************************************************
*
* m8641InstConnect - third level initialization routine of DMA device
*
* This is the function called to perform the third level initialization of
* the DMA device.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void m8641InstConnect(VXB_DEVICE_ID  pDev)
{
    vxbIntConnect(pDev, 0, m8641DmaHandle,pDev);
    vxbIntEnable(pDev, 0, m8641DmaHandle,pDev);
    return;
}

/******************************************************************************
*
* m8641InstUnlink - VxBus unlink method
*
* This is a cleanup routine invoked by VxBus when this device instance is
* destroyed. We release our allocated resources here.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL STATUS m8641InstUnlink(VXB_DEVICE_ID pDev,void * unused)
{
    logMsg("m8641InstUnlink \n",0,0,0,0,0,0);
    
    vxbIntDisconnect (pDev, 0, m8641DmaHandle, pDev);
    vxbIntDisable (pDev, 0, m8641DmaHandle, pDev);
    
    return (OK);
}

/******************************************************************************
*
* m8641ResourceGet - method to get the m8641 resources
*
* This function is the driver method used to get the m8641 resources.
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/

LOCAL STATUS m8641ResourceGet(VXB_DEVICE_ID pDev,VXB_DEVICE_ID pReqDev,
                              struct vxbDmaRequest * pReq)
{
	VXB_DMA_RESOURCE_ID     pChan = NULL;
	DMA_M8641_DEDICATED_S  *pReqInfo = NULL;
	DMA_M8641_INFO_S       *pstDmaInfo = NULL;

	pReqInfo = pReq->pDedicatedChanInfo;
	if((DMA_M8641_MAGIC != pReqInfo->uiMagic) || (pDev->unitNumber > 3))
	{
		logMsg("m8641ResourceGet maigc 0x%x error\n",pReqInfo->uiMagic,0,0,0,0,0);
		return ERROR;
	}
	
	if(pReqInfo->uiUnit != pDev->unitNumber)
	{
	    return ERROR;
	}

	pstDmaInfo = gapstDmaInfo[pDev->unitNumber];
	
	if(OK != semTake(pstDmaInfo->semDmaSrc, DMA_WAIT_TIMEOUT))
	{
		logMsg("dma channel %d is in used\n",pDev->unitNumber,0,0,0,0,0);
		return ERROR;
	}

	pChan = pReq->pChan;
	pChan->dmaInst = pDev;
	pDev->pDrvCtrl = pChan;
	pChan->dmaFuncs.dmaRead = m8641DmaRead;
	pChan->dmaFuncs.dmaReadAndWait = m8641DmaSyncRead;
	pChan->dmaFuncs.dmaWrite = m8641DmaWrite;
	pChan->dmaFuncs.dmaWriteAndWait = m8641DmaSyncWrite;
	pChan->dmaFuncs.dmaCancel = m8641DmaCancel;
	pChan->dmaFuncs.dmaPause = m8641DmaPause;
	pChan->dmaFuncs.dmaResume = m8641DmaResume;
	pChan->dmaFuncs.dmaStatus = m8641DmaStatus;
	pstDmaInfo->uiDestAttr = pReqInfo->uiDestAttr;
	pstDmaInfo->uiPriority = pReqInfo->uiPriority;
	pstDmaInfo->uiSrcAttr = pReqInfo->uiSrcAttr;
	pstDmaInfo->uiOption = pReqInfo->uiOption;
	pChan->pDmaChan = pstDmaInfo;

	return OK;
}

/******************************************************************************
*
* m8641ResourceRelease - method to release DMA resources
*
* This function is the driver method for releasing DMA resources when
* a channel is deallocated.
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/

LOCAL STATUS m8641ResourceRelease(VXB_DEVICE_ID pDev,struct vxbDmaRequest *pReq)
{
    DMA_M8641_INFO_S       *pstDmaInfo = NULL;

    pstDmaInfo = gapstDmaInfo[pDev->unitNumber];;
    semGive(pstDmaInfo->semDmaSrc);
    
    return (OK);
}

/******************************************************************************
*
* m8641DmaRead - initiate a DMA read transaction on a channel
*
* This routine initiates a DMA read operation on a given DMA channel.
* The DMA will be started, and when it completes, the supplied completion
* routine will be invoked.
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/

LOCAL STATUS m8641DmaRead(VXB_DMA_RESOURCE_ID pChan,char* src,char* dest,
                          int transferSize,int unitSize,UINT32 flags,
                          pVXB_DMA_COMPLETE_FN complFunc,void * pArg)
{
    DMA_M8641_INFO_S  *pstDmaInfo;

    if(0 == transferSize)
    {
        return OK;
    }
    pstDmaInfo = pChan->pDmaChan;
	pstDmaInfo->uiSize = transferSize;
    pstDmaInfo->pfIsr = complFunc;
    pstDmaInfo->pvArg = pArg;

    return m8641DmaStart(pChan->dmaInst,pstDmaInfo->uiChan,pstDmaInfo->uiOption,
           		         (UINT32)src,pstDmaInfo->uiSrcAttr,(UINT32)dest,
           		         pstDmaInfo->uiDestAttr,
                         transferSize);
}

/******************************************************************************
*
* m8641DmaWrite - initiate a DMA write transaction on a channel
*
* This routine initiates a DMA write operation on a given DMA channel.
* The DMA will be started, and when it completes, the supplied completion
* routine will be invoked.
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/

LOCAL STATUS m8641DmaWrite(VXB_DMA_RESOURCE_ID pChan,char * src,char * dest,
                           int tranSize,int unitSize,UINT32 flags,
                           pVXB_DMA_COMPLETE_FN complFunc,void * pArg)
{
    return m8641DmaRead(pChan,src,dest,tranSize,unitSize,flags,complFunc,pArg);
}

/******************************************************************************
*
* m8641DmaSyncRead - initiate a synchronous DMA read transaction on a channel
*
* This routine initiates a synchronous read operation on a given DMA channel.
* The function will block until the DMA completes.
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/
LOCAL STATUS m8641DmaSyncRead(VXB_DMA_RESOURCE_ID pChan,char * src,char * dest,
                              int *tranSize,int unitSize,UINT32 flags)
{
    UINT32             uiSize = 0;
    DMA_M8641_INFO_S  *pstDmaInfo = NULL;
    STATUS             iRet = 0;
    
    pstDmaInfo = pChan->pDmaChan;
    pstDmaInfo->pfIsr = NULL;
    pstDmaInfo->pvArg = NULL;
    uiSize = *tranSize;
	
    if(0 == uiSize)
    {
        return OK;
    }
    
    iRet = m8641DmaStart(pChan->dmaInst,pstDmaInfo->uiChan,pstDmaInfo->uiOption,
            		      (UINT32)src,pstDmaInfo->uiSrcAttr,(UINT32)dest,
            		      pstDmaInfo->uiDestAttr,uiSize);
    if(OK != iRet)
    {
        return DMA_ERROR_START;
    }
    
    return semTake(pstDmaInfo->semDmaSync,DMA_WAIT_TIMEOUT);
}

/******************************************************************************
*
* m8641DmaSyncWrite - initiate a synchronous DMA write transaction on a channel
*
* This routine initiates a synchronous write operation on a given DMA channel.
* The function will block until the DMA completes.
*
* RETURNS: OK or ERROR if there is an error.
*
* ERRNO
*/

LOCAL STATUS m8641DmaSyncWrite(VXB_DMA_RESOURCE_ID pChan,char * src,char * dest,
                               int * tranSize,int unitSize,UINT32 flags)
{
    return m8641DmaSyncRead(pChan,src,dest,tranSize,unitSize,flags);
}

/******************************************************************************
*
* m8641DmaCancel - cancel a DMA operation on a given channel
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
    return OK;
    }

/******************************************************************************
*
* m8641DmaPause - pause DMA operation on a given channel
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

    return (OK);
    }

/******************************************************************************
*
* m8641DmaResume - resume a DMA channel that has paused
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
    return (OK);
    }

/******************************************************************************
*
* m8641DmaStatus - return DMA channel status
*
* This routine returns the status of a current DMA channel. A channel
* may be paused, idle, running or have no DMA operation in progress.
*
* RETURNS: MCD-specific status code
*
* ERRNO
*/

LOCAL int m8641DmaStatus(VXB_DMA_RESOURCE_ID pChan)
{
    logMsg("m8641DmaStatus \n",0,0,0,0,0,0);
    return OK;
}

LOCAL void m8641DmaHandle(VXB_DEVICE_ID pDev)
{
	VXB_DMA_RESOURCE_ID pChan = NULL;
	DMA_M8641_INFO_S   *pDmaInfo = NULL;    
	UINT32   uiVal = 0;

	uiVal = DMA_CSR_READ_4(pDev,MPC8641_DMA_SR);
	DMA_CSR_WRITE_4(pDev,MPC8641_DMA_SR,uiVal);
	pChan = (VXB_DMA_RESOURCE_ID)pDev->pDrvCtrl;
    
    
	if((NULL != pChan) && (NULL != pChan->pDmaChan))
	{
		pDmaInfo = (DMA_M8641_INFO_S*)pChan->pDmaChan;
		if(NULL != pDmaInfo->pfIsr)
		{
			pDmaInfo->pfIsr(pDmaInfo->pvArg,pDmaInfo->uiSize,OK);
		}
		else
		{
			semGive(pDmaInfo->semDmaSync);
		}
	}
}

void m8641ClrPend(VXB_DEVICE_ID pDev)
{
    UINT32 uiVal = 0;
    UINT32 uiLoop = 0;
    
    DMA_CSR_WRITE_4(pDev,MPC8641_DMA_MR,0);
    for(uiLoop = 0;uiLoop < 10;uiLoop++)
    {
        uiVal = DMA_CSR_READ_4(pDev,MPC8641_DMA_SR);
        if(0 == (uiVal & DMA8641_SR_CB_BUSY))
        {
            break;
        }
        taskDelay(1);
    }
   
    uiVal = DMA_CSR_READ_4(pDev,MPC8641_DMA_SR);
    DMA_CSR_WRITE_4(pDev,MPC8641_DMA_SR,uiVal);
}

LOCAL STATUS m8641DmaStart(VXB_DEVICE_ID pDev,UINT32 uiChan,UINT32 uiOption,
                           UINT32 uiSrc,UINT32 uiSrcAttr,UINT32 uiDest,
                           UINT32 uiDestAttr,int uiSize)
{
    UINT32   uiVal = 0;
    UINT32   uiMrVal = 0;
    UINT32   uiSrVal = 0;
    
    /*assert(0 != uiSize);*/
    m8641ClrPend(pDev);
    uiMrVal = DMA_CSR_READ_4(pDev,MPC8641_DMA_MR);
    uiSrVal = DMA_CSR_READ_4(pDev,MPC8641_DMA_SR);
    
    if((0 != (uiMrVal & DMA8641_MR_CC_MASK)) && 
       (0 != (uiSrVal & (DMA8641_SR_CB_MASK | DMA8641_SR_TE_MASK))))
    {
        logMsg("the channel is not available mr 0x%x sr 0x%x\n",uiMrVal,uiSrVal,
               0,0,0,0);
		return ERROR;
    }

	if (uiSize >= 64*1024*1024)
	{
	    logMsg("the size is more than 64M\n",0,0,0,0,0,0);
		return ERROR;
	}

    DMA_CSR_WRITE_4(pDev,MPC8641_DMA_MR,uiOption | DMA8641_MR_CTM_DIRECT);
    DMA_CSR_WRITE_4(pDev,MPC8641_DMA_SAR,uiSrc);
    DMA_CSR_WRITE_4(pDev,MPC8641_DMA_SATR,uiSrcAttr);
    DMA_CSR_WRITE_4(pDev,MPC8641_DMA_DAR,uiDest);
    DMA_CSR_WRITE_4(pDev,MPC8641_DMA_DATR,uiDestAttr);
    DMA_CSR_WRITE_4(pDev,MPC8641_DMA_BCR,uiSize);
    uiVal = uiOption | DMA8641_MR_CTM_DIRECT|DMA8641_MR_CS_START;
    DMA_CSR_WRITE_4(pDev,MPC8641_DMA_MR,uiVal);
    uiVal = DMA_CSR_READ_4(pDev,MPC8641_DMA_MR);
    
    if((uiVal & DMA8641_MR_CS_START) != DMA8641_MR_CS_START)
    {
        logMsg("start DMA error!\n",uiMrVal,uiSrVal,0,0,0,0);
    	return ERROR;
    }

    return OK;
}

