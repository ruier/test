/* hwconf.c - Hardware configuration support module */

/*
 * Copyright (c) 2011, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,30nov12,d_l  fix wrong MDIO offset. (WIND00391142)
01c,05mar12,y_y  add vxBus I2C support.
01b,12jan12,agf  remove exIrqWkrd from epic resource, deprecated in driver
                 (WIND00176018)
01a,10oct11,fao  derived from fsl_p1020_rdb/hwconf.c
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
#include "config.h"

#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
#include <../src/hwif/intCtlr/vxbIntDynaCtlrLib.h>
#endif  /* INCLUDE_INTCTLR_DYNAMIC_LIB */

#ifdef DRV_INTCTLR_EPIC
extern UINT32 sysPicClkFreqGet(UINT32);
#endif  /* DRV_INTCTLR_EPIC */

#ifdef INCLUDE_PCI_BUS
IMPORT STATUS  sysPciEx2AutoConfigInclude(PCI_SYSTEM *,PCI_LOC *,UINT);
IMPORT UCHAR   sysPciEx2AutoConfigIntAssign(PCI_SYSTEM *,PCI_LOC *, UCHAR);
#endif  /* INCLUDE_PCI_BUS */

#ifdef INCLUDE_EHCI
IMPORT void   ehciInit (void);
IMPORT void   ehciPostResetHook (void);
#endif /* INCLUDE_EHCI */

#ifdef DRV_STORAGE_SDHC
#   include <h/storage/vxbSdMmcLib.h>
#   include <h/storage/vxbSdhcStorage.h>
#endif /* DRV_STORAGE_SDHC */

#ifdef DRV_I2CBUS_FSL
#   include <hwif/vxbus/vxbI2cLib.h>
IMPORT UINT32 sysI2cClkFreqGet(void);
#endif /* DRV_I2CBUS_FSL */

#define ETSEC_NAME  "motetsec"

/*
 * On the P1010RDB board, PHYs are physically wired according
 * to the following table:
 *
 *      Data port pins  Management port pins    MII address
 *      --------------  --------------------    -----------
 * PHY0:    TSEC0       TSEC0
 * PHY1:    TSEC1       TSEC0
 * PHY2:    TSEC2       TSEC0
 *
 * The tricky part is that both PHYs have their management pins
 * connected to TSEC0. The VxBus TSEC driver allows us to specify
 * the name of the device that provides PHY management support.
 * This device is "motetsec0" in all cases. We also specify the
 * "phyAddr" resource for each device, which is set according
 * to the MII address in the table above.
 */

#if defined(INCLUDE_ETSEC_VXB_END) || defined(INCLUDE_ETSEC_VXB_END2)

const struct hcfResource etsec0Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)ETSEC0_GROUP0_BASE } },
    { "regBase1",           HCF_RES_INT,    { (void *)ETSEC0_MDIO_BASE } },
    { "phyAddr",            HCF_RES_INT,    { (void *)1 } },
    { "miiIfName",          HCF_RES_STRING, { (void *)"tsecMdio" } },
    { "miiIfUnit",          HCF_RES_INT,    { (void *)0 } },
};
#define etsec0Num NELEMENTS(etsec0Resources)

const struct hcfResource etsec1Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)ETSEC1_GROUP0_BASE } },
    { "regBase1",           HCF_RES_INT,    { (void *)ETSEC1_MDIO_BASE } },
    { "phyAddr",            HCF_RES_INT,    { (void *)0 } },
    { "miiIfName",          HCF_RES_STRING, { (void *)"tsecMdio" } },
    { "miiIfUnit",          HCF_RES_INT,    { (void *)0 } },
};
#define etsec1Num NELEMENTS(etsec1Resources)

const struct hcfResource etsec2Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)ETSEC2_GROUP0_BASE } },
    { "regBase1",           HCF_RES_INT,    { (void *)ETSEC2_MDIO_BASE } },
    { "phyAddr",            HCF_RES_INT,    { (void *)2 } },
    { "miiIfName",          HCF_RES_STRING, { (void *)"tsecMdio" } },
    { "miiIfUnit",          HCF_RES_INT,    { (void *)0 } },
};
#define etsec2Num NELEMENTS(etsec2Resources)

