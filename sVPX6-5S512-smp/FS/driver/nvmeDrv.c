/******************************************************************************

                  VERSION (C), 2001-2011, 

 ******************************************************************************
  file       : nvmeinit.c
  version    : 1.0
  data       : 2016.3.25
  function   : nvme device driver
******************************************************************************/

/*----------------------------------------------*
 * header file                                  *
 *----------------------------------------------*/
#include "nvmeInc.h"

/*----------------------------------------------*
 * external  variable                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external  function                           *
 *----------------------------------------------*/
extern STATUS nvmeCreatQueue(NVME_DRV_S *pstDrv,NVME_QUEUE_S *pstQueue);
extern STATUS nvmeSendIdentify(NVME_DRV_S *);
extern STATUS nvmeDisableQInt(NVME_DRV_S *pstDrv,NVME_QUEUE_S *pstQueue);
extern STATUS nvmeRWSector(BLK_REQ_S  *pstBlkReq);
extern STATUS nvmeFormat(UINT32 uiPhyId);
/*----------------------------------------------*
 * internal  function                           *
 *----------------------------------------------*/
LOCAL void vxbNvmeInstInit(VXB_DEVICE_ID);
LOCAL void vxbNvmeInstInit2(VXB_DEVICE_ID);
LOCAL void vxbNvmeInstConnect(VXB_DEVICE_ID);

/*----------------------------------------------*
 * global  variable                             *
 *----------------------------------------------*/
UINT32 guiNvmeDbgLevel = NVME_DBG_INFO;
extern UINT32		uiPhyNumEx;
LOCAL PCI_DEVVEND vxbNvmeIdList[] =
{
	/*device,vendor*/
	{0xa804,0x144d},/*samsung 1TB TLC*/
	{0xa802,0x144d},/*samsung 512GB*/
	{0x010f,0x1179},/*toshiba 512GB MLC*/
	{0x5007,0x1987}
};

NVME_DRV_S   gastNvmeDrv[NVME_MAX_PHY];

LOCAL DRIVER_INITIALIZATION gstNvmeFuncs =
{
    vxbNvmeInstInit,
    vxbNvmeInstInit2,
    vxbNvmeInstConnect
};

LOCAL struct vxbPciRegister gstNvmeVxbDevRegistration =
{
   {
		NULL,				/* pNext */
		VXB_DEVID_DEVICE,	/* devID */
		VXB_BUSID_PCI,		/* busID = PLB */
		VXB_VER_4_0_0,		/* vxbVersion */
		"nvme", 			/* drvName */
		&gstNvmeFuncs,	    /* pDrvBusFuncs */
		NULL,				/* pMethods */
		NULL,				/* devProbe */
		NULL				/* pParamDefaults */
	},
	NELEMENTS(vxbNvmeIdList),
	vxbNvmeIdList
};

NVME_DRV_S    gastNvmeDrv[NVME_MAX_PHY];

/*----------------------------------------------*
 * constant variable                            *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macro definition                             *
 *----------------------------------------------*/

/*****************************************************************************
* nvmeGetPhyNum - get phy number by bus number
*****************************************************************************/
UINT32 nvmeGetPhyNum (VXB_DEVICE_ID pDev)
{
	static UINT32          uiNvmePhyCfg[NVME_MAX_PHY] = {3,4,5,7,8,11,0xFF,0xFF,0xFF};
	UINT32                 uiLoop = 0;
	UINT32                 uiVal = 0;
	struct vxbPciDevice   *pPciDevice = NULL;
	UINT8                  ucPciBus = 0;
    UINT8                  ucPciDev = 0;
    UINT8                  ucPciFunc = 0;
	UINT16                 usValue = 0;
	UINT8                  ucOffset = 0;

	pPciDevice = pDev->pBusSpecificDevInfo;
	ucPciBus = pPciDevice->pciBus;
	ucPciDev = pPciDevice->pciDev;
	ucPciFunc = pPciDevice->pciFunc;
	for(uiLoop = 0;uiLoop < NVME_MAX_PHY;uiLoop ++)
	{
		if(0xFF == uiNvmePhyCfg[uiLoop])
		{
			continue;
		}
		if(pPciDevice->pciBus == uiNvmePhyCfg[uiLoop])
        {
			NVME_LOG(NVME_DBG_ERR,"nvme bus %d phy id %d\n",uiNvmePhyCfg[uiLoop],
				     uiLoop,0,0);
			uiPhyNumEx |= (1 << uiLoop);
			return uiLoop;
		}
	}

	NVME_LOG(NVME_DBG_ERR,"bus %d get phy id error\n",pPciDevice->pciBus,0,0,0);
	
	return 0xFF;
}

