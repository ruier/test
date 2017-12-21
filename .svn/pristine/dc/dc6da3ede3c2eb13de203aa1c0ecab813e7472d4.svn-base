/******************************************************************************

                  VERSION (C), 2001-2011, 

 ******************************************************************************
  file       : ahciReq.c
  version    : 1.0
  author     : lgh
  data       : 2015.06.04
  function   : ahci request process
******************************************************************************/

/*----------------------------------------------*
 * header file                                  *
 *----------------------------------------------*/
#include "ahciReq.h"
#include "ahciDrv.h"

/*----------------------------------------------*
 * external  variable                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external  function                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * internal  function                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * global  variable                             *
 *----------------------------------------------*/

 
/*----------------------------------------------*
 * constant variable                            *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macro definition                             *
 *----------------------------------------------*/

/*****************************************************************************
 function     : ahciGetReq
 description  : get free request
 input        : 
 output       : 
 return       : NULL or REQ 
*****************************************************************************/
AHCI_REQ_S* ahciGetReq(AHCI_DRV_S *pstDrv)
{   
    UINT32          uiLoop = 0;
	FAST int   		oldlevel;
	
    oldlevel = intLock ();
    for(uiLoop = 0;uiLoop < AHCI_MAX_REQ;uiLoop++)
    {
        if(AHCI_REQ_UNUSED == pstDrv->astReq[uiLoop].uiStatus)
        {
            pstDrv->astReq[uiLoop].uiStatus = AHCI_REQ_USED;
			pstDrv->astReq[uiLoop].ucTag = uiLoop;
			pstDrv->astReq[uiLoop].ucTag = (UINT8)uiLoop;
			intUnlock(oldlevel);
			AHCI_LOG(AHCI_DBG_INFO,"get req tag %d\n",uiLoop,0,0,0,0,0);
            return (&(pstDrv->astReq[uiLoop]));
        }
    }
	intUnlock(oldlevel);

    return NULL;  
}

/*****************************************************************************
 function     : ahciFreeReq
 description  : free request
 input        : 
 output       : 
 return       : NULL or REQ 
*****************************************************************************/
VOID ahciFreeReq(AHCI_DRV_S *pstDrv,UINT8 ucTag)
{
	assert(ucTag < AHCI_MAX_REQ);
    if(AHCI_REQ_UNUSED == pstDrv->astReq[ucTag].uiStatus)
    {
        AHCI_LOG(AHCI_DBG_ERR,"the request %d has been freed\n",ucTag,0,0,0,0,0);
        return;
    }
    pstDrv->uiCmndTblFlag &= (~(1 << ucTag));
    pstDrv->astReq[ucTag].uiStatus = AHCI_REQ_UNUSED;
    memset(&pstDrv->astReq[ucTag].stFis,0,sizeof(SAS_HOST_TO_DEV_S));
    memset(&pstDrv->astReq[ucTag].stSGL,0,sizeof(AHCI_SGL_S));
    pstDrv->astReq[ucTag].aucLBA[0] = 0;
    pstDrv->astReq[ucTag].aucLBA[1] = 0;
    pstDrv->astReq[ucTag].aucLBA[2] = 0;
    pstDrv->astReq[ucTag].aucLBA[3] = 0;
    pstDrv->astReq[ucTag].aucLBA[4] = 0;
    pstDrv->astReq[ucTag].aucLBA[5] = 0;
    pstDrv->astReq[ucTag].pucBuf = NULL;
    pstDrv->astReq[ucTag].pvFsReq = NULL;
    AHCI_LOG(AHCI_DBG_DATA,"phy %d free tag %d\n",pstDrv->uiPhyId,
		         ucTag,0,0,0,0);
}

