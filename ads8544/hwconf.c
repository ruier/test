/* hwconf.c - Hardware configuration support module */

/*
 * Copyright (c) 2006-2009, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
-------------------- 
01q,12jan12,agf  remove exIrqWkrd from epic resource, deprecated in driver
                 (WIND00176018)
01p,05jan09,b_m  remove openPicTimer vectors.
01o,23oct08,d_l  Add OpenPic timer vector support. (WIND00140273)
01n,03sep08,b_m  remove booke timer vectors.
01m,07aug08,x_f  Update UART1 interrupt vector.
01l,14jul08,dtr  Remove dynamicInterrupts table.
01k,06jun08,dlk  Make entries for e/tsec devices unconditional.
                 Support END2 versions of the drivers also.
01j,04mar08,h_k  swap the init order between epic and ppcIntCtlr.
01i,23aug07,dtr  Use vxb etsec driver. Include support for latest MSI.
01h,01aug07,h_k  added LAWAR_ENABLE and LAWBAR_ADRS_SHIFT.
01g,22feb07,dtr  Add support for vxBus sio ns16550.
01f,16feb07,dtr  Make inbound window prefetchable.
01e,22sep06,mze  added SM_MAX_PKTS and SM_PKTS_SIZE for WIND00058841
01d,24aug06,pmr  Use CCSR driver.
01c,09mar06,wap  Allow either mottsec or motetsec driver to be used
                 (SPR #118829)
01b,07feb06,wap  Add VxBus parameter table
01a,16jan06,dtr  written.
*/

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#include <../src/hwif/h/vxbus/vxbRapidIO.h>
#include "config.h"
#include "sysDuart.h"
#include <hwif/vxbus/vxbIntrCtlr.h>

#ifdef DRV_INTCTLR_EPIC
#   include <hwif/intCtlr/vxbEpicIntCtlr.h>
#   include <../src/hwif/intCtlr/vxbIntDynaCtlrLib.h>
IMPORT UINT32 sysPicClkFreqGet(UINT32);
#endif /* DRV_INTCTLR_EPIC */

#ifdef INCLUDE_PCI_BUS
#include "mot85xxPci.h"
#include "drv/pci/pciAutoConfigLib.h"
#endif /* INCLUDE_PCI_BUS */

#define XTSEC_NAME "mottsec"
#if defined (INCLUDE_ETSEC_VXB_END) || defined (INCLUDE_ETSEC_VXB_END2)
#   undef   XTSEC_NAME
#   define  XTSEC_NAME "motetsec"
#endif /* defined (INCLUDE_ETSEC_VXB_END) || defined (INCLUDE_ETSEC_VXB_END2) */

#ifdef  INCLUDE_PCI_BUS
IMPORT  STATUS sysPci1AutoconfigInclude();
IMPORT  UCHAR sysPci1AutoconfigIntrAssign(PCI_SYSTEM *, PCI_LOC *, UCHAR);
IMPORT  STATUS sysPci2AutoconfigInclude();              
IMPORT  UCHAR sysPci2AutoconfigIntrAssign(PCI_SYSTEM *, PCI_LOC *, UCHAR);
IMPORT  STATUS sysPci3AutoconfigInclude();              
IMPORT  UCHAR sysPci3AutoconfigIntrAssign(PCI_SYSTEM *, PCI_LOC *, UCHAR);
IMPORT  STATUS sysPci4AutoconfigInclude();              
IMPORT  UCHAR sysPci4AutoconfigIntrAssign(PCI_SYSTEM *, PCI_LOC *, UCHAR);
#endif  /* INCLUDE_PCI_BUS */

/*
 * On the ads8544 board, PHYs are physically wired according
 * to the following table:
 *
 *      Data port pins  Management port pins MII address
 *      --------------  -------------------- -----------
 * PHY0:    TSEC0              TSEC0             0
 * PHY1:    TSEC2              TSEC0             1
 *
 * The tricky part is that all PHYs have their management pins
 * connected to TSEC0. We have to make it look like PHY1 is connected
 * to TSEC1, so we provide a remapping resource that will cause
 * PHY1 to be attached to miibus1 instead of miibus0.
 */

