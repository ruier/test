/******************************************************************************

                  VERSION (C), 2001-2011, 

 ******************************************************************************
  file       : blkDev.c
  version    : 1.0
  author     : lgh
  data       : 2012.9.3
  function   : block device management
******************************************************************************/

/*----------------------------------------------*
 * header file                                  *
 *----------------------------------------------*/
#include "blkInc.h"

/*----------------------------------------------*
 * external  variable                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external  function                           *
 *----------------------------------------------*/
extern STATUS fsRecoverVol(FS_VOL_S *vpstVol);

/*----------------------------------------------*
 * internal  function                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * global  variable                             *
 *----------------------------------------------*/

BLK_CTRL_S  gstBlkCtrl;

/*----------------------------------------------*
 * constant variable                            *
 *----------------------------------------------*/
extern STATUS raidSendReq(FS_REQ_S *vpstReq);
extern BLK_REQ_S* blkGetReq(VOID);
extern STATUS blkRWSector(BLK_REQ_S *vpstReq);
extern VOID fsListAllVol(VOID);

/*----------------------------------------------*
 * macro definition                             *
 *----------------------------------------------*/

/*****************************************************************************
 function     : blkExit
 description  : block layer Exit
*****************************************************************************/
STATUS blkExit(VOID)
{
    BLK_REQ_S *pstReq = NULL;
    UINT32     uiLoop = 0;
    
    for(uiLoop = 0;uiLoop < BLK_MAX_REQ;uiLoop++)
    {
        pstReq = &gstBlkCtrl.astReq[uiLoop];
        if(NULL == pstReq->pucAlloc)
        {
            free(pstReq->pucAlloc);
        }
    }

    return OK;
}
/*****************************************************************************
 function     : blkFreeReq
 description  : blk layer free request
*****************************************************************************/
VOID blkFreeDev(BLK_DEV_S *pstDev)
{
    FAST int   oldlevel;
    
    oldlevel = intLock ();
    
    pstDev->uiStatus = BLK_STATUS_FREE;

    intUnlock (oldlevel);
}

/*****************************************************************************
 function     : blkGetReq
 description  : blk layer get device request
*****************************************************************************/
BLK_DEV_S* blkGetDev(VOID)
{
    BLK_DEV_S  *pstDev = NULL;
    UINT32      uiLoop = 0;
    FAST int    oldlevel = 0;
    
    oldlevel = intLock ();
        
    for(uiLoop = BLK_MAX_DISK;uiLoop < BLK_MAX_DEV;uiLoop++)
    {
        pstDev = &gstBlkCtrl.astDev[uiLoop];
        if(BLK_REQ_FREE == pstDev->uiStatus)
        {
            pstDev->uiStatus = BLK_DEV_NOT_READY;
            intUnlock (oldlevel);
            FS_LOG(FS_DBG_ERR,"get device id %d\n",uiLoop,0,0,0,0,0);
            return pstDev;
        }
    }

    intUnlock (oldlevel);
    
    FS_LOG(FS_DBG_ERR,"get device failed\n",0,0,0,0,0,0);
    
    return NULL;
}

/*****************************************************************************
 function     : blkInit
 description  : block layer init
*****************************************************************************/
STATUS blkInit(VOID)
{
    BLK_REQ_S *pstReq = NULL;
    BLK_DEV_S *pstDev = NULL;
    UINT32     uiLoop = 0;
    
    memset(&gstBlkCtrl,0,sizeof(BLK_CTRL_S));
    if(OK != fsInit())
    {
        return ERROR;
    }
    fsRegFunc(raidSendReq);

    for(uiLoop = 0;uiLoop < BLK_MAX_REQ;uiLoop++)
    {
        pstReq = &gstBlkCtrl.astReq[uiLoop];
        pstReq->uiFlag = BLK_REQ_FREE;
        pstReq->stReqSem = semBCreate(SEM_Q_PRIORITY,SEM_EMPTY);
        pstReq->pucAlloc = cacheDmaMalloc(BLK_STRIPE_SIZE);
        if(NULL == pstReq->pucAlloc)
        {
            goto ErrExit;
        }
    }
    
    for(uiLoop = 0;uiLoop < BLK_MAX_DEV;uiLoop++)
    {
        pstDev = &gstBlkCtrl.astDev[uiLoop];
        pstDev->uiStatus = BLK_STATUS_FREE;
        pstDev->uiPhyNum = BLK_INVALID_PHY;
        pstDev->uiDevNo = uiLoop;
    }
    
    gstBlkCtrl.stBlkMem = semBCreate(SEM_Q_PRIORITY,SEM_FULL);

    return OK;
    
ErrExit:
    blkExit();
    return ERROR;
}

