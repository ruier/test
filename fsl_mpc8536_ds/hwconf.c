/* hwconf.c - hardware configuration support module */

/*
 * Copyright (c) 2008-2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,10mar11,my_  rename DRV_STORAGE_FSL_SDHC to DRV_STORAGE_SDHC.
01c,05jun10,sye  Add support to eSDHC.
01b,10feb09,b_m  update openPicTimer support.
01a,20oct08,b_m  written.
*/

/* includes */

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#include <hwif/vxbus/vxbIntrCtlr.h>
#include <hwif/intCtlr/vxbEpicIntCtlr.h>
#include <drv/pci/pciAutoConfigLib.h>

#ifdef DRV_STORAGE_SDHC
#include <h/storage/vxbSdMmcLib.h>
#include <h/storage/vxbSdhcStorage.h>
#endif /* DRV_STORAGE_SDHC */

#include "config.h"

/* externs */

IMPORT STATUS sysPciEx1AutoConfigInclude (PCI_SYSTEM *, PCI_LOC *, UINT);
IMPORT UCHAR  sysPciEx1AutoConfigIntAssign (PCI_SYSTEM *, PCI_LOC *, UCHAR);
IMPORT STATUS sysPciEx2AutoConfigInclude (PCI_SYSTEM *, PCI_LOC *, UINT);
IMPORT UCHAR  sysPciEx2AutoConfigIntAssign (PCI_SYSTEM *, PCI_LOC *, UCHAR);
IMPORT STATUS sysPciEx3AutoConfigInclude (PCI_SYSTEM *, PCI_LOC *, UINT);
IMPORT UCHAR  sysPciEx3AutoConfigIntAssign (PCI_SYSTEM *, PCI_LOC *, UCHAR);
IMPORT STATUS sysPciAutoConfigInclude (PCI_SYSTEM *, PCI_LOC *, UINT);
IMPORT UCHAR  sysPciAutoConfigIntAssign (PCI_SYSTEM *, PCI_LOC *, UCHAR);
IMPORT UINT32 sysOpenPicTimerClkFreqGet (UINT32);

IMPORT void   ehci0Init (void);
IMPORT void   ehci1Init (void);
IMPORT void   ehci0PostResetHook (void);
IMPORT void   ehci1PostResetHook (void);

/* hardware resources */

const struct intrCtlrInputs ppcIntCtlrInputs[] = {
    { 0, "epic",          0, 0 },
    { 1, "m85xxTimerDev", 0, 0 },
    { 3, "m85xxTimerDev", 0, 2 },
    { 4, "m85xxTimerDev", 0, 1 },
};

const struct hcfResource ppcIntCtlrResources[] = {
    { "regBase",        HCF_RES_INT,  { (void *)TRUE } },
    { "input",          HCF_RES_ADDR, { (void *)&ppcIntCtlrInputs[0] } },
    { "inputTableSize", HCF_RES_INT,  { (void *)NELEMENTS(ppcIntCtlrInputs) } },
};
#define ppcIntCtlrNum   NELEMENTS(ppcIntCtlrResources)

struct intrCtlrInputs epicInputs[] = {
    { INUM_DUART,      "ns16550",       0, 0 },
    { INUM_DUART,      "ns16550",       1, 0 },
#if defined(INCLUDE_ETSEC_VXB_END) || defined(INCLUDE_ETSEC_VXB_END2)
    { INUM_ETSEC1_TX,  "motetsec",      0, 0 },
    { INUM_ETSEC1_RX,  "motetsec",      0, 1 },
    { INUM_ETSEC1_ERR, "motetsec",      0, 2 },
    { INUM_ETSEC3_TX,  "motetsec",      1, 0 },
    { INUM_ETSEC3_RX,  "motetsec",      1, 1 },
    { INUM_ETSEC3_ERR, "motetsec",      1, 2 },
#endif
#ifdef DRV_STORAGE_FSLSATA
    { INUM_SATA1,      "fslSata",       0, 0 },
    { INUM_SATA2,      "fslSata",       1, 0 },
#endif
#ifdef INCLUDE_EHCI
    { INUM_USB1,       "vxbPlbUsbEhci", 0, 0 },
    { INUM_USB2,       "vxbPlbUsbEhci", 1, 0 },
#endif
#ifdef DRV_STORAGE_SDHC
    { INUM_ESDHC,      "sdhci",         0, 0 },
#endif /* DRV_STORAGE_SDHC */
#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    { INUM_GT_TIMER_A0, "openPicTimer", 0, 0 },
    { INUM_GT_TIMER_A1, "openPicTimer", 0, 1 },
    { INUM_GT_TIMER_A2, "openPicTimer", 0, 2 },
    { INUM_GT_TIMER_A3, "openPicTimer", 0, 3 },
    { INUM_GT_TIMER_B0, "openPicTimer", 1, 0 },
    { INUM_GT_TIMER_B1, "openPicTimer", 1, 1 },
    { INUM_GT_TIMER_B2, "openPicTimer", 1, 2 },
    { INUM_GT_TIMER_B3, "openPicTimer", 1, 3 },
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};

