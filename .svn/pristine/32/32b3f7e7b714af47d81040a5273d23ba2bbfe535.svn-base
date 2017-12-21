/* vxbNs16550Sio.c - National Semiconductor 16550 Serial Driver */

/*
 * Copyright (c) 1984-2009 Wind River Systems, Inc. All rights are reserved.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
03a,13apr09,h_k  updated for kprintf() support in SMP. (CQ:158523)
02z,10dec08,h_k  added more information in the API reference manual.
02y,03sep08,jpb  Renamed VSB header file
02x,10jul08,tor  update version
02w,18jun08,h_k  removed pAccess
                 removed redundant zero clear after hwMemAlloc().
02v,23jun08,l_g  Add ierInit and handle support for PXA310 UART
02u,18jun08,jpb  Renamed _WRS_VX_SMP to _WRS_CONFIG_SMP.  Added include path
                 for kernel configurations options set in vsb.
02t,21may08,tor  support OXPCIe952
02s,16may08,tor  support OXCB950 and OXmPCI954
02r,01apr08,tor  additional meta-data comments
02q,28feb08,tor  resource documentation
02p,28feb08,l_g  Add ierInit and handle support for PXA310 UART  
02o,20sep07,tor  VXB_VERSION_3
02n,13aug07,h_k  updated for new reg access methods.
                 added DMA mode 1 support for faster transmissions. (CQ:101435)
02m,09aug07,h_k  added fifoLen parameter. (CQ:100695)
02l,23may07,h_k  changed to binary release.
02k,09may07,tor  remove undef LOCAL
02j,09apr07,h_k  added TX FIFO support.
                 added malta BSP support.
02i,29mar07,h_k  replaced spinlockIsrSingleTake with spinlockIsrTake.
02h,21mar07,sru  pQueue changed to queueId
02g,26feb07,h_k  cleaned up a compiler warning.
02f,02feb07,wap  defect 83778: serial initialization
02e,03jan07,h_k  updated for SMP.
02d,11sep06,tor  Correct typo in CompUSA deviceID
02c,07sep06,pdg  Fix for error in targetServer connection(FixWIND00064640)
02b,25aug06,pdg  changed 'sysDelay' to 'vxbDelay'
02a,28Jul06,tor  Add EasySync device support, non-vxbus compatability
01z,21jul06,h_k  added the case regDelta is not 1 byte.
01y,25may06,dtr  Add extra call out to BSP for divisor calc.
01x,15mar06,mdo  Fix possible memory leak
01w,31jan06,pdg  updated vxbus version
01v,14dec05,pdg  removed bsp reference for clock frequency
01u,29sep05,mdo  Fix gnu warnings
01t,28sep05,pdg  Support for changes to access functions(SPR #112950)
01s,12sep05,mdo  SPR#111986 - vxBus interrupt mode fix.
01r,01sep05,mdo  Add vxb prefix
01q,11aug05,mdo  Cleanup
01p,02Aug05,mdo  add new access methods
01o,07apr05,tor  port ns16550Sio driver to bus subsystem
01n,03jun04,pch  SPR 74889: fix hardware flow control
01m,24apr02,pmr  SPR 75161: returning int from ns16550TxStartup() as required.
01l,03apr02,jnz  fixed bug at ns16550SioDrvFuncs declaration.
01k,31oct01,mdg  Corrected transmit interrupt race condition found on
		 on MIPS Malta board with RM5261 processor. (SPR# 71830)
01j,14mar01,rcs  corrected baud rate divisor calculation formula. (SPR# 63899)
01i,17sep97,dat  fixed merge problems that caused baud rate setting to fail.
01h,06mar97,dat  SPR 7899, max baud rate set to 115200.
01g,08may97,db   added hardware options and modem control(SPRs #7570, #7082).
01f,18dec95,myz  added case IIR_TIMEOUT in ns16550Int routine.
01e,28nov95,myz  fixed bugs to work at 19200 baud or above with heavy traffic.
01d,09nov95,jdi  doc: style cleanup.
01c,02nov95,myz  undo 01b fix
01b,02nov95,p_m  added test for 960CA and 960JX around access to lcr field
		 in order to compile on all architectures.
01a,24oct95,myz  written from ns16550Serial.c.
串口共享功能：
   通过组合按键选择指定CPU的串口：
   “esc+0"    CPUA
   “esc+1"    CPUB
   “esc+2"    CPUC
   “esc+3"    CPUD
 */

/* includes */

#include <vxWorks.h>
#include <vsbConfig.h>
#include <intLib.h>
#include <errnoLib.h>
#include <errno.h>
#include <sioLib.h>
#include <ioLib.h>

#include <stdio.h>
#include <string.h>

#ifdef	_WRS_CONFIG_SMP
#include <private/spinLockLibP.h>
#endif	/* _WRS_CONFIG_SMP */

#include <hwif/util/hwMemLib.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/vxbus/vxbPciLib.h>
#include <hwif/vxbus/hwConf.h>

#include <drv/pci/pciConfigLib.h>

#include "../h/util/sioChanUtil.h"
#include "../h/sio/vxbNs16550Sio.h"

DEVMETHOD_DEF(sioEnable, "board-specific serial I/O enable");
DEVMETHOD_DEF(sioIntEnable, "board-specific serial interrupt enable");


/* debug */
#ifdef NS16550_DEBUG_ON

/*
 * NOTE: printf() and logMsg() debugging cannot be used before the
 * call to ns16550vxbInstConnect().  Any use before that may cause the
 * system to crash before finishing the boot process.  To debug
 * the probe routine, init routine, and init2 routine, either
 * use a hardware debugger, move the specified functionality to
 * ns16550vxbInstConnect(), or manually call the routine after boot.
 */
int ns16550vxbDebugLevel = 0;

#ifndef NS16550_DBG_MSG
#define NS16550_DBG_MSG(level,fmt,a,b,c,d,e,f) if ( ns16550vxbDebugLevel >= level ) logMsg(fmt,a,b,c,d,e,f)
#endif /* NS16550_DBG_MSG */

#else /* NS16550_DEBUG_ON */

#define NS16550_DBG_MSG(level,fmt,a,b,c,d,e,f)

#endif /* NS16550_DEBUG_ON */
/* typedefs */

LOCAL BOOL ns16550vxbProbe(struct vxbDev * pDev);
LOCAL void ns16550vxbInstInit(struct vxbDev * pDev);
LOCAL void ns16550vxbInstInit2(struct vxbDev * pDev);
LOCAL void ns16550vxbInstConnect(struct vxbDev * pDev);
LOCAL void ns16550vxbSioChanGet(VXB_DEVICE_ID pDev, void * pArg);
LOCAL void ns16550vxbSioChanConnect(VXB_DEVICE_ID pDev, void * pArg);
void ns16550vxbSioShow(VXB_DEVICE_ID pInst, int verboseLevel);

LOCAL struct drvBusFuncs ns16550vxbFuncs =
{
		ns16550vxbInstInit,        /* devInstanceInit */
		ns16550vxbInstInit2,       /* devInstanceInit2 */
		ns16550vxbInstConnect      /* devConnect */
};

/*
 * deviceDesc {
 * supported PCI devices:
 * }
 */

LOCAL struct vxbPciID pciDevIDList[] =
{
		/* { devID, vendID } */

#define UART_VENDOR_OXFORDSEMI		0x1415
#define UART_VENDOR_AXXON		0x1b1f

		/*
		 * Axxon LF659KB
		 * PCI High-Speed 1-port serial PCIe adapter
		 *
		 * deviceDesc {
		 *	Axxon LF659KB 1-port serial PCIe adapter
		 *		(Oxford Semiconductor OXCB950 chip)
		 *
		 *	Axxon LF658KB 2-port serial PCIe I/O card
		 *		(dual Oxford Semiconductor OXCB950 chips)
		 * }
		 */

#define UART_DEVID_AXXON_LF659KB		0x950b
		{ UART_DEVID_AXXON_LF659KB, UART_VENDOR_OXFORDSEMI},
		{ UART_DEVID_AXXON_LF659KB, UART_VENDOR_AXXON},

		/*
		 * deviceDesc {
		 *	Axxon LF653KB 4-port serial PCIe I/O card
		 *		(Oxford Semiconductor OXmPCI954 chip)
		 * }
		 */

#define UART_DEVID_AXXON_LF653KB_FUNC0  0x9501  
		{ UART_DEVID_AXXON_LF653KB_FUNC0, UART_VENDOR_OXFORDSEMI },
		{ UART_DEVID_AXXON_LF653KB_FUNC0, UART_VENDOR_AXXON },

		/*
		 * The OX16PCI954 device provides two functions, but none of the serial
		 * ports is visible on func1.  We could claim it and zero all
		 * pRegBase[] values, but we leave it alone in case some
		 * other driver claims the parallel port.
		 */

#define UART_DEVID_AXXON_LF653KB_FUNC1  0x9500  

		/*
		 * deviceDesc {
		 *	Axxon LF659KB 8-port serial PCIe I/O card
		 *		(dual Oxford Semiconductor OXmPCI954 chips)
		 * }
		 */

#define UART_DEVID_AXXON_LF662KB_FUNC0  0x9501
#define UART_DEVID_AXXON_LF662KB_FUNC1  0x9511
		{ UART_DEVID_AXXON_LF662KB_FUNC0, UART_VENDOR_OXFORDSEMI },
		{ UART_DEVID_AXXON_LF662KB_FUNC1, UART_VENDOR_OXFORDSEMI },
		{ UART_DEVID_AXXON_LF662KB_FUNC0, UART_VENDOR_AXXON },
		{ UART_DEVID_AXXON_LF662KB_FUNC1, UART_VENDOR_AXXON },

		/*
		 * The Axxon LF769KB encodes configuration information
		 * in the device ID.  The encoding is:
		 * ( 0xC10N | 0x00M0 | 0x000G )
		 * where
		 *	N encodes the function number in the lowest two bits
		 *	M represents the mode in the lowest three bits
		 *	G represents the GPIO_EN bit at poxition 0b0100
		 *
		 * The configuration can be either 1S1P+GPIO or 2S+GPIO.
		 *
		 * GPIO_EN  MODE  func0    func1   func2
		 *
		 *       1   000  PPORT    legacy   GPIO (1S1P+GPIO)
		 *                         UART
		 *
		 *       1   100  legacy   legacy   GPIO (2S+GPIO)
		 *                UART     UART
		 *
		 *       0   101  native   N/A      N/A	 (2SN)
		 *                UARTs(2)
		 *
		 *       1   101  GPIO     native   N/A  (2SNG)
		 *                         UARTs(2)
		 *
		 * The default configuration is 1S1P, with M=0b000 and
		 * G = 0b1.  This is the only supported configuration
		 * at this time.  The parallel port and GPIO are not
		 * claimed by this driver.
		 *
		 * See the Oxford Semiconductor OXPCIe952 data sheet
		 * for more information.
		 * The configuration can be changed by using the Oxide
		 * utility available from Axxon.
		 *
		 * deviceDesc {
		 *	Axxon LF769KB 1S1P PCIe I/O card
		 *		(single serial port only)
		 *		(Oxford Semiconductor OXPCIe952 chip)
		 * }
		 */

#define UART_DEVID_AXXON_LF769KB_1S1P_FUNC1     0xC105
		{ UART_DEVID_AXXON_LF769KB_1S1P_FUNC1, UART_VENDOR_OXFORDSEMI },
		{ UART_DEVID_AXXON_LF769KB_1S1P_FUNC1, UART_VENDOR_AXXON },
		/* parallel and GPIO not claimed */
#define UART_DEVID_AXXON_LF769KB_1S1P_FUNC0     0xC104
#define UART_DEVID_AXXON_LF769KB_1S1P_FUNC2     0xC106

		/*
		 * claim the LF769KB 2S configuration.
		 * although *NOT SUPPORTED*, it is expected to work.
		 */
#define UART_DEVID_AXXON_LF769KB_2S_FUNC0	0xC114
#define UART_DEVID_AXXON_LF769KB_2S_FUNC1	0xC115
		{ UART_DEVID_AXXON_LF769KB_2S_FUNC0, UART_VENDOR_OXFORDSEMI },
		{ UART_DEVID_AXXON_LF769KB_2S_FUNC0, UART_VENDOR_AXXON },
		{ UART_DEVID_AXXON_LF769KB_2S_FUNC1, UART_VENDOR_OXFORDSEMI },
		{ UART_DEVID_AXXON_LF769KB_2S_FUNC1, UART_VENDOR_AXXON },

		/* LF769KB 2S native not claimed */
#define UART_DEVID_AXXON_LF769KB_2SN		0xC154
#define UART_DEVID_AXXON_LF769KB_2SNG_FUNC1	0xC151

		/* Board: IC0607KB - Axxon (softio)
		 * PCI High-Speed 2-Serial and 1-Parallel Port Adapter
		 * Multi-function device: Function 0 = UARTs
		 *                        Function 1 = 8-bit local bus or parallel port
		 *
		 * Controller: Oxford Semiconductor
		 *   Board controller has OXmPCI952 silkscreened but is OXmPCI954.
		 *
		 * Configuration Notes:
		 *   Has 4-UARTs; only 2 are pinned.
		 *   Ships with UARTs in Common IO Space
		 *
		 * deviceDesc {
		 *	Axxon/SoftIO IC0607KB, PCI High-Speed 2-Serial and 1-Parallel Port Adapter
		 *		(Oxford Semiconductor OX16PCI954 chip)
		 * }
		 */

#define UART_DEVID_SOFTIO_IC0607KB      0x950A
		{ UART_DEVID_SOFTIO_IC0607KB, UART_VENDOR_OXFORDSEMI},
		{ UART_DEVID_SOFTIO_IC0607KB, UART_VENDOR_AXXON},

#define UART_VENDOR_EASYSYNC            0x14d2

		/*
		 * Board: EasySYNC ES-D-1001
		 * PCI high-speed serial port, for 3.3V or 5V slot
		 *   VX COM
		 *   BSP9216U
		 *   F1MK4-001
		 *   0546
		 *
		 *  chan: 0, 1, 2, 3 created, 1 is real
		 *
		 * deviceDesc {
		 *	EasySYNC ES-D-1001, PCI high-speed serial port, for 3.3V or 5V slot
		 * }
		 */

#define UART_EASYSYNC_ESD1001           0x8010
		{ UART_EASYSYNC_ESD1001, UART_VENDOR_EASYSYNC },

		/*
		 * Board: EasySYNC ES-D-1002
		 *
		 * chan: 0, 1, 2, 3 created, 1 is upper, 2 is lower
		 *
		 * deviceDesc {
		 *	EasySYNC ES-D-1002, PCI high-speed 2 serial ports, for 3.3V or 5V slot
		 * }
		 */

#define UART_EASYSYNC_ESD1002           0x8020
		{ UART_EASYSYNC_ESD1002, UART_VENDOR_EASYSYNC },

		/*
		 * Board: EasySYNC ES-D-1004
		 * Label on board: "uPCI-400HS"
		 */

		/* we don't currently support the ESD1004 */
#define UART_EASYSYNC_ESD1004           0xa003
#define UART_EASYSYNC_ESD1004_1         0xffff
		{ UART_EASYSYNC_ESD1004, UART_VENDOR_EASYSYNC },    /* function 0 */
		{ UART_EASYSYNC_ESD1004_1, UART_VENDOR_EASYSYNC },  /* function 1 */

		/* Board: CompUSA
		 * PCI High-Speed 2-Serial and 1-Parallel Port Adapter
		 * UPC# 0-49696-10155-4
		 * SKU# 271773
		 *
		 * CHIP:    NetMos technology
		 *      Nm9825CV
		 *      FHFFT-000
		 *      0345
		 *
		 * deviceDesc {
		 *	CompUSA PCI High-Speed 2-Serial and 1-Parallel Port Adapter
		 * }
		 */

#define UART_VENDOR_COMPUSA     0x9710
#define UART_DEVID_COMPUSA_SIO  0x9835
		{ UART_DEVID_COMPUSA_SIO, UART_VENDOR_COMPUSA},


		/* add additional DevVend IDs here */
};


