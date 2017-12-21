/******************************************************************************

                  VERSION (C), 2001-2011, 

 ******************************************************************************
  file       : rioDrv.c
  version    : 1.0
  author     : lgh
  data       : 2013.1.24
  function   : rapid io driver
******************************************************************************/

/*----------------------------------------------*
 * header file                                  *
 *----------------------------------------------*/
#include <vxWorks.h>
#include <string.h>
#include <stdlib.h>
#include <intLib.h>
#include <semLib.h>
#include "assert.h"
#include "rioDrv.h"
#include "rioHal.h"

/*----------------------------------------------*
 * external  variable                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external  function                           *
 *----------------------------------------------*/
extern STATUS rioAcquireDevLock(UINT32 ,UINT32 ,UINT32 ,UINT32 ,UINT32 *);
extern VOID rioSetHostId(UINT32 vuiLocPort,UINT32 uiHostId);
extern VOID rioGetSwitchPortInfo(UINT32 ,UINT32 ,UINT32 ,UINT32 *);
extern VOID rioRouteAddEntry(UINT32,UINT32,UINT32,UINT32,UINT32,UINT32);
extern VOID rioGetPortErrStatus(UINT32,UINT32,UINT32,UINT32,UINT32,UINT32*);
extern BOOL rioIsSwitch(UINT32 localPort,UINT32 destId,UINT32 hopCount);
extern BOOL rioIsAgent(UINT32 vuiLocPort);
extern BOOL rioMPortIsActive(UINT32 vuiLocPort);
extern UINT32 rioGetHostPort(UINT32 localPort,UINT32 destId,UINT32 hopCount);
extern VOID rioSwitchInit(UINT32,UINT32,UINT32);
extern VOID rioSetDeviceId (UINT32 ,UINT32 ,UINT32 ,UINT32 );
extern VOID rioSetRemoteDevFound(UINT32 localPort,UINT32 destId,UINT32 hopCnt);
extern STATUS rioCheckAccess(UINT32 localPort,UINT32 destid,UINT32 hopcount);
extern VOID rioGetCompTag(UINT32 ,UINT32 ,UINT32 ,UINT32 *);
extern VOID rioSetCompTag(UINT8 ,UINT32 ,UINT8 ,UINT32 );
extern VOID rioGetHostId(UINT32 vuiLocPort,UINT32 *vpuiHostId);
extern STATUS rioPortRecover(UINT32,UINT32,UINT32,UINT32);
extern STATUS rioRouteGetEntry(UINT32,UINT32,UINT32,UINT32,UINT32,UINT32 *);
extern STATUS rioSetLocDevToHost(UINT32 localPort);
extern BOOL rioLocDevIsHost(UINT32 localPort);
extern VOID rioCfgRead(UINT32,UINT32,UINT32,UINT32,UINT32 *);
extern VOID rioCfgWrite(UINT32,UINT32,UINT32,UINT32,UINT32);
extern STATUS rioSyncAckID(UINT32 ,UINT32 ,UINT32,UINT32 ,UINT32);

/*----------------------------------------------*
 * internal  function                           *
 *----------------------------------------------*/
VOID rioSystemSnoop(VOID);
STATUS rioSwitchHasFound(UINT32 localPort,UINT32 destId,UINT32 hopCount);
VOID rioGetSwitchIdx(UINT32,UINT32,UINT32,UINT32*);
VOID rioGetRouteInfo(UINT32 localPort,UINT32 uiHop);

/*----------------------------------------------*
 * global  variable                             *
 *----------------------------------------------*/

 UINT32 guiDbgLevel = SRIO_DBG_ERR;
 SRIO_MPORT_S  gstSrioMport[SRIO_MAX_MPORT];
 LOCAL BOOL    guiSrioInit = FALSE;
 
 SRIO_DID_S gastDIDCAR[] = {{0x70100002,"8641"},
                            {0x70110002,"8641D"},
                            {SRIO_TSI578_DID,"TSI578"},
                            {SRIO_CP1848_DID,"CPS1848"},
                            {0x0000000e,"XILINX"},
                            {SRIO_HOST_DEV_TYE,"HOST"},
                            {0x00120002,"8548E"},
                            {0x270000e,"k7"}
                            };
                            
/*----------------------------------------------*
 * constant variable                            *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macro definition                             *
 *----------------------------------------------*/

VOID rioSwitchSrcInit(SRIO_SWITCH_S *vpstSwitch)
{
    UINT32        uiPortIdx = 0;

    memset(vpstSwitch->aucRouteTable,SRIO_INVALID_PORT,SRIO_MAX_DEV_NUM);
    memset(vpstSwitch->aucSWPort,SRIO_INVALID_SW,SRIO_MAX_PORT_PER_SW);
    vpstSwitch->uiCfgStatus = 0;
    vpstSwitch->uiUpPort = 0;
    vpstSwitch->uiUpSWIdx = 0;
    vpstSwitch->uiHop = 0;
    for(uiPortIdx = 0;uiPortIdx < SRIO_MAX_PORT_PER_SW;uiPortIdx++)
    {
        vpstSwitch->astPort[uiPortIdx].uiDevId = SRIO_INVALID_DEV;
        vpstSwitch->astPort[uiPortIdx].uiDevType = 0;
        vpstSwitch->astPort[uiPortIdx].uiStatus = SRIO_PORT_DOWN;
    }  
}