VOID pcieSetMPS(VXB_DEVICE_ID pDev,PCIE_MPS_SIZE_E eSize)
{
	VXB_DEVICE_ID          busCtrlID = NULL;
	struct vxbPciDevice   *pPciDevice = NULL;
	UINT8                  ucPciBus = 0;
    UINT8                  ucPciDev = 0;
    UINT8                  ucPciFunc = 0;
	UINT16                 usValue = 0;
	UINT8                  ucOffset = 0;

	busCtrlID = vxbDevParent(pDev);
	pPciDevice = pDev->pBusSpecificDevInfo;
	ucPciBus = pPciDevice->pciBus;
	ucPciDev = pPciDevice->pciDev;
	ucPciFunc = pPciDevice->pciFunc;
	vxbPciConfigExtCapFind(busCtrlID, PCI_EXT_RXCAP,ucPciBus,ucPciDev,
                           ucPciFunc,&ucOffset);
	VXB_PCI_BUS_CFG_READ (pDev,ucOffset,2,usValue);
	NVME_LOG(NVME_DBG_ERR,"bus %d get mps 0x%x offset 0x%x\n",ucPciBus,
		     ((usValue >> 5) & 0x7),ucOffset,0);
	usValue = (usValue & 0xFF1F) | (eSize << 5);
    VXB_PCI_BUS_CFG_WRITE (pDev,ucOffset,2,usValue);
	NVME_LOG(NVME_DBG_ERR,"bus %d set mps 0x%x\n",ucPciBus,((usValue>>5) & 0x7),0,0);
}

STATUS nvmeWaitReady(NVME_DRV_S *pstDrv,UINT64 ullCap,BOOL bEnabled)
{
	UINT32  uiBit = bEnabled ? NVME_CSTS_RDY : 0;
	UINT32  uiTime = 0;

	while((NVME_READ_DWORD(&pstDrv->stDev.pstBar->uiCsts)& NVME_CSTS_RDY) != uiBit)
	{
		taskDelay(10);
		if(uiTime > NVME_MAX_DELAY)
		{
			NVME_LOG(NVME_DBG_ERR,"phy %d device not ready\n",pstDrv->uiPhyId,0,0,0);
			return ERROR;
		}
			
		uiTime++;
	}
	
	return OK;
}

STATUS nvmeDisableCtrl(NVME_DRV_S *pstDrv)
{
	UINT32 uiCC = 0;
	
	uiCC = NVME_READ_DWORD(&pstDrv->stDev.pstBar->uiCC);
	NVME_LOG(NVME_DBG_INFO,"phy %d disable control cc addr 0x%x 0x%x\n",pstDrv->uiPhyId,
		     &pstDrv->stDev.pstBar->uiCC,uiCC,0);
	
	if (uiCC & NVME_CC_ENABLE)
	{
		NVME_WRITE_DWORD(&pstDrv->stDev.pstBar->uiCC,(uiCC & ~NVME_CC_ENABLE));
	}
	return nvmeWaitReady(pstDrv,pstDrv->stDev.ullCapacity,FALSE);
}

STATUS nvmeEnableCtrl(NVME_DRV_S *pstDrv)
{
	return nvmeWaitReady(pstDrv,pstDrv->stDev.ullCapacity,TRUE);
}

