/******************************************************************************

                  VERSION (C), 2001-2011, 

 ******************************************************************************
  file       : fsFDT.c
  version    : 1.0
  author     : lgh
  data       : 2012.9.4
  function   : file directory table management 
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
STATUS fsStrCmp(CHAR *vpcStr1,CHAR *vpcStr2,UINT32 vuiLen);

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
function     : fsFdtCacheInit
description  : read fdt information to cache
*****************************************************************************/
STATUS fsFdtCacheInit(FS_VOL_S  *vpstVol)
{
#ifdef FS_FDT_CACHE_ENABLE    
     UINT32       uiLen = 0;
     UINT32       uiLoop = 0;
     UINT32       uiClu = 0;
     CHAR        *pcBuf = NULL;
 
     uiLen = vpstVol->uiCluByte * vpstVol->uiFDTCluCnt;         
     if(NULL == vpstVol->pcFDTCache)
     {
         vpstVol->pcFDTCache = malloc(uiLen);
         if(NULL == vpstVol->pcFDTCache)
         {
             FS_LOG(FS_DBG_ERR,"malloc FDT cache data buffer %d failed\n",uiLen,
                    0,0,0,0,0);
             return ERROR;
         }
     }
 
     FS_LOG(FS_DBG_ERR,"FDT cache buffer 0x%x len 0x%x FDTCluCnt %d \n",
            vpstVol->pcFDTCache,uiLen,
            vpstVol->uiFDTCluCnt,0,0,0);
     bzero(vpstVol->pcFDTCache,uiLen);
     vpstVol->uiFDTCacheMinClu = vpstVol->uiRootClu;
     vpstVol->uiFDTCacheMaxClu = vpstVol->uiRootClu + vpstVol->uiFDTCluCnt;
     vpstVol->uiFDTCacheLen = uiLen;
     /*
      *read fdt to cache
     */
     pcBuf = vpstVol->pcFDTCache;
     for(uiLoop = 0;uiLoop < vpstVol->uiFDTCluCnt;uiLoop++)
     {
         uiClu = uiLoop + vpstVol->uiRootClu;
         if(OK != fsCluRW(vpstVol,uiClu,vpstVol->uiCluByte,0,pcBuf,FS_OPER_READ))
         {
             FS_LOG(FS_DBG_ERR,"read vol %d clu 0x%x failed\n",
                    vpstVol->uiVolId,uiClu,0,0,0,0);
             return ERROR;
         }
         pcBuf = pcBuf + vpstVol->uiCluByte;
     }    
     FS_LOG(FS_DBG_ERR,"read vol %p fdt cache %p min clu %d succeed\n",vpstVol,
            vpstVol->pcFDTCache,vpstVol->uiFDTCacheMinClu,0,0,0);           
#endif     
     return OK;    
}

/*****************************************************************************
function     : fsFdtInit
description  : file system fdt init
*****************************************************************************/
STATUS fsFdtInit(FS_VOL_S  *vpstVol)
{
	return fsFdtCacheInit(vpstVol);
}

/*****************************************************************************
function     : fsDataInCache
description  : data in cache or not
*****************************************************************************/
STATUS fsDataInFDTCache(FS_VOL_S *vpstVol,UINT32 vuiStartClu,UINT32 vuiCluCnt)
{
    UINT32  uiCluEnd = 0;

#ifdef FS_FDT_CACHE_ENABLE
    uiCluEnd = vuiStartClu + vuiCluCnt;
    if((vuiStartClu >= vpstVol->uiFDTCacheMinClu) && 
       (vpstVol->uiFDTCacheMaxClu >= uiCluEnd))
    {
        return OK;
    }
#endif
    return ERROR;
}

