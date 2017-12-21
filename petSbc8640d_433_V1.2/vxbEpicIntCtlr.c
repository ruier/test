/* vxbEpicIntCtlr.c - Driver for Embedded Programmable Interrupt Controller */

/*
 * Copyright (c) 2006-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
03a,21sep08,l_z  Fix possible data structure conflict. (WIND00179352)
02z,14aug09,x_z  update EPIC driver to fix MPC8569 WHOAMI bug.
02y,17jul09,rgo  Handle multipled MSI interrupts with new method(CQ:145042)
02x,20feb09,pmr  individual pin triggering options.
02w,19feb09,pmr  fix init on core != 0 (CQ:156492).
02v,05jan09,b_m  add global timer group B support.
02u,08dec08,pmr  external proxy mode support
02u,18nov08,h_k  fixed MSI handler for concurrent interrupts. (CQ:142719)
                 cleaned up a compile warning in GNU.
02t,29oct08,pmr  add vxbEpicIntCtlrCpuDisable routine
02s,16sep08,tcr  add SV instrumentation
02r,04sep08,dtr  Non-vxbus chained interrupts are not enabled.
02q,27aug08,jpb  Renamed VSB header file
02p,29jul08,h_k  replaced sprintf() with a simple stack over write.
02o,23jul08,h_k  fixed text section overwrite. (CQ:128170)
                 fixed intDisconnet(). (CQ:125388)
                 cleaned up compiler warnings in GNU.
02n,18jul08,kk   added inclusion of private/vsbConfigP.h to pick up macros
02m,14jul08,dtr  Make dynamicInterruptTable local to driver.
02l,11jun08,pmr  resource documentation
02k,04jun08,dtr  Move regTable initialization to general init not boot cpu
                 init. Remove static IRQ0 definitions.
02j,02jun08,h_k  split ISR and IPI Emit portions out.
                 removed redundant zero clear after hwMemAlloc().
02h,22may08,jpb  Modifications for source build.  Renamed WV_INSTRUMENTATION
                 to _WRS_CONFIG_SV_INSTRUMENTATION, _WRS_VX_SMP to
                 _WRS_CONFIG_SMP.
02g,08may08,pmr  check dynamic interrupt function return values
02f,05may08,tor  update version
02i,08may08,pmr  check dynamic interrupt function return values
02h,30may08,pmr  flexible interrupt assignment
02g,05may08,tor  update version
02f,21apr08,pmr  initial e500mc support
02e,07apr08,h_k  removed isync.
02d,20mar08,dtr  Make driver partially compatible with Linux if guest OS.
02c,19mar08,h_k  enabled interrupt nesting in vxbSysEpicIntHandler().
                 (CQ:118761)
02b,04mar08,h_k  removed vxbRead/Write access. connected int handler to
                 exception vector table directly. changed from private IACK,
                 EOI and IPI dispatch egs to globals' to skip the cpu check.
02a,14feb08,h_k  optimized vxbSysEpicIntHandler().
01z,05feb08,h_k  fixed vxIpiDisconnect. (CQ:116800)
01y,20sep07,tor  VXB_VERSION_3
01x,05sep07,p_g  modifications done for ApiGen
01w,27jul07,dlk  Fix read of wrong CPU's IACK reg in vxbSysEpicIntHandler().
01v,02jul07,pmr  simplified int handling and other optimizations
                 change WindView macro to WV_INSTRUMENTATION (WIND00097442)
01u,10jul07,dtr  Add support for dynamic interrupt allocation.
01t,28jun07,pmr  fix int routing
01s,05jun07,pmr  make compilable standalone, rerouting and IPI priority
01r,11may07,tor  remove undef LOCAL
01q,10may07,pmr  fix includes
01p,23apr07,pmr  single CPU support
01o,16apr07,pch  move vxbEpicIntCtlr.h to target/h/hwif/intCtlr
01n,03apr07,tor  vxIpiLib support added
01m,19Mar07,tor  update intCtlr structs
01l,12mar07,kk   reinstate PCI and SMSG support
01k,12mar07,kk   backed out PCI and SMSG support temporarily, breaks SMP
01j,26feb07,dtr  Support Message Shared Interrupts.
01i,23feb07,pch  Set IPI used for CPC to highest priority; fix copy/paste error
		 that caused the "disable" driver method to enable instead.
01h,12Dec06,tor  Cleanup
01g,08Nov06,tor  port to VxBus
01f,01sep06,kk   added back _func_intConnect routines
01e,26jun06,mmi  add compile condition around IPI_CTRL_INIT and init statically
01d,05may05,mmi  Add vxbSysEpicIpiInit and replaced int with cpuset_t type
01c,05may05,mig  Add CPU1 IPI support
01b,18apr06,dtr  Add IPI support.
01b,18apr06,dtr  IPI support.
01a,06feb06,dtr  Created from cds8548/vxbSysEpic.c/01b
*/

/*
DESCRIPTION
This module implements the Embedded Programmable Interrupt Controller (EPIC)
driver for the MPC8548.

The EPIC is an integrated interrupt controller in the 8548 which
provides following major capabilities:

  Support for twelve external interrupt sources and thirty-two internal
  interrupt sources

  Support for connection of external interrupt controller device e.g. 8259
  like as implemented on a WinBond chip

  12 external interrupt sources
  32 internal interrupt sources
  16 programmable interrupt priority levels
  Fully-nested interrupt delivery
  Spurious vector generation
  Route to critical interrupt destinations
  Route to external pin
  Configurable sense and polarity at initialization and runtime
  Inter-processor interrupts

The current implementation of this EPIC controller does not support
the following features or mode of operations:

  PIC global timers
  Messaging interrupts

EPIC features are customized by writing into general control registers
or into interrupt level specific registers (IVPRs).

This driver allows a basic interface to the EPIC such as initializing it,
setting interrupt vectors, priorities, level/edge sense and interrupt
polarities, as well as enabling and disabling specific interrupts.

This driver implements a complete interrupt architecture system, complete
with vector table.

Since interrupt vectors can be shared, this driver does provide for
overloading of interrupt routines (i.e. there is
a list of interrupt routines for each interrupt vector (level)).  To service
a vector requires that all connected interrupt routines be called in order
of their connection.

The following diagram shows an example of how interrupts can
be configured in this system.

    EPIC Vector table
0  |-------|  <-- external INT0 starts
   |-------|
   |-------|------------------------------+
    ....                                  |
   |-------|                              |
   |-------|                              |
12 |-------| <-- internal INT0 starts     |   (EPIC_MAX_EXT_IRQS) = 12
   |-------|                              |
   |-------|                              |
    ....                                  |
   |-------|                              |
   |-------|                              |
44 |-------| <-- global timer INT0 starts |   (EPIC_MAX_EXT_IRQS + \
   |-------|                              |    EPIC_MAX_IN_IRQS) = 44
   |-------|                              |
   |-------|                              |
48 |-------| <-- message INT0 starts      |   (EPIC_MAX_EXT_IRQS + \
   |-------|                              |    EPIC_MAX_IN_IRQS + \
   |-------|                              |    EPIC_MAX_GT_IRQS) = 48
   |-------|                              |
52 |-------| <-- IPI INT0 starts          |   (EPIC_MAX_EXT_IRQS + \
   |-------|                              |    EPIC_MAX_IN_IRQS + \
   |-------|                              |    EPIC_MAX_GT_IRQS + \
55 |-------|                              |    EPIC_MAX_MSG_IRQS) = 52
56 |-------|<-----------------------------+   sysVectorIRQ0 = 56 \
    ....         WinBond int handler      |   = 52 + EPIC_MAX_IPI_IRQS
   |-------|                              +-------------+
   |-------|                                            |
   |-------|                                 PCI slot 3 int handler
   |-------|
   |-------|<----- Cascaded 8259s
   |-------|
   |-------|
    ....
   |-------|
   |-------|
   |-------|
   |-------|
256|-------|

The driver is designed to put external interrupts at the beginning of the
vector table.  As a result, devices that route their interrupt to the EPIC
on the MPC8540 does not need to translate the vector number.  Therefore,
the macros IVEC_TO_INUM(x) and INUM_TO_IVEC(x) are not necessary.  For
some existing drivers, it may be necessary to use the following defines:
  #undef  INUM_TO_IVEC
  #define INUM_TO_IVEC(x) (x)
  #undef  IVEC_TO_INUM
  #define IVEC_TO_INUM(x) (x)

If there are other devices in the system capable of generating their own
vectors then we presume that an appropriate interrupt handler is created
and attached to the vector associated with the correct IRQ number.  That
interrupt handler would get a new vector directly from the device and then
call all of the handlers attached to that new vector.  Vector information is
stored in a linked list of INT_HANDLER_DESC structures. The vxbEpicIntTbl array
contains a pointer to the first entry for each vector.

INITIALIZATION

This driver is initialized from the BSP, usually as part of sysHwInit().
The first routine to be called is vxbSysEpicInit(). The routine resets the
global configuration register and resets the vxbEpic registers to default
values.

The second routine to be called is vxbSysEpicIntrInit().  This routine takes no
arguments. This routine allocates the vector table and initializes the
chips to a default state.  All individual interrupt sources are disabled.
Each has to be individually enabled by intEnable() before it will be
unmasked and allowed to generate an interrupt.

CRITICAL INTERRUPT

To enable the EPIC to handle also critical interrupt, or if a normal
interrupt is to be rerouted to the critical input pin, add the following to
the hcfResource table for the epic in hwconf.c:
  const struct hcfResource epic0Resources[] = {
      /@ ... @/
      { "crtEnable",	HCF_RES_INT,	{(void *)TRUE } },
      /@ ... @/
  };
The critical interrupt handler uses information from the summary registers
CISR0 and CISR1.  The EPIC does not manage critical interrupts and hence
Iack or EOI do not apply.  It was seen that the summary registers go
through a transient state before settling on the result.  This causes
spurious interrupts to be generated, and the vectors being called.
A typical behavior is the printout of "uninitialized PIC interrupt
vector 0xXX".  This is observed only when at least one source has been
routed to critical pin.

CUSTOMIZING THIS DRIVER

The BSP can change the default polarity and sensitivity for the external
interrupt and the internal interrupt independently.  They are:
  exSense		/@ default to EPIC_SENSE_LVL @/
  exPolar		/@ default to EPIC_INT_ACT_HIGH @/
  inPolar		/@ default to EPIC_INT_ACT_HIGH @/
The default can be overridden by adding the above as parameters in hwconf.c.
For example,
  const struct hcfResource epic0Resources[] = {
      /@ ... @/
      { "exPolar",	HCF_RES_INT,	{(void *)EPIC_INT_ACT_LOW } },
      /@ ... @/
  };
  The available options are:
  exSense		EPIC_SENSE_LVL, EPIC_SENSE_EDG
  xxPolar		EPIC_INT_ACT_LOW, EPIC_INT_ACT_HIGH /@if level-sense @/
			EPIC_INT_EDG_NEG, EPIC_INT_EDG_POS  /@if edge-sense @/

The macros CPU_INT_LOCK() and CPU_INT_UNLOCK provide the access
to the CPU level interrupt lock/unlock routines.  We presume that there
is a single interrupt line to the CPU.  By default these macros call
intLock() and intUnlock() respectively.

INCLUDE FILES:
*/

/* includes */

#include <vxWorks.h>
#include <vsbConfig.h>
#include <hwif/intCtlr/vxbEpicIntCtlr.h>

#include <sysLib.h>
#include <stdio.h>
#include <string.h>
#include <intLib.h>
#include <ffsLib.h>
#include <arch/ppc/excPpcLib.h>
#include <arch/ppc/vxPpcLib.h>

#include <private/funcBindP.h>
#include <hwif/vxbus/vxBus.h>
#include <hwif/vxbus/hwConf.h>
#include <../src/hwif/h/vxbus/vxbAccess.h>
#include <hwif/util/hwMemLib.h>
#include <hwif/vxbus/vxbPlbLib.h>
#include <hwif/vxbus/vxbIntrCtlr.h>
#include <../src/hwif/intCtlr/vxbIntCtlrLib.h>
#include <../src/hwif/intCtlr/vxbIntDynaCtlrLib.h>

#ifdef _WRS_CONFIG_SV_INSTRUMENTATION
#include <private/eventP.h>
#endif /* _WRS_CONFIG_SV_INSTRUMENTATION */
#ifdef _WRS_CONFIG_SMP
#include <private/cpcLibP.h>
#endif /* _WRS_CONFIG_SMP */

#include <vxIpiLib.h>

/* debug */

#ifdef VXB_EPICINTCTLR_DBG_ON
#undef  LOCAL
#define LOCAL

/*
 * NOTE: printf() and logMsg() debugging cannot be used before the
 * call to templateDriverInstConnect().  Any use before that may cause the
 * system to crash before finishing the boot process.  To debug
 * the probe routine, init routine, and init2 routine, either
 * use a hardware debugger, move the specified functionality to
 * templateDriverInstConnect(), or manually call the routine after boot.
 */
int vxbEpicIntCtlrDbgLvl = 0;

#ifndef VXB_EPICINTCTLR_DBG_MSG
#define VXB_EPICINTCTLR_DBG_MSG(level,fmt,a,b,c,d,e,f) if ( vxbEpicIntCtlrDbgLvl >= level ) logMsg(fmt,a,b,c,d,e,f)
#endif /* VXB_EPICINTCTLR_DBG_MSG */

#else /* VXB_EPICINTCTLR_DBG_ON */

#define VXB_EPICINTCTLR_DBG_MSG(level,fmt,a,b,c,d,e,f)

#endif /* VXB_EPICINTCTLR_DBG_ON */

/* defines */

#define PPC_PVR_E500V2		0x80210000
#define PPC_PVR_MASK		0xFFFF0000

#define PPC_SVR_8569		0x80800000
#define PPC_SVR_MASK		0xFFF00000

#ifndef CPU_INT_LOCK
#   define CPU_INT_LOCK(pData) \
	(*pData = INT_CPU_LOCK())
#endif /* !CPU_INT_LOCK */

#ifndef CPU_INT_UNLOCK
#   define CPU_INT_UNLOCK(data) \
	(INT_CPU_UNLOCK(data))
#endif /* !CPU_INT_UNLOCK */

#define EPIC_DRV_CTRL(member) ((struct vxbEpicIntCtlrDrvCtrl *)(epicVxbId->pDrvCtrl))->member

#ifndef	_EXC_OFF_CRTL
#define _EXC_OFF_CRTL	0x00100		/* critical interrupt */
#endif /* _EXC_OFF_CRTL */

/* macros to fields in intr cntlr pin array */

#define EPIC_DESTCPU(pEnt,inputPin,destCpu)                  \
        {                                                    \
        struct vxbIntCtlrPin * pPin =                        \
           vxbIntCtlrPinEntryGet(pEnt,inputPin);             \
        destCpu = pPin->pinCpu;                              \
        }

#define EPIC_ISR(pEnt,inputPin,func)                         \
        {                                                    \
        struct vxbIntCtlrPin * pPin =                        \
           vxbIntCtlrPinEntryGet(pEnt,inputPin);             \
        func = pPin->isr;                                    \
        }

typedef struct vxbEpicIntCtlrDrvCtrl
    {
    VXB_DEVICE_ID         pInst;
    struct intCtlrHwConf  isrHandle;
    struct dynamicIntrTable *dynamicInterruptTable;
    UINT32                dynamicInterruptTableSize;
#ifdef	VXB_EPICINTCTLR_ACCESS_ROUTINES
    void *                regAccessHandle;
#endif	/* VXB_EPICINTCTLR_ACCESS_ROUTINES */
    UINT32                numCpus;
    UINT32                exPolar;
    UINT32                exSense;
    UINT32                inPolar;
    BOOL		  exIrqWkrd;
    BOOL		  crtEnable;
    int			  numInts;
    ULONG * 		  regTable;
    INT_BANK_DESC *	  intBankDesc[INT_BANK_DESC_NUM];
    int                   extIrq0;
    int                   inIrq0;
    int                   gtAIrq0;
    int                   gtBIrq0;
    int                   smsgIrq0;
    int                   msgIrq0;
    int                   ipiIrq0;
   } VXB_EPICINTCTLR_DRV_CTRL;