LOCAL struct vxbPciRegister ns16550vxbPciDevRegistration =
{
		/* b */
		{
				NULL,               /* pNext */
				VXB_DEVID_DEVICE,   /* devID */
				VXB_BUSID_PCI,      /* busID = PCI */
				VXB_VER_4_0_0,      /* vxbVersion */
				"ns16550",          /* drvName */
				&ns16550vxbFuncs,   /* pDrvBusFuncs */
				NULL,               /* pMethods */
				ns16550vxbProbe     /* devProbe */
		},
		NELEMENTS(pciDevIDList),
		&pciDevIDList[0],
};

/*
 * deviceDesc {
 *
 * supported PLB devices:
 *
 *	Many soldered-on ns16550 compatible devices
 * }
 */

LOCAL struct vxbPlbRegister ns16550vxbDevRegistration =
{
		/* b */
		{
				(struct vxbDevRegInfo *)&ns16550vxbPciDevRegistration, /* pNext */
				VXB_DEVID_DEVICE,     /* devID */
				VXB_BUSID_PLB,        /* busID = PLB */
				VXB_VER_4_0_0,        /* vxbVersion */
				"ns16550",            /* drvName */
				&ns16550vxbFuncs,     /* pDrvBusFuncs */
				NULL,                 /* pMethods */
				ns16550vxbProbe       /* devProbe */
		},
};

/* defines */

#define PCI_CLK_FREQ 1843200

#define DEFAULT_NS16550_COMM_PARAMS \
		( \
				CHAR_LEN_8 | \
				PARITY_NONE | \
				ONE_STOP \
		)

/*
 * access method definitions:
 *
 * We use two different sets of macros to access registers, and either can
 * be overridden by the BSP.  The first set is for code which users may want
 * to have inline in order to increase performance.  The second set is for
 * code which users may not want to have inline.  The default configuration,
 * if no optimized versions are provided by the BSP, is to make one or more
 * function calls for each access.
 *
 * For this driver, or for any other slow-speed device driver, these
 * should not be overridden by the BSP.  However, they are included here
 * for completeness.  These should normally be defined in the driver's
 * header file.
 */

#define _D_(i)   NS16550SIO_ ## i

#include "../h/vxbus/vxbAccess.h"

/* locals */

IMPORT char sioChanConnect_desc[];
METHOD_DECL(busDevShow);

LOCAL device_method_t ns16550vxb_methods[] =
{
		DEVMETHOD(sioChanGet, ns16550vxbSioChanGet),
		DEVMETHOD(sioChanConnect, ns16550vxbSioChanConnect),

#ifdef NS16550_DEBUG_ON
		DEVMETHOD(busDevShow, ns16550vxbSioShow),
#endif /* NS16550_DEBUG_ON */

		{ 0, 0}
};

/* forward declarations */

IMPORT int sioNextChannelNumberAssign(void);
IMPORT void vxbUsDelay(int	counter);
LOCAL void ns16550vxbDevInit (NS16550VXB_CHAN * pChan);
LOCAL void ns16550vxbInt (VXB_DEVICE_ID pDev);

/* ------------------------------------------------------------ */

/*
DESCRIPTION

This is the VxBus driver for the NS16550 (or compatible) DUART.
In general, this device includes two universal asynchronous
receiver/transmitters, a baud rate generator, and a complete
modem control capability.

A NS16550VXB_CHAN structure is used to describe the
serial channel. This data structure is defined in
${WIND_BASE}/target/src/hwif/h/sio/vxbNs16550Sio.h.

Only asynchronous serial operation is supported by this driver.
The default serial settings are 8 data bits, 1 stop bit, no parity,
9600 baud, and software flow control.

TARGET-SPECIFIC PARAMETERS

The parameters are provided through `ns16550' registration defined in
hwconf.c in the target BSP for PLB device.

\is
\i <regBase>
NS16550 Sio register base address for index 0.
The parameter type is HCF_RES_INT.
If not specified, 0 will be set in the register base address and it will not
initialize the NS16550 device assined to index 0.

\i <regBaseX>
NS16550 Sio register base address for index X. (X can be 1 - 9)
The parameter type is HCF_RES_INT.
If not specified, 0 will be set in the register base address and it will not
initialize the NS16550 device assined to index X.

\i <irq>
Interrupt vector number (ivec) for the serial interrupt source.
The parameter type is HCF_RES_INT.
Missing parameter will cause failure to connect interrupt.

\i <irqLevel>
Interrupt number (inum) for the serial interrupt source.
The parameter type is HCF_RES_INT.
Missing parameter will cause failure to enable interrupt.

\i <clkFreq>
The frequency of the oscillator in Hz if the parameter type is HCF_RES_INT.
The function pointer to get the frequency of the oscillator in Hz if the
parameter type is HCF_RES_ADDR.
If not specified, 0 Hz will be used as the frequency.

\i <regInterval>
The amount of address space between sucessive 8-bit registers.
If not specified, 1 byte will be used as the register interval.
The parameter type is HCF_RES_INT.

\i <fifoLen>
The depth of the FIFO on this device.
If not specified and the device supports FIFO, the FIFO depth is set 16 bytes.
If not specified and the device doesn't support FIFO, the FIFO depth is set 1
byte.
The parameter type is HCF_RES_INT.

\i <divisorCalc>
The function pointer to calculate the divisor used for baud rate calculations.
If not specified, the default baud rate calculation provided by this driver
is used.
The parameter type is HCF_RES_ADDR.

\i <ierInit>
The IER bits which will be ored to the initial value at bootup and the
adjusted value is used as the initial value.
The parameter type is HCF_RES_INT.

\i <handle>
The adrress of the BSP specific register access routine for the BSP which
requires a particular register access such as 32-bit register access for the
16-bit registers on the NS16550 Sio controller.
The parameter type is HCF_RES_ADDR.

\ie

By convention all the BSP-specific device parameters are registered in
a file called hwconf.c, which is #include'ed by sysLib.c. The following is
an example for configuring two SIO units in hpcNet8641 BSP:

\cs
LOCAL struct hcfResource ns1655x1Resources[] = {
    { VXB_REG_BASE,  HCF_RES_INT,  {(void *)COM1_ADR} },
    { "irq",         HCF_RES_INT,  {(void *)EPIC_DUART_INT_VEC} },
    { "regInterval", HCF_RES_INT,  {(void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT,  {(void *)EPIC_DUART_INT_VEC} },
    { "divsorCalc",  HCF_RES_ADDR, {(void *)&divisorGet} },
    { VXB_CLK_FREQ,     HCF_RES_ADDR, {(void *)&sysClkFreqGet} }
};
#define ns1655x1Num NELEMENTS(ns1655x1Resources)

LOCAL const struct hcfResource ns1655x2Resources[] = {
    { VXB_REG_BASE,  HCF_RES_INT,  {(void *)COM2_ADR} },
    { "irq",         HCF_RES_INT,  {(void *)EPIC_DUART2_INT_VEC} },
    { "regInterval", HCF_RES_INT,  {(void *)DUART_REG_ADDR_INTERVAL} },
    { "irqLevel",    HCF_RES_INT,  {(void *)EPIC_DUART_INT_VEC} },
    { "divsorCalc",  HCF_RES_ADDR, {(void *)&divisorGet} },
    { VXB_CLK_FREQ,     HCF_RES_ADDR, {(void *)&sysClkFreqGet} }
};
#define ns1655x2Num NELEMENTS(ns1655x2Resources)


const struct hcfDevice hcfDeviceList[] = {

        ...


    { "ns16550", 0, VXB_BUSID_PLB, 0, ns1655x1Num, ns1655x1Resources },
    { "ns16550", 1, VXB_BUSID_PLB, 0, ns1655x2Num, ns1655x2Resources },

        ...

};
\ce

USAGE

This driver is largely self-contained.  In general, the BSP does
not need to provide any code to support this driver.  If the
device is located on PLB, then the BSP does need to provide an
entry describing the device in hwconf.c.  There are a couple of
cases where the BSP may provide code to support the 16550 driver.
These are the integer clkFreq resource entry, when specified
with an HCF_RES_ADDR data type, and the divisorCalc resource.
For information about these resources, see the comments associated
with each.

This driver handles setting of hardware options such as parity(odd,
even) and number of data bits(5, 6, 7, 8). Hardware flow control
is provided with the handshaking RTS/CTS. The function HUPCL(hang
up on last close) is available.  When hardware flow control is
enabled, the signals RTS and DTR are set TRUE and remain set
until a HUPCL is performed.

To connect all the serial channels, call sysSerialConnectAll() in
SIO Channel Utility 'INCLUDE_SIO_UTILS', from sysHwInit2() in your BSP.

\cs
void sysHwInit2 (void)
    {

        ...

    /@ hardware interface Post-Kernel Initialization @/

    vxbDevInit ();

        ...

    #ifdef INCLUDE_SIO_UTILS
    /@ connect all SIO_CHAN devices @/

    sysSerialConnectAll ();
    #endif

        ...

    }
\ce

The SIO Channel Utility requires BSP-supplied serial channels support routine,
bspSerialChanGet (), in your BSP.
If there is no serial channel reserved by the BSP, such as the one assigned to
non-VxBus SIO driver, ERROR should be always returned.
Otherwise, return the SIO_CHAN structure pointer for the channel which is
reserved by the BSP and the SIO Channel Utility will skip the channel to be
connected.

\cs
#ifdef INCLUDE_SIO_UTILS
/@

  bspSerialChanGet - get the SIO_CHAN device associated with a serial channel

  The sysSerialChanGet() routine returns a pointer to the SIO_CHAN
  device associated with a specified serial channel. It is called
  by usrRoot() to obtain pointers when creating the system serial
  devices, `/tyCo/x'. It is also used by the WDB agent to locate its
  serial channel.  The VxBus function requires that the BSP provide a
  function named bspSerialChanGet () to provide the information about
  any non-VxBus serial channels, provided by the BSP.  As this BSP
  does not support non-VxBus serial channels, this routine always
  returns ERROR.

  RETURNS: ERROR, always

@/

SIO_CHAN * bspSerialChanGet
    (
    int channel         /@ serial channel @/
    )
    {
    return ((SIO_CHAN *) ERROR);
    }
#endif
\ce

These are the replacement of the sysSerialChanGet(), typically in
sysSerial.c file in the BSP, supported for Non-VxBus serial drivers.
If you are migrating the BSP to VxBus, move all the initialization code to
the VxBus SIO driver and remove the sysSerial.c in your BSP.
The BSP specifc initializations done by the sysSerialHwInit() and
sysSerialHwInit2() can be moved to templateSioInstInit () and
templateSioChannelConnect (). If there are BSP specific information this
driver needs to get, create the hardware configuration resource entries
and pass them through the hcfResource structure in hwconf.c.
And the driver will have the information by devResourceGet() with the
matching resource entries.

CONFIGURATION
To use the NS16550 SIO driver, configure VxWorks with the
DRV_SIO_NS16550 component.
The required components for this driver are
INCLUDE_VXBUS
INCLUDE_PLB_BUS
INCLUDE_SIO_UTILS
INCLUDE_ISR_DEFER
which will be dragged by Kernel Configuration tool if not included in the
VxWorks Image Project (VIP) when the DRV_SIO_NS16550 is added.
You need to define all the required components when the DRV_SIO_NS16550
is added in config.h for BSP build unless they are already defined.
NOTE: The dependency from DRV_SIO_NS16550 to INCLUDE_ISR_DEFER is creataed in
${WIND_BASE}/target/src/config/usrDepend.c which is used by BSP build and the
INCLUDE_ISR_DEFER is not required to be added in config.h.

INCLUDE FILES: ../h/sio/vxbNs16550Sio.h

SEE ALSO: sioChanUtil and VxWorks Device Driver Developer's Guide.
 */

