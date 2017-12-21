/* hwconf.c - Hardware configuration support module */

/*
 * Copyright (c) 2005-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01p,28jul08,fao  Fix INTERNAL_MEM_MAP_ADDR macro (WIND00124448)
01o,06jun08,dlk  Make hcfDeviceList entries for tsec devices unconditional.
01n,12aug07,wap  Fix resource initialization for PCI and CCSR drivers
                 (WIND00099736)
01m,01aug07,h_k  added LAWAR_ENABLE and LAWBAR_ADRS_SHIFT.
01l,05mar07,dtr  Mod for new PCI bus controller driver.
01k,01feb07,wap  Convert to VxBus TSEC driver
01k,22jan07,pmr  fix WIND00083778: serial driver issue
01j,22sep06,pmr  fix WIND00065418: incorrect parameters for 8540 serial
01i,08sep06,pmr  fix brace usage.
01h,30jul06,tor  Add PCI support
01g,07feb06,wap  Add VxBus parameter table
01f,04jan06,wap  Correct TSEC setup so interrupts work
01e,26oct05,mdo  renaming header files
01d,15sep05,to   
01c,14sep05,mdo  Add vxb prefix
01b,08aug05,mdo  Change WINDBUS to VXBUS
01a,28jul05,rls  Created.
*/

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#include <drv/pci/pciConfigLib.h>
#include <drv/pci/pciAutoConfigLib.h>
#include <drv/pci/pciIntLib.h>
#include "sysEpic.h"
#include "config.h"

/*
 * On the wrSbc8560 board, PHYs are physically wired according
 * to the following table:
 *
 * 		Data port pins	Management port pins	MII address
 *		--------------	--------------------	-----------
 * PHY0:	TSEC0		TSEC0			25
 * PHY1:	TSEC1		TSEC0			26
 *
 * The tricky part is that both PHYs have their management pins
 * connected to TSEC0. The VxBus TSEC driver allows us to specify
 * the name of the device that provides PHY management support.
 * This device is "mottsec0" in both cases. We also specify the
 * "phyAddr" resource for each device, which is 25 for TSEC0
 * and 26 for TSEC1.
 */

const struct hcfResource tsecVxbEnd0Resources[] = {
    { "regBase",	HCF_RES_INT,	{ (void *)(CCSBAR + 0x24000) } },
    { "intr0",		HCF_RES_INT,	{ (void *)EPIC_TSEC1TX_INT_VEC } },
    { "intr0Level",	HCF_RES_INT,	{ (void *)EPIC_TSEC1TX_INT_VEC } },
    { "intr1",		HCF_RES_INT,	{ (void *)EPIC_TSEC1RX_INT_VEC } },
    { "intr1Level",	HCF_RES_INT,	{ (void *)EPIC_TSEC1RX_INT_VEC } },
    { "intr2",		HCF_RES_INT,	{ (void *)EPIC_TSEC1ERR_INT_VEC } },
    { "intr2Level",	HCF_RES_INT,	{ (void *)EPIC_TSEC1ERR_INT_VEC } },
    { "phyAddr",	HCF_RES_INT,	{ (void *)25 } },
    { "miiIfName",	HCF_RES_STRING,	{ (void *)"mottsec" } },
    { "miiIfUnit",	HCF_RES_INT,	{ (void *)0 } },
};
#define tsecVxbEnd0Num NELEMENTS(tsecVxbEnd0Resources)

