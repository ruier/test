/******************************************************************************

                  VERSION (C), 2001-2011, 

 ******************************************************************************
  file       : fsatareq.c
  version    : 1.0
  data       : 2016.3.25
  function   : fsl sata request process
******************************************************************************/

/*----------------------------------------------*
 * header file                                  *
 *----------------------------------------------*/
#include "fslSataInc.h"

/*----------------------------------------------*
 * external  variable                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external  function                           *
 *----------------------------------------------*/
extern STATUS blkDevAdd(UINT64_S vullSec,UINT32  vuiBytePerSec,UINT32 vuiPhyId);

/*----------------------------------------------*
 * internal  function                           *
 *----------------------------------------------*/
STATUS fsataRWSector(BLK_REQ_S  *pstBlkReq);
/*----------------------------------------------*
 * global  variable                             *
 *----------------------------------------------*/

/*****************************************************************************
* fsataHexDump - hex dump
*****************************************************************************/
VOID fsataHexDump(CHAR *pstName,UINT8 *pbuf, UINT32 len)
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
 function     : fsataGetReq
 description  : get free request
 input        : 
 output       : 
 return       : NULL or REQ 
*****************************************************************************/
FSATA_REQ_S* fsataGetReq(FSATA_PORT_S *pstPort)
{   
    UINT32          uiLoop = 0;

	/*REQ 0 is used for NON NCQ COMMAND*/
    SPIN_LOCK_ISR_TAKE(&pstPort->stLock);
    for(uiLoop = 1;uiLoop < FSATA_MAX_REQ;uiLoop++)
    {
        if(FSATA_REQ_UNUSED == pstPort->astReq[uiLoop].uiStatus)
        {
            pstPort->astReq[uiLoop].uiStatus = FSATA_REQ_USED;
			pstPort->astReq[uiLoop].uiTag = (UINT8)uiLoop;
			SPIN_LOCK_ISR_GIVE(&pstPort->stLock);
			FSATA_LOG(FSATA_DBG_DATA,"get req tag %d\n",uiLoop,0,0,0);
            return (&(pstPort->astReq[uiLoop]));
        }
    }
	SPIN_LOCK_ISR_GIVE(&pstPort->stLock);

    return NULL;  
}

/*****************************************************************************
 function     : fsataFreeReq
 description  : free request
 input        : 
 output       : 
 return       : NULL or REQ 
*****************************************************************************/
VOID fsataFreeReq(FSATA_PORT_S *pstPort,UINT8 ucTag)
{
	assert(ucTag < FSATA_MAX_REQ);
    if(FSATA_REQ_UNUSED == pstPort->astReq[ucTag].uiStatus)
    {
        FSATA_LOG(FSATA_DBG_ERR,"phy %d request %d has been freed\n",
			      pstPort->uiPortId,ucTag,0,0);
        return;
    }
	SPIN_LOCK_ISR_TAKE(&pstPort->stLock);
    pstPort->astReq[ucTag].uiStatus = FSATA_REQ_UNUSED;
    pstPort->astReq[ucTag].pucBuf = NULL;
    pstPort->astReq[ucTag].pstBlkReq = NULL;
	pstPort->astReq[ucTag].pfDone = NULL;
	SPIN_LOCK_ISR_GIVE(&pstPort->stLock);
    FSATA_LOG(FSATA_DBG_DATA,"phy %d free tag %d\n",pstPort->uiPortId,ucTag,0,0);
}

/*****************************************************************************
 function     : fsataConvertIdentify
 description  : convert identify data to cpu endian
 input        : 
 output       : 
 return       : 
*****************************************************************************/
VOID fsataConvertIdentify(FSATA_ID_DATA_S *vpstIden)
{
    UINT16  *pusBuf = NULL;
    UINT32   uiLoop = 0;

    pusBuf = (UINT16*)vpstIden;
    for(uiLoop = 0;uiLoop < (sizeof(FSATA_ID_DATA_S)/2);uiLoop++)
    {
        *pusBuf = FSATA_CPU_LE16(*pusBuf);
        pusBuf++;
    }
}

