/*include*/

#include <vxWorks.h>
#include <vxLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <intLib.h>
#include <sysLib.h>
#include <taskLib.h>
#include <vxBusLib.h>
#include <logLib.h>			/* for logMsg() */
#include "hwif/util/vxbParamSys.h"
#include "wrSbc8641.h"
#include "m8641Srio.h"
#include "bspBridgeDma8641.h"

/***********memory mode******************/
/*static UINT32 dmaSendFinal[4]={0,0,0,0};*/
static STATUS gDmaInitFlag=0;
static SEM_ID gSysDmaSem[4];
extern FUNCPTR pISRDMA[4]; 
SEM_ID rioDMASendSem[4];
extern int dmaParam[4];

#define DMA_OPTIONS     MPC8641_DMA_DEF_OPTIONS /* default options */

#define DMA_CHAIN_DEMO
#ifdef DMA_CHAIN_DEMO
/* Local definitions */
#define MAX_BYTES_PER_LINE       16
#define TRANSFER_SIZE   0x40                /* entire transfer size */
#define BUFFER_SIZE     (3*TRANSFER_SIZE)       /* more buffer than required for demo*/
#define DESC_CNT        10                      /* number of decriptors in pool */
#define SRC_ATTRIB      DMA8641_SATR_SREADTTYPE_RD_SNOOP
#define DST_ATTRIB      DMA8641_DATR_DWRITETTYPE_WR_SNOOP
#endif


STATUS sysExDmaSendNoBlock(UINT8 channel,
		UINT32 src,UINT32  srcStrideSize,UINT32  srcStrideDist,
		UINT32 dst,UINT32  dstStrideSize, UINT32  dstStrideDist,
		UINT32 size);

STATUS sysExDmaSend(UINT8 channel,
		UINT32 src,UINT32  srcStrideSize,UINT32  srcStrideDist,
		UINT32 dst,UINT32  dstStrideSize, UINT32  dstStrideDist,
		UINT32 size);

LOCAL BOOL bspBridgeDmaPoolValid
    ( 
    BSP_DMA_BRIDGE_POOL_ID  poolId
    );
LOCAL BOOL bspBridgeDmaPoolReady
    ( 
    BSP_DMA_BRIDGE_POOL_ID  poolId
    );

BSP_DMA_BRIDGE_POOL_ID bspBridgeDmaDescPoolCreate
    (
    UINT32 descCnt
    );
    
STATUS bspBridgeDmaListDescAdd
    (
    BSP_DMA_BRIDGE_POOL_ID  poolId,
    UINT32                  srcStrideDist,
    UINT32                  srcStrideSize,
    UINT32                  dstStrideDist,
    UINT32                  dstStrideSize
    );
STATUS bspBridgeDmaLinkDescAdd
    ( 
    BSP_DMA_BRIDGE_POOL_ID  poolId,
    UINT64                  src,
    UINT32                  srcAttrib,
    UINT64                  dst,
    UINT32                  dstAttrib,
    UINT32                  size
    );
STATUS bspBridgeDmaDescPoolFree
    (
    BSP_DMA_BRIDGE_POOL_ID  poolId
    );
STATUS sysDmaChainSend(BSP_BRIDGE_DMA_CHAN_ID  channel,
        BSP_DMA_BRIDGE_POOL_ID  poolId);

STATUS bspBridgeDmaDescPoolShow
    ( 
    BSP_DMA_BRIDGE_POOL_ID  poolId
    );

#ifdef DMA_CHAIN_DEMO
LOCAL BOOL checkTransfer(UINT8 val,
                         UINT32 srcStrideSize,
                         UINT32 srcStrideDist,
                         UINT32* dstMem,
                         UINT32 dstStrideSize, 
                         UINT32 dstStrideDist,
                         UINT32 transfer_size
                        );
LOCAL void bufset(UINT8  val,
                  UINT32* srcMem,  
                  UINT32 strideSize,
                  UINT32 strideDist,
                  UINT32 size
                 );
LOCAL void d
    (
    void *  adrs,   /* address to display (if 0, display next block */
    int     nunits, /* number of units to print (if 0, use default) */
    int     width   /* width of displaying unit (1, 2, 4, 8) */
    );
#endif




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





