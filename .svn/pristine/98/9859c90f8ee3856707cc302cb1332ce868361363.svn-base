/******************************************************************************

                  VERSION (C), 2001-2011, 

 ******************************************************************************
  file       : fsUsr.c
  version    : 1.0
  author     : lgh
  data       : 2012.9.3
  function   : file system user api 
******************************************************************************/

/*----------------------------------------------*
 * header file                                  *
 *----------------------------------------------*/
#include "fsInc.h"

/*----------------------------------------------*
 * external  variable                           *
 *----------------------------------------------*/
 UINT32   guiFsDbgLevel = FS_DBG_ERR;

/*----------------------------------------------*
 * external  function                           *
 *----------------------------------------------*/
extern STATUS UINT64_CMPG(UINT64_S *pullValA,UINT64_S *pullValB);
extern STATUS UINT64_CMPGE(UINT64_S *pullValA,UINT64_S *pullValB);

/*----------------------------------------------*
 * internal  function                           *
 *----------------------------------------------*/
    
/*----------------------------------------------*
 * global  variable                             *
 *----------------------------------------------*/

FS_CTRL_S   gstFsCtrl;

/*----------------------------------------------*
 * constant variable                            *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macro definition                             *
 *----------------------------------------------*/

 /*****************************************************************************
  function     : fsGetFd
  description  : get free file description
 *****************************************************************************/
 FS_FD_S* fsGetFd(FS_VOL_S *vpstVol)
 {
    UINT32       uiLoop = 0;
 
    semTake(vpstVol->stFdSem,WAIT_FOREVER); 
    for(uiLoop = 0;uiLoop < FS_MAX_FD;uiLoop++)
    {
         if(FS_FD_FREE == vpstVol->astFd[uiLoop].uiUsed)
         {
             memset(&vpstVol->astFd[uiLoop], 0, sizeof(FS_FD_S));
             vpstVol->astFd[uiLoop].pstVol = vpstVol;
             vpstVol->astFd[uiLoop].uiUsed = FS_FD_USED;
             vpstVol->astFd[uiLoop].uiRcvrId = uiLoop;
             semGive(vpstVol->stFdSem);
             return &vpstVol->astFd[uiLoop];
         }
    }
    semGive(vpstVol->stFdSem);
    
    return NULL;
 }

 /*****************************************************************************
  function     : fsFreeFd
  description  : free fd
  input        : 
  output       : 
  return       : 
 *****************************************************************************/
 VOID fsFreeFd(FS_FD_S *vpstFd)
 {
    semTake(vpstFd->pstVol->stFdSem,WAIT_FOREVER); 
    vpstFd->uiUsed = FS_FD_FREE;
    semGive(vpstFd->pstVol->stFdSem);
 }

 /*****************************************************************************
 function     : fsIsRoot
 description  : directory is root or not
 *****************************************************************************/
 STATUS fsIsRoot(CHAR *vpcFileName)
 {
     if((0x0 == strlen(vpcFileName)) || ((0x1 == strlen(vpcFileName)) && 
         ('/' == *vpcFileName)))
     {
         return TRUE;
     }
 
     return FALSE;
 }
 

 /*****************************************************************************
 function     : fsGetFile
 description  : get file directory table description
 input        : 
 output       : 
 return       : 
 *****************************************************************************/
STATUS fsGetFile(FS_VOL_S *vpstVol,FS_PATH_ARRAY_S *vpcPath,
                  UINT32 vuiPathLevel,FS_FDT_INFO_S *vpstFDTInfo)
{
	 FS_PATH_ARRAY_S  *pucPath = NULL;
	 UINT32            uiPathLevel = 0;
	 UINT32            uiDirFirstClu = 0;
	 UINT32            uiLoop = 0;
	 
	 /*assert(vuiPathLevel >= 1);*/
	 
	 pucPath = vpcPath;
	 uiDirFirstClu = vpstVol->uiRootClu;
	 uiPathLevel = vuiPathLevel;
	 
	 for(uiLoop = 0;uiLoop < uiPathLevel;uiLoop++)
	 {
	     if(OK != gstFsCtrl.stFDTFunc.fsFindFDT(vpstVol,&pucPath->aucName[0],
	                                            uiDirFirstClu,vpstFDTInfo))
	     {
	         FS_LOG(FS_DBG_DATA,"cannot find path %s\n",
	                (UINT32)pucPath->aucName,0,0,0,0,0);
	         return ERROR;
	     }
	     uiDirFirstClu = vpstFDTInfo->stFDT.uiFirstClu;
	     pucPath++;
	 }

	 return OK;
}

 /*****************************************************************************
 function     : fsGetFileSize
 description  : get file size from clulen clunum
 input        : 
 output       : 
 return       : uiCluNum * clenLen = filesize clulen must be divided by 1M
 *****************************************************************************/
 VOID fsGetFileSize(UINT32 uiCluNum,UINT32 uiCluLen,UINT64_S *pullSize)
 {
    UINT32 uiMByte = 0;
    UINT32 uiResByte = 0;

    /*
     *[pullSize->hi][pullSize->lo] = uiCluNum * [2^20 uiMByte + uiResByte]
    */
    if(0x0 == uiCluNum)
    {
        pullSize->hi = 0;
        pullSize->lo = 0;
        return;
    }
    
    uiMByte = uiCluLen >> 20;
    uiResByte = uiCluLen & 0xfffff;
    pullSize->hi = (uiCluNum * uiMByte) >> 12;
    pullSize->lo = (uiCluNum * uiMByte) << 20;
    pullSize->lo += uiResByte * uiCluNum;
 }
 
 STATUS fsFillFdByFDTInfo(FS_VOL_S *vpstVol,FS_FD_S *vpstFd,
                          FS_FDT_INFO_S *vpstFDTInfo,UINT32 vuiFlag)
 {
     UINT32    uiCluNum = 0;
	 UINT32    uiTime = 0;
     
     vpstFd->uiCluOffset = 0;
     vpstFd->uiFDTCluId = vpstFDTInfo->uiClu;
     vpstFd->uiFDTCluOffset = vpstFDTInfo->uiCluOffset;
     vpstFd->uiCurClu = vpstFDTInfo->stFDT.uiFirstClu;
     vpstFd->uiPreClu = FS_CLUST_INVALID;
     vpstFd->uiAttr = vpstFDTInfo->stFDT.ucAttr;
     UINT64_SET(vpstFd->ullFileSize,vpstFDTInfo->stFDT.ullFileSize);
     vpstFd->uiFirstClu = vpstFDTInfo->stFDT.uiFirstClu;
     if(0x0 != (vuiFlag & (O_APPEND)))
     {
        vpstFd->ullPos.lo = vpstFDTInfo->stFDT.ullFileSize.lo;
        vpstFd->ullPos.hi = vpstFDTInfo->stFDT.ullFileSize.hi;
     }
     else
     {
        vpstFd->ullPos.lo = 0;
        vpstFd->ullPos.hi = 0;
     }
     vpstFd->ullCyclePos.hi = vpstFDTInfo->stFDT.ullCyclePos.hi;
	 vpstFd->ullCyclePos.lo = vpstFDTInfo->stFDT.ullCyclePos.lo;
     bcopy(vpstFDTInfo->stFDT.aucName,vpstFd->aucName,FS_MAX_NAME_LEN);
#if 1
	 vpstFd->uiTime = (vpstFDTInfo->stFDT.ucUpdateTime[1]  << 24) | 
                      (vpstFDTInfo->stFDT.ucUpdateTime[0]  << 16) |
                      (vpstFDTInfo->stFDT.ucUpdateDate[1]  << 8) |
                      (vpstFDTInfo->stFDT.ucUpdateDate[0]  << 0);
#endif
     uiCluNum = gstFsCtrl.stEntFunc.fsGetLastEnt(vpstVol,
                              vpstFDTInfo->stFDT.uiFirstClu,&vpstFd->uiLastClu);
     return OK;
 }

 /*****************************************************************************
 function     : fsClrDirEmptyClu
 description  : clear directory empty cluster
 *****************************************************************************/
 STATUS fsClrDirEmptyClu(FS_VOL_S *vpstVol,FS_FDT_INFO_S *vpstDirFDTInfo)
 {
     UINT32    uiPrevClu = 0;
     UINT32    uiCurClu = 0;
     UINT32    uiNextClu = 0;
     UINT32    uiFirstClu = FS_CLUST_INVALID;
     BOOL      bFirstFound = FALSE;
     UINT32    uiFDTCnt = 0;
     
     uiCurClu = vpstDirFDTInfo->stFDT.uiFirstClu;
     if((FS_CLUST_INVALID == uiCurClu) || (FS_CLUST_EOF == uiCurClu))
     {
         return OK;
     }
     uiNextClu = FS_CLUST_AVAIL;
     uiPrevClu = uiCurClu;
 
     while((FS_CLUST_EOF != uiNextClu) && (FS_CLUST_INVALID != uiCurClu))
     {
         if(FS_CLUST_EOF == uiCurClu)
         {
            FS_LOG(FS_DBG_ERR,"fsClrDirEmptyClu curr free clu invalid\n",
                   0,0,0,0,0,0);
         }
         if(OK != gstFsCtrl.stEntFunc.fsReadEnt(vpstVol,uiCurClu,&uiNextClu))
         {
             return ERROR;
         }
         if(OK != gstFsCtrl.stFDTFunc.fsGetFDTCnt(vpstVol,uiCurClu,&uiFDTCnt))
         {
             return ERROR;
         }
         if((FALSE == bFirstFound) && (0 != uiFDTCnt))
         {
             uiFirstClu = uiCurClu;
             bFirstFound = TRUE;
         }
         if(0 == uiFDTCnt)
         {
             if(OK != gstFsCtrl.stEntFunc.fsWriteEnt(vpstVol,uiCurClu,
                                                     FS_CLUST_AVAIL,0))
             {
                 return ERROR;
             }
             if(uiPrevClu != uiCurClu)
             {
                 if(OK != gstFsCtrl.stEntFunc.fsWriteEnt(vpstVol,uiPrevClu,
                                                         uiNextClu,0))
                 {
                     return ERROR;
                 }
             }
         }
         
         uiPrevClu = uiCurClu;
         uiCurClu = uiNextClu;
     }
 
     vpstDirFDTInfo->stFDT.uiFirstClu = uiFirstClu;
     
     return OK;
 }

 /*****************************************************************************
 function     : fsClearDirFDT
 description  : delete file in directory
 input        : 
 output       : 
 return       : 
*****************************************************************************/
STATUS fsClearFileFromDir(FS_VOL_S *vpstVol,FS_FDT_INFO_S *vpstDirFDTInfo,
                          FS_FDT_INFO_S *vpstFileFDTInfo)
{
    memset(&vpstFileFDTInfo->stFDT,0,sizeof(FS_FDT_S));    
    if(OK != gstFsCtrl.stFDTFunc.fsWriteFDT(vpstVol,&vpstFileFDTInfo->stFDT,
                         vpstFileFDTInfo->uiClu,vpstFileFDTInfo->uiCluOffset))
    {
        return ERROR;
    }
    
    /*if(vpstDirFDTInfo->stFDT.ullFileSize < sizeof(FS_FDT_S))
    {
        FS_LOG(FS_DBG_ERR,"dir size %d error\n",vpstDirFDTInfo->stFDT.ullFileSize,0,0,0,0,0);
        return ERROR;
    }*/
    if(OK != fsClrDirEmptyClu(vpstVol,vpstDirFDTInfo))
    {
        return ERROR;
    }

    UINT64_SUB32(&vpstDirFDTInfo->stFDT.ullFileSize,
                 &vpstDirFDTInfo->stFDT.ullFileSize,sizeof(FS_FDT_S));
    if(OK != gstFsCtrl.stFDTFunc.fsWriteFDT(vpstVol,&vpstDirFDTInfo->stFDT,
                         vpstDirFDTInfo->uiClu,vpstDirFDTInfo->uiCluOffset))
    {
        return ERROR;
    }
    
    return OK;
}
 