/*****************************************************************************
 function     : fsataIdentifyDone
 description  : freescale sata identify done
 input        : 
 output       : write data to LBA
 return       : 
*****************************************************************************/
VOID fsataIdentifyDone(VOID  *pstReq)
{
    FSATA_ID_DATA_S      *pstIdDevData = NULL;
    FSATA_DRV_S          *pstDrv = NULL;
	FSATA_PORT_S         *pstPort = NULL;
    UINT32                uiLBALo = 0;
    UINT64_S              ullLBA;

    pstIdDevData = (FSATA_ID_DATA_S*)(((FSATA_REQ_S*)pstReq)->pucBuf);
	CACHE_DMA_INVALIDATE(pstIdDevData,512);
    fsataConvertIdentify(pstIdDevData);
    pstPort = (FSATA_PORT_S*)(((FSATA_REQ_S*)pstReq)->pvPort);
    pstPort->uiNCQMaxIO = (UINT32)pstIdDevData->usQueueDepth;
    pstPort->uiNCQSupport = FALSE;
    if(pstIdDevData->usCapabilities & 0x100)
    {
        pstPort->uiNCQSupport = TRUE;
    }

    pstPort->ui48BitLBA = FALSE;
    if((pstIdDevData->usCmndSetSupported1 & 0x400) &&
       (pstIdDevData->usCmndSetFeaEnabled1 & 0x400))
    {
        pstPort->ui48BitLBA = TRUE;
    }

    pstPort->uiRemovable = FALSE;
    if(pstIdDevData->usCmndSetSupported & 0x4)
    {
        pstPort->uiRemovable = TRUE;
    }
    pstPort->uiRemoveEnable = FALSE;
    if(pstIdDevData->usCmndSetFeaEnabled & 0x4)
    {
        pstPort->uiRemoveEnable = TRUE;
    }

    pstPort->uiWrCacheEnable = FALSE;
    if(pstIdDevData->usCmndSetFeaEnabled & 0x20)
    {
        pstPort->uiWrCacheEnable = TRUE;
    }
    
    pstPort->uiDMASupport = FALSE;
    if(pstIdDevData->usDmaLbaIodTimer & 0x100)
    {
        pstPort->uiDMASupport = TRUE;
    }

    pstPort->uiDMAEnable = FALSE;
    if(pstIdDevData->usUltraDMAModes & 0x7f7f)
    {
        pstPort->uiDMAEnable = TRUE;
    }

    pstPort->uiMaxAddrSectors = pstIdDevData->usUserSecHi << 16;
    pstPort->uiMaxAddrSectors |= pstIdDevData->usUserSecLo;

    if(TRUE == pstPort->ui48BitLBA)
    {
        pstPort->ucCapacity[0] = (UINT8)(pstIdDevData->usMaxLBA48 >> 8);
        pstPort->ucCapacity[1] = (UINT8)(pstIdDevData->usMaxLBA48 & 0xff);
        pstPort->ucCapacity[2] = (UINT8)(pstIdDevData->usMaxLBA32 >> 8);
        pstPort->ucCapacity[3] = (UINT8)(pstIdDevData->usMaxLBA32 & 0xff);
        uiLBALo = pstIdDevData->usMaxLBA16 << 16;
        uiLBALo |= pstIdDevData->usMaxLBA0;
        /*uiLBALo = uiLBALo - 1;*/
        pstPort->ucCapacity[4] = (UINT8)((uiLBALo >> 24) & 0xFF);
        pstPort->ucCapacity[5] = (UINT8)((uiLBALo >> 16) & 0xFF);
        pstPort->ucCapacity[6] = (UINT8)((uiLBALo >> 8)  & 0xFF);
        pstPort->ucCapacity[7] = (UINT8)((uiLBALo )      & 0xFF);
    }
    else
    {
        pstPort->ucCapacity[0] = 0;
        pstPort->ucCapacity[1] = 0;
        pstPort->ucCapacity[2] = 0;
        pstPort->ucCapacity[3] = 0;
        uiLBALo = pstIdDevData->usUserSecHi << 16;
        uiLBALo |= pstIdDevData->usUserSecLo;
        uiLBALo = uiLBALo - 1;
        pstPort->ucCapacity[4] = (UINT8)((uiLBALo >> 24) & 0xFF);
        pstPort->ucCapacity[5] = (UINT8)((uiLBALo >> 16) & 0xFF);
        pstPort->ucCapacity[6] = (UINT8)((uiLBALo >> 8)  & 0xFF);
        pstPort->ucCapacity[7] = (UINT8)((uiLBALo )      & 0xFF); 
    }

    /*
     *word106 bit12:device logical sector longer than 256 words
     *        bit3~0:2^x logical sectors per physical sector
     *word 117~118:words per logical sector
    */
    if (pstIdDevData->usWord104[2] & 0x1000)
    {
        FSATA_LOG(FSATA_DBG_INFO,"disk %d logical sector loger than 256 words",
                pstPort->uiPortId,0,0,0);
    }
    fsataFreeReq(pstPort,((FSATA_REQ_S*)pstReq)->uiTag);
	pstPort->uiStatus = FSATA_LINK_UP;
    ullLBA.hi  = ((UINT32)(pstPort->ucCapacity[0])) << 24;
    ullLBA.hi |= ((UINT32)(pstPort->ucCapacity[1])) << 16;
    ullLBA.hi |= ((UINT32)(pstPort->ucCapacity[2])) << 8;
    ullLBA.hi |= ((UINT32)(pstPort->ucCapacity[3])) << 0;
    ullLBA.lo = ((UINT32)(pstPort->ucCapacity[4])) << 24;
    ullLBA.lo |= ((UINT32)(pstPort->ucCapacity[5])) << 16;
    ullLBA.lo |= ((UINT32)(pstPort->ucCapacity[6])) << 8;
    ullLBA.lo |= ((UINT32)(pstPort->ucCapacity[7]));
	FSATA_LOG(FSATA_DBG_DATA,"disk %d contain 0x%x sectors\n",
             pstPort->uiPortId,ullLBA.lo,0,0);
    blkDevAdd(ullLBA,FSATA_SECTOR_SIZE,pstPort->uiPortId);
	semGive(pstPort->semMux);
}

