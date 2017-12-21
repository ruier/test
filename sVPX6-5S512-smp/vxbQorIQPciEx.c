/* vxbQorIQPciEx.c - Freescale QorIQ VxBus PCIe driver */

/*
 * Copyright (c) 2009, 2011-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01i,04mar13,j_z  add vxbPciMSIErase, vxbPciDevIntCapabCheck. (WIND00333514)
01j,28mar13,syt  fix coverity error.
01i,18oct12,b_m  add QorIQ T4 support.
01h,31oct12,l_z  add vxbPciMSIGet. (WIND00373602)
01g,04sep12,l_z  switch to use vxbPciIntEnable/Disable. (WIND365027)
01g,30aug12,jpb  Renamed _WRS_CONFIG_MILS_VDK_1_1 to _WRS_CONFIG_MILS_VBI.
01f,12oct11,fao  Decouple QorIQLaw driver.
01e,09sep11,b_m  Do not zero all the registers (WIND00264966)
01d,08aug11,b_m  Fix MSI support; fix lower 16MB inbound window mapping;
                 consolidate support for P5020/P3041 as well as P4080.
01c,21oct09,wap  Test LTSSM register twice during bridge initialization to
                 ensure config space accesses actually work
01b,02oct09,wap  Remove debug code
01a,25sep09,wap  written
*/

/*
DESCRIPTION
This module implements a driver for the PCI Express host bridge in
the Freescale QorIQ family of processors. Depending on the CPU, there
may be up to three PCIe host bridges. The actual availability depends
on the reset control word (RCW) settings, and board layout.

The driver configures the controllers in root complex (RC) mode. Support
is available for both legacy INTx interrupts and MSI interrupts. Support
for extended configuration space accesses is also available.

Each bridge requires a local access window (LAW) in which to map its outbound 
data. If using this driver on a processor which also has the QorIQLaw device, 
and if the QorIQLaw driver is present and registered on vxBus, this driver 
will automatically use the QorIQLaw driver to allocate and set-up the outbound 
windows. If the QorIQLaw is unavailable, it is the responsibility of the BSP 
author to have created corresponding LAW's separately using whatever driver the
particular processor requires. Mappings for prefetchable and non-prefetchable 
memio, io16 and io32 regions are all provided.

EXTERNAL INTERFACE
The driver provides a vxBus external interface. The only exported
routine is the vxbQorIQPexRegister() function, which registers the driver
with VxBus. Since the PCIe controller is a processor local bus device, each
device instance must be specified in the hwconf.c file in a BSP. The hwconf
entry must specify the following paremeters:

\is
\i <regBase>
pex PCI bridge register base address.
The parameter type is HCF_RES_INT.
If not specified, 0 will be set in the register base address and VXB_REG_NONE
will be set in the register base flag that will cause a failure at vxbRegMap()
to obtain the access handle.

\i <mem32Addr>
Specifies the 32-bit prefetchable memory pool base address.
Normally, this is given by the BSP constant PCI_MEM_ADRS.
The parameter type is HCF_RES_ADDR.

\i <mem32Size>
Specifies the 32-bit prefetchable memory pool size.
Normally, this is given by the BSP constant PCI_MEM_SIZE.
The parameter type is HCF_RES_INT.

\i <mem32Io32Addr>
Specifies the 32-bit non-prefetchable memory pool base address.
Normally, this is given by the BSP constant PCI_MEMIO_ADRS.
The parameter type is HCF_RES_ADDR.

\i <memIo32Size>
Specifies the 32-bit non-prefetchable memory pool size
Normally, this is given by the BSP constant PCI_MEMIO_SIZE.
The parameter type is HCF_RES_INT.

\i <io32Addr>
Specifies the 32-bit I/O pool base address.
Normally, this is given by the BSP constant PCI_IO_ADRS.
The parameter type is HCF_RES_ADDR.

\i <io32Size>
Specifies the 32-bit I/O pool size.
Normally, this is given by the BSP constant PCI_IO_SIZE.
The parameter type is HCF_RES_INT.

\i <io16Addr>
Specifies the 16-bit I/O pool base address.
Normally, this is given by the BSP constant PCI_ISA_IO_ADDR.
The parameter type is HCF_RES_ADDR.

\i <io16Size>
Specifies the 16-bit I/O pool size.
Normally, this is given by the BSP constant PCI_ISA_IO_SIZE.
The parameter type is HCF_RES_INT.

\i <msiEnable>
The msiEnable resource specifies the MSI is enabled or not.
If not specified, MSI is disabled.
The parameter type is HCF_RES_INT.

\i <pciExNum>
The pciExNum resource specifies the PCIe host controller number.
This must be set with PCIe host controller IP revision >= 3.0.
Otherwise the pciExNum is determined from the base address of
the controller (pre-T4240 SoCs).
The paramter type is HCF_RES_INT.

\ie

The following parameters are optional and affect the PCI autoconfiguration
code. The bridge will be configured correctly if these are not provided,
but the user may specify them if desired to tune autoconfig behavior.

\is

\i <cacheSize>
Specifies the value used to initialize the cache line size field
in the PCI header. The parameter type is HCF_RES_INT.

\i <maxBusSet>
Specifies the maximum number of buses to be supported on this bridge.
Note that specifying too small a number can interfere with the configuration
of devices on the other side of PCI-PCI bridges.
The parameter type is HCF_RES_INT.

\i <maxLatAllSet>
The <maxLatAllSet> resource specifies a constant
maximum latency value for all cards, if no
function has been specified with <maxLatencyFuncSet>.
The parameter type is HCF_RES_INT.

\i <maxLatencyFuncSet>
The <maxLatencyFuncSet> resource specifies a routine which is
called for each function present on the bus
when discovery takes place.  The routine must accept four
arguments, specifying bus, device, function, and a user-supplied
argument of type void *.  See <maxLatencyArgSet>.  The routine
should return a UINT8 value, which will be put into the MAX_LAT
field of the header structure.  The user supplied routine must
return a valid value each time it is called. There is no mechanism
for any ERROR condition, but a default value can be returned in
such a case. (Default = NULL.)

\i <maxLatencyArgSet>
The <maxLatencyArgSet> resource specifies the fourth argument to be
passed when the routine specified in <maxLatencyFuncSet> is
called.

\i <autoIntRouteSet>
The <autoIntRouteSet> resource enables or disables
automatic interrupt routing across bridges
during the autoconfig process.

\i <msgLogSet>
The <msgLogSet> resource specifies a routine which is called to print
warning or error messages from vxbPciAutoConfigLib if logMsg() has
not been initialized at the time vxbPciAutoConfigLib is used. The
specified routine must accept arguments in the same format as logMsg(),
but it does not necessarily need to print the actual message.
The parameter type is HCF_RES_ADDR.

\i <includeFuncSet>
The <includeFuncSet> resource specifies an optional routine to be called
by PCI AutoConfig for each and every function encountered
in the scan phase, to determine whether each function
should be auto-configured.  The routine takes as input both the
bus-device-function tuple, and a 32-bit quantity containing both
the PCI vendorID and deviceID of the function.  The BSP
developer may use any combination of the input data to ascertain
whether a device is to be excluded from the autoconfig process.
The exclusion routine then returns ERROR if a device is to be
excluded, and OK if a device is to be included in the
autoconfiguration process.
The parameter type is HCF_RES_ADDR.

\i <intAssignFuncSet>
The <intAssignFuncSet> resource specifies an optional routine which can
be used to assign specific interrupt level values to
particular functions.  The routine takes as input both the
bus-device-function tuple, and an 8-bit quantity containing the
contents of the interrupt Pin register from the PCI configuration
header of the device under consideration.  The interrupt pin
register specifies which of the four PCI Interrupt request lines
available are connected.  This routine may use any combination
of these data to ascertain the interrupt level.  This value is
returned from the function, and  is programmed into the interrupt
line register of the function's PCI configuration header.
The parameter type is HCF_RES_ADDR.

\i <bridgePreConfigFuncSet>
The <bridgePreConfigFuncSet> resource specifies an optional bridge
pre-configuration pass initialization routine which is
provided so that the BSP Developer can initialize a bridge device
prior to the configuration pass on the bus that the bridge
implements.  This optional user-specified routine takes as
input both the bus-device-function tuple, and a 32-bit quantity
containing both the PCI deviceID and vendorID of the device.
This routine may use any combination of these input data to
ascertain any special initialization requirements of a particular
type of bridge at a specified geographic location.
The parameter type is HCF_RES_ADDR.

\i <bridgePostConfigFuncSet>
The <bridgePostConfigFuncSet> resource specifies an optional bridge
post-configuration pass initialization routine which is
provided so that the BSP Developer can initialize a bridge device
after the configuration pass on the bus that the bridge
implements.  This optional user-specified routine takes as
input both the bus-device-function tuple, and a 32-bit quantity
containing both the PCI deviceID and vendorID of the device.
This routine may use any combination of these input data to
ascertain any special initialization requirements of a particular
type of bridge at a specified geographic location.
The parameter type is HCF_RES_ADDR.

\i <rollcallFuncSet>
The <rollcallFuncSet> resource specifies an optional roll call
routine, to delay PCI initialization until all devices
have had time to initialize.  If a roll call routine has
been configured, before any configuration is actually done,
the roll call routine is called repeatedly until it returns
TRUE.  A return value of TRUE indicates that all of the
PCI devices which are going to appear on the busses have
appeared and we can proceed with PCI bus configuration.
The parameter type is HCF_RES_ADDR.

\i <fbbEnable>
Enable and disable the functions which check Fast Back To Back
functionality.  PCI_FBB_UPDATE is for use with dynamic/HA
applications.  It first disables FBB on all functions, then enables
FBB on all functions, if appropriate.  In HA applications, it should
be called any time a card is added or removed.  The BOOL pointed to
by pArg for PCI_FBB_ENABLE and PCI_FBB_UPDATE is set to TRUE if all
cards allow FBB functionality and FALSE if either any card does not
allow FBB functionality or if FBB is disabled.  The BOOL pointed to
by pArg for PCI_FBB_STATUS_GET is set to TRUE if PCI_FBB_ENABLE has
been called and FBB is enabled, even if FBB is not activated on any
card.  It is set to FALSE otherwise.
This parameter is optional, the default is FALSE.
The parameter type is HCF_RES_INT.

\ie

By convention all the BSP-specific device parameters are registered in
a file called hwconf.c, which is #include'ed by sysLib.c. The following is
an example for configuring a single bridge:

\cs
const struct hcfResource pci0Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x200000) } },
    { "mem32Addr",      HCF_RES_ADDR,   { (void *)PCIEX1_MEM_ADRS } },
    { "mem32Size",      HCF_RES_INT,    { (void *)PCIEX1_MEM_SIZE } },
    { "memIo32Addr",    HCF_RES_ADDR,   { (void *)PCIEX1_MEMIO_ADRS } },
    { "memIo32Size",    HCF_RES_INT,    { (void *)PCIEX1_MEMIO_SIZE } },
    { "io32Addr",       HCF_RES_ADDR,   { (void *)PCIEX1_IO32_ADRS } },
    { "io32Size",       HCF_RES_INT,    { (void *)PCIEX1_IO32_SIZE } },
    { "io16Addr",       HCF_RES_ADDR,   { (void *)PCIEX1_IO_ADRS } },
    { "io16Size",       HCF_RES_INT,    { (void *)PCIEX1_IO_SIZE } },
    { "lawBase",        HCF_RES_ADDR,   { (void *)PCIEX1_LAW_BASE } },
    { "lawSize",        HCF_RES_INT,    { (void *)PCIEX1_LAW_SIZE } },
    { "includeFuncSet",     HCF_RES_ADDR, { (void *)sysPci1AutoconfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR, { (void *)sysPci1AutoconfigIntrAssign } },
    { "msiEnable",      HCF_RES_INT,    { (void *)TRUE } }
};

#define pci0Num NELEMENTS(pci0Resources)

const struct hcfDevice hcfDeviceList[] = {

        ...

    { "QorIQPciEx", 0, VXB_BUSID_PLB,0, pci0Num, pci0Resources },

        ...
};
\ce


SEE ALSO: vxbPci and VxWorks Device Driver Developer's Guide.
*/