/* local defines       */

#define VXB_NS16550_CONSOLE_BAUD_RATE	115200  //sjy 9600

#ifdef	_WRS_CONFIG_SMP
LOCAL BOOL ns16550SpinlockFuncReady = FALSE;

#define VXB_NS16550_SPIN_LOCK_ISR_TAKE(pChan)				\
		if (ns16550SpinlockFuncReady)					\
		SPIN_LOCK_ISR_TAKE(&pChan->spinlockIsr)
#define VXB_NS16550_SPIN_LOCK_ISR_GIVE(pChan)				\
		if (ns16550SpinlockFuncReady)					\
		SPIN_LOCK_ISR_GIVE(&pChan->spinlockIsr)
#define VXB_NS16550_SPIN_LOCK_ISR_HELD(pChan)				\
		(ns16550SpinlockFuncReady ? spinLockIsrHeld(&pChan->spinlockIsr) : FALSE)
#define VXB_NS16550_ISR_SET(pChan)      				\
		SPIN_LOCK_ISR_TAKE(&pChan->spinlockIsr)
#define VXB_NS16550_ISR_CLEAR(pChan)    				\
		SPIN_LOCK_ISR_GIVE(&pChan->spinlockIsr)
#define VXB_NS16550_SPIN_LOCK_READY					\
		ns16550SpinlockFuncReady = TRUE
#else
#define VXB_NS16550_SPIN_LOCK_ISR_TAKE(pChan)				\
		SPIN_LOCK_ISR_TAKE(&pChan->spinlockIsr)
#define VXB_NS16550_SPIN_LOCK_ISR_GIVE(pChan)				\
		SPIN_LOCK_ISR_GIVE(&pChan->spinlockIsr)
#define VXB_NS16550_SPIN_LOCK_ISR_HELD(pChan)	FALSE
#define VXB_NS16550_ISR_SET(pChan)
#define VXB_NS16550_ISR_CLEAR(pChan)
#define VXB_NS16550_SPIN_LOCK_READY
#endif	/* _WRS_CONFIG_SMP */

/* min/max baud rate */

#define NS16550_MIN_RATE 50
#define NS16550_MAX_RATE 4000000/*115200*/

/* FIFO depth */

#define NS16550_FIFO_DEPTH_UNKNOWN	0
#define NS16550_FIFO_DEPTH_NO		1
#define NS16550_FIFO_DEPTH_DEFAULT	16

#define REG_GET(reg, pchan, val)				\
		val = vxbRead8 (pchan->handle,				\
				(UINT8 *)pchan->pDev->pRegBase[pchan->regIndex] + 	\
				(reg * pchan->regDelta));

#define REG_SET(reg, pchan, val)				\
		vxbWrite8 (pchan->handle,				\
				(UINT8 *)pchan->pDev->pRegBase[pchan->regIndex] +	\
				(reg * pchan->regDelta), val);

/* static forward declarations */

LOCAL BOOL ns16550vxbProbe(struct vxbDev * pDev);
LOCAL void ns16550vxbInstInit(struct vxbDev * pDev);
LOCAL void ns16550vxbInstInit2(struct vxbDev * pDev);
LOCAL void ns16550vxbInstConnect(struct vxbDev * pDev);
LOCAL void ns16550vxbSioChanGet(VXB_DEVICE_ID pDev, void * pArg);
LOCAL void ns16550vxbSioChanConnect(VXB_DEVICE_ID pDev, void * pArg);
void ns16550vxbSioShow(VXB_DEVICE_ID pInst, int verboseLevel);

LOCAL   int     ns16550vxbCallbackInstall (SIO_CHAN *, int, STATUS (*)(), void *);
LOCAL   STATUS  ns16550vxbDummyCallback ();
LOCAL   void    ns16550vxbInitChannel (NS16550VXB_CHAN *);
LOCAL   STATUS  ns16550vxbBaudSet (NS16550VXB_CHAN *, UINT);
LOCAL   STATUS  ns16550vxbModeSet (NS16550VXB_CHAN *, UINT);
LOCAL   STATUS  ns16550vxbIoctl (NS16550VXB_CHAN *, int, int);
LOCAL   int     ns16550vxbTxStartup (NS16550VXB_CHAN *);
LOCAL   int     ns16550vxbPollOutput (NS16550VXB_CHAN *, char);
LOCAL   int     ns16550vxbPollInput (NS16550VXB_CHAN *, char *);
LOCAL   STATUS  ns16550vxbOptsSet (NS16550VXB_CHAN *, UINT);
LOCAL   STATUS  ns16550vxbOpen (NS16550VXB_CHAN * pChan );
LOCAL   STATUS  ns16550vxbHup (NS16550VXB_CHAN * pChan );
LOCAL	void	ns16550vxbIntRd2 (void * pData);
LOCAL   void    ns16550vxbIntWr2 (void * pData);

/* driver functions */

static SIO_DRV_FUNCS ns16550vxbSioDrvFuncs =
{
		(int (*)())ns16550vxbIoctl,
		(int (*)())ns16550vxbTxStartup,
		(int (*)())ns16550vxbCallbackInstall,
		(int (*)())ns16550vxbPollInput,
		(int (*)(SIO_CHAN *,char))ns16550vxbPollOutput
};

/******************************************************************************
 *
 * ns16550SioRegister - register ns16550vxb driver
 *
 * This routine registers the ns16550vxb driver and device recognition
 * data with the vxBus subsystem.
 *
 * NOTE:
 *
 * This routine is called early during system initialization, and
 * *MUST NOT* make calls to OS facilities such as memory allocation
 * and I/O.
 *
 * RETURNS: N/A
 *
 * ERRNO
 */

void ns16550SioRegister(void)
{
	vxbDevRegister((struct vxbDevRegInfo *)&ns16550vxbDevRegistration);
}

/******************************************************************************
 *
 * ns16550vxbDevProbe - probe for device presence at specific address
 *
 * Check for ns16550vxb (or compatible) device at the specified base
 * address.  We assume one is present at that address, but
 * we need to verify.
 *
 * Probe the device by the following:
 *
 * - set the device to loopback mode
 * - mask interrupts
 * - attempt to generate an interrupt
 * - check interrupt status
 *
 * If the interrupt status register shows that an interrupt
 * did occur, then we presume it's our device.
 *
 * NOTE:
 *
 * This routine is called early during system initialization, and
 * *MUST* *NOT* make calls to OS facilities such as memory allocation
 * and I/O.
 *
 * RETURNS: TRUE if probe passes and assumed a valid ns16550vxb
 * (or compatible) device.  FALSE otherwise.
 *
 */

LOCAL BOOL ns16550vxbDevProbe
(
		struct vxbDev * pDev, /* Device information */
		int regBaseIndex      /* Index of base address */
)
{
#ifdef VXBUS_NS16550_INTRUSIVE_PROBE
	UINT8   regVal;
#endif /* VXBUS_NS16550_INTRUSIVE_PROBE */
	UINT32  regInterval = 1;		/* register address spacing */
	void *  handle;

	/* sanity test */

	if ( pDev->pRegBase[regBaseIndex] == NULL )
		return(FALSE);

	vxbRegMap (pDev, regBaseIndex, &handle);

	/* If a PCI device, the address and properties are known */

	if ( pDev->busID == VXB_BUSID_PCI )
	{
		PCI_HARDWARE * pPciDevInfo = (PCI_HARDWARE *)pDev->pBusSpecificDevInfo;

		if ( pPciDevInfo->pciDevId == 0x9835 &&  pPciDevInfo->pciVendId == 0x9710 )
		{
			/* Board: CompUSA
			 * PCI High-Speed 2-Serial and 1-Parallel Port Adapter
			 * UPC# 0-49696-10155-4
			 * SKU# 271773
			 *
			 * CHIP:    NetMos technology
			 *      Nm9825CV
			 *      FHFFT-000
			 *      0345
			 * Devices = 2
			 */
			if ( regBaseIndex > 1 )
				return(FALSE);
			else
				return(TRUE);
		}
		else if ( pPciDevInfo->pciDevId == 0x950A &&  pPciDevInfo->pciVendId == 0x1415 )
		{
			if ( regBaseIndex > 1 )
				return(FALSE);
			else
				return(TRUE);
		}
		else if ( pPciDevInfo->pciDevId == 0x6520 &&  pPciDevInfo->pciVendId == 0x10B5 )
		{
			if ( regBaseIndex > 1 )
				return(FALSE);
			else
				return(TRUE);
		}
	}
	else if (pDev->busID == VXB_BUSID_PLB)
	{
		/* get the HCF_DEVICE address */

		HCF_DEVICE * pHcf = hcfDeviceGet(pDev);

		if (pHcf != NULL)
		{
			/* Update register address spacing value */

			/*
			 * resourceDesc {
			 * The regInterval resource specifies the
			 * amount of address space between sucessive
			 * 8-bit registers. }
			 */

			devResourceGet (pHcf, "regInterval", HCF_RES_INT,
					(void *)&regInterval);
		}
	}

	/*
	 * Initialize speed, modem control, and word size.
	 * Wait for the FIFO (if any) to drain.
	 */

#ifdef VXBUS_NS16550_INTRUSIVE_PROBE
	vxbRead8 (handle, pDev->pRegBase[regBaseIndex] + (MCR * regInterval));

	/*
	 * attempt to generate an interrupt by insuring that ETXRDY
	 * toggles from 0 to 1
	 */
	/* IER[ETXRDY] <- 0 */
	/* IER[ETXRDY] <- 1 */

	regVal = vxbRead8 (handle, pDev->pRegBase[regBaseIndex] +
			(IER * regInterval));

	regVal &= ~IER_ETHREI;

	vxbWrite8 (handle, pDev->pRegBase[regBaseIndex] +
			(IER * regInterval), regVal);

	regVal |= IER_ETHREI;

	vxbWrite8 (handle, pDev->pRegBase[regBaseIndex] +
			(IER * regInterval), regVal);
	/*
	 * attempt to generate an interrupt by sending a fake null
	 * character through the loopback
	 */
	/* DATA[] <- 0 */

	regVal = 0;

	vxbWrite8 (handle, pDev->pRegBase[regBaseIndex] +
			(THR * regInterval), regVal);

	/*
	 * Wait to insure the device has time
	 * to respond.
	 */

	vxbUsDelay(25);

	/* reset line control register to 8-bits */

	regVal = DEFAULT_NS16550_COMM_PARAMS;

	vxbWrite8 (handle, pDev->pRegBase[regBaseIndex] +
			(LCR * regInterval), regVal);

	/*
	 * Check for pending interrupts by reading the Interrupt ID Register
	 */

	regVal = vxbRead8 (handle, pDev->pRegBase[regBaseIndex] +
			(IIR * regInterval));

	/* If IIR is not set, something went awry; indicate problem and exit. */

	if ( ( regVal & IIR_THRE ) != IIR_THRE )
	{
		NS16550_DBG_MSG(5, "ns16550vxbDevProbe(): INVALID ns16550vxb device @ 0x%08x regIndex %d IIR=0x%02x\n",
				(int)pDev, regBaseIndex, regVal, 4,5,6);

		return(FALSE);
	}

	NS16550_DBG_MSG(5, "ns16550vxbDevProbe(): valid ns16550vxb device @ 0x%08x regIndex %d IIR=0x%02x\n",
			(int)pDev, regBaseIndex, regVal, 4,5,6);

	/*
	 * Cleanup here.
	 */

	/* reset loopback */
	/* MCR[LOOPBACK] <- 0 */

	regVal = vxbRead8 (handle, pDev->pRegBase[regBaseIndex] +
			(MCR * regInterval));

	regVal &= ~MCR_LOOP;

	vxbWrite8 (handle, pDev->pRegBase[regBaseIndex] +
			(MCR * regInterval), regVal);

	/* reset interrupt enable register to 0 */
	/* IER[] = 0 */

	regVal = 0;
	vxbWrite8 (handle, pDev->pRegBase[regBaseIndex] +
			(LCR * regInterval), regVal);

	/* reset line control register to 8-bit */

	regVal = DEFAULT_NS16550_COMM_PARAMS;

	vxbWrite8 (handle, pDev->pRegBase[regBaseIndex] +
			(LCR * regInterval), regVal);

#endif /* Intrusive Probe */

	vxbUsDelay(25);
	return(TRUE);
}

