/* hwconf.c - Hardware configuration support module */

/*
 * Copyright (c) 2011, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,05jul12,d_l  fix I2C configuration.(WIND00359788)
01c,05mar12,y_y  add vxBus I2C support.
01b,01mar12,d_l  Added epicCpu for routing interrupt to other CPUs.
01a,12dec11,d_l  Created
*/

#include <vxWorks.h>
#include <vsbConfig.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#include <../src/hwif/h/vxbus/vxbRapidIO.h>
#include "config.h"
#include "drv/pci/pciAutoConfigLib.h"
#include <hwif/vxbus/vxbIntrCtlr.h>
#include <../src/hwif/h/end/vxbDtsecEnd.h>

/* FIXME: move vector definitions elsewhere so this file is not needed here */
#ifdef DRV_INTCTLR_EPIC
#   include <hwif/intCtlr/vxbEpicIntCtlr.h>
#   include <../src/hwif/intCtlr/vxbIntDynaCtlrLib.h>
extern UINT32 sysPicClkFreqGet(UINT32);
#endif

#ifdef DRV_STORAGE_SDHC     
#   include <h/storage/vxbSdMmcLib.h>
#   include <h/storage/vxbSdhcStorage.h>
IMPORT UINT32 sysSdhcClkFreqGet(void);
#endif /* DRV_STORAGE_SDHC */
 
IMPORT UINT32 sysPpcTbClkFreqGet(void);
IMPORT UINT32 sysPlbClkFreqGet(void);

#ifdef DRV_PCIBUS_QORIQ_PCIEX
IMPORT STATUS sysPci1AutoconfigInclude ();
IMPORT UCHAR sysPci1AutoconfigIntrAssign (PCI_SYSTEM *, PCI_LOC *, UCHAR);
IMPORT UCHAR sysPci2AutoconfigIntrAssign (PCI_SYSTEM *, PCI_LOC *, UCHAR);
IMPORT UCHAR sysPci3AutoconfigIntrAssign (PCI_SYSTEM *, PCI_LOC *, UCHAR);
#endif

IMPORT int sysEprGet(void);

#ifdef INCLUDE_EHCI
IMPORT void  ehci0Init (void);
IMPORT void  ehci0PostResetHook (void);
IMPORT void  ehci1Init (void);
IMPORT void  ehci1PostResetHook (void);
#endif

#ifdef DRV_I2CBUS_FSL
#   include <hwif/vxbus/vxbI2cLib.h>
#endif /* DRV_I2CBUS_FSL */

#ifdef INCLUDE_AMP

/*
 * Users can control the assignment of devices to CPUs by using struct
 * ampCpuTable definitions.
 */

typedef struct {
        char*   deviceName;     /* hardWare device name */
        int     unit;           /* hardWare device index */
        int     cpu;            /* assigned core number */
} AMP_CPU_TABLE;

/*
 * The hardware devices on P5020 can be allocated to either core. Users just
 * need to fill the parameters such as the device driver name, device index and
 * which core the device should be assigned to, then system will install and
 * register the devices driver onto the corresponding core image. Table includes
 * only devices we want to filter. If no action required for a device per CPU
 * then it's not in the table
 */

AMP_CPU_TABLE ampCpuTable[] = {

    /* driver,          unit num,   assigned cpu */

    /* assigned cpu & coreNum resources must match for dtsec and tgec */

    { "tgec",           0,          0 },
    { "ns16550",        0,          0 },
    { "ns16550",        1,          0 },
    { "ns16550",        2,          1 },
    { "ns16550",        3,          1 },
    { "openPicTimer",   0,          0 },
    { "openPicTimer",   1,          0 },
    { "fslI2c",         0,          0 },
    { "fslI2c",         1,          0 },
    { "QorIQPciEx",     0,          0 },
    { "QorIQPciEx",     1,          0 },
    { "QorIQPciEx",     2,          0 },
};

#endif /* INCLUDE_AMP */

const struct hcfResource gpio0Resources[] = {
    { "regBase",    HCF_RES_INT,        { (void *)(CCSBAR + 0x130000) } },
};
#define gpio0Num NELEMENTS(gpio0Resources)

const struct hcfResource dtsec0Resources[] = {
    { "regBase",    HCF_RES_INT,        { (void *)(CCSBAR + 0x4E0000) } },
    { "phyAddr",    HCF_RES_INT,        { (void *)0x02 } },
    { "miiIfName",  HCF_RES_STRING,     { (void *)"dtsecMdio" } },
    { "miiIfUnit",  HCF_RES_INT,        { (void *)0 } },
    { "dtsecNum",   HCF_RES_INT,        { (void *)0 } },
    { "fmanNum",    HCF_RES_INT,        { (void *)0 } },
    { "dtsecMedia", HCF_RES_INT,        { (void *)DTSEC_MEDIA_SGMII } },
#   ifdef INCLUDE_AMP
    { "coreNum",    HCF_RES_INT,        { (void *)0 } },
#   endif /* INCLUDE_AMP */
};
#define dtsec0Num NELEMENTS(dtsec0Resources)

