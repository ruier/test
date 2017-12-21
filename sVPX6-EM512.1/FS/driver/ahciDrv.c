/******************************************************************************

                  VERSION (C), 2001-2011, 

 ******************************************************************************
  file       : ahciDrv.c
  version    : 1.0
  data       : 2015.6.4
  function   : ahci chip driver
******************************************************************************/

/*----------------------------------------------*
 * header file                                  *
 *----------------------------------------------*/
#include "ahciDrv.h"

UINT32 guiAhciDbgLevel = AHCI_DBG_ERR;

LOCAL void vxbAhciInstInit(VXB_DEVICE_ID);
LOCAL void vxbAhciInstInit2(VXB_DEVICE_ID);
LOCAL void vxbAhciInstConnect(VXB_DEVICE_ID);

LOCAL PCI_DEVVEND vxbAhciIdList[] =
{
    {AHCI_DEVICE_PKT_ID,AHCI_VENDOR_PKT_ID},
    {AHCI_DEVICE_KST_ID,AHCI_VENDOR_KST_ID},
	{AHCI_DEVICE_SAM_ID,AHCI_VENDOR_SAM_ID},
};

/*LOCAL UINT32 ahciPhyCfg[AHCI_MAX_PHY] = {7,8,9,10,11,12,0xFF,0xFF};*/
LOCAL UINT32 ahciPhyCfg[AHCI_MAX_PHY] = {3,7,8,9,10,11,12,0xFF};
UINT32		uiPhyNumEx = 0;
AHCI_DRV_S   gastAhciDrv[AHCI_MAX_PHY];

LOCAL DRIVER_INITIALIZATION gstAhciFuncs =
{
    vxbAhciInstInit,
    vxbAhciInstInit2,
    vxbAhciInstConnect
};

LOCAL struct vxbPciRegister gstAhciVxbDevRegistration =
{
   {
		NULL,				/* pNext */
		VXB_DEVID_DEVICE,	/* devID */
		VXB_BUSID_PCI,		/* busID = PLB */
		VXB_VER_4_0_0,		/* vxbVersion */
		"ahci", 			/* drvName */
		&gstAhciFuncs,	    /* pDrvBusFuncs */
		NULL,				/* pMethods */
		NULL,				/* devProbe */
		NULL				/* pParamDefaults */
	},
	NELEMENTS(vxbAhciIdList),
	vxbAhciIdList
};

/*****************************************************************************
*
* ahciGetPhyNum - get phy number by bus number
*
* RETURNS: N/A
*
*****************************************************************************/
UINT32 ahciGetPhyNum(VXB_DEVICE_ID pDev)
{
	UINT32          uiLoop = 0;
	VXB_DEVICE_ID   pBusCtrlID = NULL;
	UINT32          uiVal = 0;

	pBusCtrlID = vxbDevParent(pDev);
	if(NULL == pBusCtrlID)
	{
	    AHCI_LOG(AHCI_DBG_ERR,"bus controller not exist\n",0,0,0,0,0,0);
        return ERROR;
	}
	for(uiLoop = 0;uiLoop < AHCI_MAX_PHY;uiLoop ++)
	{
		if(0xFF == ahciPhyCfg[uiLoop])
		{
			continue;
		}
		vxbPciConfigInLong(pBusCtrlID,ahciPhyCfg[uiLoop],0,0,
                           0x24,&uiVal);
        AHCI_LOG(AHCI_DBG_DATA,"ahci bus %d bar val 0x%x\n",ahciPhyCfg[uiLoop],
                 uiVal,0,0,0,0);                   
        if((UINT32)pDev->pRegBase[5] == uiVal)
        {
			AHCI_LOG(AHCI_DBG_ERR,"ahci bus %d phy id %d\n",
			         ahciPhyCfg[uiLoop],uiLoop,0,0,0,0);
			uiPhyNumEx |= (1 << uiLoop);
			return uiLoop;
		}
	}

	AHCI_LOG(AHCI_DBG_ERR,"dev %d get phy id error\n",pDev->unitNumber,0,0,0,0,0);
	
	return 0xFF;
}

