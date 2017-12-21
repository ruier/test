/* hwconf.c - Hardware configuration support module */

/*
 * Copyright (c) 2012, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,20may13,wyt  added support for P1023RDB
01b,07aug12,syt  added support for P1021RDB and P1024RDB boards
01a,20may12,syt  derived from version 01g of fsl_p1020_rdb/hwconf.c
*/

/*
DESCRIPTION
Configuration file for vxBus
*/

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#include <hwif/vxbus/vxbIntrCtlr.h>
#include <drv/pci/pciAutoConfigLib.h>
#include <../src/hwif/h/end/vxbDtsecEnd.h>
#ifdef DRV_STORAGE_SDHC
#   include <h/storage/vxbSdMmcLib.h>
#   include <h/storage/vxbSdhcStorage.h>
#endif /* DRV_STORAGE_SDHC */

#include "config.h"

#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
#   include <../src/hwif/intCtlr/vxbIntDynaCtlrLib.h>
#endif  /* INCLUDE_INTCTLR_DYNAMIC_LIB */

#ifdef DRV_I2CBUS_FSL
#   include <hwif/vxbus/vxbI2cLib.h>
#endif /* DRV_I2CBUS_FSL */

IMPORT UINT32 sysClkFreqGet (void);

#ifdef DRV_INTCTLR_EPIC
IMPORT UINT32 sysPicClkFreqGet(UINT32);
#endif  /* DRV_INTCTLR_EPIC */

#ifdef INCLUDE_PCI_BUS
IMPORT STATUS  sysPciExAutoConfigInclude(PCI_SYSTEM *,PCI_LOC *,UINT);
IMPORT UCHAR   sysPciEx1AutoConfigIntAssign(PCI_SYSTEM *,PCI_LOC *, UCHAR);
IMPORT UCHAR   sysPciEx2AutoConfigIntAssign(PCI_SYSTEM *,PCI_LOC *, UCHAR);
#endif  /* INCLUDE_PCI_BUS */

#if (defined(INCLUDE_UCC_VXB_END) || defined(DRV_SIO_UCC))

#   include <../src/hwif/h/end/vxbUccEnd.h>
#   include <hwif/intCtlr/vxbQeIntCtlr.h>
#   include <../src/hwif/h/resource/qeIOPort.h>

IMPORT UINT8    _binary_fsl_qe_ucode_1021_10_A_bin_start[];
IMPORT void     umMiiExpose (void);
IMPORT void     umMiiRelease (void);
IMPORT void     sysLocalBusAccessEnable (BOOL);

#define  QE_MICROCODE_ADDR    _binary_fsl_qe_ucode_1021_10_A_bin_start

#endif /* INCLUDE_UCC_VXB_END || DRV_SIO_UCC */

#ifdef INCLUDE_EHCI
IMPORT void   ehci0Init (void);
IMPORT void   ehci0PostResetHook (void);
#endif /* INCLUDE_EHCI */

#define ETSEC_NAME  "motetsec"

IMPORT UINT32 sysPlbClkFreqGet (void);

/*
 * On the P10XXRDB board, PHYs are physically wired according
 * to the following table:
 *
 *      Data port pins  Management port pins    MII address
 *      --------------  --------------------    -----------
 * PHY0:    TSEC0       TSEC0                   2
 * PHY1:    TSEC1       TSEC0                   0
 * PHY2:    TSEC2       TSEC0                   1
 *
 * The tricky part is that both PHYs have their management pins
 * connected to TSEC0. The VxBus TSEC driver allows us to specify
 * the name of the device that provides PHY management support.
 * This device is "motetsec0" in all cases. We also specify the
 * "phyAddr" resource for each device, which is set according
 * to the MII address in the table above.
 */

#if defined(INCLUDE_ETSEC_VXB_END) || defined(INCLUDE_ETSEC_VXB_END2)

struct hcfResource etsec0Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)ETSEC0_GROUP0_BASE } },
    { "phyAddr",            HCF_RES_INT,    { (void *)2 } },
    { "miiIfName",          HCF_RES_STRING, { (void *)"tsecMdio" } },
    { "miiIfUnit",          HCF_RES_INT,    { (void *)0 } },
#   ifdef INCLUDE_AMP
    { "coreNum",            HCF_RES_INT,    { (void *)ETSEC0_CORE_NUM } },
#   endif /* INCLUDE_AMP */
};
#   define etsec0Num NELEMENTS(etsec0Resources)

const struct hcfResource etsec1Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)ETSEC1_GROUP0_BASE } },
    { "regBase1",           HCF_RES_INT,    { (void *)ETSEC1_MDIO_BASE } },
    { "phyAddr",            HCF_RES_INT,    { (void *)0 } },
    { "miiIfName",          HCF_RES_STRING, { (void *)"tsecMdio" } },
    { "miiIfUnit",          HCF_RES_INT,    { (void *)0 } },
#   ifdef INCLUDE_AMP
    { "coreNum",            HCF_RES_INT,    { (void *)ETSEC1_CORE_NUM } },
#   endif /* INCLUDE_AMP */ 
};
#   define etsec1Num NELEMENTS(etsec1Resources)

const struct hcfResource etsec2Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)ETSEC2_GROUP0_BASE } },
    { "regBase1",           HCF_RES_INT,    { (void *)ETSEC2_MDIO_BASE } },
    { "phyAddr",            HCF_RES_INT,    { (void *)1 } },
    { "miiIfName",          HCF_RES_STRING, { (void *)"tsecMdio" } },
    { "miiIfUnit",          HCF_RES_INT,    { (void *)0 } },
#   ifdef INCLUDE_AMP
    { "coreNum",            HCF_RES_INT,    { (void *)ETSEC2_CORE_NUM } },
#   endif /* INCLUDE_AMP */
};
#   define etsec2Num NELEMENTS(etsec2Resources)

const struct hcfResource tsecMdioResources[] ={
    { "regBase",            HCF_RES_INT,    { (void *)(ETSEC0_MDIO_BASE) } },
#   ifdef INCLUDE_AMP_CPU
    { "sharedMem",          HCF_RES_INT,    { (void *)TM_ANCHOR_ADRS } },
#   endif /* INCLUDE_AMP_CPU */
    };
#   define tsecMdioNum NELEMENTS(tsecMdioResources)

const struct hcfResource dummyMdio0Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)0 } },
    { "fullDuplex",         HCF_RES_INT,    { (void *)1 } },
    { "speed",              HCF_RES_INT,    { (void *)1000 } }
};
#   define dummyMdio0Num NELEMENTS(dummyMdio0Resources)

#endif /* INCLUDE_ETSEC_VXB_END || INCLUDE_ETSEC_VXB_END2 */

#ifdef DRV_RESOURCE_M85XXCCSR
const struct hcfResource m85xxCCSR0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)CCSBAR } },
    { "LAWAR_ENABLE",       HCF_RES_INT,    { (void *)LAWAR_ENABLE } },
    { "LAWBAR_ADRS_SHIFT",  HCF_RES_INT,    { (void *)LAWBAR_ADRS_SHIFT } },
    { "LAWBAR0",            HCF_RES_STRING, { (void *)"reserved" } },
    { "LAWBAR1",            HCF_RES_STRING, { (void *)"reserved" } },
    { "LAWBAR2",            HCF_RES_STRING, { (void *)"reserved" } },
    { "LAWBAR3",            HCF_RES_STRING, { (void *)NULL } },
    { "LAWBAR4",            HCF_RES_STRING, { (void *)NULL } },
    { "LAWBAR5",            HCF_RES_STRING, { (void *)"m85xxPci" } },
    { "LAWBAR6",            HCF_RES_STRING, { (void *)"m85xxPci" } },
    { "LAWBAR7",            HCF_RES_STRING, { (void *)NULL } },
    { "LAWBAR8",            HCF_RES_STRING, { (void *)NULL } },
    { "LAWBAR9",            HCF_RES_STRING, { (void *)NULL } }
};
#   define m85xxCCSR0Num NELEMENTS(m85xxCCSR0Resources)
#endif  /* DRV_RESOURCE_M85XXCCSR */