#include <vxWorks.h>

#include <drv/pci/pciIntLib.h>
#include <drv/pci/pciConfigLib.h>
#include <iv.h>
#include <intLib.h>
#include <stdlib.h>
#include <cacheLib.h>
#include <spinLockLib.h>

#include <hwif/vxbus/vxBus.h>
#include <vxBusLib.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/vxbus/vxbPciLib.h>
#include <hwif/util/vxbDmaBufLib.h>
#include <hwif/util/hwMemLib.h>
#include <hwif/util/vxbParamSys.h>
#include <../src/hwif/h/vxbus/vxbAccess.h>
#include <../src/hwif/h/vxbus/vxbPciBus.h>
#include <../src/hwif/intCtlr/vxbIntCtlrLib.h>
#include <../src/hwif/intCtlr/vxbIntDynaCtlrLib.h>

#include <../src/hwif/h/resource/vxbQorIQLaw.h>
#include <../src/hwif/h/resource/vxbQorIQTgtId.h>

/*
 * Putting device-specific structures and register information in
 * a separate header file is recommended, though not required. It helps
 * reduce clutter in the source code if the device has many registers.
 */

#include <../src/hwif/h/busCtlr/vxbQorIQPciEx.h>

IMPORT STATUS	vxbPciIntDisconnect2 (VXB_DEVICE_ID, VOIDFUNCPTR *,
    VOIDFUNCPTR, int);

/* VxBus methods */

LOCAL void	pexInstInit (VXB_DEVICE_ID);
LOCAL void	pexInstInit2 (VXB_DEVICE_ID);
LOCAL void	pexInstConnect (VXB_DEVICE_ID);
LOCAL STATUS	pexInstUnlink (VXB_DEVICE_ID, void *);

/* PCI methods */

LOCAL STATUS	pexMethodDevCfgRead (VXB_DEVICE_ID, PCI_HARDWARE *,
    UINT32, UINT32, void *);
LOCAL STATUS	pexMethodDevCfgWrite (VXB_DEVICE_ID, PCI_HARDWARE *,
    UINT32, UINT32, UINT32);

LOCAL STATUS	pexBaseAddressConvert (VXB_DEVICE_ID, UINT32 *);
LOCAL STATUS	pexConfigInfo (VXB_DEVICE_ID, char *);
LOCAL STATUS	pexInterruptInfo (VXB_DEVICE_ID, char *);