/*****************************************************************************
 function     : ahciSendReq
 description  : send request
 input        : 
 output       : 
 return       : OK or ERROR
*****************************************************************************/
STATUS ahciSendReq(AHCI_DRV_S *pstDrv,AHCI_REQ_S *pstReq)
{
    AHCI_CMND_TBL_S   *pstCmndTbl = 0;
    AHCI_CMND_LIST_S  *pstCmndList = 0;
    AHCI_PORT_S       *pstPort = NULL;
    UINT32             uiSGLCnt = 1;
    UINT32             uiRdWr = 0;
    UINT32             uiFisLen = 5;
    UINT8             *pucAddr = NULL;
    UINT32             uiVal = 0;
    INT32              iKey;

	assert(pstReq->ucTag < AHCI_MAX_REQ);
    pstPort = &pstDrv->stPort;
    pstCmndList = pstPort->pstCmdList + pstReq->ucTag;
	pstCmndTbl = pstPort->pstCmdTbl + pstReq->ucTag;

    /*
     *fill command list
    */
    uiSGLCnt = pstReq->ucSGLNum;
    pstCmndList->uiCmndCtrl = AHCI_CPU_LE32((uiSGLCnt << 16) | uiFisLen);
    pstCmndList->uiTranLen = 0;

    /*
     *fill command table
    */
    memcpy(&pstCmndTbl->stFis,&pstReq->stFis,sizeof(SAS_HOST_TO_DEV_S));
    
    /*
     *REF AHCI VER1.3 P.40
    */
    pstCmndTbl->stPrdt[0].uiAddrHi = AHCI_CPU_LE32(pstReq->stSGL.uiSGLHi);
    pstCmndTbl->stPrdt[0].uiAddrLo = AHCI_CPU_LE32(pstReq->stSGL.uiSGLLo);
    pstCmndTbl->stPrdt[0].uiByteCnt = AHCI_CPU_LE32(pstReq->stSGL.uiLen - 1);
	CACHE_DMA_FLUSH (pstCmndTbl, sizeof(AHCI_CMND_TBL_S));
	CACHE_DMA_FLUSH (pstCmndList, sizeof(AHCI_CMND_LIST_S));
    /*
     *update command CI
    */
    iKey = intCpuLock();
    pucAddr = pstDrv->stPci.pucVirAddr + AHCI_PORT_OFFSET(0);
    if(SATA_FPDMA == pstReq->ucReqType)
    {
		AHCI_WRITE_REG(pucAddr + AHCI_PxSACT,(1<< pstReq->ucTag));
    	uiVal = AHCI_READ_REG(pucAddr + AHCI_PxSACT);
    }
    AHCI_WRITE_REG(pucAddr + AHCI_PxCI,1 << pstReq->ucTag);
    uiVal = AHCI_READ_REG(pucAddr + AHCI_PxCI);
    pstDrv->uiCmndTblFlag |= (1 << pstReq->ucTag);
    intCpuUnlock(iKey);
    
    return OK;
}

/*****************************************************************************
 function     : ahciConvertIdentify
 description  : convert identify data to cpu endian
 input        : 
 output       : 
 return       : 
*****************************************************************************/
VOID ahciConvertIdentify(SATA_ID_DATA_S *vpstIden)
{
    UINT16  *pusBuf = NULL;
    UINT32   uiLoop = 0;

    pusBuf = (UINT16*)vpstIden;
    for(uiLoop = 0;uiLoop < (sizeof(SATA_ID_DATA_S)/2);uiLoop++)
    {
        *pusBuf = AHCI_CPU_LE16(*pusBuf);
        pusBuf++;
    }
}

