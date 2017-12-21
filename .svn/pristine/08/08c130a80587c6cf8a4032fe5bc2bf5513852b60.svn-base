/* hwconf.c - Hardware configuration support module for HPC2-8641 */

/*
 * Copyright (c) 2006-2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */
 
/*
modification history
--------------------
01u,12jan12,agf  remove exIrqWkrd from epic resource, deprecated in driver
                 (WIND00176018)
01t,09feb10,liu  add _WRS_VX_AMP for compile error. (WIND00192173)
01s,26jun09,h_w  remove MCB, MOB and VxBus references for MIPC
01r,05jan09,b_m  remove openPicTimer vectors.
01q,31oct08,ebh  Fix MOB_PLB_1 Anchor locations
01p,24oct08,ebh  Change of MOB name for 8572
01o,23oct08,d_l  Add OpenPic timer vector support. (WIND00140273) 
01n,30sep08,dtr  Code fixes and tidyup.
01m,04sep08,dtr  Remove AMP_CORE1 usage - sysDeviceFilter now used.
01l,29aug08,kab  Change _WRS_VX_SMP to _WRS_CONFIG_SMP
01k,11aug08,dtr  Add MIPC low level drivers.
01j,01aug08,dtr  Disable L2 feature in ETSEC.
01i,14jul08,dtr  Remove dynamicInterrupts table.
01h,06jun08,dlk  Make hcfDeviceList entries for e/tsec devices unconditional.
                 Support END2 versions of the drivers also.
01g,04mar08,h_k  swap the init order between epic and ppcIntCtlr.
01f,11oct07,mze  remove reference to DSHM_SM_POOL_BASE as it is now a
                 parameter WIND00104857 (99879)
01e,04sep07,wap  Add support for SMP (WIND00103522)
01d,30aug07,wap  Switch to VxBus ETSEC and PCI drivers (WIND00103172)
01c,21jun07,x_s  Modified to support ppc timer.
01b,07jun06,dtr  Add in vxBus ns16550 driver support.
01a,06mar06,dtr  Initial support for vxBus created from cds8548.
*/

/*
DESCRIPTION
Configuration file for vxBus
*/


#include <vxWorks.h>
#include <vsbConfig.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#include <../src/hwif/h/vxbus/vxbRapidIO.h>
#include "config.h"
#include "sysDuart.h"
#include "mot85xxPci.h"

#include <hwif/vxbus/vxbIntrCtlr.h>

/* FIXME: move vector definitions elsewhere so this file is not needed here */
#include <hwif/intCtlr/vxbEpicIntCtlr.h>
#include <../src/hwif/intCtlr/vxbIntDynaCtlrLib.h>

#define XTSEC_NAME "mottsec"
#if defined (INCLUDE_ETSEC_VXB_END) || defined (INCLUDE_ETSEC_VXB_END2)
#  undef  XTSEC_NAME
#  define XTSEC_NAME "motetsec"
#endif /* INCLUDE_ETSEC_VXB_END || defined (INCLUDE_ETSEC_VXB_END2) */

IMPORT UINT32 divisorGet(UINT32, UINT32);

/*
 * On the SBC8641D board, PHYs are physically wired according
 * to the following table:
 *
 * 		Data port pins	Management port pins	MII address
 *		--------------	--------------------	-----------
 * PHY0:	ETSEC0		ETSEC0			31
 * PHY1:	ETSEC1		ETSEC0			0
 * PHY2:	ETSEC2		ETSEC0			1
 * PHY3:	ETSEC3		ETSEC0			2
 *
 * The tricky part is that both PHYs have their management pins
 * connected to TSEC0. The VxBus ETSEC driver allows us to specify
 * the name of the device that provides PHY management support.
 * This device is "motetsec0" in all cases. We also specify the
 * "phyAddr" resource for each device, which is set according
 * to the MII address in the table above.
 *
 * Note: when configured for AMP mode, the ETSEC interfaces forward
 * the PHY access requests to the "tsecMdio0" device instead. This
 * device is required in order to share access to the MDIO port between
 * the two cores.
 */

#ifdef _WRS_VX_AMP
#ifdef INCLUDE_TSEC_MDIO
LOCAL const struct hcfResource mdio0Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x24000) } },
    { "sharedMem",      HCF_RES_INT,    { (void *)TM_ANCHOR_ADRS } }
};
#define mdio0Num NELEMENTS(mdio0Resources)
#endif /* INCLUDE_TSEC_MDIO */
#endif /* _WRS_VX_AMP */