/*****************************************************************************
*
* ahciPortInit - port init
*
* RETURNS: N/A
*
*****************************************************************************/
STATUS ahciPortInit(AHCI_DRV_S *pstDrv)
{
	AHCI_PORT_S    *pstPort = NULL;
	UINT8          *pucPortAddr = NULL;
	UINT32          uiVal = 0;
	UINT32          uiPortNum = 0;
	UINT32          uiSlotNum = 0;
	UINT32          uiPortStatus = 0;

	pstPort = &pstDrv->stPort;
    pucPortAddr = pstDrv->stPci.pucVirAddr + AHCI_PORT_OFFSET(0);
	uiVal = AHCI_READ_REG(pucPortAddr + AHCI_PxSERR);
	AHCI_WRITE_REG(pucPortAddr + AHCI_PxSERR, uiVal);
	uiVal = AHCI_READ_REG(pucPortAddr + AHCI_PxIS);
	AHCI_WRITE_REG(pucPortAddr + AHCI_PxIS, uiVal);
	AHCI_WRITE_REG(pstDrv->stPci.pucVirAddr +  AHCI_IS,1);
	
	/* Calculate the number of implemented ports */
    uiPortStatus = AHCI_READ_REG(pstDrv->stPci.pucVirAddr + AHCI_PI);
    if(0 == (uiPortStatus & 1))
	{
		AHCI_LOG(AHCI_DBG_ERR,"phy %d port not ready\n",pstDrv->uiPhyId,0,0,0,0,0);
		return ERROR;
	}
	uiVal = AHCI_READ_REG(pstDrv->stPci.pucVirAddr + AHCI_CAP);
    uiPortNum = (uiVal & 0x1f) + 1;
    uiSlotNum = ((uiVal & AHCI_CAP_NCS) >> AHCI_CAP_NCS_SHFT) + 1;
    pstDrv->stPort.uiQueueDepth = uiSlotNum;
    uiVal = AHCI_READ_REG(pstDrv->stPci.pucVirAddr + AHCI_GHC);
	AHCI_WRITE_REG(pstDrv->stPci.pucVirAddr + AHCI_GHC,(uiVal | AHCI_GHC_IE));
	uiVal = AHCI_READ_REG(pstDrv->stPci.pucVirAddr + AHCI_GHC);
	AHCI_LOG(AHCI_DBG_INFO,"phy %d port addr 0x%x\n",pstDrv->uiPhyId,
	         (UINT32)pucPortAddr,0,0,0,0);
	
	return OK;
}

/*****************************************************************************
*
* ahciWaitD2H - wait device to host ready
*
* RETURNS: N/A
*
*****************************************************************************/
STATUS ahciWaitD2H(UINT8 *pucAddr)
{
    UINT32 uiMaxWait = 1000000;
    UINT32 uiCnt = 0;

    for(uiCnt = 0;uiCnt < uiMaxWait;uiCnt++)
    {
        if(0x00 == (AHCI_READ_REG(pucAddr + AHCI_PxTFD) & 0x80))
        {
            return OK;
        }
    }

    return ERROR;
}

/*****************************************************************************
*
* ahciCheckPortRdy - wait port ready
*
* RETURNS: N/A
*
*****************************************************************************/
STATUS ahciWaitPortRdy(AHCI_DRV_S *pstDrv,UINT32 uiPortStatus)
{
    UINT32  uiVal = 0;
    UINT8  *pucPortAddr = NULL;
    
    /*
     *port implemented;ref AHCI VER 1.3 P.17
    */
    pucPortAddr = pstDrv->stPci.pucVirAddr + AHCI_PORT_OFFSET(0);
    uiVal = AHCI_READ_REG(pucPortAddr + AHCI_PxSSTS);
    if(0x3 == (uiVal & 0x3))
    {
        /*
         *port ready
        */
        pstDrv->stPort.uiStatus = AHCI_LINK_UP;
        AHCI_LOG(AHCI_DBG_ERR,"phy %d link up\n",pstDrv->uiPhyId,0,0,0,0,0);
        /*
         *wait for D2H ok
        */ 
        taskDelay(sysClkRateGet()/2);
        #if 0
        if(ERROR == ahciWaitD2H(pucPortAddr))
        {
            AHCI_LOG(AHCI_DBG_ERR,"phy %d wait D2H failed\n",pstDrv->uiPhyId,
                     0,0,0,0,0);
            return ERROR;
        }
		#endif
        AHCI_LOG(AHCI_DBG_ERR,"wait phy %d D2H ok\n",pstDrv->uiPhyId,
                 0,0,0,0,0);
        return OK;         
    }
    else
    {
    	AHCI_LOG(AHCI_DBG_ERR,"phy %d status %x\n",pstDrv->uiPhyId,uiVal,0,0,0,0);
    }

    return ERROR;
}

/*****************************************************************************
 function     : ahciDMAAlloc
 description  : allocate dma memory
 input        : 
 output       : 
 return       : //TODO
*****************************************************************************/
STATUS ahciDMAAlloc(AHCI_MEM_S *pstMem)
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
        AHCI_LOG(AHCI_DBG_INFO,"alloc memory %d failed\n",uiAllocLen,0,0,0,0,0);
        return ERROR;
    }
    uiAlignLen = ((UINT32)(pstMem->pvAllocVirPtr) + uiAlignLen) & (~uiAlignLen);
    uiAlignLen -= (UINT32)(pstMem->pvAllocVirPtr);
    pstMem->pvVirPtr = (VOID*)((UINT32)pstMem->pvAllocVirPtr + uiAlignLen);
    pstMem->uiPhyAddrLo = (UINT32)(pstMem->pvVirPtr);
    pstMem->uiPhyAddrHi = 0;

    AHCI_LOG(AHCI_DBG_DATA,"alloc len %d alloc %x phy 0x%x vir 0x%x\n",
            uiAllocLen,(ULONG)(pstMem->pvAllocVirPtr),pstMem->uiPhyAddrLo,
            (ULONG)(pstMem->pvVirPtr),0,0);
    
    return OK;
}


