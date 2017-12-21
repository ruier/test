/* m85xxPci.c - m85xx PCI bus controller VxBus driver */

/*
 * Copyright (c) 2007-2009 Wind River Systems, Inc.  All rights reserved.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02d,14jul09,my_  fixed memory leak in m85xxPciInstInit (WIND00173218)
02c,10jun09,rgo  A workaround for ads8572E, PCI-Ex2 Errata, to recover pcie
                 link after reset(wind127584)
02b,12may09,e_d  Add support for pci 2G window size
02a,18dec08,rgo  Add MSI Capability Check(wind00145088)
01z,13oct08,wap  Correct VXB_ACCESS_INT_VEC_GET method (WIND00123216)
01y,27aug08,jpb  Renamed VSB header file
01x,10jul08,tor  update version
01w,01jul08,dtr  Fix MSI for 8572.
01v,18jun08,jpb  Renamed _WRS_VX_SMP to _WRS_CONFIG_SMP.  Added include path
                 for kernel configurations options set in vsb.
01u,17jun08,h_k  removed pAccess.
                 reverted some deleted code in m85xxPciDevControl() done for
                 external bridge support.
01t,30may08,h_k  resource documentation.
                 removed redundant zero clear after hwMemAlloc().
01s,08may08,pmr  external bridge support.
01t,18jun08,jpb  Renamed _WRS_VX_SMP to _WRS_CONFIG_SMP.  Added include path
                 for kernel configurations options set in vsb.
01s,08may08,pmr  external bridge support.
01r,29apr08,h_k  removed CfgRead/Write.
01q,10mar08,h_k  Remove deprecated API and dead code.
01p,20sep07,tor  VXB_VERSION_3
01o,24aug07,dtr  Remove first lawbar check, not wanted.
01n,23aug07,dtr  Do not try to do any accesses if LTSSM fails.
01m,17aug07,wap  Check for PCI errors on config accesses
01l,15aug07,wap  Fix PCI config accesses to subordinate buses
01k,10aug07,wap  Set prefetchable bit on inbound I/O window, correct
                 M85XXPCI_IB_TRANS_ADRS_REG2 register definition,
                 fix test for iwAttr BSP property, use correct LAWBAR
                 allocation method in three-LAWBAR case (WIND00100585)
01j,26jul07,h_k  converted to library build
01i,17jul07,dtr  Remove Pimmr method .
01h,11jul07,dtr  Fix compiler warnings.
01g,11jul07,dtr  Move MSI program func into vxbPci.c.
01f,13jun07,tor  remove VIRT_ADDR
01e,10may07,h_k  SMP-ready.
01d,09may07,tor  remove undef LOCAL
01c,13mar07,dtr  Update InstInit2 for pciDevAnnounce.
01b,07mar07,dtr  Update to call DeviceAnnounce after AutoConfig.
01a,07feb07,dtr  created from m85x0Pci.c 
 */

/* includes */

#include <vxWorks.h>
#include <vsbConfig.h>
#include <intLib.h>
#include <iv.h>
#include <taskLib.h>

#include <stdio.h>
#include <string.h>

#include <vxBusLib.h>
#include <hwif/util/hwMemLib.h>
#include <hwif/vxbus/hwConf.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/vxbus/vxbPciLib.h>
#include <hwif/util/vxbParamSys.h>   /*add by ff 2010.08.27  */

#include <drv/pci/pciIntLib.h>
#include <drv/pci/pciConfigLib.h>
#include <drv/pci/pciAutoConfigLib.h>

#include <logLib.h>
#include "../h/vxbus/vxbPciBus.h"
#include "../h/vxbus/vxbAccess.h"
#include "../resource/m85xxCCSR.h"
#include <../src/hwif/intCtlr/vxbIntCtlrLib.h>
#include <../src/hwif/intCtlr/vxbIntDynaCtlrLib.h>

#include <spinLockLib.h>
#include "semLib.h" 
#include "simpleprintf.c"
#include "bootFlash.h"
#define 	PCIE_MAP_ADDR	0x3c000000
#define     CPUA_BAR2_ADDR	0x91000000
#define     CPUA_BAR1_ADDR	0xa4000000
/* debug */

IMPORT int ns16550Pollprintf(char* pchar);

#ifdef M85XX_DEBUG_ENABLE

/*
 * NOTE: printf() and logMsg() debugging cannot be used before the
 * call to m85xxPciInstConnect().  Any use before that may cause the
 * system to crash before finishing the boot process.  To debug
 * the probe routine, init routine, and init2 routine, either
 * use a hardware debugger, move the specified functionality to
 * m85xxPciInstConnect(), or manually call the routine after boot.
 */
UINT32 m85xxPciDebugLevel = 0;

#ifndef M85PCI_DEBUG_MSG
#define M85PCI_DEBUG_MSG(level,fmt,a,b,c,d,e,f) \
		{ \
	if ( m85xxPciDebugLevel >= level ) \
	{ \
		logMsg(fmt,a,b,c,d,e,f); \
		taskDelay(1); \
	} \
		};

#endif /* M85PCI_DEBUG_MSG */

#else /* M85XX_DEBUG_ENABLE */

#define M85PCI_DEBUG_MSG(level,fmt,a,b,c,d,e,f) 

#endif /* M85XX_DEBUG_ENABLE */

/* typedefs */

LOCAL void m85xxPciInstInit(VXB_DEVICE_ID pInst);
LOCAL void m85xxPciInstInit2(VXB_DEVICE_ID pInst);
LOCAL void m85xxPciInstConnect(VXB_DEVICE_ID pInst);

#ifdef	VXB_LEGACY_ACCESS
LOCAL STATUS m85xxPciAccessMethodOverride
(
		VXB_DEVICE_ID devID,        /* device info */
		VXB_ACCESS_LIST * pAccess   /* access structure pointer */
);

LOCAL STATUS m85xxPciMethodCfgRead
(
		VXB_DEVICE_ID  pInst,	/* device info */
		UINT8	   busNo,	/* bus number */
		UINT8	   deviceNo,	/* device number */
		UINT8	   funcNo,	/* function number */
		UINT32	   offset,	/* offset into the configuration space */
		UINT32	   width,	/* width to be read */
		void *	   pData	/* data buffer read from the offset */
);

LOCAL STATUS m85xxPciMethodCfgWrite
(
		VXB_DEVICE_ID  pInst,	/* device info */
		UINT8	   busNo,	/* bus number */
		UINT8	   deviceNo,	/* device number */
		UINT8	   funcNo,	/* function number */
		UINT32	   offset,	/* offset into the configuration space */
		UINT32	   width,	/* width to be read */
		void *	   pData	/* data buffer write to the offset */
);
#endif	/* VXB_LEGACY_ACCESS */

LOCAL STATUS m85xxPciMethodDevCfgRead
(
		VXB_DEVICE_ID  pInst,	/* device info */
		PCI_HARDWARE * pPciDev,	/* PCI device info */
		UINT32	   offset,	/* offset into the configuration space */
		UINT32	   width,	/* width to be read */
		void *	   pData	/* data buffer read from the offset */
);

LOCAL STATUS m85xxPciMethodDevCfgWrite
(
		VXB_DEVICE_ID  pInst,	/* device info */
		PCI_HARDWARE * pPciDev,	/* PCI device info */
		UINT32	   offset,	/* offset into the configuration space */
		UINT32	   width,	/* width to be read */
		UINT32	   data		/* data write to the offset */
);

LOCAL STATUS m85xxPciDevControl
(
		VXB_DEVICE_ID	devID,
		pVXB_DEVCTL_HDR	pBusDevControl
);

LOCAL STATUS m85xxPciConfigInfo 
(
		VXB_DEVICE_ID pInst,
		char * pArgs
);

LOCAL STATUS m85xxPciInterruptInfo 
(
		VXB_DEVICE_ID pInst,
		char * pArgs
);

LOCAL STATUS m85xxPciMSIProgram
(
		VXB_DEVICE_ID pInst,
		struct vxbIntDynaVecInfo *dynaVec
);

extern STATUS vxbPciMSIProgram
(
		VXB_DEVICE_ID pInst,
		struct vxbIntDynaVecInfo *dynaVec
);

extern STATUS vxbPciMSIEnable
(
		VXB_DEVICE_ID pInst,
		struct vxbIntDynaVecInfo *dynaVec
);

extern STATUS vxbPciMSIDisable
(
		VXB_DEVICE_ID pInst,
		struct vxbIntDynaVecInfo *dynaVec
);

extern STATUS vxbIntDynaConnect
(
		VXB_DEVICE_ID             pDev,          
		int                       vecCount,      
		struct vxbIntDynaVecInfo * pDynaVecIntr   
);

extern int sysGetCpuID(void);

#ifndef BOOTROM_COMPILE
int msiIndex=0;
LOCAL VXB_DEVICE_ID pInst_pci;
LOCAL FUNCPTR     pciCtrlHandler;
LOCAL BOOL getpInstFlag=0;
STATUS m85xxmsiIntSetup(int intIndex,VOIDFUNCPTR msiIsr,int pParam);
#endif

extern BOOL vxbPciMSIIsCap
(
		VXB_DEVICE_ID pInst
);

#ifdef MSI_READ_ENABLE
LOCAL STATUS m85xxPciMSIRead
(
		VXB_DEVICE_ID pDev,
		struct vxbIntDynaVecInfo *dynaVec
)
#endif

/*
 * The driver publishes driver methods.  The following driver
 * methods are for serial devices.  Change these as appropriate
 * for your device.
 */

LOCAL device_method_t m85xxPci_methods[] =
{
#ifdef	VXB_LEGACY_ACCESS
		{ PCI_CONTROLLER_METHOD_CFG_READ, (FUNCPTR)m85xxPciMethodCfgRead },
		{ PCI_CONTROLLER_METHOD_CFG_WRITE, (FUNCPTR)m85xxPciMethodCfgWrite },
#endif	/* VXB_LEGACY_ACCESS */
		{ PCI_CONTROLLER_METHOD_DEV_CFG_READ, (FUNCPTR)m85xxPciMethodDevCfgRead },
		{ PCI_CONTROLLER_METHOD_DEV_CFG_WRITE, (FUNCPTR)m85xxPciMethodDevCfgWrite },
#ifdef	VXB_LEGACY_ACCESS
		{ PCI_CONTROLLER_METHOD_BUS_ACCESS_OVERRIDE, m85xxPciAccessMethodOverride },
#else	/* VXB_LEGACY_ACCESS */
		DEVMETHOD(busCtlrDevCtlr, m85xxPciDevControl),
#endif	/* VXB_LEGACY_ACCESS */
		{ PCI_CONTROLLER_METHOD_CFG_INFO, (FUNCPTR)m85xxPciConfigInfo},
		{ PCI_CONTROLLER_METHOD_INTERRUPT_INFO, (FUNCPTR)m85xxPciInterruptInfo},
		DEVMETHOD(vxbIntDynaVecProgram, m85xxPciMSIProgram),
		DEVMETHOD(vxbIntDynaVecEnable, vxbPciMSIEnable),
		DEVMETHOD(vxbIntDynaVecDisable, vxbPciMSIDisable),
		{ 0, 0}
};


LOCAL struct drvBusFuncs m85xxPciFuncs =
{
		m85xxPciInstInit,        /* devInstanceInit */
		m85xxPciInstInit2,       /* devInstanceInit2 */
		m85xxPciInstConnect      /* devConnect */
};

LOCAL struct vxbDevRegInfo m85xxPciDevRegistration =
{
		(struct vxbDevRegInfo *)NULL, /* pNext */
		VXB_DEVID_DEVICE,     /* devID */
		VXB_BUSID_PLB,        /* busID = PLB */
		VXB_VER_4_0_0,        /* busVer */
		"m85xxPci",           /* drvName */
		&m85xxPciFuncs,       /* pDrvBusFuncs */
		&m85xxPci_methods[0], /* pMethods */
		NULL                  /* devProbe */
};