#ifdef BRIDGE_DMA



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
                             )
{
    VUINT32 mrVal,srVal;
	mrVal = *MPC8641_DMA_MR(channel);
	srVal = *MPC8641_DMA_SR(channel);

	
    if (((mrVal&DMA8641_MR_CC_MASK)!=0)
		||((srVal&(DMA8641_SR_CB_MASK|DMA8641_SR_TE_MASK))!=0))
    {
      //  printf("mrVal is 0x%x\n",mrVal);
       // printf("srVal is 0x%x\n",srVal);
        printf("the channel is not available\n");
		return -1;
    }
	if (size >= 64*1024*1024)
	{
	    printf("error:the size is more than 64M\n");
		return -1;
	}

	if(channel>3)channel=3;
	(pISRDMA[channel]) = (FUNCPTR)callback;
	dmaParam[channel] =(int)param;
	
    *MPC8641_DMA_MR(channel) = options|DMA8641_MR_CTM_DIRECT;	
	
    *MPC8641_DMA_SAR(channel) = src;
	*MPC8641_DMA_SATR(channel) = srcAttrib;

	*MPC8641_DMA_DAR(channel) = dst;
	*MPC8641_DMA_DATR(channel) = dstAttrib;

	
    *MPC8641_DMA_BCR(channel) = size;

	*MPC8641_DMA_MR(channel) = options|DMA8641_MR_CTM_DIRECT|DMA8641_MR_CS_START;	
	
    if((*MPC8641_DMA_MR(channel)&DMA8641_MR_CS_START)!=DMA8641_MR_CS_START)
	{
	    printf("start DMA error!\n",mrVal,srVal);
		return -1;
    }
	/*while((*MPC8641_DMA_SR(channel)&DMA8641_SR_CB_MASK)!=0);
	
	mrVal = *MPC8641_DMA_MR(channel);
	srVal = *MPC8641_DMA_SR(channel);
    if (((mrVal&(DMA8641_MR_CC_MASK))!=0)
		||((srVal&(DMA8641_SR_CB_MASK|DMA8641_SR_TE_MASK))!=0))
    {
        printf("mrVal is 0x%x, srVal is 0x%x\n",mrVal,srVal);
        printf("the DMA is not finished successfully\n");
		return -1;
    }*/
    return 0;
	

}

#endif


STATUS sysDmaSendNoBlock(UINT8 channel,UINT32 src,UINT32 dst,UINT32 size,void(*callback)(UINT8 channel))
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
	flag=bspBridgeDmaDirectXfer(channel,MPC8641_DMA_DEF_OPTIONS|(0x1<<9),                                /* channel options                            */
			(UINT64)((UINT32)src), 0, 0, 0x50000, /* source parameters                          */
			(UINT64)((UINT32)dst), 0, 0, 0x50000, /* destination parameters                     */
			size,                              /* transfer size                              */
			(VOIDFUNCPTR)callback,                  /* user routine called at the end of transfer */
			(void*)channel);
	if(flag==ERROR) 
	{
		printf("DMA send fail!!!\n");
		semGive(gSysDmaSem[channel]);
		return ERROR;
	}	
	semGive(gSysDmaSem[channel]);
	return OK;
}

void sysDmaIsr(UINT8 channel)
{
#if 0
	if(dmaSendFinal[channel]==0) dmaSendFinal[channel]=1;
#else
	semGive(rioDMASendSem[channel]);
#endif
}