/*****************************************************************************
*
* ahciMemAlloc - ahci driver memory alloc
*
* RETURNS: N/A
*
*****************************************************************************/
STATUS ahciMemAlloc(AHCI_DRV_S *pstDrv)
{
    AHCI_PORT_S   *pstPort = NULL;
   
    pstPort = &pstDrv->stPort;
    pstPort->stCmndListMem.uiLen = (sizeof(AHCI_CMND_LIST_S) * AHCI_MAX_REQ);
    pstPort->stCmndListMem.uiAlign = AHCI_CMND_LIST_ALIGN;
    pstPort->stRecvFisMem.uiLen = AHCI_RFIS_TABLE_SIZE + 4;
    pstPort->stRecvFisMem.uiAlign = AHCI_RFIS_TABLE_ALIGN;
    
    if(OK != ahciDMAAlloc(&pstPort->stCmndListMem))
    {
		return ERROR;
	}
	
    if(OK != ahciDMAAlloc(&pstPort->stRecvFisMem))
    {
		return ERROR;
	}

	pstPort->stCmndTbl.uiLen = (sizeof(AHCI_CMND_TBL_S) * AHCI_MAX_REQ);
	pstPort->stCmndTbl.uiAlign = 1024;
	if(OK != ahciDMAAlloc(&pstPort->stCmndTbl))
    {
		return ERROR;
	}

    return OK;
}

/*****************************************************************************
 function     : ahciCmndListInit
 description  : ahci command list intiazation
 input        : 
 output       : 
 return       : 
*****************************************************************************/
VOID ahciCmndListInit(AHCI_DRV_S *pstDrv)
{
	UINT32        uiLoop = 0;
    UINT32        uiVal = 0;
	UINT32        uiRead = 0;
    UINT8        *pucAddr = 0;
    AHCI_PORT_S  *pstPort = NULL;
	ULONG         ulPhysAddr = 0;
    UINT32		  reg = 0;
	
    pstPort = &pstDrv->stPort;
    pucAddr = pstDrv->stPci.pucVirAddr + AHCI_PORT_OFFSET(0);
	pstPort->pstCmdRecFis = (AHCI_RECV_FIS_S *)(pstPort->stRecvFisMem.pvVirPtr);
	pstPort->uiCapExt = AHCI_READ_REG(pstDrv->stPci.pucVirAddr + AHCI_CAP2);
    AHCI_WRITE_REG(pucAddr + AHCI_PxFB,pstPort->stRecvFisMem.uiPhyAddrLo);
	AHCI_WRITE_REG(pucAddr + AHCI_PxFBU,pstPort->stRecvFisMem.uiPhyAddrHi);
	pstPort->pstCmdList = (AHCI_CMND_LIST_S *)(pstPort->stCmndListMem.pvVirPtr);
	AHCI_WRITE_REG(pucAddr + AHCI_PxCLB,pstPort->stCmndListMem.uiPhyAddrLo);
    AHCI_WRITE_REG(pucAddr + AHCI_PxCLBU,pstPort->stCmndListMem.uiPhyAddrHi);
	pstPort->pstCmdTbl= (AHCI_CMND_TBL_S *)(pstPort->stCmndTbl.pvVirPtr);
	ulPhysAddr = pstPort->stCmndTbl.uiPhyAddrLo;
	for (uiLoop = 0; uiLoop < pstPort->uiQueueDepth; uiLoop++)
	{
		pstPort->pstCmdList[uiLoop].uiCmndTblAddrLo = AHCI_CPU_LE32(ulPhysAddr);
		pstPort->pstCmdList[uiLoop].uiCmndTblAddrHi = AHCI_CPU_LE32(0);
		ulPhysAddr = ulPhysAddr + sizeof(AHCI_CMND_TBL_S);
	}
	for (uiLoop = 0; uiLoop < pstPort->uiQueueDepth; uiLoop++)
	{
		pstDrv->astReq[uiLoop].semComp = semBCreate(SEM_Q_PRIORITY,SEM_EMPTY);
        if(NULL == pstDrv->astReq[uiLoop].semComp)
        {
            AHCI_LOG(AHCI_DBG_INFO,"phy %d creat req sem failed\n",pstDrv->uiPhyId,
				     0,0,0,0,0);
            return ERROR;
        }
	}
		
}

/*****************************************************************************
 function     : ahciEnable
 description  : ahci enable
 input        : 
 output       : 
 return       : 
*****************************************************************************/
STATUS ahciEnable(AHCI_DRV_S *pstDrv)
{
	UINT32        uiLoop = 0;
	UINT32        uiVal = 0;
	UINT8        *pucAddr = NULL;

	pucAddr = pstDrv->pvRegAddr;
	/*
	 *Some controllers need AHCI_EN to be written multiple times.
	 * Try a few times before giving up
	*/
	for(uiLoop = 0;uiLoop < 5;uiLoop++)
	{
		uiVal = AHCI_READ_REG(pucAddr + AHCI_GHC);
		if(uiVal & HOST_AHCI_EN)
		{
			return OK;
		}
		uiVal = uiVal | HOST_AHCI_EN;
		AHCI_WRITE_REG(pucAddr +  AHCI_GHC,uiVal);
		taskDelay(1);
	}

	return ERROR;
}

