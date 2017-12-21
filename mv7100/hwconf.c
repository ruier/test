/* hwconf.c - Hardware configuration support module for mv7100 */

/*
 * Copyright (c) 2008-2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01e,12jan12,agf  remove exIrqWkrd from epic resource, deprecated in driver
                 (WIND00176018)
01d,09feb10,liu  add _WRS_VX_AMP for compile error. (WIND00192173)
01g,21apr10,e_d  Remove MCB, MOB and VxBus references for MIPC
01f,20dec09,e_d  Change the pci law bar size
01e,13feb09,e_d  Add OpenPic timer vector support.
01d,11feb09,e_d  Remove AMP_CORE1 usage - sysDeviceFilter now used.
01c,05feb09,e_d  Change _WRS_VX_SMP to _WRS_CONFIG_SMP
01b,10sep08,y_w  code cleanup
01a,17mar08,mdg  Created from hpcNet8641.
*/

/*
DESCRIPTION
Configuration file for vxBus
*/

#include <vxWorks.h>
#include <vxBusLib.h>
#include <vsbConfig.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>

#ifdef INCLUDE_USB
extern UINT32 usbMemToPci (void * pAddr);
extern void * usbPciToMem (UINT32 addr);
#endif /* INCLUDE_USB */

#include "config.h"
#include "mot85xxPci.h"
#include <hwif/vxbus/vxbIntrCtlr.h>

/* FIXME: move vector definitions elsewhere so this file is not needed here */

#include <hwif/intCtlr/vxbEpicIntCtlr.h>
#include <../src/hwif/intCtlr/vxbIntDynaCtlrLib.h>

#define XTSEC_NAME "mottsec"
#if defined (INCLUDE_ETSEC_VXB_END) || defined (INCLUDE_ETSEC_VXB_END2)
#  undef  XTSEC_NAME
#  define XTSEC_NAME "motetsec"
#endif /* INCLUDE_ETSEC_VXB_END */

IMPORT UINT32 divisorGet(UINT32, UINT32);