/******************************************************************************
 *
 * ns16550vxbProbe - probe for device presence
 *
 * In pre bus subsystem versions of VxWorks, we assumed the
 * configuration stating that a 16550 was present at the specified
 * address.  With the vxBus subsystem, we continue to demand that
 * something or someone decides that the 16550 is present at a
 * specific address.  However, it is now verified.
 *
 * NOTE:
 *
 * This routine is called early during system initialization, and
 * *MUST NOT* make calls to OS facilities such as memory allocation
 * and I/O.
 *
 * RETURNS: TRUE if probe passes and assumed a valid ns16550vxb
 * (or compatible) device.  FALSE otherwise.
 *
 * ERRNO
 */

LOCAL BOOL ns16550vxbProbe
(
		struct vxbDev * pDev
)
{
	BOOL retVal = FALSE;
	int i;

	/* if PCI device, just accept it */

	if ( pDev->busID == VXB_BUSID_PCI )
		return(TRUE);

	for ( i = 0 ; i < 10 ; i++ )
		if ( ns16550vxbDevProbe(pDev, i) )
			return(TRUE);

	return(retVal);
}

/******************************************************************************
 *
 * ns16550vxbInstInit - initialize ns16550vxb device
 *
 * This is the ns16550vxb initialization routine.
 *
 * NOTE:
 *
 * This routine is called early during system initialization, and
 * *MUST NOT* make calls to OS facilities such as memory allocation
 * and I/O.
 *
 * RETURNS: N/A
 *
 * ERRNO
 */

LOCAL void ns16550vxbInstInit
(
		struct vxbDev * pDev
)
{
	NS16550VXB_CHAN *	pChan;
	int			i;
	int			regDelta = 1;
	int			fifoSize = NS16550_FIFO_DEPTH_UNKNOWN;
	UINT32		val;
	HCF_DEVICE *	pHcf = NULL;
	FUNCPTR		pFunc = NULL;
	PCI_HARDWARE * 	pPciDevInfo;


	NS16550_DBG_MSG(1, "ns16550vxbInstInit(0x%08x)\n",
			(int)pDev,2,3,4,5,6);

	if ( pDev->pDrvCtrl != NULL )
	{
		NS16550_DBG_MSG(1, "ns16550vxbInstInit(0x%08x): dup call\n",
				(int)pDev,2,3,4,5,6);
		return;
	}

	if ( pDev->busID == VXB_BUSID_PCI )
	{
		pPciDevInfo = (PCI_HARDWARE *)pDev->pBusSpecificDevInfo;

		if ( pPciDevInfo->pciVendId == UART_VENDOR_EASYSYNC )
		{
			if ( pPciDevInfo->pciDevId == UART_EASYSYNC_ESD1001 )
			{
				NS16550_DBG_MSG(11, "EasySync ES-D-1001\n", 1,2,3,4,5,6);

				/*
				 * pRegBase[0] and pRegBase[2-3] are invalid.
				 * Use pRegBase[1], and zero out the rest.
				 */
				pDev->pRegBase[0] = pDev->pRegBase[2] = NULL;
				pDev->pRegBase[3] = pDev->pRegBase[4] = NULL;
				pDev->pRegBase[5] = NULL;
			}
			if ( pPciDevInfo->pciDevId == UART_EASYSYNC_ESD1002 )
			{
				NS16550_DBG_MSG(11, "EasySync ES-D-1002\n", 1,2,3,4,5,6);

				/*
				 * pRegBase[0] and pRegBase[3] are invalid.
				 * Use pRegBase[1] and pRegBase[2], and zero out
				 * the rest.
				 */
				pDev->pRegBase[0] = pDev->pRegBase[3] = NULL;
				pDev->pRegBase[4] = pDev->pRegBase[5] = NULL;
			}
			/* we don't currently support the ESD1004 */
			if ( ( pPciDevInfo->pciDevId == UART_EASYSYNC_ESD1004 ) ||
					( pPciDevInfo->pciDevId == UART_EASYSYNC_ESD1004_1 ) )
			{
				NS16550_DBG_MSG(11, "EasySync ES-D-1004\n", 1,2,3,4,5,6);

				pDev->pRegBase[0] = pDev->pRegBase[2] = NULL;
				pDev->pRegBase[1] = pDev->pRegBase[3] = NULL;
				pDev->pRegBase[4] = pDev->pRegBase[5] = NULL;
			}
		}
		if ( ( pPciDevInfo->pciVendId == UART_VENDOR_OXFORDSEMI ) ||
				( pPciDevInfo->pciVendId == UART_VENDOR_AXXON ) )
		{
			if ( pPciDevInfo->pciDevId == UART_DEVID_SOFTIO_IC0607KB )
			{
				/*
				 * UARTs do not have unique BAR's
				 * UART 0 = BAR 0
				 * UART 1 = BAR 0 + 8
				 *
				 * Override the value of this RegBase and we are done
				 */
				pDev->pRegBase[1] = (void *)((int)pDev->pRegBase[0] + 8);

			}
			else if ( pPciDevInfo->pciDevId == UART_DEVID_AXXON_LF659KB )
			{
				/*
				 * 1-port and 2-port cards
				 *
				 * BAR0 uses I/O space
				 * BAR1 uses MEM space (interleaved by 4)
				 * BAR2 contains local cfg registers (I/O mapped)
				 * BAR3 contains local cfg registers (MEM mapped)
				 * BAR4 contains CardBus status regs (MEM mapped)
				 */
				pDev->pRegBase[0] = 0;
				pDev->pRegBase[2] = 0;
				pDev->pRegBase[3] = 0;
				pDev->pRegBase[4] = 0;
				regDelta = 4;
				fifoSize = 16;
			}
			else if ( ( pPciDevInfo->pciDevId == UART_DEVID_AXXON_LF653KB_FUNC0 ) ||
					( pPciDevInfo->pciDevId == UART_DEVID_AXXON_LF662KB_FUNC0 ) )
			{
				/*
				 * 4-port card, function 0 contains all UARTs
				 * 8-port card, function 0 contains UARTs 0-3
				 * 8-port card, function 1 contains UARTs 4-7
				 *
				 * BAR0 is all UARTs (I/O)
				 * BAR1 is all UARTs (MEM space)
				 * BAR2 is lcl cfg (I/O)
				 * BAR3 is lcl cfg (MEM)
				 * BAR4-5 unused
				 *
				 * We remap pRegBase[3] through pRegBase[5] so that
				 * they refer to memory space originally in BAR1.
				 */
				pDev->pRegBase[0] = 0;
				pDev->pRegBase[2] = 0;
				pDev->pRegBase[3] = (void *)((int)pDev->pRegBase[1] + 0x20);
				pDev->regBaseFlags[3] = pDev->regBaseFlags[1];
				pDev->pRegBase[4] = (void *)((int)pDev->pRegBase[1] + 0x40);
				pDev->regBaseFlags[4] = pDev->regBaseFlags[1];
				pDev->pRegBase[5] = (void *)((int)pDev->pRegBase[1] + 0x60);
				pDev->regBaseFlags[5] = pDev->regBaseFlags[1];

				regDelta = 4;
			}
			else if ( pPciDevInfo->pciDevId == UART_DEVID_AXXON_LF662KB_FUNC1 )
			{
				/*
				 * 4-port card, function 0 contains all UARTs
				 * 8-port card, function 0 contains UARTs 0-3
				 * 8-port card, function 1 contains UARTs 4-7
				 *
				 * BAR0 is all UARTs (I/O)
				 * BAR1 is all UARTs (MEM space)
				 * BAR2 is lcl cfg (I/O)
				 * BAR3 is lcl cfg (MEM)
				 * BAR4-5 unused
				 *
				 * We remap pRegBase[3] through pRegBase[5] so that
				 * they refer to memory space originally in BAR1.
				 */
				pDev->pRegBase[0] = 0;
				pDev->pRegBase[2] = 0;
				pDev->pRegBase[3] = (void *)((int)pDev->pRegBase[1] + 0x400);
				pDev->regBaseFlags[3] = pDev->regBaseFlags[1];
				pDev->pRegBase[4] = (void *)((int)pDev->pRegBase[1] + 0x800);
				pDev->regBaseFlags[4] = pDev->regBaseFlags[1];
				pDev->pRegBase[5] = (void *)((int)pDev->pRegBase[1] + 0xC00);
				pDev->regBaseFlags[5] = pDev->regBaseFlags[1];

				regDelta = 4;
			}
			else if ( ( pPciDevInfo->pciDevId == UART_DEVID_AXXON_LF769KB_1S1P_FUNC1 ) ||
					( pPciDevInfo->pciDevId == UART_DEVID_AXXON_LF769KB_2S_FUNC0 ) ||
					( pPciDevInfo->pciDevId == UART_DEVID_AXXON_LF769KB_2S_FUNC1 ) )
			{
				/*
				 * BARs already set correctly, so we don't need to do anything
				 * special here except update the FIFO size.
				 */
				fifoSize = 16;
			}
		}
	}

	/* check each BAR for valid register set, and configure if appropriate */

	for ( i = 0 ; i < 10 ; i++ )
	{
		NS16550_DBG_MSG(1, "ns16550vxb: probing BAR%d\n",
				i, 2,3,4,5,6);

		/* check non-NULL pRegBase */

		if ( pDev->pRegBase[i] == NULL )
			continue;

		NS16550_DBG_MSG(1, "ns16550vxb: probing BAR%d\n",
				i, 2,3,4,5,6);

		/* check for ns16550vxb device present at that address */

		if ( ns16550vxbDevProbe(pDev, i) != TRUE )
			continue;

		NS16550_DBG_MSG(1, "ns16550vxb: BAR%d refers to a valid ns16550vxb device\n",
				i, 2,3,4,5,6);

		/* device present, so allocate pChan */

		pChan = (NS16550VXB_CHAN *)hwMemAlloc(sizeof(*pChan));

		if ( pChan == NULL )
			return;

		/* fill in "static" structure elements */

		pChan->level = 0;
		pChan->baudRate = VXB_NS16550_CONSOLE_BAUD_RATE;
		pChan->regDelta = regDelta;
		pChan->fifoSize = fifoSize;
		if(pChan->channelNo == 2) *(UINT32*)0x30000004=pChan->txDefer;
		/*
		 * if the device resides on PLB, then retrieve the
		 * clock frequency and reg interval from the bsp
		 */

		if (pDev->busID == VXB_BUSID_PLB)
		{
			/* get the HCF_DEVICE address */

			pHcf = hcfDeviceGet(pDev);

			if (pHcf == NULL)
			{
				hwMemFree ((char *)pChan);

				return;
			}

			/* retrieve the integer value for clock frequency */

			/*
			 * resourceDesc {
			 * The clkFreq reqource specifies the
			 * frequency of the external oscillator
			 * connected to the device for baud
			 * rate determination.  When specifiesd
			 * as an integer, it represents the
			 * frequency, in Hz, of the external
			 * oscillator. }
			 */

			if (devResourceGet(pHcf,"clkFreq",HCF_RES_INT,
					(void *)&pChan->xtal) != OK)
			{

				/*
				 * if the integer value is not available, retrieve the function
				 * pointer to retrieve the clock frequency
				 */

				/*
				 * resourceDesc {
				 * The clkFreq reqource specifies the
				 * frequency of the external oscillator
				 * connected to the device for baud
				 * rate determination.  When specified
				 * as an address, it represents the address
				 * of a function to call, which returns the
				 * frequency. }
				 */

				if (devResourceGet(pHcf,"clkFreq",HCF_RES_ADDR,
						(void *)&pFunc)
						!= OK)
				{
					hwMemFree ((char *)pChan);
					return;
				}

				/* call the function to retrieve the clock frequency */

				pChan->xtal = (*pFunc)(pDev);
			}

			/*
			 * resourceDesc {
			 * The divisorCalc resource specifies
			 * the address of a routine to calculate
			 * the divisor used for baud rate
			 * calculations. }
			 */

			if (devResourceGet(pHcf,"divisorCalc",HCF_RES_ADDR,
					(void *)&pFunc) != OK)
				pChan->pDivisorFunc = NULL;
			else
				pChan->pDivisorFunc = pFunc;

			/* Update regDelta */
			/*
			 * resourceDesc {
			 * The regInterval resource specifies the
			 * amount of address space between sucessive
			 * 8-bit registers. }
			 */

			if (devResourceGet (pHcf, "regInterval", HCF_RES_INT,
					(void *)&val) == OK)
				pChan->regDelta = (UINT16) val;

			/*
			 * resourceDesc {
			 * The irq resource is used when
			 * BSP-provided interrupt code is
			 * used for interrupt management.
			 * It contains the "interrupt number"
			 * which is converted to a "vector"
			 * through the INUM_TO_IVEC() macro. }
			 */

			if (devResourceGet (pHcf, "irq", HCF_RES_INT,
					(void *)&val) == OK)
				pChan->irq = (UINT16) val;

			/*
			 * resourceDesc {
			 * The fifoLen resource specifies
			 * the depth of the FIFO on this
			 * device.  If not specified, and
			 * if the device supports a FIFO, then
			 * the default value of 16 is used. }
			 */

			if (devResourceGet (pHcf, "fifoLen", HCF_RES_INT,
					(void *)&val) == OK)
				pChan->fifoSize = (UINT16) val;

			/*
			 * resourceDesc {
			 * The ierInit resource is used by the BSP to
			 * assign an initial or default value of the IER
			 * register. }
			 */

			if (devResourceGet (pHcf,"ierInit",HCF_RES_INT,
					(void *)&val) == OK)
				pChan->ierInit = (UINT8) val;

		}
		else
		{
			if ( pDev->busID == VXB_BUSID_PCI )
			{
				PCI_HARDWARE * pPciDevInfo;

				pPciDevInfo = (PCI_HARDWARE *)pDev->pBusSpecificDevInfo;

				if ( pPciDevInfo->pciVendId == UART_VENDOR_EASYSYNC )
					pChan->xtal = PCI_CLK_FREQ * 8;
				else
					pChan->xtal = PCI_CLK_FREQ;
			}
			else
				pChan->xtal = PCI_CLK_FREQ;
		}

		/* fill in bus subsystem fields */

		pChan->pDev = pDev;
		pChan->regIndex = i;
		vxbRegMap (pDev, i, &pChan->handle);

		/*
		 * If BSP requires a special register access function,
		 * over write the handle field.
		 *
		 * resourceDesc {
		 * The handle resource is used to specify a custom
		 * register access routine. }
		 */

		if ( pHcf != NULL && 
				devResourceGet (pHcf, "handle", HCF_RES_ADDR,
						(void *)&val) == OK)
			pChan->handle = (void *) val;

		pChan->pNext = (NS16550VXB_CHAN *)(pDev->pDrvCtrl);
		pDev->pDrvCtrl = (void *)pChan;

		/* get the channel number */

		pChan->channelNo = sioNextChannelNumberAssign();

		/*
		 * if it is a plb based device, override the channel
		 * number with the unit number
		 */
		if (pDev->busID == VXB_BUSID_PLB)
			pChan->channelNo = pDev->unitNumber;

		else
			/* assign the unit number to be the same as channel number */

			pDev->unitNumber = pChan->channelNo;

		/* make non-standard methods available to OS layers */

		pDev->pMethods = &ns16550vxb_methods[0];

		NS16550_DBG_MSG(1, "ns16550vxb BAR%d: calling ns16550vxbDevInit(0x%08x) for channel %d\n",
				i, (int)pChan, pChan->channelNo,4,5,6);

		REG_GET(IER, pChan, pChan->ier);
		REG_GET(LCR, pChan, pChan->lcr);
		REG_GET(MCR, pChan, pChan->mcr);

		/* add ier initial bits in case the bits are cleared at reset */

		pChan->ier |= pChan->ierInit;

		pChan->isrDefRd.func = ns16550vxbIntRd2;
		pChan->isrDefRd.pData = pChan;

		pChan->isrDefWr.func = ns16550vxbIntWr2;
		pChan->isrDefWr.pData = pChan;

		/* Initialize spinlock. */

		SPIN_LOCK_ISR_INIT (&pChan->spinlockIsr, 0);

		/*
		 * Spinlock is not available unless MMU is enabled on SMP system.
		 *
		 * Interrupts are disabled on this device till vxbIntEnable()
		 * is called and spinlock is not necessary till then.
		 */

		/* initialize device */

		ns16550vxbDevInit(pChan);
	}


	return;

}

