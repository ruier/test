/* hwconf.c - Hardware configuration support module for HPC2-8641 */

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
//#include "vxbGpioIntCtrl.h"

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

#ifdef INCLUDE_TSEC_MDIO
LOCAL const struct hcfResource mdio0Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x24000) } },
    { "sharedMem",      HCF_RES_INT,    { (void *)TM_ANCHOR_ADRS } }
};
#define mdio0Num NELEMENTS(mdio0Resources)
#endif /* INCLUDE_TSEC_MDIO */

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
    { "miiIfName",      HCF_RES_STRING, { (void *)"prophetDummyMdio" } },/*prophetM8640dMdio*/
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
    { "phyAddr",        HCF_RES_INT,    { (void *)1 } },
#ifdef _WRS_VX_AMP
    { "miiIfName",      HCF_RES_STRING, { (void *)"tsecMdio" } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
    { "coreNum",        HCF_RES_INT,    { (void *)0 } }
#else /* _WRS_VX_AMP */
    { "miiIfName",      HCF_RES_STRING, { (void *)"prophetDummyMdio" } },
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
    { "phyAddr",        HCF_RES_INT,    { (void *)2} },
#ifdef _WRS_VX_AMP
    { "miiIfName",      HCF_RES_STRING, { (void *)"tsecMdio" } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
    { "coreNum",        HCF_RES_INT,    { (void *)1 } },
    { "physMask",       HCF_RES_INT,    { (void *)0x10000000 } }
#else /* _WRS_VX_AMP */
    { "miiIfName",      HCF_RES_STRING, { (void *)"prophetDummyMdio"/*"prophetM8640dMdio"*/ } },/**/
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
    { "phyAddr",        HCF_RES_INT,    { (void *)31 } },
#ifdef _WRS_VX_AMP
    { "miiIfName",      HCF_RES_STRING, { (void *)"tsecMdio" } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
    { "coreNum",        HCF_RES_INT,    { (void *)1 } },
    { "physMask",       HCF_RES_INT,    { (void *)0x10000000 } }
#else /* _WRS_VX_AMP */
    { "miiIfName",      HCF_RES_STRING, { (void *)"prophetDummyMdio" } },
    { "miiIfUnit",      HCF_RES_INT, { (void *)0 } }
#endif
};
#define tsecVxbEnd3Num NELEMENTS(tsecVxbEnd3Resources)

#ifdef INCLUDE_PROPHET_DUMMY_MDIO
const struct hcfResource prophetDummyMdio0Resources[] = {
    { "regBase",    HCF_RES_INT, { (void *)0 } },
    { "fullDuplex", HCF_RES_INT, { (void *)1 } },
    { "speed",      HCF_RES_INT, { (void *)1000 } }
};

#define prophetDummyMdio0Num NELEMENTS(prophetDummyMdio0Resources)
#endif


#ifdef INCLUDE_PROPHET_M8640D_MDIO
const struct hcfResource prophetM8640dMdio0Resources[] = {
    { "regBase",    HCF_RES_INT, { (void *)(0x24000+CCSBAR) } },
};

#define prophetM8640dMdio0Num NELEMENTS(prophetM8640dMdio0Resources)
#endif

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
    { "singleLawBarSize",   HCF_RES_INT,  { (void *)(0x20000000)} },	/* modify by ff 256M->512M  */
    { "pciConfigMechanism", HCF_RES_INT,  { (void *)(0)} },
//#ifndef COMPILE_BOOTROM
  //  { "autoConfig",         HCF_RES_INT,  { (void *)(FALSE)} },//TRUE
//#else
    { "autoConfig",         HCF_RES_INT,  { (void *)(TRUE)} },
//#endif
    { "pciExpressHost",     HCF_RES_INT,  { (void *)(TRUE)} },
#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB 
    { "msiEnable",          HCF_RES_INT,  { (void *)(TRUE)} },
    { "dynamicInterrupts",  HCF_RES_INT,  { (void *)(TRUE)} },
    { "msiDataWorkaround",      HCF_RES_INT,  { (void *)(TRUE)} },
    { "msiDataWorkaroundAddr",  HCF_RES_INT,  { (void *)(0xa0000000)} }
#else
    ,
    { "msiEnable",          HCF_RES_INT,  { (void *)(FALSE)} },
    { "dynamicInterrupts",  HCF_RES_INT,  { (void *)(FALSE)} },
    { "msiDataWorkaround",      HCF_RES_INT,  { (void *)(FALSE)} },
    { "msiDataWorkaroundAddr",  HCF_RES_INT,  { (void *)(0x60000000)} }
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#define m85xxPci0Num NELEMENTS(m85xxPci0Resources)


#ifdef INCLUDE_PCI_BUS2
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
#endif /*INCLUDE_PCI_BUS2*/


#endif



#ifdef DRV_M8641_SYN_UART
const struct hcfResource m8641SynUartTx0Resources[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)LOCAL_BUS_ADDR_CS3}},
 //   { "irq",         HCF_RES_INT, {(void *)EPIC_VEC_EXT_IRQ2} },
    { "dirFlag",     HCF_RES_INT, {(void *)0}} /*1:RCV 0：SEND*/
    };

