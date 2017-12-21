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
#include <arch/coldfire/mcf5475.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/util/vxbDmaLib.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/util/vxbParamSys.h>
#include <../src/hwif/h/util/vxbDmaDriverLib.h>
#include "vxbFslDma.h"


METHOD_DECL(vxbDmaResourceGet);
METHOD_DECL(vxbDmaResourceRelease);
LOCAL void m8DmaInstInit (VXB_DEVICE_ID);
LOCAL void m8DmaInstInit2 (VXB_DEVICE_ID);
LOCAL void m8DmaInstConnect (VXB_DEVICE_ID);

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
static STATUS gDmaInitFlag=0;
static SEM_ID gSysDmaSem[4];
FUNCPTR pISRDMA[4]; 
SEM_ID rioDMASendSem[4];
int dmaParam[4];

#define DMA_OPTIONS     MPC8641_DMA_DEF_OPTIONS /* default options */

/***************************NEW DMA -JINGZHONG***********************/
void dma_interrupt_handler(VXB_DEVICE_ID pDev)
{
    unsigned int srVal;

    srVal = *(unsigned int*)(CCSBAR+0x21104+(pDev->unitNumber)*0x80);
    *(unsigned int*)(CCSBAR+0x21104+(pDev->unitNumber)*0x80) = srVal;
    if (pISRDMA[pDev->unitNumber]!=NULL)
    {
		(pISRDMA[pDev->unitNumber])(dmaParam[pDev->unitNumber]);
    }
}

void sysDmaIsr(UINT8 channel)
{
	semGive(rioDMASendSem[channel]);
}

void sysDmaInit(void)
{
	UINT32 i;
	if(!gDmaInitFlag)
	{
		for(i=0;i<4;i++)  
		{
			gSysDmaSem[i] = semBCreate (SEM_Q_FIFO, SEM_FULL);
			rioDMASendSem[i] = semBCreate (SEM_Q_FIFO, SEM_EMPTY);
		}
		gDmaInitFlag=1;
	}
}

STATUS bspBridgeDmaExDirectXfer(BSP_BRIDGE_DMA_CHAN_ID channel,
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
                             )
{
    VUINT32 mrVal,srVal;
	mrVal = *MPC8641_DMA_MREG(channel);
	srVal = *MPC8641_DMA_SREG(channel);
	
    if (((mrVal&DMA8641_MR_CC_MASK)!=0)             
		||((srVal&(DMA8641_SR_CB_MASK|DMA8641_SR_TE_MASK))!=0))
    {
        printf("the channel is not available\n");
		return -1;
    }
	if(channel>3)channel=3;
	(pISRDMA[channel]) = (FUNCPTR)callback;
	dmaParam[channel] =(int)param;
	
    *MPC8641_DMA_MREG(channel) = options|DMA8641_MR_CTM_DIRECT;	 //0x0f000204
	
    *MPC8641_DMA_SAREG(channel) = src;
	*MPC8641_DMA_SATREG(channel) = srcAttrib|0x1000000;
	*MPC8641_DMA_SSREG(channel) = (srcStrideSize<<12)|srcStrideDist;

	*MPC8641_DMA_DAREG(channel) = dst;
	*MPC8641_DMA_DATREG(channel) = dstAttrib|0x1000000;
	*MPC8641_DMA_DSREG(channel) = (dstStrideSize<<12)|dstStrideDist;
	
    *MPC8641_DMA_BCREG(channel) = size;

	*MPC8641_DMA_MREG(channel) = options|DMA8641_MR_XFE_ENABLE|DMA8641_MR_CTM_DIRECT|DMA8641_MR_CS_START;	 //0x0f000205
	if((*MPC8641_DMA_MREG(channel)&DMA8641_MR_CS_START)!=DMA8641_MR_CS_START)
	{
	    printf("start DMA error!\n",mrVal,srVal);
		return -1;
    }
    return 0;	
}
/*
 *  sysExDmaSend(0,0x30000000,0x100,0x200,0x20000000,0x100,0x200,0x100)
 *  */
