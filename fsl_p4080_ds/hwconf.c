/* hwconf.c - Hardware configuration support module */

/*
 * Copyright (c) 2008-2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02g,11jan13,ylu  Update for Rev 3 silicon.
02f,12jan12,agf  remove exIrqWkrd from epic resource, deprecated in driver
                 (WIND00176018)
02e,09sep11,b_m  Add missing PCIe configuration parameters (WIND00264966)
02d,06jan11,agf  CQ214297: fix DUART2 interrupt routing with non-AMP kernels
02c,14jun10,wap  Add TGEC MDIO driver
02b,19may10,wap  Add support for alternate FMAN setup on newer boards
02a,23feb10,wap  Bman/Qman portal initialization is now automatic
01z,23feb10,wap  Update to newer rev 1 silicon Fman microcode patch
01y,27jan10,pmr  Change _WRS_VX_AMP to INCLUDE_AMP
01x,26jan10,wap  Update documentation regarding PCIe bridges
01w,11jan10,wap  Clean up secondary SGMII riser configuration
01v,29oct09,wap  Add 10 gigabit ethernet (TGEC) support
01u,14oct09,wap  Add pointer to Fman microcode patch, set up
                 dummy MDIO support for Simics
01t,08oct09,rab  Add USB, DMA, SPI, and I2c support 
01s,04oct09,wap  Make sure MSI is enabled for all 3 PCIe bridges
01r,28sep09,wap  Add PCI support
01q,21sep09,wap  Add GPIO driver, document P4080DS MDIO setup
01p,29aug09,wap  Add AMP setup for DTSEC devices
01o,11aug09,wap  Add FMAN resources
01n,08jul09,pmr  virtual mapping
01m,04jun09,pmr  support package 2067; extra uarts
01l,28jan09,pmr  fix MIPC PLB1
01k,22jan09,wap  Add buffer manager support
01j,20jan09,pmr  openPicTimer vectors (WIND00152368)
01i,22dec08,wap  Remove old LAW driver, add new LAW driver and QMAN components
01h,04dec08,pmr  etsec coreNum
01g,04sep08,pmr  AMP modifications
01g,23jan09,slk  fix EPIC_GT_INTERRUPT build error
01f,03sep08,b_m  remove booke timer vectors.
01e,29aug08,kab  Change _WRS_VX_SMP to _WRS_CONFIG_SMP
01d,14jul08,dtr  Remove dynamicInterrupts table.
01c,30may08,dtr  Set numCpus for epic. Modify interrupt vectors for linux.
01b,30may08,pmr  mods for EPIC and AMP
01a,24apr08,pmr  adapted from ads8572 version 01k
*/

#include <vxWorks.h>
#include <vsbConfig.h>
#include <vxBusLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/util/vxbParamSys.h>
#include <../src/hwif/h/vxbus/vxbRapidIO.h>
#include "config.h"
#include "sysDuart.h"
#include "drv/pci/pciAutoConfigLib.h"
#include <hwif/vxbus/vxbIntrCtlr.h>
#include <../src/hwif/h/end/vxbDtsecEnd.h>

/* FIXME: move vector definitions elsewhere so this file is not needed here */
#ifdef DRV_INTCTLR_EPIC
#include <hwif/intCtlr/vxbEpicIntCtlr.h>
#include <../src/hwif/intCtlr/vxbIntDynaCtlrLib.h>

extern UINT32 sysPicClkFreqGet(UINT32);
#endif

#ifdef DRV_STORAGE_SDHC
#   include <h/storage/vxbSdMmcLib.h>
#   include <h/storage/vxbSdhcStorage.h>

IMPORT UINT32 sysSdhcClkFreqGet(void);
#endif /* DRV_STORAGE_SDHC */
 
#define XTSEC_NAME "mottsec"
#if defined (INCLUDE_ETSEC_VXB_END) || defined (INCLUDE_ETSEC_VXB_END2)
#  undef  XTSEC_NAME
#  define XTSEC_NAME "motetsec"
#endif /* defined(INCLUDE_ETSEC_VXB_END) || defined(INCLUDE_ETSEC_VXB_END2) */

#ifdef DRV_PCIBUS_QORIQ_PCIEX
IMPORT STATUS sysPci1AutoconfigInclude ();
IMPORT UCHAR sysPci1AutoconfigIntrAssign (PCI_SYSTEM *, PCI_LOC *, UCHAR);
IMPORT UCHAR sysPci2AutoconfigIntrAssign (PCI_SYSTEM *, PCI_LOC *, UCHAR);
IMPORT UCHAR sysPci3AutoconfigIntrAssign (PCI_SYSTEM *, PCI_LOC *, UCHAR);
#endif

IMPORT int sysEprGet(void);

#ifdef INCLUDE_EHCI
IMPORT void  ehci0Init (void);
IMPORT void  ehci0PostResetHook (void);
#endif

#undef  EPIC_VEC_IN_IRQ0
#define EPIC_VEC_IN_IRQ0	16

#define EPIC_DMA1_CHAN0_INT_VEC	(12 + EPIC_VEC_IN_IRQ0)
#define EPIC_DMA1_CHAN1_INT_VEC	(13 + EPIC_VEC_IN_IRQ0)
#define EPIC_DMA1_CHAN2_INT_VEC	(14 + EPIC_VEC_IN_IRQ0)
#define EPIC_DMA1_CHAN3_INT_VEC	(15 + EPIC_VEC_IN_IRQ0)

#define EPIC_DMA2_CHAN0_INT_VEC	(16 + EPIC_VEC_IN_IRQ0)
#define EPIC_DMA2_CHAN1_INT_VEC	(17 + EPIC_VEC_IN_IRQ0)
#define EPIC_DMA2_CHAN2_INT_VEC	(18 + EPIC_VEC_IN_IRQ0)
#define EPIC_DMA2_CHAN3_INT_VEC	(19 + EPIC_VEC_IN_IRQ0)

#undef EPIC_DUART_INT_VEC
#define EPIC_DUART_INT_VEC 	(20 + EPIC_VEC_IN_IRQ0)
#undef EPIC_DUART2_INT_VEC
#define EPIC_DUART2_INT_VEC     (21 + EPIC_VEC_IN_IRQ0)

#define EPIC_USB_INT_VEC        (28 + EPIC_VEC_IN_IRQ0)

#ifdef DRV_STORAGE_SDHC
#define EPIC_ESDHC_INT_VEC      (32 + EPIC_VEC_IN_IRQ0)
#define SDHC_BASE               (CCSBAR + 0x114000)
#endif /* DRV_STORAGE_SDHC */

/*
 * Extra Duarts are available in Simics.  This allows for each CPU in
 * an AMP system to have its own dedicated console port.  Note that because
 * these are duarts, there are two serial devices available for each interrupt
 * line.  But since two CPUs cannot share a device on the same interrupt, this
 * requires that we create six additional duarts.  To keep things consistent,
 * these are set up like the two stock duarts.  Thus, twelve additional 
 * serial ports are created, potentially allowing for two ports to be 
 * dedicated to each CPU.
 */
#ifdef USE_EXTRA_DUARTS
#define EPIC_DUART3_INT_VEC 	(48 + EPIC_VEC_IN_IRQ0)
#define EPIC_DUART4_INT_VEC 	(49 + EPIC_VEC_IN_IRQ0)
#define EPIC_DUART5_INT_VEC 	(50 + EPIC_VEC_IN_IRQ0)
#define EPIC_DUART6_INT_VEC 	(51 + EPIC_VEC_IN_IRQ0)
#define EPIC_DUART7_INT_VEC 	(52 + EPIC_VEC_IN_IRQ0)
#define EPIC_DUART8_INT_VEC 	(53 + EPIC_VEC_IN_IRQ0)
#define COM5_ADR		(CCSBAR + 0x11e500)
#define COM6_ADR		(CCSBAR + 0x11e600)
#define COM7_ADR		(CCSBAR + 0x11f500)
#define COM8_ADR		(CCSBAR + 0x11f600)
#define COM9_ADR		(CCSBAR + 0x120500)
#define COM10_ADR		(CCSBAR + 0x120600)
#define COM11_ADR		(CCSBAR + 0x121500)
#define COM12_ADR		(CCSBAR + 0x121600)
#define COM13_ADR		(CCSBAR + 0x122500)
#define COM14_ADR		(CCSBAR + 0x122600)
#define COM15_ADR		(CCSBAR + 0x123500)
#define COM16_ADR		(CCSBAR + 0x123600)
#endif /* USE_EXTRA_DUARTS */

#undef EPIC_TSEC1TX_INT_VEC
#undef EPIC_TSEC1RX_INT_VEC
#undef EPIC_TSEC1ERR_INT_VEC
#ifdef USE_PACKAGE_2068
#define EPIC_TSEC1TX_INT_VEC 	87
#define EPIC_TSEC1RX_INT_VEC 	88
#define EPIC_TSEC1ERR_INT_VEC	89
#else /* USE_PACKAGE_2068 */
#define EPIC_TSEC1TX_INT_VEC 	(127 + EPIC_VEC_IN_IRQ0)
#define EPIC_TSEC1RX_INT_VEC 	(126 + EPIC_VEC_IN_IRQ0)
#define EPIC_TSEC1ERR_INT_VEC	(125 + EPIC_VEC_IN_IRQ0)
#endif /* USE_PACKAGE_2068 */

#undef EPIC_TSEC2TX_INT_VEC
#undef EPIC_TSEC2RX_INT_VEC
#undef EPIC_TSEC2ERR_INT_VEC
#ifdef USE_PACKAGE_2068
#define EPIC_TSEC2TX_INT_VEC 	90
#define EPIC_TSEC2RX_INT_VEC 	91
#define EPIC_TSEC2ERR_INT_VEC	92
#else /* USE_PACKAGE_2068 */
#define EPIC_TSEC2TX_INT_VEC 	(124 + EPIC_VEC_IN_IRQ0)
#define EPIC_TSEC2RX_INT_VEC 	(123 + EPIC_VEC_IN_IRQ0)
#define EPIC_TSEC2ERR_INT_VEC	(122 + EPIC_VEC_IN_IRQ0)
#endif /* USE_PACKAGE_2068 */

#undef EPIC_TSEC3TX_INT_VEC
#undef EPIC_TSEC3RX_INT_VEC
#undef EPIC_TSEC3ERR_INT_VEC
#ifdef USE_PACKAGE_2068
#define EPIC_TSEC3TX_INT_VEC 	93
#define EPIC_TSEC3RX_INT_VEC 	94
#define EPIC_TSEC3ERR_INT_VEC	95
#else /* USE_PACKAGE_2068 */
#define EPIC_TSEC3TX_INT_VEC 	(121 + EPIC_VEC_IN_IRQ0)
#define EPIC_TSEC3RX_INT_VEC 	(120 + EPIC_VEC_IN_IRQ0)
#define EPIC_TSEC3ERR_INT_VEC	(119 + EPIC_VEC_IN_IRQ0)
#endif /* USE_PACKAGE_2068 */

#undef EPIC_TSEC4TX_INT_VEC
#undef EPIC_TSEC4RX_INT_VEC
#undef EPIC_TSEC4ERR_INT_VEC
#ifdef USE_PACKAGE_2068
#define EPIC_TSEC4TX_INT_VEC 	116
#define EPIC_TSEC4RX_INT_VEC 	117
#define EPIC_TSEC4ERR_INT_VEC	118
#else /* USE_PACKAGE_2068 */
#define EPIC_TSEC4TX_INT_VEC 	(118 + EPIC_VEC_IN_IRQ0)
#define EPIC_TSEC4RX_INT_VEC 	(117 + EPIC_VEC_IN_IRQ0)
#define EPIC_TSEC4ERR_INT_VEC	(116 + EPIC_VEC_IN_IRQ0)
#endif /* USE_PACKAGE_2068 */

/*
 * Note: a few of these interrupt vectors overlap with
 * the ones allocated to the eTSEC devices specified above.
 * The eTSECs are not part of the real P4080 device, however:
 * they have been added to the simulation temporarily for debugging
 * purposes and should eventually be removed. For the time being,
 * only the first few software portals are being used anyway,
 * so the overlap is harmless for now.
 */