SRIO_SWITCH_S* rioGetSWitch(SRIO_MPORT_S *vpstMPort,UINT32 vuiHop)
{
    UINT32          uiLoop = 0;
    UINT32          uiVal = 0;
    UINT32          uiSWIdx = 0;

    if(TRUE == rioSwitchHasFound(0,RIO_ANY_DEV_ID,vuiHop))
    {
        rioGetSwitchIdx(0,RIO_ANY_DEV_ID,vuiHop,&uiSWIdx);
        assert(uiSWIdx < SRIO_MAX_SWITCH);
        if(SRIO_DISC_FREE == vpstMPort->astSwitch[uiSWIdx].uiDiscStatus)
        {
            rioSwitchSrcInit(&vpstMPort->astSwitch[uiSWIdx]);
            vpstMPort->astSwitch[uiSWIdx].uiDiscStatus = SRIO_WAIT_DISC;
            vpstMPort->astSwitch[uiSWIdx].uiIdx = uiSWIdx;
			/*×öÊ²Ã´ÓÃµ by liudan*/
            vpstMPort->astSwitch[uiSWIdx].uiNewAlloc = SRIO_NEW_ALLOC;
            rioCfgRead(0,RIO_ANY_DEV_ID,vuiHop,0,&uiVal);
            vpstMPort->astSwitch[uiSWIdx].uiDID = uiVal;
        }
        else
        {
            vpstMPort->astSwitch[uiSWIdx].uiNewAlloc = SRIO_ALLOCED;
        }
        return &(vpstMPort->astSwitch[uiSWIdx]);
    }

    
    for(uiLoop = 0;uiLoop < SRIO_MAX_SWITCH;uiLoop++)
    {
        if(SRIO_DISC_FREE == vpstMPort->astSwitch[uiLoop].uiDiscStatus)
        {
            vpstMPort->astSwitch[uiLoop].uiDiscStatus = SRIO_WAIT_DISC;
            vpstMPort->astSwitch[uiLoop].uiIdx = uiLoop;
            vpstMPort->astSwitch[uiLoop].uiNewAlloc = SRIO_NEW_ALLOC;
            rioSwitchSrcInit(&vpstMPort->astSwitch[uiLoop]);
            SRIO_LOG(SRIO_DBG_INFO,"new switch %x is found\n",uiLoop,0,0,0,0);
            rioSetCompTag(0,RIO_ANY_DEV_ID,vuiHop,(uiLoop + SRIO_MIN_COMP_TAG));
            rioSwitchInit(0,RIO_ANY_DEV_ID,vuiHop);
            rioCfgRead(0,RIO_ANY_DEV_ID,vuiHop,0,&uiVal);
            vpstMPort->astSwitch[uiLoop].uiDID = uiVal;
            return &vpstMPort->astSwitch[uiLoop];
        }
    }

    return NULL;
}

SRIO_SWITCH_S* rioGetNextSWitch(SRIO_MPORT_S *vpstMPort)
{
    UINT32 uiLoop = 0;

    for(uiLoop = 0;uiLoop < SRIO_MAX_SWITCH;uiLoop++)
    {
        if(SRIO_WAIT_DISC == vpstMPort->astSwitch[uiLoop].uiDiscStatus)
        {
            vpstMPort->astSwitch[uiLoop].uiDiscStatus = SRIO_DISCED;
            return &vpstMPort->astSwitch[uiLoop];
        }
    }

    return NULL;
}


/*****************ºóÃæÐèÒªÐÞ¸Ä£¬ÏÈ²»¹Ü*****************/
int sysGetCpuID(void)
{

   int reg; 
	reg = *(int*)(CCSBAR + 0xe0020);
	reg &=0xf;
	return reg;
}



STATUS rioEnumHost(SRIO_MPORT_S *vpstMPort)
{
    UINT32 uiLockId = 0;
	
	 if(OK != rioAcquireDevLock(vpstMPort->uiLocPort,RIO_ANY_DEV_ID,RIO_HOP_LOCAL,
                               vpstMPort->uiHostId,&uiLockId))
    {
        return ERROR;
    }

    rioSetHostId(vpstMPort->uiLocPort,vpstMPort->uiHostId);
       vpstMPort->uiDownSwIdx = SRIO_INVALID_SW;

	/*
     *for hsp9b watchdog reset
	*/
#if 0	
	if(0 != FSL_READ_REG(0xe80c0148))
	{
		return OK;
	}
	if(0 == sysGetCpuID())
	{
		rioSyncAckID(0,RIO_ANY_DEV_ID,RIO_HOP_LOCAL,0,2);
	}
	else
	{
		rioSyncAckID(0,RIO_ANY_DEV_ID,RIO_HOP_LOCAL,0,0);
	}
#endif	
    return OK;
}

STATUS rioSwitchHasFound(UINT32 localPort,UINT32 destId,UINT32 hopCount)
{
    UINT32 uiVal = 0;

    rioGetCompTag(localPort,destId,hopCount,&uiVal);
    if((uiVal >= SRIO_MIN_COMP_TAG) && (uiVal <= SRIO_MAX_COMP_TAG))
    {
        return TRUE;
    }
    
    return FALSE;
}

VOID rioGetSwitchIdx(UINT32 localPort,UINT32 destId,UINT32 hopCnt,
                       UINT32 *vpuiSWIdx)
{
    UINT32          uiVal = 0;

    rioGetCompTag(localPort,destId,hopCnt,&uiVal);
    *vpuiSWIdx = uiVal - SRIO_MIN_COMP_TAG;
}