/*****************************************************************************
function     : fsUpdateFile
description  : update file information
input        : 
output       : 
return       : 
*****************************************************************************/
STATUS fsUpdateFile(FS_FD_S *vpstFd)
{
    FS_FDT_S  stFDT;
    STATUS    iRet = OK;

    bzero((CHAR*)&stFDT,sizeof(FS_FDT_S));
	UINT64_SET(stFDT.ullCyclePos,vpstFd->ullPos);
    UINT64_SET(stFDT.ullFileSize,vpstFd->ullFileSize);
    stFDT.ucFileStat = FS_FILE_STAT_SUBDIR;
    bcopy(vpstFd->aucName,stFDT.aucName,FS_MAX_NAME_LEN);
    stFDT.ucAttr = (UINT8)vpstFd->uiAttr;
    stFDT.uiFirstClu = vpstFd->uiFirstClu;
    stFDT.ucUpdateTime[1] = (vpstFd->uiTime >> 24) & 0xff;
    stFDT.ucUpdateTime[0] = (vpstFd->uiTime >> 16 )& 0xff;
    stFDT.ucUpdateDate[1] = (vpstFd->uiTime >> 8 )& 0xff;
    stFDT.ucUpdateDate[0] = (vpstFd->uiTime)& 0xff;

#ifdef FS_EXTENTED_FDT
    stFDT.uiStartFrame = vpstFd->uiStartFrame;
    stFDT.uiEndFrame = vpstFd->uiEndFrame;
#endif
    /*
     *updata FDT and write the end fat of file
    */
    semTake(vpstFd->pstVol->stVolSem,WAIT_FOREVER);
    iRet |= gstFsCtrl.stFDTFunc.fsWriteFDT(vpstFd->pstVol,&stFDT,vpstFd->uiFDTCluId,
                                            vpstFd->uiFDTCluOffset);
    if(FS_CLUST_INVALID != vpstFd->uiLastClu)    
    {
        iRet |= gstFsCtrl.stEntFunc.fsWriteEnt(vpstFd->pstVol,vpstFd->uiLastClu,
                                            FS_CLUST_EOF,FS_CACHE_WR_THROUGH);
    }
    semGive(vpstFd->pstVol->stVolSem);

    return iRet;
}

/*****************************************************************************
 function     : fsReadAlign
 description  : read file to buffer
 input        : vpstFd;vpcBuf;vuiByte
 output       : *vuiActRdByte
 return       : read len
*****************************************************************************/
INT32 fsReadAlign(FS_FD_S *vpstFd,CHAR *vpcBuf,UINT32 vuiByte)
{
    INT32        iActRdByte = 0;
    UINT32       uiRdLen = 0;
    UINT32       uiCluLen = 0;
    UINT32       uiNextClu = 0;
    UINT32       uiBufPos = 0;
                                     
    if((0x0 == vuiByte) || (FS_CLUST_EOF == vpstFd->uiCurClu))
    {
        return OK;
    }
    
    uiRdLen = vuiByte;
    uiCluLen = vpstFd->pstVol->uiCluByte;
    
    do
    {
        if(uiRdLen >= uiCluLen)
        {
            if(OK != gstFsCtrl.stCacheFunc.fsCacheCluRead(vpstFd->pstVol,
                                     vpstFd->uiCurClu,&(vpcBuf[uiBufPos])))
            {
                return 0;
            }
            iActRdByte = iActRdByte + uiCluLen;
            uiRdLen = uiRdLen - uiCluLen;
            uiBufPos = uiBufPos + uiCluLen;
            semTake(vpstFd->pstVol->stVolSem,WAIT_FOREVER);
            if(FS_CLUST_EOF == vpstFd->uiCurClu)
            {
               FS_LOG(FS_DBG_ERR,"fsReadAlign curr free clu invalid\n",
                      0,0,0,0,0,0);
            }
            gstFsCtrl.stEntFunc.fsReadEnt(vpstFd->pstVol,vpstFd->uiCurClu,&uiNextClu);
            semGive(vpstFd->pstVol->stVolSem);
            vpstFd->uiCurClu = uiNextClu;
            vpstFd->uiCluOffset = 0;
        }
        else
        {
            gstFsCtrl.stCacheFunc.fsCacheByteRead(vpstFd->pstVol,vpstFd->uiCurClu,
                                                 0,&(vpcBuf[uiBufPos]),uiRdLen);
            iActRdByte = iActRdByte + (INT32)uiRdLen;
            vpstFd->uiCluOffset = uiRdLen;
            uiRdLen = 0;
        }
    }while((FS_CLUST_EOF != uiNextClu) && (uiRdLen > 0));

    UINT64_ADD(&vpstFd->ullPos,&vpstFd->ullPos,iActRdByte);
    
    return iActRdByte;
}

 /*****************************************************************************
  function     : fsWriteAlign
  description  : write the file which EOF is the cluster aligned 
  input        : 
  output       : 
  return       : OK or ERROR
 *****************************************************************************/
 STATUS fsWriteAlign(FS_FD_S *vpstFd,CHAR *vpcBuf,UINT32 vuiByte)
 {   
     UINT32       uiRest = 0;
     UINT32       uiCurClu = FS_CLUST_INVALID;
     UINT32       uiNextClu = FS_CLUST_INVALID;
     UINT32       uiCluLen = 0;
     FS_VOL_S    *pstVol = NULL;
     BOOL         bFDTWr = FALSE;
     UINT32       uiBufPos = 0;
  
     uiRest = vuiByte;
     pstVol = vpstFd->pstVol;
     uiCluLen = pstVol->uiCluByte;
     
     while(uiRest > 0)
     {   
         uiCurClu = vpstFd->uiCurClu;
         /*
          *get free cluster and update the content of the last content
         */
         semTake(vpstFd->pstVol->stVolSem,WAIT_FOREVER);
         if((FS_CLUST_INVALID == uiCurClu) || (FS_CLUST_EOF == uiCurClu))
         {
            if(OK != gstFsCtrl.stEntFunc.fsGetModLastEnt(vpstFd->pstVol,
                                                  vpstFd->uiPreClu,&uiCurClu))
            {
                semGive(vpstFd->pstVol->stVolSem);
                return ERROR;
            }
            vpstFd->uiCurClu = uiCurClu;
            vpstFd->uiLastClu = uiCurClu;
         }
         semGive(vpstFd->pstVol->stVolSem);
         /*
          *update the file fdt
         */
         if((0x00 == vpstFd->ullFileSize.hi) && (0x00 == vpstFd->ullFileSize.lo))
         {
             vpstFd->uiFirstClu = uiCurClu;
             vpstFd->uiLastClu = uiCurClu;
             if(OK != fsUpdateFile(vpstFd))
             {
                FS_LOG(FS_DBG_ERR,"update file failed\n",0,0,0,0,0,0);
                return ERROR;
             }
         }
         
         if(FALSE == bFDTWr)
         {
             vpstFd->uiOper |= FS_OPER_WRITE;
             UINT64_ADD(&vpstFd->ullPos,&vpstFd->ullPos,vuiByte);
             if(UINT64_CMPG(&vpstFd->ullPos,&vpstFd->ullFileSize))
             {
                 vpstFd->uiLastClu = uiCurClu;
                 vpstFd->ullFileSize.hi = vpstFd->ullPos.hi;
                 vpstFd->ullFileSize.lo = vpstFd->ullPos.lo;
             }
             bFDTWr = TRUE;
         }
         /*
          *write the data
         */
         if(uiRest >= uiCluLen)
         {
             if(OK != gstFsCtrl.stCacheFunc.fsCacheCluWrite(pstVol,uiCurClu,
                                                           &vpcBuf[uiBufPos]))
             {
                 return ERROR;
             }
             uiBufPos = uiBufPos + uiCluLen;
             uiRest = uiRest - uiCluLen;
             semTake(vpstFd->pstVol->stVolSem,WAIT_FOREVER);
             if(OK != gstFsCtrl.stEntFunc.fsReadEnt(vpstFd->pstVol,uiCurClu,
                                                    &uiNextClu))
             {
                semGive(vpstFd->pstVol->stVolSem);
                return ERROR;
             }
             semGive(vpstFd->pstVol->stVolSem);
             vpstFd->uiPreClu = uiCurClu;
             vpstFd->uiCurClu = uiNextClu;
         }
         else
         {                                    
             if(OK != gstFsCtrl.stCacheFunc.fsCacheByteWrite(pstVol,uiCurClu,0,
                                                    &vpcBuf[uiBufPos],uiRest))
             {
                 return ERROR;
             }
             uiRest = 0;
         }
     }
     return OK;
 }

/*****************************************************************************
 function     : fsFillStat
 description  : fill file stat
*****************************************************************************/
STATUS fsFillStat(FS_FD_S *vpstFd,struct stat * vpstStat)
{
	UINT32 uiCluLen = 0;
	
	bzero ((char *) vpstStat, sizeof (struct stat));
	uiCluLen = vpstFd->pstVol->uiCluByte;
	vpstStat->st_dev = (UINT32)vpstFd->pstVol;
	vpstStat->st_nlink = 1;
	vpstStat->st_size = vpstFd->ullFileSize.hi;
	vpstStat->st_size = vpstStat->st_size << 32;
	vpstStat->st_size |= vpstFd->ullFileSize.lo;
	vpstStat->st_blksize = uiCluLen;
	vpstStat->st_blocks = ((vpstFd->ullFileSize.lo + uiCluLen - 1)/ uiCluLen);
	vpstStat->st_attrib =  0x20;
	vpstStat->st_mode = S_IRWXU | S_IRWXG | S_IRWXO;
	vpstStat->st_reserved1 = vpstFd->ullCyclePos.hi;
	vpstStat->st_reserved2 = vpstFd->ullCyclePos.lo;
	/*vpstStat->st_ctime = vpstFd->uiTime;*/

	if(vpstFd->uiAttr & FS_ATTR_DIRECTORY)
	{
		FS_LOG(FS_DBG_DATA,"fsFillStat directory\n",0,0,0,0,0,0);
		vpstStat->st_mode |= S_IFDIR ;
	}
	else
	{
        vpstStat->st_mode |= S_IFREG;
	}

	FS_LOG(FS_DBG_DATA,"fsFillStat %s fd 0x%x size 0x%llx\n",vpstFd->aucFullName,
	       vpstFd,vpstStat->st_size,0,0,0);
	
	return OK;
}

