/******************************************************************************

                  VERSION (C), 2001-2011, 

 ******************************************************************************
  file       : fsDev.c
  version    : 1.0
  author     : lgh
  data       : 2012.1.31
  function   : file system device initiation(disk register etc) 
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
extern VOID fsFreeReq(FS_VOL_S *pstVol,FS_REQ_S *pstReq);
extern STATUS fsNewFile(FS_VOL_S *,CHAR*,FS_FDT_INFO_S *,UINT32);
              
extern STATUS fsClearEntChain(FS_VOL_S  *,UINT32 );
extern STATUS fsGetFreeEnt(FS_VOL_S  *,UINT32  *);
extern UINT32 fsGetLastEnt(FS_VOL_S *,UINT32 ,UINT32  *);
extern STATUS fsReadEnt(FS_VOL_S  *,UINT32  ,UINT32  *);
extern STATUS fsWriteEnt(FS_VOL_S  *,UINT32  ,UINT32,UINT32);
extern STATUS fsFindFDT(FS_VOL_S *,CHAR *,FS_FDT_INFO_S *);
extern STATUS fsGetModWrFDT(FS_VOL_S *,CHAR *,FS_FDT_INFO_S *);
extern STATUS fsReadFDT(FS_VOL_S *,FS_FDT_S *, UINT32 ,UINT32 );
extern STATUS fsWriteFDT(FS_VOL_S *,FS_FDT_S *,UINT32 ,UINT32 );
extern STATUS fsCacheByteRead(FS_VOL_S *,UINT32 ,UINT32 ,UINT8*,UINT32);
extern STATUS fsCacheByteWrite(FS_VOL_S *,UINT32 ,UINT32 ,UINT8*,UINT32);
extern STATUS fsCacheCluRead(FS_VOL_S *,UINT32,UINT8 *);
extern STATUS fsCacheCluWrite(FS_VOL_S *,UINT32 ,UINT8 *);
extern STATUS fsFreeFatCache(FS_VOL_S *);                     
extern STATUS fsReadFatToCache(FS_VOL_S*);
extern STATUS fsGetModLastEnt(FS_VOL_S *,UINT32,UINT32 *);
extern STATUS fsCacheMon(FS_VOL_S *vpstVol);
extern VOID  fsPrintFDT(UINT8 *vpucBuf,UINT32 vuiBufLen);
extern INT32 fsOpen(FS_VOL_S *,CHAR *,UINT32,UINT32);
extern INT32 fsClose(FS_FD_S *vpstFd);
extern INT32 fsCreate(FS_VOL_S *vpstVol,CHAR *vpcFileName,UINT32 vuiFlag);
extern STATUS fsDelete(FS_VOL_S *vpstVol,CHAR *vpcFileName);
extern INT32 fsRead(FS_FD_S *vpstFd,CHAR *vpcBuf,UINT32 vuiByte);
extern INT32 fsWrite(FS_FD_S *vpstFd,CHAR *vpcBuf,UINT32 vuiByte);
extern STATUS fsIoctl(FS_FD_S *vpstFd,INT32 viFunc,INT32  viArg);

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
function     : fsStrCmp
description  : string compare
input        : vpcStr1;vpcStr2;vuiLen
output       : none
return       : return OK or ERROR
*****************************************************************************/
STATUS fsStrCmp(CHAR *vpcStr1,CHAR *vpcStr2,UINT32 vuiLen)
{
    UINT32 uiStrLoop = 0;

    for(uiStrLoop = 0; uiStrLoop < vuiLen;uiStrLoop++)
    {   
        if((*vpcStr1) != (*vpcStr2))
        {
            return ERROR;
        }
        vpcStr1++;
        vpcStr2++;
    }

    return OK;
}


