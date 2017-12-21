/* hwconf.c - Hardware configuration support module */

/*
 * Copyright (c) 2009-2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,12jan12,agf  remove exIrqWkrd from epic resource, deprecated in driver
                 (WIND00176018)
01b,26may10,syt  added polarity settings for EXT_INT8~EXT_INT11 to workaround
                 PCIe hardware issue on P2020 (WIND00214271)
01a,22apr09,syt  derived from version 02b of ads8572/hwconf.c
*/

#include <vxWorks.h>
#include <vsbConfig.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#include <hwif/vxbus/vxbIntrCtlr.h>
#include <drv/pci/pciAutoConfigLib.h>
#include "config.h"
#include "mot85xxPci.h"

#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
#include <../src/hwif/intCtlr/vxbIntDynaCtlrLib.h>
#endif  /* INCLUDE_INTCTLR_DYNAMIC_LIB */

#ifdef DRV_INTCTLR_EPIC
extern UINT32 sysPicClkFreqGet(UINT32);
#endif  /* DRV_INTCTLR_EPIC */

#ifdef INCLUDE_PCI_BUS
IMPORT STATUS  sysPciEx1AutoConfigInclude(PCI_SYSTEM *,PCI_LOC *,UINT);
IMPORT UCHAR   sysPciEx1AutoConfigIntAssign(PCI_SYSTEM *,PCI_LOC *, UCHAR);
IMPORT STATUS  sysPciEx2AutoConfigInclude(PCI_SYSTEM *,PCI_LOC *,UINT);
IMPORT UCHAR   sysPciEx2AutoConfigIntAssign(PCI_SYSTEM *,PCI_LOC *, UCHAR);
IMPORT STATUS  sysPciEx3AutoConfigInclude(PCI_SYSTEM *,PCI_LOC *,UINT);
IMPORT UCHAR   sysPciEx3AutoConfigIntAssign(PCI_SYSTEM *,PCI_LOC *, UCHAR);
#endif  /* INCLUDE_PCI_BUS */

#ifdef INCLUDE_EHCI
IMPORT void   ehci0Init (void);
IMPORT void   ehci0PostResetHook (void);
#endif /* INCLUDE_EHCI */

/*
 * On the P2020ds board, PHYs are physically wired according
 * to the following table:
 *
 *      Data port pins  Management port pins    MII address
 *      --------------  --------------------    -----------
 * PHY0:    TSEC0       TSEC0
 * PHY1:    TSEC1       TSEC0
 * PHY2:    TSEC2       TSEC0
 *
 * The tricky part is that both PHYs have their management pins
 * connected to TSEC0. The VxBus TSEC driver allows us to specify
 * the name of the device that provides PHY management support.
 * This device is "motetsec0" in all cases. We also specify the
 * "phyAddr" resource for each device, which is set according
 * to the MII address in the table above.
 */

#if defined(INCLUDE_ETSEC_VXB_END) || defined(INCLUDE_ETSEC_VXB_END2)

const struct hcfResource etsec0Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)(CCSBAR + 0x24000) } },
    { "phyAddr",            HCF_RES_INT,    { (void *)0 } },
    { "miiIfName",          HCF_RES_STRING, { (void *)"motetsec" } },
    { "miiIfUnit",          HCF_RES_INT,    { (void *)0 } }
};
#define etsec0Num NELEMENTS(etsec0Resources)

const struct hcfResource etsec1Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)(CCSBAR + 0x25000) } },
    { "phyAddr",            HCF_RES_INT,    { (void *)1 } },
    { "miiIfName",          HCF_RES_STRING, { (void *)"motetsec" } },
    { "miiIfUnit",          HCF_RES_INT,    { (void *)0 } }
};
#define etsec1Num NELEMENTS(etsec1Resources)

const struct hcfResource etsec2Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)(CCSBAR + 0x26000) } },
    { "phyAddr",            HCF_RES_INT,    { (void *)2 } },
    { "miiIfName",          HCF_RES_STRING, { (void *)"motetsec" } },
    { "miiIfUnit",          HCF_RES_INT,    { (void *)0 } }
};
#define etsec2Num NELEMENTS(etsec2Resources)

#endif /* INCLUDE_ETSEC_VXB_END || INCLUDE_ETSEC_VXB_END2 */