/*****************************************************************************
function     : fsFillStat
description  : fill file stat
*****************************************************************************/
STATUS fsFillFSStat(FS_FD_S *vpstFd,struct statfs * vpstStat)
{
    UINT32 uiFreeClu = 0;

    fsGetVolFreeClu(vpstFd->pstVol,&uiFreeClu);
    vpstStat->f_type   = 0;
    vpstStat->f_bsize  = vpstFd->pstVol->uiCluByte;
    vpstStat->f_blocks = vpstFd->pstVol->uiVolClu - vpstFd->pstVol->uiFatClu + 1;
    vpstStat->f_bfree  = uiFreeClu;
    vpstStat->f_bavail = vpstStat->f_bfree;
    vpstStat->f_files  = -1;
    vpstStat->f_ffree  = -1;
    vpstStat->f_fsid.val[0] = (long)vpstFd->pstVol;;
    vpstStat->f_fsid.val[1] = 0;

    return OK;
}
 

 /*****************************************************************************
 function     : fsListDir
 description  : list file in the directory
 input        : 
 output       : 
 return       : gstFsCtrl.stFDTFunc.fsListFDT(vpstFd->pstVol,vpstFd->uiFirstClu);
*****************************************************************************/
STATUS fsListDir(FS_FD_S *vpstFd,DIR *vpstDir)
{
#if 1    
    FS_FDT_S  stFDT;
    UINT32    uiSize = 0;

    do
    {
        memset(&stFDT,0,sizeof(stFDT));
        uiSize = fsRead(vpstFd,(CHAR*)&stFDT,sizeof(stFDT));
        vpstDir->dd_eof = FALSE;
        if(ERROR == vpstDir->dd_cookie)
    	{
            vpstDir->dd_eof = TRUE;
            return OK;
    	}
        if(uiSize != sizeof(stFDT))
        {
            vpstDir->dd_eof = TRUE;
            return ERROR;
        }
        if(FS_FILE_STAT_SUBDIR == stFDT.ucFileStat)
        {
            memcpy(vpstDir->dd_dirent.d_name,stFDT.aucName,FS_MAX_NAME_LEN);
            vpstDir->dd_cookie += sizeof(stFDT);
            break;
        }
        if(vpstFd->ullFileSize.lo <= vpstDir->dd_cookie)
        {
            vpstDir->dd_eof = TRUE;
            return OK;
        }
    }while(uiSize != sizeof(&stFDT));
    
    return OK;
#else
    gstFsCtrl.stFDTFunc.fsListFDT(vpstFd->pstVol,vpstFd->uiFirstClu);
    return ERROR;
#endif
	
}

 /*****************************************************************************
 function     : fsPrintDir
 description  : print file information in the directory
 input        : 
 output       : 
 return       : gstFsCtrl.stFDTFunc.fsListFDT(vpstFd->pstVol,vpstFd->uiFirstClu);
*****************************************************************************/
STATUS fsPrintDir(CHAR *vpcVolName,CHAR *vpcDirName)
{
    FS_FD_S  *pstFd = NULL;
    FS_VOL_S *pstVol = NULL;

    pstVol = fsFindVolByName(vpcVolName);/*fsFindVolByName("/vol")*/
    if(NULL == pstVol)
    {
        FS_LOG(FS_DBG_ERR,"vol %s not exist\n",vpcVolName,0,0,0,0,0);
        return ERROR;
    }
    pstFd = (FS_FD_S*)fsOpen(pstVol,vpcDirName,0,0);
    if(NULL == pstFd)
    {
        return ERROR;
    }
    
    gstFsCtrl.stFDTFunc.fsListFDT(pstVol,pstFd->uiFirstClu);
    fsClose(pstFd);

    return OK;
}

/*****************************************************************************
 function     : fsRename
 description  : rename file in the directory
 input        : 
 output       : 
 return       : 
*****************************************************************************/
STATUS fsRename(FS_FD_S *vpstFd,char *vpcName)
{
    FS_FDT_S           stFDT;
    FS_FDT_INFO_S      stFDTInfo;
    UINT32             uiLen = 0;
    UINT32             uiPathLevel = 0;
    UINT8             *pucPathBuf = NULL;
    FS_PATH_ARRAY_S   *pucPath = 0;
    
    pucPathBuf = malloc(FS_MAX_DIR_LEVELS * sizeof(FS_PATH_ARRAY_S));
	if(NULL == pucPathBuf)
	{
		return ERROR;
	}
	bzero(pucPathBuf,sizeof(FS_PATH_ARRAY_S) * FS_MAX_DIR_LEVELS);
    pucPath = (FS_PATH_ARRAY_S*)pucPathBuf;
    
    uiPathLevel = fsDirPathParse(vpcName,pucPath,FS_MAX_DIR_LEVELS);
    
    if(vpstFd->pstVol->uiRootClu == vpstFd->uiFirstClu)
    {
        FS_LOG(FS_DBG_ERR,"root cannot be changed\n",0,0,0,0,0,0);
        free(pucPathBuf);
        return ERROR;
    }
    /*jingzhong*/
    if(OK == gstFsCtrl.stFDTFunc.fsFindFDT(vpstFd->pstVol,pucPath[uiPathLevel-1].aucName,
                   vpstFd->uiDirFirstClu, (FS_FDT_INFO_S*)&stFDTInfo))
    {
        FS_LOG(FS_DBG_ERR,"file %d has exist\n",vpcName,0,0,0,0,0);
        free(pucPathBuf);
        return ERROR;
    }
    
    memset(&stFDT,0,sizeof(FS_FDT_S));
    uiLen = (UINT32)(MIN(strlen(pucPath[uiPathLevel-1].aucName),FS_MAX_NAME_LEN));/*jingzhong*/
    bcopy(pucPath[uiPathLevel-1].aucName,stFDT.aucName,uiLen);
	/*jingzhong*/
	memset(vpstFd->aucName,0,sizeof(vpstFd->aucName));
	bcopy(pucPath[uiPathLevel-1].aucName,vpstFd->aucName,uiLen);
	/*end*/
    stFDT.ucAttr = vpstFd->uiAttr;
    stFDT.ucFileStat = FS_FILE_STAT_SUBDIR;
    stFDT.uiFirstClu = vpstFd->uiFirstClu;
    stFDT.ullFileSize = vpstFd->ullFileSize;

    free(pucPathBuf);
    
    return gstFsCtrl.stFDTFunc.fsWriteFDT(vpstFd->pstVol,&stFDT,vpstFd->uiFDTCluId,
                                   vpstFd->uiFDTCluOffset);
}

/*****************************************************************************
 function     : fsCheckDirRelation
 description  : check file diretory relation ship
 input        : 
 output       : 
 return       : 
*****************************************************************************/
STATUS fsCheckDirRelation(FS_PATH_ARRAY_S *vpstSrcPath,FS_PATH_ARRAY_S *vpstDestPath,
                          UINT32 vuiSrcLevel,UINT32 vuiDestLevel)
{
    FS_PATH_ARRAY_S  *pstSrcPath = NULL;
    FS_PATH_ARRAY_S  *pstDestPath = NULL;
    UINT32            uiLoop = 0;
    
    if(vuiSrcLevel > vuiDestLevel)
    {
        return OK;
    }

    pstSrcPath = vpstSrcPath;
    pstDestPath = vpstDestPath;
    
    for(uiLoop = 0;uiLoop < vuiSrcLevel;uiLoop++)
    {
        if(strlen(pstSrcPath->aucName) != strlen(pstDestPath->aucName))
        {
            return OK;
        }
        if(OK != fsStrCmp(pstSrcPath->aucName,pstDestPath->aucName,
                          strlen(pstDestPath->aucName)))
        {
            return OK;
        }
        pstDestPath++;
        pstSrcPath++;
    }

    return ERROR;
}

 /*****************************************************************************
  function     : fsMove
  description  : move file from one directory to other directory
  input        : 
  output       : 
  return       : 
 *****************************************************************************/
STATUS fsMove(FS_FD_S *vpstFd,char *vpcName)
{
    FS_FDT_INFO_S      stSrcDir;
    FS_FDT_INFO_S      stDestDir;
    FS_FDT_INFO_S      stFile;
    UINT8             *pucSrcBuf = NULL;
    FS_PATH_ARRAY_S   *pucSrcPath = 0;
    UINT8             *pucDestBuf = NULL;
    FS_PATH_ARRAY_S   *pucDestPath = 0;
    UINT32             uiSrcLevel = 0;
    UINT32             uiDestLevel = 0;
    UINT64_S           ullSize;

    pucDestBuf = malloc(FS_MAX_DIR_LEVELS * sizeof(FS_PATH_ARRAY_S));
	if(NULL == pucDestBuf)
	{
		return ERROR;
	}
	bzero(pucDestBuf,sizeof(FS_PATH_ARRAY_S) * FS_MAX_DIR_LEVELS);

    pucSrcBuf = malloc(FS_MAX_DIR_LEVELS * sizeof(FS_PATH_ARRAY_S));
	if(NULL == pucSrcBuf)
	{
        free(pucDestBuf);
		return ERROR;
	}
	bzero(pucSrcBuf,sizeof(FS_PATH_ARRAY_S) * FS_MAX_DIR_LEVELS);
    
    pucSrcPath = (FS_PATH_ARRAY_S*)pucSrcBuf;
    uiSrcLevel = fsDirPathParse(vpstFd->aucFullName,pucSrcPath,FS_MAX_DIR_LEVELS);
	if(ERROR == uiSrcLevel)
	{
		FS_LOG(FS_DBG_ERR,"path %s error\n",(UINT32)vpcName,0,0,0,0,0);
        
	}
    
    pucDestPath = (FS_PATH_ARRAY_S*)pucDestBuf;
    uiDestLevel = fsDirPathParse(vpcName,pucDestPath,FS_MAX_DIR_LEVELS);
	if(ERROR == uiDestLevel)
	{
		FS_LOG(FS_DBG_ERR,"path %s error\n",(UINT32)vpcName,0,0,0,0,0);
        goto ErrExit;
	}
    
    if(OK != fsCheckDirRelation(pucSrcPath,pucDestPath,uiSrcLevel,uiDestLevel))
    {
        FS_LOG(FS_DBG_ERR,"move path %s to path %s logical error\n",
                vpstFd->aucFullName,(UINT32)vpcName,0,0,0,0);
        goto ErrExit;
    }
    
    /*
     *find dest directory information
    */
    bzero((CHAR*)(&stDestDir),sizeof(FS_FDT_INFO_S));
    if(uiDestLevel > 1)
    {
        if(OK != fsGetFile(vpstFd->pstVol,pucDestPath,(uiDestLevel - 1),&stDestDir))
        {
            FS_LOG(FS_DBG_ERR,"cannot find file %s\n",vpcName,0,0,0,0,0);
    		goto ErrExit;
        }
    }
    else
    {
        FS_LOG(FS_DBG_ERR,"cannot copy root file %s\n",vpcName,0,0,0,0,0);
        goto ErrExit;
    }
    
    /*
     *creat file FDT information in destination directory
    */
    bzero((CHAR*)(&stFile),sizeof(FS_FDT_INFO_S));
    bcopy(pucDestPath[uiDestLevel - 1].aucName,stFile.stFDT.aucName,
           MIN(FS_MAX_NAME_LEN,(1 + strlen((char *)(pucDestPath[uiDestLevel - 1].aucName)))));  
    stFile.stFDT.ucAttr = vpstFd->uiAttr;
    stFile.stFDT.ucFileStat = FS_FILE_STAT_SUBDIR;
    stFile.stFDT.uiFirstClu = vpstFd->uiFirstClu;
	stFile.stFDT.ucNameLen = MIN(strlen((char *)(stFile.stFDT.aucName)) + 1,FS_MAX_NAME_LEN);
    UINT64_SET(stFile.stFDT.ullFileSize,vpstFd->ullFileSize);
    
    if(OK != gstFsCtrl.stFDTFunc.fsGetModWrFDT(vpstFd->pstVol,
                                      stDestDir.stFDT.uiFirstClu,&stFile.stFDT))
    {
        goto ErrExit;
    }
   
    /*
     *update dest directory information
    */
    if(FS_CLUST_INVALID == stDestDir.stFDT.uiFirstClu)
    {
        stDestDir.stFDT.uiFirstClu = stFile.uiClu;
    }
                      
    UINT64_ADD(&stDestDir.stFDT.ullFileSize,&stDestDir.stFDT.ullFileSize,
                   sizeof(FS_FDT_S));  
    if(OK != gstFsCtrl.stFDTFunc.fsWriteFDT(vpstFd->pstVol,&stDestDir.stFDT,
                                            stDestDir.uiClu,
                                            stDestDir.uiCluOffset))
    {
        goto ErrExit;
    }
    
    /*
     *find source directory information
    */
    
    bzero((CHAR*)(&stSrcDir),sizeof(FS_FDT_INFO_S));
    
    if(uiSrcLevel > 1)
    {
        if(OK != fsGetFile(vpstFd->pstVol,pucSrcPath,(uiSrcLevel - 1),&stSrcDir))
        {
            FS_LOG(FS_DBG_ERR,"cannot find file %s line %d\n",vpcName,__LINE__,
                   0,0,0,0);
    		goto ErrExit;
        }
    }
    else
    {
        FS_LOG(FS_DBG_ERR,"cannot copy root file %s\n",vpcName,0,0,0,0,0);
        goto ErrExit;
    }

    /*
     *update source directory information
    */
    stFile.uiClu = vpstFd->uiFDTCluId;
    stFile.uiCluOffset = vpstFd->uiFDTCluOffset;
    if(OK != fsClearFileFromDir(vpstFd->pstVol,&stSrcDir,&stFile))
    {
        goto ErrExit;
    }

    free(pucSrcBuf);
    free(pucDestBuf);
    FS_LOG(FS_DBG_ERR,"move file %s to %s ok\n",vpstFd->aucFullName,vpcName,
           0,0,0,0);
    return OK;
    
ErrExit:  
    FS_LOG(FS_DBG_ERR,"move file %s error\n",vpcName,0,0,0,0,0);
    free(pucSrcBuf);
    free(pucDestBuf);
    return ERROR;
}

 /*****************************************************************************
  function     : fsNewFile
  description  : file system creat new file
  input        : vpstVol,vpcFileName
  output       : vpstFd
  return       : OK or ERROR
 *****************************************************************************/
 #if 1