/*****************************************************************************
 function     : fsGetFreeVol
 description  : fs system get free vol
*****************************************************************************/
FS_VOL_S* fsAllocVol(VOID)
{
    UINT32 uiLoop = 0;
	FS_VOL_S *pstVol = NULL;
    UINT32    uiReqNum = 0;

    for(uiLoop = 0;uiLoop < FS_MAX_VOL;uiLoop++)
    {
        if(FS_VOL_FREE == gstFsCtrl.astVol[uiLoop].uiFlag)
        {
            gstFsCtrl.astVol[uiLoop].uiFlag = FS_VOL_USED;
            return &gstFsCtrl.astVol[uiLoop];
        }
    }

   return NULL;
}

/*****************************************************************************
 function     : fsGetFreeVol
 description  : fs system get free vol
 input        : 
 output       : 
 return       : 
*****************************************************************************/
VOID fsFreeVol(FS_VOL_S *vpstVol)
{
    if(FS_VOL_FREE != vpstVol->uiFlag)
    {
        memset(vpstVol,0,sizeof(FS_VOL_S));
        vpstVol->uiFlag = FS_VOL_FREE;
        vpstVol->bMnt = FALSE;
    }
}

/*****************************************************************************
 function     : fsGetVolByName
 description  : get volume by name
*****************************************************************************/
FS_VOL_S* fsFindVolByName(CHAR *vpcName)
{
    UINT32      uiLoop = 0;
    UINT32      uiStrLen = 0;
    UINT32      uiStrLoop = 0;
    FS_VOL_S   *pstVol = NULL;
    CHAR       *pcName = NULL;
    BOOL        bFound = TRUE;

    for(uiLoop = 0;uiLoop < FS_MAX_VOL;uiLoop++)
    {
        pstVol = &gstFsCtrl.astVol[uiLoop];
        uiStrLen = MIN(strlen(vpcName) + 1,FS_MAX_NAME_LEN);
        bFound = TRUE;
        pcName = vpcName;
        
        for(uiStrLoop = 0;uiStrLoop < uiStrLen;uiStrLoop++)
        {
            if(pstVol->aucPartName[uiStrLoop] != (*pcName))
            {
                bFound = FALSE;
                break;
            }
            pcName++;
        }
        
        if(TRUE == bFound)
        {
            return pstVol;
        }
    }

    FS_LOG(FS_DBG_ERR,"found vol %s failed\n",vpcName,0,0,0,0,0);
    
    return NULL;
}

/*****************************************************************************
 function     : fsGetVolFreeCap
 description  : get volume free capacity
 input        : 
 output       : 
 return       : 
*****************************************************************************/
STATUS fsGetVolFreeClu(FS_VOL_S  *pstVol,UINT32 *puiClu)
{
    UINT32    uiLoop = 0;
    UINT32    uiCluLen = 0;
    UINT32   *puiBuf = NULL;
    UINT32    uiMinClu = 0;
    UINT32    uiMaxClu = 0;
    UINT32    uiEnt = 0;
    UINT32    uiFreeEnt = 0;
    UINT32    uiCluNum = 0;
    UINT32   *puiEnt = 0;
 
    uiCluLen = pstVol->uiCluByte;
    puiBuf = (UINT32*)malloc(uiCluLen);
    if(NULL == puiBuf)
    {
        FS_LOG(FS_DBG_ERR,"malloc memory %d failed\n",uiCluLen,0,0,0,0,0);
        return ERROR;
    }
    
    uiMaxClu = pstVol->uiFatClu + pstVol->uiCluPerFat;
    uiMinClu = pstVol->uiFatClu;
    
    for(uiLoop = uiMinClu;uiLoop < uiMaxClu;uiLoop++)
    {
        bzero((CHAR*)puiBuf,uiCluLen);
        if(OK != gstFsCtrl.stCacheFunc.fsCacheCluRead(pstVol,uiLoop,(UINT8*)puiBuf))
        {
            free(puiBuf);
            *puiClu = 0;
            return OK;
        }
        
        for(uiEnt = 0;uiEnt < (uiCluLen/4);uiEnt++)
        {
            if(uiCluNum >= pstVol->uiVolClu)
            {
                break;
            }
            if((FS_CLUST_AVAIL == puiBuf[uiEnt]) && (uiCluNum > pstVol->uiRootClu))
            {
                uiFreeEnt++;
            }
            else
            {
                FS_LOG(FS_DBG_DATA,"ent %d val %d\n",uiEnt,puiBuf[uiEnt],0,0,0,0);
            }
            uiCluNum++;
        }
    }

    FS_LOG(FS_DBG_DATA,"free ent %d\n",uiFreeEnt,0,0,0,0,0);
    *puiClu = uiFreeEnt;
    free(puiBuf);

    return OK;
}

