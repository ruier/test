/******************************************************************************

  Copyright (C), 2001-2011, PROPHET ELECTRONIC TECHNOLOGY Co., Ltd.

 ******************************************************************************
  File Name     : rioHal.c
  Version       : Initial Draft
  Author        : lighuohui
  date          : 2013.1.24
  modify        :
  function      : provide a unified software interface to configuration
                  registers in both local and remote RapidIO processing
                  elements
******************************************************************************/

#include <vxWorks.h>
#include <string.h>
#include <stdlib.h>
#include <intLib.h>
#include <semLib.h>
#include "assert.h"
#include "rioHal.h"
#include "rioDrv.h"

/*----------------------------------------------*
 * external variables                           *
 *----------------------------------------------*/
extern SRIO_MPORT_S  gstSrioMport[SRIO_MAX_MPORT];

/*----------------------------------------------*
 * external routine prototypes                  *
 *----------------------------------------------*/
extern VOID rioCfgWrite(UINT32 ,UINT32,UINT32,UINT32,UINT32);
extern VOID rioCfgRead(UINT32 ,UINT32 ,UINT32 ,UINT32 ,UINT32 *);
/*----------------------------------------------*
 * internal routine prototypes                  *
 *----------------------------------------------*/

/*----------------------------------------------*
 * project-wide global variables                *
 *----------------------------------------------*/

/*----------------------------------------------*
 * module-wide global variables                 *
 *----------------------------------------------*/

/*----------------------------------------------*
 * constants                                    *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macros                                       *
 *----------------------------------------------*/

/*****************************************************************************
 func   : rioGetNumLocalPorts
 desc   : returns total number of local RapidIO Ports supported by HAL
               functions
 return : 0: error 
          n:number of RapidIO ports supported
*****************************************************************************/
UINT32 rioGetNumLocalPorts(VOID)
{
    return 1;
}

/*****************************************************************************
 func : rioInitLib
 desc : initialize library
 parm : none
*****************************************************************************/
STATUS rioInitLib(VOID)
{
	return RIO_SUCCESS;
}

/*****************************************************************************
 func : rioGetFeatures
 desc : read from the Processing Element Features CAR of the specified 
        processing element
 parm : localport: Local port number [IN]
        destid:    Destination ID of the processing element [IN]
        hopcount:  Hop count [IN]
        *features: Pointer to storage containing the received features[OUT]
*****************************************************************************/
VOID rioGetFeatures(UINT32 localPort,UINT32 destId,UINT32 hopCnt,UINT32 *feature)

{

}

/*****************************************************************************
 func : rioGetSwitchPortInfo
 desc : read from the Switch Port Information CAR of the specified processing 
        element
 parm : localport: Local port number [IN]
        destid:    Destination ID of the processing element [IN]
        hopcount:  Hop count [IN]
        *extfptr:  Pointer to storage containing the received extended feature 
                   information [OUT]
*****************************************************************************/
VOID rioGetSwitchPortInfo(UINT32 localPort,UINT32 destId,UINT32 hopCnt,
                          UINT32 *portInfo)
{
    rioCfgRead(localPort,destId,hopCnt,RIO_SWP_INFO_CAR,portInfo);
}

/*****************************************************************************
 func : rioGetSrcOps
 desc : read from the Source Operations CAR of the specified processing element
 parm : localport: Local port number [IN]
        destid:    Destination ID of the processing element [IN]
        hopcount:  Hop count [IN]
        *srcops:   Pointer to storage containing the received source
                   operation information [OUT]
*****************************************************************************/
VOID rioGetSrcOps (UINT32 localPort,UINT32 destId,UINT32 hopCnt,UINT32 *srcOps)
{
    rioCfgRead(localPort,destId,hopCnt,RIO_SRC_OPS_CAR,srcOps);
}

/*****************************************************************************
 func : rioGetDestOps
 desc : read from the Destination Operations CAR of the specified processing 
        element
 parm : localport: Local port number [IN]
        destid:    Destination ID of the processing element [IN]
        hopcount:  Hop count [IN]
        *destops:  Pointer to storage containing the received source
                   operation information [OUT]
*****************************************************************************/
VOID rioGetDestOps (UINT32 localPort,UINT32 destId,UINT32 hopCnt,UINT32 *destOps)
{
    rioCfgRead(localPort,destId,hopCnt,RIO_DST_OPS_CAR,destOps);
}