const struct hcfResource tsecVxbEnd1Resources[] = {
    { "regBase",	HCF_RES_INT,	{ (void *)(CCSBAR + 0x25000) } },
    { "intr0",		HCF_RES_INT,	{ (void *)EPIC_TSEC2TX_INT_VEC } },
    { "intr0Level",	HCF_RES_INT,	{ (void *)EPIC_TSEC2TX_INT_VEC } },
    { "intr1",		HCF_RES_INT,	{ (void *)EPIC_TSEC2RX_INT_VEC } },
    { "intr1Level",	HCF_RES_INT,	{ (void *)EPIC_TSEC2RX_INT_VEC } },
    { "intr2",		HCF_RES_INT,	{ (void *)EPIC_TSEC2ERR_INT_VEC } },
    { "intr2Level",	HCF_RES_INT,	{ (void *)EPIC_TSEC2ERR_INT_VEC } },
    { "phyAddr",	HCF_RES_INT,	{ (void *)26 } },
    { "miiIfName",	HCF_RES_STRING,	{ (void *)"mottsec" } },
    { "miiIfUnit",	HCF_RES_INT,	{ (void *)0 } },
};
#define tsecVxbEnd1Num NELEMENTS(tsecVxbEnd1Resources)

IMPORT void bspNs16550Connect (int);
IMPORT UINT32 sysClkFreqGet (void);

#ifdef DRV_PCIBUS_M85XX
IMPORT STATUS sysPciAutoconfigInclude(PCI_SYSTEM *, PCI_LOC *, UINT);
IMPORT UCHAR sysPciAutoconfigIntrAssign(PCI_SYSTEM *, PCI_LOC *, UCHAR);

