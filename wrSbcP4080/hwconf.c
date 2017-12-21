/* hwconf.c - Hardware configuration support module */

/*
 * Copyright (c) 2010-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01j,31mar12,y_y  remove unused flag I2C_RD_EXT_ACK.
01i,08mar12,y_y  add vxBus I2C support.
01h,12jan12,agf  remove exIrqWkrd from epic resource, deprecated in driver
                 (WIND00176018)
01g,09sep11,b_m  Add missing PCIe configuration parameters (WIND00264966)
01f,21jul11,e_d  switch to use PCIe legacy interrupt by default.
01e,15apr11,e_d  add fmanNum for tgec resource.(WIND00263761)
01d,21feb11,e_d  removed unused sio define and added coreNum for AMP mode
01c,11jan11,e_d  modified the tgec device index value.(WIND00249641)
01b,29nov10,e_d  code clean
01a,21oct10,wap  created from fsl_p4080_ds hwconf.c version 02c
*/

#include <vxWorks.h>
#include <vsbConfig.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#include <../src/hwif/h/vxbus/vxbRapidIO.h>
#include "config.h"
#include <drv/pci/pciAutoConfigLib.h>
#include <hwif/vxbus/vxbIntrCtlr.h>
#include <../src/hwif/h/end/vxbDtsecEnd.h>
#include <../src/hwif/h/mii/mv88E1x11Phy.h>

/* FIXME: move vector definitions elsewhere so this file is not needed here */

#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
#include <../src/hwif/intCtlr/vxbIntDynaCtlrLib.h>
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */

#ifdef DRV_INTCTLR_EPIC
extern UINT32 sysPicClkFreqGet(UINT32);
#endif /* DRV_INTCTLR_EPIC */

#ifdef DRV_I2CBUS_FSL
#   include <hwif/vxbus/vxbI2cLib.h>
#endif /* DRV_I2CBUS_FSL */

#ifdef DRV_PCIBUS_QORIQ_PCIEX
IMPORT STATUS sysPci1AutoconfigInclude ();
IMPORT UCHAR sysPci1AutoconfigIntrAssign (PCI_SYSTEM *, PCI_LOC *, UCHAR);
IMPORT UCHAR sysPci2AutoconfigIntrAssign (PCI_SYSTEM *, PCI_LOC *, UCHAR);
IMPORT UCHAR sysPci3AutoconfigIntrAssign (PCI_SYSTEM *, PCI_LOC *, UCHAR);
#endif /* DRV_PCIBUS_QORIQ_PCIEX */

IMPORT int sysEprGet(void);

#ifdef INCLUDE_EHCI
IMPORT void  ehci0Init (void);
IMPORT void  ehci0PostResetHook (void);
#endif /* INCLUDE_EHCI */

/*
 * On the wrSbcP4080 board, the default network port configuration
 * is as follows:
 *
 * MAC      PHY     Management port pins    MII address
 * -------- --------------- --------------------    -----------
 * DTSEC0   SFP module  fslI2c3         N/A
 * DTSEC1   SFP module  fslI2c3         N/A
 * DTSEC2   SFP module  fslI2c3         N/A
 * DTSEC3   SFP module  fslI2c3         N/A
 * DTSEC4   RGMII on-board  DTSEC0          0x19
 * DTSEC5   None        DTSEC0          None
 * DTSEC6   None        DTSEC0          None
 * DTSEC7   None        DTSEC0          None
 *
 * TGEC0    None        TGEC0           None
 * TGEC1    XFP module  TGEC0           0x18
 *
 * The board comes with with two reset configuration words that
 * allow for either for a single RGMII network port and two x4
 * PCIe channels, or one RGMII port, four SGMII ports and one
 * 10 gigabit XAUI port. The default configuration is to have
 * just the single RGMII port enabled. This can be changed by
 * removing jumper JP24.
 *
 * DTSEC4 (the first port on the second FMAN) is always configured
 * for RGMII mode. It's managed via DTSEC0's MDIO pins, and uses
 * a Broadcom PHY.
 *
 * DTSEC0, DTSEC1, DTSEC2 and DTSEC3, when enabled, operate in SGMII
 * mode, and are wired out to four SFP slots. DTSEC0 is populated
 * from the factory by default, while the remaining slots are left
 * empty. The provided SFP module is a Methode DM7041, which provides
 * 10/100/1000 copper media support via a Marvell 88E1111 PHY.
 *
 * Management of the PHYs in the SFP modules is performed via I2C.
 * The management pins of the SFP modules are connected to the second
 * port on the second I2C block in the P4080. This is managed via the
 * fslI2c3 VxBus device node.
 *
 * Because each module uses the same I2C address, the I2C pins are
 * multiplexed using the GPIO pins for control. The following table
 * shows how the GPIO pins are connected:
 *
 * The supported GPIO selections are shown below:
 *
 *      GPIO[18:19]     Device
 *      ---------       ----------------
 *      0 0             SFP module 0 select
 *      0 1             SFP module 1 select
 *      1 0             SFP module 2 select
 *      1 1             SFP module 3 select
 *
 *      GPIO            Device
 *      ---------       ----------------
 *      0               SFP module 0 enable
 *      4               SFP module 1 enable
 *      9               SFP module 2 enable
 *      26              SFP module 3 enable
 *
 * The i2cMdio driver provides link management support for the
 * SFP modules. It requires the fslI2c driver and the fslGpio driver.
 *
 * Note that the default BSP setup assumes the presence of the
 * Methode DM7041 modules which use the Marvell 88E1111 PHY. The PHY
 * defaults to 1000baseX mode and must be configured through software
 * to use 1000baseT mode, in order to properly negotiate 10Mbps and
 * 100Mbps links. SFP modules from other vendors that use different
 * PHY chips might not be directly compatible: if they also require
 * software setup, additional driver support may be needed.
 */