/******************************************************************************
 *
 * ns16550vxbInstInit2 - initialize ns16550vxb device
 *
 * This routine initializes the ns16550vxb device.  We do not assume a single RS232
 * device for this physical device.  Instead, we assume or more devices.  Each
 * device uses a separate pRegBase.  For each non-NULL pRegBase, we allocate a
 * pChan structure and allocate a unit number with that structure.  In
 * ns16550vxbInstConnect(), we attach the channel to the I/O system.  This bypasses
 * other BSP-based configuration mechanisms.
 *
 * Instead of waiting for ns16550vxbInstConnect(), as would be done for typical
 * devices, we initialize the serial ports and connect them to the I/O system here.
 * This allows early console output.
 *
 * This routine is called later during system initialization.  OS features
 * such as memory allocation are available at this time.
 *
 * RETURNS: N/A
 *
 * ERRNO
 */

LOCAL void ns16550vxbInstInit2
(
		struct vxbDev * pDev
)
{
	/* MMU is already enabled when it comes here and safe to use spinlock. */
	//if(pChan->channelNo == 2) *(UINT32*)0x30000008=pChan->txDefer;
	VXB_NS16550_SPIN_LOCK_READY;
}

/******************************************************************************
 *
 * ns16550vxbInstConnect - connect ns16550vxb device to I/O system
 *
 * Nothing to do here.  We want serial channels available for output reasonably
 * early during the boot process.  Because this is a serial channel, we connect
 * to the I/O system in ns16550vxbInstInit2() instead of here.
 *
 * RETURNS: N/A
 *
 * ERRNO
 */

LOCAL void ns16550vxbInstConnect
(
		struct vxbDev * pDev
)
{
	FUNCPTR sioEnableFunc;

	/*
	 * See if the parent bus has a special board-specific init
	 * routine, and if so, call it. This may be needed to pull
	 * the PHY(s) out of reset.
	 */

	sioEnableFunc = vxbDevMethodGet (vxbDevParent (pDev),
			(UINT32)&sioEnable_desc);

	if (sioEnableFunc != NULL)
		sioEnableFunc (pDev);

	return;
}

/******************************************************************************
 *
 * ns16550vxbSioChanGet - METHOD: get pChan for the specified interface
 *
 * This routine returns the pChan structure pointer for the
 * specified interface.
 *
 * RETURNS: N/A
 *
 * ERRNO
 */

LOCAL void ns16550vxbSioChanGet
(
		VXB_DEVICE_ID pDev,
		void * pArg
)
{
	NS16550VXB_CHAN * pChan;
	SIO_CHANNEL_INFO * pInfo = (SIO_CHANNEL_INFO *)pArg;

	NS16550_DBG_MSG(20, "ns16550vxbSioChanGet(%d): checking 0x%08x\n",
			pInfo->sioChanNo, (int)pDev, 3,4,5,6);

	pChan = (NS16550VXB_CHAN *)(pDev->pDrvCtrl);
	while ( pChan != NULL )
	{

		NS16550_DBG_MSG(25, "ns16550vxbSioChanGet(%d): checking channel @ 0x%08x\n",
				pInfo->sioChanNo, (int)pChan, 3,4,5,6);

		if ( pChan->channelNo == pInfo->sioChanNo )
		{
			pInfo->pChan = pChan;

			return;
		}
		pChan = pChan->pNext;
	}
}

/******************************************************************************
 *
 * ns16550vxbSioChannelConnect - connect the specified channel to /tyCo/X
 *
 * This routine connects the specified channel number to the I/O subsystem.
 * If successful, it returns the pChan structure pointer of the channel
 * connected in the SIO_CHANNEL_INFO struct.
 *
 * If the specified channel is -1, or if the specified channel matches
 * any channel on this instance, then connect the channel.
 *
 * RETURNS: N/A
 *
 * ERRNO
 */

LOCAL void ns16550vxbSioChannelConnect
(
		VXB_DEVICE_ID pDev,
		NS16550VXB_CHAN * pChan
)
{
	int interruptIndex = 0;
	FUNCPTR sioIntEnableFunc = NULL;

	NS16550_DBG_MSG(20, "ns16550vxbSioChannelConnect(%d): entry\n",
			(int)pChan, 2,3,4,5,6);


	if ( pDev == NULL )
		return;

	/* The index is always zero */

	interruptIndex = 0;

	/* connect and enable interrupts */

	vxbIntConnect(pDev, interruptIndex, ns16550vxbInt, pDev);

	sioIntEnableFunc = vxbDevMethodGet (vxbDevParent (pDev),
			(UINT32)&sioIntEnable_desc);

	if (sioIntEnableFunc != NULL)
		sioIntEnableFunc (pChan->irq);
	else
		vxbIntEnable(pDev, interruptIndex, ns16550vxbInt, pDev);
}

/******************************************************************************
 *
 * ns16550vxbSioChanConnect - METHOD: connect the specified interface to /tyCo/X
 *
 * This routine connects the specified channel number to the I/O subsystem.
 * If successful, it returns the pChan structure pointer of the channel
 * connected in the SIO_CHANNEL_INFO struct.
 *
 * If the specified channel is -1, or if the specified channel matches
 * any channel on this instance, then connect the channel.
 *
 * RETURNS: N/A
 *
 * ERRNO
 */

LOCAL void ns16550vxbSioChanConnect
(
		VXB_DEVICE_ID pDev,
		void * pArg
)
{
	SIO_CHANNEL_INFO * pInfo = (SIO_CHANNEL_INFO *)pArg;
	NS16550VXB_CHAN * pChan;

	NS16550_DBG_MSG(20, "ns16550vxbSioChanConnect(%d): checking 0x%08x\n",
			pInfo->sioChanNo, (int)pDev, 3,4,5,6);

	/* connect all channels ? */

	if ( pInfo->sioChanNo == -1 )
	{
		/* yes, connect all channels */

		pChan = pDev->pDrvCtrl;
		while ( pChan )
		{
			/* connect to I/O system and connect interrupts */

			ns16550vxbSioChannelConnect(pDev,pChan);

			pChan = pChan->pNext;
		}

	}
	else
	{
		/* no, only connect specified channel */

		/* check previous instance success */

		if ( pInfo->pChan != NULL )
			return;

		/* find channel */

		pChan = pDev->pDrvCtrl;
		while ( pChan && ( pChan->channelNo != pInfo->sioChanNo ) )
			pChan = pChan->pNext;

		/* sanity check */

		if ( pChan == NULL )
			return;
		if ( ( pInfo->sioChanNo != -1 ) && ( pChan->channelNo != pInfo->sioChanNo ) )
			return;

		/* connect to I/O system and connect interrupts */

		ns16550vxbSioChannelConnect(pDev,pChan);

		/* notify caller and downstream instances of success */

		pInfo->pChan = pChan;
	}
}

/******************************************************************************
 *
 * ns16550vxbDummyCallback - dummy callback routine.
 *
 * This routine is a dummy callback routine.
 *
 * RETURNS: ERROR, always
 *
 * ERRNO
 */

LOCAL STATUS ns16550vxbDummyCallback (void)
{
	return(ERROR);
}

/******************************************************************************
 *
 * ns16550vxbDevInit - initialize an NS16550 channel
 *
 * This routine initializes some SIO_CHAN function pointers and then resets
 * the chip in a quiescent state.  Before this routine is called, the BSP
 * must already have initialized all the device addresses, etc. in the
 * NS16550VXB_CHAN structure.
 *
 * RETURNS: N/A
 *
 * ERRNO
 */

LOCAL void ns16550vxbDevInit
(
		NS16550VXB_CHAN * pChan    /* pointer to channel */
)
{
	/* set the non BSP-specific constants */

	pChan->getTxChar    = ns16550vxbDummyCallback;
	pChan->putRcvChar   = ns16550vxbDummyCallback;
	pChan->channelMode  = 0;    /* undefined */
	pChan->options      = (CLOCAL | CREAD | CS8);
	pChan->mcr      = MCR_OUT2;

	/* reset the chip */

	ns16550vxbInitChannel (pChan);

	/* initialize the driver function pointers in the SIO_CHAN's */

	pChan->pDrvFuncs    = &ns16550vxbSioDrvFuncs;
}