#ifdef INCLUDE_TSEC_MDIO
LOCAL const struct hcfResource mdio0Resources[] =
    {
    { "regBase",   HCF_RES_INT,  {(void *)ETSEC0_MDIO_BASE } },
    };
#   define mdio0Num NELEMENTS(mdio0Resources)
#endif /* INCLUDE_TSEC_MDIO */

#endif /* INCLUDE_ETSEC_VXB_END || INCLUDE_ETSEC_VXB_END2 */

#ifdef DRV_DMA_FSL
const struct hcfResource fslDma1Resources[] = {
    { "regBase",    HCF_RES_INT,            { (void *)DMA1_REGBASE } },
};
#define fslDma1Num NELEMENTS(fslDma1Resources)

const struct hcfResource fslDma2Resources[] = {
    { "regBase",    HCF_RES_INT,            { (void *)DMA2_REGBASE } },
};
#define fslDma2Num NELEMENTS(fslDma2Resources)
#endif  /* DRV_DMA_FSL */

#ifdef INCLUDE_PCI_BUS

const struct hcfResource pci0Resources[] = {
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
    { "includeFuncSet",     HCF_RES_ADDR,   { (void *)sysPciEx2AutoConfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR,   { (void *)sysPciEx2AutoConfigIntAssign } },
#ifdef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "msiEnable",          HCF_RES_INT,    { (void *)TRUE } }
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#   define pci0Num NELEMENTS(pci0Resources)

#endif /* INCLUDE_PCI_BUS */

const struct hcfResource ns1655x0Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)UART0_BASE } },
    { "clkFreq",            HCF_RES_ADDR,   { (void *)sysClkFreqGet } },
    { "regInterval",        HCF_RES_INT,    { (void *)DUART_REG_ADDR_INTERVAL } }
};
#define ns1655x0Num NELEMENTS(ns1655x0Resources)

const struct hcfResource ns1655x1Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *)UART1_BASE } },
    { "clkFreq",            HCF_RES_ADDR,   { (void *)sysClkFreqGet } },
    { "regInterval",        HCF_RES_INT,    { (void *)DUART_REG_ADDR_INTERVAL } }
};
#define ns1655x1Num NELEMENTS(ns1655x1Resources)

