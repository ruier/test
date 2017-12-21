/* vxbTei8259xEnd2.c - Intel 82598/82599 10GbE ethernet VxBus END2 driver */

/*
 * Copyright (c) 2010, 2012, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01h,21mar13,fao  Correct statistics when 82598. (WIND00403956)
01g,27feb13,wyt  fix coverity error. (WIND00401412)
01f,09aug12,clx  correct the vxbUsDelay prototype.(WIND00354095)
01e,27sep10,dlk  Do not split packet buffers from Ipcom_pkt structures, as
                 the NAE stack does not support it.
01d,26sep10,dlk  Merge 6.8.2 -> 6.9.  Still to do: 64-bit support.
01c,17aug10,dlk  Add support for additional 82599 NICs (from 01h,16jun10,wap).
01b,29jul10,dlk  Adding MNT forwarding support.
01a,02jun10,dlk  written based on vxbTei82598End.c (01d,31aug09,wap
                 plus 01g,03jun10,wap)
*/

/*
DESCRIPTION
This module implements a driver for Intel 10 gigabit ethernet adapters
based on the 82598 and 82599 PCI Express controller chips.

The 82598 provides 1Gbps and 10Gbps support in single port and dual 
port configurations (1Gbps support is only available on 10Gbase-T
adapters). Special features include TCP/IP checksum offload for IPv4 
IPv6, hardware VLAN tag insertion and stripping, TCP segmentation
offload (TSO), Direct Cache Access (DCA), Receive Side Scaling (RSS),
interrupt throttling, header splitting, VLAN filtering, MSI-X and VMDq.
The receiver supports up to 16 individual MAC addresses and has a
4096-bit multicast hash filter. The 82599 also supports SR-IOV.

The 82598 supports 64 RX DMA queues and 32 TX DMA queues, using either
the legacy descriptor format or the new 'advanced' format. Both formats
are nearly identical to those supported by the Intel PRO/1000 'advanced'
family of devices (82575 and 82576). Each RX and TX queue can be mapped
to any one of 16 available interrupt sources. The 82599 improves upon
the 82598, supporting 128 RX and TX queues.

This driver uses the advanced descriptor format with one RX queue an
one TX queue. The checksum offload features are supported for both
receive and transmit, as well as VLAN tag insertion and stripping. The
interrupt throttling feature is used to limit the number of interrupts
per second to reduce context switching overhead. Support for jumbo
frames is also available, though it is disabled by default to reduce
memmory usage.

The 82598 provides a simplified link management scheme, which saves
driver code from having to access underlying PHY registers directly.
Interrupts are generated for link change events, allowing the driver
to detect and respond to cable unplug/replug events instantaneously.
Since no direct PHY management is required, the driver does not use
miiBus.

The default media configuration is loaded from the attached EEPROM.
Currently, this driver has only been tested with the Intel 10GbE XF
SR server adapter, which supports multimode fiber, and the Intel 10GbE
CX4 adapters, which support 8 pair shielded copper. Adapters with other
media may also function without the need for any driver modifications.

Note that on most systems, the main performance bottleneck when using
10 gigabit is likely to be memory bandwidth. In most TCP/IP implementations,
including that in VxWorks, data is frequently copied between application
buffers and stack buffers. Even if the CPU has enough spare cycles to
move the data, it may experience many stalls if the memory is too slow,
which will prevent it from keeping the network pipe saturated. One way
to overcome this is to move the data using a DMA engine, such as the
Intel EDMA or QuickData controllers available on some platforms. Using
these features requires modifying the TCP/IP stack, however.

Also note that the card must be plugged into a slot capable of
negotiating a PCIe x8 link in order to be able to achieve full speed. Some
boards have slots that physically accomodate the 82598 card, but only
negotiate a x1 link. The 82598 will work in such a configuration, but
the maximum throughput will be limited to approximately 1.4Gbps.

BOARD LAYOUT
The 82598 is available on standalone PCIe NICs as well as integrated
onto certain system boards. All configurations are jumperless.

EXTERNAL INTERFACE
The driver provides a vxBus external interface. The only exported
routine is the teiRegister() function, which registers the driver
with VxBus.

The Intel 8259x devices also support jumbo frames. This driver has
jumbo frame support, which is disabled by default in order to conserve
memory (jumbo frames require the use of a buffer pool with larger clusters).
Jumbo frames can be enabled on a per-interface basis using a parameter
override entry in the hwconf.c file in the BSP. For example, to enable
jumbo frame support for interface tei0, the following entry should be
added to the VXB_INST_PARAM_OVERRIDE table:

    { "tei", 0, "jumboEnable", VXB_PARAM_INT32, {(void *)1} }

SMP CONSIDERATIONS
This driver uses atomic variables to manage critical sections so that it
can be safely used in SMP configurations.

SEE ALSO: vxBus, ifLib
\tb "Intel(r) 82598 10 Gigabit Ethernet Controller Datasheet http://download.intel.com/design/network/datashts/319282.pdf"
\tb "Intel(r) 82598 10 Gigabit Ethernet Controller Specification Update http://www.intel.com/design/network/specupdt/321040.pdf"
\tb "Intel(r) 82599 10 GbE Controller Datasheet http://download.intel.com/design/network/datashts/82599_datasheet.pdf"
*/


#include <vxWorks.h>
#include <intLib.h>
#include <logLib.h>
#include <muxLib.h>
#include <netLib.h>
#include <semLib.h>
#include <sysLib.h>
#include <vxBusLib.h>
#include <wdLib.h>
#include <etherMultiLib.h>
#include <end.h>
#define END_MACROS
#include <endLib.h>
#include <endMedia.h>
#include <vxAtomicLib.h>
#ifndef TEI_VXB_DMA_BUF
#include <cacheLib.h>
#endif

#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/vxbus/vxbPciLib.h>
#include <hwif/util/vxbDmaBufLib.h>
#include <hwif/util/vxbParamSys.h>
#include <../src/hwif/h/vxbus/vxbAccess.h>
#include <../src/hwif/h/hEnd/hEnd.h>

#include <drv/pci/pciConfigLib.h>

#define IPCOM_SKIP_NATIVE_SOCK_API
#include <ipcom_vxworks.h>
#include <ipcom_clib.h>
#include <vxmux_pkt.h>
#include <ipnet_eth.h>

/*#include <../src/hwif/h/end2/vxbTei8259xEnd2.h>*/
#include "vxbTei8259xEnd2.h"

#ifdef TEI_FORWARDER
#include <cpuset.h>
#endif

#if defined (TEI_GUEST_VIRT_TO_PHYS)
#include <wrhv/vbi.h>

/*
 * teiVbAddrTrans.physBase is the 'host' physical address corresponding
 * to virtual board physical address 0x0.  We cache it so we don't
 * need to call the hypervisor for translations on each packet.
 *
 * Packet buffers and DMA rings may legitimately be only in the
 * VB's system memory or in the main shared memory region for
 * the forwarder.  We don't bother to check this.
 *
 * TODO: This code is rather fragile, as it assumes only one
 * single shared memory region apart from the VBs hypervisor-
 * granted memory range, both assumed to have simple offset
 * virt-to-phys mappings.
 */

struct
    {
    unsigned long virtBase;  /* VB virtual address of SM region */
    uint64_t physBase;	     /* host physical address of VB memory base */
    uint64_t shmOffset;      /* host physical minus VB virtual offset for
				shared mem region */
    } teiVbAddrTrans  = {0, ~0ULL, 0ULL};

_WRS_INLINE uint64_t teiVirtToPhys
    (
    void * vaddr
    )
    {
    uint64_t paddr;

    if ((unsigned long)vaddr < teiVbAddrTrans.virtBase)
	paddr = teiVbAddrTrans.physBase + (uint64_t)(size_t)vaddr;  /* add VB base phys addr */
    else
	paddr = teiVbAddrTrans.shmOffset + (uint64_t)(size_t)vaddr;
    return paddr;
    }
#endif

/* temporary */

IMPORT void vxbUsDelay (INT32);
IMPORT FUNCPTR _func_m2PollStatsIfPoll;

/* VxBus methods */

LOCAL void	teiInstInit (VXB_DEVICE_ID);
LOCAL void	teiInstInit2 (VXB_DEVICE_ID);
LOCAL void	teiInstConnect (VXB_DEVICE_ID);
LOCAL STATUS	teiInstUnlink (VXB_DEVICE_ID, void *);

/* mux methods */

LOCAL void	teiMuxConnect (VXB_DEVICE_ID, void *);

LOCAL struct drvBusFuncs teiFuncs =
    {
    teiInstInit,	/* devInstanceInit */
    teiInstInit2,	/* devInstanceInit2 */
    teiInstConnect	/* devConnect */
    };

LOCAL struct vxbDeviceMethod teiMethods[] =
   {
   DEVMETHOD(mux2DevConnect,	teiMuxConnect),
   DEVMETHOD(vxbDrvUnlink,	teiInstUnlink),
   { 0, 0 }
   };   

/*
 * List of supported device IDs.
 */

LOCAL struct vxbPciID teiPciDevIDList[] =
    {
        /* { devID, vendID } */
        { INTEL_DEVICEID_82598, INTEL_VENDORID },
        { INTEL_DEVICEID_82598_BX, INTEL_VENDORID },
        { INTEL_DEVICEID_82598AF_DUAL_PORT, INTEL_VENDORID },
        { INTEL_DEVICEID_82598AF_SINGLE_PORT, INTEL_VENDORID },
        { INTEL_DEVICEID_82598AT, INTEL_VENDORID },
        { INTEL_DEVICEID_82598AT2, INTEL_VENDORID },
        { INTEL_DEVICEID_82598EB_CX4, INTEL_VENDORID },
        { INTEL_DEVICEID_82598EB_SFP_LOM, INTEL_VENDORID },
        { INTEL_DEVICEID_82598_CX4_DUAL_PORT, INTEL_VENDORID },
        { INTEL_DEVICEID_82598_DA_DUAL_PORT, INTEL_VENDORID },
        { INTEL_DEVICEID_82598_SR_DUAL_PORT_EM, INTEL_VENDORID },
        { INTEL_DEVICEID_82598EB_XF_LR, INTEL_VENDORID },
        { INTEL_DEVICEID_82599_CX4, INTEL_VENDORID },
        { INTEL_DEVICEID_82599_SFP, INTEL_VENDORID },
        { INTEL_DEVICEID_82599_SFP_EM, INTEL_VENDORID },
        { INTEL_DEVICEID_82599_KX4, INTEL_VENDORID },
        { INTEL_DEVICEID_82599_COMBO_BACKPLANE, INTEL_VENDORID },
        { INTEL_DEVICEID_82599_BX, INTEL_VENDORID },
        { INTEL_DEVICEID_82599_KX4_MEZZ, INTEL_VENDORID },
        { INTEL_DEVICEID_82599_KR, INTEL_VENDORID },
        { INTEL_DEVICEID_82599_T3_LOM, INTEL_VENDORID },
        { INTEL_DEVICEID_82599_XAUI_LOM, INTEL_VENDORID }
    };

/* default queue parameters */
LOCAL HEND_RX_QUEUE_PARAM teiRxQueueDefault = {
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

LOCAL HEND_TX_QUEUE_PARAM teiTxQueueDefault = {
    NULL,                       /* jobQueId */
    0,                          /* priority */
    0,                          /* tbdNum */
    0,                          /* allowedFrags */
    NULL,                       /* pTxBdBase */
    0,                          /* txBdMemSize */
    0,                          /* txBdMemAttributes */
    NULL                        /* txBdMemFreeMethod */
};

LOCAL VXB_PARAMETERS teiParamDefaults[] =
    {
       {"rxQueue00", VXB_PARAM_POINTER, {(void *)&teiRxQueueDefault}},
       {"txQueue00", VXB_PARAM_POINTER, {(void *)&teiTxQueueDefault}},
       {"jumboEnable", VXB_PARAM_INT32, {(void *)0}},
        {NULL, VXB_PARAM_END_OF_LIST, {NULL}}
    };

LOCAL struct vxbPciRegister teiDevPciRegistration =
    {
        {
        NULL,			/* pNext */
        VXB_DEVID_DEVICE,	/* devID */
        VXB_BUSID_PCI,		/* busID = PCI */
#ifdef VXB_VER_4_0_0
        VXB_VER_4_0_0,		/* vxbVersion */
#else
        VXBUS_VERSION_3,	/* vxbVersion */
#endif
        TEI_NAME,		/* drvName */
        &teiFuncs,		/* pDrvBusFuncs */
        teiMethods,		/* pMethods */
        NULL,			/* devProbe */
        teiParamDefaults	/* pParamDefaults */
        },
    NELEMENTS(teiPciDevIDList),
    teiPciDevIDList
    };

/* Driver utility functions */

LOCAL STATUS	teiEeWordGet (VXB_DEVICE_ID, UINT32, UINT16 *);
LOCAL void	teiEepromRead (VXB_DEVICE_ID, UINT8 *, int, int);
LOCAL STATUS	teiReset (VXB_DEVICE_ID);
LOCAL void	teiIvarSet (VXB_DEVICE_ID, int, int, int);
LOCAL void	teiIvar599Set (VXB_DEVICE_ID, int, int, int);
LOCAL void	teiBufFree (TEI_DRV_CTRL * pDrvCtrl);

/* END functions */

LOCAL END_OBJ *	teiEndLoad (char *, void *);
LOCAL STATUS	teiEndUnload (END_OBJ *);
LOCAL int	teiEndIoctl (END_OBJ *, int, caddr_t);
LOCAL STATUS	teiEndMCastAddrAdd (END_OBJ *, char *);
LOCAL STATUS	teiEndMCastAddrDel (END_OBJ *, char *);
LOCAL STATUS	teiEndMCastAddrGet (END_OBJ *, MULTI_TABLE *);
LOCAL void	teiEndHashTblPopulate (TEI_DRV_CTRL *);
LOCAL STATUS	teiEndStatsDump (TEI_DRV_CTRL *);
LOCAL void	teiEndRxConfig (TEI_DRV_CTRL *);
LOCAL STATUS	teiEndStart (END_OBJ *);
LOCAL STATUS	teiEndStop (END_OBJ *);
LOCAL int	teiEndSend (END_OBJ *, Ipcom_pkt *);
LOCAL void	teiEndTbdClean (TEI_DRV_CTRL *);
LOCAL int	teiEndEncap (TEI_DRV_CTRL *, Ipcom_pkt *);
LOCAL STATUS	teiEndPollSend (END_OBJ *, Ipcom_pkt *);
LOCAL int	teiEndPollReceive (END_OBJ *, Ipcom_pkt *);
LOCAL void	teiEndInt (TEI_DRV_CTRL *);
LOCAL int	teiEndRxHandle (TEI_DRV_CTRL *);
LOCAL void	teiEndTxHandle (TEI_DRV_CTRL *);
LOCAL void	teiEndIntHandle (void *);

LOCAL END2_NET_FUNCS teiNetFuncs = {
    {
    teiEndStart,                        /* start func. */
    teiEndStop,                         /* stop func. */
    teiEndUnload,                       /* unload func. */
    teiEndIoctl,                        /* ioctl func. */
    (END_MUXSEND_RTN)teiEndSend,        /* send func. */
    teiEndMCastAddrAdd,                 /* multicast add func. */
    teiEndMCastAddrDel,                 /* multicast delete func. */
    teiEndMCastAddrGet,                 /* multicast get fun. */
    (END_MUXSEND_RTN)teiEndPollSend,    /* polling send func. */
    (END_POLLRCV_RTN)teiEndPollReceive, /* polling receive func. */
    NULL, /* endEtherAddressForm,       /@ put address info in a NET_BUFFER */
    NULL, /* endEtherPacketDataGet,     /@ get pointer to data in NET_BUFFER */
    NULL  /* endEtherPacketAddrGet      /@ Get packet addresses */
    },
    NULL				/* llhiComplete; NULL -> ethernet */
};

#ifdef TEI_FORWARDER

void            teiFwRedirect (Ipcom_fw_port_t * port, Ip_bool enable);
void            teiFwRssReseed (TEI_DRV_CTRL * pDrvCtrl, UINT32 * bits320);
void            teiFwMultiQEnable (TEI_DRV_CTRL * pDrvCtrl, BOOL enable);
LOCAL Ip_u64    teiFwQAddr (Ipcom_fw_port_t * port, Ip_u32 q_index,
			    enum fw_q_type q_type);

LOCAL void      teiFwStatisticsUpdate(Ipcom_fw_port_t * port, Ip_bool reset);

LOCAL void	teiFwPortDrvinfo(Ipcom_fw_port_t * port,
				 Ipcom_fw_port_drvinfo_t * drvinfo);

LOCAL Ipcom_fw_port_funcs_t teiFwPortFuncs =
    {
    teiFwStatisticsUpdate,
    teiFwRedirect,
    teiFwPortDrvinfo,
    teiFwQAddr
    };


extern cpuset_t ipcom_forwarder_cpus;

/* 'secret' key used in RSS hash. Picked out of the air. */
UINT32 teiFwRssKey [10] =
    {
    0x61053e38,
    0x032a9d56,
    0x4c798412,
    0xc1f39628,
    0x0d6f4bab,
    
    0x18843792,
    0xfeedf00d,
    0xcafebebe,
    0xabadaba2,
    0x91c043d6
    };

/*
 * teiFwReservedUnits is provided for debug use; by setting a bit
 * corresponding to a unit number in this variable, that device
 * will not be available as a forwarder port even if it is an
 * 82576 device. Eventually we might want to make this or something
 * similar configurable in a VIP.
 */
UINT32 teiFwReservedUnits = 0x0000;

#endif /* TEI_FORWARDER */

/*****************************************************************************
*
* teiEnd2Register - register with the VxBus subsystem
*
* This routine registers the 82598/82599 driver with VxBus as a
* child of the PCI bus type.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void teiEnd2Register(void)
    {
    vxbDevRegister((struct vxbDevRegInfo *)&teiDevPciRegistration);
    return;
    }

/*****************************************************************************
*
* teiInstInit - VxBus instInit handler
*
* This function implements the VxBus instInit handler for an tei
* device instance. The only thing done here is to select a unit
* number for the device.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void teiInstInit
    (
    VXB_DEVICE_ID pDev
    )
    {
    vxbNextUnitGet (pDev);
    return;
    }

/*****************************************************************************
*
* teiInstInit2 - VxBus instInit2 handler
*
* This function implements the VxBus instInit2 handler for an tei
* device instance. Once we reach this stage of initialization, it's
* safe for us to allocate memory, so we can create our pDrvCtrl
* structure and do some initial hardware setup. The important
* steps we do here are to connect our ISR to our assigned interrupt
* vector, read the station address from the EEPROM, and set up our
* vxbDma tags and memory regions.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void teiInstInit2
    (
    VXB_DEVICE_ID pDev
    )
    {
    TEI_DRV_CTRL * pDrvCtrl;
    VXB_INST_PARAM_VALUE val;
    TEI_CORE_SECTION core;
    UINT16 coreptr;
    UINT16 devId = 0;
    UINT32 media;
    int i;

    
#ifdef TEI_GUEST_VIRT_TO_PHYS
    if (teiVbAddrTrans.physBase == ~0ULL)
	{
	void *vbPhys;
	uint32_t length;
	uint32_t attr;

	/* assume identity virt-to-phys mapping in VB for shared memory regions */
	(void)vbiShmemRegionFind((int8_t *)"mipc_sm", &vbPhys, &length, &attr);
	teiVbAddrTrans.virtBase = (unsigned long)vbPhys;

	(void)vbiGuestPhysToPhysAddr ((vbiGuestPhysAddr_t)vbPhys,
				      &teiVbAddrTrans.shmOffset);

	teiVbAddrTrans.shmOffset -= (uint64_t)(size_t)vbPhys;

	(void)vbiGuestPhysToPhysAddr (0, &teiVbAddrTrans.physBase);
	}
