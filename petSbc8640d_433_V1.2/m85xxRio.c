/* m85xxRio.c - RapidIO host controller in PowerPC 85XX CPU */

/*
 * Copyright (c) 2005-2008 Wind River Systems, Inc. All rights are reserved.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01r,04aug08,h_k  updated VxBus version. (CQ:128598)
01q,10jul08,tor  update version
01p,07jul08,pmr  resource documentation
01o,18jun08,h_k  removed pAccess
01n,11apr08,h_k  updated for new reg access methods.
01m,24sep07,tor  VXB_VERSION_3
01l,04sep07,dtr  Fix doc errors.
01k,15aug07,h_k  removed m85xxWindowAlloc_desc.
01j,03aug07,dtr  Use the latest access routines.
01i,13jun07,tor  remove VIRT_ADDR
01h,17apr07,b_m  fix misuse of m85xxCCSR method.
01g,17jan07,dtr  Defect 85884
01f,23aug06,pmr  use CCSR driver.
01e,06jul06,dtr  Fix write/read types for windows attributes.
01d,25apr06,mdo  Fix routine header for apigen.
01c,23feb06,dtr  SPR#117998 - Fixes cpuId check which caused project image
                 failure.
01b,31jan06,pdg  updated vxbus version
01b,27jan06,dtr  Allocate memory for BusSpecifcInfo. SPR#117948
01a,05Oct05,tor  created
*/

/* includes */

#include <vxWorks.h>
#include <vxLib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <intLib.h>
#include <sysLib.h>
#include <taskLib.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/vxbPciLib.h>
#include <hwif/util/hwMemLib.h>
#include <hwif/util/vxbSmSupport.h>
#include "../h/vxbus/vxbAccess.h"
#include "../h/vxbus/vxbRapidIO.h"
#include "../h/vxbus/vxbArchAccess.h"
#include "../h/busCtlr/m85xxRio.h"
#include <private/funcBindP.h>

/*
DESCRIPTION
This is the RAPIDIO driver to support configuration of the Serial RapidIO 
device on the MPC8548 chip. It has been to to tested for configuring another 
agent serial rapidio 8548 device although may be developed to support other 
chips easily none were available for testing.

This is a vxbus driver which needs specific configuration information provided
by the board support package. This is done via the hwconf.c file in the BSP.
An example is below:

const struct hcfResource m85xxRio0Resources[] = {
    { "regBase",	HCF_RES_INT, {(void *)RAPIDIO_BASE} },
    { "deviceBase", HCF_RES_INT, {(void *)(RAPIDIO_ADRS + 0x0000000)}},
    { "deviceSize", HCF_RES_INT, {(void *)(RAPIDIO_SIZE - 0x0000000)}},
    { "rioBusAdrs", HCF_RES_INT, {(void *)RAPIDIO_BUS_ADRS }},
    { "rioBusSize", HCF_RES_INT, {(void *)RAPIDIO_BUS_SIZE }},
    { "tgtIf", HCF_RES_INT, {(void *)LAWAR_TGTIF_RAPIDIO }},
    { "localMemAdrs",HCF_RES_INT, {(void *)LOCAL_MEM_LOCAL_ADRS}}    
};
#define m85xxRio0Num    NELEMENTS(m85xxRio0Resources)

On top of this each CPU needs configuration information passed in:

const struct hcfResource m85xxCPU0Resources[] = {
	{ "regBase",	HCF_RES_INT, {(void *)RAPIDIO_BASE }},
	{ "targetID",	HCF_RES_INT, {(void *)0 }},
	{ "outboundWindow0",     HCF_RES_INT, {(void *)RIO_CHANNEL_RESERVED }},
	{ "outboundWindow1",     HCF_RES_INT, {(void *)RIO_CHANNEL_MAINT }},
	{ "outboundWindow2",     HCF_RES_INT, {(void *)RIO_CHANNEL_CFG }},
	{ "outboundWindow3",     HCF_RES_INT, {(void *)RIO_CHANNEL_UNRESERVED }},
	{ "outboundWindow4",     HCF_RES_INT, {(void *)RIO_CHANNEL_UNRESERVED }},
	{ "outboundWindow5",     HCF_RES_INT, {(void *)RIO_CHANNEL_UNRESERVED }},
	{ "outboundWindow6",     HCF_RES_INT, {(void *)RIO_CHANNEL_UNRESERVED }},
	{ "outboundWindow7",     HCF_RES_INT, {(void *)RIO_CHANNEL_UNRESERVED }},
	{ "outboundWindow8",     HCF_RES_INT, {(void *)RIO_CHANNEL_UNRESERVED }},
	{ "inboundWindow0",     HCF_RES_INT, {(void *)RIO_CHANNEL_RESERVED }},
	{ "inboundWindow1",     HCF_RES_INT, {(void *)RIO_CHANNEL_SM }},
	{ "inboundWindow2",     HCF_RES_INT, {(void *)RIO_CHANNEL_UNRESERVED }},
	{ "inboundWindow3",     HCF_RES_INT, {(void *)RIO_CHANNEL_UNRESERVED }},
	{ "inboundWindow4",     HCF_RES_INT, {(void *)RIO_CHANNEL_UNRESERVED }}

};
#define m85xxCPU0Num    NELEMENTS(m85xxCPU0Resources)

const struct hcfResource m85xxCPU1Resources[] = {
	{ "regBase",	HCF_RES_INT, {(void *)RAPIDIO_BASE }},
	{ "targetID",	HCF_RES_INT, {(void *) 0x9 }},
	{ "hopCount",	HCF_RES_INT, {(void *) 0x0 }},
	{ "outboundWindow0",     HCF_RES_INT, {(void *)RIO_CHANNEL_RESERVED }},
	{ "outboundWindow1",     HCF_RES_INT, {(void *)RIO_CHANNEL_SM }},
	{ "outboundWindow2",     HCF_RES_INT, {(void *)RIO_CHANNEL_TAS_SET }},
	{ "outboundWindow3",     HCF_RES_INT, {(void *)RIO_CHANNEL_TAS_CLEAR }},
	{ "outboundWindow4",     HCF_RES_INT, {(void *)RIO_CHANNEL_DOORBELL }},
	{ "outboundWindow5",     HCF_RES_INT, {(void *)RIO_CHANNEL_UNRESERVED }},
	{ "outboundWindow6",     HCF_RES_INT, {(void *)RIO_CHANNEL_UNRESERVED }},
	{ "outboundWindow7",     HCF_RES_INT, {(void *)RIO_CHANNEL_UNRESERVED }},
	{ "outboundWindow8",     HCF_RES_INT, {(void *)RIO_CHANNEL_UNRESERVED }},
	{ "inboundWindow0",     HCF_RES_INT, {(void *) RIO_CHANNEL_RESERVED}},
	{ "inboundWindow1",     HCF_RES_INT, {(void *)RIO_CHANNEL_UNRESERVED }},
	{ "inboundWindow2",     HCF_RES_INT, {(void *)RIO_CHANNEL_UNRESERVED }},
	{ "inboundWindow3",     HCF_RES_INT, {(void *)RIO_CHANNEL_UNRESERVED }},
	{ "inboundWindow4",     HCF_RES_INT, {(void *)RIO_CHANNEL_UNRESERVED }}

};
#define m85xxCPU1Num    NELEMENTS(m85xxCPU1Resources)
  
*/
#define M85XXRIO_DEBUG
/*#undef M85XXRIO_DEBUG*/

#ifdef M85XXRIO_DEBUG

/* defines */
/*#define M85XXRIO_LOG(X0, X1, X2, X3, X4, X5, X6)			\
	{ \
	if (_func_logMsg != NULL)					\
		{ \
		_func_logMsg (X0, (int)X1, (int)X2, (int)X3, (int)X4,	\
					(int)X5, (int)X6); \
				taskDelay(60);\
		} \
	}
*/
extern int     ns16550Pollprintf(char* pChar);
char sprintBuf[100];
#define M85XXRIO_LOG(X0, X1, X2, X3, X4, X5, X6)	{sprintf(sprintBuf,X0, X1, X2, X3, X4, X5, X6);ns16550Pollprintf(sprintBuf);}
#else

#define M85XXRIO_LOG(X0, X1, X2, X3, X4, X5, X6)	

#endif

#define VXB_RIO_READ(offset) \
            vxbRead32(pDrvCtrl->regAccessHandle,(UINT32*)offset)
#define VXB_RIO_WRITE(offset,value) \
            vxbWrite32(pDrvCtrl->regAccessHandle,(UINT32*)offset,value)
#define VXB_RIO_READ16(offset) \
            vxbRead16(pDrvCtrl->regAccessHandle,(UINT16*)offset)
#define VXB_RIO_WRITE16(offset,value) \
            vxbWrite16(pDrvCtrl->regAccessHandle,(UINT16*)offset,value)
#define VXB_RIO_READ8(offset) \
            vxbRead8(pDrvCtrl->regAccessHandle,(UINT8*)offset)
#define VXB_RIO_WRITE8(offset,value) \
            vxbWrite8(pDrvCtrl->regAccessHandle,(UINT8*)offset,value)

/* typedefs */

/*
* We track downstream devices
*/

struct rioSpecificDevInfo
    {
    UINT16	devIDLocal;	/* DeviceID of PE */
    };

/*
* With this host controller, we have six channels available
* to program destination ports.  We pre-allocate channel #5
* for dynamic usage, and make channels #0 through #4 available
* for dedicated remote PEs.
*/

struct m85xxRioDrvCtrl
    {
    VXB_DEVICE_ID pInst;
    void *regAccessHandle;
    UINT32      host;
    UINT32	bridgeStatus;	/* status of host bridge */
    UINT32 	lawbar;		/* LAWBAR allocated to RapidIO */
    UINT32 	lawar;		/* LAWAR allocated to RapidIO */
    int	        lawbarIndex;        /* LAWBAR index */
    char	*deviceBase;	/* RAPIDIO_ADRS */
    UINT32      deviceSize;
    char        *rioBusAdrs;
    UINT32      rioBusSize;
    UINT32      pHcfDevice;
    UINT32      tgtIf;
    char        *localMemAdrs;
    };

typedef struct rioCfgWindowInfo
    {
    VXB_DEVICE_ID rioDev;
    UINT32 tgtId;
    char* hostLocalMemRioAdrs;
    UINT32 agentCfgWindow;
    UINT32 agentMainWindow;
    UINT32 agentAtomicSetWindow;
    UINT32 agentAtomicClearWindow;
    UINT32 agentDoorbellWindow;
    UINT32 hostDoorbellWindow;
    } RIO_CFG_WINDOW_INFO;

typedef struct cfg_table
    {
    UINT32 cpuId;
    VOIDFUNCPTR cfgRoutine;
    UINT32 type;
    } CFG_TABLE;

typedef struct mailbox_bd
    {
    UINT32 tgtInfo;
    UINT32 srcInfo;
    } MAILBOX_BD;

struct getDevInfo
    {
    int devUnit;
    RAPIDIO_DEVICE_INFORMATION *pDevInfo;
    };

struct getCPUID
    {
    int devUnit;
    VXB_DEVICE_ID devID;
    };

#if 0
/* Forward Declarations */
LOCAL void m85xxRioCfgSetupWindow
    (
    VXB_DEVICE_ID	pInst,	/* bus controller ID */
    char 		*adrs,
    UINT32		deviceID,
    UINT32		hopCount,
    UINT32		offset,
    UINT32		window
    );

LOCAL void rioSetOW
    (
    VXB_DEVICE_ID pInst,
    UINT32 window,
    char* localAdrs,
    UINT32 size,
    UINT32 tgtID,
    char* rioAdrs
    );

LOCAL void rioSetRemoteOW
    (
    VXB_DEVICE_ID pInst,
    char* remBa,
    UINT32 window,
    char* localAdrs,
    UINT32 size,
    UINT32 tgtID,
    char* rioAdrs
    );
		
LOCAL void rioSetIW
    (
    VXB_DEVICE_ID pInst,
    UINT32 window,
    char* rioAdrs,
    UINT32 size,
    char* localAdrs
    );


LOCAL void rioSetRemoteIW
    (
    VXB_DEVICE_ID pInst,
    char* remBa,
    UINT32 window,
    char* rioAdrs,
    UINT32 size,
    char* localAdrs
    );


LOCAL void rioSetRemoteIWSpecial
    (
    VXB_DEVICE_ID pInst,
    char* remBa,
    UINT32 window,
    char* rioAdrs,
    UINT32 size,
    char* localAdrs,
    UINT32 typeRead,
    UINT32 typeWrite,
    UINT32 tgtIf
    );

LOCAL void rioSetRemoteOWSpecial
    (
    VXB_DEVICE_ID pInst,
    char* remBa,
    UINT32 window,
    char* localAdrs,
    UINT32 size,
    UINT32 tgtID,
    char* rioAdrs,
    UINT32 typeRead,
    UINT32 typeWrite
    );
#endif


/* globals */

IMPORT STATUS vxbRapidIOBusTypeInit(VXB_DEVICE_ID pInst);

VXB_DRIVER_METHOD_DECL(m85xxWindowAlloc);
VXB_DRIVER_METHOD_DECL(m85xxLawBarAllocSet);
VXB_DRIVER_METHOD_DECL(sharedMemSupportAPIGet);

/* Processor local bus global instance */
IMPORT struct vxbBusPresent * pPlbBus;

#include "../resource/m85xxCCSR.h"

BOOL rioHost = FALSE;


char*  rioHostAdrs = 0;
char*  rioTasSetAdrs=0;
char*  rioTasClearAdrs=0;

//LOCAL UINT32 rioAdrsCnt = 0;

/* Temporary to allow address allocation */

#define M85XX_RIO_ADRS1 ((char*)0x1000000)
#define M85XX_RIO_ADRS2 ((char*)0x2000000)
#define M85XX_RIO_ADRS3 ((char*)0x3000000)
#define M85XX_RIO_ADRS4 ((char*)0x4000000)
#define M85XX_RIO_ADRS5 ((char*)0x5000000)
#define M85XX_RIO_ADRS6 ((char*)0x6000000)
#define M85XX_RIO_ADRS7 ((char*)0x7000000)
#define M85XX_RIO_ADRS8 ((char*)0x8000000)
#define M85XX_RIO_ADRS9 ((char*)0x9000000)
#define M85XX_LOCAL_RIO_ADRS1 ((char*)0x1000000)
#define M85XX_LOCAL_RIO_ADRS2 ((char*)0x2000000)
#define M85XX_LOCAL_RIO_ADRS3 ((char*)0x3000000)
#define M85XX_LOCAL_RIO_ADRS4 ((char*)0x4000000)
#define M85XX_LOCAL_RIO_ADRS5 ((char*)0x5000000)
#define M85XX_LOCAL_RIO_ADRS6 ((char*)0x6000000)
#define M85XX_LOCAL_RIO_ADRS7 ((char*)0x7000000)

#if 0
LOCAL char* rioAdrs[] =
	{
        M85XX_RIO_ADRS1,
        M85XX_RIO_ADRS2,
        M85XX_RIO_ADRS3,
        M85XX_RIO_ADRS4,
        M85XX_RIO_ADRS5,
        M85XX_RIO_ADRS6,
        M85XX_RIO_ADRS7,
        M85XX_RIO_ADRS8,
        M85XX_RIO_ADRS9
	};


LOCAL char* localAdrs[] =
	{
        M85XX_LOCAL_RIO_ADRS1,
        M85XX_LOCAL_RIO_ADRS2,
        M85XX_LOCAL_RIO_ADRS3,
        M85XX_LOCAL_RIO_ADRS4,
        M85XX_LOCAL_RIO_ADRS5,
        M85XX_LOCAL_RIO_ADRS6,
        M85XX_LOCAL_RIO_ADRS7
	};


LOCAL char* outboundDoorbellAdrs[MAX_NUM_RIO_TARGETS] = {0};
#endif
#ifndef RIO_WINDOW_SIZE
#define RIO_WINDOW_SIZE 0x1000000
#endif

#define RIO_CFG_WINDOW_SIZE 0x100000

/* head of list of instances*/
IMPORT struct vxbDev* pInstListHead;
IMPORT struct vxbBusPresent* pPlbBus;

//LOCAL UINT32 m85xxRioCfgCalled = 0;
//LOCAL UINT32 hostID = 0;

//LOCAL RAPIDIO_DEVICE_INFORMATION *pDevInfoHost = NULL;
//LOCAL VXB_DEVICE_ID rioDevHost = NULL;

/* forward declarations */