LOCAL INT_BANK_DESC defaultBankDesc[INT_BANK_DESC_NUM] = {
    { EPIC_VEC_EXT_IRQ0, EPIC_MAX_EXT_IRQS, EPIC_EX_INT0_VEC_REG, EPIC_EX_VEC_REG_INTERVAL, EPIC_EX_INTERRUPT },
    { EPIC_VEC_IN_IRQ0, EPIC_MAX_IN_IRQS, EPIC_IN_INT0_VEC_REG, EPIC_IN_VEC_REG_INTERVAL , EPIC_IN_INTERRUPT },
    { EPIC_VEC_GT_A_IRQ0, EPIC_MAX_GT_A_IRQS, EPIC_TM_A0_VEC_REG, EPIC_GT_VEC_REG_INTERVAL, EPIC_GT_A_INTERRUPT },
    { EPIC_VEC_GT_B_IRQ0, EPIC_MAX_GT_B_IRQS, EPIC_TM_B0_VEC_REG, EPIC_GT_VEC_REG_INTERVAL, EPIC_GT_B_INTERRUPT },
    { EPIC_VEC_MSG_IRQ0, EPIC_MAX_MSG_IRQS, EPIC_MSG_INT0_VEC_REG, EPIC_MSG_VEC_REG_INTERVAL, EPIC_MSG_INTERRUPT },
    { EPIC_VEC_SMSG_IRQ0, EPIC_MAX_SMSG_IRQS, EPIC_SMSG_INT0_VEC_REG, EPIC_SMSG_VEC_REG_INTERVAL, EPIC_SMSG_INTERRUPT },
    { EPIC_VEC_IPI_IRQ0, EPIC_MAX_IPI_IRQS, EPIC_IPI_0_VEC_REG, EPIC_IPI_VEC_REG_INTERVAL, EPIC_IPI_INTERRUPT },
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0},
};

/* forward declarations */

/* This dynamicIntrTable table is only partially complete. The shared message 
 * interrupt info needs to be added. It is completed after interrupt 
 * assignments are made. 
 */
LOCAL struct dynamicIntrTable dynamicInterrupts[] = {
    { 0, 32},
    { 0 + 1, 32},
    { 0 + 2, 32},
    { 0 + 3, 32},
    { 0 + 4, 32},
    { 0 + 5, 32},
    { 0 + 6, 32},
    { 0 + 7, 32}
};

LOCAL int	vxbEpicIntEnable (ULONG vector);
LOCAL int	vxbEpicIntDisable (ULONG vector);
LOCAL int	vxbEpicVecOptionsSet (ULONG srcAddr, UINT32 mask, UINT32 options);
LOCAL void 	vxbSysEpicCrtIntHandler (void);
#if 0	/* (not used) */
LOCAL UINT32	vxbEpicVecOptionsGet (ULONG srcAddr);
LOCAL int	vxbEpicCrtIntSet (ULONG srcAddr);
LOCAL int	vxbEpicCrtIntUnset (ULONG srcAddr);
LOCAL int	vxbEpicCrtIntGet (ULONG srcAddr);
LOCAL STATUS    vxbEpicIntSourceGet (ULONG srcAddr, int * pEnableMask,
                                     int * pPolarity, int * pSense,
                                     int * pPriority, int * pVector);
#endif
LOCAL int       vxbEpicCpuIntGet (ULONG srcAddr);
LOCAL void      vxbEpicCpuIntSet (ULONG srcAddr, int cpu);
LOCAL STATUS    vxbSysEpicIntConnect (VOIDFUNCPTR * vector, VOIDFUNCPTR routine,
				      int parameter);
LOCAL STATUS    vxbSysEpicIntDisconnect (VOIDFUNCPTR * vector,
                                         VOIDFUNCPTR routine,
				         int parameter);
LOCAL int       vxbSysEpicIntEnable (int vector);
LOCAL int       vxbSysEpicIntDisable (int vector);
LOCAL void      vxbSysEpicInit (VXB_DEVICE_ID pInst);
LOCAL STATUS    vxbSysEpicIntrInit (VXB_DEVICE_ID pInst);
LOCAL STATUS    vxbEpicIntSourceSet (ULONG srcAddr, int polarity,
                                     int sense, int priority, int vector);
LOCAL ULONG     vxbEpicGetVecRegAdrs (int vector);
LOCAL ULONG     vxbEpicGetDestRegAdrs (int vector);
LOCAL int       vxbEpicCurTaskPrioSet (int prioNum, int cpu);
LOCAL int       vxbEpicCpu1CheckFalse (void);
LOCAL STATUS    vxbEpicCpuAvailGet (VXB_DEVICE_ID pDev, UINT32 * pAvail);

LOCAL int 	vxbEpicSenseGet (UINT8);
LOCAL int 	vxbEpicPolarGet (UINT8);
LOCAL int 	vxbEpicPrioGet  (UINT16);

LOCAL BOOL vxbEpicIntEnableState
    (
    ULONG 	vector
    );

LOCAL STATUS vxbSysEpicIntConfig
    (
    VOIDFUNCPTR * 	vector,
    VOIDFUNCPTR		routine,
    int			parameter
    );

#ifdef VXB_EPICINTCTLR_DBG_ON
LOCAL int vxbEpicIntCtlrpDrvCtrlShow
    (
    VXB_DEVICE_ID	pInst,
    int			verboseLevel
    );
#endif /* VXB_EPICINTCTLR_DBG_ON */

LOCAL STATUS vxbEpicIntCtlrDevIsrConnect
    (
    VXB_DEVICE_ID	pIntCtlr,
    VXB_DEVICE_ID	pDev,
    int			indx,
    void		(*pIsr)(void * pArg),
    void *		pArg,
    int *		pInputPin
    );

LOCAL STATUS vxbEpicIntCtlrDynaVecConnect
    (
    VXB_DEVICE_ID	pIntCtlr,
    VXB_DEVICE_ID	pDev,
    int vecCount,
    struct vxbIntDynaVecInfo* dynaVec
    );

LOCAL STATUS vxbEpicIntCtlrDevIsrDisconnect
    (
    VXB_DEVICE_ID	pIntCtlr,
    VXB_DEVICE_ID	pDev,
    int			indx,
    void		(*pIsr)(void * pArg),
    void *		pArg
    );

LOCAL STATUS vxbEpicIntCtlrDevIsrEnable
    (
    VXB_DEVICE_ID	pIntCtlr,
    VXB_DEVICE_ID	pDev,
    int			indx,
    void		(*pIsr)(void * pArg),
    void *		pArg
    );

LOCAL STATUS vxbEpicIntCtlrDevIsrDisable
    (
    VXB_DEVICE_ID	pIntCtlr,
    VXB_DEVICE_ID	pDev,
    int			indx,
    void		(*pIsr)(void * pArg),
    void *		pArg
    );

LOCAL STATUS vxbEpicIntCtlrCpuDisable
    (
    VXB_DEVICE_ID       pDev,
    void *              destCpu
    );

#ifdef _WRS_CONFIG_SMP
LOCAL STATUS vxbEpicIntCtlrCpuReroute
    (
    VXB_DEVICE_ID       pDev,
    void *              destCpu
    );
LOCAL STATUS vxbEpicIntCtlrIntReroute
    (
    VXB_DEVICE_ID       pDev,
    int                 index,
    cpuset_t            destCpu
    );
#endif /* _WRS_CONFIG_SMP */

LOCAL void epicIntCtlrInstInit(VXB_DEVICE_ID pInst);
LOCAL void epicIntCtlrInstInit2(VXB_DEVICE_ID pInst);
LOCAL void epicIntCtlrInstConnect(VXB_DEVICE_ID pInst);

LOCAL VXIPI_CTRL_INIT * vxbEpicIntCtlrIpiCntrlGet(VXB_DEVICE_ID pInst, void * pArg);

LOCAL STATUS vxbEpicIpiConnect(VXB_DEVICE_ID pCtlr, INT32 ipiId, IPI_HANDLER_FUNC ipiHandler, void * ipiArg);
LOCAL STATUS vxbEpicIpiDisconn(VXB_DEVICE_ID pCtlr, INT32 ipiId, IPI_HANDLER_FUNC ipiHandler, void * ipiArg);
LOCAL STATUS vxbEpicIpiEnable(VXB_DEVICE_ID pCtlr, INT32 ipiId);
LOCAL STATUS vxbEpicIpiDisable(VXB_DEVICE_ID pCtlr, INT32 ipiId);
LOCAL INT32 vxbEpicIpiPrioGet(VXB_DEVICE_ID pCtlr, INT32 ipiId);
LOCAL STATUS vxbEpicIpiPrioSet(VXB_DEVICE_ID pCtlr, INT32 ipiId, INT32 prio);
void vxbEpicSharedMsgHandler(struct dynamicArg *isrHandle);

/* locals */

LOCAL void * pEpicRegBase;			/* epicVxbId->pRegBase[0] */
LOCAL VXB_DEVICE_ID epicVxbId;			/* EPIC device ID */
LOCAL int (*vxbEpicCpu1Check)(void);		/* CPU1 check func pointer */

int (*vxbEpicEPRGet)(void);			/* external proxy reg get */

LOCAL struct drvBusFuncs epicIntCtlrFuncs =
    {
    epicIntCtlrInstInit,        /* devInstanceInit */
    epicIntCtlrInstInit2,       /* devInstanceInit2 */
    epicIntCtlrInstConnect      /* devConnect */
    };

/*
 * Interrupt controllers use driver methods to communicate with
 * the VxBus interrupt management routines, vxbIntConnect(),
 * vxbIntEnable(), and so on.  The policy is that if a driver
 * publishes one of these methods, and the driver recognizes
 * the device trying to manage its interrupts, then these
 * routines are called.  However, if no driver publishes
 * these methods or no driver (with these methods published)
 * recognizes the device, then the system tries to connect
 * based on legacy support in the bus controller driver.
 */

METHOD_DECL(vxbIntCtlrConnect)
METHOD_DECL(vxbIntCtlrDisconnect)
METHOD_DECL(vxbIntCtlrEnable)
METHOD_DECL(vxbIntCtlrDisable)
METHOD_DECL(vxbIntCtlrAck)
METHOD_DECL(busDevShow)
METHOD_DECL(vxIpiControlGet)
METHOD_DECL(vxbIntCtlrCpuCheck)
METHOD_DECL(vxbIntCtlrCpuAvail)
METHOD_DECL(vxbIntCtlrCpuDelete)
METHOD_DECL(vxbIntCtlrCpuDisable)
#ifdef _WRS_CONFIG_SMP
METHOD_DECL(vxbIntCtlrCpuReroute)
METHOD_DECL(vxbIntCtlrIntReroute)
#endif /* _WRS_CONFIG_SMP */

LOCAL device_method_t epicIntCtlr_methods[] =
    {
    DEVMETHOD(vxbIntCtlrConnect, vxbEpicIntCtlrDevIsrConnect),
    DEVMETHOD(vxbIntCtlrEnable, vxbEpicIntCtlrDevIsrEnable),
    DEVMETHOD(vxbIntCtlrDisconnect, vxbEpicIntCtlrDevIsrDisconnect),
    DEVMETHOD(vxbIntCtlrDisable, vxbEpicIntCtlrDevIsrDisable),
    DEVMETHOD(vxbIntCtlrCpuDisable, vxbEpicIntCtlrCpuDisable),
#ifdef _WRS_CONFIG_SMP
    DEVMETHOD(vxbIntCtlrCpuReroute, vxbEpicIntCtlrCpuReroute),
    DEVMETHOD(vxbIntCtlrIntReroute, vxbEpicIntCtlrIntReroute),
#endif /* _WRS_CONFIG_SMP */

    DEVMETHOD(vxIpiControlGet,        vxbEpicIntCtlrIpiCntrlGet),
    DEVMETHOD(vxbIntDynaVecConnect, vxbEpicIntCtlrDynaVecConnect),
#ifdef VXB_EPICINTCTLR_DBG_ON
    DEVMETHOD(busDevShow, vxbEpicIntCtlrpDrvCtrlShow),
#endif /* VXB_EPICINTCTLR_DBG_ON */
    DEVMETHOD(vxbIntCtlrCpuAvail, vxbEpicCpuAvailGet),

    { 0, 0}
    };

LOCAL struct vxbDevRegInfo epicIntCtlrDevRegistration =
    {
    NULL,                 /* pNext */
    VXB_DEVID_DEVICE,     /* devID */
    VXB_BUSID_PLB,        /* busID = PLB */
    VXB_VER_4_0_0,      /* busVer */
    "epic",               /* drvName */
    NULL,                 /* pDrvBusFuncs */
    NULL,                 /* pMethods */
    NULL,                 /* devProbe */
    NULL                  /* pParamDefaults */
    };

/* externs */

IMPORT int vxPpcIntMask;		/* MSR interrupts mask bits */

/* get the interrupt hook routines  prototypes*/

IMPORT STATUS	(*_func_intConnectRtn) (VOIDFUNCPTR *, VOIDFUNCPTR, int);
IMPORT STATUS	(*_func_intDisconnectRtn) (VOIDFUNCPTR *, VOIDFUNCPTR, int);
IMPORT int	(*_func_intEnableRtn)  (int);
IMPORT int	(*_func_intDisableRtn) (int);

IMPORT int      intCtlrPinFind (VXB_DEVICE_ID, int, VXB_DEVICE_ID, 
                                struct intCtlrHwConf *);
IMPORT void     intCtlrStrayISR (void *, int);
IMPORT void     intCtlrChainISR (void *, int);
IMPORT STATUS	vxbEpicIpiGen (VXB_DEVICE_ID pCtlr, INT32 ipiId, cpuset_t cpus);
IMPORT void 	vxbSysEpicIntHandler (void);

/* globals */

UINT32 * vxbEpicIntAckReg;			/* int ack reg adrs */
USHORT vxbEpicSpuriousVector=0xff;		/* Superious Vector Number */
struct intCtlrHwConf * pVxbEpicIsrHandle;	/* &EPIC_DRV_CTRL(isrHandle) */
UINT32 * vxbEpicIpiDpatchReg0;			/* IPI dispatch reg0 adrs */

/*
 * This structure is initialized with the control functions for the IPI
 * interface.  These set of functions allow the CPC layer to manipulate IPI
 * interrupts.
 */

LOCAL VXIPI_CTRL_INIT vxEpicIpiCtrlInit =
    {
    {NULL},                     /* ipiList */
    0,                          /* pCpus */
    vxbEpicIpiGen,              /* ipiEmitFunc */
    vxbEpicIpiConnect,          /* ipiConnectFunc */
    vxbEpicIpiEnable,           /* ipiEnableFunc */
    vxbEpicIpiDisable,          /* ipiDisableFunc */
    vxbEpicIpiDisconn,          /* ipiDisconnFunc */
    vxbEpicIpiPrioGet,          /* ipiPrioGetFunc */
    vxbEpicIpiPrioSet,          /* ipiPrioSetFunc */
    EPIC_MAX_IPI_IRQS,          /* ipiCount */
    };
int ShNO,MsirNO,inpin;  //add by ff 2011.07.05

/******************************************************************************
*
* vxbEpicIntCtlrRegister - register epicIntCtlr driver
*
* This routine registers the EPIC driver and device recognition
* data with the vxBus subsystem.
*
*/