/*****************************************************************************
 function     : nvmeDMAAlloc
 description  : allocate dma memory
*****************************************************************************/
STATUS nvmeDMAAlloc(NVME_MEM_S *pstMem)
{
    UINT32       uiAllocLen = 0;
    UINT32       uiAlignLen = 0;
    
    if(0 != pstMem->uiAlign)
    {
        uiAlignLen = pstMem->uiAlign - 1;
    }
    
    uiAllocLen = pstMem->uiLen + pstMem->uiAlign;
    pstMem->pvAllocVirPtr = (UINT8*)cacheDmaMalloc(uiAllocLen);
    if(NULL == pstMem->pvAllocVirPtr)
    {
        NVME_LOG(NVME_DBG_ERR,"alloc memory %d failed\n",uiAllocLen,0,0,0);
        return ERROR;
    }
	memset(pstMem->pvAllocVirPtr,0,uiAllocLen);
    uiAlignLen = ((UINT32)(pstMem->pvAllocVirPtr) + uiAlignLen) & (~uiAlignLen);
    uiAlignLen -= (UINT32)(pstMem->pvAllocVirPtr);
    pstMem->pvVirPtr = (VOID*)((UINT32)pstMem->pvAllocVirPtr + uiAlignLen);
    pstMem->ulPhyAddr = (ULONG)(pstMem->pvVirPtr);
	
    NVME_LOG(NVME_DBG_DATA,"alloc len %d alloc %x phy 0x%x vir 0x%x\n",
            uiAllocLen,(ULONG)(pstMem->pvAllocVirPtr),pstMem->ulPhyAddr,
            (ULONG)(pstMem->pvVirPtr));
    
    return OK;
}

VOID nvmeDMAFree(NVME_MEM_S *pstMem)
{
	if(NULL != pstMem->pvAllocVirPtr)
	{
		cacheDmaFree(pstMem->pvAllocVirPtr);
	}
}

STATUS nvmeAllocQueue(NVME_DRV_S *pstDrv,NVME_QUEUE_S *pstQueue,UINT32 uiQid,
                      UINT32 uiDepth,UINT32 uiVec)
{
	NVME_DEV_S    *pstDev = NULL;

	pstDev = &pstDrv->stDev;
	pstQueue->pstNvmeDev = pstDev;
	pstQueue->stCQMem.uiLen = MAX(NVME_PAGE_SIZE,CQ_SIZE(uiDepth));
	pstQueue->stCQMem.uiAlign = pstQueue->stCQMem.uiLen;
	if(ERROR == nvmeDMAAlloc(&pstQueue->stCQMem))
    {
    	goto ERR_EXIT;
    }
	pstQueue->pstCqes = pstQueue->stCQMem.pvVirPtr;
	pstQueue->stSQMem.uiLen = MAX(NVME_PAGE_SIZE,SQ_SIZE(uiDepth));;
	pstQueue->stSQMem.uiAlign = pstQueue->stCQMem.uiLen;
	if(ERROR == nvmeDMAAlloc(&pstQueue->stSQMem))
    {
    	goto ERR_EXIT;
    }
	if(pstDev->uiMPXMAX < 8)
	{
    	pstQueue->stPrpListMem.uiLen = 4096 * uiDepth;
    	pstQueue->stPrpListMem.uiAlign = 4096;
    	if(ERROR == nvmeDMAAlloc(&pstQueue->stPrpListMem))
        {
        	goto ERR_EXIT;
        }
		pstQueue->pstPrpList = pstQueue->stPrpListMem.pvVirPtr;
	}
	SPIN_LOCK_ISR_INIT(&pstQueue->stLock, 0);
	pstQueue->pstCmnd = pstQueue->stSQMem.pvVirPtr;
	pstQueue->usSqTail = 0;
	pstQueue->usCqHead = 0;
	pstQueue->ucCqPhase = 1;
	pstQueue->puiQdb = pstDev->puiDbs + (uiQid << (pstDev->uiDbStripe + 1));
	pstQueue->uiQueueID = uiQid;
	pstQueue->usQdepth = uiDepth;
	pstQueue->ucQSuspended = 0;
	pstQueue->uiCmdIdFlag = 0;
	pstQueue->usCqVector = uiVec;
	return OK;
ERR_EXIT:
	nvmeDMAFree(&pstQueue->stCQMem);
	nvmeDMAFree(&pstQueue->stSQMem);
	
	return ERROR;
}