#ifdef INCLUDE_PCI_BUS
IMPORT STATUS sysPci1AutoconfigInclude();
IMPORT UCHAR sysPci1AutoconfigIntrAssign(PCI_SYSTEM *, PCI_LOC *, UCHAR);
const struct hcfResource m85xxPci0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,  { (void *)(CCSBAR + CCSR_PCIE0_CFG_ADDR) } },
    { "mem32Addr",          HCF_RES_ADDR, { (void *)PCI0_MSTR_MEM_BUS } },
    { "mem32Size",          HCF_RES_INT,  { (void *)PCI0_MSTR_MEM_SIZE } },
    { "memIo32Addr",        HCF_RES_ADDR, { (void *)PCI0_MSTR_MEMIO_BUS } },
    { "memIo32Size",        HCF_RES_INT,  { (void *)PCI0_MSTR_MEMIO_SIZE } },
    { "io32Addr",           HCF_RES_ADDR, { (void *)PCI0_MSTR_IO_BUS } },
    { "io32Size",           HCF_RES_INT,  { (void *)PCI0_MSTR_IO_SIZE } },
    { "io16Addr",           HCF_RES_ADDR, { (void *)ISA0_MSTR_IO_BUS  } },
    { "io16Size",           HCF_RES_INT,  { (void *)ISA0_MSTR_IO_SIZE } },
    { "fbbEnable",          HCF_RES_INT,  { (void *)TRUE } },
    { "cacheSize",          HCF_RES_INT,  { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,  { (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet",    HCF_RES_INT,  { (void *)FALSE } },
    { "includeFuncSet",     HCF_RES_ADDR, { (void *)sysPci1AutoconfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR, { (void *)sysPci1AutoconfigIntrAssign } },

    /* non vxbPciAutoConfig() values */

    { "pimmrBase",          HCF_RES_ADDR, { (void *)CCSBAR_BUS } },
    { "lclMemAddr",         HCF_RES_ADDR, { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",      HCF_RES_INT,  { (void *)(512 * 1024 * 1024) } }, /* ajusted in sysHwInit */
    { "mstrMemBus",         HCF_RES_ADDR, { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "maxBusSet",          HCF_RES_INT,  { (void *)12 } },

    /* LAWBAR details */

    { "tgtIf",              HCF_RES_INT,  { (void *)(CCSR_LAWAR_TGTIF_VAL_PCIE0)} },

    /* Window Attributes - Defaults to 8540 type if none given */

    { "owAttrMem",          HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | PCI_OUT_ATTR_RTT_MEM |  PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrMemIo",        HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | PCI_OUT_ATTR_RTT_MEM |  PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrIo",           HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | PCI_OUT_ATTR_RTT_IO |  PCI_OUT_ATTR_WTT_IO) } },
    { "iwAttr",             HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | PCI_IN_ATTR_RTT_LM_READ_SNOOP | PCI_IN_ATTR_RTT_LM_WRITE_SNOOP | PCI_IN_ATTR_TGI_LM) } },
    { "singleLawBar",       HCF_RES_INT,  { (void *)(TRUE)} },
    { "singleLawBarSize",   HCF_RES_INT,  { (void *)(0x10000000)} },
    { "pciConfigMechanism", HCF_RES_INT,  { (void *)(0)} },
    { "autoConfig",         HCF_RES_INT,  { (void *)(TRUE)} },
    { "pciExpressHost",     HCF_RES_INT,  { (void *)(TRUE)} },
#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "msiEnable",          HCF_RES_INT,  { (void *)(FALSE)} },
    { "dynamicInterrupts",  HCF_RES_INT,  { (void *)(TRUE)} },
    { "msiDataWorkaround",  HCF_RES_INT,  { (void *)(TRUE)} },
    { "msiDataWorkaroundAddr",  HCF_RES_INT,  { (void *)(0x80000000)} }
#else
    { "msiEnable",          HCF_RES_INT,  { (void *)(FALSE)} },
    { "dynamicInterrupts",  HCF_RES_INT,  { (void *)(FALSE)} },
    { "msiDataWorkaround",  HCF_RES_INT,  { (void *)(FALSE)} },
    { "msiDataWorkaroundAddr",  HCF_RES_INT,  { (void *)(0x80000000)} }
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
#ifdef INCLUDE_USB
    ,
    { "cpuToBus", HCF_RES_ADDR, { (void *) usbMemToPci}},
    { "busToCpu", HCF_RES_ADDR, { (void *) usbPciToMem}},
#endif /* INCLUDE_USB */

};
#define m85xxPci0Num NELEMENTS(m85xxPci0Resources)

IMPORT STATUS sysPci2AutoconfigInclude();
IMPORT UCHAR sysPci2AutoconfigIntrAssign(PCI_SYSTEM *, PCI_LOC *, UCHAR);

const struct hcfResource m85xxPci1Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,  { (void *)(CCSBAR + CCSR_PCIE1_CFG_ADDR) } },
    { "mem32Addr",          HCF_RES_ADDR, { (void *)PCI1_MSTR_MEM_BUS } },
    { "mem32Size",          HCF_RES_INT,  { (void *)PCI1_MSTR_MEM_SIZE } },
    { "memIo32Addr",        HCF_RES_ADDR, { (void *)PCI1_MSTR_MEMIO_BUS } },
    { "memIo32Size",        HCF_RES_INT,  { (void *)PCI1_MSTR_MEMIO_SIZE } },
    { "io32Addr",           HCF_RES_ADDR, { (void *)PCI1_MSTR_IO_BUS } },
    { "io32Size",           HCF_RES_INT,  { (void *)PCI1_MSTR_IO_SIZE } },
    { "io16Addr",           HCF_RES_ADDR, { (void *)ISA1_MSTR_IO_BUS } },
    { "io16Size",           HCF_RES_INT,  { (void *)ISA1_MSTR_IO_SIZE } },
    { "fbbEnable",          HCF_RES_INT,  { (void *)TRUE } },
    { "cacheSize",          HCF_RES_INT,  { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,  { (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet",    HCF_RES_INT,  { (void *)FALSE } },
    { "includeFuncSet",     HCF_RES_ADDR, { (void *)sysPci2AutoconfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR, { (void *)sysPci2AutoconfigIntrAssign } },

    /* non vxbPciAutoConfig() values */

    { "pimmrBase",          HCF_RES_ADDR, { (void *)CCSBAR_BUS } },
    { "lclMemAddr",         HCF_RES_ADDR, { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",      HCF_RES_INT,  { (void *)(512 * 1024 * 1024) } }, /* ajusted in sysHwInit */
    { "mstrMemBus",         HCF_RES_ADDR, { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "maxBusSet",          HCF_RES_INT,  { (void *)12 } },

    /* LAWBAR details */

    { "tgtIf",              HCF_RES_INT,  { (void *)(CCSR_LAWAR_TGTIF_VAL_PCIE1)} },

    /* Window Attributes - Defaults to 8540 type if none given */

    { "owAttrMem",          HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | PCI_OUT_ATTR_RTT_MEM |  PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrMemIo",        HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | PCI_OUT_ATTR_RTT_MEM |  PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrIo",           HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | PCI_OUT_ATTR_RTT_IO |  PCI_OUT_ATTR_WTT_IO) } },
    { "iwAttr",             HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | PCI_IN_ATTR_RTT_LM_READ_SNOOP | PCI_IN_ATTR_RTT_LM_WRITE_SNOOP | PCI_IN_ATTR_TGI_LM) } },
    { "singleLawBar",       HCF_RES_INT,  { (void *)(TRUE)} },
    { "singleLawBarSize",   HCF_RES_INT,  { (void *)(0x10000000)} },
    { "pciConfigMechanism", HCF_RES_INT,  { (void *)(0)} },
    { "autoConfig",         HCF_RES_INT,  { (void *)(TRUE)} },
    { "pciExpressHost",     HCF_RES_INT,  { (void *)(TRUE)} },
#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "msiEnable",          HCF_RES_INT,  { (void *)(TRUE)} },
    { "dynamicInterrupts",  HCF_RES_INT,  { (void *)(TRUE)} },
    { "msiDataWorkaround",  HCF_RES_INT,  { (void *)(TRUE)} },
    { "msiDataWorkaroundAddr",  HCF_RES_INT,  { (void *)(0x80000000)} }
#else
    { "msiEnable",          HCF_RES_INT,  { (void *)(FALSE)} },
    { "dynamicInterrupts",  HCF_RES_INT,  { (void *)(FALSE)} },
    { "msiDataWorkaround",  HCF_RES_INT,  { (void *)(FALSE)} },
    { "msiDataWorkaroundAddr",  HCF_RES_INT,  { (void *)(0x80000000)} }
#endif /*INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#define m85xxPci1Num NELEMENTS(m85xxPci1Resources)
#endif /* INCLUDE_PCI_BUS */

/*
 * On the mv7100 board, PHYs are physically wired according
 * to the following table:
 *
 * Data port pins	Management port pins	MII address
 * --------------	--------------------	-----------
 * PHY0:	TSEC0		TSEC0			0
 * PHY1:	TSEC1		TSEC0			1
 * PHY2:	TSEC2		TSEC0			2
 * PHY3:	TSEC3		TSEC0			3
 *
 * The tricky part is that both PHYs have their management pins
 * connected to TSEC0. The VxBus TSEC driver allows us to specify
 * the name of the device that provides PHY management support.
 * This device is "motetsec0" in all cases. We also specify the
 * "phyAddr" resource for each device, which is set according
 * to the MII address in the table above.
 *
 * Note: when configured for AMP mode, the TSEC interfaces forward
 * the PHY access requests to the "tsecMdio0" device instead. This
 * device is required in order to share access to the MDIO port between
 * the two cores.
 */
#ifdef _WRS_VX_AMP
#   ifdef INCLUDE_TSEC_MDIO
const struct hcfResource mdio0Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)(CCSBAR + 0x24000) } },
    { "sharedMem",  HCF_RES_INT,    { (void *)TM_ANCHOR_ADRS } }
};

#define mdio0Num NELEMENTS(mdio0Resources)
#   endif /* INCLUDE_TSEC_MDIO */
#endif /* _WRS_VX_AMP */

const struct hcfResource tsecVxbEnd0Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)(CCSBAR + 0x24000) } },
    { "intr0",      HCF_RES_INT,    { (void *)EPIC_TSEC1TX_INT_VEC } },
    { "intr0Level", HCF_RES_INT,    { (void *)EPIC_TSEC1TX_INT_VEC } },
    { "intr1",      HCF_RES_INT,    { (void *)EPIC_TSEC1RX_INT_VEC } },
    { "intr1Level", HCF_RES_INT,    { (void *)EPIC_TSEC1RX_INT_VEC } },
    { "intr2",      HCF_RES_INT,    { (void *)EPIC_TSEC1ERR_INT_VEC } },
    { "intr2Level", HCF_RES_INT,    { (void *)EPIC_TSEC1ERR_INT_VEC } },
    { "rmiiMode",   HCF_RES_INT,    { (void *)TRUE} },
    { "phyAddr",    HCF_RES_INT,    { (void *)1 } },
    { "l2disable",  HCF_RES_INT,    { (void *)1 } },