#endif

    logMsg("xxxxxxx\n",0,0,0,0,0,0);

    pDrvCtrl = malloc (sizeof(TEI_DRV_CTRL));
    if (pDrvCtrl == NULL)
        return;

    bzero ((char *)pDrvCtrl, sizeof(TEI_DRV_CTRL));
    pDev->pDrvCtrl = pDrvCtrl;
    pDrvCtrl->teiDev = pDev;

    for (i = 0; i < VXB_MAXBARS; i++)
        {
        if (pDev->regBaseFlags[i] == VXB_REG_MEM)
            break;
        }

    /* Should never happen. */

    if (i == VXB_MAXBARS)
        return;

    vxbRegMap (pDev, i, &pDrvCtrl->teiHandle);
    pDrvCtrl->teiBar = pDev->pRegBase[i];

    /* create mutex to protect critical sections */

    pDrvCtrl->teiDevSem = semMCreate (SEM_Q_PRIORITY |
        SEM_DELETE_SAFE | SEM_INVERSION_SAFE);

#ifdef VXB_VER_4_0_0
    VXB_PCI_BUS_CFG_READ (pDev, PCI_CFG_DEVICE_ID, 2, devId);
#else
    {
        UINT32 flags;
        pDev->pAccess->busCfgRead (pDev, PCI_CFG_DEVICE_ID, 2,
            (char *)&devId, &flags);
    }
#endif

    /* Assume 82598 device, unless we detect otherwise. */

    pDrvCtrl->teiDevType = TEI_DEVTYPE_82598;

     logMsg("xxxxxxx1\n",0,0,0,0,0,0);
    
    if (devId == INTEL_DEVICEID_82599_SFP ||
        devId == INTEL_DEVICEID_82599_CX4 ||
        devId == INTEL_DEVICEID_82599_KX4 ||
        devId == INTEL_DEVICEID_82599_COMBO_BACKPLANE ||
        devId == INTEL_DEVICEID_82599_SFP_EM ||
        devId == INTEL_DEVICEID_82599_BX ||
        devId == INTEL_DEVICEID_82599_KX4_MEZZ ||
        devId == INTEL_DEVICEID_82599_KR ||
        devId == INTEL_DEVICEID_82599_T3_LOM ||
        devId == INTEL_DEVICEID_82599_XAUI_LOM)
        pDrvCtrl->teiDevType = TEI_DEVTYPE_82599;

#if !defined(TEI_MUX_RX_ALWAYS) || !defined(TEI_MUX_TX_ALWAYS)
    /* Default to MUX support for both TX and RX */
    pDrvCtrl->muxqs = TEI_MUX_RX | TEI_MUX_TX;
    
#endif

#ifdef TEI_FORWARDER
    /*
     * For now, we only support forwarding offload on 82599,
     * and only if the device hasn't been reserved for other
     * purposes.
     */
    pDrvCtrl->fw_port.fw_capable = IP_FALSE; /* Default */

    if (pDrvCtrl->teiDevType == TEI_DEVTYPE_82599 &&
        (teiFwReservedUnits & (1 << pDev-> unitNumber)) == 0)
	{
	UINT32 base;

	pDrvCtrl->fw_port.fw_capable = IP_TRUE;

	base = 1;
#if !defined (TEI_MUX_RX_ALWAYS)
	pDrvCtrl->muxqs &= ~TEI_MUX_RX;
	base = 0;
#endif
	pDrvCtrl->fw_port.numRxQs = min (TEI_RXQ_NUM_99 - base,
					 IPCOM_MAX_FORWARDERS);

#if IPCOM_MAX_FORWARDERS > TEI_TXQ_NUM_99 - 1
#error IPCOM_MAX_FORWARDERS is too large.
#endif
	base = 1;
#if !defined (TEI_MUX_TX_ALWAYS)
	pDrvCtrl->muxqs &= ~TEI_MUX_TX;
	base = 0;
#endif
	pDrvCtrl->fw_port.numTxQs = min (TEI_TXQ_NUM_99 - base,
					 IPCOM_MAX_FORWARDERS);
	}
    
#endif /* TEI_FORWARDER */

    /* reset the hardware */
    teiReset (pDev);
    
    /* Get the station address from the EEPROM */

    if ((CSR_READ_4(pDev, TEI_STS) & TEI_STS_LANID) == TEI_LANID_0)
        teiEepromRead (pDev, (UINT8 *)&coreptr, TEI_EE_CORE0SECT_PTR, 1);
    else
        teiEepromRead (pDev, (UINT8 *)&coreptr, TEI_EE_CORE1SECT_PTR, 1);

    coreptr = le16toh(coreptr);
    teiEepromRead (pDev, (UINT8 *)&core, coreptr, sizeof(core) / 2);

   /* pDrvCtrl->teiAddr[0] = core.tei_addr[0];
    pDrvCtrl->teiAddr[1] = core.tei_addr[1];
    pDrvCtrl->teiAddr[2] = core.tei_addr[2];
    pDrvCtrl->teiAddr[3] = core.tei_addr[3];
    pDrvCtrl->teiAddr[4] = core.tei_addr[4];
    pDrvCtrl->teiAddr[5] = core.tei_addr[5];*/

    pDrvCtrl->teiAddr[0] = 0X00;
    pDrvCtrl->teiAddr[1] = 0XA0;
    pDrvCtrl->teiAddr[2] = 0X44;
    pDrvCtrl->teiAddr[3] = 0X00;
    pDrvCtrl->teiAddr[4] = 0X00;
    pDrvCtrl->teiAddr[5] = 0X00;
   
    switch (devId)
        {
        /* shielded 8 pair copper */
        case INTEL_DEVICEID_82598EB_CX4:
        case INTEL_DEVICEID_82598_CX4_DUAL_PORT:
        case INTEL_DEVICEID_82599_CX4:
            media = IFM_10G_CX4;
            break;
        /* single mode fiber */
        case INTEL_DEVICEID_82598EB_XF_LR:
            media = IFM_10G_LR;
            break;
        /* unshielded twisted pair copper */
        case INTEL_DEVICEID_82598AT:
        case INTEL_DEVICEID_82598AT2:
            media = IFM_10G_T;
            break;
        /* multimode fiber */
        default:
            media = IFM_10G_SR;
            break;
        }

    /*
     * We don't bother using miiBus for this adapter, since it
     * doesn't really have an MII-compatible link management interface.
     * Also, there's really only one supported mode.
     */

    pDrvCtrl->teiCurStatus = IFM_AVALID;
    pDrvCtrl->teiCurMedia = IFM_ETHER | IFM_FDX | media;
    pDrvCtrl->teiMediaList = malloc(sizeof(END_MEDIALIST) + sizeof(UINT32));
    if (pDrvCtrl->teiMediaList == NULL)
        return;
    pDrvCtrl->teiMediaList->endMediaList[0] = pDrvCtrl->teiCurMedia;
    pDrvCtrl->teiMediaList->endMediaListLen = 1;
    pDrvCtrl->teiMediaList->endMediaListDefault = pDrvCtrl->teiCurMedia;

    /*
     * 10GBase-T NICs also support 1Gbps for backwards compatibility.
     */

    if (media == IFM_10G_T)
        {
        pDrvCtrl->teiMediaList->endMediaListLen++;
        pDrvCtrl->teiMediaList->endMediaList[1] =
            IFM_ETHER | IFM_FDX | IFM_1000_T;
        }

    /*
     * paramDesc {
     * The jumboEnable parameter specifies whether
     * this instance should support jumbo frames.
     * The default is false. In some cases, support
     * for jumbo frames will remain off even if this
     * parameter is enabled, if the hardware is known
     * not to support jumbo frames. }
     */
#ifdef TEI_FORWARDER
    /*
     * For the forwarder case, use a global setting for whether
     * jumbo frames are enabled (for forwarder-capable ports).
     */
    if (pDrvCtrl->fw_port.fw_capable)
	{
	i = OK;
	val.int32Val = (ipcom_forwarder_mtu() > 1500);
	}
    else
#endif
	i = vxbInstParamByNameGet (pDev, "jumboEnable", VXB_PARAM_INT32, &val);

    if (i != OK || val.int32Val == 0)
        {
        i = TEI_CLSIZE;
        pDrvCtrl->teiMaxMtu = TEI_MTU;
        }
    else
        {
        i = END_JUMBO_CLSIZE;
        pDrvCtrl->teiMaxMtu = TEI_JUMBO_MTU;
        }

#ifdef TEI_VXB_DMA_BUF
    /* Get a reference to our parent tag. */

    if (TEI_MUX_ANY_ENABLED(pDrvCtrl))
	pDrvCtrl->teiParentTag = vxbDmaBufTagParentGet (pDev, 0);

    /* Create tag for RX descriptor ring. */

    if (TEI_MUX_RX_ENABLED(pDrvCtrl))
	{
	pDrvCtrl->teiRxDescTag = vxbDmaBufTagCreate (pDev,
	     pDrvCtrl->teiParentTag,         /* parent */
	     256 /*_CACHE_ALIGN_SIZE*/,      /* alignment */
	     0,                              /* boundary */
	     0xFFFFFFFF,                     /* lowaddr */
	     0xFFFFFFFF,                     /* highaddr */
	     NULL,                           /* filter */
	     NULL,                           /* filterarg */
	     sizeof(TEI_ADV_RDESC) * TEI_RX_DESC_CNT,      /* max size */
	     1,                              /* nSegments */
	     sizeof(TEI_ADV_RDESC) * TEI_RX_DESC_CNT,      /* max seg size */
	     VXB_DMABUF_ALLOCNOW|VXB_DMABUF_NOCACHE,            /* flags */
	     NULL,                           /* lockfunc */
	     NULL,                           /* lockarg */
	     NULL);                          /* ppDmaTag */

	pDrvCtrl->teiRxDescMem = vxbDmaBufMemAlloc (pDev,
	    pDrvCtrl->teiRxDescTag, NULL, 0, &pDrvCtrl->teiRxDescMap);
	}

    if (TEI_MUX_TX_ENABLED(pDrvCtrl))
	{
	/* Create tag for TX descriptor ring. */

	pDrvCtrl->teiTxDescTag = vxbDmaBufTagCreate (pDev,
	     pDrvCtrl->teiParentTag,       /* parent */
	     256 /*_CACHE_ALIGN_SIZE*/,              /* alignment */
	     0,                              /* boundary */
	     0xFFFFFFFF,                     /* lowaddr */
	     0xFFFFFFFF,                     /* highaddr */
	     NULL,                           /* filter */
	     NULL,                           /* filterarg */
	     sizeof(TEI_ADV_TDESC) * TEI_TX_DESC_CNT,      /* max size */
	     1,                              /* nSegments */
	     sizeof(TEI_ADV_TDESC) * TEI_TX_DESC_CNT,      /* max seg size */
	     VXB_DMABUF_ALLOCNOW|VXB_DMABUF_NOCACHE,            /* flags */
	     NULL,                           /* lockfunc */
	     NULL,                           /* lockarg */
	     NULL);                          /* ppDmaTag */

	pDrvCtrl->teiTxDescMem = vxbDmaBufMemAlloc (pDev,
	    pDrvCtrl->teiTxDescTag, NULL, 0, &pDrvCtrl->teiTxDescMap);
    /*printf("pDrvCtrl->teiTxDescTag=0x%x, pDrvCtrl->teiTxDescMem=0x%x\n",
            pDrvCtrl->teiTxDescTag, pDrvCtrl->teiTxDescMem);*/
	}

    if (TEI_MUX_ANY_ENABLED(pDrvCtrl))
	{
	/* Create tag for mBlk mappings */

	pDrvCtrl->teiPktTag = vxbDmaBufTagCreate (pDev,
	    pDrvCtrl->teiParentTag,       /* parent */
	    1,                            /* alignment */
	    0,                            /* boundary */
	    0xFFFFFFFF,                   /* lowaddr */
	    0xFFFFFFFF,                   /* highaddr */
	    NULL,                         /* filter */
	    NULL,                         /* filterarg */
	    i,                            /* max size */
	    TEI_MAXFRAG,                  /* nSegments */
	    i,                            /* max seg size */
	    VXB_DMABUF_ALLOCNOW,          /* flags */
	    NULL,                         /* lockfunc */
	    NULL,                         /* lockarg */
	    NULL);                        /* ppDmaTag */
   
	}

    if (TEI_MUX_TX_ENABLED(pDrvCtrl))
	for (i = 0; i < TEI_TX_DESC_CNT; i++)
	    {
	    if (vxbDmaBufMapCreate (pDev, pDrvCtrl->teiPktTag, 0,
				    &pDrvCtrl->teiTxPktMap[i]) == NULL)
		logMsg("create Tx map %d failed\n", i, 0,0,0,0,0);
	    }

    if (TEI_MUX_RX_ENABLED(pDrvCtrl))
	for (i = 0; i < TEI_RX_DESC_CNT; i++)
	    {
	    if (vxbDmaBufMapCreate (pDev, pDrvCtrl->teiPktTag, 0,
				    &pDrvCtrl->teiRxPktMap[i]) == NULL)
		logMsg("create Rx map %d failed\n", i, 0,0,0,0,0);
	    }
#else
    /*
     * Note, cacheDmaMalloc() is guaranteed to return a cache-aligned
     * buffer only when the cache is enabled (and it may not be in bootroms).
     * So we must do manual alignment.  The descriptor base address must
     * be at least 16-byte aligned, but the memory length must be a multiple 
     * of 128 for both RX and TX; so we might as well make the start of the
     * descriptor table 128-byte aligned anyway.
     *
     * We introduce teiDescBuf to hold the (possibly) unaligned
     * start address, for freeing.
     */
    if (TEI_MUX_ANY_ENABLED(pDrvCtrl))
	{
	if ((pDrvCtrl->teiDescBuf =
	     cacheDmaMalloc (sizeof(TEI_ADV_RDESC) * TEI_RX_DESC_CNT +
			     sizeof(TEI_ADV_TDESC) * TEI_TX_DESC_CNT + 128))
	    == NULL)
	    logMsg (TEI_NAME "%d: could not allocate descriptor memory\n",
		    pDev->unitNumber, 0, 0, 0, 0, 0);

	pDrvCtrl->teiRxDescMem = (TEI_ADV_RDESC *)
	    ROUND_UP (pDrvCtrl->teiDescBuf, 128);
	pDrvCtrl->teiTxDescMem = (TEI_ADV_TDESC *)
	    (pDrvCtrl->teiRxDescMem + TEI_RX_DESC_CNT);
	}
#endif
logMsg("xxxxxxx2\n",0,0,0,0,0,0);

#ifdef TEI_FORWARDER

    if (pDrvCtrl->fw_port.fw_capable)
	{
	pDrvCtrl->fw_port.pEnd = &pDrvCtrl->teiEndObj;
	pDrvCtrl->fw_port.funcs = &teiFwPortFuncs;
	pDrvCtrl->fw_port.nRxPollers = 0; /* # of RX pollers */
	pDrvCtrl->fw_port.pktsize = ipcom_fw_pkt_size() - _CACHE_ALIGN_SIZE;

	(void)ipcom_fw_port_install (&pDrvCtrl->fw_port, pDrvCtrl->teiMaxMtu,
				     pDrvCtrl->teiAddr);
	}

#endif /* TEI_FORWARDER */

    end2BufferPoolConfig (0, /* cache line alignment is good enough */
			  TEI_RXPKT_OFFSET + TEI_MTU_ADJUST);
    return;
    }

/*****************************************************************************
*
* teiInstConnect -  VxBus instConnect handler
*
* This function implements the VxBus instConnect handler for an tei
* device instance.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void teiInstConnect
    (
    VXB_DEVICE_ID pDev
    )
    {
    return;
    }

/*****************************************************************************
*
* teiInstUnlink - VxBus unlink handler
*
* This function shuts down an tei device instance in response to an
* unlink event from VxBus. This may occur if our VxBus instance has
* been terminated, or if the tei driver has been unloaded. When an
* unlink event occurs, we must shut down and unload the END interface
* associated with this device instance and then release all the
* resources allocated during instance creation, such as vxbDma
* memory and maps, and interrupt handles.
*
* RETURNS: OK if device was successfully destroyed, otherwise ERROR
*
* ERRNO: N/A
*/

LOCAL STATUS teiInstUnlink
    (
    VXB_DEVICE_ID pDev,
    void * unused
    )
    { 
    TEI_DRV_CTRL * pDrvCtrl;
#ifdef TEI_VXB_DMA_BUF
    int i;
#endif

    pDrvCtrl = pDev->pDrvCtrl;

    /*
     * Stop the device and detach from the MUX.
     * Note: it's possible someone might try to delete
     * us after our vxBus instantiation has completed,
     * but before anyone has called our muxConnect method.
     * In this case, there'll be no MUX connection to
     * tear down, so we can skip this step.
     */

    if (pDrvCtrl->teiMuxDevCookie != NULL)
        {
        if (muxDevStop (pDrvCtrl->teiMuxDevCookie) != OK)
            return (ERROR);

        /* Detach from the MUX. */

        if (muxDevUnload (TEI_NAME, pDev->unitNumber) != OK)
            return (ERROR);
        }

    /* Release the memory we allocated for the DMA rings */
#ifdef TEI_VXB_DMA_BUF

    if (TEI_MUX_RX_ENABLED(pDrvCtrl))
	{
	vxbDmaBufMemFree (pDrvCtrl->teiRxDescTag, pDrvCtrl->teiRxDescMem,
			  pDrvCtrl->teiRxDescMap);

	for (i = 0; i < TEI_RX_DESC_CNT; i++)
	    vxbDmaBufMapDestroy (pDrvCtrl->teiPktTag,
				 pDrvCtrl->teiRxPktMap[i]);

	vxbDmaBufTagDestroy (pDrvCtrl->teiRxDescTag);
	}

    if (TEI_MUX_TX_ENABLED(pDrvCtrl))
	{
	vxbDmaBufMemFree (pDrvCtrl->teiTxDescTag, pDrvCtrl->teiTxDescMem,
			  pDrvCtrl->teiTxDescMap);

	for (i = 0; i < TEI_TX_DESC_CNT; i++)
	    vxbDmaBufMapDestroy (pDrvCtrl->teiPktTag,
				 pDrvCtrl->teiTxPktMap[i]);

	vxbDmaBufTagDestroy (pDrvCtrl->teiTxDescTag);
	}

    if (TEI_MUX_ANY_ENABLED(pDrvCtrl))
	vxbDmaBufTagDestroy (pDrvCtrl->teiPktTag);
#else
    if (TEI_MUX_ANY_ENABLED(pDrvCtrl))
	cacheDmaFree (pDrvCtrl->teiDescBuf);
#endif

    /* Disconnect the ISR. */

    vxbIntDisconnect (pDev, 0, teiEndInt, pDrvCtrl);

    semDelete (pDrvCtrl->teiDevSem);

    /* Destroy the adapter context. */
    free (pDrvCtrl);
    pDev->pDrvCtrl = NULL;

    /* Goodbye cruel world. */

    return (OK);
    }