STATUS sysDmaSend(UINT8 channel,UINT32 src,UINT32 dst,UINT32 size)
{
		STATUS flag=0;
		/*dmaSendFinal[channel]=0;*/
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

		flag=bspBridgeDmaDirectXfer(channel,MPC8641_DMA_DEF_OPTIONS|(0x1<<9),                                /* channel options                            */
		      (UINT64)((UINT32)src), 0, 0, 0x50000, /* source parameters                          */
		      (UINT64)((UINT32)dst), 0, 0, 0x50000, /* destination parameters                     */
		         size,                              /* transfer size                              */
		      (VOIDFUNCPTR)sysDmaIsr,                  /* user routine called at the end of transfer */
		      (void*)channel);

		if(flag==ERROR) 
		{
			printf("DMA send fail!!!\n");
			semGive(gSysDmaSem[channel]);
			return ERROR;
		}

	#if 0
		while(dmaSendFinal[channel]==0);
	#else	
		//DPRINTF("before1 semTake\n",1,2,3,4,5,6);
		if(semTake(rioDMASendSem[channel],0x100)==ERROR)
		{
			printf("semTake timeOut!!!!!!!!\n");
			semGive(gSysDmaSem[channel]);
			return ERROR;
		}
	#endif
	
	semGive(gSysDmaSem[channel]);
	return OK;
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
	mrVal = *MPC8641_DMA_MR(channel);
	srVal = *MPC8641_DMA_SR(channel);
	
    if (((mrVal&DMA8641_MR_CC_MASK)!=0)             
		||((srVal&(DMA8641_SR_CB_MASK|DMA8641_SR_TE_MASK))!=0))
    {
        printf("the channel is not available\n");
		return -1;
    }
//	if (size >= 64*1024*1024)
//	{
//	    printf("error:the size is more than 64M\n");
//		return -1;
//	}

	if(channel>3)channel=3;
	(pISRDMA[channel]) = (FUNCPTR)callback;
	dmaParam[channel] =(int)param;
	
    *MPC8641_DMA_MR(channel) = options|DMA8641_MR_CTM_DIRECT;	 //0x0f000204
	
    *MPC8641_DMA_SAR(channel) = src;
	*MPC8641_DMA_SATR(channel) = srcAttrib|0x1000000;
	*MPC8641_DMA_SSR(channel) = (srcStrideSize<<12)|srcStrideDist;

	*MPC8641_DMA_DAR(channel) = dst;
	*MPC8641_DMA_DATR(channel) = dstAttrib|0x1000000;
	*MPC8641_DMA_DSR(channel) = (dstStrideSize<<12)|dstStrideDist;
	
    *MPC8641_DMA_BCR(channel) = size;

	*MPC8641_DMA_MR(channel) = options|DMA8641_MR_XFE_ENABLE|DMA8641_MR_CTM_DIRECT|DMA8641_MR_CS_START;	 //0x0f000205
	printf("Direct:*MPC8641_DMA_MR(channel) = 0x%x\n",*MPC8641_DMA_MR(channel));
	if((*MPC8641_DMA_MR(channel)&DMA8641_MR_CS_START)!=DMA8641_MR_CS_START)
	{
	    printf("start DMA error!\n",mrVal,srVal);
		return -1;
    }
	/*while((*MPC8641_DMA_SR(channel)&DMA8641_SR_CB_MASK)!=0);
	
	mrVal = *MPC8641_DMA_MR(channel);
	srVal = *MPC8641_DMA_SR(channel);
    if (((mrVal&(DMA8641_MR_CC_MASK))!=0)
		||((srVal&(DMA8641_SR_CB_MASK|DMA8641_SR_TE_MASK))!=0))
    {
        printf("mrVal is 0x%x, srVal is 0x%x\n",mrVal,srVal);
        printf("the DMA is not finished successfully\n");
		return -1;
    }*/
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

	#if 0
		while(dmaSendFinal[channel]==0);
	#else	
		//DPRINTF("before1 semTake\n",1,2,3,4,5,6);
		if(semTake(rioDMASendSem[channel],0x100)==ERROR)
		{
			printf("semTake timeOut!!!!!!!!\n");
			semGive(gSysDmaSem[channel]);
			return ERROR;
		}
	#endif	
	semGive(gSysDmaSem[channel]);
	return OK;
}
/*
 * sysExDmaSendNoBlock(0,0x30000000,0x100,0x200,0x20000000,0x100,0x200,0x100)
 * 
 *  */