#define EPIC_QMAN_PORTAL0_INT_VEC	104
#define EPIC_QMAN_PORTAL1_INT_VEC	106
#define EPIC_QMAN_PORTAL2_INT_VEC	108
#define EPIC_QMAN_PORTAL3_INT_VEC	110
#define EPIC_QMAN_PORTAL4_INT_VEC	112
#define EPIC_QMAN_PORTAL5_INT_VEC	114
#define EPIC_QMAN_PORTAL6_INT_VEC	116
#define EPIC_QMAN_PORTAL7_INT_VEC	118
#define EPIC_QMAN_PORTAL8_INT_VEC	120
#define EPIC_QMAN_PORTAL9_INT_VEC	122

#define EPIC_BMAN_PORTAL0_INT_VEC       105
#define EPIC_BMAN_PORTAL1_INT_VEC       107
#define EPIC_BMAN_PORTAL2_INT_VEC       109
#define EPIC_BMAN_PORTAL3_INT_VEC       111
#define EPIC_BMAN_PORTAL4_INT_VEC       113
#define EPIC_BMAN_PORTAL5_INT_VEC       115
#define EPIC_BMAN_PORTAL6_INT_VEC       117
#define EPIC_BMAN_PORTAL7_INT_VEC       119
#define EPIC_BMAN_PORTAL8_INT_VEC       121
#define EPIC_BMAN_PORTAL9_INT_VEC       123

/* OpenPIC timer vectors */
#define OPENPIC_TIMERA0_INT_VEC		200
#define OPENPIC_TIMERB0_INT_VEC		204

/* GPIO vector */

#define GPIO_INT_VEC			55

/*
 * On the P4080DS board, the default network port configuration
 * is as follows:
 *
 * MAC		PHY		Management port pins	MII address
 * --------	---------------	--------------------	-----------
 * DTSEC0	SGMII riser	DTSEC0			28
 * DTSEC1	SGMII riser	DTSEC0			29
 * DTSEC2	SGMII riser	DTSEC0			30
 * DTSEC3	SGMII riser	DTSEC0			31
 * DTSEC4	RGMII on-board	DTSEC0			0
 * DTSEC5	None		DTSEC0			None
 * DTSEC6	None		DTSEC0			None
 * DTSEC7	None		DTSEC0			None
 *
 * TGEC0	None		TGEC0			None
 * TGEC1	None		TGEC0			None
 * 
 * The board comes with one on-board port (DTSEC4) connected via
 * a Vitesse RGMII PHY, and four additional ports (DTSEC0, 1, 2,
 * and 3) provided via an SGMII riser card in SerDes slot 5. This
 * means that only 5 DTSEC ports are available in the 'out of box'
 * configuration. It is possible to add an additional SGMII riser
 * card, in which case the on-board PHY will be disabled, or a
 * XAUI riser card to provide access to one of the 10GbE ports.
 * (The XAUI card must be purchased separately.)
 *
 * One complication is that the MDIO port accessed via DTSEC0 is
 * multiplexed among the various possible target devices, and the
 * desired target must be selected using GPIO pins. For example,
 * GPIO pins 0 and 1 must be cleared in order to address the
 * management registers on the RGMII PHY, and must be set in order
 * to access the registers on the SGMII riser.
 *
 * The supported GPIO selections are shown below:
 *
 * 		GPIO[0:1]		Device
 *		---------		----------------
 * 		0 0			On-board Vitesse RGMII
 *		0 1			Slot 3 SGMII
 *		1 0			Slot 4 SGMII
 *		1 1			Slot 5 SGMII (default)
 *
 * 		GPIO[2:3]		Device
 *		---------		----------------
 *		0 0			None
 *		0 1			None
 *		1 0			Slot 4 XAUI
 *		1 1			Slot 5 XAUI
 *
 * The dtsecMdio driver is used to provide PHY management support
 * for the DTSEC ports.
 *
 * Note that if a second SGMII riser card is used to provide access
 * to all four DTSECs on the second frame manager, the MII addresses
 * of the PHYs on both cards will be the same. That is, both will use
 * addresses 28, 29, 30 and 31. The dtsecMdio driver must be set up
 * correctly to select the right GPIO pin configuration in order for
 * the right riser to be addressed by the right DTSEC device.
 */

const struct hcfResource gpio0Resources[] = {
    { "regBase", 	HCF_RES_INT,	{ (void *)(CCSBAR + 0x130000) } },
};
#define gpio0Num NELEMENTS(gpio0Resources)

/*
 * Simics doesn't properly simulate MDIO for the Fman network
 * ports. To work around this, we use the dummy MDIO driver
 * to make the network ports believe they always have a link
 * present.
 */

#ifdef INCLUDE_DUMMY_MDIO
#define DTSEC_MDIO	"dummyMdio"
#else
#define DTSEC_MDIO	"dtsecMdio"
#endif

const struct hcfResource dtsec0Resources[] = {
#ifdef FMAN_SWAP
    { "regBase", 	HCF_RES_INT,	{ (void *)(CCSBAR + 0x5E0000) } },
#else
    { "regBase", 	HCF_RES_INT,	{ (void *)(CCSBAR + 0x4E0000) } },
#endif
    { "phyAddr",	HCF_RES_INT,	{ (void *)0x1C } },
    { "miiIfName",      HCF_RES_STRING, { (void *)DTSEC_MDIO } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
#ifdef FMAN_SWAP
    { "fmanNum",	HCF_RES_INT,    { (void *)1 } },
#endif
#ifndef INCLUDE_DUMMY_MDIO
    { "dtsecMedia",     HCF_RES_INT,    { (void *)DTSEC_MEDIA_SGMII } },
#endif
};
#define dtsec0Num NELEMENTS(dtsec0Resources)

const struct hcfResource dtsec1Resources[] = {
#ifdef FMAN_SWAP
    { "regBase", 	HCF_RES_INT,	{ (void *)(CCSBAR + 0x5E2000) } },
#else
    { "regBase", 	HCF_RES_INT,	{ (void *)(CCSBAR + 0x4E2000) } },
#endif
    { "phyAddr",	HCF_RES_INT,	{ (void *)0x1D } },
    { "miiIfName",      HCF_RES_STRING, { (void *)DTSEC_MDIO } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
    { "dtsecMedia",     HCF_RES_INT,    { (void *)DTSEC_MEDIA_SGMII } },
#ifdef FMAN_SWAP
    { "fmanNum",	HCF_RES_INT,    { (void *)1 } },
#endif
#if defined (INCLUDE_AMP_CPU_01)
    { "coreNum",        HCF_RES_INT,    { (void *)1 } },
#endif
};
#define dtsec1Num NELEMENTS(dtsec1Resources)

const struct hcfResource dtsec2Resources[] = {
#ifdef FMAN_SWAP
    { "regBase", 	HCF_RES_INT,	{ (void *)(CCSBAR + 0x5E4000) } },
#else
    { "regBase", 	HCF_RES_INT,	{ (void *)(CCSBAR + 0x4E4000) } },
#endif
    { "phyAddr",	HCF_RES_INT,	{ (void *)0x1E } },
    { "miiIfName",      HCF_RES_STRING, { (void *)DTSEC_MDIO } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
    { "dtsecMedia",     HCF_RES_INT,    { (void *)DTSEC_MEDIA_SGMII } },
#ifdef FMAN_SWAP
    { "fmanNum",	HCF_RES_INT,    { (void *)1 } },
#endif
#if defined (INCLUDE_AMP_CPU_02)
    { "coreNum",        HCF_RES_INT,    { (void *)2 } },
#endif
};
#define dtsec2Num NELEMENTS(dtsec2Resources)

const struct hcfResource dtsec3Resources[] = {
#ifdef FMAN_SWAP
    { "regBase", 	HCF_RES_INT,	{ (void *)(CCSBAR + 0x5E6000) } },
#else
    { "regBase", 	HCF_RES_INT,	{ (void *)(CCSBAR + 0x4E6000) } },
#endif
    { "phyAddr",	HCF_RES_INT,	{ (void *)0x1F } },
    { "miiIfName",      HCF_RES_STRING, { (void *)DTSEC_MDIO } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
    { "dtsecMedia",     HCF_RES_INT,    { (void *)DTSEC_MEDIA_SGMII } },
#ifdef FMAN_SWAP
    { "fmanNum",	HCF_RES_INT,    { (void *)1 } },
#endif
#if defined (INCLUDE_AMP_CPU_03)
    { "coreNum",        HCF_RES_INT,    { (void *)3 } },
#endif
};
#define dtsec3Num NELEMENTS(dtsec3Resources)

const struct hcfResource dtsec4Resources[] = {
#ifdef FMAN_SWAP
    { "regBase", 	HCF_RES_INT,	{ (void *)(CCSBAR + 0x4E2000) } },
#else
    { "regBase", 	HCF_RES_INT,	{ (void *)(CCSBAR + 0x5E0000) } },
#endif
#ifdef SECONDARY_SGMII_RISER
    { "phyAddr",	HCF_RES_INT,	{ (void *)0x1C } },
#else
    { "phyAddr",	HCF_RES_INT,	{ (void *)0 } },
#endif
    { "miiIfName",      HCF_RES_STRING, { (void *)DTSEC_MDIO } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
#ifdef FMAN_SWAP
    { "fmanNum",	HCF_RES_INT,    { (void *)0 } },
    { "dtsecNum",	HCF_RES_INT,    { (void *)1 } },
#endif
#ifdef SECONDARY_SGMII_RISER
    { "dtsecMedia",     HCF_RES_INT,    { (void *)DTSEC_MEDIA_SGMII } },
#else
    { "dtsecMedia",     HCF_RES_INT,    { (void *)DTSEC_MEDIA_RGMII } },
#endif
#if defined (INCLUDE_AMP_CPU_04)
    { "coreNum",        HCF_RES_INT,    { (void *)4 } },
#endif
};
#define dtsec4Num NELEMENTS(dtsec4Resources)

#ifdef SECONDARY_SGMII_RISER
const struct hcfResource dtsec5Resources[] = {
    { "regBase", 	HCF_RES_INT,	{ (void *)(CCSBAR + 0x5E2000) } },
    { "phyAddr",	HCF_RES_INT,	{ (void *)0x1D } },
    { "miiIfName",      HCF_RES_STRING, { (void *)DTSEC_MDIO } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
    { "dtsecMedia",     HCF_RES_INT,    { (void *)DTSEC_MEDIA_SGMII } },
#if defined (INCLUDE_AMP_CPU_05)
    { "coreNum",        HCF_RES_INT,    { (void *)5 } },
#endif
};
#define dtsec5Num NELEMENTS(dtsec5Resources)

const struct hcfResource dtsec6Resources[] = {
    { "regBase", 	HCF_RES_INT,	{ (void *)(CCSBAR + 0x5E4000) } },
    { "phyAddr",	HCF_RES_INT,	{ (void *)0x1E } },
    { "miiIfName",      HCF_RES_STRING, { (void *)DTSEC_MDIO } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
    { "dtsecMedia",     HCF_RES_INT,    { (void *)DTSEC_MEDIA_SGMII } },
#if defined (INCLUDE_AMP_CPU_06)
    { "coreNum",        HCF_RES_INT,    { (void *)6 } },
#endif
};
#define dtsec6Num NELEMENTS(dtsec6Resources)

const struct hcfResource dtsec7Resources[] = {
    { "regBase", 	HCF_RES_INT,	{ (void *)(CCSBAR + 0x5E6000) } },
    { "phyAddr",	HCF_RES_INT,	{ (void *)0x1F } },
    { "miiIfName",      HCF_RES_STRING, { (void *)DTSEC_MDIO } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
    { "dtsecMedia",     HCF_RES_INT,    { (void *)DTSEC_MEDIA_SGMII } },
#if defined (INCLUDE_AMP_CPU_07)
    { "coreNum",        HCF_RES_INT,    { (void *)7 } },
#endif
};
#define dtsec7Num NELEMENTS(dtsec7Resources)
#endif /* SECONDARY_SGMII_RISER */

#define GPIO_RGMII		0
#define GPIO_SGMII_SLOT_3	1
#define GPIO_SGMII_SLOT_4	2
#define GPIO_SGMII_SLOT_5	3

const struct hcfResource dtsecMdio0Resources[] = {
    { "regBase",	HCF_RES_INT,	{ (void *)(CCSBAR + 0x4E1000) } },
#ifdef FMAN_SWAP
    { "fman0Gpio",	HCF_RES_INT,	{ (void *)(GPIO_RGMII) } },
#else
    { "fman0Gpio",	HCF_RES_INT,	{ (void *)(GPIO_SGMII_SLOT_5) } },
#endif
#ifdef SECONDARY_SGMII_RISER
    { "fman1Gpio",	HCF_RES_INT,	{ (void *)(GPIO_SGMII_SLOT_4) } }
#else
#ifdef FMAN_SWAP
    { "fman1Gpio",	HCF_RES_INT,	{ (void *)(GPIO_SGMII_SLOT_3) } }
#else
    { "fman1Gpio",	HCF_RES_INT,	{ (void *)(GPIO_RGMII) } }
#endif
#endif
};
#define dtsecMdio0Num NELEMENTS(dtsecMdio0Resources)

#define GPIO_XAUI_SLOT_4	2
#define GPIO_XAUI_SLOT_5	3

const struct hcfResource tgecMdio0Resources[] = {
    { "regBase",	HCF_RES_INT,	{ (void *)(CCSBAR + 0x4F1000) } },
    { "fman0Gpio",	HCF_RES_INT,	{ (void *)(GPIO_XAUI_SLOT_5) } },
    { "fman1Gpio",	HCF_RES_INT,	{ (void *)(GPIO_XAUI_SLOT_4) } }
};
#define tgecMdio0Num NELEMENTS(tgecMdio0Resources)

#ifdef INCLUDE_DUMMY_MDIO

/*
 * Simics doesn't properly simulate MDIO for the Fman network
 * ports. To work around this, we use the dummy MDIO driver
 * to make the network ports believe they always have a link
 * present.
 */

const struct hcfResource dummyMdio0Resources[] = {
    { "regBase",    HCF_RES_INT, { (void *)0 } },
    { "fullDuplex", HCF_RES_INT, { (void *)1 } },
    { "speed",      HCF_RES_INT, { (void *)1000 } }
};
#define dummyMdio0Num NELEMENTS(dummyMdio0Resources)

#endif /* INCLUDE_DUMMY_MDIO  */

/*
 * Each frame manager also include a 10 gigabit ethernet MAC (TGEC).
 * Use of the TGEC with the Freescale P4080 board requires the
 * purchase of a separate XAUI media expansion riser card. The
 * reset control word must also be updated if this card is used.
 */

const struct hcfResource tgec0Resources[] = {
    { "regBase", 	HCF_RES_INT,	{ (void *)(CCSBAR + 0x4F0000) } },
#ifdef TGEC0_FIBER
    { "fiberMedia",     HCF_RES_INT,    { (void *)(1) } }
#endif
};
#define tgec0Num NELEMENTS(tgec0Resources)

const struct hcfResource tgec1Resources[] = {
    { "regBase", 	HCF_RES_INT,	{ (void *)(CCSBAR + 0x5F0000) } },
#ifdef TGEC1_FIBER
    { "fiberMedia",     HCF_RES_INT,    { (void *)(1) } }
#endif

};
#define tgec1Num NELEMENTS(tgec1Resources)

#ifdef INCLUDE_ETSEC_VXB_END
/*
 * On the simulared P4080 in Simics, PHYs are physically wired according
 * to the following table:
 *
 * 		Data port pins	Management port pins	MII address
 *		--------------	--------------------	-----------
 * PHY0:	TSEC0		TSEC0			0
 * PHY1:	TSEC1		TSEC0			1
 * PHY2:	TSEC2		TSEC0			2
 * PHY3:	TSEC3		TSEC0			3
 *
 * The tricky part is that all PHYs have their management pins
 * connected to TSEC0. We have to make it look like PHY1 is connected
 * to TSEC1, so we provide a remapping resource that will cause
 * PHY1 to be attached to miibus1 instead of miibus0.
 */
const struct hcfResource tsecVxbEnd0Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)(CCSBAR + 0x31c000) } },
    { "txInt", HCF_RES_INT, { (void *)EPIC_TSEC1TX_INT_VEC } },
    { "txIntLevel", HCF_RES_INT, { (void *)EPIC_TSEC1TX_INT_VEC } },
    { "rxInt", HCF_RES_INT, { (void *)EPIC_TSEC1RX_INT_VEC } },
    { "rxIntLevel", HCF_RES_INT, { (void *)EPIC_TSEC1RX_INT_VEC } },
    { "errInt", HCF_RES_INT, { (void *)EPIC_TSEC1ERR_INT_VEC } },
    { "errIntLevel", HCF_RES_INT, { (void *)EPIC_TSEC1ERR_INT_VEC } },
    { "phyAddr",	HCF_RES_INT,	{ (void *)0 } },
    { "physMask",	HCF_RES_INT,	{ (void *)LOCAL_MEM_LOCAL_OFFSET } },
#ifdef INCLUDE_AMP
    { "miiIfName",      HCF_RES_STRING, { (void *)"tsecMdio" } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
    { "coreNum",        HCF_RES_INT,    { (void *)XTSEC0_CORE_NUM } }
#else /* INCLUDE_AMP */
    { "miiIfName",      HCF_RES_STRING, { (void *)XTSEC_NAME } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } }
#endif /* INCLUDE_AMP */
};
#define tsecVxbEnd0Num NELEMENTS(tsecVxbEnd0Resources)

const struct hcfResource tsecVxbEnd1Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)(CCSBAR + 0x31d000) } },
    { "txInt", HCF_RES_INT, { (void *)EPIC_TSEC2TX_INT_VEC } },
    { "txIntLevel", HCF_RES_INT, { (void *)EPIC_TSEC2TX_INT_VEC } },
    { "rxInt", HCF_RES_INT, { (void *)EPIC_TSEC2RX_INT_VEC } },
    { "rxIntLevel", HCF_RES_INT, { (void *)EPIC_TSEC2RX_INT_VEC } },
    { "errInt", HCF_RES_INT, { (void *)EPIC_TSEC2ERR_INT_VEC } },
    { "errIntLevel", HCF_RES_INT, { (void *)EPIC_TSEC2ERR_INT_VEC } },
    { "phyAddr",	HCF_RES_INT,	{ (void *)1 } },
    { "physMask",	HCF_RES_INT,	{ (void *)LOCAL_MEM_LOCAL_OFFSET } },
#ifdef INCLUDE_AMP
    { "miiIfName",      HCF_RES_STRING, { (void *)"tsecMdio" } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
    { "coreNum",        HCF_RES_INT,    { (void *)XTSEC1_CORE_NUM } }
#else /* INCLUDE_AMP */
    { "miiIfName",	HCF_RES_STRING,	{ (void *)XTSEC_NAME } },
    { "miiIfUnit",	HCF_RES_INT,	{ (void *)0 } },
#endif
};
#define tsecVxbEnd1Num NELEMENTS(tsecVxbEnd1Resources)

const struct hcfResource tsecVxbEnd2Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)(CCSBAR + 0x31e000) } },
    { "txInt", HCF_RES_INT, { (void *)EPIC_TSEC3TX_INT_VEC } },
    { "txIntLevel", HCF_RES_INT, { (void *)EPIC_TSEC3TX_INT_VEC } },
    { "rxInt", HCF_RES_INT, { (void *)EPIC_TSEC3RX_INT_VEC } },
    { "rxIntLevel", HCF_RES_INT, { (void *)EPIC_TSEC3RX_INT_VEC } },
    { "errInt", HCF_RES_INT, { (void *)EPIC_TSEC3ERR_INT_VEC } },
    { "errIntLevel", HCF_RES_INT, { (void *)EPIC_TSEC3ERR_INT_VEC } },
    { "phyAddr",	HCF_RES_INT,	{ (void *)2 } },
    { "physMask",	HCF_RES_INT,	{ (void *)LOCAL_MEM_LOCAL_OFFSET } },