/*****************************************************************************
*
* teiLinkUpdate - link change event handler
*
* This function processes link change event notifications triggered by
* the interrupt handler. In most configurations, the link speed will
* remain constant at 10Gbps full duplex, but we must respond to cable
* unplug and replug events.
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

LOCAL STATUS teiLinkUpdate
    (
    VXB_DEVICE_ID pDev
    )
    {
    TEI_DRV_CTRL * pDrvCtrl;
    UINT32 oldStatus;

    if (pDev->pDrvCtrl == NULL)
        return (ERROR);

    pDrvCtrl = (TEI_DRV_CTRL *)pDev->pDrvCtrl;

    semTake (pDrvCtrl->teiDevSem, WAIT_FOREVER);

    /* Check link status */

    oldStatus = pDrvCtrl->teiCurStatus;
    CSR_READ_4(pDev, TEI_LINKS);
    if (CSR_READ_4(pDev, TEI_LINKS) &&
        (CSR_READ_4(pDev, TEI_LINKS) & TEI_LINKS_LINKSTS))
        pDrvCtrl->teiCurStatus = IFM_AVALID|IFM_ACTIVE;
    else
        pDrvCtrl->teiCurStatus = IFM_AVALID;

    /*
     * For most NICs, the speed will remain at 10G, but for the
     * 10GBase-T cards, it could be either 10G or 1G. We can tell
     * the difference between a 10G link and a 1G link by testing
     * the speed bit in the link status register.
     */

    if (CSR_READ_4(pDev, TEI_LINKS) & TEI_LINKS_SPEED)
        pDrvCtrl->teiCurMedia = pDrvCtrl->teiMediaList->endMediaList[0];
    else
        pDrvCtrl->teiCurMedia = pDrvCtrl->teiMediaList->endMediaList[1];

    /* If status went from down to up, announce link up. */

    if (pDrvCtrl->teiCurStatus & IFM_ACTIVE && !(oldStatus & IFM_ACTIVE))
	jobQueueStdPost (pDrvCtrl->teiJobQueue, NET_TASK_QJOB_PRI,
			 muxLinkUpNotify, &pDrvCtrl->teiEndObj,
			 NULL, NULL, NULL, NULL);

    /* If status went from up to down, announce link down. */

    if (!(pDrvCtrl->teiCurStatus & IFM_ACTIVE) && oldStatus & IFM_ACTIVE)
	jobQueueStdPost (pDrvCtrl->teiJobQueue, NET_TASK_QJOB_PRI,
			 muxLinkDownNotify, &pDrvCtrl->teiEndObj,
			 NULL, NULL, NULL, NULL);

    semGive (pDrvCtrl->teiDevSem);

    return (OK);
    }

/*****************************************************************************
*
* teiMuxConnect - muxConnect method handler
*
* This function handles muxConnect() events, which may be triggered
* manually or (more likely) by the bootstrap code. Most VxBus
* initialization occurs before the MUX has been fully initialized,
* so the usual muxDevLoad()/muxDevStart() sequence must be defered
* until the networking subsystem is ready. This routine will ultimately
* trigger a call to teiEndLoad() to create the END interface instance.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void teiMuxConnect
    (
    VXB_DEVICE_ID pDev,
    void * unused
    )
    {
    TEI_DRV_CTRL *pDrvCtrl;

    /*
     * Attach our ISR. For PCI, the index value is always
     * 0, since the PCI bus controller dynamically sets
     * up interrupts for us.
     */

    vxbIntConnect (pDev, 0, teiEndInt, pDev->pDrvCtrl);

    pDrvCtrl = pDev->pDrvCtrl;

    /* Save the cookie. */

    pDrvCtrl->teiMuxDevCookie = muxDevLoad (pDev->unitNumber,
        teiEndLoad, "", TRUE, pDev);

    if (pDrvCtrl->teiMuxDevCookie != NULL)
        muxDevStart (pDrvCtrl->teiMuxDevCookie);

    if (_func_m2PollStatsIfPoll != NULL)
        endPollStatsInit (pDrvCtrl->teiMuxDevCookie,
            _func_m2PollStatsIfPoll);

    return;
    }

/*****************************************************************************
*
* teiEeWordGet - read a word from the EEPROM
*
* This routine reads a single 16 bit word from a specified address
* within the EEPROM attached to the 82598/82599 controller and returns
* it to the caller.
*
* RETURNS: ERROR if EEPROM access times out, otherwise OK
*
* ERRNO: N/A
*/

LOCAL STATUS teiEeWordGet
    (
    VXB_DEVICE_ID pDev,
    UINT32 addr,
    UINT16 *dest
    )
    {
    int i;
    UINT32 tmp;

    CSR_WRITE_4(pDev, TEI_EERD, 0);
    tmp = CSR_READ_4(pDev, TEI_EERD);

    CSR_WRITE_4(pDev, TEI_EERD, TEI_EEADDR(addr) | TEI_EERD_START);

    for (i = 0; i < TEI_TIMEOUT; i++)
        {
        if (CSR_READ_4(pDev, TEI_EERD) & TEI_EERD_DONE)
            break;
        }

    if (i == TEI_TIMEOUT)
        {
        *dest = 0xFFFF;
        return (ERROR);
        }

    tmp = CSR_READ_4(pDev, TEI_EERD);
    *dest = (UINT16)TEI_EEDATA(tmp);
   
    return (OK);
    }

/*****************************************************************************
*
* teiEepromRead - read a sequence of words from the EEPROM
*
* This is the top-level EEPROM access function. It will read a
* sequence of words from the specified address into a supplied
* destination buffer. This is used mainly to read the station
* address.
*
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void teiEepromRead
    (
    VXB_DEVICE_ID pDev,
    UINT8 *dest,
    int off,
    int cnt
    )
    {
    int i;
    UINT16 word, *ptr;

    for (i = 0; i < cnt; i++)
        {
        (void)teiEeWordGet (pDev, off + i, &word);
        ptr = (UINT16 *)(dest + (i * 2));
        *ptr = le16toh(word);
        }

    return;
    }

/*****************************************************************************
*
* teiReset - reset the 82598/82599 controller
*
* This function issues a reset command to the controller and waits
* for it to complete. This routine is always used to place the
* controller into a known state prior to configuration.
*
* RETURNS: ERROR if the reset bit never clears, otherwise OK
*
* ERRNO: N/A
*/

LOCAL STATUS teiReset
    (
    VXB_DEVICE_ID pDev
    )
    {
    TEI_DRV_CTRL * pDrvCtrl;
    int i;

    pDrvCtrl = pDev->pDrvCtrl;

    CSR_SETBIT_4(pDev, TEI_CTRL, TEI_CTRL_PCIE_MST_DIS);

    for (i = 0; i < TEI_TIMEOUT; i++)
        {
        if (!(CSR_READ_4(pDev, TEI_STS) & TEI_STS_PCIE_MSTENB))
            break;
        vxbUsDelay (100);
        }

    if (i == TEI_TIMEOUT)
        return (ERROR);

    CSR_SETBIT_4(pDev, TEI_CTRL, TEI_CTRL_RST);

    vxbUsDelay (5000);

    for (i = 0; i < TEI_TIMEOUT; i++)
        {
        if (!(CSR_READ_4(pDev, TEI_CTRL) & TEI_CTRL_RST))
            break;
        vxbUsDelay (100);
        }

    if (i == TEI_TIMEOUT)
        return (ERROR);

    vxbUsDelay (50);

    /*
     * Work around erratum #10 from the specification update: "ECC
     * on the descriptor completion memory needs to be disabled."
     * Leaving ECC enabled for these regions causes corruption of
     * internal memory, which leads to various DMA transfer errors.
     *
     * (Note that this register change seems to survive a soft
     * reset.)
     *
     * This workaround does not apply to the 82599.
     */

    if (pDrvCtrl->teiDevType == TEI_DEVTYPE_82598)
        CSR_CLRBIT_4(pDev, TEI_GHECCR, TEI_GHECCR_CDAM0EE |
            TEI_GHECCR_CDEM0EE | TEI_GHECCR_CDAM1EE |
            TEI_GHECCR_CDEM1EE);

    return (OK);
    }

/*****************************************************************************
*
* teiIvarSet - program interrupt vector allocation registers
*
* This routine programs the 82598's interrupt vector allocation registers
* in order to allow an event on a given queue to trigger a particular
* interrupt source. For MSI-X interrupts, this controls which MSI-X vector
* is tied to a given queue. Note that there are fewer interrupt source
* bits and vectors than there are queues, so some of them will necessarily
* overlap.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void teiIvarSet
    (
    VXB_DEVICE_ID pDev,
    int queueType,
    int queueNum,
    int intSrc
    )
    {
    int reg = TEI_IVAR0;
    int val;

    /*
     * The first 64 slots are for RX queues, and the next
     * 32 slots are for TX queues.
     */

    if (queueType == TEI_TXQ)
        reg += 64;

    /* Each IVAR register has four slots, so divide the queue number by 4. */

    reg += queueNum >> 2;

    /* Set the source number and valid bit, then shift it to the right slot. */

    val = TEI_ALLOC0(intSrc) | TEI_IVAR_VALID0;
    val <<= queueNum & 3;

    CSR_SETBIT_4(pDev, reg, val);

    return;
    }

/*****************************************************************************
*
* teiIvar599Set - program interrupt vector allocation registers
*
* This routine programs the 82599's interrupt vector allocation registers
* in order to allow an event on a given queue to trigger a particular
* interrupt source. For MSI-X interrupts, this controls which MSI-X vector
* is tied to a given queue. Note that there are fewer interrupt source
* bits and vectors than there are queues, so some of them will necessarily
* overlap.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void teiIvar599Set
    (
    VXB_DEVICE_ID pDev,
    int queueType,
    int queueNum,
    int intSrc
    )
    {
    int reg = TEI_IVAR0;
    UINT32 val;
    UINT32 shift;

    /*
     * With the 82599, there are 64 IVAR registers, each with
     * four slots. Slots 0 and 2 are for RX queues and slots
     * 1 and 3 are for TX queues.
     */

    /*
     * Each IVAR register supports two RX queues and two TX
     * queues, so divide the queue number by 2.
     */

    reg += queueNum >> 1;

    /* Set the source number and valid bit, then shift it to the right slot. */

    val = TEI_ALLOC0(intSrc) | TEI_IVAR_VALID0;
    shift = (queueNum & 1) * 16;
    if (queueType == TEI_TXQ)
        val <<= (shift + 8);
    else
        val <<= shift;

    CSR_SETBIT_4(pDev, reg, val);

    return;
    }

/*****************************************************************************
*
* teiEndLoad - END driver entry point
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
* initialize the supported END capabilities.
*
* RETURNS: An END object pointer, or NULL on error, or 0 and the name
* of the device if the <loadStr> was empty.
*
* ERRNO: N/A
*/

LOCAL END_OBJ * teiEndLoad
    (
    char * loadStr,
    void * pArg
    )
    {
    TEI_DRV_CTRL *pDrvCtrl;
    VXB_DEVICE_ID pDev;

    /*
     * Use the endLib functions to support legacy protocols, if
     * available.
     */
    if (teiNetFuncs.funcs.formAddress == NULL)
	teiNetFuncs.funcs.formAddress = _func_endEtherAddressForm;
    if (teiNetFuncs.funcs.packetDataGet == NULL)
	teiNetFuncs.funcs.packetDataGet = _func_endEtherPacketDataGet;
    if (teiNetFuncs.funcs.addrGet == NULL)
	teiNetFuncs.funcs.addrGet = _func_endEtherPacketAddrGet;

    /* Make the MUX happy. */

    if (loadStr == NULL)
        return NULL;

    if (loadStr[0] == 0)
        {
        bcopy (TEI_NAME, loadStr, sizeof(TEI_NAME));
        return NULL;
        }

    pDev = pArg;
    pDrvCtrl = pDev->pDrvCtrl;

    pDrvCtrl->teiEndObj.hdrParse = end2EtherHdrParse;
    pDrvCtrl->teiEndObj.formLinkHdr = end2EtherIIFormLinkHdr;
    pDrvCtrl->teiEndObj.pNetPool = _end2_linkBufPool;

    if (END_OBJ_INIT (&pDrvCtrl->teiEndObj, NULL, pDev->pName,
        pDev->unitNumber, &teiNetFuncs.funcs,
        "Intel 82598/82599 10GbE VxBus END2 Driver") == ERROR)
        {
        logMsg("%s%d: END_OBJ_INIT failed\n", (int)TEI_NAME,
            pDev->unitNumber, 0, 0, 0, 0);
        return (NULL);
        }

    /*
     * Note: we lie about the interface speed here. While the
     * 82598/82599 is really a 10GbE device, VxWorks currently stores
     * the interface speed in a 32-bit variable, and 10000000000
     * is too big to fit in it.
     */

    endM2Init (&pDrvCtrl->teiEndObj, M2_ifType_ethernet_csmacd,
        pDrvCtrl->teiAddr, ETHER_ADDR_LEN, ETHERMTU, 1000000000,
        IFF_NOTRAILERS | IFF_SIMPLEX | IFF_MULTICAST | IFF_BROADCAST);

    /*
     * The ifHighSpeed field we can set correctly. And since the
     * speed is always 10gig, we never need to change it.
     */

    if (pDrvCtrl->teiEndObj.pMib2Tbl != NULL)
        {
        pDrvCtrl->teiEndObj.pMib2Tbl->m2Data.mibXIfTbl.ifHighSpeed = 1000;
        pDrvCtrl->teiEndObj.pMib2Tbl->m2Data.mibIfTbl.ifSpeed =
            pDrvCtrl->teiEndObj.mib2Tbl.ifSpeed;
        }

    /* Set up polling stats. */

    pDrvCtrl->teiEndStatsConf.ifPollInterval = sysClkRateGet();
    pDrvCtrl->teiEndStatsConf.ifEndObj = &pDrvCtrl->teiEndObj;
    pDrvCtrl->teiEndStatsConf.ifWatchdog = NULL;
    pDrvCtrl->teiEndStatsConf.ifValidCounters = (END_IFINUCASTPKTS_VALID |
        END_IFINMULTICASTPKTS_VALID | END_IFINBROADCASTPKTS_VALID |
        END_IFINOCTETS_VALID | END_IFINERRORS_VALID | END_IFINDISCARDS_VALID |
        END_IFOUTUCASTPKTS_VALID | END_IFOUTMULTICASTPKTS_VALID |
        END_IFOUTBROADCASTPKTS_VALID | END_IFOUTOCTETS_VALID |
        END_IFOUTERRORS_VALID);

    /* Set up capabilities. */
#if 0
    pDrvCtrl->teiCaps.cap_available = IFCAP_VLAN_MTU;
    pDrvCtrl->teiCaps.cap_enabled = IFCAP_VLAN_MTU;

    pDrvCtrl->teiCaps.csum_flags_tx = CSUM_IP|CSUM_TCP|CSUM_UDP;
    pDrvCtrl->teiCaps.csum_flags_tx |= CSUM_TCPv6|CSUM_UDPv6;
    pDrvCtrl->teiCaps.csum_flags_rx = CSUM_IP|CSUM_UDP|CSUM_TCP;
    pDrvCtrl->teiCaps.csum_flags_rx |= CSUM_TCPv6|CSUM_UDPv6;
    pDrvCtrl->teiCaps.cap_available |= IFCAP_TXCSUM|IFCAP_RXCSUM;
    pDrvCtrl->teiCaps.cap_enabled |= IFCAP_TXCSUM|IFCAP_RXCSUM;
#endif
    pDrvCtrl->teiCaps.cap_available = 0;
    pDrvCtrl->teiCaps.cap_enabled = 0;

    pDrvCtrl->teiCaps.csum_flags_tx = 0;
    pDrvCtrl->teiCaps.csum_flags_rx = 0;
#ifdef TEI_FORWARDER
    /*
     * The generic forwarder code expects the VLAN header still
     * in the packet. Don't strip it. Also, for now don't support
     * tag insertion...
     */
    if (!pDrvCtrl->fw_port.fw_capable)
#endif
	{
	pDrvCtrl->teiCaps.csum_flags_tx |= CSUM_VLAN;
	pDrvCtrl->teiCaps.csum_flags_rx |= CSUM_VLAN;
	pDrvCtrl->teiCaps.cap_available |= IFCAP_VLAN_HWTAGGING;
	pDrvCtrl->teiCaps.cap_enabled |= IFCAP_VLAN_HWTAGGING;
	}

    if (pDrvCtrl->teiMaxMtu == TEI_JUMBO_MTU)
        {
        pDrvCtrl->teiCaps.cap_available |= IFCAP_JUMBO_MTU;
        pDrvCtrl->teiCaps.cap_enabled |= IFCAP_JUMBO_MTU;
        }

    return (&pDrvCtrl->teiEndObj);
    }

/*****************************************************************************
*
* teiEndUnload - unload END driver instance
*
* This routine undoes the effects of teiEndLoad(). The END object
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

LOCAL STATUS teiEndUnload
    (
    END_OBJ * pEnd
    )
    {
    TEI_DRV_CTRL * pDrvCtrl;

    /* We must be stopped before we can be unloaded. */

    if (pEnd->flags & IFF_UP)
	return (ERROR);

    pDrvCtrl = (TEI_DRV_CTRL *)pEnd;

    /* terminate stats polling */
    wdDelete (pDrvCtrl->teiEndStatsConf.ifWatchdog);

    endM2Free (&pDrvCtrl->teiEndObj);

    END_OBJECT_UNLOAD (&pDrvCtrl->teiEndObj);

    return (EALREADY);  /* prevent freeing of pDrvCtrl */
    }

/*****************************************************************************
*
* teiEndHashTblPopulate - populate the multicast hash filter
*
* This function programs the 82598/82599 controller's multicast hash
* filter to receive frames sent to the multicast groups specified
* in the multicast address list attached to the END object. If
* the interface is in IFF_ALLMULTI mode, the filter will be
* programmed to receive all multicast packets by setting all the
* bits in the hash table to one.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void teiEndHashTblPopulate
    (
    TEI_DRV_CTRL * pDrvCtrl
    )
    {
    VXB_DEVICE_ID pDev;
    UINT32 addr[2];
    UINT32 h;
    int reg, bit;
    ETHER_MULTI * mCastNode = NULL;
    int parCnt;
    UINT32 parLo;
    UINT32 parHi;
    int i;

    pDev = pDrvCtrl->teiDev;

    if (pDrvCtrl->teiDevType == TEI_DEVTYPE_82599)
        {
        parCnt = TEI_PAR_CNT_599;
        parLo = TEI_PARL0_599;
        parHi = TEI_PARH0_599;
        }
    else
        {
        parCnt = TEI_PAR_CNT;
        parLo = TEI_PARL0;
        parHi = TEI_PARH0;
        }

    /* First, clear out the original filter. */

    CSR_CLRBIT_4(pDev, TEI_MCSTCTL, TEI_MCSTCTL_MFE);
    CSR_CLRBIT_4(pDev, TEI_FCTL, TEI_FCTL_MPE);

    for (i = 1; i < TEI_PAR_CNT; i++)
        {
        CSR_WRITE_4(pDev, TEI_PARL0 + (i * sizeof(UINT64)), 0);
        CSR_WRITE_4(pDev, TEI_PARH0 + (i * sizeof(UINT64)), 0);
        }

    for (i = 0; i < TEI_MAR_CNT; i++)
        CSR_WRITE_4(pDev, TEI_MAR0 + (i * sizeof(UINT32)), 0);

    if (pDrvCtrl->teiEndObj.flags & (IFF_ALLMULTI|IFF_PROMISC))
        {
        /* set allmulticast mode */
        CSR_SETBIT_4(pDev, TEI_FCTL, TEI_FCTL_MPE);
        return;
        }

    CSR_SETBIT_4(pDev, TEI_MCSTCTL, TEI_MCSTCTL_MFE);

    /* Now repopulate it. */

    i = 1;

    for (mCastNode =
        (ETHER_MULTI *) lstFirst (&pDrvCtrl->teiEndObj.multiList);
         mCastNode != NULL;
         mCastNode = (ETHER_MULTI *) lstNext (&mCastNode->node))
        {
        if (i < parCnt)
            {
            bcopy (mCastNode->addr, (char *)addr, ETHER_ADDR_LEN);
            CSR_WRITE_4(pDev, parLo + (i * sizeof(UINT64)),
                htole32(addr[0]));
            CSR_WRITE_4(pDev, parHi + (i * sizeof(UINT64)),
                (htole32(addr[1]) & 0xFFFF) | TEI_PARH_AV);
            i++;
            }
        else
            {
            h = (mCastNode->addr[4] >> 4) | (mCastNode->addr[5] << 4);
            reg = (h >> 5) & 0x7F;
            bit = h & 0x1F;
            CSR_SETBIT_4(pDev, TEI_MAR0 + (reg * sizeof(UINT32)), 1 << bit);
            }
        }

    return;
    }