/*****************************************************************************
 function     : ahciIdentifyDone
 description  : ahci identify done
 input        : 
 output       : write data to LBA
 return       : 
*****************************************************************************/
VOID ahciIdentifyDone(VOID  *pstReq)
{
    SATA_ID_DATA_S      *pstIdDevData = NULL;
    AHCI_DEV_S          *pstDev = NULL;
    AHCI_DRV_S          *pstDrv = NULL;
    UINT32               uiLBALo = 0;
    UINT64_S             ullLBA;

    pstIdDevData = (SATA_ID_DATA_S*)(((AHCI_REQ_S*)pstReq)->pucBuf);
    ahciConvertIdentify(pstIdDevData);
    pstDrv = (AHCI_DRV_S*)(((AHCI_REQ_S*)pstReq)->pvDrv);
    pstDev = &pstDrv->stDev;
    pstDev->uiNCQMaxIO = (UINT32)pstIdDevData->usQueueDepth;
    pstDev->uiNCQSupport = FALSE;
    if(pstIdDevData->usCapabilities & 0x100)
    {
        pstDev->uiNCQSupport = TRUE;
    }

    pstDev->ui48BitLBA = FALSE;
    if((pstIdDevData->usCmndSetSupported1 & 0x400) &&
       (pstIdDevData->usCmndSetFeaEnabled1 & 0x400))
    {
        pstDev->ui48BitLBA = TRUE;
    }

    pstDev->uiRemovable = FALSE;
    if(pstIdDevData->usCmndSetSupported & 0x4)
    {
        pstDev->uiRemovable = TRUE;
    }
    pstDev->uiRemoveEnable = FALSE;
    if(pstIdDevData->usCmndSetFeaEnabled & 0x4)
    {
        pstDev->uiRemoveEnable = TRUE;
    }

    pstDev->uiWrCacheEnable = FALSE;
    if(pstIdDevData->usCmndSetFeaEnabled & 0x20)
    {
        pstDev->uiWrCacheEnable = TRUE;
    }
    
    pstDev->uiDMASupport = FALSE;
    if(pstIdDevData->usDmaLbaIodTimer & 0x100)
    {
        pstDev->uiDMASupport = TRUE;
    }

    pstDev->uiDMAEnable = FALSE;
    if(pstIdDevData->usUltraDMAModes & 0x7f7f)
    {
        pstDev->uiDMAEnable = TRUE;
    }

    pstDev->uiMaxAddrSectors = pstIdDevData->usUserSecHi << 16;
    pstDev->uiMaxAddrSectors |= pstIdDevData->usUserSecLo;

    if(TRUE == pstDev->ui48BitLBA)
    {
        pstDev->ucCapacity[0] = (UINT8)(pstIdDevData->usMaxLBA48 >> 8);
        pstDev->ucCapacity[1] = (UINT8)(pstIdDevData->usMaxLBA48 & 0xff);
        pstDev->ucCapacity[2] = (UINT8)(pstIdDevData->usMaxLBA32 >> 8);
        pstDev->ucCapacity[3] = (UINT8)(pstIdDevData->usMaxLBA32 & 0xff);
        uiLBALo = pstIdDevData->usMaxLBA16 << 16;
        uiLBALo |= pstIdDevData->usMaxLBA0;
        /*uiLBALo = uiLBALo - 1;*/
        pstDev->ucCapacity[4] = (UINT8)((uiLBALo >> 24) & 0xFF);
        pstDev->ucCapacity[5] = (UINT8)((uiLBALo >> 16) & 0xFF);
        pstDev->ucCapacity[6] = (UINT8)((uiLBALo >> 8)  & 0xFF);
        pstDev->ucCapacity[7] = (UINT8)((uiLBALo )      & 0xFF);
    }
    else
    {
        pstDev->ucCapacity[0] = 0;
        pstDev->ucCapacity[1] = 0;
        pstDev->ucCapacity[2] = 0;
        pstDev->ucCapacity[3] = 0;
        uiLBALo = pstIdDevData->usUserSecHi << 16;
        uiLBALo |= pstIdDevData->usUserSecLo;
        uiLBALo = uiLBALo - 1;
        pstDev->ucCapacity[4] = (UINT8)((uiLBALo >> 24) & 0xFF);
        pstDev->ucCapacity[5] = (UINT8)((uiLBALo >> 16) & 0xFF);
        pstDev->ucCapacity[6] = (UINT8)((uiLBALo >> 8)  & 0xFF);
        pstDev->ucCapacity[7] = (UINT8)((uiLBALo )      & 0xFF); 
    }

    /*
     *word106 bit12:device logical sector longer than 256 words
     *        bit3~0:2^x logical sectors per physical sector
     *word 117~118:words per logical sector
    */
    if (pstIdDevData->usWord104[2] & 0x1000)
    {
        AHCI_LOG(AHCI_DBG_INFO,"disk %d logical sector loger than 256 words",
                pstDev->uiDevId,0,0,0,0,0);
    }
    ahciFreeReq(pstDrv,((AHCI_REQ_S*)pstReq)->ucTag);

    ullLBA.hi  = ((UINT32)(pstDev->ucCapacity[0])) << 24;
    ullLBA.hi |= ((UINT32)(pstDev->ucCapacity[1])) << 16;
    ullLBA.hi |= ((UINT32)(pstDev->ucCapacity[2])) << 8;
    ullLBA.hi |= ((UINT32)(pstDev->ucCapacity[3])) << 0;
    ullLBA.lo = ((UINT32)(pstDev->ucCapacity[4])) << 24;
    ullLBA.lo |= ((UINT32)(pstDev->ucCapacity[5])) << 16;
    ullLBA.lo |= ((UINT32)(pstDev->ucCapacity[6])) << 8;
    ullLBA.lo |= ((UINT32)(pstDev->ucCapacity[7]));

	AHCI_LOG(AHCI_DBG_ERR,"disk %d contain 0x%x sectors ncq support %d\n",
             pstDrv->uiPhyId,ullLBA.lo,pstDev->uiNCQSupport,0,0,0);
    blkDevAdd(ullLBA,AHCI_SECTOR_SIZE,pstDrv->uiPhyId);
}