const struct hcfResource m85xxCCSR0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)CCSBAR  } },
    { "LAWAR_ENABLE",       HCF_RES_INT,    { (void *)LAWAR_ENABLE } },
    { "LAWBAR_ADRS_SHIFT",  HCF_RES_INT,    { (void *)LAWBAR_ADRS_SHIFT } },
    { "LAWBAR0",            HCF_RES_STRING, { (void *)"reserved" } },
    { "LAWBAR1",            HCF_RES_STRING, { (void *)"reserved" } },
    { "LAWBAR2",            HCF_RES_STRING, { (void *)"reserved" } },
    { "LAWBAR3",            HCF_RES_STRING, { (void *)"reserved" } },
    { "LAWBAR4",            HCF_RES_STRING, { (void *)"m85xxPci" } },
    { "LAWBAR5",            HCF_RES_STRING, { (void *)"m85xxPci" } },
    { "LAWBAR6",            HCF_RES_STRING, { (void *)"m85xxPci" } },
    { "LAWBAR7",            HCF_RES_STRING, { (void *)NULL } },
    { "LAWBAR8",            HCF_RES_STRING, { (void *)NULL } },
    { "LAWBAR9",            HCF_RES_STRING, { (void *)NULL } }
};
#define m85xxCCSR0Num NELEMENTS(m85xxCCSR0Resources)

#ifdef DRV_DMA_FSL
const struct hcfResource fslDma1Resources[] = {
    { "regBase",    HCF_RES_INT,            { (void *)DMA1_REGBASE } },
};
#define fslDma1Num NELEMENTS(fslDma1Resources)

const struct hcfResource fslDma2Resources[] = {
    { "regBase",    HCF_RES_INT,            { (void *)DMA2_REGBASE } },
};
#define fslDma2Num NELEMENTS(fslDma2Resources)
#endif  /* DRV_DMA_FSL */

