/******************************************************************************

                  VERSION (C), 2001-2011, 

 ******************************************************************************
  file       : nvmereq.c
  version    : 1.0
  data       : 2016.3.25
  function   : nvme request process
******************************************************************************/

/*----------------------------------------------*
 * header file                                  *
 *----------------------------------------------*/
#include "nvmeinc.h"

/*----------------------------------------------*
 * external  variable                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external  function                           *
 *----------------------------------------------*/
extern STATUS blkDevAdd(UINT64_S vullSec,UINT32  vuiBytePerSec,UINT32 vuiPhyId);
extern NVME_DRV_S   gastNvmeDrv[NVME_MAX_PHY];

/*----------------------------------------------*
 * internal  function                           *
 *----------------------------------------------*/
 STATUS nvmeRWSector(BLK_REQ_S  *pstBlkReq);
/*----------------------------------------------*
 * global  variable                             *
 *----------------------------------------------*/

/*****************************************************************************
* nvmeHexDump - hex dump
*****************************************************************************/
VOID nvmeHexDump(CHAR *pstName,UINT8 *pbuf, UINT32 len)
{
    UINT32 i;

	taskDelay(10);
    printf("%s:\n",pstName);
    for (i = 0; i < len; )
    {
        if (len - i >= 4)
        {
          printf("%x %x %x %x \n",pbuf[i],pbuf[i+1],pbuf[i+2],pbuf[i+3]);
          i += 4;
        }
        else
        {
          printf("%x \n",pbuf[i]);
          i++;
        }
    }

    printf("\n");
	taskDelay(10);
}

/*****************************************************************************
* nvmeAllocCmndId - allocate free command id
*****************************************************************************/
UINT32 nvmeAllocCmndId(NVME_QUEUE_S *pstQueue)
{
	UINT32 uiLoop = 0;
	NVME_CMD_INFO* pstCmdInfo = pstQueue->pstCmdInfo;
	
	assert(pstQueue->usQdepth <= 32);
	for(uiLoop = 0;uiLoop < pstQueue->usQdepth;uiLoop++)
	{
		if(0 == (pstQueue->uiCmdIdFlag & (1 << uiLoop)))
		{
		    pstQueue->uiCmdIdFlag |= (1 << uiLoop);
			break;
		}
	}
	return uiLoop;
}

/*****************************************************************************
* nvmeFreeCmndId - release command id
*****************************************************************************/
VOID  nvmeFreeCmndId(NVME_QUEUE_S *pstQueue,UINT32 uiCmndId)
{
	pstQueue->uiCmdIdFlag &= (~(1 << uiCmndId));
}

/*****************************************************************************
* nvmeSubmitSyncCmd - send synchronization command
*****************************************************************************/
STATUS nvmeSubmitSyncCmd(NVME_DRV_S *pstDrv,NVME_QUEUE_S *pstQueue,NVME_CMND_S *pstCmd)
{
	UINT32        uiCmndID = 0;
	UINT16        usTail = 0;
	UINT16        usHead = 0;
	UINT8         ucPhase = 0;
	ULONG         ulFlag = 0;
	UINT32        uiLoop = 0;
	UINT32        uiStatus = 0;
	NVME_CMPL_S  *pstCmpl = NULL;
	NVME_CMND_S  *pstSendCmnd = NULL;

	SPIN_LOCK_ISR_TAKE(&pstQueue->stLock);
	uiCmndID = nvmeAllocCmndId(pstQueue);
	if(uiCmndID >= pstQueue->usQdepth)
	{
		SPIN_LOCK_ISR_GIVE(&pstQueue->stLock);
		NVME_LOG(NVME_DBG_ERR,"phy %d alloc cmnd id failed\n",pstDrv->uiPhyId,0,0,0);
		return ERROR;
	}
	NVME_LOG(NVME_DBG_DATA,"phy %d alloc cmnd id[%d],head[%d],tail[%d]\n",
	         pstDrv->uiPhyId,uiCmndID,pstQueue->usCqHead,pstQueue->usSqTail);
	
	usTail = pstQueue->usSqTail;
	pstSendCmnd = pstQueue->pstCmnd + usTail;
	pstCmd->unCmd.stCommon.usCmdId = NVME_SWAP16(uiCmndID);
	memcpy(pstSendCmnd,pstCmd, sizeof(NVME_CMND_S));
	CACHE_DMA_FLUSH (pstSendCmnd,sizeof(NVME_CMND_S));
	if (++usTail == pstQueue->usQdepth)
	{
		usTail = 0;
	}
	
	NVME_LOG(NVME_DBG_DATA,"write tail[0x%x] to doorbell reg[0x%p]\n",usTail,
			 pstQueue->puiQdb,0,0);
	NVME_WRITE_DWORD(pstQueue->puiQdb,usTail);
	SPIN_LOCK_ISR_GIVE(&pstQueue->stLock);
	pstQueue->usSqTail = usTail;
	usHead = pstQueue->usCqHead;
	taskDelay(10);/*wait complete*/
	pstCmpl = pstQueue->pstCqes + usHead;
	ucPhase = pstQueue->ucCqPhase;
	uiStatus = NVME_SWAP16(pstCmpl->usStatus);	
	while((uiStatus & 1) != ucPhase)
	{
	    taskDelay(10);
	    uiLoop++;
	    if(uiLoop > 500)
	    {
			NVME_LOG(NVME_DBG_ERR,"phy %d wait cmnd complete status 0x%x phase 0x%x\n",
	             pstDrv->uiPhyId,uiStatus,ucPhase,0);
	        break;     
	    }
	    uiStatus = NVME_SWAP16(pstCmpl->usStatus);
	}
	NVME_LOG(NVME_DBG_DATA,"complete cmdid [0x%x] sqid[0x%x] status [0x%x] phase [0x%x]\n",
		     NVME_SWAP16(pstCmpl->usCmdId),
		     NVME_SWAP16(pstCmpl->usSqId),uiStatus,ucPhase);
	/*command completion*/
	if(++usHead == pstQueue->usQdepth)
	{
		usHead = 0;
		ucPhase = !ucPhase;
	}

	/*nvmeFreeCmndId(pstQueue,NVME_SWAP16(pstCmpl->usCmdId));*/
	/* If the controller ignores the cq head doorbell and continuously
	 * writes to the queue, it is theoretically possible to wrap around
	 * the queue twice and mistakenly return IRQ_NONE.  Linux only
	 * requires that 0.1% of your interrupts are handled, so this isn't
	 * a big problem.
	 */
	if ((usHead == pstQueue->usCqHead) && (ucPhase == pstQueue->ucCqPhase))
	{
		NVME_LOG(NVME_DBG_ERR,"error:usHead[0x%x][0x%x]ucPhase[0x%x][0x%x]\n",
			usHead,pstQueue->usCqHead,ucPhase,pstQueue->ucCqPhase);
		return 0;
	}
	NVME_LOG(NVME_DBG_DATA,"===write head doorbell reg[0x%p]value[0x%x] cmpl %x\n",
		     pstQueue->puiQdb + (1 << pstDrv->stDev.uiDbStripe),usHead,pstCmpl,0);
	NVME_WRITE_DWORD(pstQueue->puiQdb + (1 << pstDrv->stDev.uiDbStripe),usHead);
	
	pstQueue->usCqHead = usHead;
	pstQueue->ucCqPhase = ucPhase;
	pstQueue->ucCqeSeen = 1;	
	
	return OK;
}