LOCAL void m85xxRioInstInit(VXB_DEVICE_ID pInst);
LOCAL void m85xxRioInstInit2(VXB_DEVICE_ID pInst);
//LOCAL void m85xxRioInstConnect(VXB_DEVICE_ID pInst);
//LOCAL BOOL m85xxRioProbe(VXB_DEVICE_ID pInst);
//LOCAL UINT32 convertTypeOWWrite(UINT32 type);
//LOCAL UINT32 convertTypeIWRead(UINT32 type);
//LOCAL UINT32 convertTypeIWWrite(UINT32 type);

//LOCAL UINT32 convertTgtIf(UINT32 tgtIf);

//LOCAL STATUS m85xxRioCfg(VXB_DEVICE_ID);
//LOCAL void m85xxRioCfgInit();
//LOCAL void rio85XXCfg(VXB_DEVICE_ID pInst,RIO_CFG_WINDOW_INFO *winInfo);
//LOCAL void rio8548Cfg(VXB_DEVICE_ID pInst,RIO_CFG_WINDOW_INFO *winInfo);
//LOCAL void rioCpuCfg(VXB_DEVICE_ID pInst,RIO_CFG_WINDOW_INFO *winInfo);
#if 0
LOCAL STATUS m85xxRioRemoteCfgRead
    (
    VXB_DEVICE_ID   pDev,           /* device info */
    UINT32          byteOffset,     /* offset into cfg space */
    UINT32          transactionSize,/* transaction size, in bytes */
    char *       pDataBuf,       /* buffer to read-from/write-to */
    UINT32 *        pFlags          /* flags */
    );

LOCAL STATUS m85xxRioRemoteCfgWrite
    (
    VXB_DEVICE_ID   pDev,           /* device info */
    UINT32          byteOffset,     /* offset into cfg space */
    UINT32          transactionSize,/* transaction size, in bytes */
    char *       pDataBuf,       /* buffer to read-from/write-to */
    UINT32 *        pFlags          /* flags */
    );



LOCAL STATUS m85xxWindowAlloc
    (
    VXB_DEVICE_ID pInst,
    char * pArg
    );

LOCAL struct sharedMemSupportAPI * m85xxRioSmSupportAPIGet
    (
    VXB_DEVICE_ID pInst,
    char * pArg
    );

LOCAL void m85xxRioBusIntInit
    (
    char* agentCCSBAR,
    UINT32 hostID,
    UINT32 agentID
    );

LOCAL BOOL m85xxRioBusTas
    (
    char* adrs          /* address to be tested and set*/
    );

LOCAL void m85xxRioBusTasClear
    (
    volatile char* adrs    /* Address of semaphore to be cleared*/
    );

LOCAL STATUS m85xxRioBusIntGen
    (
    int level,          /* interrupt level to generate   */
    int vector          /* interrupt vector for interrupt*/
    );

LOCAL int m85xxRioBusIntAck
    (
    int intLevel        /* interrupt level to acknowledge*/
    );

#endif
IMPORT int vxbRapidIOCfg
    (
    VXB_DEVICE_ID       pParent,        /* parent bus controller */
    VXB_ACCESS_LIST *   pAccess,        /* device access API */
    STATUS (*busOverride)               /* bus-specific init rtn */
    (VXB_DEVICE_ID pDev)    /* (init rtn arg) */
    );

#ifdef	VXB_LEGACY_ACCESS
LOCAL struct vxbAccessList m85xxRioAccessFuncs =
    {
    VXB_BUSID_RAPIDIO,  /* busID */
    VXB_VER_4_0_0,      /* busVer */
    NULL,               /* pCookie */
    _archRegProbe,              /* architecture  register probe function */
    _archRegisterRead8,         /* architecture  function to read 8 bits */
    _archRegisterRead16,        /* architecture  function to read 16 bits */
    _archRegisterRead32,        /* architecture  function to read 32 bits */
    _archRegisterRead64,        /* architecture  function to read 64 bits */
    _archRegisterWrite8,        /* architecture  function to write 8 bits */
    _archRegisterWrite16,       /* architecture  function to write 16 bits */
    _archRegisterWrite32,       /* architecture  function to write 32 bits */
    _archRegisterWrite64,       /* architecture  function to write 64 bits */
    _archVolatileRegisterWrite, /*
                                 * architecture  function to write to
                                 * a volatile register
                                 */
    _archVolatileRegisterRead,  /*
                                 * architecture  function to read from
                                 * a volatile register
                                 */
    m85xxRioRemoteCfgRead,    /* busCfgRead */
    m85xxRioRemoteCfgWrite,   /* busCfgWrite */
    NULL 		/* vxbDevControl */
    };
#endif	/* VXB_LEGACY_ACCESS */

/* allow downstream instances to allocate dedicated ATMU channels */

LOCAL device_method_t m85xxRio_methods[] =
    {
   // DEVMETHOD(m85xxWindowAlloc, m85xxWindowAlloc),
   // DEVMETHOD(sharedMemSupportAPIGet, m85xxRioSmSupportAPIGet),
    { 0, 0 }
    };
/*
*   Setup doorbell/interrupt init between host/agent(s) to signal data
*   Doorbell isr ack
*   Doorbell interrupt generator
*   Basic Functionality:
*   Test and Set - RMW
*/

IMPORT UINT32 sysTas(char*);


//LOCAL char * baseBDPtr;
//LOCAL char * startBDPtr;
//LOCAL char * endBDPtr;

//LOCAL UINT32 agentIDList[MAX_NUMBER_RIO_TARGETS] = {0};
//LOCAL BOOL mailboxFirstTime = TRUE;
//LOCAL int agentCount = 0;
//LOCAL int doorbellCount= 0;
#if 0
LOCAL CFG_TABLE cpuIdTable[] =
    {
    { 0x00020002 , rio85XXCfg, 0 },
    { 0x00030002 , rio85XXCfg, 0 },
    { 0x00120002 , rio8548Cfg, 0 },
    { 0,0,0}
    };

LOCAL struct sharedMemSupportAPI m85xxRioSmAPI =
    {
    (FUNCPTR)m85xxRioBusTas,
    (FUNCPTR)m85xxRioBusTasClear,
    (FUNCPTR)m85xxRioBusIntGen,
    (FUNCPTR)m85xxRioBusIntAck,
    (FUNCPTR)m85xxRioBusIntInit,
    (FUNCPTR)NULL,
    (FUNCPTR)NULL,
    (FUNCPTR)NULL,
    (FUNCPTR)NULL
    };
#endif
/* more forward declarations */

LOCAL void m85xxRioInstInit(VXB_DEVICE_ID pInst);
LOCAL void m85xxRioInstInit2(VXB_DEVICE_ID pInst);
LOCAL void m85xxRioInstConnect(VXB_DEVICE_ID pInst);


LOCAL struct drvBusFuncs m85xxRioFuncs =
    {
    m85xxRioInstInit,      /* devInstanceInit */
    m85xxRioInstInit2,     /* devInstanceInit2 */
    m85xxRioInstConnect    /* devInstanceConnect */
    };

LOCAL DRIVER_REGISTRATION m85xxRioDevRegistration =
    {
    NULL,		  /* pNext */
    VXB_DEVID_BUSCTRL,    /* devID */
    VXB_BUSID_LOCAL,      /* busID = Processor Local Bus */
    VXB_VER_4_0_0,        /* busVer */
    "m85xxRio",           /* drvName */
    &m85xxRioFuncs,       /* pDrvBusFuncs */
    &m85xxRio_methods[0], /* pMethods */
    //m85xxRioProbe         /* devProbe */
    NULL
    };

#define TAS_SET_TRANSLATE(x) ((x & 0xfffff) + rioTasSetAdrs)
#define TAS_CLEAR_TRANSLATE(x) ((x & 0xfffff) + rioTasClearAdrs)

VXB_DEVICE_ID globalRioID=NULL;

#ifdef USE_DEBUGGING
IMPORT int vxbDevTblDebugLevel;
IMPORT int usrBusDebugLevel;
#endif

/*****************************************************************************
*
* m85xxRioRegister - register PowerPC 85xx rapidIO with bus subsystem
*
* This passes the registration structure for this device to the vxBus 
* registration function. Only when this is called can the driver be used.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/
void m85xxRioRegister(void)
    {

#ifdef USE_DEBUGGING
    usrBusDebugLevel = 0;
    vxbDevTblDebugLevel = 0;
#endif
    vxbDevRegister((DRIVER_REGISTRATION *)&m85xxRioDevRegistration);
    }

/**********************************************************
* m85xxRioSmSupportAPIGet - Method to support shared memory
*
* RETURNS: shared memory funcs struct
*
*/
#if 0 
LOCAL struct sharedMemSupportAPI *m85xxRioSmSupportAPIGet
    (
    VXB_DEVICE_ID pInst,
    char * pArgs
    )
    {
    return(&m85xxRioSmAPI);
    }
#endif
/*******************************************************************************
*
* m85xxRioInstInit - Create a PPC440GP PCI host bridge instance
*
* RETURNS: N/A
*
*/
LOCAL void m85xxRioInstInit
    (
    VXB_DEVICE_ID pInst
    )
    {
    struct m85xxRioDrvCtrl *	pDrvCtrl;
    STATUS			status;
    HCF_DEVICE *	pHcfDevice;

    /* take care of pDrvCtrl and other overhead */

    pDrvCtrl = (struct m85xxRioDrvCtrl *)hwMemAlloc(sizeof(*pDrvCtrl));

    if ( pDrvCtrl == NULL )
        return;

    pDrvCtrl->bridgeStatus = RIO_NOT_INITIALIZED;

    pInst->pDrvCtrl = pDrvCtrl;

    globalRioID = pInst;


    /* initialize RapidIO bus type */
    M85XXRIO_LOG("initializing RapidIO bus type\n",1,2,3,4,5,6);
    vxbRapidIOBusTypeInit(pInst);


    /* tell VxBus that a RapidIO bus exists */

    M85XXRIO_LOG("calling vxbBusAnnounce(0x%08x, %d)\n",
		  (int)pInst, VXB_BUSID_RAPIDIO,3,4,5,6);

    status = vxbBusAnnounce(pInst, VXB_BUSID_RAPIDIO);



    if ( status != OK )
        M85XXRIO_LOG("vxbBusAnnounce() failed\n",1,2,3,4,5,6);

    /* Check for downstream devices */

    M85XXRIO_LOG("checking for RapidIO devices\n",1,2,3,4,5,6);
#ifdef	VXB_LEGACY_ACCESS
    vxbRapidIOCfg(pInst, &m85xxRioAccessFuncs, NULL);
#else	/* VXB_LEGACY_ACCESS */
    vxbRapidIOCfg(pInst, NULL, NULL); 

    
#endif	/* VXB_LEGACY_ACCESS */


    vxbRegMap(pInst,0,&pDrvCtrl->regAccessHandle);

    /* 		
     * first extract the HCF pointer from the existing structure
     */

    pHcfDevice = (HCF_DEVICE *)pInst->pBusSpecificDevInfo;
    if ( pHcfDevice == NULL )
	return;

    /*
     * Extract RapidIO information from the HCF table and store it in the RapidIO
     * device structure.
     */

    /* get 32-bit fields */
    /*
     * resourceDesc {
     * The deviceBase resource specifies the base address
     * of the RapidIO local address space. }
     */
    devResourceGet(pHcfDevice, "deviceBase", HCF_RES_INT, (void*)&pDrvCtrl->deviceBase);
    /*
     * resourceDesc {
     * The deviceSize resource specifies the size
     * of the RapidIO local address space. }
     */
    devResourceGet(pHcfDevice, "deviceSize", HCF_RES_INT, (void*)&pDrvCtrl->deviceSize);
    /*
     * resourceDesc {
     * The rioBusAdrs resource specifies the base address
     * of the RapidIO bus address space. }
     */
    devResourceGet(pHcfDevice, "rioBusAdrs", HCF_RES_INT, (void*)&pDrvCtrl->rioBusAdrs);
    /*
     * resourceDesc {
     * The deviceSize resource specifies the size
     * of the RapidIO bus address space. }
     */
    devResourceGet(pHcfDevice, "rioBusSize", HCF_RES_INT, (void*)&pDrvCtrl->rioBusSize);
    
    /*
     * resourceDesc {
     * The tgtIf resource specifies the target interface
     * value to be compared with available local access windows.
     * It should be set to LAWAR_TGTIF_RAPIDIO. }
     */
    devResourceGet(pHcfDevice, "tgtIf", HCF_RES_INT, (void*)&pDrvCtrl->tgtIf);
    /*
     * resourceDesc {
     * The localMemAdrs resource specifies the local memory
     * base address of the system (LOCAL_MEM_LOCAL_ADRS). }
     */
    devResourceGet(pHcfDevice, "localMemAdrs", HCF_RES_INT, (void*)&pDrvCtrl->localMemAdrs);

    M85XXRIO_LOG("deviceBase 0x%x\n",pDrvCtrl->deviceBase,2,3,4,5,6);

    /* set status to initialized */

    pDrvCtrl->bridgeStatus = RIO_BRIDGE_ININTIALIZED;

    return;
    }
#if 0
#ifdef USE_DEBUGGING
/*******************************************************************************
* m85xxRioRegDbgRead - read rapidIO registers
*
* RETURNS: NONE
*
*/
void m85xxRioRegDbgRead
    (
    VXB_DEVICE_ID pInst
    )
    {
    UINT32 flags;
    UINT32 dataVal;
    STATUS status;
    struct m85xxRioDrvCtrl *	pDrvCtrl;
    char * regBase = pInst->pRegBase[0];

    pDrvCtrl = pInst->pDrvCtrl;

    M85XXRIO_LOG("PESCSR @ 0x%08x\n", M85XXRIO_PESCSR(pInst->pRegBase[0]),2,3,4,5,6);

    M85XXRIO_LOG("SRIO_STATUS_CHECK = 0x%08x\n", SRIO_STATUS_CHECK,2,3,4,5,6);

    M85XXRIO_LOG("calling regRd32(0x%08x, 0x%08x, 0x%08x, &dataVal, &status)\n",
		  pInst, pInst->pRegBase[0], M85XXRIO_PESCSR_OFFSET,4,5,6);

    flags = 0;
    dataVal = VXB_RIO_READ(M85XXRIO_PESCSR(regBase));
    M85XXRIO_LOG("PESCSR read status is %s (%x), value is 0x%08x\n",
		  status == OK ? "OK" : "ERROR", status, dataVal,4,5,6);
    }

#endif


/*******************************************************************************
* m85xxRioGetDevInfo - Get each rapidIO CPU device info
*
* RETURNS: N/A
*
*/
LOCAL STATUS m85xxRioGetDevInfo
    (
    VXB_DEVICE_ID   pBusCtlr,
    VXB_DEVICE_ID   pDev,
    char *   pArg
    )
    {
    struct getDevInfo *pGetDevInfo = (struct getDevInfo *)pArg;
    	
    /* Check for free entry */
    if(((RAPIDIO_DEVICE_INFORMATION*)pBusCtlr->pBusSpecificDevInfo)->devUnit == pGetDevInfo->devUnit)
	{
	pGetDevInfo->pDevInfo = pBusCtlr->pBusSpecificDevInfo;	
	}
		
    return(OK);
    }

/*****************************************************************************
* m85xxRioGetCPUID - Get each rapidIO CPU instance
*
* RETURNS: N/A
*
*/
LOCAL STATUS m85xxRioGetCPUID
    (
    VXB_DEVICE_ID   pBusCtlr,
    VXB_DEVICE_ID   pDev,
    char *   pArg
    )
    {
    struct getCPUID *pGetCPUID;
		
    pGetCPUID = (struct getCPUID *)pArg;
    if(((RAPIDIO_DEVICE_INFORMATION*)pBusCtlr->pBusSpecificDevInfo)->devUnit == pGetCPUID->devUnit)
	{
	pGetCPUID->devID = pBusCtlr;	
	}		
			
    return(OK);
    }