#ifdef INCLUDE_PCI_BUS
const struct hcfResource m85xxPci0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)PCIEX1_REGBASE } },
    { "mem32Addr",          HCF_RES_ADDR,   { (void *)PCIEX1_MEM_ADRS } },
    { "mem32Size",          HCF_RES_INT,    { (void *)PCIEX1_MEM_SIZE } },
    { "memIo32Addr",        HCF_RES_ADDR,   { (void *)PCIEX1_MEMIO_ADRS } },
    { "memIo32Size",        HCF_RES_INT,    { (void *)PCIEX1_MEMIO_SIZE } },
    { "io32Addr",           HCF_RES_ADDR,   { (void *)PCIEX1_IO_ADRS } },
    { "io32Size",           HCF_RES_INT,    { (void *)PCIEX1_IO_SIZE } },
    { "io16Addr",           HCF_RES_ADDR,   { (void *)PCIEX1_IO_ADRS } },
    { "io16Size",           HCF_RES_INT,    { (void *)PCIEX1_IO_SIZE } },
    { "fbbEnable",          HCF_RES_INT,    { (void *)TRUE } },
    { "cacheSize",          HCF_RES_INT,    { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,    { (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet",    HCF_RES_INT,    { (void *)TRUE } },
    { "includeFuncSet",     HCF_RES_ADDR,   { (void *)sysPciEx1AutoConfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR,   { (void *)sysPciEx1AutoConfigIntAssign } },

    /* non vxbPciAutoConfig() values */

    { "pimmrBase",          HCF_RES_ADDR,   { (void *)PCI_BRIDGE_PIMMR_BASE_ADRS } },
    { "lclMemAddr",         HCF_RES_ADDR,   { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",      HCF_RES_INT,    { (void *)(512 * 1024 * 1024) } },
    { "mstrMemBus",         HCF_RES_ADDR,   { (void *)PCI_MSTR_MEM_BUS } },
    { "maxBusSet",          HCF_RES_INT,    { (void *)16 } },
    { "pciConfigMechanism", HCF_RES_INT,    { (void *)0 } },

    /* LAWBAR details */

    { "tgtIf",              HCF_RES_INT,    { (void *)(LAWAR_TGTIF_PCIEX1) } },

    /* Window Attributes - Defaults to 8540 type if none given */

    { "owAttrMem",          HCF_RES_INT,    { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                       PCI_OUT_ATTR_RTT_MEM  |
                                                       PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrMemIo",        HCF_RES_INT,    { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                       PCI_OUT_ATTR_RTT_MEM  |
                                                       PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrIo",           HCF_RES_INT,    { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                       PCI_OUT_ATTR_RTT_IO   |
                                                       PCI_OUT_ATTR_WTT_IO) } },
    { "iwAttr",             HCF_RES_INT,    { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                       PCI_PREFETCHABLE      |
                                                       PCI_IN_ATTR_RTT_LM_READ_SNOOP  |
                                                       PCI_IN_ATTR_RTT_LM_WRITE_SNOOP |
                                                       PCI_IN_ATTR_TGI_LM) } },
    { "singleLawBar",       HCF_RES_INT,    { (void *)(TRUE) } },
    { "singleLawBarSize",   HCF_RES_INT,    { (void *)(0x10000000) } },
    { "autoConfig",         HCF_RES_INT,    { (void *)(TRUE) } },
    { "pciExpressHost",     HCF_RES_INT,    { (void *)(TRUE) } },
#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "msiEnable",          HCF_RES_INT,    { (void *)TRUE } },
    { "dynamicInterrupts",  HCF_RES_INT,    { (void *)TRUE } },
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#define m85xxPci0Num NELEMENTS(m85xxPci0Resources)

const struct hcfResource m85xxPci1Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)PCIEX3_REGBASE } },
    { "mem32Addr",          HCF_RES_ADDR,   { (void *)PCIEX3_MEM_ADRS } },
    { "mem32Size",          HCF_RES_INT,    { (void *)PCIEX3_MEM_SIZE } },
    { "memIo32Addr",        HCF_RES_ADDR,   { (void *)PCIEX3_MEMIO_ADRS } },
    { "memIo32Size",        HCF_RES_INT,    { (void *)PCIEX3_MEMIO_SIZE } },
    { "io32Addr",           HCF_RES_ADDR,   { (void *)PCIEX3_IO_ADRS } },
    { "io32Size",           HCF_RES_INT,    { (void *)PCIEX3_IO_SIZE } },
    { "io16Addr",           HCF_RES_ADDR,   { (void *)PCIEX3_IO_ADRS } },
    { "io16Size",           HCF_RES_INT,    { (void *)PCIEX3_IO_SIZE } },
    { "fbbEnable",          HCF_RES_INT,    { (void *)TRUE } },
    { "cacheSize",          HCF_RES_INT,    { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,    { (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet",    HCF_RES_INT,    { (void *)FALSE } },
    { "includeFuncSet",     HCF_RES_ADDR,   { (void *)sysPciEx3AutoConfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR,   { (void *)sysPciEx3AutoConfigIntAssign } },
    { "maxBusSet",          HCF_RES_INT,    { (void *)16 } },
    { "pciConfigMechanism", HCF_RES_INT,    { (void *)0 } },

    /* non vxbPciAutoConfig() values */

    { "pimmrBase",          HCF_RES_ADDR,   { (void *)PCI_BRIDGE_PIMMR_BASE_ADRS } },
    { "lclMemAddr",         HCF_RES_ADDR,   { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",      HCF_RES_INT,    { (void *)(512 * 1024 * 1024) } },
    { "mstrMemBus",         HCF_RES_ADDR,   { (void *)PCI_MSTR_MEM_BUS } },

    /* LAWBAR details */

    { "tgtIf",              HCF_RES_INT,    { (void *)(LAWAR_TGTIF_PCIEX3)} },

    /* Window Attributes - Defaults to 8540 type if none given */

    { "owAttrMem",          HCF_RES_INT,    { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                       PCI_OUT_ATTR_RTT_MEM  |
                                                       PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrMemIo",        HCF_RES_INT,    { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                       PCI_OUT_ATTR_RTT_MEM  |
                                                       PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrIo",           HCF_RES_INT,    { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                       PCI_OUT_ATTR_RTT_IO   |
                                                       PCI_OUT_ATTR_WTT_IO) } },
    { "iwAttr",             HCF_RES_INT,    { (void *)(PCI_WINDOW_ENABLE_BIT |
                                                       PCI_PREFETCHABLE      |
                                                       PCI_IN_ATTR_RTT_LM_READ_SNOOP  |
                                                       PCI_IN_ATTR_RTT_LM_WRITE_SNOOP |
                                                       PCI_IN_ATTR_TGI_LM) } },
    { "singleLawBar",       HCF_RES_INT,    { (void *)(TRUE) } },
    { "singleLawBarSize",   HCF_RES_INT,    { (void *)(0x10000000) } },
    { "autoConfig",         HCF_RES_INT,    { (void *)(TRUE) } },
    { "pciExpressHost",     HCF_RES_INT,    { (void *)(TRUE) } },
#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "msiEnable",          HCF_RES_INT,    { (void *)TRUE } },
    { "dynamicInterrupts",  HCF_RES_INT,    { (void *)TRUE } },
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#define m85xxPci1Num NELEMENTS(m85xxPci1Resources)
#endif /* INCLUDE_PCI_BUS */

const struct hcfResource ns1655x1Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)COM1_ADR } },
    { "clkFreq",            HCF_RES_ADDR,   { (void *)sysClkFreqGet } },
    { "regInterval",        HCF_RES_INT,    { (void *)DUART_REG_ADDR_INTERVAL } }
};
#define ns1655x1Num NELEMENTS(ns1655x1Resources)

const struct hcfResource ns1655x2Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)COM2_ADR } },
    { "clkFreq",            HCF_RES_ADDR,   { (void *)sysClkFreqGet } },
    { "regInterval",        HCF_RES_INT,    { (void *)DUART_REG_ADDR_INTERVAL } }
};
#define ns1655x2Num NELEMENTS(ns1655x2Resources)

