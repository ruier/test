/* hwconf.c - Hardware configuration support module */

/*
 * Copyright (c) 2007-2008, 2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01e,26mar12,c_t  disable TAH by default. (WIND00272618)
01d,15jan10,liu  define INCLUDE_EMAC_VXB_END to avoid build error. (WIND00190329)
01c,16dec08,b_m  add vxbus timer & uic support.
01b,31jue08,x_s  Updated for new VxBus EMAC driver.
01a,11dec07,x_s  Written based on ocotea bsp.
*/

/* includes */

#include <vxWorks.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/vxbus/vxbPciLib.h>
#include <hwif/util/vxbParamSys.h>
#include <hwif/vxbus/hwConf.h>
#include <../src/hwif/h/end/vxbIbmEmacEnd.h>
#include <../src/hwif/h/mii/vxbIbmZmii.h>
#include <../src/hwif/h/mii/vxbIbmRgmii.h>
#include <../src/hwif/h/dmaCtlr/vxbIbmMalDma.h>
#include <hwif/vxbus/vxbIntrCtlr.h>
#include <hwif/intCtlr/vxbPpc4xxUicIntCtlr.h>

#include "config.h"

IMPORT UINT32 sysClkFreqGet (void);
IMPORT int sysOpbSpeedGet (void);
IMPORT STATUS sysLanIbmEmacEnetAddrGet (VXB_DEVICE_ID, UINT8 *);
IMPORT STATUS sysPciAutoConfigInclude ( PCI_SYSTEM *, PCI_LOC *, UINT);
IMPORT UCHAR sysPciAutoConfigIntAsgn (PCI_SYSTEM *, PCI_LOC *, UCHAR);
 
extern void plbRegister(void);

const VOIDFUNCPTR hcfDriverList[] = {
    plbRegister
};

#ifdef INCLUDE_VXB_IBM_ZMII

/*
 * The SMII PHYs on the ocotea board are strapped as follows:
 *
 *      MAC                     PHY
 *      ---                     ---
 *      emac0                   1
 *      emac1                   2
 *
 * The PHYs are set for serial MII mode (SMII).
 */

LOCAL const struct hcfResource zmii0Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(ZMII0_BASE) } },
    { "emac0Mode",      HCF_RES_INT,    { (void *)ZMII_EMAC_SMII } },
    { "emac1Mode",      HCF_RES_INT,    { (void *)ZMII_EMAC_SMII } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)2} }  
};
#define zmii0Num NELEMENTS(zmii0Resources)
#endif /* INCLUDE_VXB_IBM_ZMII */

#ifdef INCLUDE_VXB_IBM_RGMII

/*
 * The VXB_IBM_RGMII PHYs on the ocotea board are strapped as follows:
 *
 *      MAC                     PHY
 *      ---                     ---
 *      emac2                   16
 *      emac3                   24
 *
 * The PHYs are set for reduced pin mode (VXB_IBM_RGMII).
 */

LOCAL const struct hcfResource rgmii0Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(RGMII0_BASE) } },
    { "port0Mode",      HCF_RES_INT,    { (void *)RGMII_EMAC_RGMII } },
    { "port1Mode",      HCF_RES_INT,    { (void *)RGMII_EMAC_RGMII } },
};
#define rgmii0Num NELEMENTS(rgmii0Resources)
#endif /* INCLUDE_VXB_IBM_RGMII */

#ifdef INCLUDE_EMAC_VXB_END
const struct hcfResource emac0Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)(EMAC2_BASE) } },
    { "emacNum", HCF_RES_INT, { (void *)(2) } },        
    { "intr", HCF_RES_INT, { (void *)INT_VEC_ENET_2 } },
    { "intrLevel", HCF_RES_INT, { (void *)INT_LVL_ENET_2 } },
    { "emacType", HCF_RES_INT, { (void *)(EMAC_TYPE_QUAD) } },
    { "rgmiiPort", HCF_RES_INT, { (void *)0 } },        
    { "phyAddr", HCF_RES_INT, { (void *)PHY2_RGMII_ADDR} },
    { "miiIfName", HCF_RES_STRING, { (void *)"emac" } },
    { "miiIfUnit", HCF_RES_INT, { (void *)0 } }
};

#define emac0Num NELEMENTS(emac0Resources)

