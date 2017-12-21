/* vxbFslSRio.c - Freescale Serial RapdIO Controller VxBus driver */

/*
 * Copyright (c) 2012, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,15may13,j_z  add slave share memory support.((WIND00418128)
01d,18apr13,j_z  change sysTas to vxTas.
01c,18mar13,j_z  add T4240 support; change auto config process;
                 add force config.
01b,20dec12,j_z  free exist LAW for warm reboot, change
                 fslRioUsrIntGen parameter 2 usage.
01a,03sep12,j_z  written
*/

/*
DESCRIPTION

This module implements driver support for the Freescale processor Serial
RapidIO Interface module. It has some inbound/outbound windows, which can
support some basic RapidIO operation, SWRITE/NWRITE/NWRITE_R/MAINTENANCE write/
MAINTENANCE read/ATOMIC increment/ATOMIC decrement/ATOMIC set/ATOMIC clear.
Through configuration these windows, different boards can realize share memory
application.

There are two suggested methods by which RapidIO transactions can target RapidIO
configuration register space in local memory. The first method is based on
RapidIO NREAD and NWRITE_R requests hitting a RapidIO address window defined by
the local configuration space base address command and status register.

The second method is based on a RapidIO MAINT requests. This method allows an
external device limited access to local RapidIO configuration register space
only.

This driver implements a share memory instance, shared host memory base 'smHostAdrs'.

EXTERNAL INTERFACE
The driver provides a vxBus external interface. The only exported
routine is the fslRioRegister() function, which registers the driver with
VxBus. Since the queue manager is a processor local bus device, each device
instance must be specified in the hwconf.c file in a BSP. The hwconf entry
entry must specify the following parameters:

\is

\i <regBase>
Specifies the base address where the controller's CSR registers are mapped
into the host's address space. All register offsets are computed relative
to this address.

\i <deviceBase>
Specifies the RapidIO CPU address. In the current design, the Host board
uses it to access the RapidIO register space of the Slave board,
and the Salve board uses it to access the share memory of the Host board.

\i <lawSize>
Specifies the LAW windows size. use to config Freescale LAW module.

\i <rioPortNum>
Specifies the RapidIO port number. (0 - N)

\i <smHostAdrs>
Specifies the shared memory address of Host board, this address must be
aligned based on 'smWinSize', such as if smWinSize is 1M, the address must 1M aligned.

\i <smSlaveAdrs>
Specifies the shared memory address of Slave board, this address must be
aligned based on 'smWinSize', such as if smWinSize is 1M, the address must 1M aligned.

\i <rioBusSize>
Specifies the RapidIO bus space size this port is used.

\i <smWinSize>
Specifies the shared memory windows size.

\i <autoCfg>
Specifies if Host board need configure the entire bus system,
including device ID, switch, and etc. The value is TRUE by default.

\i <isHost>
Specifies if the board work on Host mode or not.
If not specified, the mode will be from RCW.

\i <forceId>
Specifies the device ID of the board and only if <autoCfg> is FALSE.

\i <slaveNoWait>
Specifies if Slave board need to wait for the finish of the
configuration process. Host board takes charge of the process
for all the boards in the bus system. Before the finish
of the process, the share memory of Host board can NOT be
accessed. So if one application, such as share memory END
driver, tries to access it before the finish of the process,
the board will hang directly. So the value must be set to FALSE
for this usage by default. But if only the message mode is used,
the value can be set to TRUE to let the slave board go ahead.

\ie

An example hwconf entry is shown below:

\cs

const struct hcfResource fslRio0Resources[] = {
    { "regBase",         HCF_RES_INT,  {(void *)(CCSBAR + 0xC0000)} },
    { "deviceBase",      HCF_RES_INT,  {(void *)(FSL_RIO1_MEM_ADRS)}},
    { "rioBusSize",      HCF_RES_INT,  {(void *)(FSL_RIO1_MEM_SIZE)}},
    { "lawSize",         HCF_RES_INT,  {(void *)(FSL_RIO1_LAW_SIZE)}},
    { "rioPortNum",      HCF_RES_INT,  {(void *)0}},
    { "smWinSize",       HCF_RES_INT,  {(void *)RIO_SM_WIN_SIZE_16M}},
    { "smHostAdrs",      HCF_RES_INT,  {(void *)LOCAL_MEM_LOCAL_ADRS}},
    { "smSlaveAdrs",     HCF_RES_INT,  {(void *)LOCAL_MEM_LOCAL_ADRS}},    
    { "autoCfg",         HCF_RES_INT,  {(void *)FALSE}},
    { "isHost",          HCF_RES_INT,  {(void *)TRUE}}
    { "forceId",         HCF_RES_INT,  {(void *)DEVICE_ID}},
    { "slaveNoWait",     HCF_RES_INT,  {(void *)FALSE}},
};
#define fslRio0Num    NELEMENTS(fslRio0Resources)
const struct hcfResource fslRio1Resources[] = {
    { "regBase",         HCF_RES_INT,  {(void *)(CCSBAR + 0xC0000)} },
    { "deviceBase",      HCF_RES_INT,  {(void *)(FSL_RIO2_MEM_ADRS)}},
    { "rioBusSize",      HCF_RES_INT,  {(void *)(FSL_RIO2_MEM_SIZE)}},
    { "lawSize",         HCF_RES_INT,  {(void *)(FSL_RIO2_LAW_SIZE)}},
    { "rioPortNum",      HCF_RES_INT,  {(void *)1}},
    { "smWinSize",       HCF_RES_INT,  {(void *)RIO_SM_WIN_SIZE_16M}},
    { "smHostAdrs",      HCF_RES_INT,  {(void *)LOCAL_MEM_LOCAL_ADRS}},
    { "smSlaveAdrs",     HCF_RES_INT,  {(void *)LOCAL_MEM_LOCAL_ADRS}},        
    { "autoCfg",         HCF_RES_INT,  {(void *)FALSE}},
    { "isHost",          HCF_RES_INT,  {(void *)TRUE}}
    { "forceId",         HCF_RES_INT,  {(void *)DEVICE_ID}},
    { "slaveNoWait",     HCF_RES_INT,  {(void *)FALSE}},
};
#define fslRio1Num    NELEMENTS(fslRio1Resources)

\ce

SEE ALSO: vxBus
\tb  RapidIO Interconnect Specification
\tb  QorIQ Processor Reference Manual
\tb  Serial RapidIO Bring up on PowerQuicc III
*/

#include <vxWorks.h>
#include <intLib.h>
#include <logLib.h>
#include <semLib.h>
#include <sysLib.h>
#include <vxBusLib.h>
#include <wdLib.h>
#include <string.h>
#include <vxAtomicLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/util/vxbParamSys.h>
#include <hwif/util/hwMemLib.h>
#include <hwif/util/vxbSmSupport.h>
#include <hwif/util/vxbMsgSupport.h>
#include <vxbTimerLib.h>
#include <taskLibCommon.h>
#include <vxLib.h>
#include <stdio.h>