#ifdef DRV_DMA_FSL
const struct hcfResource fslDma1Resources[] = {
    { "regBase",    HCF_RES_INT,            { (void *)DMA1_REGBASE } },
};
#   define fslDma1Num NELEMENTS(fslDma1Resources)
#endif  /* DRV_DMA_FSL */

#ifdef INCLUDE_PCI_BUS
struct hcfResource pci1Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)PCIEX1_REGBASE } },
    { "mem32Addr",          HCF_RES_ADDR,   { (void *)PCIEX1_MEM_ADRS } },
    { "mem32Size",          HCF_RES_INT,    { (void *)PCIEX1_MEM_SIZE } },
    { "memIo32Addr",        HCF_RES_ADDR,   { (void *)PCIEX1_MEMIO_ADRS } },
    { "memIo32Size",        HCF_RES_INT,    { (void *)PCIEX1_MEMIO_SIZE } },
    { "io32Addr",           HCF_RES_ADDR,   { (void *)PCIEX1_IO_ADRS } },
    { "io32Size",           HCF_RES_INT,    { (void *)PCIEX1_IO_SIZE } },
    { "io16Addr",           HCF_RES_ADDR,   { (void *)PCIEX1_IO_ADRS } },
    { "io16Size",           HCF_RES_INT,    { (void *)PCIEX1_IO_SIZE } },
    { "cacheSize",          HCF_RES_INT,    { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,    { (void *)PCI_LAT_TIMER } },
    { "maxBusSet",          HCF_RES_INT,    { (void *)16 } },
    { "lawBase",            HCF_RES_ADDR,   { (void *)PCIEX1_LAW_BASE } },
    { "lawSize",            HCF_RES_INT,    { (void *)PCIEX1_LAW_SIZE } },
    { "includeFuncSet",     HCF_RES_ADDR,   { (void *)sysPciExAutoConfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR,   { (void *)NULL } },
#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "msiEnable",          HCF_RES_INT,    { (void *)TRUE } }
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#       define pci1Num NELEMENTS(pci1Resources)

struct hcfResource pci2Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)PCIEX2_REGBASE } },
    { "mem32Addr",          HCF_RES_ADDR,   { (void *)PCIEX2_MEM_ADRS } },
    { "mem32Size",          HCF_RES_INT,    { (void *)PCIEX2_MEM_SIZE } },
    { "memIo32Addr",        HCF_RES_ADDR,   { (void *)PCIEX2_MEMIO_ADRS } },
    { "memIo32Size",        HCF_RES_INT,    { (void *)PCIEX2_MEMIO_SIZE } },
    { "io32Addr",           HCF_RES_ADDR,   { (void *)PCIEX2_IO_ADRS } },
    { "io32Size",           HCF_RES_INT,    { (void *)PCIEX2_IO_SIZE } },
    { "io16Addr",           HCF_RES_ADDR,   { (void *)PCIEX2_IO_ADRS } },
    { "io16Size",           HCF_RES_INT,    { (void *)PCIEX2_IO_SIZE } },
    { "cacheSize",          HCF_RES_INT,    { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,    { (void *)PCI_LAT_TIMER } },
    { "maxBusSet",          HCF_RES_INT,    { (void *)16 } },
    { "lawBase",            HCF_RES_ADDR,   { (void *)PCIEX2_LAW_BASE } },
    { "lawSize",            HCF_RES_INT,    { (void *)PCIEX2_LAW_SIZE } },
    { "includeFuncSet",     HCF_RES_ADDR,   { (void *)sysPciExAutoConfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR,   { (void *)NULL } },
#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "msiEnable",          HCF_RES_INT,    { (void *)TRUE } }
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#       define pci2Num NELEMENTS(pci2Resources)

struct hcfResource pci3Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)PCIEX3_REGBASE } },
    { "mem32Addr",          HCF_RES_ADDR,   { (void *)PCIEX3_MEM_ADRS } },
    { "mem32Size",          HCF_RES_INT,    { (void *)PCIEX3_MEM_SIZE } },
    { "memIo32Addr",        HCF_RES_ADDR,   { (void *)PCIEX3_MEMIO_ADRS } },
    { "memIo32Size",        HCF_RES_INT,    { (void *)PCIEX3_MEMIO_SIZE } },
    { "io32Addr",           HCF_RES_ADDR,   { (void *)PCIEX3_IO_ADRS } },
    { "io32Size",           HCF_RES_INT,    { (void *)PCIEX3_IO_SIZE } },
    { "io16Addr",           HCF_RES_ADDR,   { (void *)PCIEX3_IO_ADRS } },
    { "io16Size",           HCF_RES_INT,    { (void *)PCIEX3_IO_SIZE } },
    { "cacheSize",          HCF_RES_INT,    { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,    { (void *)PCI_LAT_TIMER } },
    { "maxBusSet",          HCF_RES_INT,    { (void *)16 } },
    { "lawBase",            HCF_RES_ADDR,   { (void *)PCIEX3_LAW_BASE } },
    { "lawSize",            HCF_RES_INT,    { (void *)PCIEX3_LAW_SIZE } },
    { "includeFuncSet",     HCF_RES_ADDR,   { (void *)sysPciExAutoConfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR,   { (void *)NULL } },
#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "msiEnable",          HCF_RES_INT,    { (void *)TRUE } }
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#       define pci3Num NELEMENTS(pci3Resources)

struct hcfResource m85xxPci0Resources[] = {
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
    { "autoIntRouteSet",    HCF_RES_INT,    { (void *)FALSE } },
    { "includeFuncSet",     HCF_RES_ADDR,   { (void *)sysPciExAutoConfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR,   { (void *)sysPciEx1AutoConfigIntAssign } },
    { "maxBusSet",          HCF_RES_INT,    { (void *)16 } },
    { "pciConfigMechanism", HCF_RES_INT,    { (void *)0 } },

    /* non vxbPciAutoConfig() values */

    { "pimmrBase",          HCF_RES_ADDR,   { (void *)PCI_BRIDGE_PIMMR_BASE_ADRS } },
    { "lclMemAddr",         HCF_RES_ADDR,   { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",      HCF_RES_INT,    { (void *)LOCAL_MEM_SIZE } },
    { "mstrMemBus",         HCF_RES_ADDR,   { (void *)PCI_MSTR_MEM_BUS } },

    /* LAWBAR details */

    { "tgtIf",              HCF_RES_INT,    { (void *)(LAWAR_TGTIF_PCIEX2)} },

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
#   ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "msiEnable",          HCF_RES_INT,    { (void *)TRUE } },
    { "dynamicInterrupts",  HCF_RES_INT,    { (void *)TRUE } },
#   endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#   define m85xxPci0Num NELEMENTS(m85xxPci0Resources)

struct hcfResource m85xxPci1Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)PCIEX2_REGBASE } },
    { "mem32Addr",          HCF_RES_ADDR,   { (void *)PCIEX2_MEM_ADRS } },
    { "mem32Size",          HCF_RES_INT,    { (void *)PCIEX2_MEM_SIZE } },
    { "memIo32Addr",        HCF_RES_ADDR,   { (void *)PCIEX2_MEMIO_ADRS } },
    { "memIo32Size",        HCF_RES_INT,    { (void *)PCIEX2_MEMIO_SIZE } },
    { "io32Addr",           HCF_RES_ADDR,   { (void *)PCIEX2_IO_ADRS } },
    { "io32Size",           HCF_RES_INT,    { (void *)PCIEX2_IO_SIZE } },
    { "io16Addr",           HCF_RES_ADDR,   { (void *)PCIEX2_IO_ADRS } },
    { "io16Size",           HCF_RES_INT,    { (void *)PCIEX2_IO_SIZE } },
    { "fbbEnable",          HCF_RES_INT,    { (void *)TRUE } },
    { "cacheSize",          HCF_RES_INT,    { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,    { (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet",    HCF_RES_INT,    { (void *)FALSE } },
    { "includeFuncSet",     HCF_RES_ADDR,   { (void *)sysPciExAutoConfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR,   { (void *)sysPciEx2AutoConfigIntAssign } },
    { "maxBusSet",          HCF_RES_INT,    { (void *)16 } },
    { "pciConfigMechanism", HCF_RES_INT,    { (void *)0 } },

    /* non vxbPciAutoConfig() values */

    { "pimmrBase",          HCF_RES_ADDR,   { (void *)PCI_BRIDGE_PIMMR_BASE_ADRS } },
    { "lclMemAddr",         HCF_RES_ADDR,   { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",      HCF_RES_INT,    { (void *)LOCAL_MEM_SIZE } },
    { "mstrMemBus",         HCF_RES_ADDR,   { (void *)PCI_MSTR_MEM_BUS } },

    /* LAWBAR details */

    { "tgtIf",              HCF_RES_INT,    { (void *)(LAWAR_TGTIF_PCIEX2)} },

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
#   ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "msiEnable",          HCF_RES_INT,    { (void *)TRUE } },
    { "dynamicInterrupts",  HCF_RES_INT,    { (void *)TRUE } },
#   endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#   define m85xxPci1Num NELEMENTS(m85xxPci1Resources)

#endif /* INCLUDE_PCI_BUS */

const struct hcfResource ns1655x0Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)UART0_BASE } },
    { "clkFreq",            HCF_RES_ADDR,   { (void *)sysClkFreqGet } },
    { "regInterval",        HCF_RES_INT,    { (void *)DUART_REG_ADDR_INTERVAL } }
};
#   define ns1655x0Num NELEMENTS(ns1655x0Resources)

const struct hcfResource ns1655x1Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)UART1_BASE } },
    { "clkFreq",            HCF_RES_ADDR,   { (void *)sysClkFreqGet } },
    { "regInterval",        HCF_RES_INT,    { (void *)DUART_REG_ADDR_INTERVAL } }
};
#   define ns1655x1Num NELEMENTS(ns1655x1Resources)

INT_BANK_DESC epicIntDesc[] = {
    { 0,   EPIC_EX_IRQ_NUM,   EPIC_EX_REG_BASE,   EPIC_EX_REG_INTERVAL,   EPIC_EX_INTERRUPT },
    { 0,   EPIC_IN_IRQ_NUM,   EPIC_IN_REG_BASE,   EPIC_IN_REG_INTERVAL,   EPIC_IN_INTERRUPT },
    { 0,   EPIC_GT_IRQ_NUM,   EPIC_GT_A_REG_BASE, EPIC_GT_REG_INTERVAL,   EPIC_GT_A_INTERRUPT },
    { 0,   EPIC_GT_IRQ_NUM,   EPIC_GT_B_REG_BASE, EPIC_GT_REG_INTERVAL,   EPIC_GT_B_INTERRUPT },
    { 0,   EPIC_MSG_IRQ_NUM,  EPIC_MSG_REG_BASE,  EPIC_MSG_REG_INTERVAL,  EPIC_MSG_INTERRUPT },
    { 0,   EPIC_SMSG_IRQ_NUM, EPIC_SMSG_REG_BASE, EPIC_SMSG_REG_INTERVAL, EPIC_SMSG_INTERRUPT },
    { 251, EPIC_IPI_IRQ_NUM,  EPIC_IPI_REG_BASE,  EPIC_IPI_REG_INTERVAL,  EPIC_IPI_INTERRUPT }
};
struct intrCtlrInputs epicInputs[] = {
    { EPIC_DUART_INT_VEC,       "ns16550",              0,      0 },
    { EPIC_DUART_INT_VEC,       "ns16550",              1,      0 },

#if defined(INCLUDE_ETSEC_VXB_END) || defined(INCLUDE_ETSEC_VXB_END2)
    { EPIC_TSEC1TX_INT_VEC,     "motetsec",             0,      0 },
    { EPIC_TSEC1RX_INT_VEC,     "motetsec",             0,      1 },
    { EPIC_TSEC1ERR_INT_VEC,    "motetsec",             0,      2 },
    { EPIC_TSEC2TX_INT_VEC,     "motetsec",             1,      0 },
    { EPIC_TSEC2RX_INT_VEC,     "motetsec",             1,      1 },
    { EPIC_TSEC2ERR_INT_VEC,    "motetsec",             1,      2 },
    { EPIC_TSEC3TX_INT_VEC,     "motetsec",             2,      0 },
    { EPIC_TSEC3RX_INT_VEC,     "motetsec",             2,      1 },
    { EPIC_TSEC3ERR_INT_VEC,    "motetsec",             2,      2 },
#endif  /* INCLUDE_ETSEC_VXB_END || INCLUDE_ETSEC_VXB_END2 */

#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    { OPENPIC_TIMERA0_INT_VEC,  "openPicTimer",         0,      0 },
    { OPENPIC_TIMERA1_INT_VEC,  "openPicTimer",         0,      1 },
    { OPENPIC_TIMERA2_INT_VEC,  "openPicTimer",         0,      2 },
    { OPENPIC_TIMERA3_INT_VEC,  "openPicTimer",         0,      3 },
    { OPENPIC_TIMERB0_INT_VEC,  "openPicTimer",         1,      0 },
    { OPENPIC_TIMERB1_INT_VEC,  "openPicTimer",         1,      1 },
    { OPENPIC_TIMERB2_INT_VEC,  "openPicTimer",         1,      2 },
    { OPENPIC_TIMERB3_INT_VEC,  "openPicTimer",         1,      3 },
#endif  /* INCLUDE_INTCTLR_DYNAMIC_LIB */

#ifdef INCLUDE_EHCI
    { EPIC_USB1_INT_VEC,        "vxbPlbUsbEhci",        0,      0 },
    { EPIC_USB2_INT_VEC,        "vxbPlbUsbEhci",        1,      0 },
#endif  /* INCLUDE_EHCI */

#ifdef DRV_DMA_FSL
    { EPIC_DMA1_CHAN0_INT_VEC,  "fslDma",               0,      0 },
    { EPIC_DMA1_CHAN1_INT_VEC,  "fslDma",               0,      1 },
    { EPIC_DMA1_CHAN2_INT_VEC,  "fslDma",               0,      2 },
    { EPIC_DMA1_CHAN3_INT_VEC,  "fslDma",               0,      3 },
#endif  /* DRV_DMA_FSL */

#ifdef DRV_STORAGE_SDHC
    { EPIC_SDHC_INT_VEC,        "sdhci",                0,      0 },
#endif  /* DRV_STORAGE_SDHC */

#ifdef DRV_I2CBUS_FSL
    { EPIC_I2C_INT_VEC,         "fslI2c",               0,      0 },
    { EPIC_I2C_INT_VEC,         "fslI2c",               1,      0 },
#endif  /* DRV_I2CBUS_FSL */

#ifdef _WRS_CONFIG_SMP
    { EPIC_VEC_IPI_IRQ0,        "ipi",                  0,      0 },
#endif /* _WRS_CONFIG_SMP */

#ifdef DRV_INTCTLR_QE
    { INUM_QE_HIGH,             "quiccEngineIntCtlr",   0,      0 },
    { INUM_QE_LOW,              "quiccEngineIntCtlr",   0,      1 },
#endif /* DRV_INTCTLR_QE */

#ifdef DRV_RESOURCE_QORIQQMAN
   { EPIC_QMAN_PORTAL0_INT_VEC, "QorIQQman",            0,      0 },
   { EPIC_QMAN_PORTAL1_INT_VEC, "QorIQQman",            0,      1 },
   { EPIC_QMAN_PORTAL2_INT_VEC, "QorIQQman",            0,      2 },
#endif /* DRV_RESOURCE_QORIQQMAN */

#ifdef DRV_RESOURCE_QORIQBMAN
   { EPIC_BMAN_PORTAL0_INT_VEC, "QorIQBman",            0,      0 },
   { EPIC_BMAN_PORTAL1_INT_VEC, "QorIQBman",            0,      1 },
   { EPIC_BMAN_PORTAL2_INT_VEC, "QorIQBman",            0,      2 },
#endif /* DRV_RESOURCE_QORIQBMAN */
};

struct intrCtlrCpu epicCpu[] = {
};

const struct hcfResource epic0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)CCSBAR } },
    { "input",              HCF_RES_ADDR,   { (void *)&epicInputs[0] } },
    { "inputTableSize",     HCF_RES_INT,    { (void *)NELEMENTS(epicInputs) } },
    { "cpuRoute",           HCF_RES_ADDR,   { (void *)&epicCpu[0] } },
    { "cpuRouteTableSize",  HCF_RES_INT,    { (void *)NELEMENTS(epicCpu) } },
    { "exPolar",            HCF_RES_INT,    { (void *)EPIC_INT_ACT_LOW } },
    { "exSense",            HCF_RES_INT,    { (void *)EPIC_SENSE_LVL } },
    { "intDesc_0",          HCF_RES_ADDR,   { (void *)&epicIntDesc[0] } },
    { "intDesc_1",          HCF_RES_ADDR,   { (void *)&epicIntDesc[1] } },
    { "intDesc_2",          HCF_RES_ADDR,   { (void *)&epicIntDesc[2] } },
    { "intDesc_3",          HCF_RES_ADDR,   { (void *)&epicIntDesc[3] } },
    { "intDesc_4",          HCF_RES_ADDR,   { (void *)&epicIntDesc[4] } },
    { "intDesc_5",          HCF_RES_ADDR,   { (void *)&epicIntDesc[5] } },
};
#define epic0Num NELEMENTS(epic0Resources)