const struct hcfResource emac1Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)(EMAC3_BASE) } },
    { "emacNum", HCF_RES_INT, { (void *)(3) } },          
    { "intr", HCF_RES_INT, { (void *)INT_VEC_ENET_3 } },
    { "intrLevel", HCF_RES_INT, { (void *)INT_LVL_ENET_3 } },
    { "emacType", HCF_RES_INT, { (void *)(EMAC_TYPE_QUAD) } },
    { "rgmiiPort", HCF_RES_INT, { (void *)1 } },        
    { "phyAddr", HCF_RES_INT, { (void *)PHY3_RGMII_ADDR } },
    { "miiIfName", HCF_RES_STRING, { (void *)"emac" } },
    { "miiIfUnit", HCF_RES_INT, { (void *)0 } }
};

#define emac1Num NELEMENTS(emac1Resources)
#endif

#ifdef INCLUDE_VXB_IBM_MAL
const struct hcfResource mal0Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)(MAL0_DCR_BASE) } },
    { "intr0", HCF_RES_INT, { (void *)(INT_VEC_MAL_TXEOB) } },
    { "intr0Level", HCF_RES_INT, { (void *)(INT_LVL_MAL_TXEOB) } },
    { "intr1", HCF_RES_INT, { (void *)(INT_VEC_MAL_RXEOB) } },
    { "intr1Level", HCF_RES_INT, { (void *)(INT_LVL_MAL_RXEOB) } },
    { "intr2", HCF_RES_INT, { (void *)(INT_VEC_MAL_SERR) } },
    { "intr2Level", HCF_RES_INT, { (void *)(INT_LVL_MAL_SERR) } },
    { "intr3", HCF_RES_INT, { (void *)(INT_VEC_MAL_TXDE) } },
    { "intr3Level", HCF_RES_INT, { (void *)(INT_LVL_MAL_TXDE) } },
    { "intr4", HCF_RES_INT, { (void *)(INT_VEC_MAL_RXDE) } },
    { "intr4Level", HCF_RES_INT, { (void *)(INT_LVL_MAL_RXDE) } },
    { "malType", HCF_RES_INT, { (void *)(MAL_TYPE_QUAD) } }
};

#define mal0Num NELEMENTS(mal0Resources)
#endif /* INCLUDE_VXB_IBM_MAL */

#ifdef INCLUDE_VXB_IBM_TAH
const struct hcfResource tah0Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)TAH0_BASE } }
};
#define tah0Num NELEMENTS(tah0Resources)

const struct hcfResource tah1Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)TAH1_BASE } }
};
#define tah1Num NELEMENTS(tah1Resources)
#endif