#define m8641SynUartTx0Num NELEMENTS(m8641SynUartTx0Resources)
const struct hcfResource m8641SynUartTx1Resources[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)LOCAL_BUS_ADDR_CS3}},
 //   { "irq",         HCF_RES_INT, {(void *)EPIC_VEC_EXT_IRQ2} },
    { "dirFlag",     HCF_RES_INT, {(void *)0}} /*1:RCV 0：SEND*/
    };

#define m8641SynUartTx1Num NELEMENTS(m8641SynUartTx1Resources)


const struct hcfResource m8641SynUartRx2Resources[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)0xe1000000}},
 //   { "irq",         HCF_RES_INT, {(void *)EPIC_VEC_EXT_IRQ2} },
    { "dirFlag",     HCF_RES_INT, {(void *)1}} /*1:RCV 0：SEND*/
    };

#define m8641SynUartRx2Num NELEMENTS(m8641SynUartRx2Resources)

const struct hcfResource m8641SynUartRx3Resources[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)LOCAL_BUS_ADDR_CS3}},
 //   { "irq",         HCF_RES_INT, {(void *)EPIC_VEC_EXT_IRQ2} },
    { "dirFlag",     HCF_RES_INT, {(void *)1}} /*1:RCV 0：SEND*/
    };

#define m8641SynUartRx3Num NELEMENTS(m8641SynUartRx3Resources)

const struct hcfResource m8641SynUartTx4Resources[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)LOCAL_BUS_ADDR_CS3}},
 //   { "irq",         HCF_RES_INT, {(void *)EPIC_VEC_EXT_IRQ2} },
    { "dirFlag",     HCF_RES_INT, {(void *)0}} /*1:RCV 0：SEND*/
    };

#define m8641SynUartTx4Num NELEMENTS(m8641SynUartTx4Resources)
#endif

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
{ "rioBusSize", HCF_RES_INT, {(void *)RAPIDIO_BUS_SIZE }},
/*hdz add */
{ "tgtIf", HCF_RES_INT, {(void *)LAWAR_TGTIF_RAPIDIO }},
{ "localMemAdrs",HCF_RES_INT, {(void *)LOCAL_MEM_LOCAL_ADRS}}  
/*end*/
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
        { "divisorCalc",	 HCF_RES_ADDR, {(void *)&divisorGet} },
        { "clkFreq",	 HCF_RES_ADDR, {(void *)&sysClkFreqGet} }

    };

#define ns1655x1Num NELEMENTS(ns1655x1Resources)

const struct hcfResource ns1655x2Resources[] =
    {
        { "regBase",     HCF_RES_INT, {(void *)COM2_ADR} },
        { "irq",         HCF_RES_INT, {(void *)EPIC_DUART2_INT_VEC} },
        { "regInterval", HCF_RES_INT, {(void *)DUART_REG_ADDR_INTERVAL} },
        { "irqLevel",    HCF_RES_INT, {(void *)EPIC_DUART2_INT_VEC} },
        { "divisorCalc",	 HCF_RES_ADDR, {(void *)&divisorGet} },
        { "clkFreq",	 HCF_RES_ADDR, {(void *)&sysClkFreqGet} }

    };
#define ns1655x2Num NELEMENTS(ns1655x2Resources)

#ifdef VXB_M8641_RS485
extern UINT32 bspRs485DivisorGet(UINT32 xtal,UINT32 baud);
extern UINT32 bspRs485ClkFreqGet();
const struct hcfResource ns1655x3Resources[] =
    {
        { "regBase",     HCF_RES_INT, {(void *)RS485_RXTX_ADDR} },
        { "irq",         HCF_RES_INT, {(void *)EPIC_RS485_RXTX_INT_VEC} },
        { "regInterval", HCF_RES_INT, {(void *)4} },
        { "irqLevel",    HCF_RES_INT, {(void *)EPIC_RS485_RXTX_INT_VEC} },
        { "divisorCalc",	 HCF_RES_ADDR, {(void *)&bspRs485DivisorGet} },
        { "clkFreq",	 HCF_RES_ADDR, {(void *)&bspRs485ClkFreqGet} }

    };
#define ns1655x3Num NELEMENTS(ns1655x3Resources)
#endif



#ifdef DRV_INTCTLR_RKTIO
//const struct hcfResource rktDrvResources[] =
//    {
//    { "regBase",     HCF_RES_INT, {(void *)RKT_BASE_ADDR}}
//    };
//#define rktDrvNum NELEMENTS(rktDrvResources)

