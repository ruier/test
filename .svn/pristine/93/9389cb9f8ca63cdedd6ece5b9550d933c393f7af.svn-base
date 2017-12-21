/* hwconf.c - Hardware configuration support module */

/* Copyright (c) 2006 Wind River Systems, Inc. */

/*
modification history
--------------------
01c,21aug08,wap  Add PCI support via legacy/virtual PCI driver
01b,30aug06,wap  Update for new PHY handling scheme
01a,07feb06,wap  written
*/

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#include "config.h"
#include "wrSbcPowerQuiccII.h"

LOCAL const struct hcfResource mdio0Resources[] = {
    { "regBase", HCF_RES_INT,
        { (void *)(M8260_IOP_PCDIR(INTERNAL_MEM_MAP_ADDR)) } },
    { "regBase1", HCF_RES_INT, { (void *)(0) } },
    { "regBase2", HCF_RES_INT,
        { (void *)(M8260_IOP_PCDAT(INTERNAL_MEM_MAP_ADDR)) } },
    { "regBase3", HCF_RES_INT, { (void *)(0) } },
    { "regBase4", HCF_RES_INT,
        { (void *)(M8260_IOP_PCDIR(INTERNAL_MEM_MAP_ADDR)) } },
    { "regBase5", HCF_RES_INT,
        { (void *)(M8260_IOP_PCDAT(INTERNAL_MEM_MAP_ADDR)) } },
    { "regBase6", HCF_RES_INT,
        { (void *)(M8260_IOP_PCDAT(INTERNAL_MEM_MAP_ADDR)) } },
    { "mdioRegWidth", HCF_RES_INT, { (void *)(4) } },
    { "mdioClkDirInBit", HCF_RES_INT, { (void *)(0) } },
    { "mdioClkDirOutBit", HCF_RES_INT, { (void *)(PC10) } },
    { "mdioClkBit", HCF_RES_INT, { (void *)(PC10) } },
    { "mdioDataDirInBit", HCF_RES_INT, { (void *)(0) } },
    { "mdioDataDirOutBit", HCF_RES_INT, { (void *)(PC9) } },
    { "mdioDataInBit", HCF_RES_INT, { (void *)(PC9) } },
    { "mdioDataOutBit", HCF_RES_INT, { (void *)(PC9) } },
};
#define mdio0Num NELEMENTS(mdio0Resources)

/*
 * The FCC controller has two register ranges: the parameter RAM (PRAM)
 * range, and the internal RAM (IRAM) range. The range offsets, relative
 * to the start of the mapped register area, are:
 *
 *		DPRAM		IRAM
 *		-----		----
 * FCC1:	0x8400		0x11300
 * FCC2:	0x8500		0x11320
 * FCC3:	0x8600		0x11340
 *
 * The wrSbcPowerQuiccII board uses FCC1, FCC2 and FCC2 for ethernet.
 * Note that the 'FCC number' is different from the unit number: it
 * refers to the controller number as defined in the 8260 manual, which
 * calls them FCC1, FCC2 and FCC3.
 * 
 * The wrSbcPowerQuiccII board also has three MII PHYs. Their RX/TX
 * data pins are wired to FCC1, FCC2 and FCC3, but their MDIO pins are
 * all wired to pins 9 and 10 on parallel I/O port C. The PHYs
 * are strapped according to the following table:
 *
 *		Data port pins	MII address
 *		--------------	-----------
 * PHY0:	FCC1		0
 * PHY1:	FCC2		1
 * PHY2:	FCC3		2
 *
 * The PHYs are actually managed by the MDIO driver, and referenced
 * indirectly by the FCC driver by calling the MDIO driver's methods.
 * The FCC driver must be told the name and unit number of the PHY
 * handling device, and the address of its corresponding PHY.
 */

LOCAL const struct hcfResource fcc0Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)(INTERNAL_MEM_MAP_ADDR + 0x8400) } },
    { "regBase1", HCF_RES_INT, { (void *)(INTERNAL_MEM_MAP_ADDR + 0x11300) } },
    { "intr0", HCF_RES_INT, { (void *)(INUM_FCC1) } },
    { "intr0Level", HCF_RES_INT, { (void *)(INUM_FCC1) } },
    { "fccnum", HCF_RES_INT, { (void *)(1) } },
    { "phyAddr", HCF_RES_INT, { (void *)0 } },
    { "miiIfName", HCF_RES_STRING, { (void *)"mdio" } },
    { "miiIfUnit", HCF_RES_INT, { (void *)10 } },
};
#define fcc0Num NELEMENTS(fcc0Resources)

