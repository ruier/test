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
--------------------
01e,08dec09,pch  CQ195058: Fix definitions of lclMemMapSize
01d,06jun08,dlk  Make entries for e/tsec devices unconditional.
                 Support END2 versions of the drivers also.
01c,20aug07,pdg  added m85xxPci support
01b,16aug07,wap  Switch to new VxBus TSEC driver (WIND00101678)
01a,25jan07,ami First
*/


/*
DESCRIPTION

This is the hardware configuration file. This file contains tables which
describe the hardware register addresses, interrupt vectors, and other
hardware-specific information for static hardware.

*/

/* includes */

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#include "config.h"
#include "sysDuart.h"
#include "sysEpic.h"
#include <drv/pci/pciConfigLib.h>
#include <drv/pci/pciAutoConfigLib.h>
#include <drv/pci/pciIntLib.h>

/* defines */

#define MOT_TSEC_ADRS_OFFSET_1  0x00024000
#define MOT_TSEC_ADRS_OFFSET_2  0x00025000


/* extern */

IMPORT UINT32 sysClkFreqGet (void);

/******************************************************************************
* vxbSysClkFreqGet - gets the frequency of the system clock
*
* This function calls the sysClkFreqGet routine defined in sysLib.c. The
* function is used to get the frequency of the system clock. This routine is
* used as a wrapper around sysClkFreqGet. This is because the ns16550sio.c file
* calls the clock frequency function with a parameter <VXB_DEVICE_ID>.
* However sysClkFreqGet () function in the BSP expects no parameter.
*
* RETURNS: sysClkFreqGet
* ERRNO: none
*/

LOCAL UINT32  vxbSysClkFreqGet
    (
    struct vxbDev * pDev
    )
    {
    return (sysClkFreqGet ());
    }

/* vxBus resource files for onboard UARTS */

const struct hcfResource cds85xxi82501Resources[] =
    {
        { "regBase",     HCF_RES_INT, {(void *)COM2_ADR} },
        { "irq",         HCF_RES_INT, {(void *)EPIC_DUART_INT_VEC} },
        { "regInterval", HCF_RES_INT, {(void *)DUART_REG_ADDR_INTERVAL} },
        { "irqLevel",    HCF_RES_INT, {(void *)EPIC_DUART_INT_VEC} },
        { "clkFreq",	 HCF_RES_ADDR, {(void *)vxbSysClkFreqGet}}
    };

#define cds85xxi82501Num NELEMENTS(cds85xxi82501Resources)

const struct hcfResource cds85xxi82502Resources[] =
    {
        { "regBase",     HCF_RES_INT, {(void *)COM1_ADR} },
        { "irq",         HCF_RES_INT, {(void *)EPIC_DUART_INT_VEC}},
        { "regInterval", HCF_RES_INT, {(void *)DUART_REG_ADDR_INTERVAL} },
        { "irqLevel",    HCF_RES_INT, {(void *)EPIC_DUART_INT_VEC} },
        { "clkFreq",	 HCF_RES_ADDR, {(void *)vxbSysClkFreqGet}}
    };

#define cds85xxi82502Num NELEMENTS(cds85xxi82502Resources)

/*
 * On the cds8541/cds8555 boards, PHYs are physically wired according
 * to the following table:
 *
 *              Data port pins  Management port pins    MII address
 *              --------------  --------------------    -----------
 * PHY0:        TSEC0           TSEC0                   0
 * PHY1:        TSEC1           TSEC0                   1
 *
 * The tricky part is that both PHYs have their management pins
 * connected to TSEC0. The VxBus TSEC driver allows us to specify
 * the name of the device that provides PHY management support.
 * This device is "mottsec0" in both cases. We also specify the
 * "phyAddr" resource for each device, which is 0 for TSEC0
 * and 1 for TSEC1.
 */