/*****************************************************************************
 function     : ahciRstController
 description  : ahci reset controller
 input        : 
 output       : 
 return       : 
*****************************************************************************/
STATUS ahciRstController(AHCI_DRV_S *pstDrv)
{
	UINT8        *pucAddr = NULL;
	UINT32        uiVal = 0;
	UINT32        uiCap = 0;
	UINT32        uiCap2 = 0;
	UINT32        uiPortMap = 0;

	pucAddr = pstDrv->pvRegAddr;
	/*
     *save config
	*/
	uiCap = AHCI_READ_REG(pucAddr + AHCI_CAP);
	uiCap2 = AHCI_READ_REG(pucAddr + AHCI_CAP2);
	uiPortMap = AHCI_READ_REG(pucAddr + AHCI_PI);

	/*
     *reset controller
	*/
    uiVal = AHCI_READ_REG(pucAddr + AHCI_GHC);
    uiVal = uiVal | AHCI_GHC_HR;
	AHCI_WRITE_REG(pucAddr + AHCI_GHC, uiVal);
	uiVal = AHCI_READ_REG(pucAddr + AHCI_GHC);/*FLUSH*/
	taskDelay(sysClkRateGet());
	if (AHCI_READ_REG(pucAddr + AHCI_GHC) & AHCI_GHC_HR)
	{
		AHCI_LOG(AHCI_DBG_ERR,"reset AHCI controller failed addr 0x%x\n",
		         (UINT32)pucAddr,0,0,0,0,0);
		return ERROR;
	}
	if(OK != ahciEnable(pstDrv))
	{
		return ERROR;
	}
	AHCI_WRITE_REG(pucAddr + AHCI_CAP,uiCap);
	AHCI_WRITE_REG(pucAddr + AHCI_CAP2,uiCap2);
	AHCI_WRITE_REG(pucAddr + AHCI_PI,uiPortMap);

	return OK;
}

/*****************************************************************************
 function     : ahciStopDMA
 description  : stop ahci dma engine
 input        : 
 output       : 
 return       : 
*****************************************************************************/
STATUS ahciStopDMA(AHCI_DRV_S *pstDrv)
{
	UINT8          *pucPortAddr = NULL;
	AHCI_PORT_S    *pstPort = NULL;
	UINT32          uiVal = 0;
	UINT32          uiLoop = 0;
	
	pstPort = &pstDrv->stPort;
    pucPortAddr = pstDrv->stPci.pucVirAddr + AHCI_PORT_OFFSET(0);
	uiVal = AHCI_READ_REG(pucPortAddr + AHCI_PxCMD);
	if(0 == (uiVal & (PORT_CMD_START | PORT_CMD_LIST_ON)))
	{
		return OK;
	}
	uiVal &= (~PORT_CMD_START);
	AHCI_WRITE_REG(pucPortAddr + AHCI_PxCMD,uiVal);
	taskDelay(1);
	uiVal = AHCI_READ_REG(pucPortAddr + AHCI_PxCMD);
	if(uiVal & PORT_CMD_LIST_ON)
	{
		return ERROR;
	}

	return OK;
}

/*****************************************************************************
 function     : ahciStopFISRx
 description  : stop ahci FIS receive
 input        : 
 output       : 
 return       : 
*****************************************************************************/
STATUS ahciStopFISRx(AHCI_DRV_S *pstDrv)
{
    UINT8          *pucPortAddr = NULL;
    AHCI_PORT_S    *pstPort = NULL;
    UINT32          uiVal = 0;
    UINT32          uiLoop = 0;
    
    pstPort = &pstDrv->stPort;
    pucPortAddr = pstDrv->stPci.pucVirAddr + AHCI_PORT_OFFSET(0);
    uiVal = AHCI_READ_REG(pucPortAddr + AHCI_PxCMD);
    uiVal &= (~PORT_CMD_FIS_RX);
    AHCI_WRITE_REG(pucPortAddr + AHCI_PxCMD,uiVal);
    taskDelay(1);
    uiVal = AHCI_READ_REG(pucPortAddr + AHCI_PxCMD);
    if(uiVal & PORT_CMD_FIS_ON)
    {
        return ERROR;
    }
    
    return OK;
}


/*****************************************************************************
 function     : ahciStopPort
 description  : stop ahci port
 input        : 
 output       : 
 return       : 
*****************************************************************************/
STATUS ahciStopPort(AHCI_DRV_S *pstDrv)
{
	if(OK != ahciStopDMA(pstDrv))
	{
		return ERROR;
	}
	if(OK != ahciStopFISRx(pstDrv))
	{
		return ERROR;
	}

	return OK;
}

/*****************************************************************************
 function     : ahciPowerUp
 description  : ahci power up
 input        : 
 output       : 
 return       : 
*****************************************************************************/
VOID ahciPowerUp(AHCI_DRV_S *pstDrv)
{
	UINT8          *pucPortAddr = NULL;
    AHCI_PORT_S    *pstPort = NULL;
    UINT32          uiVal = 0;
    UINT32          uiLoop = 0;
    
    pstPort = &pstDrv->stPort;
    pucPortAddr = pstDrv->stPci.pucVirAddr + AHCI_PORT_OFFSET(0);
    uiVal = AHCI_READ_REG(pucPortAddr + AHCI_PxCMD);
    uiVal &= (~PORT_CMD_ICC_MASK);
    uiVal |= PORT_CMD_SPIN_UP;
    AHCI_WRITE_REG(pucPortAddr + AHCI_PxCMD,uiVal);
	/*
     *wake up link
	*/
	uiVal |= PORT_CMD_ICC_MASK;
	AHCI_WRITE_REG(pucPortAddr + AHCI_PxCMD,uiVal);
}