INT_BANK_DESC epicIntDesc[] = {
    { 0,   EPIC_EX_IRQ_NUM,   EPIC_EX_REG_BASE,   EPIC_EX_REG_INTERVAL,   EPIC_EX_INTERRUPT },
    { 0,   EPIC_IN_IRQ_NUM,   EPIC_IN_REG_BASE,   EPIC_IN_REG_INTERVAL,   EPIC_IN_INTERRUPT },
    { 0,   EPIC_GT_IRQ_NUM,   EPIC_GT_A_REG_BASE, EPIC_GT_REG_INTERVAL,   EPIC_GT_A_INTERRUPT },
    { 0,   EPIC_GT_IRQ_NUM,   EPIC_GT_B_REG_BASE, EPIC_GT_REG_INTERVAL,   EPIC_GT_B_INTERRUPT },
    { 0,   EPIC_MSG_IRQ_NUM,  EPIC_MSG_REG_BASE,  EPIC_MSG_REG_INTERVAL,  EPIC_MSG_INTERRUPT },
    { 0,   EPIC_SMSG_IRQ_NUM, EPIC_SMSG_REG_BASE, EPIC_SMSG_REG_INTERVAL, EPIC_SMSG_INTERRUPT }
};
struct intrCtlrInputs epicInputs[] = {
    { EPIC_DUART_INT_VEC,       "ns16550",          0,      0 },
    { EPIC_DUART_INT_VEC,       "ns16550",          1,      0 },

#if defined(INCLUDE_ETSEC_VXB_END) || defined(INCLUDE_ETSEC_VXB_END2)
    { EPIC_TSEC1TX_INT_VEC,     "motetsec",         0,      0 },
    { EPIC_TSEC1RX_INT_VEC,     "motetsec",         0,      1 },
    { EPIC_TSEC1ERR_INT_VEC,    "motetsec",         0,      2 },
    { EPIC_TSEC2TX_INT_VEC,     "motetsec",         1,      0 },
    { EPIC_TSEC2RX_INT_VEC,     "motetsec",         1,      1 },
    { EPIC_TSEC2ERR_INT_VEC,    "motetsec",         1,      2 },
    { EPIC_TSEC3TX_INT_VEC,     "motetsec",         2,      0 },
    { EPIC_TSEC3RX_INT_VEC,     "motetsec",         2,      1 },
    { EPIC_TSEC3ERR_INT_VEC,    "motetsec",         2,      2 },
#endif  /* INCLUDE_ETSEC_VXB_END || INCLUDE_ETSEC_VXB_END2 */

#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    { OPENPIC_TIMERA0_INT_VEC,  "openPicTimer",     0,      0 },
    { OPENPIC_TIMERA1_INT_VEC,  "openPicTimer",     0,      1 },
    { OPENPIC_TIMERA2_INT_VEC,  "openPicTimer",     0,      2 },
    { OPENPIC_TIMERA3_INT_VEC,  "openPicTimer",     0,      3 },
    { OPENPIC_TIMERB0_INT_VEC,  "openPicTimer",     1,      0 },
    { OPENPIC_TIMERB1_INT_VEC,  "openPicTimer",     1,      1 },
    { OPENPIC_TIMERB2_INT_VEC,  "openPicTimer",     1,      2 },
    { OPENPIC_TIMERB3_INT_VEC,  "openPicTimer",     1,      3 },
#endif  /* INCLUDE_INTCTLR_DYNAMIC_LIB */

#ifdef INCLUDE_EHCI
    { EPIC_USB_INT_VEC,        "vxbPlbUsbEhci",    0,      0 },
#endif  /* INCLUDE_EHCI */

#ifdef DRV_DMA_FSL
    { EPIC_DMA1_CHAN0_INT_VEC,  "fslDma",           0,      0 },
    { EPIC_DMA1_CHAN1_INT_VEC,  "fslDma",           0,      1 },
    { EPIC_DMA1_CHAN2_INT_VEC,  "fslDma",           0,      2 },
    { EPIC_DMA1_CHAN3_INT_VEC,  "fslDma",           0,      3 },
#endif  /* DRV_DMA_FSL */

#ifdef DRV_STORAGE_SDHC
    { EPIC_SDHC_INT_VEC,        "sdhci",          0,      0 },
#endif  /* DRV_STORAGE_SDHC */

#ifdef DRV_STORAGE_FSLSATA
    { EPIC_SATA1_INT_VEC,       "fslSata",          0,      0 },
    { EPIC_SATA2_INT_VEC,       "fslSata",          1,      0 },
#endif  /* DRV_STORAGE_FSLSATA */

#ifdef DRV_I2CBUS_FSL
    { EPIC_I2C_INT_VEC,         "fslI2c",           0,      0 },
    { EPIC_I2C_INT_VEC,         "fslI2c",           1,      0 },
#endif /* DRV_I2CBUS_FSL */

};

const struct hcfResource epic0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)CCSBAR } },
    { "input",              HCF_RES_ADDR,   { (void *)&epicInputs[0] } },
    { "inputTableSize",     HCF_RES_INT,    { (void *)NELEMENTS(epicInputs) } },
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

#ifdef DRV_TIMER_M85XX

struct hcfResource m85xxTimerResources[] =  {
    {"regBase",             HCF_RES_INT,    { (void *)0 } },
    {"decMinClkRate",       HCF_RES_INT,    { (void *)SYS_CLK_RATE_MIN } },
    {"decMaxClkRate",       HCF_RES_INT,    { (void *)SYS_CLK_RATE_MAX } },
#ifdef INCLUDE_AUX_CLK
    {"fitMinClkRate",       HCF_RES_INT,    { (void *)AUX_CLK_RATE_MIN } },
    {"fitMaxClkRate",       HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX } },
#endif /* INCLUDE_AUX_CLK */
    {"sysClockFreq",        HCF_RES_ADDR,   { (void *)&sysClkFreqGet } },
};
#define m85xxTimerNum   NELEMENTS(m85xxTimerResources)

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
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)EHCI_CAPLENGTH(USB_BASE) } },
    { "ehciInit",           HCF_RES_ADDR,   { (void *)ehciInit } },
    { "dataSwap",           HCF_RES_ADDR,   { (void *)vxbSwap32 } },
};
#define usbEhci0Num NELEMENTS(usbEhci0Resources)

#endif /* INCLUDE_EHCI */

#ifdef DRV_TIMER_OPENPIC
const struct hcfResource openPicTimerDevAresources[] = {
    { VXB_REG_BASE,          HCF_RES_INT,    { (void *)(CCSBAR + 0x000410F0) } },
    { "instanceName",        HCF_RES_STRING, { (void *)"A" } },
    { VXB_CLK_FREQ,          HCF_RES_ADDR,   { (void *)sysPicClkFreqGet } },
    { "clkDivisor",          HCF_RES_INT,    { (void *)8 } },
    { "maxClkRate0",         HCF_RES_INT,    { (void *)SYS_CLK_RATE_MAX } },
#ifdef INCLUDE_AUX_CLK
    { "maxClkRate1",         HCF_RES_INT,    { (void *)AUX_CLK_RATE_MAX } },
#endif /* INCLUDE_AUX_CLK */
#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "vector0",             HCF_RES_INT,    { (void *)OPENPIC_TIMERA0_INT_VEC } },
    { "vector1",             HCF_RES_INT,    { (void *)OPENPIC_TIMERA1_INT_VEC } },
    { "vector2",             HCF_RES_INT,    { (void *)OPENPIC_TIMERA2_INT_VEC } },
    { "vector3",             HCF_RES_INT,    { (void *)OPENPIC_TIMERA3_INT_VEC } },
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#define openPicTimerDevAnum NELEMENTS(openPicTimerDevAresources)

const struct hcfResource openPicTimerDevBresources[] = {
    { VXB_REG_BASE,          HCF_RES_INT,    { (void *)(CCSBAR + 0x000420F0) } },
    { "instanceName",        HCF_RES_STRING, { (void *)"B" } },
    { VXB_CLK_FREQ,          HCF_RES_ADDR,   { (void *)sysPicClkFreqGet } },
    { "clkDivisor",          HCF_RES_INT,    { (void *)0 } },
#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    { "vector0",             HCF_RES_INT,    { (void *)OPENPIC_TIMERB0_INT_VEC } },
    { "vector1",             HCF_RES_INT,    { (void *)OPENPIC_TIMERB1_INT_VEC } },
    { "vector2",             HCF_RES_INT,    { (void *)OPENPIC_TIMERB2_INT_VEC } },
    { "vector3",             HCF_RES_INT,    { (void *)OPENPIC_TIMERB3_INT_VEC } },
#endif  /* INCLUDE_INTCTLR_DYNAMIC_LIB */
};
#define openPicTimerDevBnum NELEMENTS(openPicTimerDevBresources)
#endif   /* DRV_TIMER_OPENPIC */

#ifdef DRV_STORAGE_SDHC

struct hcfResource fslSdhcResources[] =  {
    { "regBase",             HCF_RES_INT,    { (void *)SDHC_BASE } },
    { "clkFreq",             HCF_RES_ADDR,   { (void *)sysClkFreqGet } },
    { "dmaMode",             HCF_RES_INT,    { (void *)0 } },
    { "polling",             HCF_RES_INT,    { (void *)0 } },
    { "flags" ,              HCF_RES_INT,   { (void *)(SDHC_PIO_NEED_DELAY | SDHC_HW_SNOOP |
                                                       SDHC_MISS_CARD_INS_INT_WHEN_INIT |
                                                       SDHC_FIFO_ENDIANESS_REVERSE |
                                                       SDHC_HOST_VER_REVERSE |
                                                       SDHC_HOST_CTRL_FREESCALE) } },
};
#define fslSdhcNum  NELEMENTS(fslSdhcResources)
#endif  /* DRV_STORAGE_SDHC */

#ifdef DRV_STORAGE_FSLSATA
const struct hcfResource fslSata0Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *) (SATA1_BASE) } },
};
#   define fslSata0Num NELEMENTS(fslSata0Resources)