/*因为fsNewFile还被其他函数调用,为不产生影响,先创建个临时函数,待测稳定后,合并为一个*/
STATUS fsNewFileTemp(FS_VOL_S *vpstVol,CHAR *vpcFileName,
			 FS_FDT_INFO_S *vpstDirInfo, FS_FDT_INFO_S *vpstFILEInfo, UINT32 uiAttr)
{
	 FS_FDT_INFO_S	 *stFDTInfo = NULL;
	 UINT32 	 uiStrLen = 0;
	 
	 /*printf("fsNewFile: vpcFileName: %s\n", vpcFileName);*/
 
	 stFDTInfo = vpstFILEInfo;
	 bzero((CHAR*)stFDTInfo,sizeof(FS_FDT_INFO_S));
	 
	 uiStrLen = MIN(strlen(vpcFileName) + 1,FS_MAX_NAME_LEN);
	 stFDTInfo->stFDT.ucNameLen = uiStrLen;
	 bcopy(vpcFileName, stFDTInfo->stFDT.aucName, uiStrLen);
	 
	 stFDTInfo->stFDT.ucFileStat = FS_FILE_STAT_SUBDIR;
	 
	 if (TRUE == fsIsRoot(vpcFileName))
	 {
		 stFDTInfo->stFDT.uiFirstClu = vpstVol->uiRootClu;
		 stFDTInfo->stFDT.ullFileSize.lo = sizeof(FS_FDT_S);
		 stFDTInfo->stFDT.ullFileSize.hi = 0;
	 }
	 else
	 {
		 stFDTInfo->stFDT.uiFirstClu = FS_CLUST_INVALID;
		 memset(&stFDTInfo->stFDT.ullFileSize,0,sizeof(UINT64_S));
	 }
	 
	 /*printf("stFDTInfo->stFDT.uiFirstClu: 0x%x \n", stFDTInfo->stFDT.uiFirstClu);*/

	 stFDTInfo->stFDT.ucAttr = uiAttr; 
	 
	 if (OK != gstFsCtrl.stFDTFunc.fsGetModWrFDT(vpstVol,
								vpstDirInfo->stFDT.uiFirstClu, stFDTInfo))
	 {
		 return ERROR;
	 }
	 if (TRUE == fsIsRoot(vpcFileName))
	 {
		 return OK;
	 }
	 
	 if (FS_CLUST_INVALID == vpstDirInfo->stFDT.uiFirstClu)
	 {
		 vpstDirInfo->stFDT.uiFirstClu = stFDTInfo->uiClu;
	 }
	 
	 UINT64_ADD(&vpstDirInfo->stFDT.ullFileSize,
		 &vpstDirInfo->stFDT.ullFileSize,sizeof(FS_FDT_S));
	 
	 if (OK != gstFsCtrl.stFDTFunc.fsWriteFDT(vpstVol, &vpstDirInfo->stFDT,
									  vpstDirInfo->uiClu, vpstDirInfo->uiCluOffset))
	 {
		 return ERROR;
	 }
	 
	 return OK;
}
#endif

 STATUS fsNewFile(FS_VOL_S *vpstVol,CHAR *vpcFileName,FS_FDT_INFO_S *vpstDirInfo,
                  UINT32 uiAttr)
 {
	FS_FDT_INFO_S  stFDTInfo;
	UINT32         uiStrLen = 0;

	/*printf("fsNewFile: vpcFileName: %s\n", vpcFileName);*/

	bzero((CHAR*)(&stFDTInfo),sizeof(FS_FDT_INFO_S));
	uiStrLen = MIN(strlen(vpcFileName) + 1,FS_MAX_NAME_LEN);
	stFDTInfo.stFDT.ucNameLen = uiStrLen;
	bcopy(vpcFileName,stFDTInfo.stFDT.aucName,uiStrLen);
	stFDTInfo.stFDT.ucFileStat = FS_FILE_STAT_SUBDIR;
	if(TRUE == fsIsRoot(vpcFileName))
	{
		stFDTInfo.stFDT.uiFirstClu = vpstVol->uiRootClu;
		stFDTInfo.stFDT.ullFileSize.lo = sizeof(FS_FDT_S);
		stFDTInfo.stFDT.ullFileSize.hi = 0;
	}
	else
	{
		stFDTInfo.stFDT.uiFirstClu = FS_CLUST_INVALID;
		memset(&stFDTInfo.stFDT.ullFileSize,0,sizeof(UINT64_S));
	}

	printf("stFDTInfo.stFDT.uiFirstClu: 0x%x \n", stFDTInfo.stFDT.uiFirstClu);

	stFDTInfo.stFDT.ucAttr = uiAttr; 

	if(OK != gstFsCtrl.stFDTFunc.fsGetModWrFDT(vpstVol,
	                              vpstDirInfo->stFDT.uiFirstClu,&stFDTInfo))
	{
		return ERROR;
	}

     /*
	*  update directory information
	*/
	if(TRUE == fsIsRoot(vpcFileName))
	{
		return OK;
	}

	if(FS_CLUST_INVALID == vpstDirInfo->stFDT.uiFirstClu)
	{
		vpstDirInfo->stFDT.uiFirstClu = stFDTInfo.uiClu;
	}
	
	UINT64_ADD(&vpstDirInfo->stFDT.ullFileSize,
	       &vpstDirInfo->stFDT.ullFileSize,sizeof(FS_FDT_S));

	if(OK != gstFsCtrl.stFDTFunc.fsWriteFDT(vpstVol,&vpstDirInfo->stFDT,
	                                    vpstDirInfo->uiClu,
	                                    vpstDirInfo->uiCluOffset))
	{
		return ERROR;
	}
	
	return OK;
}



/*****************************************************************************
function     : fsSaveOpenFile
description  : save open file
*****************************************************************************/
STATUS fsSaveOpenFile(FS_FD_S *vpstFd)
{
#ifndef FS_UPDATE_FDT_REALTIME
    FS_RECOVER_S stRcvr;
    UINT32       uiCluOffset = 0;

    if(O_RDONLY == vpstFd->uiFlag)
    {
        return OK;
    }
    assert(vpstFd->pstVol->uiCluByte >= (FS_RCVR_FD_SIZE * FS_MAX_FD));
    memset((CHAR*)&stRcvr,0,sizeof(FS_RECOVER_S));
    memcpy(stRcvr.aucName,vpstFd->aucName,FS_MAX_NAME_LEN);
    stRcvr.uiDirFirstClu = vpstFd->uiDirFirstClu;
    stRcvr.uiFDTCluId = vpstFd->uiFDTCluId;
    stRcvr.uiFDTCluOffset = vpstFd->uiFDTCluOffset;
    stRcvr.uiFirstClu = vpstFd->uiFirstClu;
    stRcvr.uiFlag = FS_RECOVER_MAGIC;
    uiCluOffset = vpstFd->uiRcvrId * FS_RCVR_FD_SIZE;
    if(OK != gstFsCtrl.stCacheFunc.fsCacheByteWrite(vpstFd->pstVol,
                              vpstFd->pstVol->uiStartClu,uiCluOffset,
                              &stRcvr,sizeof(FS_RECOVER_S)))
    {
        FS_LOG(FS_DBG_ERR,"save open file %s error\n",vpstFd->aucName,0,0,0,0,0);
        return ERROR;
    }

    FS_LOG(FS_DBG_DATA,"save open file %s cluster %d rcvrid %d ok\n",
           vpstFd->aucName,vpstFd->pstVol->uiStartClu,vpstFd->uiRcvrId,0,0,0);
#endif

    return OK;
}

/*****************************************************************************
function     : fsSaveOpenFile
description  : save open file
*****************************************************************************/
STATUS fsRestoreOpenFile(FS_FD_S *vpstFd)
{
#ifndef FS_UPDATE_FDT_REALTIME
    FS_RECOVER_S stRcvr;
    UINT32       uiCluOffset = 0;

    if(O_RDONLY == vpstFd->uiFlag)
    {
        return OK;
    }
    
    assert(vpstFd->pstVol->uiCluByte >= (FS_RCVR_FD_SIZE * FS_MAX_FD));
    memset((CHAR*)&stRcvr,0,sizeof(FS_RECOVER_S));
    uiCluOffset = vpstFd->uiRcvrId * FS_RCVR_FD_SIZE;
    if(OK != gstFsCtrl.stCacheFunc.fsCacheByteWrite(vpstFd->pstVol,
                              vpstFd->pstVol->uiStartClu,uiCluOffset,
                              &stRcvr,sizeof(FS_RECOVER_S)))
    {
        FS_LOG(FS_DBG_ERR,"restore file %s error\n",vpstFd->aucName,0,0,0,0,0);
        return ERROR;
    }

    FS_LOG(FS_DBG_DATA,"restore file %s ok\n",vpstFd->aucName,0,0,0,0,0);
#endif    
    return OK;
}

