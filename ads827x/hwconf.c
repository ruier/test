/* hwconf.c - Hardware configuration support module */

/*
 * Copyright (c) 2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,24apr05,l_g  Created.
*/

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#include <drv/parallel/m8260IOPort.h>
#include "config.h"

#define CP_OFFSET   0x119C0

/*
 * We define a special cpm driver which has access to the
 * CP command register. This driver is used to arbitrate access
 * to the CP since FCC driver need to issue commands to the CP 
 * via the command register.
 */

LOCAL const struct hcfResource cpm0Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)(INTERNAL_MEM_MAP_ADDR + CP_OFFSET) } },
};
#define cpm0Num NELEMENTS(cpm0Resources)

/*
 * The MDIO driver is used to provide access to the management registers
 * on the PHYs connected to the FCC ports. The MDIO pins for these devices
 * are connected to parallel port C on the CPM.
 */

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
    { "mdioClkDirOutBit", HCF_RES_INT, { (void *)(PC19) } },
    { "mdioClkBit", HCF_RES_INT, { (void *)(PC19) } },
    { "mdioDataDirInBit", HCF_RES_INT, { (void *)(0) } },
    { "mdioDataDirOutBit", HCF_RES_INT, { (void *)(PC18) } },
    { "mdioDataInBit", HCF_RES_INT, { (void *)(PC18) } },
    { "mdioDataOutBit", HCF_RES_INT, { (void *)(PC18) } },
};
#define mdio0Num NELEMENTS(mdio0Resources)

#define FCC1_DPRAM_OFFSET  0x8400
#define FCC2_DPRAM_OFFSET  0x8500
#define FCC1_REG_OFFSET    0x11300
#define FCC2_REG_OFFSET    0x11320
#define FCC1_VEC           32
#define FCC2_VEC           33
#define FCC1_PHY_ADRS      0
#define FCC2_PHY_ADRS      3

/*
 * The FCC controller has two register ranges: the parameter RAM (PRAM)
 * range, and the internal RAM (IRAM) range. The range offsets, relative
 * to the start of the mapped register area, are:
 *
 *              DPRAM           IRAM
 *              -----           ----
 * FCC1:        0x8400          0x11300
 * FCC2:        0x8500          0x11320
 *
 * The ads827x boards use FCC1 and FCC2 for ethernet.
 * Note that the 'FCC number' is different from the unit number: it
 * refers to the controller number as defined in the 8272 manual, which
 * calls them FCC1, FCC2.
 *
 * The wrSbc85xx board also has two MII PHYs. Their RX/TX
 * data pins are wired to FCC1 and FCC2, but their MDIO pins are
 * all wired to pins 18 and 19 on parallel I/O port C. The PHYs
 * are strapped according to the following table:
 *
 *              Data port pins  MII address
 *              --------------  -----------
 * PHY1:        FCC1            0
 * PHY2:        FCC2            3
 *
 * The PHYs are actually managed by the MDIO driver, and referenced
 * indirectly by the FCC driver by calling the MDIO driver's methods.
 * The FCC driver must be told the name and unit number of the PHY
 * handling device, and the address of its corresponding PHY.
 */

LOCAL const struct hcfResource fcc0Resources[] = {
    { "regBase",   HCF_RES_INT,
       { (void *)(INTERNAL_MEM_MAP_ADDR + FCC1_DPRAM_OFFSET) } },
    { "regBase1",  HCF_RES_INT,
       { (void *)(INTERNAL_MEM_MAP_ADDR + FCC1_REG_OFFSET) } },
    { "intr0",     HCF_RES_INT,    { (void *)(FCC1_VEC) } },
    { "intr0Level",HCF_RES_INT,    { (void *)(INUM_FCC1) } },
    { "fccnum",    HCF_RES_INT,    { (void *)(1) } },
    { "phyAddr",   HCF_RES_INT,    { (void *)FCC1_PHY_ADRS } },
    { "miiIfName", HCF_RES_STRING, { (void *)"mdio" } },
    { "miiIfUnit", HCF_RES_INT,    { (void *)0 } },

};
#define fcc0Num NELEMENTS(fcc0Resources)

LOCAL const struct hcfResource fcc1Resources[] = {
    { "regBase",   HCF_RES_INT,
       { (void *)(INTERNAL_MEM_MAP_ADDR + FCC2_DPRAM_OFFSET) } },
    { "regBase1",  HCF_RES_INT,
       { (void *)(INTERNAL_MEM_MAP_ADDR + FCC2_REG_OFFSET) } },
    { "intr0",     HCF_RES_INT,    { (void *)(FCC2_VEC) } },
    { "intr0Level",HCF_RES_INT,    { (void *)(INUM_FCC2) } },
    { "fccnum",    HCF_RES_INT,    { (void *)(2) } },
    { "phyAddr",   HCF_RES_INT,    { (void *)FCC2_PHY_ADRS } },
    { "miiIfName", HCF_RES_STRING, { (void *)"mdio" } },
    { "miiIfUnit", HCF_RES_INT,    { (void *)0 } },
};
#define fcc1Num NELEMENTS(fcc1Resources)

const struct hcfDevice hcfDeviceList[] = {
    { "mdio", 0, VXB_BUSID_PLB, 0, mdio0Num, mdio0Resources },
    { "cpm", 0, VXB_BUSID_PLB, 0, cpm0Num, cpm0Resources },
    { "motfcc", 0, VXB_BUSID_PLB, 0, fcc0Num, fcc0Resources },
    { "motfcc", 1, VXB_BUSID_PLB, 0, fcc1Num, fcc1Resources },
};

const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] =
    {
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, {(void *)0} }
    };
