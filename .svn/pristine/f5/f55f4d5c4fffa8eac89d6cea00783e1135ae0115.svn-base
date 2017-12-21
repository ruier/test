/******************************************************************************

                  VERSION (C), 2001-2011, 

 ******************************************************************************
  file       : rioApi.c
  version    : 1.0
  author     : lgh
  data       : 2013.1.29
  function   : rapid io application interface 
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
#include "rioFsl.h"

#include "rioFsl.c"
#include "rioHal.c"
#include "rioDrv.c"
/*#include <hwif/util/vxbMsgSupport.h>*/
#include "vxbMsgSupport.h"

#include "vxbQorIQRman.h"




/*----------------------------------------------*
 * external  variable                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * external  function                           *
 *----------------------------------------------*/
extern STATUS rioSystemEnumerate(UINT32 vuiHostId);
extern STATUS rioMsgInit(MSG_RXCB_FUNC pfMsgHook);
extern void  rioSetSrioOW(void);

/*----------------------------------------------*
 * internal  function                           *
 *----------------------------------------------*/

/*----------------------------------------------*
 * global  variable                             *
 *----------------------------------------------*/

 extern UINT32 guiDbgLevel;
 extern SRIO_MPORT_S  gstSrioMport[SRIO_MAX_MPORT];

/*----------------------------------------------*
 * constant variable                            *
 *----------------------------------------------*/

/*----------------------------------------------*
 * macro definition                             *
 *----------------------------------------------*/
 
INT32 rioSysInit(pfHook pfDbHook, msgHook pfMsgHook, int flag)
{
    UINT32 uiHostId = 0;
    
    if(OK != rioSetup())
    {
        return -1;
    }

    uiHostId = 0x12;
    if(OK != rioSystemEnumerate(uiHostId))
    {
        return -4;
    }
    
    if(OK != rioDoorbellInit(pfDbHook))
    {
        return -2;
    }
    
   /* if(OK != rioMsgInit(pfMsgHook))
    {
        return -5;
    }*/

   /*if(0 != flag)
    {
        rioSetSrioOW();
    }*/

    SRIO_LOG(SRIO_DBG_INFO,"rioSysInit ok\n",0,0,0,0,0);

    return 0;
}

void rioStub(void)
{
    
}