/*****************************************************************************
 function     : fsListAllVol
 description  : list all volume information
 input        : 
 output       : 
 return       : 
*****************************************************************************/
VOID fsListAllVol(VOID)
{
    UINT32  uiLoop = 0;
    UINT32  uiFreeClu = 0;
    UINT32  uiTotalClu = 0;
    UINT32  uiCluMByte = 0;

    printf("name        vol          free(M)      total(M)\n");
    
    for(uiLoop = 0;uiLoop < FS_MAX_VOL;uiLoop++)
    {
        if(FS_VOL_USED == gstFsCtrl.astVol[uiLoop].uiFlag)
        {
            uiCluMByte = (gstFsCtrl.astVol[uiLoop].uiCluByte >> 20);
            fsGetVolFreeClu(&gstFsCtrl.astVol[uiLoop],&uiFreeClu);
            uiTotalClu = gstFsCtrl.astVol[uiLoop].uiVolClu - 
                         gstFsCtrl.astVol[uiLoop].uiFatClu + 1;
            printf("%-12s0x%-8x   %-8d     %-8d\n",
                   &gstFsCtrl.astVol[uiLoop].aucPartName,
                   &gstFsCtrl.astVol[uiLoop],
                   uiFreeClu*uiCluMByte,uiTotalClu*uiCluMByte);
        }
    }
}

/*****************************************************************************
 function     : fsExit
 description  : file system exit
 input        : 
 output       : 
 return       : 
*****************************************************************************/
VOID fsExit(VOID)
{
    UINT32      uiVolNo = 0;
    
    for(uiVolNo = 0;uiVolNo < FS_MAX_VOL;uiVolNo++)
    {
        if(NULL != gstFsCtrl.astVol[uiVolNo].stVolSem)
        {
            semDelete(gstFsCtrl.astVol[uiVolNo].stVolSem);
        }
        if(NULL != gstFsCtrl.astVol[uiVolNo].stFdSem)
        {
            semDelete(gstFsCtrl.astVol[uiVolNo].stFdSem);
        }
    }
    gstFsCtrl.astVol[uiVolNo].uiCacheTaskRuning = 0;
}