const struct hcfResource fslSata1Resources[] = {
    { "regBase",            HCF_RES_INT,    { (void *) (SATA2_BASE) } },
};
#   define fslSata1Num NELEMENTS(fslSata1Resources)
#endif /* DRV_STORAGE_FSLSATA */

#ifdef DRV_I2CBUS_FSL

struct i2cDevInputs i2c0DevTbl[] = {

/* 
 * Address  Part Number  Size       Bus   Description
 * -------- ------------------------------------------------------------------
 * 0x50       M24256       256K-bit  1    Used with boot sequencer EEPROM.
 * 0x18       PCA9557                1    8-bit I2C register. 
 * 0x68       PT7C4338               1    Real time clock.
 * 0x52       AT24C01        1K-bit  2    Board eeprom.
 */

    { "eeprom_M24256",   0x50,    I2C_WORDADDR | I2C_RD_EXT_ACK},
    { "gpio_pca9557",    0x18,    I2C_BYTE_WR_ONLY | I2C_BYTE_RD_ONLY},
    { "rtc_pt7c4338",    0x68,    0},
};

const struct hcfResource i2c0Resources[] = {
    { "regBase",    HCF_RES_ADDR,   { (void *)(I2C1_BASE) } },
    { "clkFreq",    HCF_RES_ADDR,   { (void *)sysI2cClkFreqGet } },
    { "busSpeed",   HCF_RES_INT,    { (void *)400000 } },
    { "polling",    HCF_RES_INT,    { (void *)0 } },
    { "i2cDev",     HCF_RES_ADDR,   { (void *)&i2c0DevTbl} },
    { "i2cDevNum",  HCF_RES_INT,    { (void *)NELEMENTS(i2c0DevTbl)}},
};
#   define i2c0Num NELEMENTS(i2c0Resources)

struct i2cDevInputs i2c1DevTbl[] = {
    { "eeprom_at24c01",  0x52,    0},
};

const struct hcfResource i2c1Resources[] = {
    { "regBase",    HCF_RES_ADDR,   { (void *)(I2C2_BASE) } },
    { "clkFreq",    HCF_RES_ADDR,   { (void *)sysI2cClkFreqGet } },
    { "busSpeed",   HCF_RES_INT,    { (void *)400000 } },
    { "polling",    HCF_RES_INT,    { (void *)0 } },
    { "i2cDev",     HCF_RES_ADDR,   { (void *)&i2c1DevTbl} },
    { "i2cDevNum",  HCF_RES_INT,    { (void *)NELEMENTS(i2c1DevTbl)}},
};
#   define i2c1Num NELEMENTS(i2c1Resources)
#endif /* DRV_I2CBUS_FSL */