struct intrCtlrInputs rktIntCtlrInputs[] = {
		{ RKTIO_INT_VEC0		,"m85xxPci",0,0},   //SFP0 full    instatus = 0x1
		{ RKTIO_INT_VEC1		,"m85xxPci",0,1},   //SFP0 eof	   instatus = 0x2
		{ RKTIO_INT_VEC2		,"m85xxPci",0,2},	//SFP1 full		instatus = 0x10
		{ RKTIO_INT_VEC3		,"m85xxPci",0,3},	//SFP1 eof		instatus = 0x20
		{ RKTIO_INT_VEC4		,"m85xxPci",0,4},	//rkt0 full
		{ RKTIO_INT_VEC5		,"m85xxPci",0,5},	//rkt0 eof
		{ RKTIO_INT_VEC6		,"m85xxPci",0,6},	//rkt1 full
		{ RKTIO_INT_VEC7		,"m85xxPci",0,7},	//rkt1 eof
		{ RKTIO_INT_VEC8		,"m85xxPci",0,8},	//rkt2 full
		{ RKTIO_INT_VEC9		,"m85xxPci",0,9},	//rkt2 eof
		{ RKTIO_INT_VEC10		,"m85xxPci",0,10},	//rkt3 full
		{ RKTIO_INT_VEC11		,"m85xxPci",0,11},	//rkt3 eof
		{ RKTIO_INT_VEC12		,"m85xxPci",0,12},	//dma
};

const struct hcfResource rktIntCtlrResources[] = {
    { "regBase",         HCF_RES_INT,  { (void *)(RKT_BASE_ADDR) } },
    { "input",              HCF_RES_ADDR, { (void *)&rktIntCtlrInputs[0] } },
    { "inputTableSize",     HCF_RES_INT,  { (void *)NELEMENTS(rktIntCtlrInputs) } },
};

#define rktIntCtlrNum NELEMENTS(rktIntCtlrResources)
#endif

/************************ff add*****************************/

#ifdef DRV_M8641_GPIO
const struct hcfResource gpioDrvResources[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)(LOCAL_BUS_ADDR_CS3+0x800)}}
    };
#define gpioDrvNum NELEMENTS(gpioDrvResources)
#endif

#ifdef DRV_INTCTRL_GPIO
struct intrCtlrInputs gpioIntCtlrInputs[] = {
		{ GPIO_INT_VEC0		,"gpioDrv",0,0},
		{ GPIO_INT_VEC1		,"gpioDrv",0,1},
		{ GPIO_INT_VEC2		,"gpioDrv",0,2},
		{ GPIO_INT_VEC3		,"gpioDrv",0,3},
		{ GPIO_INT_VEC4		,"gpioDrv",0,4},
		{ GPIO_INT_VEC5		,"gpioDrv",0,5},
		{ GPIO_INT_VEC6		,"gpioDrv",0,6},
		{ GPIO_INT_VEC7		,"gpioDrv",0,7},
		{ GPIO_INT_VEC8		,"gpioDrv",0,8},
		{ GPIO_INT_VEC9		,"gpioDrv",0,9},
		{ GPIO_INT_VEC10	,"gpioDrv",0,10},
		{ GPIO_INT_VEC11	,"gpioDrv",0,11},
		{ GPIO_INT_VEC12	,"gpioDrv",0,12},
		{ GPIO_INT_VEC13	,"gpioDrv",0,13},
		{ GPIO_INT_VEC14	,"gpioDrv",0,14},  
		{ GPIO_INT_VEC14	,"gpioDrv",0,15},
};

const struct hcfResource gpioIntCtlrResources[] = {
    { "regBase",         HCF_RES_INT,  { (void *)(LOCAL_BUS_ADDR_CS3+0x800) } },
    { "input",              HCF_RES_ADDR, { (void *)&gpioIntCtlrInputs[0] } },
    { "maxGpioInt",     HCF_RES_INT,  {(void *)16} },
    { "inputTableSize",     HCF_RES_INT,  { (void *)NELEMENTS(gpioIntCtlrInputs) } },    
};

#define gpioIntCtlrNum NELEMENTS(gpioIntCtlrResources)
#endif
/************************ff add end****************************/


#ifdef DRV_INTCTRL_I2C

struct intrCtlrInputs i2cIntCtlrInputs[] = {
		{ GPIO_INT_I2C0_VEC0		,"gpioDrv",0,20},
		{ GPIO_INT_I2C0_VEC1		,"gpioDrv",0,21},
		{ GPIO_INT_I2C0_VEC2		,"gpioDrv",0,22},
		{ GPIO_INT_I2C1_VEC0		,"gpioDrv",0,23},
		{ GPIO_INT_I2C1_VEC1		,"gpioDrv",0,24},
		{ GPIO_INT_I2C1_VEC2		,"gpioDrv",0,22},
};