#ifdef DRV_RESOURCE_FSLGPIO
const struct hcfResource gpio0Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)(CCSBAR + 0x130000) } },
};
#   define gpio0Num NELEMENTS(gpio0Resources)
#endif /* DRV_RESOURCE_FSLGPIO */

#ifdef DRV_MII_I2C_MDIO
const struct hcfResource i2cMdio0Resources[] = {
    { "regBase",       HCF_RES_INT,    { (void *)(0) } },
#   ifdef INCLUDE_AMP
    { "sharedMem",     HCF_RES_INT,    { (void *)(TM_ANCHOR_ADRS + 4)} },
#   endif  /* INCLUDE_AMP */

    /* gpioSelect */
	
    { "gpioSelect1",   HCF_RES_INT,    { (void *)(0) } },
    { "gpioSelect2",   HCF_RES_INT,    { (void *)(1) } },
    { "gpioSelect3",   HCF_RES_INT,    { (void *)(2) } },
    { "gpioSelect4",   HCF_RES_INT,    { (void *)(3) } },

    /* gpioEnable */
	
    { "gpioEnable1",   HCF_RES_INT,    { (void *)(0) } },
    { "gpioEnable2",   HCF_RES_INT,    { (void *)(4) } },
    { "gpioEnable3",   HCF_RES_INT,    { (void *)(9) } },
    { "gpioEnable4",   HCF_RES_INT,    { (void *)(26)} },
};
#   define i2cMdio0Num NELEMENTS(i2cMdio0Resources)
#endif  /* DRV_MII_I2C_MDIO */

#define DTSEC_MDIO  "dtsecMdio"

const struct hcfResource dtsec0Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)(CCSBAR + 0x4E0000) } },
    { "phyAddr",    HCF_RES_INT,    { (void *)0x0 } },
    { "miiIfName",  HCF_RES_STRING, { (void *)"i2cMdio" } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)0 } },
    { "dtsecMedia", HCF_RES_INT,    { (void *)DTSEC_MEDIA_SGMII } },
#if defined (INCLUDE_AMP_CPU_01)
    { "coreNum",    HCF_RES_INT,    { (void *)1 } },
#endif /* INCLUDE_AMP_CPU_01 */
};
#define dtsec0Num NELEMENTS(dtsec0Resources)

const struct hcfResource dtsec1Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)(CCSBAR + 0x4E2000) } },
    { "phyAddr",    HCF_RES_INT,    { (void *)0x1 } },
    { "miiIfName",  HCF_RES_STRING, { (void *)"i2cMdio" } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)0 } },
    { "dtsecMedia", HCF_RES_INT,    { (void *)DTSEC_MEDIA_SGMII } },
#if defined (INCLUDE_AMP_CPU_02)
    { "coreNum",    HCF_RES_INT,    { (void *)2 } },
#endif /* INCLUDE_AMP_CPU_02 */
};
#define dtsec1Num NELEMENTS(dtsec1Resources)

const struct hcfResource dtsec2Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)(CCSBAR + 0x4E4000) } },
    { "phyAddr",    HCF_RES_INT,    { (void *)0x2 } },
    { "miiIfName",  HCF_RES_STRING, { (void *)"i2cMdio" } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)0 } },
    { "dtsecMedia", HCF_RES_INT,    { (void *)DTSEC_MEDIA_SGMII } },
#if defined (INCLUDE_AMP_CPU_03)
    { "coreNum",    HCF_RES_INT,    { (void *)3 } },
#endif /* INCLUDE_AMP_CPU_03 */
};
#define dtsec2Num NELEMENTS(dtsec2Resources)

const struct hcfResource dtsec3Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)(CCSBAR + 0x4E6000) } },
    { "phyAddr",    HCF_RES_INT,    { (void *)0x3 } },
    { "miiIfName",  HCF_RES_STRING, { (void *)"i2cMdio" } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)0 } },
    { "dtsecMedia", HCF_RES_INT,    { (void *)DTSEC_MEDIA_SGMII } },
#if defined (INCLUDE_AMP_CPU_04)
    { "coreNum",    HCF_RES_INT,    { (void *)4 } },
#endif /* INCLUDE_AMP_CPU_04 */
};
#define dtsec3Num NELEMENTS(dtsec3Resources)

const struct hcfResource dtsec4Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)(CCSBAR + 0x5E0000) } },
    { "phyAddr",    HCF_RES_INT,    { (void *)0x19 } },
    { "miiIfName",  HCF_RES_STRING, { (void *)DTSEC_MDIO } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)0 } },
    { "dtsecMedia", HCF_RES_INT,    { (void *)DTSEC_MEDIA_RGMII } },
#if defined (INCLUDE_AMP_CPU_00)
    { "coreNum",    HCF_RES_INT,    { (void *)0 } },
#endif /* INCLUDE_AMP_CPU_00 */
};
#define dtsec4Num NELEMENTS(dtsec4Resources)

const struct hcfResource dtsecMdio0Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)(CCSBAR + 0x4E1000) } }
};
#define dtsecMdio0Num NELEMENTS(dtsecMdio0Resources)

const struct hcfResource tgecMdio0Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)(CCSBAR + 0x4F1000) } }
};
#define tgecMdio0Num NELEMENTS(tgecMdio0Resources)

/*
 * Each frame manager also include a 10 gigabit ethernet MAC (TGEC).
 * Use of the TGEC with the wrSbcP4080 board requires the
 * purchase of a separate XFP media module. For wrSbcP4080, the tgec
 * is connected to FMAN1 on the board. Note for AMP mode, tgec0 must
 * be assigned to core0.
 */