/* Access methods */

LOCAL STATUS	pexDevControl (VXB_DEVICE_ID, pVXB_DEVCTL_HDR);

/* Chip-specific initialization code. */

LOCAL STATUS	pexBridgeInit (VXB_DEVICE_ID);

/* Driver utility functions */

LOCAL UINT32	pexWinSize (UINT64);

LOCAL STATUS	pexMSIProgram (VXB_DEVICE_ID, struct vxbIntDynaVecInfo *);

/* locals */

LOCAL struct drvBusFuncs pexFuncs =
    {
    pexInstInit,	/* devInstanceInit */
    pexInstInit2,	/* devInstanceInit2 */
    pexInstConnect	/* devInstanceConnect */
    };

LOCAL struct vxbDeviceMethod pexMethodList[] =
    { 
    DEVMETHOD(busCtlrDevCfgRead, pexMethodDevCfgRead),
    DEVMETHOD(busCtlrDevCfgWrite, pexMethodDevCfgWrite),
    DEVMETHOD(busCtlrDevCtlr, pexDevControl),

    DEVMETHOD(busCtlrBaseAddrCvt, pexBaseAddressConvert),

    DEVMETHOD(vxbIntDynaVecProgram, pexMSIProgram),
    DEVMETHOD(vxbIntDynaVecEnable, vxbPciMSIEnable),
    DEVMETHOD(vxbIntDynaVecDisable, vxbPciMSIDisable),
    DEVMETHOD(vxbIntDynaVecErase, vxbPciMSIErase),
    DEVMETHOD(vxbIntDynaVecGet, vxbPciMSIGet),
    DEVMETHOD(vxbDevIntCapabCheck, vxbPciDevIntCapabCheck),

    { PCI_CONTROLLER_METHOD_CFG_INFO, (FUNCPTR)pexConfigInfo},
    { PCI_CONTROLLER_METHOD_INTERRUPT_INFO, (FUNCPTR)pexInterruptInfo},

    DEVMETHOD(vxbDrvUnlink, pexInstUnlink),

    DEVMETHOD_END
    };

LOCAL struct vxbDevRegInfo pexDevRegistration =
    {
    NULL,		/* pNext */
    VXB_DEVID_BUSCTRL,	/* pDev */
    VXB_BUSID_PLB,	/* busID = Processor Local Bus */
    VXB_VER_4_0_0,	/* vxbVersion */
    "QorIQPciEx",	/* drvName */
    &pexFuncs,		/* pDrvBusFuncs */
    pexMethodList,	/* pMethods */
    NULL,		/* devProbe */
    NULL		/* pParamDefaults */
    };

/******************************************************************************
*
* vxbQorIQPexRegister - register pex driver
*
* This routine registers the pex driver with the vxBus subsystem.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void vxbQorIQPexRegister(void)
    {
    vxbDevRegister ((struct vxbDevRegInfo *)&pexDevRegistration);/*设备组册*/
    }

/******************************************************************************
*
* pexInstInit - initialize pex device
*
* This is the pex initialization routine.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void pexInstInit
    (
    VXB_DEVICE_ID pDev
    )
    {
    PEX_DRV_CTRL * pDrvCtrl;
    struct hcfDevice * pHcf;
    PCI_HARDWARE pciDev;
    STATUS r;

    /* Allocate pDrvCtrl */

    pDrvCtrl = (PEX_DRV_CTRL *)hwMemAlloc (sizeof (PEX_DRV_CTRL));

    if (pDrvCtrl == NULL)
        return;

    /* Set bus/dev/func to 0 */

    bzero ((char *)&pciDev, sizeof(pciDev));

    SPIN_LOCK_ISR_INIT (&pDrvCtrl->pexLock, 0);

    pDrvCtrl->pexConfig =
        (struct vxbPciConfig *)hwMemAlloc(sizeof(struct vxbPciConfig));

    if (pDrvCtrl->pexConfig == NULL)
        goto fail;

    pDrvCtrl->pexIntInfo =
        (struct vxbPciInt *)hwMemAlloc(sizeof(struct vxbPciInt));

    if (pDrvCtrl->pexIntInfo == NULL)
        goto fail;

    pDev->pDrvCtrl = pDrvCtrl;

    if (vxbRegMap (pDev, 0, &pDrvCtrl->pexHandle) != OK)
        goto fail;
    pDrvCtrl->pexBar = pDev->pRegBase[0];

    /*
     * A PCI bridge driver usually wants to perform autoconfiguration
     * of devices on the bus, particularly if VxWorks is the first thing
     * that runs when the CPU comes out of reset (i.e. there is no other
     * BIOS or firmware to do this work for us). This flag can be set
     * using a resource entry in hwconf if desired.
     */

    pDrvCtrl->pexAutoConfig = TRUE;

    /* Perform bridge hardware initialization. */

    if (pexBridgeInit (pDev) != OK)
        return;

    /* Initialize PCI interrupt handling */

    vxbPciIntLibInit (pDrvCtrl->pexIntInfo);

    /* Use the vxbPciIntEnable/Disable */

    pDrvCtrl->pexIntInfo->pciIntEnableUsed = TRUE;

    /* Announce the creation of the bus. */

    vxbBusAnnounce (pDev, VXB_BUSID_PCI);

    pHcf = (struct hcfDevice *)hcfDeviceGet (pDev);

    if (pHcf == NULL)
        goto fail;

    /*
     * resourceDesc {
     * The maxBusSet resource specifies the
     * highest sub-bus number. }
     */

    (void)devResourceGet (pHcf, "maxBusSet", HCF_RES_INT,
            (void *)&pDrvCtrl->pexMaxBus);

    if (vxbPciConfigLibInit (pDrvCtrl->pexConfig,
        pDrvCtrl->pexMaxBus) != OK)
        goto fail;

    /* perform PCI autoconfiguration */

    if (pDrvCtrl->pexAutoConfig == TRUE)
        vxbPciAutoConfig (pDev);

    /* call the PCI bus type initialization function */

    vxbPciBusTypeInit (pDev);

    /* Make sure bridge is enabled */

    pexMethodDevCfgWrite (pDev, &pciDev, PCI_CFG_COMMAND, 2,
        PCI_CMD_MASTER_ENABLE);

    return;

fail:

    if (pDrvCtrl->pexConfig != NULL)
        hwMemFree ((char *)pDrvCtrl->pexConfig);
    if (pDrvCtrl->pexIntInfo != NULL)
        hwMemFree ((char *)pDrvCtrl->pexIntInfo);
    hwMemFree ((char *)pDrvCtrl);

    return;
    }

/******************************************************************************
*
* pexInstInit2 - second stage of pex pci device initialization
*
* This is the routine called to perform the second stage of pex
* initialization.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void pexInstInit2
    (
    VXB_DEVICE_ID pDev
    )
    {
    PEX_DRV_CTRL * pDrvCtrl;

    pDrvCtrl = pDev->pDrvCtrl;

    /*
     * Flag to indicate driver can now safely use spinLocks. At
     * least on the PowerPC architecture, spinLocks require atomic
     * operators constructed using the 'load linked/store-conditional'
     * mechanism, but that approach is only guaranteed to work when
     * the data cache is enabled. This is because synchronization
     * of atomic operations between two cores in a multicore system
     * is implemented by using the cache snoop mechanism to transmit
     * information about updates to cached buffers between cores.
     * If the cache is not on, the CPU can't guarantee that validity
     * of atomic operations, so it triggers machine check.
     * In most cases, the cache is only off during the early stages
     * of OS initialization, during which the system is running
     * single-threaded, so there's no chance of overlapping operations.
     * Once we reach the instInit2 method in a VxBus driver though,
     * we know the kernel as been fully initialized and the caches
     * should be enabled.
     */

    pDrvCtrl->pexInitialized = TRUE;

    }