INT_BANK_DESC epicIntDesc[] = {
    { 0,   EPIC_EX_IRQ_NUM,   EPIC_EX_REG_BASE,   EPIC_EX_REG_INTERVAL,   EPIC_EX_INTERRUPT   },
    { 0,   EPIC_IN_IRQ_NUM,   EPIC_IN_REG_BASE,   EPIC_IN_REG_INTERVAL,   EPIC_IN_INTERRUPT   },
    { 0,   EPIC_GT_IRQ_NUM,   EPIC_GT_A_REG_BASE, EPIC_GT_REG_INTERVAL,   EPIC_GT_A_INTERRUPT },
    { 0,   EPIC_GT_IRQ_NUM,   EPIC_GT_B_REG_BASE, EPIC_GT_REG_INTERVAL,   EPIC_GT_B_INTERRUPT },
    { 0,   EPIC_MSG_IRQ_NUM,  EPIC_MSG_REG_BASE,  EPIC_MSG_REG_INTERVAL,  EPIC_MSG_INTERRUPT  },
    { 0,   EPIC_SMSG_IRQ_NUM, EPIC_SMSG_REG_BASE, EPIC_SMSG_REG_INTERVAL, EPIC_SMSG_INTERRUPT },
    { 251, EPIC_IPI_IRQ_NUM,  EPIC_IPI_REG_BASE,  EPIC_IPI_REG_INTERVAL,  EPIC_IPI_INTERRUPT  }
};

const struct hcfResource epicResources[] = {
    { "regBase",        HCF_RES_INT,  { (void *)CCSR_BASE } },
    { "input",          HCF_RES_ADDR, { (void *)&epicInputs[0] } },
    { "inputTableSize", HCF_RES_INT,  { (void *)NELEMENTS(epicInputs) } },
    { "numCpus",        HCF_RES_INT,  { (void *)1 } },
    { "exPolar",        HCF_RES_INT,  { (void *)EPIC_INT_ACT_LOW } },
    { "intDesc_0",      HCF_RES_ADDR, { (void *)&epicIntDesc[0] } },
    { "intDesc_1",      HCF_RES_ADDR, { (void *)&epicIntDesc[1] } },
    { "intDesc_2",      HCF_RES_ADDR, { (void *)&epicIntDesc[2] } },
    { "intDesc_3",      HCF_RES_ADDR, { (void *)&epicIntDesc[3] } },
    { "intDesc_4",      HCF_RES_ADDR, { (void *)&epicIntDesc[4] } },
    { "intDesc_5",      HCF_RES_ADDR, { (void *)&epicIntDesc[5] } },
    { "intDesc_6",      HCF_RES_ADDR, { (void *)&epicIntDesc[6] } },
};
#define epicNum NELEMENTS(epicResources)

struct hcfResource m85xxTimerResources[] = {
    { "regBase",       HCF_RES_INT,  { (void *)0 } },
    { "decMinClkRate", HCF_RES_INT,  { (void *)SYS_CLK_RATE_MIN } },
    { "decMaxClkRate", HCF_RES_INT,  { (void *)SYS_CLK_RATE_MAX } },
    { "sysClockFreq",  HCF_RES_ADDR, { (void *)sysCcbClkFreqGet } }
};
#define m85xxTimerNum   NELEMENTS(m85xxTimerResources)