/*****************************************************************************
* nvmeCreatCQueue - create completion queue
*****************************************************************************/
STATUS nvmeCreatCQueue(NVME_DRV_S *pstDrv,NVME_QUEUE_S *pstQueue)
{
	NVME_CMND_S     stCmnd;
	UINT64          ullAddr = 0;
	UINT32 uiCmnd   = NVME_ADMIN_CREATE_CQ;
	UINT32 uiFlag   = NVME_QUEUE_PHYS_CONTIG;/*NVME_CQ_IRQ_ENABLED*/
	
	uiFlag |= NVME_CQ_IRQ_ENABLED;

	memset(&stCmnd,0,sizeof(stCmnd));
	stCmnd.unCmd.stCreateCq.ucOpCode = uiCmnd;
	ullAddr = (UINT64)pstQueue->stCQMem.ulPhyAddr;
	stCmnd.unCmd.stCreateCq.ullPrp1 = NVME_SWAP64(ullAddr);
	stCmnd.unCmd.stCreateCq.usCqid = NVME_SWAP16(pstQueue->uiQueueID);
	stCmnd.unCmd.stCreateCq.usQSize = NVME_SWAP16(pstQueue->usQdepth - 1);
	stCmnd.unCmd.stCreateCq.usCqFlags = NVME_SWAP16(uiFlag);
	stCmnd.unCmd.stCreateCq.usIrqVec = NVME_SWAP16(pstQueue->usCqVector);

	return nvmeSubmitSyncCmd(pstDrv,&pstDrv->stAdminQueue,&stCmnd);
}

/*****************************************************************************
* nvmeCreatSQueue - create submission queue
*****************************************************************************/
STATUS nvmeCreatSQueue(NVME_DRV_S *pstDrv,NVME_QUEUE_S *pstQueue)
{
	NVME_CMND_S     stCmnd;
	UINT32 uiCmnd   = NVME_ADMIN_CREATE_SQ;
	UINT32 uiFlag   = NVME_QUEUE_PHYS_CONTIG | NVME_SQ_PRIO_MEDIUM;
	UINT64          ullAddr = 0;
	
	memset(&stCmnd,0,sizeof(stCmnd));
	stCmnd.unCmd.stCreateSq.ucOpCode = uiCmnd;
	ullAddr = (UINT64)pstQueue->stSQMem.ulPhyAddr;
	stCmnd.unCmd.stCreateSq.ullPrp1 = NVME_SWAP64(ullAddr);
	stCmnd.unCmd.stCreateSq.usSqid = NVME_SWAP16(pstQueue->uiQueueID);
	stCmnd.unCmd.stCreateSq.usQSize = NVME_SWAP16(pstQueue->usQdepth - 1);
	stCmnd.unCmd.stCreateSq.usSqFlags = NVME_SWAP16(uiFlag);
	stCmnd.unCmd.stCreateSq.usCqid = NVME_SWAP16(pstQueue->uiQueueID);

	return nvmeSubmitSyncCmd(pstDrv,&pstDrv->stAdminQueue,&stCmnd);
}