LOCAL const struct hcfResource fcc1Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)(INTERNAL_MEM_MAP_ADDR + 0x8500) } },
    { "regBase1", HCF_RES_INT, { (void *)(INTERNAL_MEM_MAP_ADDR + 0x11320) } },
    { "intr0", HCF_RES_INT, { (void *)(INUM_FCC2) } },
    { "intr0Level", HCF_RES_INT, { (void *)(INUM_FCC2) } },
    { "fccnum", HCF_RES_INT, { (void *)(2) } },
    { "phyAddr", HCF_RES_INT, { (void *)1 } },
    { "miiIfName", HCF_RES_STRING, { (void *)"mdio" } },
    { "miiIfUnit", HCF_RES_INT, { (void *)10 } },
};
#define fcc1Num NELEMENTS(fcc1Resources)

LOCAL const struct hcfResource fcc2Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)(INTERNAL_MEM_MAP_ADDR + 0x8600) } },
    { "regBase1", HCF_RES_INT, { (void *)(INTERNAL_MEM_MAP_ADDR + 0x11340) } },
    { "intr0", HCF_RES_INT, { (void *)(INUM_FCC3) } },
    { "intr0Level", HCF_RES_INT, { (void *)(INUM_FCC3) } },
    { "fccnum", HCF_RES_INT, { (void *)(3) } },
    { "phyAddr", HCF_RES_INT, { (void *)2 } },
    { "miiIfName", HCF_RES_STRING, { (void *)"mdio" } },
    { "miiIfUnit", HCF_RES_INT, { (void *)10 } },
};
#define fcc2Num NELEMENTS(fcc2Resources)

/*
 * The SCC controller also has two register ranges: the parameter RAM (PRAM)
 * range, and the internal RAM (IRAM) range. The range offsets, relative
 * to the start of the mapped register area, are:
 *
 *		DPRAM		IRAM
 *		-----		----
 * SCC1:	0x8000		0x11A00
 * SCC2:	0x8100		0x11A20
 * SCC3:	0x8200		0x11A40
 * SCC4:	0x8300		0x11A60
 *
 * The wrSbcPowerQuiccII board only uses SCC1 for ethernet.
 */

LOCAL const struct hcfResource scc0Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)(INTERNAL_MEM_MAP_ADDR + 0x8000) } },
    { "regBase1", HCF_RES_INT, { (void *)(INTERNAL_MEM_MAP_ADDR + 0x11A00) } },
    { "sccnum", HCF_RES_INT, { (void *)(1) } },
    { "intr0", HCF_RES_INT, { (void *)(INUM_SCC1) } },
    { "intr0Level", HCF_RES_INT, { (void *)(INUM_SCC1) } },
};
#define scc0Num NELEMENTS(scc0Resources)

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

#ifdef INCLUDE_PCI
STATUS pciDeviceIntConnect(int, VOIDFUNCPTR *, int);
STATUS pciDeviceIntDisconnect2(int, VOIDFUNCPTR *, int);

STATUS pciDeviceIntEnable(int);
STATUS pciDeviceIntDisable(int);

LOCAL const struct hcfResource pci0Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)0 } },
    { "intConnect", HCF_RES_ADDR, { (void *)pciDeviceIntConnect } },
    { "intDisconnect", HCF_RES_ADDR, { (void *)pciDeviceIntDisconnect2 } },
    { "intEnable", HCF_RES_ADDR, { (void *)pciDeviceIntEnable } },
    { "intDisable", HCF_RES_ADDR, { (void *)pciDeviceIntDisable } }
};
#define pci0Num NELEMENTS(pci0Resources)
#endif

const struct hcfDevice hcfDeviceList[] = {
    { "motscc", 0, VXB_BUSID_PLB, 0, scc0Num, scc0Resources },
    { "mdio", 10, VXB_BUSID_PLB, 0, mdio0Num, mdio0Resources },
    { "cpm", 0, VXB_BUSID_PLB, 0, cpm0Num, cpm0Resources },
    { "motfcc", 0, VXB_BUSID_PLB, 0, fcc0Num, fcc0Resources },
    { "motfcc", 1, VXB_BUSID_PLB, 0, fcc1Num, fcc1Resources },
    { "motfcc", 2, VXB_BUSID_PLB, 0, fcc2Num, fcc2Resources },
#ifdef INCLUDE_PCI
    { "vxbVirtualPciAdaptor", 0, VXB_BUSID_PLB, 0, pci0Num, pci0Resources }
#endif
};

const int hcfDeviceNum =NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] =
    {
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, {(void *)0} }
    };

