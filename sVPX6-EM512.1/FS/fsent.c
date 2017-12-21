/******************************************************************************

                  VERSION (C), 2001-2011, 

 ******************************************************************************
  file       : fsFAT.c
  version    : 1.0
  author     : lgh
  data       : 2012.9.4
  function   : file allocation table management 
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
 function     : fsReadFAT
 description  : read fs FAT information
 *****************************************************************************/
 STATUS fsReadEnt(FS_VOL_S  *vpstVol,UINT32  vuiEnt,UINT32  *vpuiVal)
 {
     UINT32  uiOffset = 0;
     UINT32  uiVal = 0;
     UINT32  uiClu = 0;

 	 if(vuiEnt >= vpstVol->uiVolClu)
 	 {
		FS_LOG(FS_DBG_ERR,"read cluster 0x%x total 0x%x overflow\n",
               vuiEnt,vpstVol->uiVolClu,0,0,0,0);

		return ERROR;
	 }
	 
     if(FS_CLUST_INVALID == vuiEnt)
     {
         *vpuiVal = FS_CLUST_INVALID;
         return OK;
     }

     
     FS_LOG(FS_DBG_DATA, "fsReadEnt: vuiEnt: %d, vol->uiCluByte: %d \n",
										vuiEnt, vpstVol->uiCluByte, 0, 0,0,0);
	
	 
     uiOffset = (vuiEnt * FS_BYTE_PER_FAT_ENTRY) % vpstVol->uiCluByte;
     uiClu = (vuiEnt * FS_BYTE_PER_FAT_ENTRY) / vpstVol->uiCluByte;
     uiClu += vpstVol->uiFatClu;

    
    FS_LOG(FS_DBG_DATA, "fsReadEnt: uioffset: %d, clu: %d, fatclu: %d \n",
										uiOffset, uiClu, vpstVol->uiFatClu, 0,0,0);
	
	 
     if(OK != gstFsCtrl.stCacheFunc.fsCacheByteRead(vpstVol,uiClu,uiOffset,
                                                  (UINT8*)&uiVal,sizeof(UINT32)))
     {
         FS_LOG(FS_DBG_ERR, "read dev %d cache byte failed\n",
                vpstVol->uiVolId,0,0,0,0,0);
         return ERROR;
     }
 
     *vpuiVal = uiVal;
     FS_LOG(FS_DBG_DATA, "get clust %d content %d\n",vuiEnt,uiVal,0,0,0,0);
 
     return OK;     
 }

 /*****************************************************************************
 function     : fsWriteFAT
 description  : write fs FAT information
 input        : 
 output       : 
 return       : 
 *****************************************************************************/