STATUS sysExDmaSend(UINT8 channel,
		UINT32 src,UINT32  srcStrideSize,UINT32  srcStrideDist,
		UINT32 dst,UINT32  dstStrideSize, UINT32  dstStrideDist,
		UINT32 size)
{
		STATUS flag=0;
		if(gDmaInitFlag == 0)
		{
			printf("systerm DMA is not initial\n");
			return ERROR;
		}
		
		if(semTake(gSysDmaSem[channel],0x100)==ERROR)/*WAIT_FOREVER*/
		{
			printf("DMA channel %d is used now\n",channel);
			return ERROR;
			
		}
		flag=bspBridgeDmaExDirectXfer(channel,MPC8641_DMA_DEF_OPTIONS|(0x1<<9),      /* channel options */ 
			(UINT64)((UINT32)src), srcStrideSize, srcStrideDist, 0x50000, /* source parameters                          */
		      (UINT64)((UINT32)dst), dstStrideSize, dstStrideDist, 0x50000, /* destination parameters                     */
		        size,                              /* transfer size                              */
		      (VOIDFUNCPTR)sysDmaIsr,                  /* user routine called at the end of transfer */
		      (void*)channel);

		if(flag==ERROR) 
		{
			printf("DMA send fail!!!\n");
			semGive(gSysDmaSem[channel]);
			return ERROR;
		}

		if(semTake(rioDMASendSem[channel],0x100)==ERROR)
		{
			printf("semTake timeOut!!!!!!!!\n");
			semGive(gSysDmaSem[channel]);
			return ERROR;
		}
	semGive(gSysDmaSem[channel]);
	return OK;
}

/***********************chain mode********************************************/

LOCAL BOOL bspBridgeDmaPoolValid
    ( 
    BSP_DMA_BRIDGE_POOL_ID  poolId
    )
{   
    if (poolId == NULL)
    {
        return (FALSE);
    }
    if (poolId->signature != POOL_SIGNATURE)
    {
 //   	printf("poolId->signature != POOL_SIGNATURE\n");
        return (FALSE);
    }
    if (poolId->inUse)
    {
    	printf("poolId->inUse\n");
        return (FALSE);
    }    
    return TRUE;
}

STATUS bspBridgeDmaDescPoolReset
    (
    BSP_DMA_BRIDGE_POOL_ID  poolId
    )
{
    if(!bspBridgeDmaPoolValid (poolId))
    {
        printf("BridgeDma pool in use or invalid\n");
        return ERROR;
    }
    
    if (ERROR == semTake(poolId->dmaSem, WAIT_FOREVER))         /* Lock resource        */  
    {   
        /* Should always work unless a pool allocation error occured */
        printf("BridgeDmapool alloc error\n");
        return ERROR;
    }
    bzero((char *)poolId->pMemPool, poolId->memPoolSize);
    
    poolId->pLastLinkDesc = NULL;
    poolId->pLastListDesc = NULL;
    poolId->firstDescType = DESC_TYPE_UNDEFINED;
    poolId->lastDescType = DESC_TYPE_UNDEFINED;
    poolId->listDescCnt = 0;
    poolId->linkDescCnt = 0;
    poolId->valid = FALSE;
    poolId->inUse = FALSE;
    poolId->descFreeHead = 0;
    semGive(poolId->dmaSem);                                    /* Unlock resource      */

    return OK;
}

BSP_DMA_BRIDGE_POOL_ID bspBridgeDmaDescPoolCreate
    (
    UINT32 descCnt
    )
{
    BSP_DMA_BRIDGE_POOL_ID poolId;
    
    if (descCnt == 0)
    {
        printf("INVALID_PARAMETER\n");
        return (NULL);
    }

    poolId = (BSP_DMA_BRIDGE_POOL_ID)malloc(sizeof(sBSP_DMA_BRIDGE_CHAIN_MNGT_BLK));
    if (poolId == NULL)
    {
        printf("PoolId malloc error\n");
        return (NULL);
    }
    
    poolId->signature = POOL_SIGNATURE;
    poolId->maxDescriptor = descCnt;
    poolId->descSize = max(MPC8641_DMA_LINK_DESC_SIZE, MPC8641_DMA_LIST_DESC_SIZE);
    poolId->descAlign = MPC8641_DMA_DESC_ALIGNMENT;
    poolId->memPoolSize = poolId->descSize * descCnt;
    poolId->inUse = FALSE;

    
    poolId->pMemPool = memalign(MPC8641_DMA_DESC_ALIGNMENT, poolId->memPoolSize); 
    if (poolId->pMemPool == NULL)
    {
        printf("bspBridgeDma_DESC_POOL_ALLOC_ERROR\n");
        free(poolId);
        return (NULL);
    }
    
    poolId->dmaSem = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE)
		/*semBCreate(SEM_Q_PRIORITY,SEM_FULL)*/;
    if (NULL == poolId->dmaSem)
    {
        printf("bspBridgeDma pool alloc error\n");
        free(poolId->pMemPool);
        free(poolId);
        return (NULL);
    }

    if (ERROR == bspBridgeDmaDescPoolReset(poolId))
    {
        semDelete(poolId->dmaSem);
        free(poolId->pMemPool);
        free(poolId);
        return (NULL);
    }
    
    return (poolId);
}