/*****************************************************************************
 function     : ahciSendReq
 description  : send request
 input        : 
 output       : 
 return       : OK or ERROR
*****************************************************************************/
STATUS fsataSendReq(FSATA_PORT_S *pstPort,FSATA_REQ_S *pstReq)
{
    FSATA_COMMAND_DESC_S   *pstCmndTbl = NULL;
    FSATA_CMDHDR_S         *pstCmndList = NULL;
	FSATA_PRDE_S           *pstPrde = NULL;
	UINT32                  uiSGLCnt = 1;
    UINT32                  uiDescInfo =  0;
    UINT32                  uiFisLen = 5;
	
	assert(pstReq->uiTag < FSATA_MAX_REQ);
    pstCmndList = pstPort->pstCmdslot + pstReq->uiTag;
	pstCmndTbl = pstPort->pstCmdDentry + pstReq->uiTag;

	if(FSATA_NO_DATA == pstReq->ucReqType)
	{
		uiSGLCnt = 0;
	}
    /*
     *fill command table
    */
    memcpy(&pstCmndTbl->aucCfis,&pstReq->stFis,sizeof(FSATA_HOST_TO_DEV_S));
    pstPrde = &pstCmndTbl->auiPrdt[0];
	pstPrde->uiDba = FSATA_CPU_LE32(pstReq->stSGL.uiSGLLo);
	pstPrde->uiDdcAndExt = FSATA_CPU_LE32(FSATA_HCONTROL_DATA_SNOOP_ENABLE_V2 |
		                                  pstReq->stSGL.uiLen);
	/*
     *fill command slot
    */
    assert(sizeof(FSATA_COMMAND_DESC_S) == FSATA_CMD_DESC_SIZE);
	pstCmndList->uiCda = FSATA_CPU_LE32(pstPort->uiCmdDentryPhyAddr + 
	                                    pstReq->uiTag * FSATA_CMD_DESC_SIZE);
	pstCmndList->uiPrdeFisLen = FSATA_CPU_LE32((uiSGLCnt << 16) | (uiFisLen << 2));
	pstCmndList->uiTtl = FSATA_CPU_LE32(pstReq->stSGL.uiLen);
	uiDescInfo =  FSATA_CMD_DESC_SNOOP_ENABLE;/*FSATA_CMD_DESC_RES | */
	if (FSATA_FPDMA == pstReq->ucReqType)
	{
		uiDescInfo |= FSATA_FPDMA_QUEUED_CMD;
	}
	pstCmndList->uiDescInfo = FSATA_CPU_LE32(uiDescInfo | pstReq->uiTag);
	CACHE_DMA_FLUSH (pstCmndTbl, sizeof(FSATA_COMMAND_DESC_S));
	CACHE_DMA_FLUSH (pstCmndList, sizeof(FSATA_CMDHDR_S));
    /*
     *update command SLOT
    */
    FSATA_WRITE_REG(pstPort->pvRegBase + FSATA_COMMAND_CQPMP,0);
	FSATA_WRITE_REG(pstPort->pvRegBase + FSATA_COMMAND_CQR,(1 << pstReq->uiTag));

	pstPort->uiCmndActive |= (1 << pstReq->uiTag);
	
    return OK;
}