const struct hcfResource tgec0Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)(CCSBAR + 0x5F0000) } },
    { "fmanNum",    HCF_RES_INT,    { (void *)1 } },
};

#define tgec0Num NELEMENTS(tgec0Resources)

#ifdef DRV_PCIBUS_QORIQ_PCIEX

/*
 * The wrSbcP4080 board has two x4 PCIe slots, connected to
 * PCIEX1 and PCIEX2. To use the PCIe slots, jumper JP24
 * must be present (this selects the RCW that enables PCIe
 * support). PCIEX3 is not used.
 *
 * Both legacy INTx and MSI interrupts are supported. Since
 * MSI is the prefered mechanism for handling interrupts
 * with PCIe, it's enabled by default. This can be changed by
 * setting the "msiEnable" property to FALSE (or just deleting
 * it). MSI will only be used for devices that advertise MSI
 * capability support in their configuration header.
 */

const struct hcfResource pci0Resources[] = {
    { "regBase",         HCF_RES_INT,    { (void *)(CCSBAR + 0x200000) } },
    { "mem32Addr",       HCF_RES_ADDR,   { (void *)PCIEX1_MEM_ADRS } },
    { "mem32Size",       HCF_RES_INT,    { (void *)PCIEX1_MEM_SIZE } },
    { "memIo32Addr",     HCF_RES_ADDR,   { (void *)PCIEX1_MEMIO_ADRS } },
    { "memIo32Size",     HCF_RES_INT,    { (void *)PCIEX1_MEMIO_SIZE } },
    { "io32Addr",        HCF_RES_ADDR,   { (void *)PCIEX1_IO32_ADRS } },
    { "io32Size",        HCF_RES_INT,    { (void *)PCIEX1_IO32_SIZE } },
    { "io16Addr",        HCF_RES_ADDR,   { (void *)PCIEX1_IO_ADRS } },
    { "io16Size",        HCF_RES_INT,    { (void *)PCIEX1_IO_SIZE } },
    { "cacheSize",       HCF_RES_INT,    { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",    HCF_RES_INT,    { (void *)PCI_LAT_TIMER } },
    { "maxBusSet",       HCF_RES_INT,    { (void *)16 } },
    { "lawBase",         HCF_RES_ADDR,   { (void *)PCIEX1_LAW_BASE } },
    { "lawSize",         HCF_RES_INT,    { (void *)PCIEX1_LAW_SIZE } },
    { "includeFuncSet",  HCF_RES_ADDR,   { (void *)sysPci1AutoconfigInclude } },
    { "intAssignFuncSet",HCF_RES_ADDR,   { (void *)sysPci1AutoconfigIntrAssign } },
    { "msiEnable",       HCF_RES_INT,    { (void *)FALSE } }
};

#define pci0Num NELEMENTS(pci0Resources)

const struct hcfResource pci1Resources[] = {
    { "regBase",         HCF_RES_INT,    { (void *)(CCSBAR + 0x201000) } },
    { "mem32Addr",       HCF_RES_ADDR,   { (void *)PCIEX2_MEM_ADRS } },
    { "mem32Size",       HCF_RES_INT,    { (void *)PCIEX2_MEM_SIZE } },
    { "memIo32Addr",     HCF_RES_ADDR,   { (void *)PCIEX2_MEMIO_ADRS } },
    { "memIo32Size",     HCF_RES_INT,    { (void *)PCIEX2_MEMIO_SIZE } },
    { "io32Addr",        HCF_RES_ADDR,   { (void *)PCIEX2_IO32_ADRS } },
    { "io32Size",        HCF_RES_INT,    { (void *)PCIEX2_IO32_SIZE } },
    { "io16Addr",        HCF_RES_ADDR,   { (void *)PCIEX2_IO_ADRS } },
    { "io16Size",        HCF_RES_INT,    { (void *)PCIEX2_IO_SIZE } },
    { "cacheSize",       HCF_RES_INT,    { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",    HCF_RES_INT,    { (void *)PCI_LAT_TIMER } },
    { "maxBusSet",       HCF_RES_INT,    { (void *)16 } },
    { "lawBase",         HCF_RES_ADDR,   { (void *)PCIEX2_LAW_BASE } },
    { "lawSize",         HCF_RES_INT,    { (void *)PCIEX2_LAW_SIZE } },
    { "includeFuncSet",  HCF_RES_ADDR,   { (void *)sysPci1AutoconfigInclude } },
    { "intAssignFuncSet",HCF_RES_ADDR,   { (void *)sysPci2AutoconfigIntrAssign } },
    { "msiEnable",       HCF_RES_INT,    { (void *)FALSE } }
};

#define pci1Num NELEMENTS(pci1Resources)

const struct hcfResource pci2Resources[] = {
    { "regBase",         HCF_RES_INT,    { (void *)(CCSBAR + 0x202000) } },
    { "mem32Addr",       HCF_RES_ADDR,   { (void *)PCIEX3_MEM_ADRS } },
    { "mem32Size",       HCF_RES_INT,    { (void *)PCIEX3_MEM_SIZE } },
    { "memIo32Addr",     HCF_RES_ADDR,   { (void *)PCIEX3_MEMIO_ADRS } },
    { "memIo32Size",     HCF_RES_INT,    { (void *)PCIEX3_MEMIO_SIZE } },
    { "io32Addr",        HCF_RES_ADDR,   { (void *)PCIEX3_IO32_ADRS } },
    { "io32Size",        HCF_RES_INT,    { (void *)PCIEX3_IO32_SIZE } },
    { "io16Addr",        HCF_RES_ADDR,   { (void *)PCIEX3_IO_ADRS } },
    { "io16Size",        HCF_RES_INT,    { (void *)PCIEX3_IO_SIZE } },
    { "cacheSize",       HCF_RES_INT,    { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",    HCF_RES_INT,    { (void *)PCI_LAT_TIMER } },
    { "maxBusSet",       HCF_RES_INT,    { (void *)16 } },
    { "lawBase",         HCF_RES_ADDR,   { (void *)PCIEX3_LAW_BASE } },
    { "lawSize",         HCF_RES_INT,    { (void *)PCIEX3_LAW_SIZE } },
    { "includeFuncSet",  HCF_RES_ADDR,   { (void *)sysPci1AutoconfigInclude } },
    { "intAssignFuncSet",HCF_RES_ADDR,   { (void *)sysPci3AutoconfigIntrAssign } },
    { "msiEnable",       HCF_RES_INT,    { (void *)FALSE } },
};

#define pci2Num NELEMENTS(pci2Resources)

#endif /* DRV_PCIBUS_QORIQ_PCIEX */

const struct hcfResource law0Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,     { (void *)(CCSBAR + 0xC00) } },
};
#define law0Num NELEMENTS(law0Resources)

/*
 * At least with the current silicon rev, he frame manager
 * in the P4080 requires a microcode patch in order for
 * certain features to work correctly. This includes the
 * soft parser, independent mode and the coarse classifier.
 * The microcode patch only needs to be loaded once per
 * fman instance, after a hard reset. In an AMP configuration,
 * only CPU 0 has to load it.
 */

struct hcfResource fman0Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,     { (void *)(CCSBAR + 0x400000) } },
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_00)
    { "globalInit",     HCF_RES_INT,     { (void *)1} },
    { "ucodeAddr",      HCF_RES_ADDR,    { (void *)NULL} }
#endif /* INCLUDE_AMP */
};
#define fman0Num NELEMENTS(fman0Resources)

struct hcfResource fman1Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,     { (void *)(CCSBAR + 0x500000) } },
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_00)
    { "globalInit",     HCF_RES_INT,     { (void *)1} },
    { "ucodeAddr",      HCF_RES_ADDR,    { (void *)NULL} }