#ifdef INCLUDE_AMP
    { "miiIfName",  HCF_RES_STRING, { (void *)"tsecMdio" } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)0 } },
    { "coreNum",    HCF_RES_INT,    { (void *)TSEC0_CORE } },
    { "physMask",   HCF_RES_INT,    { (void *)(TSEC0_CORE * 0x10000000) } },
#else /* INCLUDE_AMP */
    { "miiIfName",  HCF_RES_STRING, { (void *)XTSEC_NAME } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)0 } }
#endif /* INCLUDE_AMP */

};
#define tsecVxbEnd0Num NELEMENTS(tsecVxbEnd0Resources)

const struct hcfResource tsecVxbEnd1Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)(CCSBAR + 0x25000) } },
    { "intr0",      HCF_RES_INT,    { (void *)EPIC_TSEC2TX_INT_VEC } },
    { "intr0Level", HCF_RES_INT,    { (void *)EPIC_TSEC2TX_INT_VEC } },
    { "intr1",      HCF_RES_INT,    { (void *)EPIC_TSEC2RX_INT_VEC } },
    { "intr1Level", HCF_RES_INT,    { (void *)EPIC_TSEC2RX_INT_VEC } },
    { "intr2",      HCF_RES_INT,    { (void *)EPIC_TSEC2ERR_INT_VEC } },
    { "intr2Level", HCF_RES_INT,    { (void *)EPIC_TSEC2ERR_INT_VEC } },
    { "rmiiMode",   HCF_RES_INT,    { (void *)TRUE} },
    { "phyAddr",    HCF_RES_INT,    { (void *)2 } },