/*****************************************************************************
 function     : fsataFillWrFPDmaFis
 description  : fill write fpdma queue fis(sata definition command)
 input        : 
 output       : 
 return       : 
*****************************************************************************/
STATUS fsataFillWrFPDmaFis(FSATA_REQ_S *pstReq)
{
    UINT32     uiFUA = 0; /*force unit access*/
    UINT32     uiSecCnt = 0;

    uiSecCnt = pstReq->stSGL.uiLen/FSATA_SECTOR_SIZE;
    
    if(uiSecCnt > 0xffff)
    {
        FSATA_LOG(FSATA_DBG_ERR,"wrfpdma transfer size %d overflow\n",uiSecCnt,
                  0,0,0);
        return ERROR;
    }
    
    memset(&pstReq->stFis,0,sizeof(FSATA_HOST_TO_DEV_S));
    pstReq->stFis.ucFisType = FSAS_ATA_FIS_H2D;
    pstReq->stFis.ucCmd = FATA_WRITE_FPDMA_QUEUED;
    pstReq->stFis.ucFeature = (UINT8)(uiSecCnt & 0xff);
    pstReq->stFis.ucFeaturesExp = (UINT8)((uiSecCnt >> 8) & 0xff);
    pstReq->stFis.ucFlags = 0x80;
    pstReq->stFis.ucCtrl = 8;
    
    pstReq->stFis.ucLbal = pstReq->aucLBA[5];
    pstReq->stFis.ucLbam = pstReq->aucLBA[4];
    pstReq->stFis.ucLbah = pstReq->aucLBA[3];
    pstReq->stFis.ucLbalExp = pstReq->aucLBA[2];
    pstReq->stFis.ucLbamExp = pstReq->aucLBA[1];
    pstReq->stFis.ucLbahExp = pstReq->aucLBA[0];
	
    if(0x1 == uiFUA)
    {
        pstReq->stFis.ucDevice = 0xc0;
    }
    else
    {
        pstReq->stFis.ucDevice = 0x40;
    }
    
    pstReq->stFis.ucSecCnt = (pstReq->uiTag << 3); 
    pstReq->ucReqType = FSATA_FPDMA;
    
    return OK;
}

/*****************************************************************************
 function     : fsataFillSetFeatureFIS
 description  : freescale set feature done
*****************************************************************************/
VOID fsataSetFeatureDone(VOID *pstOrgReq)
{
    FSATA_REQ_S          *pstSataReq = NULL;
	FSATA_PORT_S         *pstPort = NULL;
    
    pstSataReq = (FSATA_REQ_S*)pstOrgReq;
	pstPort  = (FSATA_PORT_S*)pstSataReq->pvPort;
	fsataFreeReq(pstPort,pstSataReq->uiTag);
	semGive(pstPort->semMux);
}

/*****************************************************************************
 function     : fsataSetFeature
 description  : set feature
 input        : 
 output       : 
 return       : 
*****************************************************************************/
STATUS fsataFillSetFeatureFIS(FSATA_REQ_S *pstReq,UINT32 uiFeature)
{
    memset(&pstReq->stFis,0,sizeof(FSATA_HOST_TO_DEV_S));
    pstReq->stFis.ucFisType = FSAS_ATA_FIS_H2D;
	pstReq->stFis.ucFlags = 0x80;
    pstReq->stFis.ucCmd = FATA_CMD_SET_FEATURES;
    pstReq->stFis.ucFeature = uiFeature;
    pstReq->stFis.ucCtrl = 8;
    pstReq->ucReqType = FSATA_NO_DATA;
    
    return OK;
}