/*****************************************************************************
*
* teiEndMCastAddrAdd - add a multicast address for the device
*
* This routine adds a multicast address to whatever the driver
* is already listening for.  It then resets the address filter.
*
* RETURNS: OK or ERROR.
*
* ERRNO: N/A
*/

LOCAL STATUS teiEndMCastAddrAdd
    (
    END_OBJ * pEnd,
    char * pAddr
    )
    {
    int retVal;
    TEI_DRV_CTRL * pDrvCtrl = (TEI_DRV_CTRL *) pEnd;

    semTake (pDrvCtrl->teiDevSem, WAIT_FOREVER);

    if (!(pDrvCtrl->teiEndObj.flags & IFF_UP))
        {
        semGive (pDrvCtrl->teiDevSem);
        return (OK);
        }

    retVal = etherMultiAdd (&pEnd->multiList, pAddr);

    if (retVal == ENETRESET)
        {
        pEnd->nMulti++;
        teiEndHashTblPopulate ((TEI_DRV_CTRL *)pEnd);
        }

    semGive (pDrvCtrl->teiDevSem);
    return (OK);
    }

/*****************************************************************************
*
* teiEndMCastAddrDel - delete a multicast address for the device
*
* This routine removes a multicast address from whatever the driver
* is listening for.  It then resets the address filter.
*
* RETURNS: OK or ERROR.
*
* ERRNO: N/A
*/

LOCAL STATUS teiEndMCastAddrDel
    (
    END_OBJ * pEnd,
    char * pAddr
    )
    {
    int retVal;
    TEI_DRV_CTRL * pDrvCtrl = (TEI_DRV_CTRL *) pEnd;

    semTake (pDrvCtrl->teiDevSem, WAIT_FOREVER);

    if (!(pDrvCtrl->teiEndObj.flags & IFF_UP))
        {
        semGive (pDrvCtrl->teiDevSem);
        return (OK);
        }

    retVal = etherMultiDel (&pEnd->multiList, pAddr);

    if (retVal == ENETRESET)
        {
        pEnd->nMulti--;
        teiEndHashTblPopulate ((TEI_DRV_CTRL *)pEnd);
        }

    semGive (pDrvCtrl->teiDevSem);
    return (OK);
    }

/*****************************************************************************
*
* teiEndMCastAddrGet - get the multicast address list for the device
*
* This routine gets the multicast list of whatever the driver
* is already listening for.
*
* RETURNS: OK or ERROR.
*
* ERRNO: N/A
*/

LOCAL STATUS teiEndMCastAddrGet
    (
    END_OBJ * pEnd,
    MULTI_TABLE * pTable
    )
    {
    STATUS rval;
    TEI_DRV_CTRL * pDrvCtrl = (TEI_DRV_CTRL *) pEnd;

    semTake (pDrvCtrl->teiDevSem, WAIT_FOREVER);

    if (!(pDrvCtrl->teiEndObj.flags & IFF_UP))
        {
        semGive (pDrvCtrl->teiDevSem);
        return (OK);
        }

    rval = etherMultiGet (&pEnd->multiList, pTable);

    semGive (pDrvCtrl->teiDevSem);
    return (rval);
    }

/*****************************************************************************
*
* teiEndStatsDump - return polled statistics counts
*
* This routine is automatically invoked periodically by the polled
* statistics watchdog.
*
* RETURNS: always OK
*
* ERRNO: N/A
*/

LOCAL STATUS teiEndStatsDump
    (
    TEI_DRV_CTRL * pDrvCtrl
    )
    {
    END_IFCOUNTERS *    pEndStatsCounters;
    VXB_DEVICE_ID pDev;
    UINT32 tmp;

    semTake (pDrvCtrl->teiDevSem, WAIT_FOREVER);

    if (!(pDrvCtrl->teiEndObj.flags & IFF_UP))
        {
        semGive (pDrvCtrl->teiDevSem);
        return (ERROR);
        }

    pDev = pDrvCtrl->teiDev;

    pEndStatsCounters = &pDrvCtrl->teiEndStatsCounters;

    if (pDrvCtrl->teiDevType == TEI_DEVTYPE_82598)
        {

        /* Get number of RX'ed octets */

        tmp = CSR_READ_4(pDev, TEI_GORC);
        pEndStatsCounters->ifInOctets = tmp;

        /* Get number of TX'ed octets */

        tmp = CSR_READ_4(pDev, TEI_GOTC);
        pEndStatsCounters->ifOutOctets = tmp;
        }
    else
        {

        /* Get number of RX'ed octets */

        tmp = CSR_READ_4(pDev, TEI_GORCL);
        pEndStatsCounters->ifInOctets = tmp;
        tmp = CSR_READ_4(pDev, TEI_GORC); /* Read to clear counter */

        /* Get number of TX'ed octets */

        tmp = CSR_READ_4(pDev, TEI_GOTCL);
        pEndStatsCounters->ifOutOctets = tmp;
        tmp = CSR_READ_4(pDev, TEI_GOTC); /* Read to clear counter */
        }

    /* Get RX'ed unicasts, broadcasts, multicasts */

    tmp = CSR_READ_4(pDev, TEI_GPRC);
    pEndStatsCounters->ifInUcastPkts = tmp;
    tmp = CSR_READ_4(pDev, TEI_BPRC);
    pEndStatsCounters->ifInBroadcastPkts = tmp;
    tmp = CSR_READ_4(pDev, TEI_MPRC);
    pEndStatsCounters->ifInMulticastPkts = tmp;
    pEndStatsCounters->ifInUcastPkts -= (pEndStatsCounters->ifInMulticastPkts +
        pEndStatsCounters->ifInBroadcastPkts);

    /* Get TX'ed unicasts, broadcasts, multicasts */

    tmp = CSR_READ_4(pDev, TEI_GPTC);
    pEndStatsCounters->ifOutUcastPkts = tmp;
    tmp = CSR_READ_4(pDev, TEI_BPTC);
    pEndStatsCounters->ifOutBroadcastPkts = tmp;
    tmp = CSR_READ_4(pDev, TEI_MPTC);
    pEndStatsCounters->ifOutMulticastPkts = tmp;
    pEndStatsCounters->ifOutUcastPkts -=
        (pEndStatsCounters->ifOutMulticastPkts +
        pEndStatsCounters->ifOutBroadcastPkts);

    semGive (pDrvCtrl->teiDevSem);

    return (OK);
    }

/*****************************************************************************
*
* teiEndIoctl - the driver I/O control routine
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

LOCAL int teiEndIoctl
    (
    END_OBJ * pEnd,
    int cmd,
    caddr_t data
    )
    {
    TEI_DRV_CTRL * pDrvCtrl;
    END_MEDIALIST * mediaList;
    END_CAPABILITIES * hwCaps;
    END_MEDIA * pMedia;
    END_RCVJOBQ_INFO * qinfo;
    UINT32 nQs;
    VXB_DEVICE_ID pDev;
    INT32 value;
    int error = OK;

    pDrvCtrl = (TEI_DRV_CTRL *)pEnd;
    pDev = pDrvCtrl->teiDev;

    if (cmd != EIOCPOLLSTART && cmd != EIOCPOLLSTOP)
	semTake (pDrvCtrl->teiDevSem, WAIT_FOREVER);

    switch (cmd)
        {
	case EIOCGSTYLE:
	    if (data == NULL)
		error = EINVAL;
	    else
		*(int *)data = END_STYLE_END2;
	    break;
        case EIOCSADDR:
            if (data == NULL)
                error = EINVAL;
            else
                {
                bcopy ((char *)data, (char *)pDrvCtrl->teiAddr,
                    ETHER_ADDR_LEN);
                bcopy ((char *)data,
                    (char *)pEnd->mib2Tbl.ifPhysAddress.phyAddress,
                    ETHER_ADDR_LEN);
                if (pEnd->pMib2Tbl != NULL)
                    bcopy ((char *)data,
                        (char *)pEnd->pMib2Tbl->m2Data.mibIfTbl.ifPhysAddress.phyAddress,
                        ETHER_ADDR_LEN);
                }
            teiEndRxConfig (pDrvCtrl);
            break;

        case EIOCGADDR:
            if (data == NULL)
                error = EINVAL;
            else
                bcopy ((char *)pDrvCtrl->teiAddr, (char *)data,
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

            teiEndRxConfig (pDrvCtrl);

            break;

        case EIOCGFLAGS:
            if (data == NULL)
                error = EINVAL;
            else
                *(long *)data = END_FLAGS_GET(pEnd);

            break;

        case EIOCMULTIADD:
            error = teiEndMCastAddrAdd (pEnd, (char *) data);
            break;

        case EIOCMULTIDEL:
            error = teiEndMCastAddrDel (pEnd, (char *) data);
            break;

        case EIOCMULTIGET:
            error = teiEndMCastAddrGet (pEnd, (MULTI_TABLE *) data);
            break;

        case EIOCPOLLSTART:
	    if (!TEI_MUX_BOTH_ENABLED(pDrvCtrl))
		{
		error = EINVAL;
		break;
		}
            pDrvCtrl->teiPolling = TRUE;
            pDrvCtrl->teiIntMask = CSR_READ_4(pDev, TEI_EIMS);
            CSR_WRITE_4(pDev, TEI_EIMC, 0xFFFFFFFF);

            /*
             * We may have been asked to enter polled mode while
             * there are transmissions pending. This is a problem,
             * because the polled transmit routine expects that
             * the TX ring will be empty when it's called. In
             * order to guarantee this, we have to drain the TX
             * ring here. We could also just plain reset and
             * reinitialize the transmitter, but this is faster.
             */

            while (CSR_READ_4(pDev, TEI_TDH) != CSR_READ_4(pDev, TEI_TDT))
               ;

            while (pDrvCtrl->teiTxFree < TEI_TX_DESC_CNT)
                {
#ifdef TEI_VXB_DMA_BUF
                VXB_DMA_MAP_ID pMap;
#endif
		Ipcom_pkt * pkt;

                pkt = pDrvCtrl->teiTxPkt[pDrvCtrl->teiTxCons];

                if (pkt != NULL)
                    {
#ifdef TEI_VXB_DMA_BUF
                    pMap = pDrvCtrl->teiTxPktMap[pDrvCtrl->teiTxCons];
                    vxbDmaBufMapUnload (pDrvCtrl->teiPktTag, pMap);
#endif
		    VXIPCOM_PKT_DONE (pkt);
                    pDrvCtrl->teiTxPkt[pDrvCtrl->teiTxCons] = IP_NULL;
                    }

                pDrvCtrl->teiTxFree++;
                TEI_INC_DESC (pDrvCtrl->teiTxCons, TEI_TX_DESC_CNT);
                }

            break;

        case EIOCPOLLSTOP:
	    if (!TEI_MUX_BOTH_ENABLED(pDrvCtrl))
		{
		error = EINVAL;
		break;
		}
            pDrvCtrl->teiPolling = FALSE;
            CSR_WRITE_4(pDev, TEI_EIMS, pDrvCtrl->teiIntMask);

            break;

        case EIOCGMIB2233:
        case EIOCGMIB2:
            error = endM2Ioctl (&pDrvCtrl->teiEndObj, cmd, data);
            break;

        case EIOCGPOLLCONF:
            if (data == NULL)
                error = EINVAL;
            else
                *((END_IFDRVCONF **)data) = &pDrvCtrl->teiEndStatsConf;
            break;

        case EIOCGPOLLSTATS:
            if (data == NULL)
                error = EINVAL;
            else
                {
                error = teiEndStatsDump(pDrvCtrl);
                if (error == OK)
                    *((END_IFCOUNTERS **)data) = &pDrvCtrl->teiEndStatsCounters;
                }
            break;

        case EIOCGMEDIALIST:
            if (data == NULL)
                {
                error = EINVAL;
                break;
                }
            if (pDrvCtrl->teiMediaList->endMediaListLen == 0)
                {
                error = ENOTSUP;
                break;
                }

            mediaList = (END_MEDIALIST *)data;
            if (mediaList->endMediaListLen <
                pDrvCtrl->teiMediaList->endMediaListLen)
                {
                mediaList->endMediaListLen =
                    pDrvCtrl->teiMediaList->endMediaListLen;
                error = ENOSPC;
                break;
                }

            bcopy((char *)pDrvCtrl->teiMediaList, (char *)mediaList,
                  sizeof(END_MEDIALIST) + (sizeof(UINT32) *
                  pDrvCtrl->teiMediaList->endMediaListLen));
            break;

        case EIOCGIFMEDIA:
            if (data == NULL)
                error = EINVAL;
            else
                {
                pMedia = (END_MEDIA *)data;
                pMedia->endMediaActive = pDrvCtrl->teiCurMedia;
                pMedia->endMediaStatus = pDrvCtrl->teiCurStatus;
                }
            break;

        case EIOCSIFMEDIA:
            if (data == NULL)
                error = EINVAL;
            else
                {
                pMedia = (END_MEDIA *)data;
                pDrvCtrl->teiCurMedia = pMedia->endMediaActive;
                teiLinkUpdate (pDrvCtrl->teiDev);
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
            hwCaps->csum_flags_tx = pDrvCtrl->teiCaps.csum_flags_tx;
            hwCaps->csum_flags_rx = pDrvCtrl->teiCaps.csum_flags_rx;
            hwCaps->cap_available = pDrvCtrl->teiCaps.cap_available;
            hwCaps->cap_enabled = pDrvCtrl->teiCaps.cap_enabled;
            break;

        case EIOCSIFCAP:
            hwCaps = (END_CAPABILITIES *)data;
            if (hwCaps == NULL)
                {
                error = EINVAL;
                break;
                }

            /*
             * We must enable or disable RX VLAN tag extraction
             * as specified, since leaving it on all the time will
             * always cause the chip to strip the VLAN tag field
             * from the frame.
             */

            pDrvCtrl->teiCaps.cap_enabled =
		(hwCaps->cap_enabled & pDrvCtrl->teiCaps.cap_available);

	    if (pDrvCtrl->teiDevType == TEI_DEVTYPE_82598)
		{
		if (pDrvCtrl->teiCaps.cap_enabled & IFCAP_VLAN_HWTAGGING)
		    CSR_SETBIT_4(pDev, TEI_VLNCTL, TEI_VLNCTL_VME);
		else
		    CSR_CLRBIT_4(pDev, TEI_VLNCTL, TEI_VLNCTL_VME);
		}
	    else if (pDrvCtrl->teiDevType == TEI_DEVTYPE_82599)
		{
		if (pDrvCtrl->teiCaps.cap_enabled & IFCAP_VLAN_HWTAGGING)
		    CSR_SETBIT_4(pDev, TEI_RXDCTL, TEI_RXDCTL_VME);
		else
		    CSR_CLRBIT_4(pDev, TEI_RXDCTL, TEI_RXDCTL_VME);
		}

            break;

        case EIOCGIFMTU:
            if (data == NULL)
                error = EINVAL;
            else
                *(INT32 *)data = pEnd->mib2Tbl.ifMtu;
            break;

        case EIOCSIFMTU:
            value = (INT32)data;
            if (value <= 0 || value > pDrvCtrl->teiMaxMtu)
		{
                error = EINVAL;
		break;
		}
            pEnd->mib2Tbl.ifMtu = value;
            if (pEnd->pMib2Tbl != NULL)
                pEnd->pMib2Tbl->m2Data.mibIfTbl.ifMtu = value;
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
		qinfo->qIds[0] = pDrvCtrl->teiJobQueue;
	    break;

#ifdef TEI_FORWARDER
	case EIOCGFWPORT:
            if (data == NULL)
		{
                error = EINVAL;
		break;
		}
	    if (pDrvCtrl->fw_port.fw_capable)
		*(void**)data = &pDrvCtrl->fw_port;
	    else
		*(void**)data = NULL;
	    break;
#endif

        default:
            error = EINVAL;
            break;
        }

    if (cmd != EIOCPOLLSTART && cmd != EIOCPOLLSTOP)
	semGive (pDrvCtrl->teiDevSem);

    return (error);
    }

/*****************************************************************************
*
* teiEndRxConfig - configure the 82598/82599 RX filter
*
* This is a helper routine used by teiEndIoctl() and teiEndStart() to
* configure the controller's RX filter. The unicast address filter is
* programmed with the currently configured MAC address, and the RX
* configuration is set to allow unicast and broadcast frames to be
* received. If the interface is in IFF_PROMISC mode, the FCTL_UPE
* bit is set, which allows all packets to be received.
*
* The teiEndHashTblPopulate() routine is also called to update the
* multicast filter.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void teiEndRxConfig
    (
    TEI_DRV_CTRL * pDrvCtrl
    )
    {
    UINT32 addr[2];
    VXB_DEVICE_ID pDev;
    UINT32 parLo;
    UINT32 parHi;

    if (pDrvCtrl->teiDevType == TEI_DEVTYPE_82599)
        {
        parLo = TEI_PARL0_599;
        parHi = TEI_PARH0_599;
        }
    else
        {
        parLo = TEI_PARL0;
        parHi = TEI_PARH0;
        }

    pDev = pDrvCtrl->teiDev;

    /* Copy the address to a buffer we know is 32-bit aligned. */

    bcopy ((char *)pDrvCtrl->teiAddr, (char *)addr, ETHER_ADDR_LEN);

    /*
     * Init our MAC address.
     */

    CSR_WRITE_4(pDev, parLo, htole32(addr[0]));
    CSR_WRITE_4(pDev, parHi, (htole32(addr[1]) & 0xFFFF) | TEI_PARH_AV);

    /* Program the RX filter to receive unicasts and broadcasts */

    CSR_SETBIT_4(pDev, TEI_FCTL, TEI_FCTL_BAM);

    /* Enable promisc mode, if specified. */

    if (pDrvCtrl->teiEndObj.flags & IFF_PROMISC)
        CSR_SETBIT_4(pDev, TEI_FCTL, TEI_FCTL_UPE);
    else
        CSR_CLRBIT_4(pDev, TEI_FCTL, TEI_FCTL_UPE);

    /* Program the multicast filter. */

    teiEndHashTblPopulate (pDrvCtrl);

    return;
    }