UINT32 guiPhyLinkMap = 0;

 /*****************************************************************************
  function     : blkDevAdd
  description  : block device create
 *****************************************************************************/
 STATUS blkDevAdd(UINT64_S vullSec,UINT32  vuiBytePerSec,UINT32 vuiPhyId)
 {
     BLK_DEV_S  *pstDev = NULL;
     
     if(vuiPhyId >= BLK_MAX_DISK)
     {
         FS_LOG(FS_DBG_ERR,"device id %d overflow\n",vuiPhyId,0,0,0,0,0);
         return ERROR;
     }

     pstDev = &gstBlkCtrl.astDev[vuiPhyId];
     pstDev->uiPhyNum = vuiPhyId;
     pstDev->uiStatus = BLK_DEV_WAIT_MOUNT;
     UINT64_SET(pstDev->ullSec,vullSec);
     pstDev->uiBytePerSec = vuiBytePerSec;
     pstDev->uiDevType = BLK_TYPE_DISK;
     memset(&pstDev->stRaid,0,sizeof(BLK_RAID_S));
     if(pstDev->ullSec.lo > 0x100000)
     {
        guiPhyLinkMap |= (1 << vuiPhyId);
     }
     
     FS_LOG(FS_DBG_ERR,"device %d phy %d 0x%x%.8x sector\n",pstDev->uiDevNo,
            pstDev->uiPhyNum,pstDev->ullSec.hi,pstDev->ullSec.lo,0,0);
     
     return OK;
 }


UINT32 blkGetLinkPhyMap(void)
{
    return guiPhyLinkMap;
}

 /*****************************************************************************
  function     : checkDiskStatus
  description  : check disk status
 *****************************************************************************/
 STATUS checkDiskStatus(BLK_PART_S *pstPart,UINT32 uiMByte)
 {
    BLK_DEV_S *pstDev = NULL;
    UINT32     uiLoop = 0;
    UINT32     uiCap = 0;
    UINT32     uiMem = 0;

    if(BLK_PART_FLAG != pstPart->uiFlag)
    {
        FS_LOG(FS_DBG_ERR,"partition flag error\n",0,0,0,0,0,0);
        return ERROR;
    }

    for(uiLoop = 0;uiLoop < pstPart->uiMemNum;uiLoop++)
    {
        uiMem = pstPart->aucMem[uiLoop];
        pstDev = &gstBlkCtrl.astDev[uiMem];
        BLK_GET_MBYTE(uiCap,pstDev->ullSec);
        
        if((uiCap < (uiMByte / pstPart->uiMemNum)) || 
           (BLK_DEV_WAIT_MOUNT != pstDev->uiStatus) ||
           (BLK_PART_FREE != pstDev->auiPartStatus[pstPart->uiPartNo]))
        {
            FS_LOG(FS_DBG_ERR,"check disk %d status %x cap %d part %d cap %d flag %d "
                   "failed\n",pstDev->uiDevNo,pstDev->uiStatus,uiCap,
                   pstPart->uiPartNo,
                   uiMByte,pstDev->auiPartStatus[pstPart->uiPartNo]);
            return ERROR;
        }
    }

    if(0x0 == pstPart->uiMemNum)
    {
        FS_LOG(FS_DBG_ERR,"no member in this raid\n",0,0,0,0,0,0);
        return ERROR;
    }

    return OK;
 }