/*****************************************************************************
 func : rioGetBaseDeviceId
 desc : read from the bas device ID CSR of the local processing element
 parm : localport: Local port number [IN]
        *devid:    Pointer to storage containing the base device ID [OUT]
*****************************************************************************/
VOID rioGetLocDeviceId (UINT32 localPort,UINT32 *devId)
{
    rioCfgRead(localPort,0,RIO_HOP_LOCAL,RIO_DID_CSR,devId);
}

/*****************************************************************************
 func : rioSetDeviceId
 desc : write the base device ID in the base device ID CSR of the specified 
        processing element
 parm : localport: Local port number [IN]
        destPort:  Destination PORT of the processing element [IN]
        hopcount:  Hop count [IN]
        newid:     New base device ID to be set [IN]
*****************************************************************************/
VOID rioSetDeviceId (UINT32 localPort,UINT32 destId,UINT32 hopCnt,
                           UINT32 newId)
{
    rioCfgWrite(localPort,destId,hopCnt,RIO_DID_CSR,(newId << 16));
}

/*****************************************************************************
 func : rioSetRemoteDevFound
 desc : set remote device master bit and discovered bit
 parm : localport: Local port number [IN]
        destPort:  Destination PORT of the processing element [IN]
        hopcount:  Hop count [IN]
*****************************************************************************/
VOID rioSetRemoteDevFound(UINT32 localPort,UINT32 destId,UINT32 hopCnt)
{
    rioCfgWrite(localPort,destId,hopCnt,SRIO_CSR,SRIO_CSR_M_D);
}

/*****************************************************************************
 func : rioAcquireDevLock
 desc : acquire the hardware device lock for the specified processing element
        on behalf of the requesting host
 parm : localport:  Local port number [IN]
        destid:     Destination ID of the processing element [IN]
        hopcount:   Hop count [IN]
        hostdevid:  Host base device ID for the local processing element [IN]
        *hostlockid:Device ID of the host holding the lock if ERR_LOCK is 
                    returned [OUT]
*****************************************************************************/
STATUS rioAcquireDevLock(UINT32 localPort,UINT32 destId,UINT32 hopCnt,
                         UINT32 hostDevId,UINT32 *hostLockId)
{

    UINT32 uiVal = 0;

    rioCfgRead(localPort,destId,hopCnt,RIO_HOST_DID_LOCK_CSR,&uiVal);
    *hostLockId = uiVal;
    if(hostDevId == uiVal)
    {
        return RIO_SUCCESS;
    }
    rioCfgWrite(localPort,destId,hopCnt,RIO_HOST_DID_LOCK_CSR,hostDevId);
    rioCfgRead(localPort,destId,hopCnt,RIO_HOST_DID_LOCK_CSR,&uiVal);
    if(uiVal != hostDevId)
    {
        SRIO_LOG(SRIO_DBG_ERR,"device locked by host %d hostid %d\n",uiVal,
                  hostDevId,0,0,0);
        return RIO_ERROR;
    }
   
    return RIO_SUCCESS;
}

/*****************************************************************************
 func : rioReleaseDevLock
 desc : acquire the hardware device lock for the specified processing element
        on behalf of the requesting host
 parm : localport:  Local port number [IN]
        destid:     Destination ID of the processing element [IN]
        hopcount:   Hop count [IN]
        hostdeviid: Host base device ID for the local processing element [IN]
        *hostlockid:Device ID of the host holding the lock if ERR_LOCK is 
                    returned [OUT]
*****************************************************************************/
STATUS rioReleaseDevLock(UINT32 localPort,UINT32 destId,UINT32 hopCnt,
                         UINT32 hostdevid,UINT32 *hostlockid)
{
    UINT32 uiVal = 0;

    rioCfgRead(localPort,destId,hopCnt,RIO_HOST_DID_LOCK_CSR,&uiVal);
    *hostlockid = uiVal;
    if((hostdevid != uiVal) && (0xffff != (uiVal & 0xffff)))
    {
        SRIO_LOG(SRIO_DBG_ERR,"device has locked by other host %d\n",
                 uiVal,0,0,0,0);
        return RIO_ERROR;
    }
    rioCfgWrite(localPort,destId,hopCnt,RIO_HOST_DID_LOCK_CSR,hostdevid);
    rioCfgRead(localPort,destId,hopCnt,RIO_HOST_DID_LOCK_CSR,&uiVal);
    if(0xffff != (uiVal & 0xffff))
    {
        return RIO_ERROR;
    }
    
    return RIO_SUCCESS;
}