#include <../src/hwif/h/vxbus/vxbRapidIO.h>
#include <../src/hwif/h/resource/vxbQorIQLaw.h>
#include <../src/hwif/h/resource/vxbQorIQTgtId.h>
#include <../src/hwif/resource/m85xxCCSR.h>
#include <../src/hwif/h/busCtlr/vxbFslSRio.h>

#undef RIO_DEBUG
#ifdef RIO_DEBUG
#   include <private/kwriteLibP.h> /* INCLUDE_DEBUG_KPRINTF */
#   undef  LOCAL
#   define LOCAL

#   define DBG_INTR           0x00000001
#   define DBG_INIT           0x00000002
#   define DBG_RW             0x00000004
#   define DBG_ERR            0x00000008
#   define DBG_TEST           0x00000010
#   define DBG_ALL            0xFFFFFFFF
#   define DBG_NONE           0

UINT32 fslRioDbgMask = DBG_NONE;

#   define RIO_LOG(mask, X0, X1, X2, X3, X4, X5, X6) \
       { \
       if (_func_kprintf != NULL) \
          { \
          if (fslRioDbgMask & mask) \
               _func_kprintf (X0, (int)X1, (int)X2, (int)X3, (int)X4, \
                   (int)X5, (int)X6);  \
          } \
       }
#else /* RIO_DEBUG */
#    define RIO_LOG(mask, X0, X1, X2, X3, X4, X5, X6)
#endif /* RIO_DEBUG */

LOCAL  UINT32 fslRioAtmuSmWinSize;
IMPORT char * rioHostAdrs;
IMPORT char * rioTasSetAdrs;
IMPORT char * rioTasClearAdrs;
IMPORT BOOL sysTas (void *);
IMPORT void   vxbUsDelay (int);
LOCAL BOOL fslRioBusTas (char *);
LOCAL void fslRioBusTasClear (volatile char *);
LOCAL struct sharedMemSupportAPI * fslRioSmApiGet (VXB_DEVICE_ID, char *);
LOCAL RIO_MPORT_INFO * fslRioCtlrInfoGet (VXB_DEVICE_ID);
LOCAL void fslRioInstInit (VXB_DEVICE_ID);
LOCAL void fslRioInstInit2 (VXB_DEVICE_ID);
LOCAL void fslRioInstConnect (VXB_DEVICE_ID);
LOCAL STATUS  fslRioLocalWrite4 (VXB_DEVICE_ID, UINT32, UINT32);
LOCAL STATUS  fslRioLocalRead4 (VXB_DEVICE_ID, UINT32, UINT32 *);
LOCAL STATUS  fslRioMaintWrite4 (VXB_DEVICE_ID, UINT32, UINT32, UINT32, UINT32);
LOCAL STATUS  fslRioMaintRead4 (VXB_DEVICE_ID, UINT32, UINT32 , UINT32, UINT32 *);
LOCAL void fslRioSetInboundWin (VXB_DEVICE_ID, void *, UINT32, void *,
          UINT32, void *, UINT32, UINT32);
LOCAL void fslRioSetOutboundWin (VXB_DEVICE_ID, void *, UINT32, void *,
          UINT32, UINT32, void *, UINT32, UINT32);
LOCAL void fslRioMonitor (VXB_DEVICE_ID);

/* RapidIO generic access functions */

LOCAL VXB_RIO_OPS fslRioOps =
    {
    fslRioLocalRead4,
    fslRioLocalWrite4,
    fslRioMaintRead4,
    fslRioMaintWrite4,
    fslRioSetInboundWin,
    fslRioSetOutboundWin,
    NULL
    };

LOCAL struct sharedMemSupportAPI fslRioSmApi =
    {
    (FUNCPTR)fslRioBusTas,
    (FUNCPTR)fslRioBusTasClear,
    (FUNCPTR)NULL,
    (FUNCPTR)NULL,
    (FUNCPTR)NULL,
    (FUNCPTR)NULL,
    (FUNCPTR)NULL,
    (FUNCPTR)NULL,
    (FUNCPTR)NULL
    };

LOCAL device_method_t fslRio_methods[] =
    {
    DEVMETHOD(sharedMemSupportAPIGet, fslRioSmApiGet),
    DEVMETHOD(vxbRapidIoCtlrInfoGet, fslRioCtlrInfoGet),
    DEVMETHOD_END
    };

LOCAL struct drvBusFuncs fslRioFuncs =
    {
    fslRioInstInit,      /* devInstanceInit */
    fslRioInstInit2,     /* devInstanceInit2 */
    fslRioInstConnect    /* devInstanceConnect */
    };

LOCAL RIO_MPORT_INFO fslRioHostPort =
    {
    NULL,
    &fslRioOps,         /* basic RapidIO operations */
    0,
    FALSE,              /* Host Device or Slave device */
    0,                  /* 0 - 256 devices, 1-  65536 devices. */
    0                   /* RapidIO phy type */
    };

LOCAL DRIVER_REGISTRATION fslRioDevRegistration =
    {
    NULL,               /* pNext */
    VXB_DEVID_BUSCTRL,  /* devID */
    VXB_BUSID_PLB,      /* busID = Processor Local Bus */
    VXB_VER_4_0_0,      /* busVer */
    FSL_RIO_HOST,       /* drvName */
    &fslRioFuncs,       /* pDrvBusFuncs */
    &fslRio_methods[0], /* pMethods */
    NULL                /* devProbe */
    };

/*****************************************************************************
*
* fslRioRegister - register FreeScale Serial RapidIO host controller driver
*
* This routine registers the FreeScale CPU Serial RapidIO host
* controller driver to vxbus subsystem
*
* RETURNS: N/A
*
* ERRNO
*/

void fslRioRegister (void)
    {
    /*vxbDevRegister ((struct vxbDevRegInfo *) &fslRioDevRegistration);*/
    return;
    }