const struct hcfResource dtsec1Resources[] = {
    { "regBase",    HCF_RES_INT,        { (void *)(CCSBAR + 0x4E2000) } },
    { "phyAddr",    HCF_RES_INT,        { (void *)0x03 } },
    { "miiIfName",  HCF_RES_STRING,     { (void *)"dtsecMdio" } },
    { "miiIfUnit",  HCF_RES_INT,        { (void *)0 } },
    { "dtsecNum",   HCF_RES_INT,        { (void *)1 } },
    { "fmanNum",    HCF_RES_INT,        { (void *)0 } },
    { "dtsecMedia", HCF_RES_INT,        { (void *)DTSEC_MEDIA_SGMII } },
#if defined (INCLUDE_AMP_CPU_01)
    { "coreNum",    HCF_RES_INT,        { (void *)1 } },
#endif
};
#define dtsec1Num NELEMENTS(dtsec1Resources)

const struct hcfResource dtsec2Resources[] = {
    { "regBase",    HCF_RES_INT,        { (void *)(CCSBAR + 0x4E4000) } },
    { "phyAddr",    HCF_RES_INT,        { (void *)0x04 } },
    { "miiIfName",  HCF_RES_STRING,     { (void *)"dtsecMdio" } },
    { "miiIfUnit",  HCF_RES_INT,        { (void *)0 } },
    { "dtsecNum",   HCF_RES_INT,        { (void *)2 } },
    { "fmanNum",    HCF_RES_INT,        { (void *)0 } },
    { "dtsecMedia", HCF_RES_INT,        { (void *)DTSEC_MEDIA_SGMII } },
#if defined (INCLUDE_AMP_CPU_02)
    { "coreNum",    HCF_RES_INT,        { (void *)2 } },
#endif
};
#define dtsec2Num NELEMENTS(dtsec2Resources)

const struct hcfResource dtsec3Resources[] = {
    { "regBase",    HCF_RES_INT,        { (void *)(CCSBAR + 0x4E6000) } },
    { "phyAddr",    HCF_RES_INT,        { (void *)0x01 } },
    { "miiIfName",  HCF_RES_STRING,     { (void *)"dtsecMdio" } },
    { "miiIfUnit",  HCF_RES_INT,        { (void *)0 } },
    { "dtsecNum",   HCF_RES_INT,        { (void *)3 } },
    { "fmanNum",    HCF_RES_INT,        { (void *)0 } },
    { "dtsecMedia", HCF_RES_INT,        { (void *)DTSEC_MEDIA_RGMII } },
#if defined (INCLUDE_AMP_CPU_03)
    { "coreNum",    HCF_RES_INT,        { (void *)3 } },
#endif
};
#define dtsec3Num NELEMENTS(dtsec3Resources)

const struct hcfResource dtsec4Resources[] = {
    { "regBase",    HCF_RES_INT,        { (void *)(CCSBAR + 0x4E8000) } },
    { "phyAddr",    HCF_RES_INT,        { (void *)0x00 } },
    { "miiIfName",  HCF_RES_STRING,     { (void *)"dtsecMdio" } },
    { "miiIfUnit",  HCF_RES_INT,        { (void *)0 } },
    { "dtsecMedia", HCF_RES_INT,        { (void *)DTSEC_MEDIA_RGMII } },
    { "dtsecNum",   HCF_RES_INT,        { (void *)4 } },
    { "fmanNum",    HCF_RES_INT,        { (void *)0 } },
#if defined (INCLUDE_AMP_CPU_00)
    { "coreNum",    HCF_RES_INT,        { (void *)0 } },
#endif
};
#define dtsec4Num NELEMENTS(dtsec4Resources)

const struct hcfResource dtsecMdio0Resources[] = {
    { "regBase",    HCF_RES_INT,        { (void *)(CCSBAR + 0x4E1000) } },
#   ifdef INCLUDE_AMP
    { "sharedMem",  HCF_RES_INT,        { (void *)TM_ANCHOR_ADRS } }
#   endif /* INCLUDE_AMP */
};
#define dtsecMdio0Num NELEMENTS(dtsecMdio0Resources)

#ifdef DRV_VXBEND_TGEC

/*
 * Frame manager also include a 10 gigabit ethernet MAC (TGEC).
 * Use of the TGEC with the Freescale P2041RDB board requires the
 * purchase of a separate XAUI media expansion riser card. The 
 * target is configured for the riser card to be in slot 2 by 
 * default.
 */

const struct hcfResource tgec0Resources[] = {
    { "regBase",    HCF_RES_INT,        { (void *)(CCSBAR + 0x4F0000) } },
    { "phyAddr",    HCF_RES_INT,        { (void *)0 } },
    { "fmanNum",    HCF_RES_INT,        { (void *)0 } },
#ifdef TGEC_FIBER
    { "fiberMedia", HCF_RES_INT,        { (void *)TRUE } }
#endif /* TGEC_FIBER */
#   ifdef INCLUDE_AMP
    { "coreNum",    HCF_RES_INT,        { (void *)0 } },
#   endif /* INCLUDE_AMP */
};
#define tgec0Num NELEMENTS(tgec0Resources)

const struct hcfResource tgecMdio0Resources[] = {
    { "regBase",    HCF_RES_INT,        { (void *)(CCSBAR + 0x4F1000) } },
};
#define tgecMdio0Num NELEMENTS(tgecMdio0Resources)

#endif /* DRV_VXBEND_TGEC */

#ifdef DRV_PCIBUS_QORIQ_PCIEX