/*****************************************************************************
function     : blkRdWrPartInfo
description  : write partition information
*****************************************************************************/
STATUS blkRdWrPartInfo(UINT32  uiPhyId,UINT8 *pucBuf,UINT32 uiOper)
{
    BLK_REQ_S  *pstReq = NULL;

    pstReq = blkGetReq();
    if(NULL == pstReq)
    {
        return ERROR;
    }
    pstReq->uiSecHi = 0;
    pstReq->uiSecLo = BLK_PART_SEC_ID;
    pstReq->uiOper = uiOper;
    pstReq->pucBuf = pucBuf;
    pstReq->uiByte = BLK_PART_BYTE;
    pstReq->uiSecOff = 0;
    pstReq->uiSecNum = 1;
    pstReq->uiPhyId = uiPhyId;
	if((FS_OPER_WRITE == uiOper) && (NULL != gstBlkCtrl.pfFormat))
	{
		gstBlkCtrl.pfFormat(uiPhyId);
	}
    if(OK != blkRWSector(pstReq))
    {
        blkFreeReq(pstReq);
        return ERROR;
    }
    semTake(pstReq->stReqSem,WAIT_FOREVER);
    blkFreeReq(pstReq);
    
    return OK;
}

UINT32 blkGetPartClu(UINT8 *pucBuf)
{
	UINT32       uiMaxClu = 0;
#ifdef 	FS_DISK_MULTI_PART
	BLK_PART_S  *pstPart = NULL;
	UINT32       uiPart = 0;

	pstPart = pucBuf;
	for(uiPart = 0;uiPart < FS_MAX_PART_PER_DISK;uiPart++)
	{
		if(BLK_PART_FLAG == pstPart->uiFlag)
		{
			uiMaxClu = MAX(pstPart->uiEndClu,uiMaxClu);
		}
		pstPart++;
	}

	FS_LOG(FS_DBG_ERR,"get part max clu %d part len %d\n",uiMaxClu,
	       sizeof(BLK_PART_S),0,0,0,0,0);
#endif
	return uiMaxClu;
}

UINT32 blkGetFreePart(UINT8 *pucBuf)
{
    UINT32       uiPart = 0;
#ifdef FS_DISK_MULTI_PART
	BLK_PART_S  *pstPart = NULL;
	
	pstPart = pucBuf;
	for(uiPart = 0;uiPart < FS_MAX_PART_PER_DISK;uiPart++)
	{
		if(BLK_PART_FLAG != pstPart->uiFlag)
		{
			break;
		}
		pstPart++;
	}

	FS_LOG(FS_DBG_ERR,"get free part %d \n",uiPart,0,0,0,0,0,0);
#endif

	return uiPart;
}

/*****************************************************************************
function     : blkFillPart
description  : fill partition information
*****************************************************************************/   
STATUS blkFillPart(BLK_PART_S *pstPart,CHAR *pucName,UINT32 uiMByte,
                   UINT32 uiDevmap)
{
    UINT32         uiLen = 0;
    UINT32         uiMemIdx = 0;
    UINT32         uiLoop = 0;
    UINT32         uiCluCnt = 0;
    STATUS         iRet = ERROR;
    UINT8         *pucBuf = NULL;
	UINT32         uiFreePart = 0;
	UINT32         uiMaxClu = 0;
	UINT32         uiMaxCap = 0;

    for(uiLoop = 0;uiLoop < BLK_MAX_DEV;uiLoop++)
    {
        if(0 != (uiDevmap & (1 << uiLoop)))
        {
            pstPart->aucMem[uiMemIdx] = gstBlkCtrl.astDev[uiLoop].uiPhyNum;
            uiMaxCap = gstBlkCtrl.astDev[uiLoop].ullSec.lo >> 11;/*MByte*/
            uiMemIdx++;
        }
    }
    pstPart->uiMemNum = uiMemIdx;
    pstPart->uiFlag = BLK_PART_FLAG;
    pucBuf = malloc(BLK_PART_BYTE);
    if(NULL == pucBuf)
    {
        FS_LOG(FS_DBG_ERR,"malloc mem failed line %d\n",__LINE__,0,0,0,0,0);
        return ERROR;
    }
    memset(pucBuf,0,BLK_PART_BYTE);
    iRet = blkRdWrPartInfo(pstPart->aucMem[0],pucBuf,FS_OPER_READ);
	uiFreePart = blkGetFreePart(pucBuf);
	pstPart->uiPartNo = uiFreePart;
	assert(uiFreePart < FS_MAX_PART_PER_DISK);
    if(OK != checkDiskStatus(pstPart,uiMByte))
    {
    	free(pucBuf);
        return ERROR;
    }
    
    uiLen = MIN(strlen(pucName),FS_PART_NAME_LEN);
    memcpy(pstPart->aucName,pucName,uiLen);
    pstPart->uiCluLen = uiMemIdx * BLK_STRIPE_SIZE;
    pstPart->uiStartClu = blkGetPartClu(pucBuf) + 1;
    uiCluCnt = uiMByte / (pstPart->uiCluLen / (1024 * 1024));
    pstPart->uiEndClu = pstPart->uiStartClu + uiCluCnt;
    
    if(pstPart->uiEndClu > uiMaxCap)
    {
		FS_LOG(FS_DBG_ERR,"check part endclu %d max cap %d failed\n",
		       pstPart->uiEndClu,uiMaxCap,0,0,0,0);
		free(pucBuf);       
		return ERROR;       
    }
    pstPart->uiType = BLK_TYPE_RAID;
    if(1 == pstPart->uiMemNum)
    {
        pstPart->uiType = BLK_TYPE_DISK;
    }
	memcpy((pucBuf + (uiFreePart * sizeof(BLK_PART_S))),pstPart,sizeof(BLK_PART_S));
    for(uiLoop = 0;uiLoop < pstPart->uiMemNum;uiLoop++)
    {
        iRet = blkRdWrPartInfo(pstPart->aucMem[uiLoop],pucBuf,FS_OPER_WRITE);
    }

    free(pucBuf);
    
    return iRet;
}