typedef struct m85xxPciDrvCtrl
{
	VXB_DEVICE_ID	pInst;
	void *	regBase;
	void *	handle;
	UINT32	magicNumber1;
	UINT32	magicNumber2;
	int         pciMaxBus;	/* Max number of sub-busses */

	void *      mem32Addr;
	UINT32      mem32Size;
	void *      memIo32Addr;
	UINT32      memIo32Size;
	void *      io32Addr;
	UINT32      io32Size;

	void *	pimmrBase;
	void *	mstrMemBus;
	void *	lclMemAddr;
	UINT32	lclMemMapSize;

	UINT32      tgtIf;
	UINT32      owAttrMem;
	UINT32      owAttrMemIo;
	UINT32      owAttrIo;
	UINT32      iwAttr;
	UINT32      singleLawBar;    
	UINT32      singleLawBarSize;    
	UINT32      pciExpressHost;
	UINT32      msiEnable;
	UINT32      dynamicInterrupts;
	UINT32      msiAddress;
	UINT32      msiData;
	UINT32      msiDataWorkaround;
	UINT32      msiDataWorkaroundAddr;
	UINT32      autoConfig;
	void *      lawbar[3];	/* LAWBAR allocated to PCI */
	void *      lawar[3];	/* LAWAR allocated to PCI */
	int         lawbarIndex[3];	/* LAWBAR index */

	BOOL	lawBarAvailable;
	BOOL	initDone;
	BOOL	bridgeInitFailed;
	struct      vxbPciConfig *pPciConfig;
	struct      vxbPciInt *pIntInfo;
	struct hcfDevice * pHcf;
	spinlockIsr_t spinlockIsr;	/* ISR-callable spinlock */

	/* external bridge interrupt map */
	int *	extIntMap;
	int		extIntMapSize;

	/* a flag to invoke PCI EXT2 errata workaround */

	BOOL	pciEx2ErrataFlag;

} M85XXPCI_DRV_CTRL;

/* defines */

#ifdef  _WRS_CONFIG_SMP
LOCAL BOOL vxbM85xxPciSpinlockFuncReady = FALSE;

#define VXB_M85XXPCI_SPIN_LOCK_ISR_TAKE(pDrvCtrl)			\
		if (vxbM85xxPciSpinlockFuncReady)					\
		SPIN_LOCK_ISR_TAKE(&pDrvCtrl->spinlockIsr)
#define VXB_M85XXPCI_SPIN_LOCK_ISR_GIVE(pDrvCtrl)			\
		if (vxbM85xxPciSpinlockFuncReady)					\
		SPIN_LOCK_ISR_GIVE(&pDrvCtrl->spinlockIsr)
#define VXB_M85XXPCI_SPIN_LOCK_READY					\
		vxbM85xxPciSpinlockFuncReady = TRUE;
#else   /* _WRS_CONFIG_SMP */
#define VXB_M85XXPCI_SPIN_LOCK_ISR_TAKE(pDrvCtrl)			\
		SPIN_LOCK_ISR_TAKE(&pDrvCtrl->spinlockIsr)
#define VXB_M85XXPCI_SPIN_LOCK_ISR_GIVE(pDrvCtrl)			\
		SPIN_LOCK_ISR_GIVE(&pDrvCtrl->spinlockIsr)
#define VXB_M85XXPCI_SPIN_LOCK_READY
#endif  /* _WRS_CONFIG_SMP */

#define M85XXPCI_BAR(p)		((M85XXPCI_DRV_CTRL *)(p)->pDrvCtrl)->regBase
#define M85XXPCI_HANDLE(p)	((M85XXPCI_DRV_CTRL *)(p)->pDrvCtrl)->handle

#define CSR_READ_4(pDev, addr)                                  \
		vxbRead32 (M85XXPCI_HANDLE(pDev), 				\
				(UINT32 *)((char *)M85XXPCI_BAR(pDev) + addr))

#define CSR_WRITE_4(pDev, addr, data)				\
		vxbWrite32 (M85XXPCI_HANDLE(pDev),				\
				(UINT32 *)((char *)M85XXPCI_BAR(pDev) + addr), data)

#define CSR_READ_2(pDev, addr)                                  \
		vxbRead16 (M85XXPCI_HANDLE(pDev), 				\
				(UINT16 *)((char *)M85XXPCI_BAR(pDev) + addr))

#define CSR_WRITE_2(pDev, addr, data)				\
		vxbWrite16 (M85XXPCI_HANDLE(pDev),				\
				(UINT16 *)((char *)M85XXPCI_BAR(pDev) + addr), data)

#define CSR_READ_1(pDev, addr)                                  \
		vxbRead8 (M85XXPCI_HANDLE(pDev), 				\
				((UINT8 *)M85XXPCI_BAR(pDev) + addr))

#define CSR_WRITE_1(pDev, addr, data)				\
		vxbWrite8 (M85XXPCI_HANDLE(pDev),				\
				((UINT8 *)M85XXPCI_BAR(pDev) + addr), data)

#ifdef	VXB_LEGACY_ACCESS
#define M85XXPCI_CFG_READ(pInst, offset, width, data)		\
		m85xxPciMethodCfgRead(pInst, 0, 0, 0, offset, width, (void *) &data)
#define M85XXPCI_CFG_WRITE(pInst, offset, width, data)		\
		m85xxPciMethodCfgWrite(pInst, 0, 0, 0, offset, width, (void *) &data)
#else	/* VXB_LEGACY_ACCESS */
#define M85XXPCI_CFG_READ(pInst, offset, width, data)		\
		m85xxPciMethodDevCfgRead(pInst, pPciDev, offset, width, (void *) &data)
#define M85XXPCI_CFG_WRITE(pInst, offset, width, data)		\
		m85xxPciMethodDevCfgWrite(pInst, pPciDev, offset, width, (UINT32) data)
#endif	/* VXB_LEGACY_ACCESS */

#ifndef LAWAR_TGTIF_PCI
#define LAWAR_TGTIF_PCI         0x00000000
#endif
#ifndef LAWAR_TGTIF_LBC
#define LAWAR_TGTIF_LBC         0x00400000
#endif
#ifndef LAWAR_TGTIF_RAPIDIO
#define LAWAR_TGTIF_RAPIDIO     0x00c00000
#endif
#ifndef LAWAR_TGTIF_DDRSDRAM
#define LAWAR_TGTIF_DDRSDRAM    0x00F00000
#endif

#ifndef PCI_MSTR_MEM_BUS
#define PCI_MSTR_MEM_BUS       0x40000000      /* align on 512 MB */
#endif


/* static file scope locals */

/* TBD ALL the registers for PCI Bridge */
/* configuration space reg and int ack */

#define M85XXPCI_CONF_ADDR              0x0000
#define M85XXPCI_CONF_DATA              0x0004
#define M85XXPCI_INT_ACK		0x0008

/* PCI error registers */
#define M85XXPCI_ERR_DR				0x0E00
#define M85XXPCI_ERR_ENABLE             0x0e08
#define M85XXPCI_ERR_DISABLE		0x0E10
#define M85XXPCI_ERR_CAP_STAT		0x0e20		/* add by ff*/
#define M85XXPCI_ERR_CAP_R0				0x0e28		/* add by ff*/
#define M85XXPCI_ERR_CAP_R1				0x0e2C		/* add by ff*/
#define M85XXPCI_ERR_CAP_R2				0x0e30		/* add by ff*/
#define M85XXPCI_ERR_CAP_R3				0x0e34		/* add by ff*/

/* PCI error detect register */

/*			add by ff----begin*/
#define M85XXPCI_ERR_MULTI              0x80000000
#define M85XXPCI_ERR_PCT				0x00800000
#define M85XXPCI_ERR_PCAC				0x00200000
#define M85XXPCI_ERR_PNM				0x00100000
#define M85XXPCI_ERR_CDNSC				0x00080000
#define M85XXPCI_ERR_CRSNC				0x00040000
#define M85XXPCI_ERR_ICCA				0x00020000
#define M85XXPCI_ERR_IACA				0x00010000
#define M85XXPCI_ERR_CRST				0x00008000
#define M85XXPCI_ERR_MIS				0x00004000
#define M85XXPCI_ERR_IOIS				0x00002000
#define M85XXPCI_ERR_CIS				0x00001000
#define M85XXPCI_ERR_CIEP				0x00000800
#define M85XXPCI_ERR_IOIEP				0x00000400
#define M85XXPCI_ERR_OAC				0x00000200
#define M85XXPCI_ERR_IOIA				0x00000100
/*			add by ff----end*/

/* PCI error attribute register */

/*			add by ff----begin*/
#define M85XXPCI_ERR_CAP_STAT_TO_PEXCFG		0x40
#define M85XXPCI_ERR_CAP_STAT_GSID_PEX1		0x0
#define M85XXPCI_ERR_CAP_STAT_GSID_PEX2		0x2
#define M85XXPCI_ERR_CAP_STAT_GSID_BOOT		0x14
#define M85XXPCI_ERR_CAP_STAT_GSID_Rapio	0x18
#define M85XXPCI_ERR_CAP_STAT_GSID_ProcIns	0x10
#define M85XXPCI_ERR_CAP_STAT_GSID_ProcData	0x11
#define M85XXPCI_ERR_CAP_STAT_GSID_DMA		0x2A
#define M85XXPCI_ERR_CAP_STAT_GSID_eTSEC1	0x30
#define M85XXPCI_ERR_CAP_STAT_GSID_eTSEC2	0x31
#define M85XXPCI_ERR_CAP_STAT_GSID_eTSEC3	0x32
#define M85XXPCI_ERR_CAP_STAT_GSID_eTSEC4	0x33
#define M85XXPCI_ERR_CAP_STAT_GSID_RapMes	0x38
/*			add by ff----end*/
/* Outbound translation registers */

#define M85XXPCI_OB_TRANS_ADRS_REG0   	0x0c00
#define M85XXPCI_OB_BASE_ADRS_REG0    	0x0c08
#define M85XXPCI_OB_TRANS_ADRS_REG1   	0x0c20
#define M85XXPCI_OB_BASE_ADRS_REG1    	0x0c28
#define M85XXPCI_OB_TRANS_ADRS_REG2   	0x0c40
#define M85XXPCI_OB_BASE_ADRS_REG2    	0x0c48
#define M85XXPCI_OB_TRANS_ADRS_REG3   	0x0c60
#define M85XXPCI_OB_BASE_ADRS_REG3    	0x0c68
#define M85XXPCI_OB_TRANS_ADRS_REG4   	0x0c80
#define M85XXPCI_OB_BASE_ADRS_REG4    	0x0c88


/* Outbound attributes register definitions */

#define M85XXPCI_OB_ATTR_REG0            0x0c10
#define M85XXPCI_OB_ATTR_REG1            0x0c30
#define M85XXPCI_OB_ATTR_REG2            0x0c50
#define M85XXPCI_OB_ATTR_REG3            0x0c70
#define M85XXPCI_OB_ATTR_REG4            0x0c90

/* Outbound/Inbound Comparison mask register defines */
#define PCI_WINDOW_ENABLE_BIT            0x80000000
#define PCI_ATTR_BS_BIT                  0x40000000
#define PCI_OUT_ATTR_RTT_CFG	 		 0x00020000	/*add by ff*/
#define PCI_OUT_ATTR_RTT_MEM             0x00040000
#define PCI_OUT_ATTR_RTT_IO              0x00080000
#define PCI_OUT_ATTR_WTT_CFG	 		 0X00002000	 /*add by ff*/ 
#define PCI_OUT_ATTR_WTT_MEM             0x00004000
#define PCI_OUT_ATTR_WTT_IO              0x00008000
#define PCI_ATTR_WS_4K                   0x0000000B
#define PCI_ATTR_WS_8K                   0x0000000c
#define PCI_ATTR_WS_16K                  0x0000000D
#define PCI_ATTR_WS_32K                  0x0000000E
#define PCI_ATTR_WS_64K                  0x0000000F
#define PCI_ATTR_WS_128K                 0x00000010
#define PCI_ATTR_WS_256K                 0x00000011
#define PCI_ATTR_WS_512K                 0x00000012
#define PCI_ATTR_WS_1M                   0x00000013
#define PCI_ATTR_WS_2M                   0x00000014
#define PCI_ATTR_WS_4M                   0x00000015
#define PCI_ATTR_WS_8M                   0x00000016
#define PCI_ATTR_WS_16M                  0x00000017
#define PCI_ATTR_WS_32M                  0x00000018
#define PCI_ATTR_WS_64M                  0x00000019
#define PCI_ATTR_WS_128M                 0x0000001a
#define PCI_ATTR_WS_256M                 0x0000001b
#define PCI_ATTR_WS_512M                 0x0000001c
#define PCI_ATTR_WS_1G                   0x0000001d
#define PCI_ATTR_WS_2G                   0x0000001e
#define PCI_ATTR_WS_4G                   0x0000001f
#define PCI_ATTR_WS_8G    				 0x00000020		/*add by ff*/
#define PCI_ATTR_WS_16GB   				 0x00000021		/*add by ff*/
#define PCI_ATTR_WS_32GB     			 0X00000022    	/*add by ff*/
#define PCI_ATTR_WS_64GB     			 0X00000023    	/*add by ff*/