/*****************************************************************************
* m85xxRioTargetIdInit - Get each rapidIO CPU instance
*
* RETURNS: N/A
*
*/
LOCAL void  m85xxRioTargetIdInit
    (
    VXB_DEVICE_ID pInst
    )
    {
    VXB_DEVICE_ID   pCPUID;
    struct  m85xxRioDrvCtrl *pDrvCtrl;
    RAPIDIO_DEVICE_INFORMATION *pDevInfo;
    RIO_CPU_INFO *pCpuInfo = ((RAPIDIO_DEVICE_INFORMATION*)pInst->pBusSpecificDevInfo)->pCpuInfo;
    int index;
    UINT32 tgtId;
    VUINT32 tempVal;
    STATUS status = OK;
    PEFCAR_DESC pefcar;	
	
    pDrvCtrl = pInst->pDrvCtrl;

    /* Do not need to configure host */
    for(index = 1; index < MAX_NUMBER_RIO_TARGETS;index++)
	{
	
	pDevInfo = pCpuInfo[index].pDevInfo;
				
	if(pCpuInfo[index].devID==NULL)
	    break;
					
	pCPUID= pCpuInfo[index].devID;

	tgtId = 0xff;
	tempVal = 0xffffffff;	
	
#ifdef RECONFIG
	{
	m85xxRioCfgSetupWindow
	  (
	  pInst,
	  pDrvCtrl->deviceBase,
	  pDevInfo->targetID,
	  pDevInfo->hopCount,
	  0,
	  1
	  );

	tempVal = 0xff << RIO_HOST_ID_OFFSET;
	VXB_RIO_WRITE(M85XXRIO_BDIDCSR(pDrvCtrl->deviceBase),tempVal);
	tgtId = 0xff;
	tempVal = 0xffffffff;	
	}
#endif /* RECONFIG */

	m85xxRioCfgSetupWindow
	  (
	  pInst,
	  pDrvCtrl->deviceBase,
	  tgtId,
	  pDevInfo->hopCount,
	  0,
	  1
	  );

	/* Should call vxbBusProbe here however we know PEFCAR behaves
	   similar to memory so no need */
	status = vxMemProbe((char *)M85XXRIO_PEFCAR(pDrvCtrl->deviceBase),
			    VX_READ,
			    sizeof(UINT32),
			    (char*)&tempVal);
	
	/* Trap machine check - assume already configured */
	if(status == ERROR)
	    {
	    return;
	    }


	/* no machine check so need to configure target IDs */
					
	pefcar.word = VXB_RIO_READ(M85XXRIO_PEFCAR(pDrvCtrl->deviceBase));

	if(pefcar.desc.proc == 1)
	    {
	    tempVal = VXB_RIO_READ(M85XXRIO_HBDIDLCSR(pDrvCtrl->deviceBase));
						
	    /* Need to verify lock - TBD */

	    tempVal = pDevInfo->targetID << RIO_HOST_ID_OFFSET;
	    
	    VXB_RIO_WRITE(M85XXRIO_BDIDCSR(pDrvCtrl->deviceBase),tempVal);

	    tempVal = VXB_RIO_READ(M85XXRIO_BDIDCSR(pDrvCtrl->deviceBase));
			
	    }

	/* if not processor device is unsupported for now */
	if(pefcar.desc.sw == 1)
	    {
	    printf("Switch not supported need to configure before moving on\n");
	    /* assume already configured */
	    continue;
	    }

	}
	
    }



void RioDiscoveryScan(void);
extern int sysGetCpuID(void);

#endif
/*****************************************************************************
*
* m85xxRioInstInit2 - second stage device connection
*
* This is for a bus bridge, and it should already be configured by the first
* phase (instance creation) of device configuration.  We don't need to do
* anything during this phase.
*
* RETURNS: N/A
*
*/
LOCAL void m85xxRioInstInit2
    (
    VXB_DEVICE_ID pInst
    )
    {
    struct  m85xxRioDrvCtrl *pDrvCtrl;
    char *regBase = pInst->pRegBase[0];
    struct getDevInfo localGetDevInfo;
    struct getCPUID localGetCPUID;
    int index;
    UINT32 hostIndex = 0;




    RIO_CPU_INFO *pCpuInfo;
    UINT32 /*thisTargetIndex,thisTargetId,dataVal,*/flags;
    struct m85xxLawBarInfo	lawBarInfo;
    /*FUNCPTR         method;*/
    VXB_DEVICE_ID pInstLawBar = (VXB_DEVICE_ID)NULL;
    struct vxbBusPresent* busPresent;
    /*STATUS status;*/
    UINT32 pescsr;
	
    pDrvCtrl = (struct m85xxRioDrvCtrl *)pInst->pDrvCtrl;

    M85XXRIO_LOG("enter the  m85xxRioInstInit2\n",1,2,3,4,5,6);

#ifdef SM_DIRECT_CALLS
    vxbBusIntAckHook = m85xxRioBusIntAck;
    vxbBusTasHook = m85xxRioBusTas;
    vxbBusTasClearHook = m85xxRioBusTasClear;
    vxbBusIntGenHook = m85xxRioBusIntGen;
#endif

    /* if no pDrvCtrl, try again */
    if ( pDrvCtrl == NULL )
	{
        m85xxRioInstInit(pInst);
	pDrvCtrl = (struct m85xxRioDrvCtrl *)pInst->pDrvCtrl;
	}

    /* if not already initialized, try again */

    if ( pDrvCtrl->bridgeStatus == RIO_NOT_INITIALIZED )
        m85xxRioInstInit(pInst);


    /* Intialize area for storing cpu dev id and devInfo */
    pCpuInfo = (RIO_CPU_INFO*)hwMemAlloc(sizeof(RIO_CPU_INFO) * MAX_NUMBER_RIO_TARGETS);
    ((RAPIDIO_DEVICE_INFORMATION*)pInst->pBusSpecificDevInfo)->pCpuInfo = pCpuInfo;
    /* Populate cpu info for use by utility functions */
    for(index = 0; index < MAX_NUMBER_RIO_TARGETS;index++)
	{
	localGetDevInfo.devUnit = index;
	localGetDevInfo.pDevInfo = 0;
	localGetCPUID.devUnit = index;
	localGetCPUID.devID = (VXB_DEVICE_ID)0;
#if 0
	vxbSubDevAction(pInst,
			m85xxRioGetDevInfo,
			(char *)&localGetDevInfo,
			VXB_ITERATE_INSTANCES);

	vxbSubDevAction(pInst,
			m85xxRioGetCPUID,
			(char *)&localGetCPUID,
			VXB_ITERATE_INSTANCES);
#endif
	pCpuInfo[index].pDevInfo = localGetDevInfo.pDevInfo;

	pCpuInfo[index].devID = localGetCPUID.devID;

	if(pCpuInfo[index].pDevInfo==NULL)
	    break;
						
	if(pCpuInfo[index].pDevInfo->devUnit == 0)
	    {
	    hostIndex = index;
	    }
	}

    flags = VXBAF_BYTE_ORDER_BE_32;
    pDrvCtrl = pInst->pDrvCtrl;

    /* verify presence/functionality of hardware */

    pescsr = VXB_RIO_READ(M85XXRIO_PESCSR(regBase));
    VXB_RIO_WRITE(M85XXRIO_PESCSR(regBase),pescsr);
    pescsr = VXB_RIO_READ(M85XXRIO_PESCSR(regBase));


    if ( ( pescsr & SRIO_STATUS_CHECK ) != SRIO_STATUS_CHECK )
        {
        pDrvCtrl->bridgeStatus = RIO_NO_HARDWARE;
        M85XXRIO_LOG ("RapidIO Hardware not connnected\n",1,2,3,4,5,6);
    	return;
        }

    /* clear the status register */
	
    VXB_RIO_WRITE(M85XXRIO_PESCSR(regBase),pescsr);

    if  ( pescsr != SRIO_STATUS_CHECK )
	{
	pDrvCtrl->bridgeStatus = RIO_NO_HARDWARE;
	M85XXRIO_LOG ("RapidIO Hardware has errors:status:0x%x\n",pescsr,2,3,4,5,6);
	return;
	}


    pescsr = VXB_RIO_READ(M85XXRIO_PGCCSR(regBase));
    if ((pescsr & PGCCSR_HOST) != PGCCSR_HOST)
	{ 	
	M85XXRIO_LOG("This is the AGENT so wait for intialization by the host 0x%x\n",pescsr,2,3,4,5,6);
#if 0
	while ((pescsr & PGCCSR_MASTER)!= PGCCSR_MASTER)
	    {
	    pescsr = VXB_RIO_READ(M85XXRIO_PGCCSR(regBase));
	    }
#endif
	pDrvCtrl->host = FALSE;
	
	}
    else
	{
	pDrvCtrl->host = TRUE;
	M85XXRIO_LOG("This is the HOST 0x%x\n",pescsr,2,3,4,5,6);
	}    		
		


    /* CPU requires a local access window to be defined for rapidIO.
       There is a method for allocating this which is used */
    lawBarInfo.index = -1;
    lawBarInfo.baseAdrs = pDrvCtrl->deviceBase;
    lawBarInfo.size = ADR_SIZE_256MB;
    lawBarInfo.interface = pDrvCtrl->tgtIf;

    busPresent = pPlbBus;

    {
    BOOL match = FALSE;
	
    while((match == FALSE) && (busPresent != NULL))
	{

	for ( pInstLawBar = busPresent->instList ; pInstLawBar != NULL ; pInstLawBar = pInstLawBar->pNext )
	    {
	    if(strcmp(pInstLawBar->pDriver->drvName,"m85xxCCSR") == 0)
		{
		match=TRUE;
		break;
		}
	    else if(strcmp(pInstLawBar->pName,"m85xxCCSR") == 0)
		{
		match=TRUE;
		break;
		}

	    }
	busPresent = busPresent->pNext;

	}
    }

M85XXRIO_LOG("exit m85xxRioInstInit2!!\n",1,2,3,4,5,6);
/*
    if (sysGetCpuID() == 0)
	    RioDiscoveryScan();*/
	return;	



	
#if 0



    method = vxbDevMethodGet(pInstLawBar,(UINT32)&m85xxLawBarAllocSet_desc[0]);
	
    if(method!=NULL)
	{
	status = (*method)(pInstLawBar,pInst,&lawBarInfo);
	
	if(status == ERROR)
	    {
	    printf ("LawBar AllocSetStatus Error\n");
		M85XXRIO_LOG("LawBar AllocSetStatus Error\n",1,2,3,4,5,6);
	    return;
	    }
	
	}
    else
	{
	printf ("No method\n");
	M85XXRIO_LOG("No method\n",1,2,3,4,5,6);
	/* added by freeWinder */
	if (sysGetCpuID() == 0)
	    RioDiscoveryScan();
	return;	
	}

    /* allocate LAWBAR/LAWAR */
    pDrvCtrl->lawbar = *(UINT32*)lawBarInfo.lawbar;
    pDrvCtrl->lawar = *(UINT32*)lawBarInfo.lawar;
    pDrvCtrl->lawbarIndex = lawBarInfo.index;
	
		
    /* Here can now try to initialize Windows */
    /* First we discover the devices specified in hwconf */	
	

    if(pDrvCtrl->host == TRUE)
	{
    /* added by freeWinder */
	RioDiscoveryScan();
    
	
	m85xxRioTargetIdInit(pInst);
	}	

    /* Read local ID */

    dataVal = VXB_RIO_READ(M85XXRIO_BDIDCSR(regBase));

    if(pDrvCtrl->host == FALSE)
	{
	/* We can now read the agent configuration */
	/* Should base this on the target ID read from local registers */
	thisTargetId = dataVal >> RIO_HOST_ID_OFFSET;
	index = 0;

	while(pCpuInfo[index].pDevInfo->targetID != thisTargetId)
	    {
	    M85XXRIO_LOG("thisTargetId 0x%x 0x%x\n",thisTargetId,pCpuInfo[index].pDevInfo->targetID,1,2,3,4);
	    if (index > 9 )
		break;
	    index++;
	    }
		
	thisTargetIndex = index;
		
	/* Expect following : */
	/* Maintenance reads and writes only allowed on host */
	/* Channel 0 - outbound reserved */
	if(pCpuInfo[thisTargetIndex].pDevInfo->pChannelInfo[0].channelType != RIO_CHANNEL_RESERVED)
	    {
	    M85XXRIO_LOG("Unexpected use of Outbound Window 0\n",1,2,3,4,5,6);
	    }
				      				
	/* Channel 1 - outbound local sm memory on host */
	if(pCpuInfo[thisTargetIndex].pDevInfo->pChannelInfo[1].channelType != RIO_CHANNEL_SM)
	    {
	    M85XXRIO_LOG("Unexpected use of Outbound Window 1\n",1,2,3,4,5,6);
	    }
	else
	    {
	    /* Shared Memory Driver needs this CPU based address */
	    /* Need to store it for access */
	    }
			
	dataVal = VXB_RIO_READ(M85XXRIO_ROWBARn(regBase,1));
	rioHostAdrs = (char*)(dataVal << 12);
	
	M85XXRIO_LOG("rioHostAdrs 0x%x\n",rioHostAdrs,1,2,3,4,5);
		
	/* Channel 2 - outbound Test/Set */
	if(pCpuInfo[thisTargetIndex].pDevInfo->pChannelInfo[2].channelType != RIO_CHANNEL_TAS_SET)
	    {
	    M85XXRIO_LOG("Unexpected use of Outbound Window 2\n",1,2,3,4,5,6);
	    }
	else
	    {
	    /* Shared Memory Driver needs this CPU based address */
	    /* Need to store it for access */
	    }

	if(pCpuInfo[thisTargetIndex].pDevInfo->pChannelInfo[3].channelType != RIO_CHANNEL_TAS_CLEAR)
	    {
	    M85XXRIO_LOG("Unexpected use of Outbound Window 3\n",1,2,3,4,5,6);
	    }
	else
	    {
	    /* Shared Memory Driver needs this CPU based address */
	    /* Need to store it for access */
	    }
	/* Trying to share TAS Set/Clear as one window */
	dataVal = VXB_RIO_READ(M85XXRIO_ROWBARn(regBase,2));
	rioTasSetAdrs = (char*)(dataVal << 12);

	/* Trying to share TAS Set/Clear as one window */
	dataVal = VXB_RIO_READ(M85XXRIO_ROWBARn(regBase,3));

	rioTasClearAdrs = (char*)(dataVal << 12);

	M85XXRIO_LOG("rioTasSetAdrs 0x%x\n",rioTasSetAdrs,1,2,3,4,5);
		
		
	
	/* No inbound windows used at the moment on the agent*/
		
	/* Initialize inbound/outbound doorbell interrupt */
		
	if(sysProcNumGet() != pCpuInfo[thisTargetIndex].pDevInfo->pHcfDevice->devUnit )
	    M85XXRIO_LOG("Processor number doesn't match expected device Unit of this CPU\n Proc 0x%x devUnit 0x%x",sysProcNumGet(),pCpuInfo[thisTargetIndex].pDevInfo->pHcfDevice->devUnit,3,4,5,6);


	m85xxRioBusIntInit(0,0,thisTargetId);

	return;
	}
	
    pCpuInfo[hostIndex].pDevInfo->targetID =  dataVal >> RIO_HOST_ID_OFFSET;


    m85xxRioCfgInit();
#endif
    }


/*******************************************************************************
*
* m85xxRioInstConnect - third stage
*
* RETURNS: N/A
*
*/
LOCAL void m85xxRioInstConnect
    (
    VXB_DEVICE_ID pInst
    )
    {

    }

#if 0