struct hcfResource vxbPpc440gxPci0Resources[] = {
    { "marker",            HCF_RES_INT, {(void *)0xa5a5a5a5} },
    { "regBase",           HCF_RES_INT, {(void *)PCIX0_CONFG_BASE} },
    { "memIo32Addr",       HCF_RES_ADDR, {(void *)PCI_MSTR_MEMIO_BUS} },
    { "memIo32Size",       HCF_RES_INT, {(void *)PCI_MSTR_MEMIO_SIZE} },
    { "mem32Addr",         HCF_RES_ADDR, {(void *)PCI_MSTR_MEM_BUS} },
    { "mem32Size",         HCF_RES_INT, {(void *)PCI_MSTR_MEM_SIZE} },
    { "io32Addr",          HCF_RES_INT, {(void *)PCI_MSTR_IO_BUS} },
    { "io32Size",          HCF_RES_INT, {(void *)PCI_MSTR_IO_SIZE} },
    { "maxLatAllSet",      HCF_RES_INT, {(void *)PCI_LAT_TIMER} },
    { "autoIntRouteSet",   HCF_RES_INT, { (void *)TRUE } },
    { "plbPciIo1Start",    HCF_RES_INT, {(void *)PLB_PCI_IO_REGION_1_START} },
    { "plbPciIo1Size",     HCF_RES_INT, {(void *)PLB_PCI_IO_REGION_1_SIZE} },
    { "pciIo1Start",       HCF_RES_INT, {(void *)PCI_IO_REGION_1_START} },
    { "plbPciIo2Start",    HCF_RES_INT, {(void *)PLB_PCI_IO_REGION_2_START} },
    { "plbPciIo2Size",     HCF_RES_INT, {(void *)PLB_PCI_IO_REGION_2_SIZE} },
    { "pciIo2Start",       HCF_RES_INT, {(void *)PCI_IO_REGION_2_START} },
    { "pciCfgBase",        HCF_RES_INT, {(void *)PCIX0_CONFG_BASE} },
    { "pciCfgAddr",        HCF_RES_INT, {(void *)PCIX0_CFG_PAIR_ADRS} },
    { "pciCfgData",        HCF_RES_INT, {(void *)PCIX0_CFG_PAIR_DATA} },
    { "pom0PciSizeAttrib", HCF_RES_INT, {(void *)POM0_PCI_SIZE_ATTRIB} },
    { "pom0PciSizeAttrib", HCF_RES_INT, {(void *)POM0_PCI_SIZE_ATTRIB} },
    { "pom0LocalAdrsLow",  HCF_RES_INT, {(void *)POM0_LOCAL_ADRS_LOW} },
    { "pom0LocalAdrsHigh", HCF_RES_INT, {(void *)POM0_LOCAL_ADRS_HIGH} },
    { "pom0PciAdrsLow",    HCF_RES_INT, {(void *)POM0_PCI_ADRS_LOW} },
    { "pom0PciAdrsHigh",   HCF_RES_INT, {(void *)POM0_PCI_ADRS_HIGH} },
    { "pom1PciSizeAttrib", HCF_RES_INT, {(void *)POM1_PCI_SIZE_ATTRIB} },
    { "pom1LocalAdrsLow",  HCF_RES_INT, {(void *)POM1_LOCAL_ADRS_LOW} },
    { "pom1LocalAdrsHigh", HCF_RES_INT, {(void *)POM1_LOCAL_ADRS_HIGH} },
    { "pom1PciAdrsLow",    HCF_RES_INT, {(void *)POM1_PCI_ADRS_LOW} },
    { "pom1PciAdrsHigh",   HCF_RES_INT, {(void *)POM1_PCI_ADRS_HIGH} },
    { "pim0PciSizeAttrib", HCF_RES_INT, {(void *)PIM0_PCI_SIZE_ATTRIB} },
    { "pim0LocalAdrsLow",  HCF_RES_INT, {(void *)PIM0_LOCAL_ADRS_LOW} },
    { "pim0LocalAdrsHigh", HCF_RES_INT, {(void *)PIM0_LOCAL_ADRS_HIGH} },
    { "pim0PciAdrsLow",    HCF_RES_INT, {(void *)PIM0_PCI_ADRS_LOW} },
    { "pim0PciAdrsHigh",   HCF_RES_INT, {(void *)PIM0_PCI_ADRS_HIGH} },
    { "pim1PciSizeAttrib", HCF_RES_INT, {(void *)PIM1_PCI_SIZE_ATTRIB} },
    { "pim1LocalAdrsLow",  HCF_RES_INT, {(void *)PIM1_LOCAL_ADRS_LOW} },
    { "pim1LocalAdrsHigh", HCF_RES_INT, {(void *)PIM1_LOCAL_ADRS_HIGH} },
    { "pim1PciAdrsLow",    HCF_RES_INT, {(void *)PIM1_PCI_ADRS} },
    { "pciConfigMechanism",HCF_RES_INT, {(void *)0} },
    { "autoConfig",        HCF_RES_INT, {(void *)TRUE} },
    { "maxBusSet",         HCF_RES_INT, { (void *)3 } },
    { "includeFuncSet",    HCF_RES_ADDR,{(void *)sysPciAutoConfigInclude} },
    { "intAssignFuncSet",  HCF_RES_ADDR,{(void *)sysPciAutoConfigIntAsgn} }
};
#define vxbPpc440gxPci0Num NELEMENTS(vxbPpc440gxPci0Resources)

const struct hcfResource uart0Resources[] = {
    { "regBase",     HCF_RES_INT, {(void *)UART0_BASE } },
    { "irqLevel",    HCF_RES_INT, {(void *)INT_LVL_UART0 } },
    { "irq",         HCF_RES_INT, {(void *)INUM_TO_IVEC(INT_LVL_UART0)} },
    { "clkFreq",     HCF_RES_INT, {(void *)EXT_SER_CLK_FREQ} },
    { "regInterval", HCF_RES_INT, {(void *)UART_REG_ADDR_INTERVAL} }
};
#define uart0Num NELEMENTS(uart0Resources)