/*****************************************************************************bv
function     : fsRecoverFile
description  : recover file
*****************************************************************************/
STATUS fsRecoverFile(FS_VOL_S *vpstVol,FS_RECOVER_S *vpstRcvr)
{
    FS_FDT_S   stFDT;
    UINT32     uiCluNum = 0;
    UINT32     uiLastClu = 0;
    STATUS     iRet = OK;
    UINT64_S   ullMaxSize ;
    UINT64_S   ullMinSize ;

    memset(&stFDT,0,sizeof(FS_FDT_S));
    if(OK != gstFsCtrl.stFDTFunc.fsReadFDT(vpstVol,&stFDT,vpstRcvr->uiFDTCluId,
                                           vpstRcvr->uiFDTCluOffset))
    {
        FS_LOG(FS_DBG_ERR,"recover file %s error\n",vpstRcvr->aucName,0,0,0,0,0);
        return ERROR;
    }
    
    if(0 != memcmp(vpstRcvr->aucName,stFDT.aucName,FS_MAX_NAME_LEN))
    {
        FS_LOG(FS_DBG_ERR,"file name %s revr %s not equal\n",vpstRcvr->aucName,
               stFDT.aucName,0,0,0,0);
        return ERROR;
    }
    
    uiCluNum = gstFsCtrl.stEntFunc.fsGetLastEnt(vpstVol,stFDT.uiFirstClu,
                                                &uiLastClu);
    
    /*
     *check file size
    */
    if(0x0 == uiCluNum)
    {
       ullMinSize.hi = 0;
       ullMinSize.lo = 0;
       return OK;
    }
    
    fsGetFileSize((uiCluNum - 1),vpstVol->uiCluByte,&ullMinSize);
    fsGetFileSize(uiCluNum,vpstVol->uiCluByte,&ullMaxSize);
    if((UINT64_CMPGE(&ullMaxSize,&stFDT.ullFileSize)) &&
       (UINT64_CMPG(&stFDT.ullFileSize,&ullMinSize)))
    {
       return OK;
    }
    
    FS_LOG(FS_DBG_ERR,"recover file %s clunum %d size hi 0x%x lo 0x%x\n",
           stFDT.aucName,uiCluNum,stFDT.ullFileSize.hi,
           stFDT.ullFileSize.lo,0,0);
    stFDT.ullFileSize.hi = ullMaxSize.hi;
    stFDT.ullFileSize.lo = ullMaxSize.lo;
    iRet |= gstFsCtrl.stFDTFunc.fsWriteFDT(vpstVol,&stFDT,vpstRcvr->uiFDTCluId,
                                           vpstRcvr->uiFDTCluOffset);
    iRet |= gstFsCtrl.stEntFunc.fsWriteEnt(vpstVol,uiLastClu,FS_CLUST_EOF,
                                           FS_CACHE_WR_THROUGH);

    return OK;
}

/*****************************************************************************
function     : fsSaveOpenFile
description  : save open file
*****************************************************************************/
STATUS fsRecoverVol(FS_VOL_S *vpstVol)
{
    FS_RECOVER_S *pstRcvr = NULL;
    UINT8        *pucBuf = NULL;
    UINT32        uiLoop = 0;
    STATUS        iRet = OK;

    assert(vpstVol->uiCluByte >= (FS_RCVR_FD_SIZE * FS_MAX_FD));
    pucBuf = malloc(vpstVol->uiCluByte);
    if(NULL == pucBuf)
    {
    	FS_LOG(FS_DBG_ERR,"malloc memory failed\n",0,0,0,0,0,0);
    	return ERROR;
    }
    memset(pucBuf,0,vpstVol->uiCluByte);

    if(OK != gstFsCtrl.stCacheFunc.fsCacheCluRead(vpstVol,vpstVol->uiStartClu,
                                                  pucBuf))
    {
        free(pucBuf);
        return ERROR;
    }
    
    /*
     *find file that need recover
    */
    for(uiLoop = 0; uiLoop < FS_MAX_FD;uiLoop++)
    {
        pstRcvr = (FS_RECOVER_S*)(&pucBuf[uiLoop * FS_RCVR_FD_SIZE]);
        if(FS_RECOVER_MAGIC == pstRcvr->uiFlag)
        {
            iRet = fsRecoverFile(vpstVol,pstRcvr);
            if(OK != iRet)
            {
                FS_LOG(FS_DBG_ERR,"recover file %s failed\n",pstRcvr->aucName,
                       0,0,0,0,0);
                break;
            }
        }
    }

    memset(pucBuf,0,vpstVol->uiCluByte);
    if(OK != gstFsCtrl.stCacheFunc.fsCacheCluWrite(vpstVol,vpstVol->uiStartClu,
                                                   pucBuf))
    {
        free(pucBuf);
        return ERROR;
    }
    free(pucBuf);

    return iRet;
}

 /*****************************************************************************
 function     : fsOpen
 description  : open file api (create or truncate file)
 *****************************************************************************/
 #if 1
 INT32 fsOpen(FS_VOL_S *vpstVol,CHAR *vpcFileName,UINT32 vuiFlag,UINT32 vuiMode)
 {
	FS_FD_S           *pstFd = NULL;
	FS_PATH_ARRAY_S   *pucPath = 0;
	FS_FDT_INFO_S      stFileFDTInfo;
	FS_FDT_INFO_S      stDirFDTInfo;
	UINT8             *pucPathBuf = NULL;
	UINT32             uiPathLevel = 0;
	UINT32             uiAttr = 0;

	UINT32		   uiTimeStart = 0;
	UINT32		   uiTimeEnd = 0;


	if(strlen(vpcFileName) > FS_MAX_ABS_NAME)
	{
	    FS_LOG(FS_DBG_ERR,"name is too long\n",vpcFileName,0,0,0,0,0);
	    return ERROR;
	}

	pstFd = fsGetFd(vpstVol);
	if(NULL == pstFd)
	{
	    FS_LOG(FS_DBG_ERR,"get free fd failed\n",0,0,0,0,0,0);
	    return ERROR;
	}

	pucPathBuf = malloc(FS_MAX_DIR_LEVELS * sizeof(FS_PATH_ARRAY_S));
	if(NULL == pucPathBuf)
	{
		fsFreeFd(pstFd);
		return ERROR;
	}
	
	bzero(pucPathBuf,sizeof(FS_PATH_ARRAY_S) * FS_MAX_DIR_LEVELS);
	pucPath = (FS_PATH_ARRAY_S*)pucPathBuf;
	uiPathLevel = fsDirPathParse(vpcFileName,pucPath,FS_MAX_DIR_LEVELS);

	if (ERROR == uiPathLevel)
	{
		FS_LOG(FS_DBG_ERR,"path %s error\n",(UINT32)vpcFileName,0,0,0,0,0);
		fsFreeFd(pstFd);
		free(pucPathBuf);
		return ERROR;
	}
	/*semTake(vpstVol->stVolSem,WAIT_FOREVER);*/

	bzero((CHAR*)(&stDirFDTInfo),sizeof(FS_FDT_INFO_S));
	bzero((CHAR*)(&stFileFDTInfo),sizeof(FS_FDT_INFO_S));

	/*uiTimeStart = tickGet();*/
		
	if(uiPathLevel > 1)
	{
	    if(OK != fsGetFile(vpstVol,pucPath,(uiPathLevel - 1),&stDirFDTInfo))
	    {
	        FS_LOG(FS_DBG_ERR,"cannot find file %s\n",vpcFileName,0,0,0,0,0);
	  	  goto ErrExit;
	    }
	}

	/*uiTimeEnd = tickGet();
	uiTimeStart = uiTimeEnd;*/

	/*
	 * 获取文件夹FDT后, 判断是否是新建文件,此处要求新建文件名必须跟时间戳,即保证与
	 * 已有文件不同名
	 */
	if (vuiFlag & O_CREAT)
	{
		if (1 == uiPathLevel)
		{
			goto ErrExit;
		}

		if(vuiMode & S_IFDIR)
		{
			uiAttr |= FS_ATTR_DIRECTORY;
		}

		/*取得文件名*/
		pucPath = (FS_PATH_ARRAY_S*)pucPathBuf + (uiPathLevel - 1);
		
		semTake(vpstVol->stFsOpenSem,WAIT_FOREVER);

		/*new file的所在空闲cluster,是在stDirFDTInfo->FDT.firstCluster基础上查找的
		 *此处要看下 fsGetFile获取DIR信息时, 是否刚好落在一个有空闲cluster的簇上*/
		if(OK != fsNewFileTemp(vpstVol,&pucPath->aucName[0],&stDirFDTInfo, &stFileFDTInfo, uiAttr))
		{
			semGive(vpstVol->stFsOpenSem);
			FS_LOG(FS_DBG_ERR,"fsNewFile failed\n",0,0,0,0,0,0);
			goto ErrExit;
		}

		FS_LOG(FS_DBG_DATA,"creat file %s\n",pucPath->aucName,0,0,0,0,0);
		pstFd->uiOper |= FS_OPER_WRITE;
	
		semGive(vpstVol->stFsOpenSem);
#if 0		
		if(OK != fsGetFile(vpstVol,(FS_PATH_ARRAY_S*)pucPathBuf,uiPathLevel,
						   &stFileFDTInfo))
		{
			FS_LOG(FS_DBG_ERR,"fsGetFile failed\n",0,0,0,0,0,0);
			goto ErrExit;
		}
#endif

	}
	else  /*open 已有文件*/
	{
		if (OK != fsGetFile(vpstVol,pucPath,uiPathLevel,&stFileFDTInfo))
		{
		       FS_LOG(FS_DBG_DATA, "fsGetFile failed\n", 0,0,0,0,0,0);
			goto ErrExit;
		}

		if ((0 == (stFileFDTInfo.stFDT.ucAttr & FS_ATTR_DIRECTORY))&&
		   (0 != (vuiMode & S_IFDIR)))
		{
			FS_LOG(FS_DBG_ERR,"dir name %s same as filename\n",vpcFileName,
			       vuiFlag,vuiMode,0,0,0);
			goto ErrExit;
		}
	}


	/*uiTimeEnd = tickGet();
	uiTimeStart = uiTimeEnd;*/

	
	semTake(vpstVol->stFsOpenSem,WAIT_FOREVER);
	if(vuiFlag & O_TRUNC)
	{
		if(OK != gstFsCtrl.stEntFunc.fsClearEntChain(vpstVol,
		                                         stFileFDTInfo.stFDT.uiFirstClu))
		{
		    semGive(vpstVol->stFsOpenSem);
		    goto ErrExit;
		}
		stFileFDTInfo.stFDT.ullFileSize.hi = 0;
		stFileFDTInfo.stFDT.ullFileSize.lo = 0;
		stFileFDTInfo.stFDT.uiFirstClu = FS_CLUST_INVALID;
		pstFd->uiOper |= FS_OPER_WRITE;
	}
	pstFd->uiMode = vuiMode;
	pstFd->uiFlag = vuiFlag;
	pstFd->uiDirFirstClu = stDirFDTInfo.stFDT.uiFirstClu;

	if(OK != fsFillFdByFDTInfo(vpstVol,pstFd,&stFileFDTInfo,vuiFlag))
	{
	    semGive(vpstVol->stFsOpenSem);
	    FS_LOG(FS_DBG_ERR,"fill file %s information failed\n",
	           vpcFileName,0,0,0,0,0);
	    goto ErrExit; 
	}

	if(OK != fsSaveOpenFile(pstFd))
	{
	    semGive(vpstVol->stFsOpenSem);
	    goto ErrExit;
	}

	semGive(vpstVol->stFsOpenSem);
	bcopy(vpcFileName,pstFd->aucFullName,
	MIN((1 + strlen(vpcFileName)),FS_MAX_ABS_NAME)); 
	free(pucPathBuf);
	       
	FS_LOG(FS_DBG_DATA,"open vol %p file name %s 0x%x mode %x firstclu 0x%x"
	       " last 0x%x\n",
	       vpstVol,vpcFileName,vpcFileName,vuiMode,pstFd->uiFirstClu,
	       pstFd->uiLastClu);
	FS_LOG(FS_DBG_DATA,"size 0x%x%.8x fdt 0x%x 0x%x fd 0x%x\n",pstFd->ullFileSize.hi,
	       pstFd->ullFileSize.lo,pstFd->uiFDTCluId,
	       pstFd->uiFDTCluOffset,pstFd,0);


	/*uiTimeEnd = tickGet();
	uiTimeStart = uiTimeEnd;*/


	return (INT32)pstFd;

	ErrExit:
	FS_LOG(FS_DBG_DATA,"open file %s flag 0x%x mode 0x%x failed\n",vpcFileName,
	        vuiFlag,vuiMode,0,0,0);
	fsFreeFd(pstFd);
	free(pucPathBuf);
	return ERROR;
}
 
#else  /*default*/