void vxbEpicIntCtlrRegister(void)
    {
    epicIntCtlrDevRegistration.pDrvBusFuncs = &epicIntCtlrFuncs;
    vxbDevRegister((struct vxbDevRegInfo *)&epicIntCtlrDevRegistration);
    }

/******************************************************************************
*
* epicIntCtlrInstInit - initialize epicIntCtlr device
*
*/

LOCAL void epicIntCtlrInstInit
    (
    VXB_DEVICE_ID pInst
    )
    {
    struct vxbEpicIntCtlrDrvCtrl * pDrvCtrl;
    HCF_DEVICE * pHcf = (HCF_DEVICE *)pInst->pBusSpecificDevInfo;
    char intDescN[] = "intDesc_X";
    int i;

    pDrvCtrl = (struct vxbEpicIntCtlrDrvCtrl *)hwMemAlloc(sizeof(*pDrvCtrl));
    if ( pDrvCtrl != NULL )
	{
	/* save instance ID */

	pDrvCtrl->pInst = pInst;

        /* save device ID */

        epicVxbId = pInst;

	/* save base reg */

	pEpicRegBase = epicVxbId->pRegBase[0];

    /*
     * save interrupt acknowledge register addresss and IPI dispatch register 0
     * address
     *
     * When accessing PIC registers the driver is expected to access the global
     * registers and the PIC would route the reading/writing operation from/to
     * the per-cpu register on the value of the WHOAMI register.
     *
     * For MPC8569, due to the bug in the WHOAMI registe, the reading/writing
     * operation is not routed properly, and the operations have no effect. So
     * the driver will directly read/write from/to the per-cpu registers for
     * MPC8569.
     */

    if (((vxPvrGet() & PPC_PVR_MASK) == PPC_PVR_E500V2) &&
        ((_PPC_VX_SVR_GET & PPC_SVR_MASK) == PPC_SVR_8569))
        {
        vxbEpicIntAckReg     = (UINT32 *) ((UINT32) pEpicRegBase +
                                           EPIC_INT_ACK0_REG);

        vxbEpicIpiDpatchReg0 = (UINT32 *) ((UINT32) pEpicRegBase +
                                           EPIC_IPI_DPATCH0_REG0);
        }
    else
        {
        vxbEpicIntAckReg     = (UINT32 *) ((UINT32) pEpicRegBase +
                                           EPIC_INT_ACK_REG);

        vxbEpicIpiDpatchReg0 = (UINT32 *) ((UINT32) pEpicRegBase +
                                           EPIC_IPI_DPATCH_REG0);
        }

	/* get connectivity info from hwconf */
	intCtlrHwConfGet(pInst, pHcf, &(pDrvCtrl->isrHandle));
	
	/* get CPU selection info - default is 2 CPUs */
        vxbEpicCpuAvailGet (pInst, &pDrvCtrl->numCpus);

        /*
         * resourceDesc {
         * The numCpus resource specifies the number of CPUs
         * connected to the interrupt controller. }
         */
	devResourceGet (pHcf, "numCpus", HCF_RES_INT, 
			(void *)&pDrvCtrl->numCpus);

        /* get interrupt polarity and sense info */
        pDrvCtrl->exPolar = EPIC_EX_DFT_POLAR;
        pDrvCtrl->exSense = EPIC_EX_DFT_SENSE;
        pDrvCtrl->inPolar = EPIC_IN_DFT_POLAR;

        /*
         * resourceDesc {
         * The exPolar resource specifies the polarity (active
         * high or low) of the external interrupts. }
         */
	devResourceGet (pHcf, "exPolar", HCF_RES_INT, 
			(void *)&pDrvCtrl->exPolar);
        /*
         * resourceDesc {
         * The exSense resource specifies the sense (edge-
         * or level-triggered) of the external interrupts. }
         */
	devResourceGet (pHcf, "exSense", HCF_RES_INT, 
			(void *)&pDrvCtrl->exSense);
        /*
         * resourceDesc {
         * The inPolar resource specifies the polarity (active
         * high or low) of the internal interrupts. }
         */
	devResourceGet (pHcf, "inPolar", HCF_RES_INT, 
			(void *)&pDrvCtrl->inPolar);

        /* get external IRQ workaround and critical interrupt settings */
        pDrvCtrl->exIrqWkrd = FALSE;
        pDrvCtrl->crtEnable = FALSE;

        /*
         * resourceDesc {
         * The exIntWkrd resource specifies whether the external
         * IRQ workaround should be used (default FALSE). }
         */
	devResourceGet (pHcf, "exIntWkrd", HCF_RES_INT, 
			(void *)&pDrvCtrl->exIrqWkrd);
        /*
         * resourceDesc {
         * The crtEnable resource is used to enable or disable
         * critical interrupt support. }
         */
	devResourceGet (pHcf, "crtEnable", HCF_RES_INT, 
			(void *)&pDrvCtrl->crtEnable);

	pDrvCtrl->dynamicInterruptTableSize = 0;
	pDrvCtrl->dynamicInterruptTable = NULL;


	/* get num ints */
        pDrvCtrl->numInts = EPIC_MAX_ALL_IRQS + 1;
        /*
         * resourceDesc {
	 * The numInts resource specifies the number of
	 * IRQ numbers to be supported. }
	 */
	devResourceGet (pHcf, "numInts", HCF_RES_INT,
			(void *)&pDrvCtrl->numInts);
	pDrvCtrl->regTable = (ULONG *) hwMemAlloc (pDrvCtrl->numInts * sizeof (UINT32));

	/* get bank descriptors */
        for (i = 0; i < INT_BANK_DESC_NUM; i++)
            {
            intDescN[8] = '0' + i;

	    /*
	     * resourceDesc {
	     * The intDescN set of resources specify pointers
	     * to INT_BANK_DESC structures, one for each type
	     * of interrupt to be supported (e.g. external,
	     * internal, timer).  The structure contains the
	     * following values: starting IRQ number, number
	     * of IRQs, address of EPIC vector/destination
	     * register for first IRQ in set, interval between
	     * successive vector/destination registers, and
	     * the type of interrupt the resource describes. }
	     */
	    if (devResourceGet (pHcf, intDescN, HCF_RES_ADDR, 
                               (void *)&pDrvCtrl->intBankDesc[i]) == ERROR)
                pDrvCtrl->intBankDesc[i] = &defaultBankDesc[i];
            }

        /* get external proxy register function */
        if (devResourceGet (pHcf, "sysEprGet", HCF_RES_ADDR,
                            (void *)&vxbEpicEPRGet) == ERROR)
            vxbEpicEPRGet = NULL;

	/* get cpu check function */
        vxbEpicCpu1Check = vxbDevMethodGet (vxbDevParent(pInst),
                                            DEVMETHOD_CALL(vxbIntCtlrCpuCheck));
        if (vxbEpicCpu1Check == NULL)
            vxbEpicCpu1Check = vxbEpicCpu1CheckFalse;


	/* advertise intConnect() and other methods */

	pInst->pMethods = &epicIntCtlr_methods[0];

	/* save pDrvCtrl in VXB_DEVICE structure */

	pInst->pDrvCtrl = pDrvCtrl;

#ifdef	VXB_EPICINTCTLR_ACCESS_ROUTINES
	vxbRegMap (pInst, 0, &pDrvCtrl->regAccessHandle);
#endif	/* VXB_EPICINTCTLR_ACCESS_ROUTINES */

	vxbSysEpicInit(pInst);
	
	pDrvCtrl->dynamicInterruptTable = &dynamicInterrupts[0];
	pDrvCtrl->dynamicInterruptTableSize = NELEMENTS(dynamicInterrupts);
	for( i=0; i< pDrvCtrl->dynamicInterruptTableSize; i++)
	    pDrvCtrl->dynamicInterruptTable[i].inputPin += EPIC_DRV_CTRL(smsgIrq0);

	for( i=0; i< pDrvCtrl->dynamicInterruptTableSize; i++)
	    {
	    
	    if(_func_vxbIntDynaCtlrInputInit != NULL)
		{    
		    
		if(_func_vxbIntDynaCtlrInputInit(pDrvCtrl->isrHandle,
						 pDrvCtrl->dynamicInterruptTable[i],
						 vxbEpicSharedMsgHandler, i)==ERROR)
		    return;
		    
		}
	    }
	}
    }

/******************************************************************************
*
* epicIntCtlrInstInit2 - initialize epicIntCtlr device
*
*/

LOCAL void epicIntCtlrInstInit2
    (
    VXB_DEVICE_ID pInst
    )
    {
    vxbSysEpicIntrInit(pInst);
    }

/******************************************************************************
*
* epicIntCtlrInstConnect - connect epicIntCtlr device to I/O system
*
*/

LOCAL void epicIntCtlrInstConnect
    (
    VXB_DEVICE_ID pInst
    )
    {
    }

#ifdef VXB_EPICINTCTLR_DBG_ON

/******************************************************************************
*
* vxbEpicIntCtlrpDrvCtrlShow - show pDrvCtrl for template controller
*
* RETURNS: 0 always
*/

LOCAL int vxbEpicIntCtlrpDrvCtrlShow
    (
    VXB_DEVICE_ID	pInst,
    int			verboseLevel
    )
    {
    struct vxbEpicIntCtlrDrvCtrl * pDrvCtrl = pInst->pDrvCtrl;

    printf("\tinterrupt controller %s%d = 0x%08x\n",
	pInst->pName, pInst->unitNumber, pInst);

    if ( verboseLevel > 0 )
        printf("\tisrHandle = 0x%08x\n", &(pDrvCtrl->isrHandle));

    if ( verboseLevel > 50 )
	{
        intCtlrHwConfShow(&(pDrvCtrl->isrHandle));
	if(_func_vxbIntDynaVecProgram!=NULL)
	    {
	    int i;
	    struct intCtlrHwConf* dynamicIsrHandle;
	    for(i=0;i<EPIC_MAX_SMSG_IRQS;i++)
		{
		dynamicIsrHandle  = (struct intCtlrHwConf*)intCtlrTableArgGet(&pDrvCtrl->isrHandle, EPIC_DRV_CTRL(smsgIrq0) + i);
		printf("\tdynamicIsrHandle = 0x%08x\n", dynamicIsrHandle);
		intCtlrHwConfShow(dynamicIsrHandle);
		}
	    }
	}

    return(0);
    }

#endif /* VXB_EPICINTCTLR_DBG_ON */


/*******************************************************************************
*
* vxbSysEpicInit - initialize the vxbEpic controller
*
* This routine resets the global Configuration Register, thus it:
*  -  disables all interrupts
*  -  sets vxbEpic registers to reset values
*
* It then sets the EPIC operation mode to Mixed Mode (vs. Pass Through
* mode). At this point only mixed mode is supported which means the EPIC
* is not configured for the pass through mode.
*
* Only direct interrupt sources is supported by the PIC.  Serial
* stype interrupt is not available.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void vxbSysEpicInit 
    (
    VXB_DEVICE_ID pInst
    )
    {
    ULONG 	gcrVal;
    int 	irq;
    int		i;
    ULONG	regAdrs;
    ULONG *     regTable;
    INT_BANK_DESC * pDesc;
    int		nextIrq = 0;
    BOOL 	onBootCpu = (*vxbEpicCpu1Check)() == 0 ? TRUE : FALSE;

    /* Only one vxWorks image should initialize Epic and that is CPU0 */

    if(onBootCpu)
	{

	gcrVal = EPIC_REG_READ(EPIC_GLOBAL_REG);

	gcrVal |= (EPIC_GCR_RESET);

	EPIC_REG_WRITE(EPIC_GLOBAL_REG, gcrVal);

	/* wait for the reset sequence to be completed */

	while (EPIC_REG_READ(EPIC_GLOBAL_REG) & EPIC_GCR_RESET)
	    {
	    ; /* do nothing */
	    }

	/* set the spurious vector to match linux */
	EPIC_REG_WRITE(EPIC_SPUR_VEC_REG,vxbEpicSpuriousVector);

	gcrVal = EPIC_REG_READ(EPIC_GLOBAL_REG);

        if (vxbEpicEPRGet != NULL)
	    gcrVal |= (EPIC_GCR_MODE_EPF);	/* configure for ext. proxy */
        else
	    gcrVal |= (EPIC_GCR_MODE_MIXED);	/* configure for mixed mode */

	EPIC_REG_WRITE(EPIC_GLOBAL_REG, gcrVal);

	/* Clear all pending interrupts assume 0xXXff is spurious  */
	while (((USHORT) EPIC_REG_READ(EPIC_INT_ACK0_REG) & 0xff) != 
	       (USHORT) vxbEpicSpuriousVector)
	    {
	    /* do nothing */
	    WRS_ASM ("isync;sync");
	    }

	if (EPIC_DRV_CTRL(numCpus) > 1)
	    while (((USHORT) EPIC_REG_READ(EPIC_INT_ACK1_REG) & 0xff) != 
	           (USHORT) vxbEpicSpuriousVector)
	        {
	        /* do nothing */
	        WRS_ASM ("isync;sync");
	        }


	}

    regTable = EPIC_DRV_CTRL(regTable);
    for (i = 0; i < INT_BANK_DESC_NUM; i++)
	{
	pDesc = EPIC_DRV_CTRL(intBankDesc[i]);
	if (pDesc == NULL)
	    continue;
	
	if (pDesc->startIrq == 0)
	    pDesc->startIrq = nextIrq;
	
	nextIrq = pDesc->startIrq + pDesc->numIrqs;
	
	for (irq = pDesc->startIrq, regAdrs = pDesc->regBaseAdrs; 
	     irq < nextIrq; 
	     irq++, regAdrs += pDesc->regInterval)
	    {
	    UINT32 type = (UINT32) pDesc->type;
	    UINT32 tableValue = regAdrs | (type << 24);
	    
	    regTable[irq] = tableValue;
		  
	    /* Only initialize interrupts if boot cpu */
	    if(onBootCpu)
		vxbEpicIntDisable (irq);

            switch (type)
	        {
	        case EPIC_EX_INTERRUPT:
                    if (onBootCpu)
	        	vxbEpicIntSourceSet (tableValue, 
					     EPIC_DRV_CTRL(exPolar),
                                             EPIC_DRV_CTRL(exSense),
					     0x0, 0x0);
		    if(irq == pDesc->startIrq)
		        EPIC_DRV_CTRL(extIrq0) = irq;
		    break;
		case EPIC_IN_INTERRUPT:
                    if (onBootCpu)
			vxbEpicIntSourceSet (tableValue, 
					     EPIC_DRV_CTRL(inPolar), 
                                             0x0, 0x0, 0x0);
		    if(irq == pDesc->startIrq)
		        EPIC_DRV_CTRL(inIrq0) = irq;
		    break;
		case EPIC_MSG_INTERRUPT:
                    if (onBootCpu)
                        { 
			EPIC_REG_WRITE(EPIC_MSG_EN_REG,0xf);
			vxbEpicIntSourceSet (tableValue, 0x0, 0x0, 0x0, 0x0);
                        }
		    if(irq == pDesc->startIrq)
		        EPIC_DRV_CTRL(msgIrq0) = irq;
		    break;
	        case EPIC_GT_A_INTERRUPT:
                    if (onBootCpu)
			vxbEpicIntSourceSet (tableValue, 0x0, 0x0, 0x0, 0x0);
		    if(irq == pDesc->startIrq)
		        EPIC_DRV_CTRL(gtAIrq0) = irq;
		    break;
		case EPIC_GT_B_INTERRUPT:
                    if (onBootCpu)
			vxbEpicIntSourceSet (tableValue, 0x0, 0x0, 0x0, 0x0);
		    if(irq == pDesc->startIrq)
		        EPIC_DRV_CTRL(gtBIrq0) = irq;
		    break;
		case EPIC_SMSG_INTERRUPT:
                    if (onBootCpu)
			vxbEpicIntSourceSet (tableValue, 0x0, 0x0, 0x0, 0x0);
		    if(irq == pDesc->startIrq)
		        EPIC_DRV_CTRL(smsgIrq0) = irq;
		    break;
		case EPIC_IPI_INTERRUPT:
                    if (onBootCpu)
			vxbEpicIntSourceSet (tableValue, 0x0, 0x0, 0x0, 0x0);
		    if(irq == pDesc->startIrq)
		        EPIC_DRV_CTRL(ipiIrq0) = irq;
		    break;
		default:
		    break;
		}
	    }
	}
    
    /* Set default PRIO only if boot cpu */
    if(onBootCpu)
	{
	for (i = 0; i < EPIC_DRV_CTRL(numCpus); i++)
	    vxbEpicCurTaskPrioSet (EPIC_PRIORITY_MAX, i); 
	    
	}
    else
	{
	vxbEpicSpuriousVector = (USHORT)EPIC_REG_READ(EPIC_SPUR_VEC_REG);
	if (EPIC_DRV_CTRL(numCpus) > 1)
	    while (((USHORT) EPIC_REG_READ(EPIC_INT_ACK_REG) & 0xffff) != 
	           (USHORT) vxbEpicSpuriousVector)
	        {
	        /* do nothing */
	        WRS_ASM ("isync;sync");
	        }
	}
    }

