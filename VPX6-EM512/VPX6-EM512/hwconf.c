/* hwconf.c - Hardware configuration support module */

/*
 * Copyright (c) 2006-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01k,24oct08,y_w  Fixed the build error when INCLUDE_RAPIDIO_BUS is defined.
                 (WIND00140932)
01j,20jun08,wap  Increase maximum bus count from 3 to 16
01h,06jun08,dlk  Make entries for e/tsec devices unconditional.
                 Support END2 versions of the drivers also.
01i,20sep07,b_m  add ns16550, ppcIntCtlr, epic, m85xxTimer vxbus drivers.
01h,10sep07,wap  Switch to VxBus PCI driver (WIND00104076)
01g,07aug07,dtr  Update for latest RIO driver.
01f,02aug07,h_k  added LAWAR_ENABLE and LAWBAR_ADRS_SHIFT.
01e,31jul07,agf  change tsec hEND to vxBus END
01d,09apr07,b_m  modify to use m85xxCCSR driver.
01c,09mar06,wap  Allow either mottsec or motetsec driver to be used
                 (SPR #118829)
01b,07feb06,wap  Add VxBus parameter table
01a,16jan06,dtr  written.
*/

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/vxbus/hwConf.h>
#include <../src/hwif/h/vxbus/vxbRapidIO.h>
#include <hwif/util/vxbParamSys.h>
#include <hwif/vxbus/vxbIntrCtlr.h>
#include <hwif/intCtlr/vxbEpicIntCtlr.h>
#include <config.h>
#include "mot85xxPci.h"

#define XTSEC_NAME "mottsec"
#if defined (INCLUDE_ETSEC_VXB_END) || defined (INCLUDE_ETSEC_VXB_END2)
#  undef  XTSEC_NAME
#  define XTSEC_NAME "motetsec"
#endif /* defined(INCLUDE_ETSEC_VXB_END) || defined(INCLUDE_ETSEC_VXB_END2) */

/*
 * On the SBC8548 board, PHYs are physically wired according
 * to the following table:
 *
 * 		Data port pins	Management port pins	MII address
 *		--------------	--------------------	-----------
 * PHY0:	TSEC0		TSEC0			25
 * PHY1:	TSEC1		TSEC0			26
 * PHY2: (note) TSEC2           TSEC0                   27 (note)
 * PHY3: (note) TSEC3           TSEC0                   28 (note)
 *
 * The tricky part is that both PHYs have their management pins
 * connected to TSEC0. The VxBus TSEC driver allows us to specify
 * the name of the device that provides PHY management support.
 * This device is "mottsec0" in both cases. We also specify the
 * "phyAddr" resource for each device, which is 25 for TSEC0
 * and 26 for TSEC1.
 *
 * Note: PHY2 and PHY3 do not exist on the SBC8548, but the corresponding
 * TSEC signals are routed to a high-speed expansion connector. So, as a
 * placeholder for the following structures, addresses 27 and 28 were
 * entered for the phyAddr fields.
 *
 */

const struct hcfResource tsecVxbEnd0Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x24000) } },
    { "intr0",          HCF_RES_INT,    { (void *)EPIC_TSEC1TX_INT_VEC } },
    { "intr0Level",     HCF_RES_INT,    { (void *)EPIC_TSEC1TX_INT_VEC } },
    { "intr1",          HCF_RES_INT,    { (void *)EPIC_TSEC1RX_INT_VEC } },
    { "intr1Level",     HCF_RES_INT,    { (void *)EPIC_TSEC1RX_INT_VEC } },
    { "intr2",          HCF_RES_INT,    { (void *)EPIC_TSEC1ERR_INT_VEC } },
    { "intr2Level",     HCF_RES_INT,    { (void *)EPIC_TSEC1ERR_INT_VEC } },
    { "phyAddr",        HCF_RES_INT,    { (void *)1 } },
    { "miiIfName",      HCF_RES_STRING, { (void *)XTSEC_NAME } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } }
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
    { "phyAddr",        HCF_RES_INT,    { (void *)2 } },
    { "miiIfName",      HCF_RES_STRING, { (void *)XTSEC_NAME } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } }
};
#define tsecVxbEnd1Num NELEMENTS(tsecVxbEnd1Resources)