/*****************************************************************************
 func : rioGetComponentTag
 desc : read from the component tag CSR of the specified processing element
 parm : localport: Local port number [IN]
        destid:    Destination ID of the processing element [IN]
        hopcount:  Hop count [IN]
        *comptag:  Pointer to storage containing the received component tag 
                   information [OUT]
*****************************************************************************/
VOID rioGetCompTag(UINT32 localPort,UINT32 destId,UINT32 hopCnt,UINT32 *compTag)
{
    rioCfgRead(localPort,destId,hopCnt,RIO_COMPONENT_TAG_CSR,compTag);
}

/*****************************************************************************
 func : rioSetCompTag
 desc : write from the component tag CSR of the specified processing element
 parm : localport: Local port number [IN]
        destid:    Destination ID of the processing element [IN]
        hopcount:  Hop count [IN]
        comptag:   Pointer to storage containing the received component tag 
                   information [IN]
*****************************************************************************/
VOID rioSetCompTag(UINT8 localPort,UINT32 destId,UINT8 hopCnt,UINT32 compTag)
{
    rioCfgWrite(localPort,destId,hopCnt,RIO_COMPONENT_TAG_CSR,compTag);
}

/*****************************************************************************
 func : rioGetPortErrStatus
 desc : read the contents of the port n error and status CSR of the specified 
        processing element
 parm : localport:     Local port number [IN]
        destid:        Destination ID of the processing element [IN]
        hopcount:      Hop count [IN]
        extfoffset:    Offset from the previously reported extended features 
                       pointer [IN]
        portnum:       Port number to be accessed [IN]
        *porterrtatus: Pointer to storage for the returned value [OUT]
*****************************************************************************/
VOID rioGetPortErrStatus(UINT32 localPort,UINT32 destId,UINT32 hopCnt,
                           UINT32 extOffset,UINT32 portNum,UINT32 *portErrStatus)
{
    UINT32 uiOffset = 0;

    uiOffset = RIO_EXTENDED_FEATURE_PTR + RIO_PORT_N_ERR_STS_CSR(portNum);
    rioCfgRead(localPort,destId,hopCnt,uiOffset,portErrStatus);
}

/*****************************************************************************
 func : rioIsSwitch
 desc : get the information whether device is switch or not 
 parm : localport:   Local port number (RapidIO switch) [IN]
        destid:      Destination ID of the processing element(RapidIO switch)[IN]
        hopcount:    Hop count [IN]
*****************************************************************************/
BOOL rioIsSwitch(UINT32 localPort,UINT32 destId,UINT32 hopCount)
{
	UINT32 uiVal = 0;

    rioCfgRead(localPort,destId,hopCount,RIO_PEF_CAR,&uiVal);
    
	if(uiVal & RIO_PEF_SWITCH)
	{
        return TRUE;
	}
	
	return FALSE;
}

/*****************************************************************************
 func : rioDevHasDestId
 desc : Test if a device contains a destination ID register
        Checks the provided @src_ops and @dst_ops for the necessary transaction
        capabilities that indicate whether or not a device will implement a
        destination ID register. Returns 1 if true or 0 if false.
*****************************************************************************/
BOOL rioDevHasDestId(UINT32 localPort,UINT32 destId,UINT32 hopCnt)
{
	UINT32  uiMask = 0;
	UINT32  uiSrcOps = 0;
    UINT32  uiDestOps = 0;

	uiMask |= RIO_OPS_ATOMIC_CLR | RIO_OPS_ATOMIC_SET | RIO_OPS_ATOMIC_TST_SWP;
	uiMask |= RIO_OPS_ATOMIC_INC | RIO_OPS_ATOMIC_DEC | RIO_OPS_WRITE | RIO_OPS_READ;
    rioGetSrcOps(localPort,destId,hopCnt,&uiSrcOps);
    rioGetDestOps(localPort,destId,hopCnt,&uiDestOps);
    if(0 != ((uiSrcOps | uiDestOps) & uiMask))
    {
        return TRUE;
    }

	return FALSE;
}