#endif /* INCLUDE_AMP */
};
#define fman1Num NELEMENTS(fman1Resources)

const struct hcfResource qman0Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,    { (void *)(CCSBAR + 0x318000) } },
    { "qmanLawBase",    HCF_RES_ADDR,   { (void *)QMAN_LAW_BASE} },
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_00)
    { "qmanMemBase",    HCF_RES_ADDR,   { (void *)QMAN_MEM_BASE} },
#endif /* INCLUDE_AMP */
};
#define qman0Num NELEMENTS(qman0Resources)

#ifdef DRV_RESOURCE_QORIQBMAN
const struct hcfResource bman0Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,    { (void *)(CCSBAR + 0x31A000) } },
    { "bmanLawBase",    HCF_RES_ADDR,   { (void *)BMAN_LAW_BASE} },
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_00)
    { "bmanMemBase",    HCF_RES_ADDR,   { (void *)BMAN_MEM_BASE} },
#endif /* INCLUDE_AMP */
};
#define bman0Num NELEMENTS(bman0Resources)
#endif /* DRV_RESOURCE_QORIQBMAN */

#ifdef DRV_DMA_FSL
const struct hcfResource fslDma1Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)DMA1_REGBASE } },
};
#define fslDma1Num NELEMENTS(fslDma1Resources)

const struct hcfResource fslDma2Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)DMA2_REGBASE } },
};
#define fslDma2Num NELEMENTS(fslDma2Resources)
#endif  /* DRV_DMA_FSL */