const struct hcfResource i2cIntCtlrResources[] = {
    { "regBase",         HCF_RES_INT,  { (void *)(LOCAL_BUS_ADDR_CS3+0x160) } },
    { "input",           HCF_RES_ADDR, { (void *)&i2cIntCtlrInputs[0] } },
    { "maxGpioInt",      HCF_RES_INT,  {(void *)6} },
    { "inputTableSize",     HCF_RES_INT,  { (void *)NELEMENTS(i2cIntCtlrInputs) } },
};

#define i2cIntCtlrNum NELEMENTS(i2cIntCtlrResources)
#endif


#ifdef DRV_INTCTRL_FPGA_TIMER
struct intrCtlrInputs fpgaTimerIntCtlrInputs[] = {
		{ FPGA_TIMER_INT_VEC0		,"gpioDrv",0,17},
		//{ GPIO_INT_SPECIAL_VEC0		,"gpioDrv",0,18},
		//{ GPIO_INT_SPECIAL_VEC0		,"gpioDrv",0,19},

};

const struct hcfResource  fpgaTimerIntCtlrResources[] = {
    { "regBase",         HCF_RES_INT,  { (void *)(LOCAL_BUS_ADDR_CS3+0x130) } },
    { "input",              HCF_RES_ADDR, { (void *)&fpgaTimerIntCtlrInputs[0] } },
    { "maxGpioInt",     HCF_RES_INT,  {(void *)1} },
    { "inputTableSize",     HCF_RES_INT,  { (void *)NELEMENTS(fpgaTimerIntCtlrInputs) } },
};

#define fpgaTimerIntCtlrNum NELEMENTS(fpgaTimerIntCtlrResources)
#endif

struct intrCtlrInputs epicInputs[] = {
#ifdef VXB_M8641_DMA
    { EPIC_DMA0_INT_VEC,   "dma", 0, 0 },
    { EPIC_DMA1_INT_VEC,   "dma", 1, 0 },
    { EPIC_DMA2_INT_VEC,   "dma", 2, 0 },
    { EPIC_DMA3_INT_VEC,   "dma", 3, 0 },
#endif  

#ifdef INCLUDE_RAPIDIO_BUS
 /*hdz add for RapidIo*/
    { EPIC_SR_IN_DB1_INT_VEC,   "m85xxCPU",0,0},
    { EPIC_SR_OUT_DB1_INT_VEC,  "m85xxCPU",0,1},
  
    { EPIC_SR_IN_MU1_INT_VEC ,  "m85xxCPU",0,2},
    { EPIC_SR_OUT_MU1_INT_VEC , "m85xxCPU",0,3},
  
    { EPIC_SR_IN_MU2_INT_VEC ,  "m85xxCPU",0,4},
    { EPIC_SR_OUT_MU2_INT_VEC , "m85xxCPU",0,5},
    
    { EPIC_SR_ERR_INT_VEC , "m85xxCPU",0,6},
    
    /*hdz add end*/
#endif


    	

#ifdef DRV_INTCTRL_GPIO
    { EPIC_VEC_EXT_IRQ6 ,     "gpioIntCtrl", 0,0},
  
#endif


    
#ifdef DRV_M8641_SYN_UART
    { EPIC_VEC_EXT_IRQ8 ,     "synUart", 0,0},
    { EPIC_VEC_EXT_IRQ9 ,     "synUart", 1,0},
    { EPIC_VEC_EXT_IRQ10 ,     "synUart", 2,0},
    { EPIC_VEC_EXT_IRQ4 ,     "synUart", 3,0},
    { EPIC_VEC_EXT_IRQ5 ,     "synUart", 4,0},
#endif
#ifdef  DRV_M86XX_FCAN  
    { EPIC_VEC_EXT_IRQ11,   "ppcFpgaCan", 0, 0 },
//    { EPIC_VEC_EXT_IRQ6,   "ppcFpgaCan", 1, 0 },
#endif
	{ EPIC_DUART2_INT_VEC,   "ns16550", 1, 0 },
#ifdef VXB_M8641_RS485
	{ EPIC_RS485_RXTX_INT_VEC,   "ns16550", 2, 0 },
#endif

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
#ifdef INCLUDE_MOB_8XXX
# if defined(INCLUDE_MOB_PLB_0)&&(MOB_PLB_RX_CONTEXT_0>0)
    { EPIC_VEC_IPI_IRQ0,     "mob8572", 0, 0 }
# else
#  if defined(INCLUDE_MOB_PLB_1)&&(MOB_PLB_RX_CONTEXT_1>0)
    { EPIC_VEC_IPI_IRQ0,     "mob8572", 1, 0 }
#  else
    { EPIC_VEC_IPI_IRQ0,     "mob8572", 0, 0 }
#  endif
# endif
#endif /* INCLUDE_MOB_8XXX */
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
    { "exIrqWkrd",         HCF_RES_INT,  {(void *)TRUE } }
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
#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    {"vector0",      HCF_RES_INT,    {(void *)OPENPIC_TIMERA0_INT_VEC}},
    {"vector1",      HCF_RES_INT,    {(void *)OPENPIC_TIMERA1_INT_VEC}},
    {"vector2",      HCF_RES_INT,    {(void *)OPENPIC_TIMERA2_INT_VEC}},
    {"vector3",      HCF_RES_INT,    {(void *)OPENPIC_TIMERA3_INT_VEC}},
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
    };