/*****************************************************************************
*
* fslRioInstInit - VxBus instInit handler
*
* This function implements the VxBus instInit handler.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void fslRioInstInit
    (
    VXB_DEVICE_ID pDev
    )
    {
    RIO_DRV_CTRL * pDrvCtrl;
    HCF_DEVICE   * pHcf;

    pDrvCtrl = (RIO_DRV_CTRL *)hwMemAlloc (sizeof(RIO_DRV_CTRL));
    if (pDrvCtrl == NULL)
        return;

    pDev->pDrvCtrl = pDrvCtrl;
    pDrvCtrl->pDev = pDev;

    pDrvCtrl->rioBar = pDev->pRegBase[0];

    vxbRegMap (pDev, 0, &pDrvCtrl->riohandle);

    pHcf = (struct hcfDevice *) hcfDeviceGet (pDev);
    if (pHcf == NULL)
        {
#ifndef _VXBUS_BASIC_HWMEMLIB
        hwMemFree(pDrvCtrl);
#endif /* _VXBUS_BASIC_HWMEMLIB */
        return;
        }
    /*
     * resourceDesc {
     * The deviceBase resource specifies the base address
     * of the RapidIO local memory address space. }
     */

    (void)devResourceGet (pHcf, "deviceBase", HCF_RES_INT, (void*)&pDrvCtrl->rioDevBase);

    /*
     * resourceDesc {
     * The rioPortNum resource specifies which RapidIO port (0,1)
     * is used. }
     */

    (void)devResourceGet (pHcf, "rioPortNum", HCF_RES_INT, (void*)&pDrvCtrl->rioPortNum);

    /*
     * resourceDesc {
     * The lawSize resource specifies the size of the RapidIO
     * LAW windows space. }
     */

    (void)devResourceGet (pHcf, "lawSize", HCF_RES_INT, (void*)&pDrvCtrl->rioLawSize);

    /*
     * resourceDesc {
     * The smHostAdrs resource specifies the shared host memory
     * base address. }
     */

    (void)devResourceGet (pHcf, "smHostAdrs", HCF_RES_INT, (void*)&pDrvCtrl->rioSmHostBase);


    /*
     * resourceDesc {
     * The smSlaveAdrs resource specifies the shared host memory
     * base address. }
     */

    (void)devResourceGet (pHcf, "smSlaveAdrs", HCF_RES_INT, (void*)&pDrvCtrl->rioSmSlaveBase);


    /*
     * resourceDesc {
     * The smWinSize resource specifies the size of the shared
     * host memory. }
     */

    (void)devResourceGet (pHcf, "smWinSize", HCF_RES_INT, (void*)&pDrvCtrl->rioSmWinSize);

    /*
     * resourceDesc {
     * The rioBusSize resource specifies the size of the RapidIO bus space. }
     */

    (void)devResourceGet (pHcf, "rioBusSize", HCF_RES_INT, (void*)&pDrvCtrl->rioBusSize);

    /*
     * resourceDesc {
     * The autoCfg resource specifies controller is configed by host or itself,
     * TRUE - enable auto config, FALSE - disable}
     */

    if (devResourceGet (pHcf, "autoCfg", HCF_RES_INT,
        (void*)&pDrvCtrl->rioAutoCfg) == ERROR)
        pDrvCtrl->rioAutoCfg = TRUE;

    /*
     * resourceDesc {
     * Specifies the device ID of the board and only if <autoCfg> is FALSE.}
     */

    if (pDrvCtrl->rioAutoCfg == FALSE)
        {
        if (devResourceGet (pHcf, "forceId", HCF_RES_INT,
            (void*)&pDrvCtrl->rioForceDevId) == ERROR)
            pDrvCtrl->rioAutoCfg = TRUE;
        }

    /*
     * resourceDesc {
     * The slaveNoWait resource specifies if disable slave board poll link status.}
     */

    (void)devResourceGet (pHcf, "slaveNoWait", HCF_RES_INT,
        (void*)&pDrvCtrl->rioSlvNoWait);

    vxbBusAnnounce (pDev, VXB_BUSID_RAPIDIO);

    return;
    }