/*****************************************************************************
  function     : blkFindDevByRaidMap
  description  : find device through raid map
 *****************************************************************************/
 BLK_DEV_S* blkFindDevByDevMap(UINT32 uiDevMap)
 {
     UINT32 uiLoop = 0;

     for(uiLoop = 0;uiLoop < BLK_MAX_DEV;uiLoop++)
     {
        if(0 != (uiDevMap & (1 << uiLoop)))
        {
            return &gstBlkCtrl.astDev[uiLoop];
        }
     }

     return NULL;
 }
 
 /*****************************************************************************
  function     : blkFillDev
  description  : fill device information
 *****************************************************************************/   
 STATUS blkFillDev(BLK_PART_S *pstPart,BLK_DEV_S *pstDev)
 {
    UINT32 uiLoop = 0;
    UINT32 uiIdx = 0;

    pstDev->stRaid.uiMemNum = pstPart->uiMemNum;
    for(uiLoop = 0;uiLoop < pstPart->uiMemNum;uiLoop++)
    {
        uiIdx = pstPart->aucMem[uiLoop];
        pstDev->stRaid.aucMem[uiLoop] = uiIdx;
        /*gstBlkCtrl.astDev[uiIdx].uiStatus = BLK_STATUS_READY;*/
    }
    pstDev->stRaid.uiStripSec = BLK_STRIPE_SIZE / 512;
    pstDev->uiDevType = pstPart->uiType;
    pstDev->uiStatus = BLK_DEV_WAIT_MOUNT;
    
    return OK;
 }