/*****************************************************************************
* nvmePcieRegInt - register interrupt
*****************************************************************************/
STATUS nvmePcieRegInt(VXB_DEVICE_ID pstVxbDev,VOID *pfFunc,VOID  *pvParm)
{
#if 1    
	struct vxbIntDynaVecInfo stDynaVec;

	memset(&stDynaVec,0,sizeof(struct vxbIntDynaVecInfo));
	stDynaVec.isr = pfFunc;
	stDynaVec.pArg = pvParm;
	if(OK != vxbMsiConnect(pstVxbDev,1,&stDynaVec))
	{
		NVME_LOG(NVME_DBG_ERR,"connect msi failed pstVxbDev 0x%x\n",pstVxbDev,
		         0,0,0);
	}

	if(OK != vxbPciMSIEnable(pstVxbDev,&stDynaVec))
	{
		NVME_LOG(NVME_DBG_ERR,"program msi failed\n",0,0,0,0);
	}
	
#else
    pciIntConnect ((VOIDFUNCPTR *)0,(VOIDFUNCPTR)pfFunc,pvParm);
    intEnable(0);
#endif

    return OK;
}

LOCAL VOID nvmeIntr(UINT32  uiPhyNum)
{
	NVME_DRV_S   *pstDrv = NULL;
	NVME_QUEUE_S *pstQueue = &pstDrv->stIOQueue;

	NVME_LOG(NVME_DBG_DATA,"phy %d intr\n",uiPhyNum,0,0,0);
	if(uiPhyNum >= NVME_MAX_PHY)
	{
		NVME_LOG(NVME_DBG_DATA,"nvme phy %d number overflow\n",uiPhyNum,0,0,0);
		return;
	}
	pstDrv = &gastNvmeDrv[uiPhyNum];
	nvmeProcessCq(pstDrv);
	pstQueue->ucCqeSeen = 0;
}