const struct hcfResource ns1655x1Resources[] =
    {
    { "regBase",        HCF_RES_INT,    { (void *)COM1_ADR} },
    { "irq",            HCF_RES_INT,    { (void *)EPIC_DUART_INT_VEC} },
    { "regInterval",    HCF_RES_INT,    { (void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",       HCF_RES_INT,    { (void *)EPIC_DUART_INT_VEC} },
    { "clkFreq",        HCF_RES_ADDR,   { (void *)&sysClkFreqGet} }
    };

#define ns1655x1Num NELEMENTS(ns1655x1Resources)

struct intrCtlrInputs epicInputs[] = {
#if defined(INCLUDE_MSD) || defined(INCLUDE_AMP_CPU_00) || !defined(INCLUDE_AMP)
    { EPIC_DUART_INT_VEC,        "ns16550",      0, 0},
#endif /* INCLUDE_MSD */
    { EPIC_QMAN_PORTAL0_INT_VEC, "QorIQQman",    0, 0 },
    { EPIC_QMAN_PORTAL1_INT_VEC, "QorIQQman",    0, 1 },
    { EPIC_QMAN_PORTAL2_INT_VEC, "QorIQQman",    0, 2 },
    { EPIC_QMAN_PORTAL3_INT_VEC, "QorIQQman",    0, 3 },
    { EPIC_QMAN_PORTAL4_INT_VEC, "QorIQQman",    0, 4 },
    { EPIC_QMAN_PORTAL5_INT_VEC, "QorIQQman",    0, 5 },
    { EPIC_QMAN_PORTAL6_INT_VEC, "QorIQQman",    0, 6 },
    { EPIC_QMAN_PORTAL7_INT_VEC, "QorIQQman",    0, 7 },
    { EPIC_QMAN_PORTAL8_INT_VEC, "QorIQQman",    0, 8 },
    { EPIC_QMAN_PORTAL9_INT_VEC, "QorIQQman",    0, 9 },
    { EPIC_BMAN_PORTAL0_INT_VEC, "QorIQBman",    0, 0 },
    { EPIC_BMAN_PORTAL1_INT_VEC, "QorIQBman",    0, 1 },
    { EPIC_BMAN_PORTAL2_INT_VEC, "QorIQBman",    0, 2 },
    { EPIC_BMAN_PORTAL3_INT_VEC, "QorIQBman",    0, 3 },
    { EPIC_BMAN_PORTAL4_INT_VEC, "QorIQBman",    0, 4 },
    { EPIC_BMAN_PORTAL5_INT_VEC, "QorIQBman",    0, 5 },
    { EPIC_BMAN_PORTAL6_INT_VEC, "QorIQBman",    0, 6 },
    { EPIC_BMAN_PORTAL7_INT_VEC, "QorIQBman",    0, 7 },
    { EPIC_BMAN_PORTAL8_INT_VEC, "QorIQBman",    0, 8 },
    { EPIC_BMAN_PORTAL9_INT_VEC, "QorIQBman",    0, 9 },
    { GPIO_INT_VEC,              "fslGpio",      1, 0 },
#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    { OPENPIC_TIMERA0_INT_VEC,   "openPicTimer", 0, 0 },
    { OPENPIC_TIMERA0_INT_VEC+1, "openPicTimer", 0, 1 },
    { OPENPIC_TIMERA0_INT_VEC+2, "openPicTimer", 0, 2 },
    { OPENPIC_TIMERA0_INT_VEC+3, "openPicTimer", 0, 3 },
    { OPENPIC_TIMERB0_INT_VEC,   "openPicTimer", 1, 0 },
    { OPENPIC_TIMERB0_INT_VEC+1, "openPicTimer", 1, 1 },
    { OPENPIC_TIMERB0_INT_VEC+2, "openPicTimer", 1, 2 },
    { OPENPIC_TIMERB0_INT_VEC+3, "openPicTimer", 1, 3 },
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */

#ifdef INCLUDE_EHCI
    { EPIC_USB_INT_VEC,         "vxbPlbUsbEhci", 0, 0 },
#endif  /* INCLUDE_EHCI */

#ifdef  DRV_DMA_FSL
    { EPIC_DMA1_CHAN0_INT_VEC,  "fslDma",        0, 0 },
    { EPIC_DMA1_CHAN1_INT_VEC,  "fslDma",        0, 1 },
    { EPIC_DMA1_CHAN2_INT_VEC,  "fslDma",        0, 2 },
    { EPIC_DMA1_CHAN3_INT_VEC,  "fslDma",        0, 3 },
    { EPIC_DMA2_CHAN0_INT_VEC,  "fslDma",        1, 0 },
    { EPIC_DMA2_CHAN1_INT_VEC,  "fslDma",        1, 1 },
    { EPIC_DMA2_CHAN2_INT_VEC,  "fslDma",        1, 2 },
    { EPIC_DMA2_CHAN3_INT_VEC,  "fslDma",        1, 3 },
#endif  /* DRV_DMA_FSL */

#ifdef DRV_I2CBUS_FSL
    { EPIC_I2C_INT_VEC,         "fslI2c",        0, 0 },
    { EPIC_I2C_INT_VEC,         "fslI2c",        1, 0 },
    { EPIC_I2C2_INT_VEC,        "fslI2c",        2, 0 },
    { EPIC_I2C2_INT_VEC,        "fslI2c",        3, 0 },
#endif  /* DRV_I2CBUS_FSL */

#ifdef _WRS_CONFIG_SMP
    { EPIC_VEC_IPI_IRQ0,        "ipi",           0, 0 }
#endif /* _WRS_CONFIG_SMP */

};

struct intrCtlrXBar epicXBar[] = {
    { 0, 0 }
};

struct intrCtlrPriority epicPriority[] = {
    { EPIC_DUART2_INT_VEC, 100 },
    { EPIC_DUART_INT_VEC,  100 }
};

INT_BANK_DESC exDesc = { 0, 12, 0x50000, 0x20, EPIC_EX_INTERRUPT };
INT_BANK_DESC inDesc = { EPIC_VEC_IN_IRQ0, 128, 0x50200, 0x20, EPIC_IN_INTERRUPT };
INT_BANK_DESC msgDesc = { 176, 8, 0x51600, 0x20, EPIC_MSG_INTERRUPT };
INT_BANK_DESC gtaDesc = { OPENPIC_TIMERA0_INT_VEC, 4, 0x41120, 0x40, EPIC_GT_A_INTERRUPT };
INT_BANK_DESC gtbDesc = { OPENPIC_TIMERB0_INT_VEC, 4, 0x42120, 0x40, EPIC_GT_B_INTERRUPT };
INT_BANK_DESC smsgDesc = { 243, 8, 0x51c00, 0x20, EPIC_SMSG_INTERRUPT };
INT_BANK_DESC ipiDesc = { 251, 4, 0x410a0, 0x10, EPIC_IPI_INTERRUPT };

const struct hcfResource epic0Resources[] = {
    { VXB_REG_BASE,        HCF_RES_INT,  { (void *)CCSBAR } },
    { "input",             HCF_RES_ADDR, { (void *)&epicInputs[0] } },
    { "inputTableSize",    HCF_RES_INT,  { (void *)NELEMENTS(epicInputs) } },
    { "numCpus",           HCF_RES_INT,  { (void *)(8) } },
    { "priority",          HCF_RES_ADDR, { (void *)&epicPriority[0] } },
    { "priorityTableSize", HCF_RES_INT,  { (void *)NELEMENTS(epicPriority) } },
    { "crossBar",          HCF_RES_ADDR, { (void *)&epicXBar[0] } },
    { "crossBarTableSize", HCF_RES_INT,  { (void *)NELEMENTS(epicXBar) } },
    { "exPolar",           HCF_RES_INT,  { (void *)EPIC_INT_ACT_LOW } },
    { "numInts",           HCF_RES_INT,  { (void *)255 } },
    { "intDesc_0",         HCF_RES_ADDR, { (void *)&exDesc } },
    { "intDesc_1",         HCF_RES_ADDR, { (void *)&inDesc } },
    { "intDesc_2",         HCF_RES_ADDR, { (void *)&msgDesc } },
    { "intDesc_3",         HCF_RES_ADDR, { (void *)&gtaDesc } },
    { "intDesc_4",         HCF_RES_ADDR, { (void *)&gtbDesc } },
    { "intDesc_5",         HCF_RES_ADDR, { (void *)&smsgDesc } },
    { "intDesc_6",         HCF_RES_ADDR, { (void *)&ipiDesc } },
#ifdef EPIC_EXTERNAL_PROXY
    { "sysEprGet",         HCF_RES_ADDR, { (void *)&sysEprGet } },
#endif /* EPIC_EXTERNAL_PROXY */
};
#define epic0Num NELEMENTS(epic0Resources)

#ifdef DRV_TIMER_M85XX

struct hcfResource m85xxTimerResources[] =  {
    {"regBase",      HCF_RES_INT,  { (void *)0 } },
    {"decMinClkRate",HCF_RES_INT,  { (void *)SYS_CLK_RATE_MIN } },
    {"decMaxClkRate",HCF_RES_INT,  { (void *)SYS_CLK_RATE_MAX } },
    {"fitMinClkRate",HCF_RES_INT,  { (void *)AUX_CLK_RATE_MIN } },
    {"fitMaxClkRate",HCF_RES_INT,  { (void *)AUX_CLK_RATE_MAX } },
    {"sysClockFreq", HCF_RES_ADDR, { (void *)&sysClkFreqGet } }
};
#define m85xxTimerNum         NELEMENTS(m85xxTimerResources)

#endif /* DRV_TIMER_M85XX */

struct intrCtlrInputs ppcIntCtlrInputs[] = {
    { 0, "epic", 0, 0 }
};

const struct hcfResource ppcIntCtlr0Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,    { (void *)TRUE} },
    { "input",          HCF_RES_ADDR,   { (void *)&ppcIntCtlrInputs[0] } },
    { "inputTableSize", HCF_RES_INT,    { (void *)NELEMENTS(ppcIntCtlrInputs) } },
};
#define ppcIntCtlr0Num NELEMENTS(ppcIntCtlr0Resources)

#ifdef INCLUDE_EHCI
const struct hcfResource usbEhci0Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,    { (void *)EHCI_CAPLENGTH(CCSBAR + 0x210000) } },
    { "ehciInit",       HCF_RES_ADDR,   { (void *)ehci0Init } },
    { "dataSwap",       HCF_RES_ADDR,   { (void *)vxbSwap32 } },
};
#define usbEhci0Num NELEMENTS(usbEhci0Resources)
#endif /* INCLUDE_EHCI */