/*****************************************************************************
 function     : ahciStartFISRx
 description  : start ahci FIS receive
 input        : 
 output       : 
 return       : 
*****************************************************************************/
VOID ahciStartFISRx(AHCI_DRV_S *pstDrv)
{
    UINT8          *pucPortAddr = NULL;
    AHCI_PORT_S    *pstPort = NULL;
    UINT32          uiVal = 0;
    UINT32          uiLoop = 0;

    pstPort = &pstDrv->stPort;
    pucPortAddr = pstDrv->stPci.pucVirAddr + AHCI_PORT_OFFSET(0);
    uiVal = AHCI_READ_REG(pucPortAddr + AHCI_PxCMD);
    uiVal |= PORT_CMD_FIS_RX;
    AHCI_WRITE_REG(pucPortAddr + AHCI_PxCMD,uiVal);
    taskDelay(1);
    uiVal = AHCI_READ_REG(pucPortAddr + AHCI_PxCMD);/*FLUSH*/
    AHCI_LOG(AHCI_DBG_ERR,"phy %d start FIS status 0x%x\n",pstDrv->uiPhyId,
             uiVal,0,0,0,0);
}

/*****************************************************************************
 function     : ahciStartDMA
 description  : start ahci dma engine
 input        : 
 output       : 
 return       : 
*****************************************************************************/
VOID ahciStartDMA(AHCI_DRV_S *pstDrv)
{
    UINT8          *pucPortAddr = NULL;
    AHCI_PORT_S    *pstPort = NULL;
    UINT32          uiVal = 0;
    UINT32          uiLoop = 0;

    pstPort = &pstDrv->stPort;
    pucPortAddr = pstDrv->stPci.pucVirAddr + AHCI_PORT_OFFSET(0);
    uiVal = AHCI_READ_REG(pucPortAddr + AHCI_PxCMD);
    uiVal |= PORT_CMD_START;
    AHCI_WRITE_REG(pucPortAddr + AHCI_PxCMD,uiVal);
    taskDelay(1);
    uiVal = AHCI_READ_REG(pucPortAddr + AHCI_PxCMD);/*FLUSH*/
}

/*****************************************************************************
 function     : ahciStartPort
 description  : start ahci port
 input        : 
 output       : 
 return       : 
*****************************************************************************/
STATUS ahciStartPort(AHCI_DRV_S *pstDrv)
{
	ahciPowerUp(pstDrv);
	ahciStartDMA(pstDrv);
	ahciStartFISRx(pstDrv);
	
	return OK;
}

/*****************************************************************************
 function     : ahciStartPort
 description  : start ahci port
 input        : 
 output       : 
 return       : 
*****************************************************************************/
STATUS ahciEnableInt(AHCI_DRV_S *pstDrv)
{
	AHCI_PORT_S    *pstPort = NULL;
	UINT8          *pucPortAddr = NULL;
	UINT32          uiVal = 0;

	pstPort = &pstDrv->stPort;
    pucPortAddr = pstDrv->stPci.pucVirAddr + AHCI_PORT_OFFSET(0);
    uiVal = AHCI_READ_REG(pucPortAddr +  AHCI_PxIS);
    AHCI_WRITE_REG(pucPortAddr +  AHCI_PxIS,uiVal);
    AHCI_WRITE_REG(pstDrv->stPci.pucVirAddr +  AHCI_IS,1);
	uiVal = 0x7840007f;
	AHCI_WRITE_REG(pucPortAddr +  AHCI_PxIE,uiVal);
	uiVal = AHCI_READ_REG(pucPortAddr +  AHCI_PxIE);
	AHCI_LOG(AHCI_DBG_ERR,"phy %d port enable 0x%x\n",pstDrv->uiPhyId,
             uiVal,0,0,0,0);
             
	return OK;
}