STATUS rioEnumSwitch(SRIO_MPORT_S *vpstMPort,SRIO_SWITCH_S *vpstSwitch)
{
    SRIO_SWITCH_S  *pstNewSW = NULL;
    UINT32          uiVal = 0;
    UINT32          uiPortNum = 0;
    UINT32          uiLoop = 0;
    UINT32          uiDevId = 0;
    UINT32          uiLocPort = 0;
    UINT32          uiHop = 0;
    
    uiLocPort = vpstMPort->uiLocPort;
    uiHop = vpstSwitch->uiHop;

    rioGetSwitchPortInfo(uiLocPort,RIO_ANY_DEV_ID,uiHop,&uiVal);
    uiPortNum = (uiVal >> 8) & 0xff;
    if(SRIO_INVALID_SW == vpstMPort->uiDownSwIdx)
    {
        vpstMPort->uiDownSwIdx = vpstSwitch->uiIdx;
    }
    
    assert(uiPortNum <= SRIO_MAX_PORT_PER_SW);
    rioRouteAddEntry(uiLocPort,RIO_ANY_DEV_ID,uiHop,RIO_GLOBAL_TABLE,
                     vpstMPort->uiHostId,vpstSwitch->uiUpPort);
    vpstSwitch->aucRouteTable[vpstMPort->uiHostId] = vpstSwitch->uiUpPort;                   
    assert(vpstMPort->uiHostId < SRIO_MAX_DEV_ID);                 
    vpstSwitch->aucRouteTable[vpstMPort->uiHostId] = vpstSwitch->uiUpPort;

    for(uiLoop = 0;uiLoop < uiPortNum;uiLoop++)
    {
       if(uiLoop > 11)
       {
            break;
	   }

	   
	   /*logMsg("uiLoop = 0x%x\n",uiLoop,0,0,0,0,0);*/
       rioGetPortErrStatus(uiLocPort,RIO_ANY_DEV_ID,uiHop,
                            0,uiLoop,&uiVal);
        if((0 == (uiVal & RIO_PORT_N_ERR_STS_PORT_OK)) ||
           (SRIO_PORT_UP == vpstSwitch->astPort[uiLoop].uiStatus))
        {
            continue;
        }
        
        vpstSwitch->astPort[uiLoop].uiStatus = SRIO_PORT_UP;
        if(uiLoop == vpstSwitch->uiUpPort)
        {
            continue;
        }
        uiDevId = (vpstSwitch->uiIdx * SRIO_MAX_PORT_PER_SW) + 0x20 + uiLoop;
        
        rioRouteGetEntry(uiLocPort,RIO_ANY_DEV_ID,uiHop,RIO_GLOBAL_TABLE,
                         uiDevId,&uiVal);
                         
        /* rioCfgWrite(uiLocPort,uiDevId,(uiHop + 1),0,uiVal);*/
        
        if((RIO_INVALID_ROUTE != uiVal) && (uiVal < SRIO_MAX_PORT_PER_SW))
        {
            vpstSwitch->astPort[uiLoop].uiDevId = uiDevId;
            vpstSwitch->aucRouteTable[uiDevId] = uiLoop;
            rioCfgRead(uiLocPort,uiDevId,(uiHop + 1),0,&uiVal);
            vpstSwitch->astPort[uiLoop].uiDevType = uiVal;
            continue;
        }
        rioRouteAddEntry(uiLocPort,RIO_ANY_DEV_ID,uiHop,RIO_GLOBAL_TABLE,
                         RIO_ANY_DEV_ID,uiLoop);
        if(OK != rioAcquireDevLock(uiLocPort,RIO_ANY_DEV_ID,(uiHop + 1),
                                   vpstMPort->uiHostId,&uiVal))
        {
            return RIO_ERROR;
        }
        /*printf("uiLoop11 = 0x%x\n",uiLoop);*/
	   
        if(TRUE == rioIsSwitch(uiLocPort,RIO_ANY_DEV_ID,(uiHop + 1)))
        {
            
            pstNewSW = rioGetSWitch(vpstMPort,(uiHop + 1));
            if(NULL == pstNewSW)
            {
                return ERROR;
            }

           
            if(SRIO_ALLOCED == pstNewSW->uiNewAlloc)
            {
                vpstSwitch->aucSWPort[uiLoop] = pstNewSW->uiIdx;
                vpstSwitch->uiSWMap |= (1 << pstNewSW->uiIdx);
                vpstSwitch->astPort[uiLoop].uiDevType = pstNewSW->uiDID;
                continue;
            }
            rioCfgRead(uiLocPort,RIO_ANY_DEV_ID,(uiHop + 1),0,&uiVal);
            vpstSwitch->astPort[uiLoop].uiDevType = uiVal;
            vpstMPort->uiSWMap |= (1 << pstNewSW->uiIdx);
            vpstSwitch->uiSWMap |= (1 << pstNewSW->uiIdx);
            pstNewSW->uiSWMap |= (1 << vpstSwitch->uiIdx);
            vpstSwitch->aucSWPort[uiLoop] = pstNewSW->uiIdx;
            pstNewSW->uiUpSWIdx = vpstSwitch->uiIdx;
            pstNewSW->uiUpPort = rioGetHostPort(0,RIO_ANY_DEV_ID,(uiHop + 1));
            assert(pstNewSW->uiUpPort <= SRIO_MAX_PORT_PER_SW);
            pstNewSW->uiHop = uiHop + 1;
            pstNewSW->aucSWPort[pstNewSW->uiUpPort] = vpstSwitch->uiIdx;
            pstNewSW->astPort[pstNewSW->uiUpPort].uiDevType = vpstSwitch->uiDID;
            SRIO_LOG(SRIO_DBG_INFO,"switch %x id %d hostport %d hop %d\n",
                     (UINT32)pstNewSW,pstNewSW->uiIdx,
                     pstNewSW->uiUpPort,pstNewSW->uiHop,0);
            
            continue;
        }
        vpstSwitch->astPort[uiLoop].uiDevId = uiDevId;
        rioCfgRead(uiLocPort,RIO_ANY_DEV_ID,(uiHop + 1),0,&uiVal);
        vpstSwitch->astPort[uiLoop].uiDevType = uiVal;
        rioSetRemoteDevFound(uiLocPort,RIO_ANY_DEV_ID,(uiHop + 1));
        rioSetDeviceId(uiLocPort,RIO_ANY_DEV_ID,(uiHop + 1),uiDevId);
        rioRouteAddEntry(uiLocPort,RIO_ANY_DEV_ID,uiHop,RIO_GLOBAL_TABLE,
                         uiDevId,uiLoop);
        vpstSwitch->aucRouteTable[uiDevId] = uiLoop;       
    }
    
    return OK;
}

STATUS rioSetup(VOID)
{
    memset(&gstSrioMport[0],0,SRIO_MAX_MPORT * sizeof(SRIO_MPORT_S));
    memset(gstSrioMport[0].astOW,0xff,SRIO_MAX_OUB * sizeof(SRIO_OW_S));
    gstSrioMport[0].uiHostId = SRIO_INIT_HOSTID;
    guiSrioInit = TRUE;
    rioSetLocDevToHost(0);
    
    return OK;
}

/*****************************************************************************
 func : rioCfgSwitchRoute
 desc : config switch route table belong to other switch
*****************************************************************************/
VOID rioCfgSwitchRoute(SRIO_MPORT_S *vpstMPort,SRIO_SWITCH_S *vpstSwitch,
                       SRIO_SWITCH_S *vpstCurSwitch,UINT32 vuiPort)
{
    SRIO_PORT_S     *pstPort = NULL;
    UINT32           uiPortIdx = 0;
    UINT32           uiDevId = 0;
    
    for(uiPortIdx = 0;uiPortIdx < SRIO_MAX_PORT_PER_SW;uiPortIdx++)
    {
        pstPort = &vpstCurSwitch->astPort[uiPortIdx];
        if(RIO_PORT_N_ERR_STS_PORT_OK != pstPort->uiStatus)
        {
            continue;
        }
        uiDevId = pstPort->uiDevId;
        if((SRIO_INVALID_DEV != uiDevId) &&
           (SRIO_INVALID_PORT == vpstSwitch->aucRouteTable[uiDevId]))
        {
            SRIO_LOG(SRIO_DBG_INFO,"port %d devid 0x%x\n",uiPortIdx,uiDevId,
                     0,0,0);
            rioRouteAddEntry(vpstMPort->uiLocPort,RIO_ANY_DEV_ID,
                        vpstSwitch->uiHop,RIO_GLOBAL_TABLE,uiDevId,vuiPort);
            vpstSwitch->aucRouteTable[uiDevId] = vuiPort;            
        }
    }
}