const struct hcfResource uart1Resources[] = {
    { "regBase",     HCF_RES_INT, {(void *)UART1_BASE } },
    { "irqLevel",    HCF_RES_INT, {(void *)INT_LVL_UART1 } },
    { "irq"     ,    HCF_RES_INT, {(void *)INUM_TO_IVEC(INT_LVL_UART1)} },
    { "clkFreq",     HCF_RES_INT, {(void *)EXT_SER_CLK_FREQ} },
    { "regInterval", HCF_RES_INT, {(void *)UART_REG_ADDR_INTERVAL} }
};
#define uart1Num NELEMENTS(uart1Resources)

struct hcfResource ppc440TimerResources[] =  {
    { "regBase",       HCF_RES_INT,  { (void *)0 } },
    { "decMinClkRate", HCF_RES_INT,  { (void *)SYS_CLK_RATE_MIN } },
    { "decMaxClkRate", HCF_RES_INT,  { (void *)SYS_CLK_RATE_MAX } },
    { "fitMinClkRate", HCF_RES_INT,  { (void *)FIT_CLK_RATE_MIN } },
    { "fitMaxClkRate", HCF_RES_INT,  { (void *)FIT_CLK_RATE_MAX } },
    { "wdtMinClkRate", HCF_RES_INT,  { (void *)WDT_CLK_RATE_MIN } },
    { "wdtMaxClkRate", HCF_RES_INT,  { (void *)WDT_CLK_RATE_MAX } },
    { "clkFreq",       HCF_RES_ADDR, { (void *)&sysClkFreqGet } }
};
#define ppc440TimerNum  NELEMENTS(ppc440TimerResources)

const struct intrCtlrInputs ppcIntCtlrInputs[] = {
    { 0, "uic", 0, 0 },
    { 5, "uic", 0, 1 },
    { 1, "ppc440Timer", 0, 0 },
    { 3, "ppc440Timer", 0, 2 },
    { 4, "ppc440Timer", 0, 1 },
};

const struct hcfResource ppcIntCtlr0Resources[] = {
    { "regBase",        HCF_RES_INT,  { (void *)TRUE } },
    { "input",          HCF_RES_ADDR, { (void *)&ppcIntCtlrInputs[0] } },
    { "inputTableSize", HCF_RES_INT,  { (void *)NELEMENTS(ppcIntCtlrInputs) } },
};
#define ppcIntCtlr0Num  NELEMENTS(ppcIntCtlr0Resources)

const UINT8 uic0IntAttrTable[UIC_INT_MAX_NUM] = {
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH | UIC_INT_LEVEL ), /* interrupt 0 */
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW  | UIC_INT_LEVEL ), /* interrupt 8 */
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW  | UIC_INT_LEVEL ), /* interrupt 16 */
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW  | UIC_INT_LEVEL ), /* interrupt 24 */
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW  | UIC_INT_LEVEL ),
};

struct intrCtlrInputs uic0Inputs[] = {
    { 0, "uic", 1, 1 },
    { 1, "uic", 1, 0 },
    { 2, "uic", 2, 1 },
    { 3, "uic", 2, 0 },
    { 4, "uic", 3, 1 },
    { 5, "uic", 3, 0 },
};

const struct hcfResource uic0Resources[] = {
    { "regBase",        HCF_RES_INT,  { (void *)UICB_DCR_BASE } },
    { "input",          HCF_RES_ADDR, { (void *)&uic0Inputs[0] } },
    { "inputTableSize", HCF_RES_INT,  { (void *)NELEMENTS(uic0Inputs) } },
    { "intAttrTable",   HCF_RES_ADDR, { (void *)&uic0IntAttrTable[0] } },
};
#define uic0Num NELEMENTS(uic0Resources)