/*****************************************************************************
*
* fslRioInstInit2 - VxBus instInit2 handler
*
* This function implements the VxBus instInit2 handler for an RapidIO controller
* Once we reach this stage of initialization, its safe for us to allocate memory,
* so we can create our pDrvCtrl structure and do some initial hardware setup.
* The important steps we do here are configure host CPU and slave device to
* realize share memory application.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void fslRioInstInit2
    (
    VXB_DEVICE_ID pDev
    )
    {
    RIO_DRV_CTRL * pDrvCtrl =  pDev->pDrvCtrl;
    LAW_HANDLE lawHandle;
    VXB_DEVICE_ID pInst85XXLawBar = NULL;
    struct m85xxLawBarInfo lawBarInfo;
    FUNCPTR method;
    UINT32 value;
    int ret;
    HCF_DEVICE   * pHcf;
    char mtName[20];

    if (pDrvCtrl == NULL)
        return;

    pHcf = (struct hcfDevice *) hcfDeviceGet (pDev);

    if (pHcf == NULL)
        return;

    fslRioHostPort.phyType = (CSR_READ_4(pDev, RIO_EXTFE_BASE +
        RIO_EXT_PORT_N_CTL_CSR(pDrvCtrl->rioPortNum)) & 0x1) ?
        RIO_PHY_SERIAL : RIO_PHY_PARALLEL;
    fslRioHostPort.portnum  = pDrvCtrl->rioPortNum;
    fslRioHostPort.systemSize = (CSR_READ_4(pDev, RIO_CAR_PE_FEATURES) &
        RIO_CAR_PEF_CTLS) >> 4;

    RIO_LOG(DBG_INIT, "fslRioInstInit2():phyType %d systemSize %d \r\n", fslRioHostPort.phyType,
        fslRioHostPort.systemSize, 3,4,5,6);

    fslRioHostPort.busCtlrID = pDev;

    fslRioAtmuSmWinSize = pDrvCtrl->rioSmWinSize;

    pDrvCtrl->rioSmBusBase = (void *)(RIO_BUS_SM_ADRS_BASE +
        pDrvCtrl->rioPortNum * pDrvCtrl->rioBusSize);

    CSR_WRITE_4(pDev, RIO_EXTFE_BASE + RIO_EXT_PORT_N_ERR_STS_CSR(pDrvCtrl->rioPortNum),
        0xffffffff);

    /*
     * Use bsp parameter to override RCW HOST/SLAVE information
     * If bsp hasn't define this resource, directly use RCW default value.
     */

    if (devResourceGet (pHcf, "isHost", HCF_RES_INT, (void*)&pDrvCtrl->rioHost)
       != OK)
        {
        if ((CSR_READ_4(pDev, RIO_EXTFE_BASE + RIO_EXT_PORT_GEN_CTL_CSR) &
            RIO_EXT_PORT_GEN_HOST) == RIO_EXT_PORT_GEN_HOST)
            pDrvCtrl->rioHost = TRUE;
        else
            pDrvCtrl->rioHost = FALSE;
        }

    if ((CSR_READ_4(pDev, RIO_EXTFE_BASE +
        RIO_EXT_PORT_N_ERR_STS_CSR(pDrvCtrl->rioPortNum)) &
        RIO_EXT_PORT_N_ERR_STS_PORT_OK) !=
        RIO_EXT_PORT_N_ERR_STS_PORT_OK)
        {
        RIO_LOG(DBG_INIT, "fslRioInstInit2():RapidIO Link Fail! RESET... \n", 1, 2, 3, 4, 5, 6);

        CSR_WRITE_4(pDev, RIO_EXTFE_BASE +  RIO_EXT_PORT_N_CTL_CSR(pDrvCtrl->rioPortNum),
            0x0); /* Disable ports */
        CSR_SETBIT_4(pDev, RIO_EXTFE_BASE +  RIO_EXT_PORT_N_CTL_CSR(pDrvCtrl->rioPortNum),
            0x02000000); /* Set 1x lane */
        CSR_SETBIT_4(pDev, RIO_EXTFE_BASE +  RIO_EXT_PORT_N_CTL_CSR(pDrvCtrl->rioPortNum),
            0x00600000); /* Enable ports */
        }


    if (pDrvCtrl->rioHost == TRUE )
        {
        CSR_SETBIT_4(pDev, RIO_EXTFE_BASE + RIO_EXT_PORT_GEN_CTL_CSR,
            (RIO_EXT_PORT_GEN_HOST | RIO_EXT_PORT_GEN_MASTER));
        }

    if (pDrvCtrl->rioAutoCfg == FALSE)
        {
        CSR_WRITE_4(pDev, RIO_CSR_DID,
           RIO_DID(fslRioHostPort.systemSize, pDrvCtrl->rioForceDevId));
        }

    if (pDrvCtrl->rioHost == TRUE)
        {
        snprintf (mtName, sizeof(mtName), "tSRIOmon%d", pDev->unitNumber);

        if (taskSpawn (mtName, 200, 0, 4096, (FUNCPTR)fslRioMonitor,
            (_Vx_usr_arg_t)pDev, 0,0,0,0,0,0,0,0,0) == ERROR)
            return;
        }

    /*
     * If the QorIQLaw driver is present in the kernel, we will
     * automatically use it to allocate the  LAW's, otherwise it
     * is the responsibility of the BSP author to have done it manually.
     */

    if (vxbInstByNameFind (QORIQ_LAW_NAME, 0) != NULL)
        {
        /* Find a LAW for this rio */

        bzero ((char *)&lawHandle, sizeof(LAW_HANDLE));

        lawHandle.lawTgt = QORIQ_TGTID_RIO1 + pDrvCtrl->rioPortNum;;
        lawHandle.lawDev = pDev;
        ret = vxbResourceFind (pDev, DEVMETHOD_CALL(QorIQLawFind),
            (char *)&lawHandle, 0);

        /* Free exist one */

        if (ret == OK)
           (void) vxbResourceFind (pDev, DEVMETHOD_CALL(QorIQLawFree),
                (char *)&lawHandle, 0);

        lawHandle.lawBase = (void *)pDrvCtrl->rioDevBase;
        lawHandle.lawSize = pDrvCtrl->rioLawSize;
        lawHandle.lawIdx  = 0;

        ret = vxbResourceFind (pDev, DEVMETHOD_CALL(QorIQLawAlloc),
            (char *)&lawHandle, 0);

        RIO_LOG(DBG_INIT, "fslRioInstInit2():RapidIO QorIQLawAlloc ret = %d Law add 0x%X"
            " size 0x%X \n",ret, lawHandle.lawBase, lawHandle.lawSize, 4,5,6);

        if (ret != OK)
            return;

        }

    pInst85XXLawBar = vxbInstByNameFind("m85xxCCSR", 0);

    if (pInst85XXLawBar != NULL)
        {
        lawBarInfo.index = -1;
        lawBarInfo.baseAdrs = pDrvCtrl->rioDevBase;
        lawBarInfo.size = pDrvCtrl->rioLawSize;
        lawBarInfo.interface = RIO_85XX_LAWAR_TGTIF;

        method = vxbDevMethodGet (pInst85XXLawBar, (UINT32)&m85xxLawBarAllocSet_desc[0]);

        if (method != NULL)
            {
            ret = (*method) (pInst85XXLawBar, pDev, &lawBarInfo);

            if (ret == ERROR)
                {
                RIO_LOG(DBG_ERR, "fslRioInstInit2():LawBar AllocSetStatus Error \n", 1,2,3,4,5,6);
                return;
                }
            }
        else
            return;
        }

    pDrvCtrl->rioSem = semMCreate (SEM_Q_PRIORITY |SEM_DELETE_SAFE |
        SEM_INVERSION_SAFE);

    CSR_WRITE_4(pDev, PnPTAACR(pDrvCtrl->rioPortNum), PnPTAACR_ACC_ALL);

    if ((pDrvCtrl->rioHost == FALSE) &&
        (pDrvCtrl->rioSlvNoWait == FALSE))
        {
        /* wait other board power on, link active */

        while ((CSR_READ_4(pDev, RIO_EXTFE_BASE +
            RIO_EXT_PORT_N_ERR_STS_CSR(pDrvCtrl->rioPortNum)) &
            RIO_EXT_PORT_N_ERR_STS_PORT_OK) !=
            RIO_EXT_PORT_N_ERR_STS_PORT_OK)
            {
            RIO_LOG(DBG_INIT, "fslRioInstInit2():RapidIO Link Fail! Retry... \n", 1, 2, 3, 4, 5, 6);

            CSR_WRITE_4(pDev, RIO_EXTFE_BASE +  RIO_EXT_PORT_N_CTL_CSR(pDrvCtrl->rioPortNum),
                0x0); /* Disable ports */
            CSR_SETBIT_4(pDev, RIO_EXTFE_BASE +  RIO_EXT_PORT_N_CTL_CSR(pDrvCtrl->rioPortNum),
                0x02000000); /* Set 1x lane */
            CSR_SETBIT_4(pDev, RIO_EXTFE_BASE +  RIO_EXT_PORT_N_CTL_CSR(pDrvCtrl->rioPortNum),
                0x00600000); /* Enable ports */

            vxbMsDelay (1000);
            }

        /* wait Host Processor finish enumerate */

        value = CSR_READ_4 (pDev, RIO_EXTFE_BASE + RIO_EXT_PORT_GEN_CTL_CSR);

        RIO_LOG(DBG_INIT,"fslRioInstInit2():Waiting other device finish configuration \n",1,2,3,4,5,6);

        while ((value & (RIO_EXT_PORT_GEN_MASTER | RIO_EXT_PORT_GEN_DISCOVERED)) !=
            (RIO_EXT_PORT_GEN_DISCOVERED | RIO_EXT_PORT_GEN_MASTER))
            {
            value = CSR_READ_4 (pDev, RIO_EXTFE_BASE + RIO_EXT_PORT_GEN_CTL_CSR);

            RIO_LOG(DBG_INIT,".", 1, 2, 3, 4, 5, 6);

            vxbMsDelay (1000);
            }
        }

    if (pDrvCtrl->rioHost == TRUE)
        {
        UINT32 rType, wType, ipVer;
        ipVer = CSR_READ_4(pDev, FSL_SRIO_IPBRR1);

        if (ipVer == TXXX_SRIO_IP)
            {
            rType = RIO_RIWAR_LOCAL_R_ENHANCE_SNOOP;
            wType = RIO_RIWAR_LOCAL_W_ENHANCE_SNOOP;
            }
        else
            {
            rType = RIO_RIWAR_LOCAL_R_SNOOP_CORE;
            wType = RIO_RIWAR_LOCAL_W_SNOOP_CORE;
            }

        /* host share memory windows */

        fslRioSetInboundWin (pDev,
            pDrvCtrl->rioBar,
            RIO_HOST_SM_IN_CHANNEL,
            pDrvCtrl->rioSmBusBase,   /* RIO bus Address */
            pDrvCtrl->rioSmWinSize,
            pDrvCtrl->rioSmHostBase,  /* CPU address */
            RIO_RIWAR_LOCAL_R_SNOOP_CORE,
            RIO_RIWAR_LOCAL_W_SNOOP_CORE
            );

        /* access slave share memory windows */

        fslRioSetOutboundWin (pDev,
            pDrvCtrl->rioBar,
            RIO_HOST_SM_OUT_CHANNEL,
            (ULONG)pDrvCtrl->rioDevBase,   /* CPU address */
            pDrvCtrl->rioSmWinSize,
            RIO_ANY_DESTID,
            pDrvCtrl->rioSmBusBase, /* RIO bus Address */
            RIO_ROWAR_RDTYPE_NREAD,
            RIO_ROWAR_WRTYPE_NWRITE_R
            );

        }
    else
        {
        /* only  implement share memory END on port 0 */

        if (pDrvCtrl->rioPortNum == 0)
            {
            rioHostAdrs = (char*)pDrvCtrl->rioDevBase;
            RIO_LOG(DBG_INIT, "fslRioInstInit2():rioHostAdrs 0x%x\n", rioHostAdrs, 1,2,3,4,5);

            rioTasSetAdrs = (char*)((ULONG)pDrvCtrl->rioDevBase +
                pDrvCtrl->rioSmWinSize);
            RIO_LOG(DBG_INIT, "fslRioInstInit2():rioTasSetAdrs 0x%x\n", rioTasSetAdrs, 1,2,3,4,5);

            rioTasClearAdrs = (char*)((ULONG)pDrvCtrl->rioDevBase +
                pDrvCtrl->rioSmWinSize * 2);
            RIO_LOG(DBG_INIT, "fslRioInstInit2():rioTasClearAdrs 0x%x\n", rioTasClearAdrs, 1,2,3,4,5);
            }

        fslRioSetOutboundWin (pDev,
            pDrvCtrl->rioBar,
            RIO_SLAVE_SM_OUT_CHANNEL,
            pDrvCtrl->rioDevBase,   /* CPU address */
            pDrvCtrl->rioSmWinSize,
            RIO_HOST_DFLT_DID,
            pDrvCtrl->rioSmBusBase, /* RIO bus Address */
            RIO_ROWAR_RDTYPE_NREAD,
            RIO_ROWAR_WRTYPE_NWRITE_R
            );

        fslRioSetOutboundWin (pDev,
            pDrvCtrl->rioBar,
            RIO_SLAVE_TAS_SET_OUT_CHANNEL,
            (void *)((ULONG)pDrvCtrl->rioDevBase + pDrvCtrl->rioSmWinSize), /* CPU address */
            pDrvCtrl->rioSmWinSize,
            RIO_HOST_DFLT_DID,
            pDrvCtrl->rioSmBusBase, /* RIO_BUS - map to same host memory Address */
            RIO_ROWAR_RDTYPE_ATOMIC_SET,
            RIO_ROWAR_WRTYPE_NWRITE_R
            );

        fslRioSetOutboundWin (pDev,
            pDrvCtrl->rioBar,
            RIO_SLAVE_TAS_CLEAR_OUT_CHANNEL,
            (void *)((ULONG)pDrvCtrl->rioDevBase + pDrvCtrl->rioSmWinSize * 2), /* CPU address */
            pDrvCtrl->rioSmWinSize,
            RIO_HOST_DFLT_DID,
            pDrvCtrl->rioSmBusBase, /* RIO bus Address */
            RIO_ROWAR_RDTYPE_ATOMIC_CLR,
            RIO_ROWAR_WRTYPE_NWRITE_R
            );

        /* salve share memory windows */

        fslRioSetInboundWin (pDev,
            pDrvCtrl->rioBar,
            RIO_SLAVE_SM_IN_CHANNEL,
            pDrvCtrl->rioSmBusBase,    /* RIO bus Address */
            pDrvCtrl->rioSmWinSize,
            pDrvCtrl->rioSmSlaveBase,  /* CPU address */
            RIO_RIWAR_LOCAL_R_SNOOP_CORE,
            RIO_RIWAR_LOCAL_W_SNOOP_CORE
            );

        }
    /*
     * Share memory can call these functions directly, define it can improve
     * performance
     */