/*****************************************************************************
 func : rioGetIdxFromMap
 desc : get switch index from bitmap
*****************************************************************************/
UINT32 rioGetIdxFromMap(UINT32 vuiBitMap)
{
    UINT32 uiLoop = 0;

    for(uiLoop = 0;uiLoop < SRIO_MAX_SWITCH;uiLoop++)
    {
        if(vuiBitMap & (1 << uiLoop))
        {
            return uiLoop;
        }
    }

    return ERROR;
}

/*****************************************************************************
 func : rioCfgSigInDirectRoute
 desc : config sigle switch route table that indirect connected
*****************************************************************************/
VOID rioCfgSigInDirectRoute(SRIO_MPORT_S *vpstMPort,SRIO_SWITCH_S *vpstSwitch)
{
    UINT32    uiInDirMap = 0;
    UINT32    uiSWIdx = 0;

    uiInDirMap = vpstMPort->uiSWMap ^ vpstSwitch->uiSWMap;
    uiInDirMap = uiInDirMap ^ (1 << vpstSwitch->uiIdx);
    while(uiInDirMap != 0)
    {
        uiSWIdx = rioGetIdxFromMap(uiInDirMap);
        SRIO_LOG(SRIO_DBG_INFO,"config indirect switch %d dev on switch %d\n",
                 vpstSwitch->uiIdx,uiSWIdx,0,0,0);
        if((ERROR == uiSWIdx) || (uiSWIdx >= SRIO_MAX_SWITCH))
        {
            return;
        }
        uiInDirMap = uiInDirMap ^ (1 << uiSWIdx);
        rioCfgSwitchRoute(vpstMPort,vpstSwitch,&vpstMPort->astSwitch[uiSWIdx],
                          vpstSwitch->uiUpPort);
    }
}

/*****************************************************************************
 func : rioCfgSigSWRouteTable
 desc : config sigle switch route table
*****************************************************************************/
VOID rioCfgSigSWRoute(SRIO_MPORT_S *vpstMPort,SRIO_SWITCH_S *vpstSwitch)
{
    SRIO_SWITCH_S   *pstCurSwitch = NULL;
    UINT32           uiLoop = 0;
    UINT32           uiSwitchIdx = 0;
    
    for(uiLoop = 0;uiLoop < SRIO_MAX_PORT_PER_SW;uiLoop++)
    {
        if(SRIO_INVALID_SW == vpstSwitch->aucSWPort[uiLoop])
        {
            continue;
        }
        uiSwitchIdx = vpstSwitch->aucSWPort[uiLoop];
        pstCurSwitch = &vpstMPort->astSwitch[uiSwitchIdx];
        SRIO_LOG(SRIO_DBG_INFO,"config switch %d dev on switch %d\n",
                 vpstSwitch->uiIdx,pstCurSwitch->uiIdx,0,0,0);
        rioCfgSwitchRoute(vpstMPort,vpstSwitch,pstCurSwitch,uiLoop);
    }
    rioCfgSigInDirectRoute(vpstMPort,vpstSwitch);
    vpstSwitch->uiCfgStatus = SRIO_SW_CFG_OK;
}

/*****************************************************************************
 func : rioCfgAllSWRouteTable
 desc : config all switch route table
*****************************************************************************/
VOID rioCfgAllSWRoute(UINT32 vuiLocPort)
{
    SRIO_MPORT_S   *pstMPort = NULL;
    SRIO_SWITCH_S  *pstSwitch = NULL;
    SRIO_SWITCH_S  *pstNextSW = NULL;
    UINT32          uiLoop = 0;
    UINT32          uiSWIdx = 0;

    pstMPort = &gstSrioMport[vuiLocPort];
    if(pstMPort->uiDownSwIdx >= SRIO_MAX_SWITCH)
    {
        return;
    }
    pstSwitch = &pstMPort->astSwitch[pstMPort->uiDownSwIdx];
    rioCfgSigSWRoute(pstMPort,pstSwitch);
    
    for(uiLoop = 0;uiLoop < SRIO_MAX_PORT_PER_SW;uiLoop++)
    {
        if((SRIO_INVALID_SW == pstSwitch->aucSWPort[uiLoop]))
        {
            continue;
        }
        uiSWIdx = pstSwitch->aucSWPort[uiLoop];
        assert(uiSWIdx < SRIO_MAX_SWITCH);
        pstNextSW = &pstMPort->astSwitch[uiSWIdx];
        rioRouteAddEntry(vuiLocPort,RIO_ANY_DEV_ID,pstSwitch->uiHop,
                         RIO_GLOBAL_TABLE,RIO_ANY_DEV_ID,uiLoop);       
        rioCfgSigSWRoute(pstMPort,pstNextSW);
    }
}