/*****************************************************************************
 function     : fsataCloseWrCache
 description  : close disk write cache for specifical scene
*****************************************************************************/
VOID fsataCloseWrCache(FSATA_DRV_S *pstDrv,UINT32 uiPort)
{
	FSATA_REQ_S	  *pstReq = NULL;
	FSATA_PORT_S  *pstPort = NULL;
	char		  *ulAddr = 0;
	UINT32		   uiSlot = 0;

	assert(uiPort < FSATA_MAX_PORT);
	pstPort = &pstDrv->astPort[uiPort];
	pstReq = &pstPort->astReq[0];
	pstReq->uiStatus = FSATA_REQ_USED;
	pstReq->uiTag = 0;
	pstReq->stSGL.uiLen = 0;
	pstReq->pfDone = fsataSetFeatureDone;
	fsataFillSetFeatureFIS(pstReq,FSATA_SETFEATURES_WC_OFF);
	if(OK != fsataSendReq(pstPort,pstReq))
	{
		FSATA_LOG(FSATA_DBG_ERR,"phy %d send feature failed\n",pstPort->uiPortId,
				 0,0,0);
		return; 	   
	}
	semTake(pstPort->semMux,WAIT_FOREVER);
	FSATA_LOG(FSATA_DBG_ERR,"phy %d close write cache ok\n",pstPort->uiPortId,
				 0,0,0);
}

/*****************************************************************************
* fsataSendIdentify - send identify
*****************************************************************************/
STATUS fsataSendIdentify(FSATA_DRV_S *pstDrv,UINT32 uiPort)
{
	FSATA_REQ_S	  *pstReq = NULL;
	FSATA_PORT_S  *pstPort = NULL;
	char		  *ulAddr = 0;
	UINT32		   uiSlot = 0;

	assert(uiPort < FSATA_MAX_PORT);
	pstPort = &pstDrv->astPort[uiPort];
	pstReq = &pstPort->astReq[0];
	if(NULL == pstReq)
	{
		FSATA_LOG(FSATA_DBG_ERR,"phy %d get free req failed\n",uiPort,0,0,0);
		return;
	}
	pstReq->uiStatus = FSATA_REQ_USED;
	pstReq->uiTag = 0;
	
	/*buffer initialization */
	pstReq->pucBuf = (UINT8*)&pstPort->stIden;
	FSATA_LOG(FSATA_DBG_INFO,"send identify addr %p\n",(UINT32)pstReq->pucBuf,0,0,0);
	pstReq->stSGL.uiLen = 512;
	pstReq->stSGL.uiSGLHi = 0;
	pstReq->stSGL.uiSGLLo = (UINT32)pstReq->pucBuf;
	pstReq->pfDone = fsataIdentifyDone;
	pstReq->pvPort = (VOID*)pstPort;
	/*
	 *fill fis information
	*/
	memset(&pstReq->stFis,0,sizeof(FSATA_HOST_TO_DEV_S));
	pstReq->stFis.ucFisType = FSAS_ATA_FIS_H2D;
	pstReq->stFis.ucCmd = FATA_CMD_ID_ATA;
	pstReq->stFis.ucFlags = 0x80;/*write command register*/
	pstReq->stFis.ucDevice = 0xa0;
	pstReq->stFis.ucCtrl = 0;
	pstReq->ucReqType = FSATA_PIO;
	CACHE_DMA_FLUSH (pstReq->pucBuf,pstReq->stSGL.uiLen);
	if(OK != fsataSendReq(pstPort,pstReq))
	{
		FSATA_LOG(FSATA_DBG_ERR,"phy %d send identify failed\n",pstPort->uiPortId,
				 0,0,0);
		return; 	   
	}
}