#ifdef INCLUDE_AMP
    { "miiIfName",      HCF_RES_STRING, { (void *)"tsecMdio" } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
    { "coreNum",        HCF_RES_INT,    { (void *)XTSEC2_CORE_NUM } },
#else /* INCLUDE_AMP */
    { "miiIfName",	HCF_RES_STRING,	{ (void *)XTSEC_NAME } },
    { "miiIfUnit",	HCF_RES_INT,	{ (void *)0 } },
#endif

};
#define tsecVxbEnd2Num NELEMENTS(tsecVxbEnd2Resources)

const struct hcfResource tsecVxbEnd3Resources[] = {
    { "regBase", HCF_RES_INT, { (void *)(CCSBAR + 0x31f000) } },
    { "txInt", HCF_RES_INT, { (void *)EPIC_TSEC4TX_INT_VEC } },
    { "txIntLevel", HCF_RES_INT, { (void *)EPIC_TSEC4TX_INT_VEC } },
    { "rxInt", HCF_RES_INT, { (void *)EPIC_TSEC4RX_INT_VEC } },
    { "rxIntLevel", HCF_RES_INT, { (void *)EPIC_TSEC4RX_INT_VEC } },
    { "errInt", HCF_RES_INT, { (void *)EPIC_TSEC4ERR_INT_VEC } },
    { "errIntLevel", HCF_RES_INT, { (void *)EPIC_TSEC4ERR_INT_VEC } },
    { "phyAddr",	HCF_RES_INT,	{ (void *)3 } },
    { "physMask",	HCF_RES_INT,	{ (void *)LOCAL_MEM_LOCAL_OFFSET } },
#ifdef INCLUDE_AMP
    { "miiIfName",      HCF_RES_STRING, { (void *)"tsecMdio" } },
    { "miiIfUnit",      HCF_RES_INT,    { (void *)0 } },
    { "coreNum",        HCF_RES_INT,    { (void *)XTSEC3_CORE_NUM } },
#else /* INCLUDE_AMP */
    { "miiIfName",	HCF_RES_STRING,	{ (void *)XTSEC_NAME } },
    { "miiIfUnit",	HCF_RES_INT,	{ (void *)0 } },
#endif
};
#define tsecVxbEnd3Num NELEMENTS(tsecVxbEnd3Resources)
#endif

#ifdef DRV_PCIBUS_QORIQ_PCIEX

/*
 * The P4080 processor has three built-in PCIe host bridges,
 * but by default the P4080DS board is configured with only
 * two active PCIe slots: PCIEX1 is wired to slot 1 and
 * PCIEX3 is wired to slot 2. Slot 3 is used for SRIO by
 * default, but by changing the reset control word it is
 * possible to configure this slot as a third PCIe slot,
 * connected to PCIEX2. Slots 1 and 2 are set up for PCIe x2
 * operation only. If slot 3 is enabled for PCIe, it will
 * operate as x4. (Slot2 can also be configured to accomodate
 * a second SGMII riser card or a XAUI riser card.)
 *
 * Both legacy INTx and MSI interrupts are supported. Since
 * MSI is the prefered mechanism for handling interrupts
 * with PCIe, it's enabled by default. This can be changed by
 * setting the "msiEnable" property to FALSE (or just deleting
 * it). MSI will only be used for devices that advertise MSI
 * capability support in their configuration header.
 */