#ifdef INCLUDE_AMP
    { "miiIfName",  HCF_RES_STRING, { (void *)"tsecMdio" } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)0 } },
    { "coreNum",    HCF_RES_INT,    { (void *)TSEC1_CORE } },
    { "physMask",   HCF_RES_INT,    { (void *)(TSEC1_CORE * 0x10000000) } },
#else /* INCLUDE_AMP */
    { "miiIfName",  HCF_RES_STRING, { (void *)XTSEC_NAME } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)0 } }
#endif /* INCLUDE_AMP */
};
#define tsecVxbEnd1Num NELEMENTS(tsecVxbEnd1Resources)

const struct hcfResource tsecVxbEnd2Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)(CCSBAR + 0x26000) } },
    { "intr0",      HCF_RES_INT,    { (void *)EPIC_TSEC3TX_INT_VEC } },
    { "intr0Level", HCF_RES_INT,    { (void *)EPIC_TSEC3TX_INT_VEC } },
    { "intr1",      HCF_RES_INT,    { (void *)EPIC_TSEC3RX_INT_VEC } },
    { "intr1Level", HCF_RES_INT,    { (void *)EPIC_TSEC3RX_INT_VEC } },
    { "intr2",      HCF_RES_INT,    { (void *)EPIC_TSEC3ERR_INT_VEC } },
    { "intr2Level", HCF_RES_INT,    { (void *)EPIC_TSEC3ERR_INT_VEC } },
    { "rmiiMode",   HCF_RES_INT,    { (void *)TRUE} },
    { "phyAddr",    HCF_RES_INT,    { (void *)3 } },