/******************************************************************************
*
* pexInstConnect - third stage of pex pci device initialization
*
* This is the routine called to perform the third stage of pex
* initialization.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void pexInstConnect
    (
    VXB_DEVICE_ID pDev
    )
    {
    return;
    }

/*****************************************************************************
*
* pexInstUnlink -  VxBus unlink handler
*
* This function shuts down an pex device instance in response to an
* unlink event from VxBus. This may occur if our VxBus instance has
* been terminated, or if the pex driver has been unloaded. When an
* unlink event occurs, we must shut down the PCI bridge associated with
* with this device instance and then release all the resources allocated
* during instance creation.
*
* RETURNS: OK if device was successfully destroyed, otherwise ERROR
*
* ERRNO: N/A
*/

LOCAL STATUS pexInstUnlink
    (
    VXB_DEVICE_ID pDev,
    void * unused
    )
    {
    PEX_DRV_CTRL * pDrvCtrl;

    pDrvCtrl = pDev->pDrvCtrl;

    hwMemFree ((char *) pDrvCtrl->pexConfig);
    hwMemFree ((char *) pDrvCtrl->pexIntInfo);

    /* Destroy the adapter context. */

    hwMemFree ((char *)pDrvCtrl);
    pDev->pDrvCtrl = NULL;

    /* Goodbye cruel world. */

    return (OK);
    }

/******************************************************************************
*
* pexBridgeInit - PCI bridge initialization
*
* This routine performs initialization of the PCI bridge hardware. This
* may include resetting the bridge device and programming its window
* translation registers.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL STATUS pexBridgeInit
    (
    VXB_DEVICE_ID pDev
    )
    {
    PEX_DRV_CTRL * pDrvCtrl;
    struct hcfDevice * pHcf;
    PCI_HARDWARE pciDev;
    LAW_HANDLE lawHandle;
    UINT32 addr;
    UINT32 size;
    UINT16 ltssm;
    STATUS r;
    int i;

    pDrvCtrl = pDev->pDrvCtrl;

    pHcf = (struct hcfDevice *)hcfDeviceGet (pDev);

    if (pHcf == NULL)
        return (ERROR);

    /* Set bus/dev/func to 0 */

    bzero ((char *)&pciDev, sizeof(pciDev));

    /*
     * resourceDesc {
     * The msiEnable resource specifies the
     * MSI is enabled or not. }
     */

    (void)devResourceGet(pHcf, "msiEnable",
            HCF_RES_INT, (void*)&pDrvCtrl->pexMsi);

    /*   
     * resourceDesc {
     * The pciExNum resource specifies the
     * PCIe host controller number. }
     */

    (void)devResourceGet(pHcf, "pciExNum",
            HCF_RES_INT, (void*)&pDrvCtrl->pexNum);

    /* Reset the link */

    CSR_SETBIT_4(pDev, PEX_DBMODE, PEX_DBMODE_GLR);
    for (i = 0; i < 200000; i++)
        CSR_READ_4(pDev, PEX_DBMODE);
    CSR_CLRBIT_4(pDev, PEX_DBMODE, PEX_DBMODE_GLR);

    /*
     * Check to see if link training has completed. If not, then
     * there's probably no card plugged into this slot, and we
     * should avoid allowing this bus to be probed, otherwise the
     * CPU may hang.
     *
     * Note that we read the register twice here. In the case where
     * we are loaded as an AMP instance from Linux, it's possible
     * that Linux may have left the config space registers in
     * an inconsistent state, which may cause the first config
     * space access to fail.
     */

    if ((CSR_READ_4(pDev, PEX_IBLK_1) & PEX_IBLK_1_REV_MASK) >= PEX_IBLK_1_REV_30)
        {
        ltssm = (CSR_READ_4(pDev, PEX_CSR0) >> PEX_CSR0_LTSSM_SHIFT);

        if (ltssm != PEX_CSR0_LTSSM_L0)
            return (ERROR);
        }
    else
        {
    pexMethodDevCfgRead (pDev, &pciDev, PEX_CFG_LTSSM, 2, &ltssm);
    pexMethodDevCfgRead (pDev, &pciDev, PEX_CFG_LTSSM, 2, &ltssm);

    if (ltssm != PEX_CFG_LTSSM_L0)
        return (ERROR); 
        }

    /* Clear any pending errors. */

    CSR_WRITE_4(pDev, PEX_ERR_DR, 0xFFFFFFFF);

    /*
     * For MILS, the LAW driver is likely not going to
     * be present, so we have to assume the LAW for a given
     * PCI controller is already going to be present and
     * matches the address specified in hwconf.c.
     */

#ifndef _WRS_CONFIG_MILS_VBI

    /*
     * If the QorIQLaw driver is present in the kernel, we will
     * automatically use it to allocate the outbound LAW's, otherwise it
     * is the responsibility of the BSP author to have done it manually.
     */

    if (vxbInstByNameFind(QORIQ_LAW_NAME, 0) != NULL)
        {
        /* Find a LAW for this bridge. */

        bzero ((char *)&lawHandle, sizeof(LAW_HANDLE));

        switch ((UINT32)pDrvCtrl->pexBar & 0x0000FFFF)
            { 
            case 0x0000:
                lawHandle.lawTgt = QORIQ_TGTID_PCIE0;
                break;
            case 0x1000:
                lawHandle.lawTgt = QORIQ_TGTID_PCIE1;
                break;
            case 0x2000:
                lawHandle.lawTgt = QORIQ_TGTID_PCIE2;
                break;
            case 0x3000:
                lawHandle.lawTgt = QORIQ_TGTID_PCIE3;
                break;
            default:
                break;
            }

        /*
         * Override the LAW target ID with PCIe host controller number,
         * if the PCIe IP revision >= 3.0
         */

        if ((CSR_READ_4(pDev, PEX_IBLK_1) & PEX_IBLK_1_REV_MASK) >= PEX_IBLK_1_REV_30)
            lawHandle.lawTgt = pDrvCtrl->pexNum;

        lawHandle.lawDev = pDev;

        r = vxbResourceFind (pDev, DEVMETHOD_CALL(QorIQLawFind),
            (char *)&lawHandle, 0);

        /* No LAW exists; create one. */

        if (r != OK)
            {
            r = devResourceGet (pHcf, "lawBase", HCF_RES_ADDR, (void *)&addr);
            if (r != OK)
                return ERROR;

            r = devResourceGet (pHcf, "lawSize", HCF_RES_INT, (void *)&size);
            if (r != OK)
                return ERROR;

            lawHandle.lawBase = (void *)addr;
            lawHandle.lawSize = pexWinSize (size);

           
            r = vxbResourceFind (pDev, DEVMETHOD_CALL(QorIQLawAlloc),
                (char *)&lawHandle, 0);

            if (r != OK)
                return (ERROR);
            }
        }