/*****************************************************************************
  function     : fsInit
  description  : file system initiation
  input        : 
  output       : 
  return       : 
*****************************************************************************/
STATUS fsInit(VOID)
{
	FS_VOL_S *pstVol = NULL;
    UINT32    uiLoop = 0;
    UINT32    uiReqNum = 0;

    FS_LOG(FS_DBG_ERR,"file system version %s\n",FS_VER,0,0,0,0,0);
    
    bzero((CHAR*)&gstFsCtrl,sizeof(FS_CTRL_S));
    gstFsCtrl.stEntFunc.fsClearEntChain = fsClearEntChain;
    gstFsCtrl.stEntFunc.fsGetFreeEnt = fsGetFreeEnt;
    gstFsCtrl.stEntFunc.fsGetLastEnt = fsGetLastEnt;
    gstFsCtrl.stEntFunc.fsReadEnt = fsReadEnt;
    gstFsCtrl.stEntFunc.fsWriteEnt = fsWriteEnt;
    gstFsCtrl.stEntFunc.fsGetModLastEnt = fsGetModLastEnt;

    gstFsCtrl.stFDTFunc.fsFindFDT = fsFindFDT;
    gstFsCtrl.stFDTFunc.fsGetModWrFDT = fsGetModWrFDT;
    gstFsCtrl.stFDTFunc.fsPrintFDT = fsPrintFDT;
    gstFsCtrl.stFDTFunc.fsReadFDT = fsReadFDT;
    gstFsCtrl.stFDTFunc.fsWriteFDT = fsWriteFDT;
    gstFsCtrl.stFDTFunc.fsGetFDTCnt = NULL;

    gstFsCtrl.stCacheFunc.fsCacheByteRead = fsCacheByteRead;
    gstFsCtrl.stCacheFunc.fsCacheByteWrite = fsCacheByteWrite;
    gstFsCtrl.stCacheFunc.fsFreeFatCache = fsFreeFatCache;
    gstFsCtrl.stCacheFunc.fsReadFatToCache = fsReadFatToCache;
    gstFsCtrl.stCacheFunc.fsCacheCluRead = fsCacheCluRead;
    gstFsCtrl.stCacheFunc.fsCacheCluWrite = fsCacheCluWrite;
	gstFsCtrl.stCacheFunc.fsCacheByteWrBack = fsCacheByteWrBack;
	fsPrepCryptTable();
    for(uiLoop = 0;uiLoop < FS_MAX_VOL;uiLoop++)
    {
    	pstVol = &gstFsCtrl.astVol[uiLoop];
        pstVol->uiFlag = FS_VOL_FREE;
		
        pstVol->stVolSem = semBCreate(SEM_Q_PRIORITY,SEM_FULL);
        if(NULL == pstVol->stVolSem)
        {
            FS_LOG(FS_DBG_ERR,"file system create semphore failed\n",0,0,0,0,0,0);
            goto ErrExit;
        }

	  /*add by dongyang140513*/
        pstVol->stFsOpenSem = semBCreate(SEM_Q_PRIORITY,SEM_FULL);
        if(NULL == pstVol->stFsOpenSem)
        {
            FS_LOG(FS_DBG_ERR,"file system create semphore failed\n",0,0,0,0,0,0);
            goto ErrExit;
        }
		

		
        pstVol->stFdSem = semBCreate(SEM_Q_PRIORITY,SEM_FULL);
        if(NULL == pstVol->stFdSem)
        {
            FS_LOG(FS_DBG_ERR,"file system create semphore failed\n",0,0,0,0,0,0);
            goto ErrExit;
        }

	 	pstVol->stReqSem = semBCreate(SEM_Q_PRIORITY,SEM_FULL);
        if(NULL == pstVol->stReqSem)
        {
            FS_LOG(FS_DBG_ERR,"file system create semphore failed\n",0,0,0,0,0,0);
            goto ErrExit;
        }

	 	for(uiReqNum = 0;uiReqNum < FS_MAX_REQ_PER_VOL;uiReqNum++)
        {
            pstVol->astReq[uiReqNum].stReqSem = 
                     semBCreate(SEM_Q_PRIORITY,SEM_EMPTY);
            if(NULL == pstVol->astReq[uiReqNum].stReqSem)
            {
                FS_LOG(FS_DBG_ERR,"file system create semphore failed\n",0,0,0,
                       0,0,0);
                goto ErrExit;
            }
            fsFreeReq(pstVol,&pstVol->astReq[uiReqNum]);
        }
		
    }
    gstFsCtrl.uiDrvNum = iosDrvInstall(fsCreate,fsDelete,fsOpen,fsClose,
                                       fsRead,fsWrite,fsIoctl);

    if(ERROR == gstFsCtrl.uiDrvNum)
    {
        FS_LOG(FS_DBG_ERR,"install file system failed\n",0,0,0,0,0,0);
        goto ErrExit;
    }
    
    return OK;
    
ErrExit:
    fsExit();
    
    return ERROR;
}