#ifdef INCLUDE_AMP
    { "miiIfName",  HCF_RES_STRING, { (void *)"tsecMdio" } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)0 } },
    { "coreNum",    HCF_RES_INT,    { (void *)TSEC2_CORE } },
    { "physMask",   HCF_RES_INT,    { (void *)(TSEC2_CORE * 0x10000000) } },
#else /* INCLUDE_AMP */
    { "miiIfName",  HCF_RES_STRING, { (void *)XTSEC_NAME } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)0 } }
#endif /* INCLUDE_AMP */
};
#define tsecVxbEnd2Num NELEMENTS(tsecVxbEnd2Resources)

const struct hcfResource tsecVxbEnd3Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)(CCSBAR + 0x27000) } },
    { "intr0",      HCF_RES_INT,    { (void *)EPIC_TSEC4TX_INT_VEC } },
    { "intr0Level", HCF_RES_INT,    { (void *)EPIC_TSEC4TX_INT_VEC } },
    { "intr1",      HCF_RES_INT,    { (void *)EPIC_TSEC4RX_INT_VEC } },
    { "intr1Level", HCF_RES_INT,    { (void *)EPIC_TSEC4RX_INT_VEC } },
    { "intr2",      HCF_RES_INT,    { (void *)EPIC_TSEC4ERR_INT_VEC } },
    { "intr2Level", HCF_RES_INT,    { (void *)EPIC_TSEC4ERR_INT_VEC } },
    { "rmiiMode",   HCF_RES_INT,    { (void *)TRUE} },
    { "phyAddr",    HCF_RES_INT,    { (void *)4 } },
#ifdef INCLUDE_AMP
    { "miiIfName",  HCF_RES_STRING, { (void *)"tsecMdio" } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)0 } },
    { "coreNum",    HCF_RES_INT,    { (void *)TSEC3_CORE } },
    { "physMask",   HCF_RES_INT,    { (void *)(TSEC3_CORE * 0x10000000) } },
#else /* INCLUDE_AMP */
    { "miiIfName",  HCF_RES_STRING, { (void *)XTSEC_NAME } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)0 } }
#endif

};
#define tsecVxbEnd3Num NELEMENTS(tsecVxbEnd3Resources)