STATUS bspBridgeDmaLinkDescAdd
    ( 
    BSP_DMA_BRIDGE_POOL_ID  poolId,
    UINT64                  src,
    UINT32                  srcAttrib,
    UINT64                  dst,
    UINT32                  dstAttrib,
    UINT32                  size
    )
{   
    sDSM_DMA_MPC8641_LINK_DESC* link;
    
    if(!bspBridgeDmaPoolValid (poolId))
    {
        printf("BridgeDma pool in use or invalid\n");
        return ERROR;
    }

    if ((poolId->listDescCnt + poolId->linkDescCnt) >= poolId->maxDescriptor)
    {
       printf("BridgeDma invalid parament\n");
        return ERROR;
    }
   
    semTake(poolId->dmaSem, WAIT_FOREVER);                          /* Lock resource        */
    
    link = (sDSM_DMA_MPC8641_LINK_DESC*) (poolId->pMemPool + (poolId->descSize * poolId->descFreeHead));

    link->srcAttrib = srcAttrib | (src >> 32); /* assumes validation accounts for SRIO address cases */
    link->dstAttrib = dstAttrib | (dst >> 32);

    /* Is this link part of a extended chain ? */
    if (NULL != poolId->pLastListDesc)
    {
        /* Yes */
        /* Is source striding required ? */
        if (0 != poolId->pLastListDesc->srcStride)
        {
            /* Yes */
            link->srcAttrib |= DMA8641_SATR_SSME_ENABLE;   // Source stride mode enable
        }
        /* Is destination striding required ? */
        if (0 != poolId->pLastListDesc->dstStride)
        {
            /* Yes */
            link->dstAttrib |= DMA8641_DATR_DSME_ENABLE;  //direction stride mode enable
        }
    }

    link->srcAdrsLo = (UINT32) (src & ADDR_LO_MASK);
    link->dstAdrsLo = (UINT32) (dst & ADDR_LO_MASK);

    link->nextLinkHi = END_OF_LINK_HI;
    link->nextLinkLo = END_OF_LINK_LO | END_OF_POOL_INT;
    link->byteCnt = size;
    
    if(poolId->pLastLinkDesc != NULL)
    {
        /* mask EOL bit */
        poolId->pLastLinkDesc->nextLinkLo = (poolId->pLastLinkDesc->nextLinkLo & ~END_OF_POOL_INT); 
    }
    /* first entry */
    if (poolId->firstDescType == DESC_TYPE_UNDEFINED)   
    {
        poolId->firstDescType = DESC_TYPE_LINK;
    }
    /* extended mode ending with list */
    else if ((poolId->firstDescType == DESC_TYPE_LIST) && 
             (poolId->lastDescType == DESC_TYPE_LIST)) 
    {
        if (NULL != poolId->pLastListDesc)
        {
            poolId->pLastListDesc->firstLinkHi = 0; /* top four bytes not needed in VxWorks */ 
            poolId->pLastListDesc->firstLinkLo = ((UINT32)link & DESC_ADDR_LO_MASK); 
        }
        else
        {
            /* Internal error, should never happen unless a change in logic is made in another routine */
            printf("bspBridgeDma_DESC_POOL_ALLOC_ERROR\n");
            return(ERROR);
        }
    }
    /* extended mode ending with link OR basic mode */
    else
    {
        if (NULL != poolId->pLastLinkDesc)
        {
            poolId->pLastLinkDesc->nextLinkHi = 0; /* top four bytes not needed in VxWorks */ 
            poolId->pLastLinkDesc->nextLinkLo = ((UINT32)link & DESC_ADDR_LO_MASK); 
        }
        else
        {
            /* Internal error, should never happen unless a change in logic is made in another routine */
            printf("bspBridgeDma_DESC_POOL_ALLOC_ERROR\n");
            return(ERROR);
        }
    }

    poolId->lastDescType = DESC_TYPE_LINK;
    poolId->pLastLinkDesc = link; 
    poolId->descFreeHead++;
    poolId->linkDescCnt++;
    poolId->valid = TRUE;    
    semGive(poolId->dmaSem);                                    /* Unlock resource      */

    return OK;
}