/*******************************************************************************
 *
 * ns16550vxbInitChannel - initialize UART
 *
 * Initialize the number of data bits, parity and set the selected
 * baud rate. Set the modem control signals if the option is selected.
 *
 * RETURNS: N/A
 *
 * ERRNO
 */

LOCAL void ns16550vxbInitChannel
(
		NS16550VXB_CHAN * pChan    /* pointer to channel */
)
{

	/* set the requested baud rate */

	ns16550vxbBaudSet(pChan, pChan->baudRate);

	/* set the options */

	ns16550vxbOptsSet(pChan, pChan->options);
}

/*******************************************************************************
 *
 * ns16550vxbOptsSet - set the serial options
 *
 * Set the channel operating mode to that specified.  All sioLib options
 * are supported: CLOCAL, HUPCL, CREAD, CSIZE, PARENB, and PARODD.
 * When the HUPCL option is enabled, a connection is closed on the last
 * close() call and opened on each open() call.
 *
 * NOTE:
 *
 * This routine disables the transmitter.  The calling routine
 * may have to re-enable it.
 *
 * RETURNS: OK, or ERROR
 *
 * ERRNO
 */

LOCAL STATUS ns16550vxbOptsSet
(
		NS16550VXB_CHAN * pChan,   /* pointer to channel */
		UINT options            /* new hardware options */
)
{
	UINT8       value;


	if (pChan == NULL || options & 0xffffff00)
		return ERROR;

	VXB_NS16550_SPIN_LOCK_ISR_TAKE(pChan);

	pChan->mcr &= (~(MCR_RTS | MCR_DTR)); /* clear RTS and DTR bits */

	switch (options & CSIZE)
	{
	case CS5:
		pChan->lcr = CHAR_LEN_5; break;
	case CS6:
		pChan->lcr = CHAR_LEN_6; break;
	case CS7:
		pChan->lcr = CHAR_LEN_7; break;
	case CS8:
	default:
		pChan->lcr = CHAR_LEN_8; break;
	}

	if (options & STOPB)
		pChan->lcr |= LCR_STB;
	else
		pChan->lcr |= ONE_STOP;

	switch (options & (PARENB | PARODD))
	{
	case PARENB|PARODD:
	pChan->lcr |= LCR_PEN; break;
	case PARENB:
		pChan->lcr |= (LCR_PEN | LCR_EPS); break;
	default:
	case 0:
		pChan->lcr |= PARITY_NONE; break;
	}


	REG_SET(IER, pChan, pChan->ierInit);

	if (!(options & CLOCAL))
	{
		/* !clocal enables hardware flow control(DTR/DSR) */

				pChan->mcr |= (MCR_DTR | MCR_RTS);
		pChan->ier &= (~TxFIFO_BIT);
		pChan->ier |= IER_EMSI;    /* enable modem status interrupt */
	}
	else
		pChan->ier &= ~IER_EMSI; /* disable modem status interrupt */

	REG_SET(LCR, pChan, pChan->lcr);
	REG_SET(MCR, pChan, pChan->mcr);

	/* now reset the channel mode registers */
	value = (RxCLEAR | TxCLEAR | FIFO_ENABLE | FCR_RXTRIG_L | FCR_RXTRIG_H);
	REG_SET(FCR, pChan, value);

	/* check FIFO support if fifoSize is unknown  */

	if (pChan->fifoSize == NS16550_FIFO_DEPTH_UNKNOWN)
	{
		/* test if FIFO is supported on the device */

		REG_GET(IIR, pChan, value);

		if ((value & (FCR_RXTRIG_L|FCR_RXTRIG_H)) ==
				(FCR_RXTRIG_L|FCR_RXTRIG_H))
		{
			/*
			 * FIFO is supported.
			 * The max TX FIFO supported on this driver is 16 bytes, unless
			 * BSP configures the depth through "fifoLen" parameter explicitly.
			 */

			pChan->fifoSize = NS16550_FIFO_DEPTH_DEFAULT;
		}
		else
		{
			/* No FIFO support */

			pChan->fifoSize = NS16550_FIFO_DEPTH_NO;
		}
	}

	/* enable FIFO in DMA mode 1 */

	value = (FCR_DMA | RxCLEAR | TxCLEAR | FIFO_ENABLE);
	REG_SET(FCR, pChan, value);

	if (options & CREAD)
		pChan->ier |= RxFIFO_BIT;

	if (pChan->channelMode == SIO_MODE_INT)
	{

		REG_SET(IER, pChan, pChan->ier);
	}

	pChan->options = options;

	VXB_NS16550_SPIN_LOCK_ISR_GIVE(pChan);

	return OK;
}

/*******************************************************************************
 *
 * ns16550vxbHup - hang up the modem control lines
 *
 * Resets the RTS and DTR signals and clears both the receiver and
 * transmitter sections.
 *
 * RETURNS: OK
 *
 * ERRNO
 */

LOCAL STATUS ns16550vxbHup
(
		NS16550VXB_CHAN * pChan    /* pointer to channel */
)
{
	UINT8 value = 0;

	VXB_NS16550_SPIN_LOCK_ISR_TAKE(pChan);

	pChan->mcr &= (~(MCR_RTS | MCR_DTR));
	REG_SET(MCR, pChan, pChan->mcr);
	value = (RxCLEAR | TxCLEAR);
	REG_SET(FCR, pChan, value);

	VXB_NS16550_SPIN_LOCK_ISR_GIVE(pChan);

	return(OK);

}

/*******************************************************************************
 *
 * ns16550vxbOpen - Set the modem control lines
 *
 * Set the modem control lines(RTS, DTR) TRUE if not already set.
 * It also clears the receiver, transmitter and enables the FIFO.
 *
 * RETURNS: OK
 *
 * ERRNO
 */

LOCAL STATUS ns16550vxbOpen
(
		NS16550VXB_CHAN * pChan    /* pointer to channel */
)
{
	UINT8   mask;
	UINT8   value = 0;

	REG_GET(MCR, pChan, mask);
	mask &= (MCR_RTS | MCR_DTR);

	if (mask != (MCR_RTS | MCR_DTR))
	{
		/* RTS and DTR not set yet */

		VXB_NS16550_SPIN_LOCK_ISR_TAKE(pChan);

		/* set RTS and DTR TRUE */

		pChan->mcr |= (MCR_DTR | MCR_RTS);
		REG_SET(MCR, pChan, pChan->mcr);

		/* clear Tx and receive and enable FIFO */
		value = (FCR_DMA | RxCLEAR | TxCLEAR | FIFO_ENABLE);
		REG_SET(FCR, pChan, value);

		VXB_NS16550_SPIN_LOCK_ISR_GIVE(pChan);
	}

	return(OK);
}


/******************************************************************************
 *
 * ns16550vxbBaudSet - change baud rate for channel
 *
 * This routine sets the baud rate for the UART. The interrupts are disabled
 * during chip access.
 *
 * RETURNS: OK
 *
 * ERRNO
 */

LOCAL STATUS  ns16550vxbBaudSet
(
		NS16550VXB_CHAN * pChan,   /* pointer to channel */
		UINT       baud         /* requested baud rate */
)
{
	int   divisor;
	UINT8   value;

	VXB_ASSERT(pChan->pDev != NULL, ERROR)

	if(pChan->pDivisorFunc==NULL)
		divisor = (pChan->xtal / ( 8UL * baud ) + 1) / 2;
	else
		divisor = (*pChan->pDivisorFunc)(pChan->xtal,baud);

	NS16550_DBG_MSG(500,"ns16550vxbBaudSet(0x%08x, %d), divisor = %d / 0x%08x\n",
			(int)pChan,baud,divisor,divisor,5,6);

	/* disable interrupts during chip access */

	VXB_NS16550_SPIN_LOCK_ISR_TAKE(pChan);

	/* Enable access to the divisor latches by setting DLAB in LCR. */
	value = (LCR_DLAB | pChan->lcr);
	REG_SET(LCR, pChan, value);

	/* Set divisor latches. */
	value = (UINT8)divisor;
	REG_SET(DLL, pChan, value);
	value = divisor >> 8;
	REG_SET(DLM, pChan, value);

	/* Restore line control register */

	REG_SET(LCR, pChan, pChan->lcr);

	pChan->baudRate = baud;

	VXB_NS16550_SPIN_LOCK_ISR_GIVE(pChan);

	return(OK);
}

/*******************************************************************************
 *
 * ns16550vxbModeSet - change channel mode setting
 *
 * This driver supports both polled and interrupt modes and is capable of
 * switching between modes dynamically.
 *
 * If interrupt mode is desired this routine enables the channels receiver and
 * transmitter interrupts. If the modem control option is TRUE, the Tx interrupt
 * is disabled if the CTS signal is FALSE. It is enabled otherwise.
 *
 * If polled mode is desired the device interrupts are disabled.
 *
 * RETURNS: OK, or ERROR
 */

LOCAL STATUS ns16550vxbModeSet
(
		NS16550VXB_CHAN * pChan,   /* pointer to channel */
		UINT    newMode         /* mode requested */
)
{
	UINT8   mask;
	BOOL    spinLockOwn;

	if ((newMode != SIO_MODE_POLL) && (newMode != SIO_MODE_INT))
		return(ERROR);

	/* test if spinlock is already owned by the currect CPU core */

	spinLockOwn = VXB_NS16550_SPIN_LOCK_ISR_HELD(pChan);

	if (newMode == SIO_MODE_INT)
	{
		/* set the pointer of the  queue for deferred work */

		if (pChan->queueId == NULL)
			pChan->queueId = isrDeferQueueGet (pChan->pDev, 0, 0, 0);

		if (!spinLockOwn)
		{
			VXB_NS16550_SPIN_LOCK_ISR_TAKE(pChan);
		}

		/* Enable appropriate interrupts */

		if (pChan->options & CLOCAL)
		{
			UINT8 ierLocal = (pChan->ier | RxFIFO_BIT | TxFIFO_BIT);
			REG_SET(IER, pChan, ierLocal);
		}
		else
		{
			REG_GET(MSR, pChan, mask);
			mask &= MSR_CTS;

			/* if the CTS is asserted enable Tx interrupt */

			if (mask & MSR_CTS)
				pChan->ier |= TxFIFO_BIT;    /* enable Tx interrupt */
			else
				pChan->ier &= (~TxFIFO_BIT); /* disable Tx interrupt */
			REG_SET(IER, pChan, pChan->ier);
		}
	}
	else
	{
		if (!spinLockOwn)
		{
			VXB_NS16550_SPIN_LOCK_ISR_TAKE(pChan);
		}

		/* disable all ns16550vxb interrupts */

		REG_SET(IER, pChan, pChan->ierInit);
	}

	pChan->channelMode = newMode;

	if (!spinLockOwn)
	{
		VXB_NS16550_SPIN_LOCK_ISR_GIVE(pChan);
	}

	return(OK);
}

/*******************************************************************************
 *
 * ns16550vxbIoctl - special device control
 *
 * Includes commands to get/set baud rate, mode (INT,POLL), hardware options
 * (parity, number of data bits), and modem control (RTS/CTS and DTR/DSR).
 * The ioctl command SIO_HUP is sent by ttyDrv when the last close() function
 * call is made. Likewise SIO_OPEN is sent when the first open() function call
 * is made.
 *
 * RETURNS: OK on success
 *
 * ERRNO: EIO on device error, ENOSYS on unsupported request
 */

LOCAL STATUS ns16550vxbIoctl
(
		NS16550VXB_CHAN *  pChan,  /* pointer to channel */
		int         request,    /* request code */
		int             arg     /* some argument */
)
{
	FAST STATUS  status;

	status = OK;

	switch (request)
	{
	case SIO_BAUD_SET:
		if (arg < NS16550_MIN_RATE || arg > NS16550_MAX_RATE)
			status = EIO;       /* baud rate out of range */
		else
			status = ns16550vxbBaudSet (pChan, arg);
		break;

	case SIO_BAUD_GET:
		*(int *)arg = pChan->baudRate;
		break;

	case SIO_MODE_SET:
		status = (ns16550vxbModeSet (pChan, arg) == OK) ? OK : EIO;
		break;

	case SIO_MODE_GET:
		*(int *)arg = pChan->channelMode;
		break;

	case SIO_AVAIL_MODES_GET:
		*(int *)arg = SIO_MODE_INT | SIO_MODE_POLL;
		break;

	case SIO_HW_OPTS_SET:
		status = (ns16550vxbOptsSet (pChan, arg) == OK) ? OK : EIO;
		break;

	case SIO_HW_OPTS_GET:
		*(int *)arg = pChan->options;
		break;

	case SIO_HUP:
		/* check if hupcl option is enabled */

		if (pChan->options & HUPCL)
			status = ns16550vxbHup (pChan);
		break;

	case SIO_OPEN:
		/* check if hupcl option is enabled */

		if (pChan->options & HUPCL)
			status = ns16550vxbOpen (pChan);
		break;

#ifdef	_WRS_CONFIG_SMP	
	case SIO_DEV_LOCK:
		VXB_NS16550_SPIN_LOCK_ISR_TAKE(pChan);
		break;

	case SIO_DEV_UNLOCK:
		VXB_NS16550_SPIN_LOCK_ISR_GIVE(pChan);
		break;
#endif	/* _WRS_CONFIG_SMP */

	default:
		status = ENOSYS;
	}

	return(status);
}