const struct hcfResource m85xxCCSR0Resources[] = {
    { VXB_REG_BASE,        HCF_RES_INT, { (void *)CCSBAR  } },
    { "LAWAR_ENABLE",      HCF_RES_INT, {(void *)CCSR_LAWAR_EN_VAL_ENABLED } },
    { "LAWBAR_ADRS_SHIFT", HCF_RES_INT, {(void *)CCSR_LAWBAR_ADRS_SHIFT } },

    /*
     * Note lawbars are actually configured by the Bootrom, so the PCI
     * ones are only placeholders so VxBus will configure the buses
     */

    { "LAWBAR0", HCF_RES_STRING, { (void *)"reserved" } },
    { "LAWBAR1", HCF_RES_STRING, { (void *)"reserved" } },
    { "LAWBAR2", HCF_RES_STRING, { (void *)"reserved" } },
    { "LAWBAR3", HCF_RES_STRING, { (void *)"reserved" } },
    { "LAWBAR4", HCF_RES_STRING, { (void *)"reserved" } },
    { "LAWBAR5", HCF_RES_STRING, { (void *)"reserved" } },
    { "LAWBAR6", HCF_RES_STRING, { (void *)"reserved" } },
    { "LAWBAR7", HCF_RES_STRING, { (void *)"reserved" } },
    { "LAWBAR8", HCF_RES_STRING, { (void *)"m85xxPci" } },
    { "LAWBAR9", HCF_RES_STRING, { (void *)"m85xxPci" } }
};
#define m85xxCCSR0Num NELEMENTS(m85xxCCSR0Resources)