const struct hcfResource tsecVxbEnd0Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x24000) } },
    { "intr0",          HCF_RES_INT,    { (void *)EPIC_TSEC1TX_INT_VEC } },
    { "intr0Level",     HCF_RES_INT,    { (void *)EPIC_TSEC1TX_INT_VEC } },
    { "intr1",          HCF_RES_INT,    { (void *)EPIC_TSEC1RX_INT_VEC } },
    { "intr1Level",     HCF_RES_INT,    { (void *)EPIC_TSEC1RX_INT_VEC } },
    { "intr2",          HCF_RES_INT,    { (void *)EPIC_TSEC1ERR_INT_VEC } },
    { "intr2Level",     HCF_RES_INT,    { (void *)EPIC_TSEC1ERR_INT_VEC } },
    { "phyAddr",        HCF_RES_INT,    { (void *)31 } },
#ifdef _WRS_VX_AMP
    { "miiIfName",      HCF_RES_STRING, { (void *)"tsecMdio" } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
    { "coreNum",        HCF_RES_INT,    { (void *)0 } }
#else /* _WRS_VX_AMP */
    { "miiIfName",      HCF_RES_STRING, { (void *)XTSEC_NAME } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } }
#endif /* _WRS_VX_AMP */
};
#define tsecVxbEnd0Num NELEMENTS(tsecVxbEnd0Resources)

const struct hcfResource tsecVxbEnd1Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x25000) } },
    { "intr0",          HCF_RES_INT,    { (void *)EPIC_TSEC2TX_INT_VEC } },
    { "intr0Level",     HCF_RES_INT,    { (void *)EPIC_TSEC2TX_INT_VEC } },
    { "intr1",          HCF_RES_INT,    { (void *)EPIC_TSEC2RX_INT_VEC } },
    { "intr1Level",     HCF_RES_INT,    { (void *)EPIC_TSEC2RX_INT_VEC } },
    { "intr2",          HCF_RES_INT,    { (void *)EPIC_TSEC2ERR_INT_VEC } },
    { "intr2Level",     HCF_RES_INT,    { (void *)EPIC_TSEC2ERR_INT_VEC } },
    { "phyAddr",        HCF_RES_INT,    { (void *)0 } },
#ifdef _WRS_VX_AMP
    { "miiIfName",      HCF_RES_STRING, { (void *)"tsecMdio" } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
    { "coreNum",        HCF_RES_INT,    { (void *)0 } }
#else /* _WRS_VX_AMP */
    { "miiIfName",      HCF_RES_STRING, { (void *)XTSEC_NAME } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } }
#endif /* _WRS_VX_AMP */
};
#define tsecVxbEnd1Num NELEMENTS(tsecVxbEnd1Resources)

const struct hcfResource tsecVxbEnd2Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x26000) } },
    { "intr0",          HCF_RES_INT,    { (void *)EPIC_TSEC3TX_INT_VEC } },
    { "intr0Level",     HCF_RES_INT,    { (void *)EPIC_TSEC3TX_INT_VEC } },
    { "intr1",          HCF_RES_INT,    { (void *)EPIC_TSEC3RX_INT_VEC } },
    { "intr1Level",     HCF_RES_INT,    { (void *)EPIC_TSEC3RX_INT_VEC } },
    { "intr2",          HCF_RES_INT,    { (void *)EPIC_TSEC3ERR_INT_VEC } },
    { "intr2Level",     HCF_RES_INT,    { (void *)EPIC_TSEC3ERR_INT_VEC } },
    { "phyAddr",        HCF_RES_INT,    { (void *)1 } },
#ifdef _WRS_VX_AMP
    { "miiIfName",      HCF_RES_STRING, { (void *)"tsecMdio" } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
    { "coreNum",        HCF_RES_INT,    { (void *)1 } },
    { "physMask",       HCF_RES_INT,    { (void *)0x10000000 } }
#else /* _WRS_VX_AMP */
    { "miiIfName",      HCF_RES_STRING, { (void *)XTSEC_NAME } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } }
#endif /* _WRS_VX_AMP */
};
#define tsecVxbEnd2Num NELEMENTS(tsecVxbEnd2Resources)

const struct hcfResource tsecVxbEnd3Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x27000) } },
    { "intr0",          HCF_RES_INT,    { (void *)EPIC_TSEC4TX_INT_VEC } },
    { "intr0Level",     HCF_RES_INT,    { (void *)EPIC_TSEC4TX_INT_VEC } },
    { "intr1",          HCF_RES_INT,    { (void *)EPIC_TSEC4RX_INT_VEC } },
    { "intr1Level",     HCF_RES_INT,    { (void *)EPIC_TSEC4RX_INT_VEC } },
    { "intr2",          HCF_RES_INT,    { (void *)EPIC_TSEC4ERR_INT_VEC } },
    { "intr2Level",     HCF_RES_INT,    { (void *)EPIC_TSEC4ERR_INT_VEC } },
    { "phyAddr",        HCF_RES_INT,    { (void *)2 } },