/*******************************************************************************
 *
 * ns16550vxbIntWr2 - deferred handle a transmitter interrupt
 *
 * This routine handles write interrupts from the UART. It reads a character
 * and puts it in the transmit holding register of the device for transfer.
 *
 * If there are no more characters to transmit, transmission is disabled by
 * clearing the transmit interrupt enable bit in the IER(int enable register).
 *
 * RETURNS: N/A
 *
 * ERRNO
 */

LOCAL void ns16550vxbIntWr2
(
		void * pData		/* pointer to channel */
)
{
	NS16550VXB_CHAN * pChan = (NS16550VXB_CHAN *) pData;
	UINT8	status;
	char	outChar;

	VXB_NS16550_SPIN_LOCK_ISR_TAKE(pChan);

	/*
	 * Clear the txDefer.
	 * Another deferred job can be posted before completing the deferred job
	 * since the txDefer is cleared prior to start the getTxChar.
	 * But the job queue is already cleared before entering into
	 * ns16550vxbIntWr2() and the maximum required job queue number for
	 * transmit is one.
	 * 
	 */

	pChan->txDefer = FALSE;

	while (TRUE)
	{
		REG_GET(LSR, pChan, status);

		/* is the transmitter ready to accept a character? */
		if(pChan->channelNo == 2)
			{
			logMsg("enter ns16550vxbIntWr2 channel %d\n",pChan->channelNo,2,3,4,5,6);
//			printf("enter ns16550vxbIntWr2 channel %d\n",pChan->channelNo);
			}
		if(pChan->channelNo == 2) *(UINT32*) 0x30000000 |= (1<<3);
		if ((status & LSR_THRE) != 0x00)
		{
			int i;
			if(pChan->channelNo == 2) *(UINT32*) 0x30000000 |= (1<<4);
			VXB_NS16550_SPIN_LOCK_ISR_GIVE(pChan);

			/* Transmit characters upto the FIFO depth */

			for (i = 0; i < pChan->fifoSize; i++)
			{
				if(pChan->channelNo == 2) *(UINT32*) 0x30000000 |= (1<<5);
				if(pChan->channelNo == 2)
					{
					logMsg("send 0x%x\n",outChar,2,3,4,5,6);
//					printf("send 0x%x\n",outChar);
					}
				if ((*pChan->getTxChar) (pChan->getTxArg, &outChar) != ERROR)
				{
					/*
					 * Re-take the spinlock to mutual exclude the TX Reg
					 * access between the kprintf() in SMP mode.
					 */

					VXB_NS16550_ISR_SET(pChan);

					/* write char to Transmit Holding Reg */

					REG_SET(THR, pChan, outChar);

					VXB_NS16550_ISR_CLEAR(pChan);
				}
				else
					return;
			}
		}
		else
		{
			if(pChan->channelNo == 2) *(UINT32*) 0x30000000 |= (1<<6);
			pChan->ier |= (TxFIFO_BIT);	/* indicates to enable Tx Int */

			REG_SET(IER, pChan, pChan->ier);
			VXB_NS16550_SPIN_LOCK_ISR_GIVE(pChan);
			break;
		}

		VXB_NS16550_SPIN_LOCK_ISR_TAKE(pChan);
	}
}

/*******************************************************************************
 *
 * ns16550vxbIntRd2 - deferred handle a receiver interrupt
 *
 * This routine handles read interrupts from the UART.
 *
 * RETURNS: N/A
 *
 * ERRNO
 */



LOCAL void ns16550vxbIntRd2
(
		void * pData		/* pointer to channel */
)
{
	NS16550VXB_CHAN * pChan = (NS16550VXB_CHAN *) pData;
	char   inchar;
	char * pChar = &inchar;


	while (TRUE)
	{
		VXB_NS16550_SPIN_LOCK_ISR_TAKE(pChan);

		if ((ns16550vxbPollInput (pChan, pChar)) == OK)
		{
			VXB_NS16550_SPIN_LOCK_ISR_GIVE(pChan);
			(*pChan->putRcvChar) (pChan->putRcvArg, inchar);
		}
		else
			break;
	}

	/*
	 * Another deferred job can be posted before completing the deferred job
	 * if further Rx interrupt is raised before completing ns16550vxbIntRd2().
	 * But the job queue is already cleared before entering into
	 * this routine and the maximum required job queue number for reception
	 * is one.
	 */

	pChan->ier |= RxFIFO_BIT;		/* indicates to enable Rx Int */

	REG_SET(IER, pChan, pChan->ier);

	VXB_NS16550_SPIN_LOCK_ISR_GIVE(pChan);
}

/********************************************************************************
 *
 * ns16550vxbInt - interrupt level processing
 *
 * This routine handles four sources of interrupts from the UART. They are
 * prioritized in the following order by the Interrupt Identification Register:
 * Receiver Line Status, Received Data Ready, Transmit Holding Register Empty
 * and Modem Status.
 *
 * When a modem status interrupt occurs, the transmit interrupt is enabled if
 * the CTS signal is TRUE.
 *
 * RETURNS: N/A
 *
 * ERRNO
 */

LOCAL void ns16550vxbInt
(
		VXB_DEVICE_ID pDev
)
{
	FAST NS16550VXB_CHAN * pChan = (NS16550VXB_CHAN *)(pDev->pDrvCtrl);    /* pointer to channel */
	FAST volatile char        intStatus;
	UINT8   iirValue, lsrValue;

	if(pChan->channelNo == 2)logMsg("enter int\n",1,2,3,4,5,6);
	if(pChan->channelNo == 2) *(UINT32*) 0x30000000 = 0;
	while ( pChan != NULL )
	{
		VXB_NS16550_ISR_SET(pChan);

		/* read the Interrupt Status Register (Int. Ident.) */

		REG_GET(IIR, pChan, iirValue);
		intStatus = (char)iirValue;
		intStatus &= 0x0f;

		/*
		 * This UART chip always produces level active interrupts, and the IIR
		 * only indicates the highest priority interrupt.
		 * In the case that receive and transmit interrupts happened at
		 * the same time, we must clear both interrupt pending to prevent
		 * edge-triggered interrupt(output from interrupt controller) from locking
		 * up. One way doing it is to disable all the interrupts at the beginning
		 * of the ISR and enable at the end.
		 */

		REG_SET(IER, pChan, pChan->ierInit);
		if(pChan->channelNo == 2) logMsg("intStatus = 0x%x\n",intStatus,2,3,4,5,6);
		switch (intStatus)
		{
		case IIR_RLS:
			/* overrun,parity error and break interrupt */

			REG_GET(LSR, pChan, lsrValue); /* read LSR to reset interrupt */
			intStatus = (char)lsrValue;
			break;

		case IIR_RDA:           /* received data available */
		case IIR_TIMEOUT:
		{
			/*
			 * receiver FIFO interrupt. In some cases, IIR_RDA is
			 * not indicated in IIR register when there is more
			 * than one character in FIFO.
			 */
			if(pChan->channelNo == 2)logMsg("enter int_r\n",1,2,3,4,5,6);
			pChan->ier &= ~(RxFIFO_BIT);	/* indicate to disable Rx Int */

			REG_SET(IER, pChan, pChan->ier);

			/*
			 * Another spinlock is taken in isrDeferJobAdd().
			 * So release spinlock before entering into isrDeferJobAdd().
			 */

			VXB_NS16550_ISR_CLEAR(pChan);

			/* defer the job */

			isrDeferJobAdd (pChan->queueId, &pChan->isrDefRd);

			goto nextChan;
		}

		case IIR_THRE:  /* transmitter holding register ready */

			/*
			 * If Tx deferred job is still in queue, no need to add
			 * another one and save the number of the job queue to prevent
			 * from the queue overflow.
			 */

			pChan->ier &= ~(TxFIFO_BIT);	/* indicate to disable Tx Int */
			if(pChan->channelNo == 2) *(UINT32*) 0x30000000 |= (1<<0);
			if (!pChan->txDefer)
			{
				/* defer the job */
				if(pChan->channelNo == 2) *(UINT32*) 0x30000000 |= (1<<1);
				pChan->txDefer = TRUE;
				if(pChan->channelNo == 2)logMsg("enter int_s\n",1,2,3,4,5,6);

				REG_SET(IER, pChan, pChan->ier);

				/*
				 * Another spinlock is taken in isrDeferJobAdd ().
				 * So release spinlock before entering into
				 * isrDeferJobAdd().
				 */

				VXB_NS16550_ISR_CLEAR(pChan);

				/* defer the job */
				if(pChan->channelNo == 2) logMsg("isrDeferJobAdd isrDefWr\n",1,2,3,4,5,6);
				isrDeferJobAdd (pChan->queueId, &pChan->isrDefWr);

				goto nextChan;
			}

			break;

		case IIR_MSTAT: /* modem status register */
		{
			UINT8    msr;

			REG_GET(MSR, pChan, msr);

#define SPR_74889_FIX
#ifdef  SPR_74889_FIX

			/*
			 * If the CTS line changed, enable or
			 * disable Tx interrupt accordingly.
			 */
			if (msr & MSR_DCTS)
			{
				if (msr & MSR_CTS)
					/* CTS was turned on */
					pChan->ier |= TxFIFO_BIT;
				else
					/* CTS was turned off */
					pChan->ier &= (~TxFIFO_BIT);
			}
#else   /* SPR_74889_FIX */
			/* if CTS is asserted by modem, enable Tx interrupt */

			if ((msr & MSR_CTS) && (msr & MSR_DCTS))
				pChan->ier |= TxFIFO_BIT;
			else
				pChan->ier &= (~TxFIFO_BIT);
#endif  /* SPR_74889_FIX */
		}
		break;

		default:
			break;
		}

		REG_SET(IER, pChan, pChan->ier); /* enable interrupts accordingly */

		VXB_NS16550_ISR_CLEAR(pChan);

		nextChan:
		if ( pDev->busID == VXB_BUSID_PCI )
			pChan = pChan->pNext;
		else
			break;
	}

}

/*******************************************************************************
 *
 * ns16550vxbTxStartup - transmitter startup routine
 *
 * Call interrupt level character output routine and enable interrupt if it is
 * in interrupt mode with no hardware flow control.
 * If the option for hardware flow control is enabled and CTS is set TRUE,
 * then the Tx interrupt is enabled.
 *
 * RETURNS: OK
 *
 * ERRNO: ENOSYS if in polled mode
 */

LOCAL int ns16550vxbTxStartup
(
		NS16550VXB_CHAN * pChan    /* pointer to channel */
)
{
	UINT8 mask;
	BOOL  spinLockOwn;

	NS16550_DBG_MSG(500,"ns16550vxbTxStartup(0x%08x)\n",
			(int)pChan,2,3,4,5,6);
	if(pChan->channelNo == 2)
	{
		printf("enter ns16550vxbTxStartup,ierInit=0x%x\n",pChan->ierInit);
		if(sysGetCpuID() != *(UINT32 *)(LOCAL_BUS_ADDR_CS3+0x14))
		{
			return(ENOSYS);
		}
	}

	/* test if spinlock is already owned by the currect CPU core */

	spinLockOwn = VXB_NS16550_SPIN_LOCK_ISR_HELD(pChan);

	if (!spinLockOwn)
	{
		VXB_NS16550_SPIN_LOCK_ISR_TAKE(pChan);
	}

	if (pChan->channelMode == SIO_MODE_INT)
	{
		if (pChan->options & CLOCAL)
		{
			/* No modem control */

			/*
			 * If there is a job queue already posted for transmit, not
			 * necessary to enable Tx interrupt.
			 */
			if(pChan->channelNo == 2)  printf("enter ns16550vxbTxStartup,enable INT,pChan->txDefer=%d\n",pChan->txDefer);
			if (!pChan->txDefer)
				pChan->ier |= TxFIFO_BIT;
		}
		else
		{
			REG_GET(MSR, pChan, mask);
			mask &= MSR_CTS;

			/* if the CTS is asserted enable Tx interrupt */

			if (mask & MSR_CTS)
			{
				/*
				 * If there is a job queue already posted for transmit, not
				 * necessary to enable Tx interrupt.
				 */

				if (!pChan->txDefer)
					pChan->ier |= TxFIFO_BIT;    /* enable Tx interrupt */
			}
			else
				pChan->ier &= (~TxFIFO_BIT); /* disable Tx interrupt */
		}

		REG_SET(IER, pChan, pChan->ier);

		if (!spinLockOwn)
		{
			VXB_NS16550_SPIN_LOCK_ISR_GIVE(pChan);
		}

		NS16550_DBG_MSG(500,"ns16550vxbTxStartup(0x%08x): started\n",
				(int)pChan,2,3,4,5,6);
		return(OK);
	}
	else
	{
		if (!spinLockOwn)
		{
			VXB_NS16550_SPIN_LOCK_ISR_GIVE(pChan);
		}

		NS16550_DBG_MSG(500,"ns16550vxbTxStartup(0x%08x): ENOSYS\n",
				(int)pChan,2,3,4,5,6);
		return(ENOSYS);
	}
}