#define openPicTimerDevAnum NELEMENTS(openPicTimerDevAresources)

const struct hcfResource openPicTimerDevBresources[] =
    {
    {VXB_REG_BASE,   HCF_RES_INT,    {(void *)(CCSBAR + 0x000420F0)}},
    {"instanceName", HCF_RES_STRING, {(void *)"B" }},
    {VXB_CLK_FREQ,   HCF_RES_ADDR,   {(void *)sysPicClkFreqGet}},
    {"clkDivisor",   HCF_RES_INT,    {(void *)0}},
#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    {"vector0",      HCF_RES_INT,    {(void *)OPENPIC_TIMERB0_INT_VEC}},
    {"vector1",      HCF_RES_INT,    {(void *)OPENPIC_TIMERB1_INT_VEC}},
    {"vector2",      HCF_RES_INT,    {(void *)OPENPIC_TIMERB2_INT_VEC}},
    {"vector3",      HCF_RES_INT,    {(void *)OPENPIC_TIMERB3_INT_VEC}},
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
    };
#define openPicTimerDevBnum NELEMENTS(openPicTimerDevBresources)


#ifdef INCLUDE_MOB_8XXX
void * mob8572RegBaseDummy [10];   /* these are not real drivers, but an entry is
                                 * needed by the vxbus subsystem */

/* We increase the ANCHOR_BASE by 64 bytes to leave room for a legacy SM anchor
 * in case it is included (needed for VxMP, smNet).  In that event, the SM
 * region will use of the area that has been allocated for MOB_1.
 * Each anchor is currently calculated to be (16+4*nodes) bytes and there
 * is one anchor per node on the bus. The anchor is aligned on a 32 byte
 * boundary. */
#define MOB_8572_ANCHOR_BASE_0      (0x10000000 + SM_ANCHOR_OFFSET + 0x40)
/* The anchor for MOB_PLB_1 resides just after the anchor area for MOB 0.
 * This size is 0x40 calculated by using the maximum number of nodes on the
 * bus (2) and calculating its aligned size to be 0x20 per node.  Hence we
 * increment from the value of ANCHOR_BASE_0 */
#define MOB_8572_ANCHOR_BASE_1      (MOB_8572_ANCHOR_BASE_0 + 0x40)

/* This IPI ID may need to be modified when Linux or vxWorks SMP is running on 
 * the same multicore processor. It doesn't matter for dual core as that 
 * will never happen.
 */
#define MOB_VX_IPI_ID  0