/*****************************************************************************
*
* ahciSendIdentify - send identify dev
*
* RETURNS: N/A
*
*****************************************************************************/
VOID ahciSendIdentify(AHCI_DRV_S *pstDrv)
{
    AHCI_REQ_S  *pstReq = NULL;
    char 		*ulAddr = 0;
    UINT32		uiSlot = 0;
    

    pstReq = ahciGetReq(pstDrv);
    if(NULL == pstReq)
    {
        AHCI_LOG(AHCI_DBG_ERR,"get free req failed\n",0,0,0,0,0,0);
        return;
    }
    
    /*buffer initialization */
	pstReq->pucBuf = (UINT8*)&pstDrv->stDev.stIdenDev;
	AHCI_LOG(AHCI_DBG_INFO,"send identify addr %p\n",(UINT32)pstReq->pucBuf,0,0,0,0,0);
    pstReq->stSGL.uiLen = 512;
    pstReq->stSGL.uiSGLHi = 0;
    pstReq->stSGL.uiSGLLo = (UINT32)pstReq->pucBuf;
    pstReq->ucSGLNum = 1;
    pstReq->pfDone = ahciIdentifyDone;
	pstReq->pvDrv= (VOID*)pstDrv;
    /*
     *fill fis information
    */
	memset(&pstReq->stFis,0,sizeof(SAS_HOST_TO_DEV_S));
	pstReq->stFis.ucFisType = SAS_ATA_FIS_H2D;
	pstReq->stFis.ucCmd = ATA_CMD_ID_ATA;
	pstReq->stFis.ucFlags = 0x80;/*write command register*/
	pstReq->stFis.ucDevice = AHCI_SDH_IBM;
	pstReq->stFis.ucCtrl = AHCI_CTL_4BIT;
	pstReq->ucReqType = SATA_PIO;
	CACHE_DMA_FLUSH (pstReq->pucBuf,pstReq->stSGL.uiLen);
                            
    if(OK != ahciSendReq(pstDrv,pstReq))
    {
		AHCI_LOG(AHCI_DBG_ERR,"phy %d send identify failed\n",pstDrv->uiPhyId,
		         0,0,0,0,0);
		return;        
    }
}