#endif /* _WRS_CONFIG_MILS_VBI */

    /* Initialize outbound window 1 */

    r = devResourceGet (pHcf, "mem32Addr", HCF_RES_ADDR, (void *)&addr);
    if (r != OK)
        return ERROR;

    r = devResourceGet (pHcf, "mem32Size", HCF_RES_INT, (void *)&size);
    if (r != OK)
        return ERROR;

    CSR_WRITE_4(pDev, PEX_OWBAR1, PEX_ADDR(addr));
    CSR_WRITE_4(pDev, PEX_OTAR1, PEX_ADDR(addr));
    CSR_WRITE_4(pDev, PEX_OWAR1, PEX_OTAR_EN | PEX_RTT(PEX_RTT_MEM) |
        PEX_WTT(PEX_WTT_MEM) | pexWinSize (size));

    /* Initialize outbound window 2 */

    r = devResourceGet (pHcf, "memIo32Addr", HCF_RES_ADDR, (void *)&addr);
    if (r != OK)
        return ERROR;

    r = devResourceGet (pHcf, "memIo32Size", HCF_RES_INT, (void *)&size);
    if (r != OK)
        return ERROR;

    CSR_WRITE_4(pDev, PEX_OWBAR2, PEX_ADDR(addr));
    CSR_WRITE_4(pDev, PEX_OTAR2, PEX_ADDR(addr));
    CSR_WRITE_4(pDev, PEX_OWAR2, PEX_OTAR_EN | PEX_RTT(PEX_RTT_MEM) |
        PEX_WTT(PEX_WTT_MEM) | pexWinSize (size));

    /* Initialize outbound window 3 */

    r = devResourceGet (pHcf, "io32Addr", HCF_RES_ADDR, (void *)&addr);
    if (r != OK)
        return ERROR;

    r = devResourceGet (pHcf, "io32Size", HCF_RES_INT, (void *)&size);
    if (r != OK)
        return ERROR;

    CSR_WRITE_4(pDev, PEX_OWBAR3, PEX_ADDR(addr));
    CSR_WRITE_4(pDev, PEX_OTAR3, PEX_ADDR(addr));
    CSR_WRITE_4(pDev, PEX_OWAR3, PEX_OTAR_EN | PEX_RTT(PEX_RTT_IO) |
        PEX_WTT(PEX_WTT_IO) | pexWinSize (size));

    /* Initialize outbound window 4 */

    r = devResourceGet (pHcf, "io16Addr", HCF_RES_ADDR, (void *)&addr);
	if (r != OK)
	    return ERROR;
		
    r = devResourceGet (pHcf, "io16Size", HCF_RES_INT, (void *)&size);
    if (r != OK)
	    return ERROR;

    CSR_WRITE_4(pDev, PEX_OWBAR4, PEX_ADDR(addr));
    CSR_WRITE_4(pDev, PEX_OTAR4, PEX_ADDR(addr));
    CSR_WRITE_4(pDev, PEX_OWAR4, PEX_OTAR_EN | PEX_RTT(PEX_RTT_IO) |
        PEX_WTT(PEX_WTT_IO) | pexWinSize (size));

    /* Disable inbound window 0 */

    CSR_WRITE_4(pDev, PEX_IWAR1, 0);

    /* Initialize inbound window 1 */

    CSR_WRITE_4(pDev, PEX_IWAR2, PEX_ITAR_EN | PEX_ITAR_PF |
        PEX_TRGT(PEX_TRGT_MEM) | PEX_RTT(PEX_RTT_MEM_SNOOP) |
        PEX_WTT(PEX_WTT_MEM_SNOOP) | PEX_WSIZE_64GB);

    /* Disable MSI inbound window */

    CSR_WRITE_4(pDev, PEX_MSIIWAR, 0);

    /* Retrieve CCSR base address from PEXMSIITAR */
    
    pDrvCtrl->ccsrBar = (void *)CSR_READ_4(pDev, PEX_MSIITAR);
    pDrvCtrl->ccsrBar = (void *)((UINT32)pDrvCtrl->ccsrBar & PEX_CCSR_MASK);
    pDrvCtrl->ccsrBar = (void *)((UINT32)pDrvCtrl->ccsrBar << PEX_CCSR_SHIFT);

    return (OK);
    }

/******************************************************************************
*
* pexMethodDevCfgRead - VxBus PCI config read method
*
* This is the PCI configuration read method exported by this driver to
* the VxBus vxbPci.c module. It performs a configuration space read on a
* given PCI target. If the target specification or register offset is
* invalid, it will return error.
*
* RETURNS: OK, or ERROR if target specification or register is invalid
*
* ERRNO: N/A
*/

LOCAL STATUS pexMethodDevCfgRead
    (
    VXB_DEVICE_ID pDev,		/* device info */
    PCI_HARDWARE * pPciDev,	/* PCI device info */
    UINT32 byteOffset,		/* offset into cfg space */
    UINT32 transactionSize,	/* transaction size, in bytes */
    void * pDataBuf		/* buffer to read-from/write-to */
    )
    {
    STATUS retVal = OK;
    PEX_DRV_CTRL * pDrvCtrl;
    int bus, dev, func;
    UINT32 addr;
    volatile UINT32 val;

    pDrvCtrl = pDev->pDrvCtrl;

    bus = pPciDev->pciBus;
    dev = pPciDev->pciDev;
    func = pPciDev->pciFunc;

    addr = PEX_BUSN(bus);
    addr |= PEX_DEVN(dev);
    addr |= PEX_FUNCN(func);
    addr |= PEX_REGN(byteOffset);
    addr |= PEX_XREGN(byteOffset);

    if (pDrvCtrl->pexInitialized == TRUE)
        SPIN_LOCK_ISR_TAKE (&pDrvCtrl->pexLock);

    CSR_WRITE_4(pDev, PEX_CFG_ADDR, addr | PEX_CFGADDR_EN);
    val = CSR_READ_4(pDev, PEX_CFG_DATA);
    CSR_WRITE_4(pDev, PEX_CFG_ADDR, 0);
    if (CSR_READ_4(pDev, PEX_ERR_DR) & PEX_ERRDR_ICCA)
        {
        retVal = ERROR;
        CSR_WRITE_4(pDev, PEX_ERR_DR, PEX_ERRDR_ICCA);
        }

    if (pDrvCtrl->pexInitialized == TRUE)
        SPIN_LOCK_ISR_GIVE (&pDrvCtrl->pexLock);

    if (retVal == ERROR)
        val = 0xFFFFFFFF;
    else
        val = le32toh(val);

    switch (transactionSize)
        {
        case 1:
            *(UINT8 *)pDataBuf = val >> ((byteOffset & 3) * 8);
            break;
        case 2:
            *(UINT16 *)pDataBuf = val >> ((byteOffset & 2) * 8);
            break;
        case 4:
            *(UINT32 *)pDataBuf = val;
            break;
        default:
            break;
        }

    return (retVal);
    }

/******************************************************************************
*
* pexMethodDevCfgWrite - VxBus PCI config write method
*
* This is the PCI configuration write method exported by this driver to
* the VxBus vxbPci.c module. It performs a configuration space write on a
* given PCI target. If the target specification or register offset is
* invalid, it will return error.
*
* RETURNS: OK, or ERROR if target specification or register is invalid
*
* ERRNO: N/A
*/