/*
 * On the Freescale P1025RDB board, the 2 UEC net port PHYs are physically
 * wired according to the following table:
 *
 * Data port pins       MII address     mode
 * --------------       -----------     -----------
 * PHY0: UCC1           6               MII
 * PHY1: UCC5           3               RMII
 *
 * 1. All the 2 PHYs have their management pins connected to UCC5.
 *
 * 2. The UCC1/UCC5 is used as qefcc0/qefcc1 by default.
 */

#ifdef DRV_INTCTLR_QE
const QE_SIU_INT_MASK qeEnableHighPriInts[QE_IVEC_MAX] = {
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 },
    { 0x00000000,         0x00000000 }
 };

const UINT32 qeINumToMaskPattern[QE_IVEC_MAX] = {
    0x00000000,          /* interrupt number 0: Error */
    0x00008000,          /* interrupt number 1: SPI2 */
    0x00004000,          /* interrupt number 2: SPI */
    0x00002000,          /* interrupt number 3: Risc Timer Table */
    0x00000000,          /* interrupt number 4: */
    0x00000000,          /* interrupt number 5: */
    0x00000000,          /* interrupt number 6:  */
    0x00000000,          /* interrupt number 7:  */
    0x00000000,          /* interrupt number 8:  */
    0x00000000,          /* interrupt number 9:  */
    0x00000040,          /* interrupt number 10: SDMA */
    0x00000020,          /* interrupt number 11: USB */
    0x00000010,          /* interrupt number 12: Timer 1 */
    0x00000008,          /* interrupt number 13: Timer 2 */
    0x00000004,          /* interrupt number 14: Timer 3 */
    0x00000002,          /* interrupt number 15: Timer 4 */
    0x00000000,          /* interrupt number 16: */
    0x00000000,          /* interrupt number 17: */
    0x00000000,          /* interrupt number 18: */
    0x00000000,          /* interrupt number 19: */
    0x10000000,          /* interrupt number 20: VT*/
    0x00000000,          /* interrupt number 21: */
    0x00000000,          /* interrupt number 22: */
    0x00000000,          /* interrupt number 23: */
    0x00000000,          /* interrupt number 24: */
    0x00800000,          /* interrupt number 25: EXT1 */
    0x00400000,          /* interrupt number 26: EXT2 */
    0x00200000,          /* interrupt number 27: EXT3 */
    0x00100000,          /* interrupt number 28: EXT4 */
    0x00000000,          /* interrupt number 29: */
    0x00000000,          /* interrupt number 30: */
    0x00000000,          /* interrupt number 31: */
    0x80000000,          /* interrupt number 32: UCC1 */
    0x40000000,          /* interrupt number 33: UCC2 */
    0x20000000,          /* interrupt number 34: UCC3 */
    0x10000000,          /* interrupt number 35: UCC4 */
    0x08000000,          /* interrupt number 36: MCC1 */
    0x00000000,          /* interrupt number 37: */
    0x00000000,          /* interrupt number 38: */
    0x00000000,          /* interrupt number 39: */
    0x00800000,          /* interrupt number 40: UCC5 */
    0x00400000,          /* interrupt number 41: UCC6 */
    0x00200000,          /* interrupt number 42: UCC7 */
    0x00100000,          /* interrupt number 43: UCC8 */
    0x00080000,          /* interrupt number 44: MCC2*/
    0x00000000,          /* interrupt number 45: */
    0x00000000,          /* interrupt number 46: */
    0x00000000,          /* interrupt number 47: */
    0x00000000,          /* interrupt number 48: */
    0x00000000,          /* interrupt number 49: */
    0x00000000,          /* interrupt number 50: */
    0x00000000,          /* interrupt number 51: */
    0x00000000,          /* interrupt number 52: */
    0x00000000,          /* interrupt number 53: */
    0x00000000,          /* interrupt number 54: */
    0x00000000,          /* interrupt number 55: */
    0x00000000,          /* interrupt number 56: reserved */
    0x00000000,          /* interrupt number 57: reserved */
    0x00000000,          /* interrupt number 58: reserved */
    0x00000000,          /* interrupt number 59: reserved */
    0x00000000,          /* interrupt number 60: reserved */
    0x00000000,          /* interrupt number 61: reserved */
    0x00000000,          /* interrupt number 62: reserved */
    0x00000000           /* interrupt number 63: reserved */
 };