const struct hcfResource tsecVxbEnd0Resources[] = {
    { "regBase",     HCF_RES_INT,    { (void *)(CCSBAR + 0x24000) } },
    { "txInt",       HCF_RES_INT,    { (void *)EPIC_TSEC1TX_INT_VEC } },
    { "txIntLevel",  HCF_RES_INT,    { (void *)EPIC_TSEC1TX_INT_VEC } },
    { "rxInt",       HCF_RES_INT,    { (void *)EPIC_TSEC1RX_INT_VEC } },
    { "rxIntLevel",  HCF_RES_INT,    { (void *)EPIC_TSEC1RX_INT_VEC } },
    { "errInt",      HCF_RES_INT,    { (void *)EPIC_TSEC1ERR_INT_VEC } },
    { "errIntLevel", HCF_RES_INT,    { (void *)EPIC_TSEC1ERR_INT_VEC } },
    { "phyAddr",     HCF_RES_INT,    { (void *)0 } },
    { "miiIfName",   HCF_RES_STRING, { (void *)XTSEC_NAME } },
    { "miiIfUnit",   HCF_RES_INT,    { (void *)0 } },
};
#define tsecVxbEnd0Num NELEMENTS(tsecVxbEnd0Resources)

const struct hcfResource tsecVxbEnd1Resources[] = {
    { "regBase",     HCF_RES_INT,    { (void *)(CCSBAR + 0x26000) } },
    { "txInt",       HCF_RES_INT,    { (void *)EPIC_TSEC3TX_INT_VEC } },
    { "txIntLevel",  HCF_RES_INT,    { (void *)EPIC_TSEC3TX_INT_VEC } },
    { "rxInt",       HCF_RES_INT,    { (void *)EPIC_TSEC3RX_INT_VEC } },
    { "rxIntLevel",  HCF_RES_INT,    { (void *)EPIC_TSEC3RX_INT_VEC } },
    { "errInt",      HCF_RES_INT,    { (void *)EPIC_TSEC3ERR_INT_VEC } },
    { "errIntLevel", HCF_RES_INT,    { (void *)EPIC_TSEC3ERR_INT_VEC } },
    { "phyAddr",     HCF_RES_INT,    { (void *)1 } },
    { "miiIfName",   HCF_RES_STRING, { (void *)XTSEC_NAME } },
    { "miiIfUnit",   HCF_RES_INT,    { (void *)0 } },
};
#define tsecVxbEnd1Num NELEMENTS(tsecVxbEnd1Resources)

#ifdef INCLUDE_PCI_BUS
const struct hcfResource m85xxPci0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,  { (void *)PCIEX1_REGBASE } },
    { "mem32Addr",          HCF_RES_ADDR, { (void *)PCIEX1_MEM_ADRS } },
    { "mem32Size",          HCF_RES_INT,  { (void *)PCIEX1_MEM_SIZE } },
    { "memIo32Addr",        HCF_RES_ADDR, { (void *)PCIEX1_MEMIO_ADRS } },
    { "memIo32Size",        HCF_RES_INT,  { (void *)PCIEX1_MEMIO_SIZE } },
    { "io32Addr",           HCF_RES_ADDR, { (void *)PCIEX1_IO_ADRS } },
    { "io32Size",           HCF_RES_INT,  { (void *)PCIEX1_IO_SIZE } }, 
    { "io16Addr",           HCF_RES_ADDR, { (void *)PCIEX1_IO_ADRS } },
    { "io16Size",           HCF_RES_INT,  { (void *)PCIEX1_IO_SIZE } }, 
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
    { "maxBusSet",          HCF_RES_INT,  { (void *)3 } },
    { "pciConfigMechanism", HCF_RES_INT,  { (void *)(0) } },

    /* LAWBAR details */

    { "tgtIf",              HCF_RES_INT,  { (void *)(LAWAR_TGTIF_PCIEX1) } },

    /* Window Attributes - Defaults to 8540 type if none given */

    { "owAttrMem",    HCF_RES_INT, { (void *)(PCI_WINDOW_ENABLE_BIT | 
                                              PCI_OUT_ATTR_RTT_MEM  |  
                                              PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrMemIo",  HCF_RES_INT, { (void *)(PCI_WINDOW_ENABLE_BIT | 
                                              PCI_OUT_ATTR_RTT_MEM  |  
                                              PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrIo",     HCF_RES_INT, { (void *)(PCI_WINDOW_ENABLE_BIT | 
                                              PCI_OUT_ATTR_RTT_IO   |  
                                              PCI_OUT_ATTR_WTT_IO) } },
    { "iwAttr",       HCF_RES_INT, { (void *)(PCI_WINDOW_ENABLE_BIT |  
                                              PCI_PREFETCHABLE      | 
                                              PCI_IN_ATTR_RTT_LM_READ_SNOOP  | 
                                              PCI_IN_ATTR_RTT_LM_WRITE_SNOOP | 
                                              PCI_IN_ATTR_TGI_LM) } },
    { "singleLawBar",           HCF_RES_INT,  { (void *)(TRUE)}},
    { "singleLawBarSize",       HCF_RES_INT,  { (void *)(0x10000000)}},
    { "autoConfig",             HCF_RES_INT,  { (void *)(TRUE)}},
    { "pciExpressHost",         HCF_RES_INT,  { (void *)(TRUE)} }