#ifdef INCLUDE_OPTIONAL_TSECS
const struct hcfResource tsecVxbEnd2Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x26000) } },
    { "intr0",          HCF_RES_INT,    { (void *)EPIC_TSEC3TX_INT_VEC } },
    { "intr0Level",     HCF_RES_INT,    { (void *)EPIC_TSEC3TX_INT_VEC } },
    { "intr1",          HCF_RES_INT,    { (void *)EPIC_TSEC3RX_INT_VEC } },
    { "intr1Level",     HCF_RES_INT,    { (void *)EPIC_TSEC3RX_INT_VEC } },
    { "intr2",          HCF_RES_INT,    { (void *)EPIC_TSEC3ERR_INT_VEC } },
    { "intr2Level",     HCF_RES_INT,    { (void *)EPIC_TSEC3ERR_INT_VEC } },
    { "phyAddr",        HCF_RES_INT,    { (void *)3 } },
    { "miiIfName",      HCF_RES_STRING, { (void *)XTSEC_NAME } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
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
    { "phyAddr",        HCF_RES_INT,    { (void *)4 } },
    { "miiIfName",      HCF_RES_STRING, { (void *)XTSEC_NAME } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
};
#define tsecVxbEnd3Num NELEMENTS(tsecVxbEnd3Resources)
#endif /* INCLUDE_OPTIONAL_TSECS */

#ifdef INCLUDE_RAPIDIO_BUS
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

#ifdef DRV_RESOURCE_M85XXCCSR
const struct hcfResource m85xxLawBarResources[] = {
    { "regBase", HCF_RES_INT, { (void *)CCSBAR  } },
    { "lawBarNum",              HCF_RES_INT, {(void *)8} },
    { "LAWAR_ENABLE",		HCF_RES_INT, {(void *)LAWAR_ENABLE } },
    { "LAWBAR_ADRS_SHIFT",	HCF_RES_INT, {(void *)LAWBAR_ADRS_SHIFT } },
    { "LAWBAR0",		HCF_RES_STRING,     { (void *)"reserved" } },
    { "LAWBAR1",		HCF_RES_STRING,     { (void *)"reserved" } },
    { "LAWBAR2",		HCF_RES_STRING,     { (void *)"reserved" } },
    { "LAWBAR3",		HCF_RES_STRING,     { (void *)"reserved" } },
    { "LAWBAR4",		HCF_RES_STRING,     { (void *)"reserved" } },
    { "LAWBAR5",		HCF_RES_STRING,     { (void *)"m85xxPci" } },
    { "LAWBAR6",		HCF_RES_STRING,     { (void *)"reserved" } },
    { "LAWBAR7",		HCF_RES_STRING,     { (void *)"reserved" } }
};
#define m85xxLawBarNum    NELEMENTS(m85xxLawBarResources)
#endif /* DRV_RESOURCE_M85XXCCSR */

#ifdef DRV_PCIBUS_M85XX
IMPORT STATUS sysPci1AutoconfigInclude(PCI_SYSTEM *, PCI_LOC *, UINT);
IMPORT UCHAR sysPci1AutoconfigIntrAssign(PCI_SYSTEM *, PCI_LOC *, UCHAR);
IMPORT STATUS sysPci3AutoconfigInclude(PCI_SYSTEM *, PCI_LOC *, UINT);
IMPORT UCHAR sysPci3AutoconfigIntrAssign(PCI_SYSTEM *, PCI_LOC *, UCHAR);
#if 0
const struct hcfResource m85xxPci0Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x8000) } },
    { "mem32Addr",      HCF_RES_ADDR,   { (void *)PCI_MEM_ADRS } },
    { "mem32Size",      HCF_RES_INT,    { (void *)PCI_MEM_SIZE } },
    { "memIo32Addr",    HCF_RES_ADDR,   { (void *)PCI_MEMIO_ADRS } },
    { "memIo32Size",    HCF_RES_INT,    { (void *)PCI_MEMIO_SIZE } },
    { "io32Addr",       HCF_RES_ADDR,   { (void *)PCI_IO_ADRS } },
    { "io32Size",       HCF_RES_INT,    { (void *)PCI_IO_SIZE } },
    { "fbbEnable",      HCF_RES_INT,    { (void *)TRUE } },
    { "cacheSize",      HCF_RES_INT,    { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",   HCF_RES_INT,    { (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet",HCF_RES_INT,    { (void *)TRUE } },
    { "includeFuncSet", HCF_RES_ADDR,   { (void *)sysPci1AutoconfigInclude } },
    { "intAssignFuncSet",HCF_RES_ADDR,  { (void *)sysPci1AutoconfigIntrAssign }},
    /* non vxbPciAutoConfig() values */
    { "pimmrBase",      HCF_RES_ADDR,   { (void *)PCI_BRIDGE_PIMMR_BASE_ADRS }},
    { "lclMemAddr",     HCF_RES_ADDR,   { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",  HCF_RES_INT,    { (void *)(512 * 1024 * 1024) } },
    { "mstrMemBus",     HCF_RES_ADDR,   { (void *)PCI_MSTR_MEM_BUS } },
    { "pciConfigMechanism",HCF_RES_INT, { (void *)(0)} },
    { "maxBusSet",      HCF_RES_INT,    { (void *)16 } },
    /* LAWBAR details */
    { "singleLawBar",   HCF_RES_INT,    { (void *)(TRUE)} },
    { "singleLawBarSize",HCF_RES_INT,   { (void *)(0x10000000)} },
    { "tgtIf",          HCF_RES_INT,    { (void *)(LAWAR_TGTIF_PCI)} },
    { "autoConfig",     HCF_RES_INT,    { (void *)(TRUE)} }
};

#define m85xxPci0Num NELEMENTS(m85xxPci0Resources)
#endif
const struct hcfResource m85xxPci1Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,  { (void *)(CCSBAR + 0xA000) } },
    { "mem32Addr",          HCF_RES_ADDR, { (void *)PCI_MEM_ADRS3 } },
    { "mem32Size",          HCF_RES_INT,  { (void *)PCI_MEM_SIZE } },
    { "memIo32Addr",        HCF_RES_ADDR, { (void *)PCI_MEMIO_ADRS3 } },
    { "memIo32Size",        HCF_RES_INT,  { (void *)PCI_MEMIO_SIZE } },
    { "io32Addr",           HCF_RES_ADDR, { (void *)PCI_IO_ADRS3 } },
    { "io32Size",           HCF_RES_INT,  { (void *)PCI_IO_SIZE } },
    { "io16Addr",           HCF_RES_ADDR, { (void *)PCI_IO_ADRS3 } },
    { "io16Size",           HCF_RES_INT,  { (void *)PCI_IO_SIZE } },
    { "fbbEnable",          HCF_RES_INT,  { (void *)TRUE } },
    { "cacheSize",          HCF_RES_INT,  { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,  { (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet",    HCF_RES_INT,  { (void *)TRUE } },
    { "includeFuncSet",     HCF_RES_ADDR, { (void *)sysPci3AutoconfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR, { (void *)sysPci3AutoconfigIntrAssign } },

    /*{ "irq",         HCF_RES_INT, {(void *)EPIC_PCI_EXPRESS_INT_VEC} },*/
        
    /* non vxbPciAutoConfig() values  */
    { "pimmrBase",          HCF_RES_ADDR, { (void *)PCI_BRIDGE_PIMMR_BASE_ADRS } },
    { "lclMemAddr",         HCF_RES_ADDR, { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",      HCF_RES_INT,  { (void *)(512 * 1024 * 1024) } },
    { "mstrMemBus",         HCF_RES_ADDR, { (void *)PCI_MSTR_MEM_BUS} },
    { "maxBusSet",          HCF_RES_INT,  { (void *)16 } },
    /* LAWBAR details */
    { "tgtIf",              HCF_RES_INT,  { (void *)(LAWAR_TGTIF_PCIEX)} },
    /* Window Attributes - Defaults to 8540 type if none given */
    { "owAttrMem",          HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | PCI_OUT_ATTR_RTT_MEM |  PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrMemIo",        HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | PCI_OUT_ATTR_RTT_MEM |  PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrIo",           HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | PCI_OUT_ATTR_RTT_IO |  PCI_OUT_ATTR_WTT_IO) } },
    { "iwAttr",             HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | PCI_IN_ATTR_RTT_LM_READ_SNOOP | PCI_IN_ATTR_RTT_LM_WRITE_SNOOP | PCI_IN_ATTR_TGI_LM) } },
    { "singleLawBar",       HCF_RES_INT,  { (void *)(TRUE)} },
    { "singleLawBarSize",   HCF_RES_INT,  { (void *)(0x40000000)} },
    { "pciConfigMechanism", HCF_RES_INT,  { (void *)(0)} },
    { "autoConfig",         HCF_RES_INT,  { (void *)(TRUE)} },
    { "pciExpressHost",     HCF_RES_INT,  { (void *)(TRUE)} },       
#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "msiEnable",          HCF_RES_INT,  { (void *)(TRUE)} },
    { "dynamicInterrupts",  HCF_RES_INT,  { (void *)(TRUE)} },
    { "msiDataWorkaround",  HCF_RES_INT,  { (void *)(TRUE)} },
    { "msiDataWorkaroundAddr",HCF_RES_INT,  { (void *)(0xc0000000)} }
#else
    { "msiEnable",          HCF_RES_INT,  { (void *)(FALSE)} },
    { "dynamicInterrupts",  HCF_RES_INT,  { (void *)(FALSE)} },
    { "msiDataWorkaround",      HCF_RES_INT,  { (void *)(FALSE)} },
    { "msiDataWorkaroundAddr",  HCF_RES_INT,  { (void *)(0xc0000000)} }
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#define m85xxPci1Num NELEMENTS(m85xxPci1Resources)

#endif /* DRV_PCIBUS_M85XX */

extern UINT32 divisorGet(UINT32 xtal,UINT32 baud);

const struct hcfResource ns165500Resources[] = {
    { "regBase",     HCF_RES_INT,  { (void *)(CCSBAR + 0x4500) } },
    { "irq",         HCF_RES_INT, {(void *)EPIC_DUART_INT_VEC} },
    { "regInterval", HCF_RES_INT, {(void *)1} },
    { "irqLevel",    HCF_RES_INT, {(void *)EPIC_DUART_INT_VEC} },
    { "divsorCalc",	 HCF_RES_ADDR, {(void *)&divisorGet} },
    { "clkFreq",	 HCF_RES_ADDR, {(void *)&sysClkFreqGet}},
};
#define ns165500Num NELEMENTS(ns165500Resources)

const struct hcfResource ns165501Resources[] = {
    { "regBase",     HCF_RES_INT,  { (void *)(CCSBAR + 0x4600) } },
    { "clkFreq",     HCF_RES_ADDR, { (void *)sysClkFreqGet} },
    { "regInterval", HCF_RES_INT,  { (void *)1 } }
};
#define ns165501Num NELEMENTS(ns165501Resources)

const struct intrCtlrInputs ppcIntCtlrInputs[] = {
    { 0, "epic", 0, 0 },
    { 1, "m85xxTimerDev", 0, 0 },
    { 3, "m85xxTimerDev", 0, 2 },
    { 4, "m85xxTimerDev", 0, 1 },
};

const struct hcfResource ppcIntCtlr0Resources[] = {
    { "regBase",        HCF_RES_INT,  { (void *)TRUE } },
    { "input",		    HCF_RES_ADDR, { (void *)&ppcIntCtlrInputs[0] } },
    { "inputTableSize", HCF_RES_INT,  { (void *)NELEMENTS(ppcIntCtlrInputs) } },
};
#define ppcIntCtlr0Num NELEMENTS(ppcIntCtlr0Resources)


struct intrCtlrInputs epicInputs[] = {
    { EPIC_DMA0_INT_VEC,   "dma", 0, 0 },
    { EPIC_DMA1_INT_VEC,   "dma", 1, 0 },
    { EPIC_DMA2_INT_VEC,   "dma", 2, 0 },
    { EPIC_DMA3_INT_VEC,   "dma", 3, 0 },

    /*{ EPIC_VEC_EXT_IRQ0,"m85xxPci", 0, 0},*/
    { EPIC_DUART_INT_VEC,    "ns16550", 0, 0 },
    { EPIC_DUART_INT_VEC,    "ns16550", 1, 0 },
    
    { EPIC_TSEC1TX_INT_VEC,  XTSEC_NAME, 0, 0 },
    { EPIC_TSEC1RX_INT_VEC,  XTSEC_NAME, 0, 1 },
    { EPIC_TSEC1ERR_INT_VEC, XTSEC_NAME, 0, 2 },
    { EPIC_TSEC2TX_INT_VEC,  XTSEC_NAME, 1, 0 },
    { EPIC_TSEC2RX_INT_VEC,  XTSEC_NAME, 1, 1 },
    { EPIC_TSEC2ERR_INT_VEC, XTSEC_NAME, 1, 2 },
};

const struct hcfResource epic0Resources[] = {
    { "regBase",        HCF_RES_INT,  { (void *)CCSBAR } },
    { "input",          HCF_RES_ADDR, { (void *)&epicInputs[0] } },
    { "inputTableSize", HCF_RES_INT,  { (void *)NELEMENTS(epicInputs) } },
    { "numCpus",        HCF_RES_INT,  { (void *)1 } },
#ifdef DRV_PCIBUS_M85XX
    { "exPolar",        HCF_RES_INT,  { (void *)EPIC_INT_ACT_LOW } },
#endif
};
#define epic0Num NELEMENTS(epic0Resources)

struct hcfResource m85xxTimerResources[] =  {
    { "regBase",       HCF_RES_INT,  { (void *)0 } },
    { "decMinClkRate", HCF_RES_INT,  { (void *)SYS_CLK_RATE_MIN } },
    { "decMaxClkRate", HCF_RES_INT,  { (void *)SYS_CLK_RATE_MAX } },
    { "fitMinClkRate", HCF_RES_INT,  { (void *)AUX_CLK_RATE_MIN } },
    { "fitMaxClkRate", HCF_RES_INT,  { (void *)AUX_CLK_RATE_MAX } },
    { "sysClockFreq",  HCF_RES_ADDR, { (void *)&sysClkFreqGet } }
};
#define m85xxTimerNum   NELEMENTS(m85xxTimerResources)


const struct hcfResource m8641DmaResources[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)(CCSBAR + 0x21100)}},
    { "irq",         HCF_RES_INT, {(void *)EPIC_DMA0_INT_VEC} }
    };
#define m8641DmaNum NELEMENTS(m8641DmaResources)

const struct hcfResource m8641DmaResources1[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)(CCSBAR + 0x21180)}},
    { "irq",         HCF_RES_INT, {(void *)EPIC_DMA1_INT_VEC} }
    };