/*****************************************************************************
 func : rioRouteAddEntry
 desc : adds an entry to a routing table for the RapidIO switch specified by the
        destid and hopcount parameters
 parm : localport:   Local port number (RapidIO switch) [IN]
        destid:      Destination ID of the processing element(RapidIO switch)[IN]
        hopcount:    Hop count [IN]
        tableidx:    Routing table index for per-port switch implementations [IN]
        routedestid: Route destination ID—used to select an entry into the 
                     specified routing table [IN]
        routeportno: Route port number—value written to the selected routing 
                     table entry [IN]
*****************************************************************************/
VOID rioRouteAddEntry(UINT32 localPort,UINT32 destId,UINT32 hopCnt,
                      UINT32 tableIdx,UINT32 routeDestId,UINT32 routePortNo)
{
    UINT32 uiDestId = 0;
    UINT32 uiCfgPort = 0;
    
    SRIO_LOG(SRIO_DBG_INFO,"route add entry destid 0x%x port 0x%x hop %d\n",
             routeDestId,routePortNo,hopCnt,0,0);

    rioCfgWrite(localPort,destId,hopCnt,SPBC_ROUTE_CFG_DESTID,(routeDestId & 0xff));
    
    while(uiDestId != routeDestId)
    {
        rioCfgRead(localPort,destId,hopCnt,SPBC_ROUTE_CFG_DESTID,
                         &uiDestId); 
			
		}
	

    rioCfgWrite(localPort,destId,hopCnt,SPBC_ROUTE_CFG_PORT,routePortNo);
    while(uiCfgPort != routePortNo)
    {
        rioCfgRead(localPort,destId,hopCnt,SPBC_ROUTE_CFG_PORT,
                         &uiCfgPort);  
			
    }

	taskDelay(1);
}

/*****************************************************************************
 func : rioRouteGetEntry
 desc : reads an entry from a routing table for the RapidIO switch specified by 
        the destid and hopcount parameters
 parm : localport:   Local port number (RapidIO switch) [IN]
        destid:      Destination ID of the processing element(RapidIO switch)[IN]
        hopcount:    Hop count [IN]
        tableidx:    Routing table index for per-port switch implementations [IN]
        routedestid: Route destination ID—used to select an entry into the 
                     specified routing table [IN]
        routeportno: Route port number—pointer to value read from the selected 
                     routing table entry [OUT]
*****************************************************************************/
STATUS rioRouteGetEntry(UINT32 localPort,UINT32 destId,UINT32 hopCnt,
                        UINT32 tableIdx,UINT32 routeDestId,UINT32 *routePortNo)
{
    UINT32 uiDestId = 0;
	UINT32 uiLoop = 0;

	for(uiLoop = 0;uiLoop < 1000;uiLoop++)
    {
    	rioCfgWrite(localPort,destId,hopCnt,SPBC_ROUTE_CFG_DESTID,routeDestId);
		taskDelay(1);
        rioCfgRead(localPort,destId,hopCnt,SPBC_ROUTE_CFG_DESTID,&uiDestId);
		if(uiDestId == routeDestId)
		{
			break;
		}
    }
	if(1000 == uiLoop)
	{
		logMsg("read switch hop %d failed\n",hopCnt,0,0,0,0,0);
	}
    rioCfgRead(localPort,destId,hopCnt,SPBC_ROUTE_CFG_PORT,routePortNo);
   
	return RIO_SUCCESS;
}

/*****************************************************************************
 func : rioCheckAccess
 desc : check device is access
 parm : localport:   Local port number (RapidIO switch) [IN]
        destid:      Destination ID of the processing element(RapidIO switch)[IN]
        hopcount:    Hop count [IN]
*****************************************************************************/
STATUS rioCheckAccess(UINT32 localPort,UINT32 destid,UINT32 hopcount)
{
    UINT32 uiVal = 0;
    
    rioCfgRead(localPort,destid,hopcount,RIO_DEV_ID_CAR,&uiVal);
    if(0 != uiVal)
    {
        return OK;
    }
    
	return ERROR;
}

/*****************************************************************************
 func : rioClrSwitchRoute
 desc : clear the route table of switch 
 parm : localport:   Local port number (RapidIO switch) [IN]
        destid:      Destination ID of the processing element(RapidIO switch)[IN]
        hopcount:    Hop count [IN]
*****************************************************************************/
VOID rioClrSwitchRoute(UINT32 localPort,UINT32 destId,UINT32 hopCount)
{
	UINT32    uiLoop = 0;

    for(uiLoop = 0;uiLoop < SRIO_MAX_DEV_NUM;uiLoop++)
    {
        rioCfgWrite(localPort,0xff,hopCount,SPBC_ROUTE_CFG_DESTID,uiLoop);
        rioCfgWrite(localPort,0xff,hopCount,SPBC_ROUTE_CFG_PORT,0xff);
    }

    taskDelay(2);
}

