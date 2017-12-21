/* hwconf.c - Hardware configuration support module */

/*
 * Copyright (c) 2007-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
-------------------
01g,06jul09,z_l  Remove legacy PCI driver. (WIND00088032)
01f,13oct08,x_z  Add embedded-TT support for USB. (WIND00134142)
01e,10sep08,pgh  Fix #if/#endif mismatch
01d,31jul08,l_z  Support END2 versions of the drivers also.
01c,13jun08,wap  Switch to new VxBus TSEC driver
01b,07sep07,ami  Plb-based USB Controller Support added
01a,10jul07,pdg  written
*/

/* includes */

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>

#ifdef DRV_PCIBUS_M83XX
#include "wrSbc834x.h"
#include <hwif/vxbus/vxbPciLib.h>
#include <drv/pci/pciConfigLib.h>
#include <drv/pci/pciAutoConfigLib.h>
#include <drv/pci/pciIntLib.h>

extern STATUS sysPciAutoConfigInclude (PCI_SYSTEM * pSys,
                                       PCI_LOC * pLoc,
                                       UINT devVend);
extern UCHAR sysPciAutoConfigIntrAssign (PCI_SYSTEM * pSys,
                                         PCI_LOC * pLoc,
                                         UCHAR pin);
#endif /* DRV_PCIBUS_M83XX */

#if defined (INCLUDE_USB) && defined (INCLUDE_EHCI)

/* import to do the BSP level initialization */

extern void sysM834xEhciInit(void);
extern UINT32 vxbSwap32 (UINT32 data);

#define EHCI_M834x_INT   0x27

#endif /* defined (INCLUDE_USB) && defined (INCLUDE_EHCI) */


#if defined(INCLUDE_TSEC_VXB_END) || defined(INCLUDE_TSEC_VXB_END2)
/*
 * On the wrSbc834x boards, PHYs are physically wired according
 * to the following table:
 *
 *              Data port pins  Management port pins    MII address
 *              --------------  --------------------    -----------
 * PHY0:        TSEC0           TSEC0                   25
 * PHY1:        TSEC1           TSEC0                   26
 *
 * The tricky part is that both PHYs have their management pins
 * connected to TSEC0. The VxBus TSEC driver allows us to specify
 * the name of the device that provides PHY management support.
 * This device is "mottsec0" in both cases. We also specify the
 * "phyAddr" resource for each device, which is 25 for TSEC0
 * and 26 for TSEC1.
 */

const struct hcfResource tsecVxbEnd0Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x24000) } },
    { "intr0",          HCF_RES_INT,    { (void *)INUM_TSEC1_TX } },
    { "intr0Level",     HCF_RES_INT,    { (void *)INUM_TSEC1_TX } },
    { "intr1",          HCF_RES_INT,    { (void *)INUM_TSEC1_RX } },
    { "intr1Level",     HCF_RES_INT,    { (void *)INUM_TSEC1_RX } },
    { "intr2",          HCF_RES_INT,    { (void *)INUM_TSEC1_ERR } },
    { "intr2Level",     HCF_RES_INT,    { (void *)INUM_TSEC1_ERR } },
    { "phyAddr",        HCF_RES_INT,    { (void *)25 } },
    { "miiIfName",      HCF_RES_STRING, { (void *)"mottsec" } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
};
#define tsecVxbEnd0Num NELEMENTS(tsecVxbEnd0Resources)

const struct hcfResource tsecVxbEnd1Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x25000) } },
    { "intr0",          HCF_RES_INT,    { (void *)INUM_TSEC2_TX } },
    { "intr0Level",     HCF_RES_INT,    { (void *)INUM_TSEC2_TX } },
    { "intr1",          HCF_RES_INT,    { (void *)INUM_TSEC2_RX } },
    { "intr1Level",     HCF_RES_INT,    { (void *)INUM_TSEC2_RX } },
    { "intr2",          HCF_RES_INT,    { (void *)INUM_TSEC2_ERR } },
    { "intr2Level",     HCF_RES_INT,    { (void *)INUM_TSEC2_ERR } },
    { "phyAddr",        HCF_RES_INT,    { (void *)26 } },
    { "miiIfName",      HCF_RES_STRING, { (void *)"mottsec" } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
};
#define tsecVxbEnd1Num NELEMENTS(tsecVxbEnd1Resources)
#endif /* defined(INCLUDE_TSEC_VXB_END) || defined (INCLUDE_TSEC_VXB_END2) */

#ifdef DRV_PCIBUS_M83XX

#define PCI_LAWBAR0	0x60
#define PCI_LAWAR0	0x64
#define PCI_LAWBAR1	0x68
#define PCI_LAWAR1	0x6C