const QE_IOPORT_TABLE qeIoTable[] = {

    /* UCC_7_UART */

    { QE_PORTA,  0x16,  2,  0,  2 },   /* SER7_CD       */
    { QE_PORTA,  0x12,  2,  0,  2 },   /* SER7_CTS      */
    { QE_PORTA,  0x13,  1,  0,  2 },   /* SER7_RTS      */
    { QE_PORTA,  0x14,  2,  0,  2 },   /* SER7_RXD[0]   */
    { QE_PORTA,  0x15,  1,  0,  2 },   /* SER7_TXD[0]   */

    /* UCC_3_UART */

    { QE_PORTB,  0x00,  2,  0,  2 },   /* SER3_CD       */
    { QE_PORTA,  0x1c,  2,  0,  2 },   /* SER3_CTS      */
    { QE_PORTA,  0x1d,  1,  0,  2 },   /* SER3_RTS      */
    { QE_PORTA,  0x1e,  2,  0,  2 },   /* SER3_RXD[0]   */
    { QE_PORTA,  0x1f,  1,  0,  2 },   /* SER3_TXD[0]   */

    /* MII_CFG_QE_MUX */

    { QE_PORTB,  0x13,  1,  0,  1 },   /* MDC  */
    { QE_PORTB,  0x14,  3,  0,  1 },   /* MDIO */

    /* UCC_1_MII */

    { QE_PORTA,  0x17,  2,  0,  2 },   /* 16.Enet1_RXCLK ->clk12 */
    { QE_PORTA,  0x18,  2,  0,  1 },   /* 15.Enet1_TXCLK -> clk9 */
    { QE_PORTA,  0x07,  1,  0,  2 },   /* 1.Enet1_TxD0   */
    { QE_PORTA,  0x09,  1,  0,  2 },   /* 2.Enet1_TxD1   */
    { QE_PORTA,  0x0b,  1,  0,  2 },   /* 3.Enet1_TxD2   */
    { QE_PORTA,  0x0c,  1,  0,  2 },   /* 4.Enet1_TxD3   */
    { QE_PORTA,  0x06,  2,  0,  2 },   /* 5.Enet1_RxD0   */
    { QE_PORTA,  0x0a,  2,  0,  2 },   /* 6.Enet1_RxD1   */
    { QE_PORTA,  0x0e,  2,  0,  2 },   /* 7.Enet1_RxD2   */
    { QE_PORTA,  0x0f,  2,  0,  2 },   /* 8.Enet1_RxD3   */
    { QE_PORTA,  0x05,  1,  0,  2 },   /* 9.Enet1_TX_EN  */
    { QE_PORTA,  0x0d,  1,  0,  2 },   /* 10.Enet1_TX_ER */
    { QE_PORTA,  0x04,  2,  0,  2 },   /* 12.Enet1_RX_DV */
    { QE_PORTA,  0x08,  2,  0,  2 },   /* 11.Enet1_RX_ER */
    { QE_PORTA,  0x11,  2,  0,  2 },   /* 14.Enet1_CRS   */
    { QE_PORTA,  0x10,  2,  0,  2 },   /* 13.Enet1_COL   */

    /* UCC_5_RMII */

    { QE_PORTB,  0xb,  2,  0,  1 },    /* 8.Enet2_REF_CLK -> clk13*/
    { QE_PORTB,  0x7,  1,  0,  2 },    /* 1.Enet2_TxD0  */
    { QE_PORTB,  0xa,  1,  0,  2 },    /* 4.Enet2_TxD1  */
    { QE_PORTB,  0x6,  2,  0,  2 },    /* 3.Enet2_RxD0  */
    { QE_PORTB,  0x9,  2,  0,  2 },    /* 2.Enet2_RxD1  */
    { QE_PORTB,  0x5,  1,  0,  2 },    /* 5.Enet2_TX_EN */
    { QE_PORTB,  0x4,  2,  0,  2 },    /* 7.Enet2_RX_DV */
    { QE_PORTB,  0x8,  2,  0,  2 },    /* 6.Enet2_RX_ER */
};