#ifdef DRV_TIMER_OPENPIC
const struct hcfResource openPicTimerDevAresources[] =
    {
    {VXB_REG_BASE,      HCF_RES_INT,    { (void *)(CCSBAR + 0x000410F0) } },
    {"instanceName",    HCF_RES_STRING, { (void *)"A" }},
    {VXB_CLK_FREQ,      HCF_RES_ADDR,   { (void *)sysPicClkFreqGet } },
    {"clkDivisor",      HCF_RES_INT,    { (void *)8}},
#ifdef INCLUDE_AUX_CLK
    {"maxClkRate0",     HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX } },
    {"maxClkRate1",     HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX } },
    {"maxClkRate2",     HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX } },
    {"maxClkRate3",     HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX } },
#endif /* INCLUDE_AUX_CLK */
#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    {"vector0",         HCF_RES_INT,    { (void *)OPENPIC_TIMERA0_INT_VEC } },
    {"vector1",         HCF_RES_INT,    { (void *)OPENPIC_TIMERA0_INT_VEC+1 } },
    {"vector2",         HCF_RES_INT,    { (void *)OPENPIC_TIMERA0_INT_VEC+2 } },
    {"vector3",         HCF_RES_INT,    { (void *)OPENPIC_TIMERA0_INT_VEC+3 } },
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
    };
#define openPicTimerDevAnum NELEMENTS(openPicTimerDevAresources)

const struct hcfResource openPicTimerDevBresources[] =
    {
    {VXB_REG_BASE,      HCF_RES_INT,    { (void *)(CCSBAR + 0x000420F0) } },
    {"instanceName",    HCF_RES_STRING, { (void *)"B" }},
    {VXB_CLK_FREQ,      HCF_RES_ADDR,   { (void *)sysPicClkFreqGet } },
    {"clkDivisor",      HCF_RES_INT,    { (void *)8}},
#ifdef INCLUDE_AUX_CLK
    {"maxClkRate0",     HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX } },
    {"maxClkRate1",     HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX } },
    {"maxClkRate2",     HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX } },
    {"maxClkRate3",     HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX } },
#endif /* INCLUDE_AUX_CLK */
#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    {"vector0",         HCF_RES_INT,    { (void *)OPENPIC_TIMERB0_INT_VEC } },
    {"vector1",         HCF_RES_INT,    { (void *)OPENPIC_TIMERB0_INT_VEC+1 } },
    {"vector2",         HCF_RES_INT,    { (void *)OPENPIC_TIMERB0_INT_VEC+2 } },
    {"vector3",         HCF_RES_INT,    { (void *)OPENPIC_TIMERB0_INT_VEC+3 } },
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
    };