#ifdef SM_NET_DIRECT_CALL
    vxbBusTasHook = fslRioBusTas;
    vxbBusTasClearHook = fslRioBusTasClear;
#endif

    return;
    }

/*******************************************************************************
*
* fslRioInstConnect - third stage
*
* This function impliment VxBus third stage initialize.
*
* RETURNS: N/A
*
*/

LOCAL void fslRioInstConnect
    (
    VXB_DEVICE_ID pDev
    )
    {
    }

/*******************************************************************************
*
* fslRioMonitor - Host monitor link status
*
* This routine is used to monitor the link status by the Host. 
* Once the link is up, the Host will enable the devices on the bus.
*
* RETURNS: N/A
*/

LOCAL void fslRioMonitor
    (
    VXB_DEVICE_ID pDev
    )
    {
    RIO_DRV_CTRL * pDrvCtrl =  pDev->pDrvCtrl;

    while ((CSR_READ_4(pDev, RIO_EXTFE_BASE +
        RIO_EXT_PORT_N_ERR_STS_CSR(pDrvCtrl->rioPortNum)) &
        RIO_EXT_PORT_N_ERR_STS_PORT_OK) !=
        RIO_EXT_PORT_N_ERR_STS_PORT_OK)
        {
        RIO_LOG(DBG_INIT, "fslRioMonitor():RapidIO Link Fail! RESET... \n", 1, 2, 3, 4, 5, 6);

        CSR_WRITE_4(pDev, RIO_EXTFE_BASE +  RIO_EXT_PORT_N_CTL_CSR(pDrvCtrl->rioPortNum),
            0x0); /* Disable ports */
        CSR_SETBIT_4(pDev, RIO_EXTFE_BASE +  RIO_EXT_PORT_N_CTL_CSR(pDrvCtrl->rioPortNum),
            0x02000000); /* Set 1x lane */
        CSR_SETBIT_4(pDev, RIO_EXTFE_BASE +  RIO_EXT_PORT_N_CTL_CSR(pDrvCtrl->rioPortNum),
            0x00600000); /* Enable ports */

        taskDelay (sysClkRateGet()*2);
        }

    if (pDrvCtrl->rioAutoCfg == TRUE)
        {
        if (vxbRioAcquireLock (pDev, RIO_HOST_DFLT_DID, 0,
            RIO_HOST_DFLT_DID) == OK)
            {
            pDrvCtrl->rioHost = TRUE;

            RIO_LOG(DBG_INIT, "fslRioMonitor():This is the HOST \r\n", 1,2,3,4,5,6);

            fslRioHostPort.host = TRUE;

            if (rioBusAnnounceDevices (pDev) == OK)
                return;
            }
        }

    /*
     * There is no notifier mechanism to let two
     * boards know the opposite end finished share memory configuration,
     * In order to synchronize each other,
     * slave board (deviceId != 0) poll the discover bit, until
     * host set it. Otherwise, if slave board power on earlier, and the
     * Share Memory Net is enabled, it will access the RapidIO bus address.
     * At this point, host board may doesn't power on, this will cause
     * slave target hang directly. So use this bit to synchronization between
     * each other. When use autoConfig mode, this bit will set in library.
     */

    else /* FALSE */
        {
        UINT32 tempValue;

        if (fslRioMaintRead4 (pDev, RIO_ANY_DESTID, 0,
            RIO_EXTFE_BASE + RIO_EXT_PORT_GEN_CTL_CSR,
            &tempValue) != OK)
            {
            RIO_LOG(DBG_ERR,"fslRioMonitor():enable slave failed \n",1,2,3,4,5,6);
            return;
            }

        fslRioMaintWrite4 (pDev, RIO_ANY_DESTID, 0,
                RIO_EXTFE_BASE + RIO_EXT_PORT_GEN_CTL_CSR,
                tempValue | RIO_EXT_PORT_GEN_MASTER |
                RIO_EXT_PORT_GEN_DISCOVERED);
        }
    }