const struct hcfResource pci0Resources[] = {
    { "regBase",	HCF_RES_INT,	{ (void *)(CCSBAR + 0x200000) } },
    { "mem32Addr",	HCF_RES_ADDR,	{ (void *)PCIEX1_MEM_ADRS } },
    { "mem32Size",	HCF_RES_INT,	{ (void *)PCIEX1_MEM_SIZE } },
    { "memIo32Addr",	HCF_RES_ADDR,	{ (void *)PCIEX1_MEMIO_ADRS } },
    { "memIo32Size",	HCF_RES_INT,	{ (void *)PCIEX1_MEMIO_SIZE } },
    { "io32Addr",	HCF_RES_ADDR,	{ (void *)PCIEX1_IO32_ADRS } },
    { "io32Size",	HCF_RES_INT,	{ (void *)PCIEX1_IO32_SIZE } },
    { "io16Addr",	HCF_RES_ADDR,	{ (void *)PCIEX1_IO_ADRS } },
    { "io16Size",	HCF_RES_INT,	{ (void *)PCIEX1_IO_SIZE } },
    { "cacheSize",      HCF_RES_INT,    { (void *)(_CACHE_ALIGN_SIZE / 4) } },
    { "maxLatAllSet",   HCF_RES_INT,    { (void *)PCI_LAT_TIMER } },
    { "maxBusSet",      HCF_RES_INT,    { (void *)16 } },
    { "lawBase",	HCF_RES_ADDR, 	{ (void *)PCIEX1_LAW_BASE } },
    { "lawSize",	HCF_RES_INT, 	{ (void *)PCIEX1_LAW_SIZE } },
    { "includeFuncSet",     HCF_RES_ADDR, { (void *)sysPci1AutoconfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR, { (void *)sysPci1AutoconfigIntrAssign } },
    { "msiEnable",	HCF_RES_INT,	{ (void *)TRUE } }
};

#define pci0Num NELEMENTS(pci0Resources)

const struct hcfResource pci1Resources[] = {
    { "regBase",	HCF_RES_INT,	{ (void *)(CCSBAR + 0x201000) } },
    { "mem32Addr",	HCF_RES_ADDR,	{ (void *)PCIEX2_MEM_ADRS } },
    { "mem32Size",	HCF_RES_INT,	{ (void *)PCIEX2_MEM_SIZE } },
    { "memIo32Addr",	HCF_RES_ADDR,	{ (void *)PCIEX2_MEMIO_ADRS } },
    { "memIo32Size",	HCF_RES_INT,	{ (void *)PCIEX2_MEMIO_SIZE } },
    { "io32Addr",	HCF_RES_ADDR,	{ (void *)PCIEX2_IO32_ADRS } },
    { "io32Size",	HCF_RES_INT,	{ (void *)PCIEX2_IO32_SIZE } },
    { "io16Addr",	HCF_RES_ADDR,	{ (void *)PCIEX2_IO_ADRS } },
    { "io16Size",	HCF_RES_INT,	{ (void *)PCIEX2_IO_SIZE } },
    { "cacheSize",      HCF_RES_INT,    { (void *)(_CACHE_ALIGN_SIZE / 4) } }, 
    { "maxLatAllSet",   HCF_RES_INT,    { (void *)PCI_LAT_TIMER } }, 
    { "maxBusSet",      HCF_RES_INT,    { (void *)16 } }, 
    { "lawBase",	HCF_RES_ADDR, 	{ (void *)PCIEX2_LAW_BASE } },
    { "lawSize",	HCF_RES_INT, 	{ (void *)PCIEX2_LAW_SIZE } },
    { "includeFuncSet",     HCF_RES_ADDR, { (void *)sysPci1AutoconfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR, { (void *)sysPci2AutoconfigIntrAssign } },
    { "msiEnable",	HCF_RES_INT,	{ (void *)TRUE } }
};

#define pci1Num NELEMENTS(pci1Resources)

const struct hcfResource pci2Resources[] = {
    { "regBase",	HCF_RES_INT,	{ (void *)(CCSBAR + 0x202000) } },
    { "mem32Addr",	HCF_RES_ADDR,	{ (void *)PCIEX3_MEM_ADRS } },
    { "mem32Size",	HCF_RES_INT,	{ (void *)PCIEX3_MEM_SIZE } },
    { "memIo32Addr",	HCF_RES_ADDR,	{ (void *)PCIEX3_MEMIO_ADRS } },
    { "memIo32Size",	HCF_RES_INT,	{ (void *)PCIEX3_MEMIO_SIZE } },
    { "io32Addr",	HCF_RES_ADDR,	{ (void *)PCIEX3_IO32_ADRS } },
    { "io32Size",	HCF_RES_INT,	{ (void *)PCIEX3_IO32_SIZE } },
    { "io16Addr",	HCF_RES_ADDR,	{ (void *)PCIEX3_IO_ADRS } },
    { "io16Size",	HCF_RES_INT,	{ (void *)PCIEX3_IO_SIZE } },
    { "cacheSize",      HCF_RES_INT,    { (void *)(_CACHE_ALIGN_SIZE / 4) } }, 
    { "maxLatAllSet",   HCF_RES_INT,    { (void *)PCI_LAT_TIMER } }, 
    { "maxBusSet",      HCF_RES_INT,    { (void *)16 } }, 
    { "lawBase",	HCF_RES_ADDR, 	{ (void *)PCIEX3_LAW_BASE } },
    { "lawSize",	HCF_RES_INT, 	{ (void *)PCIEX3_LAW_SIZE } },
    { "includeFuncSet",     HCF_RES_ADDR, { (void *)sysPci1AutoconfigInclude } },
    { "intAssignFuncSet",   HCF_RES_ADDR, { (void *)sysPci3AutoconfigIntrAssign } },
    { "msiEnable",	HCF_RES_INT,	{ (void *)TRUE } },
};

#define pci2Num NELEMENTS(pci2Resources)

#endif /* DRV_PCIBUS_QORIQ_PCIEX */

const struct hcfResource law0Resources[] = {
    { VXB_REG_BASE, HCF_RES_INT, { (void *)(CCSBAR + 0xC00) } },
};
#define law0Num NELEMENTS(law0Resources)

/*
 * At least with the current silicon rev, he frame manager
 * in the P4080 requires a microcode patch in order for
 * certain features to work correctly. This includes the
 * soft parser, independent mode and the coarse classifier.
 * The microcode patch only needs to be loaded once per
 * fman instance, after a hard reset. In an AMP configuration,
 * only CPU 0 has to load it.
 */

struct hcfResource fman0Resources[] = {
    { VXB_REG_BASE,	HCF_RES_INT,	{(void *)(CCSBAR + 0x400000) } },
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_00)
    { "globalInit",	HCF_RES_INT,	{(void *)1} },
    { "ucodeAddr",	HCF_RES_ADDR,	{(void *)NULL} }
#endif
};
#define fman0Num NELEMENTS(fman0Resources)

struct hcfResource fman1Resources[] = {
    { VXB_REG_BASE,	HCF_RES_INT,	{(void *)(CCSBAR + 0x500000) } },
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_00)
    { "globalInit",	HCF_RES_INT,	{(void *)1} },
    { "ucodeAddr",	HCF_RES_ADDR,	{(void *)NULL} }
#endif
};
#define fman1Num NELEMENTS(fman1Resources)

const struct hcfResource qman0Resources[] = {
    { VXB_REG_BASE,  HCF_RES_INT,  {(void *)(CCSBAR + 0x318000) } },
    { "qmanLawBase", HCF_RES_ADDR, {(void *)QMAN_LAW_BASE} },
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_00)
    { "qmanMemBase", HCF_RES_ADDR, {(void *)QMAN_MEM_BASE} },
#endif
};
#define qman0Num NELEMENTS(qman0Resources)

#ifdef DRV_RESOURCE_QORIQBMAN 
const struct hcfResource bman0Resources[] = {
    { VXB_REG_BASE,  HCF_RES_INT,  {(void *)(CCSBAR + 0x31A000) } },
    { "bmanLawBase", HCF_RES_ADDR, {(void *)BMAN_LAW_BASE} },
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_00)
    { "bmanMemBase", HCF_RES_ADDR, {(void *)BMAN_MEM_BASE} },
#endif
};
#define bman0Num NELEMENTS(bman0Resources)
#endif

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