/*****************************************************************************
*
* vxbEpicSharedMsgHandler - Handles the interrupt and calls the relevent ISRs
* 
* RETURNS: NONE
*
* ERRNO: None
*/

void vxbEpicSharedMsgHandler
    (
    struct dynamicArg *isrHandle
    )
    {
    UINT32 state,i,regoffset;
    int key;
    int intMask = vxPpcIntMask;

    regoffset = isrHandle->vectNum;
    
    MsirNO = regoffset;  /* add by ff 2010.07.05*/
    
    CPU_INT_LOCK(&key);

#ifdef	VXB_EPICINTCTLR_DBG_ON
    {
    UINT32 msiSr = EPIC_REG_READ(EPIC_MSISR_REG);

    if (!(msiSr & (1 << regoffset)))
	VXB_EPICINTCTLR_DBG_MSG(10,
		"doesn't match MSI status MSISR=0x%x and inputPin=0x%x\n",
		msiSr,(1 << regoffset),3,4,5,6);
    }
#endif	/* VXB_EPICINTCTLR_DBG_ON */

    state = EPIC_REG_READ((EPIC_MSIR_INT0_STATE_REG + 
			   ((regoffset) * EPIC_MSIR_INTX_OFFSET)));
    
    ShNO = state;	/* add by ff 2010.07.05*/ 
    inpin = 1<<regoffset;

    /* get the least bit
     *  number */

    i = ffsLsb (state) - 1;

    /* right shift the bits unless it's 0 */

    state >>= i;

    do
	{
#if	(EPIC_MAX_SMSG_SHARED_IRQS < 32)
	if (i >= EPIC_MAX_SMSG_SHARED_IRQS)
	    break;
#endif	/* EPIC_MAX_SMSG_SHARED_IRQS < 32 */

	if (state & 1)
	    {
	    /*
	     * Need to handle each pending interrupt here as we have just
	     * cleared them above.
	     */

	    VXB_EPICINTCTLR_ENABLE_INTS (intMask);
            
	    VXB_INTCTLR_ISR_CALL((struct intCtlrHwConf*)isrHandle, i);

	    VXB_EPICINTCTLR_DISABLE_INTS (intMask);
	    }

	i++;
	state >>= 1;
	}
    while (state);

    CPU_INT_UNLOCK(key);
    }

/*******************************************************************************
*
* vxbSysEpicIntrInit - initialize the interrupt table
*
* This function initializes the interrupt mechanism of the board.
*
* RETURNS: OK, always.
*
* ERRNO: N/A
*/

LOCAL STATUS  vxbSysEpicIntrInit 
    (
    VXB_DEVICE_ID pInst
    )
    {
    int i;
    /* save pointer to isrHandle */

    pVxbEpicIsrHandle = &(EPIC_DRV_CTRL(isrHandle));

    /*
     * connect the interrupt demultiplexer to the PowerPC external
     * interrupt exception vector.
     * i. e.  put the address of this interrupt handler in
     * the PowerPC's only external interrupt exception vector
     * which is  _EXC_OFF_INTR = 0x500
     * Also connect critical input pin handler _EXC_OFF_CRTL = 0x100.
     */

    vxbIntConnect(pInst, 0, vxbSysEpicIntHandler, NULL);

    /*
     * Over write the vector table and directly connect
     * the vxbSysEpicIntHandler() to _EXC_OFF_INTR to improve performance.
     */

    excVecSet((FUNCPTR *) _EXC_OFF_INTR, (FUNCPTR) vxbSysEpicIntHandler);

    vxbIntEnable(pInst, 0, vxbSysEpicIntHandler, NULL);

    if (EPIC_DRV_CTRL(crtEnable))
	{
        vxbIntConnect(pInst, 1, vxbSysEpicCrtIntHandler, NULL);

	/*
	 * Over write the vector table and directly connect
	 * the vxbSysEpicCrtIntHandler() to _EXC_OFF_CRTL to improve
	 * performance.
	 */

	excVecSet((FUNCPTR *) _EXC_OFF_CRTL, (FUNCPTR) vxbSysEpicCrtIntHandler);

        vxbIntEnable(pInst, 1, vxbSysEpicCrtIntHandler, NULL);
	}

    /*
     * set up the BSP specific interrupt routines
     * Attach the local routines to the VxWorks system calls
     *
     */

    _func_intConnectRtn    = vxbSysEpicIntConnect;
    _func_intDisconnectRtn = vxbSysEpicIntDisconnect;
    _func_intEnableRtn     = vxbSysEpicIntEnable;
    _func_intDisableRtn    = vxbSysEpicIntDisable;

    /* set it to lowest priority */
    for (i = 0; i < EPIC_DRV_CTRL(numCpus); i++)
        vxbEpicCurTaskPrioSet (EPIC_PRIORITY_MIN, i);

    return (OK);
    }

/*******************************************************************************
*
* vxbSysEpicIntEnable - enable a EPIC interrupt level
*
* This routine enables a specified EPIC interrupt level.
*
* \NOMANUAL
*
* RETURNS: OK, ERROR, or EPIC_INV_INTER_SOURCE.
*
* NOTE: To allow compatibility, vxbSysEpicIntEnable() can be used only for
*       external and internal interrupts, and not GT, MSG, IPI.  It
*       assumes there are more internal IRQs than external.  It also
*       gives priority to external over internal given the same IRQ
*       is valid for both external and internal.
*
* ERRNO: N/A
*/

LOCAL int vxbSysEpicIntEnable
    (
    int vector
    )
    {
    if ((vector < 0)  || (vector >= EPIC_MAX_ALL_IRQS))
	return (ERROR);

    /* enable interrupt on EPIC */

    return vxbEpicIntEnable (vector);
    }

/*******************************************************************************
*
* vxbSysEpicIntDisable - disable a EPIC interrupt level
*
* This routine disables a specified EPIC interrupt level.
*
* \NOMANUAL
*
* RETURNS: OK, ERROR, or EPIC_INV_INTER_SOURCE.
*
* NOTE: To allow compatibility, vxbSysEpicIntDisable() can be used only for
*       external and internal interrupts, and not GT, MSG, IPI.  It
*       assumes there are more internal IRQs than external.  It also
*       gives priority to external over internal given the same IRQ
*       is valid for both external and internal.
*
* ERRNO: N/A
*/

LOCAL int  vxbSysEpicIntDisable
    (
    int vector
    )
    {
    if ((vector < 0) || (vector >= EPIC_MAX_ALL_IRQS))
	return (ERROR);

    /* disable interrupt on EPIC */

    return vxbEpicIntDisable (vector);
    }

/*******************************************************************************
*
* vxbEpicVecOptionsSet - set options for a vector
*
* <srcAddr> is the address offset of the Vector Priority register
* <mask> is one of EPIC_OPT_EN_MSK, EPIC_OPT_POLAR_MSK, EPIC_OPT_SENSE_MSK,
*                  EPIC_OPT_PRI_MSK, EPIC_OPT_EXPIN_MSK, EPIC_OPT_CRIT_MSK
* <options> is one of
*   EPIC_OPT_EN_Y              interrupt enabled
*   EPIC_OPT_EN_N              interrupt disabled
*   EPIC_OPT_POLAR_ACT_LOW     polarity is active low
*   EPIC_OPT_POLAR_ACT_HIGH    polarity is active high
*   EPIC_OPT_POLAR_EDG_NEG     polarity is negative edge triggered
*   EPIC_OPT_POLAR_EDG_POS     polarity is positive edge triggered
*   EPIC_OPT_SENSE_EDG         edge sensitive
*   EPIC_OPT_SENSE_LVL         level sensitive
*   EPIC_OPT_PRI_VALUE(p)      set priority to p [0..15]
*   EPIC_OPT_EXPIN_OFF         external pin off
*   EPIC_OPT_EXPIN_ON          external pin on
*   EPIC_OPT_CRIT_OFF          route to critical pin off
*   EPIC_OPT_CRIT_ON           route to critical pin on
*
* \NOMANUAL
*
* RETURNS: OK or an error code, can be one of the following:
*          OK
*          ERROR
*          EPIC_VEC_OPTION_INV
*          EPIC_INV_INTER_SOURCE
*          EPIC_VEC_OPTION_NA
*          EPIC_VEC_HAS_NO_IDR
*          EPIC_INTER_IN_SERVICE
*
* ERRNO: N/A
*/

LOCAL int vxbEpicVecOptionsSet
    (
    ULONG       srcAddr,
    UINT32 	mask,
    UINT32 	options
    )
    {
    ULONG       vprVal;
    ULONG       idrVal;
    ULONG       vprFlag;
    ULONG       idrFlag;
    int         errCode;
    int         idrOffset = 0;              /* offset of IDR from VPR */

    if ((mask & ((0x3f30 << 16) | (0x3fff))) != 0)
	return (EPIC_VEC_OPTION_INV);

    errCode = (srcAddr & REG_TYPE_MASK) >> REG_TYPE_SHIFT;

    if (errCode == EPIC_INV_INTER_SOURCE)
	{
	return (errCode);
	}

    srcAddr &= REG_ADRS_MASK;

    vprFlag = mask & 0xffff0000;            /* upper sets upper VPR */
    idrFlag = (mask & 0x0000ffff) << 16;    /* lower sets upper IDR */

    switch (errCode)
	{
	case EPIC_EX_INTERRUPT:
	    idrOffset = EPIC_EX_DEST_REG_VECREGOFF;
	break;

	case EPIC_IN_INTERRUPT:
	    idrOffset = EPIC_IN_DEST_REG_VECREGOFF;
	    if ((vprFlag & EPIC_OPT_SENSE_MSK) != 0)
		return (EPIC_VEC_OPTION_NA);
	break;

	case EPIC_GT_A_INTERRUPT:
	case EPIC_GT_B_INTERRUPT:
	    if (idrFlag != 0)
		return (EPIC_VEC_HAS_NO_IDR);
	break;

	case EPIC_MSG_INTERRUPT:
	    idrOffset = EPIC_MSG_DEST_REG_VECREGOFF;
	    if ((vprFlag & (EPIC_OPT_POLAR_MSK | EPIC_OPT_SENSE_MSK)) != 0)
		return (EPIC_VEC_OPTION_NA);
	break;

        case EPIC_SMSG_INTERRUPT:
            idrOffset = EPIC_SMSG_DEST_REG_VECREGOFF;
            if ((vprFlag & (EPIC_OPT_POLAR_MSK | EPIC_OPT_SENSE_MSK)) != 0)
                return (EPIC_VEC_OPTION_NA);
        break;


	case EPIC_IPI_INTERRUPT:
	    if (idrFlag != 0)
		return (EPIC_VEC_HAS_NO_IDR);
	break;

	default:
	    return (ERROR);
	}

    vprVal = EPIC_REG_READ(srcAddr);

    if ((vprVal & EPIC_EIVPR_INTR_ACTIVE) != 0)
	return (EPIC_INTER_IN_SERVICE);

    if (vprFlag != 0)
	{
	vprVal &= ~(vprFlag);
	vprVal |= (options & 0xffff0000);
	EPIC_REG_WRITE(srcAddr, vprVal);
	}

    if ((idrFlag != 0) && (idrOffset != 0))
	{
	idrVal = EPIC_REG_READ((srcAddr + idrOffset));
	idrVal &= ~(idrFlag);
	idrVal |= (options & 0xffff) << 16;
	EPIC_REG_WRITE((srcAddr + idrOffset), idrVal);
	}

    return OK;

    }

#if 0	/* (not used) */
/*******************************************************************************
*
* vxbEpicVecOptionsGet - get options for a vector
*
* <srcAddr> is the address offset of the Vector Priority register
*
* Use one or more of the following masks to extract the returned value:
*   EPIC_OPT_EN_MSK
*   EPIC_OPT_POLAR_MSK
*   EPIC_OPT_SENSE_MSK
*   EPIC_OPT_PRI_MSK
*   EPIC_OPT_EXPIN_MSK
*   EPIC_OPT_CRIT_MSK
*
* \NOMANUAL
*
* RETURNS: OK, ERROR, or EPIC_INV_INTER_SOURCE
*
* ERRNO: N/A
*/

LOCAL UINT32 vxbEpicVecOptionsGet
    (
    ULONG       srcAddr
    )
    {
    ULONG       vprVal;
    ULONG       idrVal;
    int         errCode;
    int         idrOffset = 0;              /* offset of IDR from VPR */

    errCode = vxbEpicSrcAddrCheck (srcAddr);

    if (errCode == EPIC_INV_INTER_SOURCE)
	{
	return (errCode);
	}

    switch (errCode)
	{
	case EPIC_EX_INTERRUPT:
	    idrOffset = EPIC_EX_DEST_REG_VECREGOFF;
	break;

	case EPIC_IN_INTERRUPT:
	    idrOffset = EPIC_IN_DEST_REG_VECREGOFF;
	break;

	case EPIC_GT_INTERRUPT:
	break;

	case EPIC_MSG_INTERRUPT:
	    idrOffset = EPIC_MSG_DEST_REG_VECREGOFF;
	break;

        case EPIC_SMSG_INTERRUPT:
            idrOffset = EPIC_SMSG_DEST_REG_VECREGOFF;
        break;

	case EPIC_IPI_INTERRUPT:
	break;

	default:
	    return ((UINT32) ERROR);
	}

    vprVal = EPIC_REG_READ(srcAddr);
    if (idrOffset != 0)
	idrVal = EPIC_REG_READ(srcAddr + idrOffset);
    else
	idrVal = 0;

    return ((vprVal & 0xffff0000) | (idrVal >> 16));
    }
#endif

/*******************************************************************************
*
* vxbSysEpicIntConnect - connect an interrupt handler to the system vector table
*
* This function connects an interrupt handler to the system vector table.
*
* RETURNS: OK or ERROR.
*
* ERRNO: N/A
*/