#define m8641DmaNum1 NELEMENTS(m8641DmaResources1)

const struct hcfResource m8641DmaResources2[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)(CCSBAR + 0x21200)}},
    { "irq",         HCF_RES_INT, {(void *)EPIC_DMA2_INT_VEC} }
    };
#define m8641DmaNum2 NELEMENTS(m8641DmaResources2)

const struct hcfResource m8641DmaResources3[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)(CCSBAR + 0x21280)}},
    { "irq",         HCF_RES_INT, {(void *)EPIC_DMA3_INT_VEC} }
    };
#define m8641DmaNum3 NELEMENTS(m8641DmaResources3)


const struct hcfDevice hcfDeviceList[] = {
    { "ppcIntCtlr", 0, VXB_BUSID_PLB, 0, ppcIntCtlr0Num, ppcIntCtlr0Resources },
    { "epic", 0, VXB_BUSID_PLB, 0, epic0Num, epic0Resources },

    { "m85xxTimerDev", 0, VXB_BUSID_PLB, 0, m85xxTimerNum, m85xxTimerResources},

    { "ns16550", 0, VXB_BUSID_PLB, 0, ns165500Num, ns165500Resources },
    { "ns16550", 1, VXB_BUSID_PLB, 0, ns165501Num, ns165501Resources },

    { XTSEC_NAME, 0, VXB_BUSID_PLB, 0, tsecVxbEnd0Num, tsecVxbEnd0Resources },
    { XTSEC_NAME, 1, VXB_BUSID_PLB, 0, tsecVxbEnd1Num, tsecVxbEnd1Resources },
#ifdef INCLUDE_OPTIONAL_TSECS
    { XTSEC_NAME, 2, VXB_BUSID_PLB, 0, tsecVxbEnd2Num, tsecVxbEnd2Resources },
    { XTSEC_NAME, 3, VXB_BUSID_PLB, 0, tsecVxbEnd3Num, tsecVxbEnd3Resources },
#endif

#ifdef INCLUDE_RAPIDIO_BUS
    { "m85xxRio", 0, VXB_BUSID_PLB, 0, m85xxRio0Num, m85xxRio0Resources },
    { "m85xxCPU", 0, VXB_BUSID_RAPIDIO, 0, m85xxCPU0Num, m85xxCPU0Resources },
    { "m85xxCPU", 1, VXB_BUSID_RAPIDIO, 0, m85xxCPU1Num, m85xxCPU1Resources },
    { "smEnd", 0, VXB_BUSID_PLB,0, smEnd0Num, smEnd0Resources },
#endif /* INCLUDE_RAPIDIO_BUS */

#ifdef DRV_RESOURCE_M85XXCCSR
    { "m85xxCCSR", 0, VXB_BUSID_PLB, 0, m85xxLawBarNum, m85xxLawBarResources },
#endif /* DRV_RESOURCE_M85XXCCSR */
#ifdef DRV_PCIBUS_M85XX
    /*{ "m85xxPci", 0, VXB_BUSID_PLB, 0, m85xxPci0Num, m85xxPci0Resources },*/
    { "m85xxPci", 0, VXB_BUSID_PLB, 0, m85xxPci1Num, m85xxPci1Resources },
#endif /* DRV_PCIBUS_M85XX */
    { "dma", 0, VXB_BUSID_PLB, 0, m8641DmaNum, m8641DmaResources},
    { "dma", 1, VXB_BUSID_PLB, 0, m8641DmaNum1, m8641DmaResources1},
    { "dma", 2, VXB_BUSID_PLB, 0, m8641DmaNum2, m8641DmaResources2},
    { "dma", 3, VXB_BUSID_PLB, 0, m8641DmaNum3, m8641DmaResources3},

};
const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] =
{/**/
    { XTSEC_NAME, 0, "l2disable", VXB_PARAM_INT32,{(void *) 1} },
    { XTSEC_NAME, 1, "l2disable", VXB_PARAM_INT32,{(void *) 1} },
    { XTSEC_NAME, 2, "l2disable", VXB_PARAM_INT32,{(void *) 1} },
    { XTSEC_NAME, 3, "l2disable", VXB_PARAM_INT32,{(void *) 1} },
    { "mv88E1x11Phy", 0, "mode", VXB_PARAM_INT32, {(void *)MV88E_MODE_RGMII} },
    { "mv88E1x11Phy", 1, "mode", VXB_PARAM_INT32, {(void *)MV88E_MODE_RGMII_TO_FIBER} },
    { "mv88E1x11Phy", 2, "mode", VXB_PARAM_INT32, {(void *)MV88E_MODE_RGMII} },
    { "mv88E1x11Phy", 3, "mode", VXB_PARAM_INT32, {(void *)MV88E_MODE_RGMII_TO_FIBER} },
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, {(void *)0} }
};



