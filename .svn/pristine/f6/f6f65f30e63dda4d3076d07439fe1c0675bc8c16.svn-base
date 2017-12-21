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
function     : fsDataInCache
description  : data in cache or not
*****************************************************************************/
STATUS fsDataInCache(FS_VOL_S *vpstVol,UINT32 vuiStartClu,UINT32 vuiCluCnt)
{
    UINT32  uiCluEnd = 0;

#ifdef FS_CACHE_ENABLE
    uiCluEnd = vuiStartClu + vuiCluCnt;
    if((vuiStartClu >= vpstVol->uiMinCacheClu) && 
       (vpstVol->uiMaxCacheClu >= uiCluEnd))
    {
        return OK;
    }
#endif
    return ERROR;
}

/*****************************************************************************
  function     : fsReadFatToCache
  description  : read fat table to cache
 *****************************************************************************/
STATUS fsReadFatToCache(FS_VOL_S  *vpstVol)
{
#ifdef FS_CACHE_ENABLE    
    UINT32       uiLen = 0;
    UINT32       uiLoop = 0;
    UINT32       uiClu = 0;
    CHAR        *pcBuf = NULL;

    uiLen = vpstVol->uiCluByte * vpstVol->uiCluPerFat;
    vpstVol->uiCacheDataLen = uiLen;
    if(NULL == vpstVol->pcCacheData)
    {
        vpstVol->pcCacheData = malloc(uiLen);
        if(NULL == vpstVol->pcCacheData)
        {
            FS_LOG(FS_DBG_ERR,"malloc cache data buffer %d failed\n",uiLen,
                   0,0,0,0,0);
            return ERROR;
        }
    }

    FS_LOG(FS_DBG_ERR,"cache buffer 0x%x len 0x%x CluPerFat %d \n",
           vpstVol->pcCacheData,uiLen,
           vpstVol->uiCluPerFat,0,0,0);
    bzero(vpstVol->pcCacheData,uiLen);
    vpstVol->uiMaxCacheClu = vpstVol->uiFatClu + vpstVol->uiCluPerFat;
    vpstVol->uiMinCacheClu = vpstVol->uiFatClu;
 
    /*
     *read fat table to cache
    */
    pcBuf = vpstVol->pcCacheData;
    for(uiLoop = 0;uiLoop < vpstVol->uiCluPerFat;uiLoop++)
    {
        uiClu = uiLoop + vpstVol->uiMinCacheClu;
        if(OK != fsCluRW(vpstVol,uiClu,vpstVol->uiCluByte,0,pcBuf,FS_OPER_READ))
        {
            FS_LOG(FS_DBG_ERR,"read vol %d clu 0x%x failed\n",
                   vpstVol->uiVolId,uiClu,0,0,0,0);
            return ERROR;
        }
        pcBuf = pcBuf + vpstVol->uiCluByte;
    }
	
    FS_LOG(FS_DBG_ERR,"read vol %p cache %p min clu %d succeed\n",vpstVol,
           vpstVol->pcCacheData,vpstVol->uiMinCacheClu,0,0,0);
	
    return OK;
#else
    return ERROR;
#endif    
}

/*****************************************************************************
  function     : fsFreeFatCache
  description  : free fat table cache memory
  input        : 
  output       : 
  return       : 
 *****************************************************************************/
STATUS fsFreeFatCache(FS_VOL_S *vpstVol)
{
	if(NULL != vpstVol->pcCacheData)
	{
		free(vpstVol->pcCacheData);
		vpstVol->pcCacheData = NULL;
	}
    return OK;
}

/*****************************************************************************
  function     : fsReadCacheData
  description  : read data from cache
  input        : 
  output       : 
  return       : 
 *****************************************************************************/
STATUS fsReadCacheData(FS_VOL_S *vpstVol,UINT32 vuiClu,UINT32 vuiOffset,
                       UINT32 vuiLen,UINT8  *vpucBuf)
{
    CHAR      *pcBuf = NULL;
    UINT32     uiOffset = 0;
    
    pcBuf = vpstVol->pcCacheData;
    uiOffset = (vuiClu - vpstVol->uiMinCacheClu)* vpstVol->uiCluByte;
    uiOffset = uiOffset + vuiOffset;
    assert((uiOffset + vuiLen) <= vpstVol->uiCacheDataLen);
    pcBuf = pcBuf + uiOffset;
    bcopy(pcBuf,vpucBuf,vuiLen);
    
    return OK;   
}