/*****************************************************************************
*
* teiEndStart - start the device
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

LOCAL STATUS teiEndStart
    (
    END_OBJ * pEnd
    )
    {
    TEI_DRV_CTRL * pDrvCtrl;
    VXB_DEVICE_ID pDev;
    VXB_INST_PARAM_VALUE val;
    HEND_RX_QUEUE_PARAM * pRxQueue;
    TEI_ADV_RDESC * pDesc;
    Ipcom_pkt * pkt;
    Ip_u8 * buf;
    
#ifdef TEI_VXB_DMA_BUF
    VXB_DMA_MAP_ID pMap;
#elif defined (TEI_GUEST_VIRT_TO_PHYS)
    uint64_t paddr;
#else
    ULONG addr;
#endif

#ifdef TEI_FORWARDER
    int base;
#endif
    int i;

    pDrvCtrl = (TEI_DRV_CTRL *)pEnd;
    pDev = pDrvCtrl->teiDev;

    semTake (pDrvCtrl->teiDevSem, WAIT_FOREVER);
    END2_TX_SEM_TAKE (pEnd, WAIT_FOREVER);

    if (pEnd->flags & IFF_UP)
        {
        END2_TX_SEM_GIVE (pEnd);
        semGive (pDrvCtrl->teiDevSem);
        return (OK);
        }

    /* Initialize job queues */

    pDrvCtrl->teiJobQueue = netJobQueueId;

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
            pDrvCtrl->teiJobQueue = pRxQueue->jobQueId;
        }

    QJOB_SET_PRI(&pDrvCtrl->teiIntJob, NET_TASK_QJOB_PRI);
    pDrvCtrl->teiIntJob.func = teiEndIntHandle;

    vxAtomicSet (&pDrvCtrl->teiIntPending, FALSE);

    teiReset (pDev);

    /*
     * Allocate the TX polling mode packet.
     * We use the IPCOM_FLAG_FC_EXACT flag so that we can use the
     * data_freefunc_cookie from this packet to free the buffers in
     * the RX ring.
     */

    if (TEI_MUX_ANY_ENABLED(pDrvCtrl))
	{
	pkt =  vxipcom_pkt_malloc (pDrvCtrl->teiMaxMtu, IPCOM_FLAG_FC_EXACT);
	pDrvCtrl->teiEndObj.pollPkt = pkt;
	if (pkt == IP_NULL)
	    {
	    if (_func_logMsg)
		_func_logMsg("%s%d: cannot allocate pollPkt\n",
			     (int)TEI_NAME, pDev->unitNumber, 0,0,0,0);
	    END2_TX_SEM_GIVE(pEnd);
	    return ERROR;
	    }
	/*
	 * This one's not going to have extra headers prepended, just
	 * set pkt->start to 2 to usually get the alignment correct.
	 */
	pkt->start = 2;
	}


    /* Set up the RX ring. */

    if (TEI_MUX_RX_ENABLED(pDrvCtrl))
	{
	bzero ((char *)pDrvCtrl->teiRxDescMem,
	       sizeof(TEI_ADV_RDESC) * TEI_RX_DESC_CNT);

	for (i = 0; i < TEI_RX_DESC_CNT; i++)
	    {
	    pkt = vxipcom_pkt_malloc (pDrvCtrl->teiMaxMtu,
				      IPCOM_FLAG_FC_EXACT);
	    if (pkt == NULL)
		{
		/* TODO: cleanup */
		END2_TX_SEM_GIVE (pEnd);
		semGive (pDrvCtrl->teiDevSem);
		return (ERROR);
		}

	    pDrvCtrl->teiRxPkt[i] = pkt;
	    buf = pkt->data + TEI_RXPKT_OFFSET;

	    pDesc = &pDrvCtrl->teiRxDescMem[i];
#ifdef TEI_VXB_DMA_BUF
	    pMap = pDrvCtrl->teiRxPktMap[i];

	    vxbDmaBufMapLoad (pDev, pDrvCtrl->teiPktTag, pMap,
			      buf, pDrvCtrl->teiMaxMtu + TEI_MTU_ADJUST, 0);

	    /*
	     * Currently, pointers in VxWorks are always 32 bits, even on
	     * 64-bit architectures. This means that for the time being,
	     * we can save a few cycles by only setting the addrlo field
	     * in DMA descriptors and leaving the addrhi field set to 0.
	     * When pointers become 64 bits wide, this will need to be
	     * changed.
	     */

#ifdef TEI_64
	    pDesc->read.tei_addrlo =
		htole32(TEI_ADDR_LO(pMap->fragList[0].frag));
	    pDesc->read.tei_addrhi =
		htole32(TEI_ADDR_HI(pMap->fragList[0].frag));
#else
	    pDesc->read.tei_addrlo = htole32((UINT32)pMap->fragList[0].frag);
	    pDesc->read.tei_addrhi = 0;
#endif
#elif defined (TEI_GUEST_VIRT_TO_PHYS)
	    paddr = teiVirtToPhys(buf);
	    pDesc->read.tei_addrlo = (UINT32)paddr; /* L.E. */
	    pDesc->read.tei_addrhi = (UINT32)(paddr >> 32); /* L.E. */
	    
#else  /* TEI_VXB_DMA_BUF */
	    addr = (ULONG)buf;
	    if (cacheUserFuncs.virtToPhysRtn)
		addr = (ULONG)cacheUserFuncs.virtToPhysRtn (buf);
	    pDesc->read.tei_addrlo = htole32(((UINT32)addr));
	    pDesc->read.tei_addrhi = 0;
#endif /* TEI_VXB_DMA_BUF */

	    pDesc->read.tei_hdrlo = 0;
	    pDesc->read.tei_hdrhi = 0;
	    }
	}

    /* Set up the TX ring. */
    if (TEI_MUX_TX_ENABLED(pDrvCtrl))
	{
	bzero ((char *)pDrvCtrl->teiTxDescMem,
	       sizeof(TEI_ADV_TDESC) * TEI_TX_DESC_CNT);
	}

#ifdef TEI_VXB_DMA_BUF
    /* Load the maps for the RX and TX DMA ring. */

    if (TEI_MUX_RX_ENABLED(pDrvCtrl))
	vxbDmaBufMapLoad (pDev, pDrvCtrl->teiRxDescTag,
			  pDrvCtrl->teiRxDescMap, pDrvCtrl->teiRxDescMem,
			  sizeof(TEI_ADV_RDESC) * TEI_RX_DESC_CNT, 0);

    if (TEI_MUX_TX_ENABLED(pDrvCtrl))
	vxbDmaBufMapLoad (pDev, pDrvCtrl->teiTxDescTag,
			  pDrvCtrl->teiTxDescMap, pDrvCtrl->teiTxDescMem,
			  sizeof(TEI_ADV_TDESC) * TEI_TX_DESC_CNT, 0);
#endif

    /* Initialize state */

    pDrvCtrl->teiRxIdx = 0;
    pDrvCtrl->teiMoreRx = 0;
    pDrvCtrl->teiTxStall = FALSE;
    pDrvCtrl->teiTxProd = 0;
    pDrvCtrl->teiTxCons = 0;
    pDrvCtrl->teiTxFree = TEI_TX_DESC_CNT;
    pDrvCtrl->teiLastCtx = 0xFFFF;
    pDrvCtrl->teiLastOffsets = -1;
    pDrvCtrl->teiLastVlan = 0;

    /*
     * FORWARDER - DLK - was code here to allocate RX/TX DMA rings for
     * the forwarders using this port; but we want the forwarders
     * themselves to do that.
     */

    /* Program the RX filter. */

    teiEndRxConfig (pDrvCtrl);

    /* Initialize RX and TX queue zero */

    if (TEI_MUX_RX_ENABLED(pDrvCtrl))
	CSR_CLRBIT_4(pDev, TEI_RXDCTL, TEI_RXDCTL_ENABLE);
    if (TEI_MUX_TX_ENABLED(pDrvCtrl))
	CSR_CLRBIT_4(pDev, TEI_TXDCTL, TEI_TXDCTL_ENABLE);

    /* Set frame sizes */

    if (pDrvCtrl->teiMaxMtu == TEI_JUMBO_MTU)
        {
	/* Note that MAXFRS is called MHADD in the 82598 documentation */
        CSR_WRITE_4(pDev, TEI_MAXFRS, TEI_MFS(9018));
        CSR_SETBIT_4(pDev, TEI_HLREG0, TEI_HLREG0_JUMBOEN);
        }
    else
        {
        CSR_WRITE_4(pDev, TEI_MAXFRS, TEI_MFS(1518));
        CSR_CLRBIT_4(pDev, TEI_HLREG0, TEI_HLREG0_JUMBOEN);
        }

    if (TEI_MUX_RX_ENABLED(pDrvCtrl))
	{
	if (pDrvCtrl->teiMaxMtu == TEI_JUMBO_MTU)
	    {
	    CSR_WRITE_4(pDev, TEI_SRRCTL, 10);
	    }
	else
	    {
	    CSR_WRITE_4(pDev, TEI_SRRCTL, 2);
	    }

	/* Select advanced RX descriptors and enable VLAN stripping */
	CSR_SETBIT_4(pDev, TEI_SRRCTL, TEI_DESCTYPE_ADV_ONE);

	if (pDrvCtrl->teiDevType == TEI_DEVTYPE_82599)
	    {
	    if (pDrvCtrl->teiCaps.cap_enabled & IFCAP_VLAN_HWTAGGING)
		CSR_SETBIT_4(pDev, TEI_RXDCTL, TEI_RXDCTL_VME);
	    else
		CSR_CLRBIT_4(pDev, TEI_RXDCTL, TEI_RXDCTL_VME);
	    }
	}

    if (pDrvCtrl->teiDevType == TEI_DEVTYPE_82598)
	{
	if (pDrvCtrl->teiCaps.cap_enabled & IFCAP_VLAN_HWTAGGING)
	    CSR_SETBIT_4(pDev, TEI_VLNCTL, TEI_VLNCTL_VME);
	else
	    CSR_CLRBIT_4(pDev, TEI_VLNCTL, TEI_VLNCTL_VME);
	}

#ifdef TEI_SECRC
    /* Hmm, the TEI_HLREG0_RXCRCSTRP bit is not documented for
       the HLREG0 register for 82599, although the documentation
       for RDRXCTL.CRCStrip indicates that RDRXCTL.CRCStrip and
       HLREG0.RXCRCSTRIP must be set the same (although they have
       different defaults!) */
    CSR_SETBIT_4(pDev, TEI_HLREG0, TEI_HLREG0_RXCRCSTRP);
    if (pDrvCtrl->teiDevType == TEI_DEVTYPE_82599)
	{
	CSR_SETBIT_4(pDev, TEI_RDRXCTL, TEI_RDRXCTL99_CRCSTRIP);
	}
#else
    CSR_CLRBIT_4(pDev, TEI_HLREG0, TEI_HLREG0_RXCRCSTRP);
    if (pDrvCtrl->teiDevType == TEI_DEVTYPE_82599)
	{
	CSR_CLRBIT_4(pDev, TEI_RDRXCTL, TEI_RDRXCTL99_CRCSTRIP);
	}
#endif /* TEI_SECRC */

#ifdef TEI_FORWARDER
    /*
     * There was code here to initialize and enable all the RX
     * rings polled by forwarders (for forwarder-capable ports).
     * (Regs: srrctl, rdlen, rdbah/l, rxdctl).
     *
     * Here we just disable all forwarder RX queues, forwarders tasks
     * will allocate and enable their own queues.
     * Also:
     * - set RSS seed
     * - Redirect all RSS traffic to queue 0
     * - write to MRQC to enable RSS
     * - write to PCSD to enable RSS value in RX descriptors
     */
    if (!pDrvCtrl->fw_port.fw_capable)
	goto nonforwarder_l1;

    base = TEI_MUX_RX_ENABLED(pDrvCtrl) ? 1 : 0;
    for (i = base ; i < pDrvCtrl->fw_port.numRxQs + base; ++i)
	{
	ULONG offset = TEI_RXDCTL + i * TEI_RXQ_IVL_99;
	CSR_CLRBIT_4(pDev, offset, TEI_RXDCTL_ENABLE);
	}

    teiFwRssReseed (pDrvCtrl, teiFwRssKey);

    /*
     * When starting, redirect all packets to queue zero.
     * Do this before enabling PCSD.
     */
    teiFwRedirect (&pDrvCtrl->fw_port, IP_FALSE);

    /* Set RXCSUM.PCSD to report RSS instead of packet checksum/IP ID */

    /*
     * For 82599, we want the non-virtualized RSS-only case, which
     * gives up to 16 queues.
     *
     * When the forwarder is enabled, we set the indirection table
     * so that all entries point at queue 1 (or distribute across
     * queues 1, 2, ... if more than one forwarder polls the port)
     * and enable RSS. If a packet is not RSSable, it's OK if it
     * goes directly to queue zero.  When the forwarder is disabled,
     * we disable RSS, in which case all packets go to queue zero.
     * Change: In S-AMP case, the forwarder now polls queue 0 too.
     *
     * Which of the RSS field hash enables do we want? For the L4 TCP
     * termination case, if we enable the TCP ports in the hash,
     * the initial SYN that establishes the connection might go
     * to a different queue than subsequent traffic on the connection...
     */

    CSR_WRITE_4(pDev, TEI_MRQC,
		TEI_MRQC_MODE_RSS_ONLY |
		TEI_MRQC_RSS_EN_TCPIPV4 |
		TEI_MRQC_RSS_EN_IPV4 |
		TEI_MRQC_RSS_EN_IPV6 |
		TEI_MRQC_RSS_EN_TCPIPV6 |
		TEI_MRQC_RSS_EN_UDPIPV4 |
		TEI_MRQC_RSS_EN_UDPIPV6
                );

    CSR_SETBIT_4(pDev, TEI_RXCSUM, TEI_RXCSUM_PCSD);

nonforwarder_l1:
    ;
#endif /* TEI_FORWARDER */

    /* Initialize RX descriptor ring size */

    if (TEI_MUX_RX_ENABLED(pDrvCtrl))
	{
	CSR_WRITE_4(pDev, TEI_RDLEN, TEI_RX_DESC_CNT * sizeof(TEI_ADV_RDESC));

	/* Set the base address of the RX ring */

#ifdef TEI_VXB_DMA_BUF
	CSR_WRITE_4(pDev, TEI_RDBAH,
		    TEI_ADDR_HI(pDrvCtrl->teiRxDescMap->fragList[0].frag));
	CSR_WRITE_4(pDev, TEI_RDBAL,
		    TEI_ADDR_LO(pDrvCtrl->teiRxDescMap->fragList[0].frag));
#elif defined (TEI_GUEST_VIRT_TO_PHYS)
	paddr = teiVirtToPhys (pDrvCtrl->teiRxDescMem);
	CSR_WRITE_4(pDev, TEI_RDBAH, (uint32_t)(paddr >> 32));
	CSR_WRITE_4(pDev, TEI_RDBAL, (uint32_t)paddr);
#else
	CSR_WRITE_4(pDev, TEI_RDBAH, 0);
	addr = (ULONG)pDrvCtrl->teiRxDescMem;
	if (cacheUserFuncs.virtToPhysRtn)
	    addr = (ULONG)cacheUserFuncs.virtToPhysRtn ((void *)addr);
	CSR_WRITE_4(pDev, TEI_RDBAL, ((UINT32)addr));
#endif

	CSR_CLRBIT_4(pDev, TEI_RXDCTL, TEI_RXDCTL_PTHRESH);
	CSR_SETBIT_4(pDev, TEI_RXDCTL, TEI_PTHRESH(0x20));
	}

    /* Initialize transmitter. */

    if (TEI_MUX_TX_ENABLED(pDrvCtrl))
	{
	/* Initialize TX descriptor ring size */

	CSR_WRITE_4(pDev, TEI_TDLEN, TEI_TX_DESC_CNT * sizeof(TEI_ADV_TDESC));

	/* Set producer and consumer indexes */

	CSR_WRITE_4(pDev, TEI_TDH, 0);
	CSR_WRITE_4(pDev, TEI_TDT, 0);

    /* Set the base address of the TX ring */

#ifdef TEI_VXB_DMA_BUF
	CSR_WRITE_4(pDev, TEI_TDBAH,
		    TEI_ADDR_HI(pDrvCtrl->teiTxDescMap->fragList[0].frag));
	CSR_WRITE_4(pDev, TEI_TDBAL,
		    TEI_ADDR_LO(pDrvCtrl->teiTxDescMap->fragList[0].frag));
	CSR_WRITE_4(pDev, TEI_TDBAH, 0);
#elif defined (TEI_GUEST_VIRT_TO_PHYS)
	paddr = teiVirtToPhys(pDrvCtrl->teiTxDescMem);
	CSR_WRITE_4(pDev, TEI_TDBAH, (uint32_t)(paddr >> 32));
	CSR_WRITE_4(pDev, TEI_TDBAL, (uint32_t)paddr);
#else
	CSR_WRITE_4(pDev, TEI_TDBAH, 0);
	addr = (ULONG)pDrvCtrl->teiTxDescMem;
	if (cacheUserFuncs.virtToPhysRtn)
	    addr = (ULONG)cacheUserFuncs.virtToPhysRtn ((void *)addr);
	CSR_WRITE_4(pDev, TEI_TDBAL, ((UINT32)addr));
#endif
	}
    /*
     * Note: when setting the global TX enable DMATXCTL.TE, the enable bit
     * of TX queue 0 is enabled as well.
     */
    if (pDrvCtrl->teiDevType == TEI_DEVTYPE_82599)
        CSR_SETBIT_4(pDev, TEI_DMATXCTL, TEI_DMATXCTL_TE);

#ifdef TEI_FORWARDER
    /*
     * FORWARDER - DLK - used to be code here to init the forwarder
     * TX rings. Now done by forwarder NAE owning each ring.
     * tdlen, tdh, tdt, tdbah, tdbal, txdctl.
     * Now just disable all TX queues.
     */
    base = TEI_MUX_TX_ENABLED(pDrvCtrl) ? 1 : 0;
    for (i = base ; i < pDrvCtrl->fw_port.numTxQs + base; i++)
	{
	ULONG offset = i * TEI_TXQ_IVL_99;
	CSR_CLRBIT_4(pDev, TEI_TXDCTL + offset, TEI_TXDCTL_ENABLE);
	}