#define PCI_IN_ATTR_PREFETCHABLE         0x20000000
#define PCI_IN_ATTR_TGI_LM               0x00f00000
#define PCI_IN_ATTR_TGI_RIO              0x00c00000
#define PCI_IN_ATTR_TGI_PCIE1			 0x00		/*add by ff*/
#define PCI_IN_ATTR_TGI_PCIE2			 0x00100000	/*add by ff*/
#define PCI_IN_ATTR_RTT_RIO_READ         0x00040000	/*not LM  FF*/
#define PCI_IN_ATTR_RTT_LM_READ_NO_SNOOP 0x00040000
#define PCI_IN_ATTR_RTT_LM_READ_SNOOP    0x00050000
#define PCI_IN_ATTR_RTT_LM_READ_UNLOCK_L2_CACHE_LINE 0x00070000
#define PCI_IN_ATTR_RTT_LM_WRITE_NO_SNOOP 0x00004000
#define PCI_IN_ATTR_RTT_LM_WRITE_SNOOP   0x00005000
#define PCI_IN_ATTR_RTT_LM_WRITE_ALLOC_L2_CACHE_LINE 0x00006000
#define PCI_IN_ATTR_RTT_LM_WRITE_ALLOC_LOCK_L2_CACHE_LINE 0x00007000

#define PCI_SNOOP_ENABLE        0x40000000
#define PCI_PREFETCHABLE        0x20000000

/* Inbound translation registers */
#define M85XXPCI_IB_TRANS_ADRS_REG1    0x0DE0
#define M85XXPCI_IB_BASE_ADRS_REG1     0x0DE8
#define M85XXPCI_IB_ATTR_REG1          0x0DF0

#define M85XXPCI_IB_TRANS_ADRS_REG2    0x0DC0
#define M85XXPCI_IB_BASE_ADRS_REG2     0x0DC8
#define M85XXPCI_IB_ATTR_REG2          0x0DD0

#define M85XXPCI_IB_TRANS_ADRS_REG3    0x0DA0
#define M85XXPCI_IB_BASE_ADRS_REG3     0x0DA8
#define M85XXPCI_IB_ATTR_REG3          0x0DB0

/* PCI error Registers */

/* Comand status register defines */
#define BUS_MASTER_ENABLE_BIT   0x4
#define MEMORY_SPACE_ACCESS_ENABLE_BIT 0x2

/* PCI Host Bridge setup File */

#define MAX_NUM_VECTORS 4

#define     M85XXPCI_ENCODED_SIZE_4KB          0x0000000B
#define     M85XXPCI_ENCODED_SIZE_8KB          0x0000000C
#define     M85XXPCI_ENCODED_SIZE_16KB         0x0000000D
#define     M85XXPCI_ENCODED_SIZE_32KB         0x0000000E
#define     M85XXPCI_ENCODED_SIZE_64KB         0x0000000F
#define     M85XXPCI_ENCODED_SIZE_128KB        0x00000010
#define     M85XXPCI_ENCODED_SIZE_256KB        0x00000011
#define     M85XXPCI_ENCODED_SIZE_512KB        0x00000012
#define     M85XXPCI_ENCODED_SIZE_1MB          0x00000013
#define     M85XXPCI_ENCODED_SIZE_2MB          0x00000014
#define     M85XXPCI_ENCODED_SIZE_4MB          0x00000015
#define     M85XXPCI_ENCODED_SIZE_8MB          0x00000016
#define     M85XXPCI_ENCODED_SIZE_16MB         0x00000017
#define     M85XXPCI_ENCODED_SIZE_32MB         0x00000018
#define     M85XXPCI_ENCODED_SIZE_64MB         0x00000019
#define     M85XXPCI_ENCODED_SIZE_128MB        0x0000001A
#define     M85XXPCI_ENCODED_SIZE_256MB        0x0000001B
#define     M85XXPCI_ENCODED_SIZE_512MB        0x0000001C
#define     M85XXPCI_ENCODED_SIZE_1GB          0x0000001D
#define     M85XXPCI_ENCODED_SIZE_2GB          0x0000001E 
#define 	M85XXPCI_ENCODED_SIZE_4GB               0x0000001f		/*add by ff*/
#define 	M85XXPCI_ENCODED_SIZE_8GB    		   0x00000020		/*add by ff*/
#define 	M85XXPCI_ENCODED_SIZE_16GB   		   0x00000021		/*add by ff*/
#define 	M85XXPCI_ENCODED_SIZE_32GB     			 0X00000022    	/*add by ff*/
#define 	M85XXPCI_ENCODED_SIZE_64GB     			 0X00000023    	/*add by ff*/


/* PEX specific */
#define M85XXPCI_LINK_DETECT_QUIET      0x00
#define M85XXPCI_LINK_DETECT_ACTIVE     0x01
#define M85XXPCI_LINK_L0                0x16
#define M85XXPCI_LTSSM_REG              0x0404
#define MSI_LOCAL_CONTROL_REG 0x72 
#define MSI_LOCAL_ADDRESS_REG 0x74
#define MSI_LOCAL_UPPER_ADDRESS_REG 0x78
#define MSI_LOCAL_DATA_REG 0x7C

#define MSI_CONTROL_REG 0xd2 
#define MSI_ADDRESS_REG 0xd4
#define MSI_UPPER_ADDRESS_REG 0xd8
#define MSI_DATA_REG 0xdc

#define PCI_CFG_ADR_REG        (CCSBAR + 0x8000)    
#define PCI_CFG_DATA_REG       (CCSBAR + 0x8004)

/* locals */
static UINT32 TEMP;
LOCAL M85XXPCI_DRV_CTRL * pM85xxPciDrvCtrl0 = NULL;

/* forward declarations */

METHOD_DECL(m85xxLawBarAlloc);
LOCAL STATUS m85xxPciBridgeInit(VXB_DEVICE_ID pInst);

LOCAL int m85xxPciIntNumGet
(
		VXB_DEVICE_ID	devID,
		pVXB_DEVCTL_HDR	pBusDevControl,
		int			devNo
);

/* extern */
IMPORT void vxbMsDelay(int); 

/* ------------------------------------------------------------ */

/*
DESCRIPTION

This is the VxBus driver for the PCI bus controller on the
m85xx CPUs.

This driver work on PLB bus, and gets configuration information
from the hwconf.c file entry.  This driver supports only 1-1
address mappings, so it uses the PCI addresses and sizes specified
in hwconf as the local addresses.

 */

/* local defines       */

#define     PCI_CFG_PBFR   0x44

/* static forward declarations */

/* driver functions */

#ifdef	VXB_LEGACY_ACCESS
/*******************************************************************************
 *
 * m85xxPciMethodCfgRead - read from the PCI configuration space
 *
 * This routine reads either a byte, word or a long word specified by
 * the argument <width>, from the PCI configuration space
 * This routine works around a problem in the hardware which hangs
 * PCI bus if device no 12 is accessed from the PCI configuration space.
 *
 * \NOMANUAL
 *
 * RETURNS: OK, or ERROR if this library is not initialized
 *
 * ERRNO
 */

LOCAL STATUS m85xxPciMethodCfgRead
(
		VXB_DEVICE_ID  pInst,	/* device info */
		UINT8	   busNo,	/* bus number */
		UINT8	   deviceNo,	/* device number */
		UINT8	   funcNo,	/* function number */
		UINT32	   offset,	/* offset into the configuration space */
		UINT32	   width,	/* width to be read */
		void *	   pData	/* data buffer read from the offset */
)
{
	M85XXPCI_DRV_CTRL * pDrvCtrl;
	UINT32		retValLong;
	STATUS		retStat = OK;

	/* simulator doesn't like this device being used */

	if (busNo == 0 && deviceNo == 0x1F)
		return (ERROR);

	pDrvCtrl = pInst->pDrvCtrl;

	switch (width)
	{
	case 1: /* byte */
		VXB_M85XXPCI_SPIN_LOCK_ISR_TAKE(pDrvCtrl);
		CSR_WRITE_4(pInst, M85XXPCI_CONF_ADDR,
				vxbPciConfigBdfPack (busNo, deviceNo, funcNo) |
				(offset & 0xfc) | 0x80000000 | ((offset & 0xf00) << 16));
		retValLong = CSR_READ_4 (pInst, M85XXPCI_CONF_DATA);
		if (CSR_READ_4(pInst, M85XXPCI_ERR_DR))
			CSR_WRITE_4(pInst, M85XXPCI_ERR_DR, 0xFFFFFFFF);
		VXB_M85XXPCI_SPIN_LOCK_ISR_GIVE(pDrvCtrl);

		*(UINT8 *)pData = (UINT8)(le32toh(retValLong) >>
				((offset & 3) * 8));
		M85PCI_DEBUG_MSG(1, "\nRead(): 0x%x\n",
				*(UINT8 *)pData, 2, 3, 4, 5, 6);
		break;

	case 2: /* word */
		VXB_M85XXPCI_SPIN_LOCK_ISR_TAKE(pDrvCtrl);
		CSR_WRITE_4(pInst, M85XXPCI_CONF_ADDR,
				vxbPciConfigBdfPack (busNo, deviceNo, funcNo) |
				(offset & 0xfc) | 0x80000000 | ((offset & 0xf00) << 16));
		retValLong = CSR_READ_4(pInst, M85XXPCI_CONF_DATA);
		if (CSR_READ_4(pInst, M85XXPCI_ERR_DR))
			CSR_WRITE_4(pInst, M85XXPCI_ERR_DR, 0xFFFFFFFF);
		VXB_M85XXPCI_SPIN_LOCK_ISR_GIVE(pDrvCtrl);

		*(UINT16 *)pData = (UINT16)(le32toh(retValLong) >>
				((offset & 2) * 8));
		M85PCI_DEBUG_MSG(1, "\nRead(): 0x%x\n",
				*(UINT16*)pData, 2, 3, 4, 5, 6);
		break;

	case 4: /* long */
		VXB_M85XXPCI_SPIN_LOCK_ISR_TAKE(pDrvCtrl);
		CSR_WRITE_4(pInst, M85XXPCI_CONF_ADDR,
				vxbPciConfigBdfPack (busNo, deviceNo, funcNo) |
				(offset & 0xfc) | 0x80000000 | ((offset & 0xf00) << 16));
		retValLong = CSR_READ_4(pInst, M85XXPCI_CONF_DATA);
		if (CSR_READ_4(pInst, M85XXPCI_ERR_DR))
			CSR_WRITE_4(pInst, M85XXPCI_ERR_DR, 0xFFFFFFFF);
		VXB_M85XXPCI_SPIN_LOCK_ISR_GIVE(pDrvCtrl);

		*(UINT32 *)pData = le32toh(retValLong);
		M85PCI_DEBUG_MSG(1, "\nRead(): 0x%x\n",
				*(UINT32*)pData, 2, 3, 4, 5, 6);
		break;

	default:
		retStat = ERROR;
		break;
	}

	return (retStat);
}

/*******************************************************************************
 *
 * m85xxPciMethodCfgWrite - write to the PCI configuration space
 *
 * This routine writes either a byte, word or a long word specified by
 * the argument <width>, to the PCI configuration space
 * This routine works around a problem in the hardware which hangs
 * PCI bus if device no 12 is accessed from the PCI configuration space.
 *
 * \NOMANUAL
 *
 * RETURNS: OK, or ERROR if this library is not initialized
 *
 * ERRNO
 */

LOCAL STATUS m85xxPciMethodCfgWrite
(
		VXB_DEVICE_ID pInst,	/* device info */
		UINT8	busNo,		/* bus number */
		UINT8	deviceNo,	/* device number */
		UINT8	funcNo,		/* function number */
		UINT32	offset,		/* offset into the configuration space */
		UINT32	width,		/* width to write */
		void *	pData		/* data buffer to write */
)
{
	M85XXPCI_DRV_CTRL * pDrvCtrl;

	/* PCI-Express errata RMW workaround should work for all PCI 
	 * Requires always to do 4 byte read/write  
	 */ 

	UINT32 localdata;
	UINT32 data2;
	UINT32 mask;

	if ((busNo == 0) && (deviceNo == 0x1f))
		return (ERROR);

	/* Establish the required mask based on width and offset */

	switch(width)
	{
	case 1:
		localdata = (UINT32)(*(UINT8*)pData);
		mask=0xff;
		mask=~(mask << ((offset&0x3)*8));
		break;
	case 2:
		localdata = (UINT32)(*(UINT16*)pData);
		mask=0xffff;
		mask=~(mask << ((offset&0x3)*8));
		break;
	case 4:
		localdata = *(UINT32*)pData;
		mask=0;
		break;
	default:
		return(ERROR);
	}

	if (m85xxPciMethodCfgRead (pInst, busNo, deviceNo, funcNo,
			(offset & ~0x3), 4, &data2) == ERROR)
		return(ERROR);

	data2 &= mask;
	/* Overwite only part of word required */
	data2 |= localdata << ((offset & 0x3) * 8);	/* final value to write */

	offset = (offset & ~0x3); /* align by 32 bits */

	pDrvCtrl = pInst->pDrvCtrl;

	VXB_M85XXPCI_SPIN_LOCK_ISR_TAKE(pDrvCtrl);
	CSR_WRITE_4(pInst, M85XXPCI_CONF_ADDR,
			vxbPciConfigBdfPack (busNo, deviceNo, funcNo) |
			(offset & 0xfc) | 0x80000000 | ((offset & 0xf00) << 16));
	CSR_WRITE_4(pInst, M85XXPCI_CONF_DATA, htole32(data2));
	CSR_WRITE_4(pInst, M85XXPCI_ERR_DR, 0xFFFFFFFF);
	VXB_M85XXPCI_SPIN_LOCK_ISR_GIVE(pDrvCtrl);

	M85PCI_DEBUG_MSG(1, "\nWrite(): data 0x%x offset 0x%x\n",
			localdata, offset, 3, 4, 5, 6);   

	return (OK);
}
#endif	/* VXB_LEGACY_ACCESS */


