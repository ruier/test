/******************************************************************************

                  VERSION (C), 2001-2011, 

 ******************************************************************************
  file       : fsLib.c
  version    : 1.0
  author     : lgh
  data       : 2012.2.2
  function   : file system library
******************************************************************************/

/*----------------------------------------------*
 * header file                                  *
 *----------------------------------------------*/
 

#include "fsLib.h"

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
UINT32 guiCryptTable[FS_HASH_TABLE_SIZE]; 

/*----------------------------------------------*
 * constant variable                            *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macro definition                             *
 *----------------------------------------------*/

/*****************************************************************************
  function     : fsDirGetDirName
  description  : get dir name from file name
  input        : 
  output       : 
  return       : ERROR or OK
 *****************************************************************************/
STATUS fsGetDirName(UINT8  *vpcFileName,FS_PATH_ARRAY_S *vpstDirName)
{
    UINT32   uiPathPos = 0;
    BOOL     bVaild = FALSE;
    UINT32   uiNameLen = 0;
    UINT8   *pcName = NULL;
    CHAR    *pcDest = NULL;
	INT32    iLoop = 0;

  	uiNameLen = strlen(vpcFileName);
  	pcDest = vpstDirName;
  	if(uiNameLen < 2)
  	{
		*pcDest = '/';
		pcDest++;
		*pcDest = '\0';
		return OK;
  	}
  	
	for(iLoop = (uiNameLen - 1);iLoop >= 0;iLoop--)
	{
		pcName = vpcFileName + iLoop;
		if((FS_SLASH == *pcName) || (FS_BACK_SLASH == *pcName))
		{
			uiPathPos = iLoop;
			break;
		}
	}

	if(0 == uiPathPos)
  	{
		*pcDest = '/';
		pcDest++;
		*pcDest = '\0';
		return OK;
  	}
  	
	for(iLoop = 0;iLoop < uiPathPos;iLoop++)
	{
		pcName = vpcFileName + iLoop;
		if(((FS_SLASH == *pcDest) || (FS_BACK_SLASH == *pcDest)) && 
		   ((FS_SLASH == *pcName) || (FS_BACK_SLASH == *pcName)))
		{
			printf("path name error\n");
			return ERROR;
		}
		*pcDest = *pcName;
		pcDest++;
	}
	*pcDest = '\0';

	/*printf("filename %s dir %s\n",vpcFileName,vpstDirName);*/
	
    return OK;
}


VOID UINT64_INC(UINT64_S *pullVal)
{
    if(0xffffffff == pullVal->lo)
    {
        pullVal->hi++;
        pullVal->lo = 0;
    }
    else
    {
        pullVal->lo++;
    }
}

VOID UINT64_ADD(UINT64_S *pullRst,UINT64_S *pullAdd1,UINT32 uiAdd)
{
    if((pullAdd1->lo + uiAdd) < pullAdd1->lo) 
    {
        pullRst->hi = pullAdd1->hi + 1;
        pullRst->lo = pullAdd1->lo + uiAdd;
    }
    else
    {
        pullRst->hi = pullAdd1->hi;
        pullRst->lo = pullAdd1->lo + uiAdd;
    }
}

VOID UINT64_SUB32(UINT64_S *pullRst,UINT64_S *pullSub1,UINT32 uiSub)
{
    if(pullSub1->lo >= uiSub)
    {
        pullRst->lo = pullSub1->lo - uiSub;
        pullRst->hi = pullSub1->hi;
    }
    else
    {
        pullRst->lo = pullSub1->lo - uiSub;
        pullRst->hi = pullSub1->hi - 1;
    }  
}

VOID UINT64_SUB64(UINT64_S *pullRst,UINT64_S *pullSub1,UINT64_S *puiSub)
{
    if(pullSub1->lo >= puiSub->lo)
    {
        pullRst->lo = pullSub1->lo - puiSub->lo;
        pullRst->hi = pullSub1->hi - puiSub->hi;
    }
    else
    {
        pullRst->lo = pullSub1->lo - puiSub->lo;
        pullRst->hi = pullSub1->hi - puiSub->hi - 1;
    }
}

