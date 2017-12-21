/******************************************************************************

                  VERSION (C), 2001-2011, 

 ******************************************************************************
  file       : blkReq.c
  version    : 1.0
  author     : lgh
  data       : 2012.9.4
  function   : block request process 
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

/*----------------------------------------------*
 * internal  function                           *
 *----------------------------------------------*/
STATUS blkRWSector(BLK_REQ_S *vpstReq);

/*----------------------------------------------*
 * global  variable                             *
 *----------------------------------------------*/

extern BLK_CTRL_S  gstBlkCtrl;

/*----------------------------------------------*
 * constant variable                            *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macro definition                             *
 *----------------------------------------------*/

/*****************************************************************************
 function     : fsRegFunc
 description  : fs system function register
*****************************************************************************/
INT32 blkRegFunc(BLK_SEC_RW_F pfFunc,BLK_FORMAT_F pfFormat)
{
    gstBlkCtrl.pfSecRw = pfFunc;
    gstBlkCtrl.pfFormat = pfFormat;
    /*logMsg("func sec rw 0x%x\n",(UINT32)gstBlkCtrl.pfSecRw,0,0,0,0,0);*/
    
    return OK;
}

/*****************************************************************************
 function     : blkFreeReq
 description  : blk layer free request
*****************************************************************************/
VOID blkFreeReq(BLK_REQ_S *pstReq)
{
    FAST int   oldlevel;
    UINT32     uiLoop = 0;
    
    oldlevel = intLock ();
    
    pstReq->uiFlag = BLK_REQ_FREE;
    pstReq->pfDone = NULL;
    pstReq->uiOper = FS_OPER_NULL;
    pstReq->pucBuf = NULL;
    pstReq->uiSecHi = 0;
    pstReq->uiSecLo = 0;
    pstReq->uiSecNum = 0;
    pstReq->uiReqNum = 0;
    pstReq->uiDoneNum = 0;
    pstReq->uiPhyId = BLK_INVALID_PHY;
    pstReq->uiSecOff =0;
    pstReq->pstOrgReq = NULL;
    pstReq->uiByte = 0;
    pstReq->pucOrgBuf = NULL;
    pstReq->uiStatus = 0;
    for(uiLoop = 0;uiLoop < BLK_RAID_MAX_NUM;uiLoop++)
    {
        pstReq->apstSubReq[uiLoop] = NULL;
    }
    intUnlock (oldlevel);
}

/*****************************************************************************
 function     : blkFreeRaidReq
 description  : blk layer free raid request
*****************************************************************************/
VOID blkFreeRaidReq(BLK_REQ_S *pstReq)
{
    UINT32 uiLoop = 0;

    for(uiLoop = 0;uiLoop < BLK_RAID_MAX_NUM;uiLoop++)
    {
        if(NULL != pstReq->apstSubReq[uiLoop])
        {
            blkFreeReq(pstReq->apstSubReq[uiLoop]);
        }
    }
    blkFreeReq(pstReq);
}

/*****************************************************************************
 function     : blkGetReq
 description  : blk layer get device request
*****************************************************************************/
BLK_REQ_S* blkGetReq(VOID)
{
    BLK_REQ_S  *pstReq = NULL;
    UINT32      uiLoop = 0;
    FAST int   oldlevel;
    
    oldlevel = intLock ();
        
    for(uiLoop = 0;uiLoop < BLK_MAX_REQ;uiLoop++)
    {
        pstReq = &gstBlkCtrl.astReq[uiLoop];
        if(BLK_REQ_FREE == pstReq->uiFlag)
        {
            pstReq->uiFlag = BLK_REQ_USED;
            pstReq->uiReqNum = 0;
            pstReq->uiDoneNum = 0;
            pstReq->pstOrgReq = NULL;
			/*semGive(gstBlkCtrl.stBlkMem); */
            intUnlock (oldlevel);
            return pstReq;
        }
    }

    intUnlock (oldlevel);
    
	/*semGive(gstBlkCtrl.stBlkMem); */
    return NULL;
}

/*****************************************************************************
 function     : blkGetStartSec
 description  : blk layer get start sec
*****************************************************************************/
VOID blkGetStartSec(UINT32 uiClu,UINT32 uiOffset,UINT32 *puiSecHi,UINT32 *puiSecLo)
{
    UINT32 uiSecHi = 0;
    UINT32 uiSecLo = 0;
    UINT32 uiSecNum = 0;

    uiSecHi = BLK_GET_SEC_HI(uiClu);
    uiSecLo = BLK_GET_SEC_LO(uiClu);
    uiSecNum = (uiOffset % BLK_STRIPE_SIZE) / BLK_BYTE_PER_SEC;
    uiSecLo += uiSecNum;
    if((uiSecLo + uiSecNum) < uiSecLo)
    {
        uiSecHi++;        
    }
    *puiSecHi = uiSecHi;
    *puiSecLo = uiSecLo;
}

