/* hwconf.c - hardware configuration support module */

/*
 * Copyright (c) 2008-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,12mar09,e_d  Updated for quad UARTs (WIND00160039)
01c,15oct08,y_x  Added virtual vxBus PCI to fix USB issue (WIND00135828)
01b,03sep08,b_m  remove booke timer vectors.
01a,23apr08,x_f  written.
*/

/* includes */

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>

#include "config.h"
#include "sysEpic.h"
#include "sbc8540A.h"


IMPORT UINT32 sysCpuBusSpd (void);

const struct hcfResource ns1655x0Resources[] = { 
    { "regBase",       HCF_RES_INT,  { (void *)COM0_BASE_ADRS } },
    { "irq",           HCF_RES_INT,  { (void *)EPIC_DUART_INT_VEC } },
    { "irqLevel",      HCF_RES_INT,  { (void *)EPIC_DUART_INT_VEC } },
    { "clkFreq",       HCF_RES_ADDR, { (void *)&sysCpuBusSpd } },
    { "regInterval",   HCF_RES_INT,  { (void *)UART_REG_INTVL} }
};
#define ns1655x0Num NELEMENTS(ns1655x0Resources)

#ifdef INCLUDE_QUAD_UART
const struct hcfResource ns1655x1Resources[] = {
    { "regBase",       HCF_RES_INT,  { (void *)COM1_BASE_ADRS } },
    { "irq",           HCF_RES_INT,  { (void *)COM1_INT_VEC } },
    { "irqLevel",      HCF_RES_INT,  { (void *)COM1_INT_LVL } },
    { "clkFreq",       HCF_RES_INT,  { (void *)QUART_REF_FREQ } },
    { "regInterval",   HCF_RES_INT,  { (void *)UART_REG_INTVL} }
};

#define ns1655x1Num NELEMENTS(ns1655x1Resources)

const struct hcfResource ns1655x2Resources[] = {
    { "regBase",       HCF_RES_INT,  { (void *)COM2_BASE_ADRS } },
    { "irq",           HCF_RES_INT,  { (void *)COM2_INT_VEC } },
    { "irqLevel",      HCF_RES_INT,  { (void *)COM2_INT_LVL } },
    { "clkFreq",       HCF_RES_INT,  { (void *)QUART_REF_FREQ } },
    { "regInterval",   HCF_RES_INT,  { (void *)UART_REG_INTVL} }
};
#define ns1655x2Num NELEMENTS(ns1655x2Resources)

const struct hcfResource ns1655x3Resources[] = {
    { "regBase",       HCF_RES_INT,  { (void *)COM3_BASE_ADRS } },
    { "irq",           HCF_RES_INT,  { (void *)COM3_INT_VEC } },
    { "irqLevel",      HCF_RES_INT,  { (void *)COM3_INT_LVL } },
    { "clkFreq",       HCF_RES_INT,  { (void *)QUART_REF_FREQ } },
    { "regInterval",   HCF_RES_INT,  { (void *)UART_REG_INTVL} }
};

#define ns1655x3Num NELEMENTS(ns1655x3Resources)

const struct hcfResource ns1655x4Resources[] = {
    { "regBase",       HCF_RES_INT,  { (void *)COM4_BASE_ADRS } },
    { "irq",           HCF_RES_INT,  { (void *)COM4_INT_VEC } },
    { "irqLevel",      HCF_RES_INT,  { (void *)COM4_INT_LVL } },
    { "clkFreq",       HCF_RES_INT,  { (void *)QUART_REF_FREQ } },
    { "regInterval",   HCF_RES_INT,  { (void *)UART_REG_INTVL} }
};
#define ns1655x4Num NELEMENTS(ns1655x4Resources)
#endif /* INCLUDE_QUAD_UART */

#ifdef DRV_TIMER_M85XX
struct hcfResource m85xxTimerResources[] = {
    { "regBase",       HCF_RES_INT,  { (void *)0} },
    { "decMinClkRate", HCF_RES_INT,  { (void *)SYS_CLK_RATE_MIN } },
    { "decMaxClkRate", HCF_RES_INT,  { (void *)SYS_CLK_RATE_MAX } },
    { "fitMinClkRate", HCF_RES_INT,  { (void *)AUX_CLK_RATE_MIN } },
    { "fitMaxClkRate", HCF_RES_INT,  { (void *)AUX_CLK_RATE_MAX } },
    { "sysClockFreq",  HCF_RES_ADDR, { (void *)&sysCpuBusSpd } }
};
#define m85xxTimerNum NELEMENTS(m85xxTimerResources)
#endif /* DRV_TIMER_M85XX */

const struct hcfResource pci0Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)0 } },
};
#define pci0Num NELEMENTS(pci0Resources)

const struct hcfDevice hcfDeviceList[] = {
    { "ns16550",              0, VXB_BUSID_PLB, 0, ns1655x0Num,   ns1655x0Resources },
        
#ifdef INCLUDE_QUAD_UART
    { "ns16550",              1, VXB_BUSID_PLB, 0, ns1655x1Num,   ns1655x1Resources },
    { "ns16550",              2, VXB_BUSID_PLB, 0, ns1655x2Num,   ns1655x2Resources },
    { "ns16550",              3, VXB_BUSID_PLB, 0, ns1655x3Num,   ns1655x3Resources },
    { "ns16550",              4, VXB_BUSID_PLB, 0, ns1655x4Num,   ns1655x4Resources },
#endif /* INCLUDE_QUAD_UART */

#ifdef DRV_TIMER_M85XX
    { "m85xxTimerDev",        0, VXB_BUSID_PLB, 0, m85xxTimerNum, m85xxTimerResources },
#endif /* DRV_TIMER_M85XX */

#ifdef INCLUDE_PCI
    { "vxbVirtualPciAdaptor", 0, VXB_BUSID_PLB, 0, pci0Num,       pci0Resources },
#endif /* INCLUDE_PCI */
};

const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] = {
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, { (void *)0 } }
};