/*****************************************************************************
*
* ahciDrvInit - driver init
*
* RETURNS: N/A
*
*****************************************************************************/
STATUS ahciDrvInit(UINT32 uiPhyNum)
{
	AHCI_DRV_S   *pstDrv = NULL;
	
	UINT32        uiPortStatus = 0;
    
    pstDrv = &gastAhciDrv[uiPhyNum];
	SPIN_LOCK_ISR_INIT(&pstDrv->stLock, 0);
    if(OK != ahciEnable(pstDrv))
	{
		AHCI_LOG(AHCI_DBG_ERR,"enable phy %d ahci failed\n",uiPhyNum,0,0,0,0,0);
		return ERROR;
	}
	
	if(OK != ahciRstController(pstDrv))
	{
		AHCI_LOG(AHCI_DBG_ERR,"reset phy %d ahci failed\n",uiPhyNum,0,0,0,0,0);
		return ERROR;
	}

	if(OK != ahciStopPort(pstDrv))
	{
		AHCI_LOG(AHCI_DBG_ERR,"stop phy %d ahci failed\n",uiPhyNum,0,0,0,0,0);
		return ERROR;
	}
	
    if(OK != ahciPortInit(pstDrv))
    {
		return ERROR;
    }
	if(OK != ahciWaitPortRdy(pstDrv,uiPortStatus))
	{
		return ERROR;
	}
	
	if(ERROR == ahciMemAlloc(pstDrv))
    {
        AHCI_LOG(AHCI_DBG_ERR,"phy %d allocate memory failed\n",pstDrv->uiPhyId,
                 0,0,0,0,0);
        return ERROR;
    }

    ahciCmndListInit(pstDrv);
    if(OK != ahciStartPort(pstDrv))
	{
		AHCI_LOG(AHCI_DBG_ERR,"start phy %d ahci failed\n",uiPhyNum,0,0,0,0,0);
		return ERROR;
	}
	
	AHCI_LOG(AHCI_DBG_ERR,"phy %d ahci drv 0x%x comand table size %d list %d init ok\n",
	         pstDrv->uiPhyId,(UINT32)pstDrv,sizeof(AHCI_CMND_TBL_S),
	         sizeof(AHCI_CMND_LIST_S),0,0);
	
    return OK;
}

/*****************************************************************************
*
* vxbAhciInstInit - first pass initialization
*
* RETURNS: N/A
*
*****************************************************************************/
LOCAL void vxbAhciInstInit(VXB_DEVICE_ID pDev)
{

}

/*****************************************************************************
*
* vxbIntelAhciInstInit2 - Second pass initialization
*
* RETURNS: N/A
*
*****************************************************************************/
LOCAL void vxbAhciInstInit2(VXB_DEVICE_ID pDev)
{
    int          uiLoop = 0;
    UINT16       usVal = 0;
    AHCI_DRV_S  *pstDrv = NULL;
    UINT32       uiPhy = 0;

    uiPhy = ahciGetPhyNum(pDev);
    assert(uiPhy < AHCI_MAX_PHY);
    pstDrv = &gastAhciDrv[uiPhy];
    bzero ((UINT8*)pstDrv, sizeof (AHCI_DRV_S));
    pstDrv->pDev = pDev;
    pstDrv->uiPhyId = uiPhy;
    pDev->pDrvCtrl = pstDrv;
    
    for (uiLoop = 0; uiLoop < VXB_MAXBARS; uiLoop++)
    {
        if (pDev->regBaseFlags[uiLoop] == VXB_REG_MEM)
            break;
    }

    vxbRegMap (pDev,uiLoop, &pstDrv->pvRegAddr);
	pstDrv->stPci.aulBarAddr = (ULONG)pDev->pRegBase[uiLoop];
	pstDrv->stPci.pucVirAddr = (UINT8*)pDev->pRegBase[uiLoop];
	pstDrv->pvRegAddr = (VOID*)pDev->pRegBase[uiLoop];
	VXB_PCI_BUS_CFG_READ(pDev,PCI_CFG_SUB_SYSTEM_ID,2,usVal);
	AHCI_LOG(AHCI_DBG_ERR,"phy %d ahci register addr 0x%x\n",uiPhy,
	         (UINT32)pstDrv->stPci.pucVirAddr,0,0,0,0);
    VXB_PCI_BUS_CFG_READ (pDev,PCI_CFG_STATUS,2,usVal);
    VXB_PCI_BUS_CFG_WRITE (pDev,PCI_CFG_STATUS,2,usVal);
    VXB_PCI_BUS_CFG_READ (pDev,PCI_CFG_COMMAND,2,usVal);
    usVal = usVal | PCI_CMD_MEM_ENABLE | PCI_CMD_MASTER_ENABLE;
    VXB_PCI_BUS_CFG_WRITE (pDev,PCI_CFG_COMMAND,2,usVal);
}

VOID ahciHexDump(UINT8 *pbuf, UINT32 len)
{
    UINT32 i;
    
    for (i = 0; i < len; )
    {
        if (len - i >= 4)
        {
          logMsg("%x %x %x %x \n",pbuf[i],pbuf[i+1],pbuf[i+2],pbuf[i+3],0,0);
          i += 4;
        }
        else
        {
          logMsg("%x \n",pbuf[i],0,0,0,0,0);
          i++;
        }
    }

    logMsg("\n",0,0,0,0,0,0);
}


void ahciDumpReq(AHCI_DRV_S *pstDrv)
{
	UINT32 uiLoop = 0;
	
	for(uiLoop = 0;uiLoop < AHCI_MAX_REQ;uiLoop++)
    {	
		if(0 != (pstDrv->uiCmndTblFlag & (1 << uiLoop)))
		{
			ahciHexDump(&pstDrv->astReq[uiLoop].stFis,sizeof(SAS_HOST_TO_DEV_S));
			logMsg("sgl hi 0x%x lo 0x%x len %d slot %d\n",pstDrv->astReq[uiLoop].stSGL.uiSGLHi,
				    pstDrv->astReq[uiLoop].stSGL.uiSGLLo,
				    pstDrv->astReq[uiLoop].stSGL.uiLen,uiLoop,0,0);
		}
    }
}