/*******************************************************************************
 *
 * m85xxPciMethodDevCfgRead - read from the PCI configuration space
 *
 * This routine reads either a byte, word or a long word specified by
 * the argument <width>, from the PCI configuration space
 * This routine works around a problem in the hardware which hangs
 * PCI bus if device no 12 is accessed from the PCI configuration space.
 *
 * \NOMANUAL
 *
 * RETURNS: OK, or ERROR if this library is not initialized
 *
 * ERRNO
 */

LOCAL STATUS m85xxPciMethodDevCfgRead
(
		VXB_DEVICE_ID  pInst,	/* device info */
		PCI_HARDWARE * pPciDev,	/* PCI device info */
		UINT32	   offset,	/* offset into the configuration space */
		UINT32	   width,	/* width to be read */
		void *	   pData	/* data buffer read from the offset */
)
{
	M85XXPCI_DRV_CTRL * pDrvCtrl;
	UINT32		retValLong;
	STATUS		retStat = OK;
	UINT8		busNo    = pPciDev->pciBus;
	UINT8		deviceNo = pPciDev->pciDev;
	UINT8		funcNo   = pPciDev->pciFunc;

	/* simulator doesn't like this device being used */

	if (busNo == 0 && deviceNo == 0x1F)
		return (ERROR);

	pDrvCtrl = pInst->pDrvCtrl;

	switch (width)
	{
	case 1: /* byte */
		VXB_M85XXPCI_SPIN_LOCK_ISR_TAKE(pDrvCtrl);
		CSR_WRITE_4(pInst, M85XXPCI_CONF_ADDR,
				vxbPciConfigBdfPack (busNo, deviceNo, funcNo) |
				(offset & 0xfc) | 0x80000000 | ((offset & 0xf00) << 16));
		retValLong = CSR_READ_4 (pInst, M85XXPCI_CONF_DATA);
		if (CSR_READ_4(pInst, M85XXPCI_ERR_DR))
			CSR_WRITE_4(pInst, M85XXPCI_ERR_DR, 0xFFFFFFFF);
		VXB_M85XXPCI_SPIN_LOCK_ISR_GIVE(pDrvCtrl);

		*(UINT8 *)pData = (UINT8)(le32toh(retValLong) >>
				((offset & 3) * 8));
		M85PCI_DEBUG_MSG(1, "\nRead(): 0x%x\n",
				*(UINT8 *)pData, 2, 3, 4, 5, 6);

		break;

	case 2: /* word */
		VXB_M85XXPCI_SPIN_LOCK_ISR_TAKE(pDrvCtrl);
		CSR_WRITE_4(pInst, M85XXPCI_CONF_ADDR,
				vxbPciConfigBdfPack (busNo, deviceNo, funcNo) |
				(offset & 0xfc) | 0x80000000 | ((offset & 0xf00) << 16));
		retValLong = CSR_READ_4(pInst, M85XXPCI_CONF_DATA);
		if (CSR_READ_4(pInst, M85XXPCI_ERR_DR))
			CSR_WRITE_4(pInst, M85XXPCI_ERR_DR, 0xFFFFFFFF);
		VXB_M85XXPCI_SPIN_LOCK_ISR_GIVE(pDrvCtrl);

		*(UINT16 *)pData = (UINT16)(le32toh(retValLong) >>
				((offset & 2) * 8));
		M85PCI_DEBUG_MSG(1, "\nRead(): 0x%x\n",
				*(UINT16*)pData, 2, 3, 4, 5, 6);

		break;

	case 4: /* long */
		VXB_M85XXPCI_SPIN_LOCK_ISR_TAKE(pDrvCtrl);
		CSR_WRITE_4(pInst, M85XXPCI_CONF_ADDR,
				vxbPciConfigBdfPack (busNo, deviceNo, funcNo) |
				(offset & 0xfc) | 0x80000000 | ((offset & 0xf00) << 16));
		retValLong = CSR_READ_4(pInst, M85XXPCI_CONF_DATA);
		if (CSR_READ_4(pInst, M85XXPCI_ERR_DR))
			CSR_WRITE_4(pInst, M85XXPCI_ERR_DR, 0xFFFFFFFF);
		VXB_M85XXPCI_SPIN_LOCK_ISR_GIVE(pDrvCtrl);

		*(UINT32 *)pData = le32toh(retValLong);
		M85PCI_DEBUG_MSG(1, "\nRead(): 0x%x\n",
				*(UINT32*)pData, 2, 3, 4, 5, 6);

		break;

	default:
		retStat = ERROR;
		break;
	}

	return (retStat);
}


/*******************************************************************************
 *
 * m85xxPciMethodDevCfgWrite - write to the PCI configuration space
 *
 * This routine writes either a byte, word or a long word specified by
 * the argument <width>, to the PCI configuration space
 * This routine works around a problem in the hardware which hangs
 * PCI bus if device no 12 is accessed from the PCI configuration space.
 *
 * \NOMANUAL
 *
 * RETURNS: OK, or ERROR if this library is not initialized
 *
 * ERRNO
 */

LOCAL STATUS m85xxPciMethodDevCfgWrite
(
		VXB_DEVICE_ID pInst,	/* device info */
		PCI_HARDWARE * pPciDev,	/* PCI device info */
		UINT32	  offset,	/* offset into the configuration space */
		UINT32	  width,	/* width to write */
		UINT32	  data		/* data to write */
)
{
	M85XXPCI_DRV_CTRL * pDrvCtrl;
	UINT8		busNo    = pPciDev->pciBus;
	UINT8		deviceNo = pPciDev->pciDev;
	UINT8		funcNo   = pPciDev->pciFunc;

	/* PCI-Express errata RMW workaround should work for all PCI 
	 * Requires always to do 4 byte read/write  
	 */ 

	UINT32 data2;
	UINT32 mask;

	if ((busNo == 0) && (deviceNo == 0x1f))
		return (ERROR);

	/* Establish the required mask based on width and offset */

	switch(width)
	{
	case 1:
		mask=0xff;
		mask=~(mask << ((offset&0x3)*8));
		break;
	case 2:
		mask=0xffff;
		mask=~(mask << ((offset&0x3)*8));
		break;
	case 4:
		mask=0;
		break;
	default:
		return(ERROR);
	}

	if (m85xxPciMethodDevCfgRead (pInst, pPciDev,
			(offset & ~0x3), 4, &data2) == ERROR)
		return(ERROR);

	data2 &= mask;
	/* Overwite only part of word required */
	data2 |= data << ((offset & 0x3) * 8);	/* final value to write */

	offset = (offset & ~0x3); /* align by 32 bits */

	pDrvCtrl = pInst->pDrvCtrl;

	VXB_M85XXPCI_SPIN_LOCK_ISR_TAKE(pDrvCtrl);
	CSR_WRITE_4(pInst, M85XXPCI_CONF_ADDR,
			vxbPciConfigBdfPack (busNo, deviceNo, funcNo) |
			(offset & 0xfc) | 0x80000000 | ((offset & 0xf00) << 16));
	CSR_WRITE_4(pInst, M85XXPCI_CONF_DATA, htole32(data2));
	CSR_WRITE_4(pInst, M85XXPCI_ERR_DR, 0xFFFFFFFF);
	VXB_M85XXPCI_SPIN_LOCK_ISR_GIVE(pDrvCtrl);

	M85PCI_DEBUG_MSG(1, "\nWrite(): data 0x%x offset 0x%x\n",
			data, offset, 3, 4, 5, 6);   

	return (OK);
}

/*****************************************************************************
 *
 * m85xxPciConfigInfo - Returns the PCI config info ptr from bus controller.
 *
 * \NOMANUAL
 *
 * RETURNS: OK, or ERROR if this library is not initialized
 *
 * ERRNO
 */
LOCAL STATUS m85xxPciConfigInfo 
(
		VXB_DEVICE_ID pInst,
		char * pArgs
)
{
	M85XXPCI_DRV_CTRL *pDrvCtrl = pInst->pDrvCtrl;
	*(UINT32*)pArgs = (UINT32)pDrvCtrl->pPciConfig;

	return(OK);
}
/*****************************************************************************
 *
 * m85xxPciMSIProgram - Returns the converted vector address for MSI
 *
 * \NOMANUAL
 *
 * RETURNS: OK
 *
 * ERRNO
 */
LOCAL STATUS m85xxPciMSIProgram
(
		VXB_DEVICE_ID pDev,
		struct vxbIntDynaVecInfo *dynaVec
)
{
	VXB_DEVICE_ID busCtrlID = vxbDevParent(pDev);    
	M85XXPCI_DRV_CTRL *pDrvCtrl = busCtrlID->pDrvCtrl;

	/* routine expect reg offset from internal memory map base addr 
	 * we can then just or in the pci internal memory map offset 
	 */

	M85PCI_DEBUG_MSG(0, "\nConvert PIMMR data 0x%x conerted 0x%x\n",
			(int)dynaVec->vecAddr,dynaVec->vecAddr | (UINT64)pDrvCtrl->msiDataWorkaroundAddr,  /*pimmrBase,change by ff */
			3,4,5,6); 

	//    dynaVec->vecAddr |= (UINT64)((UINT32)pDrvCtrl->msiDataWorkaroundAddr);  /*pimmrBase);  change by ff*/
	dynaVec->vecAddr |= (UINT64)CCSBAR;   //add by ff 2011.07.06

	return (vxbPciMSIProgram (pDev,dynaVec));
}


/*****************************************************************************
 *
 * m85xxPciInterruptInfo - Returns the PCI interrupt info ptr from the bus 
 * controller.
 *
 * \NOMANUAL
 *
 * RETURNS: OK, or ERROR if this library is not initialized
 *
 * ERRNO
 */
LOCAL STATUS m85xxPciInterruptInfo 
(
		VXB_DEVICE_ID pInst,
		char * pArgs
)
{
	M85XXPCI_DRV_CTRL *pDrvCtrl = (M85XXPCI_DRV_CTRL *)pInst->pDrvCtrl;
	*(UINT32*)pArgs = (UINT32)pDrvCtrl->pIntInfo;

	return(OK);
}



/******************************************************************************
 *
 * m85xxPciRegister - register m85xxPci driver
 *
 *  We use a two-function method here, to delay registration of the driver
 *  during debug.  This should be left in place when the driver is released.
 *
 */

void m85xxPciRegister(void)
{
	vxbDevRegister((struct vxbDevRegInfo *)&m85xxPciDevRegistration);
}

/******************************************************************************
 *
 * m85xxPciLawBarFind - find whether LAWBAR allocation is available
 *
 */

LOCAL void m85xxPciLawBarFind
(
		VXB_DEVICE_ID pDev,
		M85XXPCI_DRV_CTRL * pDrvCtrl
)
{
	FUNCPTR func;

	M85PCI_DEBUG_MSG(100,"m85xxPciLawBarFind(0x%08x, 0x%08x): called\n",
			(UINT32)pDev,(UINT32)pDrvCtrl,3,4,5,6);

	func = vxbDevMethodGet(pDev, (UINT32)&m85xxLawBarAlloc_desc[0]);
	if (func != NULL)
	{
		M85PCI_DEBUG_MSG(100,
				"m85xxPciLawBarFind(): found method from 0x%08x\n",
				(UINT32)pDev, 2,3,4,5,6);

		pDrvCtrl->lawBarAvailable = TRUE;
	}
}

/******************************************************************************
 *
 * m85xxPciBridgeHwInit - initialize PCI bridge hardware
 *
 *  There's an ordering requirement between this driver and the CCSR driver.
 *  The CCSR driver needs to be present in order for the LAWBAR allocation
 *  to succeed.  So we break out certain initialization functionality, and
 *  check whether the LAWBAR allocation method is available.
 *
 */