const struct hcfResource m85xxPci0Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)PCI_REGBASE } },
    { "mem32Addr",      HCF_RES_ADDR,   { (void *)PCI_MEM_ADRS } },
    { "mem32Size",      HCF_RES_INT,    { (void *)PCI_MEM_SIZE } },
    { "memIo32Addr",    HCF_RES_ADDR,   { (void *)PCI_MEMIO_ADRS } },
    { "memIo32Size",    HCF_RES_INT,    { (void *)PCI_MEMIO_SIZE } },
    { "io32Addr",       HCF_RES_ADDR,   { (void *)PCI_IO_ADRS } },
    { "io32Size",       HCF_RES_INT,    { (void *)PCI_IO_SIZE } },
    { "fbbEnable",      HCF_RES_INT,    { (void *)TRUE } },
    { "cacheSize",      HCF_RES_INT,    { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",   HCF_RES_INT,    { (void *)PCI_LAT_TIMER } },
    { "autoIntRouteSet",HCF_RES_INT,    { (void *)TRUE } },
    { "includeFuncSet", HCF_RES_ADDR,   { (void *)sysPciAutoconfigInclude } },
    { "intAssignFuncSet",HCF_RES_ADDR,  { (void *)sysPciAutoconfigIntrAssign }},
    /* non vxbPciAutoConfig() values */
    { "pimmrBase",      HCF_RES_ADDR,   { (void *)PCI_BRIDGE_PIMMR_BASE_ADRS }},
    { "lclMemAddr",     HCF_RES_ADDR,   { (void *)LOCAL_MEM_LOCAL_ADRS } },
    { "lclMemMapSize",  HCF_RES_INT,    { (void *)(512 * 1024 * 1024) } },
    { "mstrMemBus",     HCF_RES_ADDR,   { (void *)PCI_MSTR_MEM_BUS } },
    { "pciConfigMechanism",HCF_RES_INT, { (void *)(0)} },
    { "maxBusSet",      HCF_RES_INT,    { (void *)3 } },
    /* LAWBAR details */
    { "singleLawBar",   HCF_RES_INT,    { (void *)(TRUE)} },
    { "singleLawBarSize",HCF_RES_INT,   { (void *)(0x40000000)} },
    { "tgtIf",          HCF_RES_INT,    { (void *)(LAWAR_TGTIF_PCI)} },
    { "autoConfig",     HCF_RES_INT,    { (void *)(TRUE)} }
};

#define m85xxPci0Num NELEMENTS(m85xxPci0Resources)
#endif /* DRV_PCIBUS_M85XX */

#ifdef INCLUDE_CPM
const struct hcfResource ns165500Resources[] = { 
    { "regBase",     HCF_RES_INT, {(void *)COM1_BRD_ADRS} },
    { "irq",         HCF_RES_INT, {(void *)COM1_INT_VEC} },
    { "irqLevel",    HCF_RES_INT, {(void *)COM1_INT_LVL} },
    { "clkFreq",     HCF_RES_INT, {(void *)XTAL_BRD} },
    { "regInterval", HCF_RES_INT, {(void *)UART_REG_INTVL} },
    { "bspConnectRtn", HCF_RES_ADDR, {(void *)bspNs16550Connect } },
};
#define ns165500Num NELEMENTS(ns165500Resources)

const struct hcfResource ns165501Resources[] = { 
    { "regBase",     HCF_RES_INT, {(void *)COM2_BRD_ADRS} },
    { "irq",         HCF_RES_INT, {(void *)COM2_INT_VEC} },
    { "irqLevel",    HCF_RES_INT, {(void *)COM2_INT_LVL} },
    { "clkFreq",     HCF_RES_INT, {(void *)XTAL_BRD} },
    { "regInterval", HCF_RES_INT, {(void *)UART_REG_INTVL} },
    { "bspConnectRtn", HCF_RES_ADDR, {(void *)bspNs16550Connect } },
};
#define ns165501Num NELEMENTS(ns165501Resources)

#else /* ! defined INCLUDE_CPM */
const struct hcfResource ns165500Resources[] = { 
    { "regBase",     HCF_RES_INT, {(void *)COM1_CHIP_ADRS} },
    { "irq",         HCF_RES_INT, {(void *)EPIC_DUART_INT_VEC} },
    { "irqLevel",    HCF_RES_INT, {(void *)EPIC_DUART_INT_VEC} },
    { "clkFreq",     HCF_RES_ADDR, {(void *)sysClkFreqGet} },
    { "regInterval", HCF_RES_INT, {(void *)UART_REG_INTVL} },
    { "bspConnectRtn", HCF_RES_ADDR, {(void *)bspNs16550Connect } },
};
#define ns165500Num NELEMENTS(ns165500Resources)

const struct hcfResource ns165501Resources[] = { 
    { "regBase",     HCF_RES_INT, {(void *)COM2_CHIP_ADRS} },
    { "irq",         HCF_RES_INT, {(void *)EPIC_DUART_INT_VEC} },
    { "irqLevel",    HCF_RES_INT, {(void *)EPIC_DUART_INT_VEC} },
    { "clkFreq",     HCF_RES_ADDR, {(void *)sysClkFreqGet} },
    { "regInterval", HCF_RES_INT, {(void *)UART_REG_INTVL} },
    { "bspConnectRtn", HCF_RES_ADDR, {(void *)bspNs16550Connect } },
};
#define ns165501Num NELEMENTS(ns165501Resources)

#endif /* INCLUDE_CPM */

const struct hcfResource m85xxCCSR0Resources[] = {
    { "regBase",	HCF_RES_INT,		{ (void *)CCSBAR  } },
    { "lawBarNum",	HCF_RES_INT,		{ (void *)8} },
    { "LAWBAR0",	HCF_RES_STRING,		{ (void *)"reserved" } },
    { "LAWBAR1",	HCF_RES_STRING,		{ (void *)"reserved" } },
    { "LAWBAR2",	HCF_RES_STRING,		{ (void *)"reserved" } },
    { "LAWBAR3",	HCF_RES_STRING,		{ (void *)"reserved" } },
    { "LAWBAR4",	HCF_RES_STRING,		{ (void *)"m85xxPci" } },
    { "LAWBAR5",	HCF_RES_STRING,		{ (void *)"reserved" } },
    { "LAWBAR6",	HCF_RES_STRING,		{ (void *)"reserved" } },
    { "LAWBAR7",	HCF_RES_STRING,		{ (void *)"m8560Rio" } },
    { "LAWAR_ENABLE",	HCF_RES_INT,		{ (void *)LAWAR_ENABLE } },
    { "LAWBAR_ADRS_SHIFT", HCF_RES_INT,		{ (void *)LAWBAR_ADRS_SHIFT } },
};
#define m85xxCCSR0Num NELEMENTS(m85xxCCSR0Resources)

#define INTERNAL_MEM_MAP_ADDR (CCSBAR + 0x80000)

#ifdef INCLUDE_MDIO

#define INUM_FCC1	32
#define INUM_FCC2	33
#define INUM_FCC3	34

LOCAL const struct hcfResource mdio0Resources[] = {
    { "regBase",		HCF_RES_INT,
        { (void *)(M8260_IOP_PCDIR(INTERNAL_MEM_MAP_ADDR)) } },
    { "regBase1",		HCF_RES_INT, { (void *)(0) } },
    { "regBase2",		HCF_RES_INT,
        { (void *)(M8260_IOP_PCDAT(INTERNAL_MEM_MAP_ADDR)) } },
    { "regBase3",		HCF_RES_INT, { (void *)(0) } },
    { "regBase4",		HCF_RES_INT,
        { (void *)(M8260_IOP_PCDIR(INTERNAL_MEM_MAP_ADDR)) } },
    { "regBase5",		HCF_RES_INT,
        { (void *)(M8260_IOP_PCDAT(INTERNAL_MEM_MAP_ADDR)) } },
    { "regBase6",		HCF_RES_INT,
        { (void *)(M8260_IOP_PCDAT(INTERNAL_MEM_MAP_ADDR)) } },
    { "mdioRegWidth",		HCF_RES_INT, { (void *)(4) } },
    { "mdioClkDirInBit",	HCF_RES_INT, { (void *)(0) } },
    { "mdioClkDirOutBit",	HCF_RES_INT, { (void *)(PC10) } },
    { "mdioClkBit",		HCF_RES_INT, { (void *)(PC10) } },
    { "mdioDataDirInBit",	HCF_RES_INT, { (void *)(0) } },
    { "mdioDataDirOutBit",	HCF_RES_INT, { (void *)(PC9) } },
    { "mdioDataInBit",		HCF_RES_INT, { (void *)(PC9) } },
    { "mdioDataOutBit",		HCF_RES_INT, { (void *)(PC9) } },
};
#define mdio0Num NELEMENTS(mdio0Resources)

#endif /* INCLUDE_MDIO */

#ifdef INCLUDE_VXB_CPM

/*
 * We define a special cpm driver which has access to the
 * CP command register. This driver is used to arbitrate access
 * to the CP since both the SCC and FCC drivers need to issue
 * commands to the CP via the command register.
 */

LOCAL const struct hcfResource cpm0Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)(INTERNAL_MEM_MAP_ADDR + 0x119C0) } },
};
#define cpm0Num NELEMENTS(cpm0Resources)