INT_BANK_DESC epicIntDesc[] = {
    { 0,   EPIC_EX_IRQ_NUM,   EPIC_EX_REG_BASE,   EPIC_EX_REG_INTERVAL,   EPIC_EX_INTERRUPT   },
    { 0,   EPIC_IN_IRQ_NUM,   EPIC_IN_REG_BASE,   EPIC_IN_REG_INTERVAL,   EPIC_IN_INTERRUPT   },
    { 0,   EPIC_GT_IRQ_NUM,   EPIC_GT_A_REG_BASE, EPIC_GT_REG_INTERVAL,   EPIC_GT_A_INTERRUPT },
    { 0,   EPIC_GT_IRQ_NUM,   EPIC_GT_B_REG_BASE, EPIC_GT_REG_INTERVAL,   EPIC_GT_B_INTERRUPT },
    { 0,   EPIC_MSG_IRQ_NUM,  EPIC_MSG_REG_BASE,  EPIC_MSG_REG_INTERVAL,  EPIC_MSG_INTERRUPT  },
    { 0,   EPIC_SMSG_IRQ_NUM, EPIC_SMSG_REG_BASE, EPIC_SMSG_REG_INTERVAL, EPIC_SMSG_INTERRUPT },
    { 251, EPIC_IPI_IRQ_NUM,  EPIC_IPI_REG_BASE,  EPIC_IPI_REG_INTERVAL,  EPIC_IPI_INTERRUPT  }
};
struct intrCtlrInputs epicInputs[] = {
    { EPIC_DUART_INT_VEC,       "ns16550",          0,      0 },
    { EPIC_DUART_INT_VEC,       "ns16550",          1,      0 },

#if defined(INCLUDE_ETSEC_VXB_END) || defined(INCLUDE_ETSEC_VXB_END2)
    { EPIC_TSEC1TX_INT_VEC,     "motetsec",         0,      0 },
    { EPIC_TSEC1RX_INT_VEC,     "motetsec",         0,      1 },
    { EPIC_TSEC1ERR_INT_VEC,    "motetsec",         0,      2 },
    { EPIC_TSEC2TX_INT_VEC,     "motetsec",         1,      0 },
    { EPIC_TSEC2RX_INT_VEC,     "motetsec",         1,      1 },
    { EPIC_TSEC2ERR_INT_VEC,    "motetsec",         1,      2 },
    { EPIC_TSEC3TX_INT_VEC,     "motetsec",         2,      0 },
    { EPIC_TSEC3RX_INT_VEC,     "motetsec",         2,      1 },
    { EPIC_TSEC3ERR_INT_VEC,    "motetsec",         2,      2 },
#endif  /* INCLUDE_ETSEC_VXB_END || INCLUDE_ETSEC_VXB_END2 */

#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    { OPENPIC_TIMERA0_INT_VEC,  "openPicTimer",     0,      0 },
    { OPENPIC_TIMERA1_INT_VEC,  "openPicTimer",     0,      1 },
    { OPENPIC_TIMERA2_INT_VEC,  "openPicTimer",     0,      2 },
    { OPENPIC_TIMERA3_INT_VEC,  "openPicTimer",     0,      3 },
    { OPENPIC_TIMERB0_INT_VEC,  "openPicTimer",     1,      0 },
    { OPENPIC_TIMERB1_INT_VEC,  "openPicTimer",     1,      1 },
    { OPENPIC_TIMERB2_INT_VEC,  "openPicTimer",     1,      2 },
    { OPENPIC_TIMERB3_INT_VEC,  "openPicTimer",     1,      3 },
#endif  /* INCLUDE_INTCTLR_DYNAMIC_LIB */

#ifdef INCLUDE_EHCI
    { EPIC_USB_INT_VEC,         "vxbPlbUsbEhci",    0,      0 },
#endif  /* INCLUDE_EHCI */

#ifdef  DRV_DMA_FSL
    { EPIC_DMA1_CHAN0_INT_VEC,  "fslDma",           0,      0 },
    { EPIC_DMA1_CHAN1_INT_VEC,  "fslDma",           0,      1 },
    { EPIC_DMA1_CHAN2_INT_VEC,  "fslDma",           0,      2 },
    { EPIC_DMA1_CHAN3_INT_VEC,  "fslDma",           0,      3 },
    { EPIC_DMA2_CHAN0_INT_VEC,  "fslDma",           1,      0 },
    { EPIC_DMA2_CHAN1_INT_VEC,  "fslDma",           1,      1 },
    { EPIC_DMA2_CHAN2_INT_VEC,  "fslDma",           1,      2 },
    { EPIC_DMA2_CHAN3_INT_VEC,  "fslDma",           1,      3 },
#endif  /* DRV_DMA_FSL */

#ifdef _WRS_CONFIG_SMP
    { EPIC_VEC_IPI_IRQ0,        "ipi",              0,      0 },
#endif /* _WRS_CONFIG_SMP */
};