/*****************************************************************************
 function     : ahciRWSectorDone
 description  : ahci sector read/write callback
 input        : 
 output       : 
 return       : 
*****************************************************************************/
VOID ahciRWSectorDone(VOID *pstOrgReq)
{
    AHCI_REQ_S          *pstAhciReq = NULL;
    BLK_REQ_S           *pstBlkReq = NULL;
    
    pstAhciReq = (AHCI_REQ_S*)pstOrgReq;
    pstBlkReq = pstAhciReq->pstBlkReq;
    if(AHCI_DATA_IN == pstAhciReq->ucDirection)
    {
		CACHE_USER_INVALIDATE(pstAhciReq->pucBuf,pstAhciReq->stSGL.uiLen);
    }
    
    ahciFreeReq((AHCI_DRV_S*)pstAhciReq->pvDrv,pstAhciReq->ucTag);
    pstBlkReq->pfDone(pstBlkReq);
}

/*****************************************************************************
 function     : ahciFillReadDmaFis
 description  : fill read dma fis
 input        : 
 output       : 
 return       : 
*****************************************************************************/
STATUS ahciFillReadDmaFis(AHCI_REQ_S *pstReq)
{
    /*
     * max sector count is 256
    */
    
    if((pstReq->stSGL.uiLen/AHCI_SECTOR_SIZE) > 255)
    {
        AHCI_LOG(AHCI_DBG_ERR,"readdma transfer size 0x%x overflow\n",
                pstReq->stSGL.uiLen,
                0,0,0,0,0);
        return ERROR;
    }
    memset(&pstReq->stFis,0,sizeof(SAS_HOST_TO_DEV_S));
    pstReq->stFis.ucFisType = SAS_ATA_FIS_H2D;
    pstReq->stFis.ucCmd = ATA_CMD_READ_DMA;
    pstReq->stFis.ucFeature = 0;
    pstReq->stFis.ucDevice = 0x40 | (pstReq->aucLBA[2] & 0xf);
    pstReq->stFis.ucFlags = 0x80;
    pstReq->stFis.ucLbal = pstReq->aucLBA[5];
    pstReq->stFis.ucLbam = pstReq->aucLBA[4];
    pstReq->stFis.ucLbah = pstReq->aucLBA[3];
    pstReq->stFis.ucSecCnt = (UINT8)(pstReq->stSGL.uiLen/AHCI_SECTOR_SIZE);
    pstReq->ucReqType = SATA_DMA;

    return OK;
}

/*****************************************************************************
 function     : ahciFillWriteDmaFis
 description  : fill read dma fis
 input        : 
 output       : 
 return       : 
*****************************************************************************/
STATUS ahciFillWriteDmaFis(AHCI_REQ_S *pstReq)
{
    /*
     * max sector count is 256
    */
    
    if((pstReq->stSGL.uiLen/AHCI_SECTOR_SIZE) > 255)
    {
        AHCI_LOG(AHCI_DBG_ERR,"wrdma transfer size 0x%x overflow\n",
                 pstReq->stSGL.uiLen,
                0,0,0,0,0);
        return ERROR;
    }
    memset(&pstReq->stFis,0,sizeof(SAS_HOST_TO_DEV_S));
    pstReq->stFis.ucFisType = SAS_ATA_FIS_H2D;
    pstReq->stFis.ucCmd = ATA_CMD_WRITE_DMA;
    pstReq->stFis.ucFeature = 0;
    pstReq->stFis.ucDevice = 0x40 | (pstReq->aucLBA[2] & 0xf);
    pstReq->stFis.ucFlags = 0x80;
    pstReq->stFis.ucLbal = pstReq->aucLBA[5];
    pstReq->stFis.ucLbam = pstReq->aucLBA[4];
    pstReq->stFis.ucLbah = pstReq->aucLBA[3];
    pstReq->stFis.ucSecCnt = (UINT8)(pstReq->stSGL.uiLen/AHCI_SECTOR_SIZE);
    pstReq->ucReqType = SATA_DMA;

    return OK;
}