/*****************************************************************************
 function     : fsWriteFDT
 description  : write file diretory table
 *****************************************************************************/
 STATUS fsWriteFDT(FS_VOL_S *vpstVol,FS_FDT_S *vpstFDT,
                   UINT32 vuiClu,UINT32 vuiCluOffset)
 {
 	CHAR      *pcBuf = NULL;
    UINT32     uiOffset = 0;
    UINT32     uiSecAlignStart = 0;
    UINT32     uiSecAlignEnd = 0;
    UINT32     uiByte = 0;

    uiByte = sizeof(FS_FDT_S);
    if(OK == fsDataInFDTCache(vpstVol,vuiClu,1))
    {
		pcBuf = vpstVol->pcFDTCache;
        uiOffset = (vuiClu - vpstVol->uiFDTCacheMinClu) * vpstVol->uiCluByte;
        uiOffset = uiOffset + vuiCluOffset;
        assert((uiOffset + uiByte) <= vpstVol->uiFDTCacheLen);
        pcBuf = pcBuf + uiOffset;
        FS_LOG(FS_DBG_DATA,"fsWriteFDT clu %d len %d offset 0x%x name %s stat"
               " %d cache 0x%x\n",
             vuiClu,vpstFDT->ucNameLen,vuiCluOffset,vpstFDT->aucName,vpstFDT->ucFileStat,pcBuf);
             
        bcopy((CHAR*)vpstFDT,pcBuf,uiByte);
        uiSecAlignStart = uiOffset & (~0x1ff);
        uiSecAlignEnd =  (uiOffset + uiByte - 1) & (~0x1ff);
        if(uiSecAlignStart != uiSecAlignEnd)
        {
            FS_LOG(FS_DBG_DATA,"offset %d byte %d start %d end %d\n",uiOffset,
                   uiByte,uiSecAlignStart,uiSecAlignEnd,0,0);    
        }
        assert(uiSecAlignStart == uiSecAlignEnd);
        pcBuf = vpstVol->pcFDTCache + uiSecAlignStart;
        uiOffset = uiOffset & (~0x1ff);
        uiByte = 512; /*for performace 512byte align*/
    }
#ifdef FS_TRIM_SUPPORT
	if(FS_FILE_STAT_EMPTY == vpstFDT->ucFileStat)
	{
		if(OK != fsCluRW(vpstVol,vuiClu,uiByte,vuiCluOffset,NULL,FS_OPER_TRIM))
		{
		    FS_LOG(FS_DBG_ERR,"set cluster trim failed\n",0,0,0,0,0,0);
            return ERROR;
		}
		return OK;
	} 
#endif

	return fsCluRW(vpstVol,vuiClu,uiByte,vuiCluOffset,vpstFDT,FS_OPER_WRITE);
 }
 
 /*****************************************************************************
 function     : fsReadFDT
 description  : read file diretory table
 input        : 
 output       : 
 return       : 
 *****************************************************************************/