const struct hcfResource tsecVxbEnd0Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x24000) } },
    { "intr0",          HCF_RES_INT,    { (void *)EPIC_TSEC1TX_INT_VEC } },
    { "intr0Level",     HCF_RES_INT,    { (void *)EPIC_TSEC1TX_INT_VEC } },
    { "intr1",          HCF_RES_INT,    { (void *)EPIC_TSEC1RX_INT_VEC } },
    { "intr1Level",     HCF_RES_INT,    { (void *)EPIC_TSEC1RX_INT_VEC } },
    { "intr2",          HCF_RES_INT,    { (void *)EPIC_TSEC1ERR_INT_VEC } },
    { "intr2Level",     HCF_RES_INT,    { (void *)EPIC_TSEC1ERR_INT_VEC } },
    { "phyAddr",        HCF_RES_INT,    { (void *)0 } },
    { "miiIfName",      HCF_RES_STRING, { (void *)"mottsec" } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
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
    { "miiIfName",      HCF_RES_STRING, { (void *)"mottsec" } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
};
#define tsecVxbEnd1Num NELEMENTS(tsecVxbEnd1Resources)

#ifdef DRV_PCIBUS_M85XX

IMPORT STATUS sysPci0AutoconfigInclude(PCI_SYSTEM *, PCI_LOC *, UINT);
IMPORT UCHAR sysPci0AutoconfigIntrAssign(PCI_SYSTEM *, PCI_LOC *, UCHAR);

const struct hcfResource m85xxPci0Resources[] = {
    { "regBase",	HCF_RES_INT,	{ (void *)PCI_CFG_ADR_REG } },
    { "mem32Addr",	HCF_RES_ADDR,	{ (void *)PCI_MEM_ADRS } },
    { "mem32Size",	HCF_RES_INT,	{ (void *)PCI_MEM_SIZE } },
    { "memIo32Addr",	HCF_RES_ADDR,	{ (void *)PCI_MEMIO_ADRS } },
    { "memIo32Size",	HCF_RES_INT,	{ (void *)PCI_MEMIO_SIZE } },
    { "io32Addr",	HCF_RES_ADDR,	{ (void *)PCI_IO_ADRS } },
    { "io32Size",	HCF_RES_INT,	{ (void *)PCI_IO_SIZE } },
    { "fbbEnable",	HCF_RES_INT,	{ (void *)TRUE } },
    { "cacheSize",	HCF_RES_INT,	{ (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",	HCF_RES_INT,	{ (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet",HCF_RES_INT,	{ (void *)TRUE } },
    { "includeFuncSet",	HCF_RES_ADDR,	{ (void *)sysPci0AutoconfigInclude } },
    { "intAssignFuncSet",HCF_RES_ADDR,	{ (void *)sysPci0AutoconfigIntrAssign }},
    /* non vxbPciAutoConfig() values */
    { "pimmrBase",	HCF_RES_ADDR,	{ (void *)PCI_BRIDGE_PIMMR_BASE_ADRS }},
    { "lclMemAddr",	HCF_RES_ADDR,	{ (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",	HCF_RES_INT,	{ (void *)LOCAL_MEM_SIZE } },
    { "mstrMemBus",	HCF_RES_ADDR,	{ (void *)PCI_MSTR_MEM_BUS } },
    { "pciConfigMechanism",HCF_RES_INT,	{ (void *)(0)} },
    { "maxBusSet",	HCF_RES_INT,	{ (void *)3 } },
    /* LAWBAR details */
    { "singleLawBar",	HCF_RES_INT,	{ (void *)(TRUE)} },
    { "singleLawBarSize",HCF_RES_INT,	{ (void *)(0x40000000)} },
    { "tgtIf",		HCF_RES_INT,	{ (void *)(LAWAR_TGTIF_PCI)} },
    { "autoConfig",	HCF_RES_INT,	{ (void *)(TRUE)} }

};
#define m85xxPci0Num NELEMENTS(m85xxPci0Resources)

#ifdef INCLUDE_CDS85XX_SECONDARY_PCI
IMPORT STATUS sysPci1AutoconfigInclude(PCI_SYSTEM *, PCI_LOC *, UINT);
IMPORT UCHAR sysPci1AutoconfigIntrAssign(PCI_SYSTEM *, PCI_LOC *, UCHAR);

const struct hcfResource m85xxPci1Resources[] = {
    { "regBase",	HCF_RES_INT,	{ (void *)(PCI_CFG_ADR_REG + 0x1000) } },
    { "mem32Addr",	HCF_RES_ADDR,	{ (void *)PCI_MEM_ADRS2 } },
    { "mem32Size",	HCF_RES_INT,	{ (void *)PCI_MEM_SIZE } },
    { "memIo32Addr",	HCF_RES_ADDR,	{ (void *)PCI_MEMIO_ADRS2 } },
    { "memIo32Size",	HCF_RES_INT,	{ (void *)PCI_MEMIO_SIZE } },
    { "io32Addr",	HCF_RES_ADDR,	{ (void *)PCI_IO_ADRS2 } },
    { "io32Size",	HCF_RES_INT,	{ (void *)PCI_IO_SIZE } },
    { "fbbEnable",	HCF_RES_INT,	{ (void *)TRUE } },
    { "cacheSize",	HCF_RES_INT,	{ (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",	HCF_RES_INT,	{ (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet",HCF_RES_INT,	{ (void *)TRUE } },
    { "includeFuncSet",	HCF_RES_ADDR,	{ (void *)sysPci1AutoconfigInclude } },
    { "intAssignFuncSet",HCF_RES_ADDR,	{ (void *)sysPci1AutoconfigIntrAssign }},
    /* non vxbPciAutoConfig() values */
    { "pimmrBase",	HCF_RES_ADDR,	{ (void *)PCI_BRIDGE_PIMMR_BASE_ADRS }},
    { "lclMemAddr",	HCF_RES_ADDR,	{ (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",	HCF_RES_INT,	{ (void *)LOCAL_MEM_SIZE } },
    { "mstrMemBus",	HCF_RES_ADDR,	{ (void *)PCI_MSTR_MEM_BUS } },
    { "pciConfigMechanism",HCF_RES_INT,	{ (void *)(0)} },
    { "maxBusSet",	HCF_RES_INT,	{ (void *)3 } },
    /* LAWBAR details */
    { "singleLawBar",	HCF_RES_INT,	{ (void *)(TRUE)} },
    { "singleLawBarSize",HCF_RES_INT,	{ (void *)(0x40000000)} },
    { "tgtIf",		HCF_RES_INT,	{ (void *)(LAWAR_TGTIF_PCI2)} },
    { "autoConfig",	HCF_RES_INT,	{ (void *)(TRUE)} }

};
#define m85xxPci1Num NELEMENTS(m85xxPci1Resources)

#endif /* INCLUDE_CDS85XX_SECONDARY_PCI */

#endif /* DRV_PCIBUS_M85XX */

#ifdef DRV_RESOURCE_M85XXCCSR
const struct hcfResource m85xxLawBarResources[] = {
    { "regBase",		HCF_RES_INT, {(void *)CCSBAR} },
    { "lawBarNum",		HCF_RES_INT, {(void *)8} },
    { "LAWBAR0",		HCF_RES_STRING, {"reserved"} },
    { "LAWBAR1",		HCF_RES_STRING, {"reserved"} },
    { "LAWBAR2",		HCF_RES_STRING, {"reserved"} },
    { "LAWBAR3",		HCF_RES_STRING, {"m85xxPci"} },
    { "LAWBAR4",		HCF_RES_STRING, {"m85xxPci"} },
    { "LAWBAR5",		HCF_RES_STRING, {"reserved"} },
    { "LAWBAR6",		HCF_RES_STRING, {"reserved"} },
    { "LAWBAR7",		HCF_RES_STRING, {"reserved"} },
    { "LAWAR_ENABLE",		HCF_RES_INT, {(void *)LAWAR_ENABLE } },
    { "LAWBAR_ADRS_SHIFT",	HCF_RES_INT, {(void *)LAWBAR_ADRS_SHIFT } },
};
#define m85xxLawBarNum    NELEMENTS(m85xxLawBarResources)
#endif /* DRV_RESOURCE_M85XXCCSR */

const struct hcfDevice hcfDeviceList[] = {
    { "ns16550", 0, VXB_BUSID_PLB, 0, cds85xxi82501Num, cds85xxi82501Resources },
    { "ns16550", 1, VXB_BUSID_PLB, 0, cds85xxi82502Num, cds85xxi82502Resources },
    { "mottsec", 0, VXB_BUSID_PLB, 0, tsecVxbEnd0Num, tsecVxbEnd0Resources },
    { "mottsec", 1, VXB_BUSID_PLB, 0, tsecVxbEnd1Num, tsecVxbEnd1Resources },
#ifdef DRV_PCIBUS_M85XX
#ifdef INCLUDE_CDS85XX_SECONDARY_PCI
    { "m85xxPci", 0, VXB_BUSID_PLB,0, m85xxPci1Num, m85xxPci1Resources },
#endif /* INCLUDE_CDS85XX_SECONDARY_PCI */

    { "m85xxPci", 0, VXB_BUSID_PLB,0, m85xxPci0Num, m85xxPci0Resources },
#endif /* DRV_PCIBUS_M85XX */

#ifdef DRV_RESOURCE_M85XXCCSR
    { "m85xxCCSR", 0, VXB_BUSID_PLB, 0, m85xxLawBarNum, m85xxLawBarResources },
#endif /* DRV_RESOURCE_M85XXCCSR */
    };

const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] =
    {
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, {(void *)0} }
    };