#ifdef _WRS_VX_AMP
    { "miiIfName",      HCF_RES_STRING, { (void *)"tsecMdio" } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
    { "coreNum",        HCF_RES_INT,    { (void *)1 } },
    { "physMask",       HCF_RES_INT,    { (void *)0x10000000 } }
#else /* _WRS_VX_AMP */
    { "miiIfName",      HCF_RES_STRING, { (void *)XTSEC_NAME } },
    { "miiIfUnit",      HCF_RES_INT, { (void *)0 } }
#endif
};
#define tsecVxbEnd3Num NELEMENTS(tsecVxbEnd3Resources)

#ifdef INCLUDE_PCI_BUS
IMPORT STATUS sysPci1AutoconfigInclude();
IMPORT UCHAR sysPci1AutoconfigIntrAssign(PCI_SYSTEM *, PCI_LOC *, UCHAR);

const struct hcfResource m85xxPci0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,  { (void *)(CCSBAR + 0x8000) } },
    { "mem32Addr",          HCF_RES_ADDR, { (void *)PCI_MEM_ADRS } },
    { "mem32Size",          HCF_RES_INT,  { (void *)PCI_MEM_SIZE } },
    { "memIo32Addr",        HCF_RES_ADDR, { (void *)PCI_MEMIO_ADRS } },
    { "memIo32Size",        HCF_RES_INT,  { (void *)PCI_MEMIO_SIZE } },
    { "io32Addr",           HCF_RES_ADDR, { (void *)PCI_IO_ADRS } },
    { "io32Size",           HCF_RES_INT,  { (void *)PCI_IO_SIZE } },
    { "io16Addr",           HCF_RES_ADDR, { (void *)PCI_IO_ADRS } },
    { "io16Size",           HCF_RES_INT,  { (void *)PCI_IO_SIZE } },
    { "fbbEnable",          HCF_RES_INT,  { (void *)TRUE } },
    { "cacheSize",          HCF_RES_INT,  { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,  { (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet",    HCF_RES_INT,  { (void *)TRUE } },
    { "includeFuncSet",     HCF_RES_ADDR, { (void *)sysPci1AutoconfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR, { (void *)sysPci1AutoconfigIntrAssign } },
    /* non vxbPciAutoConfig() values */
    { "pimmrBase",          HCF_RES_ADDR, { (void *)PCI_BRIDGE_PIMMR_BASE_ADRS } },
    { "lclMemAddr",         HCF_RES_ADDR, { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",      HCF_RES_INT,  { (void *)(512 * 1024 * 1024) } },
    { "mstrMemBus",         HCF_RES_ADDR, { (void *)PCI_MSTR_MEM_BUS } },
    { "maxBusSet",          HCF_RES_INT,  { (void *)8 } },
    /* LAWBAR details */
    { "tgtIf",              HCF_RES_INT,  { (void *)(LAWAR_TGTIF_PCIEX1)} },
    /* Window Attributes - Defaults to 8540 type if none given */
    { "owAttrMem",          HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | PCI_OUT_ATTR_RTT_MEM |  PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrMemIo",        HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | PCI_OUT_ATTR_RTT_MEM |  PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrIo",           HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | PCI_OUT_ATTR_RTT_IO |  PCI_OUT_ATTR_WTT_IO) } },
    { "iwAttr",             HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | PCI_PREFETCHABLE | PCI_IN_ATTR_RTT_LM_READ_SNOOP | PCI_IN_ATTR_RTT_LM_WRITE_SNOOP | PCI_IN_ATTR_TGI_LM) } },
    { "singleLawBar",       HCF_RES_INT,  { (void *)(TRUE)} },
    { "singleLawBarSize",   HCF_RES_INT,  { (void *)(0x10000000)} },
    { "pciConfigMechanism", HCF_RES_INT,  { (void *)(0)} },
    { "autoConfig",         HCF_RES_INT,  { (void *)(TRUE)} },
    { "pciExpressHost",     HCF_RES_INT,  { (void *)(TRUE)} }
#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    ,
    { "msiEnable",          HCF_RES_INT,  { (void *)(TRUE)} },
    { "dynamicInterrupts",  HCF_RES_INT,  { (void *)(TRUE)} },
    { "msiDataWorkaround",      HCF_RES_INT,  { (void *)(TRUE)} },
    { "msiDataWorkaroundAddr",  HCF_RES_INT,  { (void *)(0x80000000)} }
#else
    ,
    { "msiEnable",          HCF_RES_INT,  { (void *)(FALSE)} },
    { "dynamicInterrupts",  HCF_RES_INT,  { (void *)(FALSE)} },
    { "msiDataWorkaround",      HCF_RES_INT,  { (void *)(FALSE)} },
    { "msiDataWorkaroundAddr",  HCF_RES_INT,  { (void *)(0x80000000)} }
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#define m85xxPci0Num NELEMENTS(m85xxPci0Resources)


IMPORT STATUS sysPci2AutoconfigInclude();
IMPORT UCHAR sysPci2AutoconfigIntrAssign(PCI_SYSTEM *, PCI_LOC *, UCHAR);

const struct hcfResource m85xxPci1Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,  { (void *)(CCSBAR + 0x9000) } },
    { "mem32Addr",          HCF_RES_ADDR, { (void *)PCI_MEM_ADRS2 } },
    { "mem32Size",          HCF_RES_INT,  { (void *)PCI_MEM_SIZE } },
    { "memIo32Addr",        HCF_RES_ADDR, { (void *)PCI_MEMIO_ADRS2 } },
    { "memIo32Size",        HCF_RES_INT,  { (void *)PCI_MEMIO_SIZE } },
    { "io32Addr",           HCF_RES_ADDR, { (void *)PCI_IO_ADRS2} },
    { "io32Size",           HCF_RES_INT,  { (void *)PCI_IO_SIZE } },
    { "io16Addr",           HCF_RES_ADDR, { (void *)PCI_IO_ADRS2 } },
    { "io16Size",           HCF_RES_INT,  { (void *)PCI_IO_SIZE } },
    { "fbbEnable",          HCF_RES_INT,  { (void *)TRUE } },
    { "cacheSize",          HCF_RES_INT,  { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,  { (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet",    HCF_RES_INT,  { (void *)TRUE } },
    { "includeFuncSet",     HCF_RES_ADDR, { (void *)sysPci2AutoconfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR, { (void *)sysPci2AutoconfigIntrAssign } },
    /* non vxbPciAutoConfig() values */
    { "pimmrBase",          HCF_RES_ADDR, { (void *)PCI_BRIDGE_PIMMR_BASE_ADRS } },
    { "lclMemAddr",         HCF_RES_ADDR, { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",      HCF_RES_INT,  { (void *)(512 * 1024 * 1024) } },
    { "mstrMemBus",         HCF_RES_ADDR, { (void *)PCI_MSTR_MEM_BUS } },
    { "maxBusSet",          HCF_RES_INT,  { (void *)8 } },
    /* LAWBAR details */
    { "tgtIf",              HCF_RES_INT,  { (void *)(LAWAR_TGTIF_PCIEX2)} },
    /* Window Attributes - Defaults to 8540 type if none given */
    { "owAttrMem",          HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | PCI_OUT_ATTR_RTT_MEM |  PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrMemIo",        HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | PCI_OUT_ATTR_RTT_MEM |  PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrIo",           HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | PCI_OUT_ATTR_RTT_IO |  PCI_OUT_ATTR_WTT_IO) } },
    { "iwAttr",             HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | PCI_PREFETCHABLE | PCI_IN_ATTR_RTT_LM_READ_SNOOP | PCI_IN_ATTR_RTT_LM_WRITE_SNOOP | PCI_IN_ATTR_TGI_LM) } },
    { "singleLawBar",       HCF_RES_INT,  { (void *)(TRUE)} },
    { "singleLawBarSize",   HCF_RES_INT,  { (void *)(0x10000000)} },
    { "pciConfigMechanism", HCF_RES_INT,  { (void *)(0)} },
    { "autoConfig",         HCF_RES_INT,  { (void *)(TRUE)} },
    { "pciExpressHost",     HCF_RES_INT,  { (void *)(TRUE)} }
#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    ,
    { "msiEnable",          HCF_RES_INT,  { (void *)(TRUE)} },
    { "dynamicInterrupts",  HCF_RES_INT,  { (void *)(TRUE)} },
    { "msiDataWorkaround",      HCF_RES_INT,  { (void *)(TRUE)} },
    { "msiDataWorkaroundAddr",  HCF_RES_INT,  { (void *)(0x80000000)} }
#else
    ,
    { "msiEnable",          HCF_RES_INT,  { (void *)(FALSE)} },
    { "dynamicInterrupts",  HCF_RES_INT,  { (void *)(FALSE)} },
    { "msiDataWorkaround",      HCF_RES_INT,  { (void *)(FALSE)} },
    { "msiDataWorkaroundAddr",  HCF_RES_INT,  { (void *)(0x80000000)} }
#endif /*INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#define m85xxPci1Num NELEMENTS(m85xxPci1Resources)

#endif /* INCLUDE_PCI_BUS */

const struct hcfResource m85xxCCSR0Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)CCSBAR  } },
    { "LAWAR_ENABLE",		HCF_RES_INT, {(void *)LAWAR_ENABLE } },
    { "LAWBAR_ADRS_SHIFT",	HCF_RES_INT, {(void *)LAWBAR_ADRS_SHIFT } },
    { "LAWBAR0",		HCF_RES_STRING,     { (void *)"reserved" } },
    { "LAWBAR1",		HCF_RES_STRING,     { (void *)"reserved" } },
    { "LAWBAR2",		HCF_RES_STRING,     { (void *)"reserved" } },
    { "LAWBAR3",		HCF_RES_STRING,     { (void *)"reserved" } },
    { "LAWBAR4",		HCF_RES_STRING,     { (void *)NULL } },
    { "LAWBAR5",		HCF_RES_STRING,     { (void *)"m85xxPci" } },
    { "LAWBAR6",		HCF_RES_STRING,     { (void *)"m85xxPci" } },
    { "LAWBAR7",		HCF_RES_STRING,     { (void *)NULL } },
    { "LAWBAR8",		HCF_RES_STRING,     { (void *)NULL } },
    { "LAWBAR9",		HCF_RES_STRING,     { (void *)NULL } }
};

#define m85xxCCSR0Num NELEMENTS(m85xxCCSR0Resources)

#ifdef INCLUDE_RAPIDIO_BUS
const struct hcfResource m85xxRio0Resources[] = {
{ "regBase",	HCF_RES_INT, {(void *)RAPIDIO_BASE} },
{ "deviceBase", HCF_RES_INT, {(void *)(RAPIDIO_ADRS + 0x0000000)}},
{ "deviceSize", HCF_RES_INT, {(void *)(RAPIDIO_SIZE - 0x0000000)}},
{ "rioBusAdrs", HCF_RES_INT, {(void *)RAPIDIO_BUS_ADRS }},
{ "rioBusSize", HCF_RES_INT, {(void *)RAPIDIO_BUS_SIZE }}
};
#define m85xxRio0Num    NELEMENTS(m85xxRio0Resources)

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

const struct hcfResource m85xxLawBarResources[] = {
{ "regBase",	HCF_RES_INT, {(void *)CCSBAR} },
{ "LAWBAR0",	HCF_RES_INT, {"reserved"} },
{ "LAWBAR1",	HCF_RES_INT, {"reserved"} },
{ "LAWBAR2",	HCF_RES_INT, {NULL} },
{ "LAWBAR3",	HCF_RES_INT, {"reserved"} },
{ "LAWBAR4",	HCF_RES_INT, {"reserved"} },
{ "LAWBAR5",	HCF_RES_INT, {NULL} },
{ "LAWBAR6",	HCF_RES_INT, {"reserved"} },
{ "LAWBAR7",	HCF_RES_INT, {"m85xxRio"} }

};

#define m85xxLawBarNum    NELEMENTS(m85xxLawBarResources)

const struct hcfResource smEnd0Resources[] = {
{ "regBase",	HCF_RES_INT, {(void *)CCSBAR} },
{ "SM_ANCHOR_OFFSET", HCF_RES_INT, {(void *)SM_ANCHOR_OFFSET}},
{ "SM_MEM_ADRS", HCF_RES_INT,{(void *)SM_MEM_ADRS }},
{ "SM_MEM_SIZE", HCF_RES_INT,{(void *)SM_MEM_SIZE}},
{ "SM_TAS_TYPE", HCF_RES_INT,{(void *)SM_TAS_TYPE} },
{ "SM_INT_TYPE", HCF_RES_INT,{(void *)SM_INT_TYPE} },
{ "SM_INT_ARG1", HCF_RES_INT,{(void *)SM_INT_ARG1}},
{ "SM_INT_ARG2", HCF_RES_INT, {(void *)SM_INT_ARG2}},
{ "SM_INT_ARG3", HCF_RES_INT, {(void *)SM_INT_ARG3}},
{ "SM_MBLK_NUM", HCF_RES_INT, {(void *)600}},
{ "SM_CBLK_NUM", HCF_RES_INT, {(void *)200}}
};

#define smEnd0Num    NELEMENTS(smEnd0Resources)
#endif /* INCLUDE_RAPIDIO_BUS */
const struct hcfResource ns1655x1Resources[] =
    {
        { "regBase",     HCF_RES_INT, {(void *)COM1_ADR} },
        { "irq",         HCF_RES_INT, {(void *)EPIC_DUART_INT_VEC} },
        { "regInterval", HCF_RES_INT, {(void *)DUART_REG_ADDR_INTERVAL} },
        { "irqLevel",    HCF_RES_INT, {(void *)EPIC_DUART_INT_VEC} },
        { "divsorCalc",	 HCF_RES_ADDR, {(void *)&divisorGet} },
        { "clkFreq",	 HCF_RES_ADDR, {(void *)&sysClkFreqGet} }

    };

#define ns1655x1Num NELEMENTS(ns1655x1Resources)

const struct hcfResource ns1655x2Resources[] =
    {
        { "regBase",     HCF_RES_INT, {(void *)COM2_ADR} },
        { "irq",         HCF_RES_INT, {(void *)EPIC_DUART2_INT_VEC} },
        { "regInterval", HCF_RES_INT, {(void *)DUART_REG_ADDR_INTERVAL} },
        { "irqLevel",    HCF_RES_INT, {(void *)EPIC_DUART_INT_VEC} },
        { "divsorCalc",	 HCF_RES_ADDR, {(void *)&divisorGet} },
        { "clkFreq",	 HCF_RES_ADDR, {(void *)&sysClkFreqGet} }

    };
#define ns1655x2Num NELEMENTS(ns1655x2Resources)

struct intrCtlrInputs epicInputs[] = {
    { EPIC_DUART2_INT_VEC,   "ns16550", 1, 0 },
    { EPIC_TSEC1TX_INT_VEC,  XTSEC_NAME, 0, 0 },
    { EPIC_TSEC1RX_INT_VEC,  XTSEC_NAME, 0, 1 },
    { EPIC_TSEC3TX_INT_VEC,  XTSEC_NAME, 2, 0 },
    { EPIC_TSEC3RX_INT_VEC,  XTSEC_NAME, 2, 1 },
    { EPIC_TSEC3ERR_INT_VEC, XTSEC_NAME, 2, 2 },
    { EPIC_TSEC1ERR_INT_VEC, XTSEC_NAME, 0, 2 },
    { EPIC_TSEC2TX_INT_VEC,  XTSEC_NAME, 1, 0 },
    { EPIC_TSEC2RX_INT_VEC,  XTSEC_NAME, 1, 1 },
    { EPIC_TSEC4TX_INT_VEC,  XTSEC_NAME, 3, 0 },
    { EPIC_TSEC4RX_INT_VEC,  XTSEC_NAME, 3, 1 },
    { EPIC_TSEC4ERR_INT_VEC, XTSEC_NAME, 3, 2 },
    { EPIC_TSEC2ERR_INT_VEC, XTSEC_NAME, 1, 2 },
    { EPIC_DUART_INT_VEC,    "ns16550", 0, 0},
#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    { OPENPIC_TIMERA0_INT_VEC,    "openPicTimer", 0, 0 },
    { OPENPIC_TIMERA1_INT_VEC,    "openPicTimer", 0, 1 },
    { OPENPIC_TIMERA2_INT_VEC,    "openPicTimer", 0, 2 },
    { OPENPIC_TIMERA3_INT_VEC,    "openPicTimer", 0, 3 },
    { OPENPIC_TIMERB0_INT_VEC,    "openPicTimer", 1, 0 },
    { OPENPIC_TIMERB1_INT_VEC,    "openPicTimer", 1, 1 },
    { OPENPIC_TIMERB2_INT_VEC,    "openPicTimer", 1, 2 },
    { OPENPIC_TIMERB3_INT_VEC,    "openPicTimer", 1, 3 },    
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
#ifdef _WRS_CONFIG_SMP
    { EPIC_VEC_IPI_IRQ0,     "ipi", 0, 0 }
#else
#ifdef INCLUDE_DSHM_BUS_PLB
    { EPIC_VEC_IPI_IRQ0,     "dshmBusCtlr8641", 0, 0 }
#endif
#endif /* _WRS_CONFIG_SMP */
};

struct intrCtlrPriority epicPriority[] = {
    { EPIC_DUART2_INT_VEC, 100 },
    { EPIC_DUART_INT_VEC,  100 }
};

struct intrCtlrXBar epicXBar[] = {
    { 0, 0 }
};

struct intrCtlrCpu epicCpu[] = {
    { EPIC_TSEC3ERR_INT_VEC, 1 },
    { EPIC_TSEC1ERR_INT_VEC, 1 },
    { EPIC_TSEC4ERR_INT_VEC, 1 },
    { EPIC_TSEC2ERR_INT_VEC, 1 }
};

const struct hcfResource epic0Resources[] = {
    { VXB_REG_BASE,        HCF_RES_INT,  {(void *)CCSBAR } },
    { "input",             HCF_RES_ADDR, {(void *)&epicInputs[0] } },
    { "inputTableSize",    HCF_RES_INT,  {(void *)NELEMENTS(epicInputs) } },
    { "priority",          HCF_RES_ADDR, {(void *)&epicPriority[0] } },
    { "priorityTableSize", HCF_RES_INT,  {(void *)NELEMENTS(epicPriority) } },
    { "crossBar",          HCF_RES_ADDR, {(void *)&epicXBar[0] } },
    { "crossBarTableSize", HCF_RES_INT,  {(void *)NELEMENTS(epicXBar) } },
    { "cpuRoute",          HCF_RES_ADDR, {(void *)&epicCpu[0] } },
    { "cpuRouteTableSize", HCF_RES_INT,  {(void *)NELEMENTS(epicCpu) } },
    { "exPolar",           HCF_RES_INT,  {(void *)EPIC_INT_ACT_LOW } },
    { "exSense",           HCF_RES_INT,  {(void *)EPIC_SENSE_LVL } },
};
#define epic0Num NELEMENTS(epic0Resources)

struct intrCtlrInputs ppcIntCtlrInputs[] = {
    { 0, "epic", 0, 0 },
    { 1, "ppcDecTimerDev", 0, 0 }
};

const struct hcfResource ppcIntCtlr0Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,  {(void *)TRUE} },
    { "input",          HCF_RES_ADDR, {(void *)&ppcIntCtlrInputs[0] } },
    { "inputTableSize", HCF_RES_INT,  {(void *)NELEMENTS(ppcIntCtlrInputs) } },
};
#define ppcIntCtlr0Num NELEMENTS(ppcIntCtlr0Resources)