#endif

    /* Set inter-frame gap */
    /* TODO: DLK Why? 82599 doesn't document this reg at all, while
       82598 notes that it may violate the flow-control timing
       specification for values greater than 0 ... */

    CSR_WRITE_4(pDev, TEI_TIPG, 3);

    /* Set up RX flow control */

    /* TODO: if autonegotiation is enabled, this bit should be set by software
       to the negotiated flow control value... */
    if (pDrvCtrl->teiDevType == TEI_DEVTYPE_82599)
        CSR_SETBIT_4(pDev, TEI_MFLCN, TEI_MFLCN_RFCE);
    else
        CSR_SETBIT_4(pDev, TEI_FCTL, TEI_FCTL_RFCE);

    /*
     * TODO: 82599 is documented to use a different granularity
     * (32 bytes) for FCRTL.RTL than 82598 (256 bytes).
     * Doc might just be wrong -- for 82598 FCRTH.RTH measures
     * in 16-byte granularity, which makes more sense.
     */
    CSR_WRITE_4(pDev, TEI_FCRTL0,
        TEI_RTL(TEI_RX_FLOW_THRESH_LOW) | TEI_FCRTL_XONE);
    CSR_WRITE_4(pDev, TEI_FCRTH0,
        TEI_RTH(TEI_RX_FLOW_THRESH_HIGH) | TEI_FCRTL_FCEN);
    CSR_WRITE_4(pDev, TEI_FCRTV, TEI_FC_TIMER >> 1);

    /*
     * Set up TX flow control
     * Note: the TX flow control enable bits are located in the
     * RMCS register, which is undocumented in the 82598 manual.
     * Apparently this register is related to the Data Center
     * Bridging (DCB) features. Why the TX flow control bits are
     * placed here, rather than in one of the TX configuration
     * registers proper, is unclear. (Note that it is documented
     * in the 82599 manual as the FCCFG register.)
     */

    CSR_SETBIT_4(pDev, TEI_RMCS, TEI_RMCS_TFCE_802_3X);
    CSR_WRITE_4(pDev, TEI_FCTTV0, TEI_FCTTV_0(TEI_FC_TIMER) |
        TEI_FCTTV_1(TEI_FC_TIMER));

    /*
     * Set interrupt throttling.
     * Intel recommends setting throttling to limit the
     * number of interrupts to approximately 8000 per
     * second. We set it a little higher than normal so
     * that we can clean the TX descriptor ring a little
     * more frequently. This improves TX UDP transmit
     * performance.
     */

    CSR_WRITE_4(pDev, TEI_EITR0, TEI_IRQ_PER_SEC(TEI_INTR_RATE));

    /*
     * For the TEI_FORWARDER case, we want to assign all forwarder
     * RX/TX queue interrupts to an always-masked interrupt bit in
     * EICR.
     * Can we just not assign them (leave the IVAR INT_Alloc_val
     * bits zero), instead?
     */

    /*
     * Enable interrupts
     * We assign RX queue 0 events to interrupt source 0 and TX
     * queue 0 events to interrupt source 1.
     */

    vxbIntEnable (pDev, 0, teiEndInt, pDrvCtrl);
    if (pDrvCtrl->teiDevType == TEI_DEVTYPE_82599)
        {
#ifdef TEI_FORWARDER
	UINT32 rxBase = 0;
	UINT32 txBase = 0;
	UINT32 ix;
	if (TEI_MUX_RX_ENABLED(pDrvCtrl))
	    {
	    teiIvar599Set (pDev, TEI_RXQ, 0, 0);
	    rxBase = 1;
	    }
	if (TEI_MUX_TX_ENABLED(pDrvCtrl))
	    {
	    teiIvar599Set (pDev, TEI_TXQ, 0, 1);
	    txBase = 1;
	    }
	if (pDrvCtrl->fw_port.fw_capable)
	    {
	    /*
	     * Map RXQ ints to EICR bit 2, TXQ ints to EICR bit 3,
	     * neither of which is enabled.
	     */
	    for (ix = 0; ix < pDrvCtrl->fw_port.numRxQs; ++ix)
		{
		teiIvar599Set (pDev, TEI_RXQ, ix + rxBase, 2);
		}
	    for (ix = 0; ix < pDrvCtrl->fw_port.numTxQs; ++ix)
		{
		teiIvar599Set (pDev, TEI_TXQ, ix + txBase, 3);
		}
	    }
#else
	if (TEI_MUX_RX_ENABLED(pDrvCtrl))
	    teiIvar599Set (pDev, TEI_RXQ, 0, 0);
	if (TEI_MUX_TX_ENABLED(pDrvCtrl))
	    teiIvar599Set (pDev, TEI_TXQ, 0, 1);
#endif /* TEI_FORWARDER */
        }
    else
        {
	if (TEI_MUX_RX_ENABLED(pDrvCtrl))
	    teiIvarSet (pDev, TEI_RXQ, 0, 0);
	if (TEI_MUX_TX_ENABLED(pDrvCtrl))
	    teiIvarSet (pDev, TEI_TXQ, 0, 1);
        }

    pDrvCtrl->teiIntrs = TEI_INTRS;
    CSR_WRITE_4(pDev, TEI_EIMS, pDrvCtrl->teiIntrs);

    /* Enable receiver and transmitter */

    if (TEI_MUX_RX_ENABLED(pDrvCtrl))
	CSR_SETBIT_4(pDev, TEI_RXDCTL, TEI_RXDCTL_ENABLE);
    if (TEI_MUX_TX_ENABLED(pDrvCtrl))
	CSR_SETBIT_4(pDev, TEI_TXDCTL, TEI_TXDCTL_ENABLE);
    CSR_SETBIT_4(pDev, TEI_RXCTRL, TEI_RXCTRL_DMBYPS|TEI_RXCTRL_RXEN);
    CSR_SETBIT_4(pDev, TEI_CTRLEXT, TEI_CTRLX_NS_DIS|TEI_CTRLX_DRV_LOAD);

    /*
     * Set producer and consumer indices.
     * Apparently, on the 82599 controllers, enabling the receiver
     * clears the RX descriptor head and tail registers. Thus,
     * we need to wait until after the receiver is enabled
     * to set them.
     */

    if (TEI_MUX_RX_ENABLED(pDrvCtrl))
	{
	CSR_WRITE_4(pDev, TEI_RDH, 0);
	CSR_WRITE_4(pDev, TEI_RDT, TEI_RX_DESC_CNT - 1);
	}

    /* Set initial link state */

    pDrvCtrl->teiCurMedia = IFM_ETHER|IFM_NONE;
    pDrvCtrl->teiCurStatus = IFM_AVALID;

    CSR_SETBIT_4(pDev, TEI_AUTOC, TEI_AUTOC_RESTART_AN);

    END_FLAGS_SET (pEnd, (IFF_UP | IFF_RUNNING));

    END2_TX_SEM_GIVE (pEnd);
    semGive (pDrvCtrl->teiDevSem);

    return (OK);
    }

/*****************************************************************************
*
* teiEndStop - stop the device
*
* This function undoes the effects of teiEndStart(). The device is shut
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

LOCAL STATUS teiEndStop
    (
    END_OBJ * pEnd
    )
    {
    TEI_DRV_CTRL * pDrvCtrl;
    VXB_DEVICE_ID pDev;
    int i;
    Ipcom_pkt * pkt;

    pDrvCtrl = (TEI_DRV_CTRL *)pEnd;
    pDev = pDrvCtrl->teiDev;

    semTake (pDrvCtrl->teiDevSem, WAIT_FOREVER);

    if (!(pEnd->flags & IFF_UP))
        {
        semGive (pDrvCtrl->teiDevSem);
        return (OK);
        }

    /*
     * TODO:
     * Figure how to handle the case when there are forwarders
     * active on the port when it is told to stop.
     */

    /* Disable interrupts */
    /*vxbIntDisable (pDev, 0, teiEndInt, pDrvCtrl);*/
    pDrvCtrl->teiIntrs = 0;
    CSR_WRITE_4(pDev, TEI_EIMC, 0xFFFFFFFF);

    /*
     * Wait for all jobs to drain.
     * Note: this must be done before we disable the receiver
     * and transmitter below. If someone tries to reboot us via
     * WDB, this routine may be invoked while the RX handler is
     * still running in tNetTask. If we disable the chip while
     * that function is running, it'll start reading inconsistent
     * status from the chip. We have to wait for that job to
     * terminate first, then we can disable the receiver and
     * transmitter.
     */

    for (i = 0; i < TEI_TIMEOUT; i++)
        {
        if (vxAtomicGet (&pDrvCtrl->teiIntPending) == FALSE)
            break;
        taskDelay(1);
        }

    if (i == TEI_TIMEOUT)
        logMsg("%s%d: timed out waiting for job to complete\n",
            (int)TEI_NAME, pDev->unitNumber, 0, 0, 0, 0);

    END2_TX_SEM_TAKE (pEnd, WAIT_FOREVER); 

    END_FLAGS_CLR (pEnd, (IFF_UP | IFF_RUNNING));

    /* Disable RX and TX. */

    teiReset (pDev);

    /* Release resources */

#ifdef TEI_VXB_DMA_BUF
    if (TEI_MUX_RX_ENABLED(pDrvCtrl))
	vxbDmaBufMapUnload (pDrvCtrl->teiRxDescTag, pDrvCtrl->teiRxDescMap);
    if (TEI_MUX_TX_ENABLED(pDrvCtrl))
	vxbDmaBufMapUnload (pDrvCtrl->teiTxDescTag, pDrvCtrl->teiTxDescMap);
#endif

    teiBufFree (pDrvCtrl);  /* Free RX packets, poll packet, and packet maps */

    if (TEI_MUX_TX_ENABLED(pDrvCtrl))
	{
	for (i = 0; i < TEI_TX_DESC_CNT; i++)
	    {
	    pkt = pDrvCtrl->teiTxPkt[i];
	    if (pkt != NULL)
		{
		pDrvCtrl->teiTxPkt[i] = IP_NULL;
#ifdef TEI_VXB_DMA_BUF
		vxbDmaBufMapUnload (pDrvCtrl->teiPktTag,
				    pDrvCtrl->teiTxPktMap[i]);
#endif
		VXIPCOM_PKT_DONE (pkt);
		}
	    }
	}
    /*
     * TEI_FORWARDER - for gei there was code here to clean up
     * forwarder queues, but we want the forwarders to do that.
     */

    END2_TX_SEM_GIVE (pEnd); 
    semGive (pDrvCtrl->teiDevSem);

    return (OK);
    }

/***************************************************************************
*
* teiBufFree - free buffer memory resources
*
* This frees all the Ipcom_pkts in the RX ring. If TEI_VXB_DMA_BUF is defined,
* it also unloads all the packet buffer RX DMA maps.
* It also frees the TX polling packet.
*
* RETURNS: N/A
*/

LOCAL void teiBufFree
    (
    TEI_DRV_CTRL * pDrvCtrl
    )
    {
    int i;
    Ipcom_pkt * pkt;

    pkt = pDrvCtrl->teiEndObj.pollPkt;
    if (pkt == IP_NULL)
	return;

    if (TEI_MUX_RX_ENABLED(pDrvCtrl))
	{
	for (i = 0; i < TEI_RX_DESC_CNT; i++)
	    {
	    pkt = pDrvCtrl->teiRxPkt[i];
	    if (pkt != NULL)
		{
		vxipcom_pkt_free (pkt);
		pDrvCtrl->teiRxPkt[i] = IP_NULL;
#ifdef TEI_VXB_DMA_BUF
		vxbDmaBufMapUnload (pDrvCtrl->teiPktTag,
				    pDrvCtrl->teiRxPktMap[i]);
#endif
		}
	    }
	}

    vxipcom_pkt_free (pDrvCtrl->teiEndObj.pollPkt);
    pDrvCtrl->teiEndObj.pollPkt = IP_NULL;
    }

/*****************************************************************************
*
* teiEndIntHandle - task level interrupt handler
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

LOCAL void teiEndIntHandle
    (
    void * pArg
    )
    {
    QJOB *pJob;
    TEI_DRV_CTRL *pDrvCtrl;
    VXB_DEVICE_ID pDev;
    UINT32 status;
    int loopCtr = 1;
 
    pJob = pArg;
    pDrvCtrl = member_to_object (pJob, TEI_DRV_CTRL, teiIntJob);
    pDev = pDrvCtrl->teiDev;

    /* Read and acknowledge interrupts here. */ 
    status = CSR_READ_4(pDev, TEI_EICR) | pDrvCtrl->teiMoreRx;
    pDrvCtrl->teiMoreRx = 0;

    if (status & TEI_RXINTRS)
        {
        
        loopCtr = teiEndRxHandle (pDrvCtrl);
        if (loopCtr == 0)
            pDrvCtrl->teiMoreRx = TEI_RXINTRS;
        }

    if (status & TEI_TXINTRS)
        
        teiEndTxHandle (pDrvCtrl);
 
    if (status & TEI_LINKINTRS)
        teiLinkUpdate (pDev);

    /* Check for additional interrupt events */

    if (loopCtr == 0 || CSR_READ_4(pDev, TEI_EICS) & TEI_INTRS)
        {
        jobQueuePost (pDrvCtrl->teiJobQueue, &pDrvCtrl->teiIntJob);
        return;
        }
 
    vxAtomicSet (&pDrvCtrl->teiIntPending, FALSE);

    /* Unmask interrupts here */

    CSR_WRITE_4(pDev, TEI_EIMS, pDrvCtrl->teiIntrs);

    return;
    }

/*****************************************************************************
*
* teiEndInt - handle device interrupts
*
* This function is invoked whenever the 8259x's interrupt line is asserted.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void teiEndInt
    (
    TEI_DRV_CTRL * pDrvCtrl
    )
    {
    VXB_DEVICE_ID pDev;

    pDev = pDrvCtrl->teiDev;

    if (!(CSR_READ_4(pDev, TEI_EICS) & TEI_INTRS))
        return;

    if (!CSR_READ_4(pDev, TEI_EIMS))
        return;

    if (vxCas (&pDrvCtrl->teiIntPending, FALSE, TRUE))
        {
        /* mask interrupts here */
        CSR_WRITE_4(pDev, TEI_EIMC, TEI_INTRS);
        jobQueuePost (pDrvCtrl->teiJobQueue, &pDrvCtrl->teiIntJob);
        }

    return;
    }

/******************************************************************************
*
* teiEndRxHandle - process received frames
*
* This function is scheduled by the ISR to run in the context of tNetTask
* whenever an RX interrupt is received. It processes packets from the
* RX window and encapsulates them into mBlk tuples which are handed up
* to the MUX.
*
* There may be several packets waiting in the window to be processed.
* We take care not to process too many packets in a single run through
* this function so as not to monopolize tNetTask and starve out other
* jobs waiting in the jobQueue. If we detect that there's still more
* packets waiting to be processed, we queue ourselves up for another
* round of processing.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL int teiEndRxHandle
    (
    TEI_DRV_CTRL *pDrvCtrl
    )
    {
    VXB_DEVICE_ID pDev;
    Ipcom_pkt * pkt;
    Ipcom_pkt * newpkt;
    TEI_ADV_RDESC * pDesc;
#ifdef TEI_VXB_DMA_BUF
    VXB_DMA_MAP_ID pMap;
#endif
    int loopCounter = TEI_MAX_RX;
    UINT32 rxLen;

    pDev = pDrvCtrl->teiDev;

    
    while (loopCounter)
        {
            
	        pDesc = (TEI_ADV_RDESC *)&pDrvCtrl->teiRxDescMem[pDrvCtrl->teiRxIdx];

        if (!(pDesc->write.tei_sts & htole16(TEI_ADV_RDESC_STS_DD)))
            break;
        

#ifdef TEI_VXB_DMA_BUF
        
        pMap = pDrvCtrl->teiRxPktMap[pDrvCtrl->teiRxIdx];
#endif
	pkt = pDrvCtrl->teiRxPkt[pDrvCtrl->teiRxIdx];

        /*
         * Ignore checksum errors here. They'll be handled by the
         * checksum offload code below, or by the stack.
         */

        if (pDesc->write.tei_err & htole16(TEI_ADV_RDESC_ERRSUM))
            {
            logMsg("%s%d: bad packet, erradv: %x (%p %d)\n", (int)TEI_NAME,
                pDev->unitNumber, le32toh(pDesc->write.tei_err), (int)pDesc,
                pDrvCtrl->teiRxIdx, 0);
            goto skip;
            }

	newpkt = vxipcom_pkt_malloc (pDrvCtrl->teiMaxMtu, IPCOM_FLAG_FC_EXACT);
        if (newpkt == IP_NULL)
            {
            logMsg("%s%d: out of mBlks at %d\n", (int)TEI_NAME,
                pDev->unitNumber, pDrvCtrl->teiRxIdx,0,0,0);
            pDrvCtrl->teiLastError.errCode = END_ERR_NO_BUF;
            muxError (&pDrvCtrl->teiEndObj, &pDrvCtrl->teiLastError);
skip:
#ifdef TEI_VXB_DMA_BUF
            pDesc->read.tei_addrlo =
                htole32(TEI_ADDR_LO(pMap->fragList[0].frag));
            pDesc->read.tei_addrhi =
                htole32(TEI_ADDR_HI(pMap->fragList[0].frag));
#elif defined (TEI_GUEST_VIRT_TO_PHYS)
	    {
	    uint64_t paddr = teiVirtToPhys(pkt->data + TEI_RXPKT_OFFSET);
	    pDesc->read.tei_addrlo = (UINT32)paddr; /* L.E. */
	    pDesc->read.tei_addrhi = (UINT32)(paddr >> 32); /* L.E. */
	    }
#else
            {
            ULONG addr = (ULONG)pkt->data + TEI_RXPKT_OFFSET;
            if (cacheUserFuncs.virtToPhysRtn)
                addr = (ULONG)cacheUserFuncs.virtToPhysRtn ((void *)addr);
	    pDesc->read.tei_addrlo = htole32(((UINT32)addr));
            pDesc->read.tei_addrhi = 0;
            }
#endif
            pDesc->read.tei_hdrlo = 0;
            pDesc->read.tei_hdrhi = 0;
            CSR_WRITE_4(pDev, TEI_RDT, pDrvCtrl->teiRxIdx);
            TEI_INC_DESC(pDrvCtrl->teiRxIdx, TEI_RX_DESC_CNT);
            loopCounter--;
            continue;
            }

	/* Swap buffers */
	pDrvCtrl->teiRxPkt[pDrvCtrl->teiRxIdx] = newpkt;
	pkt->start = TEI_RXPKT_OFFSET;

	rxLen = le16toh(pDesc->write.tei_pkt_buflen);
#ifndef TEI_SECRC
	rxLen -= 4;
#endif
	pkt->end = pkt->start + rxLen;

#ifdef TEI_VXB_DMA_BUF
        /* Sync the packet buffer and unload the map. */

        vxbDmaBufSync (pDev, pDrvCtrl->teiPktTag,
            pMap, VXB_DMABUFSYNC_PREREAD);
        vxbDmaBufMapUnload (pDrvCtrl->teiPktTag, pMap);

        /*
         * Take advantage of the fact that we know there's only
         * going to be one buffer, and use the single buffer map
         * load routine instead of the mBlk load routine.
         */

        vxbDmaBufMapLoad (pDev, pDrvCtrl->teiPktTag, pMap,
			  newpkt->data + TEI_RXPKT_OFFSET,
			  pDrvCtrl->teiMaxMtu + TEI_MTU_ADJUST, 0);

        pDesc->read.tei_addrlo = htole32(TEI_ADDR_LO(pMap->fragList[0].frag));
        pDesc->read.tei_addrhi = htole32(TEI_ADDR_HI(pMap->fragList[0].frag));
#elif defined (TEI_GUEST_VIRT_TO_PHYS)
        /* pre-invalidate the replacement buffer */
        CACHE_USER_INVALIDATE (newpkt->data + TEI_RXPKT_OFFSET,
			       pDrvCtrl->teiMaxMtu + TEI_MTU_ADJUST);
	{
	uint64_t paddr = teiVirtToPhys(newpkt->data + TEI_RXPKT_OFFSET);
	pDesc->read.tei_addrlo = (UINT32)paddr; /* L.E. */
	pDesc->read.tei_addrhi = (UINT32)(paddr >> 32); /* L.E. */
	}
#else
        /* pre-invalidate the replacement buffer */
        CACHE_USER_INVALIDATE (newpkt->data + TEI_RXPKT_OFFSET,
			       pDrvCtrl->teiMaxMtu + TEI_MTU_ADJUST);

        {
        ULONG addr = (ULONG)newpkt->data + TEI_RXPKT_OFFSET;
        if (cacheUserFuncs.virtToPhysRtn)
            addr = (ULONG)cacheUserFuncs.virtToPhysRtn ((void *)addr);
        pDesc->read.tei_addrlo = htole32(((UINT32)addr));
        pDesc->read.tei_addrhi = 0;
        }