STATUS sysExDmaSendNoBlock(UINT8 channel,
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
                        
		flag=bspBridgeDmaExDirectXfer(channel,MPC8641_DMA_DEF_OPTIONS|(0x1<<9),   /* channel options   */ 
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
	semGive(gSysDmaSem[channel]);
	return OK;
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
 	mrVal = *MPC8641_DMA_MR(channel);
 	srVal = *MPC8641_DMA_SR(channel);
 	
 	/* 		build link descriptor segment in memory		*/
 	if(channel>3)channel=3;
 	(pISRDMA[channel]) = (FUNCPTR)callback;
 	dmaParam[channel] =(int)param;

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
     /* Determine if this is a basic or extended transfer */
     if (DESC_TYPE_LINK == poolId->firstDescType)
     {
         /* Basic transfer */
    
    	 *MPC8641_DMA_CLNDAR(channel) = firstDescAddr;	
    	// *MPC8641_DMA_ECLNDAR(channel) = (firstDescAddr>>32)&0xf;    	 
    	 *MPC8641_DMA_MR(channel) = dmaOptions & 0xffffffda; 
    	 *MPC8641_DMA_MR(channel) = (dmaOptions & 0xffffffda)|DMA8641_MR_CS_START;
     }
     else
     {
         /* Extended transfer */
    	 
    	 *MPC8641_DMA_CLSDAR(channel) = firstDescAddr;
    	 
    	 *MPC8641_DMA_MR(channel) = (dmaOptions |DMA8641_MR_XFE_ENABLE) & 0xfffffffb;
    	 
 	 	 *MPC8641_DMA_MR(channel) = *MPC8641_DMA_MR(channel)&0xfffffffe;
 	 	
    	 *MPC8641_DMA_MR(channel) = *MPC8641_DMA_MR(channel) |DMA8641_MR_CS_START;
     }  
     if((*MPC8641_DMA_MR(channel)&DMA8641_MR_CS_START)!=DMA8641_MR_CS_START)
 	{
 	    printf("start DMA error!\n");
 		return -1;
     }
     return(OK);
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
    
    poolId->dmaSem = semMCreate(SEM_Q_PRIORITY | SEM_INVERSION_SAFE);
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

STATUS bspBridgeDmaListDescAdd
    (
    BSP_DMA_BRIDGE_POOL_ID  poolId,
    UINT32                  srcStrideDist,
    UINT32                  srcStrideSize,
    UINT32                  dstStrideDist,
    UINT32                  dstStrideSize
    )
{   
    sDSM_DMA_MPC8641_LIST_DESC* list;
    
    if(!bspBridgeDmaPoolValid (poolId))
    {
        printf("BridgeDma pool in use or invalid\n");
        return ERROR;
    }
    if ((poolId->listDescCnt + poolId->linkDescCnt) >= poolId->maxDescriptor)
    {
        printf("INVALID_PARAMETER\n");
        return ERROR;
    }
    if (poolId->firstDescType == DESC_TYPE_LINK)        /* basic mode           */
    {   
        printf("INVALID_PARAMETER\n");
        return ERROR;
    }
    if (poolId->lastDescType == DESC_TYPE_LIST)         /* two lists in a row   */
    {
    	printf("INVALID_PARAMETER\n");
        return ERROR;
    }
    if ((srcStrideDist > STRIDE_BIT_MAX) || (srcStrideSize > STRIDE_BIT_MAX) ||
        (dstStrideDist > STRIDE_BIT_MAX) || (dstStrideSize > STRIDE_BIT_MAX) ||
        (srcStrideDist < srcStrideSize) || (dstStrideDist < dstStrideSize ))   
    {
        printf("INVALID_PARAMETER\n");
        return ERROR;
    }  

    semTake(poolId->dmaSem, WAIT_FOREVER);              /* Lock resource        */
    
    poolId->valid = FALSE;
    
    list = (sDSM_DMA_MPC8641_LIST_DESC*) (poolId->pMemPool + (poolId->descSize * poolId->descFreeHead));
        
    if (poolId->firstDescType == DESC_TYPE_UNDEFINED)   /* first entry          */
    {
        poolId->firstDescType = DESC_TYPE_LIST;
    }
    else
    {
        poolId->pLastListDesc->nextListHi = 0; /* top four bytes not needed in VxWorks */
        poolId->pLastListDesc->nextListLo = ((UINT32) (list)& ADDR_LO_MASK);
        /* Mask EOL bit */
        poolId->pLastListDesc->nextListLo = (poolId->pLastListDesc->nextListLo & ~END_OF_LIST_LO);
    }

    list->firstLinkHi = NO_LINK_HI;
    list->firstLinkLo = NO_LINK_LO;
    list->srcStride = (srcStrideDist | (srcStrideSize << 12));
    list->dstStride = (dstStrideDist | (dstStrideSize << 12));
    list->nextListHi = END_OF_LIST_HI;
    list->nextListLo = END_OF_LIST_LO;
    
    poolId->lastDescType = DESC_TYPE_LIST;
    poolId->pLastListDesc = list;
    poolId->descFreeHead++;
    poolId->listDescCnt++;
    
    semGive(poolId->dmaSem);                            /* Unlock resource      */

    return OK;
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

STATUS sysDmaChainSend(BSP_BRIDGE_DMA_CHAN_ID  channel,
        BSP_DMA_BRIDGE_POOL_ID  poolId)
{
    if (ERROR == semTake (gSysDmaSem[channel], 100))
    {
    	printf("DMA channel %d is used now\n",channel);
    	return ERROR;
    }
    
    if (ERROR == bspBridgeDmaChainedXfer(channel, 
                                         DMA_OPTIONS, 
                                         poolId, 
                                         (VOIDFUNCPTR)sysDmaIsr,    /* user routine called at the end of transfer */
                                         (void*)channel))
    {
        poolId->inUse = FALSE;
        semGive(gSysDmaSem[channel]);
        printf("ERROR--At bspDmaChainedXfer\n");
        return ERROR;
    }
    
    /* wait until the callback releases the semaphore or until a timeout occurs */
    if (ERROR == semTake (rioDMASendSem[channel], 200))
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

void printLink
    ( 
    BSP_DMA_BRIDGE_POOL_ID  poolId,
    sDSM_DMA_MPC8641_LINK_DESC*  printPtr
    )
{
    printf("\n\tLink: 0x%08x\n", printPtr);   
    printf("\tsrcAttrib: 0x%08x\t dstAttrib: 0x%08x\n", printPtr->srcAttrib, printPtr->dstAttrib);
    printf("\tsrcAddress: 0x%08x\t dstAddress: 0x%08x\n", printPtr->srcAdrsLo, printPtr->dstAdrsLo);
    printf("\tnextLink: 0x%08x%08x\t byteCnt: 0x%08x\n", printPtr->nextLinkHi, printPtr->nextLinkLo, printPtr->byteCnt);
    if ((printPtr->nextLinkLo & END_OF_LINK_LO) != END_OF_LINK_LO)
    {
        printPtr = (sDSM_DMA_MPC8641_LINK_DESC*) (printPtr->nextLinkLo & DESC_ADDR_LO_MASK);
        printLink(poolId, printPtr);  /* recursive call */
    }
}
/************************************************************************
* printList - support routine for function bspBridgeDmaDescPoolShow 
*
* This routine prints a list descriptor at the console
*
* PARAMETERS:
* \is
* \i 'poolId'
* This is a pool identification returned by the function 
* bspBridgeDmaDescPoolCreate().
* \i 'printPtr'
* Pointer to the list descriptor for which the data is to be displayed.
* \ie
* 
* RETURNS: N/A.
*
* \NOMANUAL
*/
void printList
    ( 
    BSP_DMA_BRIDGE_POOL_ID  poolId,
    sDSM_DMA_MPC8641_LIST_DESC*  printPtr
    )
{
    printf("\nList: 0x%08x\n", printPtr);
    printf("srcStride: 0x%08x\t dstStride: 0x%08x\n", printPtr->srcStride, printPtr->dstStride);
    printf("nextList: 0x%08x%08x\t firstLink: 0x%08x%08x\n", printPtr->nextListHi, printPtr->nextListLo, printPtr->firstLinkHi, printPtr->firstLinkLo);
    if (printPtr->firstLinkLo != (UINT32)NULL)
    {
        printLink(poolId, (sDSM_DMA_MPC8641_LINK_DESC*) (printPtr->firstLinkLo & DESC_ADDR_LO_MASK));
        
        if (printPtr->nextListLo != END_OF_LIST_LO)
        {
            printPtr = (sDSM_DMA_MPC8641_LIST_DESC*) (printPtr->nextListLo & DESC_ADDR_LO_MASK);
            printList(poolId, printPtr);  /* recursive call */
        }
    }
}
/************************************************************************
* bspBridgeDmaDescPoolShow - dump the descriptor chain contained in a pool
*
* This routine is used to dump on the console the valid descriptor chain
* contained in the descriptor pool.
*
* PARAMETERS:
* \is
* \i 'poolId'
* This is a pool identification returned by the function 
* bspBridgeDmaDescPoolCreate().
* \ie
*
* RETURNS: OK if pool is valid, ERROR otherwise.
*
* \NOMANUAL
*/
STATUS bspBridgeDmaDescPoolShow
    ( 
    BSP_DMA_BRIDGE_POOL_ID  poolId
    )
{   
    void* printPtr;
    
    if(!bspBridgeDmaPoolValid (poolId))
    {
        printf("bspBridgeDmaDescPoolShow:DESC_POOL_IN_USE_OR_INVALID\n");
        return ERROR;
    }
    
    semTake(poolId->dmaSem, WAIT_FOREVER);                      /* Lock resource        */
    
    printPtr = poolId->pMemPool;    
    
    if(poolId->firstDescType == DESC_TYPE_LINK)
    {
        printLink(poolId, (sDSM_DMA_MPC8641_LINK_DESC*) printPtr);
    }
    else if (poolId->firstDescType == DESC_TYPE_LIST)
    {
        printList(poolId, (sDSM_DMA_MPC8641_LIST_DESC*) printPtr);
    }
    
    semGive(poolId->dmaSem);                                    /* Unlock resource      */
    
    return OK;
}
    

#ifdef DMA_CHAIN_DEMO
STATUS bridgeDmaBasicChaindemo(int channel)
{
    BSP_DMA_BRIDGE_POOL_ID  poolId = 0;
    STATUS errorStat = OK;
    UINT8 val1 = 0xAA;
    UINT8 val2 = 0x55;
    
    /* Set up source and destination memory */
    UINT32* srcMem1 = memalign(MPC8641_DMA_DESC_ALIGNMENT, BUFFER_SIZE);
    UINT32* srcMem2 = memalign(MPC8641_DMA_DESC_ALIGNMENT, BUFFER_SIZE);
    UINT32* dstMem  = memalign(MPC8641_DMA_DESC_ALIGNMENT, BUFFER_SIZE*2);
    
    bufset(val1, srcMem1, 0, 0, BUFFER_SIZE);
    bufset(val2, srcMem2, 0, 0, BUFFER_SIZE);
    memset(dstMem, 0x00, BUFFER_SIZE*2);
  
#if 1
    printf("Source memory (2 buffers) before transfer:\n");
    d((void*)srcMem1, 80, 1);
    d((void*)srcMem2, 80, 1);

    printf("Destination memory before transfer:\n");
    d((void*)dstMem, 160, 1);
#endif
     
    
    /* Create descriptor pool */
    poolId = bspBridgeDmaDescPoolCreate(DESC_CNT); 
    if (poolId == NULL)
    {
        printf("ERROR--At bspDmaPoolCreate\n");
        return ERROR;
    } 

    /* Build Link Descriptor Chain */
    printf("Build the descriptor chain:\n");

    if (ERROR == bspBridgeDmaLinkDescAdd(poolId, 
                                         (UINT64)((UINT32)srcMem1), SRC_ATTRIB, 
                                         (UINT64)((UINT32)dstMem), DST_ATTRIB, 
                                         TRANSFER_SIZE))
    {
        printf("ERROR--At bspDmaLinkAdd\n");
        return ERROR;
    }

    if (ERROR == bspBridgeDmaLinkDescAdd(poolId, 
                                         (UINT64)((UINT32)srcMem2), SRC_ATTRIB, 
                                         (UINT64)((UINT32)dstMem+TRANSFER_SIZE), DST_ATTRIB, 
                                         TRANSFER_SIZE))
    {
        printf("ERROR--At bspDmaLinkAdd\n");
        return ERROR;
    }
    
    if (ERROR == bspBridgeDmaDescPoolShow(poolId))
    {
        printf("ERROR--At bspDmaPoolShow\n");
        return ERROR;
    }

    printf("\nPerform the Basic Chained Transfer descriptors:\n");

   if( sysDmaChainSend(channel,poolId) == OK )
   {
	    printf("Transfer Done notification received.\nDestination memory after transfer:\n");
	    d((void*)dstMem, 160, 1);
	}
    /* Free the descriptor pool */
   bspBridgeDmaDescPoolFree(poolId);
    
    /*Check DMA transfer*/
    printf("\nChecking transfer data: ");
    if (!(checkTransfer(val1, 0, 0, dstMem, 0, 0, TRANSFER_SIZE)) ||
        !(checkTransfer(val2, 0, 0, dstMem + (TRANSFER_SIZE/4), 0, 0, TRANSFER_SIZE))
       )
    {
        errorStat = ERROR;
        printf("FAIL.\n");
    }
    else
    {
        printf("PASS.\n");
    }
    
    /* Release the memory and semaphore */
    free(srcMem1);
    free(srcMem2);
    free(dstMem); 
      
    return errorStat;
}

STATUS bridgeDmaExtChaindemo(BSP_BRIDGE_DMA_CHAN_ID channel)
{
    BSP_DMA_BRIDGE_POOL_ID  poolId = 0;
    STATUS errorStat = OK;
    UINT8 val1 = 0x11;
    UINT8 val2 = 0x22;
    UINT8 val3 = 0x33;

    /* Set up source and destination memory */
    UINT32* srcMem1 = memalign(MPC8641_DMA_DESC_ALIGNMENT, BUFFER_SIZE);
    UINT32* srcMem2 = memalign(MPC8641_DMA_DESC_ALIGNMENT, BUFFER_SIZE);
    UINT32* srcMem3 = memalign(MPC8641_DMA_DESC_ALIGNMENT, BUFFER_SIZE);
    UINT32* dstMem  = memalign(MPC8641_DMA_DESC_ALIGNMENT, BUFFER_SIZE);
    
    bufset(val1, srcMem1, 0, 0, BUFFER_SIZE);
    bufset(val2, srcMem2, 0, 0, BUFFER_SIZE);
    bufset(val3, srcMem3, 0, 0, BUFFER_SIZE);
    memset(dstMem, 0x00, BUFFER_SIZE);
     
//    semDone = semBCreate (SEM_Q_FIFO, SEM_EMPTY);
#if 1
    printf("Source memory (3 buffers) before transfer:\n");
    d((void*)srcMem1, 48, 1);
    d((void*)srcMem2, 48, 1);
    d((void*)srcMem3, 48, 1);

    printf("Destination memory before transfer:\n");
    d((void*)dstMem, 80, 1);
#endif
    /* Create descriptor pool */
    poolId = bspBridgeDmaDescPoolCreate(DESC_CNT); 
    if (poolId == NULL)
    {
        printf("ERROR--At bspDmaPoolCreate\n");
        return ERROR;
    } 

    /* Build Link Descriptor Chain */
    printf("Build the descriptor chain:\n");

    if (ERROR == bspBridgeDmaListDescAdd(poolId, 
                                         0, 0,         /* srcStrideDist,srcStrideSize */
                                         16, 8))   /* dstStrideDist,dstStrideSize */
    {
        printf("ERROR--At bspDmaListAdd\n");
        return ERROR;
    }

    if (ERROR == bspBridgeDmaLinkDescAdd(poolId, 
                                         (UINT64)((UINT32)srcMem1), SRC_ATTRIB, 
                                         (UINT64)((UINT32)dstMem), DST_ATTRIB, 
                                         TRANSFER_SIZE/2))
    {
        printf("ERROR--At bspDmaLinkAdd\n");
        return ERROR;
    }

    if (ERROR == bspBridgeDmaListDescAdd(poolId, 
                                         0, 0,     /* srcStrideDist,srcStrideSize */
                                         16, 4))    /* dstStrideDist,dstStrideSize */
    {
        printf("ERROR--At bspDmaListAdd\n");
        return ERROR;
    }

    if (ERROR == bspBridgeDmaLinkDescAdd(poolId, 
                                         (UINT64)((UINT32)srcMem2), SRC_ATTRIB, 
                                         (UINT64)((UINT32)dstMem+8), DST_ATTRIB, 
                                         TRANSFER_SIZE/4))
    {
        printf("ERROR--At bspDmaLinkAdd\n");
        return ERROR;
    }

    if (ERROR == bspBridgeDmaLinkDescAdd(poolId, 
                                         (UINT64)((UINT32)srcMem3), SRC_ATTRIB, 
                                         (UINT64)((UINT32)dstMem+12), DST_ATTRIB, 
                                         TRANSFER_SIZE/4))
    {
        printf("ERROR--At bspDmaLinkAdd\n");
        return ERROR;
    }

    if (ERROR == bspBridgeDmaDescPoolShow(poolId))
    {
        printf("ERROR--At bspDmaPoolShow\n");
        return ERROR;
    }

    printf("\nPerform the Extended Chained Transfer descriptors to demonstrates\nchained striding capabilities):\n");

    if( sysDmaChainSend(channel,poolId) == OK )
    {
    	printf("Transfer Done notification received.\nDestination memory after transfer:\n");
    	d((void*)dstMem, 80, 1);
    }

    /* Free the descriptor pool */
    bspBridgeDmaDescPoolFree(poolId);
    
    /*Check DMA transfer*/
    printf("\nChecking transfer data: ");
    if (!(checkTransfer(val1, 0, 0, dstMem, 8, 16, TRANSFER_SIZE/2)) ||
        !(checkTransfer(val2, 0, 0, dstMem + (8/4), 4, 16, TRANSFER_SIZE/4)) ||
        !(checkTransfer(val3, 0, 0, dstMem + (12/4), 4, 16, TRANSFER_SIZE/4))
       )
    {
        errorStat = ERROR;
        printf("FAIL.\n");
    }
    else
    {
        printf("PASS.\n");
    }
    
    /* Release the memory and semaphore */
    free(srcMem1);
    free(srcMem2);
    free(srcMem3);
    free(dstMem);
   
    return errorStat;
}


LOCAL BOOL checkTransfer(UINT8 val,
                         UINT32 srcStrideSize,
                         UINT32 srcStrideDist,
                         UINT32* dstMem,
                         UINT32 dstStrideSize, 
                         UINT32 dstStrideDist,
                         UINT32 transfer_size
                        )
{
    BOOL transferPass = TRUE;
    UINT8 *dst;  
    UINT32 i, data=0;
    dst = (UINT8 *)dstMem;
    
    if (srcStrideSize == 0 && srcStrideDist == 0 &&
        dstStrideSize == 0 && dstStrideDist == 0)
    {/*Zero striding*/    
        for (i = 0; i < transfer_size; i++)
        {
            if (val != *dst)
            {
                transferPass = FALSE;
            }
            dst++;
        }
    }
    else
    {/*Non-zero striding*/
        for (i = 0; i < transfer_size; i++)
        {
            if (val != *dst)
            {
                transferPass = FALSE;
            }
            dst++;
            data++;
            if (data == dstStrideSize)
            {
                dst = dst + (dstStrideDist - dstStrideSize);
                data = 0;
            }            
        }                   
    }    
    return transferPass;
}


/*************************************************************************** 
* 
* bufset - creates a source buffer in memory for a DMA transfer.
* 
* This routine creates the buffer with the desired value and striding if 
* applicable.
*
* PARAMETERS: val: The value of the source memory.
*             srcMem: source of memory to set
*             strideSize: stride size
*             strideDist: stride distance
*             size: Number of Bytes
*
* RETURNS:    void
*
* NOMANUAL
*
*/ 
LOCAL void bufset(UINT8  val,
                  UINT32* srcMem,  
                  UINT32 strideSize,
                  UINT32 strideDist,
                  UINT32 size
                 )
{

    UINT32 i = 0, data = 0;
    UINT8 *src;    
    src = (UINT8 *)srcMem;
     
    if (strideSize == 0 && strideDist == 0)
    {/*Zero striding*/    
        for (i = 0; i < size; i++)
        {
            *src = val;
            src++;
        }
    }
    else
    {/*Non-zero striding*/
        for (i = 0; i < size; i++)
        {
            *src = val;
            src++;
            data++;
            if (data == strideSize)
            {
                src = src + (strideDist - strideSize);
                data = 0;
            }            
        }                   
    }
}


LOCAL STATUS memoryDump
    (
    void *      adrs,    /* address to display (if 0, display next block */
    int         nunits,  /* number of units to print (if 0, use default) */
    int         width,   /* width of displaying unit (1, 2, 4, 8) */
    const char *    errorStr /* error string */
    )
    {
    char    ascii[MAX_BYTES_PER_LINE + 1]; /* ascii buffer for displaying */
    int     item;       /* item counter displayed per line */
    int     ix;     /* temporary count */
    ascii[MAX_BYTES_PER_LINE] = EOS;    /* put an EOS on the string */

    /* Get last address */

    if (adrs == NULL)
        {
        printf ("%s: cant displat at NULL.\n",
              errorStr);
        return ERROR;
        }

    /* Check valid width and set the default */

    if (width < 0)
        {
        printf ("%s: the display width must be positive.\n", errorStr);
        return ERROR;
        }

    if (width != 0 && width != 1 && width != 2 && width != 4 && width != 8)
        width = 1;

    if (nunits < 0)
        {
        printf ("%s: the number of units displayed must be positive.\n",
              errorStr);
        return ERROR;
        }

    /* round address down to appropriate boundary */

    adrs = (void *)((int) adrs & ~(width - 1));

    /* print leading spaces on first line */

    bfill (ascii, 16, '.');

    printf ("NOTE: memory values are displayed in hexadecimal.\n");

    printf ("0x%08x:  ", (int) adrs & ~0xf);

    for (item = 0; item < ((int) adrs & 0xf) / width; item++)
        {
        printf ("%*s ", 2 * width, " ");
        bfill (&ascii[item * width], 2 * width, ' ');
        }

    /* print out all the words */

    while (nunits-- > 0)
        {
        UINT8 * pByte;
        UINT64  longLongVal;
    
        if (item == MAX_BYTES_PER_LINE/width)
            {
            /* end of line:
             *   print out ascii format values and address of next line */
    
            printf (" *%16s*\n0x%08x:  ", ascii, (int) adrs);
            bfill (ascii, MAX_BYTES_PER_LINE, '.'); /* clear out ascii buffer */
            item = 0;               /* reset word count */
            }
    
        /* 
         * Read memory only one time, storing the values into the temporary
         * buffer longLongVal variable.
         */
    
        switch (width)          /* display in appropriate format */
            {
            case 2:
                longLongVal = (UINT64) *(USHORT *)adrs;
                printf ("%04x", (USHORT)longLongVal);
                break;
            case 4:
                longLongVal = (UINT64) *(ULONG *)adrs;
                printf ("%08lx", (ULONG)longLongVal);
                break;
            case 8:
                longLongVal = *(UINT64 *)adrs;
                printf ("%016llx", longLongVal);
                break;
            case 1:
                /* Drop through */
            default:
    
                /* Default case is to display byte after byte */
    
                longLongVal = (UINT64) *(UINT8 *)adrs;
                printf ("%02x", (UINT8)longLongVal);
                break;
            }
    
            printf (" ");   /* space between words */
    
        /* Set ascii buffer, reading the buffer longLongVal byte after byte. */
    
        pByte = (UINT8 *) &longLongVal + (sizeof (UINT64) - width);

        for (ix = 0; ix < width; ix ++)
            {
            if (*pByte == ' ' || (isascii (*pByte) && isprint (*pByte)))
                {
                ascii[item*width + ix] = *pByte;
                }
            pByte ++;
            }
    
        adrs = (void *)((int)adrs + width);
        item++;
        }

    /* print remainder of last line */

    for (; item < MAX_BYTES_PER_LINE/width; item++)
        printf ("%*s ", 2 * width, " ");

    printf (" *%16s*\n", ascii);    /* print out ascii format values */

    return OK;
    }


LOCAL void d
    (
    void *  adrs,   /* address to display (if 0, display next block */
    int     nunits, /* number of units to print (if 0, use default) */
    int     width   /* width of displaying unit (1, 2, 4, 8) */
    )
    {
    memoryDump (adrs, nunits, width, "d()");
    }
#endif