UINT32 srioEnum(UINT32 hopcount)
{
		int i,k;	
		UINT32 dataVal,temp,tempID,hostPort,portNum;

		/*   (deviceID << 22) | (hopCount << 12) | (offset>>20);*/   
		*(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) = (0xff << 22) | (hopcount << 12) | (0>>20);
				
		portNum = *(UINT32*)(RAPIDIO_ADRS + RIO_SW_PORT)>>8&0xff;
		hostPort = *(UINT32*)(RAPIDIO_ADRS + RIO_SW_PORT)&0xff;
		printstr("the hostPort is:");
		printnum(hostPort);
		printstr("tsi578 port total is:0x");
		printnum(portNum);
		printnum(*(UINT32*)(RAPIDIO_ADRS + RIO_SW_PORT));
		tempID = (*(UINT32*)(CCSBAR + 0xc0010)>>4) & 0x1;
	for(i=0;i<portNum;i++)
	{
		if(i==hostPort&&hopcount==0)
		{
			printstr("this  port");
			printnum(i);
			printstr("attach host!\r\n");
			continue;
		}
		temp = *(UINT32*)(RAPIDIO_ADRS + (0x158+ i*0x20))&0x1;
		if(temp==0)
		{	
			printstr("into system!\r\n");
			*(UINT32*)(RAPIDIO_ADRS+RIO_ROUTE_CFG_DESTID) = 0x000000ff;
			*(UINT32*)(RAPIDIO_ADRS+RIO_ROUTE_CFG_PORT) = i;
			while(*(UINT32*)(RAPIDIO_ADRS+RIO_ROUTE_CFG_PORT) != i);
/*
			{
				*(UINT32*)(0xc0000000+0x70) = 0x000000ff;
				*(UINT32*)(0xc0000000+0x74) = i;
			}
*/
			/*   (deviceID << 22) | (hopCount << 12) | (offset);*/     	
			*(UINT32*)(CCSBAR + 0xd0c20) = (0xff << 22) | (hopcount+1 << 12) ; 			
			while(*(UINT32*)(CCSBAR + 0xd0c20) != ((0xff << 22) | (hopcount+1 << 12) ));
/*				*(UINT32*)(CCSBAR + 0xd0c20) = (0xff << 22) | (0x1 << 12); */
			#if 1
			if(tempID==0)/*small system*/
			{							
				if(*(UINT32*)(RAPIDIO_ADRS+PEFCAR_OFFSET)>>28&0x1==1) /*is switch*/
				{
					dataVal = *(UINT32*)(RAPIDIO_ADRS+RIO_COMP_TAG);
					if(dataVal==0x0)
					{
						*(UINT32*)(RAPIDIO_ADRS+RIO_COMP_TAG)=0x1;
						srioEnum(hopcount+1);
					}
					else
						break;

				}	
				else
				{
					*(UINT32*)(RAPIDIO_ADRS+M86XXRIO_BDIDCSR_OFFSET) = destID<<16;
					while(*(UINT32*)(RAPIDIO_ADRS+M86XXRIO_BDIDCSR_OFFSET) != destID<<16);
/*						*(UINT32*)(0xc0000000+0x60) = destID<<16;*/
				}
			}
			else
			{				
				if(*(UINT32*)(RAPIDIO_ADRS+PEFCAR_OFFSET)>>28&0x1==1)
					srioEnum(hopcount+1);
				else
				{
					*(UINT32*)(RAPIDIO_ADRS+PEFCAR_OFFSET)= destID;
					while(*(UINT32*)(RAPIDIO_ADRS+PEFCAR_OFFSET) != destID);
/*						*(UINT32*)(0xc0000000+0x60) = destID;*/
				}
			}
			#endif
			printstr("the  attach i is:");
			printnum(i);
			printstr("the destID is:");
	/*		printnum(*(UINT32*)(0xc0000000+0x60)); */
			*(UINT32*)(CCSBAR + 0xd0c20) = (0xff << 22) | (hopcount << 12) ; 
			while(*(UINT32*)(CCSBAR + 0xd0c20) != ((0xff << 22) | (hopcount << 12)));
/*				*(UINT32*)(CCSBAR + 0xd0c20) = ((0xff << 22) | (0x0 << 12));*/
			*(UINT32*)(RAPIDIO_ADRS+RIO_ROUTE_CFG_DESTID) = destID;
			*(UINT32*)(RAPIDIO_ADRS+RIO_ROUTE_CFG_PORT) = i;
			while(*(UINT32*)(0xc0000000+RIO_ROUTE_CFG_PORT) != i);
		}
		destID++;	
		printstr("the i is:");
		printnum(i);
	}

	return 0;

}



#if 1
void RioDiscoveryScan()
{
	volatile int i,j,regVal,devIdAssign;
	int portNum,hostPortNum,portNum1,hostPortNum1;
    	M85XXRIO_LOG("enter RioDiscoveryScan\n",1,2,3,4,5,6);

    /*while(((*(int*)0xe100000c)&0xf) != 0x1);*/
	/* set the maintenence for device adja                                        targetID              hop              maintenance offset*/
    *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) = (0xff << 22) | (0 << 12) | (0>>20);
	*(volatile unsigned int*)(CCSBAR+0xd0000+0xc28) = (0xc0000000 >> 12);
	*(volatile unsigned int*)(CCSBAR+0xd0000+0xc30) = 0x80077013;
	devIdAssign = 1;
    while(*(volatile unsigned int*)(CCSBAR+0xd0000+0xc30) != 0x80077013);	
    
    M85XXRIO_LOG("exit while\n",1,2,3,4,5,6);

/*    srioEnum(0);*/

	if((*(volatile unsigned int*)0xc0000000) == 0x0578000d)
	{
	    M85XXRIO_LOG("find a switch TSI578\n",1,2,3,4,5,6);
		/* get the switch port status */
		regVal = *(volatile unsigned int*)0xc0000014;
	    portNum = (regVal>>8) & 0xff;
	    hostPortNum = regVal & 0xff;
        M85XXRIO_LOG("portNum is %d, hostportNum is %d\n",portNum,hostPortNum,3,4,5,6);
		/* read the lock reg */
		regVal = *(volatile unsigned int*)0xc0000068;
		if (regVal != 0xffff)
		{
		    M85XXRIO_LOG("switch TSI578 has locked by another host\n",1,2,3,4,5,6);
			return;
		}
		
		*(volatile unsigned int*)0xc0000068=0x00;
		
		regVal = *(volatile unsigned int*)0xc0000068;
		if (regVal != 0x00)
		{
		    M85XXRIO_LOG("lock switch TSI578 error\n",1,2,3,4,5,6);
			return;
		}

		/* route the response packet to host port */
		*(volatile unsigned int*)0xc0000070 = 0x00;
		*(volatile unsigned int*)0xc0000074 = hostPortNum;

        M85XXRIO_LOG("scan switch TSI578 port\n",1,2,3,4,5,6);
		for (i=0; i<portNum; i++)
		/*for (i=6;i<9;i++)*/
		{
			/*M85XXRIO_LOG("port %d: reg offset 0x%x = 0x%x\n",i,(0x158+i*0x20),*(volatile unsigned int*)(0xc0000000+0x158+i*0x20),4,5,6);*/
			if(((*(volatile unsigned int*)(0xc0000000+0x158+i*0x20)) & 0x2)!=0x2)
				continue;
			if ((i==hostPortNum))
				continue;
			M85XXRIO_LOG("find a device from port %d \n",i,2,3,4,5,6);
            *(volatile unsigned int*)(0x20000000+i*4) = 0x12340000+i;
            *(volatile unsigned int*)0xc0000070 = 0xff;
		    *(volatile unsigned int*)0xc0000074 = i;
			if(*(volatile unsigned int*)0xc0000070 != 0xff)
				M85XXRIO_LOG("added 0xff route error\n",1,2,3,4,5,6);
			
			if(*(volatile unsigned int*)0xc0000074 != i)
				M85XXRIO_LOG("added 0xff route port error\n",1,2,3,4,5,6);	
			
            *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) = (0xff << 22) | (0x1 << 12) | (0>>20);
			if(*(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) != ((0xff << 22) | (0x1 << 12) | (0>>20)))
				M85XXRIO_LOG("route tgtID 0xff error\n",1,2,3,4,5,6);	/* read, assure the write is finished */
		
			if(((*(volatile unsigned int*)0xc0000000) != 0x70110002)&&((*(volatile unsigned int*)0xc0000000) != 0x0578000d))
			{
			
				M85XXRIO_LOG("didn't support a device other than mpc8641d and Tsi578\n",1,2,3,4,5,6);
				*(volatile unsigned int*)0x1fffff04=*(volatile unsigned int*)0xc0000000;
				*(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) = (0xff << 22) | (0x00 << 12) | (0>>20);
				continue;
			}

			/* lock the reg */
			/*M85XXRIO_LOG("lock the device\n",1,2,3,4,5,6);*/
			if((*(volatile unsigned int*)0xc0000000) == 0x70110002)
            {
                M85XXRIO_LOG("It is MPC8640d\n",1,2,3,4,5,6);
                regVal = *(volatile unsigned int*)0xc0000068;
		        if (regVal != 0xffff)
		        {
		            M85XXRIO_LOG("mpc8641d has locked by another host\n",1,2,3,4,5,6);
			        return;
		        }
		
		        *(volatile unsigned int*)0xc0000068=0x00;
		
		        regVal = *(volatile unsigned int*)0xc0000068;
		        if (regVal != 0x00)
		        {
		            M85XXRIO_LOG("lock mpc8641d error\n",1,2,3,4,5,6);
			        return;
		        }
                /* update the devID */
			    /*M85XXRIO_LOG("update devID\n",1,2,3,4,5,6);*/
			    *(volatile unsigned int*)0xc0000060 = devIdAssign<<16;
				regVal = *(volatile unsigned int*)0xc0000060; /* read, assure the write is finished */
			
			    /*M85XXRIO_LOG("update route\n",1,2,3,4,5,6);*/
			    *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) = (0xff << 22) | (0x00 << 12) | (0>>20);
                regVal = *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20);/* read, assure the write is finished */
			
			    *(volatile unsigned int*)0xc0000070 = devIdAssign;
			    *(volatile unsigned int*)0xc0000074 = i;
			    regVal = *(volatile unsigned int*)0xc0000074;/* read, assure the write is finished */

		        *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) = (devIdAssign << 22) | (0xff << 12) | (0>>20);  
			    regVal = *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20);/* read, assure the write is finished */
			    /*read back DID and enable master */
#if 1
		   	    *(volatile unsigned int*)0xc000013c |= PGCCSR_MASTER;
			    while(((*(volatile unsigned int*)0xc000013c)&PGCCSR_MASTER) != PGCCSR_MASTER);
#endif
			    devIdAssign++;
				
                
			}
	        if((*(volatile unsigned int*)0xc0000000) == 0x0578000d)
	        {
        	    M85XXRIO_LOG("it is TSI578\n",1,2,3,4,5,6);
        		/* get the switch port status */
        		regVal = *(volatile unsigned int*)0xc0000014;
        	    portNum1 = (regVal>>8) & 0xff;
        	    hostPortNum1 = regVal & 0xff;
	            M85XXRIO_LOG("portNum1 is %d, hostportNum1 is %d\n",portNum1,hostPortNum1,3,4,5,6);
	        	/* read the lock reg */
        		regVal = *(volatile unsigned int*)0xc0000068;
        		if (regVal != 0xffff)
        		{
        		    M85XXRIO_LOG("switch TSI578 has locked by another host\n",1,2,3,4,5,6);
        			return;
        		}
        		
        		*(volatile unsigned int*)0xc0000068=0x00;
        		
        		regVal = *(volatile unsigned int*)0xc0000068;
        		if (regVal != 0x00)
        		{
        		    M85XXRIO_LOG("lock switch TSI578 error\n",1,2,3,4,5,6);
        			return;
        		}
			

        		/* route the response packet to host port */
        		*(volatile unsigned int*)0xc0000070 = 0x00;
        		*(volatile unsigned int*)0xc0000074 = hostPortNum1;

                M85XXRIO_LOG("scan switch TSI578 port\n",1,2,3,4,5,6);
        		for (j=0; j<portNum1; j++)
        		{
        		    if(((*(volatile unsigned int*)(0xc0000000+0x158+j*0x20)) & 0x2)!=0x2)
        				continue;
        			if (j==hostPortNum1)
        				continue;
        			M85XXRIO_LOG("find a device from port %d \n",j,2,3,4,5,6);


                    
					
                    *(volatile unsigned int*)(0x20000000+i*4) = 0x12340000+i;
                    *(volatile unsigned int*)0xc0000070 = 0xff;
        		    *(volatile unsigned int*)0xc0000074 = j;
                    if(*(volatile unsigned int*)0xc0000070 != 0xff)
        				M85XXRIO_LOG("added 0xff route error\n",1,2,3,4,5,6);
        			
        			if(*(volatile unsigned int*)0xc0000074 != j)
				        M85XXRIO_LOG("added 0xff route port error\n",1,2,3,4,5,6);	
			
                    *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) = (0xff << 22) | (0x2 << 12) | (0>>20);
			        if(*(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) != ((0xff << 22) | (0x2 << 12) | (0>>20)))
			        	M85XXRIO_LOG("route tgtID 0xff error\n",1,2,3,4,5,6);	/* read, assure the write is finished */
			
			        if(((*(volatile unsigned int*)0xc0000000) != 0x70110002)&&((*(volatile unsigned int*)0xc0000000) != 0x0578000d))
			        {
			            M85XXRIO_LOG("didn't support a device other than mpc8641d and Tsi578\n",1,2,3,4,5,6);
			        	*(volatile unsigned int*)0x1fffff04=*(volatile unsigned int*)0xc0000000;
				        *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) = (0xff << 22) | (0x00 << 12) | (0>>20);
				        continue;
			        }
			        /* lock the reg */
			        /*M85XXRIO_LOG("lock the device\n",1,2,3,4,5,6);*/
			        if((*(volatile unsigned int*)0xc0000000) == 0x70110002)
                    {
                        M85XXRIO_LOG("It is MPC8640d\n",1,2,3,4,5,6);
                        regVal = *(volatile unsigned int*)0xc0000068;
		                if (regVal != 0xffff)
		                {
		                    M85XXRIO_LOG("mpc8641d has locked by another host\n",1,2,3,4,5,6);
			                return;
		                }
		
		                *(volatile unsigned int*)0xc0000068=0x00;
		
		                regVal = *(volatile unsigned int*)0xc0000068;
		                if (regVal != 0x00)
		                {
		                    M85XXRIO_LOG("lock mpc8641d error\n",1,2,3,4,5,6);
			                return;
		                }
                        /* update the devID */
			            /*M85XXRIO_LOG("update devID\n",1,2,3,4,5,6);*/
			            *(volatile unsigned int*)0xc0000060 = devIdAssign<<16;
				        regVal = *(volatile unsigned int*)0xc0000060; /* read, assure the write is finished */
			
			            /*M85XXRIO_LOG("update route\n",1,2,3,4,5,6);*/
			            *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) = (0xff << 22) | (0x01 << 12) | (0>>20);
                        regVal = *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20);/* read, assure the write is finished */
			
			            *(volatile unsigned int*)0xc0000070 = devIdAssign;
			            *(volatile unsigned int*)0xc0000074 = j;
			            regVal = *(volatile unsigned int*)0xc0000074;/* read, assure the write is finished */

						/*M85XXRIO_LOG("update route\n",1,2,3,4,5,6);*/
			            *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) = (0xff << 22) | (0x00 << 12) | (0>>20);
                        regVal = *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20);/* read, assure the write is finished */
			
			            *(volatile unsigned int*)0xc0000070 = devIdAssign;
			            *(volatile unsigned int*)0xc0000074 = i;
			            regVal = *(volatile unsigned int*)0xc0000074;/* read, assure the write is finished */

		                *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) = (devIdAssign << 22) | (0xff << 12) | (0>>20);  
			            regVal = *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20);/* read, assure the write is finished */
			            /*read back DID and enable master */
		   	            *(volatile unsigned int*)0xc000013c |= PGCCSR_MASTER;
			            while(((*(volatile unsigned int*)0xc000013c)&PGCCSR_MASTER) != PGCCSR_MASTER);
						devIdAssign++;
			
                        *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) = (0xff << 22) | (0x01 << 12) | (0>>20);
                        regVal = *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20);/* read, assure the write is finished */
			        }
					
        	    }
	        }

			*(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) = (0xff << 22) | (0x00 << 12) | (0>>20);
            regVal = *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20);/* read, assure the write is finished */
		}
		
        M85XXRIO_LOG("end RioDiscoveryScan\n",1,2,3,4,5,6);
		
	}
	M85XXRIO_LOG("exit RioDiscoveryScan\n",1,2,3,4,5,6);

}
#else
void RioDiscoveryScan()
{
	volatile int i,j,regVal,devIdAssign;
	UINT32 reg;
	int portNum,hostPortNum,portNum1,hostPortNum1;
    	M85XXRIO_LOG("enter RioDiscoveryScan\n",1,2,3,4,5,6);

    /*while(((*(int*)0xe100000c)&0xf) != 0x1);*/
	/* set the maintenence for device adja                                        targetID              hop              maintenance offset*/
    *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) = (0xff << 22) | (0 << 12) | (0>>20);
	*(volatile unsigned int*)(CCSBAR+0xd0000+0xc28) = (0xc0000000 >> 12);
	*(volatile unsigned int*)(CCSBAR+0xd0000+0xc30) = 0x80077013;
	devIdAssign = 1;
    while(*(volatile unsigned int*)(CCSBAR+0xd0000+0xc30) != 0x80077013);	
    
    M85XXRIO_LOG("exit while\n",1,2,3,4,5,6);