STATUS rioSystemEnumerate(UINT32 vuiHostId)
{
    SRIO_MPORT_S   *pstMPort = NULL;
    SRIO_SWITCH_S  *pstSwitch = NULL;
    SRIO_SWITCH_S  *pstNextSW = NULL;
    UINT32          uiLoop = 0;
    UINT32          uiSWIdx = 0;
    UINT32          uiVal = 0;
    static UINT32   uiSnoop = 0;

    if(FALSE == guiSrioInit)
    {
        SRIO_LOG(SRIO_DBG_ERR,"device has not inited\n",0,0,0,0,0);
        return OK;
    }

    if(TRUE == rioIsAgent(0))
    {
        SRIO_LOG(SRIO_DBG_INFO,"device is agent\n",0,0,0,0,0);
        return OK;
    }

    if(TRUE != rioMPortIsActive(0))
    {
        SRIO_LOG(SRIO_DBG_ERR,"port %d is offline\n",0,0,0,0,0);
        return ERROR;
    }
    
    pstMPort = &gstSrioMport[0];
    pstMPort->uiHostId = vuiHostId;
    pstMPort->uiLocPort = 0;
 
    if(OK != rioEnumHost(pstMPort))
    {
        return ERROR;
    }

    if(OK != rioAcquireDevLock(0,RIO_ANY_DEV_ID,0,pstMPort->uiHostId,&uiVal))
    {
        return ERROR;
    }
    
     if(TRUE != rioIsSwitch(0,RIO_ANY_DEV_ID,0))
    {
        /*config end device to do*/
        SRIO_LOG(SRIO_DBG_ERR,"TODO:host is connect to end device\n",0,0,0,0,0);
		
		rioCfgRead(0,RIO_ANY_DEV_ID,0,0,&uiVal);
	
	    rioSetRemoteDevFound(0,RIO_ANY_DEV_ID,0);
	    rioSetDeviceId(0,RIO_ANY_DEV_ID,0,0x33);
        return OK;
    }

    pstSwitch = rioGetSWitch(pstMPort,0);
    if(NULL == pstSwitch)
    {
        return ERROR;
    }
    
    if(SRIO_NEW_ALLOC == pstSwitch->uiNewAlloc)
    {
        pstSwitch->uiUpSWIdx = SRIO_MASTER_SW_IDX;
        pstSwitch->uiUpPort = rioGetHostPort(0,RIO_ANY_DEV_ID,0);
        pstSwitch->astPort[pstSwitch->uiUpPort].uiDevId = pstMPort->uiHostId;
        pstSwitch->uiHop = 0;
        pstSwitch->astPort[pstSwitch->uiUpPort].uiDevType = SRIO_HOST_DEV_TYE;
    }
    
    pstMPort->uiSWMap |= (1 << pstSwitch->uiIdx);
    SRIO_LOG(SRIO_DBG_INFO,"master port 0x%x switch 0x%x\n",pstMPort,pstSwitch,0,0,0);   
    rioEnumSwitch(pstMPort,pstSwitch);
    pstSwitch->uiDiscStatus = SRIO_DISCED;

    for(uiLoop = 0;uiLoop < SRIO_MAX_PORT_PER_SW;uiLoop++)
    {
        if((SRIO_INVALID_SW == pstSwitch->aucSWPort[uiLoop]))
        {
            continue;
        }
        uiSWIdx = pstSwitch->aucSWPort[uiLoop];
        assert(uiSWIdx < SRIO_MAX_SWITCH);
        pstNextSW = &pstMPort->astSwitch[uiSWIdx];
        SRIO_LOG(SRIO_DBG_INFO,"master port 0x%x switch %d 0x%x\n",pstMPort,
                 uiSWIdx,pstNextSW,0,0); 
        rioRouteAddEntry(0,RIO_ANY_DEV_ID,pstSwitch->uiHop,RIO_GLOBAL_TABLE,
                         RIO_ANY_DEV_ID,uiLoop);
        rioEnumSwitch(pstMPort,pstNextSW);
        pstNextSW->uiDiscStatus = SRIO_DISCED;
    }
	
	rioCfgAllSWRoute(0);

    if(0 != uiSnoop)
    {
        return OK;
    }
    
    taskSpawn("rioMon",100,0,0x100000,(FUNCPTR)rioSystemSnoop,0,0,0,
                0,0,0,0,0,0,0);
    uiSnoop++;
    
    return OK;
}

VOID rioSwitchDelRoute(SRIO_SWITCH_S *vpstSwitch,SRIO_SWITCH_S *vpstDownSwitch)
{
    SRIO_PORT_S  *pstPort = NULL;
    UINT32        uiLoop = 0;
    
    for(uiLoop = 0;uiLoop < SRIO_MAX_PORT_PER_SW;uiLoop++)
    {
        pstPort = &vpstDownSwitch->astPort[uiLoop];
        if(SRIO_PORT_UP == pstPort->uiStatus)
        {
            rioRouteAddEntry(0,RIO_ANY_DEV_ID,vpstSwitch->uiHop,RIO_GLOBAL_TABLE,
                             pstPort->uiDevId,SRIO_INVALID_PORT);
            vpstSwitch->aucRouteTable[pstPort->uiDevId] = SRIO_INVALID_PORT;
        }
    }
}

VOID rioSwitchSnoop(SRIO_MPORT_S *vpstMPort,SRIO_SWITCH_S *vpstSwitch,
                    UINT32 *vuiNeedEnum)
{
    SRIO_PORT_S     *pstPort = NULL;
    UINT32           uiLoop = 0;
    UINT32           uiPortStatus = 0;
    UINT32           uiSWIdx = 0;

    *vuiNeedEnum = 0;
    for(uiLoop = 0;uiLoop < SRIO_MAX_PORT_PER_SW;uiLoop++)
    {
        pstPort = &vpstSwitch->astPort[uiLoop];
        rioGetPortErrStatus(vpstMPort->uiLocPort,RIO_ANY_DEV_ID,vpstSwitch->uiHop,
                            0,uiLoop,&uiPortStatus);
        uiPortStatus = uiPortStatus & RIO_PORT_N_ERR_STS_PORT_OK;
		
        if((SRIO_PORT_UP == pstPort->uiStatus) &&
           (RIO_PORT_N_ERR_STS_PORT_OK != uiPortStatus))
        {
            pstPort->uiStatus = SRIO_PORT_RECOVER;
            SRIO_LOG(SRIO_DBG_ERR,"switch %d port %d link down\n",
                     vpstSwitch->uiIdx,uiLoop,0,0,0);
            if(SRIO_INVALID_SW != vpstSwitch->aucSWPort[uiLoop])
            {
                uiSWIdx = vpstSwitch->aucSWPort[uiLoop];
                vpstSwitch->aucSWPort[uiLoop] = SRIO_INVALID_SW; 
                SRIO_LOG(SRIO_DBG_ERR,"switch %d port %d switch %d link down\n",
                         vpstSwitch->uiIdx,uiLoop,uiSWIdx,0,0);
                rioSwitchDelRoute(vpstSwitch,&vpstMPort->astSwitch[uiSWIdx]);
                vpstMPort->astSwitch[uiSWIdx].uiDiscStatus = SRIO_DISC_FREE;
            }
            else
            {
                SRIO_LOG(SRIO_DBG_ERR,"switch %d port %d device %d status 0x%x"
                         " link down\n",vpstSwitch->uiIdx,
                         uiLoop,pstPort->uiDevId,uiPortStatus,0);
                rioRouteAddEntry(0,RIO_ANY_DEV_ID,vpstSwitch->uiHop,
                           RIO_GLOBAL_TABLE,pstPort->uiDevId,SRIO_INVALID_PORT);
                vpstSwitch->aucRouteTable[pstPort->uiDevId] = SRIO_INVALID_PORT;
            }
        }
		
        if((SRIO_PORT_RECOVER == pstPort->uiStatus) &&
           (RIO_PORT_N_ERR_STS_PORT_OK == uiPortStatus))
        {
            SRIO_LOG(SRIO_DBG_ERR,"switch %d port %d status 0x%x link up\n",
                     vpstSwitch->uiIdx,uiLoop,uiPortStatus,0,0);
			rioPortReset(0,0xff,vpstSwitch->uiHop,uiLoop,vpstSwitch->uiDID);
            if(OK == rioPortRecover(0,RIO_ANY_DEV_ID,vpstSwitch->uiHop,uiLoop))
            {
                *vuiNeedEnum = 1;
            }
        }
		
        if((SRIO_PORT_DOWN == pstPort->uiStatus) &&
           (RIO_PORT_N_ERR_STS_PORT_OK == uiPortStatus))
        {
            SRIO_LOG(SRIO_DBG_ERR,"switch %d port %d 0x%x link up\n",
                     vpstSwitch->uiIdx,uiLoop,uiPortStatus,0,0);
            *vuiNeedEnum = 1;         
        }
    }
}

