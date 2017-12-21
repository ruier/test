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
#include <hwif/vxbus/vxBus.h>		
#include <vxBusLib.h>			
#include <hwif/vxbus/vxBus.h>
#include <hwif/util/hwMemLib.h>		
#include <hwif/vxbus/hwConf.h>	
#include <hwif/vxbus/vxbPciLib.h>
#include <driverControl.h>
#include "fslSataInc.h"
 
 /*----------------------------------------------*
  * external  variable							 *
  *----------------------------------------------*/
 
 
 /*----------------------------------------------*
  * external  function							 *
  *----------------------------------------------*/

 extern STATUS blkInit(VOID);
 extern STATUS fsataRWSector(BLK_REQ_S  *pstBlkReq);
 extern VOID fsataCloseWrCache(FSATA_DRV_S *pstDrv,UINT32 uiPort);
 extern STATUS fsataSendIdentify(FSATA_DRV_S *pstDrv,UINT32 uiPort);
 /*----------------------------------------------*
  * internal  function							 *
  *----------------------------------------------*/
 
 /*----------------------------------------------*
  * global	variable							 *
  *----------------------------------------------*/
 UINT32      guiFSataDbgLevel = FSATA_DBG_ERR;
 FSATA_DRV_S gstFSataDrv;
 
 /*----------------------------------------------*
  * constant variable							 *
  *----------------------------------------------*/

 /*----------------------------------------------*
  * macro definition							 *
  *----------------------------------------------*/
 
 /*****************************************************************************
  function	   : fsataDMAAlloc
  description  : allocate dma memory
 *****************************************************************************/
 STATUS fsataDMAAlloc(FSATA_MEM_S *pstMem)
 {
	 UINT32 	  uiAllocLen = 0;
	 UINT32 	  uiAlignLen = 0;
	 
	 if(0 != pstMem->uiAlign)
	 {
		 uiAlignLen = pstMem->uiAlign - 1;
	 }
	 
	 uiAllocLen = pstMem->uiTotalLen + pstMem->uiAlign;
	 pstMem->pvAllocVirPtr = (UINT8*)cacheDmaMalloc(uiAllocLen);
	 if(NULL == pstMem->pvAllocVirPtr)
	 {
		 FSATA_LOG(FSATA_DBG_ERR,"alloc memory %d failed\n",uiAllocLen,0,0,0);
		 return ERROR;
	 }
	 memset(pstMem->pvAllocVirPtr,0,uiAllocLen);
	 uiAlignLen = ((UINT32)(pstMem->pvAllocVirPtr) + uiAlignLen) & (~uiAlignLen);
	 uiAlignLen -= (UINT32)(pstMem->pvAllocVirPtr);
	 pstMem->pvVirPtr = (VOID*)((UINT32)pstMem->pvAllocVirPtr + uiAlignLen);
	 pstMem->uiPhyAddrLo = (UINT32)(pstMem->pvVirPtr);
	 pstMem->uiPhyAddrHi = 0;
 
	 FSATA_LOG(FSATA_DBG_INFO,"alloc len %d alloc %x phy 0x%x vir 0x%x\n",
			 uiAllocLen,(ULONG)(pstMem->pvAllocVirPtr),pstMem->uiPhyAddrLo,
			 (ULONG)(pstMem->pvVirPtr));
	 
	 return OK;
 }
 
 /*****************************************************************************
  function	   : fsataDMAFree
  description  : allocate dma memory
 *****************************************************************************/
 VOID fsataDMAFree(FSATA_MEM_S *pstMem)
 {
	 cacheDmaFree(pstMem->pvAllocVirPtr);
 }

 STATUS fsataIsr(UINT32  uiPhyNum)
 {
	FSATA_PORT_S    *pstPort = NULL;
	UINT32           uiHStatus = 0;
	UINT32           uiSsrError = 0;
	UINT32           uiDoneMask = 0;
	UINT32           uiSsrOffset = 0;
	UINT32           uiCmdDone = 0;
	UINT32           uiLoop = 0;
	UINT32           uiVal = 0;
	UINT32           uiTmp = 0;

	assert(uiPhyNum < FSATA_MAX_PORT);
	pstPort = &gstFSataDrv.astPort[uiPhyNum];
	SPIN_LOCK_ISR_TAKE(&pstPort->stLock);
	uiHStatus = FSATA_READ_REG(pstPort->pvRegBase + FSATA_COMMAND_HSTATUS);
	
	uiSsrOffset = pstPort->uiSsrAdrsOff + FSATA_SUPERSET_SERROR;
	uiSsrError = FSATA_READ_REG(pstPort->pvRegBase + uiSsrOffset);
	uiDoneMask = FSATA_READ_REG(pstPort->pvRegBase + FSATA_COMMAND_CCR);
	FSATA_WRITE_REG(pstPort->pvRegBase + FSATA_COMMAND_CCR,uiDoneMask);
	if(uiHStatus & FSATA_INT_ON_PHYRDY_CHG)
	{
		FSATA_WRITE_REG(pstPort->pvRegBase + uiSsrOffset,uiSsrError);
		semGive(pstPort->semMux);
	}
	
	if((uiHStatus & FSATA_INT_ON_ERROR) || (uiSsrError & 0xFFFF0000))
	{
		FSATA_LOG(FSATA_DBG_ERR,"port %d interrupt error uiHStatus 0x%x "
			      "uiScrError 0x%x\n",uiPhyNum,uiHStatus,uiSsrError,0);
		FSATA_WRITE_REG(pstPort->pvRegBase + uiSsrOffset,uiSsrError);
		if (uiSsrError & 0xFFFF0000)
        {      
			FSATA_WRITE_REG(pstPort->pvRegBase + uiSsrOffset,uiSsrError);
        }
        uiVal = FSATA_READ_REG(pstPort->pvRegBase + FSATA_COMMAND_CER);
        if(uiVal)
        {
        	uiTmp = FSATA_READ_REG(pstPort->pvRegBase + FSATA_COMMAND_DER);
			FSATA_WRITE_REG(pstPort->pvRegBase + FSATA_COMMAND_DER,uiTmp);
			FSATA_WRITE_REG(pstPort->pvRegBase + FSATA_COMMAND_DER,uiVal);
        }
		FSATA_WRITE_REG(pstPort->pvRegBase + FSATA_COMMAND_HSTATUS,uiHStatus);
		SPIN_LOCK_ISR_GIVE(&pstPort->stLock);
		return ERROR;
	}
	FSATA_WRITE_REG(pstPort->pvRegBase + FSATA_COMMAND_HSTATUS,uiHStatus);
	SPIN_LOCK_ISR_GIVE(&pstPort->stLock);
	
	FSATA_LOG(FSATA_DBG_INFO,"port %d interrupt active 0x%x donemask 0x%x\n",
		      uiPhyNum,pstPort->uiCmndActive,uiDoneMask,0);
	
	
	if(uiDoneMask & (1))/*REQ 0 FOR NON NCQ COMMAND*/
	{
		pstPort->astReq[0].pfDone(&pstPort->astReq[0]);
		pstPort->uiCmndActive &= (~(1 << 0));
	}
	else
	{
		for(uiLoop = 1;uiLoop < FSATA_MAX_REQ;uiLoop++)
		{
			if(0 != (uiDoneMask & (1 << uiLoop)))
			{
				if(0 == (pstPort->uiCmndActive & (1 << uiLoop)))
				{
					FSATA_LOG(FSATA_DBG_ERR,"port %d interrupt active 0x%x done"
						      "mask 0x%x error\n",uiPhyNum,pstPort->uiCmndActive,
						      uiDoneMask,0);
					continue;
				}
				pstPort->uiCmndActive &= (~(1 << uiLoop));
				pstPort->astReq[uiLoop].pfDone(&pstPort->astReq[uiLoop]);
			}
		}
	}

	return OK;
 }