LOCAL STATUS pexMethodDevCfgWrite
    (
    VXB_DEVICE_ID pDev,		/* device info */
    PCI_HARDWARE * pPciDev,	/* PCI device info */
    UINT32 byteOffset,		/* offset into cfg space */
    UINT32 transactionSize,	/* transaction size, in bytes */
    UINT32 data			/* data write to the offset */
    )
    {
    STATUS retVal = OK;
    PEX_DRV_CTRL * pDrvCtrl;
    int bus, dev, func;
    UINT32 addr;
    volatile UINT32 val;
    UINT32 mask;

    pDrvCtrl = pDev->pDrvCtrl;

    bus = pPciDev->pciBus;
    dev = pPciDev->pciDev;
    func = pPciDev->pciFunc;

    addr = PEX_BUSN(bus);
    addr |= PEX_DEVN(dev);
    addr |= PEX_FUNCN(func);
    addr |= PEX_REGN(byteOffset);
    addr |= PEX_XREGN(byteOffset);

    switch (transactionSize)
        {
        case 1:
            mask = 0xFF;
            mask = ~(mask << ((byteOffset & 0x3) * 8));
            break;
        case 2:
            mask = 0xFFFF;
            mask = ~(mask << ((byteOffset & 0x2) * 8));
            break;
        case 4:
            mask = 0;
            break;
        default:
            return(ERROR);
        }

    if (pDrvCtrl->pexInitialized == TRUE)
        SPIN_LOCK_ISR_TAKE (&pDrvCtrl->pexLock);

    CSR_WRITE_4(pDev, PEX_CFG_ADDR, addr | PEX_CFGADDR_EN);
    val = CSR_READ_4(pDev, PEX_CFG_DATA);

    val = le32toh(val);

    val &= mask;
    val |= data << ((byteOffset & 0x3) * 8);

    val = htole32(val);

    CSR_WRITE_4(pDev, PEX_CFG_DATA, val);
    val = CSR_READ_4(pDev, PEX_CFG_DATA);
    CSR_WRITE_4(pDev, PEX_CFG_ADDR, 0);
    if (CSR_READ_4(pDev, PEX_ERR_DR) & PEX_ERRDR_ICCA)
        {
        retVal = ERROR;
        CSR_WRITE_4(pDev, PEX_ERR_DR, PEX_ERRDR_ICCA);
        }

    if (pDrvCtrl->pexInitialized == TRUE)
        SPIN_LOCK_ISR_GIVE (&pDrvCtrl->pexLock);

    return (retVal);
    }

/******************************************************************************
*
* pexDevControl - vxbAccess devControl method
*
* This routine is the busCtlrDevCtlr method specific to this
* controller. It's used primarily to retrieve the
* the interrupt vector number used by the vxbPci module when drivers
* perform a vxbIntConnect(), vxbIntDisconnect(), intEnable() or
* intDisable() operation. This routine can also be used to retrieve
* the hardware-specific interrupt number for a given device. (In
* rare cases, a PCI function driver may need to know this.)
*
* RETURNS: OK, or ERROR if target specification or register is invalid
*
* ERRNO: N/A
*/

LOCAL STATUS pexDevControl
    (
    VXB_DEVICE_ID pDev,
    pVXB_DEVCTL_HDR pBusDevControl
    )
    {
    STATUS rc = OK;
    VXB_DEVICE_ID pBusDev;
    PEX_DRV_CTRL * pDrvCtrl;
    pVXB_ACCESS_INT_VECTOR_GET vecGetArg;
    pVXB_ACCESS_INTERRUPT_INFO intArg;
    struct pciIntrEntry * pciIntInfo;
    struct vxbIntDynaVecInfo dynaArg;
    VOIDFUNCPTR * ivec;
    int inum = 0;

    VXB_ASSERT(((pDev != NULL) && (pBusDevControl != NULL)), ERROR)

    pBusDev = vxbDevParent(pDev);
    pDrvCtrl = pBusDev->pDrvCtrl;
    pciIntInfo = (struct pciIntrEntry *)(pDev->pIntrInfo);

    /*
     * Decode and validate arguments. The argument supplied with
     * the 'vector get' method is a pVXB_ACCESS_INT_VECTOR_GET,
     * while for all other methods it's a pVXB_ACCESS_INTERRUPT_INFO.
     */

    if (pBusDevControl->vxbAccessId == VXB_ACCESS_INT_VEC_GET)
        {
        vecGetArg = (pVXB_ACCESS_INT_VECTOR_GET) pBusDevControl;

        /* check if the index is valid */
        if (vecGetArg->intIndex >= 255)
            return (ERROR);

        inum = pciIntInfo->intVecInfo[vecGetArg->intIndex].index;
        }
    else
        {
        intArg = (pVXB_ACCESS_INTERRUPT_INFO) pBusDevControl;

        /* check if the index is valid */
        if (intArg->intIndex >= 255)
            return (ERROR);

        ivec =
            (VOIDFUNCPTR *)pciIntInfo->intVecInfo[intArg->intIndex].intVector;
        }

    switch (pBusDevControl->vxbAccessId)
        {
        case VXB_ACCESS_INT_VEC_GET:
            vecGetArg = (pVXB_ACCESS_INT_VECTOR_GET) pBusDevControl;

            if (pDrvCtrl->pexMsi == TRUE && vxbPciMSIIsCap (pDev) == TRUE)
                vecGetArg->pIntVector = (VOIDFUNCPTR *)VXB_INTR_DYNAMIC;
            else
                vecGetArg->pIntVector = (VOIDFUNCPTR *)INUM_TO_IVEC(inum);

            break;

        case VXB_ACCESS_INT_ENABLE:
            if (ivec == (VOIDFUNCPTR *)VXB_INTR_DYNAMIC)
                rc = vxbIntEnable (pDev, 0, intArg->pISR, intArg->pParameter);
            else
                rc = vxbPciIntEnable(pDev, ivec, (VOIDFUNCPTR)intArg->pISR,
                    (int)intArg->pParameter, (int)ivec);
            break;


        case VXB_ACCESS_INT_DISABLE:
            if (ivec == (VOIDFUNCPTR *)VXB_INTR_DYNAMIC)
                rc = vxbIntDisable (pDev, 0, intArg->pISR, intArg->pParameter);
            else
                rc = vxbPciIntDisable (pDev, ivec, (VOIDFUNCPTR)intArg->pISR,
                    (int)intArg->pParameter, (int)ivec);

            break;

        case VXB_ACCESS_INT_ACKNOWLEDGE:
            break;

        case VXB_ACCESS_INT_CONNECT:

            /*
             * If dynamic interrupt support is enabled, use the dynamic
             * interrupt connection handler. This is provided to allow
             * PCI function drivers to use MSI interrupts transparently.
             * That is, the driver can continue to use the vxbIntConnect()
             * API instead of explicitly using vxbMsiConnect(), such that
             * the same driver code will work unmodified using either
             * legacy and MSI interrupts.
             */

            if (ivec == (VOIDFUNCPTR *)VXB_INTR_DYNAMIC)
                {
                dynaArg.isr = intArg->pISR;
                dynaArg.pArg = intArg->pParameter;
                rc = _func_vxbIntDynaConnect (pDev, 1, &dynaArg);
                }
            else
                rc = vxbPciIntConnect (pDev, ivec, (VOIDFUNCPTR)intArg->pISR,
                    (int)intArg->pParameter);
            
            break;

        case VXB_ACCESS_INT_DISCONN:

            if (ivec == (VOIDFUNCPTR *)VXB_INTR_DYNAMIC)
                rc = OK;
            else
                rc = vxbPciIntDisconnect2 (pDev, ivec,
                    (VOIDFUNCPTR)intArg->pISR, (int)intArg->pParameter);

            break;

        default:
            rc = ERROR;
            break;
        }

    return (rc);
    }