const struct hcfResource ns1655x0Resources[] = {
    { "regBase",     HCF_RES_INT,  { (void *)UART0_BASE } },
    { "clkFreq",     HCF_RES_ADDR, { (void *)sysCcbClkFreqGet } },
    { "regInterval", HCF_RES_INT,  { (void *)1 } }
};
#define ns1655x0Num NELEMENTS(ns1655x0Resources)

const struct hcfResource ns1655x1Resources[] = {
    { "regBase",     HCF_RES_INT,  { (void *)UART1_BASE } },
    { "clkFreq",     HCF_RES_ADDR, { (void *)sysCcbClkFreqGet} },
    { "regInterval", HCF_RES_INT,  { (void *)1 } }
};
#define ns1655x1Num NELEMENTS(ns1655x1Resources)

/*
 * On the Freescale MPC8536DS board, PHYs are physically wired according to
 * the following table:
 *
 * Data port pins   Management port pins    MII address
 * --------------   --------------------    -----------
 * PHY0: ETSEC1     ETSEC1                  1
 * PHY1: ETSEC3     ETSEC1                  0
 *
 * The tricky part is that both the 2 PHYs have their management pins connected
 * to ETSEC1.  The VxBus ETSEC driver allows us to specify the name of the
 * device that provides PHY management support.  This device is "motetsec0"
 * in all the cases.  We also specify the "phyAddr" resource for each device,
 * which is 1 for ETSEC1, 0 for ETSEC3.
 */

#if defined(INCLUDE_ETSEC_VXB_END) || defined(INCLUDE_ETSEC_VXB_END2)

const struct hcfResource etsec0Resources[] = {
    { "regBase",   HCF_RES_INT,    { (void *)ETSEC1_BASE } },
    { "phyAddr",   HCF_RES_INT,    { (void *)1 } },
    { "miiIfName", HCF_RES_STRING, { (void *)"motetsec" } },
    { "miiIfUnit", HCF_RES_INT,    { (void *)0 } },};
#define etsec0Num   NELEMENTS(etsec0Resources)

const struct hcfResource etsec1Resources[] = {
    { "regBase",   HCF_RES_INT,    { (void *)ETSEC3_BASE } },
    { "phyAddr",   HCF_RES_INT,    { (void *)0 } },
    { "miiIfName", HCF_RES_STRING, { (void *)"motetsec" } },
    { "miiIfUnit", HCF_RES_INT,    { (void *)0 } },};
#define etsec1Num   NELEMENTS(etsec1Resources)

#endif /* defined(INCLUDE_ETSEC_VXB_END) || defined(INCLUDE_ETSEC_VXB_END2) */

#ifdef DRV_RESOURCE_M85XXCCSR

const struct hcfResource m85xxLawBarResources[] = {
    { "regBase",           HCF_RES_INT,    { (void *)CCSR_BASE } },
    { "LAWBAR0",           HCF_RES_STRING, { "reserved" } },
    { "LAWBAR1",           HCF_RES_STRING, { "reserved" } },
    { "LAWBAR2",           HCF_RES_STRING, { "reserved" } },
    { "LAWBAR3",           HCF_RES_STRING, { "reserved" } },
    { "LAWBAR4",           HCF_RES_STRING, { "m85xxPci" } },
    { "LAWBAR5",           HCF_RES_STRING, { "m85xxPci" } },
    { "LAWBAR6",           HCF_RES_STRING, { "m85xxPci" } },
    { "LAWBAR7",           HCF_RES_STRING, { "m85xxPci" } },
    { "LAWBAR8",           HCF_RES_STRING, { "reserved" } },
    { "LAWBAR9",           HCF_RES_STRING, { "reserved" } },
    { "LAWAR_ENABLE",      HCF_RES_INT,    { (void *)LAWAR_ENABLE } },
    { "LAWBAR_ADRS_SHIFT", HCF_RES_INT,    { (void *)LAWBAR_ADRS_SHIFT } },
};
#define m85xxLawBarNum  NELEMENTS(m85xxLawBarResources)