struct intrCtlrInputs quiccEngineIntCtlrInputs[] = {
    { QE_IC_INTR_UCC_1,     "qefcc",    0,  0 },
    { QE_IC_INTR_UCC_5,     "qefcc",    1,  0 },
    { QE_IC_INTR_UCC_7,     "qeUart",   2,  0 },
    { QE_IC_INTR_UCC_3,     "qeUart",   3,  0 },
};

const struct hcfResource quiccEngineResources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)(QUICC_ENGINE_BASE) } },
    { "regBase1",           HCF_RES_INT,    { (void *)(CCSBAR + QE_IO_PORTS) } },
    { "muRamBase",          HCF_RES_INT,    { (void *)(QE_MURAM_POOL_BA)}},
    { "muRamPoolSize",      HCF_RES_INT,    { (void *) 0x6000 } },
    { "firmwareAdrs",       HCF_RES_INT,    { (void *) QE_MICROCODE_ADDR } },
    { "qeIoTable",          HCF_RES_ADDR,   { (void *)&qeIoTable[0] } },
    { "qeIoTableSize",      HCF_RES_INT,    { (void *)NELEMENTS(qeIoTable) } }
};
#   define quiccEngine0Num NELEMENTS(quiccEngineResources)

const struct hcfResource quiccEngineIntCtlr0Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *) (QUICC_ENGINE_BASE) } },
    { "input",              HCF_RES_ADDR,   { (void *) &quiccEngineIntCtlrInputs[0] } },
    { "inputTableSize",     HCF_RES_INT,    { (void *) NELEMENTS(quiccEngineIntCtlrInputs) } },
    { "intMaskTable",       HCF_RES_ADDR,   { (void *) &qeEnableHighPriInts[0] } },
    { "intMaskTableSize",   HCF_RES_INT,    { (void *) QE_IVEC_MAX } },
    { "inumMaskTable",      HCF_RES_ADDR,   { (void *) &qeINumToMaskPattern[0] } },
    { "inumMaskTableSize",  HCF_RES_INT,    { (void *) QE_IVEC_MAX } }
 };
#   define quiccEngineIntCtlr0Num NELEMENTS(quiccEngineIntCtlr0Resources)

#   ifdef   DRV_SIO_UCC
struct hcfResource uccSioDev0Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *) (QUICC_ENGINE_BASE + UCC7_BASE) } },
    { "regBase1",           HCF_RES_INT,    { (void *) (QUICC_ENGINE_BASE) } },
    { "uccNum",             HCF_RES_INT,    { (void *) 7 } },
    { "qeFreq",             HCF_RES_ADDR,   { (void *) &sysClkFreqGet } },
    { "rxBrgNum",           HCF_RES_INT,    { (void *) 6 } },
    { "txBrgNum",           HCF_RES_INT,    { (void *) 6 } }
};
#       define uccSio0Num NELEMENTS(uccSioDev0Resources)

struct hcfResource uccSioDev1Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *) (QUICC_ENGINE_BASE + UCC3_BASE) } },
    { "regBase1",           HCF_RES_INT,    { (void *) (QUICC_ENGINE_BASE) } },
    { "uccNum",             HCF_RES_INT,    { (void *) 3 } },
    { "qeFreq",             HCF_RES_ADDR,   { (void *) &sysClkFreqGet } },
    { "rxBrgNum",           HCF_RES_INT,    { (void *) 2 } },
    { "txBrgNum",           HCF_RES_INT,    { (void *) 2 } }
};
#       define uccSio1Num NELEMENTS(uccSioDev1Resources)
#   endif /* DRV_SIO_UCC */

#   ifdef  INCLUDE_UCC_VXB_END
const struct hcfResource qefcc0Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *) (QUICC_ENGINE_BASE + UCC1_BASE) } },
    { "uccNum",             HCF_RES_INT,    { (void *) 1 } },
    { "phyAddr",            HCF_RES_INT,    { (void *) 6 } },
    { "uccMedia",           HCF_RES_INT,    { (void *) UCC_MEDIA_MII_GMII } },
    { "miiIfName",          HCF_RES_STRING, { (void *) "uccMdio" } }, 
    { "miiIfUnit",          HCF_RES_INT,    { (void *) 0 } }
 };
#       define qefcc0Num   NELEMENTS(qefcc0Resources)

const struct hcfResource qefcc1Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *) (QUICC_ENGINE_BASE + UCC5_BASE) } },
    { "uccNum",             HCF_RES_INT,    { (void *) 5 } },
    { "phyAddr",            HCF_RES_INT,    { (void *) 3 } },
    { "uccMedia",           HCF_RES_INT,    { (void *) UCC_MEDIA_RMII } },
    { "miiIfName",          HCF_RES_STRING, { (void *) "uccMdio" } }, 
    { "miiIfUnit",          HCF_RES_INT,    { (void *) 0 } }
 };