/******************************************************************************
*
* pexBaseAddressConvert - PCI BAR address conversion method
*
* This routine implements the convertBaseAddress method exported by this
* driver via its VxBus methods table. The vxbPci.c module uses this
* routine to apply fixups to the addresses read from the BARs on
* devices that are enumerated on this bus.
*
* RETURNS: ERROR if base address is invalid, otherwise OK
*
* ERRNO: N/A
*/

LOCAL STATUS pexBaseAddressConvert
    (
    VXB_DEVICE_ID pDev,      /* device info */
    UINT32 * pBaseAddr   /* pointer to base address */
    )
    {
    UINT32  baseAddress;

    /* if base address pointer is NULL, return ERROR */

    if (pBaseAddr == NULL)
        return ERROR;

    baseAddress = *pBaseAddr;
    baseAddress &= ~PCI_BAR_ALL_MASK;
    *pBaseAddr = baseAddress;

    return (OK);
    }

/*****************************************************************************
*
* pexConfigInfo - Returns the PCI config info ptr from bus controller.
*
* \NOMANUAL
*
* RETURNS: OK, or ERROR if this library is not initialized
*
* ERRNO: N/A
*/

LOCAL STATUS pexConfigInfo
    (
    VXB_DEVICE_ID pDev,
    char * pArgs
    )
    {
    PEX_DRV_CTRL * pDrvCtrl;

    pDrvCtrl = pDev->pDrvCtrl;
    *(UINT32*)pArgs = (UINT32)pDrvCtrl->pexConfig;

    return (OK);
    }


/*****************************************************************************
*
* pexInterruptInfo - Returns the PCI interrupt info ptr from the bus
* controller.
*
* \NOMANUAL
*
* RETURNS: OK, or ERROR if this library is not initialized
*
* ERRNO: N/A
*/

LOCAL STATUS pexInterruptInfo
    (
    VXB_DEVICE_ID pDev,
    char * pArgs
    )
    {
    PEX_DRV_CTRL * pDrvCtrl;

    pDrvCtrl = pDev->pDrvCtrl;
    *(UINT32*)pArgs = (UINT32)pDrvCtrl->pexIntInfo;

    return (OK);
    }

/*****************************************************************************
*
* pexWinSize - Convert window size values to window size codes
*
* \NOMANUAL
*
* RETURNS: size code used when programming window attributes
*
* ERRNO: N/A
*/

LOCAL UINT32 pexWinSize
    (
    UINT64 size
    )
    {
    UINT32 win = 0;

    /*
     * Diab can't handle non-integral values
     * in a switch statement.
     */

#if defined(__DCC__)
    switch ((UINT32)(size & 0xFFFFFFFF))
#else
    switch (size)
#endif
        {
        case 0x1000:
            win = PEX_WSIZE_4KB;
            break;
        case 0x2000:
            win = PEX_WSIZE_8KB;
            break;
        case 0x4000:
            win = PEX_WSIZE_16KB;
            break;
        case 0x8000:
            win = PEX_WSIZE_32KB;
            break;
        case 0x10000:
            win = PEX_WSIZE_64KB;
            break;
        case 0x20000:
            win = PEX_WSIZE_128KB;
            break;
        case 0x40000:
            win = PEX_WSIZE_256KB;
            break;
        case 0x80000:
            win = PEX_WSIZE_512KB;
            break;
        case 0x100000:
            win = PEX_WSIZE_1MB;
            break;
        case 0x200000:
            win = PEX_WSIZE_2MB;
            break;
        case 0x400000:
            win = PEX_WSIZE_4MB;
            break;
        case 0x800000:
            win = PEX_WSIZE_8MB;
            break;
        case 0x1000000:
            win = PEX_WSIZE_16MB;
            break;
        case 0x2000000:
            win = PEX_WSIZE_32MB;
            break;
        case 0x4000000:
            win = PEX_WSIZE_64MB;
            break;
        case 0x8000000:
            win = PEX_WSIZE_128MB;
            break;
        case 0x10000000:
            win = PEX_WSIZE_256MB;
            break;
        case 0x20000000:
            win = PEX_WSIZE_512MB;
            break;
        case 0x40000000:
            win = PEX_WSIZE_1GB;
            break;
        case 0x80000000:
            win = PEX_WSIZE_2GB;
            break;
#if !defined(__DCC__)
        case 0x100000000ULL:
            win = PEX_WSIZE_4GB;
            break;
        case 0x200000000ULL:
            win = PEX_WSIZE_8GB;
            break;
        case 0x400000000ULL:
            win = PEX_WSIZE_16GB;
            break;
        case 0x800000000ULL:
            win = PEX_WSIZE_32GB;
            break;
        case 0x1000000000ULL:
            win = PEX_WSIZE_64GB;
            break;
        case 0x2000000000ULL:
            win = PEX_WSIZE_128GB;
            break;
#endif
        default:
            break;
        }

    return (win);
    }

/*****************************************************************************
*
* pexMSIProgram - QorIQ PCIE specific wrapper routine for vxbPciMSIProgram
*
* \NOMANUAL
*
* RETURNS: OK
*
* ERRNO
*/

LOCAL STATUS pexMSIProgram
    (
    VXB_DEVICE_ID pDev,
    struct vxbIntDynaVecInfo *dynaVec
    )
    {
    VXB_DEVICE_ID busCtrlID = vxbDevParent(pDev);
    PEX_DRV_CTRL *pDrvCtrl = busCtrlID->pDrvCtrl;

    dynaVec->vecAddr |= (UINT64)((UINT32)pDrvCtrl->ccsrBar);

    return (vxbPciMSIProgram(pDev, dynaVec));
    }