#endif /* DRV_RESOURCE_M85XXCCSR */

#ifdef DRV_PCIBUS_M85XX

const struct hcfResource m85xxPciEx0Resources[] = {
    { "regBase",            HCF_RES_INT,  { (void *)PCIEX1_BASE } },
    { "mem32Addr",          HCF_RES_ADDR, { (void *)PCIEX1_MEM_ADRS} },
    { "mem32Size",          HCF_RES_INT,  { (void *)PCI_MEM_SIZE} },
    { "memIo32Addr",        HCF_RES_ADDR, { (void *)PCIEX1_MEMIO_ADRS } },
    { "memIo32Size",        HCF_RES_INT,  { (void *)PCI_MEMIO_SIZE} },
    { "io32Addr",           HCF_RES_ADDR, { (void *)PCIEX1_IO_ADRS } },
    { "io32Size",           HCF_RES_INT,  { (void *)PCI_IO_SIZE} },
    { "io16Addr",           HCF_RES_ADDR, { (void *)PCIEX1_IO_ADRS } },
    { "io16Size",           HCF_RES_INT,  { (void *)PCI_IO_SIZE } },
    { "fbbEnable",          HCF_RES_INT,  { (void *)TRUE } },
    { "cacheSize",          HCF_RES_INT,  { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,  { (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet",    HCF_RES_INT,  { (void *)TRUE } },
    { "includeFuncSet",     HCF_RES_ADDR, { (void *)sysPciEx1AutoConfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR, { (void *)sysPciEx1AutoConfigIntAssign } },

    /* non-vxbPciAutoConfig() values */

    { "pimmrBase",          HCF_RES_ADDR, { (void *)PCI_BRIDGE_PIMMR_BASE_ADRS } },
    { "lclMemAddr",         HCF_RES_ADDR, { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",      HCF_RES_INT,  { (void *)LOCAL_MEM_SIZE } },
    { "mstrMemBus",         HCF_RES_ADDR, { (void *)PCI_MSTR_MEM_BUS } },
    { "maxBusSet",          HCF_RES_INT,  { (void *)16 } },

    /* LAWBAR details */

    { "tgtIf",              HCF_RES_INT,  { (void *)LAWAR_TGTIF_PCIEX1 } },

    /* Window Attributes */

    { "owAttrMem",          HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                     PCI_OUT_ATTR_RTT_MEM |
                                                     PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrMemIo",        HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                     PCI_OUT_ATTR_RTT_MEM |
                                                     PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrIo",           HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                     PCI_OUT_ATTR_RTT_IO |
                                                     PCI_OUT_ATTR_WTT_IO) } },
    { "iwAttr",             HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                     PCI_PREFETCHABLE |
                                                     PCI_IN_ATTR_RTT_LM_READ_SNOOP |
                                                     PCI_IN_ATTR_RTT_LM_WRITE_SNOOP |
                                                     PCI_IN_ATTR_TGI_LM) } },

    { "singleLawBar",       HCF_RES_INT,  { (void *)TRUE } },
    { "singleLawBarSize",   HCF_RES_INT,  { (void *)0x10000000 } },

    { "pciConfigMechanism", HCF_RES_INT,  { (void *)0 } },
    { "autoConfig",         HCF_RES_INT,  { (void *)TRUE } },
    { "pciExpressHost",     HCF_RES_INT,  { (void *)TRUE } },

#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "msiEnable",          HCF_RES_INT,  { (void *)TRUE } },
    { "dynamicInterrupts",  HCF_RES_INT,  { (void *)TRUE } },
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#define m85xxPciEx0Num  NELEMENTS(m85xxPciEx0Resources)

const struct hcfResource m85xxPciEx1Resources[] = {
    { "regBase",            HCF_RES_INT,  { (void *)PCIEX2_BASE } },
    { "mem32Addr",          HCF_RES_ADDR, { (void *)PCIEX2_MEM_ADRS} },
    { "mem32Size",          HCF_RES_INT,  { (void *)PCI_MEM_SIZE} },
    { "memIo32Addr",        HCF_RES_ADDR, { (void *)PCIEX2_MEMIO_ADRS } },
    { "memIo32Size",        HCF_RES_INT,  { (void *)PCI_MEMIO_SIZE} },
    { "io32Addr",           HCF_RES_ADDR, { (void *)PCIEX2_IO_ADRS } },
    { "io32Size",           HCF_RES_INT,  { (void *)PCI_IO_SIZE} },
    { "io16Addr",           HCF_RES_ADDR, { (void *)PCIEX2_IO_ADRS } },
    { "io16Size",           HCF_RES_INT,  { (void *)PCI_IO_SIZE } },
    { "fbbEnable",          HCF_RES_INT,  { (void *)TRUE } },
    { "cacheSize",          HCF_RES_INT,  { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,  { (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet",    HCF_RES_INT,  { (void *)TRUE } },
    { "includeFuncSet",     HCF_RES_ADDR, { (void *)sysPciEx2AutoConfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR, { (void *)sysPciEx2AutoConfigIntAssign } },

    /* non-vxbPciAutoConfig() values */

    { "pimmrBase",          HCF_RES_ADDR, { (void *)PCI_BRIDGE_PIMMR_BASE_ADRS } },
    { "lclMemAddr",         HCF_RES_ADDR, { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",      HCF_RES_INT,  { (void *)LOCAL_MEM_SIZE } },
    { "mstrMemBus",         HCF_RES_ADDR, { (void *)PCI_MSTR_MEM_BUS } },
    { "maxBusSet",          HCF_RES_INT,  { (void *)16 } },

    /* LAWBAR details */

    { "tgtIf",              HCF_RES_INT,  { (void *)LAWAR_TGTIF_PCIEX2 } },

    /* Window Attributes */

    { "owAttrMem",          HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                     PCI_OUT_ATTR_RTT_MEM |
                                                     PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrMemIo",        HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                     PCI_OUT_ATTR_RTT_MEM |
                                                     PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrIo",           HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                     PCI_OUT_ATTR_RTT_IO |
                                                     PCI_OUT_ATTR_WTT_IO) } },
    { "iwAttr",             HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                     PCI_PREFETCHABLE |
                                                     PCI_IN_ATTR_RTT_LM_READ_SNOOP |
                                                     PCI_IN_ATTR_RTT_LM_WRITE_SNOOP |
                                                     PCI_IN_ATTR_TGI_LM) } },

    { "singleLawBar",       HCF_RES_INT,  { (void *)TRUE } },
    { "singleLawBarSize",   HCF_RES_INT,  { (void *)0x10000000 } },

    { "pciConfigMechanism", HCF_RES_INT,  { (void *)0 } },
    { "autoConfig",         HCF_RES_INT,  { (void *)TRUE } },
    { "pciExpressHost",     HCF_RES_INT,  { (void *)TRUE } },

#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "msiEnable",          HCF_RES_INT,  { (void *)TRUE } },
    { "dynamicInterrupts",  HCF_RES_INT,  { (void *)TRUE } },
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#define m85xxPciEx1Num  NELEMENTS(m85xxPciEx1Resources)

const struct hcfResource m85xxPciEx2Resources[] = {
    { "regBase",            HCF_RES_INT,  { (void *)PCIEX3_BASE } },
    { "mem32Addr",          HCF_RES_ADDR, { (void *)PCIEX3_MEM_ADRS} },
    { "mem32Size",          HCF_RES_INT,  { (void *)PCI_MEM_SIZE} },
    { "memIo32Addr",        HCF_RES_ADDR, { (void *)PCIEX3_MEMIO_ADRS } },
    { "memIo32Size",        HCF_RES_INT,  { (void *)PCI_MEMIO_SIZE} },
    { "io32Addr",           HCF_RES_ADDR, { (void *)PCIEX3_IO_ADRS } },
    { "io32Size",           HCF_RES_INT,  { (void *)PCI_IO_SIZE} },
    { "io16Addr",           HCF_RES_ADDR, { (void *)PCIEX3_IO_ADRS } },
    { "io16Size",           HCF_RES_INT,  { (void *)PCI_IO_SIZE } },
    { "fbbEnable",          HCF_RES_INT,  { (void *)TRUE } },
    { "cacheSize",          HCF_RES_INT,  { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,  { (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet",    HCF_RES_INT,  { (void *)TRUE } },
    { "includeFuncSet",     HCF_RES_ADDR, { (void *)sysPciEx3AutoConfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR, { (void *)sysPciEx3AutoConfigIntAssign } },

    /* non-vxbPciAutoConfig() values */

    { "pimmrBase",          HCF_RES_ADDR, { (void *)PCI_BRIDGE_PIMMR_BASE_ADRS } },
    { "lclMemAddr",         HCF_RES_ADDR, { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",      HCF_RES_INT,  { (void *)LOCAL_MEM_SIZE } },
    { "mstrMemBus",         HCF_RES_ADDR, { (void *)PCI_MSTR_MEM_BUS } },
    { "maxBusSet",          HCF_RES_INT,  { (void *)16 } },

    /* LAWBAR details */

    { "tgtIf",              HCF_RES_INT,  { (void *)LAWAR_TGTIF_PCIEX3 } },

    /* Window Attributes */

    { "owAttrMem",          HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                     PCI_OUT_ATTR_RTT_MEM |
                                                     PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrMemIo",        HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                     PCI_OUT_ATTR_RTT_MEM |
                                                     PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrIo",           HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                     PCI_OUT_ATTR_RTT_IO |
                                                     PCI_OUT_ATTR_WTT_IO) } },
    { "iwAttr",             HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                     PCI_PREFETCHABLE |
                                                     PCI_IN_ATTR_RTT_LM_READ_SNOOP |
                                                     PCI_IN_ATTR_RTT_LM_WRITE_SNOOP |
                                                     PCI_IN_ATTR_TGI_LM) } },

    { "singleLawBar",       HCF_RES_INT,  { (void *)TRUE } },
    { "singleLawBarSize",   HCF_RES_INT,  { (void *)0x10000000 } },

    { "pciConfigMechanism", HCF_RES_INT,  { (void *)0 } },
    { "autoConfig",         HCF_RES_INT,  { (void *)TRUE } },
    { "pciExpressHost",     HCF_RES_INT,  { (void *)TRUE } },

#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "msiEnable",          HCF_RES_INT,  { (void *)TRUE } },
    { "dynamicInterrupts",  HCF_RES_INT,  { (void *)TRUE } },
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#define m85xxPciEx2Num  NELEMENTS(m85xxPciEx2Resources)

const struct hcfResource m85xxPciResources[] = {
    { "regBase",            HCF_RES_INT,  { (void *)PCI_BASE } },
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
    { "includeFuncSet",     HCF_RES_ADDR, { (void *)sysPciAutoConfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR, { (void *)sysPciAutoConfigIntAssign } },

    /* non-vxbPciAutoConfig() values */

    { "pimmrBase",          HCF_RES_ADDR, { (void *)PCI_BRIDGE_PIMMR_BASE_ADRS } },
    { "lclMemAddr",         HCF_RES_ADDR, { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",      HCF_RES_INT,  { (void *)LOCAL_MEM_SIZE } },
    { "mstrMemBus",         HCF_RES_ADDR, { (void *)PCI_MSTR_MEM_BUS } },
    { "maxBusSet",          HCF_RES_INT,  { (void *)16 } },

    /* LAWBAR details */

    { "tgtIf",              HCF_RES_INT,  { (void *)LAWAR_TGTIF_PCI } },

    /* Window Attributes */

    { "owAttrMem",          HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                     PCI_OUT_ATTR_RTT_MEM  |
                                                     PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrMemIo",        HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                     PCI_OUT_ATTR_RTT_MEM  |
                                                     PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrIo",           HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                     PCI_OUT_ATTR_RTT_IO   |
                                                     PCI_OUT_ATTR_WTT_IO) } },
    { "iwAttr",             HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                     PCI_PREFETCHABLE      |
                                                     PCI_IN_ATTR_RTT_LM_READ_SNOOP  |
                                                     PCI_IN_ATTR_RTT_LM_WRITE_SNOOP |
                                                     PCI_IN_ATTR_TGI_LM) } },

    { "singleLawBar",       HCF_RES_INT,  { (void *)TRUE } },
    { "singleLawBarSize",   HCF_RES_INT,  { (void *)0x10000000 } },

    { "pciConfigMechanism", HCF_RES_INT,  { (void *)0 } },
    { "autoConfig",         HCF_RES_INT,  { (void *)TRUE } },

#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "msiEnable",          HCF_RES_INT,  { (void *)TRUE } },
    { "dynamicInterrupts",  HCF_RES_INT,  { (void *)TRUE } },
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#define m85xxPciNum NELEMENTS(m85xxPciResources)

#endif /* DRV_PCIBUS_M85XX */

#ifdef DRV_STORAGE_FSLSATA

const struct hcfResource fslSata0Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)SATA0_BASE } },
};
#define fslSata0Num NELEMENTS(fslSata0Resources)

const struct hcfResource fslSata1Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)SATA1_BASE } },
};
#define fslSata1Num NELEMENTS(fslSata1Resources)

#endif /* DRV_STORAGE_FSLSATA */

#ifdef INCLUDE_EHCI

const struct hcfResource usbEhci0Resources[] = {
    { "regBase",  HCF_RES_INT,  { (void *)EHCI_CAPLENGTH(USB1_BASE) } },
    { "ehciInit", HCF_RES_ADDR, { (void *)ehci0Init } },
    { "dataSwap", HCF_RES_ADDR, { (void *)vxbSwap32 } },
};
#define usbEhci0Num NELEMENTS(usbEhci0Resources)

const struct hcfResource usbEhci1Resources[] = {
    { "regBase",  HCF_RES_INT,  { (void *)EHCI_CAPLENGTH(USB2_BASE) } },
    { "ehciInit", HCF_RES_ADDR, { (void *)ehci1Init } },
    { "dataSwap", HCF_RES_ADDR, { (void *)vxbSwap32 } },
};
#define usbEhci1Num NELEMENTS(usbEhci1Resources)

#endif /* INCLUDE_EHCI */

#ifdef DRV_TIMER_OPENPIC

const struct hcfResource openPicTimer0Resources[] = {
    { "regBase",      HCF_RES_INT,    { (void *)EPIC_TIMER_A_BASE } },
    { "instanceName", HCF_RES_STRING, { (void *)"A" } },
    { "clkFreq",      HCF_RES_ADDR,   { (void *)sysOpenPicTimerClkFreqGet } },
    { "clkDivisor",   HCF_RES_INT,    { (void *)8 } },
    { "maxClkRate0",  HCF_RES_INT,    { (void *)SYS_CLK_RATE_MAX } },
#ifdef INCLUDE_AUX_CLK
    { "maxClkRate1",  HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX } },
#endif
};
#define openPicTimer0Num    NELEMENTS(openPicTimer0Resources)

const struct hcfResource openPicTimer1Resources[] = {
    { "regBase",      HCF_RES_INT,    { (void *)EPIC_TIMER_B_BASE } },
    { "instanceName", HCF_RES_STRING, { (void *)"B" } },
    { "clkFreq",      HCF_RES_ADDR,   { (void *)sysOpenPicTimerClkFreqGet } },
    { "clkDivisor",   HCF_RES_INT,    { (void *)0 } },
    { "maxClkRate0",  HCF_RES_INT,    { (void *)SYS_CLK_RATE_MAX } },
};
#define openPicTimer1Num    NELEMENTS(openPicTimer1Resources)

#endif /* DRV_TIMER_OPENPIC */

#ifdef DRV_STORAGE_SDHC
IMPORT UINT32 sysSdhcClkFreqGet (void);

struct hcfResource sdhcResources[] =  {
    { "regBase",      HCF_RES_INT,    { (void *)SDHC_BASE } },
    { "clkFreq",      HCF_RES_ADDR,   { (void *)sysSdhcClkFreqGet } },
    { "dmaMode",      HCF_RES_INT,    { (void *)0 } },
    { "polling",      HCF_RES_INT,    { (void *)0 } },
    { "flags"  ,      HCF_RES_INT,    { (void *)(SDHC_PIO_NEED_DELAY | SDHC_HW_SNOOP |
                                               SDHC_FIFO_ENDIANESS_REVERSE |
                                               SDHC_HOST_VER_REVERSE |
                                               SDHC_HOST_CTRL_FREESCALE) } },
};
#define sdhcNum  NELEMENTS(sdhcResources)
#endif /* DRV_STORAGE_SDHC */

const struct hcfDevice hcfDeviceList[] = {
    { "ppcIntCtlr", 0, VXB_BUSID_PLB, 0, ppcIntCtlrNum, ppcIntCtlrResources },
    { "epic", 0, VXB_BUSID_PLB, 0, epicNum, epicResources },
    { "m85xxTimerDev", 0, VXB_BUSID_PLB, 0, m85xxTimerNum, m85xxTimerResources},
    { "ns16550", 0, VXB_BUSID_PLB, 0, ns1655x0Num, ns1655x0Resources },
    { "ns16550", 1, VXB_BUSID_PLB, 0, ns1655x1Num, ns1655x1Resources },

#if defined(INCLUDE_ETSEC_VXB_END) || defined(INCLUDE_ETSEC_VXB_END2)
    { "motetsec", 0, VXB_BUSID_PLB, 0, etsec0Num, etsec0Resources },
    { "motetsec", 1, VXB_BUSID_PLB, 0, etsec1Num, etsec1Resources },
#endif

#ifdef DRV_RESOURCE_M85XXCCSR
    { "m85xxCCSR", 0, VXB_BUSID_PLB, 0, m85xxLawBarNum, m85xxLawBarResources },
#endif

#ifdef DRV_PCIBUS_M85XX
    { "m85xxPci",  0, VXB_BUSID_PLB, 0, m85xxPciEx0Num, m85xxPciEx0Resources },
    { "m85xxPci",  1, VXB_BUSID_PLB, 0, m85xxPciEx1Num, m85xxPciEx1Resources },
    { "m85xxPci",  2, VXB_BUSID_PLB, 0, m85xxPciEx2Num, m85xxPciEx2Resources },
    { "m85xxPci",  3, VXB_BUSID_PLB, 0, m85xxPciNum,    m85xxPciResources },
#endif

#ifdef DRV_STORAGE_FSLSATA
    { "fslSata", 0, VXB_BUSID_PLB, 0, fslSata0Num, fslSata0Resources },
    { "fslSata", 1, VXB_BUSID_PLB, 0, fslSata1Num, fslSata1Resources },
#endif

#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci", 0, VXB_BUSID_PLB, 0, usbEhci0Num, usbEhci0Resources },
    { "vxbPlbUsbEhci", 1, VXB_BUSID_PLB, 0, usbEhci1Num, usbEhci1Resources },
#endif

#ifdef DRV_TIMER_OPENPIC
    { "openPicTimer", 0, VXB_BUSID_PLB, 0, openPicTimer0Num, openPicTimer0Resources },
    { "openPicTimer", 1, VXB_BUSID_PLB, 0, openPicTimer1Num, openPicTimer1Resources },
#endif

#ifdef DRV_STORAGE_SDHC
    { "sdhci", 0, VXB_BUSID_PLB, 0, sdhcNum, sdhcResources },
#endif /* DRV_STORAGE_SDHC */
};
const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] = {
#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci", 0, "hasEmbeddedTT", VXB_PARAM_INT32,   { (void *)TRUE } },
    { "vxbPlbUsbEhci", 0, "postResetHook", VXB_PARAM_FUNCPTR, { (void *)ehci0PostResetHook } },
    { "vxbPlbUsbEhci", 1, "hasEmbeddedTT", VXB_PARAM_INT32,   { (void *)TRUE } },
    { "vxbPlbUsbEhci", 1, "postResetHook", VXB_PARAM_FUNCPTR, { (void *)ehci1PostResetHook } },
#endif
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, { (void *)0 } }
};