/*****************************************************************************
function     : blkFillVolByPart
description  : fill volume(partition)
*****************************************************************************/ 
VOID blkFillVolByPart(FS_VOL_S *pstVol,BLK_PART_S *pstPart,UINT32 uiDevId)
{
    pstVol->uiDevId = uiDevId;
    memcpy(pstVol->aucPartName,pstPart->aucName,FS_PART_NAME_LEN);
    pstVol->uiStartClu = pstPart->uiStartClu;
    pstVol->uiFatClu = pstPart->uiStartClu + FS_RESV_CLU_NUM;
    pstVol->uiCluByte = pstPart->uiCluLen;
    pstVol->uiVolClu = pstPart->uiEndClu;
    pstVol->uiCluPerFat = ((pstVol->uiVolClu * 4)/pstVol->uiCluByte) + 1;
    pstVol->uiRootClu = pstVol->uiFatClu + pstVol->uiCluPerFat + 1;
    pstVol->uiFDTCluCnt = (pstPart->uiEndClu - pstPart->uiStartClu)/8192;
    pstVol->uiFDTCluCnt = MIN(FS_MAX_FDT_CLU_CNT,pstVol->uiFDTCluCnt);
    pstVol->uiFDTCnt = pstVol->uiFDTCluCnt * (pstVol->uiCluByte / 512);

    printf("vol %s fatclu %d clubyte %d volclu %d cluperfat %d root %d FDTClu "
           "%d FDTCnt %d\n",pstVol->aucPartName,pstVol->uiFatClu,
           pstVol->uiCluByte,pstVol->uiVolClu,pstVol->uiCluPerFat,
           pstVol->uiRootClu,pstVol->uiFDTCluCnt,pstVol->uiFDTCnt);
} 

 /*****************************************************************************
  function     : blkSetPartStatus
  description  : set partition status
 *****************************************************************************/
 VOID blkSetPartStatus(BLK_DEV_S  *vpstDev,UINT32 vuiPartNo,UINT32 vuiFlag)
 {
     UINT32 uiFirstMem = 0;

	 assert(vuiPartNo < FS_MAX_PART_PER_DISK);
	 
     if(BLK_TYPE_DISK == vpstDev->uiDevType)
     {
		vpstDev->auiPartStatus[vuiPartNo] = vuiFlag;
		return;
     }
     
	 assert(vpstDev->stRaid.aucMem[0] < BLK_MAX_DEV);
	 uiFirstMem = vpstDev->stRaid.aucMem[0];
	 gstBlkCtrl.astDev[uiFirstMem].auiPartStatus[vuiPartNo] = vuiFlag;
 }
 
/*****************************************************************************
function     : addvol
description  : add new volume
*****************************************************************************/
STATUS addvol(CHAR *pucName,UINT32 uiMByte,UINT32 uiDevmap)
{
    FS_VOL_S  *pstVol = NULL;
    BLK_DEV_S *pstDev = NULL;
    BLK_PART_S stPart;

    if(uiMByte < 8)
    {
        FS_LOG(FS_DBG_ERR,"part minimum 8M\n",0,0,0,0,0,0);
        return ERROR;
    }
    memset(&stPart,0,sizeof(BLK_PART_S));
    if(OK != blkFillPart(&stPart,pucName,uiMByte,uiDevmap))
    {
        return ERROR;
    }
    if(BLK_TYPE_DISK == stPart.uiType)
    {
        pstDev = blkFindDevByDevMap(uiDevmap);
    }
    else
    {
        pstDev = blkGetDev();
    }
    if(NULL == pstDev)
    {
        FS_LOG(FS_DBG_ERR,"get free device failed line %d\n",__LINE__,0,0,0,0,0);
        return ERROR;
    }
    
    if(OK != blkFillDev(&stPart,pstDev))
    {
        return ERROR;
    }
    
    pstVol = fsAllocVol();
    if(NULL == pstVol)
    {
        FS_LOG(FS_DBG_ERR,"alloc volume failed line %d\n",__LINE__,0,0,0,0,0);
        return ERROR;
    }
  	
    blkFillVolByPart(pstVol,&stPart,pstDev->uiDevNo);
    if(OK != fsAddVol(pstVol))
    {
        fsFreeVol(pstVol);
        return ERROR;
    }
    
    if(OK != fsMountVol(pstVol))
    {
        fsFreeVol(pstVol);
        return ERROR;
    }
  	blkSetPartStatus(pstDev,stPart.uiPartNo,BLK_PART_MOUNT);
   
    return OK;
}