/*    srioEnum(0);*/
#if 1	
	if((*(volatile unsigned int*)0xc0000000) == 0x0578000d)
	{
	    M85XXRIO_LOG("find a switch TSI578\n",1,2,3,4,5,6);
		/* get the switch port status */
		regVal = *(volatile unsigned int*)0xc0000014;
	    portNum = (regVal>>8) & 0xff;
	    hostPortNum = regVal & 0xff;
        M85XXRIO_LOG("portNum is %d, hostportNum is %d\n",portNum,hostPortNum,3,4,5,6);
		/* read the lock reg */
		regVal = *(volatile unsigned int*)0xc0000068;
		if (regVal != 0xffff)
		{
		    M85XXRIO_LOG("switch TSI578 has locked by another host\n",1,2,3,4,5,6);
			return;
		}
		
		*(volatile unsigned int*)0xc0000068=0x00;
		
		regVal = *(volatile unsigned int*)0xc0000068;
		if (regVal != 0x00)
		{
		    M85XXRIO_LOG("lock switch TSI578 error\n",1,2,3,4,5,6);
			return;
		}

		/* route the response packet to host port */
		*(volatile unsigned int*)0xc0000070 = 0x00;
		*(volatile unsigned int*)0xc0000074 = hostPortNum;

        M85XXRIO_LOG("scan switch TSI578 port\n",1,2,3,4,5,6);
		for (i=0; i<portNum; i++)
		/*for (i=6;i<7;i++)*/
		{
		    if(((*(volatile unsigned int*)(0xc0000000+0x158+i*0x20)) & 0x2)!=0x2)
				continue;
			if ((i==hostPortNum)||(i==0)) /*hdz*/
				continue;
			M85XXRIO_LOG("find a device from port %d \n",i,2,3,4,5,6);
            *(volatile unsigned int*)(0x20000000+i*4) = 0x12340000+i;
            *(volatile unsigned int*)0xc0000070 = 0xff;
		    *(volatile unsigned int*)0xc0000074 = i;
			if(*(volatile unsigned int*)0xc0000070 != 0xff)
				M85XXRIO_LOG("added 0xff route error\n",1,2,3,4,5,6);
			
			if(*(volatile unsigned int*)0xc0000074 != i)
				M85XXRIO_LOG("added 0xff route port error\n",1,2,3,4,5,6);	
			
            *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) = (0xff << 22) | (0x1 << 12) | (0>>20);
			if(*(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) != ((0xff << 22) | (0x1 << 12) | (0>>20)))
				M85XXRIO_LOG("route tgtID 0xff error\n",1,2,3,4,5,6);	/* read, assure the write is finished */
			
			if(((*(volatile unsigned int*)0xc0000000) != 0x70110002)&&((*(volatile unsigned int*)0xc0000000) != 0x0578000d))
			{
			    M85XXRIO_LOG("didn't support a device other than mpc8641d and Tsi578\n",1,2,3,4,5,6);
				*(volatile unsigned int*)0x1fffff04=*(volatile unsigned int*)0xc0000000;
				*(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) = (0xff << 22) | (0x00 << 12) | (0>>20);
				continue;
			}
			/* lock the reg */
			/*M85XXRIO_LOG("lock the device\n",1,2,3,4,5,6);*/
			if((*(volatile unsigned int*)0xc0000000) == 0x70110002)
            {
                M85XXRIO_LOG("It is MPC8640d\n",1,2,3,4,5,6);
                regVal = *(volatile unsigned int*)0xc0000068;
               
                
		        if (regVal != 0xffff)
		        {
		            M85XXRIO_LOG("mpc8641d has locked by another host\n",1,2,3,4,5,6);
			        return;
		        }
		
		        *(volatile unsigned int*)0xc0000068=0x00;
		
		        regVal = *(volatile unsigned int*)0xc0000068;
		        if (regVal != 0x00)
		        {
		            M85XXRIO_LOG("lock mpc8641d error\n",1,2,3,4,5,6);
			        return;
		        }
                /* update the devID */
			    M85XXRIO_LOG("update devID\n",1,2,3,4,5,6);
			    while(1)
			    {
			    	reg=*(volatile unsigned int*)0xc0000008; /*hdz add*/
			    	if(reg!=0) break; 
			    }
			    M85XXRIO_LOG("0xc0000008 = 0x%x  port = %d\n",reg,i,3,4,5,6);
		        devIdAssign=reg&0xffff;
			   *(volatile unsigned int*)0xc0000060 = devIdAssign<<16;
		        /* *(volatile unsigned int*)0xc0000060 = ((reg+1)&0xff)<<16; */
				regVal = *(volatile unsigned int*)0xc0000060; /* read, assure the write is finished */
			
			    /*M85XXRIO_LOG("update route\n",1,2,3,4,5,6);*/
			    *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) = (0xff << 22) | (0x00 << 12) | (0>>20);
                regVal = *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20);/* read, assure the write is finished */
			
			    *(volatile unsigned int*)0xc0000070 = devIdAssign;
			    *(volatile unsigned int*)0xc0000074 = i;
			    regVal = *(volatile unsigned int*)0xc0000074;/* read, assure the write is finished */

		        *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) = (devIdAssign << 22) | (0xff << 12) | (0>>20);  
			    regVal = *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20);/* read, assure the write is finished */
			    /*read back DID and enable master */
		   	    *(volatile unsigned int*)0xc000013c |= PGCCSR_MASTER;
			    while(((*(volatile unsigned int*)0xc000013c)&PGCCSR_MASTER) != PGCCSR_MASTER);
			    devIdAssign++;
				
                
			}
	        if((*(volatile unsigned int*)0xc0000000) == 0x0578000d)
	        {
        	    M85XXRIO_LOG("it is TSI578\n",1,2,3,4,5,6);
        		/* get the switch port status */
        		regVal = *(volatile unsigned int*)0xc0000014;
        	    portNum1 = (regVal>>8) & 0xff;
        	    hostPortNum1 = regVal & 0xff;
	            M85XXRIO_LOG("portNum1 is %d, hostportNum1 is %d\n",portNum1,hostPortNum1,3,4,5,6);
	        	/* read the lock reg */
        		regVal = *(volatile unsigned int*)0xc0000068;
        		if (regVal != 0xffff)
        		{
        		    M85XXRIO_LOG("switch TSI578 has locked by another host\n",1,2,3,4,5,6);
        			return;
        		}
        		
        		*(volatile unsigned int*)0xc0000068=0x00;
        		
        		regVal = *(volatile unsigned int*)0xc0000068;
        		if (regVal != 0x00)
        		{
        		    M85XXRIO_LOG("lock switch TSI578 error\n",1,2,3,4,5,6);
        			return;
        		}
			

        		/* route the response packet to host port */
        		*(volatile unsigned int*)0xc0000070 = 0x00;
        		*(volatile unsigned int*)0xc0000074 = hostPortNum1;

                M85XXRIO_LOG("scan switch TSI578 port\n",1,2,3,4,5,6);
        		for (j=0; j<portNum1; j++)
        		{
        		    if(((*(volatile unsigned int*)(0xc0000000+0x158+j*0x20)) & 0x2)!=0x2)
        				continue;
        			if (j==hostPortNum1)
        				continue;
        			M85XXRIO_LOG("find a device from port %d \n",j,2,3,4,5,6);


                    
					
                    *(volatile unsigned int*)(0x20000000+i*4) = 0x12340000+i;
                    *(volatile unsigned int*)0xc0000070 = 0xff;
        		    *(volatile unsigned int*)0xc0000074 = j;
                    if(*(volatile unsigned int*)0xc0000070 != 0xff)
        				M85XXRIO_LOG("added 0xff route error\n",1,2,3,4,5,6);
        			
        			if(*(volatile unsigned int*)0xc0000074 != j)
				        M85XXRIO_LOG("added 0xff route port error\n",1,2,3,4,5,6);	
			
                    *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) = (0xff << 22) | (0x2 << 12) | (0>>20);
			        if(*(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) != ((0xff << 22) | (0x2 << 12) | (0>>20)))
			        	M85XXRIO_LOG("route tgtID 0xff error\n",1,2,3,4,5,6);	/* read, assure the write is finished */
			
			        if(((*(volatile unsigned int*)0xc0000000) != 0x70110002)&&((*(volatile unsigned int*)0xc0000000) != 0x0578000d))
			        {
			            M85XXRIO_LOG("didn't support a device other than mpc8641d and Tsi578\n",1,2,3,4,5,6);
			        	*(volatile unsigned int*)0x1fffff04=*(volatile unsigned int*)0xc0000000;
				        *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) = (0xff << 22) | (0x00 << 12) | (0>>20);
				        continue;
			        }
			        /* lock the reg */
			        /*M85XXRIO_LOG("lock the device\n",1,2,3,4,5,6);*/
			        if((*(volatile unsigned int*)0xc0000000) == 0x70110002)
                    {
                        M85XXRIO_LOG("It is MPC8640d\n",1,2,3,4,5,6);
                        regVal = *(volatile unsigned int*)0xc0000068;
		                if (regVal != 0xffff)
		                {
		                    M85XXRIO_LOG("mpc8641d has locked by another host\n",1,2,3,4,5,6);
			                return;
		                }
		
		                *(volatile unsigned int*)0xc0000068=0x00;
		
		                regVal = *(volatile unsigned int*)0xc0000068;	        		         
		                if (regVal != 0x00)
		                {
		                    M85XXRIO_LOG("lock mpc8641d error\n",1,2,3,4,5,6);
			                return;
		                }
		                
		        	    M85XXRIO_LOG("update devID\n",1,2,3,4,5,6);
		        		while(1)
		        		{
		        			reg=*(volatile unsigned int*)0xc0000008; /*hdz add*/
		        			if(reg!=0) break; 
		        		}
		        		M85XXRIO_LOG("0xc0000008 = 0x%x  port = %d\n",reg,i,3,4,5,6);

				        devIdAssign=reg&0xffff;
				        
		                *(volatile unsigned int*)0xc0000060 = devIdAssign<<16;
		                *(volatile unsigned int*)0xc0000060 = (reg&0xff)<<16;
			            
				        regVal = *(volatile unsigned int*)0xc0000060; /* read, assure the write is finished */
			
			            /*M85XXRIO_LOG("update route\n",1,2,3,4,5,6);*/
			            *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) = (0xff << 22) | (0x01 << 12) | (0>>20);
                        regVal = *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20);/* read, assure the write is finished */
			
			            *(volatile unsigned int*)0xc0000070 = devIdAssign;
			            *(volatile unsigned int*)0xc0000074 = j;
			            regVal = *(volatile unsigned int*)0xc0000074;/* read, assure the write is finished */

						/*M85XXRIO_LOG("update route\n",1,2,3,4,5,6);*/
			            *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) = (0xff << 22) | (0x00 << 12) | (0>>20);
                        regVal = *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20);/* read, assure the write is finished */
			
			            *(volatile unsigned int*)0xc0000070 = devIdAssign;
			            *(volatile unsigned int*)0xc0000074 = i;
			            regVal = *(volatile unsigned int*)0xc0000074;/* read, assure the write is finished */

		                *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) = (devIdAssign << 22) | (0xff << 12) | (0>>20);  
			            regVal = *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20);/* read, assure the write is finished */
			            /*read back DID and enable master */
		   	            *(volatile unsigned int*)0xc000013c |= PGCCSR_MASTER;
			            while(((*(volatile unsigned int*)0xc000013c)&PGCCSR_MASTER) != PGCCSR_MASTER);
						devIdAssign++;
			
                        *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) = (0xff << 22) | (0x01 << 12) | (0>>20);
                        regVal = *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20);/* read, assure the write is finished */
			        }
					
        	    }
	        }

			*(volatile unsigned int*)(CCSBAR+0xd0000+0xc20) = (0xff << 22) | (0x00 << 12) | (0>>20);
            regVal = *(volatile unsigned int*)(CCSBAR+0xd0000+0xc20);/* read, assure the write is finished */
		}
		
        M85XXRIO_LOG("end RioDiscoveryScan\n",1,2,3,4,5,6);
		
	}
	M85XXRIO_LOG("exit RioDiscoveryScan\n",1,2,3,4,5,6);
	#endif
}

#endif



#ifdef M85XXRIO_DEBUG
/*******************************************************************************
*
* m85xxRioStatusShow - show state of RapidIO interface
*
* RETURNS: NONE
*
*/
int m85xxRioStatusShow
    (
    VXB_DEVICE_ID pInst
    )
    {
    struct m85xxRioDrvCtrl *	pDrvCtrl;

    pDrvCtrl = (struct m85xxRioDrvCtrl *)pInst->pDrvCtrl;

    if ( pDrvCtrl == NULL )
	{
	M85XXRIO_LOG("m85xxRioInstConnect(): no pDrvCtrl present\n",1,2,3,4,5,6);
        return(0);
	}

    switch ( pDrvCtrl->bridgeStatus )
        {
	case RIO_NO_HARDWARE:
	    M85XXRIO_LOG("m85xxRioInstConnect(): bridge status: no hardware (0x%08x)\n",
			  pDrvCtrl->bridgeStatus,2,3,4,5,6);
	    break;
	case RIO_NOT_INITIALIZED:
	    M85XXRIO_LOG("m85xxRioInstConnect(): bridge status: not initialized (0x%08x)\n",
			  pDrvCtrl->bridgeStatus,2,3,4,5,6);
	    break;
	case RIO_BRIDGE_ININTIALIZED:
	    M85XXRIO_LOG("m85xxRioInstConnect(): bridge status: initialized (0x%08x)\n",
			  pDrvCtrl->bridgeStatus,2,3,4,5,6);
	    break;
	case RIO_BUS_CONFIG_IN_PROGRESS:
	    M85XXRIO_LOG("m85xxRioInstConnect(): bridge status: initialization in progress (0x%08x)\n",
			  pDrvCtrl->bridgeStatus,2,3,4,5,6);
	    break;
	case RIO_BUS_CONFIG_DONE:
	    M85XXRIO_LOG("m85xxRioInstConnect(): bridge status: initialization complete (0x%08x)\n",
			  pDrvCtrl->bridgeStatus,2,3,4,5,6);
	    break;
	case RIO_WORKING:
	    M85XXRIO_LOG("m85xxRioInstConnect(): bridge status: running (0x%08x)\n",
			  pDrvCtrl->bridgeStatus,2,3,4,5,6);
	    break;
	default:
	    M85XXRIO_LOG("m85xxRioInstConnect(): ERROR: Unknown bridge status 0x%08x\n",
			  pDrvCtrl->bridgeStatus,2,3,4,5,6);
	    break;
	}

    return(0);
    }
#endif

/*******************************************************************************
*
* m85xxRioProbe - Check whether device is a RapidIO bridge
*
* The PowerPC 8540 and 8560 processors share the same hardware, but
* can be configured differently.  If configured as an 8540, the
* processor does not support RapidIO, but only PCI.  If configured
* as an 8560, the processor does not support PCI, but only RapidIO.
* For this reason, the check on whether this board supports RapidIO
* can be done by checking the processor configuration.  We do this
* by calling the sysModel() BSP routine, and checking the first
* seven characters of the returned string.
*
* NOTE: We do not check the actual device.  We trust the BSP
* configuration to provide the appropriate regBase value.  For
* this reason, the check does not actually involve the device
* itself.
*
* RETURNS: N/A
*
*/
LOCAL BOOL m85xxRioProbe
    (
    VXB_DEVICE_ID pInst
    )
    {
    return(TRUE);
    }


/*******************************************************************************
*
* m85xxRioCfgSetupWindow - set up a maintenance window for config transactions
*
*  This configures a maintenance window for the device.  Although
*  the access is to be made on behalf of the downstream device,
*  the VXB_DEVICE_ID here refers to the bus controller, and not
*  to the device making the request.
*
* RETURNS: N/A
*
*/
LOCAL void m85xxRioCfgSetupWindow
    (
    VXB_DEVICE_ID	pInst,	/* bus controller ID */
    char* 		adrs,
    UINT32		deviceID,
    UINT32		hopCount,
    UINT32		offset,
    UINT32		window
    )
    {
    UINT32		dataVal;
    UINT32		flags;
    struct m85xxRioDrvCtrl *	pDrvCtrl;
    char *regBase = pInst->pRegBase[0];

    pDrvCtrl = (struct m85xxRioDrvCtrl *)pInst->pDrvCtrl;

    if ( window == 0 )
        window = 1;

    flags = VXBAF_BYTE_ORDER_BE_32;

    dataVal = ( M85XXRIO_RxWAR_TYPE_READ(M85XXRIO_ROWAR_RDTYPE_MAINT_READ)  |
		M85XXRIO_RxWAR_TYPE_WRITE(M85XXRIO_ROWAR_WRTYPE_MAINT_WRITE) |
		M85XXRIO_RxWAR_EN |
		M85XXRIO_RxWAR_SIZE_1MB);
				
    M85XXRIO_LOG("ROWAR 0x%x 0x%x",M85XXRIO_ROWARn(regBase,window),dataVal,3,4,5,6);

    VXB_RIO_WRITE(M85XXRIO_ROWARn(regBase,window),dataVal);
		
    dataVal = (deviceID << 22) | (hopCount << 12) | (offset>>20);
    M85XXRIO_LOG("ROWTAR 0x%x 0x%x",M85XXRIO_ROWTARn(regBase,window),dataVal,3,4,5,6);
    VXB_RIO_WRITE(M85XXRIO_ROWTARn(regBase,window),dataVal);

    dataVal = (UINT32)adrs >> 12;
    M85XXRIO_LOG("ROWBAR 0x%x",dataVal,1,2,3,4,5);
    VXB_RIO_WRITE(M85XXRIO_ROWBARn(regBase,window),dataVal);

    dataVal = 0;

    dataVal= VXB_RIO_READ(M85XXRIO_ROWBARn(regBase,window));
    	
    }