const struct hcfResource ppcDecTimerDevResources[] = 
    {
    {"regBase", HCF_RES_INT, {(void *)0}},
    {"decClockFreq",HCF_RES_ADDR,{(void*)sysClkFreqGet}},
    {"minClkRate", HCF_RES_INT,{(void *)SYS_CLK_RATE_MIN}},
    {"maxClkRate", HCF_RES_INT,{(void *)SYS_CLK_RATE_MAX}}
    };

#define ppcDecTimerDevNum NELEMENTS(ppcDecTimerDevResources)

const struct hcfResource openPicTimerDevAresources[] =
    {
    {VXB_REG_BASE,   HCF_RES_INT,    {(void *)(CCSBAR + 0x000410F0)}},
    {"instanceName", HCF_RES_STRING, {(void *)"A" }},
    {VXB_CLK_FREQ,   HCF_RES_ADDR,   {(void *)sysPicClkFreqGet}},
    {"clkDivisor",   HCF_RES_INT,    {(void *)8}},
    };
#define openPicTimerDevAnum NELEMENTS(openPicTimerDevAresources)

const struct hcfResource openPicTimerDevBresources[] =
    {
    {VXB_REG_BASE,   HCF_RES_INT,    {(void *)(CCSBAR + 0x000420F0)}},
    {"instanceName", HCF_RES_STRING, {(void *)"B" }},
    {VXB_CLK_FREQ,   HCF_RES_ADDR,   {(void *)sysPicClkFreqGet}},
    {"clkDivisor",   HCF_RES_INT,    {(void *)0}},
    };
