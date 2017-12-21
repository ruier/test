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

/*----------------------------------------------*
 * constant variable                            *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macro definition                             *
 *----------------------------------------------*/

/*****************************************************************************
  function     : fsDirPathParse
  description  : read fat table to cache
  input        : 
  output       : 
  return       : ERROR or OK
 *****************************************************************************/
STATUS fsDirPathParse(UINT8   *vpcPath,FS_PATH_ARRAY_S *vpstNameArray,
                      UINT32   vuiNumArray)
{
    UINT32	 uiPathLevels = 0;
    UINT32   uiPathPos = 0;
    BOOL     bVaild = FALSE;
    UINT8   *pcName = NULL;
	UINT32   uiLoop = 0;

    /*assert*/
    pcName = vpcPath;
    uiPathLevels = 0;
    sprintf ((char *)vpstNameArray, "%s","/");
    uiPathLevels++;

    while(*pcName != FS_EOS)
	{
    	/* 
    	 *pass slashes
    	*/
    	if((*pcName == FS_SLASH) || (*pcName == FS_BACK_SLASH))
        {
            pcName++;
			uiPathPos = 0;
			if(TRUE == bVaild)
			{
				uiPathLevels++;
			}
			bVaild = FALSE;
            continue;
        }
        if((*pcName == FS_DOT) && (*(pcName + 1) == FS_SLASH))
        {
            pcName = pcName + 2;
            continue;
        }
    	/*
    	 *regular name: insert it into array 
    	*/
    	if((uiPathLevels >= vuiNumArray) || (uiPathPos >= (FS_MAX_NAME_LEN - 1)))
    	{
    		break;
    	}
		
		bVaild = TRUE;
    	vpstNameArray[uiPathLevels].aucName[uiPathPos] = *pcName;
		vpstNameArray[uiPathLevels].aucName[uiPathPos + 1] = '\0';
		uiPathPos++;
		pcName++;
	}
	/* 
	 *path termination has not been reached 
	*/
    if(*pcName != FS_EOS)	
	{
		return ERROR;
	}

	if(TRUE == bVaild)
	{
		uiPathLevels++;
	}
	
	for(uiLoop = 0;uiLoop < uiPathLevels;uiLoop++)
	{
		FS_LOG(FS_DBG_DATA," %s \n",vpstNameArray[uiLoop].aucName,0,0,0,0,0);
	}

    return uiPathLevels;
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