/*****************************************************************************
* nvmeGetLogPage - get log page information
*****************************************************************************/
STATUS nvmeGetLogPage(NVME_DRV_S *pstDrv,NVME_QUEUE_S *pstQueue)
{
    NVME_CMND_S       stCmnd;
    VOID             *pstMem = NULL;
    UINT64            ullAddr = 0;
    
    pstMem = cacheDmaMalloc(4096);
    if(NULL == pstMem)
    {
        NVME_LOG(NVME_DBG_ERR,"phy %d get memory failed\n",pstDrv->uiPhyId,0,0,0);
        return ERROR;
    }
	memset(pstMem,0,4096);
	ullAddr = (UINT64)pstMem;
	stCmnd.unCmd.stCommon.ucOpCode = NVME_ADMIN_GET_LOG_PAGE;
	stCmnd.unCmd.stCommon.uiNsId = NVME_SWAP32(0xFFFFFFFF);
	stCmnd.unCmd.stCommon.ullPrp1 = NVME_SWAP64(ullAddr);
	stCmnd.unCmd.stCommon.auiCdw10[0] = NVME_SWAP32(((sizeof(NVME_SMART_LOG_S) >> 2)
	                                          << 16 )| NVME_GET_SMART_LOG_ERROR);                          
	if(OK != nvmeSubmitSyncCmd(pstDrv,&pstDrv->stAdminQueue,&stCmnd))
	{
		NVME_LOG(NVME_DBG_INFO,"phy %d submit get log page command failed\n",pstDrv->uiPhyId,0,0,0);
		return ERROR;
	}
	
	cacheDmaFree(pstMem);

	return OK;
}

STATUS nvmeProcessCq(NVME_DRV_S *pstDrv)
{
	UINT16       	usHead = 0;
	UINT16 		 	usTail = 0;
	UINT8       	ucPhase = 0;
	UINT16			usStatus = 0;
	NVME_QUEUE_S   *pstQueue = NULL;
	NVME_CMPL_S  	stCmpl;
	UINT32          uiReqId = 0;

	pstQueue = &pstDrv->stIOQueue;
	
	for (;;) {
		SPIN_LOCK_ISR_TAKE(&pstQueue->stLock);
		usHead = pstQueue->usCqHead;
		usTail = pstQueue->usSqTail;
		ucPhase = pstQueue->ucCqPhase;
		stCmpl = pstQueue->pstCqes[usHead];
		usStatus = NVME_SWAP16(stCmpl.usStatus);
		NVME_LOG(NVME_DBG_DBG,"Complete id[0x%x],SqId[0x%x],"
      			"status[0x%x],phase[0x%x]\n",
      			NVME_SWAP16(stCmpl.usCmdId),
      			NVME_SWAP16(stCmpl.usSqId),
      			NVME_SWAP16(stCmpl.usStatus),ucPhase);
		if ((usStatus & 1) != ucPhase)
		{
			SPIN_LOCK_ISR_GIVE(&pstQueue->stLock);
			break;
		}
		pstQueue->usSqHead = NVME_SWAP16(stCmpl.usSqHead); 
		if (++usHead == pstQueue->usQdepth)
		{
			usHead = 0;
			ucPhase = !ucPhase;
		}
		uiReqId = NVME_SWAP16(stCmpl.usCmdId); 
		if(uiReqId >= NVME_MAX_REQ)
		{
			NVME_LOG(NVME_DBG_ERR,"phy %d req id %d overflow\n",pstDrv->uiPhyId,
		     		uiReqId,0,0);
			continue;
		}
		if(0 != (usStatus & 0xfe))
		{
			NVME_LOG(NVME_DBG_ERR,"phy %d req %d failed\n",pstDrv->uiPhyId,
		     		uiReqId,0,0);
		}
		if (usHead == pstQueue->usCqHead && ucPhase == pstQueue->ucCqPhase)
    	{
    		SPIN_LOCK_ISR_GIVE(&pstQueue->stLock);
    		NVME_LOG(NVME_DBG_ERR,"error in int\n",0,0,0,0);
    		return 0;
    	}
		
		NVME_WRITE_DWORD(pstQueue->puiQdb + (1 << pstQueue->pstNvmeDev->uiDbStripe),
						 usHead);
		pstQueue->usCqHead = usHead;
		pstQueue->ucCqPhase = ucPhase;
		pstQueue->ucCqeSeen = 1;
		NVME_LOG(NVME_DBG_DATA,"usSqTail[0x%x],usCqHead[0x%x]\n",pstQueue->usSqTail,
		         pstQueue->usCqHead,0,0);
		SPIN_LOCK_ISR_GIVE(&pstQueue->stLock);
		NVME_LOG(NVME_DBG_DATA,"phy %d req id %d done\n",pstDrv->uiPhyId,uiReqId,0,0);
		pstDrv->astReq[uiReqId].pfDone(&pstDrv->astReq[uiReqId]);
	}
	
	return OK;
}