struct intrCtlrCpu epicCpu[] = {
    { EPIC_TSEC3ERR_INT_VEC,    1 },
    { EPIC_TSEC1ERR_INT_VEC,    1 },
    { EPIC_TSEC2ERR_INT_VEC,    1 }
};

/* 
 * workaround for Programmable Interrupt Controller (PIC), set polarity
 * of EXT_IRQ8 ~ EXT_IRQ11 as active-high
 */

struct intrCtlrTrigger epicTrigger[] = {
    { EPIC_EXT_IRQ8_INT_VEC,  VXB_INTR_TRIG_ACTIVE_HIGH },
    { EPIC_EXT_IRQ9_INT_VEC,  VXB_INTR_TRIG_ACTIVE_HIGH },
    { EPIC_EXT_IRQ10_INT_VEC, VXB_INTR_TRIG_ACTIVE_HIGH },
    { EPIC_EXT_IRQ11_INT_VEC, VXB_INTR_TRIG_ACTIVE_HIGH },
};

const struct hcfResource epic0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)CCSBAR } },
    { "input",              HCF_RES_ADDR,   { (void *)&epicInputs[0] } },
    { "inputTableSize",     HCF_RES_INT,    { (void *)NELEMENTS(epicInputs) } },
    { "cpuRoute",           HCF_RES_ADDR,   { (void *)&epicCpu[0] } },
    { "cpuRouteTableSize",  HCF_RES_INT,    { (void *)NELEMENTS(epicCpu) } },
    { "exPolar",            HCF_RES_INT,    { (void *)EPIC_INT_ACT_LOW } },
    { "exSense",            HCF_RES_INT,    { (void *)EPIC_SENSE_LVL } },
    { "trigger",            HCF_RES_ADDR,   { (void *)&epicTrigger[0] } },
    { "triggerTableSize",   HCF_RES_INT,    { (void *)NELEMENTS(epicTrigger) } },	
    { "intDesc_0",          HCF_RES_ADDR,   { (void *)&epicIntDesc[0] } },
    { "intDesc_1",          HCF_RES_ADDR,   { (void *)&epicIntDesc[1] } },
    { "intDesc_2",          HCF_RES_ADDR,   { (void *)&epicIntDesc[2] } },
    { "intDesc_3",          HCF_RES_ADDR,   { (void *)&epicIntDesc[3] } },
    { "intDesc_4",          HCF_RES_ADDR,   { (void *)&epicIntDesc[4] } },
    { "intDesc_5",          HCF_RES_ADDR,   { (void *)&epicIntDesc[5] } },
};
#define epic0Num NELEMENTS(epic0Resources)