/*****************************************************************************
* nvmeDrvInit - driver init
*****************************************************************************/
STATUS nvmeDrvInit(NVME_DRV_S *pstDrv,UINT32 uiPhyNum)
{
	UINT32  uiAqa = 0;
	INT32	iRet = 0;
	UINT32  uiCfg = 0;
	UINT64  ullCap = 0;
	UINT64  ullMPXMAX = 0;

	ullCap = pstDrv->stDev.ullCapacity;
	NVME_LOG(NVME_DBG_ERR,"phy %d ullCap is 0x%x%x dev %x\n",uiPhyNum,
		     (ullCap >> 32),ullCap,&pstDrv->stDev);
	
	nvmeDisableCtrl(pstDrv);
	ullMPXMAX = (ullCap >> 52) & 0xF;
	pstDrv->stDev.uiMPXMAX = (UINT32)ullMPXMAX;
	SPIN_LOCK_ISR_INIT(&pstDrv->stLock, 0);
	if(OK != nvmeAllocQueue(pstDrv,&pstDrv->stAdminQueue,NVME_ADMIN_QID,NVME_MAX_REQ,0)) 
	{
		NVME_LOG(NVME_DBG_ERR,"phy %d alloc admin queue failed\n",pstDrv->uiPhyId,0,0,0);
        return ERROR;
	}
	/* admin submission queue size */
	uiAqa = (pstDrv->stAdminQueue.usQdepth -1) << NVME_AQA_ASQS_MASK;	
	uiAqa |= uiAqa << NVME_AQA_ACQS_MASK;		/* admin completion queue size */
		
	uiCfg = NVME_CC_ENABLE | NVME_CC_CSS_NVM;
	uiCfg |= (NVME_PAGE_SHIFT - 12) << NVME_CC_MPS_SHIFT;
	uiCfg |= NVME_CC_ARB_RR | NVME_CC_SHN_NONE;
	uiCfg |= NVME_CC_IOSQES | NVME_CC_IOCQES;	/* SQ-64bytes,SQ-16bytes */

	NVME_WRITE_DWORD(&pstDrv->stDev.pstBar->uiAqa,uiAqa);
	NVME_WRITE_QWORD(&pstDrv->stDev.pstBar->uiAsq,pstDrv->stAdminQueue.stSQMem.ulPhyAddr);
	NVME_WRITE_QWORD(&pstDrv->stDev.pstBar->uiAcq,pstDrv->stAdminQueue.stCQMem.ulPhyAddr);
	NVME_WRITE_DWORD(&pstDrv->stDev.pstBar->uiCC,uiCfg);
	taskDelay(1);
	if(OK != nvmeEnableCtrl(pstDrv))
	{
		NVME_LOG(NVME_DBG_ERR,"phy %d enable device failed\n",pstDrv->uiPhyId,0,0,0);
        return ERROR;
	}
	uiCfg = NVME_READ_DWORD(&pstDrv->stDev.pstBar->uiCC);
	NVME_LOG(NVME_DBG_ERR,"phy %d enable device,control 0x%x MPSMAX 0x%x\n",pstDrv->uiPhyId,
		     uiCfg,pstDrv->stDev.uiMPXMAX,0);
	if(OK != nvmeSendIdentify(pstDrv))
	{
		NVME_LOG(NVME_DBG_ERR,"phy %d send identify failed\n",pstDrv->uiPhyId,0,0,0);
        return ERROR;
	}
	if(OK != nvmeAllocQueue(pstDrv,&pstDrv->stIOQueue,1,NVME_MAX_REQ,0))
	{
		NVME_LOG(NVME_DBG_ERR,"phy %d alloc admin queue failed\n",pstDrv->uiPhyId,0,0,0);
        return ERROR;
	}
	if(OK != nvmeCreatQueue(pstDrv,&pstDrv->stIOQueue))
	{
		NVME_LOG(NVME_DBG_ERR,"phy %d create admin queue failed\n",pstDrv->uiPhyId,0,0,0);
        return ERROR;
	}
	
	return OK;
}

LOCAL void vxbNvmeInstInit(VXB_DEVICE_ID pDev)
{

}

LOCAL void vxbNvmeInstInit2(VXB_DEVICE_ID pDev)
{
	int 		 uiLoop = 0;
	UINT16		 usVal = 0;
	NVME_DRV_S	*pstDrv = NULL;
	UINT32		 uiPhy = 0;

	NVME_LOG(NVME_DBG_ERR,"nvme driver version %s %d\n",NVME_DRV_VERSION,0,0,0);
	uiPhy = nvmeGetPhyNum(pDev);
	assert(uiPhy < NVME_MAX_PHY);
	pstDrv = &gastNvmeDrv[uiPhy];
	bzero ((UINT8*)pstDrv, sizeof (NVME_DRV_S));
	pstDrv->uiPhyId = uiPhy;
	pDev->pDrvCtrl = pstDrv;
	for (uiLoop = 0; uiLoop < VXB_MAXBARS; uiLoop++)
	{
		if (pDev->regBaseFlags[uiLoop] == VXB_REG_MEM)
			break;
	}
	vxbRegMap (pDev,uiLoop, &pstDrv->pvRegAddr);
	pstDrv->stPci.ulBaseAddr = (ULONG)pDev->pRegBase[uiLoop];
	pstDrv->stPci.pucVirAddr = (UINT8*)pDev->pRegBase[uiLoop];
	pstDrv->stPci.uiBus = ((struct vxbPciDevice*)(pDev->pBusSpecificDevInfo))->pciBus;
	pstDrv->pvRegAddr = (VOID*)pDev->pRegBase[uiLoop];
	VXB_PCI_BUS_CFG_READ(pDev,PCI_CFG_SUB_SYSTEM_ID,2,usVal);
	VXB_PCI_BUS_CFG_READ (pDev,PCI_CFG_COMMAND,2,usVal);
	usVal = usVal | PCI_CMD_MEM_ENABLE | PCI_CMD_MASTER_ENABLE;
	VXB_PCI_BUS_CFG_WRITE (pDev,PCI_CFG_COMMAND,2,usVal);
	NVME_LOG(NVME_DBG_DBG,"bus %d bar %d phy 0x%lx vir 0x%x\n",
		     pstDrv->stPci.uiBus,uiLoop,pstDrv->stPci.ulBaseAddr,
        	 pstDrv->stPci.pucVirAddr);
	
    /*disable interrupt*/
	VXB_PCI_BUS_CFG_READ (pDev,PCI_CFG_COMMAND,2,usVal);
	usVal |= (1 << 10);
	VXB_PCI_BUS_CFG_WRITE (pDev,PCI_CFG_COMMAND,2,usVal);
	/*pcieSetMPS(pDev,PCIE_MPS_512_BYTE);*/
}

