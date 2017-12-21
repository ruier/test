/* vxbEtsecEnd.c - Freescale Enhanced TSEC VxBus END driver */

/*
 * Copyright (c) 2007-2009 Wind River Systems, Inc. All rights are reserved.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01x,19aug09,d_c  Add support for VBI 2.0 send/receive/reply protocol
                 for MDIO read/write.
01w,20may09,wap  Avoid use of spinlocks WIND00167144
01v,29jan09,rec  remove guestOs defines
01u,23jan09,to   remove direct MII access for guest OS
01t,20jan09,rec  merge in razor changes
01s,06nov08,dlk  Combined RX/TX handling job.
01r,10oct08,kk   renamed spinlockIsrNdXXX() to spinLockIsrNdXXX()
01q,07aug08,dtr  Add l2disable parameter to default list.
01p,29jul08,wap  Ensure I/O synchronization in PHY access routines
01o,13jun08,pmr  resource documentation
01n,12jun08,wap  Issue graceful reset of RX and TX in etsecReset() routine
01m,18may08,wap  Correct PHY read routine
01l,05may08,tor  update version
01k,17jan07,dlk  Rename spinLockFastLib --> spinlockIsrNdLib.
01j,11dec07,dlk  Use the new fast spin locks.
01i,25sep07,tor  VXB_VERSION_3
01h,20sep07,dlk  In etsecLinkUpdate(), do not call muxError() or muxTxRestart()
                 directly (WIND00100800). Also, support the EIOCGRCVJOBQ ioctl.
01g,29aug07,wap  Make setting of the TBIPA register configurable (WIND00103172)
01f,17aug07,wap  Set ECNTRL.R100 correctly when running in 10Mbps mode
                 (WIND00101782)
01e,06aug07,wap  Correct cut&paste error in etsecEndErrHandle() (WIND00100583)
01d,25jul07,wap  Fix WIND000099316 (typo, rxqueue -> rxQueue)
01c,17jul07,dlk  In etsecEndEncap() TX checksum offload support, fetch
                 pseudo-header checksum for UDP as well as TCP.
01b,16jul07,wap  Add support for TBI mode
01a,21jun07,wap  written
*/

/*
DESCRIPTION
This module implements a driver for the Motorola/Freescale Enhanced
Three Speed Ethernet Controller (ETSEC) network interface. The ETSEC
supports 10, 100 and 1000Mbps operation over copper and fiber media.

The ETSEC is unusual in that it uses three different interrupt vectors:
one for RX events, one for TX events and one for error events. The
intention is to shave some cycles from the interrupt service path
by jumping directly to the proper event handler routine instead of
the driver having to determine the nature of pending events itself.

Note that while this driver is VxBus-compliant, it does not use
vxbDmaBufLib. The reason for this is that vxbDmaBufLib is technically
only required for drivers for DMA-based devices that must be portable
among multiple architectures (e.g. PCI or VMEbus adapters). The ETSEC
is not a standalone device: it only exists as an integrated component
of certain MPC85xx and MPC83xx PowerPC CPUs. It is always big-endian,
it is always cache-coherent (since we always enable the ETSEC's snooping
features), and it never needs bounce buffering or address translation.
Given this, we may as well forgo the use of vxbDmaBufLib entirely,
since using it will do nothing except add a bit of extra overhead to
the packet processing paths.

The ETSEC supports several advanced features not present in the original
TSEC. This includes TCP/IP checksum offload support, hardware VLAN tag
insertion and stripping, hardware packet parsing and filing, and transmit
packet prioritization. The receive filter logic also supports a wider
multicast hash filter (512 bits vs 256 in the original TSEC) and a 16
entry CAM filter. This driver includes support for the checksum features
and the filer, and uses the CAM filter and expanded hash table to provided
improved multicast filtering, but does not support the VLAN tag
insertion/stripping feature or the transmit prioritization.

The VLAN tagging/stripping is not supported because of an incompatibility
between the hardware and our VLAN support code. When VLAN insertion is
enabled for transmit, the hardware always inserts a VLAN tag: if no frame
control block with a valid VLAN control field is present, it uses the value
from the DFVLAN register. This is not the behavior we expect: the decision
whether or not to insert a tag at all should be done on a per-frame basis,
not globally. The ETSEC's design makes it impossible to send untagged
frames as long as the VLINS bit in the transmit control register is set,
which is not what we want. (I am mystified as to why Freescale felt the
need to impliment it this way.)

The transmit packet prioritization is not supported, because it requires
the use of multiple transmit DMA queues. Currently, there is no API
available in the MUX or the TCP/IP stack to enable us to support multiple
TX queues.

For the RX frame parser and filer, the ETSEC supports up to 8 physical
DMA queues, and up to 64 virtual queues. Currently, we support only 8
queues, with the queue ID corresponding to one of the physical DMA rings.
When the filer is enabled, special device-specific ioctls can be used to
program the filer to distribute frames among the queues depending on
the various filtering properties supported by the hardware. A few simple
cases are also provided as examples. Currently, the MUX/END API does not
support multiple input queues, so the driver uses the filer and RX queues
to prioritize the order in which the frames are sent to the stack. The
driver considers queue 0 to have the highest priority and queue 7 the
lowest. When frames arrive, those directed to queue 0 will be processed
first, followed by queue 1, and queue 2, and so on. If the filer is
programmed to reject certain frames, they won't be seen at all.

Since using the filer requires configuring all 8 RX DMA queues, which
consumes a large amount of memory, the filer support is off by default.
It can be enabled using the "filerEnable" configuration parameter, as
shown below.

SMP CONSIDERATIONS
While the ETSEC has three interrupt vectors, it has only one interrupt
status register and one interrupt mask register. Each interrupt service
routine can only mask off specific interrupt events without touching
the others (i.e. the RX ISR can only mask RX events). Doing this requires
clearing bits in the mask register using a read-modify-write operation,
which is not atomic. A similar read-modify-write operation is used in
the task level interrupt handler code to un-mask interrupts as well.
For proper SMP synchronization, these operations must be guarded with
a spinlock.

AMP CONSIDERATIONS
The MPC8641D dual core processor contains 4 ETSEC devices, and can be
run in either SMP or AMP mode. In AMP mode, it may be desireable to
allocate the on-board ETSEC interface among the cores, i.e. assigning
ETSEC0 and ETSEC1 to core0, and ETSEC2 and ETSEC3 to core1. There are two
complications involved in doing this.

The first is that while there are four complete ETSEC controllers, there
is only one functional MDIO port (namely the one associated with ETSEC0).
Reading or writing a PHY register through the MDIO registers is not an
atomic operation, which means simultaneous accesses by both cores will
overlap and likely fail. To guard against this, the tsecMdio driver should
be used in conjuction with the ETSEC driver. This driver provides a
simple inter-core synchronization mechanism which allows both cores to
share access to the MDIO port without contention.

The second complication involves address translation. To use VxWorks in
AMP mode on the 8641D, low memory offset mode must be enabled. In this
mode, the second core applies a bias of 256MB to memory addresses. The
Freescale 8641D eval board is configured with 512MB of RAM: using low
memory offset mode effectively splits the memory into 256MB banks, with
core0 using the first bank and core1 using the second. Using low memory
offset mode allows the second core to think that the second bank of RAM
starting at address 0x10000000 actually starts at address 0x0. This allows
the same VxWorks image linked for core0 to run on core1 unmodified.

This bias only applies to the CPU core, however: it does not apply to
the ETSEC controllers, which still need to be provided with the absolute
physical addresses of descriptors and packet buffers in order to perform
DMA correctly. This means that when running on the second core, the
ETSEC driver must perform a virtual to physical address translation on
all DMA addresses. A <physMask> configuration property is provided to
specify the virtual to physical bias offset. For ETSEC instances allocated
to core0, this property can be left undefined, or explicitly set to 0.
For ETSEC instances allocated to core1, <physMask> should be set to
0x10000000.

Assigning an ETSEC instance to a particular core is done by setting the
<coreNum> configuration property. If <coreNum> is not defined, or
explicitly set to 0 for a particular ETSEC instance, then that instance
will be assigned to core0. If <coreNum> is set to 1, it will be assigned
to core1 instead. Any ETSEC can be assigned to any core (the BSP defaults
to assigning ETSEC0 and ETSEC1 to the first core and ETSEC2 and ETSEC3 to
the second).

BOARD LAYOUT
The ETSEC is directly integrated into the CPU. All configurations are
jumperless.

EXTERNAL INTERFACE
The driver provides a vxBus external interface. The only exported
routine is the etsecRegister() function, which registers the driver
with VxBus. Since the ETSEC is a processor local bus device, each device
instance must be specified in the hwconf.c file in a BSP. The hwconf entry
The hwconf entry must specify the following parameters:
parameters:
 
\is
 
\i <regBase>
Specifies the base address where the controller's CSR registers are mapped
into the host's address space. All register offsets are computed relative
to this address.
 
\i <intr0>
Specifies the interrupt vector for the ETSEC's TX interrupts.
 
\i <intrLevel0>
Specifies the interrupt level for the ETSEC's TX interrupts.

\i <intr1>
Specifies the interrupt vector for the ETSEC's RX interrupts.
 
\i <intrLevel1>
Specifies the interrupt level for the ETSEC's RX interrupts.

\i <intr2>
Specifies the interrupt vector for the ETSEC's error interrupts.
 
\i <intrLevel2>
Specifies the interrupt level for the ETSEC's error interrupts.

\i <tbiAddr>
Specifies the address to use for the internal TBI management interface.
This value will be programmed into the TBIPA register, and controls
the address at which the TBI management registers will be visible when
accessing the MII management port. When the ETSEC is not operating in
TBI mode, this value should be set so that it does not conflict with
the address of a copper PHY, or else the management registers of the
copper PHY will be obscured.

\i <phyAddr>
Specifies the address of the PHY allocated to this ETSEC instance. On
most boards, all PHYs share the same management port, so we must specify
explicitly which PHY on the management bus maps to which ETSEC.

\i <miiIfName>
Specifies the name of the VxBus device driver that manages this ETSEC's
MDIO management port. In the UP and SMP cases, motetsec0 is designated
as the manager of the MDIO port. For AMP, this must be set to the
"tsecMdio" driver: this driver has special support for synchronizing
access to ETSEC0's MDIO registers between cores.

\i <miiIfUnit>
The unit number that goes with <miiIfName>. Together, these describe
a specific VxBus instance (i.e. motesec0, or tsecMdio0).

\i <coreNum>
Specifies the core to which to allocate this particular ETSEC in an AMP
configuration. If this property is omitted or explicitly set to 0, then the
ETSEC instance will be bound to core0. If set to 1, it will be bound to
core1. Any ETSEC may be mapped to any core. In UP and SMP mode, this
property should not be used.

\i <physMask>
Specifies the memory offset to apply when translating virtual addresses
to physical addresses on ETSECs that are bound to the second core in an
AMP configuration. For ETSECs bound to core0, this property should be
ommitted or set to 0. For ETSECs bound to core1, it should be set to
0x10000000. This property should only be used when <coreNum> is set to 1.

\i <l2disable>
Specifies whether L2 cache extraction should be disabled. According to
Freescale, the ETSEC ports on the MPC8641D do not support this feature,
in spite of the fact that the manual says it does. Unfortunately
there's no way to autoprobe this, so we provide a way to disable this
capability on a per-interface basis. If this property is set to 0 or
omitted, L2 cache extraction for received packets is enabled. If set
to any non-zero value, L2 cache extraction is disabled.
\ie

An example hwconf entry is shown below:

\cs
const struct hcfResource tsecVxbEnd0Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x24000) } },
    { "intr0",          HCF_RES_INT,    { (void *)EPIC_TSEC1TX_INT_VEC } },
    { "intr0Level",     HCF_RES_INT,    { (void *)EPIC_TSEC1TX_INT_VEC } },
    { "intr1",          HCF_RES_INT,    { (void *)EPIC_TSEC1RX_INT_VEC } },
    { "intr1Level",     HCF_RES_INT,    { (void *)EPIC_TSEC1RX_INT_VEC } },
    { "intr2",          HCF_RES_INT,    { (void *)EPIC_TSEC1ERR_INT_VEC } },
    { "intr2Level",     HCF_RES_INT,    { (void *)EPIC_TSEC1ERR_INT_VEC } },
    { "phyAddr",        HCF_RES_INT,    { (void *)0 } },
#ifdef INCLUDE_AMP
    { "miiIfName",      HCF_RES_STRING, { (void *)"tsecMdio" } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
    { "coreNum",        HCF_RES_INT,    { (void *)0 } }
#else
    { "miiIfName",      HCF_RES_STRING, { (void *)"motetsec" } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } }
#endif
};
#define tsecVxbEnd0Num NELEMENTS(tsecVxbEnd0Resources)
\ce

The ETSEC controller also supports jumbo frames. This driver has 
jumbo frame support, which is disabled by default in order to conserve 
memory (jumbo frames require the use of an buffer pool with larger clusters).
Jumbo frames can be enabled on a per-interface basis using a parameter
override entry in the hwconf.c file in the BSP. For example, to enable
jumbo frame support for interface motetsec0, the following entry should be
added to the VXB_INST_PARAM_OVERRIDE table:

    { "motetsec", 0, "jumboEnable", VXB_PARAM_INT32, {(void *)1} }

The ETSEC controller also supports interrupt coalescing. This driver
has coalescing support, which is disabled by default so that the
'out of the box' configuration has the smallest interrupt latency.
Coalescing can be anabled on a per-interface basis using parameter
overrides in the hwconf.c file, in the same way as jumbo frame
support. In addition to turning the coalescing support on and
off, the timeout and packet count values can be set:

    { "motetsec", 0, "coalesceEnable", VXB_PARAM_INT32, {(void *)1} }
    { "motetsec", 0, "coalesceRxTicks", VXB_PARAM_INT32, {(void *)10} }
    { "motetsec", 0, "coalesceRxPkts", VXB_PARAM_INT32, {(void *)8} }
    { "motetsec", 0, "coalesceTxTicks", VXB_PARAM_INT32, {(void *)100} }
    { "motetsec", 0, "coalesceTxPkts", VXB_PARAM_INT32, {(void *)16} }

If only the <coalesceEnable> property is set, the driver will use
default timeout and packet count values as shown above. Specifying
alternate values via the BSP will override the defaults.

To enable support for the filer and multiple RX queues, the <filerEnable>
property should also be set, as illustrated below:

    { "motetsec", 0, "filerEnable", VXB_PARAM_INT32, {(void *)1} }

Note that the filer support can not be enabled or disabled on the fly
at runtime.

SEE ALSO: vxBus, ifLib, miiBus
\tb "Writing an Enhanced Network Driver"
\tb "MPC8548E PowerQUICC III Integrated Communications Processor Reference Manual, http://www.freescale.com/files/32bit/doc/ref_manual/MPC8548ERM.pdf"
*/


#include <vxWorks.h>
#include <vxAtomicLib.h>
#include <logLib.h>
#include <muxLib.h>
#include <netLib.h>
#include <netBufLib.h>
#include <semLib.h>
#include <sysLib.h>
#include <vxBusLib.h>
#include <wdLib.h>
#include <etherMultiLib.h>
#include <end.h>
#define END_MACROS
#include <endLib.h>
#include <cacheLib.h>

#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#include <hwif/vxbus/vxBus.h>

#ifdef _WRS_CONFIG_WRHV_GUEST
#include <wrhv/vbi.h>
#endif

#include "common_lib_def/vxbEtsecEnd.h"

#if defined(VPX6_HSP9_1_PROJECT)
#include <hwif/h/mii/miiBus.h>
#include <hwif/h/vxbus/vxbAccess.h>
#include <hwif/h/hEnd/hEnd.h>
#include <hwif/h/mii/genericTbiPhy.h>


#else

#include <../src/hwif/h/mii/miiBus.h>
#include <../src/hwif/h/vxbus/vxbAccess.h>
#include <../src/hwif/h/hEnd/hEnd.h>
#include <../src/hwif/h/mii/genericTbiPhy.h>



#endif


/*IMPORT void vxbDelay (UINT32);*/

IMPORT STATUS sysNetMacNVRamAddrGet (char *, int, UINT8 *, int);
METHOD_DECL(vxbIntCtlrCpuCheck);

IMPORT FUNCPTR _func_m2PollStatsIfPoll;

/* VxBus methods */

LOCAL void	etsecInstInit (VXB_DEVICE_ID);
LOCAL void	etsecInstInit2 (VXB_DEVICE_ID);
LOCAL void	etsecInstConnect (VXB_DEVICE_ID);
LOCAL BOOL	etsecProbe (VXB_DEVICE_ID);
LOCAL STATUS	etsecInstUnlink (VXB_DEVICE_ID, void *);

/* miiBus methods */

LOCAL STATUS	etsecPhyRead (VXB_DEVICE_ID, UINT8, UINT8, UINT16 *);
LOCAL STATUS    etsecPhyWrite (VXB_DEVICE_ID, UINT8, UINT8, UINT16);
LOCAL STATUS    etsecLinkUpdate (VXB_DEVICE_ID);

#ifdef _WRS_CONFIG_WRHV_GUEST

#if (VBI_VERSION_MAJOR == 2)

LOCAL STATUS    hvBspServerHandleGet (VBI_NS_HANDLE *);

#endif /* (VBI_VERSION_MAJOR == 2) */

LOCAL STATUS    virtualBoardMdioRead (UINT8, UINT8, UINT16 *);
LOCAL STATUS    virtualBoardMdioWrite (UINT8, UINT8, UINT16);

#undef GUEST_DEBUG
#ifdef GUEST_DEBUG
#define GUEST_DEBUG_PRINTF(fmt,...) \
        {char logBuf[82]; sprintf (logBuf,fmt,__VA_ARGS__); vbiKputs(logBuf);}
#else
#define GUEST_DEBUG_PRINTF(fmt,...)
#endif /* GUEST_DEBUG */

#endif /* _WRS_CONFIG_WRHV_GUEST */

/* mux methods */

LOCAL void	etsecMuxConnect (VXB_DEVICE_ID, void *);

LOCAL struct drvBusFuncs etsecFuncs =
    {
    etsecInstInit,	/* devInstanceInit */
    etsecInstInit2,	/* devInstanceInit2 */
    etsecInstConnect	/* devConnect */
    };

LOCAL struct vxbDeviceMethod etsecMethods[] =
   {
   DEVMETHOD(miiRead,		etsecPhyRead),
   DEVMETHOD(miiWrite,		etsecPhyWrite),
   DEVMETHOD(miiMediaUpdate,	etsecLinkUpdate),
   DEVMETHOD(muxDevConnect,	etsecMuxConnect),
   DEVMETHOD(vxbDrvUnlink,	etsecInstUnlink),
   { 0, 0 }
   };   

/* default queue parameters */
LOCAL HEND_RX_QUEUE_PARAM etsecRxQueueDefault = {
    NULL,                       /* jobQueId */
    0,                          /* priority */
    0,                          /* rbdNum */
    0,                          /* rbdTupleRatio */
    0,                          /* rxBufSize */
    NULL,                       /* pBufMemBase */
    0,                          /* rxBufMemSize */
    0,                          /* rxBufMemAttributes */
    NULL,                       /* rxBufMemFreeMethod */
    NULL,                       /* pRxBdBase */
    0,                          /* rxBdMemSize */
    0,                          /* rxBdMemAttributes */
    NULL                        /* rxBdMemFreeMethod */
};

LOCAL HEND_TX_QUEUE_PARAM etsecTxQueueDefault = {
    NULL,                       /* jobQueId */
    0,                          /* priority */
    0,                          /* tbdNum */
    0,                          /* allowedFrags */
    NULL,                       /* pTxBdBase */
    0,                          /* txBdMemSize */
    0,                          /* txBdMemAttributes */
    NULL                        /* txBdMemFreeMethod */
};

LOCAL VXB_PARAMETERS etsecParamDefaults[] =
    {
        {"rxQueue00", VXB_PARAM_POINTER, {(void *)&etsecRxQueueDefault}},
        {"txQueue00", VXB_PARAM_POINTER, {(void *)&etsecTxQueueDefault}},
        {"jumboEnable", VXB_PARAM_INT32, {(void *)0}},
        {"coalesceEnable", VXB_PARAM_INT32, {(void *)0}},
        {"coalesceRxTicks", VXB_PARAM_INT32, {(void *)10}},
        {"coalesceRxPkts", VXB_PARAM_INT32, {(void *)8}},
        {"coalesceTxTicks", VXB_PARAM_INT32, {(void *)100}},
        {"coalesceTxPkts", VXB_PARAM_INT32, {(void *)16}},
        {"filerEnable", VXB_PARAM_INT32, {(void *)0}},
        {"l2disable", VXB_PARAM_INT32, {(void *)0}},
        {NULL, VXB_PARAM_END_OF_LIST, {NULL}}
    };

LOCAL struct vxbPlbRegister etsecDevPlbRegistration =
    {
        {
        NULL,			/* pNext */
        VXB_DEVID_DEVICE,	/* devID */
        VXB_BUSID_PLB,		/* busID = PCI */
        VXB_VER_4_0_0,		/* vxbVersion */
        ETSEC_NAME,		/* drvName */
        &etsecFuncs,		/* pDrvBusFuncs */
        etsecMethods,		/* pMethods */
        etsecProbe,		/* devProbe */
        etsecParamDefaults	/* pParamDefaults */
        },
    };