/******************************************************************************
 *
 * ns16550vxbPollOutput - output a character in polled mode
 *
 * This routine transmits a character in polled mode.
 *
 * RETURNS: OK if a character sent
 *
 * ERRNO: EAGAIN if the output buffer is full
 */

LOCAL int ns16550vxbPollOutput
(
		NS16550VXB_CHAN *  pChan,  /* pointer to channel */
		char            outChar /* char to send */
)
{
	UINT8 pollStatus;
	UINT8 msr;

	NS16550_DBG_MSG(500,"ns16550vxbPollOutput(0x%08x, '%c')\n",
			(int)pChan,outChar,3,4,5,6);
	if(pChan->channelNo == 2)  logMsg("enter ns16550vxbPollOutput\n",1,2,3,4,5,6);

	REG_GET(LSR, pChan, pollStatus);
	REG_GET(MSR, pChan, msr);

	/* is the transmitter ready to accept a character? */

	if ((pollStatus & LSR_THRE) == 0x00)
	{
		NS16550_DBG_MSG(500,"ns16550vxbPollOutput(): not ready\n",
				1,2,3,4,5,6);
		return(EAGAIN);
	}

	if (!(pChan->options & CLOCAL))  /* modem flow control ? */
	{
		if (msr & MSR_CTS)
		{
			REG_SET(THR, pChan, outChar);
		}
		else
		{
			NS16550_DBG_MSG(500,"ns16550vxbPollOutput(): flow control\n",
					1,2,3,4,5,6);
			return(EAGAIN);
		}
	}
	else
	{
		REG_SET(THR, pChan, outChar);       /* transmit character */
	}

	NS16550_DBG_MSG(500,"ns16550vxbPollOutput(0x%08x, '%c'): sent\n",
			(int)pChan,outChar,3,4,5,6);

	return(OK);
}

/******************************************************************************
 *
 * ns16550vxbPollInput - poll for input
 *
 * This routine polls the device for input.
 *
 * RETURNS: OK if a character arrived
 *
 * ERRNO: EAGAIN if the input buffer if empty
 */

LOCAL int ns16550vxbPollInput
(
		NS16550VXB_CHAN *  pChan,  /* pointer to channel */
		char *          pChar   /* pointer to char */
)
{
	UINT8 pollStatus;
	UINT8 *inChar = (UINT8 *)pChar;

	NS16550_DBG_MSG(500,"ns16550vxbPollInput(0x%08x, 0x%08x)\n",
			(int)pChan,(int)pChar,3,4,5,6);
	//  if(pChan->channelNo == 2)  logMsg("enter ns16550vxbPollInput\n",1,2,3,4,5,6);
	REG_GET(LSR, pChan, pollStatus);

	if ((pollStatus & LSR_DR) == 0x00)
	{
		NS16550_DBG_MSG(500,"ns16550vxbPollInput(): LSR_DR\n",
				1,2,3,4,5,6);
		return(EAGAIN);
	}

	/* got a character */

	REG_GET(RBR, pChan, *inChar);

	NS16550_DBG_MSG(500,"ns16550vxbPollInput(0x%08x, 0x%08x): got '%c'\n",
			(int)pChan,(int)pChar,*inChar,4,5,6);

	return(OK);
}

/******************************************************************************
 *
 * ns16550vxbCallbackInstall - install ISR callbacks
 *
 * This routine installs the callback functions to get/put chars for the
 * driver.
 *
 * RETURNS: OK on success
 *
 * ERRNO: ENOSYS on unsupported callback type
 */

LOCAL int ns16550vxbCallbackInstall
(
		SIO_CHAN *  pSioChan,       /* pointer to device to control */
		int         callbackType,   /* callback type(Tx or receive) */
		STATUS      (*callback)(),  /* pointer to callback function */
		void *      callbackArg     /* callback function argument */
)
{
	NS16550VXB_CHAN * pChan = (NS16550VXB_CHAN *)pSioChan;

	switch (callbackType)
	{
	case SIO_CALLBACK_GET_TX_CHAR:
		pChan->getTxChar    = callback;
		pChan->getTxArg     = callbackArg;
		return(OK);
	case SIO_CALLBACK_PUT_RCV_CHAR:
		pChan->putRcvChar   = callback;
		pChan->putRcvArg    = callbackArg;
		return(OK);
	default:
		return(ENOSYS);
	}

}

#ifdef NS16550_DEBUG_ON

void pChanStructShow
(
		NS16550VXB_CHAN * pChan
)
{
	printf("\t\tpDrvFuncs @ 0x%08x\n", pChan->pDrvFuncs);
	printf("\t\tgetTxChar @ 0x%08x\n", (int)(pChan->getTxChar));
	printf("\t\tgetTxArg = 0x%08x\n", (int)(pChan->getTxArg));
	printf("\t\tputRcvChar @ 0x%08x\n", (int)(pChan->putRcvChar));
	printf("\t\tputRcvArg = 0x%08x\n", (int)(pChan->putRcvArg));
	printf("\t\tlevel = %d\n", pChan->level);
	printf("\t\tier = 0x%02x\n", pChan->ier);
	printf("\t\tlcr = 0x%02x\n", pChan->lcr);
	printf("\t\tmcr = 0x%02x\n", pChan->mcr);
	printf("\t\tchannelMode = 0x%04x\n", pChan->channelMode);
	printf("\t\tregDelta = %d\n", pChan->regDelta);
	printf("\t\tfifoLen = 0x%02x\n", pChan->fifoSize);
	printf("\t\tbaudRate = %d\n", pChan->baudRate);
	printf("\t\txtal = 0x%08x\n", pChan->xtal);
	printf("\t\toptions = 0x%08x\n", pChan->options);
	printf("\t\tregIndex = %d\n", pChan->regIndex);
	printf("\t\tpDev @ 0x%08x\n", pChan->pDev);
}

void ns16550vxbSioShow
(
		VXB_DEVICE_ID	pInst,
		int			verboseLevel
)
{
	NS16550VXB_CHAN * pChan;
	device_method_t * pMethod;

	printf("        %s on %s @ 0x%08x with busInfo 0x%08x\n",
			pInst->pName, vxbBusTypeString(pInst->busID),
			(int)pInst,
			pInst->u.pSubordinateBus);

	printf("\t    pDrvCtrl @ 0x%08x\n\t    unitNumber %d\n",
			pInst->pDrvCtrl, pInst->unitNumber);

	if ( verboseLevel == 0 )
		return;

	pChan = (NS16550VXB_CHAN *)(pInst->pDrvCtrl);
	while ( pChan != NULL )
	{
		printf("\t  channel %d at 0x%08x\n",
				pChan->channelNo, (int)pChan);

		if ( verboseLevel > 10 )
			pChanStructShow(pChan);

		pChan = pChan->pNext;
	}

	if ( verboseLevel > 1000 )
	{
		pMethod = &ns16550vxb_methods[0];
		printf("\tPublished Methods:\n");
		while ( ( pMethod->devMethodId != NULL ) && ( pMethod->handler != NULL ) )
		{
			printf("\t    0x%08x => '%s'\n", (int)pMethod->handler, pMethod->devMethodId);
			pMethod++;
		}
	}
}

int ns16550vxbPollEcho
(
		NS16550VXB_CHAN * pChan
)
{
	char chr;
	int retVal;

	do
	{
		do
			retVal = ns16550vxbPollInput(pChan, &chr);
		while ( retVal == EAGAIN );

		printf("pChan(0x%08x) -> '%c'\n", (int)pChan, chr);

		ns16550vxbPollOutput(pChan,chr);
	}
	while ( chr != '\004' );

	return(0);
}

int ns16550vxbReadInputChars
(
		NS16550VXB_CHAN * pChan,
		char * buff,
		int delay
)
{
	int retVal;

	if ( delay == 0 )
		delay = 10 * sysClkRateGet();

	do
	{
		retVal = ns16550vxbPollInput(pChan, buff);
		while ( ( retVal == EAGAIN ) && ( delay > 0 ) )
		{
			taskDelay(delay);
			delay = delay / 2;
			retVal = ns16550vxbPollInput(pChan, buff);
		}
	}
	while ( retVal != EAGAIN );

	return(0);
}

int ns16550vxbDumpOutputChars
(
		NS16550VXB_CHAN * pChan,
		char chr,
		int count
)
{
	int retVal;

	if ( 0 == count )
		count = -1;

	while ( count != 0 )
	{
		do
			retVal = ns16550vxbPollOutput(pChan, chr);
		while ( retVal == EAGAIN );

		printf("%c", chr);
		if ( ( count % 80 ) == 0 )
			printf("\n");

		count--;
	}

	printf("\nfinished\n");
	return(0);
}

int ns16550vxbChanSetup
(
		int chan
)
{
	NS16550VXB_CHAN * pChan;
	UINT8 regData;

	pChan = (NS16550VXB_CHAN *)sysSerialChanGet((int)chan);

	/* set to 8-bit, 1-stop, no parity */
	regData = CHAR_LEN_8;
	REG_SET(LCR, pChan, regData);

	return(0);
}

int ns16550vxbTestOutput
(
		int chan,
		int count,
		int delay,
		int specChr
)
{
	NS16550VXB_CHAN *  pChan;
	char        chr = 'a';
	char        outchr;

	ns16550vxbChanSetup(chan);

	if ( count == 0 )
		count = 26000;

	pChan = (NS16550VXB_CHAN *)sysSerialChanGet(chan);
	if ( pChan == NULL )
	{
		printf("can't open SIO_CHAN %d\n", chan);
		return(0);
	}

	taskDelay(sysClkRateGet() * delay);

	while ( count-- > 0 )
	{
		if ( specChr != '\0' )
			outchr = specChr;
		else
			outchr = chr + (count % 26);
		ns16550vxbPollOutput(pChan, outchr);
	}
	return(0);
}


STATUS ttyTester
(
		int chan
)
{
	char ttyname[64];
	char dbuf[128];
	int ttyFd, len;

	sprintf(ttyname,"/tyCo/%d", chan);

	ttyFd = open (ttyname, O_RDWR, 0);

	(void) ioctl (ttyFd, FIOBAUDRATE, VXB_NS16550_CONSOLE_BAUD_RATE);
	(void) ioctl (ttyFd, FIOSETOPTIONS, OPT_TERMINAL);

	sprintf(dbuf, "Testing %s\n", ttyname);
	len = strlen(dbuf);

	write(ttyFd, dbuf, len);

	/* taskDelay(100); */

	close(ttyFd);

	return 0;
}

int ns16550TestChannels
(
		int			tyCoMin,
		int			tyCoMax,
		int			baudMin,
		int			baudMax
)
{
	int			fd;
	int			i;
	int			baud;
	char		tyCoName[10];
	char		tyCoSpeed[10];
	int			retVal;
	int			ctr;

	for ( i = tyCoMin ; i <= tyCoMax ; i++ )
	{
		sprintf(tyCoName, "/tyCo/%d", i);
		printf("testing %s\n", tyCoName);
		fd = open(tyCoName, 2, 0);

		ioctl (fd, FIOSETOPTIONS, OPT_TERMINAL);

		for ( baud = baudMin ; baud <= baudMax ; baud *= 2 )
		{
			printf("    baud @ %d\n", baud);

			ioctl(fd, FIOBAUDRATE, baud);
			write(fd, tyCoName, strlen(tyCoName));
			write(fd, "\n", 1);

			sprintf(tyCoSpeed, "%d\n", baud);
			write(fd, tyCoSpeed, strlen(tyCoSpeed));

			ctr = 0;
			do
				retVal = ns16550vxbPollOutput(
						(NS16550VXB_CHAN *)sysSerialChanGet(i),
						'a'+i);

			while (( retVal == EAGAIN ) && (ctr++ < 1000));
			if ( retVal == EAGAIN )
				printf("%d: no poll\n", i);

			taskDelay(30);
		}

		ioctl(fd, FIOBAUDRATE, 9600);

		close(fd);
	}

	return(0);
}

#endif /* NS16550_DEBUG_ON */