/*****************************************************************************
 function     : fsataFillReadFPDmaQueueFis
 description  : fill read fpdma queue fis(sata definition command)
 input        : 
 output       : 
 return       : 
*****************************************************************************/
STATUS fsataFillReadFPDmaQueueFis(FSATA_REQ_S *pstReq)
{
    UINT32     uiFUA = 0; /*force unit access*/
    UINT32     uiSecCnt = 0;

    uiSecCnt = pstReq->stSGL.uiLen / FSATA_SECTOR_SIZE;
    if(uiSecCnt > 0xffff)
    {
        FSATA_LOG(FSATA_DBG_ERR,"readfpdma transfer size 0x%x overflow\n",
                pstReq->stSGL.uiLen,0,0,0);
        return ERROR;
    }
    memset(&pstReq->stFis,0,sizeof(FSATA_HOST_TO_DEV_S));
    pstReq->stFis.ucFisType = FSAS_ATA_FIS_H2D;
    pstReq->stFis.ucCmd = FATA_READ_FPDMA_QUEUED;
    pstReq->stFis.ucFeature = (UINT8)uiSecCnt;
    pstReq->stFis.ucFeaturesExp = (UINT8)(uiSecCnt >> 8);
    pstReq->stFis.ucFlags = 0x80;
    pstReq->stFis.ucCtrl = 8;
    pstReq->stFis.ucLbal = pstReq->aucLBA[5];
    pstReq->stFis.ucLbam = pstReq->aucLBA[4];
    pstReq->stFis.ucLbah = pstReq->aucLBA[3];
    pstReq->stFis.ucLbalExp = pstReq->aucLBA[2];
    pstReq->stFis.ucLbamExp = pstReq->aucLBA[1];
    pstReq->stFis.ucLbahExp = pstReq->aucLBA[0];

    if(0x1 == uiFUA)
    {
        pstReq->stFis.ucDevice = 0xc0;
    }
    else
    {
        pstReq->stFis.ucDevice = 0x40;
    }
    
    pstReq->stFis.ucSecCnt = (pstReq->uiTag << 3);/**/
    pstReq->ucReqType = FSATA_FPDMA;

    return OK;
}

/*****************************************************************************
 function     : fsataBlkRead
 description  : freescale sata read from uiStartBlk
              : READ CATEGORY:SAT_READ_SECTORS,SAT_READ_DMA, 
              :               SAT_READ_SECTORS_EXT,SAT_READ_DMA_EXT
              :               SAT_READ_FPDMA_QUEUED
 input        : 
 output       : get data to buffer
 return       : 
*****************************************************************************/
STATUS fsataBlkRead(FSATA_PORT_S *pstPort,FSATA_REQ_S *pstReq)
{
    /*
     *ncq not supported;48bit not supported;send read dma command 
     *ahciFillReadDmaFis
    */
    if(ERROR == fsataFillReadFPDmaQueueFis(pstReq))
    {
        return ERROR;
    }
    SPIN_LOCK_ISR_TAKE(&pstPort->stLock);
    if(ERROR == fsataSendReq(pstPort,pstReq))
    {
		SPIN_LOCK_ISR_GIVE(&pstPort->stLock);
        return ERROR;
    }
    SPIN_LOCK_ISR_GIVE(&pstPort->stLock);
    return OK;
}

/*****************************************************************************
 function     : fsataBlkWrite
 description  : fsata write from uiStartBlk
 input        : 
 output       : write data to LBA
 return       : 
*****************************************************************************/
STATUS fsataBlkWrite(FSATA_PORT_S *pstPort,FSATA_REQ_S *pstReq)
{
    if(ERROR == fsataFillWrFPDmaFis(pstReq))
    {
        return ERROR;
    }
    SPIN_LOCK_ISR_TAKE(&pstPort->stLock);
    if(ERROR == fsataSendReq(pstPort,pstReq))
    {
		SPIN_LOCK_ISR_GIVE(&pstPort->stLock);
        return ERROR;
    }
    SPIN_LOCK_ISR_GIVE(&pstPort->stLock);
    return OK;
}