LOCAL void m85xxPciBridgeHwInit
(
		VXB_DEVICE_ID	pInst,
		int			from
)
{
	M85XXPCI_DRV_CTRL * pDrvCtrl = pInst->pDrvCtrl;

	M85PCI_DEBUG_MSG(100,"m85xxPciInstInit2(): called\n",1,2,3,4,5,6);

	if ( ( pDrvCtrl->initDone != TRUE ) && ( pDrvCtrl->lawBarAvailable == TRUE ) )
	{
		/* book-keeping */

		pDrvCtrl->initDone = TRUE;


		/* initialize the host bridge */

		M85PCI_DEBUG_MSG(100,"m85xxPciBridgeHwInit(0x%08x,%d): initializing bridge hw\n",
				(UINT32)pInst,from,3,4,5,6);

		if ( m85xxPciBridgeInit(pInst) != OK )
		{
			M85PCI_DEBUG_MSG(1, "m85xxPciBridgeHwInit(0x%08x,%d): error in HW init\n",
					(UINT32)pInst,from,3,4,5,6); 
			return;
		}
		/* announce that there's a PCI bus */

		vxbBusAnnounce(pInst, VXB_BUSID_PCI);

		M85PCI_DEBUG_MSG(100,"m85xxPciBridgeHwInit(0x%08x,%d): announce PCI to VxBus\n",
				(UINT32)pInst,from,3,4,5,6);
	}
}

/******************************************************************************
 *
 * m85xxPciInstInit - initialize m85xxPci device
 *
 * This is the m85xxPci initialization routine.
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

LOCAL void m85xxPciInstInit
(
		VXB_DEVICE_ID pInst
)
{
	STATUS stat;
	M85XXPCI_DRV_CTRL * pDrvCtrl;
	struct hcfDevice *  pHcf;

	/* allocate pDrvCtrl */

	pDrvCtrl = (M85XXPCI_DRV_CTRL *)hwMemAlloc(sizeof(*pDrvCtrl));

	if (pDrvCtrl == NULL)
		return;

	vxbRegMap (pInst, 0, &pDrvCtrl->handle);
	pDrvCtrl->regBase = pInst->pRegBase[0];


	pDrvCtrl->initDone = FALSE;
	pDrvCtrl->lawBarAvailable = FALSE;
	pDrvCtrl->pInst = pInst;

	/* magic number = "m85xxPci" */

	pDrvCtrl->magicNumber1 = 0x6D383578;
	pDrvCtrl->magicNumber2 = 0x78506369;


	/* extract BSP information */

	pHcf = (struct hcfDevice *)hcfDeviceGet(pInst);
	pDrvCtrl->pHcf = pHcf;

	/*
	 * resourceDesc {
	 * The mem32Addr resource specifies the
	 * 32-bit prefetchable memory address pool base address. }
	 */

	stat = devResourceGet(pHcf, "mem32Addr", HCF_RES_ADDR, &pDrvCtrl->mem32Addr);

	/*
	 * resourceDesc {
	 * The mem32Size resource specifies the
	 * 32-bit prefetchable memory address pool size. }
	 */

	stat = devResourceGet(pHcf, "mem32Size", HCF_RES_INT, (void *)&pDrvCtrl->mem32Size);

	/*
	 * resourceDesc {
	 * The memIo32Addr resource specifies the
	 * 32-bit non-prefetchable memory address pool base address. }
	 */

	stat = devResourceGet(pHcf, "memIo32Addr", HCF_RES_ADDR, &pDrvCtrl->memIo32Addr);

	/*
	 * resourceDesc {
	 * The memIo32Size resource specifies the
	 * 32-bit non-prefetchable memory address pool size. }
	 */

	stat = devResourceGet(pHcf, "memIo32Size", HCF_RES_INT, (void *)&pDrvCtrl->memIo32Size);

	/*
	 * resourceDesc {
	 * The io32Addr resource specifies the
	 * 32-bit I/O address pool base address. }
	 */

	stat = devResourceGet(pHcf, "io32Addr", HCF_RES_ADDR, &pDrvCtrl->io32Addr);

	/*
	 * resourceDesc {
	 * The io32Size resource specifies the
	 * 32-bit I/O address pool size. }
	 */

	stat = devResourceGet(pHcf, "io32Size", HCF_RES_INT, (void *)&pDrvCtrl->io32Size);

	/*
	 * resourceDesc {
	 * The pimmrBase resource specifies the
	 * PIMMR base address. }
	 */

	stat = devResourceGet(pHcf, "pimmrBase", HCF_RES_ADDR, &pDrvCtrl->pimmrBase);

	/*
	 * resourceDesc {
	 * The mstrMemBus resource specifies the
	 * address on CPU from PCI buss. }
	 */

	stat = devResourceGet(pHcf, "mstrMemBus", HCF_RES_ADDR, &pDrvCtrl->mstrMemBus);

	/*
	 * resourceDesc {
	 * The lclMemAddr resource specifies the
	 * local memory address on the BSP. }
	 */

	stat = devResourceGet(pHcf, "lclMemAddr", HCF_RES_ADDR, &pDrvCtrl->lclMemAddr);

	/*
	 * resourceDesc {
	 * The lclMemMapSize resource specifies the
	 * mappable local memory size on the BSP. }
	 */

	stat = devResourceGet(pHcf, "lclMemMapSize", HCF_RES_INT, (void *)&pDrvCtrl->lclMemMapSize);

	/*
	 * resourceDesc {
	 * The tgtIf resource specifies the
	 * target interface setting for [8-11] bit on LAWAR. }
	 */

	stat = devResourceGet(pHcf, "tgtIf", HCF_RES_INT, (void *)&pDrvCtrl->tgtIf);

	/*
	 * resourceDesc {
	 * The owAttrMem resource specifies the
	 * outbound attributes register setting for prefetchable memory. }
	 */

	stat = devResourceGet(pHcf, "owAttrMem", HCF_RES_INT, (void *)&pDrvCtrl->owAttrMem);

	/*
	 * resourceDesc {
	 * The owAttrMemIo resource specifies the
	 * outbound attributes register setting for non-prefetchable memory. }
	 */

	stat = devResourceGet(pHcf, "owAttrMemIo", HCF_RES_INT, (void *)&pDrvCtrl->owAttrMemIo);

	/*
	 * resourceDesc {
	 * The owAttrMem resource specifies the
	 * outbound attributes register setting for I/O. }
	 */

	stat = devResourceGet(pHcf, "owAttrIo", HCF_RES_INT, (void *)&pDrvCtrl->owAttrIo);

	/*
	 * resourceDesc {
	 * The iwAttr resource specifies the
	 * inbound attributes. }
	 */

	stat = devResourceGet(pHcf, "iwAttr", HCF_RES_INT, (void *)&pDrvCtrl->iwAttr);

	/*
	 * resourceDesc {
	 * The singleLawBar resource specifies the
	 * LawBar is single or multiple. }
	 */

	stat = devResourceGet(pHcf, "singleLawBar", HCF_RES_INT, (void *)&pDrvCtrl->singleLawBar);

	/*
	 * resourceDesc {
	 * The singleLawBarSize resource specifies the
	 * single LawBar size. }
	 */

	stat = devResourceGet(pHcf, "singleLawBarSize", HCF_RES_INT, (void *)&pDrvCtrl->singleLawBarSize);

	/*
	 * resourceDesc {
	 * The maxBusSet resource specifies the
	 * highest sub-bus number. }
	 */

	stat = devResourceGet(pHcf, "maxBusSet", HCF_RES_INT, (void *)&pDrvCtrl->pciMaxBus);

	/*
	 * resourceDesc {
	 * The pciExpressHost resource specifies the
	 * PCI Express Host is enabled or not. }
	 */

	stat = devResourceGet(pHcf, "pciExpressHost", HCF_RES_INT, (void*)&pDrvCtrl->pciExpressHost);

	/*
	 * resourceDesc {
	 * The autoConfig resource specifies whether
	 * Auto Configuration is enabled or not. }
	 */

	stat = devResourceGet(pHcf, "autoConfig", HCF_RES_INT, (void*)&pDrvCtrl->autoConfig);

	/*
	 * resourceDesc {
	 * The msiEnable resource specifies the
	 * MSI is enabled or not. }
	 */

	stat = devResourceGet(pHcf, "msiEnable", HCF_RES_INT, (void*)&pDrvCtrl->msiEnable);

	/*
	 * resourceDesc {
	 * The dynamicInterrupts resource specifies the
	 * Dynamic Interrupts are enabled or not. }
	 */

	stat = devResourceGet(pHcf, "dynamicInterrupts", HCF_RES_INT, (void*)&pDrvCtrl->dynamicInterrupts);

	/*
	 * resourceDesc {
	 * The msiAddress resource specifies the
	 * MSI base address. }
	 */

	stat = devResourceGet(pHcf, "msiAddress", HCF_RES_INT, (void*)&pDrvCtrl->msiAddress);

	/*
	 * resourceDesc {
	 * The msiData resource specifies the
	 * MSI data. }
	 */

	stat = devResourceGet(pHcf, "msiData", HCF_RES_INT, (void*)&pDrvCtrl->msiData);

	/*
	 * resourceDesc {
	 * The msiDataWorkaround resource specifies the
	 * MSI data workaround is required or not. }
	 */

	stat = devResourceGet(pHcf, "msiDataWorkaround", HCF_RES_INT, (void*)&pDrvCtrl->msiDataWorkaround);

	/*
	 * resourceDesc {
	 * The msiDataWorkaroundAddr resource specifies the
	 * MSI data workaround base address. }
	 */

	stat = devResourceGet(pHcf, "msiDataWorkaroundAddr", HCF_RES_INT, (void*)&pDrvCtrl->msiDataWorkaroundAddr);

	/*
	 * resourceDesc {
	 * The extIntMap resource specifies the
	 * address of the PCI interrupt mapping table. }
	 */

	stat = devResourceGet(pHcf, "extIntMap", HCF_RES_ADDR, (void*)&pDrvCtrl->extIntMap);

	/*
	 * resourceDesc {
	 * The extIntMap resource specifies the
	 * size of the PCI interrupt mapping table. }
	 */

	stat = devResourceGet(pHcf, "extIntMapSize", HCF_RES_INT, (void*)&pDrvCtrl->extIntMapSize);

	/*
	 * resourceDesc {
	 * The pciEx2ErrataFlag resource specifies the
	 * flag to use workaround for PCI-Express EX2 Errata. }  
	 */

	stat = devResourceGet(pHcf, "pciEx2ErrataFlag",HCF_RES_INT,(void*)&pDrvCtrl->pciEx2ErrataFlag);

	pInst->pDrvCtrl = pDrvCtrl;

	pDrvCtrl->bridgeInitFailed = FALSE;

	if ( pM85xxPciDrvCtrl0 == NULL )
		pM85xxPciDrvCtrl0 = pDrvCtrl;


	/* initialize PCI configuration library */
	pDrvCtrl->pIntInfo = (struct vxbPciInt *)hwMemAlloc(sizeof(struct vxbPciInt));
	if(pDrvCtrl->pIntInfo == NULL)
		return;

	vxbPciIntLibInit(pDrvCtrl->pIntInfo);

	pDrvCtrl->pPciConfig = (struct vxbPciConfig*)hwMemAlloc(sizeof(struct vxbPciConfig));
	if(pDrvCtrl->pPciConfig == NULL)
		return;

	SPIN_LOCK_ISR_INIT (&pDrvCtrl->spinlockIsr, 0);

	if (vxbPciConfigLibInit(pDrvCtrl->pPciConfig,
			pDrvCtrl->pciMaxBus) != OK)
		return;

	/* initialize PCI interrupt library */

	pInst->pMethods = &m85xxPci_methods[0];


}

/******************************************************************************
 *
 * m85xxPciInstInit2 - initialize m85xxPci device
 *
 */


UINT8 getPciBar1Addr(UINT32 *pBar1Addr)
{

	UINT32 value,i,barAddr;
	UINT8 busNo[1]= {8};
	UINT8 count = 0;

	for(i=0;i<1;i++)
	{
		value = 0x80000014|(busNo[i]<<16);
		*(UINT32*)PCI_CFG_ADR_REG = value;
		while(*(UINT32*)PCI_CFG_ADR_REG != value) *(UINT32*)PCI_CFG_ADR_REG = value;
		barAddr = LONGSWAP(*(UINT32*)PCI_CFG_DATA_REG);	
		printf("barAddr =0x%x\n",barAddr);
		if(barAddr != 0xffffffff)
		{
			*(UINT32*)0x20000000 = barAddr;
			*(pBar1Addr+count) = barAddr;
		}
		count++;
	}
	return count;
}