/* Driver utility functions */

LOCAL STATUS    etsecReset (VXB_DEVICE_ID);
LOCAL STATUS	etsecFilerTableWrite (VXB_DEVICE_ID, ETSEC_FILER_ENTRY *);
LOCAL STATUS	etsecFilerTableRead (VXB_DEVICE_ID, ETSEC_FILER_ENTRY *);
LOCAL STATUS	etsecFilerInit (VXB_DEVICE_ID);
LOCAL STATUS	etsecFilerLoad (VXB_DEVICE_ID);

/* END functions */

LOCAL END_OBJ *	etsecEndLoad (char *, void *);
LOCAL STATUS	etsecEndUnload (END_OBJ *);
LOCAL int	etsecEndIoctl (END_OBJ *, int, caddr_t);
LOCAL STATUS	etsecEndMCastAddrAdd (END_OBJ *, char *);
LOCAL STATUS	etsecEndMCastAddrDel (END_OBJ *, char *);
LOCAL STATUS	etsecEndMCastAddrGet (END_OBJ *, MULTI_TABLE *);
LOCAL void	etsecEndHashTblPopulate (ETSEC_DRV_CTRL *);
LOCAL STATUS	etsecEndStatsDump (ETSEC_DRV_CTRL *);
LOCAL void	etsecEndRxConfig (ETSEC_DRV_CTRL *);
LOCAL STATUS	etsecEndStart (END_OBJ *);
LOCAL STATUS	etsecEndStop (END_OBJ *);
LOCAL int	etsecEndSend (END_OBJ *, M_BLK_ID);
LOCAL int	etsecEndEncap (ETSEC_DRV_CTRL *, M_BLK_ID);
LOCAL STATUS	etsecEndPollSend (END_OBJ *, M_BLK_ID);
LOCAL int	etsecEndPollReceive (END_OBJ *, M_BLK_ID);
LOCAL void	etsecEndInt (ETSEC_DRV_CTRL *);
LOCAL void	etsecEndIntHandle (void *);
LOCAL void	etsecEndRxHandle (ETSEC_DRV_CTRL *);
LOCAL void	etsecEndRxDemux (ETSEC_DRV_CTRL *);
LOCAL void	etsecEndTxHandle (ETSEC_DRV_CTRL *);
LOCAL void	etsecEndErrHandle (void *, UINT32);

LOCAL NET_FUNCS etsecNetFuncs =
    {
    etsecEndStart,                      /* start func. */
    etsecEndStop,                       /* stop func. */
    etsecEndUnload,                     /* unload func. */
    etsecEndIoctl,                      /* ioctl func. */
    etsecEndSend,                       /* send func. */
    etsecEndMCastAddrAdd,               /* multicast add func. */
    etsecEndMCastAddrDel,               /* multicast delete func. */
    etsecEndMCastAddrGet,               /* multicast get fun. */
    etsecEndPollSend,                   /* polling send func. */
    etsecEndPollReceive,                /* polling receive func. */
    endEtherAddressForm,                /* put address info into a NET_BUFFER */
    endEtherPacketDataGet,              /* get pointer to data in NET_BUFFER */
    endEtherPacketAddrGet               /* Get packet addresses */
    };

/*****************************************************************************
*
* etsecRegister - register with the VxBus subsystem
*
* This routine registers the ETSEC driver with VxBus as a
* child of the PLB bus type.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void etsecRegister(void)
    {
    vxbDevRegister ((struct vxbDevRegInfo *)&etsecDevPlbRegistration);
    return;
    }

/*****************************************************************************
*
* etsecInstInit - VxBus instInit handler
*
* This function implements the VxBus instInit handler for an etsec
* device instance. The only thing done here is to select a unit
* number for the device.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void etsecInstInit
    (
    VXB_DEVICE_ID pDev
    )
    {
    struct hcfDevice *pHcf;

    /* Always use the unit number allocated to us in the hwconf file. */

    pHcf = (struct hcfDevice *)pDev->pBusSpecificDevInfo;
    vxbInstUnitSet (pDev, pHcf->devUnit);

    return;
    }

/*****************************************************************************
*
* etsecInstInit2 - VxBus instInit2 handler
*
* This function implements the VxBus instInit2 handler for an etsec
* device instance. Once we reach this stage of initialization, it's
* safe for us to allocate memory, so we can create our pDrvCtrl
* structure and do some initial hardware setup. The important
* steps we do here are to create a child miiBus instance, connect
* our ISR to our assigned interrupt vector, read the station
* address from the EEPROM, and set up our vxbDma tags and memory
* regions. We need to allocate a 64K region for the RX DMA window
* here.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void etsecInstInit2
    (
    VXB_DEVICE_ID pDev
    )
    {
    ETSEC_DRV_CTRL * pDrvCtrl;
    struct hcfDevice * pHcf;
    VXB_INST_PARAM_VALUE val;
    STATUS r;
    int i;

    pDrvCtrl = malloc (sizeof(ETSEC_DRV_CTRL));
    bzero ((char *)pDrvCtrl, sizeof(ETSEC_DRV_CTRL));
    pDev->pDrvCtrl = pDrvCtrl;
    pDrvCtrl->etsecDev = pDev;

    /*
     * If we're running on a core other than core 0 in an AMP configuration,
     * we may need to perform some minor virtual to physical address
     * translation.
     */

    pHcf = (struct hcfDevice *)pDev->pBusSpecificDevInfo;

#ifndef _WRS_CONFIG_WRHV_GUEST
    /*
     * resourceDesc {
     * The physMask resource specifies the memory offset to 
     * apply when translating virtual addresses to physical 
     * addresses on ETSECs that are bound to the second core
     * in an AMP configuration. For ETSECs bound to core0, 
     * this property should be ommitted or set to 0. For 
     * ETSECs bound to core1, it should be set to 0x10000000. 
     * This property should only be used when <coreNum> is 
     * set to 1. }
     */ 
    devResourceGet (pHcf, "physMask", HCF_RES_INT,
		    (void *)&pDrvCtrl->etsecMask); 

#else /* _WRS_CONFIG_WRHV_GUEST */
    {
    FUNCPTR physAddrGet;
    if(devResourceGet (pHcf, "physMask", HCF_RES_ADDR,
		       (void *)&physAddrGet) == OK)
	{
	pDrvCtrl->etsecMask = physAddrGet();
	}
    }
#endif /* _WRS_CONFIG_WRHV_GUEST */

    
    /* Retrieve the station address */

    sysNetMacNVRamAddrGet (pDev->pName, pDev->unitNumber,
        pDrvCtrl->etsecAddr, ETHER_ADDR_LEN);

    /* Reset the device */

    etsecReset (pDev);

    pDrvCtrl->etsecDevSem = semMCreate (SEM_Q_PRIORITY|
        SEM_DELETE_SAFE|SEM_INVERSION_SAFE);

    /*
     * Configure the MII interface clock and set the address
     * for the TBI management interface. When using TBI mode,
     * we can make the management registers appear at any
     * arbitrary address on the MII bus. We need to make sure
     * we set it for a value that won't collide with a real
     * PHY, or else it'll obscure it.
     */

#ifndef _WRS_CONFIG_WRHV_GUEST
    CSR_WRITE_4(pDev, ETSEC_MIIMCFG, ETSEC_MIIMCFG_MCS_28);
#endif /* _WRS_CONFIG_WRHV_GUEST */

    CSR_CLRBIT_4(pDev, ETSEC_IMASK, ETSEC_IEVENT_MMRD|ETSEC_IEVENT_MMWR);
    /*
     * resourceDesc {
     * The tbiddr resource specifies the MII address of
     * the TBI interface associated with this instance. }
     */
    if (devResourceGet (pHcf, "tbiAddr", HCF_RES_INT,
        (void *)&pDrvCtrl->etsecTbiAddr) != OK)
        pDrvCtrl->etsecTbiAddr = ETSEC_TBI_ADDR;

    CSR_WRITE_4(pDev, ETSEC_TBIPA, pDrvCtrl->etsecTbiAddr);

    /*
     * paramDesc {
     * The filerEnable resource specifies whether
     * the frame filer and multiple RX queues should be
     * enabled. }
     */
    r = vxbInstParamByNameGet (pDev, "filerEnable", VXB_PARAM_INT32, &val);

    if (r == OK && val.int32Val)
        pDrvCtrl->etsecFiler = TRUE;
    else
        pDrvCtrl->etsecFiler = FALSE;

    /*
     * Allocate properly aligned DMA descriptor rings.
     * Note: to work around erratum eTSEC 2, RX DMA rings
     * should be aligned on 64 byte boundaries.
     */

    pDrvCtrl->etsecRxQueue[0] = memalign(_CACHE_ALIGN_SIZE * 2,
        sizeof(ETSEC_DESC) * ETSEC_RX_DESC_CNT);

    /* If the filer is enabled, allocate descriptors for the other 7 rings */

    if (pDrvCtrl->etsecFiler == TRUE)
        {
        for (i = 1; i < ETSEC_MAX_RX_QUEUES; i++)
            pDrvCtrl->etsecRxQueue[i] = memalign(_CACHE_ALIGN_SIZE * 2,
                sizeof(ETSEC_DESC) * ETSEC_RX_DESC_CNT);
        }

    pDrvCtrl->etsecTxDesc = memalign(_CACHE_ALIGN_SIZE * 2,
        sizeof(ETSEC_DESC) * ETSEC_TX_DESC_CNT);

    /*
     * Allocate frame control blocks for TX use.
     * Note: to workaround erratum eTSEC 23, FCBs must
     * always be at least 16 byte aligned.
     */

    for (i = 0; i < ETSEC_TX_DESC_CNT; i++)
        pDrvCtrl->etsecTxFcb[i] = memalign(_CACHE_ALIGN_SIZE * 2,
            sizeof(ETSEC_TX_FCB));

    return;
    }

/*****************************************************************************
*
* etsecInstConnect -  VxBus instConnect handler
*
* This function implements the VxBus instConnect handler for an etsec
* device instance.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void etsecInstConnect
    (
    VXB_DEVICE_ID pDev
    )
    {
    return;
    }

/*****************************************************************************
*
* etsecProbe - VxBus device probe routine
*
* This function implements a VxBus probe method. A probe method is not
* normally needed for processor local bus devices (they can't really be
* autoprobed), but we use one here to allow ETSEC devices to be assigned
* to different cores in an AMP configuration. If a "coreNum" property is
* specified in the hwconf entry for a particular ETSEC instance, that
* property will be used to ensure that the instance only probes successfully
* on its assigned core.
*
* RETURNS: FALSE if a core check method exists, and if a coreNum property
*          exists, and if the specified coreNum doesn't match the current
*          core, otherwise TRUE
*
* ERRNO: N/A
*/

LOCAL BOOL etsecProbe
    (
    VXB_DEVICE_ID pDev
    )
    {
    FUNCPTR coreCheck;
    struct hcfDevice * pHcf;
    int coreNum, core;

    coreCheck = vxbDevMethodGet (vxbDevParent (pDev),
        DEVMETHOD_CALL(vxbIntCtlrCpuCheck));

    /* No core check method exists, return success. */

    if (coreCheck == NULL)
        return (TRUE);

    pHcf = (struct hcfDevice *)pDev->pBusSpecificDevInfo;
    
    /* If no coreNum assigned for this instance, return success. */

    /*
     * resourceDesc {
     * The coreNum resource specifies the core to which
     * to allocate this particular ETSEC in an AMP 
     * configuration. If this property is omitted or
     * explicitly set to 0, then the ETSEC instance will
     * be bound to core0. If set to 1, it will be bound
     * to core1. Any ETSEC may be mapped to any core. In
     * UP and SMP mode, this property should not be used. }
     */ 
    if (devResourceGet (pHcf, "coreNum", HCF_RES_INT, (void *)&coreNum) != OK)
        return (TRUE);

    core = coreCheck ();

    /* We're not assigned to this core, return failure. */

    if (core != coreNum)
        return (FALSE);

    /* We're explicitly assigned to this core, return success. */

    return (TRUE);
    }

/*****************************************************************************
*
* etsecInstUnlink -  VxBus unlink handler
*
* This function shuts down an etsec device instance in response to an
* unlink event from VxBus. This may occur if our VxBus instance has
* been terminated, or if the etsec driver has been unloaded. When an
* unlink event occurs, we must shut down and unload the END interface
* associated with this device instance and then release all the
* resources allocated during instance creation, such as vxbDma
* memory and maps, and interrupt handles. We also must destroy our
* child miiBus and PHY devices.
*
* RETURNS: OK if device was successfully destroyed, otherwise ERROR
*
* ERRNO: N/A
*/

LOCAL STATUS etsecInstUnlink
    (
    VXB_DEVICE_ID pDev,
    void * unused
    )
    { 
    ETSEC_DRV_CTRL * pDrvCtrl;
    int i;

    pDrvCtrl = pDev->pDrvCtrl;

    /*
     * Stop the device and detach from the MUX.
     * Note: it's possible someone might try to delete
     * us after our vxBus instantiation has completed,
     * but before anyone has called our muxConnect method.
     * In this case, there'll be no MUX connection to
     * tear down, so we can skip this step.
     */

    if (pDrvCtrl->etsecMuxDevCookie != NULL)
        {
        if (muxDevStop (pDrvCtrl->etsecMuxDevCookie) != OK)
            return (ERROR);

        /* Detach from the MUX. */

        if (muxDevUnload (ETSEC_NAME, pDev->unitNumber) != OK)
            return (ERROR);
        }

    /* Free descriptor ring memory. */

    for (i = 0; i < ETSEC_MAX_RX_QUEUES; i++)
        {
        if (pDrvCtrl->etsecRxQueue[i] != NULL)
            free (pDrvCtrl->etsecRxQueue[i]);
        pDrvCtrl->etsecRxQueue[i] = NULL;
        }

    free (pDrvCtrl->etsecTxDesc);
    for (i = 0; i < ETSEC_TX_DESC_CNT; i++)
        free (pDrvCtrl->etsecTxFcb[i]);

    /* Disconnect the ISR. */

    vxbIntDisconnect (pDev, 0, etsecEndInt, pDrvCtrl);
    vxbIntDisconnect (pDev, 1, etsecEndInt, pDrvCtrl);
    vxbIntDisconnect (pDev, 2, etsecEndInt, pDrvCtrl);

    /* Destroy the adapter context. */
    free (pDrvCtrl);
    pDev->pDrvCtrl = NULL;

    /* Goodbye cruel world. */

    return (OK);
    }

/*****************************************************************************
*
* etsecPhyRead - miiBus miiRead method
*
* This function implements an miiRead() method that allows PHYs
* on the miiBus to access our MII management registers.
*
* RETURNS: ERROR if invalid PHY addr or register is specified, else OK
*
* ERRNO: N/A
*/

LOCAL STATUS etsecPhyRead
    (
    VXB_DEVICE_ID pDev,
    UINT8 phyAddr,
    UINT8 regAddr,
    UINT16 *pDataVal
    )
    {
    ETSEC_DRV_CTRL * pDrvCtrl;
    STATUS rval = ERROR;
    FUNCPTR miiRead;
    int i;

    pDrvCtrl = pDev->pDrvCtrl;

    if (CSR_READ_4(pDev, ETSEC_ECNTRL) & ETSEC_ECNTRL_TBIM)
        {
        if (regAddr == MII_PHY_ID1_REG)
            {
            *pDataVal = TBI_ID1;
            return (OK);
            }
        if (regAddr == MII_PHY_ID2_REG)
            {
            *pDataVal = TBI_ID2;
            return (OK);
            }
        }
    else
        {
        /* Avoid tripping over the TBI management port */

        if (phyAddr == pDrvCtrl->etsecTbiAddr)
            {
            *pDataVal = 0xFFFF;
            return (ERROR);
            }
        }

    if (phyAddr != pDrvCtrl->etsecMiiPhyAddr && phyAddr < 0x20)
        {
        *pDataVal = 0xFFFF;
        return (ERROR);
        }

    if (phyAddr > 0x1F)
        phyAddr -= 32;

    /*
     * If we're not the management device (ETSEC0), then
     * forward the read request.
     */

    if (pDrvCtrl->etsecMiiDev != pDev && pDrvCtrl->etsecMiiPhyRead != NULL)
        {
        miiRead = pDrvCtrl->etsecMiiPhyRead;
        phyAddr += 32;
        return (miiRead (pDrvCtrl->etsecMiiDev, phyAddr, regAddr, pDataVal));
        }

    semTake (pDrvCtrl->etsecDevSem, WAIT_FOREVER);

#if defined(VPX6_HSP9_1_PROJECT)
    
	if(pDev->unitNumber < 4)
	{
    switch (regAddr)
    {
        case 0x0:
			*pDataVal = 0x1000;
			break;
		case 0x1:
			*pDataVal = 0x796d;
			break;
		case 0x2:
			*pDataVal = 0x141;
			break;
		case 0x3:
			*pDataVal = 0xcd4;
			break;
		case 0x4:
			*pDataVal = 0xde1;
			break;
		case 0x5:
			*pDataVal = 0x0081;
			break;
		case 0x6:
			*pDataVal = 0x6;
			break;
        case 0x7:
			*pDataVal = 0x2801;
			break;
        case 0x8:
			*pDataVal = 0x4876;
			break;
		case 0x9:
			*pDataVal = 0x0300;
			break;
		case 0xa:
			*pDataVal = 0x7c00;
			break;
		case 0x10:
		    *pDataVal = 0x78;
			break;
		case 0x11:
		    *pDataVal = 0xac4c;/*1000M:0xac4c 100M: 0x4d00*/
			break;	
		case 0x14:
		    *pDataVal = 0xce2;
			break;
		case 0x1b:
		    *pDataVal = 0x848b;
			break; 
		default:
			*pDataVal = 0xffff;
			break;
    	}
		rval = OK;
    }
    else
    {
#endif


    #ifdef _WRS_CONFIG_WRHV_GUEST
    
    rval = virtualBoardMdioRead (phyAddr, regAddr, pDataVal);
    
    #else /* _WRS_CONFIG_WRHV_GUEST */
    
        CSR_WRITE_4(pDev, ETSEC_MIIMADD, regAddr | (phyAddr << 8));
        CSR_WRITE_4(pDev, ETSEC_MIIMCOM, ETSEC_MIIMCOM_READ);
    
        /*
         * Perform an isync here to make sure the writes above occur
         * before the reads below. Normally each CSR_WRITE_4() performs
         * an eieio, but on some processors, if the MMU is disabled.
         * eieio is a no-op. There are cases where we run with the
         * MMU off, but the instruction cache on (such as in the
         * bootrom), so we need the additional isync instruction to
         * ensure synchronization.
         */
    
        WRS_ASM("isync");
    
        for (i = 0; i < ETSEC_TIMEOUT; i++)
            {
            if ((CSR_READ_4(pDev, ETSEC_MIIMIND) &
                (ETSEC_MIIMIND_BUSY|ETSEC_MIIMIND_NOT_VALID)) == 0)
                break;
            }
    
        if (i == ETSEC_TIMEOUT)
            *pDataVal = 0xFFFF;
        else
            {
            *pDataVal = CSR_READ_4(pDev, ETSEC_MIIMSTAT) & 0xFFFF;
            rval = OK;
            }
    
        CSR_WRITE_4(pDev, ETSEC_MIIMCOM, 0);
    
    #endif /* _WRS_CONFIG_WRHV_GUEST */

#if defined(VPX6_HSP9_1_PROJECT)
    }
#endif

    semGive (pDrvCtrl->etsecDevSem);
 
    return (rval);
    }

/*****************************************************************************
*
* etsecPhyWrite - miiBus miiRead method
*
* This function implements an miiWrite() method that allows PHYs
* on the miiBus to access our MII management registers. This routine
* works in much the same way as etsecPhyRead(), using the shortcut
* PHY management registers to make it look like there's a single
* PHY at MII address 0.
*
* RETURNS: ERROR if invalid PHY addr or register is specified, else OK
*
* ERRNO: N/A
*/

LOCAL STATUS etsecPhyWrite
    (
    VXB_DEVICE_ID pDev,
    UINT8 phyAddr,
    UINT8 regAddr,
    UINT16 dataVal
    )
    {
    ETSEC_DRV_CTRL * pDrvCtrl;
    FUNCPTR miiWrite;
    STATUS rval;
    int i;

#if defined(VPX6_HSP9_1_PROJECT)
    return OK;
#endif

    pDrvCtrl = pDev->pDrvCtrl;

    if ((CSR_READ_4(pDev, ETSEC_ECNTRL) & ETSEC_ECNTRL_TBIM) == 0 &&
        phyAddr == pDrvCtrl->etsecTbiAddr)
        return (ERROR);

    if (phyAddr != pDrvCtrl->etsecMiiPhyAddr && phyAddr < 0x20)
        return (ERROR);

    if (phyAddr > 0x1F)
        phyAddr -= 32;

    /*
     * If we're not the management device (ETSEC0), then
     * forward the write request.
     */

    if (pDrvCtrl->etsecMiiDev != pDev && pDrvCtrl->etsecMiiPhyRead != NULL)
        {
        miiWrite = pDrvCtrl->etsecMiiPhyWrite;
        phyAddr += 32;
        return (miiWrite (pDrvCtrl->etsecMiiDev, phyAddr, regAddr, dataVal));
        }

    semTake (pDrvCtrl->etsecDevSem, WAIT_FOREVER);

#ifdef _WRS_CONFIG_WRHV_GUEST
    
    rval = virtualBoardMdioWrite (phyAddr, regAddr, dataVal);
    
#else /* _WRS_CONFIG_WRHV_GUEST */
    CSR_WRITE_4(pDev, ETSEC_MIIMADD, regAddr | (phyAddr << 8));
    CSR_WRITE_4(pDev, ETSEC_MIIMCON, dataVal);

    /*
     * Perform an isync here to make sure the writes above occur
     * before the reads below. Normally each CSR_WRITE_4() performs
     * an eieio, but on some processors, if the MMU is disabled.
     * eieio is a no-op. There are cases where we run with the
     * MMU off, but the instruction cache on (such as in the
     * bootrom), so we need the additional isync instruction to
     * ensure synchronization.
     */

    WRS_ASM("isync");

    for (i = 0; i < ETSEC_TIMEOUT; i++)
        {
        if (!(CSR_READ_4(pDev, ETSEC_MIIMIND) & ETSEC_MIIMIND_BUSY))
            break;
        }

    if (i == ETSEC_TIMEOUT)
        rval = ERROR;
    else
        rval = OK;
#endif /* _WRS_CONFIG_WRHV_GUEST */

    semGive (pDrvCtrl->etsecDevSem);

    return (rval);
    }