#define openPicTimerDevBnum NELEMENTS(openPicTimerDevBresources)

#ifdef INCLUDE_DSHM_BUS_PLB
void * dshmRegBaseDummy [10];   /* these are not real drivers, but an entry is
                                 * needed by the vxbus subsystem */

#define DSHM_ANCHOR_BASE        (0x10000000 + SM_ANCHOR_OFFSET)

/* Virtual bus controller managing the local node. Most values are user-defined
 * by component parameters. The intr and intrLevel entries are specific to
 * this BSP and should not be changed. The real node, anchor and smStart
 * parameters will be computed at runtime, depending on which core is running.
 * It has to be done this way since the same VxWorks image runs on the
 * different cores.
 */
const struct hcfResource dshmBusCtlrPlbRes[] = {
    { "regBase",    RES_INT,  (void *) dshmRegBaseDummy },
    { "bus_type",   RES_ADDR, (void *) "plb" },
    { "intr",       RES_INT,  (void *) EPIC_VEC_IPI_IRQ0},
    { "intrLevel",  RES_INT,  (void *) EPIC_VEC_IPI_IRQ0},
    { "anchorBase", RES_ADDR, (void *) DSHM_ANCHOR_BASE},
    { "smStart",    RES_ADDR, (void *) DSHM_SM_POOL_BASE },
    { "szPool",     RES_INT,  (void *) DSHM_BUS_PLB_POOLSIZE },
    { "rmw",        RES_ADDR, (void *) DSHM_BUS_PLB_RMW },
    { "node",       RES_INT,  (void *) -1 },
    { "nRetries",   RES_INT,  (void *) DSHM_BUS_PLB_NRETRIES },
    { "maxNodes",   RES_INT,  (void *) DSHM_BUS_PLB_MAXNODES },
    { "nEntries",   RES_INT,  (void *) DSHM_BUS_PLB_NENTRIES },
    { "szEntry",    RES_INT,  (void *) DSHM_BUS_PLB_ENTRY_SIZE},
};
#endif /* INCLUDE_DSHM_BUS_PLB */