/*******************************************************************************
*
* fslRioSetMaintWin - set up a maintenance window for remote transactions
*
* set up a maintenance window for config transactions, pInst is Host controller
* VxBus ID. adrs is device configuration base addres.
*
* RETURNS: OK - maintenance access success. ERROR - maintenance failed
*
*/

LOCAL STATUS fslRioSetMaintWin
    (
    VXB_DEVICE_ID       pInst,  /* bus controller ID */
    void *              adrs,
    UINT32              deviceID,
    UINT32              hopCount,
    UINT32              offset,
    UINT32              window
    )
    {
    RIO_DRV_CTRL *      pDrvCtrl = pInst->pDrvCtrl;
    UINT32              dataVal;
    STATUS              retSts;
    UINT32              retryCnt = 10, tempVal;

    dataVal = (deviceID << 22) | (hopCount << 12) | (offset >> 20);
    RIO_LOG(DBG_RW, "fslRioSetMaintWin():MAINTROWTAR 0x%x 0x%x \n", RIO_ROWTARn(pDrvCtrl->rioPortNum,
        window), dataVal, 3, 4, 5, 6);

    CSR_WRITE_4(pInst, RIO_ROWTARn(pDrvCtrl->rioPortNum, window), dataVal);

    dataVal = (UINT32)adrs >> RIO_WIN_ADDR_4KALIGN;

    RIO_LOG(DBG_RW, "fslRioSetMaintWin():MAINTROWBAR 0x%x adrs 0x%x \n", dataVal, adrs, 2, 3, 4, 5);

    CSR_WRITE_4(pInst, RIO_ROWBARn(pDrvCtrl->rioPortNum, window), dataVal);

    if (pDrvCtrl->rioMaintInit == FALSE)
        {
        dataVal = (RIO_RxWAR_TYPE_READ(RIO_ROWAR_RDTYPE_MAINT_READ)   |
            RIO_RxWAR_TYPE_WRITE(RIO_ROWAR_WRTYPE_MAINT_WRITE) |
            RIO_RxWAR_EN | RIO_MAINT_WIN_SIZE);

        RIO_LOG(DBG_RW, "fslRioSetMaintWin():MAINTROWAR 0x%x 0x%x \n", RIO_ROWARn(pDrvCtrl->rioPortNum,
            window), dataVal, 3, 4, 5, 6);

        CSR_WRITE_4(pInst, RIO_ROWARn(pDrvCtrl->rioPortNum, window), dataVal);

        /* Waiting Access OK */

        while (retryCnt --)
            {
            retSts = vxMemProbe ((char *)adrs + 0x10,
                         VX_READ, sizeof(UINT32), (char*)&tempVal);
            if (retSts != OK)
                {
                RIO_LOG(DBG_INIT, "fslRioSetMaintWin():MAINT Access Failed \n",1,2,3,4,5,6);
                }
            else
                {
                RIO_LOG(DBG_INIT, "fslRioSetMaintWin():MAINT Access OK 0x%x \n", tempVal,2,3,4,5,6);
                pDrvCtrl->rioMaintInit = TRUE;
                return OK;
                }
            }

        return ERROR;
        }

    return OK;
    }

/*******************************************************************************
*
* fslRioLocalWrite4 - write local controller's register
*
* host port write local CPU register 4 byte
*
* RETURNS: OK
*
*/

LOCAL STATUS fslRioLocalWrite4
    (
    VXB_DEVICE_ID   pDev,         /* device info */
    UINT32          byteOffset,   /* offset into cfg space */
    UINT32          dataVal       /* buffer to read-from/write-to */
    )
    {
    CSR_WRITE_4(pDev, byteOffset, dataVal);
    RIO_LOG(DBG_RW, "fslRioLocalWrite4(): %X %X \r\n", byteOffset, dataVal, 3, 4, 5, 6);
    return(OK);
    }

/******************************************************************************
*
* fslRioLocalRead4 - read local controller register
*
* host read local CPU register 4 byte
*
* RETURNS: OK
*
*/

LOCAL STATUS fslRioLocalRead4
    (
    VXB_DEVICE_ID   pDev,           /* device info */
    UINT32          byteOffset,     /* offset into cfg space */
    UINT32        * dataVal         /* buffer to read-from/write-to */
    )
    {
    *dataVal = CSR_READ_4(pDev, byteOffset);
    RIO_LOG(DBG_RW, "fslRioLocalRead4(): %X %X \r\n", byteOffset, *dataVal, 3, 4, 5, 6);
    return(OK);
    }

/*******************************************************************************
*
* fslRioMaintWrite4 - maintenance write transaction
*
* maintenance write target configuration space 4 bytes.
*
* RETURNS: OK
*
*/