#endif

        /* Handle checksum offload. */

        if (pDrvCtrl->teiCaps.cap_enabled & IFCAP_RXCSUM)
            {
#if 0
            if (pDesc->write.tei_sts & htole16(TEI_ADV_RDESC_STS_IPCS))
                pMblk->m_pkthdr.csum_flags |= CSUM_IP_CHECKED;
            if (!(pDesc->write.tei_err & htole16(TEI_ADV_RDESC_ERR_IPE)))
                pMblk->m_pkthdr.csum_flags |= CSUM_IP_VALID;
#endif
            if ((pDesc->write.tei_sts & htole16(TEI_ADV_RDESC_STS_UDPCS)) &&
                !(pDesc->write.tei_err & htole16(TEI_ADV_RDESC_ERR_TCPE)))
                {
		pkt->chk = 0xffff;
		IP_BIT_SET(pkt->flags, IPCOM_PKT_FLAG_HW_CHECKSUM);
                }
            }

        /* Handle VLAN tags */

        if (pDrvCtrl->teiCaps.cap_enabled & IFCAP_VLAN_HWTAGGING)
           {
           if (pDesc->write.tei_sts & htole32(TEI_ADV_RDESC_STS_VLAN))
               {
	       Ip_u16 vtag = le16toh(pDesc->write.tei_vlan);
	       IPNET_ETH_PKT_SET_VLAN_TAG(pkt, vtag);
	       IP_BIT_SET(pkt->flags, IPCOM_PKT_FLAG_VLAN_TAG);
               }
           }

        /* Reset this descriptor's status fields. */

        pDesc->read.tei_hdrlo = 0;
        pDesc->read.tei_hdrhi = 0;

        /* Advance to the next descriptor */

        CSR_WRITE_4(pDev, TEI_RDT, pDrvCtrl->teiRxIdx);
        TEI_INC_DESC(pDrvCtrl->teiRxIdx, TEI_RX_DESC_CNT);
        loopCounter--;

        END2_RCV_RTN_CALL (&pDrvCtrl->teiEndObj, pkt);
        }

    return loopCounter;
    }

/******************************************************************************
*
* teiEndTbdClean - clean the TX ring
*
* This function is called from both teiEndSend() or teiEndTxHandle() to
* reclaim TX descriptors, and to free packets and DMA maps associated
* with completed transmits.  This routine must be called with the END
* TX semaphore already held.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void teiEndTbdClean
    (
    TEI_DRV_CTRL * pDrvCtrl
    )
    {
#ifdef TEI_VXB_DMA_BUF
    VXB_DMA_MAP_ID pMap;
#endif
    TEI_ADV_TDESC * pDesc;
    Ipcom_pkt * pkt;
    UINT32 j;

    while (pDrvCtrl->teiTxFree < TEI_TX_DESC_CNT)
        {
	/*
	 * Since not all descriptors are free, there must
	 * be a descriptor with an Ipcom_pkt to be cleaned.
	 * We don't consider any descriptors associated with
	 * the packet to be cleaned until the last descriptor
	 * of the packet has its RS bit set.
	 * We no longer allow isolated context descriptors
	 * not associated with a following packet in the ring.
	 * So the loop below must terminate.
	 * A context descriptor never has an Ipcom_pkt in
	 * the corresponding slot.
	 */
	j = pDrvCtrl->teiTxCons;
	while ((pkt = pDrvCtrl->teiTxPkt[j]) == IP_NULL)
	    {
	    TEI_INC_DESC (j, TEI_TX_DESC_CNT);
	    }

        pDesc = &pDrvCtrl->teiTxDescMem[j];

        if ((pDesc->tei_sts & htole32(TEI_ADV_STS_DD)) == 0)
            break;

#ifdef TEI_VXB_DMA_BUF
        pMap = pDrvCtrl->teiTxPktMap[pDrvCtrl->teiTxCons];
	vxbDmaBufMapUnload (pDrvCtrl->teiPktTag, pMap);
#endif
	VXIPCOM_PKT_DONE (pkt);
	pDrvCtrl->teiTxPkt[j] = IP_NULL;

        TEI_INC_DESC (j, TEI_TX_DESC_CNT);
	pDrvCtrl->teiTxFree +=
	    (j - pDrvCtrl->teiTxCons) & (TEI_TX_DESC_CNT - 1);
	pDrvCtrl->teiTxCons = j;
        }

    return;
    }

/******************************************************************************
*
* teiEndTxHandle - process TX completion events
*
* This function is scheduled by the ISR to run in the context of tNetTask
* whenever an TX interrupt is received. It runs through all of the
* TX register pairs and checks the TX status to see how many have
* completed. For each completed transmission, the associated TX mBlk
* is released, and the outbound packet stats are updated.
*
* In the event that a TX underrun error is detected, the TX FIFO
* threshold is increased. This will continue until the maximum TX
* FIFO threshold is reached.
*
* If the transmitter has stalled, this routine will also call muxTxRestart()
* to drain any packets that may be waiting in the protocol send queues,
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void teiEndTxHandle
    (
    TEI_DRV_CTRL *pDrvCtrl
    )
    {
    UINT32 origFree;

    END2_TX_SEM_TAKE (&pDrvCtrl->teiEndObj, WAIT_FOREVER);
    origFree = pDrvCtrl->teiTxFree;
    teiEndTbdClean (pDrvCtrl);
    if (pDrvCtrl->teiTxStall && origFree < pDrvCtrl->teiTxFree)
        {
        pDrvCtrl->teiTxStall = FALSE;
        origFree = ~0U; /* flag that we should TX restart */
        }
    END2_TX_SEM_GIVE (&pDrvCtrl->teiEndObj);

    if (origFree == ~0U)
        muxTxRestart (pDrvCtrl);

    return;
    }

/******************************************************************************
*
* teiEndEncap - encapsulate an outbound packet using advanced descriptors
*
* This function sets up a descriptor for a packet transmit operation.
* With the 82598/82599 ethernet controller, the TX DMA ring consists of
* descriptors that each describe a single packet fragment. We consume
* as many descriptors as there are mBlks in the outgoing packet, unless
* the chain is too long. The length is limited by the number of DMA
* segments we want to allow in a given DMA map. If there are too many
* segments, this routine will fail, and the caller must coalesce the
* data into fewer buffers and try again.
*
* The 82598/82599 also has context descriptors that are used to set
* up what kind of checksum offload will be used for the frames that
* follow. One context descriptor is required whenever the frame payload
* changes between UDP and TCP.
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

LOCAL int teiEndEncap
    (
    TEI_DRV_CTRL * pDrvCtrl,
    Ipcom_pkt * pkt
    )
    {
    VXB_DEVICE_ID pDev;
    TEI_ADV_TDESC * pDesc = NULL;
    TEI_ADV_TDESC * pDesc1;
    TEI_ADV_CDESC * pCtx;
    UINT32 firstIdx, lastIdx = 0;
#ifdef TEI_VXB_DMA_BUF
    int i;
    VXB_DMA_MAP_ID pMap;
#else
    Ipcom_pkt * pTmp;
    UINT32 nFrags;
    ULONG addr;
#endif
    UINT32 cmd;
    UINT32 val;
    UINT32 paylen;
    Ip_u16 vtag;

    pDev = pDrvCtrl->teiDev;

    /*
     * Check if we have enough descriptors. Require an extra for
     * a context descriptor even though we might not consume use one.
     * Do this check first so that we are always sure during TX
     * cleanup that only whole packets are in the ring to be consumed,
     * never just a few context descriptors.
     */

#ifdef TEI_VXB_DMA_BUF
    pMap = pDrvCtrl->teiTxPktMap[pDrvCtrl->teiTxProd];

    /*
     * Load the DMA map to build the segment list.
     * This will fail if there are too many segments.
     */

    if (vxbDmaBufMapIpcomLoad (pDev, pDrvCtrl->teiPktTag, pMap, pkt, 0) != OK
	|| (pMap->nFrags >= pDrvCtrl->teiTxFree))
        {
        vxbDmaBufMapUnload (pDrvCtrl->teiPktTag, pMap);
        return (ENOSPC);
        }
#else /* TEI_VXB_DMA_BUF */
    nFrags = 0;
    for (pTmp = pkt; pTmp != IP_NULL; pTmp = pTmp->next_part)
        {
	if (pTmp->end != pTmp->start)
            ++nFrags;
        }

    if (nFrags >= pDrvCtrl->teiTxFree)
        return ENOSPC;
#endif /* TEI_VXB_DMA_BUF */

    firstIdx = pDrvCtrl->teiTxProd;

    /* Set up a TCP/IP offload context descriptor if needed. */

    if (IP_BIT_ISSET (pkt->flags, (IPCOM_PKT_FLAG_HW_CHECKSUM |
				   IPCOM_PKT_FLAG_VLAN_TAG)))
        {
	UINT32 v1 = pkt->ipstart - pkt->start;
	UINT32 v2 = pkt->tlstart - pkt->ipstart;
	UINT32 offsets = (pkt->chk + (v1 << 8) + (v2 << 16));
	vtag = IPNET_ETH_PKT_GET_VLAN_TAG(pkt);

	/* TODO: If the last packet had IPCOM_PKT_FLAG_VLAN_TAG
	 * but this one doesn't, do we need a new context descriptor
	 * even if the offsets are the same? (Note, the v1 offset
	 * doesn't include the VLAN header size when vlan insertion
	 * is requested.)
	 */
	if (offsets != pDrvCtrl->teiLastOffsets ||
	    (IP_BIT_ISSET (pkt->flags, IPCOM_PKT_FLAG_VLAN_TAG) &&
	     vtag != pDrvCtrl->teiLastVlan))
            {
            pDrvCtrl->teiLastOffsets = offsets;
	    pDrvCtrl->teiLastVlan = vtag;

            pCtx = (TEI_ADV_CDESC *)&pDrvCtrl->teiTxDescMem[pDrvCtrl->teiTxProd];

	    val = htole32(TEI_ADV_TDESC_CMD_DEXT | TEI_ADV_TDESC_DTYP_CTX);

	    /*
	     * We identify UDP by a transport header checksum offset of 6.
	     * If not UDP (e.g. for TCP, ICMP) there's no special handling of
	     * the case of a zero checksum value; for UDP this case gets
	     * inverted to 0xffff.
	     *
	     * So treat everything but UDP as TCP.
	     * If we eventually support TCP segmentation offload, we may need
	     * to revisit this.
	     */
	    if (pkt->chk != 6)
		val |= htole32(TEI_ADV_CDESC_L4T_TCP);

	    /* distinguish IPv4 from IPv6 */
	    if ((pkt->data[pkt->ipstart] & 0xf0) == 0x40)
		val |= htole32(TEI_ADV_CDESC_CMD_IPV4);

	    pCtx->tei_cmd = val;

	    val = TEI_ADV_MACLEN(v1) | TEI_ADV_IPLEN(v2);
	    pCtx->tei_macip = htole16(val);

	    if (IP_BIT_ISSET (pkt->flags, IPCOM_PKT_FLAG_VLAN_TAG))
		pCtx->tei_vlan = htole16(vtag);

            pDrvCtrl->teiTxFree--;
            TEI_INC_DESC(pDrvCtrl->teiTxProd, TEI_TX_DESC_CNT);
            }
        }

    cmd = (TEI_ADV_TDESC_CMD_DEXT | TEI_ADV_TDESC_DTYP_DSC |
	   TEI_ADV_TDESC_CMD_IFCS /* | TEI_ADV_TDESC_CMD_RS */ );

    if (IP_BIT_ISSET (pkt->flags, IPCOM_PKT_FLAG_VLAN_TAG))
	cmd |= TEI_ADV_TDESC_CMD_VLE;

    pDesc1 = (TEI_ADV_TDESC *)&pDrvCtrl->teiTxDescMem[pDrvCtrl->teiTxProd];
    paylen = 0;

#ifdef TEI_VXB_DMA_BUF
    for (i = 0; i < pMap->nFrags; i++)
        {
        pDesc = (TEI_ADV_TDESC *)&pDrvCtrl->teiTxDescMem[pDrvCtrl->teiTxProd];
#ifdef TEI_64
        pDesc->tei_addrlo = htole32(TEI_ADDR_LO(pMap->fragList[i].frag));
        pDesc->tei_addrhi = htole32(TEI_ADDR_HI(pMap->fragList[i].frag));
#else
        pDesc->tei_addrlo = htole32((UINT32)pMap->fragList[i].frag);
	pDesc->tei_addrhi = 0;
#endif
	val = pMap->fragList[i].fragLen;
	paylen += val;
	val |= cmd;

        pDesc->tei_cmd = htole32(val);
	pDesc->tei_sts = 0;

        pDrvCtrl->teiTxFree--;
        lastIdx = pDrvCtrl->teiTxProd;
        TEI_INC_DESC(pDrvCtrl->teiTxProd, TEI_TX_DESC_CNT);
        }

#else /* TEI_VXB_DMA_BUF */
    for (pTmp = pkt; pTmp != IP_NULL; pTmp = pTmp->next_part)
        {
	val = pTmp->end - pTmp->start;
	if (val == 0)
	    continue;
	paylen += val;

        pDesc = (TEI_ADV_TDESC *)&pDrvCtrl->teiTxDescMem[pDrvCtrl->teiTxProd];

        addr = (ULONG)(pTmp->data + pTmp->start);
        CACHE_USER_FLUSH (addr, val);
#ifdef TEI_GUEST_VIRT_TO_PHYS
	{
	uint64_t paddr = teiVirtToPhys ((void *)addr);
	pDesc->tei_addrlo = (UINT32)paddr; /* L.E. */
	pDesc->tei_addrhi = (UINT32)(paddr >> 32); /* L.E. */
	}
#else
        if (cacheUserFuncs.virtToPhysRtn)
            addr = (ULONG)cacheUserFuncs.virtToPhysRtn ((void *)addr);
        pDesc->tei_addrlo = htole32(((UINT32)addr));
	pDesc->tei_addrhi = 0;
#endif
	val |= cmd;

        pDesc->tei_cmd = htole32(val);
        pDesc->tei_sts = 0;

        pDrvCtrl->teiTxFree--;
        lastIdx = pDrvCtrl->teiTxProd;
        TEI_INC_DESC(pDrvCtrl->teiTxProd, TEI_TX_DESC_CNT);
        }
#endif /* TEI_VXB_DMA_BUF */

    /* Set tei_sts word for first descriptor */
    val = TEI_ADV_PAYLEN(paylen);
    if (IP_BIT_ISSET (pkt->flags, IPCOM_PKT_FLAG_HW_CHECKSUM))
	val |= TEI_ADV_POPT_TXSM;
    pDesc1->tei_sts = htole32(val);

    /* Set tei_cmd word for last descriptor */
    pDesc->tei_cmd |= htole32(TEI_ADV_TDESC_CMD_EOP|TEI_ADV_TDESC_CMD_RS);

    /* Save pkt for later. */

    pDrvCtrl->teiTxPkt[lastIdx] = pkt;

#ifdef TEI_VXB_DMA_BUF
    /*
     * Ensure that the map for this transmission
     * is placed at the array index of the last descriptor
     * in this chain.  (Swap last and first dmamaps.)
     */

    pDrvCtrl->teiTxPktMap[firstIdx] = pDrvCtrl->teiTxPktMap[lastIdx];
    pDrvCtrl->teiTxPktMap[lastIdx] = pMap;

    /* Sync the buffer. */

    vxbDmaBufSync (pDev, pDrvCtrl->teiPktTag, pMap, VXB_DMABUFSYNC_POSTWRITE);
#endif

    return (OK);
    }


/******************************************************************************
*
* teiEndSend - transmit a packet
*
* This function transmits the packet specified in <pkt>.
*
* RETURNS: OK, ERROR, or END_ERR_BLOCK.
*
* ERRNO: N/A
*/

LOCAL int teiEndSend
    (
    END_OBJ * pEnd,
    Ipcom_pkt * pkt
    )
    {
    TEI_DRV_CTRL * pDrvCtrl;
    VXB_DEVICE_ID pDev;
    Ipcom_pkt * pTmp;
    int rval;

    pDrvCtrl = (TEI_DRV_CTRL *)pEnd;

    pDev = pDrvCtrl->teiDev;

    END2_TX_SEM_TAKE (pEnd, WAIT_FOREVER);

    /* Attempt early cleanup */

    if (!(pDrvCtrl->teiCurStatus & IFM_ACTIVE))
	goto blocked;

    if (pDrvCtrl->teiTxFree < TEI_TX_DESC_CNT - TEI_TX_CLEAN_THRESH)
        teiEndTbdClean (pDrvCtrl);

    /*
     * We may need a context descriptor as well as a data descriptor.
     * Don't even bother trying if there are not at least 2 free
     * descriptors.
     */
    if (pDrvCtrl->teiTxFree < 2)
        goto blocked;

    /*
     * First, try to do an in-place transmission, using
     * gather-write DMA.
     */

    rval = teiEndEncap (pDrvCtrl, pkt);

    /*
     * If teiEndEncap() returns ENOSPC, it means it ran out
     * of TX descriptors and couldn't encapsulate the whole
     * packet fragment chain. In that case, we need to
     * coalesce everything into a single buffer and try
     * again. If any other error is returned, then something
     * went wrong, and we have to abort the transmission
     * entirely.
     */

    if (rval == ENOSPC)
        {
	Ip_u8 * buf;
	Ipcom_pkt * pkt2;

       /* copy discontiguous info from pkt chain to pTmp... */
	if ((pTmp = vxipcom_pkt_malloc (pDrvCtrl->teiMaxMtu, 0)) == NULL)
            goto blocked;

	pTmp->start = pkt->start & 0x1f; /* use alignment from pkt */

	buf = &pTmp->data[pTmp->start];
	for (pkt2 = pkt; pkt2 != IP_NULL; pkt2 = pkt->next_part)
	    {
	    int len2 = pkt2->end - pkt2->start;
	    bcopy ((caddr_t)&pkt2->data[pkt2->start], (caddr_t)buf, len2);
	    buf += len2;
	    }
	pTmp->end = buf - pTmp->data;
	/* for checksum offload & vlan offload stuff */
	pTmp->flags = pkt->flags;
	pTmp->chk = pkt->chk;
	pTmp->ipstart = pTmp->start + (pkt->ipstart - pkt->start);
	pTmp->tlstart = pTmp->start + (pkt->tlstart - pkt->start);
	pTmp->link_cookie = pkt->link_cookie;

        /* Try transmission again, should succeed this time. */
        rval = teiEndEncap (pDrvCtrl, pTmp);
        if (rval == OK)
	    VXIPCOM_PKT_DONE (pkt); /* hmm, not realy done in terms of
					    send flow control ... */
        else
	    vxipcom_pkt_free (pTmp);
        }

    if (rval != OK)
        goto blocked;

    /* Issue transmit command */

    CSR_WRITE_4(pDev, TEI_TDT, pDrvCtrl->teiTxProd);

    END2_TX_SEM_GIVE (pEnd);
    return (0);

blocked:

    pDrvCtrl->teiTxStall = TRUE;
    END2_TX_SEM_GIVE (pEnd);

    return (-IP_ERRNO_EWOULDBLOCK);
    }

/******************************************************************************
*
* teiEndPollSend - polled mode transmit routine
*
* This function is similar to the teiEndSend() routine shown above, except
* it performs transmissions synchronously with interrupts disabled. After
* the transmission is initiated, the routine will poll the state of the
* TX status register associated with the current slot until transmission
* completed. If transmission times out, this routine will return ERROR.
*
* RETURNS: OK, EAGAIN, or ERROR
*
* ERRNO: N/A
*/