INT32 fsOpen(FS_VOL_S *vpstVol,CHAR *vpcFileName,UINT32 vuiFlag,UINT32 vuiMode)
 {
	FS_FD_S           *pstFd = NULL;
	FS_PATH_ARRAY_S   *pucPath = 0;
	FS_FDT_INFO_S      stFileFDTInfo;
	FS_FDT_INFO_S      stDirFDTInfo;
	UINT8             *pucPathBuf = NULL;
	UINT32             uiPathLevel = 0;
	UINT32             uiAttr = 0;

	if(strlen(vpcFileName) > FS_MAX_ABS_NAME)
	{
	    FS_LOG(FS_DBG_ERR,"name is too long\n",vpcFileName,0,0,0,0,0);
	    return ERROR;
	}

	pstFd = fsGetFd(vpstVol);
	if(NULL == pstFd)
	{
	    FS_LOG(FS_DBG_ERR,"get free fd failed\n",0,0,0,0,0,0);
	    return ERROR;
	}

	pucPathBuf = malloc(FS_MAX_DIR_LEVELS * sizeof(FS_PATH_ARRAY_S));
	if(NULL == pucPathBuf)
	{
		fsFreeFd(pstFd);
		return ERROR;
	}
	
	bzero(pucPathBuf,sizeof(FS_PATH_ARRAY_S) * FS_MAX_DIR_LEVELS);
	pucPath = (FS_PATH_ARRAY_S*)pucPathBuf;
	uiPathLevel = fsDirPathParse(vpcFileName,pucPath,FS_MAX_DIR_LEVELS);
	if(ERROR == uiPathLevel)
	{
		FS_LOG(FS_DBG_ERR,"path %s error\n",(UINT32)vpcFileName,0,0,0,0,0);
	    fsFreeFd(pstFd);
	    free(pucPathBuf);
	    return ERROR;
	}
	/*semTake(vpstVol->stVolSem,WAIT_FOREVER);*/

	bzero((CHAR*)(&stDirFDTInfo),sizeof(FS_FDT_INFO_S));
	bzero((CHAR*)(&stFileFDTInfo),sizeof(FS_FDT_INFO_S));

	printf("\nfsOpen, filename: %s, pucPath: %s, uiPathLevel: %d \n", vpcFileName, pucPath, uiPathLevel);
		
	if(uiPathLevel > 1)
	{
	    if(OK != fsGetFile(vpstVol,pucPath,(uiPathLevel - 1),&stDirFDTInfo))
	    {
	        FS_LOG(FS_DBG_ERR,"cannot find file %s\n",vpcFileName,0,0,0,0,0);
	  	  goto ErrExit;
	    }
	}

	if(OK != fsGetFile(vpstVol,pucPath,uiPathLevel,&stFileFDTInfo))
	{
		if((0x0 == (vuiFlag & (O_CREAT))) || (1 == uiPathLevel))
		{
			goto ErrExit;
		}
		if(vuiMode & S_IFDIR)
		{
			uiAttr |= FS_ATTR_DIRECTORY;
		}

		pucPath = (FS_PATH_ARRAY_S*)pucPathBuf;
		pucPath = pucPath + (uiPathLevel - 1);

		semTake(vpstVol->stVolSem,WAIT_FOREVER);
		if(OK != fsNewFile(vpstVol,&pucPath->aucName[0],&stDirFDTInfo,uiAttr))
		{
		    semGive(vpstVol->stVolSem);
		    FS_LOG(FS_DBG_ERR,"fsNewFile failed\n",0,0,0,0,0,0);
			goto ErrExit;
		}
		else
		{
		    FS_LOG(FS_DBG_ERR,"creat file %s\n",pucPath->aucName,0,0,0,0,0);
		    pstFd->uiOper |= FS_OPER_WRITE;
		}
		semGive(vpstVol->stVolSem);
		if(OK != fsGetFile(vpstVol,(FS_PATH_ARRAY_S*)pucPathBuf,uiPathLevel,
		                   &stFileFDTInfo))
		{
		    FS_LOG(FS_DBG_ERR,"fsGetFile failed\n",0,0,0,0,0,0);
		    goto ErrExit;
		}
	}
	else
	{
		if((0 == (stFileFDTInfo.stFDT.ucAttr & FS_ATTR_DIRECTORY))&&
		   (0 != (vuiMode & S_IFDIR)))
		{
			FS_LOG(FS_DBG_ERR,"dir name %s same as filename\n",vpcFileName,
			       vuiFlag,vuiMode,0,0,0);

			goto ErrExit;
		}
	}
	
	semTake(vpstVol->stVolSem,WAIT_FOREVER);
	if(vuiFlag & O_TRUNC)
	{
		if(OK != gstFsCtrl.stEntFunc.fsClearEntChain(vpstVol,
		                                         stFileFDTInfo.stFDT.uiFirstClu))
		{
		    semGive(vpstVol->stVolSem);
		    goto ErrExit;
		}
		stFileFDTInfo.stFDT.ullFileSize.hi = 0;
		stFileFDTInfo.stFDT.ullFileSize.lo = 0;
		stFileFDTInfo.stFDT.uiFirstClu = FS_CLUST_INVALID;
		pstFd->uiOper |= FS_OPER_WRITE;
	}
	pstFd->uiMode = vuiMode;
	pstFd->uiFlag = vuiFlag;
	pstFd->uiDirFirstClu = stDirFDTInfo.stFDT.uiFirstClu;

	if(OK != fsFillFdByFDTInfo(vpstVol,pstFd,&stFileFDTInfo,vuiFlag))
	{
	    semGive(vpstVol->stVolSem);
	    FS_LOG(FS_DBG_ERR,"fill file %s information failed\n",
	           vpcFileName,0,0,0,0,0);
	    goto ErrExit; 
	}

	if(OK != fsSaveOpenFile(pstFd))
	{
	    semGive(vpstVol->stVolSem);
	    goto ErrExit;
	}

	semGive(vpstVol->stVolSem);
	bcopy(vpcFileName,pstFd->aucFullName,
	MIN((1 + strlen(vpcFileName)),FS_MAX_ABS_NAME)); 
	free(pucPathBuf);
	       
	FS_LOG(FS_DBG_DATA,"open vol %p file name %s 0x%x mode %x firstclu 0x%x"
	       " last 0x%x\n",
	       vpstVol,vpcFileName,vpcFileName,vuiMode,pstFd->uiFirstClu,
	       pstFd->uiLastClu);
	FS_LOG(FS_DBG_DATA,"size 0x%x%.8x fdt 0x%x 0x%x fd 0x%x\n",pstFd->ullFileSize.hi,
	       pstFd->ullFileSize.lo,pstFd->uiFDTCluId,
	       pstFd->uiFDTCluOffset,pstFd,0);

	return (INT32)pstFd;

	ErrExit:
	FS_LOG(FS_DBG_DATA,"open file %s flag 0x%x mode 0x%x failed\n",vpcFileName,
	        vuiFlag,vuiMode,0,0,0);
	fsFreeFd(pstFd);
	free(pucPathBuf);
	return ERROR;
}



#endif




 /*****************************************************************************
 function     : fsClose
 description  : close file
 *****************************************************************************/
 INT32 fsClose(FS_FD_S *vpstFd)
 {
    STATUS iRet = 0;

    FS_LOG(FS_DBG_DATA,"close file %s size hi %d lo %d oper %d\n",
           vpstFd->aucFullName,vpstFd->ullFileSize.hi,vpstFd->ullFileSize.lo,vpstFd->uiOper,0,0);
           
    if(0 != (FS_OPER_WRITE & vpstFd->uiOper))
    {
        iRet = fsUpdateFile(vpstFd);
    }
    
    semTake(vpstFd->pstVol->stVolSem,WAIT_FOREVER);
    fsRestoreOpenFile(vpstFd);
    semGive(vpstFd->pstVol->stVolSem);
    
    fsFreeFd(vpstFd);

    return iRet;
 }

/*****************************************************************************
function     : fsCreate
description  : create file
input        : 
output       : 
return       : 
*****************************************************************************/
INT32 fsCreate(FS_VOL_S *vpstVol,CHAR *vpcFileName,UINT32 vuiFlag)
{
	printf("fsCreate is called\n");
    
    return fsOpen(vpstVol,vpcFileName,vuiFlag | O_CREAT,S_IFREG);
}

/*****************************************************************************
 function     : fsDelete
 description  : delete file
 input        : 
 output       : 
 return       : 
*****************************************************************************/
STATUS fsDelete(FS_VOL_S *vpstVol,CHAR *vpcFileName)
{
    FS_FDT_INFO_S      stFileFDTInfo;
    FS_FDT_INFO_S      stDirFDTInfo;
    UINT8             *pucPathBuf = NULL;
    FS_PATH_ARRAY_S   *pstPath = NULL;
    UINT32             uiPathLevel = 0;
    STATUS             iRet = ERROR;

    pucPathBuf = malloc(FS_MAX_DIR_LEVELS * sizeof(FS_PATH_ARRAY_S));
	if(NULL == pucPathBuf)
	{
		return ERROR;
	}
	bzero((CHAR*)pucPathBuf,sizeof(FS_PATH_ARRAY_S) * FS_MAX_DIR_LEVELS);
    pstPath = (FS_PATH_ARRAY_S*)pucPathBuf;
    uiPathLevel = fsDirPathParse(vpcFileName,pstPath,FS_MAX_DIR_LEVELS);
	if((ERROR == uiPathLevel) || (0x1 == uiPathLevel))
	{
		FS_LOG(FS_DBG_ERR,"path %s error\n",vpcFileName,0,0,0,0,0);
        free(pucPathBuf);
        return ERROR;
	}
    bzero((CHAR*)(&stDirFDTInfo),sizeof(FS_FDT_INFO_S));
    bzero((CHAR*)(&stFileFDTInfo),sizeof(FS_FDT_INFO_S));
	
	semTake(vpstVol->stVolSem,WAIT_FOREVER);
    if(OK != fsGetFile(vpstVol,pstPath,(uiPathLevel - 1),&stDirFDTInfo))
    {
        FS_LOG(FS_DBG_ERR,"cannot find file %s\n",vpcFileName,0,0,0,0,0);
		goto ErrExit;
    }
    if(FS_ATTR_DIRECTORY != (stDirFDTInfo.stFDT.ucAttr & FS_ATTR_DIRECTORY))
    {
        
        FS_LOG(FS_DBG_ERR,"%s is not a directory\n",stDirFDTInfo.stFDT.aucName,
               0,0,0,0,0);
		goto ErrExit;
    }
    if(OK != fsGetFile(vpstVol,pstPath,uiPathLevel,&stFileFDTInfo))
    {
        FS_LOG(FS_DBG_ERR,"cannot find file %s\n",vpcFileName,0,0,0,0,0);
		goto ErrExit;
    }

    if((FS_ATTR_DIRECTORY == (stFileFDTInfo.stFDT.ucAttr & FS_ATTR_DIRECTORY))&&
       (!UINT64_ISZERO(&stFileFDTInfo.stFDT.ullFileSize)))
    {
        FS_LOG(FS_DBG_ERR,"directory %s is not empty %d\n",vpcFileName,
               stDirFDTInfo.stFDT.ullFileSize.lo,0,0,0,0);
		goto ErrExit;
    }
    
    if(OK != gstFsCtrl.stEntFunc.fsClearEntChain(vpstVol,
                                                 stFileFDTInfo.stFDT.uiFirstClu))
    {
        goto ErrExit;
    }

    if(OK != fsClearFileFromDir(vpstVol,&stDirFDTInfo,&stFileFDTInfo))
    {
        goto ErrExit;
    }
    iRet = OK;
    
ErrExit:
    semGive(vpstVol->stVolSem);
    free(pucPathBuf);
    return iRet;
}