#       define qefcc1Num   NELEMENTS(qefcc1Resources)
#   endif /* INCLUDE_UCC_VXB_END */

#   ifdef  INCLUDE_UCC_MDIO
const struct hcfResource uccMdioResources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)(QUICC_ENGINE_BASE + UCC5_BASE) } },
    { "umMiiExpose",        HCF_RES_ADDR,   { (void *)umMiiExpose} },
    { "umMiiRelease",       HCF_RES_ADDR,   { (void *)umMiiRelease } },
};
#       define uccMdioNum NELEMENTS(uccMdioResources)
#   endif /* INCLUDE_UCC_MDIO */
#endif /* DRV_INICTLR_QE */

#ifdef DRV_TIMER_M85XX

struct hcfResource m85xxTimerResources[] =  {
    {"regBase",             HCF_RES_INT,    { (void *)0 } },
    {"decMinClkRate",       HCF_RES_INT,    { (void *) SYS_CLK_RATE_MIN} },
    {"decMaxClkRate",       HCF_RES_INT,    { (void *)SYS_CLK_RATE_MAX } },
#   ifdef INCLUDE_AUX_CLK
    {"fitMinClkRate",       HCF_RES_INT,    { (void *)P1025RDB_FIT_CLK_RATE_MIN } },
    {"fitMaxClkRate",       HCF_RES_INT,    { (void *)P1025RDB_FIT_CLK_RATE_MAX } },
#   endif /* INCLUDE_AUX_CLK */
    {"sysClockFreq",        HCF_RES_ADDR,   { (void *)&sysClkFreqGet } },
};
#   define m85xxTimerNum   NELEMENTS(m85xxTimerResources)

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
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)EHCI_CAPLENGTH(USB1_BASE) } },
    { "ehciInit",           HCF_RES_ADDR,   { (void *)ehci0Init } },
};
#   define usbEhci0Num NELEMENTS(usbEhci0Resources)

#endif /* INCLUDE_EHCI */

#ifdef DRV_TIMER_OPENPIC
const struct hcfResource openPicTimerDevAresources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)(CCSBAR + 0x000410F0) } },
    { "instanceName",       HCF_RES_STRING, { (void *)"A" } },
    { VXB_CLK_FREQ,         HCF_RES_ADDR,   { (void *)sysPicClkFreqGet } },
    { "clkDivisor",         HCF_RES_INT,    { (void *)8 } },
    { "maxClkRate0",        HCF_RES_INT,    { (void *)SYS_CLK_RATE_MAX } },
#   ifdef INCLUDE_AUX_CLK
    { "maxClkRate1",        HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX } },
    { "maxClkRate2",        HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX } },
    { "maxClkRate3",        HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX } },
#   endif /* INCLUDE_AUX_CLK */
#   ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "vector0",            HCF_RES_INT,    { (void *)OPENPIC_TIMERA0_INT_VEC } },
    { "vector1",            HCF_RES_INT,    { (void *)OPENPIC_TIMERA1_INT_VEC } },
    { "vector2",            HCF_RES_INT,    { (void *)OPENPIC_TIMERA2_INT_VEC } },
    { "vector3",            HCF_RES_INT,    { (void *)OPENPIC_TIMERA3_INT_VEC } },
#   endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#   define openPicTimerDevAnum NELEMENTS(openPicTimerDevAresources)

const struct hcfResource openPicTimerDevBresources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)(CCSBAR + 0x000420F0) } },
    { "instanceName",       HCF_RES_STRING, { (void *)"B" } },
    { VXB_CLK_FREQ,         HCF_RES_ADDR,   { (void *)sysPicClkFreqGet } },
    { "clkDivisor",         HCF_RES_INT,    { (void *)8 } },
#   ifdef INCLUDE_AUX_CLK
    { "maxClkRate0",        HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX } },
    { "maxClkRate1",        HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX } },
    { "maxClkRate2",        HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX } },
    { "maxClkRate3",        HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX } },
#   endif /* INCLUDE_AUX_CLK */
#   ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "vector0",            HCF_RES_INT,    { (void *)OPENPIC_TIMERB0_INT_VEC } },
    { "vector1",            HCF_RES_INT,    { (void *)OPENPIC_TIMERB1_INT_VEC } },
    { "vector2",            HCF_RES_INT,    { (void *)OPENPIC_TIMERB2_INT_VEC } },
    { "vector3",            HCF_RES_INT,    { (void *)OPENPIC_TIMERB3_INT_VEC } },
#   endif  /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#   define openPicTimerDevBnum NELEMENTS(openPicTimerDevBresources)
#endif   /* DRV_TIMER_OPENPIC */

#ifdef DRV_STORAGE_SDHC

struct hcfResource sdhciResources[] =  {
    { "regBase",            HCF_RES_INT,    { (void *)SDHC_BASE } },
    { "clkFreq",            HCF_RES_ADDR,   { (void *)sysPlbClkFreqGet } },
    { "dmaMode",            HCF_RES_INT,    { (void *)0 } },
    { "polling",            HCF_RES_INT,    { (void *)FALSE } },
    { "flags"  ,            HCF_RES_INT,    { (void *)(SDHC_PIO_NEED_DELAY | SDHC_HW_SNOOP |
                                                       SDHC_FIFO_ENDIANESS_REVERSE |
                                                       SDHC_HOST_VER_REVERSE |
                                                       SDHC_HOST_CTRL_FREESCALE) } },
};
#   define sdhciNum  NELEMENTS(sdhciResources)
#endif  /* DRV_STORAGE_SDHC */

#ifdef DRV_VXB_NORFLASH
struct hcfResource norFlash0Resources[]= {
    { "regBase",            HCF_RES_INT,    { (void *)(FLASH_BASE_PART1) } },
    { "maxSize",            HCF_RES_INT,    { (void *)(FLASH_SIZE_PART1) } },
#   if (defined(INCLUDE_UCC_VXB_END) || defined(DRV_SIO_UCC))
    { "wrEna",              HCF_RES_ADDR,   { (void *)sysLocalBusAccessEnable } },
#   endif /* INCLUDE_UCC_VXB_END || DRV_SIO_UCC */
#   ifdef INCLUDE_AMP_CPU
    { "sharedMem",          HCF_RES_INT,    { (void *)(TM_ANCHOR_ADRS + _CACHE_ALIGN_SIZE) } },
#   endif /* INCLUDE_AMP_CPU */
    };
#   define norFlash0Num NELEMENTS(norFlash0Resources)
#endif /* DRV_VXB_NORFLASH */

#ifdef DRV_I2CBUS_FSL

/*
 * On P1025RDB board, I2C Eeprom M24256, I2C RTC PTC4338 and I2C I/O expander
 * device PCA9557 are connected to I2C bus 1.
 */

/*
 * Address  Part Number Size        Bus     Description
 * -------  ----------- --------    ---     --------------
 * 0x50     M24256      32K-Bytes   0       Boot sequencer eeprom.
 * 0x68     PT7C4338                0       Real time clock.
 * 0x23     PCA9555                 0       16-bit I2C AND SMBus I/O EXPANDER
 */

struct i2cDevInputs p1025rdbI2c0DevTbl[] = {
    { "gpio_pca9555",       0x23,   I2C_BYTE_WR_ONLY | I2C_BYTE_RD_ONLY},
    { "eeprom_M24256",      0x50,   I2C_WORDADDR     | I2C_RD_EXT_ACK},
    { "rtc_pt7c4338",       0x68,   0},
};

/*
 * On P1021RDB and P1024RDB boards, I2C Eeprom M24256 and I2C RTC PTC4338 are
 * connected to I2C bus 1.
 */

/*
 * Address  Part Number Size        Bus     Description
 * -------  ----------- --------    ---     --------------
 * 0x50     M24256      32K-Bytes   0       Boot sequencer eeprom.
 * 0x68     PT7C4338                0       Real time clock.
 */