/*****************************************************************************
* nvmeCreatQueue - create completion and submission queue
*****************************************************************************/
STATUS nvmeCreatQueue(NVME_DRV_S *pstDrv,NVME_QUEUE_S *pstQueue)
{
	
	if(OK != nvmeCreatCQueue(pstDrv,pstQueue))
	{
		NVME_LOG(NVME_DBG_ERR,"phy %d create completion queue faild\n",
		         pstDrv->uiPhyId,0,0,0);
		return ERROR;         
	}	
	
	if(OK != nvmeCreatSQueue(pstDrv,pstQueue))
	{
		NVME_LOG(NVME_DBG_ERR,"phy %d create submission queue faild\n",
		         pstDrv->uiPhyId,0,0,0);
		return ERROR; 
	}
	return OK;
}
/*****************************************************************************
* nvmeSendIdentify - send identify
                     uiNsid: name space id
                     uiCns:  0(controller) 1(name space)
*****************************************************************************/
STATUS nvmeSendIdentify(NVME_DRV_S *pstDrv)
{
	NVME_CMND_S     stCmnd;
	NVME_ID_NS_S   *pstIDNs = NULL;
	NVME_ID_CTRL_S *pstCtrl = NULL;
	UINT64          ullAddr = 0;
	VOID           *pstMem = NULL;
	UINT8          *pcBuf = NULL;
	UINT8           ucFLba = 0;
	UINT32          uiLbaShift = 0;
	UINT32          uiNsid = 1;
	UINT32          uiCns = 0;
	NVME_DEV_S     *pstDev = NULL;
	UINT8           ucWrZeroSupport = 0;
	UINT64_S        ullLBA;

	pstMem = cacheDmaMalloc(4096);	
	memset(pstMem,0,4096);
	ullAddr = (UINT64)pstMem;
	memset(&stCmnd,0,sizeof(NVME_CMND_S));
	stCmnd.unCmd.stIdentify.ucOpCode = NVME_ADMIN_IDENTIFY;
	stCmnd.unCmd.stIdentify.uiNsid = NVME_SWAP32(uiNsid);
	stCmnd.unCmd.stIdentify.ullPrp1 = NVME_SWAP64(ullAddr);
	stCmnd.unCmd.stIdentify.uiCns = NVME_SWAP32(uiCns);
	if(OK != nvmeSubmitSyncCmd(pstDrv,&pstDrv->stAdminQueue,&stCmnd))
	{
		NVME_LOG(NVME_DBG_INFO,"phy %d submit sync command\n",pstDrv->uiPhyId,0,0,0);
		return ERROR;
	}
	pstIDNs = (NVME_ID_NS_S*)pstMem;
	ucFLba = pstIDNs->ucFlbas & 0xf;
	uiLbaShift = pstIDNs->astLbaf[ucFLba].ucDs;
	pstDev = &pstDrv->stDev;
	pstDev->ullCapacity = NVME_SWAP64(pstIDNs->ullNsze) << (uiLbaShift - 9);
	NVME_LOG(NVME_DBG_ERR,"phy %d contain 0x%x sector lba shift 0x%x\n",
	         pstDrv->uiPhyId,(UINT32)pstDev->ullCapacity,uiLbaShift,0);	
	memset(pstMem,0,4096);
	uiNsid = 0;
	uiCns = 1;
	stCmnd.unCmd.stIdentify.uiNsid = NVME_SWAP32(uiNsid);
	stCmnd.unCmd.stIdentify.uiCns = NVME_SWAP32(uiCns);
	if(OK != nvmeSubmitSyncCmd(pstDrv,&pstDrv->stAdminQueue,&stCmnd))
	{
		NVME_LOG(NVME_DBG_INFO,"phy %d submit sync command\n",pstDrv->uiPhyId,0,0,0);
		return ERROR;
	}
	pstCtrl = (NVME_ID_CTRL_S*)pstMem;
	memcpy(pstDev->acSerial,pstCtrl->aucSn,sizeof(pstCtrl->aucSn));
	memcpy(pstDev->acModel,pstCtrl->aucMn,sizeof(pstCtrl->aucMn));
	memcpy(pstDev->acFirmwareRev,pstCtrl->aucFr,sizeof(pstCtrl->aucFr));
	pcBuf = (UINT8*)pstMem;
	ucWrZeroSupport = pcBuf[520];
	uiLbaShift = (((NVME_READ_QWORD(&pstDev->pstBar->uiCap)) >> 48) & 0xF) + 12;
	pstDev->uiMaxSec = 1 << (pstCtrl->ucMdts + uiLbaShift - 9);
	NVME_LOG(NVME_DBG_ERR,"phy %d max request io 0x%x sector mdts %x %x\n",
	         pstDrv->uiPhyId,pstDev->uiMaxSec,pstCtrl->ucMdts,uiLbaShift);
	NVME_LOG(NVME_DBG_ERR,"phy %d write zero support 0x%x\n",pstDrv->uiPhyId,
		                  ucWrZeroSupport,0,0);
	cacheDmaFree(pstMem);
	pstDrv->uiStatus = NVME_LINK_UP;
	ullLBA.hi = pstDev->ullCapacity >> 32;
	ullLBA.lo = pstDev->ullCapacity;
	blkDevAdd(ullLBA,NVME_SECTOR_SIZE,pstDrv->uiPhyId);
	
	return OK;
}