#   if (QORIQ_PCIEX1_ENABLE == TRUE)
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
    { "cacheSize",      HCF_RES_INT,    { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",   HCF_RES_INT,    { (void *)PCI_LAT_TIMER } },
    { "maxBusSet",      HCF_RES_INT,    { (void *)16 } },
    { "lawBase",        HCF_RES_ADDR,   { (void *)PCIEX1_LAW_BASE } },
    { "lawSize",        HCF_RES_INT,    { (void *)PCIEX1_LAW_SIZE } },
    { "includeFuncSet",     HCF_RES_ADDR, { (void *)sysPci1AutoconfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR, { (void *)sysPci1AutoconfigIntrAssign } },
#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "msiEnable",      HCF_RES_INT,    { (void *)TRUE } }
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};

#define pci0Num NELEMENTS(pci0Resources)
#   endif /* QORIQ_PCIEX1_ENABLE */

#   if (QORIQ_PCIEX2_ENABLE == TRUE)
const struct hcfResource pci1Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x201000) } },
    { "mem32Addr",      HCF_RES_ADDR,   { (void *)PCIEX2_MEM_ADRS } },
    { "mem32Size",      HCF_RES_INT,    { (void *)PCIEX2_MEM_SIZE } },
    { "memIo32Addr",    HCF_RES_ADDR,   { (void *)PCIEX2_MEMIO_ADRS } },
    { "memIo32Size",    HCF_RES_INT,    { (void *)PCIEX2_MEMIO_SIZE } },
    { "io32Addr",       HCF_RES_ADDR,   { (void *)PCIEX2_IO32_ADRS } },
    { "io32Size",       HCF_RES_INT,    { (void *)PCIEX2_IO32_SIZE } },
    { "io16Addr",       HCF_RES_ADDR,   { (void *)PCIEX2_IO_ADRS } },
    { "io16Size",       HCF_RES_INT,    { (void *)PCIEX2_IO_SIZE } },
    { "cacheSize",      HCF_RES_INT,    { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",   HCF_RES_INT,    { (void *)PCI_LAT_TIMER } },
    { "maxBusSet",      HCF_RES_INT,    { (void *)16 } },
    { "lawBase",        HCF_RES_ADDR,   { (void *)PCIEX2_LAW_BASE } },
    { "lawSize",        HCF_RES_INT,    { (void *)PCIEX2_LAW_SIZE } },
    { "includeFuncSet",     HCF_RES_ADDR, { (void *)sysPci1AutoconfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR, { (void *)sysPci2AutoconfigIntrAssign } },
#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "msiEnable",      HCF_RES_INT,    { (void *)TRUE } }
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};

#define pci1Num NELEMENTS(pci1Resources)
#   endif /* QORIQ_PCIEX2_ENABLE */

#   if (QORIQ_PCIEX3_ENABLE == TRUE)
const struct hcfResource pci2Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x202000) } },
    { "mem32Addr",      HCF_RES_ADDR,   { (void *)PCIEX3_MEM_ADRS } },
    { "mem32Size",      HCF_RES_INT,    { (void *)PCIEX3_MEM_SIZE } },
    { "memIo32Addr",    HCF_RES_ADDR,   { (void *)PCIEX3_MEMIO_ADRS } },
    { "memIo32Size",    HCF_RES_INT,    { (void *)PCIEX3_MEMIO_SIZE } },
    { "io32Addr",       HCF_RES_ADDR,   { (void *)PCIEX3_IO32_ADRS } },
    { "io32Size",       HCF_RES_INT,    { (void *)PCIEX3_IO32_SIZE } },
    { "io16Addr",       HCF_RES_ADDR,   { (void *)PCIEX3_IO_ADRS } },
    { "io16Size",       HCF_RES_INT,    { (void *)PCIEX3_IO_SIZE } },
    { "cacheSize",      HCF_RES_INT,    { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",   HCF_RES_INT,    { (void *)PCI_LAT_TIMER } },
    { "maxBusSet",      HCF_RES_INT,    { (void *)16 } },
    { "lawBase",        HCF_RES_ADDR,   { (void *)PCIEX3_LAW_BASE } },
    { "lawSize",        HCF_RES_INT,    { (void *)PCIEX3_LAW_SIZE } },
    { "includeFuncSet",     HCF_RES_ADDR, { (void *)sysPci1AutoconfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR, { (void *)sysPci3AutoconfigIntrAssign } },
#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "msiEnable",      HCF_RES_INT,    { (void *)TRUE } }
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};

#define pci2Num NELEMENTS(pci2Resources)
#   endif /* QORIQ_PCIEX3_ENABLE */

#endif /* DRV_PCIBUS_QORIQ_PCIEX */

const struct hcfResource law0Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,    { (void *)(CCSBAR + 0xC00) } },
};
#define law0Num NELEMENTS(law0Resources)

struct hcfResource fman0Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,    { (void *)(CCSBAR + 0x400000) } },
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_00)
    { "globalInit",     HCF_RES_INT,    { (void *)1} },
    { "ucodeAddr",      HCF_RES_ADDR,   { (void *)NULL} }
#endif
};
#define fman0Num NELEMENTS(fman0Resources)

const struct hcfResource qman0Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,    { (void *)(CCSBAR + 0x318000) } },
    { "qmanLawBase",    HCF_RES_ADDR,   { (void *)QMAN_LAW_BASE} },
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_00)
    { "qmanMemBase",    HCF_RES_ADDR,   { (void *)QMAN_MEM_BASE} },
#endif
};
#define qman0Num NELEMENTS(qman0Resources)