#ifdef INCLUDE_MOB_PLB_0
struct hcfResource mob8572Res_0[] = {
    { "regBase",	RES_INT,  {(void *) mob8572RegBaseDummy} },
    { "bus_type",	RES_ADDR, {(void *) "plb_0"} },
    { "intr",		RES_INT,  {(void *) EPIC_VEC_IPI_IRQ0} },
    { "intrLevel",	RES_INT,  {(void *) EPIC_VEC_IPI_IRQ0} },
    { "anchor_base",	RES_ADDR, {(void *) MOB_8572_ANCHOR_BASE_0} },
    { "sm_start",	RES_ADDR, {(void *) MOB_PLB_SM_POOL_BASE_0} },
    { "sz_pool",	RES_INT,  {(void *) MOB_PLB_SM_POOL_SIZE_0} },
    { "bus_num",	RES_INT,  {(void *) MOB_PLB_BUS_NUM_0} },
    { "node_num",	RES_INT,  {(void *) MOB_PLB_NODE_NUM_0} },
    { "max_nodes",	RES_INT,  {(void *) MOB_PLB_MAX_NODES_0} },
    { "num_events",     RES_INT,  {(void *) MOB_PLB_NUM_EVENTS_0} },
    { "rx_context",	RES_INT,  {(void *) MOB_PLB_RX_CONTEXT_0} },
    { "vxIpiId",	RES_INT,  {(void *) MOB_VX_IPI_ID} }
};
#endif /* INCLUDE_MOB_PLB_0 */
#ifdef INCLUDE_MOB_PLB_1
struct hcfResource mob8572Res_1[] = {
    { "regBase",	RES_INT,  {(void *) mob8572RegBaseDummy} },
    { "bus_type",	RES_ADDR, {(void *) "plb_1"} },
    { "intr",		RES_INT,  {(void *) EPIC_VEC_IPI_IRQ0} },
    { "intrLevel",	RES_INT,  {(void *) EPIC_VEC_IPI_IRQ0} },
    { "anchor_base",	RES_ADDR, {(void *) MOB_8572_ANCHOR_BASE_1} },
    { "sm_start",	RES_ADDR, {(void *) MOB_PLB_SM_POOL_BASE_1} },
    { "sz_pool",	RES_INT,  {(void *) MOB_PLB_SM_POOL_SIZE_1} },
    { "bus_num",	RES_INT,  {(void *) MOB_PLB_BUS_NUM_1} },
    { "node_num",	RES_INT,  {(void *) MOB_PLB_NODE_NUM_1} },
    { "max_nodes",	RES_INT,  {(void *) MOB_PLB_MAX_NODES_1} },
    { "num_events",     RES_INT,  {(void *) MOB_PLB_NUM_EVENTS_1} },
    { "rx_context",	RES_INT,  {(void *) MOB_PLB_RX_CONTEXT_1} },
    { "vxIpiId",	RES_INT,  {(void *) MOB_VX_IPI_ID} }
};
#endif /* INCLUDE_MOB_PLB_1 */
#endif /* INCLUDE_MOB_PLB */


#ifdef INCLUDE_MOB_PLB_0
struct hcfResource mobVx8572Res0_0[] = {
    { "regBase",    RES_INT,  {(void *) mob8572RegBaseDummy} },
    { "node_num",   RES_INT,  {(void *) 0} },
    { "bus_num",    RES_INT,  {(void *) MOB_PLB_BUS_NUM_0} },
    { "anchor",     RES_ADDR, {(void *) MOB_8572_ANCHOR_BASE_0} },
    { "max_nodes",  RES_INT,  {(void *) MOB_PLB_MAX_NODES_0} },
    { "vxIpiId",    RES_INT,  {(void *) MOB_VX_IPI_ID} }
};

struct hcfResource mobVx8572Res1_0[] = {
    { "regBase",    RES_INT,  {(void *) mob8572RegBaseDummy} },
    { "node_num",   RES_INT,  {(void *) 1} },
    { "bus_num",    RES_INT,  {(void *) MOB_PLB_BUS_NUM_0} },
    { "anchor",     RES_ADDR, {(void *) MOB_8572_ANCHOR_BASE_0} },
    { "max_nodes",  RES_INT,  {(void *) MOB_PLB_MAX_NODES_0} },
    { "vxIpiId",    RES_INT,  {(void *) MOB_VX_IPI_ID} }
};
#endif
#ifdef INCLUDE_MOB_PLB_1
struct hcfResource mobVx8572Res0_1[] = {
    { "regBase",    RES_INT,  {(void *) mob8572RegBaseDummy} },
    { "node_num",   RES_INT,  {(void *) 0} },
    { "bus_num",    RES_INT,  {(void *) MOB_PLB_BUS_NUM_1} },
    { "anchor",     RES_ADDR, {(void *) MOB_8572_ANCHOR_BASE_1} },
    { "max_nodes",  RES_INT,  {(void *) MOB_PLB_MAX_NODES_1} },
    { "vxIpiId",    RES_INT,  {(void *) MOB_VX_IPI_ID} }
};

struct hcfResource mobVx8572Res1_1[] = {
    { "regBase",    RES_INT,  {(void *) mob8572RegBaseDummy} },
    { "node_num",   RES_INT,  {(void *) 1} },
    { "bus_num",    RES_INT,  {(void *) MOB_PLB_BUS_NUM_1} },
    { "anchor",     RES_ADDR, {(void *) MOB_8572_ANCHOR_BASE_1} },
    { "max_nodes",  RES_INT,  {(void *) MOB_PLB_MAX_NODES_1} },
    { "vxIpiId",    RES_INT,  {(void *) MOB_VX_IPI_ID} }
};
#endif

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

#ifdef VXB_M8641_DMA
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

#endif


#ifdef DRV_M86XX_FCAN
const struct hcfResource m86xxfCan0Resources[] = {
	 { "regBase",        HCF_RES_INT,   (void *)0xe1000000 },
	 { "memBaseAddr",    HCF_RES_INT, 	(void *)0xe1000100 },
	 { "canDeviceId",    HCF_RES_INT,   (void *)0x11  },	  
	 { "canClkDiv",  	 HCF_RES_INT, 	(void *)4 },
};
#define m86xxfCan0Num NELEMENTS(m86xxfCan0Resources)