STATUS fsReadFDT(FS_VOL_S *vpstVol,FS_FDT_S *vpstFDT,
                 UINT32 vuiClu,UINT32 vuiCluOffset)
{
	CHAR      *pcBuf = NULL;
    UINT32     uiOffset = 0;
    UINT32     uiSecAlignStart = 0;
    UINT32     uiSecAlignEnd = 0;
    UINT32     uiByte = 0;
    
    uiByte = sizeof(FS_FDT_S);
    if(OK == fsDataInFDTCache(vpstVol,vuiClu,1))
    {
    	pcBuf = vpstVol->pcFDTCache;
        uiOffset = (vuiClu - vpstVol->uiFDTCacheMinClu) * vpstVol->uiCluByte;
        uiOffset = uiOffset + vuiCluOffset;
        assert((uiOffset + uiByte) <= vpstVol->uiFDTCacheLen);
        pcBuf = pcBuf + uiOffset;
        bcopy(pcBuf,(CHAR*)vpstFDT,uiByte);

        return OK;
    }
    
    return fsCluRW(vpstVol,vuiClu,uiByte,vuiCluOffset,vpstFDT,FS_OPER_READ);
}
 
 /*****************************************************************************
 function     : fsGetFreeFDT
 description  : get free file diretory table entry
 input        : vpcName
 output       : vpuiFDTNum
 return       : OK or ERROR
 *****************************************************************************/
 STATUS fsGetFreeFDT(FS_VOL_S *vpstVol,CHAR *vpcName,FS_FDT_INFO_S *vpstFDTInfo)
 {
     FS_FDT_S    stFDT;
     UINT32      uiLoop = 0;
     UINT32      uiHashVal = 0;
     UINT32      uiHashKey = 0;
     UINT32      uiClu = 0;
     UINT32      uiCluOffset = 0;
     UINT32      uiCacheOffset = 0;

	 for(uiHashKey = 0;uiHashKey < FS_MAX_HASH_KEY;uiHashKey++)
	 {
		uiHashVal = fsHashString(vpcName,uiHashKey) % vpstVol->uiFDTCnt;
		uiClu = (uiHashVal * sizeof(FS_FDT_S)) / vpstVol->uiCluByte;
		uiClu = uiClu + vpstVol->uiFDTCacheMinClu;
		uiCluOffset = (uiHashVal * sizeof(FS_FDT_S)) % vpstVol->uiCluByte;
		assert(uiClu <= vpstVol->uiFDTCacheMaxClu);
		memset(&stFDT,0,sizeof(FS_FDT_S));
		if(OK != fsReadFDT(vpstVol,&stFDT,uiClu,uiCluOffset))
		{
			FS_LOG(FS_DBG_ERR,"read vol %d fdt failed\n",vpstVol->uiVolId,
			       0,0,0,0,0);
			return ERROR;
		}
		if(FS_FILE_STAT_EMPTY == stFDT.ucFileStat)
		{
			vpstFDTInfo->uiClu = uiClu;
     		vpstFDTInfo->uiCluOffset = uiCluOffset;
     		vpstFDTInfo->stFDT.uiHash = uiHashVal;
     		vpstFDTInfo->stFDT.uiHashKey = uiHashKey;
     		uiCacheOffset = (uiClu - vpstVol->uiFDTCacheMinClu) * vpstVol->uiCluByte;
     		uiCacheOffset = uiCacheOffset + uiCluOffset;
     		FS_LOG(FS_DBG_DATA,"get empty fdt - name %s hash %d key %d cache"
     		       " offset 0x%x len %d\n",vpcName,
			       uiHashVal,uiHashKey,uiCacheOffset,vpstFDTInfo->stFDT.ucNameLen,0);
     		return OK;
		}
	 }
	 
     return ERROR;
 }
 
 /*****************************************************************************
 function     : fsGetModWrFDT
 description  : get free fdt;modify it ;write it to disk
 input        : vpstVol;vuiFirstClu;vpstFDTInfo->stFDT
 output       : vpstFDTInfo->uiClu;vpstFDTInfo->uiCluOffset
 return       : OK or ERROR
 *****************************************************************************/
 STATUS fsGetModWrFDT(FS_VOL_S *vpstVol,CHAR *vpcFileName,
                      FS_FDT_INFO_S *vpstFDTInfo)
 {
     UINT8      *pucBuf = NULL;
     UINT32      uiCluLen = 0;
     UINT32      uiClu = 0;
     UINT32      uiNextClu = 0;
     UINT32      uiCluOffset = 0;
     BOOL        bFound = FALSE;
     STATUS      iRet = ERROR;
     UINT32      uiFDTNum = 0;
     
     uiCluLen = vpstVol->uiCluByte;
     /*
	  *  get free fdt
	 */
	 if(OK != fsGetFreeFDT(vpstVol,vpcFileName,vpstFDTInfo))
	 {
		return ERROR;
	 }
	 
     /*
      *modify clust content and update it
     */
     uiCluOffset = vpstFDTInfo->uiCluOffset;
     uiClu = vpstFDTInfo->uiClu;
     if(OK != fsWriteFDT(vpstVol,&vpstFDTInfo->stFDT,uiClu,uiCluOffset))
     {
     	return ERROR;
     }
     
     return OK;
 }

 /*****************************************************************************
 function     : fsFindFDT
 description  : find file directory table
 input        : vpcName;vpstVol
 output       : vpstFDTInfo
 return       : 
 *****************************************************************************/
 STATUS fsFindFDT(FS_VOL_S *vpstVol,CHAR *vpcName,FS_FDT_INFO_S *vpstFDTInfo)
 {
     FS_FDT_S   *pstFDT = NULL;
     CHAR        astRoot[2];
     CHAR       *pcName = NULL;
     UINT32      uiLoop = 0;
     BOOL        bFound = FALSE;
     UINT32      uiHashVal = 0;
     UINT32      uiHashKey = 0;
     UINT32      uiClu = 0;
     UINT32      uiCluOffset = 0;
     UINT32      uiStrLen = 0;

	 pcName = vpcName;
	 if(strlen(vpcName) < 2)
	 {
	 	astRoot[0] = '/';
	 	astRoot[1] = 0;
		pcName = &astRoot[0];
	 }
	 pstFDT = &vpstFDTInfo->stFDT;
	 for(uiHashKey = 0;uiHashKey < FS_MAX_HASH_KEY;uiHashKey++)
	 {
		uiHashVal = fsHashString(pcName,uiHashKey) % vpstVol->uiFDTCnt;
		uiClu = (uiHashVal * sizeof(FS_FDT_S)) / vpstVol->uiCluByte;
		uiClu = uiClu + vpstVol->uiFDTCacheMinClu;
		uiCluOffset = (uiHashVal * sizeof(FS_FDT_S)) % vpstVol->uiCluByte;
		memset(pstFDT,0,sizeof(FS_FDT_S));
		if(OK != fsReadFDT(vpstVol,pstFDT,uiClu,uiCluOffset))
		{
			FS_LOG(FS_DBG_DATA,"find vol %d fdt failed\n",vpstVol->uiVolId,
			       0,0,0,0,0);
			return ERROR;
		}
		uiStrLen = MIN(strlen(pcName) + 1,FS_MAX_NAME_LEN);
		FS_LOG(FS_DBG_DATA,"find name %s hash %d key %d fdt name %s clu %d "
		       "offset 0x%x\n",pcName,
			    uiHashVal,uiHashKey,pstFDT->aucName,uiClu,uiCluOffset);

		if((FS_FILE_STAT_EMPTY != pstFDT->ucFileStat) &&
		   (0 == fsStrCmp(pcName,pstFDT->aucName,uiStrLen)))
		{
			bFound = TRUE;
			vpstFDTInfo->uiClu = uiClu;
     		vpstFDTInfo->uiCluOffset = uiCluOffset;

     		return OK;
		}
	 }

	 FS_LOG(FS_DBG_DATA,"find vol %d name %s fdt failed\n",vpstVol->uiVolId,
			      pcName,0,0,0,0);
     return ERROR;
 }

 /*****************************************************************************
 function     : fsPrintFDT
 description  : find file directory table by name from the buffer
 input        : 
 output       : 
 return       : 
 *****************************************************************************/
 VOID fsPrintFDT(UINT8 *vpucBuf,UINT32 vuiBufLen)
 {
     FS_FDT_S   *pstFDT = NULL;
     UINT32      uiFDTCnt = 0;
     UINT32      uiLoop = 0;
     UINT32      uiTime = 0;
     
     pstFDT = (FS_FDT_S*)vpucBuf;
     uiFDTCnt = vuiBufLen/sizeof(FS_FDT_S);
     for(uiLoop = 0;uiLoop < uiFDTCnt;uiLoop++)
     {
         if(FS_FILE_STAT_SUBDIR == pstFDT->ucFileStat)
         {
             uiTime = (pstFDT->ucUpdateTime[1]  << 24) | 
                      (pstFDT->ucUpdateTime[0]  << 16) |
                      (pstFDT->ucUpdateDate[1]  << 8) |
                      (pstFDT->ucUpdateDate[0]  << 0);
			 #if 0
             printf(" %-32s size 0x%x%.8x attr 0x%2x firstclu 0x%x namelen %d "
                    "time 0x%x",
                    pstFDT->aucName,
                    pstFDT->ullFileSize.hi,
                    pstFDT->ullFileSize.lo,
                    pstFDT->ucAttr,
                    pstFDT->uiFirstClu,
                    pstFDT->ucNameLen,uiTime);
             printf(" start 0x%x end 0x%x hash %d key %d parent %d\n",pstFDT->uiStartFrame,
                    pstFDT->uiEndFrame,pstFDT->uiHash,pstFDT->uiHashKey,
                    pstFDT->uiPartentHash);
			#else
			 printf(" %-32s size 0x%x%.8x\n",pstFDT->aucName,
					 pstFDT->ullFileSize.hi,pstFDT->ullFileSize.lo);

			 #endif
         }
         pstFDT++;
     }
 }
 
 /*****************************************************************************
 function     : fsReadDirFDT
 description  : read directory information
 input        : 
 output       : 
 return       : 
 *****************************************************************************/
 UINT32 fsReadDirFDT(FS_FD_S *vpstFd,CHAR *vpcBuf,UINT32 vuiByte)
 {
	CHAR               *pcDestBuf = NULL;
	CHAR               *pcSrcBuf = NULL;
	UINT32              uiRdLen = 0;
	FS_VOL_S           *pstVol = NULL;
	FS_FDT_S           *pstFDT = NULL;
    UINT32              uiClu = 0;
    UINT32              uiFDTCnt = 0;
    UINT32              uiLoop = 0;
    UINT32              uiByte = 0;
    UINT32              uiOffset = 0;
	UINT32              uiCluOffset = 0;
    UINT32              uiPartentHash = 0;

	if(FS_ATTR_DIRECTORY != vpstFd->uiAttr)
	{
		return 0;
	}
	pcDestBuf = vpcBuf;
	pstVol = vpstFd->pstVol;
	uiPartentHash = vpstFd->uiHashVal;
    uiByte = sizeof(FS_FDT_S);
    uiFDTCnt = pstVol->uiCluByte / sizeof(FS_FDT_S);
    if(FS_CLUST_INVALID == vpstFd->uiCurClu)
    {
		vpstFd->uiCurClu = pstVol->uiFDTCacheMinClu;
		vpstFd->uiCluOffset = 0;
    }
    if((vpstFd->uiCurClu < pstVol->uiFDTCacheMinClu) || 
       (vpstFd->uiCurClu > pstVol->uiFDTCacheMaxClu) ||
       (0 != (vpstFd->uiCluOffset % uiByte)))
    {
		FS_LOG(FS_DBG_ERR,"find vol %d dir clu error\n",pstVol->uiVolId,
			   0,0,0,0,0);
		return 0;	   
    }
	uiCluOffset = vpstFd->uiCluOffset;
    for(uiClu = vpstFd->uiCurClu;uiClu < pstVol->uiFDTCacheMaxClu;uiClu++)
    {
		if(OK == fsDataInFDTCache(pstVol,uiClu,1))
        {
            uiOffset = (uiClu - pstVol->uiFDTCacheMinClu) * pstVol->uiCluByte;
            pstFDT = (FS_FDT_S*)(pstVol->pcFDTCache + uiOffset + uiCluOffset);
            uiLoop = uiCluOffset / uiByte;
			uiCluOffset = 0;
            for(;uiLoop < uiFDTCnt;uiLoop++)
            {
            	if((FS_FILE_STAT_EMPTY != pstFDT->ucFileStat) &&
            	   (uiPartentHash == pstFDT->uiPartentHash))
            	{
            		FS_LOG(FS_DBG_DATA,"fdt 0x%x not null file name %s \n",
								pstFDT,pstFDT->aucName,uiPartentHash,pstFDT->uiPartentHash,0,0);
					bcopy(pstFDT,(CHAR*)pcDestBuf,uiByte);
					pcDestBuf = pcDestBuf + uiByte;
					uiRdLen = uiRdLen + uiByte;
					if((uiRdLen + uiByte) > vuiByte)
					{
						vpstFd->uiCluOffset = uiLoop * uiByte;
						vpstFd->uiCurClu = uiClu;
						if(pstVol->uiCluByte == vpstFd->uiCluOffset)
						{
							vpstFd->uiCurClu = uiClu + 1;
							vpstFd->uiCluOffset = 0;	
						}
						return uiRdLen;
					}
            	}
				pstFDT++;
            }
        }
        else
        {
			pcSrcBuf = malloc(pstVol->uiCluByte);
            if(NULL == pcSrcBuf)
            {
            	FS_LOG(FS_DBG_ERR,"malloc memory failed\n",0,0,0,0,0,0);
            	return ERROR;
            }
            memset(pcSrcBuf,0,pstVol->uiCluByte);
            if(OK != fsCluRW(pstVol,uiClu,pstVol->uiCluByte,0,pcSrcBuf,FS_OPER_READ))
            {
				return ERROR;
            }
            pstFDT = (FS_FDT_S*)(pcSrcBuf + uiCluOffset);
			uiLoop = uiCluOffset / uiByte;
			uiCluOffset = 0;
            for(;uiLoop < uiFDTCnt;uiLoop++)
            {
            
				FS_LOG(FS_DBG_ERR,"////fdt 0x%x not null file name %s \n",
						pstFDT,pstFDT->aucName,uiPartentHash,pstFDT->uiPartentHash,0,0);
            	if((FS_FILE_STAT_EMPTY != pstFDT->ucFileStat) &&
            	   (uiPartentHash == pstFDT->uiPartentHash))
            	{
					bcopy(pstFDT,(CHAR*)pcDestBuf,uiByte);					
					pcDestBuf = pcDestBuf + uiByte;
					uiRdLen = uiRdLen + uiByte;
					if((uiRdLen + uiByte) > vuiByte)
					{
						vpstFd->uiCluOffset = uiLoop * uiByte;
						vpstFd->uiCurClu = uiClu;
						if(pstVol->uiCluByte == vpstFd->uiCluOffset)
						{
							vpstFd->uiCurClu = uiClu + 1;
							vpstFd->uiCluOffset = 0;	
						}
						return uiRdLen;
					}
            	}
				pstFDT++;
            }
        }
        vpstFd->uiCurClu++;
    }
    vpstFd->uiCluOffset = 0;
	
    return uiRdLen;
}