const struct hcfResource ns1655x1Resources[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)COM1_ADR} },
    { "irq",         HCF_RES_INT, {(void *)EPIC_DUART_INT_VEC} },
    { "regInterval", HCF_RES_INT, {(void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT, {(void *)EPIC_DUART_INT_VEC} },
    { "clkFreq",     HCF_RES_ADDR, {(void *)&sysClkFreqGet} }
    };

#define ns1655x1Num NELEMENTS(ns1655x1Resources)

const struct hcfResource ns1655x2Resources[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)COM2_ADR} },
    { "irq",         HCF_RES_INT, {(void *)EPIC_DUART_INT_VEC} },
    { "regInterval", HCF_RES_INT, {(void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT, {(void *)EPIC_DUART_INT_VEC} },
    { "clkFreq",     HCF_RES_ADDR, {(void *)&sysClkFreqGet} }
    };
#define ns1655x2Num NELEMENTS(ns1655x2Resources)

const struct hcfResource ns1655x3Resources[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)COM3_ADR} },
    { "irq",         HCF_RES_INT, {(void *)EPIC_DUART2_INT_VEC} },
    { "regInterval", HCF_RES_INT, {(void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT, {(void *)EPIC_DUART2_INT_VEC} },
    { "clkFreq",     HCF_RES_ADDR, {(void *)&sysClkFreqGet} }
    };
#define ns1655x3Num NELEMENTS(ns1655x3Resources)

const struct hcfResource ns1655x4Resources[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)COM4_ADR} },
    { "irq",         HCF_RES_INT, {(void *)EPIC_DUART2_INT_VEC} },
    { "regInterval", HCF_RES_INT, {(void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT, {(void *)EPIC_DUART2_INT_VEC} },
    { "clkFreq",     HCF_RES_ADDR, {(void *)&sysClkFreqGet} }
    };
#define ns1655x4Num NELEMENTS(ns1655x4Resources)

#ifdef USE_EXTRA_DUARTS
const struct hcfResource ns1655x5Resources[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)COM5_ADR} },
    { "irq",         HCF_RES_INT, {(void *)EPIC_DUART3_INT_VEC} },
    { "regInterval", HCF_RES_INT, {(void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT, {(void *)EPIC_DUART3_INT_VEC} },
    { "clkFreq",     HCF_RES_ADDR, {(void *)&sysClkFreqGet} }
    };

#define ns1655x5Num NELEMENTS(ns1655x5Resources)

const struct hcfResource ns1655x6Resources[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)COM6_ADR} },
    { "irq",         HCF_RES_INT, {(void *)EPIC_DUART3_INT_VEC} },
    { "regInterval", HCF_RES_INT, {(void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT, {(void *)EPIC_DUART3_INT_VEC} },
    { "clkFreq",     HCF_RES_ADDR, {(void *)&sysClkFreqGet} }
    };
#define ns1655x6Num NELEMENTS(ns1655x6Resources)

const struct hcfResource ns1655x7Resources[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)COM7_ADR} },
    { "irq",         HCF_RES_INT, {(void *)EPIC_DUART4_INT_VEC} },
    { "regInterval", HCF_RES_INT, {(void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT, {(void *)EPIC_DUART4_INT_VEC} },
    { "clkFreq",     HCF_RES_ADDR, {(void *)&sysClkFreqGet} }
    };
#define ns1655x7Num NELEMENTS(ns1655x7Resources)

const struct hcfResource ns1655x8Resources[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)COM8_ADR} },
    { "irq",         HCF_RES_INT, {(void *)EPIC_DUART4_INT_VEC} },
    { "regInterval", HCF_RES_INT, {(void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT, {(void *)EPIC_DUART4_INT_VEC} },
    { "clkFreq",     HCF_RES_ADDR, {(void *)&sysClkFreqGet} }
    };
#define ns1655x8Num NELEMENTS(ns1655x8Resources)

const struct hcfResource ns1655x9Resources[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)COM9_ADR} },
    { "irq",         HCF_RES_INT, {(void *)EPIC_DUART5_INT_VEC} },
    { "regInterval", HCF_RES_INT, {(void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT, {(void *)EPIC_DUART5_INT_VEC} },
    { "clkFreq",     HCF_RES_ADDR, {(void *)&sysClkFreqGet} }
    };

#define ns1655x9Num NELEMENTS(ns1655x9Resources)

const struct hcfResource ns1655x10Resources[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)COM10_ADR} },
    { "irq",         HCF_RES_INT, {(void *)EPIC_DUART5_INT_VEC} },
    { "regInterval", HCF_RES_INT, {(void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT, {(void *)EPIC_DUART5_INT_VEC} },
    { "clkFreq",     HCF_RES_ADDR, {(void *)&sysClkFreqGet} }
    };
#define ns1655x10Num NELEMENTS(ns1655x10Resources)

const struct hcfResource ns1655x11Resources[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)COM11_ADR} },
    { "irq",         HCF_RES_INT, {(void *)EPIC_DUART6_INT_VEC} },
    { "regInterval", HCF_RES_INT, {(void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT, {(void *)EPIC_DUART6_INT_VEC} },
    { "clkFreq",     HCF_RES_ADDR, {(void *)&sysClkFreqGet} }
    };
#define ns1655x11Num NELEMENTS(ns1655x11Resources)

const struct hcfResource ns1655x12Resources[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)COM8_ADR} },
    { "irq",         HCF_RES_INT, {(void *)EPIC_DUART6_INT_VEC} },
    { "regInterval", HCF_RES_INT, {(void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT, {(void *)EPIC_DUART6_INT_VEC} },
    { "clkFreq",     HCF_RES_ADDR, {(void *)&sysClkFreqGet} }
    };
#define ns1655x12Num NELEMENTS(ns1655x12Resources)

const struct hcfResource ns1655x13Resources[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)COM13_ADR} },
    { "irq",         HCF_RES_INT, {(void *)EPIC_DUART7_INT_VEC} },
    { "regInterval", HCF_RES_INT, {(void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT, {(void *)EPIC_DUART7_INT_VEC} },
    { "clkFreq",     HCF_RES_ADDR, {(void *)&sysClkFreqGet} }
    };

#define ns1655x13Num NELEMENTS(ns1655x13Resources)

const struct hcfResource ns1655x14Resources[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)COM14_ADR} },
    { "irq",         HCF_RES_INT, {(void *)(EPIC_DUART7_INT_VEC)} },
    { "regInterval", HCF_RES_INT, {(void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT, {(void *)(EPIC_DUART7_INT_VEC)} },
    { "clkFreq",     HCF_RES_ADDR, {(void *)&sysClkFreqGet} }
    };
#define ns1655x14Num NELEMENTS(ns1655x14Resources)

const struct hcfResource ns1655x15Resources[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)COM15_ADR} },
    { "irq",         HCF_RES_INT, {(void *)EPIC_DUART8_INT_VEC} },
    { "regInterval", HCF_RES_INT, {(void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT, {(void *)EPIC_DUART8_INT_VEC} },
    { "clkFreq",     HCF_RES_ADDR, {(void *)&sysClkFreqGet} }
    };
#define ns1655x15Num NELEMENTS(ns1655x15Resources)

const struct hcfResource ns1655x16Resources[] =
    {
    { "regBase",     HCF_RES_INT, {(void *)COM16_ADR} },
    { "irq",         HCF_RES_INT, {(void *)EPIC_DUART8_INT_VEC} },
    { "regInterval", HCF_RES_INT, {(void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT, {(void *)EPIC_DUART8_INT_VEC} },
    { "clkFreq",     HCF_RES_ADDR, {(void *)&sysClkFreqGet} }
    };
#define ns1655x16Num NELEMENTS(ns1655x16Resources)
#endif /* USE_EXTRA_DUARTS */

struct intrCtlrInputs epicInputs[] = {
#if defined(INCLUDE_MSD) || defined(INCLUDE_AMP_CPU_00) || !defined(INCLUDE_AMP)
    { EPIC_DUART_INT_VEC,    "ns16550", 0, 0},
#endif
#if defined (INCLUDE_AMP_CPU_01)
    { EPIC_DUART2_INT_VEC,   "ns16550", 0, 0},
#endif
#ifdef USE_EXTRA_DUARTS
#if defined (INCLUDE_AMP_CPU_02)
    { EPIC_DUART3_INT_VEC,    "ns16550", 0, 0},
#endif
#if defined (INCLUDE_AMP_CPU_03)
    { EPIC_DUART4_INT_VEC,    "ns16550", 0, 0},
#endif
#if defined (INCLUDE_AMP_CPU_04)
    { EPIC_DUART5_INT_VEC,    "ns16550", 0, 0},
#endif
#if defined (INCLUDE_AMP_CPU_05)
    { EPIC_DUART6_INT_VEC,    "ns16550", 0, 0},
#endif
#if defined (INCLUDE_AMP_CPU_06)
    { EPIC_DUART7_INT_VEC,    "ns16550", 0, 0},
#endif
#if defined (INCLUDE_AMP_CPU_07)
    { EPIC_DUART8_INT_VEC,    "ns16550", 0, 0},
#endif
#endif /* USE_EXTRA_DUARTS */
#ifdef INCLUDE_ETSEC_VXB_END
    { EPIC_TSEC1TX_INT_VEC,  XTSEC_NAME, 0, 0 },
    { EPIC_TSEC1RX_INT_VEC,  XTSEC_NAME, 0, 1 },
    { EPIC_TSEC1ERR_INT_VEC, XTSEC_NAME, 0, 2 },
    { EPIC_TSEC2TX_INT_VEC,  XTSEC_NAME, 1, 0 },
    { EPIC_TSEC2RX_INT_VEC,  XTSEC_NAME, 1, 1 },
    { EPIC_TSEC2ERR_INT_VEC, XTSEC_NAME, 1, 2 },
    { EPIC_TSEC3TX_INT_VEC,  XTSEC_NAME, 2, 0 },
    { EPIC_TSEC3RX_INT_VEC,  XTSEC_NAME, 2, 1 },
    { EPIC_TSEC3ERR_INT_VEC, XTSEC_NAME, 2, 2 },
    { EPIC_TSEC4TX_INT_VEC,  XTSEC_NAME, 3, 0 },
    { EPIC_TSEC4RX_INT_VEC,  XTSEC_NAME, 3, 1 },
    { EPIC_TSEC4ERR_INT_VEC, XTSEC_NAME, 3, 2 },
#endif
    { EPIC_QMAN_PORTAL0_INT_VEC, "QorIQQman", 0, 0 },
    { EPIC_QMAN_PORTAL1_INT_VEC, "QorIQQman", 0, 1 },
    { EPIC_QMAN_PORTAL2_INT_VEC, "QorIQQman", 0, 2 },
    { EPIC_QMAN_PORTAL3_INT_VEC, "QorIQQman", 0, 3 },
    { EPIC_QMAN_PORTAL4_INT_VEC, "QorIQQman", 0, 4 },
    { EPIC_QMAN_PORTAL5_INT_VEC, "QorIQQman", 0, 5 },
    { EPIC_QMAN_PORTAL6_INT_VEC, "QorIQQman", 0, 6 },
    { EPIC_QMAN_PORTAL7_INT_VEC, "QorIQQman", 0, 7 },
    { EPIC_QMAN_PORTAL8_INT_VEC, "QorIQQman", 0, 8 },
    { EPIC_QMAN_PORTAL9_INT_VEC, "QorIQQman", 0, 9 },
    { EPIC_BMAN_PORTAL0_INT_VEC, "QorIQBman", 0, 0 },
    { EPIC_BMAN_PORTAL1_INT_VEC, "QorIQBman", 0, 1 },
    { EPIC_BMAN_PORTAL2_INT_VEC, "QorIQBman", 0, 2 },
    { EPIC_BMAN_PORTAL3_INT_VEC, "QorIQBman", 0, 3 },
    { EPIC_BMAN_PORTAL4_INT_VEC, "QorIQBman", 0, 4 },
    { EPIC_BMAN_PORTAL5_INT_VEC, "QorIQBman", 0, 5 },
    { EPIC_BMAN_PORTAL6_INT_VEC, "QorIQBman", 0, 6 },
    { EPIC_BMAN_PORTAL7_INT_VEC, "QorIQBman", 0, 7 },
    { EPIC_BMAN_PORTAL8_INT_VEC, "QorIQBman", 0, 8 },
    { EPIC_BMAN_PORTAL9_INT_VEC, "QorIQBman", 0, 9 },
    { GPIO_INT_VEC, "fslGpio", 0, 0 },
#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    { OPENPIC_TIMERA0_INT_VEC,      "openPicTimer", 0, 0 },
    { OPENPIC_TIMERA0_INT_VEC+1,    "openPicTimer", 0, 1 },
    { OPENPIC_TIMERA0_INT_VEC+2,    "openPicTimer", 0, 2 },
    { OPENPIC_TIMERA0_INT_VEC+3,    "openPicTimer", 0, 3 },
    { OPENPIC_TIMERB0_INT_VEC,      "openPicTimer", 1, 0 },
    { OPENPIC_TIMERB0_INT_VEC+1,    "openPicTimer", 1, 1 },
    { OPENPIC_TIMERB0_INT_VEC+2,    "openPicTimer", 1, 2 },
    { OPENPIC_TIMERB0_INT_VEC+3,    "openPicTimer", 1, 3 },
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */

#ifdef INCLUDE_EHCI
    { EPIC_USB_INT_VEC,         "vxbPlbUsbEhci",    0,      0 },
#endif  /* INCLUDE_EHCI */

#ifdef  DRV_DMA_FSL
    { EPIC_DMA1_CHAN0_INT_VEC,  "fslDma",           0,      0 },
    { EPIC_DMA1_CHAN1_INT_VEC,  "fslDma",           0,      1 },
    { EPIC_DMA1_CHAN2_INT_VEC,  "fslDma",           0,      2 },
    { EPIC_DMA1_CHAN3_INT_VEC,  "fslDma",           0,      3 },
    { EPIC_DMA2_CHAN0_INT_VEC,  "fslDma",           1,      0 },
    { EPIC_DMA2_CHAN1_INT_VEC,  "fslDma",           1,      1 },
    { EPIC_DMA2_CHAN2_INT_VEC,  "fslDma",           1,      2 },
    { EPIC_DMA2_CHAN3_INT_VEC,  "fslDma",           1,      3 },
#endif  /* DRV_DMA_FSL */

#ifdef DRV_STORAGE_SDHC
    { EPIC_ESDHC_INT_VEC,        "sdhci",           0,      0 },
#endif /* DRV_STORAGE_SDHC */

#ifdef _WRS_CONFIG_SMP
    { EPIC_VEC_IPI_IRQ0,     "ipi", 0, 0 }
#else
#ifdef INCLUDE_MOB_8XXX
# if defined(INCLUDE_MOB_PLB_0)&&(MOB_PLB_RX_CONTEXT_0>0)
    { EPIC_VEC_IPI_IRQ0,     "mob8572", 0, 0 }
# else
#   if defined(INCLUDE_MOB_PLB_1)&&(MOB_PLB_RX_CONTEXT_1>0)
    { EPIC_VEC_IPI_IRQ0,     "mob8572", 1, 0 }
#   else
    { EPIC_VEC_IPI_IRQ0,     "mob8572", 0, 0 }
#   endif
# endif
#endif /* INCLUDE_MOB_8XXX */
#endif /* _WRS_CONFIG_SMP */

};

struct intrCtlrXBar epicXBar[] = {
    { 0, 0 }
};

struct intrCtlrCpu epicCpu[] = {
    { EPIC_TSEC3ERR_INT_VEC, 1 },
    { EPIC_TSEC1ERR_INT_VEC, 1 },
    { EPIC_TSEC4ERR_INT_VEC, 1 },
    { EPIC_TSEC2ERR_INT_VEC, 1 }
};

struct intrCtlrPriority epicPriority[] = {
    { EPIC_DUART2_INT_VEC, 100 },
    { EPIC_DUART_INT_VEC,  100 }
};

INT_BANK_DESC exDesc = { 0, 12, 0x50000, 0x20, EPIC_EX_INTERRUPT };
INT_BANK_DESC inDesc = { EPIC_VEC_IN_IRQ0, 128, 0x50200, 0x20, EPIC_IN_INTERRUPT };
INT_BANK_DESC msgDesc = { 176, 8, 0x51600, 0x20, EPIC_MSG_INTERRUPT };
INT_BANK_DESC gtaDesc = { OPENPIC_TIMERA0_INT_VEC, 4, 0x41120, 0x40, EPIC_GT_A_INTERRUPT };
INT_BANK_DESC gtbDesc = { OPENPIC_TIMERB0_INT_VEC, 4, 0x42120, 0x40, EPIC_GT_B_INTERRUPT };
INT_BANK_DESC smsgDesc = { 243, 8, 0x51c00, 0x20, EPIC_SMSG_INTERRUPT };
INT_BANK_DESC ipiDesc = { 251, 4, 0x410a0, 0x10, EPIC_IPI_INTERRUPT };

const struct hcfResource epic0Resources[] = {
    { VXB_REG_BASE,	   HCF_RES_INT,  {(void *)CCSBAR } },
    { "input",		   HCF_RES_ADDR, {(void *)&epicInputs[0] } },
    { "inputTableSize",    HCF_RES_INT,  {(void *)NELEMENTS(epicInputs) } },
    { "numCpus",           HCF_RES_INT,  {(void *)(8) } },
    { "priority",	   HCF_RES_ADDR, {(void *)&epicPriority[0] } },
    { "priorityTableSize", HCF_RES_INT,  {(void *)NELEMENTS(epicPriority) } },
    { "crossBar",	   HCF_RES_ADDR, {(void *)&epicXBar[0] } },
    { "crossBarTableSize", HCF_RES_INT,  {(void *)NELEMENTS(epicXBar) } },
    { "cpuRoute",	   HCF_RES_ADDR, {(void *)&epicCpu[0] } },
    { "cpuRouteTableSize", HCF_RES_INT,  {(void *)NELEMENTS(epicCpu) } },
    { "exPolar",           HCF_RES_INT,  {(void *)EPIC_INT_ACT_LOW } },
    { "numInts", HCF_RES_INT, {(void *)255 } },
    { "intDesc_0", HCF_RES_ADDR, {(void *)&exDesc } },
    { "intDesc_1", HCF_RES_ADDR, {(void *)&inDesc } },
    { "intDesc_2", HCF_RES_ADDR, {(void *)&msgDesc } },
    { "intDesc_3", HCF_RES_ADDR, {(void *)&gtaDesc } },
    { "intDesc_4", HCF_RES_ADDR, {(void *)&gtbDesc } },
    { "intDesc_5", HCF_RES_ADDR, {(void *)&smsgDesc } },
    { "intDesc_6", HCF_RES_ADDR, {(void *)&ipiDesc } },
#ifdef EPIC_EXTERNAL_PROXY
    { "sysEprGet", HCF_RES_ADDR, {(void *)&sysEprGet } },
#endif
};
#define epic0Num NELEMENTS(epic0Resources)

#ifdef DRV_TIMER_M85XX

struct hcfResource m85xxTimerResources[] =  {
    {"regBase", HCF_RES_INT, {(void *)0}},
    {"decMinClkRate",HCF_RES_INT, {(void *)SYS_CLK_RATE_MIN}},
    {"decMaxClkRate",HCF_RES_INT, {(void *)SYS_CLK_RATE_MAX}},
    {"fitMinClkRate",HCF_RES_INT, {(void *)AUX_CLK_RATE_MIN}},
    {"fitMaxClkRate",HCF_RES_INT, {(void *)AUX_CLK_RATE_MAX}},
    {"sysClockFreq",HCF_RES_ADDR, {(void *)&sysClkFreqGet}}
};
#define m85xxTimerNum         NELEMENTS(m85xxTimerResources)

#endif /* DRV_TIMER_M85XX */

struct intrCtlrInputs ppcIntCtlrInputs[] = {
    { 0, "epic", 0, 0 }
};

const struct hcfResource ppcIntCtlr0Resources[] = {
    { VXB_REG_BASE,     HCF_RES_INT,  {(void *)TRUE} },
    { "input",		HCF_RES_ADDR, {(void *)&ppcIntCtlrInputs[0] } },
    { "inputTableSize", HCF_RES_INT,  {(void *)NELEMENTS(ppcIntCtlrInputs) } },
};
#define ppcIntCtlr0Num NELEMENTS(ppcIntCtlr0Resources)

#ifdef INCLUDE_EHCI
const struct hcfResource usbEhci0Resources[] = {
    { VXB_REG_BASE,         HCF_RES_INT,    { (void *)EHCI_CAPLENGTH(CCSBAR + 0x210000) } },
    { "ehciInit",           HCF_RES_ADDR,   { (void *)ehci0Init } },
    { "dataSwap",           HCF_RES_ADDR,   { (void *)vxbSwap32 } },
};
#define usbEhci0Num NELEMENTS(usbEhci0Resources)
#endif /* INCLUDE_EHCI */

#ifdef DRV_TIMER_OPENPIC
const struct hcfResource openPicTimerDevAresources[] =
    {
    {VXB_REG_BASE,   HCF_RES_INT,    {(void *)(CCSBAR + 0x000410F0)}},
    {"instanceName", HCF_RES_STRING, {(void *)"A" }},
    {VXB_CLK_FREQ,   HCF_RES_ADDR,   {(void *)sysPicClkFreqGet}},
    {"clkDivisor",   HCF_RES_INT,    {(void *)8}},
#ifdef INCLUDE_AUX_CLK
    {"maxClkRate0",  HCF_RES_INT,    {(void *)AUX_CLK_RATE_MAX }},
    {"maxClkRate1",  HCF_RES_INT,    {(void *)AUX_CLK_RATE_MAX }},
    {"maxClkRate2",  HCF_RES_INT,    {(void *)AUX_CLK_RATE_MAX }},
    {"maxClkRate3",  HCF_RES_INT,    {(void *)AUX_CLK_RATE_MAX }},
#endif /* INCLUDE_AUX_CLK */
#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB
    {"vector0",      HCF_RES_INT,    {(void *)OPENPIC_TIMERA0_INT_VEC }},
    {"vector1",      HCF_RES_INT,    {(void *)OPENPIC_TIMERA0_INT_VEC+1 }},
    {"vector2",      HCF_RES_INT,    {(void *)OPENPIC_TIMERA0_INT_VEC+2 }},
    {"vector3",      HCF_RES_INT,    {(void *)OPENPIC_TIMERA0_INT_VEC+3 }},
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
    };
#define openPicTimerDevAnum NELEMENTS(openPicTimerDevAresources)

const struct hcfResource openPicTimerDevBresources[] =
    {
    {VXB_REG_BASE,   HCF_RES_INT,    {(void *)(CCSBAR + 0x000420F0)}},
    {"instanceName", HCF_RES_STRING, {(void *)"B" }},
    {VXB_CLK_FREQ,   HCF_RES_ADDR,   {(void *)sysPicClkFreqGet}},
    {"clkDivisor",   HCF_RES_INT,    {(void *)0}},
#ifdef INCLUDE_AUX_CLK
    {"maxClkRate0",  HCF_RES_INT,    {(void *)AUX_CLK_RATE_MAX }},
    {"maxClkRate1",  HCF_RES_INT,    {(void *)AUX_CLK_RATE_MAX }},
    {"maxClkRate2",  HCF_RES_INT,    {(void *)AUX_CLK_RATE_MAX }},
    {"maxClkRate3",  HCF_RES_INT,    {(void *)AUX_CLK_RATE_MAX }},
#endif /* INCLUDE_AUX_CLK */
#ifndef INCLUDE_INTCTLR_DYNAMIC_LIB  
    {"vector0",      HCF_RES_INT,    {(void *)OPENPIC_TIMERB0_INT_VEC }},
    {"vector1",      HCF_RES_INT,    {(void *)OPENPIC_TIMERB0_INT_VEC+1 }},
    {"vector2",      HCF_RES_INT,    {(void *)OPENPIC_TIMERB0_INT_VEC+2 }},
    {"vector3",      HCF_RES_INT,    {(void *)OPENPIC_TIMERB0_INT_VEC+3 }},
#endif /* INCLUDE_INTCTLR_DYNAMIC_LIB */
    };
#define openPicTimerDevBnum NELEMENTS(openPicTimerDevBresources)
#endif /* DRV_TIMER_OPENPIC */

#ifdef DRV_STORAGE_SDHC
struct hcfResource fslSdhcResources[] =  {
    { "regBase",             HCF_RES_INT,   { (void *)(CCSBAR + 0x114000) } },
    { "clkFreq",             HCF_RES_ADDR,  { (void *)sysSdhcClkFreqGet } },
    { "dmaMode",             HCF_RES_INT,   { (void *)0 } },
    { "polling",             HCF_RES_INT,   { (void *)0 } },
    { "flags" ,              HCF_RES_INT,   { (void *)(SDHC_PIO_NEED_DELAY | SDHC_HW_SNOOP |
                                                       SDHC_FIFO_ENDIANESS_REVERSE |
                                                       SDHC_HOST_VER_REVERSE |
                                                       SDHC_HOST_CTRL_FREESCALE) } },
};
#   define fslSdhcNum  NELEMENTS(fslSdhcResources)
#endif  /* DRV_STORAGE_SDHC */

#ifdef INCLUDE_TSEC_MDIO
LOCAL const struct hcfResource mdio0Resources[] = {
    { "regBase",        HCF_RES_INT,    { (void *)(CCSBAR + 0x31c000) } },
    { "sharedMem",      HCF_RES_INT,    { (void *)TM_ANCHOR_ADRS } }
};
#define mdio0Num NELEMENTS(mdio0Resources)
#endif /* INCLUDE_TSEC_MDIO */

#ifdef INCLUDE_MOB_8XXX
void * mob8572RegBaseDummy [10]; /* these are not real drivers, but an entry is
                                  * needed by the vxbus subsystem */

/* We increase the ANCHOR_BASE by 64 bytes to leave room for a legacy SM anchor
 * in case it is included (needed for VxMP, smNet).  In that event, the SM
 * region will use of the area that has been allocated for MOB_1 */
#define MOB_8572_ANCHOR_BASE_0      (0x20000000 + SM_ANCHOR_OFFSET + 0x40)
#define MOB_8572_ANCHOR_BASE_1      (MOB_8572_ANCHOR_BASE_0 + 0x600)

/* This IPI ID may need to be modified when Linux or vxWorks SMP is running on 
 * the same multicore processor. It doesn't matter for dual core as that 
 * will never happen.
 */
#define MOB_VX_IPI_ID  0

/* Virtual bus controller managing the local node. Most values are user-defined
 * by component parameters. The intr and intrLevel entries are specific to
 * this BSP and should not be changed. The real node, anchor and smStart
 * parameters will be computed at runtime, depending on which core is running.
 * It has to be done this way since the same VxWorks image runs on the
 * different cores.
 */

#ifdef INCLUDE_MOB_PLB_0
struct hcfResource mob8572Res_0[] = {
    { "regBase",	RES_INT,  {(void *) mob8572RegBaseDummy} },
    { "bus_type",	RES_ADDR, {(void *) "plb_0"} },
    { "intr",		RES_INT,  {(void *) EPIC_VEC_IPI_IRQ0} },
    { "intrLevel",	RES_INT,  {(void *) EPIC_VEC_IPI_IRQ0} },
    { "anchor_base",	RES_ADDR, {(void *) MOB_8572_ANCHOR_BASE_0} },
    { "sm_start",	RES_ADDR, {(void *) MOB_PLB_SM_POOL_BASE_0} },
    { "sz_pool",	RES_INT,  {(void *) MOB_PLB_SM_POOL_SIZE_0} },
    { "bus_num",	RES_INT,  {(void *) MOB_PLB_BUS_NUM_0} },
    { "node_num",	RES_INT,  {(void *) MOB_PLB_NODE_NUM_0} },
    { "max_nodes",	RES_INT,  {(void *) MOB_PLB_MAX_NODES_0} },
    { "num_events",     RES_INT,  {(void *) MOB_PLB_NUM_EVENTS_0} },
    { "rx_context",	RES_INT,  {(void *) MOB_PLB_RX_CONTEXT_0} },
    { "vxIpiId",	RES_INT,  {(void *) MOB_VX_IPI_ID} }
};
#endif /* INCLUDE_MOB_PLB_0 */
#ifdef INCLUDE_MOB_PLB_1
struct hcfResource mob8572Res_1[] = {
    { "regBase",	RES_INT,  {(void *) mob8572RegBaseDummy} },
    { "bus_type",	RES_ADDR, {(void *) "plb_1"} },
    { "intr",		RES_INT,  {(void *) EPIC_VEC_IPI_IRQ0} },
    { "intrLevel",	RES_INT,  {(void *) EPIC_VEC_IPI_IRQ0} },
    { "anchor_base",	RES_ADDR, {(void *) MOB_8572_ANCHOR_BASE_1} },
    { "sm_start",	RES_ADDR, {(void *) MOB_PLB_SM_POOL_BASE_1} },
    { "sz_pool",	RES_INT,  {(void *) MOB_PLB_SM_POOL_SIZE_1} },
    { "bus_num",	RES_INT,  {(void *) MOB_PLB_BUS_NUM_1} },
    { "node_num",	RES_INT,  {(void *) MOB_PLB_NODE_NUM_1} },
    { "max_nodes",	RES_INT,  {(void *) MOB_PLB_MAX_NODES_1} },
    { "num_events",     RES_INT,  {(void *) MOB_PLB_NUM_EVENTS_1} },
    { "rx_context",	RES_INT,  {(void *) MOB_PLB_RX_CONTEXT_1} },
    { "vxIpiId",	RES_INT,  {(void *) MOB_VX_IPI_ID} }
};
#endif /* INCLUDE_MOB_PLB_1 */
#endif /* INCLUDE_MOB_PLB */

#ifdef INCLUDE_MOB_PLB_0
struct hcfResource mobVx8572Res0_0[] = {
    { "regBase",    RES_INT,  {(void *) mob8572RegBaseDummy} },
    { "node_num",   RES_INT,  {(void *) 0} },
    { "bus_num",    RES_INT,  {(void *) MOB_PLB_BUS_NUM_0} },
    { "anchor",     RES_ADDR, {(void *) MOB_8572_ANCHOR_BASE_0} },
    { "max_nodes",  RES_INT,  {(void *) MOB_PLB_MAX_NODES_0} },
    { "vxIpiId",    RES_INT,  {(void *) MOB_VX_IPI_ID} }
};

struct hcfResource mobVx8572Res1_0[] = {
    { "regBase",    RES_INT,  {(void *) mob8572RegBaseDummy} },
    { "node_num",   RES_INT,  {(void *) 1} },
    { "bus_num",    RES_INT,  {(void *) MOB_PLB_BUS_NUM_0} },
    { "anchor",     RES_ADDR, {(void *) MOB_8572_ANCHOR_BASE_0} },
    { "max_nodes",  RES_INT,  {(void *) MOB_PLB_MAX_NODES_0} },
    { "vxIpiId",    RES_INT,  {(void *) MOB_VX_IPI_ID} }
};

#if (MOB_PLB_MAX_NODES_0 > 2)
struct hcfResource mobVx8572Res2_0[] = {
    { "regBase",    RES_INT,  {(void *) mob8572RegBaseDummy} },
    { "node_num",   RES_INT,  {(void *) 2} },
    { "bus_num",    RES_INT,  {(void *) MOB_PLB_BUS_NUM_0} },
    { "anchor",     RES_ADDR, {(void *) MOB_8572_ANCHOR_BASE_0} },
    { "max_nodes",  RES_INT,  {(void *) MOB_PLB_MAX_NODES_0} },
    { "vxIpiId",    RES_INT,  {(void *) MOB_VX_IPI_ID} }
};
#endif 

#if (MOB_PLB_MAX_NODES_0 > 3)
struct hcfResource mobVx8572Res3_0[] = {
    { "regBase",    RES_INT,  {(void *) mob8572RegBaseDummy} },
    { "node_num",   RES_INT,  {(void *) 3} },
    { "bus_num",    RES_INT,  {(void *) MOB_PLB_BUS_NUM_0} },
    { "anchor",     RES_ADDR, {(void *) MOB_8572_ANCHOR_BASE_0} },
    { "max_nodes",  RES_INT,  {(void *) MOB_PLB_MAX_NODES_0} },
    { "vxIpiId",    RES_INT,  {(void *) MOB_VX_IPI_ID} }
};
#endif 

#if (MOB_PLB_MAX_NODES_0 > 4)
struct hcfResource mobVx8572Res4_0[] = {
    { "regBase",    RES_INT,  {(void *) mob8572RegBaseDummy} },
    { "node_num",   RES_INT,  {(void *) 4} },
    { "bus_num",    RES_INT,  {(void *) MOB_PLB_BUS_NUM_0} },
    { "anchor",     RES_ADDR, {(void *) MOB_8572_ANCHOR_BASE_0} },
    { "max_nodes",  RES_INT,  {(void *) MOB_PLB_MAX_NODES_0} },
    { "vxIpiId",    RES_INT,  {(void *) MOB_VX_IPI_ID} }
};
#endif 

#if (MOB_PLB_MAX_NODES_0 > 5)
struct hcfResource mobVx8572Res5_0[] = {
    { "regBase",    RES_INT,  {(void *) mob8572RegBaseDummy} },
    { "node_num",   RES_INT,  {(void *) 5} },
    { "bus_num",    RES_INT,  {(void *) MOB_PLB_BUS_NUM_0} },
    { "anchor",     RES_ADDR, {(void *) MOB_8572_ANCHOR_BASE_0} },
    { "max_nodes",  RES_INT,  {(void *) MOB_PLB_MAX_NODES_0} },
    { "vxIpiId",    RES_INT,  {(void *) MOB_VX_IPI_ID} }
};
#endif 

#if (MOB_PLB_MAX_NODES_0 > 6)
struct hcfResource mobVx8572Res6_0[] = {
    { "regBase",    RES_INT,  {(void *) mob8572RegBaseDummy} },
    { "node_num",   RES_INT,  {(void *) 6} },
    { "bus_num",    RES_INT,  {(void *) MOB_PLB_BUS_NUM_0} },
    { "anchor",     RES_ADDR, {(void *) MOB_8572_ANCHOR_BASE_0} },
    { "max_nodes",  RES_INT,  {(void *) MOB_PLB_MAX_NODES_0} },
    { "vxIpiId",    RES_INT,  {(void *) MOB_VX_IPI_ID} }
};
#endif 

#if (MOB_PLB_MAX_NODES_0 > 7)
struct hcfResource mobVx8572Res7_0[] = {
    { "regBase",    RES_INT,  {(void *) mob8572RegBaseDummy} },
    { "node_num",   RES_INT,  {(void *) 7} },
    { "bus_num",    RES_INT,  {(void *) MOB_PLB_BUS_NUM_0} },
    { "anchor",     RES_ADDR, {(void *) MOB_8572_ANCHOR_BASE_0} },
    { "max_nodes",  RES_INT,  {(void *) MOB_PLB_MAX_NODES_0} },
    { "vxIpiId",    RES_INT,  {(void *) MOB_VX_IPI_ID} }
};
#endif 
#endif /* INCLUDE_MOB_PLB_0 */

#ifdef INCLUDE_MOB_PLB_1
struct hcfResource mobVx8572Res0_1[] = {
    { "regBase",    RES_INT,  {(void *) mob8572RegBaseDummy} },
    { "node_num",   RES_INT,  {(void *) 0} },
    { "bus_num",    RES_INT,  {(void *) MOB_PLB_BUS_NUM_1} },
    { "anchor",     RES_ADDR, {(void *) MOB_8572_ANCHOR_BASE_1} },
    { "max_nodes",  RES_INT,  {(void *) MOB_PLB_MAX_NODES_1} },
    { "vxIpiId",    RES_INT,  {(void *) MOB_VX_IPI_ID} }
};

struct hcfResource mobVx8572Res1_1[] = {
    { "regBase",    RES_INT,  {(void *) mob8572RegBaseDummy} },
    { "node_num",   RES_INT,  {(void *) 1} },
    { "bus_num",    RES_INT,  {(void *) MOB_PLB_BUS_NUM_1} },
    { "anchor",     RES_ADDR, {(void *) MOB_8572_ANCHOR_BASE_1} },
    { "max_nodes",  RES_INT,  {(void *) MOB_PLB_MAX_NODES_1} },
    { "vxIpiId",    RES_INT,  {(void *) MOB_VX_IPI_ID} }
};

#if (MOB_PLB_MAX_NODES_1 > 2)
struct hcfResource mobVx8572Res2_1[] = {
    { "regBase",    RES_INT,  {(void *) mob8572RegBaseDummy} },
    { "node_num",   RES_INT,  {(void *) 2} },
    { "bus_num",    RES_INT,  {(void *) MOB_PLB_BUS_NUM_1} },
    { "anchor",     RES_ADDR, {(void *) MOB_8572_ANCHOR_BASE_1} },
    { "max_nodes",  RES_INT,  {(void *) MOB_PLB_MAX_NODES_1} },
    { "vxIpiId",    RES_INT,  {(void *) MOB_VX_IPI_ID} }
};
#endif 

#if (MOB_PLB_MAX_NODES_1 > 3)
struct hcfResource mobVx8572Res3_1[] = {
    { "regBase",    RES_INT,  {(void *) mob8572RegBaseDummy} },
    { "node_num",   RES_INT,  {(void *) 3} },
    { "bus_num",    RES_INT,  {(void *) MOB_PLB_BUS_NUM_1} },
    { "anchor",     RES_ADDR, {(void *) MOB_8572_ANCHOR_BASE_1} },
    { "max_nodes",  RES_INT,  {(void *) MOB_PLB_MAX_NODES_1} },
    { "vxIpiId",    RES_INT,  {(void *) MOB_VX_IPI_ID} }
};
#endif 

#if (MOB_PLB_MAX_NODES_1 > 4)
struct hcfResource mobVx8572Res4_1[] = {
    { "regBase",    RES_INT,  {(void *) mob8572RegBaseDummy} },
    { "node_num",   RES_INT,  {(void *) 4} },
    { "bus_num",    RES_INT,  {(void *) MOB_PLB_BUS_NUM_1} },
    { "anchor",     RES_ADDR, {(void *) MOB_8572_ANCHOR_BASE_1} },
    { "max_nodes",  RES_INT,  {(void *) MOB_PLB_MAX_NODES_1} },
    { "vxIpiId",    RES_INT,  {(void *) MOB_VX_IPI_ID} }
};
#endif 

#if (MOB_PLB_MAX_NODES_1 > 5)
struct hcfResource mobVx8572Res5_1[] = {
    { "regBase",    RES_INT,  {(void *) mob8572RegBaseDummy} },
    { "node_num",   RES_INT,  {(void *) 5} },
    { "bus_num",    RES_INT,  {(void *) MOB_PLB_BUS_NUM_1} },
    { "anchor",     RES_ADDR, {(void *) MOB_8572_ANCHOR_BASE_1} },
    { "max_nodes",  RES_INT,  {(void *) MOB_PLB_MAX_NODES_1} },
    { "vxIpiId",    RES_INT,  {(void *) MOB_VX_IPI_ID} }
};
#endif 

#if (MOB_PLB_MAX_NODES_1 > 6)
struct hcfResource mobVx8572Res6_1[] = {
    { "regBase",    RES_INT,  {(void *) mob8572RegBaseDummy} },
    { "node_num",   RES_INT,  {(void *) 6} },
    { "bus_num",    RES_INT,  {(void *) MOB_PLB_BUS_NUM_1} },
    { "anchor",     RES_ADDR, {(void *) MOB_8572_ANCHOR_BASE_1} },
    { "max_nodes",  RES_INT,  {(void *) MOB_PLB_MAX_NODES_1} },
    { "vxIpiId",    RES_INT,  {(void *) MOB_VX_IPI_ID} }
};
#endif 

#if (MOB_PLB_MAX_NODES_1 > 7)
struct hcfResource mobVx8572Res7_1[] = {
    { "regBase",    RES_INT,  {(void *) mob8572RegBaseDummy} },
    { "node_num",   RES_INT,  {(void *) 7} },
    { "bus_num",    RES_INT,  {(void *) MOB_PLB_BUS_NUM_1} },
    { "anchor",     RES_ADDR, {(void *) MOB_8572_ANCHOR_BASE_1} },
    { "max_nodes",  RES_INT,  {(void *) MOB_PLB_MAX_NODES_1} },
    { "vxIpiId",    RES_INT,  {(void *) MOB_VX_IPI_ID} }
};
#endif 
#endif /* INCLUDE_MOB_PLB_1 */

const struct hcfDevice hcfDeviceList[] = {
    
    /*
     * Initialize ppcIntCtlr before epic.
     * The vector table for external interrupts are over written by epic
     * for an optimized purpose.
     */

    { "ppcIntCtlr", 0, VXB_BUSID_PLB, 0, ppcIntCtlr0Num, ppcIntCtlr0Resources },
    { "epic", 0,       VXB_BUSID_PLB, 0, epic0Num, epic0Resources },
#ifdef INCLUDE_ETSEC_VXB_END
    { XTSEC_NAME, 0, VXB_BUSID_PLB, 0, tsecVxbEnd0Num, tsecVxbEnd0Resources },
    { XTSEC_NAME, 1, VXB_BUSID_PLB, 0, tsecVxbEnd1Num, tsecVxbEnd1Resources },
    { XTSEC_NAME, 2, VXB_BUSID_PLB, 0, tsecVxbEnd2Num, tsecVxbEnd2Resources },
    { XTSEC_NAME, 3, VXB_BUSID_PLB, 0, tsecVxbEnd3Num, tsecVxbEnd3Resources },
#endif
#ifdef INCLUDE_TSEC_MDIO
    { "tsecMdio", 0, VXB_BUSID_PLB, 0, mdio0Num, mdio0Resources },
#endif

#ifdef DRV_SIO_NS16550
#if defined (INCLUDE_MSD) || !defined (INCLUDE_AMP)
    { "ns16550", 0, VXB_BUSID_PLB, 0, ns1655x1Num, ns1655x1Resources },
    { "ns16550", 1, VXB_BUSID_PLB, 0, ns1655x2Num, ns1655x2Resources },
    { "ns16550", 2, VXB_BUSID_PLB, 0, ns1655x3Num, ns1655x3Resources },
    { "ns16550", 3, VXB_BUSID_PLB, 0, ns1655x4Num, ns1655x4Resources },
#else
# ifdef INCLUDE_AMP_CPU_00
    { "ns16550", 0, VXB_BUSID_PLB, 0, ns1655x1Num, ns1655x1Resources },
    { "ns16550", 1, VXB_BUSID_PLB, 0, ns1655x2Num, ns1655x2Resources },
# else /* INCLUDE_AMP_CPU_00 */
#  ifdef USE_EXTRA_DUARTS
    #if   defined (INCLUDE_AMP_CPU_01)
    { "ns16550", 0, VXB_BUSID_PLB, 0, ns1655x3Num, ns1655x3Resources },
    { "ns16550", 1, VXB_BUSID_PLB, 0, ns1655x4Num, ns1655x4Resources },
    #elif defined (INCLUDE_AMP_CPU_02)
    { "ns16550", 0, VXB_BUSID_PLB, 0, ns1655x5Num, ns1655x5Resources },
    { "ns16550", 1, VXB_BUSID_PLB, 0, ns1655x6Num, ns1655x6Resources },
    #elif defined (INCLUDE_AMP_CPU_03)
    { "ns16550", 0, VXB_BUSID_PLB, 0, ns1655x7Num, ns1655x7Resources },
    { "ns16550", 1, VXB_BUSID_PLB, 0, ns1655x8Num, ns1655x8Resources },
    #elif defined (INCLUDE_AMP_CPU_04)
    { "ns16550", 0, VXB_BUSID_PLB, 0, ns1655x9Num, ns1655x9Resources },
    { "ns16550", 1, VXB_BUSID_PLB, 0, ns1655x10Num, ns1655x10Resources },
    #elif defined (INCLUDE_AMP_CPU_05)
    { "ns16550", 0, VXB_BUSID_PLB, 0, ns1655x11Num, ns1655x11Resources },
    { "ns16550", 1, VXB_BUSID_PLB, 0, ns1655x12Num, ns1655x12Resources },
    #elif defined (INCLUDE_AMP_CPU_06)
    { "ns16550", 0, VXB_BUSID_PLB, 0, ns1655x13Num, ns1655x13Resources },
    { "ns16550", 1, VXB_BUSID_PLB, 0, ns1655x14Num, ns1655x14Resources },
    #elif defined (INCLUDE_AMP_CPU_07)
    { "ns16550", 0, VXB_BUSID_PLB, 0, ns1655x15Num, ns1655x15Resources },
    { "ns16550", 1, VXB_BUSID_PLB, 0, ns1655x16Num, ns1655x16Resources },
    #endif
#  else /* USE_EXTRA_DUARTS */
    { "ns16550", 0, VXB_BUSID_PLB, 0, ns1655x3Num, ns1655x3Resources },
    { "ns16550", 1, VXB_BUSID_PLB, 0, ns1655x4Num, ns1655x4Resources },
#  endif /* USE_EXTRA_DUARTS */
# endif /* INCLUDE_AMP_CPU_00 */
#endif /* defined (INCLUDE_MSD) || !defined (INCLUDE_AMP) */
#endif /* DRV_SIO_NS16550 */

#ifdef DRV_RESOURCE_FSLGPIO
    { "fslGpio", 0, VXB_BUSID_PLB,0, gpio0Num, gpio0Resources },    
#endif

#ifdef DRV_RESOURCE_QORIQLAW
    { "QorIQLaw", 0, VXB_BUSID_PLB,0, law0Num, law0Resources },    
#endif

#ifdef DRV_RESOURCE_QORIQBMAN
    { "QorIQBman", 0, VXB_BUSID_PLB,0, bman0Num, bman0Resources },    
#endif

#ifdef DRV_RESOURCE_QORIQQMAN
    { "QorIQQman", 0, VXB_BUSID_PLB,0, qman0Num, qman0Resources },    
#endif

#ifdef DRV_RESOURCE_QORIQFMAN
    { "QorIQFman", 0, VXB_BUSID_PLB,0, fman0Num, fman0Resources },    
    { "QorIQFman", 1, VXB_BUSID_PLB,0, fman1Num, fman1Resources },    
#endif

/*
 * Keep PCIe briges on AMP instance 0 by default. They can
 * be assigined to individual codes if needed.
 */

#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_00)

#ifdef DRV_PCIBUS_QORIQ_PCIEX
    { "QorIQPciEx", 0, VXB_BUSID_PLB,0, pci0Num, pci0Resources },
#ifdef SECONDARY_PCIE_BRIDGE
    { "QorIQPciEx", 1, VXB_BUSID_PLB,0, pci1Num, pci1Resources },
#endif /* SECONDARY_PCIE_BRIDGE */
    { "QorIQPciEx", 2, VXB_BUSID_PLB,0, pci2Num, pci2Resources },
#endif /* DRV_PCIBUS_QORIQ_PCIEX */

#endif

/*
 * The P4080DS 'out of box' configuration has only 5 of the 8
 * DTSEC ports available.
 */

#ifdef DRV_VXBEND_DTSEC
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_00)
    { "dtsec", 0, VXB_BUSID_PLB, 0, dtsec0Num, dtsec0Resources },
#endif
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_01)
    { "dtsec", 1, VXB_BUSID_PLB, 0, dtsec1Num, dtsec1Resources },
#endif
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_02)
    { "dtsec", 2, VXB_BUSID_PLB, 0, dtsec2Num, dtsec2Resources },
#endif
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_03)
    { "dtsec", 3, VXB_BUSID_PLB, 0, dtsec3Num, dtsec3Resources },