/* Peer devices that represent each possible peer in the system. The intr and
 * intrLevel are set to -1 since they are needed but unused, the routine
 * sysEpicIpiGen() taking care of triggering the interrupt on the correct
 * core with the correct vector and level. The ISR is installed in the bus
 * controller, which itself needs the real value for both intr and intrLevel.
 *
 * Since the AMP support for this board is using the same the same image on
 * both cores, the anchor address for shared memory messaging cannot be set
 * statically here, but rather it must be obtained at runtime, depending on
 * the core. What is passed here is a base address that is used as a starting
 * point to allocate each core's anchor. The DSHM bus controller and peer
 * drivers then know how to find the real anchor based on its size and cache
 * alignment.
 */
#ifdef INCLUDE_DSHM_BUS_PLB
const struct hcfResource dshmPeerVxPlb0[] = {
    { "regBase",    RES_INT,  (void *) dshmRegBaseDummy },
    { "intr",       RES_INT,  (void *) -1},
    { "intrLevel",  RES_INT,  (void *) -1},
    { "node",       RES_INT,  (void *) 0},
    { "anchor",     RES_ADDR, (void *) DSHM_ANCHOR_BASE},
};

const struct hcfResource dshmPeerVxPlb1[] = {
    { "regBase",    RES_INT,  (void *) dshmRegBaseDummy },
    { "intr",       RES_INT,  (void *) -1},
    { "intrLevel",  RES_INT,  (void *) -1},
    { "node",       RES_INT,  (void *) 1},
    { "anchor",     RES_ADDR, (void *) DSHM_ANCHOR_BASE},
};
#endif