#define openPicTimerDevBnum NELEMENTS(openPicTimerDevBresources)
#endif /* DRV_TIMER_OPENPIC */

#ifdef INCLUDE_TSEC_MDIO
LOCAL const struct hcfResource mdio0Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x31c000) } },
    { "sharedMem",      HCF_RES_INT,    { (void *)TM_ANCHOR_ADRS } }
};
#define mdio0Num NELEMENTS(mdio0Resources)
#endif /* INCLUDE_TSEC_MDIO */

#ifdef DRV_I2CBUS_FSL

struct i2cDevInputs i2c0DevTbl[] = {

/* 
 * Address  Part Number  Size     Description
 * -------- -------------------------------------------------
 * 0x50       24C64       64K-bit   Reset configuration word
 * 0x51       24C64       64K-bit   Reset configuration word
 * 0x56       DM7041                SFP module                      
 * 0x50       24C01        1K-bit   SFP module Serial ID info
 * 0x57       24C02        2K-bit   System configuration data
 */
 
    { "eeprom_at24c64",  0x50,    I2C_RDONLY | I2C_WORDADDR},
    { "eeprom_at24c64",  0x51,    I2C_RDONLY | I2C_WORDADDR},
    { "eeprom_at24c02",  0x57,    0}
};

const struct hcfResource i2c0Resources[] = {
    { "regBase",    HCF_RES_ADDR,   { (void *)(CCSBAR + 0x118000) } },
    { "clkFreq",    HCF_RES_ADDR,   { (void *)sysClkFreqGet } },
    { "busSpeed",   HCF_RES_INT,    { (void *)400000 } },
    { "polling",    HCF_RES_INT,    { (void *)0 } },
    { "i2cDev",     HCF_RES_ADDR,   { (void *)&i2c0DevTbl} },
    { "i2cDevNum",  HCF_RES_INT,    { (void *)NELEMENTS(i2c0DevTbl) } },
};
#   define i2c0Num NELEMENTS(i2c0Resources)

struct i2cDevInputs i2c3DevTbl[] = {
    { "eeprom_sfp",      0x50,    I2C_RDONLY},
    { "i2cMdio",         0x56,    0}
};

const struct hcfResource i2c3Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)(CCSBAR + 0x119100) } },
    { "clkFreq",    HCF_RES_ADDR,   { (void *)sysClkFreqGet } },
    { "busSpeed",   HCF_RES_INT,    { (void *)400000 } },
#   ifdef INCLUDE_AMP /* For SFP module in AMP mode, use polling mode */
    { "polling",    HCF_RES_INT,    { (void *)1 } },
#   else
    { "polling",    HCF_RES_INT,    { (void *)0 } },
#   endif
    { "i2cDev",     HCF_RES_ADDR,   { (void *)&i2c3DevTbl} },
    { "i2cDevNum",  HCF_RES_INT,    { (void *)NELEMENTS(i2c3DevTbl) } },
};
#   define i2c3Num NELEMENTS(i2c3Resources)
#endif /* DRV_I2CBUS_FSL */