const struct hcfResource m83xxPci0Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)(CCSBAR + 0x8000) } },
    { "mem32Addr", HCF_RES_ADDR, { (void *)PCI_MEM_ADRS } },
    { "mem32Size", HCF_RES_INT, { (void *)PCI_MEM_SIZE } },
    { "memIo32Addr", HCF_RES_ADDR, { (void *)PCI_MEMIO_ADRS } },
    { "memIo32Size", HCF_RES_INT, { (void *)PCI_MEMIO_SIZE } },
    { "io32Addr", HCF_RES_ADDR, { (void *)PCI_IO_ADRS } },
    { "io32Size", HCF_RES_INT, { (void *)PCI_IO_SIZE } },
    { "fbbEnable", HCF_RES_INT, { (void *)TRUE } },
    { "cacheSize", HCF_RES_INT, { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet", HCF_RES_INT, { (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet", HCF_RES_INT, { (void *)TRUE } },
    { "includeFuncSet", HCF_RES_ADDR, { (void *)sysPciAutoConfigInclude } },
    { "intAssignFuncSet", HCF_RES_ADDR, { (void *)sysPciAutoConfigIntrAssign }},
     /* non vxbPciAutoConfig() values */
    { "pimmrBase", HCF_RES_ADDR, { (void *)PCI_BRIDGE_PIMMR_BASE_ADRS} },
    { "lclMemAddr", HCF_RES_ADDR, { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize", HCF_RES_INT, { (void *)(512 * 1024 * 1024) } },
    { "mstrMemBus", HCF_RES_ADDR, { (void *)PCI_MSTR_MEM_BUS } },

    { "pciConfigMechanism", HCF_RES_INT, { (void *)0 } },
    { "maxBusSet", HCF_RES_INT, { (void *)1 } },
    { "autoConfig",HCF_RES_INT, { (void *)TRUE } },

    /* Window Attributes - Defaults to 8540 type if none given */
    { "owAttrMem",HCF_RES_INT, { (void *)(0xa00f8000) } },
    { "owAttrMemIo", HCF_RES_INT, { (void *)(0xa00fc000) } },
    { "owAttrIo", HCF_RES_INT, { (void *)(0xc00fc000) } },
    { "iwAttr", HCF_RES_INT, { (void *)(PCI_WINDOW_ENABLE_BIT | 
                                        PCI_IN_ATTR_RTT_LM_WRITE_SNOOP |
                                        PCI_IN_ATTR_RTT_LM_READ_SNOOP | 
                                        PCI_ATTR_WS_256M) } },
    { "lawbar",HCF_RES_INT, { (void *)(PCI_LAWBAR0)}},
    { "lawar",HCF_RES_INT, { (void *)(PCI_LAWAR0)}},
    { "lawarAttr",HCF_RES_INT, { (void *)(LAWAR_ENABLE  | LAWAR_SIZE_256MB) } },
    { "pciExpressHost",HCF_RES_INT, { (void *)FALSE } }
};

#define m83xxPci0Num	NELEMENTS (m83xxPci0Resources)

#endif /* DRV_PCIBUS_M83XX */

#ifdef INCLUDE_EHCI
const struct hcfResource vxbPlbUsbEhciDevResources[]=
    {
        {"regBase",  HCF_RES_INT, {(void *)M83XX_CAPLENGTH(CCSBAR)} },
        {"irq",      HCF_RES_INT, {(void *)(EHCI_M834x_INT)} },
        {"irqLevel", HCF_RES_INT, {(void *)(EHCI_M834x_INT)} },
        {"ehciInit", HCF_RES_ADDR, {(void *)sysM834xEhciInit} },
        {"dataSwap", HCF_RES_ADDR, {(void *)vxbSwap32}}
    };

#define vxbPlbUsbEhciDevNum NELEMENTS(vxbPlbUsbEhciDevResources)
#endif /* INCLUDE_EHCI */


const struct hcfDevice hcfDeviceList[] = { 
#if defined(INCLUDE_TSEC_VXB_END) || defined(INCLUDE_TSEC_VXB_END2)
    { "mottsec", 0, VXB_BUSID_PLB, 0, tsecVxbEnd0Num, tsecVxbEnd0Resources },
    { "mottsec", 1, VXB_BUSID_PLB, 0, tsecVxbEnd1Num, tsecVxbEnd1Resources },
#endif /* defined(INCLUDE_TSEC_VXB_END) || defined(INCLUDE_TSEC_VXB_END2) */
#ifdef DRV_PCIBUS_M83XX
    { "m83xxPci", 0, VXB_BUSID_PLB, 0, m83xxPci0Num, m83xxPci0Resources },
#endif

#ifdef INCLUDE_EHCI
    {"vxbPlbUsbEhci", 0, VXB_BUSID_PLB, 0, vxbPlbUsbEhciDevNum, vxbPlbUsbEhciDevResources},
#endif /* INCLUDE_EHCI */

    { NULL, 0, 0, 0, 0, NULL }
};

const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] =
    {
#ifdef INCLUDE_EHCI    
    { "vxbPlbUsbEhci", 0, "hasEmbeddedTT",   VXB_PARAM_INT32,       {(void *)TRUE} },
    { "vxbPlbUsbEhci", 0, "fixupPortNumber", VXB_PARAM_INT32,       {(void *)FALSE} },
    { "vxbPlbUsbEhci", 0, "postResetHook",   VXB_PARAM_FUNCPTR,     {(void *)NULL} },
#endif /* INCLUDE_EHCI */  
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, {(void *)0} }
    };