UINT64 nvmeGetPrp2(UINT64 ullPrp1,UINT32 uiMPS)
{
	UINT64 ullPrp2 = 0;
	UINT64 ullTemp = ullPrp1;
	UINT32 uiMod = 0;

	uiMod = ullTemp % uiMPS;
	ullPrp2 = (0 == uiMod) ? 0 : (UINT64)(ullPrp1 + uiMPS - uiMod);
	
	NVME_LOG(NVME_DBG_DATA,"Get prp2 is 0x%x \n",ullPrp2,0,0,0);
	
	return ullPrp2;
}

VOID nvmeSetPrpList(UINT8 *pstBuf,UINT64 ullPhyAddr,UINT32 uiDataLen,UINT32 uiPageSize)
{
	NVME_PRP_LIST_S  *pstPRPList = NULL;
	UINT64            ullPrp = 0;
	UINT64 			  ullPrp2 = 0;
	UINT64            ullTemp = 0;
	UINT32            uiMod = 0;
	UINT32            uiAlignLen = 0;
	INT32             iLeftLen = 0;
	UINT32            uiLoop = 0;
	
	ullPrp = ullPhyAddr;
	pstPRPList = (NVME_PRP_LIST_S*)pstBuf;
	ullPrp2 = ullPhyAddr;
	iLeftLen = uiDataLen;
	while(iLeftLen > 0)
	{
		assert(uiLoop < NVME_MAX_PRP_LIST);
		pstPRPList->aullPrpList[uiLoop] = NVME_SWAP64(ullPrp2);
		iLeftLen = iLeftLen - uiPageSize;
		ullPrp2 = ullPrp2 + uiPageSize;
		uiLoop++;
	}
}

/*****************************************************************************
 function     : nvmeBlkRW
 description  : nvme read from uiStartBlk
 input        : 
 output       : get data to buffer
 return       : 
*****************************************************************************/
STATUS nvmeBlkRW(NVME_REQ_S *pstReq,UINT8 ucOpCode)
{
	NVME_QUEUE_S   *pstQueue = NULL;
	NVME_DRV_S     *pstDrv = NULL;
	NVME_CMND_S    *pstCmnd = NULL;
	UINT32          uiCmndID = 0;
	UINT16          usTail = 0;
	ULONG           ulFlag = 0;
	UINT32          uiNsId = 1;
	UINT64          ullPrp2 = 0;
	UINT32          uiLen = 0;
	UINT32          uiFirstLen = 0;
	UINT8          *pstPrpList = 0;

	assert(NULL != pstReq);
	assert(NULL != pstReq->pvDrv);
	pstDrv = (NVME_DRV_S*)pstReq->pvDrv;
	pstQueue = &pstDrv->stIOQueue;
	SPIN_LOCK_ISR_TAKE(&pstQueue->stLock);
	uiCmndID = pstReq->ucTag;
	if(uiCmndID >= pstQueue->usQdepth)
	{
		SPIN_LOCK_ISR_GIVE(&pstQueue->stLock);
		NVME_LOG(NVME_DBG_ERR,"phy %d alloc cmnd id failed\n",pstDrv->uiPhyId,0,0,0);
		return ERROR;
	}
	usTail = pstQueue->usSqTail;
	pstCmnd = pstQueue->pstCmnd + usTail;
	memset(pstCmnd,0,sizeof(NVME_CMND_S));
	pstCmnd->unCmd.stCommon.usCmdId = NVME_SWAP16(uiCmndID);
	if (++pstQueue->usSqTail == pstQueue->usQdepth)
	{
		pstQueue->usSqTail = 0;
	}	
	pstCmnd->unCmd.stRW.ucOpCode = ucOpCode;/**/
	pstCmnd->unCmd.stRW.uiNsId = NVME_SWAP32(uiNsId);
	pstCmnd->unCmd.stRW.ucFlags = NVME_PRP_TRAN;
	pstCmnd->unCmd.stRW.ullSlba = NVME_SWAP64(pstReq->ullLBA);
	pstCmnd->unCmd.stRW.usLen = NVME_SWAP16(pstReq->uiSecNum - 1);/*NLBA*/
	pstCmnd->unCmd.stRW.usCtrl = NVME_SWAP16(0);
	pstCmnd->unCmd.stRW.uiDsmGmt = NVME_SWAP16(0);
#if 0	
	if(pstQueue->pstNvmeDev->uiMPXMAX >= 8)
	{
#endif		
		pstCmnd->unCmd.stRW.ullPrp1 = NVME_SWAP64(pstReq->ullAddr);
		ullPrp2 = nvmeGetPrp2(pstReq->ullAddr,NVME_PAGE_SIZE);
		pstCmnd->unCmd.stRW.ullPrp2 = NVME_SWAP64(ullPrp2);
#if 0		
	}
	else
	{
		pstCmnd->unCmd.stRW.ullPrp1 = NVME_SWAP64(pstReq->ullAddr);
		ullPrp2 = (UINT64)pstQueue->stPrpListMem.ulPhyAddr + (usTail * sizeof(NVME_PRP_LIST_S));
		pstCmnd->unCmd.stRW.ullPrp2 = NVME_SWAP64(ullPrp2);
		ullPrp2 = nvmeGetPrp2(pstReq->ullAddr,NVME_PAGE_SIZE);
		uiFirstLen = ullPrp2 - pstReq->ullAddr;
		if(0 == ullPrp2)
		{
			uiFirstLen = NVME_PAGE_SIZE;
		}
		uiLen = (pstReq->uiSecNum * 512) - uiFirstLen;
		ullPrp2 = pstReq->ullAddr + uiFirstLen;
		pstPrpList = (UINT8*)pstQueue->pstPrpList;
		pstPrpList = pstPrpList + (usTail * sizeof(NVME_PRP_LIST_S));
		nvmeSetPrpList(pstPrpList,ullPrp2,uiLen,4096);
	}
#endif		
	NVME_WRITE_DWORD(pstQueue->puiQdb,pstQueue->usSqTail);
	SPIN_LOCK_ISR_GIVE(&pstQueue->stLock);
	
    return OK;
}