VOID rioSystemSnoop(VOID)
{
    SRIO_MPORT_S   *pstMPort = NULL;
    SRIO_SWITCH_S  *pstSwitch = NULL;
    SRIO_SWITCH_S  *pstNextSW = NULL;
    UINT32          uiLoop = 0;
    UINT32          uiSWIdx = 0;
    UINT32          uiVal = 0;
    UINT32          uiEnum = 0;

    pstMPort = &gstSrioMport[0];
    if(pstMPort->uiDownSwIdx >= SRIO_MAX_SWITCH)
    {
        return;
    }
    pstSwitch = &pstMPort->astSwitch[pstMPort->uiDownSwIdx];
    while(1)
    {
        taskDelay(30);
        rioSwitchSnoop(pstMPort,pstSwitch,&uiVal);
        uiEnum |= uiVal;
		
        for(uiLoop = 0;uiLoop < SRIO_MAX_PORT_PER_SW;uiLoop++)
        {
            if((SRIO_INVALID_SW == pstSwitch->aucSWPort[uiLoop]))
            {
                continue;
            }
            uiSWIdx = pstSwitch->aucSWPort[uiLoop];
            assert(uiSWIdx < SRIO_MAX_SWITCH);
            pstNextSW = &pstMPort->astSwitch[uiSWIdx];
            rioRouteAddEntry(0,RIO_ANY_DEV_ID,pstSwitch->uiHop,
                             RIO_GLOBAL_TABLE,RIO_ANY_DEV_ID,uiLoop);       
            rioSwitchSnoop(pstMPort,pstNextSW,&uiVal);
            uiEnum |= uiVal;
        }
        
        if(uiEnum)
        {
            taskDelay(1);
            rioGetHostId(0,&uiVal);		
            rioSystemEnumerate(uiVal);
            uiEnum = 0;
        }
    }

}

VOID rioGetRouteInfo(UINT32 localPort,UINT32 uiHop)
{
    SRIO_SWITCH_S  *pstSwitch = NULL;
    SRIO_PORT_S    *pstPort = NULL;
    UINT32          uiLoop = 0;
    UINT32          uiPort = 0;

    printf("master port 0x%x SWMAP 0x%x\n",(UINT32)(&gstSrioMport[0]),
           gstSrioMport[0].uiSWMap);

    pstSwitch = &gstSrioMport[0].astSwitch[0];
    
    for(uiLoop = 0;uiLoop < SRIO_MAX_DEV_ID;uiLoop++)
    {
        rioRouteGetEntry(localPort,RIO_ANY_DEV_ID,uiHop,uiLoop,uiLoop,&uiPort);
        if(SRIO_INVALID_PORT != uiPort)
        {
            printf("dev 0x%x on port %d is up\n",uiLoop,uiPort);
            if((uiLoop < 0x20) && (TRUE == rioIsAgent(0)) &&
               (uiPort < SRIO_MAX_PORT_PER_SW))
            {
                pstSwitch->uiDiscStatus = SRIO_DISCED;
                pstPort = &pstSwitch->astPort[uiPort];
                pstPort->uiStatus = SRIO_PORT_UP;
                pstPort->uiDevId = uiLoop;
            }
        }
    }
}

STATUS rioDevOnLine(UINT32 vuiTgtId)
{
    SRIO_SWITCH_S  *pstSwitch = NULL;
    SRIO_PORT_S    *pstPort = NULL;
    UINT32          uiLoop = 0;
    UINT32          uiPort = 0;
    
    if(FALSE == rioLocDevIsHost(0))
    {
        rioRouteGetEntry(0,RIO_ANY_DEV_ID,0,0xff,vuiTgtId,&uiPort);
        if((SRIO_INVALID_PORT != uiPort) && (uiPort < SRIO_MAX_PORT_PER_SW))
        {
            return OK;
        }
        else
        {
            return ERROR;
        }
        return OK;
    }
    
    for(uiLoop = 0;uiLoop < SRIO_MAX_SWITCH;uiLoop++)
    {
        pstSwitch = &gstSrioMport[0].astSwitch[uiLoop];
        for(uiPort = 0;uiPort < SRIO_MAX_PORT_PER_SW;uiPort++)
        {
            pstPort = &pstSwitch->astPort[uiPort];
            if((SRIO_PORT_UP == pstPort->uiStatus) &&
               (vuiTgtId == pstPort->uiDevId))
            {
                return OK;
            }
        }
    }
    
    return ERROR;
}