const struct hcfDevice hcfDeviceList[] = {

    /*
     * Initialize ppcIntCtlr before epic.
     * The vector table for external interrupts are over written by epic
     * for an optimized purpose.
     */

    { "ppcIntCtlr",     0,  VXB_BUSID_PLB,  0,  ppcIntCtlr0Num, ppcIntCtlr0Resources },
    { "epic",           0,  VXB_BUSID_PLB,  0,  epic0Num,       epic0Resources },

#ifdef DRV_I2CBUS_FSL
    { "fslI2c",         0,  VXB_BUSID_PLB,  0,  i2c0Num,        i2c0Resources },
    { "fslI2c",         1,  VXB_BUSID_PLB,  0,  i2c1Num,        i2c1Resources },
#endif /* DRV_I2CBUS_FSL */

#if defined(INCLUDE_ETSEC_VXB_END) || defined(INCLUDE_ETSEC_VXB_END2)
    { "motetsec",       0,  VXB_BUSID_PLB,  0,  etsec0Num,      etsec0Resources },
    { "motetsec",       1,  VXB_BUSID_PLB,  0,  etsec1Num,      etsec1Resources },
    { "motetsec",       2,  VXB_BUSID_PLB,  0,  etsec2Num,      etsec2Resources },
#endif /* INCLUDE_ETSEC_VXB_END || INCLUDE_ETSEC_VXB_END2 */

#ifdef INCLUDE_TSEC_MDIO
    { "tsecMdio",       0,  VXB_BUSID_PLB,  0,  mdio0Num,       mdio0Resources },
#endif /* INCLUDE_TSEC_MDIO */

#ifdef DRV_SIO_NS16550
    { "ns16550",        0,  VXB_BUSID_PLB,  0,  ns1655x0Num,    ns1655x0Resources },
    { "ns16550",        1,  VXB_BUSID_PLB,  0,  ns1655x1Num,    ns1655x1Resources },
#endif  /* DRV_SIO_NS16550 */

#ifdef DRV_TIMER_M85XX
    { "m85xxTimerDev",  0,  VXB_BUSID_PLB,  0,  m85xxTimerNum,  m85xxTimerResources },
#endif /* DRV_TIMER_M85XX */

#ifdef DRV_TIMER_OPENPIC
    { "openPicTimer",   0,  VXB_BUSID_PLB,  0,  openPicTimerDevAnum, openPicTimerDevAresources },
    { "openPicTimer",   1,  VXB_BUSID_PLB,  0,  openPicTimerDevBnum, openPicTimerDevBresources },
#endif /* DRV_TIMER_M85XX */

#ifdef INCLUDE_PCI_BUS
    { "QorIQPciEx",     0,  VXB_BUSID_PLB,  0,  pci0Num,    pci0Resources },
#endif  /* INCLUDE_PCI_BUS */

#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci",  0, VXB_BUSID_PLB,   0,  usbEhci0Num,    usbEhci0Resources },
#endif  /* INCLUDE_EHCI */

#ifdef DRV_DMA_FSL
    { "fslDma",         0,  VXB_BUSID_PLB,  0,  fslDma1Num,     fslDma1Resources },
    { "fslDma",         1,  VXB_BUSID_PLB,  0,  fslDma2Num,     fslDma2Resources },
#endif /* DRV_DMA_FSL */

#ifdef DRV_STORAGE_SDHC
    { "sdhci",          0, VXB_BUSID_PLB,   0, fslSdhcNum,      fslSdhcResources },
#endif  /* DRV_STORAGE_SDHC */

#ifdef DRV_STORAGE_FSLSATA
    { "fslSata",        0, VXB_BUSID_PLB,   0, fslSata0Num,     fslSata0Resources },
    { "fslSata",        1, VXB_BUSID_PLB,   0, fslSata1Num,     fslSata1Resources },
#endif /* DRV_STORAGE_FSLSATA */

};

const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] = {
#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci", 0, "hasEmbeddedTT", VXB_PARAM_INT32,   { (void *)TRUE } },
    { "vxbPlbUsbEhci", 0, "postResetHook", VXB_PARAM_FUNCPTR, { (void *)ehciPostResetHook } },
#endif  /* INCLUDE_EHCI */

#ifdef DRV_I2C_EEPROM
    { "eeprom_M24256",  0, "writeDelay",   VXB_PARAM_INT32,   { (void *)4000 } },
    { "eeprom_at24c01", 1, "writeDelay",   VXB_PARAM_INT32,   { (void *)2000 } },
#endif  /* DRV_I2C_EEPROM */

    { NULL,            0,  NULL,           VXB_PARAM_END_OF_LIST, { (void *)0 } }
};