/*****************************************************************************
 function     : nvmeWriteZero
 description  : nvme write zero 
*****************************************************************************/
STATUS nvmeWriteZero(NVME_REQ_S *pstReq,UINT8 ucOpCode)
{
	NVME_QUEUE_S   *pstQueue = NULL;
	NVME_DRV_S     *pstDrv = NULL;
	NVME_CMND_S    *pstCmnd = NULL;
	UINT32          uiCmndID = 0;
	UINT16          usTail = 0;
	UINT32          uiNsId = 1;
	
	assert(NULL != pstReq);
	assert(NULL != pstReq->pvDrv);
	pstDrv = (NVME_DRV_S*)pstReq->pvDrv;
	pstQueue = &pstDrv->stIOQueue;
	SPIN_LOCK_ISR_TAKE(&pstQueue->stLock);
	uiCmndID = pstReq->ucTag;
	if(uiCmndID >= pstQueue->usQdepth)
	{
		SPIN_LOCK_ISR_GIVE(&pstQueue->stLock);
		NVME_LOG(NVME_DBG_ERR,"phy %d alloc cmnd id failed\n",pstDrv->uiPhyId,0,0,0);
		return ERROR;
	}
	usTail = pstQueue->usSqTail;
	pstCmnd = pstQueue->pstCmnd + usTail;
	memset(pstCmnd,0,sizeof(NVME_CMND_S));
	pstCmnd->unCmd.stCommon.usCmdId = NVME_SWAP16(uiCmndID);
	if (++pstQueue->usSqTail == pstQueue->usQdepth)
	{
		pstQueue->usSqTail = 0;
	}
	NVME_LOG(NVME_DBG_DATA,"phy %d write zero secnum %x lba 0x%x\n",pstDrv->uiPhyId,
		      pstReq->uiSecNum,(UINT32)pstReq->ullLBA,0);
	pstCmnd->unCmd.stRW.ucOpCode = ucOpCode;
	pstCmnd->unCmd.stRW.uiNsId = NVME_SWAP32(uiNsId);
	pstCmnd->unCmd.stRW.ullSlba = NVME_SWAP64(pstReq->ullLBA);
	pstCmnd->unCmd.stRW.usLen = NVME_SWAP16(pstReq->uiSecNum - 1);
	pstCmnd->unCmd.stRW.usCtrl = NVME_SWAP16(0);
	NVME_WRITE_DWORD(pstQueue->puiQdb,pstQueue->usSqTail);
	SPIN_LOCK_ISR_GIVE(&pstQueue->stLock);
	
    return OK;
}