STATUS rioDevOnLine1(UINT32 vuiTgtId)
{
    SRIO_SWITCH_S  *pstSwitch = NULL;
    SRIO_PORT_S    *pstPort = NULL;
    UINT32          uiLoop = 0;
    UINT32          uiPort = 0;
    
    if(FALSE == rioLocDevIsHost(0))
    {
        return OK;
    }
    
    for(uiLoop = 0;uiLoop < SRIO_MAX_SWITCH;uiLoop++)
    {
        pstSwitch = &gstSrioMport[0].astSwitch[uiLoop];
        for(uiPort = 0;uiPort < SRIO_MAX_PORT_PER_SW;uiPort++)
        {
            pstPort = &pstSwitch->astPort[uiPort];
            if((SRIO_PORT_UP == pstPort->uiStatus) &&
               (vuiTgtId == pstPort->uiDevId))
            {
                return OK;
            }
        }
    }
    
    return ERROR;
}

UINT32 rioGetDevIdFromAddr(UINT32 uiAddr,UINT32 *puiOffset)
{
	UINT32     uiLoop = 0;
	UINT32     uiEndAddr = 0;
	UINT32     uiStartAddr = 0;
	SRIO_OW_S *pstOW = NULL;

	for(uiLoop = 0;uiLoop < SRIO_MAX_OUB;uiLoop++)
	{
		pstOW = &gstSrioMport[0].astOW[uiLoop]; 
		uiStartAddr = pstOW->uiMemLoc;
		uiEndAddr = pstOW->uiSize * 0x100000 + uiStartAddr;

		if((uiAddr >= uiStartAddr) && (uiAddr < uiEndAddr))
		{
		    *puiOffset = uiAddr - uiStartAddr + pstOW->uiMemTran;
		    return pstOW->uiDevId;
		}
	}

	return ERROR;
}

UINT32 rioDeviceNum(UINT32 p[])
{
    SRIO_SWITCH_S  *pstSwitch = NULL;
    SRIO_PORT_S    *pstPort = NULL;
    UINT32         *puiTgtId = NULL;
    UINT32          uiLoop = 0;
    UINT32          uiPort = 0;
    UINT32          uiNum = 0;

    puiTgtId = p;
    if(FALSE == rioLocDevIsHost(0))
    {
        for(uiLoop = 0;uiLoop < SRIO_MAX_DEV_ID;uiLoop++)
        {
            if(OK == rioDevOnLine(uiLoop))
            {
                *puiTgtId = uiLoop;
                puiTgtId++;
                uiNum++;
            }
        }

        return uiNum;
    }
    
    for(uiLoop = 0;uiLoop < SRIO_MAX_SWITCH;uiLoop++)
    {
        pstSwitch = &gstSrioMport[0].astSwitch[uiLoop];
        for(uiPort = 0;uiPort < SRIO_MAX_PORT_PER_SW;uiPort++)
        {
            pstPort = &pstSwitch->astPort[uiPort];
            if(SRIO_PORT_UP == pstPort->uiStatus)
            {
                *puiTgtId = pstPort->uiDevId;
                puiTgtId++;
                uiNum++;
            }
        }
    }
    
    return uiNum;
}

UINT32 rioSetBaseDeviceId(UINT8 localPort,UINT16 dstPortID,UINT8 hopCount,
                          UINT32 newDevID)
{
    SRIO_SWITCH_S  *pstSwitch = NULL;
    UINT32          uiHostId = 0xe;
    UINT32          uiVal = 0;
    
    if(0 != hopCount)
    {
        printf("hop %d is not support\n",hopCount);
        return ERROR;
    }
    if(0xffff == dstPortID)
    {
        rioSetDeviceId(0,RIO_ANY_DEV_ID,RIO_HOP_LOCAL,newDevID);
        return OK;
    }
    if(dstPortID >= SRIO_MAX_PORT_PER_SW)
    {
        printf("destination port %d not exist\n",dstPortID);
        return ERROR;
    }
    if(TRUE != guiSrioInit)
    {
        rioSetup();
    }
    if(SRIO_INIT_HOSTID == gstSrioMport[0].uiHostId)
    {
        rioSetHostId(0,uiHostId);
        gstSrioMport[0].uiHostId = uiHostId;
    }
    pstSwitch = rioGetSWitch(&gstSrioMport[0],0);
    if(NULL == pstSwitch)
    {
        return ERROR;
    }
    
    if(SRIO_NEW_ALLOC == pstSwitch->uiNewAlloc)
    {
        pstSwitch->uiUpSWIdx = SRIO_MASTER_SW_IDX;
        pstSwitch->uiUpPort = rioGetHostPort(0,RIO_ANY_DEV_ID,0);
        uiHostId = gstSrioMport[0].uiHostId;
        pstSwitch->astPort[pstSwitch->uiUpPort].uiDevId = uiHostId;
        pstSwitch->uiHop = 0;
    }

    if(dstPortID == pstSwitch->uiUpPort)
    {
        printf("port %d is host port\n",dstPortID);
        return ERROR;
    }
    
    rioRouteGetEntry(0,RIO_ANY_DEV_ID,0,RIO_GLOBAL_TABLE,uiHostId,&uiVal);
    if(SRIO_INVALID_PORT == uiVal)
    {
        rioRouteAddEntry(0,RIO_ANY_DEV_ID,0,RIO_GLOBAL_TABLE,uiHostId,
                         pstSwitch->uiUpPort);
    }
        
    rioGetPortErrStatus(0,RIO_ANY_DEV_ID,0,0,dstPortID,&uiVal);
    if(0 == (uiVal & RIO_PORT_N_ERR_STS_PORT_OK))
    {
        printf("port %d is down\n",dstPortID);
        return ERROR;
    }
    
    uiVal = pstSwitch->astPort[dstPortID].uiDevId;
    if((uiVal >= RIO_ANY_DEV_ID) || (0x0 == uiVal))
    {
        rioRouteAddEntry(0,RIO_ANY_DEV_ID,0,RIO_GLOBAL_TABLE,RIO_ANY_DEV_ID,
                         dstPortID);
        uiVal = RIO_ANY_DEV_ID;                 
    }
    rioSetDeviceId(0,uiVal,1,newDevID);
    rioRouteAddEntry(0,RIO_ANY_DEV_ID,0,RIO_GLOBAL_TABLE,newDevID,dstPortID);
    pstSwitch->aucRouteTable[newDevID] = dstPortID;
    
    return OK;
}