LOCAL STATUS fslRioMaintWrite4
    (
    VXB_DEVICE_ID   pDev,
    UINT32          targetId,
    UINT32          hopCount,
    UINT32          byteOffset,
    UINT32          dataVal
    )
    {
    RIO_DRV_CTRL * pDrvCtrl = pDev->pDrvCtrl;
    UINT32 tempValue;
    void *  maintBase = (ULONG)pDrvCtrl->rioDevBase + (UINT32)pDrvCtrl->rioSmWinSize;
        
    semTake (pDrvCtrl->rioSem, WAIT_FOREVER);

    if (fslRioSetMaintWin (pDev, maintBase,
        targetId, hopCount, byteOffset,
        RIO_MAINT_CHANNEL) != OK)
        return ERROR;

    vxbWrite32(pDrvCtrl->riohandle, (UINT32 *)((ULONG)maintBase + byteOffset),
        dataVal);
    tempValue = vxbRead32(pDrvCtrl->riohandle, (UINT32 *)((ULONG)maintBase +
        byteOffset));

    semGive (pDrvCtrl->rioSem);

    RIO_LOG(DBG_RW, "fslRioMaintWrite4(): data(0x%x) written 0x%x to hopCount(%d) targetId(%d) \n",
        dataVal, tempValue, hopCount, targetId, 5, 6);

    return(OK);
    }

/******************************************************************************
*
* fslRioMaintRead4 - maintenance read  transaction
*
* maintenance read target configuration space 4 bytes.
*
* RETURNS: OK
*
*/

LOCAL STATUS fslRioMaintRead4
    (
    VXB_DEVICE_ID   pDev,
    UINT32          targetId,
    UINT32          hopCount,
    UINT32          byteOffset,
    UINT32        * dataVal
    )
    {
    RIO_DRV_CTRL * pDrvCtrl = pDev->pDrvCtrl;
    void * maintBase = (ULONG)pDrvCtrl->rioDevBase + (UINT32)pDrvCtrl->rioSmWinSize;

    semTake (pDrvCtrl->rioSem, WAIT_FOREVER);

    if (fslRioSetMaintWin (pDev, maintBase,
        targetId, hopCount, byteOffset,
        RIO_MAINT_CHANNEL) != OK)
        return ERROR;

    *dataVal = vxbRead32 (pDrvCtrl->riohandle, (UINT32 *)((ULONG)maintBase +
        byteOffset));

    semGive (pDrvCtrl->rioSem);

    RIO_LOG(DBG_RW, "fslRioMaintRead4(): 0x%x to hopCount(%d) targetId(%d) \n",*dataVal,
        hopCount, targetId, 4, 5, 6);

    return(OK);
    }

/*******************************************************************************
*
* fslRioSetInboundWin - Set RapidIO controller inbound windows
*
* This function configure ATMU inbound windows register, map RapidIO bus address
* to CPU address
*
* RETURNS: N/A
*
*/

LOCAL void fslRioSetInboundWin
    (
    VXB_DEVICE_ID pInst,
    void  *       bar,
    UINT32        window,
    void  *       rioAdrs,
    UINT32        size,
    void  *       localAdrs,
    UINT32        rType,
    UINT32        wType
    )
    {
    UINT32 tempVal,index, tgtIf;
    RIO_DRV_CTRL * pDrvCtrl =  pInst->pDrvCtrl;
    semTake (pDrvCtrl->rioSem, WAIT_FOREVER);

    tgtIf = RIO_WIN_LOCAL_MEM_TGTIF;

    tempVal = ((UINT32)rioAdrs >> RIO_WIN_ADDR_4KALIGN);

    vxbWrite32(pDrvCtrl->riohandle, (UINT32 *)((ULONG)bar +
        RIO_RIWBARn(pDrvCtrl->rioPortNum, window)), tempVal);

    RIO_LOG(DBG_RW, "fslRioSetInboundWin():IW RIO_RIWBARn 0x%x val 0x%x\n",
        RIO_RIWBARn(pDrvCtrl->rioPortNum, window), tempVal, 0, 0, 0, 0);

    tempVal = ((UINT32)localAdrs >> RIO_WIN_ADDR_4KALIGN);
    vxbWrite32(pDrvCtrl->riohandle, (UINT32 *)((ULONG)bar +
        RIO_RIWTARn(pDrvCtrl->rioPortNum, window)), tempVal);

    RIO_LOG(DBG_RW, "fslRioSetInboundWin():IW RIO_RIWTARn 0x%x val 0x%x\n",
        RIO_RIWTARn(pDrvCtrl->rioPortNum, window), tempVal, 0, 0, 0, 0);

    for (index = 0; index < 32; index ++)
        {
        if ((size >> index) == 1)
            break;
        }

    index = index - 1;

    RIO_LOG(DBG_RW, "fslRioSetInboundWin():IW index %d\n", index, 2, 3, 4, 5, 6);

    tempVal = RIO_WIN_ENABLE | (tgtIf << RIO_WIN_LOCAL_MEM_TGTIF_SHIFT) |
        RIO_RxWAR_TYPE_READ(rType) | RIO_RxWAR_TYPE_WRITE(wType) | index;
    vxbWrite32(pDrvCtrl->riohandle, (UINT32 *)((ULONG)bar +
        RIO_RIWARn(pDrvCtrl->rioPortNum, window)), tempVal);

    semGive (pDrvCtrl->rioSem);

    RIO_LOG(DBG_RW, "fslRioSetInboundWin():IW RIO_RIWARn 0x%x val 0x%x\n",
        RIO_RIWARn(pDrvCtrl->rioPortNum, window), tempVal, 0, 0, 0, 0);

    }