/*****************************************************************************
 function     : fsataRWSectorDone
 description  : freescale sector read/write callback
 input        : 
 output       : 
 return       : 
*****************************************************************************/
VOID fsataRWSectorDone(VOID *pstOrgReq)
{
    FSATA_REQ_S          *pstSataReq = NULL;
    BLK_REQ_S            *pstBlkReq = NULL;
	FSATA_PORT_S         *pstPort = NULL;
    
    pstSataReq = (FSATA_REQ_S*)pstOrgReq;
    pstBlkReq = pstSataReq->pstBlkReq;
	pstPort  = (FSATA_PORT_S*)pstSataReq->pvPort;
    if(FSATA_DATA_IN == pstSataReq->ucDirection)
    {
		CACHE_DMA_INVALIDATE(pstSataReq->pucBuf,pstSataReq->stSGL.uiLen);
    }
	pstBlkReq->pfDone(pstBlkReq);
	fsataFreeReq(pstPort,pstSataReq->uiTag);
}

/*****************************************************************************
 function     : fsataRWSector
 description  : freescale sata sector read/write interface
 input        : 
 output       : 
 return       : 
*****************************************************************************/
STATUS fsataRWSector(BLK_REQ_S  *pstBlkReq)
{
    FSATA_REQ_S      *pstReq = NULL;
    FSATA_PORT_S     *pstPort = NULL;
    /*FAST int        oldlevel;*/
    
    if((pstBlkReq->uiPhyId >= FSATA_MAX_PORT) || 
       (FSATA_LINK_UP != gstFSataDrv.astPort[pstBlkReq->uiPhyId].uiStatus))
    {
        FSATA_LOG(FSATA_DBG_ERR,"dev %d is not ready\n",pstBlkReq->uiPhyId,0,0,0);
        return ERROR;
    }
    
	if(0x0 == pstBlkReq->uiSecNum)
	{
        FSATA_LOG(FSATA_DBG_ERR,"sector number is 0\n",0,0,0,0);
        return OK;
    }
    
	pstPort = &gstFSataDrv.astPort[pstBlkReq->uiPhyId];
	SPIN_LOCK_ISR_TAKE(&pstPort->stLock);
    pstReq = fsataGetReq(pstPort);
    if(NULL == pstReq)
    {
		SPIN_LOCK_ISR_GIVE(&pstPort->stLock);
        FSATA_LOG(FSATA_DBG_ERR,"dev get free request failed\n",0,0,0,0);
        return ERROR;
    }
    SPIN_LOCK_ISR_GIVE(&pstPort->stLock);
    /*
     *buffer initialization
    */
    pstReq->pucBuf = pstBlkReq->pucBuf; 
    pstReq->stSGL.uiLen = pstBlkReq->uiSecNum * FSATA_SECTOR_SIZE;
    pstReq->stSGL.uiSGLHi = 0;
    pstReq->stSGL.uiSGLLo = ((ULONG)(pstBlkReq->pucBuf)) & 0xffffffff;
    pstReq->pvPort = (VOID*)pstPort;
    pstReq->ucReqType = FSATA_FPDMA;
    pstReq->pfDone = fsataRWSectorDone;
    pstReq->pstBlkReq = pstBlkReq;
   	
    pstReq->aucLBA[0] = (pstBlkReq->uiSecHi >> 8) & 0xff;
    pstReq->aucLBA[1] = (pstBlkReq->uiSecHi) & 0xff;   
    pstReq->aucLBA[2] = (pstBlkReq->uiSecLo >> 24) & 0xff;
    pstReq->aucLBA[3] = (pstBlkReq->uiSecLo >> 16) & 0xff;
    pstReq->aucLBA[4] = (pstBlkReq->uiSecLo >> 8) & 0xff;
    pstReq->aucLBA[5] = (pstBlkReq->uiSecLo) & 0xff;
	
    if(FS_OPER_READ == pstBlkReq->uiOper)
    {
    	pstReq->ucDirection = FSATA_DATA_IN;
        if(OK != fsataBlkRead(pstPort,pstReq))
        {
            return ERROR;
        }
    }
    else
    {
    	pstReq->ucDirection = FSATA_DATA_OUT;
    	CACHE_DMA_FLUSH (pstReq->pucBuf,pstReq->stSGL.uiLen);
        if(OK != fsataBlkWrite(pstPort,pstReq))
        {
            return ERROR;
        }
    }
    return OK;
}