const UINT8 uic1IntAttrTable[UIC_INT_MAX_NUM] = {
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ), /* interrupt 0 */
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_EDGE_RISING | UIC_INT_EDGE  ),
    ( UIC_INT_NORMAL | UIC_INT_EDGE_RISING | UIC_INT_EDGE  ), /* interrupt 8 */
    ( UIC_INT_NORMAL | UIC_INT_EDGE_RISING | UIC_INT_EDGE  ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ), /* interrupt 16 */
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW   | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW   | UIC_INT_LEVEL ), /* interrupt 24 */
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW   | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW   | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW   | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW   | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW   | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW   | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW   | UIC_INT_LEVEL ),
};

struct intrCtlrInputs uic1Inputs[] = {
    { INT_VEC_UART0,     "ns16550", 0, 0 },
    { INT_VEC_UART1,     "ns16550", 1, 0 },
    { INT_VEC_MAL_TXEOB, "mal",     0, 0 },
    { INT_VEC_MAL_RXEOB, "mal",     0, 1 },
    { INT_VEC_EXT_IRQ_0, "legacy",  0, INT_VEC_EXT_IRQ_0 },
    { INT_VEC_EXT_IRQ_1, "legacy",  0, INT_VEC_EXT_IRQ_1 },
    { INT_VEC_EXT_IRQ_2, "legacy",  0, INT_VEC_EXT_IRQ_2 },
    { INT_VEC_EXT_IRQ_3, "legacy",  0, INT_VEC_EXT_IRQ_3 },
};

const struct hcfResource uic1Resources[] = {
    { "regBase",        HCF_RES_INT,  { (void *)UIC0_DCR_BASE } },
    { "input",          HCF_RES_ADDR, { (void *)&uic1Inputs[0] } },
    { "inputTableSize", HCF_RES_INT,  { (void *)NELEMENTS(uic1Inputs) } },
    { "intAttrTable",   HCF_RES_ADDR, { (void *)&uic1IntAttrTable[0] } },
};
#define uic1Num NELEMENTS(uic1Resources)

const UINT8 uic2IntAttrTable[UIC_INT_MAX_NUM] = {
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ), /* interrupt 0 */
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_EDGE_RISING | UIC_INT_EDGE  ), /* interrupt 8 */
    ( UIC_INT_NORMAL | UIC_INT_EDGE_RISING | UIC_INT_EDGE  ),
    ( UIC_INT_NORMAL | UIC_INT_EDGE_RISING | UIC_INT_EDGE  ),
    ( UIC_INT_NORMAL | UIC_INT_EDGE_RISING | UIC_INT_EDGE  ),
    ( UIC_INT_NORMAL | UIC_INT_EDGE_RISING | UIC_INT_EDGE  ),
    ( UIC_INT_NORMAL | UIC_INT_EDGE_RISING | UIC_INT_EDGE  ),
    ( UIC_INT_NORMAL | UIC_INT_EDGE_RISING | UIC_INT_EDGE  ),
    ( UIC_INT_NORMAL | UIC_INT_EDGE_RISING | UIC_INT_EDGE  ),
    ( UIC_INT_NORMAL | UIC_INT_EDGE_RISING | UIC_INT_EDGE  ), /* interrupt 16 */
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW   | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW   | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW   | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW   | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW   | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW   | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ), /* interrupt 24 */
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
};

struct intrCtlrInputs uic2Inputs[] = {
    { INT_VEC_MAL_SERR, "mal", 0, 2 },
    { INT_VEC_MAL_TXDE, "mal", 0, 3 },
    { INT_VEC_MAL_RXDE, "mal", 0, 4 },
};

const struct hcfResource uic2Resources[] = {
    { "regBase",        HCF_RES_INT,  { (void *)UIC1_DCR_BASE } },
    { "input",          HCF_RES_ADDR, { (void *)&uic2Inputs[0] } },
    { "inputTableSize", HCF_RES_INT,  { (void *)NELEMENTS(uic2Inputs) } },
    { "intAttrTable",   HCF_RES_ADDR, { (void *)&uic2IntAttrTable[0] } },
};
#define uic2Num NELEMENTS(uic2Resources)