#ifdef _WRS_CONFIG_WRHV_GUEST

#if (VBI_VERSION_MAJOR == 2)

/*****************************************************************************
*
* hvBspServerHandleGet - Get the hypervisor BSP server handle
*
* This function performs a Hypervisor name service lookup to find the
* Hypervisor BSP server handle. The handle is cached in a static
* variable.
*
* RETURNS: ERROR if name service lookup fails, else OK
*
* ERRNO: N/A
*/

LOCAL STATUS hvBspServerHandleGet
    (
    VBI_NS_HANDLE *pHvBspServerHandle /* Returned HV name service handle */
    )
    {
    static VBI_NS_HANDLE hvBspServerHandle;
    static BOOL hvBspServerHandleInitialized = FALSE;
    STATUS retStatus = OK;

    if (!hvBspServerHandleInitialized)
	{
	retStatus = (STATUS) vbiNsLookup ("bspServer", 0, &hvBspServerHandle);
	if (retStatus == OK)
	    {
	    hvBspServerHandleInitialized = TRUE;
	    GUEST_DEBUG_PRINTF("vxbEtsecEnd.c/hvBspServerHandleGet: %d\n",
			       hvBspServerHandle);
	    }
	}
    
	*pHvBspServerHandle = hvBspServerHandle;
	return retStatus;
    }

#endif /* (VBI_VERSION_MAJOR == 2) */

/*****************************************************************************
*
* virtualBoardMdioRead - Perform MDIO read for a virtual board
*
* This function performs an MDIO read for a virtual board, using
* a Hypervisor service call.
*
* RETURNS: ERROR if MDIO read fails, else OK
*
* ERRNO: N/A
*/

LOCAL STATUS virtualBoardMdioRead
    (
    UINT8 phyAddr,
    UINT8 regAddr,
    UINT16 *pDataVal
    )
    {
#if (VBI_VERSION_MAJOR == 2)
    VBI_BSP_MSG requestMsg;
    VBI_BSP_MSG_REPLY replyMsg;
    VBI_NS_HANDLE hvBspServerHandle;
    
    if (hvBspServerHandleGet (&hvBspServerHandle) != OK)
	{
	*pDataVal = 0xffff;
	return ERROR;
	}
    
    requestMsg.request = VBI_MDIO_READ;
    requestMsg.arg.mdioRead.bus = 0;
    requestMsg.arg.mdioRead.phyAddr = (uint32_t) phyAddr;
    requestMsg.arg.mdioRead.regNum = (uint32_t) regAddr;
    requestMsg.arg.mdioRead.page = 0;
    
    if (vbiSend (hvBspServerHandle, &requestMsg, sizeof(requestMsg),
		 &replyMsg, sizeof(replyMsg), NULL, NULL) != OK)
	{
	*pDataVal = 0xffff;
	return ERROR;
	}

    if ((replyMsg.dataVal >> 16 != 0) || (replyMsg.status != OK))
	{	
	*pDataVal = 0xffff;
	return ERROR;
	}

    GUEST_DEBUG_PRINTF("vxbEtsecEnd.c/virtualBoardMdioRead: "
		       "phyAddr:0x%x  regAddr:0x%x  val:0x%x\n",
		       phyAddr, regAddr, replyMsg.dataVal);

    *pDataVal = (UINT16) replyMsg.dataVal;
    return OK;
    
#elif (VBI_VERSION_MAJOR == 1)
    int value;

    value = vbiBspIoctl (VBI_BSPIOCTL_DRV_MDIO, MDIO_READ, 0, phyAddr,
			 regAddr, 0, 0, 0);

    if (value >> 16 != 0)
	{	
	*pDataVal = 0xffff;
	return ERROR;
	}

    GUEST_DEBUG_PRINTF("vxbEtsecEnd.c/virtualBoardMdioRead: "
		       "phyAddr:0x%x  regAddr:0x%x  val:0x%x\n",
		       phyAddr, regAddr, value);
    
    *pDataVal = (UINT16) value;
    return OK;

#endif /* (VBI_VERSION_MAJOR == 1) */
    }

/*****************************************************************************
*
* virtualBoardMdioWrite - Perform MDIO write for a virtual board
*
* This function performs an MDIO write for a virtual board, using
* a Hypervisor service call.
*
* RETURNS: ERROR if MDIO write fails, else OK
*
* ERRNO: N/A
*/

LOCAL STATUS virtualBoardMdioWrite
    (
    UINT8 phyAddr,
    UINT8 regAddr,
    UINT16 dataVal
    )
    {
#if (VBI_VERSION_MAJOR == 2)
    VBI_BSP_MSG requestMsg;
    VBI_BSP_MSG_REPLY replyMsg;
    VBI_NS_HANDLE hvBspServerHandle;
    
    if (hvBspServerHandleGet (&hvBspServerHandle) != OK)
	{
	return ERROR;
	}
    
    requestMsg.request = VBI_MDIO_WRITE;
    requestMsg.arg.mdioWrite.bus = 0;
    requestMsg.arg.mdioWrite.phyAddr = (uint32_t) phyAddr;
    requestMsg.arg.mdioWrite.regNum = (uint32_t) regAddr;
    requestMsg.arg.mdioWrite.page = 0;
    requestMsg.arg.mdioWrite.dataVal = (uint32_t) dataVal;
    
    if (vbiSend (hvBspServerHandle, &requestMsg, sizeof(requestMsg),
		 &replyMsg, sizeof(replyMsg), NULL, NULL) != OK)
	{
	return ERROR;
	}

    if (replyMsg.status != OK)
	{	
	return ERROR;
	}

    GUEST_DEBUG_PRINTF("vxbEtsecEnd.c/virtualBoardMdioWrite: "
		       "phyAddr:0x%x  regAddr:0x%x  val:0x%x\n",
		       phyAddr, regAddr, dataVal);

    return OK;
    
#elif (VBI_VERSION_MAJOR == 1)
    
    STATUS status = vbiBspIoctl(VBI_BSPIOCTL_DRV_MDIO, MDIO_WRITE, 0,
				phyAddr, regAddr, 0, (UINT32)dataVal, 0);
    
    if (status == OK)
	{
	GUEST_DEBUG_PRINTF("vxbEtsecEnd.c/virtualBoardMdioWrite: "
			   "phyAddr:0x%x  regAddr:0x%x  val:0x%x\n",
			   phyAddr, regAddr, dataVal);
	}
    
    return status;
    
#endif /* (VBI_VERSION_MAJOR == 1) */
    }

#endif /* _WRS_CONFIG_WRHV_GUEST */

/*****************************************************************************
*
* etsecLinkUpdate - miiBus miiLinkUpdate method
*
* This function implements an miiLinkUpdate() method that allows
* miiBus to notify us of link state changes. This routine will be
* invoked by the miiMonitor task when it detects a change in link
* status. Normally, the miiMonitor task checks for link events every
* two seconds.
*
* Once we determine the new link state, we will announce the change
* to any bound protocols via muxError(). We also update the ifSpeed
* fields in the MIB2 structures so that SNMP queries can detect the
* correct link speed.
*
* RETURNS: ERROR if obtaining the new media setting fails, else OK
*
* ERRNO: N/A
*/

LOCAL STATUS etsecLinkUpdate
    (
    VXB_DEVICE_ID pDev
    )
    {
    ETSEC_DRV_CTRL * pDrvCtrl;
    UINT32 oldStatus;

    if (pDev->pDrvCtrl == NULL)
        return (ERROR);

    pDrvCtrl = (ETSEC_DRV_CTRL *)pDev->pDrvCtrl;

    semTake (pDrvCtrl->etsecDevSem, WAIT_FOREVER);

    if (pDrvCtrl->etsecMiiBus == NULL)
        {
        semGive (pDrvCtrl->etsecDevSem);
        return (ERROR);
        }

    oldStatus = pDrvCtrl->etsecCurStatus;
    if (miiBusModeGet(pDrvCtrl->etsecMiiBus,
        &pDrvCtrl->etsecCurMedia, &pDrvCtrl->etsecCurStatus) == ERROR)
        {
        semGive (pDrvCtrl->etsecDevSem);
        return (ERROR);
        }

    /*
     * Briefly disable the receiver and transmitter while we update
     * the MAC configuration.
     */

    CSR_CLRBIT_4(pDev, ETSEC_MACCFG1, ETSEC_MACCFG1_TX_EN|ETSEC_MACCFG1_RX_EN);

    /*
     * Set the MACCFG2 register correctly depending
     * on the link speed. For a gigE link we need to
     * specify GMII mode, otherwise we must select MII
     * mode.
     */

    CSR_CLRBIT_4(pDev, ETSEC_MACCFG2, ETSEC_MACCFG2_IF_MODE);

    switch (IFM_SUBTYPE(pDrvCtrl->etsecCurMedia))
        {
        case IFM_1000_T:
        case IFM_1000_SX:
            CSR_SETBIT_4(pDev, ETSEC_MACCFG2, ETSEC_IFMODE_GMII_TBI);
            /* R100M is "don't care" for gigE, but clear it anyway */
            CSR_CLRBIT_4(pDev, ETSEC_ECNTRL, ETSEC_ECNTRL_R100M);
            pDrvCtrl->etsecEndObj.mib2Tbl.ifSpeed = 1000000000;
            break;

        case IFM_100_TX:
            CSR_SETBIT_4(pDev, ETSEC_MACCFG2, ETSEC_IFMODE_MII);
            /* Set R100M bit if configured for reduced pin mode MII */
            if (CSR_READ_4(pDev, ETSEC_ECNTRL) & ETSEC_ECNTRL_RPM)
                CSR_SETBIT_4(pDev, ETSEC_ECNTRL, ETSEC_ECNTRL_R100M);
            pDrvCtrl->etsecEndObj.mib2Tbl.ifSpeed = 100000000;
            break;

        case IFM_10_T:
            CSR_SETBIT_4(pDev, ETSEC_MACCFG2, ETSEC_IFMODE_MII);
            /* The R100M bit is always 0 for 10Mbps operation. */
            CSR_CLRBIT_4(pDev, ETSEC_ECNTRL, ETSEC_ECNTRL_R100M);
            pDrvCtrl->etsecEndObj.mib2Tbl.ifSpeed = 10000000;
            break;

        default:
            break;
        }

    if (pDrvCtrl->etsecEndObj.pMib2Tbl != NULL)
	{
        pDrvCtrl->etsecEndObj.pMib2Tbl->m2Data.mibIfTbl.ifSpeed =
            pDrvCtrl->etsecEndObj.mib2Tbl.ifSpeed;
	pDrvCtrl->etsecEndObj.pMib2Tbl->m2Data.mibXIfTbl.ifHighSpeed =
            (pDrvCtrl->etsecEndObj.mib2Tbl.ifSpeed + 500000) / 1000000;
	}

    /* Make sure the ETSEC's duplex setting matches the PHY. */

    if ((pDrvCtrl->etsecCurMedia & IFM_GMASK) == IFM_FDX)
        CSR_SETBIT_4(pDev, ETSEC_MACCFG2, ETSEC_MACCFG2_FULL_DUPLEX);
    else
        CSR_CLRBIT_4(pDev, ETSEC_MACCFG2, ETSEC_MACCFG2_FULL_DUPLEX);

    CSR_SETBIT_4(pDev, ETSEC_MACCFG1, ETSEC_MACCFG1_TX_EN|ETSEC_MACCFG1_RX_EN);

    if (!(pDrvCtrl->etsecEndObj.flags & IFF_UP))
        {
        semGive (pDrvCtrl->etsecDevSem);
        return (OK);
        }

    /* If status went from down to up, announce link up. */

    if (pDrvCtrl->etsecCurStatus & IFM_ACTIVE && !(oldStatus & IFM_ACTIVE))
        {
            /*logMsg("eth 0x%x link up\n",pDev->unitNumber,
                   pDrvCtrl->etsecEndObj.mib2Tbl.ifSpeed/1000000,0,0,0,0);*/
            jobQueueStdPost (pDrvCtrl->etsecJobQueue, NET_TASK_QJOB_PRI,
                 muxLinkUpNotify, &pDrvCtrl->etsecEndObj,
                 NULL, NULL, NULL, NULL);
        }

    /* If status went from up to down, announce link down. */

    else if (!(pDrvCtrl->etsecCurStatus & IFM_ACTIVE) && oldStatus & IFM_ACTIVE)
        {
            /*logMsg("eth 0x%x link down\n",pDev->unitNumber,0,0,0,0,0);*/
            jobQueueStdPost (pDrvCtrl->etsecJobQueue, NET_TASK_QJOB_PRI,
                 muxLinkDownNotify, &pDrvCtrl->etsecEndObj,
                 NULL, NULL, NULL, NULL);
        }

    semGive (pDrvCtrl->etsecDevSem);

    return (OK);
    }

/*****************************************************************************
*
* etsecMuxConnect - muxConnect method handler
*
* This function handles muxConnect() events, which may be triggered
* manually or (more likely) by the bootstrap code. Most VxBus
* initialization occurs before the MUX has been fully initialized,
* so the usual muxDevLoad()/muxDevStart() sequence must be defered
* until the networking subsystem is ready. This routine will ultimately
* trigger a call to etsecEndLoad() to create the END interface instance.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void etsecMuxConnect
    (
    VXB_DEVICE_ID pDev,
    void * unused
    )
    {
    ETSEC_DRV_CTRL *pDrvCtrl;
    struct hcfDevice *pHcf;
    VXB_DEVICE_ID miiDev;
    char *miiIfName;
    int miiIfUnit;

    pDrvCtrl = pDev->pDrvCtrl;

    /*
     * Initialize MII bus.
     * Note that we defer this until the MuxConnect stage in this
     * driver, unlike the others. This is because all of the PHYs are
     * managed through the etsec0 interface, and we have to allow
     * primary initialization of etsec0 to complete before we can
     * start searching for PHYs.
     */

    pHcf = (struct hcfDevice *)pDev->pBusSpecificDevInfo;

    /*
     * resourceDesc {
     * The phyAddr resource specifies the MII address of
     * the PHY associated with this instance. }
     */
    devResourceGet (pHcf, "phyAddr", HCF_RES_INT,
        (void *)&pDrvCtrl->etsecMiiPhyAddr);

    /*
     * resourceDesc {
     * The miiIfName resource specifies the device
     * which this driver should reference to manage the PHY. }
     */
    devResourceGet (pHcf, "miiIfName", HCF_RES_STRING, (void *)&miiIfName);
    /*
     * resourceDesc {
     * The miiIfUnit resource specifies the instance of 'miiIfName'
     * which this driver should reference to manage the PHY. }
     */
    devResourceGet (pHcf, "miiIfUnit", HCF_RES_INT, (void *)&miiIfUnit);

    miiDev = vxbInstByNameFind (miiIfName, miiIfUnit);

    pDrvCtrl->etsecMiiDev = miiDev;
    pDrvCtrl->etsecMiiPhyRead = vxbDevMethodGet (miiDev, (UINT32)&miiRead_desc);
    pDrvCtrl->etsecMiiPhyWrite = vxbDevMethodGet (miiDev,
        (UINT32)&miiWrite_desc);

    /* Create our MII bus. */

    miiBusCreate (pDev, &pDrvCtrl->etsecMiiBus);
    miiBusMediaListGet (pDrvCtrl->etsecMiiBus, &pDrvCtrl->etsecMediaList);
    miiBusModeSet (pDrvCtrl->etsecMiiBus,
         pDrvCtrl->etsecMediaList->endMediaListDefault);

    /* Save the cookie. */

    pDrvCtrl->etsecMuxDevCookie = muxDevLoad (pDev->unitNumber,
        etsecEndLoad, "", TRUE, pDev);

    if (pDrvCtrl->etsecMuxDevCookie != NULL)
        muxDevStart (pDrvCtrl->etsecMuxDevCookie);

    if (_func_m2PollStatsIfPoll != NULL)
        endPollStatsInit (pDrvCtrl->etsecMuxDevCookie,
            _func_m2PollStatsIfPoll);

    return;
    }

/*****************************************************************************
*
* etsecReset - reset the ETSEC controller
*
* This function issues a reset command to the controller and waits
* for it to complete. This routine is always used to place the
* controller into a known state prior to configuration.
*
* RETURNS: ERROR if the reset bit never clears, otherwise OK
*
* ERRNO: N/A
*/

LOCAL STATUS etsecReset
    (
    VXB_DEVICE_ID pDev
    )
{
/*#if defined(VPX6_HSP9_1_PROJECT)*/

#if 1
    CSR_SETBIT_4(pDev, ETSEC_MACCFG1, ETSEC_MACCFG1_SOFT_RESET);
    vxbDelay ();
    CSR_CLRBIT_4(pDev, ETSEC_MACCFG1, ETSEC_MACCFG1_SOFT_RESET);

    CSR_SETBIT_4(pDev, ETSEC_MACCFG1, ETSEC_MACCFG1_RESET_RX_MC);
    vxbDelay ();
    CSR_CLRBIT_4(pDev, ETSEC_MACCFG1, ETSEC_MACCFG1_RESET_RX_MC);

    CSR_SETBIT_4(pDev, ETSEC_MACCFG1, ETSEC_MACCFG1_RESET_TX_MC);
    vxbDelay ();
    CSR_CLRBIT_4(pDev, ETSEC_MACCFG1, ETSEC_MACCFG1_RESET_TX_MC);

    CSR_SETBIT_4(pDev, ETSEC_MACCFG1, ETSEC_MACCFG1_RESET_RX_FUN);
    vxbDelay ();
    CSR_CLRBIT_4(pDev, ETSEC_MACCFG1, ETSEC_MACCFG1_RESET_RX_FUN);

    CSR_SETBIT_4(pDev, ETSEC_MACCFG1, ETSEC_MACCFG1_RESET_TX_FUN);
    vxbDelay ();
    CSR_CLRBIT_4(pDev, ETSEC_MACCFG1, ETSEC_MACCFG1_RESET_TX_FUN);

    CSR_SETBIT_4(pDev, ETSEC_DMACTRL, ETSEC_DMACTRL_GTS|ETSEC_DMACTRL_GRS);
    vxbDelay ();
    
#else	/*default*/ 
        /*notice dongyang , 本来9.1项目用上面代码,SP2020用下面的代码
         *但目前SP2020的文件也依赖到vxbDelay()的定义了.....
         */
	CSR_SETBIT_4(pDev, ETSEC_MACCFG1, ETSEC_MACCFG1_SOFT_RESET);
	vxbDelay (20);
	CSR_CLRBIT_4(pDev, ETSEC_MACCFG1, ETSEC_MACCFG1_SOFT_RESET);

	CSR_SETBIT_4(pDev, ETSEC_MACCFG1, ETSEC_MACCFG1_RESET_RX_MC);
	vxbDelay (20);
	CSR_CLRBIT_4(pDev, ETSEC_MACCFG1, ETSEC_MACCFG1_RESET_RX_MC);

	CSR_SETBIT_4(pDev, ETSEC_MACCFG1, ETSEC_MACCFG1_RESET_TX_MC);
	vxbDelay (20);
	CSR_CLRBIT_4(pDev, ETSEC_MACCFG1, ETSEC_MACCFG1_RESET_TX_MC);

	CSR_SETBIT_4(pDev, ETSEC_MACCFG1, ETSEC_MACCFG1_RESET_RX_FUN);
	vxbDelay (20);
	CSR_CLRBIT_4(pDev, ETSEC_MACCFG1, ETSEC_MACCFG1_RESET_RX_FUN);

	CSR_SETBIT_4(pDev, ETSEC_MACCFG1, ETSEC_MACCFG1_RESET_TX_FUN);
	vxbDelay (20);
	CSR_CLRBIT_4(pDev, ETSEC_MACCFG1, ETSEC_MACCFG1_RESET_TX_FUN);

	CSR_SETBIT_4(pDev, ETSEC_DMACTRL, ETSEC_DMACTRL_GTS|ETSEC_DMACTRL_GRS);
	vxbDelay (200);
#endif



#ifndef _WRS_CONFIG_WRHV_GUEST
    CSR_WRITE_4(pDev, ETSEC_MIIMCOM, 0);
#endif /* _WRS_CONFIG_WRHV_GUEST */

    /* Initialize the filer. */

    etsecFilerInit (pDev);

    return (OK);
    }