/*****************************************************************************
 function     : fsUpdateFileSize
 description  : update file size
 return       : OK[file size is updated] ERROR[file size not updated]
*****************************************************************************/
STATUS fsUpdateFileSize(FS_VOL_S *pstVol,FS_FD_S *vpstFd)
{
	CHAR     *pcName = NULL;
	FS_FD_S  *pstCurFd = NULL;
	UINT32    uiLoop = 0;
	UINT32    uiLen = 0;
	FS_FDT_S  stFDT;
	STATUS    iRet = 0;

	for(uiLoop = 0;uiLoop < FS_MAX_FD;uiLoop++)
	{
		pstCurFd = &pstVol->astFd[uiLoop];
		if(pstCurFd == vpstFd)
		{
			continue;
		}
		if((FS_FD_USED == pstCurFd->uiUsed) &&
		   (FS_OPER_WRITE == pstCurFd->uiOper) &&
		   (0 == strcmp(vpstFd->aucFullName,pstCurFd->aucFullName)))
		{
			if(UINT64_CMPG(&pstCurFd->ullFileSize,&vpstFd->ullFileSize))
			{
				FS_LOG(FS_DBG_DATA,"find fd %x update file size %x.%x %d ok\n",pstCurFd,
					   pstCurFd->ullFileSize.hi,
					   pstCurFd->ullFileSize.lo,pstCurFd->uiUsed,0,0);
				vpstFd->ullFileSize.lo = pstCurFd->ullFileSize.lo;
				vpstFd->ullFileSize.hi = pstCurFd->ullFileSize.hi;
				return OK;
			}
		}	
	}
	/*
     * check file has been write or not
	*/
	semTake(vpstFd->pstVol->stVolSem,WAIT_FOREVER);
	iRet = gstFsCtrl.stFDTFunc.fsReadFDT(vpstFd->pstVol,&stFDT,vpstFd->uiFDTCluId,
                                            vpstFd->uiFDTCluOffset);
	semGive(vpstFd->pstVol->stVolSem);
	if(OK != iRet)
	{
		return ERROR;
	}
	if(UINT64_CMPG(&stFDT.ullFileSize,&vpstFd->ullFileSize))
	{
		FS_LOG(FS_DBG_DATA,"update file size %x.%x ok\n",stFDT.ullFileSize.hi,
			   stFDT.ullFileSize.lo,0,0,0,0);
		vpstFd->ullFileSize.lo = stFDT.ullFileSize.lo;
		vpstFd->ullFileSize.hi = stFDT.ullFileSize.hi;
		return 0;
	}

	return ERROR;
}

/*****************************************************************************
 function     : fsRead
 description  : read file to buffer
 input        : 
 output       : 
 return       : 
*****************************************************************************/
INT32 fsRead(FS_FD_S *vpstFd,CHAR *vpcBuf,UINT32 vuiByte)
{  
    FS_VOL_S    *pstVol = NULL;
    UINT32       uiRdLen = 0;
    UINT32       uiRest = 0;
    UINT32       uiCluLen = 0;
    INT32        iActRdByte = 0;
    UINT64_S     ullFileRest;
    
    vpstFd->uiOper |= FS_OPER_READ;
	pstVol = vpstFd->pstVol;
    if((FS_CLUST_EOF == vpstFd->uiCurClu) || (FS_CLUST_INVALID == vpstFd->uiCurClu))
    {
        if(OK != fsUpdateFileSize(pstVol,vpstFd))
        {
			return 0;
		}
    }
    
    uiRest = vuiByte;

    UINT64_SUB64(&ullFileRest,&vpstFd->ullFileSize,&vpstFd->ullPos);
    if((0x0 == ullFileRest.hi) && (vuiByte > ullFileRest.lo) &&
       (0 == (vpstFd->uiAttr & FS_ATTR_DIRECTORY)))
    {
        uiRest = ullFileRest.lo;
    }
    if(0x0 == uiRest)
    {
        if(OK != fsUpdateFileSize(pstVol,vpstFd))
        {
			return 0;
		}
		UINT64_SUB64(&ullFileRest,&vpstFd->ullFileSize,&vpstFd->ullPos);
		uiRest = vuiByte;
		if((0x0 == ullFileRest.hi) && (vuiByte > ullFileRest.lo) &&
           (0 == (vpstFd->uiAttr & FS_ATTR_DIRECTORY)))
        {
            uiRest = ullFileRest.lo;
        }
    }
    
    if(0 == vpstFd->uiCluOffset)
    {
        return fsReadAlign(vpstFd,vpcBuf,uiRest);
    }   
    uiCluLen = pstVol->uiCluByte;
    uiRdLen = MIN((uiCluLen - vpstFd->uiCluOffset),uiRest);

    if(OK != gstFsCtrl.stCacheFunc.fsCacheByteRead(pstVol,vpstFd->uiCurClu,
                                          vpstFd->uiCluOffset,vpcBuf,uiRdLen))
    {
        return ERROR;
    }
    UINT64_ADD(&vpstFd->ullPos,&vpstFd->ullPos,uiRdLen);
    vpstFd->uiCluOffset = (vpstFd->uiCluOffset + uiRdLen) % uiCluLen;
    uiRest = uiRest - uiRdLen;    
    if(0x00 == vpstFd->uiCluOffset)
    {
		semTake(vpstFd->pstVol->stVolSem,WAIT_FOREVER);
		if(FS_CLUST_EOF == vpstFd->uiCurClu)
        {
           FS_LOG(FS_DBG_ERR,"fsRead curr free clu invalid\n",
                  0,0,0,0,0,0);
        }
        gstFsCtrl.stEntFunc.fsReadEnt(vpstFd->pstVol,vpstFd->uiCurClu,
                                      &vpstFd->uiCurClu);
		semGive(vpstFd->pstVol->stVolSem);
    }

    if(0x0 != uiRest)
    {
        vpstFd->uiCluOffset = 0;
        iActRdByte = fsReadAlign(vpstFd,&(vpcBuf[uiRdLen]),uiRest); 
    }
    iActRdByte = iActRdByte + uiRdLen;
    
    return iActRdByte;
}

/*****************************************************************************
 function     : fsWrite
 description  : write buffer content to disk
 input        : 
 output       : 
 return       : 
*****************************************************************************/
UINT32 fsCalcCluOffset(UINT32 vuiSizeHi,UINT32 vuiSizeLo,UINT32 uiCluLen)
{
    UINT32 uiMbyte = 0;
    UINT32 uiByte = 0;
    UINT32 uiResvMByte = 0;
    UINT32 uiCluOffset = 0;

    uiMbyte = (vuiSizeHi << 12)  | (vuiSizeLo >> 20);
    uiByte = (vuiSizeLo & 0xFFFFF) % uiCluLen;
    uiResvMByte = uiMbyte % (uiCluLen >> 20);
    uiCluOffset = (uiResvMByte << 20) | uiByte;
    uiCluOffset = uiCluOffset % uiCluLen;
    
    return uiCluOffset;
}

/*****************************************************************************
 function     : fsWrite
 description  : write buffer content to disk
 input        : 
 output       : 
 return       : 
*****************************************************************************/
INT32 fsWrite(FS_FD_S *vpstFd,CHAR *vpcBuf,UINT32 vuiByte)
{     
    FS_VOL_S    *pstVol = NULL;
    UINT32       uiCurClu = FS_CLUST_INVALID;
    UINT32       uiNextClu = FS_CLUST_INVALID;
    UINT32       uiCluOffset = 0;
    UINT32       uiMbyte = 0;
    UINT32       uiWrLen = 0;
    UINT32       uiCluLen = 0;

    if(O_RDONLY == vpstFd->uiFlag)
    {
        FS_LOG(FS_DBG_ERR,"file is open in readonly\n",0,0,0,0,0,0);
        return 0;
    }
    
    pstVol = vpstFd->pstVol;
    uiCluLen = pstVol->uiCluByte;
    uiCluOffset = fsCalcCluOffset(vpstFd->ullPos.hi,vpstFd->ullPos.lo,
                                  uiCluLen);
    if(0 == uiCluOffset)
    {
        if(OK != fsWriteAlign(vpstFd,vpcBuf,vuiByte))
        {
            return 0;
        }
        
        goto EXIT_OK;
    }
    
    /*
     *fill the space in the last cluster of the file 
    */
    uiCurClu = vpstFd->uiCurClu;
    uiWrLen = MIN((uiCluLen - uiCluOffset),vuiByte);
    if(OK != gstFsCtrl.stCacheFunc.fsCacheByteWrite(pstVol,uiCurClu,uiCluOffset,
                                                    vpcBuf,uiWrLen))
    {
        return 0;
    }
    vpstFd->uiOper |= FS_OPER_WRITE;
    UINT64_ADD(&vpstFd->ullPos,&vpstFd->ullPos,uiWrLen);
    if(UINT64_CMPG(&vpstFd->ullPos,&vpstFd->ullFileSize))
    {
        vpstFd->ullFileSize.hi = vpstFd->ullPos.hi;
        vpstFd->ullFileSize.lo = vpstFd->ullPos.lo;
    }

    /*
     *current cluster is full;jump to next cluster
    */
    if((uiCluOffset + uiWrLen) >= uiCluLen)
    {
        semTake(vpstFd->pstVol->stVolSem,WAIT_FOREVER);
        if(OK != gstFsCtrl.stEntFunc.fsReadEnt(vpstFd->pstVol,uiCurClu,
                                               &uiNextClu))
        {
            FS_LOG(FS_DBG_ERR,"fsWrite entry failed\n",0,0,0,0,0,0);
             
            semGive(vpstFd->pstVol->stVolSem);
            return 0;
        }
        semGive(vpstFd->pstVol->stVolSem);
        vpstFd->uiPreClu = uiCurClu;
        vpstFd->uiCurClu = uiNextClu; 
    }
    
    /*
     *there is other data to write
    */
    if(0 != (vuiByte - uiWrLen))
    {
        if(OK != fsWriteAlign(vpstFd,&vpcBuf[uiWrLen],(vuiByte - uiWrLen)))
        {
            return 0;
        }
        
        goto EXIT_OK;
    }

EXIT_OK:

#ifdef FS_UPDATE_FDT_REALTIME
    if(OK != fsUpdateFile(vpstFd))
    {
        FS_LOG(FS_DBG_ERR,"fsWrite update file %s failed\n",vpstFd->aucFullName,
               0,0,0,0,0);
    }
#endif

    
    return vuiByte;
}

/*****************************************************************************
 function     : fsGetVolCap
 description  : get vol total capacity
*****************************************************************************/
STATUS fsGetVolCap(FS_FD_S *vpstFd,INT32 viArg)
{
    UINT32 uiTotalClu = 0;
    UINT32 uiMByte = 0;
    
    uiTotalClu = vpstFd->pstVol->uiVolClu - vpstFd->pstVol->uiFatClu + 1;
    uiMByte = vpstFd->pstVol->uiCluByte >> 20;
    uiMByte = uiMByte * uiTotalClu;
    *(UINT32*)viArg = uiMByte;

    return OK;
}

/*****************************************************************************
 function     : fsGetVolCap
 description  : get vol free capacity
*****************************************************************************/
STATUS fsGetVolFreeCap(FS_FD_S *vpstFd,INT32 viArg)
{
    UINT32 uiFreeClu = 0;
    UINT32 uiMByte = 0;

    
    fsGetVolFreeClu(vpstFd->pstVol,&uiFreeClu);
    uiMByte = vpstFd->pstVol->uiCluByte >> 20;
    uiMByte = uiMByte * uiFreeClu;
    *(UINT32*)viArg = uiMByte;

    return OK;
}