/*****************************************************************************
function     : mountvol
description  : mount volume(partition)
*****************************************************************************/
STATUS mntvol(UINT32 vuiDevId)
{
    FS_VOL_S   *pstVol = NULL;
    BLK_PART_S *pstPart = NULL;
    BLK_DEV_S  *pstDev = NULL;
    UINT8      *pucBuf = NULL;
    UINT32      uiCap = 0;
    UINT32      uiPart = 0;
    BOOL        bMount = FALSE;
    UINT32      uiMaxPart = 1;

    pucBuf = malloc(BLK_PART_BYTE);
    if(NULL == pucBuf)
    {
        return ERROR;
    }
    memset(pucBuf,0,BLK_PART_BYTE);

    pstDev = &gstBlkCtrl.astDev[vuiDevId];
 
    if(OK != blkRdWrPartInfo(pstDev->uiPhyNum,pucBuf,FS_OPER_READ))
    {
        free(pucBuf);
        return ERROR;
    }

    pstPart = (BLK_PART_S*)pucBuf;
#ifdef  FS_DISK_MULTI_PART  
	uiMaxPart = FS_MAX_PART_PER_DISK;
#endif
	for(uiPart = 0;uiPart < uiMaxPart;uiPart++)
    {
    	if(BLK_PART_FLAG != pstPart->uiFlag)
    	{
			pstPart++;
			continue;
    	}
	    uiCap = (pstPart->uiCluLen >> 20) * (pstPart->uiEndClu - pstPart->uiStartClu);
        printf("part capacity %d MByte clulen %d Byte\n",uiCap,pstPart->uiCluLen);
        
        if(OK != checkDiskStatus(pstPart,uiCap))
        {
            free(pucBuf);
            return ERROR;
        }
        
        if(BLK_TYPE_RAID == pstPart->uiType)
        {
            pstDev = blkGetDev();
        }
        if(NULL == pstDev)
        {
            free(pucBuf);
            return ERROR;
        }
        
        if(OK != blkFillDev(pstPart,pstDev))
        {
            free(pucBuf);
            return ERROR;
        }
        
        pstVol = fsAllocVol();
        if(NULL == pstVol)
        {
            free(pucBuf);
            return ERROR;
        }
        
        blkFillVolByPart(pstVol,pstPart,pstDev->uiDevNo);
        if(OK != fsRecoverVol(pstVol))
        {
            free(pucBuf);
            fsFreeVol(pstVol);
            return ERROR;
        }
        fsFdtInit(pstVol);
        if(OK != fsMountVol(pstVol))
        {
            free(pucBuf);
            fsFreeVol(pstVol);
            return ERROR;
        }
        pstPart++;
        pstVol->uiPartNo = uiPart;
        blkSetPartStatus(pstDev,uiPart,BLK_PART_MOUNT);
        bMount = TRUE;
    }

    free(pucBuf);
    if(TRUE == bMount)
    {
		return OK;
    }
    return ERROR;
}

 /*****************************************************************************
 function     : delvol
 description  : delete volume
 *****************************************************************************/
 STATUS delvol(CHAR *pucName)
 {
    FS_VOL_S   *pstVol = NULL;
    BLK_DEV_S  *pstDev = NULL;
    UINT8      *pucBuf = NULL;
    UINT32      uiLoop = 0;
	UINT32      uiDevId = 0;
	INT32       iRet = 0;

    pstVol = fsFindVolByName(pucName);
    if(NULL == pstVol)
    {
        return ERROR;
    }
    assert(pstVol->uiDevId < BLK_MAX_DEV);
    pstDev = &gstBlkCtrl.astDev[pstVol->uiDevId];
    pucBuf = malloc(BLK_PART_BYTE);
    if(NULL == pucBuf)
    {
        FS_LOG(FS_DBG_ERR,"malloc memory failed %d\n",__LINE__,0,0,0,0,0);
        return ERROR;
    }
    iRet = blkRdWrPartInfo(pstDev->stRaid.aucMem[uiLoop],pucBuf,FS_OPER_READ);
    memset((pucBuf + (pstVol->uiPartNo * sizeof(BLK_PART_S))),0,sizeof(BLK_PART_S));
    for(uiLoop = 0;uiLoop < pstDev->stRaid.uiMemNum;uiLoop++)
    {
        if(OK != blkRdWrPartInfo(pstDev->stRaid.aucMem[uiLoop],pucBuf,FS_OPER_WRITE))
        {
            FS_LOG(FS_DBG_ERR,"delete dev %d part information failed\n",
                    pstDev->stRaid.aucMem[uiLoop],0,0,0,0,0);
            free(pucBuf);
            return ERROR;
        }
		uiDevId = pstDev->stRaid.aucMem[uiLoop];
    }
    blkSetPartStatus(pstDev,pstVol->uiPartNo,BLK_PART_FREE);
    if(OK != fsUnMountVol(pstVol))
    {
        free(pucBuf);
        return ERROR;
    }

    if(BLK_TYPE_DISK == pstDev->uiDevType)
    {
        pstDev->uiStatus = BLK_DEV_WAIT_MOUNT;
    }
    else
    {
        blkFreeDev(pstDev);
    }
    
    memset(&pstDev->stRaid,0,sizeof(BLK_RAID_S));
    fsFreeVol(pstVol);
    free(pucBuf);

    return OK;
 }

 /*****************************************************************************
   function     : diskinit
   description  : disk initiazation
  *****************************************************************************/
 STATUS blkFormat(UINT32 uiDevmap)
 {
    UINT32     uiLoop = 0;
    UINT32     uiCap = 0;
    UINT32     uiMem = 0;

    for(uiLoop = 0;uiLoop < BLK_MAX_DEV;uiLoop++)
    {
        if(0 != (uiDevmap & (1 << uiLoop)))
        {
            if(OK != gstBlkCtrl.pfFormat(gstBlkCtrl.astDev[uiLoop].uiPhyNum))
            {
                FS_LOG(FS_DBG_ERR,"format disk %d failed\n",uiLoop,0,0,0,0,0);
                return ERROR;
            }
			FS_LOG(FS_DBG_ERR,"format disk %d ok\n",uiLoop,0,0,0,0,0);
        }
    }
    
    return OK;
 }

 /*****************************************************************************
  function     : lsvol
  description  : list volume information
 *****************************************************************************/
 VOID lsvol(VOID)
 {
    fsListAllVol();
 }

 /*****************************************************************************
  function     : formatvol
  description  : format vol
 *****************************************************************************/
 STATUS formatvol(CHAR *vpcName)
 {
    FS_VOL_S   *pstVol = NULL;

    pstVol = fsFindVolByName(vpcName);
    if(NULL == pstVol)
    {
        return ERROR;
    }

    return fsFormatVol(pstVol);
 }
 
 /*****************************************************************************
  function     : lsdev
  description  : list disk information
 *****************************************************************************/
 VOID lsdev(VOID)
 {
    BLK_DEV_S *pstDev = NULL;
    UINT32     uiLoop = 0;
    UINT32     uiMem = 0;

    printf("dev          id     phy id     type     phy member\n");
    
    for(uiLoop = 0;uiLoop < BLK_MAX_DEV;uiLoop++)
    {
        pstDev = &gstBlkCtrl.astDev[uiLoop];
        if(BLK_DEV_WAIT_MOUNT == pstDev->uiStatus)
        {
            printf("0x%x",pstDev);
            printf("   %2d       0x%2x     ",pstDev->uiDevNo,pstDev->uiPhyNum);
            if(BLK_TYPE_DISK == pstDev->uiDevType)
            {
                printf(" disk     0");
            }
            else
            {
                printf(" raid     ",pstDev->uiDevNo);
                for(uiMem = 0;uiMem < pstDev->stRaid.uiMemNum;uiMem++)
                {
                    if(0 != uiMem)
                    {
                        printf(",");
                    }
                    printf("%d",pstDev->stRaid.aucMem[uiMem]);
                }
            }
            printf("\n");
        }
    }
 }

/*****************************************************************************
 function     : lsdir
 description  : list dir information
*****************************************************************************/
STATUS lsdir(CHAR *vpcVolName,CHAR *vpcDirName)
{
    return fsPrintDir(vpcVolName,vpcDirName);
}

/*****************************************************************************
function     : file system command help
description  : help information information
*****************************************************************************/
VOID lshelp(VOID)
{
    printf("lsdev                          list device information\n");
    printf("lsvol                          list volume information\n");
    printf("lsdir  volname dirname         list directory information\n");
    printf("addvol volname Mbyte devmap    add vol on device bitmap\n");
    printf("delvol volname                 delete vol\n");
    printf("mntvol devid                   get volume information of device\n");
    printf("formatvol volname              format volume\n\n");
    
    printf("cd     directory               change current directory\n");
    printf("ls    [directory]              list directory file name\n");
    printf("mkdir  directory               creat a new directory\n");
    printf("creat  filename                creat a new file\n");
    printf("remove dir/file                delete directory or file\n");
    printf("rename src dest                rename one file or dir\n");
    printf("mv src dest                    move file or directory\n");
}