STATUS UINT64_CMPG(UINT64_S *pullValA,UINT64_S *pullValB)
{
    if(pullValA->hi > pullValB->hi)
    {
        return TRUE;
    }
    if(pullValA->hi < pullValB->hi)
    {
        return FALSE;
    }
    if(pullValA->lo > pullValB->lo)
    {
        return TRUE;
    }

    return FALSE;
}

STATUS UINT64_CMPGE(UINT64_S *pullValA,UINT64_S *pullValB)
{
    if(pullValA->hi > pullValB->hi)
    {
        return TRUE;
    }
    if(pullValA->hi < pullValB->hi)
    {
        return FALSE;
    }
    if(pullValA->lo >= pullValB->lo)
    {
        return TRUE;
    }

    return FALSE;
}

STATUS UINT64_ISZERO(UINT64_S *pullValA)
{
    if((0x0 == pullValA->hi) && (0x0 == pullValA->lo))
    {
        return TRUE;
    }
    
    return FALSE;
}

VOID fsHexDump(UINT8 *pbuf, UINT32 len)
{
    UINT32 i;
    
    for (i = 0; i < len; )
    {
        if (len - i >= 4)
        {
          logMsg("%x %x %x %x \n",pbuf[i],pbuf[i+1],pbuf[i+2],pbuf[i+3],0,0);
          i += 4;
        }
        else
        {
          logMsg("%x \n",pbuf[i],0,0,0,0,0);
          i++;
        }
    }

    logMsg("\n",0,0,0,0,0,0);
    
}


/*****************************************************************************
function     : fsPrepCryptTable
description  : prepare hash table
*****************************************************************************/
VOID fsPrepCryptTable(VOID)  
{   
    UINT32 uiSeed = 0x00100001;
    UINT32 uiIdx1 = 0;
    UINT32 uiIdx2 = 0;
    UINT32 uiTemp1 = 0;
    UINT32 uiTemp2 = 0;
    UINT32 uiLoop = 0;  

	memset(guiCryptTable,0,FS_HASH_TABLE_SIZE);
    for( uiIdx1 = 0; uiIdx1 < 0x1500; uiIdx1++ )  
    {   
        for(uiIdx2 = uiIdx1,uiLoop = 0;uiLoop < 5;uiLoop++,uiIdx2 += 0x1500)  
        {
            uiSeed = (uiSeed * 125 + 3) % 0x2AAAAB;  
            uiTemp1 = (uiSeed & 0xFFFF) << 0x10;  
  
            uiSeed = (uiSeed * 125 + 3) % 0x2AAAAB;  
            uiTemp2 = (uiSeed & 0xFFFF); 
  			assert(uiIdx2 < FS_HASH_TABLE_SIZE);
            guiCryptTable[uiIdx2] = ( uiTemp1 | uiTemp2);   
       }   
   }   
} 

/*****************************************************************************
function     : fsHashString
description  : calc hash value for hash string and key
*****************************************************************************/
UINT32 fsHashString(CHAR *pcFileName,UINT32 uiHashType)  
{   
    CHAR  *cKey  = pcFileName;  
    UINT32 uiSeed1 = 0x7FED7FED;  
    UINT32 uiSeed2 = 0xEEEEEEEE;
    UINT32 uiIdx = 0;
    CHAR  iChar;  
  
    while( *cKey != 0 )  
    {   
        iChar = toupper(*cKey++);     
        uiIdx = (uiHashType << 8) + iChar;
        assert(uiIdx < FS_HASH_TABLE_SIZE);
        uiSeed1 = guiCryptTable[uiIdx] ^ (uiSeed1 + uiSeed2);  
        uiSeed2 = iChar + uiSeed1 + uiSeed2 + (uiSeed2 << 5) + 3;   
    }
    
    return uiSeed1;
}

VOID fsHashTest(CHAR *pcFileName)
{
	UINT32 uiLoop = 0;
	UINT32 uiHash = 0;

	logMsg("hash table 0x%x\n",&guiCryptTable[0],0,0,0,0,0);
	for(uiLoop = 0;uiLoop < FS_MAX_HASH_KEY;uiLoop++)
	{
		uiHash = fsHashString(pcFileName,uiLoop);
		logMsg("hash key %d hash %d\n",uiLoop,uiHash,0,0,0,0);
	}
}