LOCAL STATUS vxbSysEpicIntConnect 
    (
    VOIDFUNCPTR * 	vector,		/* interrupt vector to attach */
    VOIDFUNCPTR		routine,	/* routine to be called */
    int			parameter	/* parameter to be passed to routine */
    )
    {
    BOOL stateEnabled;

    stateEnabled = vxbEpicIntEnableState((int)vector);

    if(stateEnabled == TRUE)
	vxbSysEpicIntDisable((int)vector);

    intCtlrISRAdd(&(EPIC_DRV_CTRL(isrHandle)), (int)vector, routine, 
		  (void *)parameter);

    if(stateEnabled == TRUE)
	vxbSysEpicIntEnable((int)vector);

    intCtlrISREnable(&(EPIC_DRV_CTRL(isrHandle)),(int)vector, routine, 
		     (void *)parameter);

    return (vxbSysEpicIntConfig(vector, routine, parameter));
    }

/*******************************************************************************
*
* vxbSysEpicIntDisconnect - disconnect an interrupt handler from
                            the system vector table
*
* This function disconnects an interrupt handler from the system vector table.
*
* RETURNS: OK or ERROR.
*
* ERRNO: N/A
*/

LOCAL STATUS vxbSysEpicIntDisconnect 
    (
    VOIDFUNCPTR * 	vector,		/* interrupt vector to attach */
    VOIDFUNCPTR		routine,	/* routine to be called */
    int			parameter	/* parameter to be passed to routine */
    )
    {
    BOOL stateEnabled;

    stateEnabled = vxbEpicIntEnableState((int)vector);

    if(stateEnabled == TRUE)
	vxbSysEpicIntDisable((int)vector);

    return (intCtlrISRRemove(&(EPIC_DRV_CTRL(isrHandle)), (int)vector, routine, 
		             (void *)parameter));
    }

/*******************************************************************************
*
* vxbSysEpicIntConfig - configure newly connected interrupt
*
* RETURNS: OK or ERROR.
*
* ERRNO: N/A
*/

LOCAL STATUS vxbSysEpicIntConfig
    (
    VOIDFUNCPTR * 	vector,		/* interrupt vector to attach */
    VOIDFUNCPTR		routine,	/* routine to be called */
    int			parameter	/* parameter to be passed to routine */
    )
    {
    struct vxbIntCtlrPin * pPin = 
	 vxbIntCtlrPinEntryGet(&(EPIC_DRV_CTRL(isrHandle)),(int)vector);
    ULONG *     regTable = EPIC_DRV_CTRL(regTable);
    ULONG 	srcAddr = regTable[(int)vector];
    int         interruptType;
    int 	sense;
    int 	polar;
    int 	prio;

    if (((int)vector < 0)  || ((int) vector >= EPIC_DRV_CTRL(numInts)))
	{
	return (ERROR);   /*  out of range  */
	}

    if (pPin->isr == intCtlrChainISR)
	return (OK);

    /* process triggering / priority */
    sense = vxbEpicSenseGet (pPin->pinTrigger);
    polar = vxbEpicPolarGet (pPin->pinTrigger);
    prio  = vxbEpicPrioGet  (pPin->pinPriority);

    if ((int)vector >= 0 && (int)vector < EPIC_DRV_CTRL(numInts) )
	{
	ULONG vecreg = vxbEpicGetVecRegAdrs ((int)vector);
	ULONG destreg = vxbEpicGetDestRegAdrs ((int)vector);
	int cpu = (*vxbEpicCpu1Check)();

	/* EPIC IRQ set EPIC registers */
	
	interruptType = (srcAddr & REG_TYPE_MASK) >> REG_TYPE_SHIFT;

	switch(interruptType)
	    {
	    case EPIC_EX_INTERRUPT:
                if (polar == -1)
                    polar = EPIC_DRV_CTRL(exPolar);
                if (sense == -1)
                    sense = EPIC_DRV_CTRL(exSense);
                if (prio  == -1)
		    prio  = EPIC_PRIORITY_DEFAULT;

		vxbEpicIntSourceSet (vecreg, polar, sense, prio, (int) vector);

		if (cpu != 0)
		    vxbEpicCpuIntSet(destreg, cpu);
		break;

	    case EPIC_IN_INTERRUPT:
                if (polar == -1)
                    polar = EPIC_DRV_CTRL(inPolar);
                if (prio  == -1)
		    prio  = EPIC_PRIORITY_DEFAULT;

		vxbEpicIntSourceSet (vecreg, polar, 0x0, prio, (int) vector);

		if (cpu != 0)
		    vxbEpicCpuIntSet(destreg, cpu);
		break;
		    
	    case EPIC_GT_A_INTERRUPT:
	    case EPIC_GT_B_INTERRUPT:
                if (polar == -1)
                    polar = EPIC_DRV_CTRL(inPolar);
                if (prio  == -1)
		    prio  = EPIC_PRIORITY_DEFAULT;

		vxbEpicIntSourceSet (vecreg, polar, 0x0, prio, (int) vector);

		if (cpu != 0)
		    vxbEpicCpuIntSet(destreg, cpu);
		break;
		
	    case EPIC_MSG_INTERRUPT:
                if (prio  == -1)
		    prio  = EPIC_PRIORITY_DEFAULT;

		vxbEpicIntSourceSet (vecreg, 0x0, 0x0, prio, (int) vector);

		if (cpu != 0)
		    vxbEpicCpuIntSet(destreg, cpu);
		break;
		
	    case EPIC_SMSG_INTERRUPT:
                if (prio  == -1)
		    prio  = EPIC_PRIORITY_DEFAULT;

		vxbEpicIntSourceSet (vecreg, 0x0, 0x0, prio, (int) vector);

		if (cpu != 0)
		    vxbEpicCpuIntSet(destreg, cpu);
		break;
		
	    case EPIC_IPI_INTERRUPT:
                if (prio  == -1)
		    prio  = EPIC_PRIORITY_MAX;

		vxbEpicIntSourceSet (vecreg, 0x0, 0x0, prio, (int) vector);
		break;
	    default:
		vxbEpicIntSourceSet (vecreg, 0x0, 0x0, EPIC_PRIORITY_DEFAULT,
					 (int) vector);
		break;
	    }

	}

    return (OK);
    }

/*
*
* vxbSysEpicCrtIntHandler
*
*/

LOCAL void  vxbSysEpicCrtIntHandler (void)
    {
    unsigned long extSumm;
    unsigned long intSumm;
    int i;
    int intMask = vxPpcIntMask;
    struct intCtlrHwConf * pIsrHandle = pVxbEpicIsrHandle;
#if _WRS_CONFIG_SV_INSTRUMENTATION
    int loopCnt = -1;
#endif

    intSumm = EPIC_REG_READ(EPIC_CRIT_SUMM_REG1);
    extSumm = EPIC_REG_READ(EPIC_CRIT_SUMM_REG0) & 0xfff;

    do
	{
	/* service external first */

	if (extSumm)
	    {
	    /* get the least bit number */

	    i = ffsLsb (extSumm) - 1;

	    /* right shift the bits unless it's 0 */

	    extSumm >>= i;

	    do
		{
		if (extSumm & 1)
		    {
		    VXB_EPICINTCTLR_ENABLE_INTS (intMask);
#if _WRS_CONFIG_SV_INSTRUMENTATION
                    WV_EVT_INT_ENT(11-i)
                    loopCnt ++;
#endif /* _WRS_CONFIG_SV_INSTRUMENTATION */
		    VXB_INTCTLR_ISR_CALL(pIsrHandle, (11 - i));
		    VXB_EPICINTCTLR_DISABLE_INTS (intMask);
		    }

		i++;
		extSumm >>= 1;
		}
	    while (extSumm);	/* continue till done for all the interrupts */
	    }

	/* service internal */

	if (intSumm)
	    {
	    /* get the least bit number */

	    i = ffsLsb (intSumm) - 1;

	    /* right shift the bits unless it's 0 */

	    intSumm >>= i;

	    do
		{
		if (intSumm & 1)
		    {
		    VXB_EPICINTCTLR_ENABLE_INTS (intMask);
#if _WRS_CONFIG_SV_INSTRUMENTATION
                    WV_EVT_INT_ENT(EPIC_DRV_CTRL(inIrq0) + 31 - i)
                    loopCnt ++;
#endif /* _WRS_CONFIG_SV_INSTRUMENTATION */
		    VXB_INTCTLR_ISR_CALL(pIsrHandle, (EPIC_DRV_CTRL(inIrq0) + 31 - i));
		    VXB_EPICINTCTLR_DISABLE_INTS (intMask);
		    }

		i++;
		intSumm >>= 1;
		}
	    while (intSumm);	/* continue till done for all the interrupts */
	    }

	intSumm = EPIC_REG_READ(EPIC_CRIT_SUMM_REG1);
	extSumm = EPIC_REG_READ(EPIC_CRIT_SUMM_REG0) & 0xfff;
	}
    while (extSumm | intSumm);

#ifdef _WRS_CONFIG_SV_INSTRUMENTATION
    while (loopCnt-- > 0)
        EVT_CTX_0(EVENT_INT_EXIT);
#endif /* _WRS_CONFIG_SV_INSTRUMENTATION */
    }

#define EPIC_CTASK_PRIn_REG(cpu) (EPIC_CTASK_PRI0_REG + (0x1000 * cpu))

/*******************************************************************************
*
* vxbEpicCurTaskPrioSet - set the priority of the current task.
*
* NOTES
*
* vxbEpicCurTaskPrioSet sets the priority of the Processor Current Task
* Priority register to the value of the prioNum parameter.  This function
* should be called after vxbSysEpicInit() to lower the priority of the processor
* current task. Note that valid priority values are 0 through 15 (15 being
* the highest priority)
*
* \NOMANUAL
*
* RETURNS: previous priority of the task.
*
* ERRNO: N/A
*/

LOCAL int vxbEpicCurTaskPrioSet
    (
    int prioNum,
    int cpu
    )
    {
    ULONG oldPrio;

    if ((prioNum < EPIC_PRIORITY_MIN) || (prioNum > EPIC_PRIORITY_MAX))
	{
	return ((int) EPIC_INV_PRIO_ERROR);
	}

    oldPrio = EPIC_REG_READ(EPIC_CTASK_PRIn_REG(cpu));
    EPIC_REG_WRITE(EPIC_CTASK_PRIn_REG(cpu), prioNum);

    return (oldPrio);
    }

/*******************************************************************************
*
* vxbEpicIntEnable - enable an EPIC interrupt, given its IVPR
*
* This function clears the mask bit of an external, an internal or
* a Timer register to enable the interrupt.
*
* <srcAddr> is the address offset of the Vector Priority register
*
* \NOMANUAL
*
* RETURNS: OK or an error code if the IVPR passed in was invalid.
*
* ERRNO: N/A
*/

LOCAL int vxbEpicIntEnable
    (
    ULONG 	vector
    )
    {
    ULONG 	srcVal;
    int 	errCode;
    ULONG *     regTable = EPIC_DRV_CTRL(regTable);
    ULONG 	srcAddr = regTable[vector];
         
    errCode = (srcAddr & REG_TYPE_MASK) >> REG_TYPE_SHIFT;

    if (errCode == EPIC_INV_INTER_SOURCE)
	{
	return (errCode);
	}

    srcAddr &= REG_ADRS_MASK;

    srcVal = EPIC_REG_READ(srcAddr);
    switch (errCode)
	{
	case EPIC_EX_INTERRUPT:
	    srcVal &= ~(EPIC_EIVPR_INTR_MSK);   /* clear the mask bit */
	break;

	case EPIC_IN_INTERRUPT:
	    srcVal &= ~(EPIC_IIVPR_INTR_MSK);   /* clear the mask bit */
	break;

	case EPIC_GT_A_INTERRUPT:
	case EPIC_GT_B_INTERRUPT:
	    srcVal &= ~(EPIC_GTVPR_INTR_MSK);   /* clear the mask bit */
	break;

	case EPIC_MSG_INTERRUPT:
	    srcVal &= ~(EPIC_MIVPR_INTR_MSK);   /* clear the mask bit */
    
	break;

        case EPIC_SMSG_INTERRUPT:
            srcVal &= ~(EPIC_MSIVPR_INTR_MSK);   /* clear the mask bit */             
        break;

	case EPIC_IPI_INTERRUPT:
	    srcVal &= ~(EPIC_IPIVPR_INTR_MSK);  /* clear the mask bit */
	break;

	default:
	    return (ERROR);
	}

    EPIC_REG_WRITE(srcAddr, srcVal);

    return OK;

    }

/****************************************************************************
*
* vxbEpicIntEnableState - Check whether interrupt is enabled
*
* \NOMANUAL
*
*/

LOCAL BOOL vxbEpicIntEnableState
    (
    ULONG 	vector
    )
    {
    ULONG 	srcVal;
    int 	errCode = TRUE;
    STATUS      status = TRUE;
    ULONG *	regTable = EPIC_DRV_CTRL(regTable);
    ULONG 	srcAddr = regTable[vector];

    errCode = (srcAddr & REG_TYPE_MASK) >> REG_TYPE_SHIFT;

    if (errCode == EPIC_INV_INTER_SOURCE)
	{
	return (FALSE);
	}

    srcAddr &= REG_ADRS_MASK;

    srcVal = EPIC_REG_READ(srcAddr);

    switch (errCode)
	{
	case EPIC_EX_INTERRUPT:
	    if ((srcVal & EPIC_EIVPR_INTR_MSK) != 0)
		status = FALSE;		
	break;

	case EPIC_IN_INTERRUPT:
	    if ((srcVal & EPIC_IIVPR_INTR_MSK) != 0)
		status = FALSE;		
	break;

	case EPIC_GT_A_INTERRUPT:
	case EPIC_GT_B_INTERRUPT:
	    if ((srcVal & EPIC_GTVPR_INTR_MSK) != 0)
		status = FALSE;		
	break;

	case EPIC_MSG_INTERRUPT:
	    if ((srcVal & EPIC_MIVPR_INTR_MSK) != 0)
		status = FALSE;		
	break;

        case EPIC_SMSG_INTERRUPT:
	    if ((srcVal & EPIC_EIVPR_INTR_MSK) != 0)
		status = FALSE;		
            srcVal |= EPIC_MSIVPR_INTR_MSK;	/* set the mask bit */
        break;

	case EPIC_IPI_INTERRUPT:
	    if ((srcVal & EPIC_IPIVPR_INTR_MSK) != 0)
		status = FALSE;		
	break;

	default:
	    return (FALSE);
	}

    return status;
    }


/*******************************************************************************
*
* vxbEpicIntDisable - disable an EPIC interrupt, given its IVPR
*
* This function sets the mask bit of an external, an internal,
* a timer, a message, or an IPI register to disable the interrupt.
* <srcAddr> is the address offset of the Vector Priority register.
*
* \NOMANUAL
*
* RETURNS: OK or an error code if the IVPR passed in was invalid.
*
* ERRNO: N/A
*/

LOCAL int vxbEpicIntDisable
    (
    ULONG 	vector
    )
    {
    ULONG 	srcVal;
    int 	errCode;
    ULONG *	regTable = EPIC_DRV_CTRL(regTable);
    ULONG 	srcAddr = regTable[vector];

    errCode = (srcAddr & REG_TYPE_MASK) >> REG_TYPE_SHIFT;

    if (errCode == EPIC_INV_INTER_SOURCE)
	{
	return (errCode);
	}

    srcAddr &= REG_ADRS_MASK;

    srcVal = EPIC_REG_READ(srcAddr);

    switch (errCode)
	{
	case EPIC_EX_INTERRUPT:
	    srcVal |= EPIC_EIVPR_INTR_MSK;	/* set the mask bit */
	break;

	case EPIC_IN_INTERRUPT:
	    srcVal |= EPIC_IIVPR_INTR_MSK;	/* set the mask bit */
	break;

	case EPIC_GT_A_INTERRUPT:
	case EPIC_GT_B_INTERRUPT:
	    srcVal |= EPIC_GTVPR_INTR_MSK;	/* set the mask bit */
	break;

	case EPIC_MSG_INTERRUPT:
	    srcVal |= EPIC_MIVPR_INTR_MSK;	/* set the mask bit */
	break;

    case EPIC_SMSG_INTERRUPT:
            srcVal |= EPIC_MSIVPR_INTR_MSK;	/* set the mask bit */ 
        break;

	case EPIC_IPI_INTERRUPT:
	    srcVal |= EPIC_IPIVPR_INTR_MSK;	/* set the mask bit */
	break;

	default:
	    return (ERROR);
	}
  
    EPIC_REG_WRITE(srcAddr, srcVal);

    return OK;
    }