/*****************************************************************************
*
* etsecFilerTableRead - read the ETSEC filer table
*
* This function reads one more more RX filer rules out of the ETSEC's internal
* filer table. The filer table is 256 entries in size, and each entry can
* be accessed via the RQFCR and RQFPR registers. The RQFAR register selects
* the address of the entry to be read.
*
* The <pTable> argument points to a variable size array of filer entry
* structures. Each filer entry contains a table index number and RQFCR
* and RQFPR fields to hold the value from that table index. It also contains
* a field to hold the contents of the RBIFX register, which controls the
* extraction of user-specified portions of the frame that can be matched
* using the 'arbitrary data' filer rule. There is only one RBIFX register,
* so if the caller asks to read multiple table entries, each one will
* contain the same RBIFX value.
*
* Each entry also contains a <filterEnable> field. If this field is set
* to ETSEC_FILER_ENABLE, the entry will be populated with the requested
* rule from the table. If the field is set to ETSEC_FILER_DISABLE, the
* the entry will be skipped. If the field is set to ETSEC_FILER_END, the
* the entry is treated as an 'end of list' marker and causes processing
* of the entry array to terminate. (The other contents of the
* ETSEC_FILER_END entry are ignored.)
*
* Since the filer table is limited to 256 entries, this function will only
* process the first 256 entries in an array, even if the array contains
* more than that. If the array is larger than 256 entries, the routine
* will return error.
*
* RETURNS: ERROR if <pTable> is NULL or if there are too many entries,
*          otherwise OK
*
* ERRNO: N/A
*/

LOCAL STATUS etsecFilerTableRead
    (
    VXB_DEVICE_ID pDev,
    ETSEC_FILER_ENTRY * pTable
    )
    {
    ETSEC_FILER_ENTRY * p;
    int i = 0;

    if (pTable == NULL)
        return (ERROR);

    p = pTable;

    while (i < ETSEC_FILER_TABLE_SIZE)
        {
        if (p->filerEnable == ETSEC_FILER_END)
            break;

        if (p->filerEnable == ETSEC_FILER_DISABLE)
            {
            p++;
            continue;
            }

        /* Specify the table index to read */

        CSR_WRITE_4(pDev, ETSEC_RQFAR, p->rqfar.val);

        /*
         * Read the table control and property fields,
         * and the RBIFX register
         */

        p->rqfcr.val = CSR_READ_4(pDev, ETSEC_RQFCR);
        p->rqfpr.val = CSR_READ_4(pDev, ETSEC_RQFPR);
        p->rbifx.val = CSR_READ_4(pDev, ETSEC_RBIFX);

        i++;
        p++;
        }

    if (p->filerEnable != ETSEC_FILER_END)
        return (ERROR);

    return (OK);
    }

/*****************************************************************************
*
* etsecFilerTableWrite - modify the ETSEC filer table
*
* This function writes one more more RX filer rules into the ETSEC's internal
* filer table. The filer table is 256 entries in size, and each entry can
* be updated via the RQFCR and RQFPR registers. The RQFAR register selects
* the address of the entry to be updated.
*
* The <pTable> argument points to a variable size array of filer entry
* structures. Each filer entry contains a table index number and the RQFCR
* and RQFPR values to be written at that table index. It also contains a
* value that can be written to the RBIFX register, which controls the
* extraction of user-specified portions of the frame that can be matched
* using the 'arbitrary data' filer rule. There is only one RBIFX register,
* so if the table entry contains a non-zero RBIFX value, that value will
* always overwrite the previous value.
*
* Each entry also contains a <filterEnable> field. If this field is set
* to ETSEC_FILER_ENABLE, the rule will be written. If the field is set
* to ETSEC_FILER_DISABLE, the entry will be skipped. If the field is set
* to ETSEC_FILER_END, the entry is treated as an 'end of list' marker
* and causes processing of the entry array to terminate. (The other
* contents of the ETSEC_FILER_END entry are ignored.)
*
* Since the filer table is limited to 256 entries, this function will only
* process the first 256 entries in an array, even if the array contains
* more than that. If the array is larger than 256 entries, the first 256
* will be processed, but the routine will return error.
*
* RETURNS: ERROR if <pTable> is NULL or if there are too many entries,
*          otherwise OK
*
* ERRNO: N/A
*/

LOCAL STATUS etsecFilerTableWrite
    (
    VXB_DEVICE_ID pDev,
    ETSEC_FILER_ENTRY * pTable
    )
    {
    ETSEC_FILER_ENTRY * p;
    int i = 0;

    if (pTable == NULL)
        return (ERROR);

    CSR_CLRBIT_4(pDev, ETSEC_RCTRL, ETSEC_RCTRL_FILREN);

    p = pTable;

    while (i < ETSEC_FILER_TABLE_SIZE)
        {
        if (p->filerEnable == ETSEC_FILER_END)
            break;

        if (p->filerEnable == ETSEC_FILER_DISABLE)
            {
            p++;
            continue;
            }

        /*
         * The RBIFX register really should only be set
         * once, not once per rule.
         */

        if (p->rbifx.details.b0ctl != ETSEC_BCTL_NONE ||
            p->rbifx.details.b1ctl != ETSEC_BCTL_NONE ||
            p->rbifx.details.b2ctl != ETSEC_BCTL_NONE ||
            p->rbifx.details.b3ctl != ETSEC_BCTL_NONE)
            CSR_WRITE_4(pDev, ETSEC_RBIFX, p->rbifx.val);

        /* Specify the table index to update */

        CSR_WRITE_4(pDev, ETSEC_RQFAR, p->rqfar.val);

        /* Write the table control and property fields */

        CSR_WRITE_4(pDev, ETSEC_RQFCR, p->rqfcr.val);
        CSR_WRITE_4(pDev, ETSEC_RQFPR, p->rqfpr.val);

        i++;
        p++;
        }

    CSR_SETBIT_4(pDev, ETSEC_RCTRL, ETSEC_RCTRL_FILREN);

    if (p->filerEnable != ETSEC_FILER_END)
        return (ERROR);

    return (OK);
    }

/*****************************************************************************
*
* etsecFilerInit - initialize the ETSEC RX frame filer
*
* This function programs the ETSEC's frame filer with startup default
* values. All 256 rule slots are initially reset, and one default rule
* is installed which matches all inbound traffic and deposits it in
* RX DMA ring 7. The user-specified byte extraction register is also
* cleared.
*
* RETURNS: ERROR if the filer isn't enabled, otherwise OK
*
* ERRNO: N/A
*/

LOCAL STATUS etsecFilerInit
    (
    VXB_DEVICE_ID pDev
    )
    {
    int i;
    ETSEC_DRV_CTRL * pDrvCtrl;
    ETSEC_FILER_ENTRY table[2];
    ETSEC_FILER_ENTRY * p;

    pDrvCtrl = pDev->pDrvCtrl;

    if (pDrvCtrl->etsecFiler == FALSE)
        return (ERROR);

    bzero ((char *)table, sizeof(table));

    /* Clear the whole filer rule table */

    CSR_WRITE_4(pDev, ETSEC_RBIFX, 0);

    for (i = 0; i < ETSEC_FILER_TABLE_SIZE; i++)
        {
        CSR_WRITE_4(pDev, ETSEC_RQFAR, i);
        CSR_WRITE_4(pDev, ETSEC_RQFCR, 0);
        CSR_WRITE_4(pDev, ETSEC_RQFPR, 0);
        }

    /* Default rule: put everything in ring 7 */

    p = &table[0];
    p->filerEnable = ETSEC_FILER_ENABLE;
    p->rqfar.details.tableIndex = 0;
    p->rqfcr.details.qnum = 7;
    p->rqfcr.details.pid = ETSEC_RQFCR_PID_MASK;
    p->rqfcr.details.cmp = ETSEC_RQFCR_CMP_EQ;

    /* terminate ruleset */

    p = &table[1];
    p->filerEnable = ETSEC_FILER_END;

    etsecFilerTableWrite (pDev, table);

    return (OK);
    }

/*****************************************************************************
*
* etsecFilerLoad - load sample rules into the filer
*
* This function loads an example set of rules into the ETSEC's RX frame
* filer. The rules are designed to illustrate how the filer is programmed
* as a development aid for those wishing to construct their own rulesets.
*
* The five rules are designed to process incoming traffic as follows:
*
* - All packets with a valid TCP header are put into DMA queue 0
* - All packets with a value UDP header are put into DMA queue 1
* - All IP packets with a protocol time of IPPROTO_ICMP are put
*   into DMA queue 2
* - All ethernet ARP frames are put into DMA queue 3
* - All other frames are diverted to DMA queue 7
*
* Note that when the filer is enabled, all 8 DMA queues are enabled, but
* only one will will be used if the filer is left unprogrammed, so in
* addition to acting as a coding/API usage sample, this routine also allows
* us to exercise and validate the multi-queue RX handling code in the driver.
*
* The TCP and UDP packet selection is done using the 'frame properties'
* filtering rule. We set the filter_mask register so that on the 'TCP
* header parsed' and 'UDP header parsed' bits in the frame properties
* word are considered. (The mask is restored afterwards so as not to
* affect subsequent rules.)
*
* Filtering of ICMP traffic is performed using a 'level 4 protocol
* identifier' rule that matches all packets with a protocol ID of
* IPPROTO_ICMP. Note that matching of TCP and UDP traffic can also be
* done using this type of rule (to test for IPPROTO_TCP or IPPROTO_UDP),
* but we filter TCP and UDP using the 'frame properties' rule instead
* to give an example of both rule types.
*
* Filtering of ARP frames is done using an 'ethertype' filtering rule,
* which matches on frames containing ETHERTYPE_ARP.
*
* The final rule is an 'always match' rule that dumps all traffic that
* doesn't match any of the previous rules into  DMA queue 7.
*
* RETURNS: ERROR if updating the filer table fails, otherwise OK
*
* ERRNO: N/A
*/

LOCAL STATUS etsecFilerLoad
    (
    VXB_DEVICE_ID pDev
    )
    {
    ETSEC_DRV_CTRL * pDrvCtrl;
    ETSEC_FILER_ENTRY table[8];
    ETSEC_FILER_ENTRY * p;

    pDrvCtrl = pDev->pDrvCtrl;

    bzero ((char *)table, sizeof(table));

    /*
     * We want to use the properties list to detect TCP and UDP
     * packets, however we can only easily use the 'equals' comparison
     * operator for this. But the properties list may have other bits
     * set, which makes it hard to do an exact match. To get around
     * this, we use the mask PID to set the filter's mask register
     * to only allow the 'parsed TCP header' and 'parsed UDP header'
     * bits in the properties list to participate in the filtering
     * decision. Once we've performed the TCP and UDP test, we
     * restore the mask to all ones so that we can properly evaluate
     * any subsequent rules.
     */

    /*
     * Set the properties mask to allow only the TCP and UDP bits through.
     * This rule programs the mask register, and never matches any frames.
     */

    p = &table[0];
    p->filerEnable = ETSEC_FILER_ENABLE;
    p->rqfar.details.tableIndex = 0;
    p->rqfcr.details.qnum = 0;
    p->rqfcr.details.pid = ETSEC_RQFCR_PID_MASK;
    p->rqfcr.details.cmp = ETSEC_RQFCR_CMP_NE;
    p->rqfpr.propertyType.tcp = 1;
    p->rqfpr.propertyType.udp = 1;

    /* Place all TCP traffic in queue 0 */

    p = &table[1];
    p->filerEnable = ETSEC_FILER_ENABLE;
    p->rqfar.details.tableIndex = 1;
    p->rqfcr.details.qnum = 0;
    p->rqfcr.details.cmp = ETSEC_RQFCR_CMP_EQ;
    p->rqfcr.details.pid = ETSEC_RQFCR_PID_PROP;
    p->rqfpr.propertyType.tcp = 1;

    /* Place all UDP traffic in queue 1 */

    p = &table[2];
    p->filerEnable = ETSEC_FILER_ENABLE;
    p->rqfar.details.tableIndex = 2;
    p->rqfcr.details.qnum = 1;
    p->rqfcr.details.cmp = ETSEC_RQFCR_CMP_EQ;
    p->rqfcr.details.pid = ETSEC_RQFCR_PID_PROP;
    p->rqfpr.propertyType.udp = 1;

    /*
     * Restore the mask to all ones. This rule also never
     * matches any frames.
     */

    p = &table[3];
    p->filerEnable = ETSEC_FILER_ENABLE;
    p->rqfar.details.tableIndex = 3;
    p->rqfcr.details.qnum = 0;
    p->rqfcr.details.pid = ETSEC_RQFCR_PID_MASK;
    p->rqfcr.details.cmp = ETSEC_RQFCR_CMP_NE;
    p->rqfpr.val = 0xFFFFFFFF;

    /* Place all ICMP traffic in queue 2 */

    p = &table[4];
    p->filerEnable = ETSEC_FILER_ENABLE;
    p->rqfar.details.tableIndex = 4;
    p->rqfcr.details.qnum = 2;
    p->rqfcr.details.cmp = ETSEC_RQFCR_CMP_EQ;
    p->rqfcr.details.pid = ETSEC_RQFCR_PID_L4P;
    p->rqfpr.l4p.l4ProtoId = IPPROTO_ICMP;

    /* Place all ARP traffic in queue 3 */

    p = &table[5];
    p->filerEnable = ETSEC_FILER_ENABLE;
    p->rqfar.details.tableIndex = 5;
    p->rqfcr.details.qnum = 3;
    p->rqfcr.details.cmp = ETSEC_RQFCR_CMP_EQ;
    p->rqfcr.details.pid = ETSEC_RQFCR_PID_ETY;
    p->rqfpr.ety.etherType = ETHERTYPE_ARP;

    /* Default rule: everything else in queue 7 */

    p = &table[6];
    p->filerEnable = ETSEC_FILER_ENABLE;
    p->rqfar.details.tableIndex = 6;
    p->rqfcr.details.qnum = 7;
    p->rqfcr.details.pid = ETSEC_RQFCR_PID_MASK;
    p->rqfcr.details.cmp = ETSEC_RQFCR_CMP_GTE;

    /* terminate ruleset (all other fields are "don't care") */

    p = &table[7];
    p->filerEnable = ETSEC_FILER_END;

    etsecEndIoctl (&pDrvCtrl->etsecEndObj, EIOCSIFRULES, (caddr_t)table);

    return (OK);
    }


/*****************************************************************************
*
* etsecEndLoad - END driver entry point
*
* This routine initializes the END interface instance associated
* with this device. In traditional END drivers, this function is
* the only public interface, and it's typically invoked by a BSP
* driver configuration stub. With VxBus, the BSP stub code is no
* longer needed, and this function is now invoked automatically
* whenever this driver's muxConnect() method is called.
*
* For older END drivers, the load string would contain various
* configuration parameters, but with VxBus this use is deprecated.
* The load string should just be an empty string. The second
* argument should be a pointer to the VxBus device instance
* associated with this device. Like older END drivers, this routine
* will still return the device name if the init string is empty,
* since this behavior is still expected by the MUX. The MUX will
* invoke this function twice: once to obtain the device name,
* and then again to create the actual END_OBJ instance.
*
* When this function is called the second time, it will initialize
* the END object, perform MIB2 setup, allocate a buffer pool, and
* initialize the supported END capabilities. The only special
* capability we support is VLAN_MTU, since we can receive slightly
* larger than normal frames.
*
* RETURNS: An END object pointer, or NULL on error, or 0 and the name
* of the device if the <loadStr> was empty.
*
* ERRNO: N/A
*/

LOCAL END_OBJ * etsecEndLoad
    (
    char * loadStr,
    void * pArg
    )
    {
    ETSEC_DRV_CTRL * pDrvCtrl;
    VXB_DEVICE_ID pDev;
    VXB_INST_PARAM_VALUE val;
    STATUS r;
    int cnt;

    /* Make the MUX happy. */

    if (loadStr == NULL)
        return (NULL);

    if (loadStr[0] == 0)
        {
        bcopy (ETSEC_NAME, loadStr, sizeof(ETSEC_NAME));
        return (NULL);
        }

    pDev = pArg;
    pDrvCtrl = pDev->pDrvCtrl;

    if (END_OBJ_INIT (&pDrvCtrl->etsecEndObj, NULL, pDev->pName,
        pDev->unitNumber, &etsecNetFuncs,
        "Enhanced TSEC VxBus END Driver") == ERROR)
        {
        logMsg("%s%d: END_OBJ_INIT failed\n", (int)ETSEC_NAME,
            pDev->unitNumber, 0, 0, 0, 0);
        return (NULL);
        }

    endM2Init (&pDrvCtrl->etsecEndObj, M2_ifType_ethernet_csmacd,
        pDrvCtrl->etsecAddr, ETHER_ADDR_LEN, ETHERMTU, 100000000,
        IFF_NOTRAILERS | IFF_SIMPLEX | IFF_MULTICAST | IFF_BROADCAST);

    /*
     * See if the user wants jumbo frame support for this
     * interface. If the "jumboEnable" option isn't specified,
     * or it's set to 0, then jumbo support stays off,
     * otherwise we switch it on.
     */

    /*
     * paramDesc {
     * The jumboEnable parameter specifies whether
     * this instance should support jumbo frames.
     * The default is false. }
     */
    r = vxbInstParamByNameGet (pDev, "jumboEnable", VXB_PARAM_INT32, &val);

    cnt = (ETSEC_RX_DESC_CNT * 2);

    if (pDrvCtrl->etsecFiler == TRUE)
        cnt *= ETSEC_MAX_RX_QUEUES;

    cnt += ETSEC_TX_DESC_CNT;

    if (r != OK || val.int32Val == 0)
        {
        pDrvCtrl->etsecMaxMtu = ETSEC_MTU;
        r = endPoolCreate (cnt, &pDrvCtrl->etsecEndObj.pNetPool);
        }
    else
        {
        pDrvCtrl->etsecMaxMtu = ETSEC_JUMBO_MTU;
        r = endPoolJumboCreate (cnt, &pDrvCtrl->etsecEndObj.pNetPool);
        }

    /* Allocate a buffer pool */

    if (r == ERROR)
        {
        logMsg("%s%d: pool creation failed\n", (int)ETSEC_NAME,
            pDev->unitNumber, 0, 0, 0, 0);
        return (NULL);
        }

    pDrvCtrl->etsecPollBuf = endPoolTupleGet (pDrvCtrl->etsecEndObj.pNetPool);

    /* Set up polling stats. */

    pDrvCtrl->etsecEndStatsConf.ifPollInterval = sysClkRateGet();
    pDrvCtrl->etsecEndStatsConf.ifEndObj = &pDrvCtrl->etsecEndObj;
    pDrvCtrl->etsecEndStatsConf.ifWatchdog = NULL;
    pDrvCtrl->etsecEndStatsConf.ifValidCounters = (END_IFINUCASTPKTS_VALID |
        END_IFINMULTICASTPKTS_VALID | END_IFINBROADCASTPKTS_VALID |
        END_IFINOCTETS_VALID | END_IFINERRORS_VALID | END_IFINDISCARDS_VALID |
        END_IFOUTUCASTPKTS_VALID | END_IFOUTMULTICASTPKTS_VALID |
        END_IFOUTBROADCASTPKTS_VALID | END_IFOUTOCTETS_VALID |
        END_IFOUTERRORS_VALID);


    /*
     * Set up capabilities.
     * We support checksum offload for IPv4 and IPv6 and VLAN MTU.
     */

    pDrvCtrl->etsecCaps.cap_available = IFCAP_VLAN_MTU;
    pDrvCtrl->etsecCaps.cap_enabled = IFCAP_VLAN_MTU;

    pDrvCtrl->etsecCaps.csum_flags_tx = CSUM_IP|CSUM_TCP|CSUM_UDP;
    pDrvCtrl->etsecCaps.csum_flags_rx = CSUM_IP|CSUM_UDP|CSUM_TCP;
    pDrvCtrl->etsecCaps.csum_flags_tx |= CSUM_TCPv6|CSUM_UDPv6;
    pDrvCtrl->etsecCaps.csum_flags_rx |= CSUM_TCPv6|CSUM_UDPv6;
    pDrvCtrl->etsecCaps.cap_available |= IFCAP_TXCSUM|IFCAP_RXCSUM;
    pDrvCtrl->etsecCaps.cap_enabled |= IFCAP_TXCSUM|IFCAP_RXCSUM;

    if (pDrvCtrl->etsecMaxMtu == ETSEC_JUMBO_MTU)
        {
        pDrvCtrl->etsecCaps.cap_available |= IFCAP_JUMBO_MTU;
        pDrvCtrl->etsecCaps.cap_enabled |= IFCAP_JUMBO_MTU;
        }

    /*
     * Attach our ISRs. The ETSEC has three separate interrupt
     * vectors: one for TX events, one for RX events and one
     * for error events. We need to provide handlers for all
     * three.
     */

    vxbIntConnect (pDev, 0, etsecEndInt, pDrvCtrl);
    vxbIntConnect (pDev, 1, etsecEndInt, pDrvCtrl);
    vxbIntConnect (pDev, 2, etsecEndInt, pDrvCtrl);

    return (&pDrvCtrl->etsecEndObj);
    }