const UINT8 uic3IntAttrTable[UIC_INT_MAX_NUM] = {
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ), /* interrupt 0 */
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ), /* interrupt 8 */
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_EDGE_RISING | UIC_INT_EDGE  ),
    ( UIC_INT_NORMAL | UIC_INT_EDGE_RISING | UIC_INT_EDGE  ),
    ( UIC_INT_NORMAL | UIC_INT_EDGE_RISING | UIC_INT_EDGE  ),
    ( UIC_INT_NORMAL | UIC_INT_EDGE_RISING | UIC_INT_EDGE  ),
    ( UIC_INT_NORMAL | UIC_INT_EDGE_RISING | UIC_INT_EDGE  ), /* interrupt 16 */
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW   | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW   | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW   | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW   | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_LOW   | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_EDGE_RISING | UIC_INT_EDGE  ),
    ( UIC_INT_NORMAL | UIC_INT_EDGE_RISING | UIC_INT_EDGE  ),
    ( UIC_INT_NORMAL | UIC_INT_EDGE_RISING | UIC_INT_EDGE  ), /* interrupt 24 */
    ( UIC_INT_NORMAL | UIC_INT_EDGE_RISING | UIC_INT_EDGE  ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
    ( UIC_INT_NORMAL | UIC_INT_LEVEL_HIGH  | UIC_INT_LEVEL ),
};

struct intrCtlrInputs uic3Inputs[] = {
    { INT_VEC_ENET_2, "emac", 0, 0 },
    { INT_VEC_ENET_3, "emac", 1, 0 },
};

const struct hcfResource uic3Resources[] = {
    { "regBase",        HCF_RES_INT,  { (void *)UIC2_DCR_BASE } },
    { "input",          HCF_RES_ADDR, { (void *)&uic3Inputs[0] } },
    { "inputTableSize", HCF_RES_INT,  { (void *)NELEMENTS(uic3Inputs) } },
    { "intAttrTable",   HCF_RES_ADDR, { (void *)&uic3IntAttrTable[0] } },
};
#define uic3Num NELEMENTS(uic3Resources)

const struct hcfDevice hcfDeviceList[] = {
    { "ns16550", 0, VXB_BUSID_PLB, 0, uart0Num, uart0Resources },
    { "ns16550", 1, VXB_BUSID_PLB, 0, uart1Num, uart1Resources },

#ifdef INCLUDE_VXB_IBM_ZMII
    { "zmii", 0, VXB_BUSID_PLB, 0, zmii0Num, zmii0Resources  },
#endif

#ifdef INCLUDE_VXB_IBM_MAL
    { "mal", 0, VXB_BUSID_PLB, 0, mal0Num, mal0Resources  },
#endif

#ifdef INCLUDE_VXB_IBM_TAH
    { "tah", 0, VXB_BUSID_PLB, 0, tah0Num, tah0Resources  },
    { "tah", 1, VXB_BUSID_PLB, 0, tah1Num, tah1Resources  },
#endif

#if (defined(EMAC2_ENABLE) && defined(INCLUDE_EMAC_VXB_END))
    { "emac", 0, VXB_BUSID_PLB, 0, emac0Num, emac0Resources },
#endif

#if (defined(EMAC3_ENABLE) && defined(INCLUDE_EMAC_VXB_END))
    { "emac", 1, VXB_BUSID_PLB, 0, emac1Num, emac1Resources },
#endif

#ifdef INCLUDE_VXB_IBM_RGMII
    { "rgmii", 0, VXB_BUSID_PLB, 0, rgmii0Num, rgmii0Resources  },
#endif

    { "vxbPpc440gxPci", 0, VXB_BUSID_NEXUS, 0, vxbPpc440gxPci0Num, vxbPpc440gxPci0Resources },

    { "ppc440Timer", 0, VXB_BUSID_PLB, 0, ppc440TimerNum, ppc440TimerResources },
    { "ppcIntCtlr", 0, VXB_BUSID_PLB, 0, ppcIntCtlr0Num, ppcIntCtlr0Resources },
    { "uic", 0, VXB_BUSID_PLB, 0, uic0Num, uic0Resources },
    { "uic", 1, VXB_BUSID_PLB, 0, uic1Num, uic1Resources },
    { "uic", 2, VXB_BUSID_PLB, 0, uic2Num, uic2Resources },
    { "uic", 3, VXB_BUSID_PLB, 0, uic3Num, uic3Resources },
};

const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] =
    {
    { "emac", 0, "cksmDisable", VXB_PARAM_INT32, {(void *)1} },
    { "emac", 1, "cksmDisable", VXB_PARAM_INT32, {(void *)1} },
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, {(void *)0} }
    };