const struct hcfDevice hcfDeviceList[] = {

    /*
     * Initialize ppcIntCtlr before epic.
     * The vector table for external interrupts are over written by epic
     * for an optimized purpose.
     */

    { "ppcIntCtlr", 0, VXB_BUSID_PLB, 0, ppcIntCtlr0Num, ppcIntCtlr0Resources },
    { "epic",       0, VXB_BUSID_PLB, 0, epic0Num,       epic0Resources },

#ifdef DRV_SIO_NS16550
#   if defined (INCLUDE_MSD) || !defined (INCLUDE_AMP)
    { "ns16550",    0, VXB_BUSID_PLB, 0, ns1655x1Num,    ns1655x1Resources },
#   else
#   ifdef INCLUDE_AMP_CPU_00
    { "ns16550",    0, VXB_BUSID_PLB, 0, ns1655x1Num,    ns1655x1Resources },
#   endif /* INCLUDE_AMP_CPU_00 */
#   endif /* defined (INCLUDE_MSD) || !defined (INCLUDE_AMP) */
#endif /* DRV_SIO_NS16550 */

/*
 * Make sure the GPIO driver is initialized before the
 * I2C and I2C MDIO drivers. Also, make the GPIO instance
 * unit 1 instead of unit 0. The dtsecMdio driver unconditionally
 * attaches to fslGpio0 if it exists, however its manipulation
 * of GPIO pins conflicts with the i2cMdio driver. To prevent
 * the conflict, the i2cMdio driver uses fslGpio1 instead.
 */

#ifdef DRV_RESOURCE_FSLGPIO
    { "fslGpio",    1, VXB_BUSID_PLB,0, gpio0Num,        gpio0Resources },
#endif /* DRV_RESOURCE_FSLGPIO */

/*
 * Because SFP module need I2C bus to control the phy device,
 * so we add vxbus driver for freescale P4080 I2C. We only init
 * I2C bus 3 for SFP module. For others I2C bus init, we used the
 * driver in BSP.
 */

#ifdef DRV_I2CBUS_FSL
    { "fslI2c",     0, VXB_BUSID_PLB,   0,  i2c0Num,        i2c0Resources },
    { "fslI2c",     3, VXB_BUSID_PLB,   0,  i2c3Num,        i2c3Resources },
#endif /* DRV_I2CBUS_FSL */

#ifdef DRV_MII_I2C_MDIO
    { "i2cMdio",    0, VXB_BUSID_I2C,   0,  i2cMdio0Num,     i2cMdio0Resources },
#endif /* DRV_MII_I2C_MDIO */

#ifdef DRV_RESOURCE_QORIQLAW
    { "QorIQLaw",   0, VXB_BUSID_PLB,0, law0Num,         law0Resources },
#endif /* DRV_RESOURCE_QORIQLAW */

#ifdef DRV_RESOURCE_QORIQBMAN
    { "QorIQBman",  0, VXB_BUSID_PLB,0, bman0Num,        bman0Resources },
#endif /* DRV_RESOURCE_QORIQBMAN */

#ifdef DRV_RESOURCE_QORIQQMAN
    { "QorIQQman",  0, VXB_BUSID_PLB,0, qman0Num,        qman0Resources },
#endif /* DRV_RESOURCE_QORIQQMAN */

#ifdef DRV_RESOURCE_QORIQFMAN
    { "QorIQFman",  0, VXB_BUSID_PLB,0, fman0Num,        fman0Resources },
    { "QorIQFman",  1, VXB_BUSID_PLB,0, fman1Num,        fman1Resources },
#endif /* DRV_RESOURCE_QORIQFMAN */

/*
 * Keep PCIe briges on AMP instance 0 by default. They can
 * be assigined to individual codes if needed.
 */

#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_00)
#   ifdef DRV_PCIBUS_QORIQ_PCIEX
    { "QorIQPciEx", 0, VXB_BUSID_PLB,0, pci0Num,         pci0Resources },
    { "QorIQPciEx", 1, VXB_BUSID_PLB,0, pci1Num,         pci1Resources },
#   endif /* DRV_PCIBUS_QORIQ_PCIEX */
#endif /* ! INCLUDE_AMP */

/*
 * The wrSbcP4080 has only 5 of the 8 DTSEC ports available
 */

#ifdef DRV_VXBEND_DTSEC
#   if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_01)
    { "dtsec",      0, VXB_BUSID_PLB, 0, dtsec0Num,      dtsec0Resources },
#   endif /* ! INCLUDE_AMP */
#   if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_02)
    { "dtsec",      1, VXB_BUSID_PLB, 0, dtsec1Num,      dtsec1Resources },
#   endif /* ! INCLUDE_AMP */
#   if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_03)
    { "dtsec",      2, VXB_BUSID_PLB, 0, dtsec2Num,      dtsec2Resources },
#   endif /* ! INCLUDE_AMP */
#   if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_04)
    { "dtsec",      3, VXB_BUSID_PLB, 0, dtsec3Num,      dtsec3Resources },
#   endif /* ! INCLUDE_AMP */
#   if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_00)
    { "dtsec",      4, VXB_BUSID_PLB, 0, dtsec4Num,      dtsec4Resources },
#   endif /* ! INCLUDE_AMP */
#endif /* DRV_VXBEND_DTSEC */

#ifdef DRV_MII_DTSEC_MDIO
    { "dtsecMdio",  0, VXB_BUSID_PLB,0, dtsecMdio0Num,   dtsecMdio0Resources },
#endif /* DRV_MII_DTSEC_MDIO */

/*
 * For AMP mode, tgec0 should be assigned to core0
 */

#ifdef DRV_VXBEND_TGEC
#   if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_00)
    { "tgec",       0, VXB_BUSID_PLB, 0, tgec0Num,       tgec0Resources },
#   endif /* ! INCLUDE_AMP */
#endif /* DRV_VXBEND_TGEC */

#ifdef DRV_TIMER_M85XX
    {"m85xxTimerDev",  0, VXB_BUSID_PLB, 0, m85xxTimerNum, m85xxTimerResources},
#endif /* DRV_TIMER_M85XX */

#ifdef DRV_TIMER_OPENPIC
    { "openPicTimer",  0, VXB_BUSID_PLB, 0, openPicTimerDevAnum, openPicTimerDevAresources },
    { "openPicTimer",  1, VXB_BUSID_PLB, 0, openPicTimerDevBnum, openPicTimerDevBresources },
#endif /* DRV_TIMER_OPENPIC */

#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci", 0, VXB_BUSID_PLB, 0, usbEhci0Num,         usbEhci0Resources },
#endif  /* INCLUDE_EHCI */

#ifdef DRV_DMA_FSL
    { "fslDma",        0, VXB_BUSID_PLB, 0, fslDma1Num,          fslDma1Resources },
    { "fslDma",        1, VXB_BUSID_PLB, 0, fslDma2Num,          fslDma2Resources },
#endif /* DRV_DMA_FSL */
};
const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] =
    {
#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci", 0, "hasEmbeddedTT", VXB_PARAM_INT32,   { (void *)TRUE } },
    { "vxbPlbUsbEhci", 0, "postResetHook", VXB_PARAM_FUNCPTR, { (void *)ehci0PostResetHook } },
#endif  /* INCLUDE_EHCI */

#ifdef  INCLUDE_MV88E1X11PHY
    { "mv88E1x11Phy",  0, "mode",          VXB_PARAM_INT32,   {(void *)MV88E_MODE_SGMII_NOCLK} },
    { "mv88E1x11Phy",  1, "mode",          VXB_PARAM_INT32,   {(void *)MV88E_MODE_SGMII_NOCLK} },
    { "mv88E1x11Phy",  2, "mode",          VXB_PARAM_INT32,   {(void *)MV88E_MODE_SGMII_NOCLK} },
    { "mv88E1x11Phy",  3, "mode",          VXB_PARAM_INT32,   {(void *)MV88E_MODE_SGMII_NOCLK} },
#endif  /* INCLUDE_MV88E1X11PHY */

#ifdef DRV_I2C_EEPROM
    { "eeprom_at24c02", 2, "writeDelay",   VXB_PARAM_INT32,   { (void *)1000 } },
#endif  /* DRV_I2C_EEPROM */

    { NULL,            0, NULL,            VXB_PARAM_END_OF_LIST, {(void *)0} }
    };