LOCAL void m85xxPciInstInit2
(
		VXB_DEVICE_ID pInst
)
{
	UINT32 i;
	M85XXPCI_DRV_CTRL * pDrvCtrl = pInst->pDrvCtrl;
	/*
	 * MMU is already enabled when it comes here and safe to use spinlock
	 * in SMP.
	 */

	VXB_M85XXPCI_SPIN_LOCK_READY;

	/* check whether LAWBARs are available */

	vxbDevIterate((FUNCPTR)m85xxPciLawBarFind, (void *)pDrvCtrl, 
			VXB_ITERATE_INSTANCES);

	if ( pDrvCtrl->lawBarAvailable == TRUE ) 
	{
		m85xxPciBridgeHwInit(pInst, 1);
		if(pDrvCtrl->bridgeInitFailed==TRUE)
			return;
		if((TEMP&0x0100)==0)
		{    
			if(pDrvCtrl->autoConfig==TRUE)
			{
				UINT8 count/*,i*/;
				UINT32 barAddr[3];
				ns16550Pollprintf("pci auto config....");
				vxbPciAutoConfig(pInst);
				count = getPciBar1Addr(&barAddr[0]);	 
				ns16550Pollprintf("done\n");
				for(i=0;i<count;i++) *(UINT32*)barAddr[i] = 0x5a5a5a5a;	

			}	
		}    
		vxbPciBusTypeInit(pInst);

	}
}



/******************************************************************************
 *
 * m85xxPciInstConnect - connect m85xxPci device to I/O system
 *
 * Nothing to do here.  We want serial channels available for output reasonably
 * early during the boot process.  Because this is a serial channel, we connect
 * to the I/O system in m85xxPciInstInit2() instead of here.
 *
 * RETURNS: N/A
 *
 * ERRNO
 */

LOCAL void m85xxPciInstConnect
(
		VXB_DEVICE_ID pInst
)
{

	M85PCI_DEBUG_MSG(1,"m85xxPciInstConnect(): called\n",1,2,3,4,5,6);


}


#ifdef M85XX_DEBUG_ENABLE

/******************************************************************************
 *
 * m85xxPcipDrvCtrlShow - show pDrvCtrl for m85xxPci bus controller
 *
 */

int m85xxPcipDrvCtrlShow
(
		VXB_DEVICE_ID pInst
)
{
	M85XXPCI_DRV_CTRL *	pDrvCtrl;
	int				i;
	union
	{
		UINT32 i[2];
		char magic[10];
	}			magic;

	if ( pInst == NULL )
		pInst = pM85xxPciDrvCtrl0->pInst;
	pDrvCtrl = pInst->pDrvCtrl;

	printf("pDrvCtrl @ 0x%08x\n", pDrvCtrl);

	printf("    pInst @ 0x%08x\n", pDrvCtrl->pInst);
	printf("    pHcf @ 0x%08x\n", pDrvCtrl->pHcf);

	bzero(magic.magic,10);
	magic.i[0] = pDrvCtrl->magicNumber1;
	magic.i[1] = pDrvCtrl->magicNumber2;
	printf("    magic = \"%s\"\n", &magic.magic[0]);

	printf("    mem   @ 0x%08x, size %d (0x%08x)\n",
			pDrvCtrl->mem32Addr, pDrvCtrl->mem32Size,
			pDrvCtrl->mem32Size);

	printf("    memIO @ 0x%08x, size %d (0x%08x)\n",
			pDrvCtrl->memIo32Addr, pDrvCtrl->mem32Size,
			pDrvCtrl->memIo32Size);

	printf("    io32  @ 0x%08x, size %d (0x%08x)\n",
			pDrvCtrl->io32Addr, pDrvCtrl->mem32Size,
			pDrvCtrl->io32Size);

	printf("    pimmr @ 0x%08x\n", pDrvCtrl->pimmrBase);
	printf("    mstrMemBus @ 0x%08x\n", pDrvCtrl->mstrMemBus);
	printf("    lclMemAddr @ 0x%08x\n", pDrvCtrl->lclMemAddr);
	printf("    lclMemMapSize = 0x%08x\n", pDrvCtrl->lclMemMapSize);
	printf("    autoConfig = 0x%08x\n", pDrvCtrl->autoConfig);
	printf("    pciExpressHost = 0x%08x\n", pDrvCtrl->pciExpressHost);


	printf("    LAWBAR info:\n");
	for ( i = 0 ; i < 3 ; i++ )
	{
		printf("       lawbar[%d] @ 0x%08x\n",
				pDrvCtrl->lawbarIndex[i], pDrvCtrl->lawbar[i]);
		printf("       lawar[%d] @  0x%08x\n",
				pDrvCtrl->lawbarIndex[i], pDrvCtrl->lawar[i]);
	}

	printf("    flags: LAWBAR available: %s\n",
			pDrvCtrl->lawBarAvailable ? "TRUE" : "FALSE");
	printf("           Init done:        %s\n",
			pDrvCtrl->initDone ? "TRUE" : "FALSE"); 

	return(0);
}

#endif /* M85XX_DEBUG_ENABLE */


/*************************************************************************
 *
 * m85xxPciEncodedSizeGet - get size encoding
 *
 *  This function converts a standard size to the encoded form needed
 *  by the m85xxPci device for writing into size registers.  While the
 *  device supports sizes up to 4GB, this function supports sizes only
 *  up to 1GB.
 */

LOCAL UINT32 m85xxPciEncodedSizeGet(UINT32 size)
{
	switch (size)
	{
	case ( 4 * 1024 ):
	    		return(M85XXPCI_ENCODED_SIZE_4KB);
	case ( 8 * 1024 ):
            		return(M85XXPCI_ENCODED_SIZE_8KB);
	case ( 16 * 1024 ):
            		return(M85XXPCI_ENCODED_SIZE_16KB);
	case ( 32 * 1024 ):
            		return(M85XXPCI_ENCODED_SIZE_32KB);
	case ( 64 * 1024 ):
            		return(M85XXPCI_ENCODED_SIZE_64KB);
	case ( 128 * 1024 ):
            		return(M85XXPCI_ENCODED_SIZE_128KB);
	case ( 256 * 1024 ):
            		return(M85XXPCI_ENCODED_SIZE_256KB);
	case ( 512 * 1024 ):
            		return(M85XXPCI_ENCODED_SIZE_512KB);
	case ( 1 * 1024 * 1024 ):
            		return(M85XXPCI_ENCODED_SIZE_1MB);
	case ( 2 * 1024 * 1024 ):
            		return(M85XXPCI_ENCODED_SIZE_2MB);
	case ( 4 * 1024 * 1024 ):
            		return(M85XXPCI_ENCODED_SIZE_4MB);
	case ( 8 * 1024 * 1024 ):
            		return(M85XXPCI_ENCODED_SIZE_8MB);
	case ( 16 * 1024 * 1024 ):
            		return(M85XXPCI_ENCODED_SIZE_16MB);
	case ( 32 * 1024 * 1024 ):
            		return(M85XXPCI_ENCODED_SIZE_32MB);
	case ( 64 * 1024 * 1024 ):
            		return(M85XXPCI_ENCODED_SIZE_64MB);
	case ( 128 * 1024 * 1024 ):
            		return(M85XXPCI_ENCODED_SIZE_128MB);
	case ( 256 * 1024 * 1024 ):
            		return(M85XXPCI_ENCODED_SIZE_256MB);
	case ( 512 * 1024 * 1024 ):
            		return(M85XXPCI_ENCODED_SIZE_512MB);
	case ( 1 * 1024 * 1024 * 1024 ):
            		return(M85XXPCI_ENCODED_SIZE_1GB);
	/*
    case ( 2 * 1024 * 1024 * 1024 ):
            return(M85XXPCI_ENCODED_SIZE_2GB);
	 */
	default:
		return(0);
	}
}

/*************************************************************************
 *
 * m85xxPciBridgeInit - initialize PCI bridge
 *
 * This function performs all the initialisation required for the
 * Bridge/Interrupts/PCI Bus to function. It does some
 * low level processor initialisation which might normally be done in
 * romInit as it is optional to do use this and shows what the core
 * changes required to bring up the bridge.
 *
 * RETURNS : NONE
 *
 */