STATUS fsWriteEnt(FS_VOL_S *vpstVol,UINT32 vuiEnt,UINT32 vuiVal,UINT32 uiOption)
{
    UINT32  uiOffset = 0;
    UINT32  uiClu = 0;
    
    if(vuiEnt > vpstVol->uiVolClu)
    {
        FS_LOG(FS_DBG_ERR,"write cluster %d total %d overflow\n",
               vuiEnt,vpstVol->uiVolClu,0,0,0,0);
        return ERROR;
    }
    
    if(FS_CLUST_INVALID == vuiEnt)
    {
        return OK;
    }
    
    uiOffset = (vuiEnt * FS_BYTE_PER_FAT_ENTRY) % vpstVol->uiCluByte;
    uiClu = (vuiEnt * FS_BYTE_PER_FAT_ENTRY) / vpstVol->uiCluByte;
    uiClu += vpstVol->uiFatClu;
    
    if(vuiEnt >= vpstVol->uiVolClu)
    {
        FS_LOG(FS_DBG_ERR,"write vol %s clu %d val %d cnt %d error\n",
               vpstVol->aucPartName,vuiEnt,vuiVal,vpstVol->uiVolClu,0,0);
        return ERROR;
    }

    if(FS_CACHE_WR_BACK != uiOption)
    {
        if(OK != gstFsCtrl.stCacheFunc.fsCacheByteWrite(vpstVol,uiClu,uiOffset,
                                (UINT8*)&vuiVal,sizeof(UINT32)))
        {
            FS_LOG(FS_DBG_ERR,"write dev %d cache byte failed\n",
                    vpstVol->uiVolId,0,0,0,0,0);
            return ERROR;
        }
        return OK;
    }
    
    if(OK != gstFsCtrl.stCacheFunc.fsCacheByteWrBack(vpstVol,uiClu,uiOffset,
                                (UINT8*)&vuiVal,sizeof(UINT32)))
    {
        FS_LOG(FS_DBG_ERR,"write dev %d cache byte failed\n",
                vpstVol->uiVolId,0,0,0,0,0);
        return ERROR;
    }
    
    return OK;  
}

 /*****************************************************************************
 function     : fsGetFreeFAT
 description  : get one free fs fat entry
 input        : 
 output       : 
 return       : 
 *****************************************************************************/
 STATUS fsGetFreeEnt(FS_VOL_S  *vpstVol,UINT32  *vpuiClust)
 {
    UINT32 uiLoop = 0;
    UINT32 uiEnt = 0;
    UINT32 uiStartClu = 0;
    static UINT32 uiLastFree = 0;

	uiStartClu = vpstVol->uiRootClu + vpstVol->uiFDTCluCnt + 1;
    for(uiLoop = uiStartClu;uiLoop < vpstVol->uiVolClu;uiLoop++)
    {
        uiLastFree = uiLastFree % vpstVol->uiVolClu;
        if(uiLastFree < uiStartClu)
        {
            uiLastFree = uiStartClu;
        }
        if(FS_CLUST_EOF == uiLastFree)
        {
            FS_LOG(FS_DBG_ERR,"fsGetFreeEnt last free clu invalid\n",
                   uiLastFree,0,0,0,0,0);
        }
        
        if(OK != fsReadEnt(vpstVol,uiLastFree,&uiEnt))
        {
            return ERROR;
        }
        
        if(FS_CLUST_AVAIL == uiEnt)
        {
            /*FS_CACHE_WR_BACK*/
            if(OK != fsWriteEnt(vpstVol,uiLastFree,FS_CLUST_EOF,FS_CACHE_WR_BACK))
            {
               return ERROR;
            }         
            *vpuiClust = uiLastFree;
             uiLastFree++;
            return OK;
        }
        uiLastFree++;
    }

    return ERROR;
 }
 
 /*****************************************************************************
 function     : fsGetModLastFAT
 description  : get free fat and write last fat
 *****************************************************************************/
 STATUS fsGetModLastEnt(FS_VOL_S *vpstVol,UINT32 vuiLastClu,UINT32 *vpuiClust)
 {
     UINT32 uiCurClu = 0;
     
     if(OK != fsGetFreeEnt(vpstVol,&uiCurClu))
     {
         return ERROR;
     }
 
     *vpuiClust = uiCurClu;    
     /*
      *update cluster
     */
     if(FS_CLUST_INVALID != vuiLastClu)
     {
         if(OK != fsWriteEnt(vpstVol,vuiLastClu,uiCurClu,FS_CACHE_WR_THROUGH))
         {
             return ERROR;
         }
     }
     return OK;
 }

 /*****************************************************************************
 function     : fsClearFatChain
 description  : clear fat table from start of clust
 *****************************************************************************/
 STATUS fsClearEntChain(FS_VOL_S  *vpstVol,UINT32 vuiFirstClust)
 {
     UINT32   uiCurClu = 0;
     UINT32   uiNextClu = 0;
 
     uiCurClu = vuiFirstClust;
     uiNextClu = uiCurClu;
 
     if(FS_CLUST_INVALID == vuiFirstClust)
     {
         return OK;
     }
     
     while((FS_CLUST_EOF != uiNextClu) && (FS_CLUST_AVAIL != uiNextClu))
     {
        if(FS_CLUST_EOF == uiCurClu)
        {
            FS_LOG(FS_DBG_ERR,"fsClearEntChain current clu invalid\n",
                   0,0,0,0,0,0);
        }
        
         if(OK != fsReadEnt(vpstVol,uiCurClu,&uiNextClu))
         {
             FS_LOG(FS_DBG_INFO,"read fat failed\n",0,0,0,0,0,0);
             return ERROR;
         }
         
         FS_LOG(FS_DBG_DATA,"clear fat %d \n",uiCurClu,0,0,0,0,0);
         
         if(OK != fsWriteEnt(vpstVol,uiCurClu,FS_CLUST_AVAIL,FS_CACHE_WR_THROUGH))
         {
             FS_LOG(FS_DBG_ERR,"clear fat failed\n",0,0,0,0,0,0);
             return ERROR;
         }
#ifdef FS_TRIM_SUPPORT		 
		 if(OK != fsCluRW(vpstVol,uiCurClu,vpstVol->uiCluByte,0,NULL,FS_OPER_TRIM))
		 {
			FS_LOG(FS_DBG_ERR,"set cluster trim failed\n",0,0,0,0,0,0);
            return ERROR;
		 }
#endif
         uiCurClu = uiNextClu;
     }
     
     return OK;
 }
 
 /*****************************************************************************
 function     : fsGetLastClu
 description  : get the last clust of the file
 *****************************************************************************/
 UINT32 fsGetLastEnt(FS_VOL_S *vpstVol,UINT32 vuiFirstClust,UINT32  *vuiLastClu)
 {
     UINT32   uiCurClu = 0;
     UINT32   uiNextClu = 0;
     UINT32   uiCluNum = 0;
 
     if(FS_CLUST_INVALID == vuiFirstClust)
     {
         *vuiLastClu = FS_CLUST_INVALID;
         return uiCluNum;
     }
     
     uiNextClu = vuiFirstClust;
     do
     {
         uiCurClu = uiNextClu;
         if(FS_CLUST_EOF == uiCurClu)
         {
             FS_LOG(FS_DBG_ERR,"fsGetLastEnt current clu invalid\n",
                    0,0,0,0,0,0);
         }
        FS_LOG(FS_DBG_DATA,"111GET LAST ENT uicurclu 0x%x ,next clu 0x%x \n",
                    uiCurClu,uiNextClu,0,0,0,0);
         if(OK != fsReadEnt(vpstVol,uiCurClu,&uiNextClu))
         {
             FS_LOG(FS_DBG_INFO,"read fat failed\n",0,0,0,0,0,0);
             return uiCluNum;
         }
		  FS_LOG(FS_DBG_DATA,"GET LAST ENT uicurclu 0x%x ,next clu 0x%x \n",
                    uiCurClu,uiNextClu,0,0,0,0);
         uiCluNum++;
     }while((FS_CLUST_EOF != uiNextClu)&&(FS_CLUST_AVAIL != uiNextClu));
/*2015-12-03 jingzhong */
#if 1
	 if(FS_CLUST_AVAIL == uiNextClu)
	 {
	 	FS_LOG(FS_DBG_ERR,"current cluster %d next %d\n",uiCurClu,uiNextClu,0,0,0,0,0);
	 	fsWriteEnt(vpstVol,uiCurClu,FS_CLUST_EOF,0);
	 }
#endif
     *vuiLastClu = uiCurClu;
     
     FS_LOG(FS_DBG_DATA,"get the first clu %d last clu %d\n",vuiFirstClust,
             uiCurClu,0,0,0,0);
 
     return uiCluNum;
 }