STATUS rioSetPortSpeed(UINT32 localPort,UINT32 destId,UINT32 hopCnt,
                     UINT32 portNo, UINT32 portSpeed)
{
	UINT32 uiVal = 0;
	UINT32 uiPllNum = 0;
	UINT32 uiLineNum = 0;
	UINT32 uiLoop = 0;
	if ((RIO_PORT_SPEED125 != portSpeed) && (RIO_PORT_SPEED25 != portSpeed) 
									  && (RIO_PORT_SPEED3125 != portSpeed))
	{
		SRIO_LOG(SRIO_DBG_ERR, "the input speed is error! \n", 0,0,0,0,0);
		return ERROR;
	}

	rioCfgRead(localPort,destId,hopCnt,RIO_1848_CTL1_CSR(portNo), &uiVal);
	uiVal |= 0x00800000;
	rioCfgWrite(localPort,destId,hopCnt,RIO_1848_CTL1_CSR(portNo), uiVal);
	uiVal = 0;
	taskDelay(1);
	uiPllNum = portNo;
	if (uiPllNum >= 12)
	{
		uiPllNum -= 12; 
	}
    
	rioCfgRead(localPort,destId,hopCnt, RIO_1848_PLL_CTL1_REG(uiPllNum), &uiVal);
	if (RIO_PORT_SPEED3125 == portSpeed)
	{
		uiVal |= 0x1;
	}
	else
	{
		uiVal = uiVal & (~0x1);	
	}
   
	rioCfgWrite(localPort,destId,hopCnt, RIO_1848_PLL_CTL1_REG(uiPllNum), uiVal);
	uiVal = 0;
	uiLineNum = portNo * 4;
   
	rioCfgRead(localPort,destId,hopCnt, RIO_1848_LANE_CTL_REG(uiLineNum), &uiVal);
	uiVal = uiVal & (~0x1e);
	if ((RIO_PORT_SPEED25 == portSpeed) || (RIO_PORT_SPEED3125 == portSpeed))
	{
		uiVal |= 0xa;
	}
     
	for (uiLoop=0; uiLoop<4; uiLoop++)
	{
		rioCfgWrite(localPort,destId,hopCnt,
					RIO_1848_LANE_CTL_REG(uiLineNum + uiLoop), uiVal);		
	}
	rioCfgWrite(localPort,destId,hopCnt, RIO_1848_DEVICE_RESET_CTL_REG, 
		(0x80000000 | (1 << (uiPllNum + 18)) | (1 << portNo)));	   
	uiVal = 0;
	rioCfgRead(localPort,destId,hopCnt,RIO_1848_CTL1_CSR(portNo), &uiVal);
	uiVal = uiVal & (~0x00800000);
	rioCfgWrite(localPort,destId,hopCnt,RIO_1848_CTL1_CSR(portNo), uiVal);
	return OK;	
}



INT32 rioGetPortSpeed(UINT32 localPort,UINT32 destId,UINT32 hopCnt,
                     UINT32 portNo)
{
    UINT32 uiVal = 0;
	UINT32 uiPllNum = 0;
	UINT32 uiLineNum = 0;
	UINT32 uiSpeedSelect = 0;
	

	uiPllNum = portNo;
	if (uiPllNum >= 12)
	{
		uiPllNum -= 12; 
	}
    
	rioCfgRead(localPort,destId,hopCnt, RIO_1848_PLL_CTL1_REG(uiPllNum), &uiVal);
	uiSpeedSelect = uiVal & 0x1;
	
	uiVal = 0;
	uiLineNum = portNo * 4;
   
	rioCfgRead(localPort,destId,hopCnt, RIO_1848_LANE_CTL_REG(uiLineNum), &uiVal);
	uiVal = ((uiVal & 0x6) >> 1);
	if(0 == uiSpeedSelect)
	{
        if(0 == uiVal)
        {
            return RIO_PORT_SPEED125;
        }
        else if(1 == uiVal)
        {
            return RIO_PORT_SPEED25;
        }
        else
        {
            return RIO_PORT_SPEED5;
        }
	}

	if(1 == uiSpeedSelect)
	{
	    if(0 == uiVal)
        {
            return ERROR;
        }
        else if(1 == uiVal)
        {
            return RIO_PORT_SPEED3125;   
        }
        else
        {
             return RIO_PORT_SPEED625;
        }

	}
     
	return ERROR;	
}