#ifdef DRV_TIMER_M85XX

struct hcfResource m85xxTimerResources[] =  {
    {"regBase",             HCF_RES_INT,    { (void *)0 } },
    {"decMinClkRate",       HCF_RES_INT,    { (void *)SYS_CLK_RATE_MIN } },
    {"decMaxClkRate",       HCF_RES_INT,    { (void *)SYS_CLK_RATE_MAX } },
#ifdef INCLUDE_AUX_CLK
    {"fitMinClkRate",       HCF_RES_INT,    { (void *)AUX_CLK_RATE_MIN } },
    {"fitMaxClkRate",       HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX } },
#endif /* INCLUDE_AUX_CLK */
    {"sysClockFreq",        HCF_RES_ADDR,   { (void *)&sysClkFreqGet } },
};
#define m85xxTimerNum   NELEMENTS(m85xxTimerResources)

#endif /* DRV_TIMER_M85XX */

struct intrCtlrInputs ppcIntCtlrInputs[] = {
    { 0,    "epic",             0,      0 },
    { 1,    "m85xxTimerDev",    0,      0 },
    { 3,    "m85xxTimerDev",    0,      2 },
    { 4,    "m85xxTimerDev",    0,      1 },
};

const struct hcfResource ppcIntCtlr0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)TRUE } },
    { "input",              HCF_RES_ADDR,   { (void *)&ppcIntCtlrInputs[0] } },
    { "inputTableSize",     HCF_RES_INT,    { (void *)NELEMENTS(ppcIntCtlrInputs) } },
};
#define ppcIntCtlr0Num NELEMENTS(ppcIntCtlr0Resources)

#ifdef INCLUDE_EHCI
const struct hcfResource usbEhci0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)EHCI_CAPLENGTH(CCSBAR + 0x22000) } },
    { "ehciInit",           HCF_RES_ADDR,   { (void *)ehci0Init } },
    { "dataSwap",           HCF_RES_ADDR,   { (void *)vxbSwap32 } },
};
#define usbEhci0Num NELEMENTS(usbEhci0Resources)
#endif /* INCLUDE_EHCI */

#ifdef DRV_TIMER_OPENPIC
const struct hcfResource openPicTimerDevAresources[] = {
    { VXB_REG_BASE,          HCF_RES_INT,    { (void *)(CCSBAR + 0x000410F0) } },
    { "instanceName",        HCF_RES_STRING, { (void *)"A" } },
    { VXB_CLK_FREQ,          HCF_RES_ADDR,   { (void *)sysPicClkFreqGet } },
    { "clkDivisor",          HCF_RES_INT,    { (void *)8 } },
    { "maxClkRate0",         HCF_RES_INT,    { (void *)SYS_CLK_RATE_MAX } },
#ifdef INCLUDE_AUX_CLK
    { "maxClkRate1",         HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX } },
#endif /* INCLUDE_AUX_CLK */
#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "vector0",             HCF_RES_INT,    { (void *)OPENPIC_TIMERA0_INT_VEC } },
    { "vector1",             HCF_RES_INT,    { (void *)OPENPIC_TIMERA1_INT_VEC } },
    { "vector2",             HCF_RES_INT,    { (void *)OPENPIC_TIMERA2_INT_VEC } },
    { "vector3",             HCF_RES_INT,    { (void *)OPENPIC_TIMERA3_INT_VEC } },
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#define openPicTimerDevAnum NELEMENTS(openPicTimerDevAresources)