LOCAL STATUS m85xxPciBridgeInit(VXB_DEVICE_ID pInst)
{
	M85XXPCI_DRV_CTRL * pDrvCtrl = pInst->pDrvCtrl;
	UINT32	encodedSize;
	struct m85xxLawBarInfo      lawBarInfo;
	STATUS	status;
	UINT8 tempByte;
	UINT16 tempWord;
	volatile UINT32 reg;
#ifndef	VXB_LEGACY_ACCESS
	PCI_HARDWARE pciDev;
	PCI_HARDWARE * pPciDev = (PCI_HARDWARE *) &pciDev;
	pPciDev->pciBus  = 0;
	pPciDev->pciDev  = 0;
	pPciDev->pciFunc = 0;
#endif	/* !VXB_LEGACY_ACCESS */

	M85PCI_DEBUG_MSG(100,
			"m85xxPciBridgeInit(): allocating LawBar for mem32\n",1,2,3,4,5,6);

	CSR_WRITE_4(pInst, 0x0c,0x80FFFFFF);
	/* Clear all errors */

	CSR_WRITE_4(pInst, M85XXPCI_ERR_DR, 0xFFFFFFFF);
	M85XXPCI_CFG_READ (pInst, 0x08, 4, TEMP); 

	/* hod_off boot mode ,delete in normal boot mode  */
	if(TEMP&0x0100)
	{	
		*(UINT32*)(CCSBAR+0xca8)=PCI_LAW_BASE >> LAWBAR_ADRS_SHIFT;
		*(UINT32*)(CCSBAR+0xcb0)=LAWAR_ENABLE | LAWAR_TGTIF_PCIEX1| LAWAR_SIZE_512MB;
		//ns16550Pollprintf("In m85xxPciBridgeInit set pci_law\n");
		*(UINT32*)(CCSBAR+0xcc8)=0xa0000000 >> LAWBAR_ADRS_SHIFT;
			*(UINT32*)(CCSBAR+0xcd0)=LAWAR_ENABLE | LAWAR_TGTIF_PCIEX1| LAWAR_SIZE_256MB;
//				 *(UINT32*)(CCSBAR+0xcc8)=0x60000;
//				 *(UINT32*)(CCSBAR+0xcd0)=0x8000001b; 

		return (OK);
	}
	if (pDrvCtrl->singleLawBar==FALSE)
	{

		/*
		 * Allocate LAWBAR for MEM32 and set adrs/size/if,
		 * using vxbResourceFind()
		 */

		lawBarInfo.index = -1;
		lawBarInfo.baseAdrs = pDrvCtrl->mem32Addr;
		lawBarInfo.size = pDrvCtrl->mem32Size;

		if (pDrvCtrl->tgtIf == -1)
			lawBarInfo.interface = LAWAR_TGTIF_PCI;
		else
			lawBarInfo.interface = pDrvCtrl->tgtIf;

		status = vxbResourceFind (pInst, (UINT32)&m85xxLawBarAllocSet_desc[0],
				(char *)&lawBarInfo, 0);

		if (status != OK)
		{
			M85PCI_DEBUG_MSG(1, "m85xxPci: can't get LAWBAR for mem32\n",
					1,2,3,4,5,6);
			return (ERROR);
		}

		pDrvCtrl->lawbar[0] = lawBarInfo.lawbar;
		pDrvCtrl->lawar[0] = lawBarInfo.lawar;
		pDrvCtrl->lawbarIndex[0] = lawBarInfo.index;

		/*
		 * Allocate LAWBAR for MEMIO32 and set adrs/size/if,
		 * using vxbResourceFind()
		 */

		M85PCI_DEBUG_MSG(100,
				"m85xxPciBridgeInit(): allocating LawBar for memIo32\n",
				1,2,3,4,5,6);

		lawBarInfo.index = -1;
		lawBarInfo.baseAdrs = pDrvCtrl->memIo32Addr;
		lawBarInfo.size = pDrvCtrl->memIo32Size;

		status = vxbResourceFind(pInst, (UINT32)&m85xxLawBarAllocSet_desc[0],
				(char *)&lawBarInfo, 0);

		if (status != OK)
		{
			M85PCI_DEBUG_MSG(1, "m85xxPci: can't get LAWBAR for memIo32\n",
					1,2,3,4,5,6);
			return (ERROR);
		}

		pDrvCtrl->lawbar[1] = lawBarInfo.lawbar;
		pDrvCtrl->lawar[1] = lawBarInfo.lawar;
		pDrvCtrl->lawbarIndex[1] = lawBarInfo.index;

		/*
		 * Allocate LAWBAR for io32 and set adrs/size/if,
		 * using vxbResourceFind()
		 */

		M85PCI_DEBUG_MSG(100,
				"m85xxPciBridgeInit(): allocating LawBar for io32\n", 1,2,3,4,5,6);

		lawBarInfo.index = -1;
		lawBarInfo.baseAdrs = pDrvCtrl->io32Addr;
		lawBarInfo.size = pDrvCtrl->io32Size;

		status = vxbResourceFind (pInst, (UINT32)&m85xxLawBarAllocSet_desc[0],
				(char *)&lawBarInfo, 0);

		if ( status != OK )
		{
			M85PCI_DEBUG_MSG(1, "m85xxPci: can't get LAWBAR for io32\n",
					1,2,3,4,5,6);
			return (ERROR);
		}

		pDrvCtrl->lawbar[2] = lawBarInfo.lawbar;
		pDrvCtrl->lawar[2] = lawBarInfo.lawar;
		pDrvCtrl->lawbarIndex[2] = lawBarInfo.index;
	}
	else
	{
		/* Allocate just one LAWBAR for everything */
		/* add by ff-------2009.11.04---begin*/    	
		lawBarInfo.index = -1;
		lawBarInfo.baseAdrs = pDrvCtrl->memIo32Addr;     
		lawBarInfo.size = 0x20000000;   

		*(UINT32*)(CCSBAR+0xcc8)=0xa0000000 >> LAWBAR_ADRS_SHIFT;	
		*(UINT32*)(CCSBAR+0xcd0)= LAWAR_ENABLE | LAWAR_TGTIF_PCIEX1| LAWAR_SIZE_256MB;


		if (pDrvCtrl->tgtIf == -1)
			lawBarInfo.interface = LAWAR_TGTIF_PCI;
		else
			lawBarInfo.interface = pDrvCtrl->tgtIf;

		status = vxbResourceFind (pInst, (UINT32)&m85xxLawBarAllocSet_desc[0],
				(char *)&lawBarInfo, 0);

		if (status != OK)
		{
			M85PCI_DEBUG_MSG(1, "m85xxPci: can't get LAWBAR for mem32\n",
					1,2,3,4,5,6);
			return(ERROR);
		}

		pDrvCtrl->lawbar[0] = lawBarInfo.lawbar;
		pDrvCtrl->lawar[0] = lawBarInfo.lawar;
		pDrvCtrl->lawbarIndex[0] = lawBarInfo.index;
	}


	if (pDrvCtrl->pciExpressHost == TRUE)
	{
		/* Check PCIe link status. If no link exit as config cycle will hang */

		M85XXPCI_CFG_READ (pInst, M85XXPCI_LTSSM_REG, 0x2, tempWord);

		/* 
		 * This is a workaround to bring the PCIe link up between the controller
		 * and PEX device after power-cycle or reset for Errata PCI-EX2.
		 * (For more information, refer to target.ref of ads8572.) 
		 */

		if( (tempWord == M85XXPCI_LINK_DETECT_ACTIVE) && 
				(pDrvCtrl->pciEx2ErrataFlag == TRUE) )
		{
			/* Reset the pcie controller */

			reg = CSR_READ_4(pInst, 0xF00 );
			CSR_WRITE_4(pInst, 0xF00, (reg | 0x08000000) );

			/* Wait 1 msec */

			vxbMsDelay(1);

			/* Deassert the Reset */

			CSR_WRITE_4(pInst, 0xF00, (reg & ~0x08000000) );

			/* Wait 1sec for Link/Retraining/negotiation settles. */

			vxbMsDelay(1000);

			/* Read the LTSSM register again */

			M85XXPCI_CFG_READ (pInst, M85XXPCI_LTSSM_REG, 0x2, tempWord);
		}

		if (tempWord != M85XXPCI_LINK_L0 )
		{
			pDrvCtrl->bridgeInitFailed = TRUE;
			return (ERROR);
		}

	}


	/* Set up register windows */	
	CSR_WRITE_4(pInst, M85XXPCI_OB_TRANS_ADRS_REG0,
			((UINT32)pDrvCtrl->mem32Addr >> 12) & 0xFFFFF);
	if (pDrvCtrl->owAttrMem == 0)
		CSR_WRITE_4(pInst, M85XXPCI_OB_ATTR_REG0,
				PCI_WINDOW_ENABLE_BIT | PCI_OUT_ATTR_RTT_MEM |
				PCI_OUT_ATTR_WTT_MEM | encodedSize);
	else
		CSR_WRITE_4(pInst, M85XXPCI_OB_ATTR_REG0,
				pDrvCtrl->owAttrMem | encodedSize);

	CSR_WRITE_4(pInst, M85XXPCI_OB_BASE_ADRS_REG1,
			((UINT32)0x80000000 >> 12) & 0xFFFFF);
	CSR_WRITE_4(pInst, M85XXPCI_OB_TRANS_ADRS_REG1,
			((UINT32)0x80000000 >> 12) & 0xFFFFF);
	CSR_WRITE_4(pInst, M85XXPCI_OB_ATTR_REG1, 0x8004401c); 

	CSR_WRITE_4(pInst, M85XXPCI_OB_BASE_ADRS_REG2,
			((UINT32)0xa0000000 >> 12) & 0xFFFFF);					

	CSR_WRITE_4(pInst, M85XXPCI_OB_TRANS_ADRS_REG2,
			((UINT32)0xa0000000>>12) & 0xFFFFF);

	CSR_WRITE_4(pInst, M85XXPCI_OB_ATTR_REG2, 0x8004401b);

	encodedSize = m85xxPciEncodedSizeGet (pDrvCtrl->mem32Size);
	/*
	 * Switch on the inbound translation windows
	 * Workaround is for PIC7 errata documented for 8548,
	 * but also affects 8544 and 8641
	 */

	M85XXPCI_CFG_WRITE (pInst, PCI_CFG_BASE_ADDRESS_0, 4,
			pDrvCtrl->msiDataWorkaroundAddr);

	CSR_WRITE_4(pInst, M85XXPCI_IB_BASE_ADRS_REG1,
			((UINT32)CPUA_BAR1_ADDR >> 12) & 0xFFFFF);	
	CSR_WRITE_4(pInst, M85XXPCI_IB_TRANS_ADRS_REG1,
			((PCIE_MAP_ADDR & 0xFFF00000) >> 12) & 0xFFFFF);     
	CSR_WRITE_4(pInst, M85XXPCI_IB_ATTR_REG1,
			PCI_WINDOW_ENABLE_BIT|PCI_IN_ATTR_TGI_LM|PCI_IN_ATTR_RTT_LM_READ_SNOOP|PCI_IN_ATTR_RTT_LM_WRITE_SNOOP|PCI_ATTR_WS_64M );	/* 0x80F55000|PCI_ATTR_WS_64M     modified by ff----091028*/

	CSR_WRITE_4(pInst, M85XXPCI_IB_BASE_ADRS_REG2,
			((UINT32)CPUA_BAR2_ADDR >> 12) & 0xFFFFF);	
	CSR_WRITE_4(pInst, M85XXPCI_IB_TRANS_ADRS_REG2,
			((FL_BASE_ADDR & 0xFFF00000) >> 12) & 0xFFFFF);     
	CSR_WRITE_4(pInst, M85XXPCI_IB_ATTR_REG2,
			PCI_WINDOW_ENABLE_BIT|PCI_IN_ATTR_TGI_LM|PCI_IN_ATTR_RTT_LM_READ_SNOOP|PCI_IN_ATTR_RTT_LM_WRITE_SNOOP|PCI_ATTR_WS_16M);	/* 0x80F55000|PCI_ATTR_WS_16M*/

	M85XXPCI_CFG_WRITE(pInst, 0x54, 4, 0x00002830);  /* payload  256Byte*/

	/* configure the bridge as bus master */

	tempWord = PCI_CMD_IO_ENABLE | PCI_CMD_MEM_ENABLE | PCI_CMD_MASTER_ENABLE;

	M85XXPCI_CFG_WRITE (pInst, PCI_CFG_COMMAND, 2, tempWord);

	/* PCI_EXPRESS only */

	if (pDrvCtrl->pciExpressHost == TRUE)
	{
		/* Make bus 1 visible in configuration space */
		/* subordinate = 0xff secondary = 0x01 primary = 0 */

		tempByte = 0;

		M85XXPCI_CFG_WRITE (pInst, PCI_CFG_PRIMARY_BUS, 1, tempByte);

		tempByte = 1;

		M85XXPCI_CFG_WRITE (pInst, PCI_CFG_SECONDARY_BUS, 1, tempByte);

		tempByte = 0xFF;

		M85XXPCI_CFG_WRITE (pInst, PCI_CFG_SUBORDINATE_BUS, 1, tempByte);
	}



	return(OK);
}

#ifdef	VXB_LEGACY_ACCESS
/*******************************************************************************
 *
 * m85xxPciAccessMethodOverride - method for PCI code to override access funcs
 *
 *  This routine is called by the PCI bus type code.  That code is responsible
 *  for creating downstream devices.  For each device created, the standard
 *  access functions are assigned, then this routine is called to allow the
 *  bus controller driver to override any functions that need to be non-standard.
 *
 *  For this device, the functions are the cfg read/write methods.
 */

LOCAL STATUS m85xxPciAccessMethodOverride
(
		VXB_DEVICE_ID devID,        /* device info */
		VXB_ACCESS_LIST * pAccess   /* access structure pointer */
)
{
	M85PCI_DEBUG_MSG(10, "m85xxPciAccessMethodOverride(0x%08x,0x%08x) called\n",
			(UINT32)devID, (UINT32)pAccess, 0,0,0,0);

	VXB_ASSERT(devID != NULL && pAccess != NULL, ERROR)

	pAccess->vxbDevControl = m85xxPciDevControl;

	return OK;
}
#endif	/* VXB_LEGACY_ACCESS */

/*******************************************************************************
 *
 * m85xxPciDevControl - device control routine
 *
 *  This routine handles manipulation of downstream devices, such as
 *  interrupt management.
 *
 */

LOCAL STATUS m85xxPciDevControl
(
		VXB_DEVICE_ID	devID,
		pVXB_DEVCTL_HDR	pBusDevControl
)
{ 
	VXB_DEVICE_ID busCtrlID;
	struct vxbIntDynaVecInfo dynaVec;
	M85XXPCI_DRV_CTRL *pDrvCtrl; 

	VXB_ASSERT(devID != NULL && pBusDevControl != NULL, ERROR)

	busCtrlID = vxbDevParent(devID);
	pDrvCtrl = ((M85XXPCI_DRV_CTRL *)busCtrlID->pDrvCtrl);

	switch ( pBusDevControl->vxbAccessId)
	{
	case VXB_ACCESS_INT_VEC_GET: 
	{
		pVXB_ACCESS_INT_VECTOR_GET	accessVectorGet;
		struct pciIntrEntry * pciIntInfo;
		pciIntInfo = (struct pciIntrEntry *)(devID->pIntrInfo);

		accessVectorGet = (pVXB_ACCESS_INT_VECTOR_GET) pBusDevControl; 

		/*
		 * Check if MSI is enabled and if device is MSI capable 
		 * while it is getting registered.
		 */			

		if ((pDrvCtrl->dynamicInterrupts==TRUE) && 
				(pDrvCtrl->msiEnable == TRUE) &&
				(vxbPciMSIIsCap (devID) == TRUE))
		{
			accessVectorGet->pIntVector = (VOIDFUNCPTR)VXB_INTR_DYNAMIC;
			return(OK);
		}

		if (pDrvCtrl->extIntMap != NULL)
		{
			struct vxbPciDevice *	pPciDev;
			int			devNo;
			int			intNum;

			pPciDev = (struct vxbPciDevice *)devID->pBusSpecificDevInfo;
			devNo  = pPciDev->pciDev;

			intNum = m85xxPciIntNumGet (devID, pBusDevControl, devNo);
			if (intNum == ERROR)
				return ERROR;

			accessVectorGet->pIntVector = (VOIDFUNCPTR)INUM_TO_IVEC(intNum);

			return(OK);
		}

		/* check if the index is valid */

		if (accessVectorGet->intIndex >= 255)
			return ERROR;

		accessVectorGet->pIntVector =
				(VOIDFUNCPTR)INUM_TO_IVEC(pciIntInfo->intVecInfo[accessVectorGet->intIndex].index);

		return(OK);
	}             

	case VXB_ACCESS_INT_CONNECT:
	{
		pVXB_ACCESS_INTERRUPT_INFO	accessIntrInfo;
		struct pciIntrEntry *	pInterruptInfo;
		STATUS			retVal;

		/* get appropriate structures */

		accessIntrInfo = (pVXB_ACCESS_INTERRUPT_INFO) pBusDevControl;
		pInterruptInfo = (struct pciIntrEntry *)(devID->pIntrInfo);


		/*
		 * check if the interrupt vector is set to a dynamic one; 
		 * This only happens when the device is already registered
		 * and checked for MSI enable and MSI capability.
		 */

		if (pInterruptInfo->intVecInfo[accessIntrInfo->intIndex].intVector
				== VXB_INTR_DYNAMIC)
		{
			dynaVec.isr = accessIntrInfo->pISR;
			dynaVec.pArg = accessIntrInfo->pParameter;

			retVal=ERROR;

			if(_func_vxbIntDynaConnect!=NULL)
				retVal = _func_vxbIntDynaConnect(devID,
						1,
						&dynaVec);


			if (retVal == OK)
				return(OK);

			/* Drop through to the original mechanism and prevent further 
		   attempts if function not available or fails */
			pDrvCtrl->dynamicInterrupts=FALSE; 

		}

		/* connect the interrupt */

		retVal = vxbPciIntConnect(devID,
				(VOIDFUNCPTR*)pInterruptInfo->intVecInfo[accessIntrInfo->intIndex].intVector,
				(VOIDFUNCPTR)accessIntrInfo->pISR,
				(int)accessIntrInfo->pParameter
		);

		return(retVal); 
	}

	case VXB_ACCESS_INT_ENABLE:
	{
		pVXB_ACCESS_INTERRUPT_INFO	accessIntrInfo;
		struct pciIntrEntry *	pInterruptInfo;
		STATUS			retVal;

		accessIntrInfo = (pVXB_ACCESS_INTERRUPT_INFO) pBusDevControl;
		pInterruptInfo = (struct pciIntrEntry *)(devID->pIntrInfo);

		/*
		 * check if the interrupt vector is set to a dynamic one; 
		 * This only happens when the device is already registered
		 * and checked for MSI enable and MSI capability.
		 */

		if (pInterruptInfo->intVecInfo[accessIntrInfo->intIndex].intVector
				== VXB_INTR_DYNAMIC )
		{
			retVal = vxbIntEnable(devID,
					0,
					(VOIDFUNCPTR)accessIntrInfo->pISR,
					(void*)accessIntrInfo->pParameter);
			return (retVal);
		}

		retVal = intEnable(IVEC_TO_INUM(pInterruptInfo->intVecInfo[accessIntrInfo->intIndex].intVector));

		return(retVal);
	}


	case VXB_ACCESS_INT_DISABLE:
	{
		pVXB_ACCESS_INTERRUPT_INFO	accessIntrInfo;
		struct pciIntrEntry *	pInterruptInfo;
		STATUS			retVal;

		accessIntrInfo = (pVXB_ACCESS_INTERRUPT_INFO) pBusDevControl;
		pInterruptInfo = (struct pciIntrEntry *)(devID->pIntrInfo);


		/*
		 * check if the interrupt vector is set to a dynamic one; 
		 * This only happens when the device is already registered
		 * and checked for MSI enable and MSI capability.
		 */

		if (pInterruptInfo->intVecInfo[accessIntrInfo->intIndex].intVector
				== VXB_INTR_DYNAMIC )
		{
			retVal = vxbIntDisable(devID,
					0,
					(VOIDFUNCPTR)accessIntrInfo->pISR,
					(void*)accessIntrInfo->pParameter);
			return(retVal);
		}

		/* disable the interrupt */

		retVal = intDisable(IVEC_TO_INUM(pInterruptInfo->intVecInfo[accessIntrInfo->intIndex].intVector));

		return(retVal);

	}

	case VXB_ACCESS_INT_ACKNOWLEDGE: 
		return OK;

	case VXB_ACCESS_INT_DISCONN:
		return(ERROR);

	default:
		return ERROR; 
	}

}
/*******************************************************************************
 *
 * m85xxPciIntNumGet - get int num for specified device
 *
 *  This routine looks through the interrupt table passed in as extIntMap and
 *  and returns the appropriate value, or ERROR, if the device number is not
 *  found.
 *
 */