#endif
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_04)
    { "dtsec", 4, VXB_BUSID_PLB, 0, dtsec4Num, dtsec4Resources },
#endif

#ifdef SECONDARY_SGMII_RISER
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_05)
    { "dtsec", 5, VXB_BUSID_PLB, 0, dtsec5Num, dtsec5Resources },
#endif
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_06)
    { "dtsec", 6, VXB_BUSID_PLB, 0, dtsec6Num, dtsec6Resources },
#endif
#if !defined (INCLUDE_AMP) || defined (INCLUDE_AMP_CPU_07)
    { "dtsec", 7, VXB_BUSID_PLB, 0, dtsec7Num, dtsec7Resources },
#endif
#endif

#endif

#ifdef INCLUDE_DUMMY_MDIO
    { "dummyMdio", 0, VXB_BUSID_PLB,0, dummyMdio0Num, dummyMdio0Resources },    
#endif

#ifdef DRV_MII_DTSEC_MDIO
    { "dtsecMdio", 0, VXB_BUSID_PLB,0, dtsecMdio0Num, dtsecMdio0Resources },    
#endif

#ifdef DRV_VXBEND_TGEC
   { "tgec", 1, VXB_BUSID_PLB, 0, tgec1Num, tgec1Resources },
#endif

#ifdef DRV_MII_TGEC_MDIO
    { "tgecMdio", 0, VXB_BUSID_PLB,0, tgecMdio0Num, tgecMdio0Resources },    