/*****************************************************************************
*
* etsecEndUnload - unload END driver instance
*
* This routine undoes the effects of etsecEndLoad(). The END object
* is destroyed, our network pool is released, the endM2 structures
* are released, and the polling stats watchdog is terminated.
*
* Note that the END interface instance can't be unloaded if the
* device is still running. The device must be stopped with muxDevStop()
* first.
*
* RETURNS: ERROR if device is still in the IFF_UP state, otherwise OK
*
* ERRNO: N/A
*/

LOCAL STATUS etsecEndUnload
    (
    END_OBJ * pEnd
    )
    {
    ETSEC_DRV_CTRL * pDrvCtrl;

    /* We must be stopped before we can be unloaded. */

    if (pEnd->flags & IFF_UP)
	return (ERROR);

    pDrvCtrl = (ETSEC_DRV_CTRL *)pEnd;

    endPoolTupleFree (pDrvCtrl->etsecPollBuf);

    /* Relase our buffer pool */
    endPoolDestroy (pDrvCtrl->etsecEndObj.pNetPool);

    /* terminate stats polling */
    wdDelete (pDrvCtrl->etsecEndStatsConf.ifWatchdog);

    endM2Free (&pDrvCtrl->etsecEndObj);

    /* Destroy our MII bus and child PHYs. */

    miiBusDelete (pDrvCtrl->etsecMiiBus);

    semDelete (pDrvCtrl->etsecDevSem);

    END_OBJECT_UNLOAD (&pDrvCtrl->etsecEndObj);

    return (EALREADY);  /* prevent freeing of pDrvCtrl */
    }

/*****************************************************************************
*
* etsecEndHashTblPopulate - populate the multicast hash filter
*
* This function programs the ETSEC controller's multicast hash
* filter to receive frames sent to the multicast groups specified
* in the multicast address list attached to the END object. If
* the interface is in IFF_ALLMULTI mode, the filter will be
* programmed to receive all multicast packets by setting all the
* bits in the hash table to one.
*
* The ETSEC supports both a 16 entry perfect CAM filter and a 512
* bit multicast hash table. (The individual address hash table and
* the group address table can be combined into a single double-wide
* group hash table.) We use the first CAM entry for the station address
* and the remaining entries for the first 15 multicast addresses.
* Any remaining multicast addresses are used to populate the hash
* table.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void etsecEndHashTblPopulate
    (
    ETSEC_DRV_CTRL * pDrvCtrl
    )
    {
    VXB_DEVICE_ID pDev;
    ETHER_MULTI * mCastNode = NULL;
    UINT32 hashes[16] = { 0, 0, 0, 0, 0, 0, 0, 0,
                          0, 0, 0, 0, 0, 0, 0, 0 };
    UINT32 addr[2];
    UINT32 h;
    int i;

    pDev = pDrvCtrl->etsecDev;

    /* Always initialize the CAM filter. */

    for (i = 1; i < ETSEC_CAM_CNT; i++)
        {
        CSR_WRITE_4(pDev, ETSEC_PAR0 + (i * sizeof(UINT64)), 0);
        CSR_WRITE_4(pDev, ETSEC_PAR1 + (i * sizeof(UINT64)), 0);
        }

    if (pDrvCtrl->etsecEndObj.flags & IFF_ALLMULTI)
        {
        /* set allmulticast mode */
        for (i = 0; i < 8; i++)
            {
            CSR_WRITE_4(pDev, ETSEC_IADDR0 + (i * sizeof(UINT32)), 0xFFFFFFFF);
            CSR_WRITE_4(pDev, ETSEC_GADDR0 + (i * sizeof(UINT32)), 0xFFFFFFFF);
            }
        return;
        }

    /* First, clear out the original filter. */

    for (i = 0; i < 8; i++)
        {
        CSR_WRITE_4(pDev, ETSEC_IADDR0 + (i * sizeof(UINT32)), 0);
        CSR_WRITE_4(pDev, ETSEC_GADDR0 + (i * sizeof(UINT32)), 0);
        }

    /* Now repopulate it. */

    i = 1;

    for (mCastNode =
        (ETHER_MULTI *) lstFirst (&pDrvCtrl->etsecEndObj.multiList);
         mCastNode != NULL;
         mCastNode = (ETHER_MULTI *) lstNext (&mCastNode->node))
        {
        if (i < ETSEC_CAM_CNT)
            {
            bcopy (mCastNode->addr, (char *)addr + 2, ETHER_ADDR_LEN);
            CSR_WRITE_4(pDev, ETSEC_PAR1 + (i * sizeof(UINT64)),
                htole32(addr[0]));
            CSR_WRITE_4(pDev, ETSEC_PAR0 + (i * sizeof(UINT64)),
                htole32(addr[1]));
            i++;
            }
        else
            {
            h = endEtherCrc32BeGet ((const UINT8 *) mCastNode->addr,
                ETHER_ADDR_LEN);

            /* The 9 most significant bits determine the table bit to set. */

            h = ((~h) >> 23) & 0x1FF;
            hashes[((h >> 5))] |= (1 << (h & 0x1F));
            }
        }

    /* reload hash filter */

    for (i = 0; i < 8; i++)
        {
        CSR_WRITE_4(pDev, ETSEC_GADDR0 + (i * sizeof(UINT32)), hashes[7 - i]);
        CSR_WRITE_4(pDev, ETSEC_IADDR0 + (i * sizeof(UINT32)), hashes[15 - i]);
        }

    return;
    }

/*****************************************************************************
*
* etsecEndMCastAddrAdd - add a multicast address for the device
*
* This routine adds a multicast address to whatever the driver
* is already listening for.  It then resets the address filter.
*
* RETURNS: OK or ERROR.
*
* ERRNO: N/A
*/

LOCAL STATUS etsecEndMCastAddrAdd
    (
    END_OBJ * pEnd,
    char * pAddr
    )
    {
    int retVal;
    ETSEC_DRV_CTRL * pDrvCtrl;

    pDrvCtrl = (ETSEC_DRV_CTRL *)pEnd;

    semTake (pDrvCtrl->etsecDevSem, WAIT_FOREVER);

    if (!(pDrvCtrl->etsecEndObj.flags & IFF_UP))
        {
        semGive (pDrvCtrl->etsecDevSem);
        return (OK);
        }

    retVal = etherMultiAdd (&pEnd->multiList, pAddr);

    if (retVal == ENETRESET)
        {
        pEnd->nMulti++;
        etsecEndHashTblPopulate ((ETSEC_DRV_CTRL *)pEnd);
        }

    semGive (pDrvCtrl->etsecDevSem);
    return (OK);
    }

/*****************************************************************************
*
* etsecEndMCastAddrDel - delete a multicast address for the device
*
* This routine removes a multicast address from whatever the driver
* is listening for.  It then resets the address filter.
*
* RETURNS: OK or ERROR.
*
* ERRNO: N/A
*/

LOCAL STATUS etsecEndMCastAddrDel
    (
    END_OBJ * pEnd,
    char * pAddr
    )
    {
    int retVal;
    ETSEC_DRV_CTRL * pDrvCtrl;

    pDrvCtrl = (ETSEC_DRV_CTRL *)pEnd;

    semTake (pDrvCtrl->etsecDevSem, WAIT_FOREVER);

    if (!(pDrvCtrl->etsecEndObj.flags & IFF_UP))
        {
        semGive (pDrvCtrl->etsecDevSem);
        return (OK);
        }

    retVal = etherMultiDel (&pEnd->multiList, pAddr);

    if (retVal == ENETRESET)
        {
        pEnd->nMulti--;
        etsecEndHashTblPopulate ((ETSEC_DRV_CTRL *)pEnd);
        }

    semGive (pDrvCtrl->etsecDevSem);

    return (OK);
    }

/*****************************************************************************
*
* etsecEndMCastAddrGet - get the multicast address list for the device
*
* This routine gets the multicast list of whatever the driver
* is already listening for.
*
* RETURNS: OK or ERROR.
*
* ERRNO: N/A
*/

LOCAL STATUS etsecEndMCastAddrGet
    (
    END_OBJ * pEnd,
    MULTI_TABLE * pTable
    )
    {
    STATUS rval;
    ETSEC_DRV_CTRL * pDrvCtrl;

    pDrvCtrl = (ETSEC_DRV_CTRL *)pEnd;

    semTake (pDrvCtrl->etsecDevSem, WAIT_FOREVER);

    if (!(pDrvCtrl->etsecEndObj.flags & IFF_UP))
        {
        semGive (pDrvCtrl->etsecDevSem);
        return (OK);
        }

    rval = etherMultiGet (&pEnd->multiList, pTable);

    semGive (pDrvCtrl->etsecDevSem);

    return (rval);
    }

/*****************************************************************************
*
* etsecEndStatsDump - return polled statistics counts
*
* This routine is automatically invoked periodically by the polled
* statistics watchdog.
*
* RETURNS: always OK
*
* ERRNO: N/A
*/

LOCAL STATUS etsecEndStatsDump
    (
    ETSEC_DRV_CTRL * pDrvCtrl
    )
    {
    END_IFCOUNTERS *    pEndStatsCounters;
    VXB_DEVICE_ID pDev;

    if (!(pDrvCtrl->etsecEndObj.flags & IFF_UP))
        return (ERROR);

    pDev = pDrvCtrl->etsecDev;

    pEndStatsCounters = &pDrvCtrl->etsecEndStatsCounters;

    /* Get number of RX'ed octets */

    pEndStatsCounters->ifInOctets = CSR_READ_4(pDev, ETSEC_RBYT);

    /* Get number of TX'ed octets */
   
    pEndStatsCounters->ifOutOctets = CSR_READ_4(pDev, ETSEC_TBYT);

    /* Get RX'ed unicasts, broadcasts, multicasts */

    pEndStatsCounters->ifInUcastPkts = CSR_READ_4(pDev, ETSEC_RPKT);

    pEndStatsCounters->ifInBroadcastPkts = CSR_READ_4(pDev, ETSEC_RBCA);

    pEndStatsCounters->ifInMulticastPkts = CSR_READ_4(pDev, ETSEC_RMCA);

    pEndStatsCounters->ifInUcastPkts -= (pEndStatsCounters->ifInMulticastPkts +
                                         pEndStatsCounters->ifInBroadcastPkts);

    /* Get TX'ed unicasts, broadcasts, multicasts */

    pEndStatsCounters->ifOutUcastPkts = CSR_READ_4(pDev, ETSEC_TPKT);

    pEndStatsCounters->ifOutBroadcastPkts = CSR_READ_4(pDev, ETSEC_TBCA);

    pEndStatsCounters->ifOutMulticastPkts = CSR_READ_4(pDev, ETSEC_TMCA);
    pEndStatsCounters->ifOutUcastPkts -=
        (pEndStatsCounters->ifOutMulticastPkts +
         pEndStatsCounters->ifOutBroadcastPkts);

    return (OK);
    }

/*****************************************************************************
*
* etsecEndIoctl - the driver I/O control routine
*
* This function processes ioctl requests supplied via the muxIoctl()
* routine. In addition to the normal boilerplate END ioctls, this
* driver supports the IFMEDIA ioctls, END capabilities ioctls, and
* polled stats ioctls.
*
* RETURNS: A command specific response, usually OK or ERROR.
*
* ERRNO: N/A
*/

LOCAL int etsecEndIoctl
    (
    END_OBJ * pEnd,
    int cmd,
    caddr_t data
    )
    {
    ETSEC_DRV_CTRL * pDrvCtrl;
    END_MEDIALIST * mediaList;
    END_CAPABILITIES * hwCaps;
    END_MEDIA * pMedia;
    END_RCVJOBQ_INFO * qinfo;
    UINT32 nQs;
    VXB_DEVICE_ID pDev;
    INT32 value;
    int error = OK;

    pDrvCtrl = (ETSEC_DRV_CTRL *)pEnd;
    pDev = pDrvCtrl->etsecDev;

    if (cmd != EIOCPOLLSTART && cmd != EIOCPOLLSTOP)
        semTake (pDrvCtrl->etsecDevSem, WAIT_FOREVER);

    switch (cmd)
        {
        case EIOCSADDR:
            if (data == NULL)
                error = EINVAL;
            else
                {
                bcopy ((char *)data, (char *)pDrvCtrl->etsecAddr,
                    ETHER_ADDR_LEN);
                bcopy ((char *)data,
                    (char *)pEnd->mib2Tbl.ifPhysAddress.phyAddress,
                    ETHER_ADDR_LEN);
                if (pEnd->pMib2Tbl != NULL)
                    bcopy ((char *)data,
                        (char *)pEnd->pMib2Tbl->m2Data.mibIfTbl.ifPhysAddress.phyAddress,
                        ETHER_ADDR_LEN);
                }
            etsecEndRxConfig (pDrvCtrl);
            break;

        case EIOCGADDR:
            if (data == NULL)
                error = EINVAL;
            else
                bcopy ((char *)pDrvCtrl->etsecAddr, (char *)data,
                    ETHER_ADDR_LEN);
            break;
        case EIOCSFLAGS:

            value = (INT32) data;
            if (value < 0)
                {
                value = -value;
                value--;
                END_FLAGS_CLR (pEnd, value);
                }
            else
                END_FLAGS_SET (pEnd, value);

            etsecEndRxConfig (pDrvCtrl);

            break;

        case EIOCGFLAGS:
            if (data == NULL)
                error = EINVAL;
            else
                *(long *)data = END_FLAGS_GET(pEnd);

            break;

        case EIOCMULTIADD:
            error = etsecEndMCastAddrAdd (pEnd, (char *) data);
            break;

        case EIOCMULTIDEL:
            error = etsecEndMCastAddrDel (pEnd, (char *) data);
            break;

        case EIOCMULTIGET:
            error = etsecEndMCastAddrGet (pEnd, (MULTI_TABLE *) data);
            break;

        case EIOCPOLLSTART:
            pDrvCtrl->etsecPolling = TRUE;
            pDrvCtrl->etsecIntMask = CSR_READ_4(pDev, ETSEC_IMASK);
            CSR_WRITE_4(pDev, ETSEC_IMASK, 0);

            /*
             * We may have been asked to enter polled mode while
             * there are transmissions pending. This is a problem,
             * because the polled transmit routine expects that
             * the TX ring will be empty when it's called. In
             * order to guarantee this, we have to drain the TX
             * ring here. We could also just plain reset and
             * reinitialize the transmitter, but this is faster.
             */

            while (pDrvCtrl->etsecTxFree < ETSEC_TX_DESC_CNT)
                {
                ETSEC_DESC * pDesc;
                M_BLK_ID pMblk;

                pDesc = &pDrvCtrl->etsecTxDesc[pDrvCtrl->etsecTxCons];

                /* Wait for ownership bit to clear */

                while (pDesc->bdSts & ETSEC_TBD_R)
                    ;

                pMblk = pDrvCtrl->etsecTxMblk[pDrvCtrl->etsecTxCons];

                if (pMblk != NULL)
                    {
                    endPoolTupleFree (pMblk);
                    pDrvCtrl->etsecTxMblk[pDrvCtrl->etsecTxCons] = NULL;
                    }

                pDesc->bdLen = 0;
                pDesc->bdAddr = 0;
                pDesc->bdSts &= ETSEC_TBD_W;

                pDrvCtrl->etsecTxFree++;
                ETSEC_INC_DESC (pDrvCtrl->etsecTxCons, ETSEC_TX_DESC_CNT);
                }

            break;

        case EIOCPOLLSTOP:
            pDrvCtrl->etsecPolling = FALSE;
            CSR_WRITE_4(pDev, ETSEC_IMASK, pDrvCtrl->etsecIntMask);
            break;

        case EIOCGMIB2233:
        case EIOCGMIB2:
            error = endM2Ioctl (&pDrvCtrl->etsecEndObj, cmd, data);
            break;

        case EIOCGPOLLCONF:
            if (data == NULL)
                error = EINVAL;
            else
                *((END_IFDRVCONF **)data) = &pDrvCtrl->etsecEndStatsConf;
            break;

        case EIOCGPOLLSTATS:
            if (data == NULL)
                error = EINVAL;
            else
                {
                error = etsecEndStatsDump (pDrvCtrl);
                if (error == OK)
                    *((END_IFCOUNTERS **)data) =
                        &pDrvCtrl->etsecEndStatsCounters;
                }
            break;

        case EIOCGMEDIALIST:
            if (data == NULL)
                {
                error = EINVAL;
                break;
                }
            if (pDrvCtrl->etsecMediaList->endMediaListLen == 0)
                {
                error = ENOTSUP;
                break;
                }

            mediaList = (END_MEDIALIST *)data;
            if (mediaList->endMediaListLen <
                pDrvCtrl->etsecMediaList->endMediaListLen)
                {
                mediaList->endMediaListLen =
                    pDrvCtrl->etsecMediaList->endMediaListLen;
                error = ENOSPC;
                break;
                }

            bcopy((char *)pDrvCtrl->etsecMediaList, (char *)mediaList,
                  sizeof(END_MEDIALIST) + (sizeof(UINT32) *
                  pDrvCtrl->etsecMediaList->endMediaListLen));
            break;

        case EIOCGIFMEDIA:
            if (data == NULL)
                error = EINVAL;
            else
                {
                pMedia = (END_MEDIA *)data;
                pMedia->endMediaActive = pDrvCtrl->etsecCurMedia;
                pMedia->endMediaStatus = pDrvCtrl->etsecCurStatus;
                }
            break;

        case EIOCSIFMEDIA:
            if (data == NULL)
                error = EINVAL;
            else
                {
                pMedia = (END_MEDIA *)data;
                miiBusModeSet (pDrvCtrl->etsecMiiBus, pMedia->endMediaActive);
                etsecLinkUpdate (pDrvCtrl->etsecDev);
                error = OK;
                }
            break;

        case EIOCGIFCAP:
            hwCaps = (END_CAPABILITIES *)data;
            if (hwCaps == NULL)
                {
                error = EINVAL;
                break;
                }
            hwCaps->csum_flags_tx = pDrvCtrl->etsecCaps.csum_flags_tx;
            hwCaps->csum_flags_rx = pDrvCtrl->etsecCaps.csum_flags_rx;
            hwCaps->cap_available = pDrvCtrl->etsecCaps.cap_available;
            hwCaps->cap_enabled = pDrvCtrl->etsecCaps.cap_enabled;
            break;

        case EIOCSIFCAP:
            hwCaps = (END_CAPABILITIES *)data;
            if (hwCaps == NULL)
                {
                error = EINVAL;
                break;
                }
            pDrvCtrl->etsecCaps.cap_enabled = hwCaps->cap_enabled;
            break;

        case EIOCGIFMTU:
            if (data == NULL)
                error = EINVAL;
            else
                *(INT32 *)data = pEnd->mib2Tbl.ifMtu;
            break;

        case EIOCSIFMTU:
            value = (INT32)data;
            if (value <= 0 || value > pDrvCtrl->etsecMaxMtu)
		{
                error = EINVAL;
		break;
		}
            pEnd->mib2Tbl.ifMtu = value;
            if (pEnd->pMib2Tbl != NULL)
                pEnd->pMib2Tbl->m2Data.mibIfTbl.ifMtu = value;
            break;

        case EIOCGIFRULES:
            if (data == NULL)
                error = EINVAL;
            else if (pDrvCtrl->etsecFiler == FALSE)
                error = ENOTSUP;
            else
                {
                if (etsecFilerTableRead (pDev,
                    (ETSEC_FILER_ENTRY *)data) != OK)
                    error = EIO;
                }
            break;

        case EIOCSIFRULES:
            if (data == NULL)
                error = EINVAL;
            else if (pDrvCtrl->etsecFiler == FALSE)
                error = ENOTSUP;
            else
                {
                if (etsecFilerTableWrite (pDev,
                    (ETSEC_FILER_ENTRY *)data) != OK)
                    error = EIO;
                }
            break;

        case EIOCINITRULES:
            if (pDrvCtrl->etsecFiler == FALSE)
                error = ENOTSUP;
            else
                {
                if (etsecFilerInit (pDev) != OK)
                    error = EIO;
                }
            break;

	case EIOCGRCVJOBQ:
            if (data == NULL)
		{
                error = EINVAL;
		break;
		}

	    qinfo = (END_RCVJOBQ_INFO *)data;
	    nQs = qinfo->numRcvJobQs;
	    qinfo->numRcvJobQs = 1;
	    if (nQs < 1)
		error = ENOSPC;
	    else
		qinfo->qIds[0] = pDrvCtrl->etsecJobQueue;
	    break;

        default:
            error = EINVAL;
            break;
        }

    if (cmd != EIOCPOLLSTART && cmd != EIOCPOLLSTOP)
        semGive (pDrvCtrl->etsecDevSem);

    return (error);
    }

