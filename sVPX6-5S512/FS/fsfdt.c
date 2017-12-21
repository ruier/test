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
 function     : fsGetFreeCluFDT
 description  : get free file diretory table from the appointed cluster
 input        : vpucBuf,vuiBufLen
 output       : vpuiOffset
 return       : OK or ERROR
 *****************************************************************************/
 STATUS fsBufGetFreeFDT(UINT8 *vpucBuf,UINT32 vuiBufLen,UINT32 *vpuiOffset)
 {
     FS_FDT_S   *pstFDT = NULL;
     UINT32      uiFDTCnt = 0;
     UINT32      uiLoop = 0;
     BOOL        bFound = FALSE;

     /*
      *get free FDT from buffer
     */
     pstFDT = (FS_FDT_S*)vpucBuf;
     uiFDTCnt = vuiBufLen/sizeof(FS_FDT_S);
     for(uiLoop = 0;uiLoop < uiFDTCnt;uiLoop++)
     {
         if(FS_FILE_STAT_EMPTY == pstFDT->ucFileStat)
         {
             *vpuiOffset = (uiLoop * sizeof(FS_FDT_S));             
             bFound = TRUE;
             break;
         }
         pstFDT++;
     }
 
     if(TRUE == bFound)
     {
         return OK;
     }
    
     return ERROR;
 }
 
 /*****************************************************************************
 function     : fsWriteFDT
 description  : write file diretory table
 *****************************************************************************/
 STATUS fsWriteFDT(FS_VOL_S *vpstVol,FS_FDT_S *vpstFDT,
                   UINT32 vuiClu,UINT32 vuiCluOffset)
 { 
 	 FS_LOG(FS_DBG_DATA,"write clu %d offset %d name %s size %d\n",
                vuiClu,vuiCluOffset,vpstFDT->aucName,vpstFDT->ullFileSize.lo,0,0);
     return gstFsCtrl.stCacheFunc.fsCacheByteWrite(vpstVol,vuiClu,vuiCluOffset,
                                              (UINT8*)vpstFDT,sizeof(FS_FDT_S));
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
 	  FS_LOG(FS_DBG_DATA,"read clu %d offset %d name %s size %d\n",
                vuiClu,vuiCluOffset,vpstFDT->aucName,vpstFDT->ullFileSize.lo,0,0);
		
      return gstFsCtrl.stCacheFunc.fsCacheByteRead(vpstVol,vuiClu,vuiCluOffset,
                                           (UINT8*)(vpstFDT),sizeof(FS_FDT_S));
 }
 
 /*****************************************************************************
 function     : fsNewCluModWrFDT
 description  : allocate new clust;modify the fdt;write it to disk
 input        : vpstVol;vuiFirstClu;vpstFDTInfo->stFDT
 output       : vpstFDTInfo->uiClu;vpstFDTInfo->uiCluOffset
 return       : OK or ERROR
 *****************************************************************************/
 STATUS fsNewCluModWrFDT(FS_VOL_S *vpstVol,UINT8  *vpucBuf,UINT32 vuiBufLen,
                         UINT32 vuiLastClu,FS_FDT_INFO_S *vpstFDTInfo)
 {
     UINT32  uiNewClu = 0;
     
     if(OK != gstFsCtrl.stEntFunc.fsGetFreeEnt(vpstVol,&uiNewClu))
     {
         FS_LOG(FS_DBG_INFO,"dev %d get free clu failed\n",
                vpstVol->uiVolId,0,0,0,0,0);
         return ERROR;
     }
     
     /*
      *zero this clust data information
     */
     bzero((CHAR*)vpucBuf,vuiBufLen);
 
     /*
      *update cluster content
     */
     bcopy((CHAR*)&vpstFDTInfo->stFDT,(CHAR*)&vpucBuf[0],sizeof(FS_FDT_S));
     if(OK != gstFsCtrl.stCacheFunc.fsCacheCluWrite(vpstVol,uiNewClu,vpucBuf))
     {
         FS_LOG(FS_DBG_INFO,"write cache sector failed\n",0,0,0,0,0,0);
         return ERROR;
     }
     vpstFDTInfo->uiClu = uiNewClu;
     vpstFDTInfo->uiCluOffset = 0;
 
     /*
      *update fat table
     */
     if(FS_CLUST_INVALID != vuiLastClu)
     {
         if(OK != gstFsCtrl.stEntFunc.fsWriteEnt(vpstVol,vuiLastClu,uiNewClu,0))
         {
             FS_LOG(FS_DBG_INFO,"write fat clu 0x%x failed\n",vuiLastClu,0,0,0,0,0);
             return ERROR;
         }
     }
 
     if(OK != gstFsCtrl.stEntFunc.fsWriteEnt(vpstVol,uiNewClu,FS_CLUST_EOF,0))
     {
         FS_LOG(FS_DBG_INFO,"write fat clu 0x%x failed\n",vuiLastClu,0,0,0,0,0);
         return ERROR;
     }
 
     return OK;
 }

 /*****************************************************************************
 function     : fsGetModWrFDT
 description  : get free fdt;modify it ;write it to disk
 input        : vpstVol;vuiFirstClu;vpstFDTInfo->stFDT
 output       : vpstFDTInfo->uiClu;vpstFDTInfo->uiCluOffset
 return       : OK or ERROR
 *****************************************************************************/
 STATUS fsGetModWrFDT(FS_VOL_S *vpstVol,UINT32 vuiFirstClu,
                      FS_FDT_INFO_S *vpstFDTInfo)
 {
     UINT8      *pucBuf = NULL;
     UINT32      uiCluLen = 0;
     UINT32      uiCurClu = 0;
     UINT32      uiNextClu = 0;
     UINT32      uiCluOffset = 0;
     BOOL        bFound = FALSE;
     STATUS      iRet = ERROR;
     
     uiCluLen = vpstVol->uiCluByte;
     pucBuf = malloc(uiCluLen);
     if(NULL == pucBuf)
     {
         FS_LOG(FS_DBG_ERR,"malloc memory %d failed\n",uiCluLen,0,0,0,0,0);
         return ERROR;
     }
 
     uiNextClu = vuiFirstClu;
     uiCurClu = vuiFirstClu;
     
     /*
	*  get free fdt
	*/
	
     while((FS_CLUST_EOF != uiNextClu) &&  (FS_CLUST_INVALID != uiNextClu))
     {
         uiCurClu = uiNextClu;
         bzero((CHAR*)pucBuf,uiCluLen);
         
         if(OK != gstFsCtrl.stCacheFunc.fsCacheCluRead(vpstVol,uiCurClu,pucBuf))
         {
             FS_LOG(FS_DBG_INFO,"read cache sector failed %d \n",__LINE__,0,0,0,0,0);
             free(pucBuf);
             return ERROR;
         }

         if(OK == fsBufGetFreeFDT(pucBuf,uiCluLen,&uiCluOffset))
         {
             vpstFDTInfo->uiClu = uiCurClu;
             vpstFDTInfo->uiCluOffset = uiCluOffset;
             bFound = TRUE;
             break;
         }
         
         if(FS_CLUST_EOF == uiCurClu)
         {
            FS_LOG(FS_DBG_ERR,"fsGetModWrFDT curr free clu invalid\n",
                   0,0,0,0,0,0);
         }
        
         if(OK != gstFsCtrl.stEntFunc.fsReadEnt(vpstVol,uiCurClu,&uiNextClu))
         {
             FS_LOG(FS_DBG_INFO,"read fat clu 0x%x failed\n",uiCurClu,0,0,0,0,0);
             free(pucBuf);
             return ERROR;
         }       
     }
     
     /*
      *get free fdt failed;allocat new cluster for fdt
     */
     if(FALSE == bFound)
     {
         iRet = fsNewCluModWrFDT(vpstVol,pucBuf,uiCluLen,uiCurClu,vpstFDTInfo);
         free(pucBuf);
         return iRet;
     }
    
     /*
      *modify clust content and update it
     */
     uiCluOffset = vpstFDTInfo->uiCluOffset;
     bcopy((CHAR*)&vpstFDTInfo->stFDT,(CHAR*)&pucBuf[uiCluOffset],sizeof(FS_FDT_S));
 
     if(OK != gstFsCtrl.stCacheFunc.fsCacheCluWrite(vpstVol,uiCurClu,pucBuf))
     {
         FS_LOG(FS_DBG_INFO,"write cache sector failed\n",0,0,0,0,0,0);
         free(pucBuf);
         return ERROR;
     }

     free(pucBuf);
     return OK;
 }

 /*****************************************************************************
 function     : fsFindFDTFromBuf
 description  : find file directory table by name from the buffer
 input        : 
 output       : 
 return       : 
 *****************************************************************************/
 STATUS fsFindBufFDT(FS_VOL_S *vpstVol,CHAR *vpcName,UINT8 *vpucBuf,
                         UINT32 vuiBufLen,UINT32 *vuiOffset)
 {
     FS_FDT_S   *pstFDT = NULL;
     UINT32      uiFDTCnt = 0;
     UINT32      uiLoop = 0;
     UINT32      uiStrLen = 0;
     BOOL        bFound = FALSE;
 
     /*
      *find FDT by name
     */
     pstFDT = (FS_FDT_S*)vpucBuf;
     uiFDTCnt = vuiBufLen/sizeof(FS_FDT_S);
     for(uiLoop = 0;uiLoop < uiFDTCnt;uiLoop++)
     {
         uiStrLen = MIN(strlen(vpcName) + 1,FS_MAX_NAME_LEN);
         if(OK == fsStrCmp(vpcName,pstFDT->aucName,uiStrLen))
         {
             bFound = TRUE;
             *vuiOffset = (uiLoop * sizeof(FS_FDT_S));
             break;
         }
         pstFDT++;
     }
 
     if(TRUE == bFound)
     {
         return OK;
     }
 
     return ERROR;
 }

 /*****************************************************************************
 function     : fsFindFDT
 description  : find file directory table from the first cluster to end of file
 input        : 
 output       : 
 return       : 
 *****************************************************************************/
 STATUS fsFindFDT(FS_VOL_S *vpstVol,CHAR *vpcName,UINT32 vuiFirstClu,
                  FS_FDT_INFO_S *vpstFDTInfo)
 {
     UINT32    uiCurClu = 0;
     UINT32    uiCluLen = 0;
     UINT32    uiNextClu = 0;
     UINT32    uiCluOffset = 0;
     BOOL      bFound = FALSE;
     UINT8    *pucBuf = NULL;
 
     if(FS_CLUST_INVALID == vuiFirstClu)
     {
         return ERROR;
     }
     
     uiCluLen = vpstVol->uiCluByte;
     pucBuf = malloc(uiCluLen);
     if(NULL == pucBuf)
     {
         FS_LOG(FS_DBG_ERR,"malloc memory %d failed\n",uiCluLen,0,0,0,0,0);
         return ERROR;
     }
     
     bzero((CHAR*)pucBuf,uiCluLen);
     bzero((CHAR*)vpstFDTInfo,sizeof(FS_FDT_INFO_S));
     
     uiNextClu = vuiFirstClu;
     do
     {
         uiCurClu = uiNextClu;
         if(OK != gstFsCtrl.stCacheFunc.fsCacheCluRead(vpstVol,uiCurClu,pucBuf))
         {
             FS_LOG(FS_DBG_INFO,"read cache sector failed %d\n",__LINE__,0,0,0,0,0);
             free(pucBuf);
             return ERROR;
         }
         if(OK == fsFindBufFDT(vpstVol,vpcName,pucBuf,uiCluLen,&uiCluOffset))
         {
             bcopy((CHAR*)(&pucBuf[uiCluOffset]),(CHAR*)(&vpstFDTInfo->stFDT),
                   sizeof(FS_FDT_S));
             vpstFDTInfo->uiClu = uiCurClu;
             vpstFDTInfo->uiCluOffset = uiCluOffset;
             bFound = TRUE;
             break;
         }
         
         if(FS_CLUST_EOF == uiCurClu)
         {
            FS_LOG(FS_DBG_ERR,"fsFindFDT curr free clu invalid\n",
                   0,0,0,0,0,0);
         }
         
         if(OK != gstFsCtrl.stEntFunc.fsReadEnt(vpstVol,uiCurClu,&uiNextClu))
         {
             free(pucBuf);
             return ERROR;
         }
     }while(FS_CLUST_EOF != uiNextClu);
 
     if(TRUE == bFound)
     { 
         free(pucBuf);
         return OK;
     }
     
     free(pucBuf);
     return ERROR;
 }
 
 /*****************************************************************************
 function     : fsPrintBufFDT
 description  : find file directory table by name from the buffer
 input        : 
 output       : 
 return       : 
 *****************************************************************************/
 VOID fsPrintBufFDT(FS_VOL_S *vpstVol,UINT8 *vpucBuf,UINT32 vuiBufLen)
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
             printf(" %-32s size 0x%x%.8x attr 0x%2x firstclu 0x%x namelen %d "
                    "time 0x%x",
                    pstFDT->aucName,
                    pstFDT->ullFileSize.hi,
                    pstFDT->ullFileSize.lo,
                    pstFDT->ucAttr,
                    pstFDT->uiFirstClu,
                    pstFDT->ucNameLen,uiTime);
             #ifdef FS_EXTENTED_FDT
             printf(" start 0x%x end 0x%x\n",pstFDT->uiStartFrame,
                    pstFDT->uiEndFrame);
             #endif
         }
         pstFDT++;
     }
 }
 
 /*****************************************************************************
 function     : fsListFDT
 description  : list all file of current directory
 input        : 
 output       : 
 return       : 
 *****************************************************************************/
 STATUS fsListFDT(FS_VOL_S *vpstVol,UINT32 vuiFirstClu)
 {
     UINT8    *pucBuf = NULL;
     UINT32    uiCurClu = 0;
     UINT32    uiNextClu = 0;
     UINT32    uiCluLen = 0;
 
     if(FS_CLUST_INVALID == vuiFirstClu)
     {
         return OK;
     }
     
     uiCluLen = vpstVol->uiCluByte;
     pucBuf = malloc(uiCluLen);
     if(NULL == pucBuf)
     {
         FS_LOG(FS_DBG_ERR,"malloc memory %d failed\n",uiCluLen,0,0,0,0,0);
         return ERROR;
     }
     bzero((CHAR*)pucBuf,uiCluLen);
 
     uiNextClu = vuiFirstClu;
     /*
      *print all file name of current directory
     */
     do
     {
         uiCurClu = uiNextClu;
         if(OK != gstFsCtrl.stCacheFunc.fsCacheCluRead(vpstVol,uiCurClu,pucBuf))
         {
             FS_LOG(FS_DBG_INFO,"read cache sector failed %d\n",__LINE__,0,0,0,0,0);
             free(pucBuf);
             return ERROR;
         }
         
         /*
          *print file name of current cluster
         */
         fsPrintBufFDT(vpstVol,pucBuf,uiCluLen);
         
         if(OK != gstFsCtrl.stEntFunc.fsReadEnt(vpstVol,uiCurClu,&uiNextClu))
         {
             free(pucBuf);
             return ERROR;
         }
         if(FS_CLUST_AVAIL == uiNextClu)
         {
            FS_LOG(FS_DBG_INFO,"read entry %d error\n",uiCurClu,0,0,0,0,0);
            free(pucBuf);
            return ERROR;
         }
     }while(FS_CLUST_EOF != uiNextClu);
 
     free(pucBuf);
     return OK;
 }
 
 /*****************************************************************************
 function     : fsGetFDTCnt
 description  : get fdt count of cluster
 input        : 
 output       : 
 return       : 
 *****************************************************************************/
 STATUS fsGetFDTCnt(FS_VOL_S *vpstVol,UINT32 vuiCurClu,UINT32 *vpuiFDTCnt)
 {
     FS_FDT_S   *pstFDT = NULL;
     UINT8      *pucBuf = NULL;
     UINT32      uiFDTCnt = 0;
     UINT32      uiFDTUse = 0;
     UINT32      uiCluLen = 0;
     UINT32      uiLoop = 0;
 
     if((FS_CLUST_INVALID == vuiCurClu) || (FS_CLUST_EOF == vuiCurClu))
     {
         *vpuiFDTCnt = 0;
         return OK;
     }
     
     uiCluLen = vpstVol->uiCluByte;
     pucBuf = malloc(uiCluLen);
     if(NULL == pucBuf)
     {
         FS_LOG(FS_DBG_ERR,"malloc memory %d failed\n",uiCluLen,0,0,0,0,0);
         return ERROR;
     }
     bzero((CHAR*)pucBuf,uiCluLen);
 
     if(OK != gstFsCtrl.stCacheFunc.fsCacheCluRead(vpstVol,vuiCurClu,pucBuf))
     {
         FS_LOG(FS_DBG_INFO,"read cache sector failed %d\n",__LINE__,0,0,0,0,0);
         free(pucBuf);
         return ERROR;
     }
     
     pstFDT = (FS_FDT_S*)pucBuf;
     uiFDTCnt = uiCluLen/sizeof(FS_FDT_S);
     for(uiLoop = 0;uiLoop < uiFDTCnt;uiLoop++)
     {
         if(FS_FILE_STAT_SUBDIR == pstFDT->ucFileStat)
         {
             uiFDTUse++;
         }
         pstFDT++;
     }
 
     printf("clust %d cnt %d\n",vuiCurClu,uiFDTUse);
     *vpuiFDTCnt = uiFDTUse;
     free(pucBuf);
     
     return OK;
 }


