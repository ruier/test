/******************************************************************************

                  VERSION (C), 2001-2011, 

 ******************************************************************************
  file       : fsBuffer.c
  version    : 1.0
  author     : lgh
  data       : 2012.9.4
  function   : file system buffer management 
******************************************************************************/

/*----------------------------------------------*
 * header file                                  *
 *----------------------------------------------*/
#include "fsInc.h"

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
 function     : fsFreeReq
 description  : fs system free device request
*****************************************************************************/
VOID fsFreeReq(FS_VOL_S *pstVol,FS_REQ_S *pstReq)
{
    semTake(pstVol->stReqSem,WAIT_FOREVER);
    pstReq->uiFlag = FS_REQ_FREE;
    pstReq->pfDone = NULL;
    pstReq->uiOper = FS_OPER_NULL;
    pstReq->pucBuf = NULL;
    semGive(pstVol->stReqSem);
}

/*****************************************************************************
 function     : blkGetReq
 description  : fs system get device request
*****************************************************************************/
FS_REQ_S* fsGetReq(FS_VOL_S *pstVol)
{
    FS_REQ_S  *pstReq = NULL;
    UINT32     uiLoop = 0;

    semTake(pstVol->stReqSem,WAIT_FOREVER);
    
    for(uiLoop = 0;uiLoop < FS_MAX_REQ_PER_VOL;uiLoop++)
    {
        pstReq = &pstVol->astReq[uiLoop];
        if(FS_REQ_FREE == pstReq->uiFlag)
        {
            pstReq->uiFlag = FS_REQ_USED;
            semGive(pstVol->stReqSem);
            return pstReq;
        }
    }
    semGive(pstVol->stReqSem);

    return NULL;
}

/*****************************************************************************
function     : fsSecRWDone
description  : sector read or write completion 
*****************************************************************************/
STATUS fsCluRWDone(FS_REQ_S *vpstFsReq)
{
    semGive(vpstFsReq->stReqSem);

    return OK;
}

extern SEM_ID gstSingal;

/*****************************************************************************
function     : fsCluRWAlign
description  : read/write some byte of one cluster
*****************************************************************************/
STATUS fsCluRW(FS_VOL_S *vpstVol,UINT32 vuiClu,UINT32 vuiLen,UINT32 vuiOffset,
               UINT8   *vpucBuf,UINT32 vuiOper)
{
    FS_REQ_S  *pstReq = NULL;

    assert(vuiClu <= vpstVol->uiVolClu);
    
    if(0x0 == vuiClu)
    {
        printf("fsCluRW failed\n");
        return ERROR;
    }
    
	if((vuiLen + vuiOffset) > vpstVol->uiCluByte)
	{
		FS_LOG(FS_DBG_INFO,"fsCluRW offset %d len %d clu %d error\n",vuiOffset,vuiLen,
			   vpstVol->uiCluByte,0,0,0);
		return ERROR;
	}
    
    pstReq = fsGetReq(vpstVol);
    if(NULL == pstReq)
    {
        FS_LOG(FS_DBG_INFO,"fsSecRW get request failed\n",0,0,0,0,0,0);
         /*to do*/
        goto ErrExit;
    }
    
    pstReq->uiVolId = vpstVol->uiVolId;
    pstReq->uiOper = vuiOper;
    pstReq->pfDone = fsCluRWDone;
    pstReq->uiClu = vuiClu;
    pstReq->uiBytes = vuiLen;
	pstReq->uiOffset = vuiOffset;
    pstReq->pucBuf = vpucBuf;
    pstReq->uiDevId = vpstVol->uiDevId;

    if(NULL == gstFsCtrl.pfCluRw)
    {
        goto ErrExit;
    }
    
    if(OK != gstFsCtrl.pfCluRw(pstReq))
    {
        FS_LOG(FS_DBG_ERR,"sector read/write vol %d 0x%x byte failed\n",
							               vpstVol->uiVolId,pstReq->uiBytes,0,0,0,0);   
        goto ErrExit;
    }
#if 0    
    if(OK != semTake(pstReq->stReqSem,WAIT_FOREVER))
    {
         /*todo*/
         FS_LOG(FS_DBG_ERR,"wait read/write timeout\n",0,0,0,0,0,0); 
         goto ErrExit;
    }
#endif
    fsFreeReq(vpstVol,pstReq);
    
    return OK;
    
ErrExit:
    FS_LOG(FS_DBG_ERR,"dev %d send request failed\n",vpstVol->uiVolId,0,0,0,0,0); 
    fsFreeReq(vpstVol,pstReq);
    return ERROR;
}

