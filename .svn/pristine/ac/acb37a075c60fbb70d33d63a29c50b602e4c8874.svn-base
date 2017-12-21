/******************************************************************************

                  VERSION (C), 2001-2011, 

 ******************************************************************************
  file       : 
  version    : 1.0
  author     :  
  data       :  
  function   :  
******************************************************************************/

/*----------------------------------------------*
 * header file                                  *
 *----------------------------------------------*/
#include "common_lib_def/rioDepend.h"

/*
#include "common_lib_def/rioHal.h"
#include "common_lib_def/rioFsl.h"
*/

/*----------------------------------------------*
 * external  variable                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external  function                           *
 *----------------------------------------------*/
extern STATUS rioSetPortSpeed(UINT32 localPort,UINT32 destId,UINT32 hopCnt,
                         UINT32 portNo, UINT32 portSpeed);


/*----------------------------------------------*
 * internal  function                           *
 *----------------------------------------------*/


/*----------------------------------------------*
 * global  variable                             *
 *----------------------------------------------*/
UINT32 guiDbgLevel = SRIO_DBG_ERR;    /*SRIO_DBG_ERR  SRIO_DBG_INFO  SRIO_DBG_BUTT*/
BOOL    guiSrioInit = FALSE;

/*----------------------------------------------*
 * constant variable                            *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macro definition                             *
 *----------------------------------------------*/

STATUS rioSetSwPortSpeed(UINT32 localPort,UINT32 destId,UINT32 hopCnt)
{
    /*  PORT 10  板内K7, 不修改速率*/
    /*  PORT 5  host, 不修改速率*/

#if 0    
    rioSetPortSpeed(localPort, 0xff, 0, 0, RIO_PORT_SPEED25);   
    rioSetPortSpeed(localPort, 0xff, 0, 1, RIO_PORT_SPEED25);
    rioSetPortSpeed(localPort, 0xff, 0, 2, RIO_PORT_SPEED25);   
    rioSetPortSpeed(localPort, 0xff, 0, 4, RIO_PORT_SPEED25);    
    rioSetPortSpeed(localPort, 0xff, 0, 11, RIO_PORT_SPEED25);   

    /*rioSetPortSpeed(localPort, 0xff, 0, 10, RIO_PORT_SPEED25);  */
#endif

}



/*注意在P2020.1项目中, portID使用0, 而不是1*/
UINT32 rioSysInit(UINT32 vuiPortID, pfHook pfDbHook, msgHook pfMsgHook, int flag)
{
	UINT32 uiHostId = 0;

#ifdef  SRIO_PORT_WRITE
	rioInitPortWrite();
#endif

	if (OK != rioSetup(vuiPortID))
	{
	    return -1;
	}

	uiHostId = rioGetLocalPort() + 0x10;
	/*uiHostId = 0xe;*/

	uiHostId = 0x12;

	if (OK != rioSystemEnumerate(vuiPortID, uiHostId))
	{
	    return -4;
	}


	if (OK != rioInitDoorbell(vuiPortID, pfDbHook))
	{
		return -2;
	}

#if 0
	if(OK != rioMsgInit(vuiPortID, pfMsgHook))
	{
	    return -5;
	}

	if(0 != flag)
	{
	    rioSetSrioOW(vuiPortID);
	}
#endif

	SRIO_LOG(SRIO_DBG_INFO, "rioSysInit ok\n",0,0,0,0,0);

	return 0;
}