/*****************************************************************************
 func : rioSwitchInit
 desc : switch initization 
 parm : localport:   Local port number (RapidIO switch) [IN]
        destid:      Destination ID of the processing element(RapidIO switch)[IN]
        hopcount:    Hop count [IN]
*****************************************************************************/
VOID rioSwitchInit(UINT32 localPort,UINT32 destId,UINT32 hopCnt)
{
    UINT32 uiOffset = 0;
    UINT32 uiVal = 0;
    UINT32 uiLoop = 0;
    UINT32 uiPortNum = 0;

    uiOffset = RIO_EXTENDED_FEATURE_PTR + RIO_PORT_LINKTO_CTL_CSR;
    rioCfgWrite(localPort,destId,hopCnt,uiOffset,(0x9a << 8));
    rioClrSwitchRoute(localPort,destId,hopCnt);
    rioGetSwitchPortInfo(localPort,RIO_ANY_DEV_ID,hopCnt,&uiVal);
    uiPortNum = (uiVal >> 8) & 0xff;

    /*enable switch output and input*/
    for(uiLoop = 0;uiLoop < uiPortNum;uiLoop++)
    {
        rioCfgRead(localPort,0xff,hopCnt,RIO_SP_ERR_CTRL(uiLoop),&uiVal);
        uiVal |= ((1 << 21) | (1 << 22));
        rioCfgWrite(localPort,0xff,hopCnt,RIO_SP_ERR_CTRL(uiLoop),uiVal);
    }

    #if 0
	/*交换接口速度设置*/
    if(hopCnt == 0)
    {
        rioSetPortSpeed(localPort, 0xff, 0, 0,  RIO_PORT_SPEED3125);
        rioSetPortSpeed(localPort, 0xff, 0, 1,  RIO_PORT_SPEED3125);
        rioSetPortSpeed(localPort, 0xff, 0, 2,  RIO_PORT_SPEED3125);
        rioSetPortSpeed(localPort, 0xff, 0, 3,  RIO_PORT_SPEED3125);
        rioSetPortSpeed(localPort, 0xff, 0, 4,  RIO_PORT_SPEED3125);
        
        rioSetPortSpeed(localPort, 0xff, 0, 6,  RIO_PORT_SPEED3125);
        rioSetPortSpeed(localPort, 0xff, 0, 7,  RIO_PORT_SPEED3125);
        rioSetPortSpeed(localPort, 0xff, 0, 8,  RIO_PORT_SPEED3125);
        rioSetPortSpeed(localPort, 0xff, 0, 9,  RIO_PORT_SPEED3125);
        rioSetPortSpeed(localPort, 0xff, 0, 10, RIO_PORT_SPEED3125);
        rioSetPortSpeed(localPort, 0xff, 0, 11, RIO_PORT_SPEED3125);
    }
	#endif
    
    /*
   *FPGA SWAP LANE
   */
    /*rioCfgRead(localPort,destId,hopCnt,0x13cc8,&uiVal);
    uiVal |= (0xc0);
    rioCfgWrite(localPort,destId,hopCnt,0x13cc8,uiVal); 
    rioCfgWrite(localPort,destId,hopCnt,0x13ec8,uiVal);*/

}