/*****************************************************************************
*
* etsecEndRxConfig - configure the ETSEC RX filter
*
* This is a helper routine used by etsecEndIoctl() and etsecEndStart() to
* configure the controller's RX filter. The unicast address filter is
* programmed with the currently configured MAC address, and the RX
* configuration is set to allow unicast and broadcast frames to be
* received. If the interface is in IFF_PROMISC mode, the RX_PROMISC
* bit is set, which allows all packets to be received.
*
* The etsecEndHashTblPopulate() routine is also called to update the
* multicast filter.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void etsecEndRxConfig
    (
    ETSEC_DRV_CTRL * pDrvCtrl
    )
    {
    VXB_DEVICE_ID pDev;
    UINT32 addr[2] = { 0, 0 };

    pDev = pDrvCtrl->etsecDev;

    /* Copy the address to a buffer we know is 32-bit aligned. */

    bcopy ((char *)pDrvCtrl->etsecAddr, (char *)addr + 2, ETHER_ADDR_LEN);

    /* Set station address */

    CSR_WRITE_4(pDev, ETSEC_PAR1, htole32(addr[0]));
    CSR_WRITE_4(pDev, ETSEC_PAR0, htole32(addr[1]));

    /* Program the RX filter to receive unicasts and broadcasts */

    CSR_CLRBIT_4(pDev, ETSEC_RCTRL, ETSEC_RCTRL_BC_REJ);

    /* Enable promisc mode, if specified. */

    if (pDrvCtrl->etsecEndObj.flags & IFF_PROMISC)
        CSR_SETBIT_4(pDev, ETSEC_RCTRL, ETSEC_RCTRL_PROM);
    else
        CSR_CLRBIT_4(pDev, ETSEC_RCTRL, ETSEC_RCTRL_PROM);

    /* Program the multicast filter. */

    etsecEndHashTblPopulate (pDrvCtrl);

    return;
    }

/*****************************************************************************
*
* etsecEndStart - start the device
*
* This function resets the device to put it into a known state and
* then configures it for RX and TX operation. The RX and TX configuration
* registers are initialized, and the address of the RX DMA window is
* loaded into the device. Interrupts are then enabled, and the initial
* link state is configured.
*
* Note that this routine also checks to see if an alternate jobQueue
* has been specified via the vxbParam subsystem. This allows the driver
* to divert its work to an alternate processing task, such as may be
* done with TIPC. This means that the jobQueue can be changed while
* the system is running, but the device must be stopped and restarted
* for the change to take effect.
*
* RETURNS: ERROR if device initialization failed, otherwise OK
*
* ERRNO: N/A
*/

LOCAL STATUS etsecEndStart
    (
    END_OBJ * pEnd
    )
    {
    ETSEC_DRV_CTRL * pDrvCtrl;
    VXB_DEVICE_ID pDev;
    VXB_INST_PARAM_VALUE val;
    HEND_RX_QUEUE_PARAM * pRxQueue;
    M_BLK_ID pMblk;
    ETSEC_DESC * pDesc;
    int coalTicks, coalPkts;
    BOOL coalEnable = FALSE;
    int i, q;

    pDrvCtrl = (ETSEC_DRV_CTRL *)pEnd;
    pDev = pDrvCtrl->etsecDev;

    semTake (pDrvCtrl->etsecDevSem, WAIT_FOREVER);
    END_TX_SEM_TAKE (pEnd, WAIT_FOREVER);

    if (pEnd->flags & IFF_UP)
        {
        END_TX_SEM_GIVE (pEnd);
        semGive (pDrvCtrl->etsecDevSem);
        return (OK);
        }

    /* Initialize job queues */

    pDrvCtrl->etsecJobQueue = netJobQueueId;

    /* Override the job queue ID if the user supplied an alternate one. */

    /*
     * paramDesc {
     * The rxQueue00 parameter specifies a pointer to a
     * HEND_RX_QUEUE_PARAM structure, which contains,
     * among other things, an ID for the job queue
     * to be used for this instance. }
     */
    if (vxbInstParamByNameGet (pDev, "rxQueue00",
        VXB_PARAM_POINTER, &val) == OK)
        {
        pRxQueue = (HEND_RX_QUEUE_PARAM *) val.pValue;
        if (pRxQueue->jobQueId != NULL)
            pDrvCtrl->etsecJobQueue = pRxQueue->jobQueId;
        }

    /*
     * paramDesc {
     * The coalesceEnable parameter specifies whether
     * the instance should coalesce interrupts.
     * The default is false. }
     */
    if (vxbInstParamByNameGet (pDev, "coalesceEnable",
        VXB_PARAM_INT32, &val) == OK && val.int32Val)
        coalEnable = TRUE;

    QJOB_SET_PRI(&pDrvCtrl->etsecIntJob, NET_TASK_QJOB_PRI);
    pDrvCtrl->etsecIntJob.func = etsecEndIntHandle;

    vxAtomicSet (&pDrvCtrl->etsecIntPending, FALSE);

    /* Set up the RX ring. */

    for (q = 0; q < ETSEC_MAX_RX_QUEUES; q++)
        {
        pDrvCtrl->etsecRxDesc = pDrvCtrl->etsecRxQueue[q];
        pDrvCtrl->etsecRxMblk = pDrvCtrl->etsecRxMblks[q];
        for (i = 0; i < ETSEC_RX_DESC_CNT; i++)
            {
            pMblk = endPoolTupleGet (pDrvCtrl->etsecEndObj.pNetPool);
            if (pMblk == NULL)
                {
                END_TX_SEM_GIVE (pEnd);
                semGive (pDrvCtrl->etsecDevSem);
                return (ERROR);
                }

            pMblk->m_next = NULL;
            pDrvCtrl->etsecRxMblk[i] = pMblk;

            pDesc = &pDrvCtrl->etsecRxDesc[i];

            pDesc->bdAddr = ETSEC_PHYSADDR(mtod(pMblk, UINT32));
            pDesc->bdLen = 0;
            pDesc->bdSts = ETSEC_RBD_E|ETSEC_RBD_I;

            if (i == (ETSEC_RX_DESC_CNT - 1))
                pDesc->bdSts |= ETSEC_RBD_W;
            }
        /* Only populate 1 ring if the filer is off */
        if (pDrvCtrl->etsecFiler == FALSE)
            break;
        }

    /* Set up the TX ring. */

    bzero ((char *)pDrvCtrl->etsecTxDesc,
        sizeof(ETSEC_DESC) * ETSEC_TX_DESC_CNT);
    pDesc = &pDrvCtrl->etsecTxDesc[ETSEC_TX_DESC_CNT - 1];
    pDesc->bdSts = ETSEC_TBD_W;

    /* Initialize state */

    if (pDrvCtrl->etsecFiler == TRUE)
        {
        pDrvCtrl->etsecRxIdxs[0] = 0;
        pDrvCtrl->etsecRxIdxs[1] = 0;
        pDrvCtrl->etsecRxIdxs[2] = 0;
        pDrvCtrl->etsecRxIdxs[3] = 0;
        pDrvCtrl->etsecRxIdxs[4] = 0;
        pDrvCtrl->etsecRxIdxs[5] = 0;
        pDrvCtrl->etsecRxIdxs[6] = 0;
        pDrvCtrl->etsecRxIdxs[7] = 0;
        }
    else
        {
        pDrvCtrl->etsecRxIdxs[0] = 0;
        pDrvCtrl->etsecRxIdx = &pDrvCtrl->etsecRxIdxs[0];
        }
    pDrvCtrl->etsecTxLast = 0;
    pDrvCtrl->etsecTxStall = FALSE;
    pDrvCtrl->etsecTxProd = 0;
    pDrvCtrl->etsecTxCons = 0;
    pDrvCtrl->etsecTxFree = ETSEC_TX_DESC_CNT;
    pDrvCtrl->etsecRxTxEvents = 0;

    /* Set the RX buffer sizes */

    if (pDrvCtrl->etsecMaxMtu > ETSEC_MTU)
        {
        CSR_WRITE_4(pDev, ETSEC_MRBLR, END_JUMBO_CLSIZE);
        CSR_WRITE_4(pDev, ETSEC_MAXFRM, 9022);
        }
    else
        {
        CSR_WRITE_4(pDev, ETSEC_MRBLR, END_MCACHE_CLSIZE);
        CSR_WRITE_4(pDev, ETSEC_MAXFRM, 1522);
        }

    CSR_WRITE_4(pDev, ETSEC_MINFLR, 64);

    /*
     * Note: as a workaround for erratum etsec 82, we always set
     * the huge frame enable bit to disable frame truncation.
     */

    CSR_CLRBIT_4(pDev, ETSEC_MACCFG2, ETSEC_MACCFG2_HUGE_FRAME);

    /* Load the RX and TX ring addresses */

    if (pDrvCtrl->etsecFiler == TRUE)
        {
        CSR_WRITE_4(pDev, ETSEC_RBASE0,
            ETSEC_PHYSADDR(pDrvCtrl->etsecRxQueue[0]));
        CSR_WRITE_4(pDev, ETSEC_RBASE1,
            ETSEC_PHYSADDR(pDrvCtrl->etsecRxQueue[1]));
        CSR_WRITE_4(pDev, ETSEC_RBASE2,
            ETSEC_PHYSADDR(pDrvCtrl->etsecRxQueue[2]));
        CSR_WRITE_4(pDev, ETSEC_RBASE3,
            ETSEC_PHYSADDR(pDrvCtrl->etsecRxQueue[3]));
        CSR_WRITE_4(pDev, ETSEC_RBASE4,
            ETSEC_PHYSADDR(pDrvCtrl->etsecRxQueue[4]));
        CSR_WRITE_4(pDev, ETSEC_RBASE5,
            ETSEC_PHYSADDR(pDrvCtrl->etsecRxQueue[5]));
        CSR_WRITE_4(pDev, ETSEC_RBASE6,
            ETSEC_PHYSADDR(pDrvCtrl->etsecRxQueue[6]));
        CSR_WRITE_4(pDev, ETSEC_RBASE7,
            ETSEC_PHYSADDR(pDrvCtrl->etsecRxQueue[7]));
        }
    else
        CSR_WRITE_4(pDev, ETSEC_RBASE, ETSEC_PHYSADDR(pDrvCtrl->etsecRxDesc));
    CSR_WRITE_4(pDev, ETSEC_TBASE, ETSEC_PHYSADDR(pDrvCtrl->etsecTxDesc));

    /*
     * Enable snooping for both RX and TX
     * Note: as a workaround for erratum eTSEC 38, we must leave the
     * WWR bit in the DMACTRL register turned off.
     */

    CSR_WRITE_4(pDev, ETSEC_ATTR, ETSEC_ATTR_RDSEN|ETSEC_ATTR_RBDSEN);
    CSR_WRITE_4(pDev, ETSEC_DMACTRL, ETSEC_DMACTRL_TDSEN|ETSEC_DMACTRL_TBDSEN);

    /* Enable TX queue 0 only */

    CSR_WRITE_4(pDev, ETSEC_TQUEUE, ETSEC_TQUEUE_EN0);

    /*
     * Initialize transmit control. We enable IP and TCP/UDP
     * checksum offload. Note however that we do not enable VLAN
     * tag insertion. This is because of a horrible design flaw in
     * the ETSEC: when the VLINS bit is set, _all_ frames are
     * VLAN-tagged, whether we want it or not. If we don't specify
     * a valid VLAN tag in a given frame's FCB, the chip will
     * insert the default one from the DFVLAN register. This makes
     * the feature nearly useless for us.
     */

    CSR_WRITE_4(pDev, ETSEC_TCTRL, ETSEC_TCTRL_TUCSEN|ETSEC_TCTRL_IPCSEN);

    /*
     * Initialize receive control. We enable IP and TCP/UDP
     * checksum offload, the parser, the extended multicast hash table,
     * and the additional exact match filter entries (CAM1 - CAM15).
     * Note: to work around erratum eTSEC 8, the RSF bit must be set.
     */

    CSR_WRITE_4(pDev, ETSEC_RCTRL, ETSEC_RXALIGN(ETSEC_ALIGN)|
        ETSEC_RCTRL_GHTX|ETSEC_PARSER_L2_L3_L4|ETSEC_RCTRL_TUCSEN|
        ETSEC_RCTRL_IPCSEN|ETSEC_RCTRL_EMEN|ETSEC_RCTRL_RSF);

    /*
     * If the filer is enabled, turn on all 8 RX queues, and program
     * the filer table with some example rules.
     */

    if (pDrvCtrl->etsecFiler == TRUE)
        {
        CSR_WRITE_4(pDev, ETSEC_RQUEUE, ETSEC_RQUEUE_EXT0|ETSEC_RQUEUE_EN0);
        CSR_SETBIT_4(pDev, ETSEC_RQUEUE, ETSEC_RQUEUE_EXT1|ETSEC_RQUEUE_EN1);
        CSR_SETBIT_4(pDev, ETSEC_RQUEUE, ETSEC_RQUEUE_EXT2|ETSEC_RQUEUE_EN2);
        CSR_SETBIT_4(pDev, ETSEC_RQUEUE, ETSEC_RQUEUE_EXT3|ETSEC_RQUEUE_EN3);
        CSR_SETBIT_4(pDev, ETSEC_RQUEUE, ETSEC_RQUEUE_EXT4|ETSEC_RQUEUE_EN4);
        CSR_SETBIT_4(pDev, ETSEC_RQUEUE, ETSEC_RQUEUE_EXT5|ETSEC_RQUEUE_EN5);
        CSR_SETBIT_4(pDev, ETSEC_RQUEUE, ETSEC_RQUEUE_EXT6|ETSEC_RQUEUE_EN6);
        CSR_SETBIT_4(pDev, ETSEC_RQUEUE, ETSEC_RQUEUE_EXT7|ETSEC_RQUEUE_EN7);
        CSR_SETBIT_4(pDev, ETSEC_RCTRL, ETSEC_RCTRL_FILREN);
        etsecFilerLoad (pDev);
        }
    else
        CSR_WRITE_4(pDev, ETSEC_RQUEUE, ETSEC_RQUEUE_EXT0|ETSEC_RQUEUE_EN0);

    /* Enable and zero stats counters */

    CSR_WRITE_4(pDev, ETSEC_ECNTRL, ETSEC_ECNTRL_CLRCNT|
        ETSEC_ECNTRL_AUTOZ|ETSEC_ECNTRL_STEN);

    /* Set the WOP bit so we can manually trigger transmissions. */

    CSR_SETBIT_4(pDev, ETSEC_DMACTRL, ETSEC_DMACTRL_WOP);

    /* Set receive thresholds */

    CSR_WRITE_4(pDev, ETSEC_FIFO_RX_ALARM, 8);
    CSR_WRITE_4(pDev, ETSEC_FIFO_RX_ALARM_SHUT, 128);

    /* Set transmit thresholds */

    CSR_WRITE_4(pDev, ETSEC_FIFO_TX_THR, 32);
    CSR_WRITE_4(pDev, ETSEC_FIFO_STRV, 16);
    CSR_WRITE_4(pDev, ETSEC_FIFO_STRV_SHUT, 32);

    /*
     * Note: to work around erratum eTSEC 12, we have to make sure
     * the 'excess deferral retry' bit in the half duplex control
     * register is set. (This avoids a locking in 10/100 half duplex
     * mode.)
     */

    CSR_SETBIT_4(pDev, ETSEC_HALFDUP, ETSEC_HALFDUP_EXCESS_DEF);

    /*
     * The following two settings enable L2 'extraction,'
     * which allows us to place a portion of each received
     * frame into the L2 cache. We configure the device
     * to extract the first 128 bytes of frame data. This
     * should put the IP header into the cache, which can
     * improve IP forwarding performance.
     *
     * Note that the extraction feature is not supported
     * on the 83xx processors (which have no L2 cache).
     * On these devices, the ATTRELI register has no effect,
     * and only the RX snooping bits in the ATTR register
     * have meaning.
     *
     * Note II: according to Freescale, the ETSEC controllers
     * on the MPC8641D are not supposed to support L2 cache
     * extraction (even though the manual says otherwise) and
     * recommends leaving it disabled. We use the "l2disable"
     * property here to determine whether or not to leave it
     * off.
     */

    CSR_CLRBIT_4(pDev, ETSEC_ATTR, ETSEC_ATTR_ELCWT|ETSEC_ATTR_BDLWT);

    /*
     * paramDesc {
     * The l2disable parameter specifies whether L2 cache
     * extraction should be disabled. }
     */
    if (vxbInstParamByNameGet (pDev, "l2disable",
        VXB_PARAM_INT32, &val) != OK || val.int32Val == 0)
        {
        CSR_SETBIT_4(pDev, ETSEC_ATTR, ETSEC_ELCWT_L2|ETSEC_BDLWT_L2);
        CSR_WRITE_4(pDev, ETSEC_ATTRELI, ETSEC_ATTRELI_EL(128) |
            ETSEC_ATTRELI_EI(0));
        }

    /* Enable padding of short frames and CRC generation */

    CSR_SETBIT_4(pDev, ETSEC_MACCFG2, ETSEC_MACCFG2_PADCRC);

    /* Enable RX frame length checking. */

    CSR_SETBIT_4(pDev, ETSEC_MACCFG2, ETSEC_MACCFG2_LENGTH_CHECK);

    /* Set up interrupt coalescing, if enabled. */

    if (coalEnable)
        {
        /*
         * paramDesc {
         * The coalesceRxTicks parameter specifies
         * the timeout, in ticks, required to force
         * raising the receive interrupt, if the
         * packet threshold has not been reached. }
         */
        vxbInstParamByNameGet (pDev, "coalesceRxTicks", VXB_PARAM_INT32, &val);
        coalTicks = val.int32Val;
        /*
         * paramDesc {
         * The coalesceRxPkts parameter specifies
         * the minimum number of packets required
         * to raise the receive interrupt. }
         */
        vxbInstParamByNameGet (pDev, "coalesceRxPkts", VXB_PARAM_INT32, &val);
        coalPkts = val.int32Val;

        CSR_WRITE_4(pDev, ETSEC_RXIC, ETSEC_RXIC_ICEN |
            ETSEC_COAL_FRAMES(coalPkts) | ETSEC_COAL_TIMER(coalTicks));

        /*
         * paramDesc {
         * The coalesceTxTicks parameter specifies
         * the timeout, in ticks, required to force
         * raising the send interrupt, if the
         * packet threshold has not been reached. }
         */
        vxbInstParamByNameGet (pDev, "coalesceTxTicks", VXB_PARAM_INT32, &val);
        coalTicks = val.int32Val;
        /*
         * paramDesc {
         * The coalesceTxPkts parameter specifies
         * the minimum number of packets required
         * to raise the send interrupt. }
         */
        vxbInstParamByNameGet (pDev, "coalesceTxPkts", VXB_PARAM_INT32, &val);
        coalPkts = val.int32Val;

        CSR_WRITE_4(pDev, ETSEC_TXIC, ETSEC_TXIC_ICEN |
            ETSEC_COAL_FRAMES(coalPkts) | ETSEC_COAL_TIMER(coalTicks));
        }
    else
        {
        CSR_WRITE_4(pDev, ETSEC_RXIC, 0);
        CSR_WRITE_4(pDev, ETSEC_TXIC, 0);
        }

    /* Program the RX filter. */

    etsecEndRxConfig (pDrvCtrl);

    /* Enable interrupts */

    vxbIntEnable (pDev, 0, etsecEndInt, pDrvCtrl);
    vxbIntEnable (pDev, 1, etsecEndInt, pDrvCtrl);
    vxbIntEnable (pDev, 2, etsecEndInt, pDrvCtrl);

    CSR_WRITE_4(pDev, ETSEC_EDIS, ~ETSEC_IEVENT_XFUN);
    pDrvCtrl->etsecIntrs = ETSEC_INTRS;
    CSR_WRITE_4(pDev, ETSEC_IMASK, ETSEC_INTRS);

    /* Enable RX and TX */

    CSR_CLRBIT_4(pDev, ETSEC_DMACTRL, ETSEC_DMACTRL_GTS|ETSEC_DMACTRL_GRS);
    CSR_WRITE_4(pDev, ETSEC_MACCFG1, ETSEC_MACCFG1_TX_EN|ETSEC_MACCFG1_RX_EN);


    /*notice dongyang 此处修改同上方notice*/
    vxbDelay ();
/*
#if defined(VPX6_HSP9_1_PROJECT)
    vxbDelay ();
#else
    vxbDelay (20);
#endif
  */  
    
    
    /* Set initial link state */

    pDrvCtrl->etsecCurMedia = IFM_ETHER|IFM_NONE;
    pDrvCtrl->etsecCurStatus = IFM_AVALID;

    miiBusModeSet (pDrvCtrl->etsecMiiBus,
        pDrvCtrl->etsecMediaList->endMediaListDefault);

    END_FLAGS_SET (pEnd, (IFF_UP | IFF_RUNNING));

    END_TX_SEM_GIVE (pEnd);
    semGive (pDrvCtrl->etsecDevSem);

    return (OK);
    }

/*****************************************************************************
*
* etsecEndStop - stop the device
*
* This function undoes the effects of etsecEndStart(). The device is shut
* down and all resources are released. Note that the shutdown process
* pauses to wait for all pending RX, TX and link event jobs that may have
* been initiated by the interrupt handler to complete. This is done
* to prevent tNetTask from accessing any data that might be released by
* this routine.
*
* RETURNS: ERROR if device shutdown failed, otherwise OK
*
* ERRNO: N/A
*/