//const struct hcfResource m86xxfCan1Resources[] = {
//	 { "regBase",        HCF_RES_INT,   (void *)0xe1000100 },
//	 { "memBaseAddr",    HCF_RES_INT, 	(void *)0xe1000100 },
//	 { "canDeviceId",    HCF_RES_INT, 	(void *)0x22 },	  
//	 { "canClkDiv",  	 HCF_RES_INT, 	(void *)4 },
//};
//#define m86xxfCan1Num NELEMENTS(m86xxfCan1Resources)
#endif

#ifdef DRV_M8641_IIC
const struct hcfResource m8641I2cResources[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)(CCSBAR + 0x3000)}},
    /*{ "irq",         HCF_RES_INT, {(void *)EPIC_I2C_INT_VEC} },*/
    { "clkFreq",         HCF_RES_INT, {(void *)(0x3f)} },/*FDR=(500M/2)/1024 = 250K左右。其中MPX CLK =500M  0x07对应于1024分频*/
    { "i2cSlaveAddr",         HCF_RES_INT, {(void *)(0x01)}} ,
    { "count", HCF_RES_INT, {(void *)(0x01)}} 
    };

#define m8641I2cNum NELEMENTS(m8641I2cResources)


const struct hcfResource m8641I2c1Resources[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)(CCSBAR + 0x3100)}},
  /*  { "irq",         HCF_RES_INT, {(void *)EPIC_I2C_INT_VEC} },*/
    { "clkFreq",         HCF_RES_INT, {(void *)(0x07)} },/*FDR=(500M/2)/1024 = 250K左右。其中MPX CLK =500M  0x07对应于1024分频*/
    { "i2cSlaveAddr",         HCF_RES_INT, {(void *)(0x02)} } ,
    { "count",         HCF_RES_INT, {(void *)(0x01)}} 
    };


#define m8641I2c1Num NELEMENTS(m8641I2c1Resources)

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
#ifdef DRV_INTCTLR_RKTIO
    { "rktIntCtrl", 0, VXB_BUSID_PLB, 0, rktIntCtlrNum, rktIntCtlrResources },
#endif
#ifdef DRV_INTCTRL_GPIO
    { "gpioIntCtrl", 0, VXB_BUSID_PLB, 0, gpioIntCtlrNum, gpioIntCtlrResources },
#endif
#ifdef DRV_M8641_GPIO
    { "gpioDrv",  	0, 	VXB_BUSID_PLB, 0, gpioDrvNum,  gpioDrvResources },   
#endif
    
    

    

#ifdef INCLUDE_TSEC_MDIO
    { "tsecMdio", 0, VXB_BUSID_PLB, 0, mdio0Num, mdio0Resources },
#endif

  //  { XTSEC_NAME, 0, VXB_BUSID_PLB, 0, tsecVxbEnd0Num, tsecVxbEnd0Resources },
   // { XTSEC_NAME, 1, VXB_BUSID_PLB, 0, tsecVxbEnd1Num, tsecVxbEnd1Resources },
    { XTSEC_NAME, 2, VXB_BUSID_PLB, 0, tsecVxbEnd2Num, tsecVxbEnd2Resources },
    { XTSEC_NAME, 3, VXB_BUSID_PLB, 0, tsecVxbEnd3Num, tsecVxbEnd3Resources },
#ifdef INCLUDE_PROPHET_DUMMY_MDIO    
    {"prophetDummyMdio",0,VXB_BUSID_PLB,0,prophetDummyMdio0Num,prophetDummyMdio0Resources},
#endif
#ifdef INCLUDE_PROPHET_M8640D_MDIO
    {"prophetM8640dMdio",0,VXB_BUSID_PLB,0,prophetM8640dMdio0Num,prophetM8640dMdio0Resources},
#endif

    { "ns16550", 0, VXB_BUSID_PLB, 0, ns1655x1Num, ns1655x1Resources },
    { "ns16550", 1, VXB_BUSID_PLB, 0, ns1655x2Num, ns1655x2Resources },

#ifdef VXB_M8641_RS485
    { "ns16550", 2, VXB_BUSID_PLB, 0, ns1655x3Num, ns1655x3Resources },
#endif

    

/* This is a static way to ensure only one core accesses PCI/LAWBARs */
#ifdef INCLUDE_PCI_BUS
    { "m85xxCCSR", 0, VXB_BUSID_PLB,0, m85xxCCSR0Num, m85xxCCSR0Resources },
    { "m85xxPci",  0, VXB_BUSID_PLB,0, m85xxPci0Num,  m85xxPci0Resources },
 /*   { "m85xxPci",  1, VXB_BUSID_PLB,0, m85xxPci1Num,  m85xxPci1Resources },	*/	/*delete by ff---2009.10.27*/