STATUS fsataLineStsWait(FSATA_PORT_S *pstPort,UINT32 uiMask,UINT32 uiExpect,
                        UINT32 uiTimeoutTicks)
{
    UINT32          uiVal = 0;
    UINT32          uiLoop = 0;
    
    for(uiLoop = 0; uiLoop < uiTimeoutTicks; uiLoop++)
    {
        uiVal = FSATA_READ_REG(pstPort->pvRegBase + FSATA_COMMAND_HSTATUS);
        /* return from taskDelay is not needed */
        if((uiVal & uiMask) != uiExpect)
        {
			taskDelay(1);
		}
        else
        {
            break;
        }
    }
    if(uiLoop >= uiTimeoutTicks)
    {
		return ERROR;
	}

	return OK;
}

STATUS fsataWaitPhyReady(FSATA_PORT_S *pstPort,UINT32 uiTimeoutTicks)
{
    UINT32          uiVal = 0;
    UINT32          uiLoop = 0;
    
    for(uiLoop = 0; uiLoop < uiTimeoutTicks; uiLoop++)
    {
        uiVal = FSATA_READ_REG(pstPort->pvRegBase + FSATA_COMMAND_SSTATUS);
        /* return from taskDelay is not needed */
        if((uiVal & FSATA_SSTATUS_RDY) == FSATA_SSTATUS_RDY)
        {
			switch(uiVal & FSATA_SSTATUS_SPEED_MASK)
			{
				case FSATA_SSTATUS_SPEED_15:
					FSATA_LOG(FSATA_DBG_ERR,"port %d phy link up 1.5G\n",
						                              pstPort->uiPortId,0,0,0);
					break;
				case FSATA_SSTATUS_SPEED_30:
					FSATA_LOG(FSATA_DBG_ERR,"port %d phy link up 3.0G\n",
						                              pstPort->uiPortId,0,0,0);
					break;
				default:
					FSATA_LOG(FSATA_DBG_ERR,"port %d phy link unknown speed 0x%x\n",
						                              pstPort->uiPortId,uiVal,0,0);
					break;
			}
			break;
		}
    }
    if(uiLoop >= uiTimeoutTicks)
    {
		return ERROR;
	}

	return OK;
}