/*****************************************************************************
 function     : fsGetDirFDTCnt
 description  : get directory fdt count
 input        : vpstVol ;vuiHash
 output       : fdt count
 return       : 
 *****************************************************************************/
 UINT32 fsGetDirFDTCnt(FS_VOL_S *vpstVol,UINT32 vuiHash)
 {
	CHAR               *pcSrcBuf = NULL;
	UINT32              uiRdLen = 0;
	FS_FDT_S           *pstFDTtmp = NULL;
    UINT32              uiClu = 0;
    UINT32              uiFDTPerClu = 0;
    UINT32              uiFDTFind = 0;
    UINT32              uiLoop = 0;
    UINT32              uiByte = 0;
    UINT32              uiOffset = 0;
    UINT32              uiPartentHash = 0;

	
    uiByte = sizeof(FS_FDT_S);
    uiFDTPerClu = vpstVol->uiCluByte / sizeof(FS_FDT_S);
    pstFDTtmp = (FS_FDT_S*)vpstVol->pcFDTCache;
    uiPartentHash = vuiHash;
    for(uiClu = vpstVol->uiFDTCacheMinClu;uiClu < vpstVol->uiFDTCacheMaxClu;uiClu++)
    {
    	uiOffset = (uiClu - vpstVol->uiFDTCacheMinClu) * vpstVol->uiCluByte;
        pstFDTtmp = (FS_FDT_S*)(vpstVol->pcFDTCache + uiOffset);
		if(OK == fsDataInFDTCache(vpstVol,uiClu,1))
        {
            for(uiLoop = 0;uiLoop < uiFDTPerClu;uiLoop++)
            {
            	if((FS_FILE_STAT_EMPTY != pstFDTtmp->ucFileStat) &&
            	   (uiPartentHash == pstFDTtmp->uiPartentHash))
            	{
					uiFDTFind++;
            	}
				pstFDTtmp++;
            }
        }
        else
        {
			pcSrcBuf = malloc(vpstVol->uiCluByte);
            if(NULL == pcSrcBuf)
            {
            	FS_LOG(FS_DBG_ERR,"malloc memory failed\n",0,0,0,0,0,0);
            	return ERROR;
            }
            memset(pcSrcBuf,0,vpstVol->uiCluByte);
            if(OK != fsCluRW(vpstVol,uiClu,vpstVol->uiCluByte,0,pcSrcBuf,FS_OPER_READ))
            {
				return ERROR;
            }
            pstFDTtmp = (FS_FDT_S*)(pcSrcBuf);
            for(uiLoop = 0;uiLoop < uiFDTPerClu;uiLoop++)
            {                             
            	if((FS_FILE_STAT_EMPTY != pstFDTtmp->ucFileStat) &&
            	   (uiPartentHash == pstFDTtmp->uiPartentHash))
            	{
					uiFDTFind++;
            	}
				pstFDTtmp++;
            }
        }
    }
    
    return uiFDTFind;
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
    CHAR     *pucBuf = NULL;
    UINT32    uiRDLen = 0;

    pstVol = fsFindVolByName(vpcVolName);/*fsFindVolByName("/vol")*/
    if(NULL == pstVol)
    {
        FS_LOG(FS_DBG_ERR,"vol %s not exist\n",vpcVolName,0,0,0,0,0);
        return ERROR;
    }
    FS_LOG(FS_DBG_ERR,"found vol %p\n",pstVol,0,0,0,0,0);
    
    pstFd = (FS_FD_S*)fsOpen(pstVol,vpcDirName,0,0);
    if(ERROR == pstFd)
    {
        return ERROR;
    }
    pucBuf = malloc(pstVol->uiCluByte);
    if(NULL == pucBuf)
    {
		FS_LOG(FS_DBG_ERR,"fsPrintDir malloc buffer failed\n",0,0,0,0,0,0);
		fsClose(pstFd);
		return ERROR;
    }
    memset(pucBuf,0,pstVol->uiCluByte);
    do
    {
		uiRDLen = fsReadDirFDT(pstFd,pucBuf,pstVol->uiCluByte);
		if(uiRDLen <= pstVol->uiCluByte)
		{
			fsPrintFDT(pucBuf,uiRDLen);
		}
    }while(pstVol->uiCluByte == uiRDLen);
    
    fsClose(pstFd);

    return OK;
}

 