#endif /* INCLUDE_PCI_BUS */

    { "ppcDecTimerDev", 0, VXB_BUSID_PLB,0, ppcDecTimerDevNum,   ppcDecTimerDevResources },

    { "openPicTimer", 0,   VXB_BUSID_PLB,0, openPicTimerDevAnum, openPicTimerDevAresources },
    { "openPicTimer", 1,   VXB_BUSID_PLB,0, openPicTimerDevBnum, openPicTimerDevBresources },

#ifdef INCLUDE_RAPIDIO_BUS
    { "m85xxRio", 0, VXB_BUSID_PLB, 0, m85xxRio0Num, m85xxRio0Resources },
    { "m85xxCPU", 0, VXB_BUSID_RAPIDIO, 0, m85xxCPU0Num, m85xxCPU0Resources },
/*    { "m85xxCPU", 1, VXB_BUSID_RAPIDIO, 0, m85xxCPU1Num, m85xxCPU1Resources },
    { "smEnd", 0, VXB_BUSID_PLB,0, smEnd0Num, smEnd0Resources},*/
#endif

#ifdef INCLUDE_MOB_PLB_0
    { "mob8572", 0, VXB_BUSID_PLB, 0,
        NELEMENTS(mob8572Res_0), mob8572Res_0 },
    { "mobVx8572", 0, VXB_BUSID_VIRTUAL, 0,
        NELEMENTS(mobVx8572Res0_0), mobVx8572Res0_0 },
    { "mobVx8572", 1, VXB_BUSID_VIRTUAL, 0,
        NELEMENTS(mobVx8572Res1_0), mobVx8572Res1_0 },
#endif
#ifdef INCLUDE_MOB_PLB_1
    { "mob8572", 1, VXB_BUSID_PLB, 0,
        NELEMENTS(mob8572Res_1), mob8572Res_1 },
    { "mobVx8572", 2, VXB_BUSID_VIRTUAL, 1,
        NELEMENTS(mobVx8572Res0_1), mobVx8572Res0_1 },
    { "mobVx8572", 3, VXB_BUSID_VIRTUAL, 1,
        NELEMENTS(mobVx8572Res1_1), mobVx8572Res1_1 },
#endif

#ifdef INCLUDE_DSHM
    { "dshmBusCtlr8641", 0, VXB_BUSID_PLB, 0,
        NELEMENTS(dshmBusCtlrPlbRes), dshmBusCtlrPlbRes },
    { "dshmPeerVx8641", 0, VXB_BUSID_VIRTUAL, 0,
        NELEMENTS(dshmPeerVxPlb0), dshmPeerVxPlb0 },
    { "dshmPeerVx8641", 1, VXB_BUSID_VIRTUAL, 0,
        NELEMENTS(dshmPeerVxPlb1), dshmPeerVxPlb1 },
#endif
#ifdef VXB_M8641_DMA
    { "dma", 0, VXB_BUSID_PLB, 0, m8641DmaNum, m8641DmaResources },
    { "dma", 1, VXB_BUSID_PLB, 0, m8641DmaNum1, m8641DmaResources1 },
    { "dma", 2, VXB_BUSID_PLB, 0, m8641DmaNum2, m8641DmaResources2 },
    { "dma", 3, VXB_BUSID_PLB, 0, m8641DmaNum3, m8641DmaResources3 },
#endif
#ifdef DRV_M8641_IIC /*hdz 20100302*/
    { "i2c", 0, VXB_BUSID_PLB, 0, m8641I2cNum, m8641I2cResources },
    { "i2c", 1, VXB_BUSID_PLB, 0, m8641I2c1Num, m8641I2c1Resources },
#endif
#ifdef DRV_M8641_SYN_UART
    { "synUart", 0, VXB_BUSID_PLB, 0, m8641SynUartTx0Num, m8641SynUartTx0Resources },
    { "synUart", 1, VXB_BUSID_PLB, 0, m8641SynUartTx1Num, m8641SynUartTx1Resources },
    { "synUart", 2, VXB_BUSID_PLB, 0, m8641SynUartRx2Num, m8641SynUartRx2Resources },
    { "synUart", 3, VXB_BUSID_PLB, 0, m8641SynUartRx3Num, m8641SynUartRx3Resources },
    { "synUart", 4, VXB_BUSID_PLB, 0, m8641SynUartTx4Num, m8641SynUartTx4Resources },
#endif
#ifdef DRV_M86XX_FCAN
    {"ppcFpgaCan",	0,VXB_BUSID_PLB,0,m86xxfCan0Num,m86xxfCan0Resources},
//    {"ppcFpgaCan",	1,VXB_BUSID_PLB,0,m86xxfCan1Num,m86xxfCan1Resources},
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