/*****************************************************************************
  function     : fsWriteCacheData
  description  : write data to cache
  input        : 
  output       : 
  return       : 
 *****************************************************************************/
STATUS fsWriteCacheData(FS_VOL_S *vpstVol,UINT32 vuiClu,UINT32 vuiOffset,
                        UINT32 vuiLen,UINT8 *vpucBuf)

{
    CHAR      *pcBuf = NULL;
    UINT32     uiOffset = 0;
    
    pcBuf = vpstVol->pcCacheData;
    uiOffset = (vuiClu - vpstVol->uiMinCacheClu) * vpstVol->uiCluByte;
    uiOffset = uiOffset + vuiOffset;
    assert((uiOffset + vuiLen) <= vpstVol->uiCacheDataLen);
    pcBuf = pcBuf + uiOffset;
    bcopy(vpucBuf,pcBuf,vuiLen);  

	return OK;
}

/*****************************************************************************
  function     : fsCacheSecRead
  description  : cluster read or write sync
 *****************************************************************************/
STATUS fsCacheCluRead(FS_VOL_S *vpstVol,UINT32 vuiClu,UINT8 *vpucBuf)
{
    assert(vuiClu <= vpstVol->uiVolClu);
    
    if(OK == fsDataInCache(vpstVol,vuiClu,1))
    {
         return fsReadCacheData(vpstVol,vuiClu,0,vpstVol->uiCluByte,vpucBuf);
    }

    return fsCluRW(vpstVol,vuiClu,vpstVol->uiCluByte,0,vpucBuf,FS_OPER_READ);
}

/*****************************************************************************
  function     : fsCacheSecWrite
  description  : sector write sync
 *****************************************************************************/
STATUS fsCacheCluWrite(FS_VOL_S *vpstVol,UINT32 vuiClu,UINT8 *vpucBuf)
{   
    assert(vuiClu <= vpstVol->uiVolClu);
    
    /*
     *update buffer
    */
    if(OK == fsDataInCache(vpstVol,vuiClu,1))
    {
         fsWriteCacheData(vpstVol,vuiClu,0,vpstVol->uiCluByte,vpucBuf);
    }

    return fsCluRW(vpstVol,vuiClu,vpstVol->uiCluByte,0,vpucBuf,FS_OPER_WRITE);
}

/*****************************************************************************
  function     : fsCacheByteRead
  description  : byte read 
  input        : vuiOffset:offset in the buffer
 *****************************************************************************/
STATUS fsCacheByteRead(FS_VOL_S *vpstVol,UINT32 vuiClu,UINT32 vuiOffset,
                       UINT8    *vpucBuf,UINT32 vuiBytes)
{
    assert((vuiOffset + vuiBytes) <= vpstVol->uiCluByte);
    assert(vuiClu <= vpstVol->uiVolClu);

    if(OK == fsDataInCache(vpstVol,vuiClu,1))
    {
         return fsReadCacheData(vpstVol,vuiClu,vuiOffset,vuiBytes,vpucBuf);
    }

    return fsCluRW(vpstVol,vuiClu,vuiBytes,vuiOffset,vpucBuf,FS_OPER_READ);
}

/*****************************************************************************
  function     : fsCacheByteRead
  description  : byte read 
  input        : vuiOffset:offset in the buffer
 *****************************************************************************/