/*****************************************************************************
  function     : fsCalcCluSize
  description  : calc file size from cluster
  input        : 
  output       : 
  return       : 
*****************************************************************************/
STATUS fsCalcCluSize(UINT32 uiCluNum,UINT32 uiCluLen,UINT64_S *pullSize)
{
    UINT32 uiMByte = 0;
   
    if(uiCluLen < 0x100000)
    {
        FS_LOG(FS_DBG_ERR,"cluster len %d is not supported\n",uiCluLen,0,0,0,0,0);
        return ERROR;
    }
    uiMByte = uiCluLen >> 20;
    pullSize->hi = (uiMByte * uiCluNum) >> 12;
    pullSize->lo = (uiMByte * uiCluNum) << 20;

    return OK;
}

STATUS fsAddVol(FS_VOL_S *vpstVol)
{
    FS_FDT_INFO_S stFDTInfo;
    UINT32   uiLoop = 0;
    UINT32   uiClu = 0;
    UINT32   uiTotalClu = 0;
    UINT8   *pucBuf = NULL;

    pucBuf = malloc(vpstVol->uiCluByte);
    if(NULL == pucBuf)
    {
    	FS_LOG(FS_DBG_ERR,"malloc memory failed\n",0,0,0,0,0,0);
    	return ERROR;
    }
    memset(pucBuf,0,vpstVol->uiCluByte);

    /*
     *fat table init
    */
    for(uiLoop = 0; uiLoop < vpstVol->uiCluPerFat;uiLoop++)
    {
    	uiClu = uiLoop + vpstVol->uiFatClu;
    	if(OK != fsCluRW(vpstVol,uiClu,vpstVol->uiCluByte,0,pucBuf,FS_OPER_WRITE))
    	{
            free(pucBuf);
    		return ERROR;
    	}
    }

    /*
     *reserver cluster init
    */
    for(uiLoop = 0; uiLoop < FS_RESV_CLU_NUM;uiLoop++)
    {
    	uiClu = uiLoop + vpstVol->uiStartClu;
    	if(OK != fsCluRW(vpstVol,uiClu,vpstVol->uiCluByte,0,pucBuf,FS_OPER_WRITE))
    	{
            free(pucBuf);
    		return ERROR;
    	}
    }
    
    /*
     *fat root cluster init
    */
    for(uiLoop = 0; uiLoop < vpstVol->uiFDTCluCnt;uiLoop++)
    {
    	uiClu = vpstVol->uiRootClu + uiLoop;
    	if(OK != fsCluRW(vpstVol,uiClu,vpstVol->uiCluByte,
                         0,pucBuf,FS_OPER_WRITE))
    	{
            free(pucBuf);
    		return ERROR;
    	}
    }
    
    if(OK != gstFsCtrl.stEntFunc.fsWriteEnt(vpstVol,vpstVol->uiRootClu,
    	                                    FS_CLUST_EOF,FS_CACHE_WR_THROUGH))
    {
    	return ERROR;
    }
    
    memset(&stFDTInfo,0,sizeof(FS_FDT_INFO_S));
    stFDTInfo.uiClu = 0;
    stFDTInfo.uiCluOffset = 0;
    stFDTInfo.stFDT.uiFirstClu = vpstVol->uiRootClu;   
    fsFdtInit(vpstVol);
    if(OK != fsNewFile(vpstVol,FS_ROOT_DIR,&stFDTInfo,FS_ATTR_DIRECTORY))
    {
        free(pucBuf);
    	return ERROR;
    }   
    free(pucBuf);
    FS_LOG(FS_DBG_ERR,"add vol 0x%x size 0x%x%.8x root 0x%x clulen 0x%x fat %d\n",
           vpstVol,stFDTInfo.stFDT.ullFileSize.hi,
           stFDTInfo.stFDT.ullFileSize.lo,vpstVol->uiRootClu,
           vpstVol->uiCluByte,vpstVol->uiFatClu);
    
    return OK;
}