VOID rioDBGDisplay(VOID)
{
    UINT32  uiLoop = 0;

    printf("master port 0x%x\n",(UINT32)(&gstSrioMport[0]));

    printf("-----------inbound------------\n");
    for(uiLoop = 0; uiLoop < 16; uiLoop++)
    {
        printf("0x%-19x0x%-16x%-12d\n",
                  gstSrioMport[0].astIW[uiLoop].uiMemLoc,
                  gstSrioMport[0].astIW[uiLoop].uiMemTran,
                  gstSrioMport[0].astIW[uiLoop].uiSize);
    }

    printf("-----------outbound------------\n");
    for(uiLoop = 0; uiLoop < 16; uiLoop++)
    {
        printf("0x%-8x    0x%-19x0x%-16x%-12d\n",
                  gstSrioMport[0].astOW[uiLoop].uiDevId,
                  gstSrioMport[0].astOW[uiLoop].uiMemLoc,
                  gstSrioMport[0].astOW[uiLoop].uiMemTran,
                  gstSrioMport[0].astOW[uiLoop].uiSize);
    }
}

UINT32 rioGetDevOWWin(UINT32 vuiTgtId)
{
    UINT32     uiLoop = 0;
    SRIO_OW_S *pstOW = NULL;

    for(uiLoop = 0;uiLoop < SRIO_MAX_OUB;uiLoop++)
    {
        pstOW = &gstSrioMport[0].astOW[uiLoop]; 
        if(vuiTgtId == pstOW->uiDevId)
        {
            return uiLoop;
        }
    }
    
    return ERROR;
}

UINT32 rioGetDIDWin(UINT32 vuiDID)
{
    UINT32     uiLoop = 0;
    UINT32     uiDIDNum = 0;

    uiDIDNum = sizeof(gastDIDCAR) / sizeof(SRIO_DID_S);
    
    for(uiLoop = 0;uiLoop < uiDIDNum;uiLoop++)
    { 
        if(vuiDID == gastDIDCAR[uiLoop].uiDevId)
        {
            return uiLoop;
        }
    }

    
    return ERROR;
}

VOID rioSystemDisplay(VOID)
{
    SRIO_SWITCH_S  *pstSwitch = NULL;
    SRIO_PORT_S    *pstPort = NULL;
    UINT32          uiOWWin = ERROR;
    UINT32          uiDID = ERROR;
    UINT32          uiLoop = 0;
    UINT32          uiPort = 0;
    UINT32          uiSWIdx = 0;

	if(FALSE == guiSrioInit)
	{
		printf("rio is not initialized\n");
		return;
	}

    printf("switch   port    type      id      start address"
               "    tran address   size(MB)      status\n");
    for(uiLoop = 0;uiLoop < SRIO_MAX_SWITCH;uiLoop++)
    {
        pstSwitch = &gstSrioMport[0].astSwitch[uiLoop];
        if(SRIO_DISCED == pstSwitch->uiDiscStatus)
        {
            for(uiPort = 0;uiPort < SRIO_MAX_PORT_PER_SW;uiPort++)
            {
                pstPort = &pstSwitch->astPort[uiPort];
                if(SRIO_PORT_DOWN == pstPort->uiStatus)
                {
                    continue;
                }
                uiOWWin = rioGetDevOWWin(pstPort->uiDevId);
                uiDID = rioGetDIDWin(pstPort->uiDevType);
                printf("%1d       %2d      ",pstSwitch->uiIdx,uiPort);
                if(ERROR == uiDID)
                {
                    printf(" %-8s  ","unknown");
                }
                else
                {
                    printf(" %-8s  ",gastDIDCAR[uiDID].aucName);
                }
                
                if((SRIO_INVALID_SW != pstSwitch->aucSWPort[uiPort]) &&
                   (FALSE == rioIsAgent(0))) 
                {    /*switch port*/
                    uiSWIdx = (UINT32)pstSwitch->aucSWPort[uiPort];
                    assert(uiSWIdx < SRIO_MAX_SWITCH);
                    printf("0x%-2x    0x%-8x       0x%8x     0x%2x",
                            uiSWIdx,-1,-1,0);
                    printf("          %1d\n",pstPort->uiStatus);
                    continue;
                }
                
                if(ERROR == uiOWWin)
                {
                    printf("0x%-2x    0x%-8x       0x%8x     0x%2x",
                            pstPort->uiDevId,-1,-1,0);
                }
                else
                {
                    printf("0x%-2x    0x%-8x       0x%8x     0x%2x",
                            gstSrioMport[0].astOW[uiOWWin].uiDevId,
                            gstSrioMport[0].astOW[uiOWWin].uiMemLoc,
                            gstSrioMport[0].astOW[uiOWWin].uiMemTran,
                            gstSrioMport[0].astOW[uiOWWin].uiSize);        
                }
                printf("          %1d\n",pstPort->uiStatus);
            }
            printf("\n");
        }  
    }
}

void  rioDeviceNumPrint(void)
{
    UINT32  uiOWWin = 0;
    UINT32  uiMaxDev = 0;
    UINT32 *puiDev = NULL;
    UINT32  uiLoop = 0;

    puiDev = malloc(SRIO_MAX_DEV_ID * sizeof(UINT32));
    if(NULL == puiDev)
    {
        logMsg("malloc device print buf failed\n",0,0,0,0,0,0);
        return ;
    }
    uiMaxDev = rioDeviceNum(puiDev);
    printf("id      start address    tran address   size(MB)\n");
    for(uiLoop = 0;uiLoop < uiMaxDev;uiLoop++)
    {
        uiOWWin = rioGetDevOWWin(puiDev[uiLoop]);
        if(ERROR == uiOWWin)
        {
            logMsg("0x%-2x    0x%-8x       0x%8x     0x%2x\n",
                      puiDev[uiLoop],-1,-1,0,0,0);
        }
        else
        {
           logMsg("0x%-2x    0x%-8x       0x%8x     0x%2x\n",
                      gstSrioMport[0].astOW[uiOWWin].uiDevId,
                      gstSrioMport[0].astOW[uiOWWin].uiMemLoc,
                      gstSrioMport[0].astOW[uiOWWin].uiMemTran,
                      gstSrioMport[0].astOW[uiOWWin].uiSize, 0, 0);        
        }
    }

    free(puiDev);
}