/*****************************************************************************
 function     : blkGetSecNum
 description  : blk layer get sec number
*****************************************************************************/
UINT32 blkGetSecNum(UINT32 uiOffset,UINT32 uiByte)
{
    UINT32 uiSecNum = 0;
    UINT32 uiLen = 0;
    UINT32 uiRest = 0;

    uiRest = uiByte;
    if(uiOffset % BLK_BYTE_PER_SEC)
    {
        uiLen = BLK_BYTE_PER_SEC - (uiOffset % BLK_BYTE_PER_SEC);
        if(uiLen >= uiByte)
        {
            uiSecNum = 1;
            return uiSecNum;
        }
        else
        {
            uiSecNum++;
            uiRest = uiByte - uiLen;
        }
    }
    
    uiSecNum += uiRest / BLK_BYTE_PER_SEC;
    if(0x0 != (uiRest % BLK_BYTE_PER_SEC))
    {
        uiSecNum++;
    }

    return uiSecNum;
}

/*****************************************************************************
 function     : raidReqDone
 description  : blk layer request done
*****************************************************************************/
STATUS raidReqDone(BLK_REQ_S *pstBlkReq)
{
	pstBlkReq->uiDoneNum++;

    if((pstBlkReq->uiDoneNum == pstBlkReq->uiReqNum) && (NULL != pstBlkReq->pstReq))
    {
        semGive(pstBlkReq->stReqSem);
    }
    
    return OK;
}

/*****************************************************************************
 function     : raidGetMember
 description  : get raid request member
*****************************************************************************/
STATUS raidGetFirstMember(BLK_REQ_S *vpstBlkReq,UINT32 *puiMember)
{
    UINT32      uiStripeByte = 0;
    UINT32      uiMem = 0;
    BLK_DEV_S  *pstDev = NULL;
    
    pstDev = &gstBlkCtrl.astDev[vpstBlkReq->uiDevId];
    uiStripeByte = pstDev->stRaid.uiStripSec * BLK_BYTE_PER_SEC;
    uiMem = vpstBlkReq->pstReq->uiOffset / uiStripeByte;
    if(uiMem >= pstDev->stRaid.uiMemNum)
    {
        FS_LOG(FS_DBG_ERR,"split request failed\n",0,0,0,0,0,0);
        return ERROR;
    }
    
    *puiMember = uiMem;
    
    return OK;
}

/*****************************************************************************
 function     : blkRWSecDone
 description  : blk layer request done
*****************************************************************************/
STATUS blkRWSecDone(BLK_REQ_S *pstBlkReq)
{
    if(NULL != pstBlkReq->pstOrgReq)
    {
        pstBlkReq->pstOrgReq->pfDone(pstBlkReq->pstOrgReq);
    }
    else
    {
        semGive(pstBlkReq->stReqSem);
    }

    return OK;
}

/*****************************************************************************
 function     : blkRdByteDone
 description  : blk layer request done
*****************************************************************************/
STATUS blkRdByteDone(BLK_REQ_S *pstBlkReq)
{
    UINT8 *pucBuf = NULL;
    
    pucBuf = pstBlkReq->pucBuf + pstBlkReq->uiSecOff;
    memcpy(pstBlkReq->pucOrgBuf,pucBuf,pstBlkReq->uiByte);
    if(NULL != pstBlkReq->pstOrgReq)
    {
        pstBlkReq->pstOrgReq->pfDone(pstBlkReq->pstOrgReq);
    }
    else
    {
        semGive(pstBlkReq->stReqSem);
    }
    return OK;
}

/*****************************************************************************
 function     : blkRdFirstDone
 description  : blk layer request done
*****************************************************************************/
STATUS blkWrByteDone(BLK_REQ_S *pstBlkReq)
{
    UINT8 *pucBuf = NULL;

    pucBuf = pstBlkReq->pucBuf + pstBlkReq->uiSecOff;
    memcpy(pucBuf,pstBlkReq->pucOrgBuf,pstBlkReq->uiByte);
    pstBlkReq->uiOper = FS_OPER_WRITE;
    if(OK != blkRWSector(pstBlkReq))
    {
        return ERROR;
    }

    return OK;
}