/*获取总线号上盘的类型，ahci return 0 nvme return 1*/
INT32 p5020GetDiskType(UINT32 BassId)
{
    UINT16 uiVendorId;
    UINT16 uiDeviceId;
    
    pciConfigInWord(BassId,0,0,0,&uiVendorId);/*vendorId*/
    pciConfigInWord(BassId,0,0,2,&uiDeviceId);/*DeviceId*/
    
    if((uiVendorId == NVME_VENDOR_SAM_ID) && uiDeviceId == NVME_DEVICE_SAM_ID)
    {
        return 1;
    }
    else if(uiVendorId == AHCI_VENDOR_SAM_ID && uiDeviceId == AHCI_DEVICE_SAM_ID)
    {
        return 0;
    }
    else 
    {
        return 0;
    }
}
VOID p5020SetPayLoad256(VOID)
{
	UINT32 uiVal = 0x20;
	UINT32 uiLoop = 0;
	UINT16 usValue = 0;


	#if 0
	pciConfigInWord(0,0,0,0x54,&usValue);
	usValue = ((usValue & 0xff1f)|(1<<5));
	pciConfigOutWord(0,0,0,0x54,usValue);
	pciConfigInWord(0,0,0,0x54,&usValue);
	/*printf("%d, %d, %d, 0x%x\n",0,0,0,usValue);*/

	pciConfigInWord(1,0,0,0x70,&usValue);
	usValue = ((usValue & 0xff1f)|(1<<5));
	pciConfigOutWord(1,0,0,0x70,usValue);
	pciConfigInWord(1,0,0,0x70,&usValue);
	/*printf("%d, %d, %d, 0x%x\n",1,0,0,usValue);*/
	
	for(uiLoop = 1; uiLoop <= 19; uiLoop++)
	{
		if((4 <= uiLoop)&&(uiLoop <= 7))
		{
			continue;
		}
		if(uiLoop == 2)
		{
            continue;
		}
		if((11 <= uiLoop)&&(uiLoop<=15))
		{
			continue;
		}

		pciConfigInWord(2, uiLoop, 0, 0x70, &usValue);
	    usValue = ((usValue & 0xff1f)|(1<<5));
	    pciConfigOutWord(2, uiLoop, 0, 0x70, usValue);
		pciConfigInWord(2,uiLoop,0,0x70,&usValue);
	   /* printf("%d, %d, %d, 0x%x\n",2,uiLoop,0,usValue);*/
	}

	for(uiLoop = 3; uiLoop <= 11;uiLoop++)
	{
		if((4 <= uiLoop)&&(uiLoop <= 5))
		{
			continue;
		}

		
		if(uiLoop == 6)
		{
			pciConfigInWord(uiLoop, 0, 0, 0xc8, &usValue);
			usValue = ((usValue & 0xff1f)|(1<<5));
			pciConfigOutWord(uiLoop, 0, 0, 0xc8, usValue);
			pciConfigInWord(uiLoop,0,0,0xc8,&usValue);
			/*printf("%d, %d, %d, 0x%x\n",uiLoop,0,0,usValue);*/

			continue;
		}

		pciConfigInWord(uiLoop, 0, 0, 0x78, &usValue);
	    usValue = ((usValue & 0xff1f)|(1<<5));
	    pciConfigOutWord(uiLoop, 0, 0, 0x78, usValue);
		pciConfigInWord(uiLoop,0,0,0x78,&usValue);
	   /* printf("%d, %d, %d, 0x%x\n",uiLoop,0,0,usValue);*/
	}
    #endif

	for(uiLoop = 0; uiLoop < 4; uiLoop++)
    {
        FSL_WRITE_REG(0X80000000+0X1000*uiLoop+0x78,0x20000000);
    }

	for(uiLoop = 8; uiLoop < 12; uiLoop++)
    {
        FSL_WRITE_REG(0X80000000+0X1000*uiLoop+0x78,0x20000000);
    }

	for(uiLoop = 16; uiLoop < 20; uiLoop++)
    {
        FSL_WRITE_REG(0X80000000+0X1000*uiLoop+0x78,0x20000000);
    }
}

/*void m85xxCapabShow(UINT bus,UINT device,UINT function,UINT8 capOffset)   
capOffset 0x34

*/
VOID p5020SetPayLoad512(VOID)
{
	UINT32 uiVal = 0x20;
	UINT32 uiLoop = 0;
	UINT16 usValue = 0;

    

    pciConfigInWord(0,0,0,0x54,&usValue);
	usValue = ((usValue & 0xff1f)|(1<<6));
	pciConfigOutWord(0,0,0,0x54,usValue);
	pciConfigInWord(0,0,0,0x54,&usValue);
	printf("%d, %d, %d, 0x%x\n",0,0,0,usValue);

	pciConfigInWord(1,0,0,0x70,&usValue);
	usValue = ((usValue & 0xff1f)|(1<<6));
	pciConfigOutWord(1,0,0,0x70,usValue);
	pciConfigInWord(1,0,0,0x70,&usValue);
	printf("%d, %d, %d, 0x%x\n",1,0,0,usValue);
	
	for(uiLoop = 1; uiLoop <= 19; uiLoop++)
	{
		if((4 <= uiLoop)&&(uiLoop <= 7))
		{
			continue;
		}
		if((11 <= uiLoop)&&(uiLoop<=15))
		{
			continue;
		}

		pciConfigInWord(2, uiLoop, 0, 0x70, &usValue);
	    usValue = ((usValue & 0xff1f)|(1<<6));
	    pciConfigOutWord(2, uiLoop, 0, 0x70, usValue);
		pciConfigInWord(2,uiLoop,0,0x70,&usValue);
	    printf("%d, %d, %d, 0x%x\n",2,uiLoop,0,usValue);
	}

	for(uiLoop = 3; uiLoop <= 11;uiLoop++)
	{
		if((4 <= uiLoop)&&(uiLoop <= 5))
		{
			continue;
		}

		if(uiLoop == 6)
		{
		    pciConfigInWord(uiLoop, 0, 0, 0xc8, &usValue);
		    usValue = ((usValue & 0xff1f)|(1<<6));
		    pciConfigOutWord(uiLoop, 0, 0, 0xc8, usValue);
			pciConfigInWord(uiLoop,0,0,0xc8,&usValue);
		    printf("%d, %d, %d, 0x%x\n",uiLoop,0,0,usValue);

			continue;
		}

		pciConfigInWord(uiLoop, 0, 0, 0x78, &usValue);
	    usValue = ((usValue & 0xff1f)|(1<<6));
	    pciConfigOutWord(uiLoop, 0, 0, 0x78, usValue);
		pciConfigInWord(uiLoop,0,0,0x78,&usValue);
	    printf("%d, %d, %d, 0x%x\n",uiLoop,0,0,usValue);
	}
}



	
	
#define PCI_EXP_CAP_REG         0x02    /* PCI Express Cap. Reg. Offset */
#define PCI_EXP_CAP_PORT_TYPE   0x00f0  /* Device Port type */
	
void m85xxCapabShow(UINT bus,UINT device,UINT function,UINT8 capOffset)   
{
	UINT8 devCapID, cap_id, cap_id_next;
	UINT16 capReg, type, slot = 0;
	UINT16 usOffset,devCtl = 0;
	
	/* Capabilities List Implemented: Get first capability ID */
	pciConfigInByte(bus, device, function, capOffset, &devCapID);
	devCapID &= ~0x03;
	while (devCapID)
	{	 
		pciConfigInByte(bus, device, function, devCapID, &cap_id);
		pciConfigInByte(bus, device, function, devCapID+1, &cap_id_next);
	
		/* longword align */
		cap_id_next &= ~0x03;
		
		printf("Capabilities id %d \n",cap_id);
		/* Enhancement: Dump specific capabilities regs */
		if (cap_id == 0xff)
			break;
		
		if(PCI_EXT_CAP_EXP == cap_id)
		{
			usOffset = devCapID + 4;
			pciConfigInWord (bus, device, function,usOffset, &capReg);
			printf("device cap offset 0x%x val 0x%x\n",usOffset,capReg);
			type = (capReg & PCI_EXP_CAP_PORT_TYPE) >> 4;
			pciConfigInWord (bus, device, function,(usOffset + 4), &devCtl);
			printf("device control offset 0x%x val 0x%x\n",(usOffset + 4),devCtl);
		}
		devCapID = cap_id_next;
	}
	printf("\n");
}