#ifdef DRV_RESOURCE_QORIQBMAN 
const struct hcfResource bman0Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,    { (void *)(CCSBAR + 0x31A000) } },
    { "bmanLawBase",    HCF_RES_ADDR,   { (void *)BMAN_LAW_BASE} },
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_00)
    { "bmanMemBase",    HCF_RES_ADDR,   { (void *)BMAN_MEM_BASE} },
#endif
};
#define bman0Num NELEMENTS(bman0Resources)
#endif

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

const struct hcfResource ns1655x1Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x11c500)} },
    { "irq",            HCF_RES_INT,    { (void *)EPIC_DUART_INT_VEC} },
    { "regInterval",    HCF_RES_INT,    { (void *)1} },
    { "irqLevel",       HCF_RES_INT,    { (void *)EPIC_DUART_INT_VEC} },
    { "clkFreq",        HCF_RES_ADDR,   { (void *)&sysPlbClkFreqGet} }
};

#define ns1655x1Num NELEMENTS(ns1655x1Resources)

const struct hcfResource ns1655x2Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x11c600)} },
    { "irq",            HCF_RES_INT,    { (void *)EPIC_DUART_INT_VEC} },
    { "regInterval",    HCF_RES_INT,    { (void *)1} },
    { "irqLevel",       HCF_RES_INT,    { (void *)EPIC_DUART_INT_VEC} },
    { "clkFreq",        HCF_RES_ADDR,   { (void *)&sysPlbClkFreqGet} }
};
#define ns1655x2Num NELEMENTS(ns1655x2Resources)

const struct hcfResource ns1655x3Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x11d500)} },
    { "irq",            HCF_RES_INT,    { (void *)EPIC_DUART2_INT_VEC} },
    { "regInterval",    HCF_RES_INT,    { (void *)1} },
    { "irqLevel",       HCF_RES_INT,    { (void *)EPIC_DUART2_INT_VEC} },
    { "clkFreq",        HCF_RES_ADDR,   { (void *)&sysPlbClkFreqGet} }
};
#define ns1655x3Num NELEMENTS(ns1655x3Resources)

const struct hcfResource ns1655x4Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x11d600)} },
    { "irq",            HCF_RES_INT,    { (void *)EPIC_DUART2_INT_VEC} },
    { "regInterval",    HCF_RES_INT,    { (void *)1} },
    { "irqLevel",       HCF_RES_INT,    { (void *)EPIC_DUART2_INT_VEC} },
    { "clkFreq",        HCF_RES_ADDR,   { (void *)&sysPlbClkFreqGet} }
};
#define ns1655x4Num NELEMENTS(ns1655x4Resources)