STATUS bspBridgeDmaDescPoolFree
    (
    BSP_DMA_BRIDGE_POOL_ID  poolId
    )
{
    if(!bspBridgeDmaPoolValid (poolId))
    {
        printf("bspBridgeDma_DESC_POOL_IN_USE_OR_INVALID\n");
        return ERROR;
    }
        
    semTake(poolId->dmaSem, WAIT_FOREVER);                      /* Lock resource        */

   /* free descriptor memory */
    free((char*) poolId->pMemPool);
    
    if (semDelete(poolId->dmaSem) == ERROR)                     /* Delete Semaphore     */
    {
        printf("bspBridgeDma_DESC_POOL_ALLOC_ERROR\n");
        return ERROR;
    }

   poolId->signature = 0;
    free((char*) poolId);
   return OK;
}

LOCAL BOOL bspBridgeDmaPoolReady
    ( 
    BSP_DMA_BRIDGE_POOL_ID  poolId
    )
{   
    if (poolId == NULL)
    {
        return (FALSE);
    }
    if (poolId->signature != POOL_SIGNATURE)
    {
        return (FALSE);
    }
    if (poolId->inUse)
    {
        return (FALSE);
    }
    if (!poolId->valid)
    {
        return (FALSE);
    }
    
    return TRUE;
}


STATUS bspBridgeDmaChainedXfer(BSP_BRIDGE_DMA_CHAN_ID  channel,
                               UINT32                  options,
                               BSP_DMA_BRIDGE_POOL_ID  poolId,
                               VOIDFUNCPTR             callback,
                               void*                   param
                              )
{
    UINT32          dmaOptions;
    UINT32          firstDescAddr;
     
    VUINT32 mrVal,srVal;
 	mrVal = *MPC8641_DMA_MREG(channel);
 	srVal = *MPC8641_DMA_SREG(channel);
 	
 	/* 		build link descriptor segment in memory		*/
 	if(channel>3)channel=3;
 	(pISRDMA[channel]) = (FUNCPTR)callback;
 	dmaParam[channel] =(int)*param;

     /* Is this descriptor pool valid and ready */
     if (!bspBridgeDmaPoolReady(poolId))
     {
         /* No */
         printf("BridgeDma pool not ready\n");
         return(ERROR);
     }

     /* Take exclusive access to this pool */
     poolId->inUse = TRUE;
	
  	/*	poll the channel state,to confirm that DMA Channel is idle */
      if (((mrVal&DMA8641_MR_CC_MASK)!=0)             
  		||((srVal&(DMA8641_SR_CB_MASK|DMA8641_SR_TE_MASK))!=0))
      {
    	  poolId->inUse = FALSE;
          printf("the channel is not available\n");
  		  return -1;
      }
     /* Set the device options */     
     /* Determine if this is a basic or extended transfer */

     if (DESC_TYPE_LINK == poolId->firstDescType)
     {
         /* Regular chained transfer, Enable the error and end of links interrupts */
    	 dmaOptions = options |MPC8641_DMA_DEF_OPTIONS|DMA8641_MR_EOLNIE_ENABLE;
     }
     else
     {
         /* Extended chained transfer, Enable the error and end of lists interrupts */
    	 dmaOptions = options |MPC8641_DMA_DEF_OPTIONS|DMA8641_MR_EOLSIE_ENABLE;
     }     

     firstDescAddr = (UINT64)((UINT32)poolId->pMemPool);
     /*Determine if this is a basic or extended transfer */
     if (DESC_TYPE_LINK == poolId->firstDescType)
     {
         /* Basic transfer */
		*MPC8641_DMA_CLNDAREG(channel) = firstDescAddr;	
		//*MPC8641_DMA_ECLNDAREG(channel) = (firstDescAddr>>32)&0xf;    	 
		*MPC8641_DMA_MREG(channel) = dmaOptions & 0xffffffda; 
		*MPC8641_DMA_MREG(channel) = (dmaOptions & 0xffffffda)|DMA8641_MR_CS_START;
     }
	 else
	 {
	     /* Extended transfer */
		*MPC8641_DMA_CLSDAREG(channel) = firstDescAddr;
		*MPC8641_DMA_MREG(channel) = (dmaOptions |DMA8641_MR_XFE_ENABLE )& 0xfffffffb;
		*MPC8641_DMA_MREG(channel) = *MPC8641_DMA_MREG(channel)&0xfffffffe;
		*MPC8641_DMA_MREG(channel) = *MPC8641_DMA_MREG(channel) |DMA8641_MR_CS_START;
	 }
	 /*¿ªÊ¼´«Êä*/
	if((*MPC8641_DMA_MREG(channel)&DMA8641_MR_CS_START)!=DMA8641_MR_CS_START)
	{
	    printf("start DMA error!\n");
		return -1;
	}
     return(OK);
}