#endif /* INCLUDE_VXB_CPM */

#ifdef INCLUDE_FCC_VXB_END

/*
 * The FCC controller has two register ranges: the parameter RAM (PRAM)
 * range, and the internal RAM (IRAM) range. The range offsets, relative
 * to the start of the mapped register area, are:
 *
 *              DPRAM           IRAM
 *              -----           ----
 * FCC1:        0x8400          0x11300
 * FCC2:        0x8500          0x11320
 * FCC3:        0x8600          0x11340
 *
 * The wrSbc85xx boards use FCC2 and FCC2 for ethernet.
 * Note that the 'FCC number' is different from the unit number: it
 * refers to the controller number as defined in the 8660 manual, which
 * calls them FCC1, FCC2 and FCC3.
 *
 * The wrSbc85xx board also has two MII PHYs. Their RX/TX
 * data pins are wired to FCC2 and FCC3, but their MDIO pins are
 * all wired to pins 9 and 10 on parallel I/O port C. The PHYs
 * are strapped according to the following table:
 *
 *              Data port pins  MII address
 *              --------------  -----------
 * PHY1:        FCC2            1
 * PHY2:        FCC3            0
 *
 * The PHYs are actually managed by the MDIO driver, and referenced
 * indirectly by the FCC driver by calling the MDIO driver's methods.
 * The FCC driver must be told the name and unit number of the PHY
 * handling device, and the address of its corresponding PHY.
 */