struct intrCtlrInputs epicInputs[] = {
    { EPIC_DUART_INT_VEC,           "ns16550",   0, 0 },
    { EPIC_DUART_INT_VEC,           "ns16550",   1, 0 },
    { EPIC_DUART2_INT_VEC,          "ns16550",   2, 0 },
    { EPIC_DUART2_INT_VEC,          "ns16550",   3, 0 },
    { EPIC_QMAN_PORTAL0_INT_VEC,    "QorIQQman", 0, 0 },
    { EPIC_QMAN_PORTAL1_INT_VEC,    "QorIQQman", 0, 1 },
    { EPIC_QMAN_PORTAL2_INT_VEC,    "QorIQQman", 0, 2 },
    { EPIC_QMAN_PORTAL3_INT_VEC,    "QorIQQman", 0, 3 },
    { EPIC_QMAN_PORTAL4_INT_VEC,    "QorIQQman", 0, 4 },
    { EPIC_QMAN_PORTAL5_INT_VEC,    "QorIQQman", 0, 5 },
    { EPIC_QMAN_PORTAL6_INT_VEC,    "QorIQQman", 0, 6 },
    { EPIC_QMAN_PORTAL7_INT_VEC,    "QorIQQman", 0, 7 },
    { EPIC_QMAN_PORTAL8_INT_VEC,    "QorIQQman", 0, 8 },
    { EPIC_QMAN_PORTAL9_INT_VEC,    "QorIQQman", 0, 9 },
    { EPIC_BMAN_PORTAL0_INT_VEC,    "QorIQBman", 0, 0 },
    { EPIC_BMAN_PORTAL1_INT_VEC,    "QorIQBman", 0, 1 },
    { EPIC_BMAN_PORTAL2_INT_VEC,    "QorIQBman", 0, 2 },
    { EPIC_BMAN_PORTAL3_INT_VEC,    "QorIQBman", 0, 3 },
    { EPIC_BMAN_PORTAL4_INT_VEC,    "QorIQBman", 0, 4 },
    { EPIC_BMAN_PORTAL5_INT_VEC,    "QorIQBman", 0, 5 },
    { EPIC_BMAN_PORTAL6_INT_VEC,    "QorIQBman", 0, 6 },
    { EPIC_BMAN_PORTAL7_INT_VEC,    "QorIQBman", 0, 7 },
    { EPIC_BMAN_PORTAL8_INT_VEC,    "QorIQBman", 0, 8 },
    { EPIC_BMAN_PORTAL9_INT_VEC,    "QorIQBman", 0, 9 },
    { GPIO_INT_VEC,                 "fslGpio",   0, 0 },
#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    { OPENPIC_TIMERA0_INT_VEC,      "openPicTimer", 0, 0 },
    { OPENPIC_TIMERA0_INT_VEC+1,    "openPicTimer", 0, 1 },
    { OPENPIC_TIMERA0_INT_VEC+2,    "openPicTimer", 0, 2 },
    { OPENPIC_TIMERA0_INT_VEC+3,    "openPicTimer", 0, 3 },
    { OPENPIC_TIMERB0_INT_VEC,      "openPicTimer", 1, 0 },
    { OPENPIC_TIMERB0_INT_VEC+1,    "openPicTimer", 1, 1 },
    { OPENPIC_TIMERB0_INT_VEC+2,    "openPicTimer", 1, 2 },
    { OPENPIC_TIMERB0_INT_VEC+3,    "openPicTimer", 1, 3 },
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */

#ifdef DRV_STORAGE_SDHC
    { EPIC_SDHC_INT_VEC,            "sdhci", 0, 0 },
#endif  /* DRV_STORAGE_SDHC */

#ifdef DRV_STORAGE_FSLSATA
    { EPIC_SATA1_INT_VEC,           "fslSata", 0, 0 },
    { EPIC_SATA2_INT_VEC,           "fslSata", 1, 0 },
#endif  /* DRV_STORAGE_FSLSATA */

#ifdef INCLUDE_EHCI
    { EPIC_USB1_INT_VEC,            "vxbPlbUsbEhci", 0, 0 },
    { EPIC_USB2_INT_VEC,            "vxbPlbUsbEhci", 1, 0 },
#endif  /* INCLUDE_EHCI */

#ifdef  DRV_DMA_FSL
    { EPIC_DMA1_CHAN0_INT_VEC,      "fslDma", 0, 0 },
    { EPIC_DMA1_CHAN1_INT_VEC,      "fslDma", 0, 1 },
    { EPIC_DMA1_CHAN2_INT_VEC,      "fslDma", 0, 2 },
    { EPIC_DMA1_CHAN3_INT_VEC,      "fslDma", 0, 3 },
    { EPIC_DMA2_CHAN0_INT_VEC,      "fslDma", 1, 0 },
    { EPIC_DMA2_CHAN1_INT_VEC,      "fslDma", 1, 1 },
    { EPIC_DMA2_CHAN2_INT_VEC,      "fslDma", 1, 2 },
    { EPIC_DMA2_CHAN3_INT_VEC,      "fslDma", 1, 3 },
#endif  /* DRV_DMA_FSL */

#ifdef DRV_I2CBUS_FSL
    { EPIC_I2C_INT_VEC,             "fslI2c", 0, 0 },
    { EPIC_I2C_INT_VEC,             "fslI2c", 1, 0 },
    { EPIC_I2C2_INT_VEC,            "fslI2c", 2, 0 },
    { EPIC_I2C2_INT_VEC,            "fslI2c", 3, 0 },
#endif  /* DRV_I2CBUS_FSL */


#ifdef _WRS_CONFIG_SMP
    { EPIC_VEC_IPI_IRQ0,            "ipi", 0, 0 }
#else
#endif /* _WRS_CONFIG_SMP */

};

struct intrCtlrXBar epicXBar[] = {
    { 0, 0 }
};

struct intrCtlrPriority epicPriority[] = {
    { EPIC_DUART2_INT_VEC, 100 },
    { EPIC_DUART_INT_VEC,  100 }
};

struct intrCtlrCpu epicCpu[] = {
    { EPIC_DUART_INT_VEC,  0 }
};

INT_BANK_DESC exDesc = { 0, 12, 0x50000, 0x20, EPIC_EX_INTERRUPT };
INT_BANK_DESC inDesc = { EPIC_VEC_IN_IRQ0, 128, 0x50200, 0x20, EPIC_IN_INTERRUPT };
INT_BANK_DESC msgDesc = { 176, 8, 0x51600, 0x20, EPIC_MSG_INTERRUPT };
INT_BANK_DESC gtaDesc = { OPENPIC_TIMERA0_INT_VEC, 4, 0x41120, 0x40, EPIC_GT_A_INTERRUPT };
INT_BANK_DESC gtbDesc = { OPENPIC_TIMERB0_INT_VEC, 4, 0x42120, 0x40, EPIC_GT_B_INTERRUPT };
INT_BANK_DESC smsgDesc = { 243, 8, 0x51c00, 0x20, EPIC_SMSG_INTERRUPT };
INT_BANK_DESC ipiDesc = { 251, 4, 0x410a0, 0x10, EPIC_IPI_INTERRUPT };

const struct hcfResource epic0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,  { (void *)CCSBAR } },
    { "input",              HCF_RES_ADDR, { (void *)&epicInputs[0] } },
    { "inputTableSize",     HCF_RES_INT,  { (void *)NELEMENTS(epicInputs) } },
    { "priority",           HCF_RES_ADDR, { (void *)&epicPriority[0] } },
    { "priorityTableSize",  HCF_RES_INT,  { (void *)NELEMENTS(epicPriority) } },
    { "crossBar",           HCF_RES_ADDR, { (void *)&epicXBar[0] } },
    { "crossBarTableSize",  HCF_RES_INT,  { (void *)NELEMENTS(epicXBar) } },
    { "cpuRoute",           HCF_RES_ADDR, { (void *)&epicCpu[0] } },
    { "cpuRouteTableSize",  HCF_RES_INT,  { (void *)NELEMENTS(epicCpu) } },
    { "exPolar",            HCF_RES_INT,  { (void *)EPIC_INT_ACT_LOW } },
    { "numInts",            HCF_RES_INT,  { (void *)255 } },
    { "intDesc_0",          HCF_RES_ADDR, { (void *)&exDesc } },
    { "intDesc_1",          HCF_RES_ADDR, { (void *)&inDesc } },
    { "intDesc_2",          HCF_RES_ADDR, { (void *)&msgDesc } },
    { "intDesc_3",          HCF_RES_ADDR, { (void *)&gtaDesc } },
    { "intDesc_4",          HCF_RES_ADDR, { (void *)&gtbDesc } },
    { "intDesc_5",          HCF_RES_ADDR, { (void *)&smsgDesc } },
    { "intDesc_6",          HCF_RES_ADDR, { (void *)&ipiDesc } },