struct i2cDevInputs i2c0DevTbl[] = {
    { "eeprom_M24256",      0x50,   I2C_WORDADDR     | I2C_RD_EXT_ACK},
    { "rtc_pt7c4338",       0x68,   0},
};

struct hcfResource i2c0Resources[] = {
    { "regBase",            HCF_RES_ADDR,   { (void *)(I2C_BUS1_BASE) } },
    { "clkFreq",            HCF_RES_ADDR,   { (void *)sysPlbClkFreqGet } },
    { "busSpeed",           HCF_RES_INT,    { (void *)400000 } },
    { "polling",            HCF_RES_INT,    { (void *)FALSE } },
    { "i2cDev",             HCF_RES_ADDR,   { (void *)&i2c0DevTbl} },
    { "i2cDevNum",          HCF_RES_INT,    { (void *)NELEMENTS(i2c0DevTbl)}},
};
#   define i2c0Num NELEMENTS(i2c0Resources)

/* 
 * On P1021RDB and P1025RDB boards, I2C Eeprom CAT24C05 and I2C 8-bit I/O expander
 * PCA9557 are connected to I2C bus2. I2C Eeprom CAT24C05 has little difference with
 * ATMEL's AT24C04, so driver for Eeprom AT24xx is used here. When system probed
 * the I2C bus, two device will be find, their device address are shown as 0x52 and
 * 0x53. This phenomenon is caused by one of the CAT24C05's slave address bites is
 * used for storing the I2C Eeprom's word address.
 */

/*
 * Address  Part Number Size        Bus     Description
 * -------  ----------- --------    ---     --------------
 * 0x18     PCA9557                 1       16-bit I2C AND SMBus I/O EXPANDER
 * 0x52     AT24C02     256-Bytes   1       I2C EEPROM
 * 0x53     AT24C02     256-Bytes   1       I2C EEPROM
 */

struct i2cDevInputs i2c1DevTbl[] = {
    { "gpio_pca9557",       0x18,  I2C_BYTE_WR_ONLY | I2C_BYTE_RD_ONLY},
    { "eeprom_at24c02",     0x52,  0},
    { "eeprom_at24c02",     0x53,  0},
};

/*
 * On P1024RDB board, I2C Eeprom AT24C01 and I2C I/O expander device PCA9557
 * are connected to I2C bus 2.
 */

/*
 * Address  Part Number Size        Bus     Description
 * -------  ----------- --------    ---     --------------
 * 0x18     PCA9555                 1       8-bit I2C AND SMBus I/O EXPANDER
 * 0x52     AT24C01     128-Bytes   1       I2C EEPROM
 */

struct i2cDevInputs p1024rdbI2c1DevTbl[] = {
    { "gpio_pca9557",       0x18,  I2C_BYTE_WR_ONLY | I2C_BYTE_RD_ONLY},
    { "eeprom_at24c01",     0x52,  0},
};

struct hcfResource i2c1Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)(I2C_BUS2_BASE) } },
    { "clkFreq",            HCF_RES_ADDR,   { (void *)sysPlbClkFreqGet } },
    { "busSpeed",           HCF_RES_INT,    { (void *)400000 } },
    { "polling",            HCF_RES_INT,    { (void *)FALSE } },
    { "i2cDev",             HCF_RES_ADDR,   { (void *)&i2c1DevTbl } },
    { "i2cDevNum",          HCF_RES_INT,    { (void *)NELEMENTS(i2c1DevTbl) } },
};
#   define i2c1Num NELEMENTS(i2c1Resources)

#endif /* DRV_I2CBUS_FSL */

#ifdef DRV_VXBEND_DTSEC

const struct hcfResource dtsec0Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)(CCSBAR + 0x1E0000) } },
    { "phyAddr",    HCF_RES_INT,    { (void *)DTSEC0_PHY_ADDR } },
    { "miiIfName",  HCF_RES_STRING, { (void *)"dtsecMdio" } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)0 } },
    { "dtsecNum",   HCF_RES_INT,    { (void *)0 } },
    { "fmanNum",    HCF_RES_INT,    { (void *)0 } },
    { "dtsecMedia", HCF_RES_INT,    { (void *)DTSEC_MEDIA_RGMII } },
#   ifdef INCLUDE_AMP_CPU_00
    { "coreNum",    HCF_RES_INT,    { (void *)0 } },
#   endif /* INCLUDE_AMP_CPU_00 */
};
#define dtsec0Num NELEMENTS(dtsec0Resources)

const struct hcfResource dtsec1Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)(CCSBAR + 0x1E2000) } },
    { "phyAddr",    HCF_RES_INT,    { (void *)DTSEC1_PHY_ADDR } },
    { "miiIfName",  HCF_RES_STRING, { (void *)"dtsecMdio" } },
    { "miiIfUnit",  HCF_RES_INT,    { (void *)0 } },
    { "dtsecNum",   HCF_RES_INT,    { (void *)1 } },
    { "fmanNum",    HCF_RES_INT,    { (void *)0 } },
    { "dtsecMedia", HCF_RES_INT,    { (void *)DTSEC_MEDIA_RGMII } },
#   ifdef INCLUDE_AMP_CPU_01
    { "coreNum",    HCF_RES_INT,    { (void *)1 } },
#   endif /* INCLUDE_AMP_CPU_00 */
};
#define dtsec1Num NELEMENTS(dtsec1Resources)

#endif /* DRV_VXBEND_DTSEC */

#ifdef  DRV_MII_DTSEC_MDIO

const struct hcfResource dtsecMdio0Resources[] = {
    { "regBase",    HCF_RES_INT,    { (void *)(CCSBAR + 0x1E1000) } },
#   ifdef INCLUDE_AMP
    { "sharedMem",  HCF_RES_INT,    { (void *)TM_ANCHOR_ADRS } },
#   endif /* INCLUDE_AMP */
};
#   define dtsecMdio0Num NELEMENTS(dtsecMdio0Resources)

#endif  /* DRV_MII_DTSEC_MDIO */

#ifdef DRV_RESOURCE_QORIQFMAN

struct hcfResource fman0Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,    { (void *)(CCSBAR + 0x100000) } },
#   if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_00)
    { "globalInit",     HCF_RES_INT,    { (void *)1 } },
    { "ucodeAddr",      HCF_RES_ADDR,   { (void *)NULL } }
#   endif
};
#   define fman0Num NELEMENTS(fman0Resources)

#endif /* DRV_RESOURCE_QORIQFMAN */

#ifdef DRV_RESOURCE_QORIQQMAN
const struct hcfResource qman0Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,    { (void *)(CCSBAR + 0x88000) } },
    { "qmanLawBase",    HCF_RES_ADDR,   { (void *)QMAN_LAW_BASE} },
#   if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_00)
    { "qmanMemBase",    HCF_RES_ADDR,   { (void *)QMAN_MEM_BASE} },
#   endif
};

#   define qman0Num NELEMENTS(qman0Resources)

#endif /* DRV_RESOURCE_QORIQQMAN */

#ifdef DRV_RESOURCE_QORIQBMAN

const struct hcfResource bman0Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,    { (void *)(CCSBAR + 0x8A000) } },
    { "bmanLawBase",    HCF_RES_ADDR,   { (void *)BMAN_LAW_BASE } },
#   if !defined (INCLUDE_AMP) || defined   (INCLUDE_AMP_CPU_00)
    { "bmanMemBase",    HCF_RES_ADDR,   { (void *)BMAN_MEM_BASE } },
#   endif
};
#   define bman0Num NELEMENTS(bman0Resources)

#endif /* DRV_RESOURCE_QORIQBMAN */