/*****************************************************************************
 function     : nvmeTrim
 description  : nvme trim data 
*****************************************************************************/
STATUS nvmeTrim(NVME_REQ_S *pstReq,UINT8 ucOpCode)
{
	NVME_QUEUE_S       *pstQueue = NULL;
	NVME_DRV_S         *pstDrv = NULL;
	NVME_CMND_S        *pstCmnd = NULL;
	NVME_TRIM_DATA_S   *pstTrim = NULL;
	UINT32              uiCmndID = 0;
	UINT16              usTail = 0;
	UINT32              uiNsId = 1;

	assert(NULL != pstReq);
	assert(NULL != pstReq->pvDrv);
	pstDrv = (NVME_DRV_S*)pstReq->pvDrv;
	pstQueue = &pstDrv->stIOQueue;
	SPIN_LOCK_ISR_TAKE(&pstQueue->stLock);
	uiCmndID = pstReq->ucTag;
	if(uiCmndID >= pstQueue->usQdepth)
	{
		SPIN_LOCK_ISR_GIVE(&pstQueue->stLock);
		NVME_LOG(NVME_DBG_ERR,"phy %d alloc cmnd id failed\n",pstDrv->uiPhyId,0,0,0);
		return ERROR;
	}
	usTail = pstQueue->usSqTail;
	pstCmnd = pstQueue->pstCmnd + usTail;
	memset(pstCmnd,0,sizeof(NVME_CMND_S));
	pstCmnd->unCmd.stCommon.usCmdId = NVME_SWAP16(uiCmndID);
	if (++pstQueue->usSqTail == pstQueue->usQdepth)
	{
		pstQueue->usSqTail = 0;
	}	
	pstCmnd->unCmd.stTrim.ucOpCode = NVME_CMD_DSM;
	pstCmnd->unCmd.stTrim.uiNsId = NVME_SWAP32(uiNsId);
	pstTrim = (NVME_TRIM_DATA_S*)pstReq->pucBuf;
	pstTrim->uiCattr = NVME_SWAP32(0);
	pstTrim->uiNlb = NVME_SWAP32(pstReq->uiSecNum - 1);
	pstTrim->ullSlba = NVME_SWAP64(pstReq->ullLBA);
	pstCmnd->unCmd.stTrim.ullPrp1 = NVME_SWAP64(pstReq->ullAddr);
	pstCmnd->unCmd.stTrim.ullSlba = 0;
	pstCmnd->unCmd.stTrim.uiAttr = NVME_SWAP32(NVME_DSMGMT_AD);
	NVME_WRITE_DWORD(pstQueue->puiQdb,pstQueue->usSqTail);
	SPIN_LOCK_ISR_GIVE(&pstQueue->stLock);

	NVME_LOG(NVME_DBG_DATA,"phy %d trim\n",pstDrv->uiPhyId,0,0,0);
	
    return OK;
}

/*****************************************************************************
 function     : nvmeGetReq
 description  : get free request
 input        : 
 output       : 
 return       : NULL or REQ 
*****************************************************************************/
NVME_REQ_S* nvmeGetReq(NVME_DRV_S *pstDrv)
{   
    UINT32          uiLoop = 0;
	
    SPIN_LOCK_ISR_TAKE(&pstDrv->stLock);
    for(uiLoop = 1;uiLoop < NVME_MAX_REQ;uiLoop++)
    {
        if(NVME_REQ_UNUSED == pstDrv->astReq[uiLoop].uiStatus)
        {
            pstDrv->astReq[uiLoop].uiStatus = NVME_REQ_USED;
			pstDrv->astReq[uiLoop].ucTag = uiLoop;
			pstDrv->astReq[uiLoop].ucTag = (UINT8)uiLoop;
			SPIN_LOCK_ISR_GIVE(&pstDrv->stLock);
			NVME_LOG(NVME_DBG_DATA,"xx get req tag %d\n",uiLoop,0,0,0);
            return (&(pstDrv->astReq[uiLoop]));
        }
    }
	SPIN_LOCK_ISR_GIVE(&pstDrv->stLock);

    return NULL;  
}

/*****************************************************************************
 function     : nvmeFreeReq
 description  : free request
 input        : 
 output       : 
 return       : NULL or REQ 
*****************************************************************************/
VOID nvmeFreeReq(NVME_DRV_S *pstDrv,UINT8 ucTag)
{
	assert(ucTag < NVME_MAX_REQ);
    if(NVME_REQ_UNUSED == pstDrv->astReq[ucTag].uiStatus)
    {
        NVME_LOG(NVME_DBG_ERR,"the request %d has been freed\n",ucTag,0,0,0);
        return;
    }
	SPIN_LOCK_ISR_TAKE(&pstDrv->stLock);
	nvmeFreeCmndId(&pstDrv->stIOQueue,ucTag);
    pstDrv->astReq[ucTag].uiStatus = NVME_REQ_UNUSED;
    pstDrv->astReq[ucTag].ullAddr = 0;
    pstDrv->astReq[ucTag].pucBuf = NULL;
    pstDrv->astReq[ucTag].pstBlkReq = NULL;
	SPIN_LOCK_ISR_GIVE(&pstDrv->stLock);
    NVME_LOG(NVME_DBG_DATA,"phy %d free tag %d\n",pstDrv->uiPhyId,ucTag,0,0);
}