#ifdef EPIC_EXTERNAL_PROXY
    { "sysEprGet",          HCF_RES_ADDR, { (void *)&sysEprGet } },
#endif
};
#define epic0Num NELEMENTS(epic0Resources)

#ifdef DRV_TIMER_M85XX
struct hcfResource m85xxTimerResources[] =  {
    {"regBase",         HCF_RES_INT,    { (void *)0}},
    {"decMinClkRate",   HCF_RES_INT,    { (void *)SYS_CLK_RATE_MIN}},
    {"decMaxClkRate",   HCF_RES_INT,    { (void *)SYS_CLK_RATE_MAX}},
    {"fitMinClkRate",   HCF_RES_INT,    { (void *)FIT_CLK_RATE_MIN}},
    {"fitMaxClkRate",   HCF_RES_INT,    { (void *)FIT_CLK_RATE_MAX}},
    {"sysClockFreq",    HCF_RES_ADDR,   { (void *)&sysPpcTbClkFreqGet}}
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
    { VXB_REG_BASE,     HCF_RES_INT,    { (void *)EHCI_CAPLENGTH(USB1_BASE) } },
    { "ehciInit",       HCF_RES_ADDR,   { (void *)ehci0Init } },
};
#define usbEhci0Num NELEMENTS(usbEhci0Resources)

const struct hcfResource usbEhci1Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,    { (void *)EHCI_CAPLENGTH(USB2_BASE) } },
    { "ehciInit",       HCF_RES_ADDR,   { (void *)ehci1Init } },
};
#define usbEhci1Num NELEMENTS(usbEhci1Resources)

#endif /* INCLUDE_EHCI */

#ifdef DRV_TIMER_OPENPIC
const struct hcfResource openPicTimerDevAresources[] =
    {
    {VXB_REG_BASE,      HCF_RES_INT,    { (void *)(CCSBAR + 0x000410F0)}},
    {"instanceName",    HCF_RES_STRING, { (void *)"A" }},
    {VXB_CLK_FREQ,      HCF_RES_ADDR,   { (void *)sysPicClkFreqGet}},
    {"clkDivisor",      HCF_RES_INT,    { (void *)8}},
#ifdef INCLUDE_AUX_CLK
    {"maxClkRate0",     HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX }},
    {"maxClkRate1",     HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX }},
    {"maxClkRate2",     HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX }},
    {"maxClkRate3",     HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX }},
#endif /* INCLUDE_AUX_CLK */
#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    {"vector0",         HCF_RES_INT,    { (void *)OPENPIC_TIMERA0_INT_VEC }},
    {"vector1",         HCF_RES_INT,    { (void *)OPENPIC_TIMERA0_INT_VEC+1 }},
    {"vector2",         HCF_RES_INT,    { (void *)OPENPIC_TIMERA0_INT_VEC+2 }},
    {"vector3",         HCF_RES_INT,    { (void *)OPENPIC_TIMERA0_INT_VEC+3 }},
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
    };
#define openPicTimerDevAnum NELEMENTS(openPicTimerDevAresources)

const struct hcfResource openPicTimerDevBresources[] =
    {
    {VXB_REG_BASE,      HCF_RES_INT,    { (void *)(CCSBAR + 0x000420F0)}},
    {"instanceName",    HCF_RES_STRING, { (void *)"B" }},
    {VXB_CLK_FREQ,      HCF_RES_ADDR,   { (void *)sysPicClkFreqGet}},
    {"clkDivisor",      HCF_RES_INT,    { (void *)8}},
#ifdef INCLUDE_AUX_CLK
    {"maxClkRate0",     HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX }},
    {"maxClkRate1",     HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX }},
    {"maxClkRate2",     HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX }},
    {"maxClkRate3",     HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX }},
#endif /* INCLUDE_AUX_CLK */
#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB  
    {"vector0",         HCF_RES_INT,    { (void *)OPENPIC_TIMERB0_INT_VEC }},
    {"vector1",         HCF_RES_INT,    { (void *)OPENPIC_TIMERB0_INT_VEC+1 }},
    {"vector2",         HCF_RES_INT,    { (void *)OPENPIC_TIMERB0_INT_VEC+2 }},
    {"vector3",         HCF_RES_INT,    { (void *)OPENPIC_TIMERB0_INT_VEC+3 }},
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
    };
#define openPicTimerDevBnum NELEMENTS(openPicTimerDevBresources)
#endif /* DRV_TIMER_OPENPIC */