/*****************************************************************************
 func : rioGetAckID
 desc : get the ack id 
 parm : localport:   Local port number (RapidIO switch) [IN]
        destid:      Destination ID of the processing element(RapidIO switch)[IN]
        hopcount:    Hop count [IN]
        puiVal:      ackid
*****************************************************************************/
STATUS rioGetAckID(UINT32 localPort,UINT32 destId,UINT32 hopCnt,UINT32 portNo,
                   UINT32 *puiVal)
{
    UINT32   uiOffset = 0;
    UINT32   uiVal = 0;
    UINT32   uiLoop = 0;

    for(uiLoop = 0;uiLoop < RIO_REQ_MAX_RETRY;uiLoop++)
    {
        uiOffset = RIO_PORT_N_MNT_REQ_CSR(portNo) + RIO_EXTENDED_FEATURE_PTR;
        rioCfgWrite(localPort,destId,hopCnt,uiOffset,RIO_MNT_REQ_CMD_IS);
        uiOffset = RIO_PORT_N_MNT_RSP_CSR(portNo) + RIO_EXTENDED_FEATURE_PTR;
        rioCfgRead(localPort,destId,hopCnt,uiOffset,&uiVal);
        if(0x80000010 == (uiVal & 0x80000010))
        {
            break;
        }
        taskDelay(1);
    }
    if(RIO_REQ_MAX_RETRY == uiLoop)
    {
        SRIO_LOG(SRIO_DBG_ERR,"check ack hop %d port %d resp failed\n",hopCnt,
                 portNo,0,0,0);
        return ERROR;
    }

	*puiVal = uiVal;
    SRIO_LOG(SRIO_DBG_INFO,"get hop %d port %d ack 0x%x ok\n",hopCnt,
             portNo,uiVal,0,0);
             
    return OK;         
}

/*****************************************************************************
 func : rioSyncAckID
 desc : synchroniz local outbound ack id with peer inbound
        synchroniz local inbound ack id with peer outbound
 parm : localport:   Local port number (RapidIO switch) [IN]
        destid:      Destination ID of the processing element(RapidIO switch)[IN]
        hopcount:    Hop count [IN]
*****************************************************************************/
STATUS rioSyncAckID(UINT32 localPort,UINT32 destId,UINT32 hopCnt,
                    UINT32 portNo,UINT32 uiPeerPort)
{
	UINT32	 uiOurAckId = 0;
	UINT32	 uiFarAckId = 0;
	UINT32	 uiVal = 0;
	
	/*
	 *sets the PORT_LOCKOUT bit in order to flush the ingress and egress buffers
	 *in the port 
	*/
	rioCfgRead(localPort,destId,hopCnt,RIO_SP_ERR_CTRL(portNo),&uiVal);
	uiVal |= 0x2;
	rioCfgWrite(localPort,destId,hopCnt,RIO_SP_ERR_CTRL(portNo),uiVal); 
	taskDelay(10);

	/*
	 *clears the PORT_LOCKOUT bit in order to be able to perform a
	 *maintenance transaction to the link partner
	*/
	uiVal = uiVal & (~2);
	rioCfgWrite(localPort,destId,hopCnt,RIO_SP_ERR_CTRL(portNo),uiVal); 
	taskDelay(10);

	/*send control symbol to recover foram IES/OES*/
	rioCfgWrite(localPort,destId,hopCnt,RIO_SP_CS_TX(portNo),0x40fc8000);
	taskDelay(10);
	
	if(OK != rioGetAckID(localPort,destId,hopCnt,portNo,&uiVal))
	{
		return ERROR;
	}
	uiFarAckId = (uiVal >> 5) & 0x1f;
	
	/*align our outbound/outstanding ackID with partner's inbound*/
	rioCfgRead(localPort,destId,hopCnt,RIO_SP_ACKID_STAT(portNo),
			   &uiVal);

	uiOurAckId = (uiVal & 0x1f000000) >> 24;
	
	if((uiFarAckId != ((uiVal & RIO_PORT_N_ACK_OUTSTAND) >> 8)) ||
	   (uiFarAckId != (uiVal & RIO_PORT_N_ACK_OUTBOUND)))
	{
		SRIO_LOG(SRIO_DBG_ERR,"hop %d port %d sync ackid\n",hopCnt,portNo,0,0,0);
		uiVal = (uiOurAckId << 24) | (uiFarAckId << 8) | uiFarAckId;
		rioCfgWrite(localPort,destId,hopCnt,RIO_SP_ACKID_STAT(portNo),uiVal);

		uiFarAckId++;
		uiVal = (uiFarAckId << 24) | (uiOurAckId << 8) | uiOurAckId;
		rioCfgWrite(localPort,destId,(hopCnt + 1),RIO_SP_ACKID_STAT(uiPeerPort),uiVal);
	}
	
	return OK;
}