/******************************************************************************
*
* m85xxRioRemoteCfgRead - read data from device configuration space
*
* RETURNS: OK if success, or ERROR
*
*/

LOCAL STATUS m85xxRioRemoteCfgRead
    (
    VXB_DEVICE_ID   pDev,           /* device info */
    UINT32          byteOffset,     /* offset into cfg space */
    UINT32          transactionSize,/* transaction size, in bytes */
    char *       pDataBuf,       /* buffer to read-from/write-to */
    UINT32 *        pFlags          /* flags */
    )
    {
    int				oldLock;
    struct m85xxRioDrvCtrl	*pDrvCtrl;
    RAPIDIO_DEVICE_INFORMATION	*pCpuDevInfo = (RAPIDIO_DEVICE_INFORMATION	*)0;
    VXB_DEVICE_ID pRioDrv;
    UINT8 byteWord;
    UINT16 shortWord;
    UINT32 longWord;
    UINT32 index,index2,flags,dataVal;
    int	window=0;
    UINT32 targetID;
    RIO_CPU_INFO *pCpuInfo;	
    targetID = *pFlags;
	
    pRioDrv = vxbDevParent(pDev);
    pDrvCtrl = pRioDrv->pDrvCtrl;
    pCpuInfo = ((RAPIDIO_DEVICE_INFORMATION*)pRioDrv->pBusSpecificDevInfo)->pCpuInfo;

    if(pDrvCtrl == NULL)
	return(ERROR);

    if(pDrvCtrl->deviceBase == NULL)
	return(ERROR);
	
    for(index = 0; index < MAX_NUMBER_RIO_TARGETS;index++)
	{
	pCpuDevInfo = pCpuInfo[index].pDevInfo;
	M85XXRIO_LOG("pCpuDevInfo->targetID 0x%x\n",pCpuDevInfo->targetID,2,3,4,5,6);	
	if(pCpuInfo[index].pDevInfo->targetID == targetID)
	    break;
	}
		
    if(index==MAX_NUMBER_RIO_TARGETS)
	return(ERROR);

    
    for (index2=0;index2<RIO_MAX_OUTBOUND_CHANNELS;index2++)
	{
	if(pCpuInfo[index].pDevInfo->pChannelInfo[index2].channelType == RIO_CHANNEL_MAINT)
	    {
	    window = index2;
	    break;
	    }	
	}
	
    if(index2 == RIO_MAX_OUTBOUND_CHANNELS)
	window = 1; /* use default */
	
    oldLock = intCpuLock();
		
    m85xxRioCfgSetupWindow
      (
      pDev,
      pDrvCtrl->deviceBase,
      targetID,
      pCpuDevInfo->hopCount,
      0,
      window
      );

    dataVal = 0;
    flags = VXBAF_BYTE_ORDER_BE_32;

    switch ( transactionSize )
	{
	case 1:
	    byteWord = VXB_RIO_READ8(((UINT32)pDrvCtrl->deviceBase + byteOffset));
	    *(UINT32*)pDataBuf = (UINT32)byteWord;
	    break;
	case 2:
	    shortWord = VXB_RIO_READ16(((UINT32)pDrvCtrl->deviceBase + byteOffset));
	    *(UINT32*)pDataBuf = (UINT32)shortWord;	
	    break;
	case 4:
	    longWord = VXB_RIO_READ(((UINT32)pDrvCtrl->deviceBase + byteOffset));
	    *(UINT32*)pDataBuf = (UINT32)longWord;	
	    break;
	    
	default:
	    intCpuUnlock(oldLock);
	    return(ERROR);
	}

    M85XXRIO_LOG("Read val 0x%x\n",*(UINT32*)pDataBuf,2,3,4,5,6);
    
    intCpuUnlock(oldLock);

    return(OK);
    }

/*******************************************************************************
*
* m85xxRioRemoteCfgWrite - write data to device configuration space
*
* RETURNS: OK if success, or ERROR
*
*/

LOCAL STATUS m85xxRioRemoteCfgWrite
    (
    VXB_DEVICE_ID   pDev,           /* device info */
    UINT32          byteOffset,     /* offset into cfg space */
    UINT32          transactionSize,/* transaction size, in bytes */
    char *          pDataBuf,       /* buffer to read-from/write-to */
    UINT32 *        pFlags          /* flags */
    )
    {
    int				oldLock;
    RAPIDIO_DEVICE_INFORMATION	*pCpuDevInfo = (RAPIDIO_DEVICE_INFORMATION	*)NULL;
    RIO_CPU_INFO *pCpuInfo;
    VXB_DEVICE_ID pRioDrv;
    struct m85xxRioDrvCtrl	*pDrvCtrl;
    VUINT32 index,flags,dataVal;
    VUINT16 dataValShort;
    VUINT8  dataValChar;
    int	   window = 0;
    UINT32 targetID;
		
    targetID = *pFlags;

    pRioDrv = vxbDevParent(pDev);
    pCpuInfo = ((RAPIDIO_DEVICE_INFORMATION*)pRioDrv->pBusSpecificDevInfo)->pCpuInfo;
    pDrvCtrl = pRioDrv->pDrvCtrl;

    if(pDrvCtrl == NULL)
	return(ERROR);

    if(pDrvCtrl->deviceBase == NULL)
	return(ERROR);

    for(index = 1; index < MAX_NUMBER_RIO_TARGETS;index++)
	{

	pCpuDevInfo=pCpuInfo[index].pDevInfo;
	M85XXRIO_LOG("pCpuDevInfo->targetID 0x%x\n",pCpuDevInfo->targetID,2,3,4,5,6);
	/* FIXME only works for first device */
	if(targetID == 0xff)
	    break;
	if(pCpuDevInfo->targetID == targetID)
	    break;
	}
		
    if(index==MAX_NUMBER_RIO_TARGETS)
	return(ERROR);

    for (index=0;index<RIO_MAX_OUTBOUND_CHANNELS;index++)
	{
	if(pCpuDevInfo->pChannelInfo[index].channelType == RIO_CHANNEL_MAINT)
	    {
	    window = index;
	    break;
	    }	
	}
	
    if(index == RIO_MAX_OUTBOUND_CHANNELS)
	window = 1; /* use default */

    oldLock = intCpuLock();

    m85xxRioCfgSetupWindow
      (
      pDev,
      pDrvCtrl->deviceBase,
      targetID,
      pCpuDevInfo->hopCount,
      0,
      window
      );


    flags = VXBAF_BYTE_ORDER_BE_32;

    dataVal = *(UINT32*)pDataBuf;
	
    switch ( transactionSize )
	{
	case 1:
	    dataValChar = (char)dataVal;
	    VXB_RIO_WRITE8(((UINT32)pDrvCtrl->deviceBase + byteOffset),dataValChar);
	    break;
	case 2:
	    dataValShort = (UINT16)dataVal;
            VXB_RIO_WRITE16(((UINT32)pDrvCtrl->deviceBase + byteOffset),dataValShort);
	    break;
	case 4:
	    VXB_RIO_WRITE(((UINT32)pDrvCtrl->deviceBase + byteOffset),dataVal);
	    break;
	
	default:
	    intCpuUnlock(oldLock);
	    return(ERROR);
	}

    dataVal = 0;

    dataVal = VXB_RIO_READ(((UINT32)pDrvCtrl->deviceBase + byteOffset));

    intCpuUnlock(oldLock);
	
    M85XXRIO_LOG("\ndata to be written 0x%x size %d targetId %d\n",dataVal,transactionSize,pCpuDevInfo->targetID,4,5,6);
		
    return(OK);
    }



/*******************************************************************************
*
* m85xxRioWindowAlloc - allocate dedicated Outbound Window channel to 
* downstream instance
*
* RETURNS: OK if success, or ERROR
*
*/
LOCAL STATUS m85xxWindowAlloc
    (
    VXB_DEVICE_ID pInst, 
    char * pArg
    )
    {
    RAPIDIO_DEVICE_INFORMATION	*pDevInfo;
    int			index;
    int			oldLock;
    VXB_DEVICE_ID rioDevID;
    RIO_WINDOW_ALLOC *pAlloc;
	
    pAlloc = (RIO_WINDOW_ALLOC *)pArg;
    pAlloc->channelNumber = (UINT32)-1;

    rioDevID = pAlloc->cpuID;

    if(rioDevID == NULL)
	return(ERROR);
				
    pDevInfo = rioDevID->pBusSpecificDevInfo;

    if(pAlloc->channelDirection == RIO_DIRECTION_OUTBOUND)
	{
	oldLock = intCpuLock();
	
	/* Allocating from device index */
	for(index=0;index<RIO_MAX_OUTBOUND_CHANNELS;index++)
	    {
	    if(pDevInfo->pChannelInfo[index].channelType == RIO_CHANNEL_UNRESERVED)
		{
		pDevInfo->pChannelInfo[index].channelType = RIO_CHANNEL_RESERVED;
		pAlloc->channelNumber = index;
		break;
		}
	    }	

	intCpuUnlock(oldLock);
	}
    else
	{
	oldLock = intCpuLock();
	
	/* Allocating from device index */
	for(index=0;index<RIO_MAX_INBOUND_CHANNELS;index++)
	    {
	    if(pDevInfo->pChannelInfo[index+RIO_MAX_OUTBOUND_CHANNELS].channelType == RIO_CHANNEL_UNRESERVED)
		{
		pDevInfo->pChannelInfo[index+RIO_MAX_OUTBOUND_CHANNELS].channelType = RIO_CHANNEL_RESERVED;
		pAlloc->channelNumber = index;
		break;
		}
				
	    }	

	intCpuUnlock(oldLock);
	}	

    if(pAlloc->channelNumber == (UINT32)-1)
	goto errorExit;


    hwMemFree((char*)rioDevID);

    return(OK);

    errorExit:
    M85XXRIO_LOG("Window Alloc Failed!\n",1,2,3,4,5,6);

    hwMemFree((char*)rioDevID);
    return(ERROR);

    }

#ifdef DEBUG

/******************************************************************
*
* displayRapidIOCfgRegs - displays RIO registers given a base address
*
* RETURNS: N/A
*
*/
void displayRapidIOCfgRegs
    (
    RAPIDIOCAR* rioRegs
    )
    {
    M85XXRIO_LOG("\n\ndeviceID 0x%x \n",rioRegs->deviceID,0,0,0,0,0);
    M85XXRIO_LOG("deviceInfo 0x%x \n",rioRegs->deviceInfo,0,0,0,0,0);
    M85XXRIO_LOG("assemblyID 0x%x \n",rioRegs->assemblyID,0,0,0,0,0);
    M85XXRIO_LOG("assemblyInfo 0x%x \n",rioRegs->assemblyInfo,0,0,0,0,0);
    M85XXRIO_LOG("processorFeatures 0x%x \n",rioRegs->processorFeatures,0,0,0,0,0);
    M85XXRIO_LOG("switchPortFeatures 0x%x \n",rioRegs->switchPortFeatures,0,0,0,0,0);
    M85XXRIO_LOG("sourceOperations 0x%x \n",rioRegs->sourceOperations,0,0,0,0,0);
    M85XXRIO_LOG("destOperations 0x%x \n",rioRegs->destOperations,0,0,0,0,0);
    M85XXRIO_LOG("msr 0x%x \n",rioRegs->msr,0,0,0,0,0);
    M85XXRIO_LOG("pwdcsr 0x%x \n",rioRegs->pwdcsr,0,0,0,0,0);
    M85XXRIO_LOG("pellccsr 0x%x \n",rioRegs->pellccsr,0,0,0,0,0);
    M85XXRIO_LOG("lcsba1csr 0x%x \n",rioRegs->lcsba1csr,0,0,0,0,0);
    M85XXRIO_LOG("bdidcsr 0x%x RIO ID %d\n",rioRegs->bdidcsr,((UINT32)(rioRegs->bdidcsr))>>16,0,0,0,0);
    M85XXRIO_LOG("hbdidlcsr 0x%x \n",rioRegs->hbdidlcsr,0,0,0,0,0);
    M85XXRIO_LOG("ctcsr 0x%x \n",rioRegs->ctcsr,0,0,0,0,0);
    }

void m85xxRioDrvCtrlShow(VXB_DEVICE_ID pInst)
    {
    struct m85xxRioDrvCtrl *	pDrvCtrl;

    pDrvCtrl = (struct m85xxRioDrvCtrl *)pInst->pDrvCtrl;

    M85XXRIO_LOG("RapidIO bridge @ 0x%08x, pDrvCtrl @ 0x%08x\n",
		  (int)pInst, (int)pDrvCtrl,3,4,5,6);

    M85XXRIO_LOG("\tLAWBAR%d @ 0x%08x\n",
		  pDrvCtrl->lawbarIndex, pDrvCtrl->lawbar,3,4,5,6);
    M85XXRIO_LOG("\tLAWAR%d @ 0x%08x\n",
		  pDrvCtrl->lawbarIndex, pDrvCtrl->lawar,3,4,5,6);

    switch (pDrvCtrl->bridgeStatus)
        {
	case RIO_NO_HARDWARE:
	    M85XXRIO_LOG("\tNo RapidIO bridge hardware available\n",1,2,3,4,5,6);
	    break;
	case RIO_NOT_INITIALIZED:
	    M85XXRIO_LOG("\tRapidIO bridge hardware not initialized\n",1,2,3,4,5,6);
	    break;
	case RIO_BRIDGE_ININTIALIZED:
	    M85XXRIO_LOG("\tRapidIO bridge hardware initialized\n",1,2,3,4,5,6);
	    break;
	case RIO_BUS_CONFIG_IN_PROGRESS:
	    M85XXRIO_LOG("\tRapidIO configuration in progress\n",1,2,3,4,5,6);
	    break;
	case RIO_BUS_CONFIG_DONE:
	    M85XXRIO_LOG("\tRapidIO configuration done\n",1,2,3,4,5,6);
	    break;
	case RIO_WORKING:
	    M85XXRIO_LOG("\tRapidIO working\n",1,2,3,4,5,6);
	    break;
	default:
	    M85XXRIO_LOG("\tIllegal bridge status value\n",1,2,3,4,5,6);
	    break;
	}

    }
	
#endif /* DEBUG */



/*******************************************************************
*
* m85xxRioCfgInit - Used to establish device IDs and info for
*                    configuration library
*
* RETURNS: NONE
*
*/
LOCAL void m85xxRioCfgInit()
    {
    VXB_DEVICE_ID pInst;
    struct vxbBusPresent* pBusPres;

    rioHost = TRUE;
	
    m85xxRioCfgCalled++;
    pBusPres = pPlbBus;
    while ( pBusPres != NULL )
	{
	for ( pInst = pBusPres->instList ; pInst != NULL ; pInst = pInst->pNext )
	    {
	    if(strcmp(pInst->pName,"m85xxRio") == OK)
		{
		m85xxRioCfgCalled++;
	
		if(m85xxRioCfg(pInst)==ERROR)
		    printf("RIO config FAILED\n");
		break;
		}
	    }
		
	pBusPres = pBusPres->pNext;
	}
    }

/*******************************************************************
*
* windowAllocGet - Allocate and return a window to use
*
* RETURNS: UINT32
*
*/
LOCAL UINT32 windowAllocGet
    (
    VXB_DEVICE_ID pInst,
    VXB_DEVICE_ID devID,
    UINT32 direction
    )
    {
    RIO_WINDOW_ALLOC windowAlloc;
    UINT32 channelNumber = (UINT32)-1;
    FUNCPTR method;
    STATUS retVal = ERROR;
	
    windowAlloc.cpuID = devID;
    windowAlloc.channelDirection = direction;
		
    method = vxbDevMethodGet(pInst,(UINT32)&m85xxWindowAlloc_desc[0]);
	
    if(method!=NULL)
	{
	retVal = (*method)(devID,&windowAlloc);
	channelNumber = windowAlloc.channelNumber;
	if(retVal == ERROR)
	    {
	    printf("Window Alloc Error\n");
	    return(channelNumber);
	    }
	
	}
    else
	printf("Window Alloc Channel %d\n",windowAlloc.channelNumber);
    return(channelNumber);

    }