LOCAL const struct hcfResource fcc0Resources[] = {
    { "regBase",	HCF_RES_INT,
        { (void *)(INTERNAL_MEM_MAP_ADDR + 0x8500) } },
    { "regBase1",	HCF_RES_INT,
        { (void *)(INTERNAL_MEM_MAP_ADDR + 0x11320) } },
    { "intr0",		HCF_RES_INT,	{ (void *)(INUM_FCC2) } },
    { "intr0Level",	HCF_RES_INT,	{ (void *)(INUM_FCC2) } },
    { "fccnum",		HCF_RES_INT,	{ (void *)(2) } },
    { "phyAddr",	HCF_RES_INT,	{ (void *)1 } },
    { "miiIfName",	HCF_RES_STRING,	{ (void *)"mdio" } },
    { "miiIfUnit",	HCF_RES_INT,	{ (void *)10 } },

};
#define fcc0Num NELEMENTS(fcc0Resources)

LOCAL const struct hcfResource fcc1Resources[] = {
    { "regBase",	HCF_RES_INT,
        { (void *)(INTERNAL_MEM_MAP_ADDR + 0x8600) } },
    { "regBase1",	HCF_RES_INT,
        { (void *)(INTERNAL_MEM_MAP_ADDR + 0x11340) } },
    { "intr0",		HCF_RES_INT,	{ (void *)(INUM_FCC3) } },
    { "intr0Level",	HCF_RES_INT,	{ (void *)(INUM_FCC3) } },
    { "fccnum",		HCF_RES_INT,	{ (void *)(3) } },
    { "phyAddr",	HCF_RES_INT,	{ (void *)0 } },
    { "miiIfName",	HCF_RES_STRING,	{ (void *)"mdio" } },
    { "miiIfUnit",	HCF_RES_INT,	{ (void *)10 } },
};
#define fcc1Num NELEMENTS(fcc1Resources)

#endif /* INCLUDE_FCC_VXB_END */

const struct hcfDevice hcfDeviceList[] = {
#ifdef INCLUDE_MDIO
    { "mdio", 10, VXB_BUSID_PLB, 0, mdio0Num, mdio0Resources },
#endif
#ifdef INCLUDE_VXB_CPM
    { "cpm", 0, VXB_BUSID_PLB, 0, cpm0Num, cpm0Resources },
#endif
#ifdef INCLUDE_FCC_VXB_END
    { "motfcc", 0, VXB_BUSID_PLB, 0, fcc0Num, fcc0Resources },
    { "motfcc", 1, VXB_BUSID_PLB, 0, fcc1Num, fcc1Resources },
#endif
    { "mottsec", 0, VXB_BUSID_PLB, 0, tsecVxbEnd0Num, tsecVxbEnd0Resources },
    { "mottsec", 1, VXB_BUSID_PLB, 0, tsecVxbEnd1Num, tsecVxbEnd1Resources },
    { "m85xxCCSR", 0, VXB_BUSID_PLB,0, m85xxCCSR0Num, m85xxCCSR0Resources },
#ifdef DRV_PCIBUS_M85XX
    { "m85xxPci", 0, VXB_BUSID_PLB,0, m85xxPci0Num, m85xxPci0Resources },
#endif /* DRV_PCIBUS_M85XX */
    { "ns16550", 0, VXB_BUSID_PLB, 0, ns165500Num, ns165500Resources },
#ifdef INCLUDE_MEZZ_COM2
    { "ns16550", 1, VXB_BUSID_PLB, 0, ns165501Num, ns165501Resources },
#endif /* INCLUDE_MEZZ_COM2 */
};

const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] =
    {
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, {(void *)0} }
    };