/*****************************************************************************
 function     : ahciFillWrFPDmaFis
 description  : fill read fpdma queue fis(sata definition command)
 input        : 
 output       : 
 return       : 
*****************************************************************************/
STATUS ahciFillWrFPDmaFis(AHCI_REQ_S *pstReq)
{
    UINT32     uiFUA = 0; /*force unit access*/
    UINT32     uiSecCnt = 0;

    uiSecCnt = pstReq->stSGL.uiLen/AHCI_SECTOR_SIZE;
    
    if(uiSecCnt > 0xffff)
    {
        AHCI_LOG(AHCI_DBG_ERR,"wrfpdma transfer size %d overflow\n",uiSecCnt,
                0,0,0,0,0);
        return ERROR;
    }
    
    memset(&pstReq->stFis,0,sizeof(SAS_HOST_TO_DEV_S));
    pstReq->stFis.ucFisType = SAS_ATA_FIS_H2D;
    pstReq->stFis.ucCmd = ATA_WRITE_FPDMA_QUEUED;
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
    
    pstReq->stFis.ucSecCnt = pstReq->ucTag << 3; 
    pstReq->ucReqType = SATA_FPDMA;
    return OK;
}


/*****************************************************************************
 function     : spcFillReadFPDmaQueueFis
 description  : fill read fpdma queue fis(sata definition command)
 input        : 
 output       : 
 return       : 
*****************************************************************************/
STATUS ahciFillReadFPDmaQueueFis(AHCI_REQ_S *pstReq)
{
    UINT32     uiFUA = 0; /*force unit access*/
    UINT32     uiSecCnt = 0;

    uiSecCnt = pstReq->stSGL.uiLen / AHCI_SECTOR_SIZE;
    if(uiSecCnt > 0xffff)
    {
        AHCI_LOG(AHCI_DBG_ERR,"readfpdma transfer size 0x%x overflow\n",
                pstReq->stSGL.uiLen,0,0,0,0,0);
        return ERROR;
    }
    memset(&pstReq->stFis,0,sizeof(SAS_HOST_TO_DEV_S));
    pstReq->stFis.ucFisType = SAS_ATA_FIS_H2D;
    pstReq->stFis.ucCmd = ATA_READ_FPDMA_QUEUED;
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
    
    pstReq->stFis.ucSecCnt = pstReq->ucTag << 3; 
    pstReq->ucReqType = SATA_FPDMA;

    return OK;
}

/*****************************************************************************
 function     : ahciBlkRead
 description  : ahci read from uiStartBlk
              : READ CATEGORY:SAT_READ_SECTORS,SAT_READ_DMA, 
              :               SAT_READ_SECTORS_EXT,SAT_READ_DMA_EXT
              :               SAT_READ_FPDMA_QUEUED
 input        : 
 output       : get data to buffer
 return       : 
*****************************************************************************/
STATUS ahciBlkRead(AHCI_REQ_S *pstReq)
{
    /*
     *ncq not supported;48bit not supported;send read dma command 
     *ahciFillReadDmaFis
    */
    if(ERROR == ahciFillReadFPDmaQueueFis(pstReq))
    {
        return ERROR;
    }
    
    if(ERROR == ahciSendReq((AHCI_DRV_S*)pstReq->pvDrv,pstReq))
    {
        return ERROR;
    }
    
    return OK;
}

/*****************************************************************************
 function     : spcBlkWrite
 description  : spc write from uiStartBlk
 input        : 
 output       : write data to LBA
 return       : 
*****************************************************************************/
STATUS ahciBlkWrite(AHCI_REQ_S *pstReq)
{
    if(ERROR == ahciFillWrFPDmaFis(pstReq))
    {
        return ERROR;
    }
    
    if(ERROR == ahciSendReq((AHCI_DRV_S*)pstReq->pvDrv,pstReq))
    {
        return ERROR;
    }
    
    return OK;
}