#endif

#ifdef DRV_TIMER_M85XX
    {"m85xxTimerDev", 0, VXB_BUSID_PLB, 0, m85xxTimerNum, m85xxTimerResources},
#endif /* DRV_TIMER_M85XX */

#ifdef INCLUDE_MOB_PLB_0
    { "mob8572", 0, VXB_BUSID_PLB, 0,
        NELEMENTS(mob8572Res_0), mob8572Res_0 },
    { "mobVx8572", 0, VXB_BUSID_VIRTUAL, 0,
        NELEMENTS(mobVx8572Res0_0), mobVx8572Res0_0 },
    { "mobVx8572", 1, VXB_BUSID_VIRTUAL, 0,
        NELEMENTS(mobVx8572Res1_0), mobVx8572Res1_0 },
#if (MOB_PLB_MAX_NODES_0 > 2)
    { "mobVx8572", 2, VXB_BUSID_VIRTUAL, 0,
        NELEMENTS(mobVx8572Res2_0), mobVx8572Res2_0 },
#endif
#if (MOB_PLB_MAX_NODES_0 > 3)
    { "mobVx8572", 3, VXB_BUSID_VIRTUAL, 0,
        NELEMENTS(mobVx8572Res3_0), mobVx8572Res3_0 },
#endif
#if (MOB_PLB_MAX_NODES_0 > 4)
    { "mobVx8572", 4, VXB_BUSID_VIRTUAL, 0,
        NELEMENTS(mobVx8572Res4_0), mobVx8572Res4_0 },