/*******************************************************************************
*
* vxbEpicCpuIntGet - get the CPU bit for an interrupt
*
* This function gets the CPU bit of an external, an internal,
* a timer, a message, or an IPI.
* <srcAddr> is the address offset of the Vector Destination register.
*
* \NOMANUAL
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL int vxbEpicCpuIntGet
    (
    ULONG  srcAddr
    )
    {
    ULONG srcVal;
    int i;

    /* parameters assumed to be legal */

    srcVal = EPIC_REG_READ(srcAddr);

    for (i = 0; i < EPIC_DRV_CTRL(numCpus); i++)
        {
        if (srcVal & EPIC_BIT(i))
            return (i);
        }

    return (-1); 
    }

/*******************************************************************************
*
* vxbEpicCpuIntSet - set the CPU bit for an interrupt
*
* This function sets the CPU bit of an external, an internal,
* a timer, a message, or an IPI to route the interrupt to the desired CPU.
* <srcAddr> is the address offset of the Vector Destination register.
*
* \NOMANUAL
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void vxbEpicCpuIntSet
    (
    ULONG  srcAddr,
    int cpu
    )
    {
    ULONG srcVal;
    int i;

    /* parameters assumed to be legal */

    srcVal = EPIC_REG_READ(srcAddr);

    for (i = 0; i < EPIC_DRV_CTRL(numCpus); i++)
        {
	if (i == cpu)
	    srcVal |= EPIC_BIT(i);	/* set Px bit corresponding to cpu */
        else
	    srcVal &= ~(EPIC_BIT(i));	/* clear Px bit not matching cpu */
        }

    EPIC_REG_WRITE(srcAddr, srcVal);
    }

#if 0	/* (not used) */
/*
*
*/
LOCAL int vxbEpicCrtIntSet
    (
    ULONG 	srcAddr
    )
    {
    ULONG 	srcVal;
    int 	errCode = EPIC_EX_INTERRUPT;    /* same bits for all types */

    srcVal = EPIC_REG_READ(srcAddr);

    switch (errCode)
	{
	case EPIC_EX_INTERRUPT:
	    srcVal |= EPIC_EIDR_CRIT_INT;       /* set critical bit */
	break;

	case EPIC_IN_INTERRUPT:
	    srcVal |= EPIC_IIDR_CRIT_INT;       /* set critical bit */
	break;

	case EPIC_GT_INTERRUPT:
	    return (ERROR);                     /* not supported */
	break;

	case EPIC_MSG_INTERRUPT:
	    srcVal |= EPIC_MIDR_CRIT_INT;       /* set critical bit */
	break;

        case EPIC_SMSG_INTERRUPT:
            srcVal |= EPIC_MSIDR_CRIT_INT;       /* set critical bit */
        break;

	case EPIC_IPI_INTERRUPT:
	    return (ERROR);                     /* not supported */
	break;

	default:
	    return (ERROR);
	}

    EPIC_REG_WRITE(srcAddr, srcVal);

    return OK;
    }

/*
*
*/
LOCAL int vxbEpicCrtIntUnset
    (
    ULONG 	srcAddr
    )
    {
    ULONG 	srcVal;
    int 	errCode = EPIC_EX_INTERRUPT;    /* same bits for all types */

    srcVal = EPIC_REG_READ(srcAddr);

    switch (errCode)
	{
	case EPIC_EX_INTERRUPT:
	    srcVal &= ~(EPIC_EIDR_CRIT_INT);    /* set critical bit */
	break;

	case EPIC_IN_INTERRUPT:
	    srcVal &= ~(EPIC_IIDR_CRIT_INT);    /* set critical bit */
	break;

	case EPIC_GT_INTERRUPT:
	    return (ERROR);                     /* not supported */
	break;

	case EPIC_MSG_INTERRUPT:
	    srcVal &= ~(EPIC_MIDR_CRIT_INT);    /* set critical bit */
	break;

        case EPIC_SMSG_INTERRUPT:
            srcVal &= ~(EPIC_MSIDR_CRIT_INT);    /* set critical bit */
        break;

	case EPIC_IPI_INTERRUPT:
	    return (ERROR);                     /* not supported */
	break;

	default:
	    return (ERROR);
	}

    EPIC_REG_WRITE(srcAddr, srcVal);

    return OK;
    }

/*
*
*/
LOCAL int vxbEpicCrtIntGet
    (
    ULONG 	srcAddr
    )
    {
    ULONG 	srcVal;
    int 	errCode = EPIC_EX_INTERRUPT;    /* same bits for all types */

    srcVal = EPIC_REG_READ(srcAddr);

    switch (errCode)
	{
	case EPIC_EX_INTERRUPT:
	    srcVal = (srcVal >> 30) & 1;
	break;
	}

    return srcVal;
}
#endif /* 0 */

/*******************************************************************************
*
* vxbEpicGetVecRegAdrs - translate a vector to vector reg address
*
* This routine translates a vector to vector reg address.
*
* \NOMANUAL
*
* RETURNS: vector register address of the corresponding vector type
*/

LOCAL ULONG vxbEpicGetVecRegAdrs
    (
    int vector
    )
    {
    ULONG * regTable = EPIC_DRV_CTRL(regTable);
    ULONG regAdrs;

    if ((vector < 0) || (vector >= EPIC_VEC_CTRL_EXT))
	return ((ULONG) ERROR);

    regAdrs = regTable[vector];

    if (regAdrs == 0)
	return ((ULONG) ERROR);
    else 
        return (regAdrs);
    }

/*******************************************************************************
*
* vxbEpicGetDestRegAdrs - translate a vector to destination reg address
*
* This routine translates a vector to destination reg address
*
* \NOMANUAL
*
* RETURNS: the destination register address of the corresponding vector type
*
* ERRNO: N/A
*/

LOCAL ULONG vxbEpicGetDestRegAdrs
    (
    int vector
    )
    {
    ULONG * regTable = EPIC_DRV_CTRL(regTable);
    ULONG regAdrs;

    if ((vector < 0) || (vector >= EPIC_VEC_CTRL_EXT))
	return ((ULONG) ERROR);

    regAdrs = regTable[vector];

    if (regAdrs == 0)
	return ((ULONG) ERROR);
    else 
        switch ((regAdrs & REG_TYPE_MASK) >> REG_TYPE_SHIFT)
            {
            case EPIC_GT_A_INTERRUPT:
            case EPIC_GT_B_INTERRUPT:
            case EPIC_IPI_INTERRUPT:
                return ((ULONG) ERROR);
            default:
                return (regAdrs + 0x10);
            }
    }

/*******************************************************************************
*
* vxbEpicIntSourceSet - set interrupt parameters for an interrupt register
*
* This function sets the interrupt parameters for:
*     External vector/priority register (EIVPR)
*     Internal vector/priority register (IIVPR)
*     Global Timer vector/priority register (GTVPR)
*     Message vector/priority register (MIVPR)
*     IPI vector/priority register (IPIVPR)
* The interrupt parameters can only be set when the current source is not
* in-request or in-service, which is determined by the Activity bit.  This
* routine reads the Activity bit; if the value of this bit is 1 (in-request
* or in-service), it returns error; otherwise, it sets the value of the
* parameters to the register offset defined in srcAddr.
*
* inputs:  srcAddr:   Address Offset of the source interrupt register.  This
*		      routine assumes that the srcAddr passed in is an valid
*		      Source Vector Priority address.
*          polarity: Use by external & internal interrupts only.
*                    1 -  Enable active high or positive edge
*                    0 -  Enable active low or negative edge
*          sense:    Use by external interrupts only.
*                    1 -  Enable level sensitive interrupts
*                    0 -  Enable edge sensitive interrupts
*          priority: valid number 0 to 15
*          vector:   valid number 0 - 65535 (16 bits)
*
* Errors:  EPIC_INV_INTER_SOURCE Invalid Source Address,
*          EPIC_INTER_IN_SERVICE Interrupt currently in service
*          ERROR Unknown type
*
* \NOMANUAL
*
* RETURNS: OK or one of errors above
*
* ERRNO: N/A
*/

LOCAL STATUS vxbEpicIntSourceSet
    (
    ULONG 	srcAddr,
    int 	polarity,
    int 	sense,
    int 	priority,
    int 	vector
    )
    {
    ULONG 	srcVal;
    ULONG	errCode;

    errCode = (srcAddr & REG_TYPE_MASK) >> REG_TYPE_SHIFT;

    if (errCode == EPIC_INV_INTER_SOURCE)
	{
	return (errCode);
	}

    srcAddr &= REG_ADRS_MASK;

    srcVal = EPIC_REG_READ(srcAddr);

    switch (errCode)
	{
	case EPIC_EX_INTERRUPT:
	    if (srcVal & EPIC_EIVPR_INTR_ACTIVE)
		{
		return (EPIC_INTER_IN_SERVICE);
		}
	    /* mask off current settings */
	    srcVal &= ~(EPIC_EIVPR_INTR_POLARITY |
			EPIC_EIVPR_INTR_SENSE |
			EPIC_EIVPR_PRIORITY_MSK  |
			EPIC_EIVPR_VECTOR_MSK);
	    /* set new values */
	    srcVal |= (EPIC_EIVPR_POLARITY (polarity) |
		       EPIC_EIVPR_SENS (sense) |
		       EPIC_EIVPR_PRIORITY (priority) |
		       EPIC_EIVPR_VECTOR (vector));
	break;

	case EPIC_IN_INTERRUPT:
	    if (srcVal & EPIC_IIVPR_INTR_ACTIVE)
		{
		return (EPIC_INTER_IN_SERVICE);
		}
	    /* mask off current settings */
	    srcVal &= ~(EPIC_IIVPR_INTR_POLARITY |
			EPIC_IIVPR_PRIORITY_MSK  |
			EPIC_IIVPR_VECTOR_MSK);
	    /* set new values */
	    srcVal |= (EPIC_IIVPR_POLARITY (polarity) |
		       EPIC_IIVPR_PRIORITY (priority) |
		       EPIC_IIVPR_VECTOR (vector));
	break;

	case EPIC_GT_A_INTERRUPT:
	case EPIC_GT_B_INTERRUPT:
	    if (srcVal & EPIC_GTVPR_INTR_ACTIVE)
		{
		return (EPIC_INTER_IN_SERVICE);
		}
	    /* mask off current settings */
	    srcVal &= ~(EPIC_GTVPR_PRIORITY_MSK  |
			EPIC_GTVPR_VECTOR_MSK);
	    /* set new values */
	    srcVal |= (EPIC_GTVPR_PRIORITY (priority) |
		       EPIC_GTVPR_VECTOR (vector));
	break;

	case EPIC_MSG_INTERRUPT:
	    if (srcVal & EPIC_MIVPR_INTR_ACTIVE)
		{
		return (EPIC_INTER_IN_SERVICE);
		}
	    /* mask off current settings */
	    srcVal &= ~(EPIC_MIVPR_PRIORITY_MSK  |
			EPIC_MIVPR_VECTOR_MSK);
	    /* set new values */
	    srcVal |= (EPIC_MIVPR_PRIORITY (priority) |
		       EPIC_MIVPR_VECTOR (vector));
	break;

        case EPIC_SMSG_INTERRUPT:
            if (srcVal & EPIC_MSIVPR_INTR_ACTIVE)
                {
                return (EPIC_INTER_IN_SERVICE);
                }
            /* mask off current settings */
            srcVal &= ~(EPIC_MSIVPR_PRIORITY_MSK  |
                        EPIC_MSIVPR_VECTOR_MSK);
            /* set new values */
            srcVal |= (EPIC_MSIVPR_PRIORITY (priority) |
                       EPIC_MSIVPR_VECTOR (vector));
        break;

	case EPIC_IPI_INTERRUPT:
	    if (srcVal & EPIC_IPIVPR_INTR_ACTIVE)
		{
		return (EPIC_INTER_IN_SERVICE);
		}
	    /* mask off current settings */
	    srcVal &= ~(EPIC_IPIVPR_PRIORITY_MSK  |
			EPIC_IPIVPR_VECTOR_MSK);
	    /* set new values */
	    srcVal |= (EPIC_IPIVPR_PRIORITY (priority) |
		       EPIC_IPIVPR_VECTOR (vector));
	break;

	default:
	    return (ERROR);
	}

    EPIC_REG_WRITE(srcAddr, srcVal);

    return (OK);
    }


#if 0	/* (not used) */
/*******************************************************************************
*
* vxbEpicIntSourceGet - retrieve information of an EPIC source vector register.
*
* This function retrieves information of an vxbEpic source vector register.
* The information includes the Enable bit, the polarity, sense bits, the
* interrupt priority and interrupt vector number.
* Input:  srcAddr   - address of the source vector register
* Output: enable    - whether the interrupt is enabled
*          polarity - interrupt polarity (high or low)
*          sense    - interrupt sense (level or edge)
*          Priority - interrupt priority
*          Vector   - interrupt vector number
*
* \NOMANUAL
*
* RETURNS: OK or ERROR or EPIC_INV_INTER_SOURCE
*
* ERRNO: N/A
*/

LOCAL STATUS vxbEpicIntSourceGet
    (
    ULONG 	srcAddr,
    int *	pEnableMask,
    int *	pPolarity,
    int *	pSense,
    int *	pPriority,
    int *	pVector
    )
    {
    ULONG 	srcVal;
    int 	errCode;

    errCode = vxbEpicSrcAddrCheck (srcAddr);

    if (errCode == EPIC_INV_INTER_SOURCE)
	{
	return errCode;
	}

    srcVal = EPIC_REG_READ(srcAddr);

    switch (errCode)
	{
	case EPIC_EX_INTERRUPT:
	    *pEnableMask  = (srcVal & EPIC_EIVPR_INTR_MSK)      >> 31;
	    *pPolarity    = (srcVal & EPIC_EIVPR_INTR_POLARITY) >> 23;
	    *pSense       = (srcVal & EPIC_EIVPR_INTR_SENSE)    >> 22;
	    *pPriority    = (srcVal & EPIC_EIVPR_PRIORITY_MSK)  >> 16;
	    *pVector      = (srcVal & EPIC_EIVPR_VECTOR_MSK);
	break;

	case EPIC_IN_INTERRUPT:
	    *pEnableMask  = (srcVal & EPIC_IIVPR_INTR_MSK)      >> 31;
	    *pPolarity    = (srcVal & EPIC_IIVPR_INTR_POLARITY) >> 23;
	    *pPriority    = (srcVal & EPIC_IIVPR_PRIORITY_MSK)  >> 16;
	    *pVector      = (srcVal & EPIC_IIVPR_VECTOR_MSK);
	break;

	case EPIC_GT_INTERRUPT:
	    *pEnableMask  = (srcVal & EPIC_GTVPR_INTR_MSK)      >> 31;
	    *pPriority    = (srcVal & EPIC_GTVPR_PRIORITY_MSK)  >> 16;
	    *pVector      = (srcVal & EPIC_GTVPR_VECTOR_MSK);
	break;

	case EPIC_MSG_INTERRUPT:
	    *pEnableMask  = (srcVal & EPIC_MIVPR_INTR_MSK)      >> 31;
	    *pPriority    = (srcVal & EPIC_MIVPR_PRIORITY_MSK)  >> 16;
	    *pVector      = (srcVal & EPIC_MIVPR_VECTOR_MSK);
	break;

        case EPIC_SMSG_INTERRUPT:
            *pEnableMask  = (srcVal & EPIC_MSIVPR_INTR_MSK)      >> 31;
            *pPriority    = (srcVal & EPIC_MSIVPR_PRIORITY_MSK)  >> 16;
            *pVector      = (srcVal & EPIC_MSIVPR_VECTOR_MSK);
        break;

	case EPIC_IPI_INTERRUPT:
	    *pEnableMask  = (srcVal & EPIC_IPIVPR_INTR_MSK)     >> 31;
	    *pPriority    = (srcVal & EPIC_IPIVPR_PRIORITY_MSK) >> 16;
	    *pVector      = (srcVal & EPIC_IPIVPR_VECTOR_MSK);
	break;

	default:
	    return (ERROR);
	}

    return (OK);
    }