/*******************************************************************
*
* channelTest - test to see if channel/window is available
*
* RETURNS: UINT32
*
*/
LOCAL UINT32 channelTest
    (
    RAPIDIO_DEVICE_INFORMATION *pDevInfoHost,
    UINT32 direction,
    UINT32 channelType
    )
    {
    int index;
    UINT32 channelNumber = (UINT32)-1;

    for(index=0;index<RIO_MAX_CHANNELS;index++)
	{
	if  ((pDevInfoHost->pChannelInfo[index].channelDirection == direction) && (pDevInfoHost->pChannelInfo[index].channelType == channelType))
	    {
	    channelNumber = pDevInfoHost->pChannelInfo[index].channelNumber;
	    }
	}

    return(channelNumber);

    }


/*******************************************************************
*
* m85xxRioCfg - main function for configuring agent rapidIO devices
*
* RETURNS: STATUS
*
*/
LOCAL STATUS m85xxRioCfg
    (
    VXB_DEVICE_ID pInst
    )
    {
    RAPIDIO_DEVICE_INFORMATION*pDevInfo = NULL;
    VXB_DEVICE_ID rioDev = NULL;
    RIO_CFG_WINDOW_INFO rioWindowInfo;
    STATUS retVal = ERROR;
    PEFCAR_DESC pefcar;
    UINT32 channelNumber = 0xffffffff;
    int index;
    RIO_CPU_INFO *pCpuInfo = ((RAPIDIO_DEVICE_INFORMATION*)pInst->pBusSpecificDevInfo)->pCpuInfo;
    struct m85xxRioDrvCtrl *pDrvCtrl = pInst->pDrvCtrl;

    /* Get number of devices in system and populate a device ID table*/
    /* Assume devUnit == 0 is the host*/
			
    pDevInfoHost = pCpuInfo[0].pDevInfo;
    rioDevHost = pCpuInfo[0].devID;

    /* Test if inbound window already allocated*/
    channelNumber = channelTest(pDevInfoHost,RIO_DIRECTION_INBOUND,RIO_CHANNEL_SM);

    /* if not allocate dynamically */
    if(channelNumber == -1)
	{
	channelNumber = windowAllocGet(pInst,rioDevHost,RIO_DIRECTION_INBOUND);
	}

    if(channelNumber == -1)
	return(ERROR);

    rioWindowInfo.hostLocalMemRioAdrs = rioAdrs[rioAdrsCnt++] + (UINT32)((struct m85xxRioDrvCtrl *)pInst->pDrvCtrl)->rioBusAdrs;

    rioSetIW(pInst,channelNumber,rioWindowInfo.hostLocalMemRioAdrs,RIO_WINDOW_SIZE,pDrvCtrl->localMemAdrs);

    hostID = pDevInfoHost->targetID;


    for(index=1;index<MAX_NUMBER_RIO_TARGETS;index++)
	{

	pDevInfo = pCpuInfo[index].pDevInfo;
	rioDev = pCpuInfo[index].devID;
			
				
	if(rioDev == (VXB_DEVICE_ID)0)
	    break;
				
	/* Do this for multiple targets if necessary*/

	retVal = m85xxRioRemoteCfgRead
	  (
	  rioDev,
	  M85XXRIO_PEFCAR_OFFSET,
	  sizeof(UINT32),
	  (char *)&pefcar.word,
	  (UINT32*)&pDevInfo->targetID
	  );

	if(retVal == ERROR)
	    printf("busCfgRead Error\n");
			
	/* host */	
	/* Test if inbound window already allocated*/

	channelNumber = channelTest(pDevInfoHost,RIO_DIRECTION_OUTBOUND,RIO_CHANNEL_CFG);
	if(channelNumber == -1)
	    {
	    channelNumber = windowAllocGet(pInst,rioDevHost,RIO_DIRECTION_OUTBOUND);
	    }

	rioWindowInfo.agentCfgWindow = channelNumber;

    	/* agent */
	
	channelNumber = channelTest(pDevInfo,RIO_DIRECTION_OUTBOUND,RIO_CHANNEL_SM);
	if(channelNumber == -1)
	    {
	    channelNumber = windowAllocGet(pInst,rioDev,RIO_DIRECTION_OUTBOUND);
	    }
	

	rioWindowInfo.agentMainWindow = channelNumber;

	channelNumber = channelTest(pDevInfo,RIO_DIRECTION_OUTBOUND,RIO_CHANNEL_TAS_SET);
	if(channelNumber == -1)
	    {
	    channelNumber = windowAllocGet(pInst,rioDev,RIO_DIRECTION_OUTBOUND);
	    }

	rioWindowInfo.agentAtomicSetWindow = channelNumber;

	/* Test if inbound window already allocated*/
	channelNumber = channelTest(pDevInfo,RIO_DIRECTION_OUTBOUND,RIO_CHANNEL_TAS_CLEAR);
	if(channelNumber == -1)
	    {
	    channelNumber = windowAllocGet(pInst,rioDev,RIO_DIRECTION_OUTBOUND);
	    }

	rioWindowInfo.agentAtomicClearWindow = channelNumber;
	rioWindowInfo.rioDev = rioDev;
	rioWindowInfo.tgtId = pDevInfo->targetID;

	if (pefcar.desc.proc == 1)
	    {
	    rioCpuCfg(pInst,&rioWindowInfo);
	    }
	else
	    {
	    if (pefcar.desc.bridge == 1)
		{
		M85XXRIO_LOG("Switch is unsupported at the moment\n",0,0,0,0,0,0);
		return(ERROR);
		}
	    else
		{
		if (pefcar.desc.sw == 1)
		    {
		    M85XXRIO_LOG("Switch is unsupported at the moment\n",0,0,0,0,0,0);
		    return(ERROR);
		    }

		else
		    {
		    M85XXRIO_LOG("Standard device is unsupported at the moment\n",0,0,0,0,0,0);
		    return(ERROR);
		    }
		}
	    }
			
	}
		
    return (OK);
    }


/********************************************************************
*
* rioCpuCfg - Try to found out cpu id and configure target appropriately
*             by calling that specific configuration routine.
*
* RETURNS: N/A
*
* ERRNO
*/
LOCAL void rioCpuCfg
    (
    VXB_DEVICE_ID pInst,
    RIO_CFG_WINDOW_INFO *winInfo
    )
    {
    UINT32  didcar;
    int index;
    STATUS retVal = ERROR;
    VXB_DEVICE_ID rioDev;

    rioDev = winInfo->rioDev;

    retVal = m85xxRioRemoteCfgRead
      (
      rioDev,
      0x0 /* DIDCAR OFFSET */,
      sizeof(UINT32),
      (char *)&didcar,
      (UINT32*)&winInfo->tgtId
      );

    M85XXRIO_LOG("DIDCAR 0x%x",didcar,0,0,0,0,0);

    for(index=0;index<(sizeof(cpuIdTable)/sizeof(CFG_TABLE));index++)
	{
	if (cpuIdTable[index].cpuId == didcar)
	    {
	    cpuIdTable[index].cfgRoutine(pInst,winInfo);
	
	    break;
	    }
	}

    if (index==(sizeof(cpuIdTable)/sizeof(CFG_TABLE)))
        M85XXRIO_LOG("Unrecognized DIDCAR 0x%x",didcar,0,0,0,0,0);

    }



/********************************************************************
*
* rio85XXCfg - 85xx cpu specific configuration routine.
*              Configures the target configuration registers to RIO so
*              host/target can communicate with each other
*
* RETURNS: N/A
*
* ERRNO
*/


LOCAL void rio85XXCfg
    (
    VXB_DEVICE_ID pInst,
    RIO_CFG_WINDOW_INFO *winInfo
    )
    {
    VXB_DEVICE_ID rioDev;
    UINT32 tempVal;
    STATUS retVal;
    char* agentMainLocalAdrs ;
    char* agentAtomicSetLocalAdrs;
    char* agentAtomicClearLocalAdrs;
    char* agentDoorbellLocalAdrs;
    char* hostDoorbellLocalAdrs;
    char* agentCCSBAR;
    char* agentRioAdrs;
    UINT32 deviceBase = (UINT32)((struct m85xxRioDrvCtrl *)pInst->pDrvCtrl)->deviceBase;

    agentMainLocalAdrs = (char*)deviceBase;

    agentAtomicSetLocalAdrs = (char*)((UINT32)localAdrs[winInfo->agentAtomicSetWindow] + deviceBase);
    agentAtomicClearLocalAdrs = (char*)((UINT32)localAdrs[winInfo->agentAtomicClearWindow] + deviceBase);
    agentDoorbellLocalAdrs = (char*)((UINT32)localAdrs[winInfo->agentDoorbellWindow] + deviceBase);
    hostDoorbellLocalAdrs = (char*)((UINT32)localAdrs[winInfo->hostDoorbellWindow] + deviceBase);
    /* host window count */

    agentRioAdrs = (char*)((UINT32)rioAdrs[rioAdrsCnt++] + (UINT32)((struct m85xxRioDrvCtrl *)pInst->pDrvCtrl)->rioBusAdrs);

    tempVal = ((UINT32)agentRioAdrs >> 3);

    rioDev = winInfo->rioDev;

    retVal = m85xxRioRemoteCfgWrite
      (
      rioDev,
      M85XXRIO_LCSBA1CSR_OFFSET,
      sizeof(UINT32),
      (char *)&tempVal,
      (UINT32*)&winInfo->tgtId
      );


    rioSetOW(pInst,winInfo->agentCfgWindow,
	     (char*)((UINT32)localAdrs[winInfo->agentCfgWindow] + deviceBase + RAPIDIO_BA),
	     RIO_CFG_WINDOW_SIZE,
	     winInfo->tgtId,
	     agentRioAdrs);	
		
    /* Setup Outbound window to see configuration*/
    /* need to configure channels as pairs */

    agentCCSBAR = (char*)((UINT32)localAdrs[winInfo->agentCfgWindow] + deviceBase + RAPIDIO_BA);

    /* Main window to host*/
    rioSetRemoteOW(pInst,
		   agentCCSBAR,
		   winInfo->agentMainWindow,
		   agentMainLocalAdrs,
		   RIO_WINDOW_SIZE,
		   hostID,
		   winInfo->hostLocalMemRioAdrs);

    /* ATOMIC SET*/
    rioSetRemoteOWSpecial(pInst,
			  agentCCSBAR,
			  winInfo->agentAtomicSetWindow,
			  agentAtomicSetLocalAdrs,
			  RIO_WINDOW_SIZE,
			  hostID,
			  winInfo->hostLocalMemRioAdrs,
			  0xe,
			  0x5);

    /* ATOMIC CLEAR*/
    rioSetRemoteOWSpecial(pInst,agentCCSBAR,
			  winInfo->agentAtomicClearWindow,
			  agentAtomicClearLocalAdrs,
			  RIO_WINDOW_SIZE,hostID,
			  winInfo->hostLocalMemRioAdrs,
			  0xf,
			  0x5);

    /* AGENT DOORBELL */
    rioSetRemoteOWSpecial(pInst,
			  agentCCSBAR,
			  winInfo->agentDoorbellWindow,
			  agentDoorbellLocalAdrs,
			  RIO_WINDOW_SIZE,hostID,
			  winInfo->hostLocalMemRioAdrs,
			  0x0,
			  0x2);

    /* HOST DOORBELL */
    rioSetRemoteOWSpecial(pInst,
			  pInst->pRegBase[0],
			  winInfo->hostDoorbellWindow,
			  hostDoorbellLocalAdrs,
			  RIO_WINDOW_SIZE,
			  winInfo->tgtId,
			  agentRioAdrs,
			  0x0,
			  0x2);

    outboundDoorbellAdrs[doorbellCount++] = hostDoorbellLocalAdrs;

    m85xxRioBusIntInit(agentCCSBAR,hostID,winInfo->tgtId);
   
    /* Master Enable*/
    M85XXRIO_LOG("agentCCSBAR 0x%x hostDoorbellLocalAdrs 0x%x\n",agentCCSBAR,hostDoorbellLocalAdrs,0,0,0,0);

    *M85XXRIO_PGCCSR((UINT32)agentCCSBAR) |= PGCCSR_MASTER;

    }

/********************************************************************
*
* rio8548Cfg - 8548 cpu specific configuration routine.
*              Configures the target configuration registers to RIO so
*              host/target can communicate with each other
*
* RETURNS: N/A
*
* ERRNO
*/


LOCAL void rio8548Cfg
    (
    VXB_DEVICE_ID pInst,
    RIO_CFG_WINDOW_INFO *winInfo
    )
    {
    VXB_DEVICE_ID rioDev;
    UINT32 tempVal;
    STATUS retVal;
    UINT32 agentMainLocalAdrs ;
    UINT32 agentAtomicSetLocalAdrs;
    UINT32 agentAtomicClearLocalAdrs;
    UINT32 agentDoorbellLocalAdrs;
    UINT32 agentCCSBAR,agentRioAdrs;
    UINT32 deviceBase = (UINT32)((struct m85xxRioDrvCtrl *)pInst->pDrvCtrl)->deviceBase;

    agentMainLocalAdrs = deviceBase;
    agentAtomicSetLocalAdrs = (UINT32)localAdrs[winInfo->agentAtomicSetWindow] + deviceBase;
    agentAtomicClearLocalAdrs = (UINT32)localAdrs[winInfo->agentAtomicClearWindow] + deviceBase;
    agentDoorbellLocalAdrs = (UINT32)localAdrs[winInfo->agentDoorbellWindow] + deviceBase;

    /* host window count */

    agentRioAdrs =(UINT32) rioAdrs[rioAdrsCnt++] + (UINT32)((struct m85xxRioDrvCtrl *)pInst->pDrvCtrl)->rioBusAdrs;
    tempVal = (agentRioAdrs >> 3);

    rioDev = winInfo->rioDev;

    retVal = m85xxRioRemoteCfgWrite
      (
      rioDev,
      M85XXRIO_LCSBA1CSR_OFFSET,
      sizeof(UINT32),
      (char *)&tempVal,
      (UINT32*)&winInfo->tgtId
      );
			
    rioSetOW(pInst,winInfo->agentCfgWindow,
	     (char*)((UINT32)localAdrs[winInfo->agentCfgWindow] + deviceBase),
	     RIO_CFG_WINDOW_SIZE,
	     winInfo->tgtId,
	     (char*)agentRioAdrs);	
		
    /* Setup Outbound window to see configuration*/
    /* need to configure channels as pairs */

    agentCCSBAR = (UINT32)localAdrs[winInfo->agentCfgWindow] + deviceBase + RAPIDIO_BA;

    /* Main window to host*/
    rioSetRemoteOW(pInst,(char*)agentCCSBAR,winInfo->agentMainWindow,
		   (char*)agentMainLocalAdrs,RIO_WINDOW_SIZE,hostID,
		   winInfo->hostLocalMemRioAdrs);

    /* ATOMIC SET*/
    rioSetRemoteOWSpecial(pInst,(char*)agentCCSBAR,winInfo->agentAtomicSetWindow,
			  (char*)agentAtomicSetLocalAdrs,RIO_WINDOW_SIZE,hostID,
			  winInfo->hostLocalMemRioAdrs,
			  M85XXRIO_ROWAR_RDTYPE_ATOMIC_SET,
			  M85XXRIO_ROWAR_WRTYPE_NWRITE_R);

    /* ATOMIC CLEAR*/
    rioSetRemoteOWSpecial(pInst,(char*)agentCCSBAR,winInfo->agentAtomicClearWindow,
			  (char*)agentAtomicClearLocalAdrs,RIO_WINDOW_SIZE,hostID,
			  winInfo->hostLocalMemRioAdrs,
			  M85XXRIO_ROWAR_RDTYPE_ATOMIC_CLR,
			  M85XXRIO_ROWAR_WRTYPE_NWRITE_R);

   m85xxRioBusIntInit((char*)agentCCSBAR,hostID,winInfo->tgtId);

    /* Master Enable*/

   *M85XXRIO_PGCCSR(agentCCSBAR) |= PGCCSR_MASTER;

    }