STATUS fsCacheByteWrite(FS_VOL_S *vpstVol,UINT32 vuiClu,UINT32 vuiOffset,
                        UINT8    *vpucBuf,UINT32 vuiBytes)
{
    CHAR      *pcBuf = NULL;
    UINT32     uiOffset = 0;
    UINT32     uiSecAlignStart = 0;
    UINT32     uiSecAlignEnd = 0;
    UINT32     uiByte = 0;
    
    assert((vuiOffset + vuiBytes) <= vpstVol->uiCluByte);
    assert(vuiClu <= vpstVol->uiVolClu);
    
    pcBuf  = vpucBuf;
    uiByte = vuiBytes;
    uiOffset = vuiOffset;
    /*
     *update buffer
    */
    if(OK == fsDataInCache(vpstVol,vuiClu,1))
    {
        pcBuf = vpstVol->pcCacheData;
        uiOffset = (vuiClu - vpstVol->uiMinCacheClu) * vpstVol->uiCluByte;
        uiOffset = uiOffset + vuiOffset;
        assert((uiOffset + vuiBytes) <= vpstVol->uiCacheDataLen);
        pcBuf = pcBuf + uiOffset;
        bcopy(vpucBuf,pcBuf,vuiBytes);
        uiSecAlignStart = uiOffset & (~0x1ff);
        uiSecAlignEnd =  (uiOffset + vuiBytes - 1) & (~0x1ff);
        if(uiSecAlignStart != uiSecAlignEnd)
        {
            FS_LOG(FS_DBG_ERR,"offset %d byte %d start %d end %d\n",uiOffset,
                   vuiBytes,uiSecAlignStart,uiSecAlignEnd,0,0);    
        }
        assert(uiSecAlignStart == uiSecAlignEnd);
        pcBuf = vpstVol->pcCacheData + uiSecAlignStart;
        uiOffset = uiOffset & (~0x1ff);
        uiByte = 512; /*for performace 512byte align*/
#ifdef FS_CACHE_MON_POLICY		
         vpstVol->uiCacheDirty = FS_CACHE_DIRTY;        
         return OK;
#endif        
    }
	uiOffset = uiOffset % vpstVol->uiCluByte;
    return fsCluRW(vpstVol,vuiClu,uiByte,uiOffset,pcBuf,FS_OPER_WRITE);
}

/*****************************************************************************
  function     : fsCacheByteWrBack
  description  : byte write back (write to cache only)
  input        : vuiOffset:offset in the buffer
 *****************************************************************************/
STATUS fsCacheByteWrBack(FS_VOL_S *vpstVol,UINT32 vuiClu,UINT32 vuiOffset,
                         UINT8    *vpucBuf,UINT32 vuiBytes)
{
    assert((vuiOffset + vuiBytes) <= vpstVol->uiCluByte);

    /*
     *update buffer
    */
    if(OK == fsDataInCache(vpstVol,vuiClu,1))
    {
         fsWriteCacheData(vpstVol,vuiClu,vuiOffset,vuiBytes,vpucBuf);
#ifdef FS_CACHE_MON_POLICY		
         vpstVol->uiCacheDirty = FS_CACHE_DIRTY;        
         return OK;
#endif
    }

    return fsCluRW(vpstVol,vuiClu,vuiBytes,vuiOffset,vpucBuf,FS_OPER_WRITE);
}

/*****************************************************************************
  function     : fsCacheMon
  description  : cache monitor task
  input        : vuiOffset:offset in the buffer
 *****************************************************************************/
STATUS fsCacheMon(FS_VOL_S *vpstVol)
{
	UINT32     uiClu = 0;
	UINT8     *pcBuf = NULL;
	
	while(vpstVol->uiCacheTaskRuning)
	{	
		if(FS_CACHE_DIRTY != vpstVol->uiCacheDirty)
		{
			taskDelay(sysClkRateGet());
			continue;
		}
		pcBuf = vpstVol->pcCacheData;
		semTake(vpstVol->stVolSem,WAIT_FOREVER);
		vpstVol->uiCacheDirty = 0;
		for(uiClu = vpstVol->uiMinCacheClu;uiClu < vpstVol->uiMaxCacheClu;uiClu++)
		{
			fsCluRW(vpstVol,uiClu,vpstVol->uiCluByte,0,pcBuf,FS_OPER_WRITE);
			FS_LOG(FS_DBG_DATA,"fsCacheMon clu %d byte %d\n",uiClu,
			       vpstVol->uiCluByte,0,0,0,0); 
			pcBuf = pcBuf + vpstVol->uiCluByte;
		}
		semGive(vpstVol->stVolSem);
		taskDelay(sysClkRateGet());
	}
}