/*****************************************************************************
 function     : ahciRWSector
 description  : ahci sector read/write interface
 input        : 
 output       : 
 return       : 
*****************************************************************************/
STATUS ahciRWSector(BLK_REQ_S  *pstBlkReq)
{
    AHCI_REQ_S     *pstReq = NULL;
    AHCI_DRV_S     *pstDrv = NULL;
    FAST int        oldlevel;
    
    if((pstBlkReq->uiPhyId >= AHCI_MAX_PHY) || 
       (AHCI_LINK_UP != gastAhciDrv[pstBlkReq->uiPhyId].stPort.uiStatus))
    {
        AHCI_LOG(AHCI_DBG_ERR,"dev %d is not ready\n",pstBlkReq->uiPhyId,0,0,0,0,0);
        return ERROR;
    }
    
	if(0x0 == pstBlkReq->uiSecNum)
	{
        AHCI_LOG(AHCI_DBG_ERR,"sector number is 0\n",0,0,0,0,0,0);
        return OK;
    }
    
	pstDrv = &gastAhciDrv[pstBlkReq->uiPhyId];
    oldlevel = intLock ();
    pstReq = ahciGetReq(pstDrv);
    if(NULL == pstReq)
    {

        AHCI_LOG(AHCI_DBG_ERR,"dev get free request failed\n",0,0,0,0,0,0);
        intUnlock(oldlevel);
        return ERROR;
    }
    
    /*
     *buffer initialization
    */
    pstReq->pucBuf = pstBlkReq->pucBuf; 
    pstReq->stSGL.uiLen = pstBlkReq->uiSecNum * AHCI_SECTOR_SIZE;
    pstReq->stSGL.uiSGLHi = 0;
    pstReq->stSGL.uiSGLLo = ((ULONG)(pstBlkReq->pucBuf)) & 0xffffffff;
    pstReq->ucSGLNum = 1;
    pstReq->pvDrv = (VOID*)pstDrv;
    pstReq->ucReqType = SATA_DMA;
    pstReq->pfDone = ahciRWSectorDone;
    pstReq->pstBlkReq = pstBlkReq;
   	
    pstReq->aucLBA[0] = (pstBlkReq->uiSecHi >> 8) & 0xff;
    pstReq->aucLBA[1] = (pstBlkReq->uiSecHi) & 0xff;   
    pstReq->aucLBA[2] = (pstBlkReq->uiSecLo >> 24) & 0xff;
    pstReq->aucLBA[3] = (pstBlkReq->uiSecLo >> 16) & 0xff;
    pstReq->aucLBA[4] = (pstBlkReq->uiSecLo >> 8) & 0xff;
    pstReq->aucLBA[5] = (pstBlkReq->uiSecLo) & 0xff;  

    AHCI_LOG(AHCI_DBG_DATA,"ahciRWSector %x %x %x %x %x %x\n",pstBlkReq->uiPhyId,
              pstBlkReq->uiSecNum,(UINT32)pstBlkReq->pucBuf,pstBlkReq->uiSecHi,
              pstBlkReq->uiSecLo,pstBlkReq->uiOper);
    
    if(FS_OPER_READ == pstBlkReq->uiOper)
    {
    	pstReq->ucDirection = AHCI_DATA_IN;
    	CACHE_USER_INVALIDATE(pstReq->pucBuf,pstReq->stSGL.uiLen);
        if(OK != ahciBlkRead(pstReq))
        {
            intUnlock(oldlevel);
            return ERROR;
        }
    }
    else
    {
    	pstReq->ucDirection = AHCI_DATA_OUT;
    	CACHE_DMA_FLUSH (pstReq->pucBuf,pstReq->stSGL.uiLen);
        if(OK != ahciBlkWrite(pstReq))
        {
            intUnlock(oldlevel);
            return ERROR;
        }
    }
    intUnlock(oldlevel);

    return OK;
}