/*****************************************************************************
 func : rioPortRecover
 desc : used to recover port when it is up from down 
 parm : localport:   Local port number (RapidIO switch) [IN]
        destid:      Destination ID of the processing element(RapidIO switch)[IN]
        hopcount:    Hop count [IN]
*****************************************************************************/
STATUS rioPortRecover(UINT32 localPort,UINT32 destId,UINT32 hopCnt,UINT32 portNo)
{
    UINT32 uiVal = 0;
    UINT32 uiLoop = 0;
       
    for(uiLoop = 0;uiLoop < 5;uiLoop++)
    {
        rioCfgWrite(localPort,destId,hopCnt,RIO_SP_ACKID_STAT(portNo),0);
        rioCfgRead(localPort,destId,hopCnt,RIO_SP_ACKID_STAT(portNo),&uiVal);
        if(0 == uiVal)
        {
            SRIO_LOG(SRIO_DBG_ERR,"recover device ok\n",0,0,0,0,0);
            return OK;
        }
    }
    if(0 != uiVal)
    {
        SRIO_LOG(SRIO_DBG_ERR,"recover device failed\n",0,0,0,0,0);
        return ERROR;
    }

    return OK;
}


/*****************************************************************************
 func : rioLocDevIsHost
 desc : used to check device is host or not 
 parm : localport:   Local port number (RapidIO switch) [IN]
*****************************************************************************/
BOOL rioLocDevIsHost(UINT32 localPort)
{
    UINT32 uiVal = 0;
    
    rioCfgRead(localPort,0xff,RIO_HOP_LOCAL,SRIO_CSR,&uiVal);
    if(0 != (uiVal & (1 << 31)))
    {
        return TRUE;
    }

    return FALSE;
}

/*****************************************************************************
 func : rioPortRecover
 desc : used to recover port when it is up from down 
 parm : localport:   Local port number (RapidIO switch) [IN]
        destid:      Destination ID of the processing element(RapidIO switch)[IN]
        hopcount:    Hop count [IN]
*****************************************************************************/
STATUS rioSetLocDevToHost(UINT32 localPort)
{
    UINT32 uiVal = 0;
    INT32  iClkRate = 0;
    UINT32 uiLoop = 0;

    iClkRate = sysClkRateGet();
    
    if(FALSE == rioLocDevIsHost(localPort))
    {
        for(uiLoop = 0;uiLoop < iClkRate * 8;uiLoop++)
        {
            rioCfgRead(localPort,0xff,RIO_HOP_LOCAL,RIO_DID_CSR,&uiVal);
            if(0xffffff != uiVal)
            {
                SRIO_LOG(SRIO_DBG_INFO,"power pc is agent\n",0,0,0,0,0);
                return ERROR;
            }
            taskDelay(1);
        }
        rioCfgWrite(localPort,0xff,RIO_HOP_LOCAL,SRIO_CSR,0xe0000000);
    }

    return OK;
} 

/*****************************************************************************
 func : tsi578PortReset
 desc : used to reset port when it is down 
 parm : localport:   Local port number (RapidIO switch) [IN]
        destid:      Destination ID of the processing element(RapidIO switch)[IN]
        hopcount:    Hop count [IN]
*****************************************************************************/
STATUS rioPortReset(UINT32 localPort,UINT32 destId,UINT32 hopCnt,
                    UINT32 portNo,UINT32 uiDID)
{
    UINT32 uiVal = 0;
	UINT32 uiComp = 0;

    if(SRIO_TSI578_DID != uiDID)
    {
        return OK;
    }
    /*port 0 reset will reset comp tag register*/
    if(0x0 == portNo)
    {
        rioCfgRead(localPort,destId,hopCnt,RIO_COMPONENT_TAG_CSR,&uiComp);
    }
    rioCfgRead(localPort,destId,hopCnt,RIO_TSI578_CLK_SEL(portNo),&uiVal);
    if(0 == (portNo % 2))
    {
        rioCfgWrite(localPort,destId,hopCnt,RIO_TSI578_CLK_SEL(portNo),uiVal | 4);
    }
    else
    {
        rioCfgWrite(localPort,destId,hopCnt,RIO_TSI578_CLK_SEL(portNo),uiVal | 8);
    }
    
    rioCfgWrite(localPort,destId,hopCnt,RIO_TSI578_CLK_SEL(portNo),uiVal);
    
    if(0x0 == portNo)
    {	
        rioCfgWrite(localPort,destId,hopCnt,RIO_COMPONENT_TAG_CSR,uiComp);
		taskDelay(1);
		rioCfgWrite(localPort,destId,hopCnt,RIO_COMPONENT_TAG_CSR,uiComp);
		taskDelay(1);
    }

    return OK;
}