const struct hcfResource openPicTimerDevBresources[] = {
    { VXB_REG_BASE,          HCF_RES_INT,    { (void *)(CCSBAR + 0x000420F0) } },
    { "instanceName",        HCF_RES_STRING, { (void *)"B" } },
    { VXB_CLK_FREQ,          HCF_RES_ADDR,   { (void *)sysPicClkFreqGet } },
    { "clkDivisor",          HCF_RES_INT,    { (void *)0 } },
#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "vector0",             HCF_RES_INT,    { (void *)OPENPIC_TIMERB0_INT_VEC } },
    { "vector1",             HCF_RES_INT,    { (void *)OPENPIC_TIMERB1_INT_VEC } },
    { "vector2",             HCF_RES_INT,    { (void *)OPENPIC_TIMERB2_INT_VEC } },
    { "vector3",             HCF_RES_INT,    { (void *)OPENPIC_TIMERB3_INT_VEC } },
#endif  /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#define openPicTimerDevBnum NELEMENTS(openPicTimerDevBresources)
#endif   /* DRV_TIMER_OPENPIC */

const struct hcfDevice hcfDeviceList[] = {

    /*
     * Initialize ppcIntCtlr before epic.
     * The vector table for external interrupts are over written by epic
     * for an optimized purpose.
     */

    { "ppcIntCtlr", 0,  VXB_BUSID_PLB,  0,  ppcIntCtlr0Num, ppcIntCtlr0Resources },
    { "epic",       0,  VXB_BUSID_PLB,  0,  epic0Num,       epic0Resources },

#if defined(INCLUDE_ETSEC_VXB_END) || defined(INCLUDE_ETSEC_VXB_END2)
    { "motetsec",   0,  VXB_BUSID_PLB,  0,  etsec0Num, etsec0Resources },
    { "motetsec",   1,  VXB_BUSID_PLB,  0,  etsec1Num, etsec1Resources },
    { "motetsec",   2,  VXB_BUSID_PLB,  0,  etsec2Num, etsec2Resources },
#endif /* INCLUDE_ETSEC_VXB_END || INCLUDE_ETSEC_VXB_END2 */

#ifdef DRV_SIO_NS16550
    { "ns16550",    0,  VXB_BUSID_PLB,  0,  ns1655x1Num,    ns1655x1Resources },
    { "ns16550",    1,  VXB_BUSID_PLB,  0,  ns1655x2Num,    ns1655x2Resources },
#endif  /* DRV_SIO_NS16550 */

#ifdef DRV_RESOURCE_M85XXCCSR
    { "m85xxCCSR",  0,  VXB_BUSID_PLB,  0,  m85xxCCSR0Num,  m85xxCCSR0Resources },
#endif  /* DRV_RESOURCE_M85XXCCSR */

#ifdef DRV_TIMER_M85XX
    { "m85xxTimerDev",  0,  VXB_BUSID_PLB,  0,  m85xxTimerNum,  m85xxTimerResources },
#endif /* DRV_TIMER_M85XX */

#ifdef DRV_TIMER_OPENPIC
    { "openPicTimer",   0,  VXB_BUSID_PLB,  0,  openPicTimerDevAnum, openPicTimerDevAresources },
    { "openPicTimer",   1,  VXB_BUSID_PLB,  0,  openPicTimerDevBnum, openPicTimerDevBresources },
#endif /* DRV_TIMER_M85XX */

#ifdef INCLUDE_PCI_BUS
    { "m85xxPci",   0,  VXB_BUSID_PLB,      0,  m85xxPci0Num,   m85xxPci0Resources },
    { "m85xxPci",   1,  VXB_BUSID_PLB,      0,  m85xxPci1Num,   m85xxPci1Resources },
#endif  /* INCLUDE_PCI_BUS */

#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci", 0, VXB_BUSID_PLB,    0,  usbEhci0Num,    usbEhci0Resources },
#endif  /* INCLUDE_EHCI */

#ifdef DRV_DMA_FSL
    { "fslDma",     0,  VXB_BUSID_PLB,      0,  fslDma1Num,     fslDma1Resources },
    { "fslDma",     1,  VXB_BUSID_PLB,      0,  fslDma2Num,     fslDma2Resources },
#endif /* DRV_DMA_FSL */
};

const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] = {
#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci", 0, "hasEmbeddedTT", VXB_PARAM_INT32,   { (void *)TRUE } },
    { "vxbPlbUsbEhci", 0, "postResetHook", VXB_PARAM_FUNCPTR, { (void *)ehci0PostResetHook } },
#endif  /* INCLUDE_EHCI */
    { NULL,            0, NULL,            VXB_PARAM_END_OF_LIST, { (void *)0 } }
};