#ifdef DRV_STORAGE_SDHC
struct hcfResource fslSdhcResources[] =  {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x114000) } },
    { "clkFreq",        HCF_RES_ADDR,   { (void *)sysPlbClkFreqGet } },
    { "dmaMode",        HCF_RES_INT,    { (void *)0 } },
    { "polling",        HCF_RES_INT,    { (void *)0 } },
    { "flags" ,         HCF_RES_INT,    { (void *)(SDHC_PIO_NEED_DELAY | SDHC_HW_SNOOP |
                                                  SDHC_MISS_CARD_INS_INT_WHEN_INIT |
                                                  SDHC_FIFO_ENDIANESS_REVERSE |
                                                  SDHC_HOST_VER_REVERSE |
                                                  SDHC_HOST_CTRL_FREESCALE) } },
};
#   define fslSdhcNum  NELEMENTS(fslSdhcResources)
#endif  /* DRV_STORAGE_SDHC */

#ifdef DRV_STORAGE_FSLSATA
const struct hcfResource fslSata0Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *) (CCSBAR + 0x220000) } },
};
#   define fslSata0Num NELEMENTS(fslSata0Resources)

const struct hcfResource fslSata1Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *) (CCSBAR + 0x221000) } },
};
#   define fslSata1Num NELEMENTS(fslSata1Resources)
#endif /* DRV_STORAGE_FSLSATA */

#ifdef DRV_I2CBUS_FSL

struct i2cDevInputs i2c0DevTbl[] = {

/* 
 * Address  Part Number  Size     Bus    Description
 * -------- --------------------------------------------------
 * 0x50       M24256     256K-bit  1    Boot sequencer eeprom.
 * 0x52       24C02        2K-bit  1    Memory module-SPD.
 * 0x68       PT7C4338             1    Real time clock.
 * 0x50       M24256     256K-bit  2    eeprom.
 */

    { "eeprom_M24256",   0x50,     I2C_WORDADDR | I2C_RD_EXT_ACK},
    { "eeprom_at24c02",  0x52,     I2C_RDONLY},
    { "rtc_pt7c4338",    0x68,     0},
};

const struct hcfResource i2c0Resources[] = {
    { "regBase",    HCF_RES_ADDR,   { (void *)(I2C_BUS1_BASE) } },
    { "clkFreq",    HCF_RES_ADDR,   { (void *)sysPlbClkFreqGet } },
    { "busSpeed",   HCF_RES_INT,    { (void *)400000 } },
    { "polling",    HCF_RES_INT,    { (void *)0 } },
    { "i2cDev",     HCF_RES_ADDR,   { (void *)&i2c0DevTbl} },
    { "i2cDevNum",  HCF_RES_INT,    { (void *)NELEMENTS(i2c0DevTbl)}},
};
#   define i2c0Num NELEMENTS(i2c0Resources)

struct i2cDevInputs i2c1DevTbl[] = { 
    { "eeprom_M24256",   0x50,     I2C_WORDADDR | I2C_RD_EXT_ACK},
};

const struct hcfResource i2c1Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)(I2C_BUS2_BASE) } },
    { "clkFreq",    HCF_RES_ADDR,   { (void *)sysPlbClkFreqGet } },
    { "busSpeed",   HCF_RES_INT,    { (void *)400000 } },
    { "polling",    HCF_RES_INT,    { (void *)0 } },
    { "i2cDev",     HCF_RES_ADDR,   { (void *)&i2c1DevTbl} },
    { "i2cDevNum",  HCF_RES_INT,    { (void *)NELEMENTS(i2c1DevTbl)}},
};
#   define i2c1Num NELEMENTS(i2c1Resources)

#endif /* DRV_I2CBUS_FSL */

const struct hcfDevice hcfDeviceList[] = {
    /*
     * Initialize ppcIntCtlr before epic.
     * The vector table for external interrupts are over written by epic
     * for an optimized purpose.
     */

    { "ppcIntCtlr", 0, VXB_BUSID_PLB, 0, ppcIntCtlr0Num, ppcIntCtlr0Resources },
    { "epic",       0, VXB_BUSID_PLB, 0, epic0Num, epic0Resources },
#ifdef DRV_SIO_NS16550
    { "ns16550",    0, VXB_BUSID_PLB, 0, ns1655x1Num, ns1655x1Resources },
    { "ns16550",    1, VXB_BUSID_PLB, 0, ns1655x2Num, ns1655x2Resources },
    { "ns16550",    2, VXB_BUSID_PLB, 0, ns1655x3Num, ns1655x3Resources },
    { "ns16550",    3, VXB_BUSID_PLB, 0, ns1655x4Num, ns1655x4Resources },
#endif /* DRV_SIO_NS16550 */

#ifdef DRV_RESOURCE_FSLGPIO
    { "fslGpio",    0, VXB_BUSID_PLB,0, gpio0Num, gpio0Resources },    
#endif

#ifdef DRV_RESOURCE_QORIQLAW
    { "QorIQLaw",   0, VXB_BUSID_PLB,0, law0Num, law0Resources },    
#endif

#ifdef DRV_RESOURCE_QORIQBMAN
    { "QorIQBman",  0, VXB_BUSID_PLB,0, bman0Num, bman0Resources },    
#endif

#ifdef DRV_RESOURCE_QORIQQMAN
    { "QorIQQman",  0, VXB_BUSID_PLB,0, qman0Num, qman0Resources },    
#endif

#ifdef DRV_RESOURCE_QORIQFMAN
    { "QorIQFman",  0, VXB_BUSID_PLB,0, fman0Num, fman0Resources },    
#endif

/*
 * Keep PCIe briges on AMP instance 0 by default. They can
 * be assigined to individual codes if needed.
 */

#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_00)

#ifdef DRV_PCIBUS_QORIQ_PCIEX
#   if (QORIQ_PCIEX1_ENABLE == TRUE)
    { "QorIQPciEx", QORIQ_PCIEX1_UNIT_NUM,  VXB_BUSID_PLB,  0,  pci0Num,    pci0Resources },
#   endif /* QORIQ_PCIEX1_ENABLE */

#   if (QORIQ_PCIEX2_ENABLE == TRUE)
    { "QorIQPciEx", QORIQ_PCIEX2_UNIT_NUM,  VXB_BUSID_PLB,  0,  pci1Num,    pci1Resources },
#   endif /* QORIQ_PCIEX2_ENABLE */

#   if (QORIQ_PCIEX3_ENABLE == TRUE)
    { "QorIQPciEx", QORIQ_PCIEX3_UNIT_NUM,  VXB_BUSID_PLB,  0,  pci2Num,    pci2Resources },
#   endif /* QORIQ_PCIEX3_ENABLE */
#endif /* DRV_PCIBUS_QORIQ_PCIEX */
#endif /* DRV_PCIBUS_QORIQ_PCIEX */

#ifdef DRV_VXBEND_DTSEC
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_00)
    { "dtsec",      0, VXB_BUSID_PLB, 0, dtsec0Num, dtsec0Resources },