const struct hcfResource ns1655x1Resources[] = {
    { VXB_REG_BASE,  HCF_RES_INT,  {(void *)COM0_BASE_ADRS} },
    { "irq",         HCF_RES_INT,  {(void *)EPIC_DUART_INT_VEC} },
    { "regInterval", HCF_RES_INT,  {(void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT,  {(void *)EPIC_DUART_INT_VEC} },
    { "divsorCalc",  HCF_RES_ADDR, {(void *)&divisorGet} },
    { VXB_CLK_FREQ,  HCF_RES_ADDR, {(void *)&sysClkFreqGet} }
};
#define ns1655x1Num NELEMENTS(ns1655x1Resources)

#ifdef INCLUDE_QUAD_UART

const struct hcfResource ns1655x2Resources[] = {
    { VXB_REG_BASE,  HCF_RES_INT,  {(void *)COM1_BASE_ADRS} },
    { "irq",         HCF_RES_INT,  {(void *)QUART_INT_VEC} },
    { "regInterval", HCF_RES_INT,  {(void *)QUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT,  {(void *)QUART_INT_VEC} },
    { "clkFreq",     HCF_RES_INT,  {(void *)QUART_REF_FREQ} },
};
#define ns1655x2Num NELEMENTS(ns1655x2Resources)

const struct hcfResource ns1655x3Resources[] = {
    { VXB_REG_BASE,  HCF_RES_INT,  {(void *)COM2_BASE_ADRS} },
    { "irq",         HCF_RES_INT,  {(void *)QUART_INT_VEC} },
    { "regInterval", HCF_RES_INT,  {(void *)QUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT,  {(void *)QUART_INT_VEC} },
    { "clkFreq",     HCF_RES_INT,  {(void *)QUART_REF_FREQ} },
};
#define ns1655x3Num NELEMENTS(ns1655x3Resources)

const struct hcfResource ns1655x4Resources[] = {
    { VXB_REG_BASE,  HCF_RES_INT,  {(void *)COM3_BASE_ADRS} },
    { "irq",         HCF_RES_INT,  {(void *)QUART_INT_VEC} },
    { "regInterval", HCF_RES_INT,  {(void *)QUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT,  {(void *)QUART_INT_VEC} },
    { "clkFreq",     HCF_RES_INT,  {(void *)QUART_REF_FREQ} },
};
#define ns1655x4Num NELEMENTS(ns1655x4Resources)

const struct hcfResource ns1655x5Resources[] = {
    { VXB_REG_BASE,  HCF_RES_INT,  {(void *)COM4_BASE_ADRS} },
    { "irq",         HCF_RES_INT,  {(void *)QUART_INT_VEC} },
    { "regInterval", HCF_RES_INT,  {(void *)QUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT,  {(void *)QUART_INT_VEC} },
    { "clkFreq",     HCF_RES_INT,  {(void *)QUART_REF_FREQ} },
};
#define ns1655x5Num NELEMENTS(ns1655x5Resources)
#endif /* INCLUDE_QUAD_UART */

struct intrCtlrInputs epicInputs[] = {
    { EPIC_DUART_INT_VEC,         "ns16550",  0, 0 },
    { EPIC_TSEC1TX_INT_VEC,       XTSEC_NAME, 0, 0 },
    { EPIC_TSEC1RX_INT_VEC,       XTSEC_NAME, 0, 1 },
    { EPIC_TSEC3TX_INT_VEC,       XTSEC_NAME, 2, 0 },
    { EPIC_TSEC3RX_INT_VEC,       XTSEC_NAME, 2, 1 },
    { EPIC_TSEC3ERR_INT_VEC,      XTSEC_NAME, 2, 2 },
    { EPIC_TSEC1ERR_INT_VEC,      XTSEC_NAME, 0, 2 },
    { EPIC_TSEC2TX_INT_VEC,       XTSEC_NAME, 1, 0 },
    { EPIC_TSEC2RX_INT_VEC,       XTSEC_NAME, 1, 1 },
    { EPIC_TSEC4TX_INT_VEC,       XTSEC_NAME, 3, 0 },
    { EPIC_TSEC4RX_INT_VEC,       XTSEC_NAME, 3, 1 },
    { EPIC_TSEC4ERR_INT_VEC,      XTSEC_NAME, 3, 2 },
    { EPIC_TSEC2ERR_INT_VEC,      XTSEC_NAME, 1, 2 },
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
    { EPIC_VEC_IPI_IRQ0,     "ipi", 0, 0 },
#else
#ifdef INCLUDE_DSHM_BUS_PLB
    { EPIC_VEC_IPI_IRQ0,     "dshmBusCtlr8641", 0, 0 },
#endif
#endif /* _WRS_CONFIG_SMP */
#ifdef INCLUDE_QUAD_UART
    { QUART_INT_VEC,         "ns16550",  1, 0},
    { QUART_INT_VEC,         "ns16550",  2, 0},
    { QUART_INT_VEC,         "ns16550",  3, 0},
    { QUART_INT_VEC,         "ns16550",  4, 0},
#endif /* INCLUDE_QUAD_UART */
};

struct intrCtlrPriority epicPriority[] = {
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

struct hcfResource epic0Resources[] = {
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
    {VXB_REG_BASE,   HCF_RES_INT,  {(void *)0}},
    {"decClockFreq", HCF_RES_ADDR, {(void *)sysClkFreqGet}},
    {"minClkRate",   HCF_RES_INT,  {(void *)SYS_CLK_RATE_MIN}},
    {"maxClkRate",   HCF_RES_INT,  {(void *)SYS_CLK_RATE_MAX}}
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

/* distributed shared memory virtual devices */

#ifdef INCLUDE_DSHM_BUS_PLB
void * dshmRegBaseDummy  [10];   /* these are not real drivers, but an entry */
                                 /* is needed by the vxbus subsystem */

#   define DSHM_ANCHOR_BASE (0x10000000 + SM_ANCHOR_OFFSET)

/*
 * Virtual bus controller managing the local node. Most values are user-defined
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

/*
 * Peer devices that represent each possible peer in the system. The intr and
 * intrLevel are set to -1 since they are needed but unused, the routine
 * sysEpicIpiGen() taking care of triggering the interrupt on the correct
 * core with the correct vector and level. The ISR is installed in the bus
 * controller, which itself needs the real value for both intr and intrLevel.
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
#endif /* INCLUDE_DSHM_BUS_PLB */

/* end of distributed shared memory */

const struct hcfDevice hcfDeviceList[] = {

    /*
     * Initialize ppcIntCtlr before epic.
     * The vector table for external interrupts are over written by epic
     * for an optimized purpose.
     */

    { "ppcIntCtlr",     0, VXB_BUSID_PLB, 0, ppcIntCtlr0Num,      ppcIntCtlr0Resources },
    { "epic",           0, VXB_BUSID_PLB, 0, epic0Num,            epic0Resources },
    { "m85xxCCSR",      0, VXB_BUSID_PLB, 0, m85xxCCSR0Num,       m85xxCCSR0Resources },
    { "ppcDecTimerDev", 0, VXB_BUSID_PLB, 0, ppcDecTimerDevNum,   ppcDecTimerDevResources },
    { "openPicTimer",   0, VXB_BUSID_PLB, 0, openPicTimerDevAnum, openPicTimerDevAresources },
    { "openPicTimer",   1, VXB_BUSID_PLB, 0, openPicTimerDevBnum, openPicTimerDevBresources },
    { "ns16550",        0, VXB_BUSID_PLB, 0, ns1655x1Num,         ns1655x1Resources },

#ifdef _WRS_VX_AMP
#   ifdef INCLUDE_TSEC_MDIO
    { "tsecMdio",       0, VXB_BUSID_PLB, 0, mdio0Num,            mdio0Resources },
#   endif /* INCLUDE_TSEC_MDIO */
#endif /* _WRS_VX_AMP */

#if defined (INCLUDE_ETSEC_VXB_END) || defined (INCLUDE_TSEC_VXB_END)
    { XTSEC_NAME,       0, VXB_BUSID_PLB, 0, tsecVxbEnd0Num,      tsecVxbEnd0Resources },
    { XTSEC_NAME,       1, VXB_BUSID_PLB, 0, tsecVxbEnd1Num,      tsecVxbEnd1Resources },
    { XTSEC_NAME,       2, VXB_BUSID_PLB, 0, tsecVxbEnd2Num,      tsecVxbEnd2Resources },
    { XTSEC_NAME,       3, VXB_BUSID_PLB, 0, tsecVxbEnd3Num,      tsecVxbEnd3Resources },
#endif /* INCLUDE_ETSEC_VXB_END || INCLUDE_TSEC_VXB_END */

#ifdef INCLUDE_QUAD_UART
    { "ns16550",        1, VXB_BUSID_PLB, 0, ns1655x2Num,         ns1655x2Resources },
    { "ns16550",        2, VXB_BUSID_PLB, 0, ns1655x3Num,         ns1655x3Resources },
    { "ns16550",        3, VXB_BUSID_PLB, 0, ns1655x4Num,         ns1655x4Resources },
    { "ns16550",        4, VXB_BUSID_PLB, 0, ns1655x5Num,         ns1655x5Resources },
#endif /* INCLUDE_QUAD_UART */

#ifdef INCLUDE_PCI_BUS
    { "m85xxPci",       0, VXB_BUSID_PLB, 0, m85xxPci1Num,        m85xxPci1Resources },
    { "m85xxPci",       1, VXB_BUSID_PLB, 0, m85xxPci0Num,        m85xxPci0Resources },
#endif /* INCLUDE_PCI_BUS */

#ifdef INCLUDE_DSHM
    { "dshmBusCtlr8641", 0, VXB_BUSID_PLB, 0,
      NELEMENTS(dshmBusCtlrPlbRes), dshmBusCtlrPlbRes },
    { "dshmPeerVx8641",  0, VXB_BUSID_VIRTUAL, 0,
      NELEMENTS(dshmPeerVxPlb0), dshmPeerVxPlb0 },
    { "dshmPeerVx8641",  1, VXB_BUSID_VIRTUAL, 0,
      NELEMENTS(dshmPeerVxPlb1), dshmPeerVxPlb1 },
#endif /* INCLUDE_DSHM */
    { NULL }
};
const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] = {
    { XTSEC_NAME,   0,  "l2disable",    VXB_PARAM_INT32,        {(void *) 1} },
    { XTSEC_NAME,   1,  "l2disable",    VXB_PARAM_INT32,        {(void *) 1} },
    { XTSEC_NAME,   2,  "l2disable",    VXB_PARAM_INT32,        {(void *) 1} },
    { XTSEC_NAME,   3,  "l2disable",    VXB_PARAM_INT32,        {(void *) 1} },
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, {(void *)0} }
};