LOCAL STATUS etsecEndStop
    (
    END_OBJ * pEnd
    )
    {
    ETSEC_DRV_CTRL * pDrvCtrl;
    VXB_DEVICE_ID pDev;
    UINT32 tmp;
    int i, q;

    pDrvCtrl = (ETSEC_DRV_CTRL *)pEnd;
    pDev = pDrvCtrl->etsecDev;

    semTake (pDrvCtrl->etsecDevSem, WAIT_FOREVER);

    if (!(pEnd->flags & IFF_UP))
        {
        semGive (pDrvCtrl->etsecDevSem);
        return (OK);
        }

    END_FLAGS_CLR (pEnd, (IFF_UP | IFF_RUNNING));

    /* Disable interrupts */

    pDrvCtrl->etsecIntrs = 0;
    CSR_WRITE_4(pDev, ETSEC_IMASK, 0);

    vxbIntDisable (pDev, 0, etsecEndInt, pDrvCtrl);
    vxbIntDisable (pDev, 1, etsecEndInt, pDrvCtrl);
    vxbIntDisable (pDev, 2, etsecEndInt, pDrvCtrl);

    /*
     * Wait for all jobs to drain.
     * Note: this must be done before we disable the receiver
     * and transmitter below. If someone tries to reboot us via
     * WDB, this routine may be invoked while the RX handler is
     * still running in tNetTask. If we disable the chip while
     * that function is running, it'll start reading inconsistent
     * status from the chip. We have to wait for that job to
     * terminate first, then we can disable the receiver and
     * transmitter
     */

    for (i = 0; i < ETSEC_TIMEOUT; i++)
        {
        if (vxAtomicGet (&pDrvCtrl->etsecIntPending) == FALSE)
            break;
        taskDelay(1);
        }

    if (i == ETSEC_TIMEOUT)
        logMsg("%s%d: timed out waiting for job to complete\n",
            (int)ETSEC_NAME, pDev->unitNumber, 0, 0, 0, 0);

    END_TX_SEM_TAKE (pEnd, WAIT_FOREVER);

    /* Perform graceful stop of RX and TX DMA engines. */

    tmp = CSR_READ_4(pDev, ETSEC_DMACTRL);

    if ((tmp & (ETSEC_DMACTRL_GRS|ETSEC_DMACTRL_GTS)) !=
        (ETSEC_DMACTRL_GRS|ETSEC_DMACTRL_GTS))
        {
        tmp |= ETSEC_DMACTRL_GRS|ETSEC_DMACTRL_GTS;
        CSR_WRITE_4(pDev, ETSEC_DMACTRL, tmp);

        for (i = 0; i < ETSEC_TIMEOUT; i++)
            {
            tmp = CSR_READ_4(pDev, ETSEC_IEVENT);
            if ((tmp & (ETSEC_IEVENT_GTSC|ETSEC_IEVENT_GRSC)) ==
                (ETSEC_IEVENT_GTSC|ETSEC_IEVENT_GRSC))
                break;
            taskDelay (1);
            }

        CSR_WRITE_4(pDev, ETSEC_IEVENT, ETSEC_IEVENT_GTSC|ETSEC_IEVENT_GRSC);

        if (i == ETSEC_TIMEOUT)
            logMsg("%s%d: timed out waiting graceful stop\n",
                (int)ETSEC_NAME, pDev->unitNumber, 0, 0, 0, 0);
        }

    /* Disable RX and TX. */

    CSR_WRITE_4(pDev, ETSEC_MACCFG1, 0);

    /* Release resources */

    for (q = 0; q < ETSEC_MAX_RX_QUEUES; q++)
        {
        pDrvCtrl->etsecRxMblk = pDrvCtrl->etsecRxMblks[q];
        for (i = 0; i < ETSEC_RX_DESC_CNT; i++)
            {
            if (pDrvCtrl->etsecRxMblk[i] != NULL)
                {
                endPoolTupleFree (pDrvCtrl->etsecRxMblk[i]);
                pDrvCtrl->etsecRxMblk[i] = NULL;
                }
            }
        }

    /*
     * Flush the recycle cache to shake loose any of our
     * mBlks that may be stored there.
     */

    endMcacheFlush ();

    for (i = 0; i < ETSEC_TX_DESC_CNT; i++)
        {
        if (pDrvCtrl->etsecTxMblk[i] != NULL)
            {
            endPoolTupleFree (pDrvCtrl->etsecTxMblk[i]);
            pDrvCtrl->etsecTxMblk[i] = NULL;
            }
        }

    END_TX_SEM_GIVE (pEnd);
    semGive (pDrvCtrl->etsecDevSem);

    return (OK);
    }

/*****************************************************************************
*
* etsecEndErrHandle - task level interrupt handler
*
* This routine is scheduled to run in tNetTask by the interrupt service
* routine whenever a chip interrupt occurs. This function will check
* what interrupt events are pending and schedule additional jobs to
* service them. Once there are no more events waiting to be serviced
* (i.e. the chip has gone idle), interrupts will be unmasked so that
* the ISR can fire again.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void etsecEndErrHandle
    (
    void * pArg,
    UINT32 status
    )
    {
    ETSEC_DRV_CTRL *pDrvCtrl;
    VXB_DEVICE_ID pDev;
    int thrTab[] = { 32, 64, 128, 256, 511, 0 };
    int i = 0;
    UINT32 curThresh;
 
    pDrvCtrl = pArg;
    pDev = pDrvCtrl->etsecDev;

    /*
     * Transmit underrun detected: increase the TX start threshold
     * to compensate.
     */

    if (status & ETSEC_IEVENT_XFUN)
        {
        curThresh = CSR_READ_4(pDev, ETSEC_FIFO_TX_THR);

        while (thrTab[i])
            {
            if (curThresh < thrTab[i])
                break;
            i++;
            }

        if (thrTab[i])
            CSR_WRITE_4(pDev, ETSEC_FIFO_TX_THR, thrTab[i]);
        }

    return;
    }

/*****************************************************************************
*
* etsecEndInt - handle device TX interrupts
*
* This function is invoked whenever one of the ETSEC's interrupt
* lines is asserted.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void etsecEndInt
    (
    ETSEC_DRV_CTRL * pDrvCtrl
    )
    {
    VXB_DEVICE_ID pDev;

    pDev = pDrvCtrl->etsecDev;

    /*
     * Note: we don't bother to test if there are interrupts
     * pending here. The TSEC's interrupt vectors are not
     * shared with any other device, so if this ISR was
     * called, we know there's an event pending for us.
     */

    if (vxCas (&pDrvCtrl->etsecIntPending, FALSE, TRUE))
        {
        /* mask interrupts here */
        CSR_WRITE_4(pDev, ETSEC_IMASK, 0);
        pDrvCtrl->etsecRxTxEvents = CSR_READ_4(pDev, ETSEC_IEVENT);
        CSR_WRITE_4(pDev, ETSEC_IEVENT, pDrvCtrl->etsecRxTxEvents);
        jobQueuePost (pDrvCtrl->etsecJobQueue, &pDrvCtrl->etsecIntJob);
        }

    return;
    }

/*****************************************************************************
*
* etsecEndRxDemux - demultiplex RX events from multiple queues
*
* This routine operates as the RX handler entry point when the ETSEC
* RX frame filer is enabled. In filer mode, the driver initializes all
* 8 RX DMA queues, and we must process RX events on all of them. Whenever
* a frame is routed to a particular queue, a corresponding bit will be
* set in the RSTAT register. We check each bit in the RSTAT register
* in sequence, starting from queue 0, and if there's traffic pending,
* we invoke the RX handler to process it. We consider queue 0 to be
* the highest priority queue and queue 7 the lowest; frames on queue 0
* will therefore be processed and sent to the stack first.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void etsecEndRxDemux
    (
    ETSEC_DRV_CTRL *pDrvCtrl
    )
    {
    VXB_DEVICE_ID pDev;
    ETSEC_DESC * pDesc;
    int q;
    BOOL reQueue = FALSE;

    pDev = pDrvCtrl->etsecDev;

    /*
     * Note: as a workaround for erratum eTSEC 38, we have to turn off
     * the WWR bit in the DMACTRL register. However, setting WWR to 0
     * can cause erratum eTSEC 46 to occur: when multiple RX queues
     * are used, RXF0 in the RSTAT register may sometimes be set even
     * though the RX event actually occured in another ring. This makes
     * the RSTAT register unreliable. To work around this, we avoid using
     * RSTAT, and instead test the first descriptor in each ring to see
     * if it has any traffic pending.
     */

    for (q = 0; q < ETSEC_MAX_RX_QUEUES; q++)
        {
        pDrvCtrl->etsecRxDesc = pDrvCtrl->etsecRxQueue[q];
        pDrvCtrl->etsecRxIdx = &pDrvCtrl->etsecRxIdxs[q];

        pDesc = &pDrvCtrl->etsecRxDesc[*pDrvCtrl->etsecRxIdx];

        if (!(pDesc->bdSts & ETSEC_RBD_E))
            {
            pDrvCtrl->etsecRxMblk = pDrvCtrl->etsecRxMblks[q];
            etsecEndRxHandle (pDrvCtrl);
            CSR_WRITE_4(pDev, ETSEC_RSTAT, (ETSEC_RSTAT_RXF0 >> q));
            }

        /*
         * Workaround for erratum eTSEC 38: ensure that we don't miss any
         * RX frames when WWR is 0.
         */

        if (CSR_READ_4(pDev, ETSEC_RBPTR0 + (q * 8)) !=
            ETSEC_PHYSADDR(&pDrvCtrl->etsecRxDesc[*pDrvCtrl->etsecRxIdx]))
            reQueue = TRUE;
        }

    if (reQueue == TRUE)
	{
	pDrvCtrl->etsecRxTxEvents |= ETSEC_RXINTRS;
	}
    else
        {
        /* Resume the RX channels, if needed */

        CSR_WRITE_4 (pDev, ETSEC_RSTAT, ETSEC_RSTAT_QHLT0|ETSEC_RSTAT_QHLT1|
            ETSEC_RSTAT_QHLT2|ETSEC_RSTAT_QHLT3|ETSEC_RSTAT_QHLT4|
            ETSEC_RSTAT_QHLT5|ETSEC_RSTAT_QHLT6|ETSEC_RSTAT_QHLT7);
        }

    return;
    }

/******************************************************************************
*
* etsecEndRxHandle - process received frames
*
* This function called by etsecEndIntHandle() (or by etsecEndRxDemux())
* in the context of tNet0 whenever RX handling needs to be done. It processes
* packets from the RX window and encapsulates them into mBlk tuples
* which are handed up to the MUX.
*
* There may be several packets waiting in the window to be processed.
* We take care not to process too many packets in a single run through
* this function so as not to monopolize tNet0 and starve out other
* jobs waiting in the jobQueue. If we detect that there's still more
* packets waiting to be processed, we queue ourselves up for another
* round of processing.
*
* This function limits itself to processing at most ETSEC_MAX_RX packets.
* If that limit is reached, or the function otherwise determines that there
* are likely to be more packets received, it ORs ETSEC_RXINTRS into
* pDrvCtrl->etsecRxTxEvents to signal that the function should be called
* again in a new network job.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void etsecEndRxHandle
    (
    ETSEC_DRV_CTRL *pDrvCtrl
    )
    {
    VXB_DEVICE_ID pDev;
    M_BLK_ID pMblk;
    M_BLK_ID pNewMblk;
    ETSEC_DESC * pDesc;
    ETSEC_RX_FCB * pFcb;
    int loopCounter = ETSEC_MAX_RX;

    pDev = pDrvCtrl->etsecDev;

    pDesc = &pDrvCtrl->etsecRxDesc[*pDrvCtrl->etsecRxIdx];

    while (loopCounter && !(pDesc->bdSts & ETSEC_RBD_E))
        {

        if (pDesc->bdSts & (ETSEC_RBD_LG|ETSEC_RBD_NO|
            ETSEC_RBD_SH|ETSEC_RBD_CR|ETSEC_RBD_OV))
            {
            /*pDrvCtrl->etsecInErrors++;
            logMsg("%s%d: bad packet, sts: %x (%p %d)\n", (int)ETSEC_NAME,
                pDev->unitNumber, pDesc->bdSts, (int)pDesc,
                *pDrvCtrl->etsecRxIdx, 0);*/
            goto skip;
            }

        /*
         * Try to conjure up a new mBlk tuple to hold the incoming
         * packet. If this fails, we have to skip the packet and move
         * to the next one.
         */

        pNewMblk = endPoolTupleGet(pDrvCtrl->etsecEndObj.pNetPool);
        if (pNewMblk == NULL)
            {
            /*pDrvCtrl->etsecInDiscards++;*/
            logMsg("%s%d: out of mBlks at %d\n", (int)ETSEC_NAME,
                pDev->unitNumber, *pDrvCtrl->etsecRxIdx, 0, 0, 0);
            pDrvCtrl->etsecLastError.errCode = END_ERR_NO_BUF;
            muxError (&pDrvCtrl->etsecEndObj, &pDrvCtrl->etsecLastError);
skip:
            if (*pDrvCtrl->etsecRxIdx & 0x3)
                pDesc->bdSts |= ETSEC_RBD_E;
            if ((*pDrvCtrl->etsecRxIdx & 0x3) == 0x3)
                {
                pDesc -= 3;
                pDesc->bdSts |= ETSEC_RBD_E;
                }
            ETSEC_INC_DESC(*pDrvCtrl->etsecRxIdx, ETSEC_RX_DESC_CNT);
            loopCounter--;
            pDesc = &pDrvCtrl->etsecRxDesc[*pDrvCtrl->etsecRxIdx];
            continue;
            }

        /* Swap the mBlks. */

        pMblk = pDrvCtrl->etsecRxMblk[*pDrvCtrl->etsecRxIdx];
        pDrvCtrl->etsecRxMblk[*pDrvCtrl->etsecRxIdx] = pNewMblk;

        /* Set the mBlk header up with the frame length. */

        pMblk->m_len = pMblk->m_pkthdr.len = pDesc->bdLen - ETSEC_RX_ADJ;
        pMblk->m_flags = M_PKTHDR|M_EXT;

        pFcb = (ETSEC_RX_FCB *)pMblk->m_data;
        pMblk->m_data += sizeof (ETSEC_RX_FCB) + ETSEC_ALIGN;

        /* Handle checksum offload. */

        if (pDrvCtrl->etsecCaps.cap_enabled & IFCAP_RXCSUM)
            {
            if (pFcb->fcbFlags & ETSEC_RX_FCB_CIP)
                pMblk->m_pkthdr.csum_flags |= CSUM_IP_CHECKED;
            if (!(pFcb->fcbFlags & ETSEC_RX_FCB_EIP))
                pMblk->m_pkthdr.csum_flags |= CSUM_IP_VALID;
            if (pFcb->fcbFlags & ETSEC_RX_FCB_CTU &&
                !(pFcb->fcbFlags & ETSEC_RX_FCB_ETU))
                {
                pMblk->m_pkthdr.csum_flags |= CSUM_DATA_VALID|CSUM_PSEUDO_HDR;
                pMblk->m_pkthdr.csum_data = 0xffff;
                }
            }

        /*
         * Reset this descriptor's CMD/STS fields.
         * Note: to work around erratum eTSEC 2, RX descriptors must
         * be returned to the chip in groups of 4. We achieve this
         * by not setting the 'empty' bit in the first descriptor in
         * every group of four until the fourth one has been been
         * processed.
         */

        pDesc->bdAddr = ETSEC_PHYSADDR(mtod(pNewMblk, UINT32));

        if (*pDrvCtrl->etsecRxIdx & 0x3)
            pDesc->bdSts |= ETSEC_RBD_E;

        if ((*pDrvCtrl->etsecRxIdx & 0x3) == 0x3)
            {
            pDesc -= 3;
            pDesc->bdSts |= ETSEC_RBD_E;
            }

        /* Advance to the next descriptor */

        ETSEC_INC_DESC(*pDrvCtrl->etsecRxIdx, ETSEC_RX_DESC_CNT);
        loopCounter--;

        /* Give the frame to the stack. */

        END_RCV_RTN_CALL (&pDrvCtrl->etsecEndObj, pMblk);

        pDesc = &pDrvCtrl->etsecRxDesc[*pDrvCtrl->etsecRxIdx];
        }

    if (loopCounter == 0)
	{
	pDrvCtrl->etsecRxTxEvents |= ETSEC_RXINTRS;
	}
    /*
     * Note: as a workaround for erratum eTSEC 38, we must leave the
     * WWR bit in the DMACTRL register turned off. But doing this
     * opens up the possibility that we may receive an RX interrupt
     * before the update to the current RX descriptor is written out
     * to memory. This could cause us to miss a packet: if we get
     * to the descriptor before hardware does, it will appear to
     * still be empty. To avoid this problem, we test the RBPTR
     * register to confirm that there are actually descriptors
     * outstanding.
     */
    else if (!pDrvCtrl->etsecFiler &&
	     CSR_READ_4(pDev, ETSEC_RBPTR) !=
	     ETSEC_PHYSADDR(&pDrvCtrl->etsecRxDesc[*pDrvCtrl->etsecRxIdx]))
	{
	pDrvCtrl->etsecRxTxEvents |= ETSEC_RXINTRS;
	}

    return;
    }

/******************************************************************************
*
* etsecEndTxHandle - process TX completion events
*
* This function is called by etsecEndIntHandle() in the context of tNet0
* whenever TX completion handling needs to be done. It runs through
* the outstanding TX descriptors and checks the ready bit of each to see if
* it has completed. For each completed transmission, the associated TX mBlk
* (if any) is released.
*
* If the transmitter has stalled, this routine will also call muxTxRestart()
* to drain any packets that may be waiting in the protocol send queues,
*
* If this function decides that it needs to be run again in a new
* network job, it ORs ETSEC_TXINTRS into pDrvCtrl->etsecRxTxEvents.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void etsecEndTxHandle
    (
    ETSEC_DRV_CTRL *pDrvCtrl
    )
    {
    VXB_DEVICE_ID pDev;
    ETSEC_DESC * pDesc;
    M_BLK_ID pMblk;
    BOOL restart = FALSE;
    int txFree = 0;

    pDev = pDrvCtrl->etsecDev;

    END_TX_SEM_TAKE (&pDrvCtrl->etsecEndObj, WAIT_FOREVER); 

    while (pDrvCtrl->etsecTxFree < ETSEC_TX_DESC_CNT)
        {
        pDesc = &pDrvCtrl->etsecTxDesc[pDrvCtrl->etsecTxCons];

        if (pDesc->bdSts & ETSEC_TBD_R)
            break;

        /*
         * Note: as a workaround for erratum eTSEC 38, we must leave the
         * WWR bit in the DMACTRL register turned off. But doing this
         * opens up the possibility that we may receive a TX interrupt
         * before the update to the current TX descriptor is written out
         * to memory. This could cause us to miss a packet: if we get
         * to the descriptor before hardware does, it will appear to
         * still be busy. To avoid this problem, once the TX handler has
         * been scheduled, we keep rescheduling it until at least one
         * descriptor has been processed.
         */

        txFree++;

        /*
         * Some TX error conditions will stall the transmitter.
         * We need to restart it here.
         */
        if (pDesc->bdSts & (ETSEC_TBD_RL|ETSEC_TBD_HFE_LC|ETSEC_TBD_UN))
            CSR_WRITE_4(pDev, ETSEC_TSTAT, ETSEC_TSTAT_THLT);

        pMblk = pDrvCtrl->etsecTxMblk[pDrvCtrl->etsecTxCons];

        if (pMblk != NULL)
            {
            endPoolTupleFree (pMblk);
            pDrvCtrl->etsecTxMblk[pDrvCtrl->etsecTxCons] = NULL;
            }

        pDesc->bdSts &= ETSEC_TBD_W;

        pDrvCtrl->etsecTxFree++;

        ETSEC_INC_DESC (pDrvCtrl->etsecTxCons, ETSEC_TX_DESC_CNT);

        /*
         * We released at least one descriptor: if the transmit
         * channel is stalled, unstall it.
         */

        if (pDrvCtrl->etsecTxStall == TRUE)
            {
            pDrvCtrl->etsecTxStall = FALSE;
            restart = TRUE;
            }

        }

    END_TX_SEM_GIVE (&pDrvCtrl->etsecEndObj); 

    if (restart == TRUE)
        muxTxRestart (pDrvCtrl);

    /*
     * If there are outstanding TX descriptors and none were freed this
     * time, repost this job.
     */
    if (txFree == 0 && pDrvCtrl->etsecTxFree != ETSEC_TX_DESC_CNT)
	pDrvCtrl->etsecRxTxEvents |= ETSEC_TXINTRS;

    return;
    }

/******************************************************************************
*
* etsecEndIntHandle - handle RX packets and TX completions
*
* This function is scheduled as a network job whenever an RX or TX
* interrupt is received, if the job has not already been scheduled.
* This function calls the RX and TX handler functions as appropriate, and
* reschedules itself to run again if either handler requested rescheduling,
* or if more RX or TX events occurred since the IEVENT register was originally
* read.
*
* When this function does not reschedule itself, it reenables
* device RX and TX interrupts before returning.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void etsecEndIntHandle
    (
    void * pArg
    )
    {
    QJOB * pJob;
    ETSEC_DRV_CTRL *pDrvCtrl;
    VXB_DEVICE_ID pDev;
    atomicVal_t events;

    pJob = pArg;
    pDrvCtrl = member_to_object (pJob, ETSEC_DRV_CTRL, etsecIntJob);
    pDev = pDrvCtrl->etsecDev;

    /*
     * Read the events that we should handle from both the IEVENT
     * register and the etsecRxTxEvents flags. Clear the events
     * that we will handle.
     */
    events = CSR_READ_4(pDev, ETSEC_IEVENT);
    events |= pDrvCtrl->etsecRxTxEvents;
    pDrvCtrl->etsecRxTxEvents = 0;
    CSR_WRITE_4(pDev, ETSEC_IEVENT, events);

    /*
     * The RX and TX handler functions may set bits
     * in pDrvCtrl->etsecRxTxEvents if they want this job to be
     * rescheduled.
     */
    if (events & ETSEC_RXINTRS)
	{
	if (pDrvCtrl->etsecFiler)
	    etsecEndRxDemux (pDrvCtrl);
	else
	    {
	    etsecEndRxHandle (pDrvCtrl);
	    /* Resume the RX channel, if we are not reposting the job for RX. */
	    if (pDrvCtrl->etsecRxTxEvents == 0)
		CSR_WRITE_4 (pDev, ETSEC_RSTAT, ETSEC_RSTAT_QHLT);
	    }
	}

    if (events & ETSEC_TXINTRS)
	etsecEndTxHandle (pDrvCtrl);

    if (events & ETSEC_ERRINTRS)
        etsecEndErrHandle (pDrvCtrl, events);

    if (pDrvCtrl->etsecRxTxEvents != 0 ||
	(CSR_READ_4(pDev, ETSEC_IEVENT) & ETSEC_INTRS) != 0)
	{
        jobQueuePost (pDrvCtrl->etsecJobQueue, &pDrvCtrl->etsecIntJob);
	return;
	}

    vxAtomicSet (&pDrvCtrl->etsecIntPending, FALSE);

    /* Unmask interrupts. */

    CSR_SETBIT_4(pDev, ETSEC_IMASK, pDrvCtrl->etsecIntrs);

    return;
    }