LOCAL int m85xxPciIntNumGet
(
		VXB_DEVICE_ID	devID,
		pVXB_DEVCTL_HDR	pBusDevControl,
		int			devNo
)
{
	pVXB_ACCESS_INTERRUPT_INFO	accessIntrInfo;
	int l;
	UINT8 header;
#ifndef	VXB_LEGACY_ACCESS
	PCI_HARDWARE * pPciDev = (PCI_HARDWARE *) devID->pBusSpecificDevInfo;
#endif	/* !VXB_LEGACY_ACCESS */
	int pciIntSlots = pM85xxPciDrvCtrl0->extIntMapSize;
	int * pciIntMap = pM85xxPciDrvCtrl0->extIntMap;
	int * pPciInt;

	accessIntrInfo = (pVXB_ACCESS_INTERRUPT_INFO) pBusDevControl;

	if (accessIntrInfo->intIndex == 0)
	{
		for (pPciInt = pciIntMap, l = 0; l < pciIntSlots; pPciInt += 5, l++)
		{
			if (devNo == pPciInt[0])
				return pPciInt[accessIntrInfo->intIndex + 1];
		}
		return ERROR;
	} 
	else if (0 < accessIntrInfo->intIndex && accessIntrInfo->intIndex < 4)
	{
		M85XXPCI_CFG_READ(devID, PCI_CFG_HEADER_TYPE, 1, header);

		if ((header & PCI_HEADER_MULTI_FUNC) != PCI_HEADER_MULTI_FUNC)
		{
			M85PCI_DEBUG_MSG(1,"device is not multifunction , illegal intPin %d\n",accessIntrInfo->intIndex,2,3,4,5,6);
			return ERROR;
		}
		else
		{
			for (pPciInt = pciIntMap, l = 0; l < pciIntSlots; pPciInt += 5, l++)
			{
				if (devNo == pPciInt[0])
					return pPciInt[accessIntrInfo->intIndex + 1];
			}
			return ERROR;
		}
	}
	else
	{
		M85PCI_DEBUG_MSG(1,"illegal intPin %d, allowed 0,1,2,3\n",accessIntrInfo->intIndex,2,3,4,5,6);
		return ERROR;
	}
}

#ifndef BOOTROM_COMPILE
LOCAL STATUS getPciInst()
{
	pInst_pci = (VXB_DEVICE_ID)vxbInstByNameFind ("m85xxPci", 0);
	if(pInst_pci==NULL)
	{
		printf ("Get pInst failed\n");
		return ERROR;
	}
	pciCtrlHandler = vxbDevMethodGet(pInst_pci, DEVMETHOD_CALL(busCtlrDevCtlr));
	if( pciCtrlHandler == NULL )
	{
		logMsg("vxbDevMethodGet != OK!!!!\n",1,2,3,4,5,6);
		return (ERROR);
	}
	getpInstFlag = 1;
	return OK;
}

STATUS m85xxmsiIntSetup(int intIndex,VOIDFUNCPTR msiIsr,int pParam)
{
	VXB_DEVICE_ID pInst;	
	FUNCPTR     handler;
	pVXB_DEVCTL_HDR	pBusDevControl;
	pVXB_ACCESS_INTERRUPT_INFO	accessIntrInfo;
	struct pciIntrEntry *	pInterruptInfo;	 
//	 ns16550Pollprintf("1");
	if(getpInstFlag == 0)
	{
		if(getPciInst()==ERROR)
			return ERROR;
	}
	pInst = pInst_pci;
	handler = pciCtrlHandler;
//	ns16550Pollprintf("2");
	accessIntrInfo = (pVXB_ACCESS_INTERRUPT_INFO)pInst;
	pInterruptInfo = (struct pciIntrEntry *)(pInst->pIntrInfo);
//	ns16550Pollprintf("3");
	accessIntrInfo->pISR = msiIsr;	
	accessIntrInfo->pParameter = (void*)pParam;	//NULL;
	accessIntrInfo->header.vxbAccessId = VXB_ACCESS_INT_CONNECT;
	pBusDevControl = (pVXB_DEVCTL_HDR)accessIntrInfo;
//	ns16550Pollprintf("4");
	msiIndex = intIndex&0xff;
//	ns16550Pollprintf("41");
	pInterruptInfo->numVectors = 1;
//	ns16550Pollprintf("42");
	pInterruptInfo->intVecInfo[accessIntrInfo->intIndex].intVector = VXB_INTR_DYNAMIC;
//	ns16550Pollprintf("43");
	pInterruptInfo->intVecInfo[accessIntrInfo->intIndex].index = 0xff;
//	ns16550Pollprintf("44");
	pInst->pIntrInfo = (void*)pInterruptInfo;
	pInst->busID = 3;
//	ns16550Pollprintf("5");
	handler(pInst,pBusDevControl);	 
	return OK;
}
/******************************************************************************
 * m85xxSetInboundWindow -- set 8640D PCIe inbound window.
 * 
 * This routine set the pcie interface's inbound window for the 8640D chip.
 * 
 */

STATUS m85xxSetInboundWindow(UINT32 windNum,UINT32 pcieAddr,UINT32 transAddr,UINT32 size)
{
	VXB_DEVICE_ID pInst;
	UINT32 sizeType;
	
	pInst=(VXB_DEVICE_ID)vxbInstByNameFind("m85xxPci",0);

	switch(size)
	{
	case 0x1000:
		sizeType = PCI_ATTR_WS_4K;
		break;
	case 0x2000:
		sizeType = PCI_ATTR_WS_8K;
		break;
	case 0x4000:
		sizeType = PCI_ATTR_WS_16K;
		break;
	case 0x8000:
		sizeType = PCI_ATTR_WS_32K;
		break;
	case 0x10000:
		sizeType = PCI_ATTR_WS_64K;
		break;
	case 0x20000:
		sizeType = PCI_ATTR_WS_128K;
		break;
	case 0x40000:
		sizeType = PCI_ATTR_WS_256K;
		break;
	case 0x80000:
		sizeType = PCI_ATTR_WS_512K;
		break;
	case 0x100000:
		sizeType = PCI_ATTR_WS_1M;
		break;
	case 0x200000:
		sizeType = PCI_ATTR_WS_2M;
		break;
	case 0x400000:
		sizeType = PCI_ATTR_WS_4M;
		break;
	case 0x800000:
		sizeType = PCI_ATTR_WS_8M;
		break;
	case 0x1000000:
		sizeType = PCI_ATTR_WS_16M;
		break;
	case 0x2000000:
		sizeType = PCI_ATTR_WS_32M;
		break;
	case 0x4000000:
		sizeType = PCI_ATTR_WS_64M;
		break;
	case 0x8000000:
		sizeType = PCI_ATTR_WS_128M;
		break;
	case 0x10000000:
		sizeType = PCI_ATTR_WS_256M;
		break;
	case 0x20000000:
		sizeType = PCI_ATTR_WS_512M;
		break;
	case 0x40000000:
		sizeType = PCI_ATTR_WS_1G;
		break;
	case 0x80000000:
		sizeType = PCI_ATTR_WS_2G;
		break;
	default:
		sizeType = PCI_ATTR_WS_128M;
		break;
	}
	
	switch(windNum)
	{
	case 2:
		CSR_WRITE_4(pInst, M85XXPCI_IB_BASE_ADRS_REG2,
				((UINT32)pcieAddr >> 12) & 0xFFFFF);
		CSR_WRITE_4(pInst, M85XXPCI_IB_TRANS_ADRS_REG2,
				((transAddr) >> 12) & 0xFFFFF);  
		CSR_WRITE_4(pInst, M85XXPCI_IB_ATTR_REG2,
				PCI_WINDOW_ENABLE_BIT|PCI_IN_ATTR_TGI_LM|PCI_IN_ATTR_RTT_LM_READ_SNOOP|PCI_IN_ATTR_RTT_LM_WRITE_SNOOP|sizeType );	
		break;
	case 1:
		CSR_WRITE_4(pInst, M85XXPCI_IB_BASE_ADRS_REG1,
				((UINT32)pcieAddr >> 12) & 0xFFFFF);
		CSR_WRITE_4(pInst, M85XXPCI_IB_TRANS_ADRS_REG1,
				((transAddr) >> 12) & 0xFFFFF);  
		CSR_WRITE_4(pInst, M85XXPCI_IB_ATTR_REG1,
				PCI_WINDOW_ENABLE_BIT|PCI_IN_ATTR_TGI_LM|PCI_IN_ATTR_RTT_LM_READ_SNOOP|PCI_IN_ATTR_RTT_LM_WRITE_SNOOP|sizeType );	
		break;
	default:
		break;
	}
	return OK;
}


/*  MSIIR: bit24~26 SRS,select interrupt register 0~7;bit27~31 IBS ,select interrupt bit 0~31
 *   0x1f000000 :MSIR0,BIT 31;
 * intcode:
 * run in endpoint
 */
void m85xxMsiTrigger(int dstId ,int msiIntIndex)
{
	UINT32 msiRegAddr=0;
	UINT32 intVal=0;
	int cpuId = sysGetCpuID() ;
	if(cpuId == dstId)
		msiRegAddr = CCSBAR+0x41740;
	else
	{
		switch(dstId)
		{
		case 0:
			msiRegAddr = 0xa0041740;
			break;
		case 1:
			msiRegAddr = 0x80141740;
			break;	
		default:
			break;
		}		
	}
	intVal = ((((msiIntIndex%8)*2)<<4)|(msiIntIndex/8))<<24 ;
	*(UINT32*)msiRegAddr = intVal;
	//	*(UINT32*)msiRegAddr =  msiIntIndex << 24; 
}
#endif