LOCAL STATUS teiEndPollSend
    (
    END_OBJ * pEnd,
    Ipcom_pkt * pkt
    )
    {
    TEI_DRV_CTRL * pDrvCtrl;
    VXB_DEVICE_ID pDev;
#ifdef TEI_VXB_DMA_BUF
    VXB_DMA_MAP_ID pMap;
#endif
    Ipcom_pkt * pTmp;
    int i;

    pDrvCtrl = (TEI_DRV_CTRL *)pEnd;

    if (pDrvCtrl->teiPolling == FALSE)
        return (ERROR);

    /*
     * We require that the polled send routine be called only
     * with a single-segment packet.
     */
    ip_assert (pkt->next_part == IP_NULL);

    pDev = pDrvCtrl->teiDev;

    /* Encapsulate buffer */

    if (teiEndEncap (pDrvCtrl, pkt) != OK)
        return (EAGAIN);

    /* Issue transmit command */

    CSR_WRITE_4(pDev, TEI_TDT, pDrvCtrl->teiTxProd);

    /* Wait for transmission to complete */

    for (i = 0; i < TEI_TIMEOUT; i++)
        {
        if (CSR_READ_4(pDev, TEI_TDH) == CSR_READ_4(pDev, TEI_TDT))
            break;
        }

    CSR_WRITE_4(pDev, TEI_EICR, TEI_TXINTRS);

    /* Remember to unload the map once transmit completes. */

    FOREVER
        {
	UINT32 ix = pDrvCtrl->teiTxCons;
        pDrvCtrl->teiTxFree++;
        TEI_INC_DESC(pDrvCtrl->teiTxCons, TEI_TX_DESC_CNT);
	pTmp = pDrvCtrl->teiTxPkt[ix];
        if (pTmp != NULL)
            {
#ifdef TEI_VXB_DMA_BUF
            pMap = pDrvCtrl->teiTxPktMap[ix];
            vxbDmaBufMapUnload (pDrvCtrl->teiPktTag, pMap);
#endif
            pDrvCtrl->teiTxPkt[ix] = NULL;
	    break;
            }
        }

    if (i == TEI_TIMEOUT)
        return (ERROR);

    return (OK);
    }

/******************************************************************************
*
* teiEndPollReceive - polled mode receive routine
*
* This function receive a packet in polled mode, with interrupts disabled.
* It's similar in operation to the teiEndRxHandle() routine, except it
* doesn't process more than one packet at a time and does not load out
* buffers. Instead, the caller supplied an mBlk tuple into which this
* function will place the received packet.
*
* If no packet is available, this routine will return EAGAIN. If the
* supplied mBlk is too small to contain the received frame, the routine
* will return ERROR.
*
* RETURNS: OK, EAGAIN, or ERROR
*
* ERRNO: N/A
*/

LOCAL int teiEndPollReceive
    (
    END_OBJ * pEnd,
    Ipcom_pkt * pkt
    )
    {
    TEI_DRV_CTRL * pDrvCtrl;
    VXB_DEVICE_ID pDev;
#ifdef TEI_VXB_DMA_BUF
    VXB_DMA_MAP_ID pMap;
#endif
    TEI_ADV_RDESC * pDesc;
    Ip_u8 * buf;
    int rval = EAGAIN;
    UINT16 rxLen = 0;
    Ipcom_pkt * rpkt;

    pDrvCtrl = (TEI_DRV_CTRL *)pEnd;
    if (pDrvCtrl->teiPolling == FALSE)
        return (ERROR);

    pDev = pDrvCtrl->teiDev;

    pDesc = (TEI_ADV_RDESC *)&pDrvCtrl->teiRxDescMem[pDrvCtrl->teiRxIdx];

    if (!(pDesc->write.tei_sts & htole16(TEI_ADV_RDESC_STS_DD)))
        return (EAGAIN);

    CSR_WRITE_4(pDev, TEI_EICR, TEI_RXINTRS);

    rpkt = pDrvCtrl->teiRxPkt[pDrvCtrl->teiRxIdx];
    buf = rpkt->data + TEI_RXPKT_OFFSET;

#ifdef TEI_VXB_DMA_BUF
    pMap = pDrvCtrl->teiRxPktMap[pDrvCtrl->teiRxIdx];
#endif
    rxLen = le16toh(pDesc->write.tei_pkt_buflen);
#ifndef TEI_SECRC
    rxLen -= 4;
#endif

#ifdef TEI_VXB_DMA_BUF
    vxbDmaBufSync (pDev, pDrvCtrl->teiPktTag, pMap, VXB_DMABUFSYNC_PREREAD);
#endif

    /*
     * Ignore checksum errors here. They'll be handled by the
     * checksum offload code below, or by the stack.
     */

    if (!(pDesc->write.tei_err & htole16(TEI_ADV_RDESC_ERRSUM)))
        {
        /* Handle checksum offload. */

        if (pDrvCtrl->teiCaps.cap_enabled & IFCAP_RXCSUM)
            {
#if 0
            if (pDesc->write.tei_sts & htole16(TEI_ADV_RDESC_STS_IPCS))
                pMblk->m_pkthdr.csum_flags |= CSUM_IP_CHECKED;
            if (!(pDesc->write.tei_err & htole16(TEI_ADV_RDESC_ERR_IPE)))
                pMblk->m_pkthdr.csum_flags |= CSUM_IP_VALID;
#endif
            if ((pDesc->write.tei_sts & htole16(TEI_ADV_RDESC_STS_UDPCS)) &&
                !(pDesc->write.tei_err & htole16(TEI_ADV_RDESC_ERR_TCPE)))
                {
		pkt->chk = 0xffff;
		IP_BIT_SET(pkt->flags, IPCOM_PKT_FLAG_HW_CHECKSUM);
                }
            }

        /* Handle VLAN tags */

        if (pDrvCtrl->teiCaps.cap_enabled & IFCAP_VLAN_HWTAGGING)
           {
           if (pDesc->write.tei_sts & htole16(TEI_ADV_RDESC_STS_VLAN))
               {
	       Ip_u16 vtag = le16toh(pDesc->write.tei_vlan);
	       IPNET_ETH_PKT_SET_VLAN_TAG(pkt, vtag);
	       IP_BIT_SET(pkt->flags, IPCOM_PKT_FLAG_VLAN_TAG);
               }
           }

	pkt->end = pkt->start + rxLen;
	bcopy ((caddr_t)buf, (caddr_t)&pkt->data[pkt->start], rxLen);
        rval = OK;
        }

    /*
     * Pre-invalidate the re-used buffer.
     */
    CACHE_DMA_INVALIDATE (buf, pDrvCtrl->teiMaxMtu + TEI_MTU_ADJUST);

    /* Reset the descriptor */

#ifdef TEI_VXB_DMA_BUF
    pDesc->read.tei_addrlo = htole32(TEI_ADDR_LO(pMap->fragList[0].frag));
    pDesc->read.tei_addrhi = htole32(TEI_ADDR_HI(pMap->fragList[0].frag));
#elif defined (TEI_GUEST_VIRT_TO_PHYS)
    {
    uint64_t paddr = teiVirtToPhys(buf);
    pDesc->read.tei_addrlo = (UINT32)paddr; /* L.E. */
    pDesc->read.tei_addrhi = (UINT32)(paddr >> 32); /* L.E. */
    }
#else
    {
    ULONG addr = (ULONG)buf;
    if (cacheUserFuncs.virtToPhysRtn)
        addr = (ULONG)cacheUserFuncs.virtToPhysRtn (buf);
    pDesc->read.tei_addrlo = htole32(((UINT32)addr));
    pDesc->read.tei_addrhi = 0;
    }
#endif
    pDesc->read.tei_hdrhi = 0;
    pDesc->read.tei_hdrlo = 0;

    CSR_WRITE_4(pDev, TEI_RDT, pDrvCtrl->teiRxIdx);
    TEI_INC_DESC(pDrvCtrl->teiRxIdx, TEI_RX_DESC_CNT);

    return (rval);

    }

#ifdef TEI_FORWARDER

LOCAL void teiFwPortDrvinfo
    (
    Ipcom_fw_port_t * port,
    Ipcom_fw_port_drvinfo_t * drvinfo
    )
    {
    TEI_DRV_CTRL * pDrvCtrl = member_to_object (port, TEI_DRV_CTRL, fw_port);

    drvinfo->maxMtu = pDrvCtrl->teiMaxMtu;
#if 0 /* TODO: not used if forwarders allocate their own buffers and TX/RX rings */
    drvinfo->bufsPerRxQ = TEI_FW_RX_DESC_CNT;
    drvinfo->bufsPerTxQ = TEI_FW_TX_CLEAN_THRESH;
#endif
    }

LOCAL Ip_u64 teiFwQAddr
    (
    Ipcom_fw_port_t * port,
    Ip_u32 q_index,
    enum fw_q_type q_type
    )
    {
    TEI_DRV_CTRL * pDrvCtrl = member_to_object (port, TEI_DRV_CTRL, fw_port);
    VXB_DEVICE_ID pDev = pDrvCtrl->teiDev;
    Ip_u64 addr = (Ip_u64)(unsigned long)(pDev->pRegBase[0]);

    if (q_type == FW_Q_RX)
	{
	if (TEI_MUX_RX_ENABLED(pDrvCtrl))
	    q_index++;
	if (q_index < 64) /* always */
	    addr += TEI_RDBAL0;
	else /* never */
	    addr += TEI_RDBAL64;
	addr += TEI_RXQ_IVL_99 * q_index;
	}
    else
	{
	if (TEI_MUX_TX_ENABLED(pDrvCtrl))
	    q_index++;
	addr += TEI_TDBAL0;
	addr += TEI_TXQ_IVL_99 * q_index;
	}

    return addr;
    }

void teiFwRssReseed
    (
    TEI_DRV_CTRL * pDrvCtrl,
    UINT32 * bits320
    )
    {
    VXB_DEVICE_ID pDev;
    int i;
    UINT32 val;

    pDev = pDrvCtrl->teiDev;

    for (i = 0; i < 10; ++i)
	{
	val = bits320[i];
	val = htole32 (val);
	CSR_WRITE_4(pDev, TEI_RSSRK0 + 4*i, val);
	}
    }

/* TODO: UNUSED */
void teiFwMultiQEnable
    (
    TEI_DRV_CTRL * pDrvCtrl,
    BOOL enable
    )
    {
    UINT32 val;
    VXB_DEVICE_ID pDev;

    if (pDrvCtrl == NULL)
	return;

    pDev = pDrvCtrl->teiDev;

    val = 0;
    if (enable)
	val = (TEI_MRQC_MODE_RSS_ONLY |
	       TEI_MRQC_RSS_EN_TCPIPV4 |
	       TEI_MRQC_RSS_EN_IPV4 |
	       TEI_MRQC_RSS_EN_IPV6 |
	       TEI_MRQC_RSS_EN_TCPIPV6 |
	       TEI_MRQC_RSS_EN_UDPIPV4 |
	       TEI_MRQC_RSS_EN_UDPIPV6);

    CSR_WRITE_4(pDev, TEI_MRQC, val);
    }

/* CTRL */
void teiFwRedirect
    (
    Ipcom_fw_port_t * port,
    Ip_bool enable
    )
    {
    TEI_DRV_CTRL * pDrvCtrl = member_to_object(port, TEI_DRV_CTRL, fw_port);
    VXB_DEVICE_ID pDev;
    UINT32 reta;
    UINT32 qnum;
    UINT32 offset;
    UINT32 j;
    UINT32 base;

    pDev = pDrvCtrl->teiDev;

    if (pDrvCtrl->fw_port.nRxPollers == 0)
	enable = IP_FALSE;

    /*
     * If enable is IP_FALSE, set the redirection table so that all
     * frames go to RX queue zero. If enable is IP_TRUE, set the
     * redirection table so as to distribute the load
     * among the forwarders that poll this port, i.e. among RX queues
     * base .. base + pDrvCtrl->nRxPollers - 1.
     *
     * Packets for which RSS doesn't apply (e.g. non-IP packets)
     * will still go to queue zero.
     *
     * It must be the case that pDrvCtrl->nRxPollers is less than
     * the number of RX queues supported by this device! That's
     * checked/ensured elsewhere.
     *
     * NOTE, this routine does not touch RXCSUM.PCSD or MRQC.
     */
    base = 0;
    if (TEI_MUX_RX_ENABLED(pDrvCtrl))
	base = 1;

    qnum = base;

    for (offset = 0; offset < TEI_RETA_NUM_ENTRIES; offset += 4)
	{
	reta = 0;
	if (enable)
	    {
	    for (j = 0; j < 32; j += 8)
		{
		reta |= qnum << j;

		if (++qnum == base + pDrvCtrl->fw_port.nRxPollers)
		    qnum = base;
		}
	    }

	CSR_WRITE_4 (pDev, TEI_RETA + offset, reta);
	}

    pDrvCtrl->fw_port.redirState = enable;
    }


/* DEBUG */
void teiCpuAffinityChange
    (
    unsigned int cpuId,
    cpuset_t * orig
    )
    {
    cpuset_t cpus;

    taskCpuAffinityGet (0, orig);

    CPUSET_ZERO (cpus);
    CPUSET_SET (cpus, cpuId);

    taskCpuAffinitySet (0, cpus);
    }

/* DEBUG */
void * teiGetFwd
    (
    void * pTei
    )
    {
    TEI_DRV_CTRL * pDrvCtrl = (TEI_DRV_CTRL *)pTei;

    if (pDrvCtrl == NULL || !pDrvCtrl->fw_port.fw_capable)
	return NULL;

    return &pDrvCtrl->fw_port;
    }

STATUS teiEndStatsFwDump
    (
    TEI_DRV_CTRL * pDrvCtrl
    )
    {
    VXB_DEVICE_ID pDev;
    uint32_t tmp;
    port_statistics_t * ps = &pDrvCtrl->fw_port.stats;

    /*
     * TODO - what are the intended definitions of each of
     * the statistics port_statistics_t?
     *
     * TODO - handle case where both m2Lib and the fwctrl want
     * to read statistics, such that they don't interfere with
     * each other.
     */

    semTake (pDrvCtrl->teiDevSem, WAIT_FOREVER);

    if (!(pDrvCtrl->teiEndObj.flags & IFF_UP))
        {
        semGive (pDrvCtrl->teiDevSem);
        return (ERROR);
        }

    pDev = pDrvCtrl->teiDev;

    /*
     * Note, all the 8259x statistics registers are clear-on-read,
     * And they stick at 0xFF..F when the maximum value is reached(!).
     * The two-word (64-bit) counters clear when the high-order
     * word is read.
     */

    /*
     * Ugh, we need clearer definitions of the desired statistics
     * below. FIXME.
     */

    /* ps->dropped_octets += 0; ? */

    tmp = CSR_READ_4(pDev, TEI_MPC0);  /* we use only hardware
					  'packet buffer' 0 */
    ps->dropped_packets += tmp;
    /* ps->pci_raw_packets += 0; ? */

#ifdef TEI_USE_TOT_PKTS_RCVD
    /*
     * TPR / TORL / TORH include missed packets counted by MPC.
     * Reporting these in 'fwctrl stats' is somewhat confusing,
     * in particular since the 'hardware offloaded' statistic
     * is found via subtraction and there is no 'octets dropped'
     * statistic available.
     */
    tmp = CSR_READ_4(pDev, TEI_TORL);
    ps->octets += tmp;
    tmp = CSR_READ_4(pDev, TEI_TORH);
    ps->octets += (uint64_t)tmp << 32;

    tmp = CSR_READ_4(pDev, TEI_TPR);
    ps->packets += tmp;
#endif

    tmp = CSR_READ_4(pDev, TEI_BPRC);
    ps->broadcast_packets += tmp;

    tmp = CSR_READ_4(pDev, TEI_MPRC);
    ps->multicast_packets += tmp;
    
    tmp = CSR_READ_4(pDev, TEI_PRC64); /* 64 bytes exactly, DA through CRC */
    ps->len_64_packets += tmp;

    tmp = CSR_READ_4(pDev, TEI_PRC127); /* 65 - 127 */
    ps->len_65_127_packets += tmp;

    tmp = CSR_READ_4(pDev, TEI_PRC255); /* 128 - 255 */
    ps->len_128_255_packets += tmp;

    tmp = CSR_READ_4(pDev, TEI_PRC511); /* 256 - 5122 */
    ps->len_256_511_packets += tmp;

    tmp = CSR_READ_4(pDev, TEI_PRC1023); /* 512 - 1023 */
    ps->len_512_1023_packets += tmp;

    /* Hmm,
     * PRC1522 includes both ps->len_1024_1518_packets and
     * ps->len_1519_max_packets.
     * Lie and include all counts in the first.
     */

    tmp = CSR_READ_4(pDev, TEI_PRC1522); /* 1024 - max */
    ps->len_1024_1518_packets += tmp;
    /* ps->len_1519_max_packets += 0; */

    tmp = CSR_READ_4(pDev, TEI_CRCERRS); /* note, CRCERRC includes ALIGNERRC */
    ps->fcs_align_err_packets += tmp;

    tmp = CSR_READ_4(pDev, TEI_RUC);
    ps->runt_packets += tmp;
    
    /* ps->runt_crc_packets = 0; ? */

    tmp = CSR_READ_4(pDev, TEI_ROC);
    ps->oversize_packets += tmp;

    /* ps->oversize_crc_packets = 0; ? */

    tmp = CSR_READ_4(pDev, TEI_GPRC);
    ps->inb_packets += tmp;
#ifndef TEI_USE_TOT_PKTS_RCVD
    ps->packets += tmp;
#endif
    
    tmp = CSR_READ_4(pDev, TEI_GORCL);
    ps->inb_octets += tmp;
#ifndef TEI_USE_TOT_PKTS_RCVD
    ps->octets += tmp;
#endif
    tmp = CSR_READ_4(pDev, TEI_GORCH);
    ps->inb_octets += (uint64_t)tmp << 32;
#ifndef TEI_USE_TOT_PKTS_RCVD
    ps->octets += (uint64_t)tmp << 32;
#endif


#if 0 /* TODO: What to use instead? TPR - GPRC? */
    tmp = CSR_READ_4(pDev, TEI_RXERRC);
    ps->inb_errors += tmp;
#endif
    /* ps->non_fw_frames adjusted per-packet */
    /* ps->non_fw_bytes adjusted per-packet */

    tmp = CSR_READ_4(pDev, TEI_TPT);
    ps->tx_packets += tmp;

    /* TODO: TOTL/TOTR 40C8/40CC not documented. Let's use GOTCL/GOTCH */
    tmp = CSR_READ_4(pDev, TEI_GOTCL);
    ps->tx_octets += tmp;
    tmp = CSR_READ_4(pDev, TEI_GOTCH);
    ps->tx_octets += (uint64_t)tmp << 32;

    semGive (pDrvCtrl->teiDevSem);

    return (OK);
    }

LOCAL void teiFwStatisticsUpdate
    (
    Ipcom_fw_port_t * port,
    Ip_bool reset
    )
    {
    TEI_DRV_CTRL * pDrvCtrl = member_to_object(port, TEI_DRV_CTRL, fw_port);

    (void) teiEndStatsFwDump (pDrvCtrl);

    if (reset)
	{
	/* Hmm, zeroes non_fw stats also... */
	bzero ((caddr_t)&pDrvCtrl->fw_port.stats, sizeof (port_statistics_t));
	}
    }

#endif /* TEI_FORWARDER */