/********************************************************************
*
* convertTypeOWRead - returns value for write type or the same value.
*                      As it's 1-1 conversion on this board.
*
* RETURNS: UINT32 type
*
* ERRNO
*/

LOCAL UINT32 convertTypeOWRead
    (
    UINT32 type
    )
    {
    if (type == 0)
	return(M85XXRIO_ROWAR_RDTYPE_NREAD);
    else
	return(type);

    }

/********************************************************************
*
* convertTypeOWWrite - returns value for write type or the same value.
*                      As it's 1-1 conversion on this board.
*
* RETURNS: UINT32 type
*
* ERRNO
*/

LOCAL UINT32 convertTypeOWWrite
    (
    UINT32 type
    )
    {
    if (type == 0)
	return(M85XXRIO_ROWAR_WRTYPE_NWRITE_R);
    else
	return(type);

    }


/********************************************************************
*
* convertTypeIWRead - returns value for read type or the same value.
*                     As it's 1-1 conversion on this board.
*
* RETURNS: UINT32 type
*
* ERRNO
*/

LOCAL UINT32 convertTypeIWRead
    (
    UINT32 type
    )
    {
    if (type == 0)
	return(M85XXRIO_RIWAR_LOCAL_R_SNOOP_CORE);
    else
	return(type);

    }

/********************************************************************
*
* convertTypeIWWrite - returns value for write type or the same value.
*                      As it's 1-1 conversion on this board.
*
* RETURNS: UINT32 type
*
* ERRNO
*/

LOCAL UINT32 convertTypeIWWrite
    (
    UINT32 type
    )
    {
    if (type == 0)
	return(M85XXRIO_RIWAR_LOCAL_W_SNOOP_CORE);
    else
	return(type);
    }

/********************************************************************
*
* convertTgtIf - convert tgtif from standard to target specific value.
*                Here it's 1-1
*
*
* RETURNS: UINT32 tgtIf
*
* ERRNO
*/

LOCAL UINT32 convertTgtIf
    (
    UINT32 tgtIf
    )
    {
    if (tgtIf == 0)
	return(0xf);
    else
	return(tgtIf);
    }
	



/********************************************************************
*
* rioSetOW - Set local outbound window
*
* RETURNS: N/A
*
* ERRNO
*/
LOCAL void rioSetOW
    (
    VXB_DEVICE_ID pInst,
    UINT32 window,
    char* localAdrs,
    UINT32 size,
    UINT32 tgtID,
    char* rioAdrs
    )
    {
		
    rioSetRemoteOWSpecial(pInst,pInst->pRegBase[0],window,localAdrs,size,tgtID,rioAdrs,0,0);
    }

/********************************************************************
*
* rioSetRemoteOW - Set remote outbound window
*
* RETURNS: N/A
*
* ERRNO
*/
LOCAL void rioSetRemoteOW
    (
    VXB_DEVICE_ID pInst,
    char* remBa,
    UINT32 window,
    char* localAdrs,
    UINT32 size,
    UINT32 tgtID,
    char* rioAdrs
    )
    {
    rioSetRemoteOWSpecial(pInst,remBa,window,localAdrs,size,tgtID,rioAdrs,0,0);
    }

/********************************************************************
*
* rioSetIW - Set local inbound window
*
* RETURNS: N/A
*
* ERRNO
*/
LOCAL void rioSetIW
    (
    VXB_DEVICE_ID pInst,
    UINT32 window,
    char* rioAdrs,
    UINT32 size,
    char* localAdrs
    )
    {
    rioSetRemoteIWSpecial(pInst,pInst->pRegBase[0],window,rioAdrs,size,localAdrs,0,0,0);
    }

#if 0	/* (not used) */
/********************************************************************
*
* rioSetRemoteIW - Set remote inbound window
*
* RETURNS: N/A
*
* ERRNO
*/
LOCAL void rioSetRemoteIW
    (
    VXB_DEVICE_ID pInst,
    char * remBa,
    UINT32 window,
    char * rioAdrs,
    UINT32 size,
    char * localAdrs
    )
    {
    rioSetRemoteIWSpecial(pInst,remBa,window,rioAdrs,size,localAdrs,0,0,0);
    }
#endif

/********************************************************************
*
* rioSetRemoteIWSpecial - Set remote inbound window but allow setting
*                         of the special attributes.
*
* RETURNS: N/A
*
* ERRNO
*/
LOCAL void rioSetRemoteIWSpecial
    (
    VXB_DEVICE_ID pInst,
    char * remBa,
    UINT32 window,
    char * rioAdrs,
    UINT32 size,
    char * localAdrs,
    UINT32 typeRead,
    UINT32 typeWrite,
    UINT32 tgtIf
    )
    {
    int index;
    UINT32 rType,wType,tgtInterface;
    UINT32 tempVal,flags;
    struct m85xxRioDrvCtrl	*pDrvCtrl;

    pDrvCtrl= pInst->pDrvCtrl;

    flags = VXBAF_BYTE_ORDER_BE_32;

    rType = convertTypeIWRead(typeRead);
    wType = convertTypeIWWrite(typeWrite);

    tgtInterface = convertTgtIf(tgtIf);

    tempVal = ((UINT32)rioAdrs>>12);

    M85XXRIO_LOG("\nIW remBa %x\n",remBa,0,0,0,0,0);
    VXB_RIO_WRITE(M85XXRIO_RIWBARn(remBa,window),tempVal);

    tempVal = ((UINT32)localAdrs >> 12);
    VXB_RIO_WRITE(M85XXRIO_RIWTARn(remBa,window),tempVal);

    for(index=0;index<32;index++)
	{
	if((size >> index) == 1)
	    break;
	}

    index--;

    tempVal = 0x80000000 | (tgtInterface << 20) | (rType << 16) | (wType << 12) | index;


    VXB_RIO_WRITE(M85XXRIO_RIWARn(remBa,window),tempVal);

    M85XXRIO_LOG("\nIW %x\n",*M85XXRIO_RIWARn(remBa,window),0,0,0,0,0);

    }

/********************************************************************
*
* rioSetRemoteOWSpecial - Set remote outbound window but allow setting
*                         of the special attributes.
*
* RETURNS: N/A
*
* ERRNO
*/
LOCAL void rioSetRemoteOWSpecial
    (
    VXB_DEVICE_ID pInst,
    char * remBa,
    UINT32 window,
    char * localAdrs,
    UINT32 size,
    UINT32 tgtID,
    char * rioAdrs,
    UINT32 typeRead,
    UINT32 typeWrite
    )
    {
    int index;
    UINT32 rType,wType;
    UINT32 tempVal,flags;
    struct m85xxRioDrvCtrl	*pDrvCtrl;

    pDrvCtrl= pInst->pDrvCtrl;
		

    flags = VXBAF_BYTE_ORDER_BE_32;


    rType = convertTypeOWRead(typeRead);
    wType = convertTypeOWWrite(typeWrite);


    for(index=0;index<32;index++)
	{
	if((size >> index) == 1)
	    break;
	}

    index--;

    tempVal = ((UINT32)localAdrs)>>12;
    M85XXRIO_LOG("\nOW remBa %x\n",remBa,0,0,0,0,0);		

    VXB_RIO_WRITE(M85XXRIO_ROWBARn(remBa,window),tempVal);
    M85XXRIO_LOG("\nOW ROWBAR 0x%x val 0x%x\n",M85XXRIO_ROWBARn(remBa,window),tempVal,0,0,0,0);		
	
    tempVal = ((tgtID & 0xff) << 22) | ((UINT32)rioAdrs >> 12);
		
    VXB_RIO_WRITE(M85XXRIO_ROWTARn(remBa,window),tempVal);

    M85XXRIO_LOG("\nOW ROWTAR 0x%x val 0x%x\n",M85XXRIO_ROWTARn(remBa,window),tempVal,0,0,0,0);		

    tempVal = 0x82000000 | (rType << 16) | (wType << 12) | index ;


    VXB_RIO_WRITE(M85XXRIO_ROWARn(remBa,window),tempVal);

    M85XXRIO_LOG("\nOW ROWAR 0x%x val 0x%x\n",M85XXRIO_ROWARn(remBa,window),tempVal,0,0,0,0);		
		
    }


/*******************************************************************************
*
* m85xxRioBusIntAck - acknowledge a bus interrupt
*
* This routine acknowledges a specified VMEbus interrupt level.
*
* NOTE: This routine is included for BSP compliance only.  Since VMEbus
* interrupts are re-enabled in the interrupt handler, and acknowledged
* automatically by hardware, this routine is a no-op.
*
* RETURNS: NULL.
*
* SEE ALSO: m85xxRioBusIntGen()
*/

LOCAL int m85xxRioBusIntAck
    (
    int intLevel        /* interrupt level to acknowledge*/
    )
    {
    int lockKey;

    /* Doorbell status register Special1 0x0000_0000*/
    lockKey = intCpuLock();

   while (*M85XXRIO_IDSR((UINT32)globalRioID->pRegBase[0])&0x1)
	{
 	*M85XXRIO_IDMR((UINT32)globalRioID->pRegBase[0]) &=~0x1; /* Disable doorbell */
	*M85XXRIO_IDSR((UINT32)globalRioID->pRegBase[0]) = 0xffffffff; /* clear interrupt status */
	*M85XXRIO_IPWSR((UINT32)globalRioID->pRegBase[0]) = 0xffffffff; /* clear port status */
 	*M85XXRIO_IDMR((UINT32)globalRioID->pRegBase[0]) |=0x3; /* increment doorbell enqueue ptr */
	}

    intCpuUnlock(lockKey);


    return (OK);
    }



/*******************************************************************************
*
* m85xxRioBusIntGen - generate a bus interrupt
*
* This routine generates a VMEbus interrupt for a specified level with a
* specified vector.  Only one VME interrupt can be generated at a time and
* none can be generated if a previously generated VME interrupt has not been
* acknowledged, i.e., if no VME bus IACK cycle has completed.
*
* RETURNS: OK, or ERROR if <level> or <vector> are out of range or if an
* interrupt is already in progress.
*
* SEE ALSO: m85xxRioBusIntAck()
*
* ERRNO
*/

LOCAL STATUS m85xxRioBusIntGen
    (
    int level,          /* interrupt level to generate   */
    int vector          /* interrupt vector for interrupt*/
    )
    {
    int index;

    /* Wait for interrupt to complete */
    while((*M85XXRIO_ODSR((UINT32)globalRioID->pRegBase[0]) & 0x4) != 0)
		;

    *M85XXRIO_ODSR((UINT32)globalRioID->pRegBase[0]) = 0xffffffff;

    /* Toggle interrupt line */
    
    if(rioHost == TRUE)
	{
	/* signal all CPUs for now */
	for(index = 0;index<agentCount;index++)
	    {
	    *M85XXRIO_ODDPR((UINT32)globalRioID->pRegBase[0]) = agentIDList[index];
	    /* Toggle interrupt line */
	    *M85XXRIO_ODMR((UINT32)globalRioID->pRegBase[0]) = 0x0;
	    *M85XXRIO_ODMR((UINT32)globalRioID->pRegBase[0]) = 0x1;
	    }
	}
    else
	{
	*M85XXRIO_ODDPR((UINT32)globalRioID->pRegBase[0]) = (hostID << 16);
	    /* Toggle interrupt line */
	*M85XXRIO_ODMR((UINT32)globalRioID->pRegBase[0]) = 0x0;
	*M85XXRIO_ODMR((UINT32)globalRioID->pRegBase[0]) = 0x1;
	}

    /* Wait for interrupt to complete */
    while((*M85XXRIO_ODSR((UINT32)globalRioID->pRegBase[0]) & 0x4) != 0)
		;

    return(OK);

    }



/**************************************************************************
*
* m85xxRioBusTas - test and set a specified location
*
* This routine performs a test-and-set (TAS) instruction on the specified
* address.  To prevent deadlocks, interrupts are disabled during the
* test-and-set operation.  The following table defines the method used to
* insure an atomic operation.
*
* RETURNS: TRUE if the value had not been set but is now, or
* FALSE if the value was set already.
* 
* SEE ALSO: vxTas(), m85xxRioBusTasClear()
*
* ERRNO
*/

LOCAL BOOL m85xxRioBusTas
    (
    char* adrs          /* address to be tested and set*/
    )
    {
    /* Check for master node*/

    BOOL state = FALSE;  /* semaphore state*/
    int  lockKey;        /* interrupt lock key*/

    if (((UINT32)adrs >= (UINT32)rioHostAdrs) && ((UINT32)adrs < ((UINT32)rioHostAdrs + 0x100000)))
	{
	lockKey = intCpuLock();

	if(sysProcNumGet()!=0)
	    {	
	    /* perform RMW to try and set TAS location*/
	    state =*(VUINT32*)TAS_SET_TRANSLATE((VUINT32)adrs);
	
	    if (state!=0x0)
		state=FALSE;
	    else
		state=TRUE;

	    }
	else
	    {
	    state = sysTas(adrs);
	    }

	
	intCpuUnlock(lockKey);
	}	

    return(state);

    }


/**************************************************************************
*
* m85xxRioBusTasClear - clear a location set by m85xxRioBusTas()
*
* This routine clears the specified 32-bit location typically set
* by m85xxRioBusTas().
*
* RETURNS: N/A
*
* SEE ALSO: m85xxRioBusTas()
*
* ERRNO
*/

LOCAL void m85xxRioBusTasClear
    (
    volatile char* adrs    /* Address of semaphore to be cleared*/
    )
    {
    VUINT32  state;
    int  lockKey;        /* interrupt lock key*/

    if (((UINT32)adrs >= (UINT32)rioHostAdrs) && ((UINT32)adrs < ((UINT32)rioHostAdrs + 0x100000)))
	{

	lockKey = intCpuLock();
	
	if(sysProcNumGet()!=0)
	    {
	    /* lock interrupts so that vxTas() can execute without preemption*/
	    state =*(VUINT32*)TAS_CLEAR_TRANSLATE((VUINT32)adrs);
	    }
	else
	    {
	    WRS_ASM("isync;sync;eieio");
	    *(VUINT32*)adrs = 0;
	    WRS_ASM("isync;sync;eieio");
	    }

	intCpuUnlock(lockKey);
	}


    }

/********************************************************************
*
* m85xxRioBusIntInit - sets up the mailbox messaging
*
* RETURNS: N/A
*
* ERRNO
*/
LOCAL void m85xxRioBusIntInit
    (
    char* agentCCSBAR,
    UINT32 localHostID,
    UINT32 agentID
    )
    {

    if(mailboxFirstTime)
	{
	baseBDPtr = (char*)ROUND_UP(malloc(64* sizeof(MAILBOX_BD) + 0x10),0x10);

	memset(baseBDPtr,0,(64* sizeof(MAILBOX_BD)));

	startBDPtr = baseBDPtr;

	endBDPtr = baseBDPtr + (63* sizeof(MAILBOX_BD));

	*M85XXRIO_IDMR((UINT32)globalRioID->pRegBase[0]) = 0x0;

	/* Doorbell status register Special1 0x0000_0000*/
	*M85XXRIO_IDSR((UINT32)globalRioID->pRegBase[0]) = 0xffffffff;


	/* Doorbell queue dequeue pointer address register R/W 0x0000_0000*/
	*M85XXRIO_IDQDPAR((UINT32)globalRioID->pRegBase[0])  = (UINT32)startBDPtr;

	/* Doorbell queue enqueue pointer address register R/W 0x0000_0000*/
	*M85XXRIO_IDQEPAR((UINT32)globalRioID->pRegBase[0]) = (UINT32)startBDPtr;
	/* Clear interrupt*/
	*M85XXRIO_ODSR((UINT32)globalRioID->pRegBase[0]) = 0x2;
	*M85XXRIO_ODDATR((UINT32)globalRioID->pRegBase[0]) = 0x20000000;
	*M85XXRIO_ODRETCR((UINT32)globalRioID->pRegBase[0]) = 0xff;
	*M85XXRIO_IDMR((UINT32)globalRioID->pRegBase[0]) = 0x00104041;
	mailboxFirstTime = FALSE;
	}
		

    if(rioHost==TRUE)
	{
	agentIDList[agentCount++] = (agentID << 16);
	*M85XXRIO_ODDPR((UINT32)globalRioID->pRegBase[0]) = (agentID << 16);
	*M85XXRIO_ODDPR(agentCCSBAR) = (localHostID << 16);
	}
    if(rioHost != TRUE)	
	hostID =*M85XXRIO_ODDPR((UINT32)globalRioID->pRegBase[0]) >> 16;

    }
#endif