/*****************************************************************************
 function     : nvmeFormat
 description  : nvme format
*****************************************************************************/
STATUS nvmeFormat(UINT32 uiPhyId)
{
	NVME_CMND_S 	  stCmnd;
	VOID			 *pstMem = NULL;
	NVME_DRV_S       *pstDrv = NULL;
	NVME_QUEUE_S     *pstQueue = NULL;
	UINT64			  ullAddr = 0;
	UINT32            uiNsId = 1;
	
	pstMem = cacheDmaMalloc(4096);
	if(NULL == pstMem)
	{
		NVME_LOG(NVME_DBG_ERR,"phy %d get memory failed\n",pstDrv->uiPhyId,0,0,0);
		return ERROR;
	}
	memset(pstMem,0,4096);
	ullAddr = (UINT64)pstMem;
	assert(uiPhyId < NVME_MAX_PHY);
	pstDrv = &gastNvmeDrv[uiPhyId];
	pstQueue = &pstDrv->stAdminQueue;
	stCmnd.unCmd.stCommon.ucOpCode = NVME_ADMIN_FORMAT_NVM;
	stCmnd.unCmd.stCommon.uiNsId = NVME_SWAP32(uiNsId);
	stCmnd.unCmd.stCommon.ullPrp1 = NVME_SWAP64(ullAddr);
	stCmnd.unCmd.stCommon.auiCdw10[0] = NVME_SWAP32(NVME_USER_DATA_ERASE);						   
	if(OK != nvmeSubmitSyncCmd(pstDrv,&pstDrv->stAdminQueue,&stCmnd))
	{
		NVME_LOG(NVME_DBG_INFO,"phy %d submit get log page command failed\n",
			     pstDrv->uiPhyId,0,0,0);
		return ERROR;
	}
	
	cacheDmaFree(pstMem);

	return OK;
}

/*****************************************************************************
 function     : nvmeRWSectorDone
 description  : nvme sector read/write callback
 input        : 
 output       : 
 return       : 
*****************************************************************************/
VOID nvmeRWSectorDone(VOID *pstOrgReq)
{
    NVME_REQ_S          *pstNvmeReq = NULL;
    BLK_REQ_S           *pstBlkReq = NULL;

	NVME_LOG(NVME_DBG_DATA,"phy %d nvmeRWSectorDone\n",pstBlkReq->uiPhyId,0,0,0);
    pstNvmeReq = (NVME_REQ_S*)pstOrgReq;
    pstBlkReq = pstNvmeReq->pstBlkReq;
	nvmeFreeReq((NVME_DRV_S*)pstNvmeReq->pvDrv,pstNvmeReq->ucTag);
	pstBlkReq->pfDone(pstBlkReq);
}

STATUS nvmeRWSector(BLK_REQ_S  *pstBlkReq)
{
	NVME_REQ_S	   *pstReq = NULL;
	NVME_DRV_S	   *pstDrv = NULL;
	UINT32          uiOpCode = 0;
	
	if((pstBlkReq->uiPhyId >= NVME_MAX_PHY) || 
	   (NVME_LINK_UP != gastNvmeDrv[pstBlkReq->uiPhyId].uiStatus))
	{
		NVME_LOG(NVME_DBG_ERR,"dev %d is not ready\n",pstBlkReq->uiPhyId,0,0,0);
		return ERROR;
	}
	
	if(0x0 == pstBlkReq->uiSecNum)
	{
		NVME_LOG(NVME_DBG_ERR,"sector number is 0\n",0,0,0,0);
		return OK;
	}
	NVME_LOG(NVME_DBG_DATA,"nvmeRWSector %x %x %x %x\n",pstBlkReq->uiDevId,
              pstBlkReq->uiSecNum,(UINT32)pstBlkReq->pucBuf,pstBlkReq->uiSecLo);
		
	pstDrv = &gastNvmeDrv[pstBlkReq->uiPhyId];
	pstReq = nvmeGetReq(pstDrv);
	if(NULL == pstReq)
	{

		NVME_LOG(NVME_DBG_ERR,"dev get free request failed\n",0,0,0,0);
		return ERROR;
	}
	
	/*
	 *buffer initialization
	*/
	pstReq->pucBuf = pstBlkReq->pucBuf; 
	pstReq->uiSecNum = pstBlkReq->uiSecNum;
	pstReq->ullAddr = pstBlkReq->pucBuf;
	pstReq->ucSGLNum = 1;
	pstReq->pvDrv = (VOID*)pstDrv;
	pstReq->ucReqType = NVME_SATA_DMA;
	pstReq->pfDone = nvmeRWSectorDone;
	pstReq->pstBlkReq = pstBlkReq;
	pstReq->ullLBA = pstBlkReq->uiSecHi;
	pstReq->ullLBA = (pstReq->ullLBA << 32) | pstBlkReq->uiSecLo;
	
	if(FS_OPER_READ == pstBlkReq->uiOper)
	{
		uiOpCode = NVME_CMD_READ;
	}
	else if(FS_OPER_WRITE == pstBlkReq->uiOper)
	{
		uiOpCode = NVME_CMD_WRITE;
		CACHE_DMA_FLUSH (pstReq->pucBuf,pstReq->uiSecNum * NVME_SECTOR_SIZE);
	}
	else/*TRIM*/
	{
		uiOpCode = NVME_CMD_WRITE_ZERO;
		pstReq->pucBuf = pstBlkReq->pucAlloc;
		pstReq->ullAddr = pstBlkReq->pucAlloc;
		return nvmeTrim(pstReq,uiOpCode);
	}
	if(OK != nvmeBlkRW(pstReq,uiOpCode))
	{
		return ERROR;
	}

	return OK;
}