#endif
#if (MOB_PLB_MAX_NODES_0 > 5)
    { "mobVx8572", 5, VXB_BUSID_VIRTUAL, 0,
        NELEMENTS(mobVx8572Res5_0), mobVx8572Res5_0 },
#endif
#if (MOB_PLB_MAX_NODES_0 > 6)
    { "mobVx8572", 6, VXB_BUSID_VIRTUAL, 0,
        NELEMENTS(mobVx8572Res6_0), mobVx8572Res6_0 },
#endif
#if (MOB_PLB_MAX_NODES_0 > 7)
    { "mobVx8572", 7, VXB_BUSID_VIRTUAL, 0,
        NELEMENTS(mobVx8572Res7_0), mobVx8572Res7_0 },
#endif
#endif /* INCLUDE_MOB_PLB_0 */
#ifdef INCLUDE_MOB_PLB_1
    { "mob8572", 1, VXB_BUSID_PLB, 0,
        NELEMENTS(mob8572Res_1), mob8572Res_1 },
    { "mobVx8572", 8, VXB_BUSID_VIRTUAL, 1,
        NELEMENTS(mobVx8572Res0_1), mobVx8572Res0_1 },
    { "mobVx8572", 9, VXB_BUSID_VIRTUAL, 1,
        NELEMENTS(mobVx8572Res1_1), mobVx8572Res1_1 },
#if (MOB_PLB_MAX_NODES_1 > 2)
    { "mobVx8572", 10, VXB_BUSID_VIRTUAL, 1,
        NELEMENTS(mobVx8572Res2_1), mobVx8572Res2_1 },
#endif
#if (MOB_PLB_MAX_NODES_1 > 3)
    { "mobVx8572", 11, VXB_BUSID_VIRTUAL, 1,
        NELEMENTS(mobVx8572Res3_1), mobVx8572Res3_1 },
#endif
#if (MOB_PLB_MAX_NODES_1 > 4)
    { "mobVx8572", 12, VXB_BUSID_VIRTUAL, 1,
        NELEMENTS(mobVx8572Res4_1), mobVx8572Res4_1 },
#endif
#if (MOB_PLB_MAX_NODES_1 > 5)
    { "mobVx8572", 13, VXB_BUSID_VIRTUAL, 1,
        NELEMENTS(mobVx8572Res5_1), mobVx8572Res5_1 },
#endif
#if (MOB_PLB_MAX_NODES_1 > 6)
    { "mobVx8572", 14, VXB_BUSID_VIRTUAL, 1,
        NELEMENTS(mobVx8572Res6_1), mobVx8572Res6_1 },
#endif
#if (MOB_PLB_MAX_NODES_1 > 7)
    { "mobVx8572", 15, VXB_BUSID_VIRTUAL, 1,
        NELEMENTS(mobVx8572Res7_1), mobVx8572Res7_1 },
#endif
#endif /* INCLUDE_MOB_PLB_1 */

#ifdef DRV_TIMER_OPENPIC
    { "openPicTimer", 0,   VXB_BUSID_PLB,0, openPicTimerDevAnum, openPicTimerDevAresources },
#ifndef SIMICS
    { "openPicTimer", 1,   VXB_BUSID_PLB,0, openPicTimerDevBnum, openPicTimerDevBresources },
#endif /* SIMICS */
#endif /* DRV_TIMER_OPENPIC */

#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci", 0, VXB_BUSID_PLB, 0, usbEhci0Num,    usbEhci0Resources },
#endif  /* INCLUDE_EHCI */

#ifdef DRV_DMA_FSL
    { "fslDma",     0,  VXB_BUSID_PLB,      0,  fslDma1Num,     fslDma1Resources },
    { "fslDma",     1,  VXB_BUSID_PLB,      0,  fslDma2Num,     fslDma2Resources },
#endif /* DRV_DMA_FSL */

#ifdef DRV_STORAGE_SDHC
    { "sdhci",      0,  VXB_BUSID_PLB,      0,  fslSdhcNum,     fslSdhcResources },
#endif  /* DRV_STORAGE_SDHC */

};
const int hcfDeviceNum = NELEMENTS(hcfDeviceList);

VXB_INST_PARAM_OVERRIDE sysInstParamTable[] =
    {
#ifdef INCLUDE_EHCI
    { "vxbPlbUsbEhci", 0, "hasEmbeddedTT", VXB_PARAM_INT32,   { (void *)TRUE } },
    { "vxbPlbUsbEhci", 0, "postResetHook", VXB_PARAM_FUNCPTR, { (void *)ehci0PostResetHook } },
#endif  /* INCLUDE_EHCI */
    { NULL, 0, NULL, VXB_PARAM_END_OF_LIST, {(void *)0} }
    };