LOCAL void vxbNvmeInstConnect(VXB_DEVICE_ID pDev)
{
	UINT32		 uiPhy = 0;
	NVME_DRV_S	*pstDrv = NULL;
	UINT32		 uiVal = 0;
	
	pstDrv = (NVME_DRV_S*)pDev->pDrvCtrl;
	uiPhy = pstDrv->uiPhyId;
	NVME_LOG(NVME_DBG_INFO,"phy %d driver 0x%p\n",uiPhy,pstDrv,0,0);
	pstDrv->stPci.pvUpLayer = ((VOID*)pDev);
	pstDrv->pvRegAddr = pstDrv->stPci.pucVirAddr;
	pstDrv->stDev.pstBar = (NVME_BAR_S*)pstDrv->pvRegAddr;
	/*Doorbell address*/
	pstDrv->stDev.puiDbs = (UINT32*)((UINT8*)pstDrv->pvRegAddr + NVME_DOORBELL_OFFSET);
	pstDrv->stDev.ullCapacity = NVME_READ_QWORD(&pstDrv->stDev.pstBar->uiCap);
	NVME_LOG(NVME_DBG_INFO,"nvme phy %d cap %x %x \n",uiPhy,
		     pstDrv->stDev.ullCapacity >> 32,pstDrv->stDev.ullCapacity,0); 
	
	pstDrv->stDev.uiDbStripe = (UINT32)((pstDrv->stDev.ullCapacity >> 32 ) & 0xF);
	uiVal = NVME_READ_DWORD(&pstDrv->stDev.pstBar->uiVs);
	
	NVME_LOG(NVME_DBG_INFO,"phy %d doorbell stripesize 0x%x version "
			 "%d.%d\n",pstDrv->uiPhyId,pstDrv->stDev.uiDbStripe,
			 (uiVal >> 16 & 0xffff),((uiVal >> 8) & 0xff));

	if(OK != nvmeDrvInit(pstDrv,uiPhy))
	{
		NVME_LOG(NVME_DBG_INFO,"nvme phy %d drv init failed\n",uiPhy,0,0,0); 
		return ;
	}

	if(OK != nvmePcieRegInt(pDev,nvmeIntr,(VOID*)pstDrv->uiPhyId))
	{
		NVME_LOG(NVME_DBG_ERR,"phy %d enable queue interrupt failed\n",
			pstDrv->uiPhyId,0,0,0);
        return ERROR;
	}
	
	NVME_LOG(NVME_DBG_INFO,"nvme phy %d probe over !\n",uiPhy,0,0,0); 
}

void nvmeVxbRegister(void)
{
	blkInit();
	blkRegFunc(nvmeRWSector,nvmeFormat);
	vxbDevRegister((struct vxbDevRegInfo *)&gstNvmeVxbDevRegistration);
	taskDelay(10);
}

