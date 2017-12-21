/* hwconf.c - Hardware configuration support module */

/* Copyright 2008-2009 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */


/* 
Modfication Histroy 
--------------------
01b,20apr09,wap  Switch to VxBus EMAC driver
01a,21apr08,fao  created
*/

/* includes */

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/vxbus/vxbPciLib.h>
#include <hwif/util/vxbParamSys.h>
#include <hwif/vxbus/hwConf.h>
#include "config.h"

#include "ppc405GP.h"

/* externals */

IMPORT UINT32 vxbSysGetClkFreq(struct vxbDev * pDev);

struct hcfResource ns16650channel1Resources[] = {
    {"regBase",     HCF_RES_INT,  {(void *)UART0_BASE} },
    {"irq",         HCF_RES_INT,  {(void *)(INT_VEC_UART0)} },
    {"regInterval", HCF_RES_INT,  {(void *)UART_REG_ADDR_INTERVAL} },
    {"irqLevel",    HCF_RES_INT,  {(void *)INT_LVL_UART0} }, 
    {"clkFreq",     HCF_RES_ADDR, {(void *)vxbSysGetClkFreq} }
};

#define ns16550Channel1Num	NELEMENTS(ns16650channel1Resources)

struct hcfResource ns16650channel2Resources[] = {
    {"regBase",     HCF_RES_INT,  {(void *)UART1_BASE} },
    {"irq",         HCF_RES_INT,  {(void *)(INT_VEC_UART1)} },
    {"regInterval", HCF_RES_INT,  {(void *)UART_REG_ADDR_INTERVAL} },
    {"irqLevel",    HCF_RES_INT,  {(void *)INT_LVL_UART1} }, 
    {"clkFreq",     HCF_RES_ADDR, {(void *)vxbSysGetClkFreq} }
};          

#define ns16550Channel2Num	NELEMENTS(ns16650channel2Resources)

/*
 * The PHY on the walnut board is strapped for
 * MDIO address 1.
 */

const struct hcfResource emac0Resources[] = {
    { "regBase",   HCF_RES_INT,    { (void *)(EMAC0_BASE) } },
    { "intr",      HCF_RES_INT,    { (void *)INT_VEC_ENET_0 } },
    { "intrLevel", HCF_RES_INT,    { (void *)INT_LVL_ENET_0 } },
    { "phyAddr",   HCF_RES_INT,    { (void *)EMAC_PHY_ADRS } },
    { "miiIfName", HCF_RES_STRING, { (void *)"emac" } },
    { "miiIfUnit", HCF_RES_INT,    { (void *)0 } }
};

#define emac0Num NELEMENTS(emac0Resources)

const struct hcfResource mal0Resources[] = {
    { "regBase",    HCF_RES_INT, { (void *)(MAL0_DCR_BASE) } },
    { "intr0",      HCF_RES_INT, { (void *)(INT_VEC_MAL_TXEOB) } },
    { "intr0Level", HCF_RES_INT, { (void *)(INT_LVL_MAL_TXEOB) } },
    { "intr1",      HCF_RES_INT, { (void *)(INT_VEC_MAL_RXEOB) } },
    { "intr1Level", HCF_RES_INT, { (void *)(INT_LVL_MAL_RXEOB) } },
    { "intr2",      HCF_RES_INT, { (void *)(INT_VEC_MAL_SERR) } },
    { "intr2Level", HCF_RES_INT, { (void *)(INT_LVL_MAL_SERR) } },
    { "intr3",      HCF_RES_INT, { (void *)(INT_VEC_MAL_TXDE) } },
    { "intr3Level", HCF_RES_INT, { (void *)(INT_LVL_MAL_TXDE) } },
    { "intr4",      HCF_RES_INT, { (void *)(INT_VEC_MAL_RXDE) } },
    { "intr4Level", HCF_RES_INT, { (void *)(INT_LVL_MAL_RXDE) } }
};

#define mal0Num NELEMENTS(mal0Resources)

const struct hcfDevice hcfDeviceList[] = {
    { "mal", 0, VXB_BUSID_PLB, 0, mal0Num, mal0Resources },
    { "emac", 0, VXB_BUSID_PLB, 0, emac0Num, emac0Resources },
    { "ns16550", 0, VXB_BUSID_PLB, 0, ns16550Channel1Num, ns16650channel1Resources },
    { "ns16550", 1, VXB_BUSID_PLB, 0, ns16550Channel2Num, ns16650channel2Resources }
};

const int hcfDeviceNum = NELEMENTS(hcfDeviceList);


VXB_INST_PARAM_OVERRIDE sysInstParamTable[] =
    {
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, {(void *)0} }
    };