STATUS bridgeDmaBasicChaindemo(int channel)
{
    BSP_DMA_BRIDGE_POOL_ID  poolId = 0;
    STATUS errorStat = OK;
	UINT32* srcMem1 = memalign(MPC8641_DMA_DESC_ALIGNMENT, 0x100000);
	UINT32* srcMem2 = memalign(MPC8641_DMA_DESC_ALIGNMENT, 0x100000);
	UINT32* dstMem = memalign(MPC8641_DMA_DESC_ALIGNMENT, 0x200000);
	UINT32  uiLoop = 0;
	
	/*rmSetInc(srcMem1,0x100000);
	rmSetInc(srcMem2,0x100000);*/
	memset(dstMem, 0x00, BUFFER_SIZE*2);
	printf("srcmem1 0x%x,srcmem2 0x%x,srcmem3 0x%x \n",srcMem1,srcMem2,dstMem);
	/*Create descriptor pool */
	
    poolId = bspBridgeDmaDescPoolCreate(1000); 
    if (poolId == NULL)
    {
        printf("ERROR--At bspDmaPoolCreate\n");
        return ERROR;
    }
	for(uiLoop = 0;uiLoop < 100;uiLoop++)
	{
	    if (ERROR == bspBridgeDmaLinkDescAdd(poolId, 
                                         (UINT64)((UINT32)srcMem1 +uiLoop*0x800), SRC_ATTRIB, 
                                         (UINT64)((UINT32)dstMem + uiLoop*0x4000), DST_ATTRIB, 
                                         0x800))
	    {
	        printf("ERROR--At bspDmaLinkAdd\n");
	        return ERROR;
	    }
	}
	if( sysDmaChainSend(channel,poolId) != OK )
	{
		 printf("ERROR--At sysDmaChainSend\n");
		 return ERROR;
	}
	 /* Free the descriptor pool */
	bspBridgeDmaDescPoolFree(poolId);	   
	return OK;

}

STATUS sysDmaChainSend(BSP_BRIDGE_DMA_CHAN_ID  channel,
        BSP_DMA_BRIDGE_POOL_ID  poolId)
{
	INT32 dmaOption = 0;

	dmaOption |= (0xf >> 4);
    if (ERROR == semTake (gSysDmaSem[channel], 100))
    {
    	printf("DMA channel %d is used now\n",channel);
    	return ERROR;
    }

    if (ERROR == bspBridgeDmaChainedXfer(channel, 
                                         DMA_OPTIONS ,
                                         poolId, 
                                         (VOIDFUNCPTR)sysDmaIsr,    /* user routine called at the end of transfer */
                                         (void*)&channel))
    {
        poolId->inUse = FALSE;
        semGive(gSysDmaSem[channel]);
        printf("ERROR--At bspDmaChainedXfer\n");
        return ERROR;
    }

    /* wait until the callback releases the semaphore or until a timeout occurs */
    if (ERROR == semTake(rioDMASendSem[channel], 200))
    {
    	printf("*MPC8641_DMA_SSR(channel)=0x%x, *MPC8641_DMA_NLNDAR(channel)=0x%x\n",*MPC8641_DMA_SSR(channel),*MPC8641_DMA_NLNDAR(channel));
    	 semGive(gSysDmaSem[channel]);
    	 poolId->inUse = FALSE;
    	 printf("Timeout, Transfer failed\n");       
         return ERROR;
    }
    semGive(gSysDmaSem[channel]);
    poolId->inUse = FALSE;
    return OK;
}