#endif /* 0 */

/*******************************************************************************
*
* vxbEpicIntCtlrIpiCntrlGet - Method to retrieve IPI control structure
*
*/

LOCAL VXIPI_CTRL_INIT * vxbEpicIntCtlrIpiCntrlGet
    (
    VXB_DEVICE_ID pInst,
    void * pArg
    ) 
    {
    UINT32 frr = EPIC_REG_READ(EPIC_FEATURES_REG);

    vxEpicIpiCtrlInit.pCpus = (frr & EPIC_FRR_NCPU_MASK) >> EPIC_FRR_NCPU_SHIFT;
    vxEpicIpiCtrlInit.pCtlr = pInst;
    return(&vxEpicIpiCtrlInit);
    }

UINT32 vxbEpicTimerVecAllocCount = 0;

UINT32 vxbEpicTimerVecAlloc()
    {
    return(vxbEpicTimerVecAllocCount++);
    }
#if 0
int vxbEpicNextIntSource = 0;
int vxbEpicNextSharedInSource = 0;
#else
extern int msiIndex;
#endif
int vxbEpicSharedMsgVecAlloc()
{

#if 0
 int vector = ((vxbEpicNextIntSource) << 5) | vxbEpicNextSharedInSource;
 
/* FIXME try errata fix for 8641 and 8544 before rotating through vector 
 * allocation
 */

 vxbEpicNextIntSource= (vxbEpicNextIntSource+1) & (EPIC_MAX_SMSG_IRQS-1);
 
 if(vxbEpicNextIntSource==0)
     vxbEpicNextSharedInSource = (vxbEpicNextSharedInSource+1) & (EPIC_MAX_SMSG_SHARED_IRQS-1);
#else
	int vector ;
	int vxbEpicNextIntSource = msiIndex%8;
	int vxbEpicNextSharedInSource = msiIndex/8;			
	vector = ((vxbEpicNextIntSource) << 5) | vxbEpicNextSharedInSource;

#endif  
/* FIXME try errata fix for 8641 and 8544 before rotating through vector 
 * allocation
 */
 
 return(vector);
};


LOCAL STATUS vxbEpicIntCtlrDynaVecConnect
    (
    VXB_DEVICE_ID	pIntCtlr,
    VXB_DEVICE_ID	pDev,
    int vecCount,
    struct vxbIntDynaVecInfo* dynaVec
    )
    {
    struct vxbEpicIntCtlrDrvCtrl * pDrvCtrl = pIntCtlr->pDrvCtrl;
    int                 inputPin = ERROR;
    struct intCtlrHwConf *dynamicIsrHandle;
    UINT32 allocatedVector;
    STATUS status = ERROR;
    void *pArg;
    int indx=0;
    BOOL stateEnabled;

    /* If not the vector owner look for who is 
     *
     */     
   
    while(vecCount!=0)
	{
	/*
	 * Currently for EPIC, only PCI MSI and openPicTimer support
	 * dynamic vectors connect.
	 */
	
	if ( pDev->busID != VXB_BUSID_PCI )
	    {
	    /* global timer group A and group B use consecutive pin numbers */

	    allocatedVector = vxbEpicTimerVecAlloc();
	    inputPin = EPIC_DRV_CTRL(gtAIrq0) + allocatedVector;
	    
	    dynaVec->vecVal = inputPin;
	    dynaVec->vecAddr = 0;

	    stateEnabled = vxbEpicIntEnableState(inputPin);

	    if(stateEnabled == TRUE)
		vxbSysEpicIntDisable(inputPin);

	    intCtlrISRAdd(&pDrvCtrl->isrHandle, (int)inputPin, dynaVec->isr ,dynaVec->pArg);

	    /* Update table with device info */
	    intCtlrTableUserSet(&pDrvCtrl->isrHandle,
				(int)inputPin, 
				(char*)pDev->pName, 
				pDev->unitNumber,
				indx);
	    if(stateEnabled == TRUE)
		vxbSysEpicIntEnable(inputPin);

	    status = vxbSysEpicIntConfig((VOIDFUNCPTR *)inputPin, dynaVec->isr, (int)dynaVec->pArg);

	    if(status == ERROR)
		return(ERROR);
	    }
	else
	    {

	    /* Allocate Vector */

	    allocatedVector =  vxbEpicSharedMsgVecAlloc();

	    /* Set inputPin based on the allocation */
	    inputPin = EPIC_DRV_CTRL(smsgIrq0) + (allocatedVector >> 5);
	
	    pArg= (void*) (allocatedVector & 0x1f);

	    /* Program the Device  with the allocated vector */
	    dynaVec->vecVal = allocatedVector;
	    dynaVec->vecAddr = EPIC_MSIIR_REG;
#if 0 //    deleted by ff 2011.07.06
	    if (_func_vxbIntDynaVecProgram != NULL)
         {
		if (_func_vxbIntDynaVecProgram(pIntCtlr,pDev,dynaVec) == ERROR)
		{
	 	    return (ERROR); 	  	    
		}
         }
	    else
			return(ERROR);
#endif   
	    /* Get the correct interrupt table based on allocated vector */
	    dynamicIsrHandle  = (struct intCtlrHwConf*)intCtlrTableArgGet(&pDrvCtrl->isrHandle, inputPin);

	    /*
	     * Store the MSI source.
	     *
	     * XXX:
	     * Temporary use the reserved field in the top-level table of
	     * the interrupt input pin entries for a workaround.
	     */

	    dynamicIsrHandle->pTop->reserved = (allocatedVector >> 5);

	    /* Update table with device info */
	    intCtlrTableUserSet(&pDrvCtrl->isrHandle,
				(int)inputPin, 
				(char*)"dynamic", 
				(allocatedVector >> 5),
				0);

	    stateEnabled = vxbEpicIntEnableState(inputPin);
	    if(stateEnabled == TRUE)
		vxbSysEpicIntDisable(inputPin);

	    /* Add handlers to specific interrupt table */
	    status = intCtlrISRAdd(dynamicIsrHandle, (int)pArg, dynaVec->isr ,dynaVec->pArg);
		
	    /* Update table with device info */
	    intCtlrTableUserSet(dynamicIsrHandle,
				(int)pArg, 
				(char*)pDev->pName,
				pDev->unitNumber,
				indx);
	    if(stateEnabled == TRUE)
		vxbSysEpicIntEnable(inputPin);

	    if(status == ERROR) 
		return(ERROR);
    
	    /* Connect Epic Interrupt */
	
	    status = vxbSysEpicIntConfig((VOIDFUNCPTR *)inputPin, dynaVec->isr, (int)dynaVec->pArg);

	    if(status == ERROR)
		return(ERROR);
	    }

	vecCount--;
	dynaVec++;
	indx++;
	}

    return(OK);
    }

/*******************************************************************************
*
* vxbEpicIntCtlrDevIsrConnect - connect device ISR
*
*/

LOCAL STATUS vxbEpicIntCtlrDevIsrConnect
    (
    VXB_DEVICE_ID	pIntCtlr,
    VXB_DEVICE_ID	pDev,
    int			indx,
    void		(*pIsr)(void * pArg),
    void *		pArg,
    int *		pInputPin
    )
    {
    struct vxbEpicIntCtlrDrvCtrl * pDrvCtrl = pIntCtlr->pDrvCtrl;
    int                 inputPin = ERROR;
    struct vxbIntDynaVecInfo dynaVec;
    BOOL stateEnabled;

    inputPin = intCtlrPinFind(pDev, indx, pIntCtlr, &pDrvCtrl->isrHandle);

    if ( inputPin == ERROR )
        return(ERROR);

    /* If the inputPin is dynamic we need to do the following:
     *
     * Allocate a vector 
     * Connect ISR to selected interrupt table (dynamicIsrHandle).
     * Update the interrupt table with device information.
     * Finally configure the Epic
     */
    if (inputPin == VXB_INTR_DYNAMIC)
	{
	/* Check device for local allocation */
	struct intCtlrHwConf *dynamicIsrHandle;
	UINT32 allocatedVector;
	STATUS status = ERROR;

	/*
	 * Currently for EPIC, only PCI MSI and openPicTimer support
	 * dynamic vectors connect.
	 */

	if ( pDev->busID != VXB_BUSID_PCI )
	    {
	    /* global timer group A and group B use consecutive pin numbers */
		
	    allocatedVector = vxbEpicTimerVecAlloc();
	    inputPin = EPIC_DRV_CTRL(gtAIrq0) + allocatedVector;
	    dynaVec.vecVal = inputPin;
	    dynaVec.vecAddr = 0;


	    stateEnabled = vxbEpicIntEnableState(inputPin);
	    if(stateEnabled == TRUE)
		vxbSysEpicIntDisable(inputPin);

	    intCtlrISRAdd(&pDrvCtrl->isrHandle, (int)inputPin, pIsr ,pArg);

	    /* Update table with device info */
	    intCtlrTableUserSet(&pDrvCtrl->isrHandle,
				(int)inputPin, 
				(char*)pDev->pName, 
				pDev->unitNumber,
				indx);

	    if(stateEnabled == TRUE)
		vxbSysEpicIntEnable(inputPin);
	    }
	else
	    {
	    /* protect alloc! */
	    allocatedVector =  vxbEpicSharedMsgVecAlloc();
	    inputPin = EPIC_DRV_CTRL(smsgIrq0) + (allocatedVector >> 5);  
	    
	    dynaVec.isr = pIsr;
	    dynaVec.pArg= (void*) (allocatedVector & 0x1f);
	    
	    dynaVec.vecVal = allocatedVector;
	    dynaVec.vecAddr = EPIC_MSIIR_REG;

	
	    if (_func_vxbIntDynaVecProgram !=NULL)
		status = _func_vxbIntDynaVecProgram(pIntCtlr,pDev,&dynaVec);
	    else
		return(ERROR);
	
	    if(status == ERROR) 
		return(ERROR);
	
	    dynamicIsrHandle  = (struct intCtlrHwConf*)intCtlrTableArgGet(&pDrvCtrl->isrHandle, inputPin);

	    /*
	     * Store the MSI source.
	     *
	     * XXX:        
	     * Temporary use the reserved field in the top-level table of 
	     * the interrupt input pin entries for a workaround.
	     */

	    dynamicIsrHandle->pTop->reserved = (allocatedVector >> 5);

	    intCtlrTableUserSet(dynamicIsrHandle,
				(int)dynaVec.pArg, 
				(char*)pDev->pName, 
				pDev->unitNumber,
				indx);

	    /* Update table with device info */
	    intCtlrTableUserSet(&pDrvCtrl->isrHandle,
				(int)inputPin, 
				(char*)pDev->pName, 
				pDev->unitNumber,
				indx);

	    stateEnabled = vxbEpicIntEnableState(inputPin);

	    if(stateEnabled == TRUE)
		vxbSysEpicIntDisable(inputPin);

	    intCtlrISRAdd(dynamicIsrHandle, (int)dynaVec.pArg, pIsr ,pArg);

	    if(stateEnabled == TRUE)
		vxbSysEpicIntEnable(inputPin);

	    }
	}
    else
	{
	stateEnabled = vxbEpicIntEnableState(inputPin);

	if(stateEnabled == TRUE)
	    vxbSysEpicIntDisable(inputPin);

	intCtlrISRAdd(&pDrvCtrl->isrHandle, inputPin, pIsr, pArg);
	
	if(stateEnabled == TRUE)
	    vxbSysEpicIntEnable(inputPin);
	}

    *pInputPin = inputPin;
 
    return (vxbSysEpicIntConfig((VOIDFUNCPTR *)inputPin, pIsr, (int)pArg));
    }

/*******************************************************************************
*
* vxbEpicIntCtlrDevIsrDisconnect - disconnect device ISR
*
*  FIXME: This needs to be implemented
*
*/

LOCAL STATUS vxbEpicIntCtlrDevIsrDisconnect
    (
    VXB_DEVICE_ID	pIntCtlr,
    VXB_DEVICE_ID	pDev,
    int			indx,
    void		(*pIsr)(void * pArg),
    void *		pArg
    )
    {
    return(ERROR);
    }

/*******************************************************************************
*
* vxbEpicIntCtlrDevIsrEnable - enable device ISR
*
*/

LOCAL STATUS vxbEpicIntCtlrDevIsrEnable
    (
    VXB_DEVICE_ID	pIntCtlr,
    VXB_DEVICE_ID	pDev,
    int			indx,
    void		(*pIsr)(void * pArg),
    void *		pArg
    )
    {
    struct vxbEpicIntCtlrDrvCtrl * pDrvCtrl = pIntCtlr->pDrvCtrl;
    int                 inputPin;
    struct vxbIntDynaVecInfo dynaVec;
    
    inputPin = intCtlrPinFind(pDev, indx, pIntCtlr, &pDrvCtrl->isrHandle);
    
    if ( inputPin == ERROR )
        return(ERROR);

    /* Note shared message interrupt already enabled we will enable the 
     * actual MSI or device interrrupt here. What determines wether 
     * VXB_INTR_DYNAMIC or the actual source is specified depends on wether 
     * vxbIntDynaConnect or vxbIntConnect was called.
     * NOTE For enable the timer interrupt is just standard */

    if ( (inputPin == VXB_INTR_DYNAMIC) ||
	 ( (inputPin >= EPIC_DRV_CTRL(smsgIrq0)) && 
	   (inputPin < EPIC_DRV_CTRL(smsgIrq0) + EPIC_MAX_SMSG_IRQS)))
	{
	/* VecEnable */
	struct intCtlrHwConf* dynamicIsrHandle;
	
	/* This also gets the MSI vector info set in Connect */
	if(_func_vxbIntDynaVecEnable!=NULL)
           {
	   if (_func_vxbIntDynaVecEnable(pIntCtlr,pDev,&dynaVec) == ERROR)
	       return ERROR;
	  
	   inputPin = EPIC_DRV_CTRL(smsgIrq0) + 
	     ((dynaVec.vecVal >> 5) & (EPIC_MAX_SMSG_IRQS-1));
	   
	   /* Use info from DynaEnable call to get dynamicIsrHandle */
	   dynamicIsrHandle  = (struct intCtlrHwConf*)intCtlrTableArgGet(&pDrvCtrl->isrHandle,inputPin); 	   
	   intCtlrISREnable(dynamicIsrHandle, 
			    (int)(dynaVec.vecVal >> 5) & (EPIC_MAX_SMSG_SHARED_IRQS - 1), 
			    pIsr ,
			    pArg);	   
	   }
	else
	    {
	    /* Case where no dynamic interrupt controller library do default */
	    intCtlrISREnable(&(pDrvCtrl->isrHandle), inputPin, pIsr, pArg);
	    }
	}
    else
	{
	/* This enable is already done for MSI interrupts */
	intCtlrISREnable(&(pDrvCtrl->isrHandle), inputPin, pIsr, pArg);
	}
    vxbSysEpicIntEnable(inputPin);
    return(OK);
    }