/* end of distributed shared memory */

const struct hcfDevice hcfDeviceList[] = {
    /*
     * Initialize ppcIntCtlr before epic.
     * The vector table for external interrupts are over written by epic
     * for an optimized purpose.
     */

    { "ppcIntCtlr", 0, VXB_BUSID_PLB, 0, ppcIntCtlr0Num, ppcIntCtlr0Resources },
    { "epic", 0,       VXB_BUSID_PLB, 0, epic0Num, epic0Resources },

#ifdef _WRS_VX_AMP
#ifdef INCLUDE_TSEC_MDIO
    { "tsecMdio", 0, VXB_BUSID_PLB, 0, mdio0Num, mdio0Resources },
#endif /* INCLUDE_TSEC_MDIO */
#endif /* _WRS_VX_AMP */

    { XTSEC_NAME, 0, VXB_BUSID_PLB, 0, tsecVxbEnd0Num, tsecVxbEnd0Resources },
    { XTSEC_NAME, 1, VXB_BUSID_PLB, 0, tsecVxbEnd1Num, tsecVxbEnd1Resources },
    { XTSEC_NAME, 2, VXB_BUSID_PLB, 0, tsecVxbEnd2Num, tsecVxbEnd2Resources },
    { XTSEC_NAME, 3, VXB_BUSID_PLB, 0, tsecVxbEnd3Num, tsecVxbEnd3Resources },

    { "ns16550", 0, VXB_BUSID_PLB, 0, ns1655x1Num, ns1655x1Resources },
    { "ns16550", 1, VXB_BUSID_PLB, 0, ns1655x2Num, ns1655x2Resources },
/* This is a static way to ensure only one core accesses PCI/LAWBARs */
#ifdef INCLUDE_PCI_BUS
    { "m85xxCCSR", 0, VXB_BUSID_PLB,0, m85xxCCSR0Num, m85xxCCSR0Resources },
    { "m85xxPci",  0, VXB_BUSID_PLB,0, m85xxPci0Num,  m85xxPci0Resources },
    { "m85xxPci",  1, VXB_BUSID_PLB,0, m85xxPci1Num,  m85xxPci1Resources },
#endif /* INCLUDE_PCI_BUS */

    { "ppcDecTimerDev", 0, VXB_BUSID_PLB,0, ppcDecTimerDevNum,   ppcDecTimerDevResources },

    { "openPicTimer", 0,   VXB_BUSID_PLB,0, openPicTimerDevAnum, openPicTimerDevAresources },
    { "openPicTimer", 1,   VXB_BUSID_PLB,0, openPicTimerDevBnum, openPicTimerDevBresources },

#ifdef INCLUDE_RAPIDIO_BUS
    { "m85xxRio", 0, VXB_BUSID_PLB, 0, m85xxRio0Num, m85xxRio0Resources },
    { "m85xxCPU", 0, VXB_BUSID_RAPIDIO, 0, m85xxCPU0Num, m85xxCPU0Resources },
    { "m85xxCPU", 1, VXB_BUSID_RAPIDIO, 0, m85xxCPU1Num, m85xxCPU1Resources },
    { "smEnd", 0, VXB_BUSID_PLB,0, smEnd0Num, smEnd0Resources}
#endif

#ifdef INCLUDE_DSHM
    { "dshmBusCtlr8641", 0, VXB_BUSID_PLB, 0,
        NELEMENTS(dshmBusCtlrPlbRes), dshmBusCtlrPlbRes },
    { "dshmPeerVx8641", 0, VXB_BUSID_VIRTUAL, 0,
        NELEMENTS(dshmPeerVxPlb0), dshmPeerVxPlb0 },
    { "dshmPeerVx8641", 1, VXB_BUSID_VIRTUAL, 0,
        NELEMENTS(dshmPeerVxPlb1), dshmPeerVxPlb1 },
#endif

    { NULL }
};
const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] =
    {
    { XTSEC_NAME, 0, "l2disable", VXB_PARAM_INT32,{(void *) 1} },
    { XTSEC_NAME, 1, "l2disable", VXB_PARAM_INT32,{(void *) 1} },
    { XTSEC_NAME, 2, "l2disable", VXB_PARAM_INT32,{(void *) 1} },
    { XTSEC_NAME, 3, "l2disable", VXB_PARAM_INT32,{(void *) 1} },
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, {(void *)0} }
    };