/******************************************************************************
*
* etsecEndEncap - encapsulate an outbound packet in the TX DMA ring
*
* This function sets up a descriptor for a packet transmit operation.
* With the ETSEC ethrnet controller, the TX DMA ring consists of
* descriptors that each describe a single packet fragment. We consume
* as many descriptors as there are mBlks in the outgoing packet, unless
* the chain is too long. The length is limited by the number of DMA
* segments we want to allow in a given DMA map. If there are too many
* segments, this routine will fail, and the caller must coalesce the
* data into fewer buffers and try again.
*
* This routine will also fail if there aren't enough free descriptors
* available in the ring, in which case the caller must defer the
* transmission until more descriptors are completed by the chip.
*
* RETURNS: ENOSPC if there are too many fragments in the packet, EAGAIN
* if the DMA ring is full, otherwise OK.
*
* ERRNO: N/A
*/

LOCAL int etsecEndEncap
    (
    ETSEC_DRV_CTRL * pDrvCtrl,
    M_BLK_ID pMblk
    )
    {
    ETSEC_DESC * pDesc = NULL, * pFirst;
    ETSEC_TX_FCB * pFcb;
    M_BLK_ID pCurr, pStart;
    UINT32 firstIdx, lastIdx = 0;
    struct ether_header * eh;
    int used = 0, i;

    firstIdx = pDrvCtrl->etsecTxProd;

    pFirst = &pDrvCtrl->etsecTxDesc[pDrvCtrl->etsecTxProd];

    /*
     * Handle TX checksum offload. Offload processing is controlled
     * using a frame control block, which must reside in the first 8
     * bytes of the first buffer in a TX DMA transfer. The assumption
     * is that outbound frames will have at least 8 bytes of leading
     * space that we can use. Unfortunately, this assumption is not
     * always true, so we have to handle two cases: the case where
     * we have leading space, and the case where we don't. When we
     * do have space, we simply insert the FCB at the head of the
     * first buffer and then fall through the code below to traverse
     * the remaining mBlks in the chain. In the case where we don't
     * have sufficient space, we add an extra fragment to the DMA,
     * using a spare buffer of our own. DMAing such a small buffer
     * is not a very efficient use of bandwidth, but it is legal
     * according to the hardware spec, and it's really the only
     * choice we have (aside from recopying the packet). Fortunately,
     * the latter case does not occur very frequently in normal usage.
     *
     * Note: to work around erratum eTSEC 23, TX FCBs must always be
     * at least 16 byte aligned.
     */

    if (pMblk->m_pkthdr.csum_flags & (CSUM_IP|CSUM_UDP|
        CSUM_TCP|CSUM_UDPv6|CSUM_TCPv6))
        {
        pDesc = &pDrvCtrl->etsecTxDesc[pDrvCtrl->etsecTxProd];
        if (ETSEC_LEADINGSPACE(pMblk) < sizeof(ETSEC_TX_FCB) ||
            mtod(pMblk, UINT32) & 0xF)
            {
            pFcb = pDrvCtrl->etsecTxFcb[pDrvCtrl->etsecTxProd];
            pStart = pMblk;
            pDesc->bdLen = sizeof(ETSEC_TX_FCB);
            }
        else
            {
            pStart = pMblk->m_next;
            pDesc->bdLen = pMblk->m_len + sizeof(ETSEC_TX_FCB);
            pFcb = (ETSEC_TX_FCB *)(mtod(pMblk, char *) - sizeof(ETSEC_TX_FCB));
            }
        pDesc->bdAddr = ETSEC_PHYSADDR(pFcb);
        pDesc->bdSts |= ETSEC_TBD_TOE;
        pFcb->fcbFlags = ETSEC_TX_FCB_IP|ETSEC_TX_FCB_NPH;
        if (pMblk->m_pkthdr.csum_flags & CSUM_IP)
            pFcb->fcbFlags |= ETSEC_TX_FCB_CIP;
        eh = mtod(pMblk, struct ether_header *);
        i = SIZEOF_ETHERHEADER;
        if (eh->ether_type == htons(ETHERTYPE_VLAN))
            i += 4;
	pFcb->fcbl3os = i;
        i += (pFcb->fcbl4os = CSUM_IP_HDRLEN(pMblk));
	/*
	 * Since the etsec doesn't handle IP options or extension headers,
	 * but the stack may send us packets with IP options or extension
	 * headers, we must set the NPH bit and extract the pseudo-header
	 * checksum from the packet into the FCB.
	 */
        if (pMblk->m_pkthdr.csum_flags &
            (CSUM_TCP|CSUM_TCPv6|CSUM_UDP|CSUM_UDPv6))
	    {
            pFcb->fcbFlags |= ETSEC_TX_FCB_TUP|ETSEC_TX_FCB_CTU;
	    if (pMblk->m_pkthdr.csum_flags & (CSUM_TCPv6|CSUM_UDPv6))
		pFcb->fcbFlags |= ETSEC_TX_FCB_IP6;
	    if (pMblk->m_pkthdr.csum_flags & (CSUM_UDP|CSUM_UDPv6))
		pFcb->fcbFlags |= ETSEC_TX_FCB_UDP;
            i += CSUM_XPORT_CSUM_OFF(pMblk);
            pCurr = pMblk;
            while (i >= pCurr->m_len)
                {
                i -= pCurr->m_len;
                pCurr = pCurr->m_next;
                }
            /* Note: assumes big endian mode. */
            pFcb->fcbPhcs = *(UINT16 *)(pCurr->m_data + i);
            }
        lastIdx = pDrvCtrl->etsecTxProd;
        ETSEC_INC_DESC(pDrvCtrl->etsecTxProd, ETSEC_TX_DESC_CNT);
        used++;
        pDrvCtrl->etsecTxFree--;
        }
    else
        pStart = pMblk;

    for (pCurr = pStart; pCurr != NULL; pCurr = pCurr->m_next)
        {
        if (pCurr->m_len != 0)
            {
            if (used == ETSEC_MAXFRAG || pDrvCtrl->etsecTxFree == 0)
                break;

            pDesc = &pDrvCtrl->etsecTxDesc[pDrvCtrl->etsecTxProd];

            if (pDesc->bdSts & ETSEC_TBD_R)
                break;

            pDesc->bdLen = pCurr->m_len;
            pDesc->bdAddr = ETSEC_PHYSADDR(mtod(pCurr, UINT32));

            if (used)
                pDesc->bdSts |= ETSEC_TBD_R;

            lastIdx = pDrvCtrl->etsecTxProd;
            ETSEC_INC_DESC(pDrvCtrl->etsecTxProd, ETSEC_TX_DESC_CNT);
            used++;
            pDrvCtrl->etsecTxFree--;
            }
        }

    if (pCurr != NULL)
        goto noDescs;

    pDesc->bdSts |= ETSEC_TBD_I|ETSEC_TBD_L;

    /* Save the mBlk for later. */
    pDrvCtrl->etsecTxMblk[lastIdx] = pMblk;

    /*
     * If this is a jumbo frame, set the huge frame bit.
     * Supposedly, this is not necessary if the huge frame
     * bit is set in the MACCFG2 register, but testing has
     * shown this is not the case: the bit must be set here
     * in order for jumbo frames to work reliably.
     */
          
     if (pMblk->m_pkthdr.len > 1518)
         pFirst->bdSts |= ETSEC_TBD_HFE_LC;

    /* Transfer descriptors to the chip. */
    pFirst->bdSts |= ETSEC_TBD_R;

    return (OK);

noDescs:

    /*
     * Ran out of descriptors: undo all relevant changes
     * and fall back to copying.
     */

    pDrvCtrl->etsecTxProd = firstIdx;

    for (i = 0; i < used; i++)
        {
        pDrvCtrl->etsecTxFree++;
        pDesc = &pDrvCtrl->etsecTxDesc[pDrvCtrl->etsecTxProd];
        pDesc->bdLen = 0;
        pDesc->bdSts &= ETSEC_TBD_W;
        pDesc->bdAddr = (UINT32) NULL;
        ETSEC_INC_DESC(pDrvCtrl->etsecTxProd, ETSEC_TX_DESC_CNT);
        }

    pDrvCtrl->etsecTxProd = firstIdx;

    return (ENOSPC);
    }

/******************************************************************************
*
* etsecEndSend - transmit a packet
*
* This function transmits the packet specified in <pMblk>.
*
* RETURNS: OK, ERROR, or END_ERR_BLOCK.
*
* ERRNO: N/A
*/

LOCAL int etsecEndSend
    (
    END_OBJ * pEnd,
    M_BLK_ID pMblk
    )
    {
    ETSEC_DRV_CTRL * pDrvCtrl;
    VXB_DEVICE_ID pDev;
    M_BLK_ID pTmp;
    UINT32 rval;

    pDrvCtrl = (ETSEC_DRV_CTRL *)pEnd;

    pDev = pDrvCtrl->etsecDev;

    END_TX_SEM_TAKE (pEnd, WAIT_FOREVER); 

    if (!pDrvCtrl->etsecTxFree || !(pDrvCtrl->etsecCurStatus & IFM_ACTIVE))
        goto blocked;

    /*
     * First, try to do an in-place transmission, using
     * gather-write DMA.
     */

    rval = etsecEndEncap (pDrvCtrl, pMblk);

    /*
     * If etsecEndEncap() returns ENOSPC, it means it ran out
     * of TX descriptors and couldn't encapsulate the whole
     * packet fragment chain. In that case, we need to
     * coalesce everything into a single buffer and try
     * again. If any other error is returned, then something
     * went wrong, and we have to abort the transmission
     * entirely.
     */

    if (rval == ENOSPC)
        {
        if ((pTmp = endPoolTupleGet (pDrvCtrl->etsecEndObj.pNetPool)) == NULL)
            goto blocked;
        pTmp->m_len = pTmp->m_pkthdr.len =
            netMblkToBufCopy (pMblk, mtod(pTmp, char *), NULL);
        pTmp->m_flags = pMblk->m_flags;
        pTmp->m_pkthdr.csum_flags = pMblk->m_pkthdr.csum_flags;
        pTmp->m_pkthdr.csum_data = pMblk->m_pkthdr.csum_data;
        CSUM_IP_HDRLEN(pTmp) = CSUM_IP_HDRLEN(pMblk);
        /* Try transmission again, should succeed this time. */
        rval = etsecEndEncap (pDrvCtrl, pTmp);
        if (rval == OK)
            endPoolTupleFree (pMblk);
        else
            endPoolTupleFree (pTmp);
        }

    if (rval != OK)
        goto blocked;

    /* Issue transmit command */

    CSR_WRITE_4(pDev, ETSEC_TSTAT, ETSEC_TSTAT_THLT);

    END_TX_SEM_GIVE (pEnd); 
    return (OK);

blocked:

    pDrvCtrl->etsecTxStall = TRUE;
    END_TX_SEM_GIVE (pEnd); 

    return (END_ERR_BLOCK);
    }

/******************************************************************************
*
* etsecEndPollSend - polled mode transmit routine
*
* This function is similar to the etsecEndSend() routine shown above, except
* it performs transmissions synchronously with interrupts disabled. After
* the transmission is initiated, the routine will poll the state of the
* TX status register associated with the current slot until transmission
* completed. If transmission times out, this routine will return ERROR.
*
* RETURNS: OK, EAGAIN, or ERROR
*
* ERRNO: N/A
*/

LOCAL STATUS etsecEndPollSend
    (
    END_OBJ * pEnd,
    M_BLK_ID pMblk
    )
    {
    ETSEC_DRV_CTRL * pDrvCtrl;
    VXB_DEVICE_ID pDev;
    M_BLK_ID pTmp;
    ETSEC_DESC * pDesc;
    UINT32 status;
    UINT16 txStat = 0;
    int len, i;

    pDrvCtrl = (ETSEC_DRV_CTRL *)pEnd;

    if (pDrvCtrl->etsecPolling == FALSE)
        return (ERROR);

    pDev = pDrvCtrl->etsecDev;
    pTmp = pDrvCtrl->etsecPollBuf;

    pTmp->m_data = pTmp->m_extBuf + sizeof(ETSEC_TX_FCB);

    len = netMblkToBufCopy (pMblk, mtod(pTmp, char *), NULL);
    pTmp->m_len = pTmp->m_pkthdr.len = len;
    pTmp->m_flags = pMblk->m_flags;
    pTmp->m_pkthdr.csum_flags = pMblk->m_pkthdr.csum_flags;
    pTmp->m_pkthdr.csum_data = pMblk->m_pkthdr.csum_data;
    CSUM_IP_HDRLEN(pTmp) = CSUM_IP_HDRLEN(pMblk);

    pDesc = &pDrvCtrl->etsecTxDesc[pDrvCtrl->etsecTxCons];

    if (etsecEndEncap (pDrvCtrl, pTmp) != OK)
        return (EAGAIN);

    CSR_WRITE_4(pDev, ETSEC_TSTAT, ETSEC_TSTAT_THLT);

    for (i = 0; i < ETSEC_TIMEOUT; i++)
        {
        status = CSR_READ_4(pDev, ETSEC_IEVENT);
        if (status & ETSEC_TXINTRS)
            break;
        }

    CSR_WRITE_4(pDev, ETSEC_IEVENT, ETSEC_TXINTRS);

    while (pDrvCtrl->etsecTxCons != pDrvCtrl->etsecTxProd)
        {
        pDesc = &pDrvCtrl->etsecTxDesc[pDrvCtrl->etsecTxCons];
        if (pDrvCtrl->etsecTxMblk[pDrvCtrl->etsecTxCons] != NULL)
            {
            txStat = pDesc->bdSts;
            pDrvCtrl->etsecTxMblk[pDrvCtrl->etsecTxCons] = NULL;
            }
        pDrvCtrl->etsecTxFree++;
        pDesc->bdSts &= ETSEC_TBD_W;
        ETSEC_INC_DESC(pDrvCtrl->etsecTxCons, ETSEC_TX_DESC_CNT);
        }

    if (i == ETSEC_TIMEOUT || (txStat & (ETSEC_TBD_UN|ETSEC_TBD_DEF|
            ETSEC_TBD_HFE_LC|ETSEC_TBD_RL)))
        return (ERROR);

    return (OK);
    }

/******************************************************************************
*
* etsecEndPollReceive - polled mode receive routine
*
* This function receive a packet in polled mode, with interrupts disabled.
* It's similar in operation to the etsecEndRxHandle() routine, except it
* doesn't process more than one packet at a time and does not loan out
* buffers. Instead, the caller supplied an mBlk tuple into which this
* function will copy the received packet.
*
* If no packet is available, this routine will return EAGAIN. If the
* supplied packet buffer is too small to contain the received frame, the
* routine will return ERROR.
*
* RETURNS: OK, EAGAIN, or ERROR
*
* ERRNO: N/A
*/

LOCAL int etsecEndPollReceive
    (
    END_OBJ * pEnd,
    M_BLK_ID pMblk
    )
    {
    ETSEC_DRV_CTRL * pDrvCtrl;
    VXB_DEVICE_ID pDev;
    ETSEC_DESC * pDesc;
    ETSEC_RX_FCB * pFcb;
    ETSEC_DESC * oldQueue = NULL;
    UINT32 * oldIdx = NULL;
    M_BLK_ID pPkt;
    int rval = EAGAIN;
    int q;

    pDrvCtrl = (ETSEC_DRV_CTRL *)pEnd;
    if (pDrvCtrl->etsecPolling == FALSE)
        return (ERROR);

    if (!(pMblk->m_flags & M_EXT))
        return (ERROR);

    pDev = pDrvCtrl->etsecDev;

    CSR_WRITE_4(pDev, ETSEC_IEVENT, ETSEC_RXINTRS);

    if (pDrvCtrl->etsecFiler == TRUE)
        {
        oldQueue = pDrvCtrl->etsecRxDesc;
        oldIdx = pDrvCtrl->etsecRxIdx;
        for (q = 0; q < ETSEC_MAX_RX_QUEUES; q++)
            {
            pDrvCtrl->etsecRxDesc = pDrvCtrl->etsecRxQueue[q];
            pDrvCtrl->etsecRxIdx = &pDrvCtrl->etsecRxIdxs[q];
            pDrvCtrl->etsecRxMblk = pDrvCtrl->etsecRxMblks[q];
            pDesc = &pDrvCtrl->etsecRxDesc[*pDrvCtrl->etsecRxIdx];
            if (!(pDesc->bdSts & ETSEC_RBD_E))
                {
                if (pMblk->m_len < (pDesc->bdLen - ETSEC_RX_ADJ))
                    rval = ERROR;
                else
                    rval = OK;
                break;
                }
            }
        if (rval != OK)
            {
            pDrvCtrl->etsecRxDesc = oldQueue;
            pDrvCtrl->etsecRxIdx = oldIdx;
            return (EAGAIN);
            }
        }
    else
        {
        pDesc = &pDrvCtrl->etsecRxDesc[*pDrvCtrl->etsecRxIdx];
        if (pDesc->bdSts & ETSEC_RBD_E)
            return (EAGAIN);
        if (pMblk->m_len < (pDesc->bdLen - ETSEC_RX_ADJ))
            return (ERROR);
        }
 
    pPkt = pDrvCtrl->etsecRxMblk[*pDrvCtrl->etsecRxIdx];

    if (pDesc->bdSts & (ETSEC_RBD_LG|ETSEC_RBD_NO|
        ETSEC_RBD_SH|ETSEC_RBD_CR|ETSEC_RBD_OV))
        rval = ERROR;
    else
        {
        pMblk->m_flags |= M_PKTHDR;
        pMblk->m_len = pMblk->m_pkthdr.len = pDesc->bdLen - ETSEC_RX_ADJ;
        bcopy (mtod(pPkt, char *) + sizeof (ETSEC_RX_FCB) + ETSEC_ALIGN,
            mtod(pMblk, char *), pMblk->m_len);

        /* Handle checksum offload. */

        if (pDrvCtrl->etsecCaps.cap_enabled & IFCAP_RXCSUM)
            {
            pFcb = (ETSEC_RX_FCB *)pPkt->m_data;
            if (pFcb->fcbFlags & ETSEC_RX_FCB_CIP)
                pMblk->m_pkthdr.csum_flags |= CSUM_IP_CHECKED;
            if (!(pFcb->fcbFlags & ETSEC_RX_FCB_EIP))
                pMblk->m_pkthdr.csum_flags |= CSUM_IP_VALID;
            if (pFcb->fcbFlags & ETSEC_RX_FCB_CTU &&
                !(pFcb->fcbFlags & ETSEC_RX_FCB_ETU))
                {
                pMblk->m_pkthdr.csum_flags |= CSUM_DATA_VALID|CSUM_PSEUDO_HDR;
                pMblk->m_pkthdr.csum_data = 0xffff;
                }
            }

        rval = OK;
        }

    /*
     * Reset the descriptor
     * Note: to work around erratum eTSEC 2, RX descriptors must
     * be returned to the chip in groups of 4.
     */

     if (*pDrvCtrl->etsecRxIdx & 0x3)
         pDesc->bdSts |= ETSEC_RBD_E;

     if ((*pDrvCtrl->etsecRxIdx & 0x3) == 0x3)
         {
         pDesc = &pDrvCtrl->etsecRxDesc[*pDrvCtrl->etsecRxIdx & ~0x3];
         pDesc->bdSts |= ETSEC_RBD_E;
         }

    ETSEC_INC_DESC(*pDrvCtrl->etsecRxIdx, ETSEC_RX_DESC_CNT);

    CSR_WRITE_4 (pDev, ETSEC_RSTAT, ETSEC_RSTAT_QHLT0|ETSEC_RSTAT_QHLT1|
        ETSEC_RSTAT_QHLT2|ETSEC_RSTAT_QHLT3|ETSEC_RSTAT_QHLT4|
        ETSEC_RSTAT_QHLT5|ETSEC_RSTAT_QHLT6|ETSEC_RSTAT_QHLT7);

    if (pDrvCtrl->etsecFiler == TRUE)
        {
        pDrvCtrl->etsecRxDesc = oldQueue;
        pDrvCtrl->etsecRxIdx = oldIdx;
        }

    return (rval);
    }