const struct hcfDevice hcfDeviceList[] = {

    /*
     * Initialize ppcIntCtlr before epic.
     * The vector table for external interrupts are over written by epic
     * for an optimized purpose.
     */

    { "ppcIntCtlr",     0,  VXB_BUSID_PLB,  0,  ppcIntCtlr0Num, ppcIntCtlr0Resources },
    { "epic",           0,  VXB_BUSID_PLB,  0,  epic0Num,       epic0Resources },

#if defined(INCLUDE_QUICC_ENGINE_UTILS) || defined(DRV_INTCTLR_QE)
    { "quiccEngineIntCtlr", 0,  VXB_BUSID_PLB, 0, quiccEngineIntCtlr0Num, quiccEngineIntCtlr0Resources },
    { "quiccEngine",        0,  VXB_BUSID_PLB, 0, quiccEngine0Num,        quiccEngineResources },
#endif /* (INCLUDE_QUICC_ENGINE_UTILS) || (DRV_INTCTLR_QE) */

#ifdef DRV_VXB_NORFLASH
    { "norFlash",       0,  VXB_BUSID_PLB,  0,  norFlash0Num,   norFlash0Resources},
#endif /* DRV_VXB_NORFLASH */

#ifdef DRV_I2CBUS_FSL
    { "fslI2c",         0,  VXB_BUSID_PLB,  0,  i2c0Num,        i2c0Resources },
    { "fslI2c",         1,  VXB_BUSID_PLB,  0,  i2c1Num,        i2c1Resources },
#endif /* DRV_I2CBUS_FSL */

#if defined(INCLUDE_ETSEC_VXB_END) || defined(INCLUDE_ETSEC_VXB_END2)
    { "motetsec",       0,  VXB_BUSID_PLB,  0,  etsec0Num,      etsec0Resources },
    { "motetsec",       1,  VXB_BUSID_PLB,  0,  etsec1Num,      etsec1Resources },
    { "motetsec",       2,  VXB_BUSID_PLB,  0,  etsec2Num,      etsec2Resources },
    { "dummyMdio",      0,  VXB_BUSID_PLB,  0,  dummyMdio0Num,  dummyMdio0Resources },
    { "tsecMdio",       0,  VXB_BUSID_PLB,  0,  tsecMdioNum,    tsecMdioResources },
#endif /* INCLUDE_ETSEC_VXB_END || INCLUDE_ETSEC_VXB_END2 */

#ifdef INCLUDE_UCC_VXB_END
    { "qefcc",          0,  VXB_BUSID_PLB,  0,  qefcc0Num,      qefcc0Resources },
    { "qefcc",          1,  VXB_BUSID_PLB,  0,  qefcc1Num,      qefcc1Resources },
#endif /* INCLUDE_UCC_VXB_END */

#ifdef INCLUDE_UCC_MDIO
    { "uccMdio",        0,  VXB_BUSID_PLB,  0,  uccMdioNum,     uccMdioResources },
#endif /* INCLUDE_UCC_MDIO */

#ifdef DRV_SIO_NS16550
    { "ns16550",        0,  VXB_BUSID_PLB,  0,  ns1655x0Num,    ns1655x0Resources },
    { "ns16550",        1,  VXB_BUSID_PLB,  0,  ns1655x1Num,    ns1655x1Resources },
#endif  /* DRV_SIO_NS16550 */

/* QE UART must be initialized after quiccEngine */

#ifdef DRV_SIO_UCC
    { "qeUart",         2,  VXB_BUSID_PLB,  0,  uccSio0Num,     uccSioDev0Resources },
    { "qeUart",         3,  VXB_BUSID_PLB,  0,  uccSio1Num,     uccSioDev1Resources },
#endif /* DRV_SIO_UCC */

#ifdef DRV_RESOURCE_M85XXCCSR
    { "m85xxCCSR",      0,  VXB_BUSID_PLB,  0,  m85xxCCSR0Num,  m85xxCCSR0Resources },
#endif  /* DRV_RESOURCE_M85XXCCSR */

#ifdef DRV_TIMER_M85XX
    { "m85xxTimerDev",  0,  VXB_BUSID_PLB,  0,  m85xxTimerNum,  m85xxTimerResources },
#endif /* DRV_TIMER_M85XX */

#ifdef DRV_TIMER_OPENPIC
    { "openPicTimer",   0,  VXB_BUSID_PLB,  0,  openPicTimerDevAnum, openPicTimerDevAresources },
    { "openPicTimer",   1,  VXB_BUSID_PLB,  0,  openPicTimerDevBnum, openPicTimerDevBresources },
#endif /* DRV_TIMER_OPENPIC */

#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci",  0,  VXB_BUSID_PLB,  0,  usbEhci0Num,    usbEhci0Resources },
#endif  /* INCLUDE_EHCI */

#ifdef DRV_DMA_FSL
    { "fslDma",         0,  VXB_BUSID_PLB,  0,  fslDma1Num,     fslDma1Resources },
#endif /* DRV_DMA_FSL */

#ifdef DRV_STORAGE_SDHC
    { "sdhci",          0,  VXB_BUSID_PLB,  0,  sdhciNum,       sdhciResources },
#endif  /* DRV_STORAGE_SDHC */

#ifdef DRV_RESOURCE_QORIQQMAN
    { "QorIQQman",      0,  VXB_BUSID_PLB,  0,  qman0Num,       qman0Resources },
#endif /* DRV_RESOURCE_QORIQQMAN */
#ifdef DRV_RESOURCE_QORIQBMAN
    { "QorIQBman",      0,  VXB_BUSID_PLB,  0,  bman0Num,       bman0Resources },
#endif /* DRV_RESOURCE_QORIQBMAN */

#ifdef DRV_RESOURCE_QORIQFMAN
    { "QorIQFman",      0,  VXB_BUSID_PLB,  0,  fman0Num,       fman0Resources },
#endif /* DRV_RESOURCE_QORIQFMAN */

#ifdef DRV_VXBEND_DTSEC
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_00)
    { "dtsec",          0,  VXB_BUSID_PLB,  0,  dtsec0Num,      dtsec0Resources },
#endif
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_01)
    { "dtsec",          1,  VXB_BUSID_PLB,  0,  dtsec1Num,      dtsec1Resources },
#endif
#   ifdef DRV_MII_DTSEC_MDIO
    { "dtsecMdio",      0,  VXB_BUSID_PLB,  0,  dtsecMdio0Num,  dtsecMdio0Resources },
#   endif /* DRV_MII_DTSEC_MDIO */
#endif /* DRV_VXBEND_DTSEC */

#ifdef INCLUDE_PCI_BUS
    { "m85xxPci",       0,  VXB_BUSID_PLB,  0,  m85xxPci0Num,   m85xxPci0Resources },
    { "m85xxPci",       1,  VXB_BUSID_PLB,  0,  m85xxPci1Num,   m85xxPci1Resources },
    { "QorIQPciEx", QORIQ_PCIEX1_UNIT_NUM,  VXB_BUSID_PLB,  0,  pci1Num,    pci1Resources },
    { "QorIQPciEx", QORIQ_PCIEX2_UNIT_NUM,  VXB_BUSID_PLB,  0,  pci2Num,    pci2Resources },
    { "QorIQPciEx", QORIQ_PCIEX3_UNIT_NUM,  VXB_BUSID_PLB,  0,  pci3Num,    pci3Resources },
#endif  /* INCLUDE_PCI_BUS */

};

const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] = {
#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci",  0,  "hasEmbeddedTT", VXB_PARAM_INT32,    { (void *)TRUE } },
    { "vxbPlbUsbEhci",  0,  "postResetHook", VXB_PARAM_FUNCPTR,  { (void *)ehci0PostResetHook } },
#endif  /* INCLUDE_EHCI */
    { NULL,            0,  NULL,           VXB_PARAM_END_OF_LIST, { (void *)0 } }
};