#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    ,
    { "msiEnable",          HCF_RES_INT,  { (void *)(TRUE)} },
    { "dynamicInterrupts",  HCF_RES_INT,  { (void *)(TRUE)} },
    { "msiDataWorkaround",      HCF_RES_INT,  { (void *)(TRUE)} },
    { "msiDataWorkaroundAddr",  HCF_RES_INT,  { (void *)(0xa0000000)} }
#else
    ,
    { "msiEnable",          HCF_RES_INT,  { (void *)(FALSE)} },
    { "dynamicInterrupts",  HCF_RES_INT,  { (void *)(FALSE)} },
    { "msiDataWorkaround",      HCF_RES_INT,  { (void *)(FALSE)} },
    { "msiDataWorkaroundAddr",  HCF_RES_INT,  { (void *)(0xa0000000)} }
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */    
};

#define m85xxPci0Num NELEMENTS(m85xxPci0Resources)

const struct hcfResource m85xxPci1Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,  { (void *)PCIEX2_REGBASE } },
    { "mem32Addr",          HCF_RES_ADDR, { (void *)PCIEX2_MEM_ADRS } },
    { "mem32Size",          HCF_RES_INT,  { (void *)PCIEX2_MEM_SIZE } },
    { "memIo32Addr",        HCF_RES_ADDR, { (void *)PCIEX2_MEMIO_ADRS } },
    { "memIo32Size",        HCF_RES_INT,  { (void *)PCIEX2_MEMIO_SIZE } },
    { "io32Addr",           HCF_RES_ADDR, { (void *)PCIEX2_IO_ADRS} },
    { "io32Size",           HCF_RES_INT,  { (void *)PCIEX2_IO_SIZE } }, 
    { "io16Addr",           HCF_RES_ADDR, { (void *)PCIEX2_IO_ADRS } },
    { "io16Size",           HCF_RES_INT,  { (void *)PCIEX2_IO_SIZE } }, 
    { "fbbEnable",          HCF_RES_INT,  { (void *)TRUE } },
    { "cacheSize",          HCF_RES_INT,  { (void *)(_CACHE_ALIGN_SIZE / 4) } }, 
    { "maxLatAllSet",       HCF_RES_INT,  { (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet",    HCF_RES_INT,  { (void *)TRUE } }, 
    { "includeFuncSet",     HCF_RES_ADDR, { (void *)sysPci2AutoconfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR, { (void *)sysPci2AutoconfigIntrAssign } },
    { "maxBusSet",          HCF_RES_INT,  { (void *)3 } },
    { "pciConfigMechanism", HCF_RES_INT,  { (void *)(0) } },

    /* non vxbPciAutoConfig() values */

    { "pimmrBase",          HCF_RES_ADDR, { (void *)PCI_BRIDGE_PIMMR_BASE_ADRS } },
    { "lclMemAddr",         HCF_RES_ADDR, { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",      HCF_RES_INT,  { (void *)(512 * 1024 * 1024) } },
    { "mstrMemBus",         HCF_RES_ADDR, { (void *)PCI_MSTR_MEM_BUS } },

    /* LAWBAR details */

    { "tgtIf",              HCF_RES_INT,  { (void *)(LAWAR_TGTIF_PCIEX2) } },

    /* Window Attributes - Defaults to 8540 type if none given */

    { "owAttrMem",    HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | 
                                               PCI_OUT_ATTR_RTT_MEM |  
                                               PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrMemIo",  HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | 
                                               PCI_OUT_ATTR_RTT_MEM  |  
                                               PCI_OUT_ATTR_WTT_MEM) } },
    { "owAttrIo",     HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | 
                                               PCI_OUT_ATTR_RTT_IO   |  
                                               PCI_OUT_ATTR_WTT_IO) } },
    { "iwAttr",       HCF_RES_INT,  { (void *)(PCI_WINDOW_ENABLE_BIT | 
                                               PCI_PREFETCHABLE      | 
                                               PCI_IN_ATTR_RTT_LM_READ_SNOOP  | 
                                               PCI_IN_ATTR_RTT_LM_WRITE_SNOOP | 
                                               PCI_IN_ATTR_TGI_LM) } },
    { "singleLawBar",           HCF_RES_INT,  { (void *)(TRUE) } },
    { "singleLawBarSize",       HCF_RES_INT,  { (void *)(0x10000000) } },
    { "autoConfig",             HCF_RES_INT,  { (void *)(TRUE) } },
    { "pciExpressHost",         HCF_RES_INT,  { (void *)(TRUE) } }
#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    ,
    { "msiEnable",          HCF_RES_INT,  { (void *)(TRUE)} },
    { "dynamicInterrupts",  HCF_RES_INT,  { (void *)(TRUE)} },
    { "msiDataWorkaround",      HCF_RES_INT,  { (void *)(TRUE)} },
    { "msiDataWorkaroundAddr",  HCF_RES_INT,  { (void *)(0xa0000000)} }
#else
    ,
    { "msiEnable",          HCF_RES_INT,  { (void *)(FALSE)} },
    { "dynamicInterrupts",  HCF_RES_INT,  { (void *)(FALSE)} },
    { "msiDataWorkaround",      HCF_RES_INT,  { (void *)(FALSE)} },
    { "msiDataWorkaroundAddr",  HCF_RES_INT,  { (void *)(0xa0000000)} }
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};

#define m85xxPci1Num NELEMENTS(m85xxPci1Resources)

const struct hcfResource m85xxPci2Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,  { (void *)PCIEX3_REGBASE } },
    { "mem32Addr",          HCF_RES_ADDR, { (void *)PCIEX3_MEM_ADRS } },
    { "mem32Size",          HCF_RES_INT,  { (void *)PCIEX3_MEM_SIZE } },
    { "memIo32Addr",        HCF_RES_ADDR, { (void *)PCIEX3_MEMIO_ADRS } },
    { "memIo32Size",        HCF_RES_INT,  { (void *)PCIEX3_MEMIO_SIZE } },
    { "io32Addr",           HCF_RES_ADDR, { (void *)PCIEX3_IO_ADRS } },
    { "io32Size",           HCF_RES_INT,  { (void *)PCIEX3_IO_SIZE } }, 
    { "io16Addr",           HCF_RES_ADDR, { (void *)PCIEX3_IO_ADRS } },
    { "io16Size",           HCF_RES_INT,  { (void *)PCIEX3_IO_SIZE } }, 
    { "fbbEnable",          HCF_RES_INT,  { (void *)TRUE } },
    { "cacheSize",          HCF_RES_INT,  { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",       HCF_RES_INT,  { (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet",    HCF_RES_INT,  { (void *)TRUE } },
    { "includeFuncSet",     HCF_RES_ADDR, { (void *)sysPci3AutoconfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR, { (void *)sysPci3AutoconfigIntrAssign } },
    { "maxBusSet",          HCF_RES_INT,  { (void *)3 } },
    { "pciConfigMechanism", HCF_RES_INT,  { (void *)(0) } },

    /* non vxbPciAutoConfig() values */

    { "pimmrBase",          HCF_RES_ADDR, { (void *)PCI_BRIDGE_PIMMR_BASE_ADRS } },
    { "lclMemAddr",         HCF_RES_ADDR, { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",      HCF_RES_INT,  { (void *)(512 * 1024 * 1024) } },
    { "mstrMemBus",         HCF_RES_ADDR, { (void *)PCI_MSTR_MEM_BUS } },

    /* LAWBAR details */

    { "tgtIf",              HCF_RES_INT,  { (void *)(LAWAR_TGTIF_PCIEX3)} },

    /* Window Attributes - Defaults to 8540 type if none given */

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
    { "singleLawBar",           HCF_RES_INT,  { (void *)(TRUE) } },
    { "singleLawBarSize",       HCF_RES_INT,  { (void *)(0x10000000) } },
    { "autoConfig",             HCF_RES_INT,  { (void *)(TRUE) } },
    { "pciExpressHost",         HCF_RES_INT,  { (void *)(TRUE) } }
#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    ,
    { "msiEnable",          HCF_RES_INT,  { (void *)(TRUE)} },
    { "dynamicInterrupts",  HCF_RES_INT,  { (void *)(TRUE)} },
    { "msiDataWorkaround",      HCF_RES_INT,  { (void *)(TRUE)} },
    { "msiDataWorkaroundAddr",  HCF_RES_INT,  { (void *)(0xa0000000)} }
#else
    ,
    { "msiEnable",          HCF_RES_INT,  { (void *)(FALSE)} },
    { "dynamicInterrupts",  HCF_RES_INT,  { (void *)(FALSE)} },
    { "msiDataWorkaround",      HCF_RES_INT,  { (void *)(FALSE)} },
    { "msiDataWorkaroundAddr",  HCF_RES_INT,  { (void *)(0xa0000000)} }
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#define m85xxPci2Num NELEMENTS(m85xxPci2Resources)

const struct hcfResource m85xxPci3Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,  { (void *)PCI_REGBASE } },
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
    { "includeFuncSet",     HCF_RES_ADDR, { (void *)sysPci4AutoconfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR, { (void *)sysPci4AutoconfigIntrAssign } },
    { "pciConfigMechanism", HCF_RES_INT,  { (void *)(0) } },
    { "maxBusSet",          HCF_RES_INT,  { (void *)3 } },

    /* non vxbPciAutoConfig() values */

    { "pimmrBase",          HCF_RES_ADDR, { (void *)PCI_BRIDGE_PIMMR_BASE_ADRS } },
    { "lclMemAddr",         HCF_RES_ADDR, { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",      HCF_RES_INT,  { (void *)(512 * 1024 * 1024) } },
    { "mstrMemBus",         HCF_RES_ADDR, { (void *)PCI_MSTR_MEM_BUS } },

    /* Window attributes which may be specific to controller */

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

    /* LAWBAR details */

    { "tgtIf",              HCF_RES_INT,  { (void *)(LAWAR_TGTIF_PCI) } },

    /* Window Attributes - Defaults to 8540 type if none given */

    { "singleLawBar",           HCF_RES_INT,  { (void *)(TRUE) } },
    { "singleLawBarSize",       HCF_RES_INT,  { (void *)(0x10000000) } },
    { "autoConfig",             HCF_RES_INT,  { (void *)(TRUE) } },
    { "pciExpressHost",         HCF_RES_INT,  { (void *)(FALSE) } }
#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    ,
    { "msiEnable",          HCF_RES_INT,  { (void *)(TRUE)} },
    { "dynamicInterrupts",  HCF_RES_INT,  { (void *)(TRUE)} },
    { "msiDataWorkaround",      HCF_RES_INT,  { (void *)(TRUE)} },
    { "msiDataWorkaroundAddr",  HCF_RES_INT,  { (void *)(0xa0000000)} }
#else
    ,
    { "msiEnable",          HCF_RES_INT,  { (void *)(FALSE)} },
    { "dynamicInterrupts",  HCF_RES_INT,  { (void *)(FALSE)} },
    { "msiDataWorkaround",      HCF_RES_INT,  { (void *)(FALSE)} },
    { "msiDataWorkaroundAddr",  HCF_RES_INT,  { (void *)(0xa0000000)} }
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#define m85xxPci3Num NELEMENTS(m85xxPci3Resources)
#endif /* INCLUDE_PCI_BUS */

const struct hcfResource m85xxCCSR0Resources[] = {
    { VXB_REG_BASE,        HCF_RES_INT,        { (void *)CCSBAR  } },
    { "LAWAR_ENABLE",      HCF_RES_INT,        { (void *)LAWAR_ENABLE } },
    { "LAWBAR_ADRS_SHIFT", HCF_RES_INT,        { (void *)LAWBAR_ADRS_SHIFT } },
    { "LAWBAR0",           HCF_RES_STRING,     { (void *)"reserved" } },
    { "LAWBAR1",           HCF_RES_STRING,     { (void *)"reserved" } },
    { "LAWBAR2",           HCF_RES_STRING,     { (void *)"reserved" } },
    { "LAWBAR3",           HCF_RES_STRING,     { (void *)"reserved" } },
    { "LAWBAR4",           HCF_RES_STRING,     { (void *)"m85xxPci" } },
    { "LAWBAR5",           HCF_RES_STRING,     { (void *)"m85xxPci" } },
    { "LAWBAR6",           HCF_RES_STRING,     { (void *)"m85xxPci" } },
    { "LAWBAR7",           HCF_RES_STRING,     { (void *)"m85xxPci" } },
    { "LAWBAR8",           HCF_RES_STRING,     { (void *)NULL } },
    { "LAWBAR9",           HCF_RES_STRING,     { (void *)NULL } }
};

#define m85xxCCSR0Num NELEMENTS(m85xxCCSR0Resources)

const struct hcfResource ns1655x1Resources[] = {
    { "regBase",     HCF_RES_INT,  { (void *)COM1_ADR } },
    { "regInterval", HCF_RES_INT,  { (void *)DUART_REG_ADDR_INTERVAL } },
    { "clkFreq",     HCF_RES_ADDR, { (void *)&sysClkFreqGet } }
};

#define ns1655x1Num NELEMENTS(ns1655x1Resources)

const struct hcfResource ns1655x2Resources[] = {
    { "regBase",     HCF_RES_INT,  { (void *)COM2_ADR } },
    { "regInterval", HCF_RES_INT,  { (void *)DUART_REG_ADDR_INTERVAL } },
    { "clkFreq",     HCF_RES_ADDR, { (void *)&sysClkFreqGet } }
};
#define ns1655x2Num NELEMENTS(ns1655x2Resources)

struct intrCtlrInputs epicInputs[] = {
    { EPIC_TSEC1TX_INT_VEC,     XTSEC_NAME, 0, 0 },
    { EPIC_TSEC1RX_INT_VEC,     XTSEC_NAME, 0, 1 },
    { EPIC_TSEC3TX_INT_VEC,     XTSEC_NAME, 1, 0 },
    { EPIC_TSEC3RX_INT_VEC,     XTSEC_NAME, 1, 1 },
    { EPIC_TSEC3ERR_INT_VEC,    XTSEC_NAME, 1, 2 },
    { EPIC_TSEC1ERR_INT_VEC,    XTSEC_NAME, 0, 2 },
    { EPIC_DUART_INT_VEC,       "ns16550",  0, 0 },
    { EPIC_DUART_INT_VEC,       "ns16550",  1, 0 },
#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    { OPENPIC_TIMERA0_INT_VEC,  "openPicTimer", 0, 0 },
    { OPENPIC_TIMERA1_INT_VEC,  "openPicTimer", 0, 1 },
    { OPENPIC_TIMERA2_INT_VEC,  "openPicTimer", 0, 2 },
    { OPENPIC_TIMERA3_INT_VEC,  "openPicTimer", 0, 3 }
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};

struct intrCtlrPriority epicPriority[] = {
    { EPIC_DUART_INT_VEC,       100 }
};

struct intrCtlrXBar epicXBar[] = {
    { 0, 0 }
};

const struct hcfResource epic0Resources[] = {
    { VXB_REG_BASE,        HCF_RES_INT,  { (void *)CCSBAR } },
    { "input",             HCF_RES_ADDR, { (void *)&epicInputs[0] } },
    { "inputTableSize",    HCF_RES_INT,  { (void *)NELEMENTS(epicInputs) } },
    { "exPolar",           HCF_RES_INT,  { (void *)EPIC_INT_ACT_LOW } },
    { "numCpus",           HCF_RES_INT,  { (void *)1 } },
};
#define epic0Num NELEMENTS(epic0Resources)

#ifdef DRV_TIMER_M85XX
struct hcfResource m85xxTimerResources[] =  {
    { "regBase",        HCF_RES_INT,  { (void *)0} },
    { "decMinClkRate",  HCF_RES_INT,  { (void *)SYS_CLK_RATE_MIN } },
    { "decMaxClkRate",  HCF_RES_INT,  { (void *)SYS_CLK_RATE_MAX } },
#ifdef INCLUDE_AUX_CLK
    { "fitMinClkRate",  HCF_RES_INT,  { (void *)AUX_CLK_RATE_MIN } },
    { "fitMaxClkRate",  HCF_RES_INT,  { (void *)AUX_CLK_RATE_MAX } },
#endif /* INCLUDE_AUX_CLK */
    { "sysClockFreq",   HCF_RES_ADDR, { (void *)&sysClkFreqGet } }
};
#define m85xxTimerNum   NELEMENTS(m85xxTimerResources)
#endif /* DRV_TIMER_M85XX */

struct intrCtlrInputs ppcIntCtlrInputs[] = {
    { 0,    "epic", 0, 0 }
};

const struct hcfResource ppcIntCtlr0Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,  { (void *)TRUE} },
    { "input",          HCF_RES_ADDR, { (void *)&ppcIntCtlrInputs[0] } },
    { "inputTableSize", HCF_RES_INT,  { (void *)NELEMENTS(ppcIntCtlrInputs) } },
};
#define ppcIntCtlr0Num NELEMENTS(ppcIntCtlr0Resources)

#ifdef DRV_TIMER_OPENPIC
const struct hcfResource openPicTimerDevAresources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,    { (void *)(CCSBAR + 0x000410F0) } },
    { "instanceName",   HCF_RES_STRING, { (void *)"A" } },
    { VXB_CLK_FREQ,     HCF_RES_ADDR,   { (void *)sysPicClkFreqGet } },
    { "clkDivisor",     HCF_RES_INT,    { (void *)8 } },
    { "maxClkRate0",    HCF_RES_INT,    { (void *)SYS_CLK_RATE_MAX } },
#ifdef INCLUDE_AUX_CLK
    { "maxClkRate1",    HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX } },
#endif /* INCLUDE_AUX_CLK */
};
#define openPicTimerDevAnum NELEMENTS(openPicTimerDevAresources)
#endif /* DRV_TIMER_OPENPIC */

const struct hcfDevice hcfDeviceList[] = {

    /*
     * Initialize ppcIntCtlr before epic.
     * The vector table for external interrupts are over written by epic
     * for an optimized purpose.
     */

    { "ppcIntCtlr",   0,  VXB_BUSID_PLB,  0, ppcIntCtlr0Num,      ppcIntCtlr0Resources },
    { "epic",         0,  VXB_BUSID_PLB,  0, epic0Num,            epic0Resources },
    { XTSEC_NAME,     0,  VXB_BUSID_PLB,  0, tsecVxbEnd0Num,      tsecVxbEnd0Resources },
    { XTSEC_NAME,     1,  VXB_BUSID_PLB,  0, tsecVxbEnd1Num,      tsecVxbEnd1Resources },
    { "ns16550",      0,  VXB_BUSID_PLB,  0, ns1655x1Num,         ns1655x1Resources },
    { "ns16550",      1,  VXB_BUSID_PLB,  0, ns1655x2Num,         ns1655x2Resources },
    { "m85xxCCSR",    0,  VXB_BUSID_PLB,  0, m85xxCCSR0Num,       m85xxCCSR0Resources },
#ifdef DRV_TIMER_M85XX
    {"m85xxTimerDev", 0,  VXB_BUSID_PLB,  0, m85xxTimerNum,       m85xxTimerResources },
#endif /* DRV_TIMER_M85XX */
#ifdef DRV_TIMER_OPENPIC
    { "openPicTimer", 0,  VXB_BUSID_PLB,  0, openPicTimerDevAnum, openPicTimerDevAresources },
#endif /* DRV_TIMER_OPENPIC */
#ifdef INCLUDE_PCI_BUS
    { "m85xxPci",     0,  VXB_BUSID_PLB,  0, m85xxPci0Num,        m85xxPci0Resources },
    { "m85xxPci",     1,  VXB_BUSID_PLB,  0, m85xxPci1Num,        m85xxPci1Resources },
    { "m85xxPci",     2,  VXB_BUSID_PLB,  0, m85xxPci2Num,        m85xxPci2Resources },
    { "m85xxPci",     3,  VXB_BUSID_PLB,  0, m85xxPci3Num,        m85xxPci3Resources }
#endif /* INCLUDE_PCI_BUS */
};
const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] = {
    { NULL,           0,  NULL,           VXB_PARAM_END_OF_LIST,  {(void *)0 } }
};