/*****************************************************************************
 function     : blkRWSector
 description  : blk layer read/write sector
*****************************************************************************/
STATUS blkRWSector(BLK_REQ_S *vpstReq)
{
    if((NULL == gstBlkCtrl.pfSecRw) || (NULL == vpstReq))
    {
        logMsg("blkRWSector send req %x failed\n",(UINT32)vpstReq,0,0,0,0,0);
        return ERROR;
    }
    
    vpstReq->pfDone = blkRWSecDone;
    
    if(OK != gstBlkCtrl.pfSecRw(vpstReq))
	{
		return ERROR;
	}
    
    return OK;
}

/*****************************************************************************
 function     : blkReadByte
 description  : blk layer read bytes
*****************************************************************************/
STATUS blkReadByte(BLK_REQ_S *vpstReq)
{
    vpstReq->pfDone = blkRdByteDone;
    vpstReq->pucOrgBuf = vpstReq->pucBuf;
    vpstReq->pucBuf = vpstReq->pucAlloc;
    
    if(OK != gstBlkCtrl.pfSecRw(vpstReq))
	{
		return ERROR;
	}
    
    return OK;
}

/*****************************************************************************
 function     : blkWriteByte
 description  : blk layer write bytes
*****************************************************************************/
STATUS blkWriteByte(BLK_REQ_S *vpstReq)
{
    vpstReq->pfDone = blkWrByteDone;
    vpstReq->uiOper = FS_OPER_READ;
    vpstReq->pucOrgBuf = vpstReq->pucBuf;
    vpstReq->pucBuf = vpstReq->pucAlloc;
    
    if(OK != gstBlkCtrl.pfSecRw(vpstReq))
	{
		return ERROR;
	}

    return OK;
}

/*****************************************************************************
 function     : blkSendReq
 description  : blk layer send request
*****************************************************************************/
STATUS blkSendReq(BLK_REQ_S *vpstReq)
{
    if((0x0 == (vpstReq->uiByte % BLK_BYTE_PER_SEC)) &&
       (0x0 == vpstReq->uiSecOff))
    {
        return blkRWSector(vpstReq);
    }
    
    if(FS_OPER_READ == vpstReq->uiOper)
    {
        return blkReadByte(vpstReq);
    }

    return blkWriteByte(vpstReq);
}

/*****************************************************************************
 function     : raidSplitReq
 description  : blk layer split device request
*****************************************************************************/
STATUS raidSplitReq(BLK_REQ_S *vpstBlkReq)
{
    BLK_DEV_S  *pstDev = NULL;
    BLK_REQ_S  *pstSubReq = NULL;
    UINT32      uiReqIdx = 0;
    UINT32      uiIdx = 0;
    UINT32      uiRestSec = 0;
    UINT32      uiRestByte = 0;
    UINT32      uiSecNum = 0;
    UINT32      uiSecOff = 0;
    UINT32      uiByte = 0;
    UINT32      uiByteOff = 0;
    UINT32      uiRdWrByte = 0;
    UINT32      uiLoop = 0;
    UINT64_S    ullSec;
    
    if(OK != raidGetFirstMember(vpstBlkReq,&uiIdx))
    {
        return ERROR;
    }
    
    pstDev = &gstBlkCtrl.astDev[vpstBlkReq->uiDevId];
    uiRestSec = vpstBlkReq->uiSecNum;
    uiSecOff = vpstBlkReq->uiSecLo % pstDev->stRaid.uiStripSec;
    uiByteOff = vpstBlkReq->uiSecOff;
    uiRestByte = vpstBlkReq->uiByte;
    ullSec.hi = vpstBlkReq->uiSecHi;
    ullSec.lo = vpstBlkReq->uiSecLo;
    
    /*
     *send member request
    */
    while(uiRestSec > 0)
    {
        uiSecNum = MIN((pstDev->stRaid.uiStripSec - uiSecOff),uiRestSec);
        uiRestSec = uiRestSec - uiSecNum;
        uiByte = MIN((uiSecNum * BLK_BYTE_PER_SEC - uiByteOff),uiRestByte);
        
        if(uiIdx >= pstDev->stRaid.uiMemNum)
        {
            FS_LOG(FS_DBG_ERR,"split request %x %x overflow\n",pstDev->stRaid.uiMemNum,
                   uiIdx,0,0,0,0);
            return ERROR;
        }
        pstSubReq = blkGetReq();
        if(NULL == pstSubReq)
        {
            FS_LOG(FS_DBG_ERR,"get free block request failed\n",0,0,0,0,0,0);
            return ERROR;
        }
		
        vpstBlkReq->apstSubReq[uiReqIdx] = pstSubReq;
        pstSubReq->pfDone = NULL;
        pstSubReq->pstOrgReq = vpstBlkReq;
        vpstBlkReq->uiReqNum ++;
        pstSubReq->pucBuf = vpstBlkReq->pstReq->pucBuf + uiRdWrByte;
        pstSubReq->uiByte = uiByte;
        pstSubReq->uiSecOff = uiByteOff;
        pstSubReq->uiSecLo = ullSec.lo;
        pstSubReq->uiSecHi = ullSec.hi;
        pstSubReq->uiPhyId = pstDev->stRaid.aucMem[uiIdx];
        pstSubReq->uiSecNum = uiSecNum;
        pstSubReq->uiOper = vpstBlkReq->pstReq->uiOper;
        
        ullSec.hi = BLK_GET_SEC_HI(vpstBlkReq->pstReq->uiClu);
        ullSec.lo = BLK_GET_SEC_LO(vpstBlkReq->pstReq->uiClu);
        FS_LOG(FS_DBG_DATA,"split phy %d sec 0x%x offset %x byte %x 0x%x 0x%x\n",
               pstSubReq->uiPhyId,pstSubReq->uiSecLo,pstSubReq->uiSecOff,
               pstSubReq->uiByte,pstSubReq->uiSecNum,0);
        
        uiReqIdx++;
        uiRdWrByte += uiByte;
        uiRestByte = uiRestByte - uiByte;
        uiIdx++;
        uiSecOff = 0;
        uiByteOff = 0;
    }
    
    /*
     *send block request
    */
    for(uiLoop = 0;uiLoop < uiReqIdx;uiLoop++)
    {
        if(OK != blkSendReq(vpstBlkReq->apstSubReq[uiLoop]))
        {
            return ERROR;
        }
    }
    
    return OK;
}