/*******************************************************************************
*
* vxbEpicIntCtlrDevIsrDisable - Disable device ISR
*
*/

LOCAL STATUS vxbEpicIntCtlrDevIsrDisable
    (
    VXB_DEVICE_ID	pIntCtlr,
    VXB_DEVICE_ID	pDev,
    int			indx,
    void		(*pIsr)(void * pArg),
    void *		pArg
    )
    {
    struct vxbEpicIntCtlrDrvCtrl * pDrvCtrl = pIntCtlr->pDrvCtrl;
    int                 inputPin;
    BOOL		allDisabled;
    struct vxbIntDynaVecInfo dynaVec;
    
    inputPin = intCtlrPinFind(pDev, indx, pIntCtlr,
    		 	      &pDrvCtrl->isrHandle);
    if ( inputPin == ERROR )
        return(ERROR);

    if( inputPin == VXB_INTR_DYNAMIC)
	{
	/* VecDisable */
	if(_func_vxbIntDynaVecDisable!=NULL)
	    _func_vxbIntDynaVecDisable(pIntCtlr,pDev,&dynaVec);
	else
	    return(ERROR);
	/* We do not ever Free shared message interrupt */
	}
    else
	{
	allDisabled = intCtlrISRDisable(&(pDrvCtrl->isrHandle), inputPin,
					pIsr, pArg);

	if (allDisabled)
	    vxbSysEpicIntDisable(inputPin);
	}

    return(OK);
    }

/*******************************************************************************
*
* vxbEpicIntCtlrCpuDisable - disable interrupts to specified CPU
*
* Disable interrupts that are configured for a CPU.
*
* RETURNS: OK if operation successful else ERROR
*
* ERRNO
*/

LOCAL STATUS vxbEpicIntCtlrCpuDisable
    (
    VXB_DEVICE_ID       pDev,
    void *              destCpu
    )
    {
    int i;
    ULONG vec, dest;

    for (i = 0; i < EPIC_DRV_CTRL(numInts); i++)
        {
        vec = vxbEpicGetVecRegAdrs (i);
        dest = vxbEpicGetDestRegAdrs (i);

        if (vec == ERROR || dest == ERROR)
	    continue;

        if (vxbEpicCpuIntGet(dest) != (int)destCpu)
            continue;

        /* mask interrupt source */
        while (vxbEpicVecOptionsSet (vec, EPIC_OPT_EN_MSK, EPIC_OPT_EN_N) 
               == EPIC_INTER_IN_SERVICE)
            /* wait */ ;
        }

    return OK;
    }

#ifdef _WRS_CONFIG_SMP

/*******************************************************************************
*
* vxbEpicIntCtlrCpuReroute - reroute interrupts to specified CPU
*
* Reroute interrupts that are configured in hwconf.c for a CPU other than
* the default CPU to that CPU.  
*
* RETURNS: OK if operation successful else ERROR
*
* ERRNO
*/

LOCAL STATUS vxbEpicIntCtlrCpuReroute
    (
    VXB_DEVICE_ID       pDev,
    void *              destCpu
    )
    {
    struct vxbEpicIntCtlrDrvCtrl * pDrvCtrl = pDev->pDrvCtrl;
    struct intCtlrHwConf *isrHandle = &(pDrvCtrl->isrHandle);
    int i;
    int configCpu;
    BOOL flag;
    ULONG vec, dest;
    void (*func)();

    if ((int)destCpu >= pDrvCtrl->numCpus) 
        return (ERROR);

    /* move interrupts to requested cpu */

    for (i = 0; i < EPIC_DRV_CTRL(numInts); i++)
	{
	/* verify this is an allocated pin so NULL references are avoided */

        VXB_INTCTLR_PINENTRY_ALLOCATED(isrHandle, i, flag);

	if (flag)
	    {
	    /* only move interrupts that are configured with an ISR */

	    EPIC_ISR(isrHandle, i, func);
	    if (func == NULL || func == intCtlrStrayISR)
		continue;

            /* move interrupts that are configured for the requested CPU */

	    EPIC_DESTCPU(isrHandle, i, configCpu);

	    if(configCpu == (int)destCpu)
		{
	        vec = vxbEpicGetVecRegAdrs (i);
	        dest = vxbEpicGetDestRegAdrs (i);

		if (vec == ERROR || dest == ERROR)
		    continue;

		/* mask interrupt source */
		while (vxbEpicVecOptionsSet (vec, EPIC_OPT_EN_MSK, 
		       EPIC_OPT_EN_N) == EPIC_INTER_IN_SERVICE)
                   /* wait */ ;

		/* set route bits */ 
		vxbEpicCpuIntSet (dest, (int)destCpu);

		/* unmask interrupt source */
		vxbEpicVecOptionsSet (vec, EPIC_OPT_EN_MSK, EPIC_OPT_EN_Y);
		}
	    }
	}

    return (OK);
    }

/******************************************************************************
*
* vxbEpicIntCtlrIntReroute - reroute interrupt to specified CPU
*
* Reroute device interrupt to requested CPU.  note that the cpu is specified
* in a cpuset_t type.  this would allow for multiple cpus to be specified
* but only 1 cpu being specified is valid.  specifying more than 1 cpu
* will return an ERROR.
*
* RETURNS: OK if operation successful else ERROR
*
* ERRNO
*/

LOCAL STATUS vxbEpicIntCtlrIntReroute
    (
    VXB_DEVICE_ID       pDev,
    int                 index,
    cpuset_t            destCpu
    )
    {
    struct intCtlrHwConf *isrHandle = &(EPIC_DRV_CTRL(isrHandle));
    ULONG vec, dest;
    BOOL flag;
    int cpu = 0;
    int bitCnt = 0;
    int i;
    int inputPin;

    /* convert cpuset_t cpu number to numeric cpu number */

    for (i = 0; i < EPIC_DRV_CTRL(numCpus); i++)
        if (destCpu & (1 << i))
            {
            bitCnt++;
            cpu = i;
            }

    /* make sure destCpu is a proper cpuset_t value */

    if (bitCnt != 1)
        return (ERROR);

    /* make sure pin is allocated */

    VXB_INTCTLR_PINENTRY_ALLOCATED(isrHandle, index, flag);

    if (!flag)
	return (ERROR);

    /* find the device input pin number */

    inputPin = intCtlrPinFind (pDev, index, epicVxbId, isrHandle);
    if (inputPin == ERROR)
        return (ERROR);

    vec = vxbEpicGetVecRegAdrs (inputPin);
    dest = vxbEpicGetDestRegAdrs (inputPin);

    if (vec == ERROR || dest == ERROR)
	return (ERROR);

    /* mask interrupt source */
    while (vxbEpicVecOptionsSet (vec, EPIC_OPT_EN_MSK, EPIC_OPT_EN_N) 
           == EPIC_INTER_IN_SERVICE)
        /* wait */ ;

    /* set route bits */ 
    vxbEpicCpuIntSet (dest, cpu);

    /* unmask interrupt source */
    vxbEpicVecOptionsSet (vec, EPIC_OPT_EN_MSK, EPIC_OPT_EN_Y);

    return (OK);
    }

#endif /* _WRS_CONFIG_SMP */

/*******************************************************************************
*
* vxbEpicIpiConnect - Connect ISR to IPI
*
*/

LOCAL STATUS vxbEpicIpiConnect
    (
    VXB_DEVICE_ID pCtlr,
    INT32 ipiId,
    IPI_HANDLER_FUNC ipiHandler,
    void * ipiArg
    )
    {
    INT32 lvl;
    
    lvl = ipiId + EPIC_DRV_CTRL(ipiIrq0);
    return(vxbSysEpicIntConnect((VOIDFUNCPTR *)lvl,
                      (VOIDFUNCPTR)ipiHandler, (int)ipiArg));
    }

/*******************************************************************************
*
* vxbEpicIpiDisconn - Disconnect ISR from IPI
*
*/

LOCAL STATUS vxbEpicIpiDisconn
    (
    VXB_DEVICE_ID pCtlr,
    INT32 ipiId,
    IPI_HANDLER_FUNC ipiHandler,
    void * ipiArg
    )
    {
    INT32 lvl;

    lvl = ipiId + EPIC_DRV_CTRL(ipiIrq0);
    return(vxbSysEpicIntDisconnect((VOIDFUNCPTR *)lvl,
                      (VOIDFUNCPTR)ipiHandler, (int)ipiArg));
    }

/*******************************************************************************
*
* vxbEpicIpiEnable - Enable specified IPI
*
*/

LOCAL STATUS vxbEpicIpiEnable
    (
    VXB_DEVICE_ID pCtlr,
    INT32 ipiId
    )
    {
    INT32 lvl;

    lvl = ipiId + EPIC_DRV_CTRL(ipiIrq0);
    return(vxbSysEpicIntEnable(lvl));
    }

/*******************************************************************************
*
* vxbEpicIpiDisable - Disable specified IPI
*
*/

LOCAL STATUS vxbEpicIpiDisable
    (
    VXB_DEVICE_ID pCtlr,
    INT32 ipiId
    )
    {
    INT32 lvl;

    lvl = ipiId + EPIC_DRV_CTRL(ipiIrq0);
    return(vxbSysEpicIntDisable(lvl));
    }

/*******************************************************************************
*
* vxbEpicIpiPrioGet - Retrieve IPI priority
*
*/

LOCAL INT32 vxbEpicIpiPrioGet
    (
    VXB_DEVICE_ID pCtlr,
    INT32 ipiId
    )
    {
    INT32 priority = -1;
    UINT32 vec;

    vec = vxbEpicGetVecRegAdrs (ipiId + EPIC_DRV_CTRL(ipiIrq0));

    if (vec != ERROR)
        priority = EPIC_IPIVPR_PRIORITY_GET(EPIC_REG_READ(vec));

    return(priority);
    }

/*******************************************************************************
*
* vxbEpicIpiPrioSet - Set IPI priority
*
*/

LOCAL STATUS vxbEpicIpiPrioSet
    (
    VXB_DEVICE_ID pCtlr,
    INT32 ipiId,
    INT32 prio
    )
    {
    UINT32 vec;
    INT32 vecVal;

    if (prio < 0 || prio > 15)
        return (ERROR);

    vec = vxbEpicGetVecRegAdrs (ipiId + EPIC_DRV_CTRL(ipiIrq0));

    if (vec == ERROR)
	return (ERROR);
    
    /* mask interrupt source */
    while (vxbEpicVecOptionsSet (vec, EPIC_OPT_EN_MSK, EPIC_OPT_EN_N) == 
	   EPIC_INTER_IN_SERVICE)
        /* wait */ ;

    /* set priority bits */ 
    vecVal = EPIC_REG_READ(vec);
    vecVal &= ~(EPIC_IPIVPR_PRIORITY_MSK);
    vecVal |= EPIC_IPIVPR_PRIORITY(prio);
    EPIC_REG_WRITE(vec, vecVal);

    /* unmask interrupt source */
    vxbEpicVecOptionsSet (vec, EPIC_OPT_EN_MSK, EPIC_OPT_EN_Y);

    return (OK);
    }

/*******************************************************************************
*
* vxbEpicCpu1CheckFalse - determine which CPU is executing (single CPU version)
*
*/

LOCAL int vxbEpicCpu1CheckFalse (void)
    {
    return (0);
    }

/*******************************************************************************
*
* vxbEpicCpuAvailGet - method to obtain number of CPUs available
*
*/

LOCAL STATUS vxbEpicCpuAvailGet 
    (
    VXB_DEVICE_ID pDev,
    UINT32 * pAvail
    )
    {
    UINT32 frr = EPIC_REG_READ(EPIC_FEATURES_REG);
    *pAvail = ((frr & EPIC_FRR_NCPU_MASK) >> EPIC_FRR_NCPU_SHIFT) + 1;
    return (OK);
    }

/*******************************************************************************
*
* vxbEpicSenseGet - get sense setting from pin structure
*
* RETURNS: sense setting, or -1 if invalid
*/

LOCAL int vxbEpicSenseGet
    (
    UINT8 trigger
    )
    {
    if (trigger & VXB_INTR_TRIG_EDGE)
        return 0;
    if (trigger & VXB_INTR_TRIG_LEVEL)
        return 1;
    return -1;
    }

/*******************************************************************************
*
* vxbEpicPolarGet - get polarity setting from pin structure
*
* RETURNS: polarity setting, or -1 if invalid
*/

LOCAL int vxbEpicPolarGet
    (
    UINT8 trigger
    )
    {
    if (trigger & VXB_INTR_TRIG_NEG)
        return 0;
    if (trigger & VXB_INTR_TRIG_POS)
        return 1;
    return -1;
    }

/*******************************************************************************
*
* vxbEpicPrioGet - get priority setting from pin structure
*
* RETURNS: priority setting, or -1 if invalid
*/

LOCAL int vxbEpicPrioGet
    (
    UINT16 priority
    )
    {
    BOOL force = (priority & VXB_INTR_PRIO_FORCE) ? TRUE : FALSE;
    priority &= ~VXB_INTR_PRIO_FORCE;

    /* Setting will be zero by default.  Interpret this as 'not set'
     * unless force bit is set. 
     */
    if ((priority == EPIC_PRIORITY_MIN && !force) ||
        (priority >  EPIC_PRIORITY_MAX))
        return -1;
        
    return (int) priority;
    } 

#ifdef VXB_EPICINTCTLR_DBG_ON

/*******************************************************************************
*
* vxbEpicIntShow - show interrupt number assignment
*
*/

void vxbEpicIntShow (void)
    {
    int i;
    INT_BANK_DESC * pDesc;
    char * typestr;
    int startIrq;

    printf ("\n"); 
    printf ("EPIC interrupt number assignment\n");
    printf ("--------------------------------\n");
    for (i = 0; i < INT_BANK_DESC_NUM; i++)
        {
        pDesc = EPIC_DRV_CTRL(intBankDesc[i]);
        
        if (pDesc == NULL || pDesc->numIrqs == 0)
            continue;

        switch (pDesc->type)
            {
            case EPIC_EX_INTERRUPT:
		typestr = "external";
                startIrq = EPIC_DRV_CTRL(extIrq0);
                break;
            case EPIC_IN_INTERRUPT:
		typestr = "internal";
                startIrq = EPIC_DRV_CTRL(inIrq0);
                break;
            case EPIC_GT_A_INTERRUPT:
		typestr = "global timer A";
                startIrq = EPIC_DRV_CTRL(gtAIrq0);
                break;
            case EPIC_GT_B_INTERRUPT:
		typestr = "global timer B";
                startIrq = EPIC_DRV_CTRL(gtBIrq0);
                break;
            case EPIC_MSG_INTERRUPT:
		typestr = "message";
                startIrq = EPIC_DRV_CTRL(msgIrq0);
                break;
            case EPIC_SMSG_INTERRUPT:
		typestr = "shared message";
                startIrq = EPIC_DRV_CTRL(smsgIrq0);
                break;
            case EPIC_IPI_INTERRUPT:
		typestr = "interprocessor";
                startIrq = EPIC_DRV_CTRL(ipiIrq0);
                break;
            default:
		typestr = "unknown";
                startIrq = 0xff; 
            }

        printf ("  %d %s interrupts (%d - %d)", 
                pDesc->numIrqs, typestr, pDesc->startIrq, 
                (pDesc->startIrq + pDesc->numIrqs - 1));
        if (pDesc->startIrq != startIrq)
            printf (" MISMATCH! (%d)", startIrq);       

        printf ("\n");
        }

    printf ("\n"); 
    }
#endif