/********************************************************************
*
* fslRioSetOutboundWin - Set RapidIO controller outbound window
*
* This function configure ATMU outbound windows register, map CPU address to
* RapidIO bus address
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void fslRioSetOutboundWin
    (
    VXB_DEVICE_ID pInst,
    void  *       bar,
    UINT32        window,
    void  *       localAdrs,
    UINT32        size,
    UINT32        destID,
    void  *       rioAdrs,
    UINT32        rType,
    UINT32        wType
    )
    {
    UINT32 tempVal, index;
    RIO_DRV_CTRL * pDrvCtrl =  pInst->pDrvCtrl;

    semTake (pDrvCtrl->rioSem, WAIT_FOREVER);

    for (index = 0; index < 32; index ++)
        {
        if ((size >> index) == 1)
            break;
        }

    index = index - 1;

    RIO_LOG(DBG_RW, "fslRioSetOutboundWin():OW index %d\n", index, 2, 3, 4, 5, 6);

    tempVal = ((UINT32)localAdrs) >> RIO_WIN_ADDR_4KALIGN;

    vxbWrite32 (pDrvCtrl->riohandle, (UINT32 *)((ULONG)bar +
        RIO_ROWBARn(pDrvCtrl->rioPortNum, window)), tempVal);

    RIO_LOG(DBG_RW, "fslRioSetOutboundWin():OW ROWBAR 0x%x val 0x%x\n",
        RIO_ROWBARn(pDrvCtrl->rioPortNum, window), tempVal, 0, 0, 0, 0);

    tempVal = ((destID & 0xff) << 22) | ((UINT32)rioAdrs >> RIO_WIN_ADDR_4KALIGN);

    vxbWrite32 (pDrvCtrl->riohandle, (UINT32 *)((ULONG)bar +
        RIO_ROWTARn(pDrvCtrl->rioPortNum, window)), tempVal);

    RIO_LOG(DBG_RW, "fslRioSetOutboundWin():OW ROWTAR 0x%x val 0x%x\n",
        RIO_ROWTARn(pDrvCtrl->rioPortNum, window), tempVal, 0,0,0,0);

    tempVal = RIO_WIN_ENABLE | RIO_WIN_PCI_ORDER | RIO_RxWAR_TYPE_READ(rType) |
        RIO_RxWAR_TYPE_READ(wType) | index ;

    vxbWrite32 (pDrvCtrl->riohandle, (UINT32 *)((ULONG)bar +
        RIO_ROWARn(pDrvCtrl->rioPortNum, window)), tempVal);

    semGive (pDrvCtrl->rioSem);

    RIO_LOG(DBG_RW, "fslRioSetOutboundWin():OW ROWAR 0x%x val 0x%x\n",
        RIO_ROWARn(pDrvCtrl->rioPortNum, window), tempVal, 0, 0, 0, 0);
    }

/*******************************************************************************
*
* fslRioSmApiGet - method to support shared memory
*
* share memory API get method, share memory END only support one instance,
* only implement on port 0
*
* RETURNS: shared memory funcs struct
*
*/

LOCAL struct sharedMemSupportAPI * fslRioSmApiGet
    (
    VXB_DEVICE_ID  pInst,
    char  *        pArgs
    )
    {
    /*
     * share memory API only support one instance
     * only  implement share memory end on port 0
     */

    if (pInst->unitNumber != 0)
        return NULL;

    return(&fslRioSmApi);
    }

/*******************************************************************************
*
* fslRioCtlrInfoGet - method to get RapidIO host port information
*
* method to get host port information
*
* RETURNS: &fslRioHostPort
*
*/

LOCAL RIO_MPORT_INFO * fslRioCtlrInfoGet
    (
    VXB_DEVICE_ID pInst
    )
    {
    return (&fslRioHostPort);
    }

/*******************************************************************************
*
* fslRioUsrIntGen - generate SRIO user interrupt
*
* This routine generate a RapidIO  interrupt for a specified port. On Freescale
* 5020/3041 board, use Rman DoorBell method to generate interrupt. rioBusIntGen
* doesn't. match RapidIO interrupt connect usage, so use user interrupt method.
*
* RETURNS: OK/ERROR
*
* ERRNO
*/

STATUS fslRioUsrIntGen
    (
    int arg1, /* SM_END parameter SM_INT_ARG1 */
    int arg2, /* SM_END parameter SM_INT_ARG2 */
    int arg3  /* SM_END parameter SM_INT_ARG3 */
    )
    {
    VXB_DEVICE_ID pDev = (VXB_DEVICE_ID)(*(volatile ULONG *)arg1);
    struct msgbuf desc;
    FUNCPTR method = NULL;
    MSG_TRANS_API * msgApi = NULL;
    RIO_TRANS_PARAMETER transArg;

    if (pDev == NULL)
        return ERROR;

    desc.buf = NULL; /* data is useless */
    desc.len = RIO_DBELL_DATA_LEN;
    desc.next = NULL;

    method = vxbDevMethodGet (pDev, (VXB_METHOD_ID)vxbMsgApiGet_desc);

    if (method != NULL)
        msgApi = (MSG_TRANS_API * )(*method)(pDev);

    RIO_LOG(DBG_INIT, "fslRioUsrIntGen():----(%p)(0x%x) \r\n",
        pDev, msgApi, 3,4,5,6);

    transArg.destId = *(volatile UINT32 *)arg2;
    transArg.transType = RIO_TYPE10;

    if (msgApi != NULL)
        return msgApi->intSend (pDev, (ULONG)&transArg, &desc, NULL, NULL);

    return (ERROR);
    }

/**************************************************************************
*
* fslRioBusTas - test and set a specified location
*
* This routine performs a test-and-set (TAS) instruction on the specified
* address.  To prevent deadlocks, interrupts are disabled during the
* test-and-set operation.  The following table defines the method used to
* insure an atomic operation.
*
* RETURNS: TRUE if the value had not been set but is now, or
* FALSE if the value was set already.
*
* SEE ALSO: vxTas()
*
* ERRNO
*/

LOCAL BOOL fslRioBusTas
    (
    char * adrs          /* address to be tested and set*/
    )
    {
    BOOL state = FALSE;
    int  lockKey;

    if (((UINT32) adrs >= (UINT32) rioHostAdrs) && ((UINT32) adrs <
        ((UINT32) rioHostAdrs + fslRioAtmuSmWinSize)))
        {
        lockKey = intCpuLock();

        if (sysProcNumGet() != 0)
            {
            /* perform RMW to try and set TAS location*/

            state = *(volatile UINT32 *)(adrs);

            if (state != 0x0)
                state = FALSE;
            else
                state = TRUE;

            }
        else
            {
            state = vxTas (adrs);
            }

        intCpuUnlock (lockKey);
        }

    RIO_LOG(DBG_INIT, "fslRioBusTas():TasAdrs 0x%x %d\r\n", adrs, state, 3,4,5,6);

    return(state);
    }

/**************************************************************************
*
* fslRioBusTasClear - clear a location set by fslRioBusTasClear()
*
* This routine clears the specified 32-bit location
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void fslRioBusTasClear
    (
    volatile char * adrs    /* Address of semaphore to be cleared*/
    )
    {
    volatile UINT32  state = 0;
    int  lockKey;

    if (((UINT32) adrs >= (UINT32) rioHostAdrs) && ((UINT32) adrs <
        ((UINT32) rioHostAdrs + fslRioAtmuSmWinSize)))
        {
        lockKey = intCpuLock ();

        if (sysProcNumGet() != 0)
            {
            /* lock interrupts so that vxTas() can execute without preemption*/

            state = *(volatile UINT32 *)(adrs);
            }
        else
            {
            WRS_ASM("isync;sync;eieio");
            *(volatile UINT32 *)adrs = 0;
            WRS_ASM("isync;sync;eieio");
            }

        intCpuUnlock (lockKey);
        }
    RIO_LOG(DBG_INIT, "fslRioBusTasClear():TasClsAdrs 0x%x 0x%x \r\n", adrs, state, 3,4,5,6);
    }