/*****************************************************************************
 function     : blkGetReq
 description  : blk layer get device request
*****************************************************************************/
STATUS raidSendReq(FS_REQ_S *vpstReq)
{
    BLK_REQ_S  *pstOrgReq = NULL;
    STATUS      iRet = ERROR;

    if(vpstReq->uiDevId >= BLK_MAX_DEV)
    {
        FS_LOG(FS_DBG_ERR,"device id %d error\n",vpstReq->uiDevId,0,0,0,0,0);
        return ERROR;
    }
    
    if(0x0 == vpstReq->uiBytes)
    {
        logMsg("raidSendReq %x byte is o\n",(UINT32)vpstReq,0,0,0,0,0);
        
        return OK;
    }
    
    pstOrgReq = blkGetReq();
    if(NULL == pstOrgReq)
    {
        FS_LOG(FS_DBG_ERR,"get free block request failed\n",0,0,0,0,0,0);
        return ERROR;
    }

    blkGetStartSec(vpstReq->uiClu,vpstReq->uiOffset,&pstOrgReq->uiSecHi,
                   &pstOrgReq->uiSecLo);

    FS_LOG(FS_DBG_DATA,"send request clu 0x%x offset %x lo %x dev %x byte %d %d\n",
            vpstReq->uiClu,vpstReq->uiOffset,pstOrgReq->uiSecLo,
            vpstReq->uiDevId,vpstReq->uiBytes,vpstReq->uiOper);
    
    pstOrgReq->pstReq = vpstReq;
    pstOrgReq->uiSecNum = blkGetSecNum(vpstReq->uiOffset,vpstReq->uiBytes);
    pstOrgReq->uiByte = vpstReq->uiBytes;
    pstOrgReq->uiSecOff = vpstReq->uiOffset % BLK_BYTE_PER_SEC;
    pstOrgReq->uiDevId = vpstReq->uiDevId;
    pstOrgReq->uiOper = vpstReq->uiOper;
    
    if(BLK_TYPE_RAID == gstBlkCtrl.astDev[vpstReq->uiDevId].uiDevType)
    {
        pstOrgReq->pfDone = raidReqDone;
        iRet = raidSplitReq(pstOrgReq);
    }
    else
    {
        pstOrgReq->uiPhyId = gstBlkCtrl.astDev[vpstReq->uiDevId].uiPhyNum;
        pstOrgReq->pucBuf = vpstReq->pucBuf;
        iRet = blkSendReq(pstOrgReq);
        /*printf("send req 0x%x sec lo %x byte 0x%x clu 0x%x phy %d oper %d"
               "sec %d 0x%x\n",
               pstOrgReq,pstOrgReq->uiSecLo,pstOrgReq->uiByte,vpstReq->uiClu,
               pstOrgReq->uiPhyId,pstOrgReq->uiOper,pstOrgReq->uiSecNum,
               pstOrgReq->pucOrgBuf);*/
    }

    if(OK != iRet)
    {
        return ERROR;
    }
    
    semTake(pstOrgReq->stReqSem,WAIT_FOREVER);
    
    blkFreeRaidReq(pstOrgReq);
    
    return OK;
}