LOCAL void m8DmaInstConnect
    (
    VXB_DEVICE_ID       pDev
    )
    {
    vxbIntConnect (pDev, 0, dma_interrupt_handler, pDev);
    vxbIntEnable (pDev, 0, dma_interrupt_handler, pDev);
    return;
    }

LOCAL void m8DmaInstInit2
	(
    VXB_DEVICE_ID pDev
    )
    {


    return;
    }


LOCAL void m8DmaInstInit
    (
    VXB_DEVICE_ID pDev
    )
    {
    return;
    }

LOCAL struct drvBusFuncs m8DmaFuncs =
    {
    m8DmaInstInit,        /* devInstanceInit */
    m8DmaInstInit2,       /* devInstanceInit2 */
    m8DmaInstConnect      /* devConnect */
    };

LOCAL struct vxbDevRegInfo m8Registration =
    {
    NULL,                       /* pNext */
    VXB_DEVID_DEVICE,           /* devID */
    VXB_BUSID_PLB,              /* busID = PLB */
    VXB_VER_4_0_0,            /* vxbVersion */
    "dma",                      /* drvName */
    &m8DmaFuncs,                  /* pDrvBusFuncs */
   NULL,// m8641DmaMethods,                 /* pMethods */
    NULL,
    NULL                        /* devProbe */
    };

/****************************END*************************************/


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
void mDmaDrvRegister (void)
    {
    vxbDevRegister (&m8Registration);
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
        pstDmaInfo->semDmaSync = semBCreate(SEM_Q_PRIORITY,SEM_EMPTY);
        pstDmaInfo->semDmaSrc = semBCreate(SEM_Q_PRIORITY,SEM_FULL);
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
    if(OK != semTake(pstDmaInfo->semDmaSrc,DMA_WAIT_TIMEOUT))
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
    UINT32 uiRd = 0;
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
    UINT32   uiLoop = 0;
    UINT32   uiMaxRetry = 0;
    
    assert(0 != uiSize);
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


STATUS ddrDma(UINT8 chan,char  *pcSrcAddr,char *pcDestAddr,UINT32 uiSize)
{
	DMA_M8641_DEDICATED_S  stChanInfo;
	VXB_DMA_RESOURCE_ID    pstDmaChan = NULL;
	VXB_DEVICE_ID          pstDevId = NULL;
	STATUS                 iRet = OK;
    
	stChanInfo.uiMagic = DMA_M8641_MAGIC;
    stChanInfo.uiUnit = chan;
    stChanInfo.uiDestAttr = lOCAL_DST_ATTRIB_SNOOP;
    stChanInfo.uiSrcAttr = lOCAL_SRC_ATTRIB_SNOOP;
    stChanInfo.uiOption = MPC8641_DMA_DEF_OPTIONS | (1 << 9);
    
	pstDevId = vxbInstByNameFind("dma",chan);
    if(NULL == pstDevId)
    {
        printf("cannot find dam engine\n");
        return DMA_ERROR_OTHER;
    }

	pstDmaChan = vxbDmaChanAlloc(pstDevId,1,DMA_COPY_MODE_NO_SOFT,&stChanInfo);
    if(NULL == pstDmaChan)
    {
        logMsg("resource get failed\n",0,0,0,0,0,0);
        return DMA_ERROR_PROM;
    }
    iRet = vxbDmaReadAndWait(pstDmaChan,pcSrcAddr,pcDestAddr,&uiSize,0,0);
    vxbDmaChanFree(pstDmaChan);

    return iRet;
}