/*****************************************************************************
 function     : fsSeek
 description  : file seek
*****************************************************************************/
STATUS fsSeek(FS_FD_S *vpstFd,INT32  viArg)
{
    UINT32 uiCluNum = 0;
    UINT32 uiLoop = 0;
    UINT32 uiOffset = 0;

    uiOffset = viArg;
    if((0 == vpstFd->ullFileSize.hi) && (viArg > vpstFd->ullFileSize.lo))
    {
        uiOffset = vpstFd->ullFileSize.lo;
    }
    
    uiCluNum = uiOffset / vpstFd->pstVol->uiCluByte;
    vpstFd->uiCurClu = vpstFd->uiFirstClu;
    vpstFd->uiPreClu = FS_CLUST_INVALID;
    
    for(uiLoop = 0;uiLoop < uiCluNum;uiLoop++)
    {
        semTake(vpstFd->pstVol->stVolSem,WAIT_FOREVER);
        if(FS_CLUST_EOF == vpstFd->uiCurClu)
        {
           FS_LOG(FS_DBG_ERR,"fsSeek curr free clu invalid\n",
                  0,0,0,0,0,0);
        }
        vpstFd->uiPreClu = vpstFd->uiCurClu;    
        gstFsCtrl.stEntFunc.fsReadEnt(vpstFd->pstVol,vpstFd->uiCurClu,
                                      &vpstFd->uiCurClu);
    	semGive(vpstFd->pstVol->stVolSem);
    }
    vpstFd->uiCluOffset = uiOffset - (uiCluNum * vpstFd->pstVol->uiCluByte);
    vpstFd->ullPos.lo = uiOffset;
    vpstFd->ullPos.hi = 0;
    
    return OK;
}

/*****************************************************************************
 function     : fsSeek
 description  : file seek
*****************************************************************************/
STATUS fsSeek64(FS_FD_S *vpstFd,INT32  viArg)
{
    UINT32  uiCluNum = 0;
    UINT32  uiLoop = 0;
    UINT32  uiHi = 0;
    UINT32  uiLo = 0;
    UINT32  uiMByte = 0;
    UINT32  uiMBAlign = 0;
    UINT32  uiOffset = 0;
    INT64   ullPos = 0;

    if(NULL == viArg)
    {
        return ERROR;
    }
    ullPos = *(INT64*)viArg;

    uiHi = ullPos >> 32;
    uiLo = ullPos & 0xffffffff;

    /*
     *seek over file size change to eof of the file
    */
    if((uiHi > vpstFd->ullFileSize.hi) ||
       ((uiHi == vpstFd->ullFileSize.hi) &&
        (uiLo > vpstFd->ullFileSize.lo)))
    {
    	FS_LOG(FS_DBG_ERR,"fsSeek64 hi 0x%x lo 0x%x size 0x%x 0x%x overflow\n",
                  uiHi,uiLo,vpstFd->ullFileSize.hi,vpstFd->ullFileSize.lo,0,0);         
        uiHi = 0;/*vpstFd->ullFileSize.hi*/;
        uiLo = 0;/*vpstFd->ullFileSize.lo*/;
    }

    ullPos = uiHi;
    ullPos = (UINT64)uiHi << 32;
    ullPos |= uiLo;
    
    uiCluNum = ullPos / vpstFd->pstVol->uiCluByte;
    vpstFd->uiCurClu = vpstFd->uiFirstClu;
    vpstFd->uiPreClu = FS_CLUST_INVALID;
    for(uiLoop = 0;uiLoop < uiCluNum;uiLoop++)
    {
        semTake(vpstFd->pstVol->stVolSem,WAIT_FOREVER);
        if(FS_CLUST_EOF == vpstFd->uiCurClu)
        {
        	FS_LOG(FS_DBG_ERR,"fsSeek64 curr free clu size 0x%x 0x%x"
                  " loop %d cluNum %d invalid\n",vpstFd->ullFileSize.hi,
                  vpstFd->ullFileSize.lo,uiLoop,uiCluNum,0,0);
        }
        vpstFd->uiPreClu = vpstFd->uiCurClu;
        gstFsCtrl.stEntFunc.fsReadEnt(vpstFd->pstVol,vpstFd->uiCurClu,
                                      &vpstFd->uiCurClu);
    	semGive(vpstFd->pstVol->stVolSem);
    }
    uiMBAlign = uiCluNum * (vpstFd->pstVol->uiCluByte / 0x100000);
    
    vpstFd->uiCluOffset = ullPos % vpstFd->pstVol->uiCluByte;
    vpstFd->ullPos.lo = uiLo;
    vpstFd->ullPos.hi = uiHi;

    return OK;
}

/*****************************************************************************
 function     : fsSetTime
 description  : file seek
*****************************************************************************/
STATUS fsSetAttr(FS_FD_S *vpstFd,FS_ATTR_S *pstAttr)
{
    vpstFd->uiTime = pstAttr->uiTime;
    vpstFd->uiStartFrame = pstAttr->uiStartFrame;
    vpstFd->uiEndFrame = pstAttr->uiEndFrame;
    vpstFd->uiOper |= FS_OPER_WRITE;	
    return OK;
}
STATUS fsGetAttr(FS_FD_S *vpstFd,FS_ATTR_S *pstAttr)
{

	memset(pstAttr,0,sizeof(FS_ATTR_S));
    pstAttr->uiTime = vpstFd->uiTime;
    pstAttr->uiStartFrame = vpstFd->uiStartFrame;
    pstAttr->uiEndFrame = vpstFd->uiEndFrame;
	pstAttr->uiAttr = vpstFd->uiAttr;
	bcopy(vpstFd->aucName, pstAttr->aucName, FS_MAX_NAME_LEN);
	UINT64_SET(pstAttr->ullFileSize,vpstFd->ullFileSize);
	return OK;
}

/*****************************************************************************
 function     : fsPreAlloc
 description  : create file of specified size 
*****************************************************************************/
STATUS fsPreAlloc(FS_FD_S *vpstFd,INT32  viArg)
{
	UINT32  uiFatNum = 0;
	UINT64  ullSize = 0;
	UINT32  uiHi = 0;
	UINT32  uiLo = 0;
	UINT32  uiLoop = 0;
	UINT32  uiCurClu = 0;
	UINT32  uiNextClu = 0;
	UINT32  uiPreClu = 0;

	ullSize =  *(UINT64*)viArg;
	uiHi = (UINT32)(ullSize >> 32);
	uiLo = (UINT32) ullSize;
    FS_LOG(FS_DBG_ERR,"file truncate to new length %x.%x\n",
           uiHi,uiLo,0,0,0,0);
	uiFatNum = ullSize / vpstFd->pstVol->uiCluByte;
	if(ullSize % vpstFd->pstVol->uiCluByte)
	{
		uiFatNum++;
	}
	if((uiHi < vpstFd->ullFileSize.hi) ||
	   ((uiHi == vpstFd->ullFileSize.hi) && (uiLo < vpstFd->ullFileSize.lo)))
	{
		FS_LOG(FS_DBG_ERR,"file size big than set size\n",0,0,0,0,0,0);
		return ERROR;
	}
	uiCurClu = vpstFd->uiFirstClu;
	semTake(vpstFd->pstVol->stVolSem,WAIT_FOREVER);
	if((FS_CLUST_INVALID == uiCurClu) || (FS_CLUST_EOF == uiCurClu))
    {
       if(OK != gstFsCtrl.stEntFunc.fsGetModLastEnt(vpstFd->pstVol,
                                                    FS_CLUST_INVALID,&uiCurClu))
       {
           semGive(vpstFd->pstVol->stVolSem);
           return ERROR;
       }
       vpstFd->uiFirstClu = uiCurClu;
    }
	uiNextClu = uiCurClu;
	uiPreClu = uiCurClu;
	for(uiLoop = 1;uiLoop < uiFatNum;uiLoop++)
	{
		if((FS_CLUST_INVALID == uiCurClu) || (FS_CLUST_EOF == uiCurClu))
		{
			if(OK != gstFsCtrl.stEntFunc.fsGetModLastEnt(vpstFd->pstVol,uiPreClu,
				                                         &uiCurClu))
            {
                semGive(vpstFd->pstVol->stVolSem);
                return ERROR;
            }
			vpstFd->uiLastClu = uiCurClu;
		}
		if(OK != gstFsCtrl.stEntFunc.fsReadEnt(vpstFd->pstVol,uiCurClu,
                                               &uiNextClu))
        {
            FS_LOG(FS_DBG_ERR,"fsWrite entry failed\n",0,0,0,0,0,0);
             
            semGive(vpstFd->pstVol->stVolSem);
            return ERROR;
        }
		uiPreClu = uiCurClu;
		uiCurClu = uiNextClu;
	}
	vpstFd->ullFileSize.hi = uiHi;
	vpstFd->ullFileSize.lo = uiLo;
	semGive(vpstFd->pstVol->stVolSem);
	fsUpdateFile(vpstFd);
}

/*****************************************************************************
 function     : fsTrunc
 description  : file truncate
*****************************************************************************/
STATUS fsTrunc(FS_FD_S *vpstFd,INT32  viArg)
{
    if(0 != viArg)
    {
    	fsPreAlloc(vpstFd,viArg);
        return ERROR;       
    }
    
    if(OK != gstFsCtrl.stEntFunc.fsClearEntChain(vpstFd->pstVol,
                                                 vpstFd->uiFirstClu))
    {
        FS_LOG(FS_DBG_ERR,"file truncate clear fat failed\n",0,0,0,0,0,0);
        return OK;
    }
    vpstFd->ullFileSize.hi = 0;
    vpstFd->ullFileSize.lo = 0;
    vpstFd->uiFirstClu = FS_CLUST_INVALID;
        
    return fsUpdateFile(vpstFd);
}

/*****************************************************************************
 function     : fsIoctl
 description  : file system ioctl
*****************************************************************************/
STATUS fsIoctl(FS_FD_S *vpstFd,INT32 viFunc,INT32  viArg)
{
	FS_LOG(FS_DBG_DATA,"fd 0x%x func %d arg 0x%x enter\n",
           vpstFd,viFunc,viArg,0,0,0);
           
	switch(viFunc)
	{
		case FIOFSTATGET:
			return fsFillStat(vpstFd,viArg);
		case FIOREADDIR:
			return fsListDir(vpstFd,viArg);
		case FIORENAME:
			return fsRename(vpstFd,viArg);
        case FIOMOVE:
            return fsMove(vpstFd,viArg);
        case FIOLABELGET:
            return fsGetVolCap(vpstFd,viArg);
        case FIONFREE:
            return fsGetVolFreeCap(vpstFd,viArg);
        case FIODISKFORMAT:
            return fsFormatVol(vpstFd->pstVol);
        case FIOUNLINK:
        case FIORMDIR:
            return fsDelete(vpstFd->pstVol,vpstFd->aucFullName);
        case FIOFSTATFSGET:
			return fsFillFSStat(vpstFd,viArg);	
        case FIOSEEK:
            return fsSeek(vpstFd,viArg);
        case FIOSEEK64:
            return fsSeek64(vpstFd,viArg);
        case FIOATTRIBSET:
            return fsSetAttr(vpstFd,(FS_ATTR_S*)viArg);
        case FIOCONTIG:
        case FIOSYNC:
        case FIOTIMESET:
            return OK;
        case FIOTRUNC:
            return fsTrunc(vpstFd,viArg);
        case FIONREAD:
            return ERROR;
        case FIODISKINIT:
        case FIOCHMOD:
        case FIOUNMOUNT:
        case FIOMKDIR:
			break;
		case FIOATTRIBGET:
			return fsGetAttr(vpstFd,(FS_ATTR_S*)viArg);
		default:
		    break;		
	}
	
	FS_LOG(FS_DBG_ERR,"fd 0x%x func %d arg 0x%x unsupported\n",
           vpstFd,viFunc,viArg,0,0,0);
           
	return ERROR;
}