/*****************************************************************************
*
* ahciIntr - interrupt handle
*
* RETURNS: N/A
*
*****************************************************************************/
#if 1
LOCAL void ahciIntr(UINT32  uiPhyNum)
{
	AHCI_PORT_S    *pstPort = NULL;
	AHCI_DRV_S     *pstDrv = NULL;
	UINT8          *pucAddr = NULL;
	UINT8          *pucPortAddr = NULL;
	UINT32          uiLoop = 0;
	UINT32          uiSataIntr = 0;
	UINT32          uiPortIntr = 0;
	UINT32          uiCtrlIntr = 0;
	UINT32          uiTaskStatus = 0;
	UINT32          uiSataStatus = 0;
	UINT32          uiCmdActive = 0;
	UINT32          uiSataActive = 0;
	UINT32          uiVal = 0;
	UINT32          uiQCActive = 0;
	UINT32          uiDoneMask = 0;

	assert(uiPhyNum < AHCI_MAX_PHY);
	pstDrv = &gastAhciDrv[uiPhyNum];
	pstPort = &pstDrv->stPort;
	pucAddr = pstDrv->stPci.pucVirAddr;
	SPIN_LOCK_ISR_TAKE(&pstDrv->stLock);
	uiCtrlIntr = AHCI_READ_REG(pstDrv->stPci.pucVirAddr + AHCI_IS);
	pucPortAddr = pstDrv->stPci.pucVirAddr + AHCI_PORT_OFFSET(0);
	uiSataIntr = AHCI_READ_REG(pucPortAddr + AHCI_PxSERR);
    uiPortIntr = AHCI_READ_REG(pucPortAddr + AHCI_PxIS);
    AHCI_WRITE_REG(pucPortAddr + AHCI_PxSERR,uiSataIntr);
    AHCI_WRITE_REG(pucPortAddr + AHCI_PxIS, uiPortIntr);
    AHCI_WRITE_REG(pstDrv->stPci.pucVirAddr + AHCI_IS, uiCtrlIntr);
    uiTaskStatus = AHCI_READ_REG(pucPortAddr + AHCI_PxTFD);
    uiSataStatus = AHCI_READ_REG(pucPortAddr + AHCI_PxSSTS);
    uiCmdActive = AHCI_READ_REG(pucPortAddr + AHCI_PxCI);
    uiSataActive = AHCI_READ_REG(pucPortAddr + AHCI_PxSACT);
    AHCI_LOG(AHCI_DBG_DATA,"ahci uiSataIntr 0x%x uiPortIntr 0x%x uiTaskStatus"
             " 0x%x uiSataStatus 0x%x uiCmdActive 0x%x uiSataActive 0x%x\n",
             uiSataIntr,uiPortIntr,uiTaskStatus,uiSataStatus,uiCmdActive,
             uiSataActive);
    if(uiPortIntr & PORT_IRQ_ERROR)
    {
    	uiVal = AHCI_READ_REG(pucPortAddr + PORT_SCR_ERR);
    	AHCI_WRITE_REG(pucPortAddr + PORT_SCR_ERR, uiVal);
		AHCI_LOG(AHCI_DBG_ERR,"phy %d intr error 0x%x port err 0x%x\n",
		         uiPhyNum,uiPortIntr,uiVal,0,0,0);
    }
	if(pstDrv->uiCmndTblFlag & pstDrv->uiActiveNCQ)
	{
		uiQCActive = uiSataActive;
	}
	else
	{
		uiQCActive = uiCmdActive;
	}
	uiDoneMask = pstDrv->uiCmndTblFlag ^ uiQCActive;
	if (uiDoneMask & uiQCActive) 
	{
		AHCI_LOG(AHCI_DBG_ERR,"phy %d illegal qc_active transition (%08x->%08x)\n",
		         uiPhyNum,pstDrv->uiCmndTblFlag,uiQCActive,0,0,0);
	}
  	SPIN_LOCK_ISR_GIVE(&pstDrv->stLock);
    for(uiLoop = 0;uiLoop < AHCI_MAX_REQ;uiLoop++)
    {	
		if(0 != (uiDoneMask & (1 << uiLoop)))
		{
			AHCI_LOG(AHCI_DBG_DATA,"req %p %d done ok\n",&pstDrv->astReq[uiLoop],
				     pstDrv->uiReqNum,0,0,0,0);
			pstDrv->astReq[uiLoop].pfDone(&pstDrv->astReq[uiLoop]);
		}
    }

}
#else
LOCAL void ahciIntr(UINT32  uiPhyNum)
{
	AHCI_PORT_S    *pstPort = NULL;
	AHCI_DRV_S     *pstDrv = NULL;
	UINT8          *pucAddr = NULL;
	UINT8          *pucPortAddr = NULL;
	UINT32          uiLoop = 0;
	UINT32          uiSataIntr = 0;
	UINT32          uiPortIntr = 0;
	UINT32          uiCtrlIntr = 0;
	UINT32          uiTaskStatus = 0;
	UINT32          uiSataStatus = 0;
	UINT32          uiCmdActive = 0;
	UINT32          uiSataActive = 0;
	UINT32          uiVal = 0;
    FAST int        oldlevel;
   
	oldlevel = intLock ();
	assert(uiPhyNum < AHCI_MAX_PHY);
	pstDrv = &gastAhciDrv[uiPhyNum];
	pstPort = &pstDrv->stPort;
	pucAddr = pstDrv->stPci.pucVirAddr;
	uiCtrlIntr = AHCI_READ_REG(pstDrv->stPci.pucVirAddr + AHCI_IS);
	pucPortAddr = pstDrv->stPci.pucVirAddr + AHCI_PORT_OFFSET(0);
	uiSataIntr = AHCI_READ_REG(pucPortAddr + AHCI_PxSERR);
    uiPortIntr = AHCI_READ_REG(pucPortAddr + AHCI_PxIS);
    AHCI_WRITE_REG(pucPortAddr + AHCI_PxSERR,uiSataIntr);
    AHCI_WRITE_REG(pucPortAddr + AHCI_PxIS, uiPortIntr);
    AHCI_WRITE_REG(pstDrv->stPci.pucVirAddr + AHCI_IS, uiCtrlIntr);
    uiTaskStatus = AHCI_READ_REG(pucPortAddr + AHCI_PxTFD);
    uiSataStatus = AHCI_READ_REG(pucPortAddr + AHCI_PxSSTS);
    uiCmdActive = AHCI_READ_REG(pucPortAddr + AHCI_PxCI);
    uiSataActive = AHCI_READ_REG(pucPortAddr + AHCI_PxSACT);
    AHCI_LOG(AHCI_DBG_DATA,"ahci uiSataIntr 0x%x uiPortIntr 0x%x uiTaskStatus"
             " 0x%x uiSataStatus 0x%x uiCmdActive 0x%x uiSataActive 0x%x\n",
             uiSataIntr,uiPortIntr,uiTaskStatus,uiSataStatus,uiCmdActive,
             uiSataActive);
    if(uiPortIntr & PORT_IRQ_ERROR)
    {
    	uiVal = AHCI_READ_REG(pucPortAddr + PORT_SCR_ERR);
    	AHCI_WRITE_REG(pucPortAddr + PORT_SCR_ERR, uiVal);
		AHCI_LOG(AHCI_DBG_ERR,"phy %d intr error 0x%x port err 0x%x\n",
		         uiPhyNum,uiPortIntr,uiVal,0,0,0);
		ahciDumpReq(pstDrv);
		intUnlock(oldlevel);
		return ;
    }
    for(uiLoop = 0;uiLoop < AHCI_MAX_REQ;uiLoop++)
    {	
		if((0 != (pstDrv->uiCmndTblFlag & (1 << uiLoop))) &&
		   (0 == (uiCmdActive & (1 << uiLoop))) &&
		   (0 == (uiSataActive & (1 << uiLoop))))
		{
			pstDrv->astReq[uiLoop].pfDone(&pstDrv->astReq[uiLoop]);
		}
    }
    intUnlock(oldlevel);
}