/*****************************************************************************
 function     : fsataDrvInit
 description  : driver intiazation and chip intiazation
*****************************************************************************/
 VOID  fsataPortInit(UINT32 uiPortNum)
 {
 	FSATA_PORT_S    *pstPort = NULL;
	UINT32           uiVal = 0;

 	assert(uiPortNum < FSATA_MAX_PORT);
	pstPort = &gstFSataDrv.astPort[uiPortNum];
	pstPort->pstDrv = &gstFSataDrv;
	pstPort->uiPortId = uiPortNum;
	pstPort->uiSsrAdrsOff = FSATA_SUPSETREG_OFFSET;
	pstPort->uiCsrAdrsOff = FSATA_CONSTSREG_OFFSET;
	pstPort->uiScrAdrsOff = FSATA_SYSREG_OFFSET;
	pstPort->pvRegBase = CCSBAR + FSATA_BASE + (uiPortNum * 0x1000);

#if 1

	uiVal = FSATA_READ_REG(pstPort->pvRegBase + FSATA_COMMAND_HSTATUS);
	FSATA_WRITE_REG(pstPort->pvRegBase + FSATA_COMMAND_HSTATUS,(uiVal & 0x3F));


	/* Disable interrupts  on the controller */
    uiVal = FSATA_READ_REG(pstPort->pvRegBase + FSATA_COMMAND_HCONTROL);
    FSATA_WRITE_REG(pstPort->pvRegBase + FSATA_COMMAND_HCONTROL,(uiVal & ~0x3F));

    /* Disable interrupt coalescing control(icc) */
    uiVal = FSATA_READ_REG(pstPort->pvRegBase + FSATA_COMMAND_ICC);
    FSATA_WRITE_REG(pstPort->pvRegBase + FSATA_COMMAND_ICC, 0x01000000);

    /* Clear error registers*/
    FSATA_WRITE_REG(pstPort->pvRegBase + FSATA_COMMAND_CER, 0x00000FFFF);
    FSATA_WRITE_REG(pstPort->pvRegBase + FSATA_COMMAND_DER, 0x00000FFFF);

    /* Assuming no Port multiplier, set CQPMP to 0 */
    FSATA_WRITE_REG(pstPort->pvRegBase + FSATA_COMMAND_CQPMP, 0x0);
	
    /* force host controller to go off-line */
    uiVal = FSATA_READ_REG(pstPort->pvRegBase + FSATA_COMMAND_HCONTROL);
    uiVal &= ~FSATA_HCONTROL_ONLINE_PHY_RST;
    FSATA_WRITE_REG(pstPort->pvRegBase + FSATA_COMMAND_HCONTROL, uiVal);
    /* return from taskDelay is not needed */
    (void)taskDelay(4 * sysClkRateGet() / 5);
	if(OK != fsataLineStsWait(pstPort,(UINT32)FSATA_HSTATUS_ONLINE,0,1000))
    {
        FSATA_LOG(FSATA_DBG_INFO,"wait port %d offline timeout\n",uiPortNum,0,0,0);
        return;
    }
#endif

	FSATA_LOG(FSATA_DBG_INFO,"set port %d offine ok\n",uiPortNum,0,0,0);
	/* Bring the host controller online again */
    uiVal = FSATA_READ_REG(pstPort->pvRegBase + FSATA_COMMAND_HCONTROL);
    uiVal |= (FSATA_HCONTROL_ONLINE_PHY_RST | FSATA_HCONTROL_SNOOP_ENABLE);
    FSATA_WRITE_REG(pstPort->pvRegBase + FSATA_COMMAND_HCONTROL, uiVal);

    /* return from taskDelay is not needed */
    (void)taskDelay(4 * sysClkRateGet() / 5);

	if(OK != fsataLineStsWait(pstPort,(UINT32)FSATA_HSTATUS_ONLINE,
		                      (UINT32)FSATA_HSTATUS_ONLINE,1000))
    {
        FSATA_LOG(FSATA_DBG_INFO,"wait port %d online timeout\n",uiPortNum,0,0,0);
        return;
    }
	FSATA_LOG(FSATA_DBG_INFO,"set port %d online ok\n",uiPortNum,0,0,0);
	if(OK != fsataWaitPhyReady(pstPort,1000))
    {
        FSATA_LOG(FSATA_DBG_ERR,"wait port %d ready timeout\n",uiPortNum,0,0,0);
        return;
    }

	pstPort->stMem.uiAlign = 16;
	pstPort->stMem.uiTotalLen = FSATA_PORT_PRIV_DMA_SZ;
	if(OK != fsataDMAAlloc(&pstPort->stMem))
	{
		FSATA_LOG(FSATA_DBG_INFO,"malloc port %d memory failed\n",uiPortNum,0,0,0);
		return;
	}
	pstPort->pstCmdslot = (FSATA_CMDHDR_S*)pstPort->stMem.pvVirPtr;
	pstPort->uiCmdSlotPhyAddr = pstPort->stMem.uiPhyAddrLo;
	pstPort->pstCmdDentry = (FSATA_COMMAND_DESC_S*)((ULONG)pstPort->stMem.pvVirPtr
		                    + FSATA_CMD_SLOT_SIZE);
	pstPort->uiCmdDentryPhyAddr = (FSATA_COMMAND_DESC_S*)(pstPort->stMem.uiPhyAddrLo
		                    + FSATA_CMD_SLOT_SIZE);
	
    /* update the CHBA register */
    FSATA_WRITE_REG(pstPort->pvRegBase + FSATA_COMMAND_CHBA, pstPort->uiCmdSlotPhyAddr);
	
    /* Enable interrupts*/
    uiVal = FSATA_READ_REG(pstPort->pvRegBase + FSATA_COMMAND_HCONTROL);
    FSATA_WRITE_REG(pstPort->pvRegBase + FSATA_COMMAND_HCONTROL,
                (uiVal | FSATA_HCONTROL_DEFAULT_PORT_IRQ_ENABLE_MASK));
    uiVal = FSATA_READ_REG(pstPort->pvRegBase + pstPort->uiSsrAdrsOff + FSATA_SUPERSET_SERROR);
    FSATA_WRITE_REG(pstPort->pvRegBase + pstPort->uiSsrAdrsOff + FSATA_SUPERSET_SERROR, uiVal);
	pstPort->semMux = semBCreate(SEM_Q_PRIORITY,SEM_EMPTY);
    if(NULL == pstPort->semMux)
    {
        FSATA_LOG(FSATA_DBG_ERR,"port %d create semphore failed\n",uiPortNum,0,0,0);
        return;
    }
	SPIN_LOCK_ISR_INIT(&pstPort->stLock, 0);
	if(0 == uiPortNum)
	{
		intConnect ((VOIDFUNCPTR *)EPIC_SATA1_INT_VEC,
                    (VOIDFUNCPTR)fsataIsr, uiPortNum);
		intEnable(EPIC_SATA1_INT_VEC);
	}
	else
	{
		intConnect ((VOIDFUNCPTR *)EPIC_SATA2_INT_VEC,
                    (VOIDFUNCPTR)fsataIsr, uiPortNum);
		intEnable(EPIC_SATA2_INT_VEC);
	}
	
	if(semTake(pstPort->semMux,5 * sysClkRateGet()))
	{
		FSATA_LOG(FSATA_DBG_ERR,"wait port %d device rdy failed\n",uiPortNum,0,0,0);
	}
	fsataSendIdentify(pstPort->pstDrv,uiPortNum);
	if(semTake(pstPort->semMux,5 * sysClkRateGet()))
	{
		FSATA_LOG(FSATA_DBG_ERR,"wait port %d identify failed\n",uiPortNum,0,0,0);
	}
 } 

 VOID fsataDrvInit(VOID)
 {
 	FSATA_LOG(FSATA_DBG_ERR,"sata driver 0x%x version %s\n",&gstFSataDrv,
		      FSATA_VERSION,0,0);
 	memset(&gstFSataDrv,0,sizeof(FSATA_DRV_S));
	blkInit();
	blkRegFunc(fsataRWSector,NULL);
	fsataPortInit(0);
	fsataPortInit(1);
 }