#endif
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_01)
    { "dtsec",      1, VXB_BUSID_PLB, 0, dtsec1Num, dtsec1Resources },
#endif
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_02)
    { "dtsec",      2, VXB_BUSID_PLB, 0, dtsec2Num, dtsec2Resources },
#endif
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_03)
    { "dtsec",      3, VXB_BUSID_PLB, 0, dtsec3Num, dtsec3Resources },
#endif
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_00)
    { "dtsec",      4, VXB_BUSID_PLB, 0, dtsec4Num, dtsec4Resources },
#endif
#endif /* DRV_VXBEND_DTSEC */

#ifdef DRV_MII_DTSEC_MDIO
    { "dtsecMdio",  0, VXB_BUSID_PLB, 0, dtsecMdio0Num, dtsecMdio0Resources },    
#endif

#ifdef DRV_VXBEND_TGEC
    { "tgec",       0, VXB_BUSID_PLB, 0, tgec0Num, tgec0Resources },
#endif

#ifdef DRV_MII_TGEC_MDIO
    { "tgecMdio",   0, VXB_BUSID_PLB, 0, tgecMdio0Num, tgecMdio0Resources },    
#endif

#ifdef DRV_TIMER_M85XX
    {"m85xxTimerDev", 0, VXB_BUSID_PLB, 0, m85xxTimerNum, m85xxTimerResources},
#endif /* DRV_TIMER_M85XX */

#ifdef DRV_TIMER_OPENPIC
    { "openPicTimer", 0, VXB_BUSID_PLB, 0, openPicTimerDevAnum, openPicTimerDevAresources },
    { "openPicTimer", 1, VXB_BUSID_PLB, 0, openPicTimerDevBnum, openPicTimerDevBresources },
#endif /* DRV_TIMER_OPENPIC */

#ifdef DRV_STORAGE_SDHC
    { "sdhci",      0, VXB_BUSID_PLB, 0, fslSdhcNum, fslSdhcResources },
#endif  /* DRV_STORAGE_SDHC */

#ifdef DRV_I2CBUS_FSL
    { "fslI2c",     0, VXB_BUSID_PLB,   0,  i2c0Num,        i2c0Resources },
    { "fslI2c",     1, VXB_BUSID_PLB,   0,  i2c1Num,        i2c1Resources },
#endif /* DRV_I2CBUS_FSL */

#ifdef DRV_STORAGE_FSLSATA
    { "fslSata",    0, VXB_BUSID_PLB, 0, fslSata0Num, fslSata0Resources },
    { "fslSata",    1, VXB_BUSID_PLB, 0, fslSata1Num, fslSata1Resources }, 
#endif /* DRV_STORAGE_FSLSATA */

#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci", 0, VXB_BUSID_PLB, 0, usbEhci0Num, usbEhci0Resources },
    { "vxbPlbUsbEhci", 1, VXB_BUSID_PLB, 0, usbEhci1Num, usbEhci1Resources },
#endif  /* INCLUDE_EHCI */

#ifdef DRV_DMA_FSL
    { "fslDma",     0, VXB_BUSID_PLB, 0, fslDma1Num, fslDma1Resources },
    { "fslDma",     1, VXB_BUSID_PLB, 0, fslDma2Num, fslDma2Resources },
#endif /* DRV_DMA_FSL */
};
const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] =
    {
#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci", 0, "hasEmbeddedTT", VXB_PARAM_INT32,   { (void *)TRUE } },
    { "vxbPlbUsbEhci", 0, "postResetHook", VXB_PARAM_FUNCPTR, { (void *)ehci0PostResetHook } },
    { "vxbPlbUsbEhci", 1, "hasEmbeddedTT", VXB_PARAM_INT32,   { (void *)TRUE } },
    { "vxbPlbUsbEhci", 1, "postResetHook", VXB_PARAM_FUNCPTR, { (void *)ehci1PostResetHook } },
#endif  /* INCLUDE_EHCI */

    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, { (void *)0} }
    };