/*****************************************************************************
  function     : fsMountVol
  description  : fs system mount vol
 *****************************************************************************/
 STATUS fsMountVol(FS_VOL_S *vpstVol)
 {
     vpstVol->uiMinCacheClu = vpstVol->uiFatClu;
     vpstVol->uiMaxCacheClu = vpstVol->uiFatClu + vpstVol->uiCluPerFat;
     if(OK != gstFsCtrl.stCacheFunc.fsReadFatToCache(vpstVol))
     {
         FS_LOG(FS_DBG_ERR,"read fat table cache failed\n",0,0,0,0,0,0);
         /*
          *notice:cannot return
         */
     }
     
     if(OK != iosDevAdd(&(vpstVol->stDevHdr),vpstVol->aucPartName,gstFsCtrl.uiDrvNum))
     {
         FS_LOG(FS_DBG_ERR,"ios add device %s failed\n",vpstVol->aucPartName,0,0,0,0,0);
         return ERROR;
     }
     
     FS_LOG(FS_DBG_ERR,"ios add device %s succeed\n",vpstVol->aucPartName,0,0,0,0,0);
#ifdef FS_CACHE_MON_POLICY     
     vpstVol->uiCacheTaskRuning = 1;
     taskSpawn("tFatMon",20,0,0x800000,(FUNCPTR)fsCacheMon,vpstVol,0,0,0,0,0,0,0,0,0);
#endif     
     FS_LOG(FS_DBG_ERR,"ios mount vol %p cnt 0x%x fat cluster 0x%x root 0x%x\n",
            vpstVol,vpstVol->uiVolClu,vpstVol->uiCluPerFat,
            vpstVol->uiRootClu,0,0);
     
     return OK;
 }

 /*****************************************************************************
   function     : fsMountVol
   description  : fs system mount vol
  *****************************************************************************/
 STATUS fsFormatVol(FS_VOL_S *vpstVol)
 {
    /*
     *clear fat buffer   
    */
    bzero(vpstVol->pcCacheData,vpstVol->uiCacheDataLen);
    bzero(vpstVol->pcFDTCache,vpstVol->uiFDTCacheLen);
    FS_LOG(FS_DBG_ERR,"fdt cache 0x%x len 0x%xinit\n",vpstVol->pcFDTCache,
           vpstVol->uiFDTCacheLen,0,0,0,0);
    
    if(OK != fsAddVol(vpstVol))
    {
        return ERROR;
    }
    /*
     *reread cache data
    */
    vpstVol->uiMinCacheClu = vpstVol->uiFatClu;
    vpstVol->uiMaxCacheClu = vpstVol->uiFatClu + vpstVol->uiCluPerFat;
    if(OK != gstFsCtrl.stCacheFunc.fsReadFatToCache(vpstVol))
    {
        FS_LOG(FS_DBG_ERR,"read fat table cache failed\n",0,0,0,0,0,0);
        /*
         *notice:cannot return
        */
    }
    
    return OK;
 }

 /*****************************************************************************
  function     : fsUnMountVol
  description  : fs system unmount vol
  input        : 
  output       : 
  return       : 
 *****************************************************************************/
 STATUS fsUnMountVol(FS_VOL_S *vpstVol)
 {
     gstFsCtrl.stCacheFunc.fsFreeFatCache(vpstVol);
     iosDevDelete(&vpstVol->stDevHdr);
#ifdef FS_FDT_CACHE_ENABLE
	 free(vpstVol->pcFDTCache);
	 vpstVol->pcFDTCache = NULL;
#endif
	 free(vpstVol->pcCacheData);
	 vpstVol->pcCacheData = NULL;
     fsFreeVol(vpstVol);
 
     return OK;
 }

/*****************************************************************************
 function     : fsRegFunc
 description  : fs system function register
*****************************************************************************/
INT32 fsRegFunc(FS_CLU_RW_F pfFunc)
{
    gstFsCtrl.pfCluRw = pfFunc;
    
    return OK;
}