#endif
STATUS ahciPcieRegInt(AHCI_PCI_S *pstPci,VOID *pfFunc,VOID  *pvParm)
{
#if 1    
	struct vxbIntDynaVecInfo stDynaVec;
	VXB_DEVICE_ID pstVxbDev = NULL;
	
	memset(&stDynaVec,0,sizeof(struct vxbIntDynaVecInfo));
	stDynaVec.isr = pfFunc;
	stDynaVec.pArg = pvParm;
	pstVxbDev = (VXB_DEVICE_ID)pstPci->pvUpLayer;
	
	if(OK != vxbMsiConnect(pstVxbDev,1,&stDynaVec))
	{
		AHCI_LOG(AHCI_DBG_ERR,"connect msi failed pstVxbDev 0x%x\n",pstVxbDev,
		         0,0,0,0,0);
	}

	if(OK != vxbPciMSIEnable(pstVxbDev,&stDynaVec))
	{
        AHCI_LOG(AHCI_DBG_ERR,"program msi failed\n",0,0,0,0,0,0);
	}
	
#else
    pciIntConnect ((VOIDFUNCPTR *)0,(VOIDFUNCPTR)pfFunc,pvParm);
    intEnable(0);
#endif

    return OK;
}

/*****************************************************************************
*
* vxbIntelAhciInstConnect - Final initialization
*
* RETURNS: N/A
*****************************************************************************/
LOCAL void vxbAhciInstConnect(VXB_DEVICE_ID pDev)
{
	AHCI_DRV_S  *pstDrv = NULL;
	UINT32       uiPhyNum = 0;

	pstDrv = (AHCI_DRV_S*)pDev->pDrvCtrl;
	pstDrv->stPci.pvUpLayer = pDev;
	if(OK != ahciDrvInit(pstDrv->uiPhyId))
	{
		return;
	}
	ahciPcieRegInt(&pstDrv->stPci,ahciIntr,(VOID*)pstDrv->uiPhyId);
	ahciEnableInt(pstDrv);
	ahciSendIdentify(pstDrv);
}

void ahciVxbRegister(void)
{
	blkInit();
	blkRegFunc(ahciRWSector,ahciFormat);
	vxbDevRegister((struct vxbDevRegInfo *)&gstAhciVxbDevRegistration);
}


