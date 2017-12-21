/* m5200FecEnd.c - Motorola MPC5200 END style FEC Ethernet network interface driver */

/*
 * Copyright (c) 1998, 2003-2008, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01p,25jan10,c_t  replace '0' with phyaddr in drvctrl structure. (WIND00197254)
01o,21jan10,liu  change task IPR number, set PTD control register to TASK 
                 priority mode. (WIND00162040)
01n,05feb10,c_t  m5200FecEnd auto neg may fail with high sysClkRate
01m,15may08,d_l  add polling task to check link status.(WIND00097032)
01l,07aug07,agf  remove if_subr.h include, not compat. with IPNet stack
01k,21feb06,pkr  bestcomm reentrancy (bit register access)
01j,21feb06,pkr  code cleanup (remove non bestcomm variant)
01i,18mar05,j_b  code cleanup (apigen)
01h,17feb05,j_b  set an undocumented XFIFO Control Register bit to fix Rev.B
01g,17jun04,bjn  RFIFO_ERROR - changes to fecStop/fecStart.
01f,01jun04,bjn  Add _CACHE_ALIGN_SIZE to cluster length.
01e,25may04,bjn  resolve various issues, see change log details (SPR97198)
01d,17mar04,bjn  Merging in external updates
01c,04feb04,bjn  Setting m5200FecForceCopy to TRUE
01b,18Jul03,pkr  adopted from motFecEnd
01a,09nov98,cn   written.
*/

/*
change log details
------------------
01g * RFIFO_ERROR/XFIFO_ERROR error interrupt - the interrupt was not
      clearing and the CPU appears to hang.
      The change made is quite significant: after fixing existing code the
      problem of not recovering from the error interrupt was still occurring,
      but not so frequently - some interrupts would recover, while others would
      not.The final change was made to schedule a call to m5200FecStop
      followed by m5200FecStart. This code resets the FEC by setting the
      RESET bit within the FEC Ethernet Control Register. With this change
      in place, the RFIFO_ERROR has always  cleared successfully.
      This change was also made to the XFIFO_ERROR interrupt as well, although
      this error interrupt does not occur very often.
    * FEC_END_GRA_SEM_TAKE: No interrupt generated when the FEC is disabled
      so add a timeout value.
    * Added m5200FecTbdReclaim and m5200FecTxRestart
    * Removed m5200FecTxRestart and m5200FecRxRestart as they are not
      currently required.
    * m5200FecMiiAnStart() - moved log message "m5200FecMiiAnStart no link"
      to an error path.
    * Added SEND_IN_COPY_MODE - with m5200FecForceCopy set to true, function
      m5200FecPktTransmit() so using this macro, the function can be
      removed from the build.
    * Added FEC_END_TST code: Used for testing purposes only.
    * Made code build when FEC_END_DBG is not defined.
    * m5200FecReset: Verify FEC comes out of RESET before returning.
01f * In change 01e, 12 bytes was added to the size of the clusters. Further
      debug of this problem showed that it was not an overshoot causing the
      tNetTask exception, but the lenght of the cluster did not allow for
      _CACHE_ALIGN_SIZE at the end of the RX frame and the start of the
      next cluster. So, we must replace the 12 with _CACHE_ALIGN_SIZE for
      FEC_END_MAX_CL_LEN. (Cache Invalidate is the cause of the problem.)
01e * FEC_END_MAX_CL_LEN: under very heavy network load with maximum frame
      size, the SDMA transfer of RX packets to the RX BD memory appear to
      experience a slight overshoot. The overshoot trashes the adjacent
      buffer in memory and tNetTask will eventually raise an exception.
      Increasing the size of the RX buffers by 12 bytes seems to prevent
      this from happening.
    * Use FEC_END_MAX_PCK_SZ instead of FEC_END_RX_BUF_SZ/FEC_END_TX_BUF_SZ
      for Bestcomm API. The maximum number of bytes within an ethernet
      frame is 1518 bytes, which includes the CRC.
    * m5200FecPktCopyTransmit(): Problem identified where the driver can stop
      sending frames to the wire because NET_BUF_ALLOC() fails. If the
      higher layers do not free buffers, then it becomes impossible to
      transmit any more data. The problem is overcome by having a local
      TX buffer that is used to send a frame only when NET_BUF_ALLOC()
      returns NULL. Added BUF_TYPE_LOCAL, txBuffAvailable and pTxBuffLocal.
    * Added WORKAROUND_BESTCOMM_FEC for future maintenance.
01d * Exported m5200FecMiiRead and m5200FecMiiWrite to allow more control over the
      PHY from say the BSP.
    * m5200FecSend() - added a taskDelay(1) and try again because resources may
      be available after a short break and so the packet send will succeed.
    * Added ifInErrors to m5200FecMibShow().
01c * Workaround for silicon problem in Bestcomm. See Motorola errata regarding
      using DMA buffers from same memory location.
*/

/*
DESCRIPTION
This module implements a Motorola Fast Ethernet Controller (FEC) network
interface driver. The FEC is fully compliant with the IEEE 802.3
10Base-T and 100Base-T specifications. Hardware support of
the Media Independent Interface (MII) is built-in in the chip.

The FEC establishes a shared memory communication system with the CPU,
which is divided into two parts: the Control/Status Registers (CSR),
and the buffer descriptors (BD).

The CSRs reside in the MPC5200 internal RAM.
They are used for mode control and to extract status information
of a global nature. For instance, the types of events that should
generate an interrupt, or features like the promiscous mode or the
max receive frame length may be set programming some of the CSRs properly.
Pointers to both the Transmit Buffer Descriptors ring (TBD) and the
Receive Buffer Descriptors ring (RBD) are also stored in the CSRs.
The CSRs are located in on-chip RAM and must be accessed using the
big-endian mode.

The BDs are used to pass data buffers and related buffer information
between the hardware and the software. They reside in the host main
memory and basically include local status information and a pointer
to the actual buffer, again in external memory.

This driver must be given several target-specific parameters, and
some external support routines must be provided.  These parameters,
and the mechanisms used to communicate them to the driver, are
detailed below.

BOARD LAYOUT
This device is on-board.  No jumpering diagram is necessary.

EXTERNAL INTERFACE

The driver provides the standard external interface, m5200FecEndLoad(), which
takes a string of colon-separated parameters. The parameters should be
specified in hexadecimal, optionally preceeded by "0x" or a minus sign "-".

The parameter string is parsed using strtok_r() and each parameter is
converted from a string representation to binary by a call to
strtoul(parameter, NULL, 16).

The format of the parameter string is:

"<bufBase>:<bufSize>:<fifoTxBase>:<fifoRxBase>
:<tbdNum>:<rbdNum>:<phyAddr>:<isoPhyAddr>:<phyDefMode>:<userFlags>
:<clockSpeed>"

TARGET-SPECIFIC PARAMETERS

\is
.IP <bufBase>
The Motorola Fast Ethernet Controller is a DMA-type device and typically
shares access to some region of memory with the CPU. This driver is designed
for systems that directly share memory between the CPU and the FEC.

This parameter tells the driver that space for the both the TBDs and the
RBDs needs not be allocated but should be taken from a cache-coherent
private memory space provided by the user at the given address. The user
should be aware that memory used for buffers descriptors must be 8-byte
aligned and non-cacheable. All the buffer descriptors should fit
in the given memory space.

If this parameter is "NONE", space for buffer descriptors is obtained
by calling cacheDmaMalloc() in m5200FecEndLoad().

\i <bufSize>
The memory size parameter specifies the size of the pre-allocated memory
region. If <bufBase> is specified as NONE (-1), the driver ignores this
parameter. Otherwise, the driver checks the size of the provided memory
region is adequate with respect to the given number of Transmit Buffer
Descriptors and Receive Buffer Descriptors.

\i <fifoTxBase>
Indicate the base location of the transmit FIFO, in internal memory.
The user does not need to initialize this parameter, as the related
FEC register defaults to a proper value after reset. The specific
reset value is microcode dependent. However, if the user wishes to
reserve some RAM for other purposes, he may set this parameter to a
different value. This should not be less than the default.

If <fifoTxBase> is specified as NONE (-1), the driver ignores it.

\i <fifoRxBase>
Indicate the base location of the receive FIFO, in internal memory.
The user does not need to initialize this parameter, as the related
FEC register defaults to a proper value after reset. The specific
reset value is microcode dependent. However, if the user wishes to
reserve some RAM for other purposes, he may set this parameter to a
different value. This should not be less than the default.

If <fifoRxBase> is specified as NONE (-1), the driver ignores it.

\i <tbdNum>
This parameter specifies the number of transmit buffer descriptors (TBDs).
Each buffer descriptor resides in 8 bytes of the processor's external
RAM space, and each one points to a 1536-byte buffer again in external
RAM. If this parameter is less than a minimum number specified in the
macro FEC_END_TBD_MIN, or if it is "NULL", a default value of 64 is used.
This default number is kept deliberately hugh, since each packet the driver
sends may consume more than a single TBD. This parameter should always
equal a even number.

\i  <rbdNum>
This parameter specifies the number of receive buffer descriptors (RBDs).
Each buffer descriptor resides in 8 bytes of the processor's external
RAM space, and each one points to a 1536-byte buffer again in external
RAM. If this parameter is less than a minimum number specified in the
macro FEC_END_RBD_MIN, or if it is "NULL", a default value of 48 is used.
This parameter should always equal a even number.

\i  <phyAddr>
This parameter specifies the logical address of a MII-compliant physical
device (PHY) that is to be used as a physical media on the network.
Valid addresses are in the range 0-31. There may be more than one device
under the control of the same management interface. If this parameter is
"NULL", the default physical layer initialization routine will find out the
PHY actual address by scanning the whole range. The one with the lowest
address will be chosen.

\i  <isoPhyAddr>
This parameter specifies the logical address of a MII-compliant physical
device (PHY) that is to be electrically isolated by the management
interface. Valid addresses are in the range 0-31. If this parameter
equals 0xff, the default physical layer initialization routine will
assume there is no need to isolate any device. However, this parameter
will be ignored unless the FEC_END_USR_PHY_ISO bit in the <userFlags>
is set to one.

\i  <phyDefMode>
This parameter specifies the operating mode that will be set up
by the default physical layer initialization routine in case all
the attempts made to establish a valid link failed. If that happens,
the first PHY that matches the specified abilities will be chosen to
work in that mode, and the physical link will not be tested.

\i  <userFlags>
This field enables the user to give some degree of customization to the
driver, especially as regards the physical layer interface.

\i  <clockSpeed>
This field enables the user to define the speed of the clock being used
to drive the interface.  The clock speed is used to derive the MII
management interface clock, which cannot exceed 2.5 MHz.  <clockSpeed>
is optional in BSPs using clocks that are 50 MHz or less, but it is
required in faster designs to ensure proper MII interface operation.

FEC_END_USR_PHY_NO_AN: the default physical layer initialization
routine will exploit the auto-negotiation mechanism as described in
the IEEE Std 802.3, to bring a valid link up. According to it, all
the link partners on the media will take part to the negotiation
process, and the highest priority common denominator technology ability
will be chosen. It the user wishes to prevent auto-negotiation from
occurring, he may set this bit in the user flags.

FEC_END_USR_PHY_TBL: in the auto-negotiation process, PHYs
advertise all their technology abilities at the same time,
and the result is that the maximum common denominator is used. However,
this behaviour may be changed, and the user may affect the order how
each subset of PHY's abilities is negotiated. Hence, when the
FEC_END_USR_PHY_TBL bit is set, the default physical layer
initialization routine will look at the m5200FecPhyAnOrderTbl[] table and
auto-negotiate a subset of abilities at a time, as suggested by the
table itself. It is worth noticing here, however, that if the
FEC_END_USR_PHY_NO_AN bit is on, the above table will be ignored.

FEC_END_USR_PHY_NO_FD: the PHY may be set to operate in full duplex mode,
provided it has this ability, as a result of the negotiation with other
link partners. However, in this operating mode, the FEC will ignore the
collision detect and carrier sense signals. If the user wishes not to
negotiate full duplex mode, he should set the FEC_END_USR_PHY_NO_FD bit
in the user flags.

FEC_END_USR_PHY_NO_HD: the PHY may be set to operate in half duplex mode,
provided it has this ability, as a result of the negotiation with other link
partners. If the user wishes not to negotiate half duplex mode, he should
set the FEC_END_USR_PHY_NO_HD bit in the user flags.

FEC_END_USR_PHY_NO_100: the PHY may be set to operate at 100Mbit/s speed,
provided it has this ability, as a result of the negotiation with
other link partners. If the user wishes not to negotiate 100Mbit/s speed,
he should set the FEC_END_USR_PHY_NO_100 bit in the user flags.

FEC_END_USR_PHY_NO_10: the PHY may be set to operate at 10Mbit/s speed,
provided it has this ability, as a result of the negotiation with
other link partners. If the user wishes not to negotiate 10Mbit/s speed,
he should set the FEC_END_USR_PHY_NO_10 bit in the user flags.

FEC_END_USR_PHY_ISO: some boards may have different PHYs controlled by the
same management interface. In some cases, there may be the need of
electrically isolating some of them from the interface itself, in order
to guarantee a proper behaviour on the medium layer. If the user wishes to
electrically isolate one PHY from the MII interface, he should set the
FEC_END_USR_PHY_ISO bit and provide its logical address in the <isoPhyAddr>
field of the load string. The default behaviour is to not isolate any
PHY on the board.

FEC_END_USR_SER: the user may set the FEC_END_USR_SER bit to enable
the 7-wire interface instead of the MII which is the default.

FEC_END_USR_LOOP: when the FEC_END_USR_LOOP bit is set, the driver will
configure the FEC to work in loopback mode, with the TX signal directly
connected to the RX. This mode should only be used for testing.

FEC_END_USR_HBC: if the FEC_END_USR_HBC bit is set, the driver will
configure the FEC to perform heartbeat check following end of transmisson
and the HB bit in the status field of the TBD will be set if the collision
input does not assert within the heartbeat window (also see _func_m5200FecHbFail,
below). The user does not normally need to set this bit.

\ie

EXTERNAL SUPPORT REQUIREMENTS
This driver requires three external support functions:
\is
\i sysFecEnetEnable()
\cs
    STATUS sysFecEnetEnable (UINT32 fecBaseAddr);
\ce
This routine is expected to handle any target-specific functions needed
to enable the FEC. These functions typically include setting the Port
on the board so that the MII interface may be used, and also
disabling the IRQ7 signal. This routine is expected to return OK on
success, or ERROR. The driver calls this routine, once per device, from the
m5200FecEndLoad() routine.
\i sysFecEnetDisable()
\cs
    STATUS sysFecEnetDisable (UINT32 fecBaseAddr);
\ce
This routine is expected to perform any target specific functions required
to disable the MII interface to the FEC.  This involves restoring the
default values for all the Port D signals. This routine is expected to
return OK on success, or ERROR. The driver calls this routine from the
m5200FecEndStop() routine each time a device is disabled.
\i sysEnetAddrGet()
\cs
STATUS sysEnetAddrGet (UINT32 fecBaseAddr, UCHAR * enetAddr);
\ce
The driver expects this routine to provide the six-byte Ethernet hardware
address that is used by this device.  This routine must copy the six-byte
address to the space provided by <enetAddr>.  This routine is expected to
return OK on success, or ERROR.  The driver calls this routine, once per
device, from the m5200FecEndLoad() routine.
\i `_func_m5200FecPhyInit'
\cs
    FUNCPTR _func_m5200FecPhyInit
\ce
This driver sets the global variable `_func_m5200FecPhyInit' to the
MII-compliant media initialization routine m5200FecPhyInit(). If the user
wishes to exploit a different way to configure the PHY, he may set
this variable to his own media initialization routine, tipically
in sysHwInit().
\i `_func_m5200FecHbFail'
\cs
    FUNCPTR _func_m5200FecPhyInit
\ce
The FEC may be configured to perform heartbeat check following end
of transmission, and to generate an interrupt, when this event occurs.
If this is the case, and if the global variable `_func_m5200FecHbFail'
is not NULL, the routine referenced to by `_func_m5200FecHbFail' is called,
with a pointer to the driver control structure as parameter. Hence,
the user may set this variable to his own heart beat check fail routine,
where he can take any action he sees appropriate.
The default value for the global variable `_func_m5200FecHbFail' is NULL.
\ie

SYSTEM RESOURCE USAGE
If the driver allocates the memory to share with the Ethernet device,
it does so by calling the cacheDmaMalloc() routine.  For the default case
of 64 transmit buffers and 48 receive buffers, the total size requested
is 912 bytes, and this includes the 16-byte alignment requirement of the
device.  If a non-cacheable memory region is provided by the user, the
size of this region should be this amount, unless the user has specified
a different number of transmit or receive BDs.

This driver can operate only if this memory region is non-cacheable
or if the hardware implements bus snooping.  The driver cannot maintain
cache coherency for the device because the BDs are asynchronously
modified by both the driver and the device, and these fields might share
the same cache line.

Data buffers are instead allocated in the external memory through the
regular memory allocation routine (memalign), and the related cache lines
are then flushed or invalidated as appropriate. The user should not allocate
memory for them.

TUNING HINTS

The only adjustable parameters are the number of TBDs and RBDs that will be
created at run-time.  These parameters are given to the driver when
m5200FecEndLoad() is called.  There is one RBD associated with each received
frame whereas a single transmit packet normally uses more than one TBD.  For
memory-limited applications, decreasing the number of RBDs may be
desirable.  Decreasing the number of TBDs below a certain point will
provide substantial performance degradation, and is not reccomended. An
adequate number of loaning buffers are also pre-allocated to provide more
buffering before packets are dropped, but this is not configurable.

The relative priority of the netTask and of the other tasks in the system
may heavily affect performance of this driver. Usually the best performance
is achieved when the netTask priority equals that of the other
applications using the driver.

INTERNAL
This driver contains conditional compilation switch FEC_END_DBG.
If defined, adds debug output routines.  Output is further
selectable at run-time via the m5200FecEndDbg global variable.
*/

#include "vxWorks.h"
#include "wdLib.h"
#include "iv.h"
#include "vme.h"
#include "net/mbuf.h"
#include "net/unixLib.h"
#include "net/protosw.h"
#include "sys/socket.h"
#include "sys/ioctl.h"
#include "errno.h"
#include "memLib.h"
#include "intLib.h"
#include "net/route.h"
#include "iosLib.h"
#include "errnoLib.h"
#include "vxLib.h"

#include "cacheLib.h"
#include "logLib.h"
#include "netLib.h"
#include "stdio.h"
#include "stdlib.h"
#include "sysLib.h"
#include "taskLib.h"

#include "net/systm.h"
#include "sys/times.h"

#undef ETHER_MAP_IP_MULTICAST
#include "etherMultiLib.h"
#include "end.h"
#include "semLib.h"

#define END_MACROS
#include "endLib.h"
#include "lstLib.h"

#include "arch/ppc/ppc5200.h"
#include "drv/intrCtl/m5200Intr.h"
#include "drv/end/m5200FecEnd.h"
#include "drv/dma/m5200Sdma.h"
#include "bestcomm_api.h"
#include "task_api/bestcomm_cntrl.h"
#include "task_api/bestcomm_api_mem.h"

/*
 * Errata issues on the bestcomm/fec are resolved in
 * software if WORKAROUND_BESTCOMM_FEC is defined.
 */

#if 1
#define WORKAROUND_BESTCOMM_FEC
#endif

/* defines */

extern void *vxMBarGet();
#define MBAR_VALUE vxMBarGet()

/* initial divisor and final XOR value for reflected CRC */

#define INIT_REFLECTED  0xFFFFFFFF
#define XOROT           INIT_REFLECTED

/* Driver debug control */

#if 0
#define FEC_END_DBG
#endif

#if 0
#define FEC_END_TST
#endif

/* Driver debug control */

#ifdef FEC_END_DBG
#define FEC_END_DBG_OFF         0x0000
#define FEC_END_DBG_RX          0x0001
#define FEC_END_DBG_TX          0x0002
#define FEC_END_DBG_POLL        0x0004
#define FEC_END_DBG_MII         0x0008
#define FEC_END_DBG_LOAD        0x0010
#define FEC_END_DBG_IOCTL       0x0020
#define FEC_END_DBG_INT         0x0040
#define FEC_END_DBG_START           0x0080
#define FEC_END_DBG_ERR         0x0100
#define FEC_END_DBG_RESTART     0x0200
#define FEC_END_DBG_ANY         0xffff

#if (BESTCOMM_API != 11)
#error please adapt this source
#endif

#undef NO_RX_INT
#undef NO_TX_INT

UINT32  m5200FecEndDbg = FEC_END_DBG_ERR;
UINT32  m5200FecBabRxErr = 0x0;
UINT32  m5200FecBabTxErr = 0x0;
UINT32  m5200FecLColErr = 0x0;
UINT32  m5200FecColRlErr = 0x0;
UINT32  m5200FecTxUnErr = 0x0;
UINT32  m5200FecHbFailErr = 0x0;
UINT32  m5200FecTxLcErr = 0x0;
UINT32  m5200FecTxUrErr = 0x0;
UINT32  m5200FecTxCslErr = 0x0;
UINT32  m5200FecTxRlErr = 0x0;
UINT32  m5200FecRxFEmptyErr = 0x0;
UINT32  m5200FecRxAlarmErr = 0x0;
UINT32  m5200FecRxFullErr = 0x0;
UINT32  m5200FecRxUfErr = 0x0;
UINT32  m5200FecRxOvErr = 0x0;
UINT32  m5200FecRxLsErr = 0x0;
UINT32  m5200FecRxMemErr = 0x0;

UINT32 m5200FecStatNewBuf = 0;
UINT32 m5200FecStatTxFreeError = 0;

UINT32 m5200FecPktNoCopy = 0;
UINT32 m5200FecPktCopy = 0;

DRV_CTRL *  pDrvCtrlDbg = NULL;


#define FEC_END_TX_PKT_NO_COPY                  \
    do {m5200FecPktNoCopy++;}while (0)

#define FEC_END_TX_PKT_COPY                 \
    do {m5200FecPktCopy++;}while (0)

#define FEC_END_TX_FREE_ERROR                   \
    do {m5200FecStatTxFreeError++;}while (0)

#define FEC_END_NEW_BUFF_ADD                    \
    do {m5200FecStatNewBuf++;}while (0)

#define FEC_END_BAB_RX_ADD                      \
    do {m5200FecBabRxErr++;}while (0)

#define FEC_END_BAB_TX_ADD                      \
    do {m5200FecBabTxErr++;}while (0)

#define FEC_END_LCOL_ADD                        \
    do {m5200FecLColErr++;}while (0)

#define FEC_END_COL_RL_ADD                      \
    do {m5200FecColRlErr++;}while (0)

#define FEC_END_TX_UN_ADD                       \
    do {m5200FecTxUnErr++;}while (0)

#define FEC_END_HB_FAIL_ADD                     \
    do {m5200FecHbFailErr++;}while (0)

#define FEC_END_TX_LC_ADD                       \
    do {m5200FecTxLcErr++;}while (0)

#define FEC_END_TX_UR_ADD                       \
    do {m5200FecTxUrErr++;}while (0)

#define FEC_END_TX_CSL_ADD                      \
    do {m5200FecTxCslErr++}while (0);

#define FEC_END_TX_RL_ADD                       \
    do {m5200FecTxRlErr++;}while (0)

#define FEC_END_RX_FEMPTY_ADD                       \
    do {m5200FecRxFEmptyErr++;}while (0)

#define FEC_END_RX_ALARM_ADD                        \
    do {m5200FecRxAlarmErr++;}while (0)

#define FEC_END_RX_FULL_ADD                     \
    do {m5200FecRxFullErr++;}while (0)

#define FEC_END_RX_UF_ADD                       \
    do {m5200FecRxUfErr++;}while (0)

#define FEC_END_RX_OV_ADD                       \
    do {m5200FecRxOvErr++;}while (0)

#define FEC_END_RX_MEM_ADD                      \
    do {m5200FecRxMemErr++;}while (0)

#define FEC_END_RX_LS_ADD                       \
    do {m5200FecRxLsErr++;}while (0)

#define FEC_END_LOG(FLG, X0, X1, X2, X3, X4, X5, X6)                    \
    {                                                                   \
    if (m5200FecEndDbg & FLG)                                             \
        logMsg (X0, X1, X2, X3, X4, X5, X6);                            \
    }

#else /* FEC_END_DBG */

#define FEC_END_TX_FREE_ERROR
#define FEC_END_NEW_BUFF_ADD
#define FEC_END_TX_PKT_NO_COPY
#define FEC_END_TX_PKT_COPY
#define FEC_END_BAB_RX_ADD
#define FEC_END_BAB_TX_ADD
#define FEC_END_LCOL_ADD
#define FEC_END_COL_RL_ADD
#define FEC_END_TX_UN_ADD
#define FEC_END_HB_FAIL_ADD
#define FEC_END_TX_LC_ADD
#define FEC_END_TX_UR_ADD
#define FEC_END_TX_CSL_ADD
#define FEC_END_TX_RL_ADD
#define FEC_END_RX_MEM_ADD
#define FEC_END_RX_LS_ADD
#define FEC_END_LOG(FLG, X0, X1, X2, X3, X4, X5, X6) do {} while (0)

#endif /* FEC_END_DBG */

/* general macros for reading/writing from/to specified locations */

/* Cache and virtual/physical memory related macros */

#define FEC_END_PHYS_TO_VIRT(physAddr)                      \
    CACHE_DRV_PHYS_TO_VIRT (&pDrvCtrl->bdCacheFuncs, (char *)(physAddr))

#define FEC_END_VIRT_TO_PHYS(virtAddr)                      \
    CACHE_DRV_VIRT_TO_PHYS (&pDrvCtrl->bdCacheFuncs, (char *)(virtAddr))

#define FEC_END_BD_CACHE_INVAL(address, len)                        \
        CACHE_DRV_INVALIDATE (&pDrvCtrl->bdCacheFuncs, (address), (len))

#define FEC_END_CACHE_INVAL(address, len)                                   \
    CACHE_DRV_INVALIDATE (&pDrvCtrl->bufCacheFuncs, (address), (len))

#define FEC_END_CACHE_FLUSH(address, len)                                   \
    CACHE_DRV_FLUSH (&pDrvCtrl->bufCacheFuncs, (address), (len))

/* driver flags */

#define FEC_END_OWN_MEM     0x01    /* internally provided memory */
#define FEC_END_INV_TBD_NUM 0x02    /* invalid tbdNum provided */
#define FEC_END_INV_RBD_NUM 0x04    /* invalid rbdNum provided */
#define FEC_END_POLLING     0x08    /* polling mode */
#define FEC_END_PROM        0x20    /* promiscuous mode */
#define FEC_END_MCAST       0x40    /* multicast addressing mode */
#define FEC_END_FD          0x80    /* full duplex mode */

/* shortcuts */

#define FEC_END_FLAG_CLEAR(clearBits)                   \
    (pDrvCtrl->flags &= ~(clearBits))

#define FEC_END_FLAG_SET(setBits)                   \
    (pDrvCtrl->flags |= (setBits))

#define FEC_END_FLAG_GET()                      \
    (pDrvCtrl->flags)

#define FEC_END_FLAG_ISSET(setBits)                 \
    (pDrvCtrl->flags & (setBits))

#define FEC_END_PHY_FLAGS_SET(setBits)                  \
    (pDrvCtrl->phyInfo->phyFlags |= (setBits))

#define FEC_END_PHY_FLAGS_ISSET(setBits)                \
    (pDrvCtrl->phyInfo->phyFlags & (setBits))

#define FEC_END_PHY_FLAGS_GET(setBits)                  \
    (pDrvCtrl->phyInfo->phyFlags)

#define FEC_END_PHY_FLAGS_CLEAR(clearBits)              \
    (pDrvCtrl->phyInfo->phyFlags &= ~(clearBits))

#define FEC_END_USR_FLAG_ISSET(setBits)                 \
    (pDrvCtrl->userFlags & (setBits))

#define END_FLAGS_ISSET(setBits)                    \
    ((&pDrvCtrl->endObj)->flags & (setBits))

#define FEC_END_ADDR_GET(pEnd)                                          \
    ((pEnd)->mib2Tbl.ifPhysAddress.phyAddress)

#define FEC_END_ADDR_LEN_GET(pEnd)                                      \
    ((pEnd)->mib2Tbl.ifPhysAddress.addrLength)

/* some BDs definitions */

/*
 * the total is 0x600 and it accounts for the required alignment
 * of receive data buffers, and the cluster overhead.
 */

#define FEC_END_MAX_CL_LEN      ((FEC_END_MAX_PCK_SZ + (_CACHE_ALIGN_SIZE) \
                 + (FEC_END_BD_ALIGN - 1)       \
                 + (CL_OVERHEAD - 1))           \
                 & (~ (CL_OVERHEAD - 1)))

#define FEC_END_MAX_RX_BUF  ((FEC_END_MAX_PCK_SZ + FEC_END_BD_ALIGN - 1) \
                & (~(FEC_END_BD_ALIGN - 1)))

#define FEC_END_BUF_V_LEN   (FEC_END_MAX_RX_BUF + 16)

#define FEC_END_RX_BUF_SZ       (FEC_END_MAX_CL_LEN)
#define FEC_END_TX_BUF_SZ       (FEC_END_MAX_CL_LEN)

#define FEC_END_MEM_SZ(pDrvCtrl)                    \
    ((pDrvCtrl)->bufSize)

#define FEC_END_LOAN_SZ                         \
    (FEC_END_RX_BUF_SZ * FEC_END_BD_LOAN_NUM)

#define FEC_END_RX_MEM(pDrvCtrl)                    \
    (FEC_END_RX_BUF_SZ * ((pDrvCtrl)->rbdNum + FEC_END_BD_LOAN_NUM))

#define FEC_END_TX_MEM(pDrvCtrl)                    \
    (FEC_END_TX_BUF_SZ * (FEC_END_TBD_POLL_NUM + FEC_END_TX_CL_NUM))

#define FEC_END_TBD_MEM(pDrvCtrl)                   \
    (FEC_END_TBD_SZ * (pDrvCtrl)->tbdNum)

#define FEC_END_RBD_MEM(pDrvCtrl)                   \
    (FEC_END_RBD_SZ * (pDrvCtrl)->rbdNum)

/* we need 4 words in front for the descriptor used in the SDMA tasks */
#define FEC_END_BD_MEM(pDrvCtrl)                    \
    (FEC_END_TBD_MEM (pDrvCtrl) + FEC_END_RBD_MEM (pDrvCtrl) + 16)

/* Control/Status Registers (CSR) definitions */

#define FEC_END_CSR_WR(csrOff, csrVal)                  \
    {                                   \
    UINT32 temp = 0;                            \
                                    \
    temp = (UINT32) FEC_END_VIRT_TO_PHYS ((pDrvCtrl->fecBaseAddr)   \
                      + (csrOff));          \
                                    \
    FEC_END_LONG_WR ((UINT32 *) (temp), (csrVal));          \
    }

#define FEC_END_CSR_RD(csrOff, csrVal)                  \
    {                                   \
    UINT32 temp = 0;                            \
                                    \
    temp = (UINT32) FEC_END_VIRT_TO_PHYS ((pDrvCtrl->fecBaseAddr)   \
                      + (csrOff));          \
                                    \
    FEC_END_LONG_RD ((UINT32 *) (temp), (csrVal));          \
    }

/* macros to read/write tx/rx buffer descriptors */

#define FEC_END_BD_WORD_WR(bdAddr, bdOff, bdVal)            \
    {                                   \
    UINT32 temp = 0;                            \
                                    \
    temp = (UINT32) FEC_END_VIRT_TO_PHYS ((bdAddr) + (bdOff));      \
                                    \
    FEC_END_WORD_WR ((UINT16 *) (temp), (bdVal));           \
    }

/* here we're writing the data pointer, so it's a physical address */

#define FEC_END_BD_LONG_WR(bdAddr, bdOff, bdVal)            \
    {                                   \
    UINT32 temp1 = 0;                           \
    UINT32 temp2 = 0;                           \
                                    \
    temp1 = (UINT32) FEC_END_VIRT_TO_PHYS ((bdAddr) + (bdOff));     \
    temp2 = (UINT32) FEC_END_VIRT_TO_PHYS (bdVal);          \
                                    \
    FEC_END_LONG_WR ((UINT32 *) (temp1), (temp2));          \
    }

#define FEC_END_BD_WORD_RD(bdAddr, bdOff, bdVal)            \
    {                                   \
    UINT32 temp = 0;                            \
                                    \
    temp = (UINT32) FEC_END_VIRT_TO_PHYS ((bdAddr) + (bdOff));      \
                                    \
    FEC_END_WORD_RD ((UINT16 *) (temp), (bdVal));                   \
    }

#define FEC_END_BD_LONG_RD(bdAddr, bdOff, bdVal)            \
    {                                   \
    UINT32 temp1 = 0;                           \
    UINT32 temp2 = 0;                           \
                                    \
    temp1 = (UINT32) FEC_END_VIRT_TO_PHYS ((bdAddr) + (bdOff));     \
                                    \
    FEC_END_LONG_RD ((UINT32 *) (temp1), (temp2));          \
                                    \
    bdVal = (char *) FEC_END_PHYS_TO_VIRT (temp2);          \
    }

#define FEC_END_NEXT_TBD(pDrvCtrl, pTbd)                \
    (pTbd) = (FEC_END_TBD_ID) ((pDrvCtrl)->tbdBase +   \
                   ((pDrvCtrl)->tbdIndex * FEC_END_TBD_SZ))

#define FEC_END_USED_TBD(pDrvCtrl, pTbd)                   \
    (pTbd) = (FEC_END_TBD_ID) ((pDrvCtrl)->tbdBase +       \
                   ((pDrvCtrl)->usedTbdIndex * FEC_END_TBD_SZ))

#define FEC_END_NEXT_RBD(pDrvCtrl, pRbd)                   \
    (pRbd) = (FEC_END_RBD_ID)TaskGetBD( (pDrvCtrl)->recvTaskNo, (pDrvCtrl)->rbdIndex )

#define FEC_END_ETH_SET                                                 \
    FEC_END_CSR_WR (FEC_END_CTRL_OFF, 0);

#define FEC_END_ETH_ENABLE                      \
    FEC_END_CSR_WR (FEC_END_CTRL_OFF, FEC_END_ETH_EN);

#define FEC_END_ETH_DISABLE                     \
    FEC_END_CSR_WR (FEC_END_CTRL_OFF, FEC_END_ETH_DIS);

#define FEC_END_INT_DISABLE                     \
    FEC_END_CSR_WR (FEC_END_MASK_OFF, 0);

#define FEC_END_INT_ENABLE                      \
    FEC_END_CSR_WR (FEC_END_MASK_OFF, (pDrvCtrl->intMask));

#define FEC_END_FEC_VECTOR(pDrvCtrl)                    \
    ((pDrvCtrl)->ivecFEC)
#define FEC_END_RDMA_VECTOR(pDrvCtrl)                   \
    ((pDrvCtrl)->ivecRDMA)
#define FEC_END_WDMA_VECTOR(pDrvCtrl)                   \
    ((pDrvCtrl)->ivecWDMA)

/* more shortcuts */

#define FEC_END_GRA_SEM_CREATE                      \
    if ((pDrvCtrl->graSem = semBCreate (SEM_Q_FIFO, SEM_EMPTY))     \
    == NULL)                            \
    goto errorExit

#define FEC_END_GRA_SEM_DELETE                      \
    if ((pDrvCtrl->graSem) != NULL)                     \
    semDelete (pDrvCtrl->graSem)

#define FEC_END_GRA_SEM_GIVE                        \
    (semGive (pDrvCtrl->graSem))

#define FEC_END_GRA_SEM_TAKE                        \
    (semTake (pDrvCtrl->graSem, 300))

#define FEC_END_MII_SEM_CREATE                      \
    if ((pDrvCtrl->miiSem = semMCreate (SEM_Q_PRIORITY | SEM_INVERSION_SAFE))     \
    == NULL)                            \
    goto errorExit

#define FEC_END_MII_SEM_DELETE                      \
    if ((pDrvCtrl->miiSem) != NULL)                     \
    semDelete (pDrvCtrl->miiSem)

#define FEC_END_MII_SEM_GIVE                        \
    (semGive (pDrvCtrl->miiSem))

#define FEC_END_MII_SEM_TAKE                        \
    (semTake (pDrvCtrl->miiSem, WAIT_FOREVER))

#define NET_BUF_ALLOC()                                                 \
    netClusterGet (pDrvCtrl->endObj.pNetPool, pDrvCtrl->pClPoolId)  \

#define NET_TO_FEC_END_BUF(netBuf)                                  \
    (((UINT32) (netBuf) + FEC_END_BD_ALIGN - 1)             \
      & ~(FEC_END_BD_ALIGN - 1))

#define NET_BUF_FREE(pBuffer)                                           \
    netClFree (pDrvCtrl->endObj.pNetPool, (pBuffer))

#define NET_MBLK_ALLOC()                                                \
    netMblkGet (pDrvCtrl->endObj.pNetPool, M_DONTWAIT, MT_DATA)

#define NET_MBLK_FREE(pMblock)                                          \
    netMblkFree (pDrvCtrl->endObj.pNetPool, (M_BLK_ID) (pMblock))

#define NET_CL_BLK_ALLOC()                                              \
    netClBlkGet (pDrvCtrl->endObj.pNetPool, M_DONTWAIT)

#define NET_CL_BLK_FREE(pClBlock)                                       \
    netClBlkFree (pDrvCtrl->endObj.pNetPool, (CL_BLK_ID) (pClBlock))

#define NET_MBLK_BUF_FREE(pMblock)                                      \
    netMblkClFree ((M_BLK_ID) (pMblock))

#define NET_MBLK_CL_JOIN(pMblock, pClBlock, ret)                        \
    ret = netMblkClJoin ((pMblock), (pClBlock))

#define NET_CL_BLK_JOIN(pClBlock, pBuffer, length, ret)                 \
    ret = netClBlkJoin ((pClBlock), (pBuffer), (length), NULL, 0, 0, 0)

/* globals */

#ifdef WORKAROUND_BESTCOMM_FEC
BOOL   m5200FecForceCopy = TRUE;
#else
BOOL   m5200FecForceCopy = FALSE;
#endif

#ifdef FEC_END_TST
DRV_CTRL * pTstDrvCtrl = NULL;
volatile int tstFifoErrorFlag;
#endif

int    m5200BusMonitorTID;

/* external */

/* locals */

/*
 * the table below is used to translate user settings
 * into MII-standard values for technology abilities.
 */

LOCAL UINT16 miiAnLookupTbl [] = {
                  MII_TECH_10BASE_T,
                  MII_TECH_10BASE_FD,
                  MII_TECH_100BASE_TX,
                  MII_TECH_100BASE_TX_FD,
                  MII_TECH_100BASE_T4
                     };

LOCAL UINT16 miiCrLookupTbl [] = {
                  MII_CR_NORM_EN,
                  MII_CR_FDX,
                  MII_CR_100,
                  (MII_CR_100 | MII_CR_FDX),
                  (MII_CR_100 | MII_CR_FDX)
                     };

/* cache control */

LOCAL   CACHE_FUNCS     m5200FecBufCacheFuncs;
LOCAL   FUNCPTR m5200FecBufInvRtn = cacheArchInvalidate;
LOCAL   FUNCPTR m5200FecBufFlushRtn = cacheArchFlush;
LOCAL   FUNCPTR m5200FecIntDisc = NULL;   /* assign a proper disc routine */


/* intermediate remainders for table-driven CRC calculations */

LOCAL UINT32  crctable[256] =
{
 0x00000000L, 0x77073096L, 0xEE0E612CL, 0x990951BAL,
 0x076DC419L, 0x706AF48FL, 0xE963A535L, 0x9E6495A3L,
 0x0EDB8832L, 0x79DCB8A4L, 0xE0D5E91EL, 0x97D2D988L,
 0x09B64C2BL, 0x7EB17CBDL, 0xE7B82D07L, 0x90BF1D91L,
 0x1DB71064L, 0x6AB020F2L, 0xF3B97148L, 0x84BE41DEL,
 0x1ADAD47DL, 0x6DDDE4EBL, 0xF4D4B551L, 0x83D385C7L,
 0x136C9856L, 0x646BA8C0L, 0xFD62F97AL, 0x8A65C9ECL,
 0x14015C4FL, 0x63066CD9L, 0xFA0F3D63L, 0x8D080DF5L,
 0x3B6E20C8L, 0x4C69105EL, 0xD56041E4L, 0xA2677172L,
 0x3C03E4D1L, 0x4B04D447L, 0xD20D85FDL, 0xA50AB56BL,
 0x35B5A8FAL, 0x42B2986CL, 0xDBBBC9D6L, 0xACBCF940L,
 0x32D86CE3L, 0x45DF5C75L, 0xDCD60DCFL, 0xABD13D59L,
 0x26D930ACL, 0x51DE003AL, 0xC8D75180L, 0xBFD06116L,
 0x21B4F4B5L, 0x56B3C423L, 0xCFBA9599L, 0xB8BDA50FL,
 0x2802B89EL, 0x5F058808L, 0xC60CD9B2L, 0xB10BE924L,
 0x2F6F7C87L, 0x58684C11L, 0xC1611DABL, 0xB6662D3DL,
 0x76DC4190L, 0x01DB7106L, 0x98D220BCL, 0xEFD5102AL,
 0x71B18589L, 0x06B6B51FL, 0x9FBFE4A5L, 0xE8B8D433L,
 0x7807C9A2L, 0x0F00F934L, 0x9609A88EL, 0xE10E9818L,
 0x7F6A0DBBL, 0x086D3D2DL, 0x91646C97L, 0xE6635C01L,
 0x6B6B51F4L, 0x1C6C6162L, 0x856530D8L, 0xF262004EL,
 0x6C0695EDL, 0x1B01A57BL, 0x8208F4C1L, 0xF50FC457L,
 0x65B0D9C6L, 0x12B7E950L, 0x8BBEB8EAL, 0xFCB9887CL,
 0x62DD1DDFL, 0x15DA2D49L, 0x8CD37CF3L, 0xFBD44C65L,
 0x4DB26158L, 0x3AB551CEL, 0xA3BC0074L, 0xD4BB30E2L,
 0x4ADFA541L, 0x3DD895D7L, 0xA4D1C46DL, 0xD3D6F4FBL,
 0x4369E96AL, 0x346ED9FCL, 0xAD678846L, 0xDA60B8D0L,
 0x44042D73L, 0x33031DE5L, 0xAA0A4C5FL, 0xDD0D7CC9L,
 0x5005713CL, 0x270241AAL, 0xBE0B1010L, 0xC90C2086L,
 0x5768B525L, 0x206F85B3L, 0xB966D409L, 0xCE61E49FL,
 0x5EDEF90EL, 0x29D9C998L, 0xB0D09822L, 0xC7D7A8B4L,
 0x59B33D17L, 0x2EB40D81L, 0xB7BD5C3BL, 0xC0BA6CADL,
 0xEDB88320L, 0x9ABFB3B6L, 0x03B6E20CL, 0x74B1D29AL,
 0xEAD54739L, 0x9DD277AFL, 0x04DB2615L, 0x73DC1683L,
 0xE3630B12L, 0x94643B84L, 0x0D6D6A3EL, 0x7A6A5AA8L,
 0xE40ECF0BL, 0x9309FF9DL, 0x0A00AE27L, 0x7D079EB1L,
 0xF00F9344L, 0x8708A3D2L, 0x1E01F268L, 0x6906C2FEL,
 0xF762575DL, 0x806567CBL, 0x196C3671L, 0x6E6B06E7L,
 0xFED41B76L, 0x89D32BE0L, 0x10DA7A5AL, 0x67DD4ACCL,
 0xF9B9DF6FL, 0x8EBEEFF9L, 0x17B7BE43L, 0x60B08ED5L,
 0xD6D6A3E8L, 0xA1D1937EL, 0x38D8C2C4L, 0x4FDFF252L,
 0xD1BB67F1L, 0xA6BC5767L, 0x3FB506DDL, 0x48B2364BL,
 0xD80D2BDAL, 0xAF0A1B4CL, 0x36034AF6L, 0x41047A60L,
 0xDF60EFC3L, 0xA867DF55L, 0x316E8EEFL, 0x4669BE79L,
 0xCB61B38CL, 0xBC66831AL, 0x256FD2A0L, 0x5268E236L,
 0xCC0C7795L, 0xBB0B4703L, 0x220216B9L, 0x5505262FL,
 0xC5BA3BBEL, 0xB2BD0B28L, 0x2BB45A92L, 0x5CB36A04L,
 0xC2D7FFA7L, 0xB5D0CF31L, 0x2CD99E8BL, 0x5BDEAE1DL,
 0x9B64C2B0L, 0xEC63F226L, 0x756AA39CL, 0x026D930AL,
 0x9C0906A9L, 0xEB0E363FL, 0x72076785L, 0x05005713L,
 0x95BF4A82L, 0xE2B87A14L, 0x7BB12BAEL, 0x0CB61B38L,
 0x92D28E9BL, 0xE5D5BE0DL, 0x7CDCEFB7L, 0x0BDBDF21L,
 0x86D3D2D4L, 0xF1D4E242L, 0x68DDB3F8L, 0x1FDA836EL,
 0x81BE16CDL, 0xF6B9265BL, 0x6FB077E1L, 0x18B74777L,
 0x88085AE6L, 0xFF0F6A70L, 0x66063BCAL, 0x11010B5CL,
 0x8F659EFFL, 0xF862AE69L, 0x616BFFD3L, 0x166CCF45L,
 0xA00AE278L, 0xD70DD2EEL, 0x4E048354L, 0x3903B3C2L,
 0xA7672661L, 0xD06016F7L, 0x4969474DL, 0x3E6E77DBL,
 0xAED16A4AL, 0xD9D65ADCL, 0x40DF0B66L, 0x37D83BF0L,
 0xA9BCAE53L, 0xDEBB9EC5L, 0x47B2CF7FL, 0x30B5FFE9L,
 0xBDBDF21CL, 0xCABAC28AL, 0x53B39330L, 0x24B4A3A6L,
 0xBAD03605L, 0xCDD70693L, 0x54DE5729L, 0x23D967BFL,
 0xB3667A2EL, 0xC4614AB8L, 0x5D681B02L, 0x2A6F2B94L,
 0xB40BBE37L, 0xC30C8EA1L, 0x5A05DF1BL, 0x2D02EF8DL
};

/* Function declarations not in any header files */

/* forward function declarations */

LOCAL STATUS    m5200FecInitParse (DRV_CTRL *pDrvCtrl, char *initString);
LOCAL STATUS    m5200FecInitMem (DRV_CTRL *pDrvCtrl);
LOCAL STATUS    m5200FecSend (DRV_CTRL *pDrvCtrl, M_BLK *pMblk);

LOCAL STATUS    m5200FecPhyInit (DRV_CTRL *pDrvCtrl);
LOCAL STATUS    m5200FecPhyPreInit (DRV_CTRL *pDrvCtrl);
LOCAL STATUS    m5200FecReset (DRV_CTRL *pDrvCtrl);
LOCAL STATUS    m5200FecBdFree (DRV_CTRL *pDrvCtrl);
LOCAL STATUS    m5200FecRbdInit (DRV_CTRL *pDrvCtrl);
LOCAL STATUS    m5200FecTbdInit (DRV_CTRL *pDrvCtrl);
LOCAL STATUS    m5200FecRxCtrlRegValGet (DRV_CTRL *pDrvCtrl, UINT32 * rxCtrlVal);
LOCAL STATUS    m5200FecTxCtrlRegValGet (DRV_CTRL *pDrvCtrl, UINT32 * txCtrlVal);
LOCAL STATUS    m5200FecHashRegValGet (DRV_CTRL *pDrvCtrl, UINT32  crcVal,
                 UINT32 * csrVal, UINT16 * offset);
LOCAL STATUS    m5200FecAddrRegValGet (DRV_CTRL *pDrvCtrl, UINT32  * csr0Val,
                 UINT32 * csr1Val);
LOCAL UINT32    m5200FecCrcGet (char * pAddr);
LOCAL STATUS    m5200FecHashTblPopulate (DRV_CTRL * pDrvCtrl);
LOCAL STATUS    m5200FecHashTblAdd (DRV_CTRL * pDrvCtrl, char * pAddr);

LOCAL STATUS    m5200FecPrePhyConfig (DRV_CTRL *pDrvCtrl);
LOCAL STATUS    m5200FecPostPhyConfig (DRV_CTRL *pDrvCtrl);
LOCAL STATUS    m5200FecMblkWalk (M_BLK * pMblk, UINT8 * pFragNum,
                UINT16 *    pPktType);
#ifndef SEND_IN_COPY_MODE
LOCAL STATUS    m5200FecPktTransmit (DRV_CTRL *pDrvCtrl, M_BLK * pMblk,
                   UINT8  fragNum, UINT16     pktType);
#endif
LOCAL STATUS    m5200FecPktCopyTransmit (DRV_CTRL *pDrvCtrl, M_BLK * pMblk,
                   UINT8  fragNum, UINT16     pktType);
LOCAL void  m5200FecTbdClean (DRV_CTRL *pDrvCtrl);
LOCAL UINT32    m5200FecTbdCheck (DRV_CTRL *pDrvCtrl, FEC_END_TBD_ID pUsedTbd);
LOCAL FEC_END_TBD_LIST_ID m5200FecTbdListSet (DRV_CTRL *pDrvCtrl, UINT8 tbdNum);
LOCAL FEC_END_TBD_ID    m5200FecTbdGet (DRV_CTRL *pDrvCtrl);
LOCAL void  m5200FecInt (DRV_CTRL *pDrvCtrl);
LOCAL void  m5200FecRdmaInt (DRV_CTRL *pDrvCtrl);
LOCAL void  m5200FecWdmaInt (DRV_CTRL *pDrvCtrl);

LOCAL void      m5200FecRxHandle (DRV_CTRL *pDrvCtrl);
LOCAL void      m5200FecTxHandle (DRV_CTRL *pDrvCtrl);

LOCAL STATUS    m5200FecMiiBasicCheck(DRV_CTRL *pDrvCtrl, UINT8 phyAddr);
LOCAL void      m5200FecHandleRecvInt (DRV_CTRL *pDrvCtrl);
LOCAL void      m5200FecBusMonTask( DRV_CTRL *pDrvCtrl );
LOCAL void      m5200FecBusMonitor (DRV_CTRL *pDrvCtrl);
LOCAL STATUS    m5200FecRelink (DRV_CTRL *pDrvCtrl);
LOCAL STATUS    m5200FecDelink (DRV_CTRL *pDrvCtrl);
#if 0
LOCAL void  m5200FecRxRestart (DRV_CTRL *pDrvCtrl);
#endif
LOCAL void  m5200FecReceive (DRV_CTRL *pDrvCtrl, FEC_END_RBD_ID pRfd);
STATUS  m5200FecMiiRead (DRV_CTRL *pDrvCtrl, UINT8 phyAddr,
                   UINT8 regAddr, UINT16 *retVal);
STATUS  m5200FecMiiWrite (DRV_CTRL *pDrvCtrl, UINT8 phyAddr,
                UINT8 regAddr, UINT16 writeData);

LOCAL STATUS    m5200FecMiiIsolate (DRV_CTRL *pDrvCtrl, UINT8 isoPhyAddr);

LOCAL STATUS    m5200FecMiiAnRun (DRV_CTRL *pDrvCtrl, UINT8 phyAddr);
LOCAL STATUS    m5200FecMiiAnStart (DRV_CTRL *pDrvCtrl, UINT8 phyAddr);
LOCAL STATUS    m5200FecMiiModeForce (DRV_CTRL *pDrvCtrl, UINT8 phyAddr);
LOCAL STATUS    m5200FecMiiDefForce (DRV_CTRL *pDrvCtrl, UINT8 phyAddr);
LOCAL STATUS    m5200FecMiiAnCheck (DRV_CTRL *pDrvCtrl, UINT8 phyAddr);
LOCAL STATUS    m5200FecMiiProbe (DRV_CTRL *pDrvCtrl, UINT8 phyAddr);
LOCAL void  m5200FecRbdClean (DRV_CTRL *pDrvCtrl,
                volatile FEC_END_RBD_ID pRBDNew);
LOCAL STATUS    m5200FecSdmaTaskInit(DRV_CTRL *pDrvCtrl);
LOCAL void  m5200FecTbdReclaim (DRV_CTRL * pDrvCtrl);
#if 0
LOCAL void  m5200FecTxRestart (DRV_CTRL * pDrvCtrl);
#endif

/* END Specific interfaces. */

END_OBJ *   m5200FecEndLoad (char *initString);
LOCAL STATUS    m5200FecStart (DRV_CTRL *pDrvCtrl);
LOCAL STATUS    m5200FecUnload (DRV_CTRL *pDrvCtrl);
LOCAL STATUS    m5200FecStop (DRV_CTRL *pDrvCtrl);
LOCAL int       m5200FecIoctl (DRV_CTRL *pDrvCtrl, int cmd, caddr_t data);
LOCAL STATUS    m5200FecSend (DRV_CTRL *pDrvCtrl, M_BLK_ID pMblk);
LOCAL STATUS    m5200FecMCastAddrAdd (DRV_CTRL *pDrvCtrl, char* pAddress);
LOCAL STATUS    m5200FecMCastAddrDel (DRV_CTRL *pDrvCtrl, char* pAddress);
LOCAL STATUS    m5200FecMCastAddrGet (DRV_CTRL *pDrvCtrl,
                                        MULTI_TABLE *pTable);
LOCAL STATUS    m5200FecPollSend (DRV_CTRL *pDrvCtrl, M_BLK_ID pMblk);
LOCAL STATUS    m5200FecPollReceive (DRV_CTRL *pDrvCtrl, M_BLK_ID pMblk);
LOCAL STATUS    m5200FecPollStart (DRV_CTRL *pDrvCtrl);
LOCAL STATUS    m5200FecPollStop (DRV_CTRL *pDrvCtrl);

/* globals */

FUNCPTR _func_m5200FecPhyInit = (FUNCPTR) m5200FecPhyInit;
FUNCPTR _func_m5200FecHbFail = (FUNCPTR) NULL;

#ifdef FEC_END_DBG
void m5200FecCsrShow (void);
void m5200FecRbdShow (int);
void m5200FecTbdShow (int);
void m5200FecErrorShow (void);
void m5200FecDrvShow (void);
void m5200FecMiiShow (void);
void m5200FecMibShow (void);

#endif /* FEC_END_DBG */

/*
 * Define the device function table.  This is static across all driver
 * instances.
 */

LOCAL NET_FUNCS netFuncs =
    {
    (FUNCPTR) m5200FecStart,        /* start func. */
    (FUNCPTR) m5200FecStop,     /* stop func. */
    (FUNCPTR) m5200FecUnload,       /* unload func. */
    (FUNCPTR) m5200FecIoctl,        /* ioctl func. */
    (FUNCPTR) m5200FecSend,     /* send func. */
    (FUNCPTR) m5200FecMCastAddrAdd,     /* multicast add func. */
    (FUNCPTR) m5200FecMCastAddrDel,     /* multicast delete func. */
    (FUNCPTR) m5200FecMCastAddrGet,     /* multicast get fun. */
    (FUNCPTR) m5200FecPollSend,     /* polling send func. */
    (FUNCPTR) m5200FecPollReceive,      /* polling receive func. */
    endEtherAddressForm,        /* put address info into a NET_BUFFER */
    (FUNCPTR) endEtherPacketDataGet,    /* get pointer to data in NET_BUFFER */
    (FUNCPTR) endEtherPacketAddrGet     /* Get packet addresses */
    };

/*******************************************************************************
*
* m5200FecEndLoad - initialize the driver and device
*
* This routine initializes both driver and device to an operational state
* using device specific parameters specified by <initString>.
*
* The parameter string, <initString>, is an ordered list of parameters each
* separated by a colon. The format of <initString> is:
*
* "<fecBaseAddr>:<ivec>:<bufBase>:<bufSize>:<fifoTxBase>:<fifoRxBase>
* :<tbdNum>:<rbdNum>:<phyAddr>:<isoPhyAddr>:<phyDefMode>:<userFlags>
* :<clockSpeed>"
*
* The FEC shares a region of memory with the driver.  The caller of this
* routine can specify the address of this memory region, or can specify that
* the driver must obtain this memory region from the system resources.
*
* A default number of transmit/receive buffer descriptors of 32 can be
* selected by passing zero in the parameters <tbdNum> and <rbdNum>.
* In other cases, the number of buffers selected should be greater than two.
*
* The <bufBase> parameter is used to inform the driver about the shared
* memory region.  If this parameter is set to the constant "NONE," then this
* routine will attempt to allocate the shared memory from the system.  Any
* other value for this parameter is interpreted by this routine as the address
* of the shared memory region to be used. The <bufSize> parameter is used
* to check that this region is large enough with respect to the provided
* values of both transmit/receive buffer descriptors.
*
* If the caller provides the shared memory region, then the driver assumes
* that this region does not require cache coherency operations, nor does it
* require conversions between virtual and physical addresses.
*
* If the caller indicates that this routine must allocate the shared memory
* region, then this routine will use cacheDmaMalloc() to obtain
* some  cache-safe memory.  The attributes of this memory will be checked,
* and if the memory is not write coherent, this routine will abort and
* return NULL.
*
* RETURNS: an END object pointer, or NULL on error.
*
* SEE ALSO: ifLib
*/

END_OBJ* m5200FecEndLoad
    (
    char *initString      /* parameter string */
    )
    {
    DRV_CTRL *  pDrvCtrl = NULL;    /* pointer to DRV_CTRL structure */
    UCHAR       enetAddr[6];        /* ethernet address */

    FEC_END_LOG (FEC_END_DBG_LOAD, ("Loading end...\n"), 1, 2, 3, 4, 5, 6);

    if (initString == NULL)
        return (NULL);

    if (initString[0] == 0)
        {
        bcopy ((char *)FEC_END_DEV_NAME, (void *)initString,
                FEC_END_DEV_NAME_LEN);
        return (NULL);
    }

    /* allocate the device structure */

    pDrvCtrl = (DRV_CTRL *) calloc (sizeof (DRV_CTRL), 1);
    if (pDrvCtrl == NULL)
        return (NULL);

    pDrvCtrl->phyInfo = (PHY_INFO *) calloc (sizeof (PHY_INFO), 1);
    if (pDrvCtrl->phyInfo == NULL)
        {
        free ((char *)pDrvCtrl);
        return (NULL);
        }

#ifdef FEC_END_DBG
    pDrvCtrlDbg = pDrvCtrl;
#endif /* FEC_END_DBG */

#ifdef FEC_END_TST
    pTstDrvCtrl = pDrvCtrl;
#endif /* FEC_END_TST */

    /* Parse InitString */

    if (m5200FecInitParse (pDrvCtrl, initString) == ERROR)
        goto errorExit;

    /* sanity check the unit number */

    if (pDrvCtrl->unit < 0 )
        goto errorExit;

    pDrvCtrl->fecBaseAddr = (UINT32)ETH_BASE_ADRS;
    pDrvCtrl->ivecFEC     = IV_ETH;

   /* memory initialization */

    if (m5200FecInitMem (pDrvCtrl) == ERROR)
        goto errorExit;

    /* initailize ethernet tasks */
    if (m5200FecSdmaTaskInit(pDrvCtrl) == ERROR)
        goto errorExit;

    pDrvCtrl->ivecRDMA    = SDMA_TASKNO_TO_IVEC(pDrvCtrl->recvTaskNo);
    pDrvCtrl->ivecWDMA    = SDMA_TASKNO_TO_IVEC(pDrvCtrl->xmitTaskNo);

    /* get our ethernet hardware address */

    SYS_FEC_ENET_ADDR_GET (enetAddr);

    FEC_END_LOG (FEC_END_DBG_LOAD, ("m5200FecEndLoad: enetAddr= \n\
                    0x%x 0x%x 0x%x 0x%x 0x%x 0x%x \n "),
                    enetAddr[0],
                    enetAddr[1],
                    enetAddr[2],
                    enetAddr[3],
                    enetAddr[4],
                    enetAddr[5]);

    /* initialize some flags */

    pDrvCtrl->loaded = TRUE;
    pDrvCtrl->intrConnect = FALSE;
    pDrvCtrl->stoppingTx = TRUE; /* send function currently not available */

    /*
     * create the synchronization semaphores for mii intr handling
     * and for graceful transmit command interrupts.
     */

    FEC_END_MII_SEM_CREATE;
    FEC_END_GRA_SEM_CREATE;

    /* endObj initializations */

    if (END_OBJ_INIT (&pDrvCtrl->endObj, (DEV_OBJ*) pDrvCtrl,
              FEC_END_DEV_NAME, pDrvCtrl->unit, &netFuncs,
              "Motorola FEC Ethernet Enhanced Network Driver") == ERROR)
        goto errorExit;

    pDrvCtrl->phyInfo->phySpeed = FEC_END_10MBS;
    if (END_MIB_INIT (&pDrvCtrl->endObj, M2_ifType_ethernet_csmacd,
              (u_char *) &enetAddr[0], FEC_END_ADDR_LEN,
              ETHERMTU, pDrvCtrl->phyInfo->phySpeed) == ERROR)
        goto errorExit;

    /* Mark the device ready */

    END_OBJ_READY (&pDrvCtrl->endObj,
                   IFF_NOTRAILERS | IFF_MULTICAST | IFF_BROADCAST);

   if ((m5200BusMonitorTID = taskSpawn( "tBusMonitor", 254, 0, 0x1000, 
            (FUNCPTR) m5200FecBusMonTask, (int)pDrvCtrl, 2,3,4,5,6,7,8,9,0)) == ERROR)
        {
        FEC_END_LOG (FEC_END_DBG_LOAD, ("m5200FecStart: failed to spawn tBusMonitor task\n"),
                            1, 2, 3, 4, 5, 6);
        goto errorExit;   
        }

    FEC_END_LOG (FEC_END_DBG_LOAD, ("m5200FecEndLoad... Done \n"),
                    1, 2, 3, 4, 5, 6);

    return (&pDrvCtrl->endObj);

errorExit:
    m5200FecUnload (pDrvCtrl);
    free ((char *) pDrvCtrl);
    return NULL;
    }

/*******************************************************************************
*
* m5200FecUnload - unload a driver from the system
*
* This routine unloads the driver pointed to by <pDrvCtrl> from the system.
*
* RETURNS: OK, always.
*
* SEE ALSO: m5200FecLoad()
*/

LOCAL STATUS m5200FecUnload
    (
    DRV_CTRL    *pDrvCtrl       /* pointer to DRV_CTRL structure */
    )
    {
    int     ix = 0;     /* a counter */

    FEC_END_LOG (FEC_END_DBG_LOAD, ("Unloading end..."), 1, 2, 3, 4, 5, 6);

    if (pDrvCtrl == NULL)
        return (ERROR);

    pDrvCtrl->loaded = FALSE;

    /* free lists */

    for (ix = 0; ix < pDrvCtrl->tbdNum; ix++)
        {
        cfree ((char *) pDrvCtrl->pTbdList [ix]);
        }

    END_OBJECT_UNLOAD (&pDrvCtrl->endObj);

    /* free allocated memory if necessary */

    /* free allocated memory if necessary */

    if ((pDrvCtrl->pMBlkArea) != NULL)
        free (pDrvCtrl->pMBlkArea);

    /* free the semaphores if necessary */

    FEC_END_MII_SEM_DELETE;
    FEC_END_GRA_SEM_DELETE;

    free (pDrvCtrl->phyInfo);

    taskDelete (m5200BusMonitorTID);
    m5200BusMonitorTID = 0;

    FEC_END_LOG (FEC_END_DBG_LOAD, ("m5200FecUnload... Done\n"),
                    1, 2, 3, 4, 5, 6);

    return (OK);
    }

/*******************************************************************************
*
* m5200FecInitParse - parse parameter values from initString
*
* This routine parses parameter values from initString and stores them in
* the related fiels of the driver control structure.
*
* RETURNS: OK or ERROR
*/

LOCAL STATUS m5200FecInitParse
    (
    DRV_CTRL *  pDrvCtrl,           /* pointer to DRV_CTRL structure */
    char *  initString      /* parameter string */
    )
    {
    char *  tok;        /* an initString token */
    char *  holder = NULL;  /* points to initString fragment beyond tok */

    tok = strtok_r (initString, ":", &holder);
    if (tok == NULL)
        return ERROR;
    pDrvCtrl->unit = atoi (tok);

    tok = strtok_r (NULL, ":", &holder);
    if (tok == NULL)
        return ERROR;
    pDrvCtrl->pBufBase = (char *) strtoul (tok, NULL, 16);

    tok = strtok_r (NULL, ":", &holder);
    if (tok == NULL)
        return ERROR;
    pDrvCtrl->bufSize = strtoul (tok, NULL, 16);

    tok = strtok_r (NULL, ":", &holder);
    if (tok == NULL)
        return ERROR;
    pDrvCtrl->fifoTxBase = (UINT32) strtoul (tok, NULL, 16);

    tok = strtok_r (NULL, ":", &holder);
    if (tok == NULL)
        return ERROR;
    pDrvCtrl->fifoRxBase = (UINT32) strtoul (tok, NULL, 16);

    tok = strtok_r (NULL, ":", &holder);
    if (tok == NULL)
        return ERROR;
    pDrvCtrl->tbdNum = (UINT16) strtoul (tok, NULL, 16);

    tok = strtok_r (NULL, ":", &holder);
    if (tok == NULL)
        return ERROR;
    pDrvCtrl->rbdNum = (UINT16) strtoul (tok, NULL, 16);

    tok = strtok_r (NULL, ":", &holder);
    if (tok == NULL)
        return ERROR;
    pDrvCtrl->phyInfo->phyAddr = (UINT8) strtoul (tok, NULL, 16);

    tok = strtok_r (NULL, ":", &holder);
    if (tok == NULL)
        return ERROR;
    pDrvCtrl->phyInfo->isoPhyAddr = (UINT8) strtoul (tok, NULL, 16);

    tok = strtok_r (NULL, ":", &holder);
    if (tok == NULL)
        return ERROR;
    pDrvCtrl->phyInfo->phyDefMode = (UINT8) strtoul (tok, NULL, 16);

    tok = strtok_r (NULL, ":", &holder);
    if (tok == NULL)
        return ERROR;
    pDrvCtrl->userFlags = strtoul (tok, NULL, 16);

    /* start of optional parameters */

    tok = strtok_r (NULL, ":", &holder);
    if (tok != NULL)
        pDrvCtrl->clockSpeed = atoi (tok);

    if (!pDrvCtrl->tbdNum || pDrvCtrl->tbdNum <= 2)
        {
        FEC_END_FLAG_SET (FEC_END_INV_TBD_NUM);
        pDrvCtrl->tbdNum = FEC_END_TBD_DEF_NUM;
        }

    if (!pDrvCtrl->rbdNum || pDrvCtrl->rbdNum <= 2)
        {
        FEC_END_FLAG_SET (FEC_END_INV_RBD_NUM);
        pDrvCtrl->rbdNum = FEC_END_RBD_DEF_NUM;
        }

    FEC_END_LOG (FEC_END_DBG_LOAD,
         ("m5200FecEndParse: unit=%d bufBase=0x%x bufSize=0x%x \n"),
         pDrvCtrl->unit,
         (int) pDrvCtrl->pBufBase,
         pDrvCtrl->bufSize,
         0, 0, 0);

    FEC_END_LOG (FEC_END_DBG_LOAD,
         ("m5200FecEndParse: fifoTxBase=0x%x fifoRxBase=0x%x\n\
         tbdNum=%d rbdNum=%d flags=0x%x\n"),
         pDrvCtrl->fifoTxBase,
         pDrvCtrl->fifoRxBase,
         pDrvCtrl->tbdNum,
         pDrvCtrl->rbdNum,
         pDrvCtrl->userFlags,
         0);

    FEC_END_LOG (FEC_END_DBG_LOAD,
                 ("m5200FecEndParse: phyAddr=0x%x isoPhyAddr=0x%x\n\
                  phyDefMode=0x%x \n"),
                 pDrvCtrl->phyInfo->phyAddr,
                 pDrvCtrl->phyInfo->isoPhyAddr,
                 pDrvCtrl->phyInfo->phyDefMode,
                 0, 0, 0);

    FEC_END_LOG (FEC_END_DBG_LOAD,
                 ("m5200FecEndParse: clock speed=%i\n"),
                 pDrvCtrl->clockSpeed, 0, 0, 0, 0, 0);
    return (OK);
    }

/*******************************************************************************
*
* m5200FecInitMem - initialize memory
*
* This routine initializes all the memory needed by the driver whose control
* structure is passed in <pDrvCtrl>.
*
* RETURNS: OK or ERROR
*/

LOCAL STATUS m5200FecInitMem
    (
    DRV_CTRL *  pDrvCtrl            /* pointer to DRV_CTRL structure */
    )
    {
    UINT32      bdSize;     /* temporary size holder */
    UINT32      clSize;     /* temporary size holder */
    UINT16      clNum;      /* a buffer number holder */
    M_CL_CONFIG     mclBlkConfig = {0, 0, NULL, 0};
                    /* cluster blocks configuration */
    CL_DESC     clDescTbl [] = { {FEC_END_MAX_CL_LEN, 0, NULL, 0} };
                    /* cluster blocks config table */
    int clDescTblNumEnt = (NELEMENTS (clDescTbl));
                    /* number of different clusters */

    /*
     * Initialize a local TX buffer for sending frames.
     * Note: TX buffers must be 4-byte aligned for SDMA.
     * However, we align on 32-byte boundary for cache-line purposes and also
     * add an additional 32 bytes at the end of the buffer as a workaround
     * to the FEC/SDMA errata (see WORKAROUND below in this file).
     * We also add an additional 2, since FEC_END_MAX_PCK_SZ is not divisible
     * by 4. Total memory allocated is: 1584 bytes.
     */

    pDrvCtrl->pTxBuffLocal = (char *) memalign (32, FEC_END_MAX_PCK_SZ + 66);

    if (pDrvCtrl->pTxBuffLocal == NULL)
        {
        pDrvCtrl->txBuffAvailable = FALSE;
        return (ERROR);
        }
    else
        {
        pDrvCtrl->txBuffAvailable = TRUE;
        }

    /* initialize the netPool */

    if ((pDrvCtrl->endObj.pNetPool = malloc (sizeof (NET_POOL))) == NULL)
        return (ERROR);

    /*
     * we include here room for both TBDs and RBDs,
     * and the alignment factor.
     */

    bdSize = (FEC_END_BD_MEM (pDrvCtrl) + FEC_END_BD_ALIGN);

    /*
     * Establish the memory area that we will share with the device.  If
     * the caller has provided an area, then we assume it is non-cacheable
     * and will not require the use of the special cache routines.
     * If the caller did provide an area, then we must obtain it from
     * the system, using the cache savvy allocation routine.
     */

    /* for now BestComm SRAM is not cached */
    pDrvCtrl->bdCacheFuncs = cacheNullFuncs;

    /* cache functions descriptor for data buffers */

    m5200FecBufCacheFuncs.flushRtn = m5200FecBufFlushRtn;
    m5200FecBufCacheFuncs.invalidateRtn = m5200FecBufInvRtn;
    m5200FecBufCacheFuncs.virtToPhysRtn = NULL;
    m5200FecBufCacheFuncs.physToVirtRtn = NULL;

    pDrvCtrl->bufCacheFuncs = m5200FecBufCacheFuncs;

    /*
     * number of clusters, including loaning buffers, a min number
     * of transmit clusters for copy-mode transmit, and one transmit
     * cluster for polling operation.
     */

    clNum = pDrvCtrl->rbdNum + FEC_END_BD_LOAN_NUM
            + 1 + FEC_END_TX_CL_NUM;

    clSize = (FEC_END_TX_MEM (pDrvCtrl) + FEC_END_RX_MEM (pDrvCtrl)
            + (CL_OVERHEAD * clNum)
            + FEC_END_BD_ALIGN);

    /* pool of mblks */

    if (mclBlkConfig.mBlkNum == 0)
       mclBlkConfig.mBlkNum = clNum * 2;

    /* pool of clusters, including loaning buffers */

    if (clDescTbl[0].clNum == 0)
        {
        clDescTbl[0].clNum = clNum;
        clDescTbl[0].clSize = FEC_END_MAX_CL_LEN;
        }

    /* there's a cluster overhead and an alignment issue */

    clDescTbl[0].memSize = (clDescTbl[0].clNum *
                            (clDescTbl[0].clSize + CL_OVERHEAD));
    clDescTbl[0].memArea = (char *) (memalign (CL_ALIGNMENT, clSize));

    if (clDescTbl[0].memArea == NULL)
        {
        return (ERROR);
        }

    /* store the pointer to the clBlock area */

    pDrvCtrl->pClBlkArea = clDescTbl[0].memArea;
    pDrvCtrl->clBlkSize = clDescTbl[0].memSize;

    /* pool of cluster blocks */

    if (mclBlkConfig.clBlkNum == 0)
        mclBlkConfig.clBlkNum = clDescTbl[0].clNum;

    /* get memory for mblks */

    if (mclBlkConfig.memArea == NULL)
        {
        /* memory size adjusted to hold the netPool pointer at the head */

        mclBlkConfig.memSize = ((mclBlkConfig.mBlkNum
                                * (M_BLK_SZ + MBLK_ALIGNMENT))
                                + (mclBlkConfig.clBlkNum
                                * (CL_BLK_SZ + CL_ALIGNMENT)));

        mclBlkConfig.memArea = (char *) memalign (MBLK_ALIGNMENT,
                                                  mclBlkConfig.memSize);

        if (mclBlkConfig.memArea == NULL)
            {
            return (ERROR);
            }

        /* store the pointer to the mBlock area */

        pDrvCtrl->pMBlkArea = mclBlkConfig.memArea;
        pDrvCtrl->mBlkSize = mclBlkConfig.memSize;
        }

    /* init the mem pool */

    if (netPoolInit (pDrvCtrl->endObj.pNetPool, &mclBlkConfig,
                     &clDescTbl[0], clDescTblNumEnt, NULL) == ERROR)
        {
        return (ERROR);
        }

    if ((pDrvCtrl->pClPoolId =
            netClPoolIdGet (pDrvCtrl->endObj.pNetPool,
                            FEC_END_MAX_CL_LEN, FALSE)) == NULL)
        {
        return (ERROR);
        }

    FEC_END_LOG (FEC_END_DBG_LOAD, ("m5200FecInitMem... Done\n"),
                    0, 0, 0, 0, 0, 0);

    return OK;
    }

/**************************************************************************
*
* m5200FecBusMonTask - poll the link status
*
* This routine continuously polls the FEC link status.
*
* RETURNS: N/A
*/

void m5200FecBusMonTask
    (
    DRV_CTRL *pDrvCtrl
    )
    {
        taskDelay (12*sysClkRateGet());
        
        FOREVER
            {
            m5200FecBusMonitor(pDrvCtrl);
            taskDelay (2*sysClkRateGet());
            }
    }

#ifdef NO_RX_INT
/**************************************************************************
*
* m5200FecRxPoll - poll the receive interrupt
*
* This routine continuously polls the FEC receive interrupt.
*
* RETURNS: N/A
*/

void m5200FecRxPoll
    (
    DRV_CTRL *pDrvCtrl
    )
    {
        FOREVER
        {
        taskDelay (sysClkRateGet());
            m5200FecHandleRecvInt(pDrvCtrl);
        }
    }
#endif

/**************************************************************************
*
* m5200FecStart - start the device
*
* This routine starts the FEC device and brings it up to an operational
* state.  The driver must have already been loaded with the m5200FecEndLoad()
* routine.
*
* RETURNS: OK, or ERROR if the device could not be initialized.
*
* INTERNAL
* The speed field inthe phyInfo structure is only set after the call
* to the physical layer init routine. On the other hand, the mib2
* interface is initialized in the m5200FecEndLoad() routine, and the default
* value of 10Mbit assumed there is not always correct. We need to
* correct it here.
*
*/

LOCAL STATUS m5200FecStart
    (
    DRV_CTRL *pDrvCtrl       /* pointer to DRV_CTRL structure */
    )
    {
    int     retVal;     /* convenient holder for return value */
#if  0
    char        bucket[4];
#endif
    int level;

    FEC_END_LOG (FEC_END_DBG_START, ("Starting end...\n"), 1, 2, 3, 4, 5, 6);

    /* must have been loaded */

    if (!pDrvCtrl->loaded)
        return (ERROR);

#if 0
    if (vxMemProbe ((char *) (pDrvCtrl->fecBaseAddr + FEC_END_CSR_OFF),
                              VX_READ, 4, &bucket[0]) != OK)
        {
        FEC_END_LOG (FEC_END_DBG_START,
                   (": need MMU mapping for address 0x%x\n"),
                   (UINT32) pDrvCtrl->fecBaseAddr, 2, 3, 4, 5, 6);
        return (ERROR);
        }
#endif

    /* reset the chip */

    FEC_END_ETH_SET;

    if (m5200FecReset (pDrvCtrl) != OK)
        return (ERROR);

    if (m5200FecTbdInit (pDrvCtrl) == ERROR)
        return (ERROR);

    if (m5200FecRbdInit (pDrvCtrl) == ERROR)
        return (ERROR);

    /* set some flags to default values */

    pDrvCtrl->txStall = FALSE;
    pDrvCtrl->tbdIndex = 0;
    pDrvCtrl->usedTbdIndex = 0;
    pDrvCtrl->cleanTbdNum = pDrvCtrl->tbdNum;
    pDrvCtrl->rbdIndex = 0;
    pDrvCtrl->stoppingTx = FALSE;

    /* connect the interrupt handlers */

    SYS_FEC_INT_CONNECT (pDrvCtrl, m5200FecInt, m5200FecRdmaInt, m5200FecWdmaInt, (int) pDrvCtrl, retVal);
    if (retVal == ERROR)
        return (ERROR);

    /* enable system interrupt: set relevant bit in SIMASK */

    SYS_FEC_INT_ENABLE (pDrvCtrl, retVal);
    if (retVal == ERROR)
        return (ERROR);

    /* call the BSP to do any other initialization */

    SYS_FEC_ENET_ENABLE;

    /* configure some chip's registers */

    if (m5200FecPrePhyConfig (pDrvCtrl) == ERROR)
        return (ERROR);

    /* initialize some fields in the PHY info structure */

    if (m5200FecPhyPreInit (pDrvCtrl) != OK)
        {
        FEC_END_LOG (FEC_END_DBG_LOAD, ("Failed to pre-initialize the PHY\n"),
                        0, 0, 0, 0, 0, 0);
        return (ERROR);
        }

    /* initialize the Physical medium layer */

    if (_func_m5200FecPhyInit == NULL)
        return (ERROR);

    if (((* _func_m5200FecPhyInit) (pDrvCtrl)) != OK)
        {
        FEC_END_LOG (FEC_END_DBG_LOAD, ("Failed to initialize the PHY\n"),
                        0, 0, 0, 0, 0, 0);
        return (ERROR);
        }

    /* configure other chip's registers */

    if (m5200FecPostPhyConfig (pDrvCtrl) == ERROR)
        return (ERROR);

    /* correct the speed for the mib2 stuff */

    pDrvCtrl->endObj.mib2Tbl.ifSpeed = pDrvCtrl->phyInfo->phySpeed;

    if (END_FLAGS_ISSET (IFF_MULTICAST))
        FEC_END_FLAG_SET (FEC_END_MCAST);

    level = intLock();
    /* Enable SmartDMA receive task & enable interrupts */
    TaskIntClear( pDrvCtrl->recvTaskNo );
    intUnlock(level);

#ifdef NO_RX_INT
    if ((pDrvCtrl->rxPollTID = taskSpawn( "tFecRxPoll", 30, 0, 0x1000, m5200FecRxPoll, pDrvCtrl, 2,3,4,5,6,7,8,9,0)) == ERROR)
        {
            FEC_END_LOG (FEC_END_DBG_LOAD, ("m5200FecStart: failed to spawn tFecRxPoll task\n"),
                                1, 2, 3, 4, 5, 6);
            return ERROR;
        }

    level = intLock();
    if (TaskStart( pDrvCtrl->recvTaskNo, TASK_AUTOSTART_ENABLE, pDrvCtrl->recvTaskNo, FALSE) != TASK_ERR_NO_ERR)

#else

    level = intLock();
    if (TaskStart( pDrvCtrl->recvTaskNo, TASK_AUTOSTART_ENABLE, pDrvCtrl->recvTaskNo, TRUE) != TASK_ERR_NO_ERR)
#endif
        {
        intUnlock(level);
        FEC_END_LOG (FEC_END_DBG_LOAD, ("m5200FecStart: failed to start RX task\n"),
                            1, 2, 3, 4, 5, 6);
        return ERROR;
        }
    intUnlock(level);

    /* enable the Ethernet Controller */

    FEC_END_ETH_ENABLE;

    /* mark the interface as up */

    END_FLAGS_SET (&pDrvCtrl->endObj, (IFF_UP | IFF_RUNNING));

    /* Flush the write pipe */

    CACHE_PIPE_FLUSH ();

    FEC_END_LOG (FEC_END_DBG_START, ("Starting end... Done\n"),
                     1, 2, 3, 4, 5, 6);

    (void) netJobAdd ((FUNCPTR) muxTxRestart,
                        (int) &pDrvCtrl->endObj, 0, 0, 0, 0);

    return (OK);
    }

/**************************************************************************
*
* m5200FecStop - stop the 'motfec' interface
*
* This routine marks the interface as inactive, disables interrupts and
* the Ethernet Controller. As a result, reception is stopped immediately,
* and transmission is stopped after a bad CRC is appended to any frame
* currently being transmitted. The reception/transmission control logic
* (FIFO pointers, buffer descriptors, etc.) is reset. To bring the
* interface back up, m5200FecStart() must be called.
*
* RETURNS: OK, always.
*/

LOCAL STATUS m5200FecStop
    (
    DRV_CTRL *  pDrvCtrl       /* pointer to DRV_CTRL structure */
    )
    {
    int     retVal;     /* convenient holder for return value */
    int level;

    FEC_END_LOG (FEC_END_DBG_LOAD, ("m5200FecStop...\n"), 1, 2, 3, 4, 5, 6);

    /* make sure driver is operational before stopping it */

    if (END_FLAGS_GET(&pDrvCtrl->endObj) & (IFF_UP | IFF_RUNNING))
        {

        /* mark the interface as down */

        END_FLAGS_CLR (&pDrvCtrl->endObj, (IFF_UP | IFF_RUNNING));

        /* issue a graceful transmit command */

        FEC_END_CSR_WR (FEC_END_TX_CTRL_OFF, FEC_END_TX_CTRL_GRA);

        /* wait for the related interrupt - with Timeout */

        FEC_END_GRA_SEM_TAKE;

        /* mask chip interrupts */

        FEC_END_INT_DISABLE;

        /* stop SmartDMA tasks */
        FEC_END_LOG (FEC_END_DBG_LOAD, ("Stop FEC TX&RX Tasks\n"),
                        0, 0, 0, 0, 0, 0);
#ifdef NO_RX_INT
        if (pDrvCtrl->rxPollTID != ERROR)
        {
            taskDelete (pDrvCtrl->rxPollTID);
            pDrvCtrl->rxPollTID = ERROR;
        }
#endif
	level = intLock();
        TaskStop (pDrvCtrl->xmitTaskNo);
        TaskStop (pDrvCtrl->recvTaskNo);
	intUnlock(level);

        TaskBDReset (pDrvCtrl->recvTaskNo);
        TaskBDReset (pDrvCtrl->xmitTaskNo);

        /* disable the Ethernet Controller */
        FEC_END_ETH_DISABLE;

        /* disable system interrupt: reset relevant bit in SIMASK */

        SYS_FEC_INT_DISABLE (pDrvCtrl, retVal);
        if (retVal == ERROR)
            return (ERROR);

        /* disconnect the interrupt handler */

        SYS_FEC_INT_DISCONNECT (pDrvCtrl, m5200FecInt, m5200FecRdmaInt, m5200FecWdmaInt, (int)pDrvCtrl, retVal);
        if (retVal == ERROR)
            return (ERROR);

        /* call the BSP to disable FEC */

        SYS_FEC_ENET_DISABLE;

        if (m5200FecBdFree (pDrvCtrl) != OK)
            return (ERROR);

        /* Reclaim net buffer waiting on transmission */

        m5200FecTbdReclaim (pDrvCtrl);
        }

    FEC_END_LOG (FEC_END_DBG_LOAD, ("m5200FecStop... Done \n"),
                    1, 2, 3, 4, 5, 6);

    return OK;
    }

/**************************************************************************
*
* m5200FecReset - reset the `m5200Fec' interface
*
* This routine resets the chip by setting the Reset bit in the Ethernet
* Control Register. The ETHER_EN bit is cleared, and all the FEC registers
* take their default values. In addition, any transmission/reception
* currently in progress is abruptly aborted.
*
* RETURNS: OK or ERROR if the FEC does not come out reset.
*/

LOCAL STATUS m5200FecReset
    (
    DRV_CTRL *  pDrvCtrl       /* pointer to DRV_CTRL structure */
    )
    {
    int timeout = 600;
    UINT32 status;

    /*
     * Clear FIFO status registers.
     */
    FEC_END_CSR_RD (FEC_END_rfifo_status_OFF, status);
    status &= FEC_FIFO_STAT_ERROR;
    FEC_END_CSR_WR (FEC_END_rfifo_status_OFF, status);

    FEC_END_CSR_RD (FEC_END_tfifo_status_OFF, status);
    status &= FEC_FIFO_STAT_ERROR;
    FEC_END_CSR_WR (FEC_END_tfifo_status_OFF, status);

    FEC_END_CSR_WR (FEC_END_reset_cntrl_OFF, 0x01000000);

    /* issue a reset command to the FEC chip */

    FEC_END_CSR_WR (FEC_END_CTRL_OFF, FEC_END_ETH_RES);

    /* wait at least 16 clock cycles */

    taskDelay (1);

    FEC_END_CSR_RD (FEC_END_CTRL_OFF, status);

    while ((status & FEC_END_ETH_RES) != 0)
        {
        taskDelay (1);

        FEC_END_CSR_RD (FEC_END_CTRL_OFF, status);

        if (timeout == 0)
            {
            FEC_END_LOG (FEC_END_DBG_ERR,
                         ("m5200FecReset: reset error 0x%08x...\n"),
                         status, 0, 0, 0, 0, 0);

            return (ERROR);
            }

        timeout--;
        }

    return (OK);
    }

/*******************************************************************************
*
* m5200FecInt - entry point for handling interrupts from the FEC
*
* The interrupting events are acknowledged to the device, so that the device
* will de-assert its interrupt signal.  The amount of work done here is kept
* to a minimum; the bulk of the work is deferred to the netTask.
*
* RETURNS: N/A
*/

LOCAL void m5200FecInt
    (
    DRV_CTRL *  pDrvCtrl       /* pointer to DRV_CTRL structure */
    )
    {
    UINT32  event = 0;  /* event intr register */
    UINT32  mask = 0;   /* holder for the mask intr register */
    UINT32  status;     /* status word */
    UINT32  status2;
    int level;

    CACHE_PIPE_FLUSH ();

    /* read and save the interrupt event register */

    FEC_END_CSR_RD (FEC_END_EVENT_OFF, event);

    /* clear all interrupts */

    FEC_END_CSR_WR (FEC_END_EVENT_OFF, (event & FEC_END_EVENT_MSK));

    CACHE_PIPE_FLUSH ();

    /* read the interrupt mask register */

    FEC_END_CSR_RD (FEC_END_MASK_OFF, mask);

    /* read CSR status word again */

    FEC_END_CSR_RD (FEC_END_EVENT_OFF, status);

    FEC_END_LOG (FEC_END_DBG_INT, ("m5200FecInt: event 0x%x, status 0x%x\n"),
                   (event & FEC_END_EVENT_MSK), status,
                   0, 0, 0, 0);

    /*
     * ----------------------------------------------------------------------
     * handle bus error interrupts
     * Note - Interrupt not generated on MPC5200 version of FEC
     * ----------------------------------------------------------------------
     */

    if ((event & FEC_END_EVENT_BERR) == FEC_END_EVENT_BERR)
        {
        FEC_END_LOG (FEC_END_DBG_ERR, ("m5200FecInt: bus error, restart chip\n"),
                           0, 0, 0, 0, 0, 0);

        /* Start the recovery process */

        /* Prevent queue of new transmissions as we are stopping */

        pDrvCtrl->stoppingTx = TRUE;

        (void) netJobAdd ((FUNCPTR) m5200FecStop, (int) pDrvCtrl,
                            event, 0, 0, 0);
        (void) netJobAdd ((FUNCPTR) m5200FecStart, (int) pDrvCtrl,
                            event, 0, 0, 0);
        }

    /*
     * ----------------------------------------------------------------------
     * XFIFO_ERROR: When the XFIFO_ERROR error condition is met, the
     * ECNTRL.ETHER_EN bit is cleared by h/w, halting the FEC.
     * To recover from this condition the MPC5200 UM states that a
     * software reset of the FEC is required.
     * ----------------------------------------------------------------------
     */

    if ((event & FEC_END_EVENT_FIFO_ERR) == FEC_END_EVENT_FIFO_ERR)
        {

#ifdef FEC_END_TST
        tstFifoErrorFlag = TRUE;
#endif

        FEC_END_CSR_RD (FEC_END_tfifo_status_OFF, status2);

        FEC_END_LOG (FEC_END_DBG_ERR,
                     ("m5200FecInt: TxFIFO error (status: 0x%x), clearing\n"),
                     status2, 0, 0, 0, 0, 0);

        /* stop SmartDMA transmit task */
	level = intLock();
        TaskStop (pDrvCtrl->xmitTaskNo);
	intUnlock(level);

        FEC_END_CSR_RD (FEC_END_tfifo_status_OFF, status2);

        status2 &= (FEC_FIFO_STAT_ERROR | FEC_FIFO_STAT_UF |
                    FEC_FIFO_STAT_OF| FEC_FIFO_STAT_ALARM);

        FEC_END_CSR_WR (FEC_END_tfifo_status_OFF, status2);

        /*
         * Set the FIFO state to empty. This is a two-step process.
         */

        FEC_END_CSR_WR (FEC_END_tfifo_rdptr_OFF, 0);
        FEC_END_CSR_WR (FEC_END_tfifo_wrptr_OFF, 4);
        FEC_END_CSR_WR (FEC_END_tfifo_lrf_ptr_OFF, 0);
        FEC_END_CSR_WR (FEC_END_tfifo_lwf_ptr_OFF, 0);
        FEC_END_CSR_WR (FEC_END_tfifo_rdptr_OFF, 0);
        FEC_END_CSR_WR (FEC_END_tfifo_wrptr_OFF, 0);

        /* Flush the write pipe */

        CACHE_PIPE_FLUSH ();

        /* Start the recovery process */

        /* Prevent queue of new transmissions as we are stopping */

        pDrvCtrl->stoppingTx = TRUE;

        (void) netJobAdd ((FUNCPTR) m5200FecStop, (int) pDrvCtrl,
                            event, 0, 0, 0);
        (void) netJobAdd ((FUNCPTR) m5200FecStart, (int) pDrvCtrl,
                            event, 0, 0, 0);
        }

    /*
     * ----------------------------------------------------------------------
     * RFIFO_ERROR: When the RFIFO_ERROR error condition is met, the
     * ECNTRL.ETHER_EN bit is cleared by h/w, halting the FEC.
     * To recover from this condition the MPC5200 UM states that a
     * software reset of the FEC is required.
     * ----------------------------------------------------------------------
     */

    if ((event & FEC_END_EVENT_RFIFO_ERR) == FEC_END_EVENT_RFIFO_ERR)
        {

#ifdef FEC_END_TST
        tstFifoErrorFlag = TRUE;
#endif

        FEC_END_CSR_RD (FEC_END_rfifo_status_OFF, status2);

        FEC_END_LOG (FEC_END_DBG_ERR,
                     ("m5200FecInt: RxFIFO error (status 0x%x), clearing\n"),
                     status2, 0, 0, 0, 0, 0);

#ifdef NO_RX_INT
        if (pDrvCtrl->rxPollTID != ERROR)
            {
            taskDelete (pDrvCtrl->rxPollTID);
            pDrvCtrl->rxPollTID = ERROR;
            }
#endif
	level = intLock();
        TaskStop (pDrvCtrl->recvTaskNo);
	intUnlock(level);

        FEC_END_CSR_RD (FEC_END_rfifo_status_OFF, status2);

        status2 &= (FEC_FIFO_STAT_ERROR | FEC_FIFO_STAT_UF | FEC_FIFO_STAT_OF);

        if (status2 > 0)
            {
            FEC_END_CSR_WR (FEC_END_rfifo_lrf_ptr_OFF, 0);
            FEC_END_CSR_WR (FEC_END_rfifo_lwf_ptr_OFF, 0);
            FEC_END_CSR_WR (FEC_END_rfifo_rdptr_OFF, 0);
            FEC_END_CSR_WR (FEC_END_rfifo_wrptr_OFF, 0);
            status2 |= FEC_FIFO_STAT_ERROR;
            FEC_END_CSR_WR (FEC_END_rfifo_status_OFF, (status2&0x00f00000));
            }

        /* Flush the write pipe */

        CACHE_PIPE_FLUSH ();

        /* Start the recovery process */

        /* Prevent queue of new transmissions as we are stopping */

        pDrvCtrl->stoppingTx = TRUE;

        (void) netJobAdd ((FUNCPTR) m5200FecStop, (int) pDrvCtrl,
                            event, 0, 0, 0);
        (void) netJobAdd ((FUNCPTR) m5200FecStart, (int) pDrvCtrl,
                            event, 0, 0, 0);
        }

    /*
     * ----------------------------------------------------------------------
     * handle babbling transmit error interrupts
     * ----------------------------------------------------------------------
     */

    if ((event & FEC_END_EVENT_BABT) == FEC_END_EVENT_BABT)
        {
        FEC_END_BAB_TX_ADD;

        FEC_END_LOG (FEC_END_DBG_ERR, ("m5200FecInt: babbling tx error\n"),
                           0, 0, 0, 0, 0, 0);
        }

    /*
     * ----------------------------------------------------------------------
     * handle babbling receive error interrupts
     * ----------------------------------------------------------------------
     */

    if ((event & FEC_END_EVENT_BABR) == FEC_END_EVENT_BABR)
        {
        FEC_END_BAB_RX_ADD;

        FEC_END_LOG (FEC_END_DBG_ERR, ("m5200FecInt: babbling rx error\n"),
                           0, 0, 0, 0, 0, 0);
        }

    /*
     * ----------------------------------------------------------------------
     * handle late collision error interrupts
     * ----------------------------------------------------------------------
     */

    if ((event & FEC_END_EVENT_LCOL) == FEC_END_EVENT_LCOL)
        {
        FEC_END_LCOL_ADD;

        FEC_END_LOG (FEC_END_DBG_ERR, ("m5200FecInt: late collision error\n"),
                           0, 0, 0, 0, 0, 0);
        }

    /*
     * ----------------------------------------------------------------------
     * handle collision retry error interrupts
     * ----------------------------------------------------------------------
     */

    if ((event & FEC_END_EVENT_COL_RL) == FEC_END_EVENT_COL_RL)
        {
        FEC_END_COL_RL_ADD;

        FEC_END_LOG (FEC_END_DBG_ERR, ("m5200FecInt: collision retry error\n"),
                           0, 0, 0, 0, 0, 0);
        }

    /*
     * ----------------------------------------------------------------------
     * handle TxFido underrun error interrupts
     * ----------------------------------------------------------------------
     */

    if ((event & FEC_END_EVENT_XFIFO_UN) == FEC_END_EVENT_XFIFO_UN)
        {
        FEC_END_TX_UN_ADD;

        FEC_END_LOG (FEC_END_DBG_ERR, ("m5200FecInt: TxFIFO underrun error\n"),
                           0, 0, 0, 0, 0, 0);
        }

    /*
     * ----------------------------------------------------------------------
     * handle heartbeat check fail interrupts
     * ----------------------------------------------------------------------
     */

    if ((event & FEC_END_EVENT_HB) == FEC_END_EVENT_HB)
        {
        FEC_END_HB_FAIL_ADD;

        if (_func_m5200FecHbFail != NULL)
            (* _func_m5200FecHbFail) (pDrvCtrl);

        FEC_END_LOG (FEC_END_DBG_ERR, ("m5200FecInt: heartbeat check fail\n"),
                           0, 0, 0, 0, 0, 0);
        }

    /*
     * ----------------------------------------------------------------------
     * handle mii frame completion interrupts
     * ----------------------------------------------------------------------
     */

    if ((event & FEC_END_EVENT_MII) == FEC_END_EVENT_MII)
        {
        FEC_END_LOG (FEC_END_DBG_INT, ("m5200FecInt: mii transfer\n"),
                           0, 0, 0, 0, 0, 0);

        }

    /*
     * ----------------------------------------------------------------------
     * handle graceful transmit interrupts
     * ----------------------------------------------------------------------
     */

    if ((event & FEC_END_EVENT_GRA) == FEC_END_EVENT_GRA)
        {
        FEC_END_LOG (FEC_END_DBG_INT, ("m5200FecInt: graceful transmit\n"),
                           0, 0, 0, 0, 0, 0);

        /* let the Stop task finish off its job */

        FEC_END_GRA_SEM_GIVE;
        }
    }

/*******************************************************************************
*
* m5200FecRdmaInt - entry point for handling interrupts from the Read DMA
*
* The interrupting events are acknowledged to the device, so that the device
* will de-assert its interrupt signal.  The amount of work done here is kept
* to a minimum; the bulk of the work is deferred to the netTask.
*
* RETURNS: N/A
*/

LOCAL void m5200FecRdmaInt
    (
    DRV_CTRL *  pDrvCtrl       /* pointer to DRV_CTRL structure */
    )
    {
    int level;

    UINT32 rfifo_status, rfifo_lrf_ptr, rfifo_lwf_ptr, rfifo_alarm,
           rfifo_rdptr, rfifo_wrptr;

    FEC_END_CSR_RD (FEC_END_rfifo_status_OFF, rfifo_status);
    FEC_END_CSR_RD (FEC_END_rfifo_lrf_ptr_OFF, rfifo_lrf_ptr );
    FEC_END_CSR_RD (FEC_END_rfifo_lwf_ptr_OFF, rfifo_lwf_ptr );
    FEC_END_CSR_RD (FEC_END_rfifo_alarm_OFF, rfifo_alarm );
    FEC_END_CSR_RD (FEC_END_rfifo_rdptr_OFF, rfifo_rdptr );
    FEC_END_CSR_RD (FEC_END_rfifo_wrptr_OFF, rfifo_wrptr );


    FEC_END_LOG (FEC_END_DBG_RX,
                 ("m5200FecRdmaInt: RFIFO status 0x%08x lrf_ptr 0x%08x lwf_ptr 0x%08x alarm 0x%08x rdptr 0x%08x wrptr 0x%08x\n"),
                 rfifo_status, rfifo_lrf_ptr, rfifo_lwf_ptr, rfifo_alarm,
                 rfifo_rdptr, rfifo_wrptr);
    FEC_END_LOG (FEC_END_DBG_RX,
                 ("m5200FecRdmaInt: SDMA curptr 0x%08x endptr 0x%08x intPend 0x%08x tcr23 0x%08x\n"),
                 sysSDMA->currentPointer, sysSDMA->endPointer,
                 sysSDMA->IntPend, sysSDMA->TCR23, 5,6);

    level = intLock();
    SDMA_INT_DISABLE( SDMA_INT_MASK, pDrvCtrl->recvTaskNo );
    TaskIntClear( pDrvCtrl->recvTaskNo );
    intUnlock(level);

    /* Flush the write pipe */

    CACHE_PIPE_FLUSH ();

    (void) netJobAdd ((FUNCPTR) m5200FecRxHandle, (int) pDrvCtrl,
              0, 0, 0, 0);
    }

/*******************************************************************************
*
* m5200FecWdmaInt - entry point for handling interrupts from the Write DMA
*
* The interrupting events are acknowledged to the device, so that the device
* will de-assert its interrupt signal.  The amount of work done here is kept
* to a minimum; the bulk of the work is deferred to the netTask.
*
* RETURNS: N/A
*/

LOCAL void m5200FecWdmaInt
    (
    DRV_CTRL *  pDrvCtrl       /* pointer to DRV_CTRL structure */
    )
    {
    int level;

    FEC_END_LOG (FEC_END_DBG_TX, ("m5200FecWdmaInt: \n"), 0,0,0,0,0,0);

    level = intLock();
    SDMA_INT_DISABLE( SDMA_INT_MASK, pDrvCtrl->xmitTaskNo );
    TaskIntClear( pDrvCtrl->xmitTaskNo );
    intUnlock(level);

    /* Flush the write pipe */

    CACHE_PIPE_FLUSH ();

    (void) netJobAdd ((FUNCPTR) m5200FecTxHandle, (int) pDrvCtrl,
              0, 0, 0, 0);
    }

/*******************************************************************************
*
* m5200FecSend - send an Ethernet packet
*
* This routine() takes a M_BLK_ID and sends off the data in the M_BLK_ID.
* The buffer must already have the addressing information properly installed
* in it. This is done by a higher layer.
*
* muxSend() calls this routine each time it wants to send a packet.
*
* RETURNS: OK - Frame handed to FEC for transmission, driver to free mBlk.
*          END_ERR_BLOCK - Frame not handed to FEC, driver doesn't free mBlk.
*          ERROR - Frame not handed to FEC, driver has freed mBlk.
*                  Or we are polling (driver has freed mBlk).
*/

LOCAL STATUS m5200FecSend
    (
    DRV_CTRL *  pDrvCtrl,       /* pointer to DRV_CTRL structure */
    M_BLK *     pMblk           /* pointer to the mBlk/cluster pair */
    )
    {
    UINT8       fragNum = 0;    /* number of fragments in this mBlk */
    UINT16      pktType = 0;    /* packet type (unicast or multicast) */
    STATUS              retVal = OK;

    FEC_END_LOG (FEC_END_DBG_TX, ("m5200FecSend...\n"), 1, 2, 3, 4, 5, 6);

    END_TX_SEM_TAKE (&pDrvCtrl->endObj, WAIT_FOREVER);

    /* check device mode */

    if (pDrvCtrl->stoppingTx == TRUE)
        {
        END_TX_SEM_GIVE (&pDrvCtrl->endObj);
        return (END_ERR_BLOCK);
        }

    if (FEC_END_FLAG_ISSET (FEC_END_POLLING))
        {
        netMblkClChainFree (pMblk); /* free the given mBlk chain */

        errno = EINVAL;
        END_ERR_ADD (&pDrvCtrl->endObj, MIB2_OUT_ERRS, +1);

        END_TX_SEM_GIVE (&pDrvCtrl->endObj);

        return (ERROR);
        }

    /* walk the mBlk */

    if (m5200FecMblkWalk (pMblk, &fragNum, &pktType) == ERROR)
        {
        retVal = END_ERR_BLOCK;
        goto m5200FecSendError;
        }

    /* check we have enough resources */

    if ((pDrvCtrl->cleanTbdNum) == 0)
        {

        /*
         * SPR 75008 - first run the clean routine and then if there still
         * are not any free Tx buffer descriptors, set the stall flag and
         * the stall condition will be handled in m5200FecTbdClean()
         */

        m5200FecTbdClean (pDrvCtrl);

        if (pDrvCtrl->cleanTbdNum == 0)
            {
            /* Delay a little and try again */
            taskDelay (1);
            m5200FecTbdClean (pDrvCtrl);
            }

        if (pDrvCtrl->cleanTbdNum == 0)
            {
            retVal = END_ERR_BLOCK;
            goto m5200FecSendError;
            }
        }

#ifdef SEND_IN_COPY_MODE

        fragNum = 1;

        if ((retVal = m5200FecPktCopyTransmit (pDrvCtrl, pMblk, fragNum, pktType)) != OK)
            {
            goto m5200FecSendError;
            }

        FEC_END_TX_PKT_COPY;

#else

/* this version of bestcomm fec code requires longword alignment */
    if (((pDrvCtrl->cleanTbdNum) >= fragNum) &&
        ((UINT32)(pMblk->mBlkHdr.mData)%4 == 0) &&
        !m5200FecForceCopy)

        {
        /* transmit the packet in zero-copy mode */

        if ((retVal = m5200FecPktTransmit (pDrvCtrl, pMblk, fragNum, pktType)) != OK)
            {
            goto m5200FecSendError;
            }

        FEC_END_TX_PKT_NO_COPY;

        }
    else
        {
        /* transmit the packet in copy mode */

        fragNum = 1;

        if ((retVal = m5200FecPktCopyTransmit (pDrvCtrl, pMblk, fragNum, pktType)) != OK)
            {
            goto m5200FecSendError;
            }

        FEC_END_TX_PKT_COPY;

        }
#endif /* SEND_IN_COPY_MODE */

    END_TX_SEM_GIVE (&pDrvCtrl->endObj);

    FEC_END_LOG (FEC_END_DBG_TX, ("m5200FecSend...Done\n"), 1, 2, 3, 4, 5, 6);

    return (OK);

m5200FecSendError:

    if (retVal == END_ERR_BLOCK)
        {
        /* make sure muxTxRestart() will be called later (SPR 30135) */
        pDrvCtrl->txStall = TRUE;
        }

    END_ERR_ADD (&pDrvCtrl->endObj, MIB2_OUT_ERRS, +1);

    END_TX_SEM_GIVE (&pDrvCtrl->endObj);

    FEC_END_LOG (FEC_END_DBG_TX, ("m5200FecSend...Failed\n"), 1, 2, 3, 4, 5, 6);

    return (END_ERR_BLOCK);
    }

#ifndef SEND_IN_COPY_MODE

/******************************************************************************
*
* m5200FecPktTransmit - transmit a packet
*
* This routine transmits the packet described by the given parameters
* over the network, without copying the mBlk to the driver's buffer.
* It also updates statistics.
*
* RETURNS: OK, or ERROR if no resources were available.
*/

LOCAL STATUS m5200FecPktTransmit
    (
    DRV_CTRL *  pDrvCtrl,       /* pointer to DRV_CTRL structure */
    M_BLK *     pMblk,          /* pointer to the mBlk */
    UINT8   fragNum,    /* number of fragments */
    UINT16  pktType     /* packet type */
    )
    {
    int         bufLen = 0; /* length of this fragment */
    int         pktLen = 0; /* length of this packet */
    int         totLen = 0; /* length of data to be sent */
    char *      pBuf = NULL;    /* pointer to data to be sent */
    FEC_END_TBD_ID  pTbd = NULL;    /* pointer to the current ready TBD */
    FEC_END_TBD_LIST_ID pTbdList = NULL;/* pointer to the TBD list*/
    UINT16      tbdStatus;  /* holder for the TBD status */
    M_BLK *     pCurr = pMblk;  /* holder for the current mBlk */
    int level;

    FEC_END_LOG (FEC_END_DBG_TX, ("m5200FecPktTransmit(0x%x, 0x%x, %i, 0x%x)...\n"),
                 (int) pDrvCtrl, (int) pMblk, fragNum, pktType, 5, 6);

    /* get the current free TBD */

    pTbdList = m5200FecTbdListSet (pDrvCtrl, fragNum);

    if (pTbdList == (FEC_END_TBD_LIST_ID) NULL)
        {
        /* set to stall condition */

        pDrvCtrl->txStall = TRUE;

        return (ERROR);
        }

    /* store the Mblk pointer */

    pTbdList->pBuf = (UCHAR *) pMblk;
    pTbdList->bufType = BUF_TYPE_MBLK;

    /* store the packet type */

    pTbdList->pktType = pktType;

    pCurr = pMblk;

    /* get the total packet length */

    pktLen = pMblk->mBlkPktHdr.len;

    /* for each mBlk fragment, set up the related TBD */

    while (fragNum > 0)
        {
        pBuf = pCurr->mBlkHdr.mData;

        bufLen = pCurr->mBlkHdr.mLen;

        pCurr = ((M_BLK *) pCurr->mBlkHdr.mNext);

        /*
        * skip fragments that contain no data (SPR 29513).
        * fragments without data were not counted in m5200FecMblkWalk()
        */
        if (bufLen == 0)
            continue;

        /* just in case we have to pad it */

        totLen += bufLen;

        /* flush the cache, if necessary */

        FEC_END_CACHE_FLUSH (pBuf, bufLen);

        /* get and set up the current TBD */

        pTbd = pTbdList->pTbd;

        FEC_END_BD_LONG_WR (pTbd, FEC_END_BD_ADDR_OFF, (UINT32) pBuf);

        FEC_END_BD_WORD_RD (pTbd, FEC_END_BD_STAT_OFF, tbdStatus);

        /*
        * complete TBD construction based on whether or not this is the last
        * fragment
        */
        if (--fragNum > 0)
            {
            FEC_END_LOG (FEC_END_DBG_TX, ("m5200FecPktTransmit more mblks \n"),
                        1, 2, 3, 4, 5, 6);

            FEC_END_BD_WORD_WR (pTbd, FEC_END_BD_LEN_OFF, (UINT32) bufLen);

            FEC_END_BD_WORD_WR (pTbd, FEC_END_BD_STAT_OFF,
                    (tbdStatus | FEC_END_TBD_RDY));
            }
        else
            {
            if (totLen < ETHERSMALL)
            bufLen = max (bufLen, (ETHERSMALL - totLen + bufLen));

            FEC_END_LOG (FEC_END_DBG_TX, ("m5200FecPktTransmit last len=0x%x\n"),
                        bufLen, 2, 3, 4, 5, 6);

            FEC_END_BD_WORD_WR (pTbd, FEC_END_BD_LEN_OFF, (UINT32) bufLen);

            FEC_END_BD_WORD_WR (pTbd, FEC_END_BD_STAT_OFF,
                    (FEC_END_TBD_LAST | FEC_END_TBD_INT | FEC_END_TBD_CRC
                    | FEC_END_TBD_RDY | tbdStatus));
            }

        /* move on to the next element */

        pTbdList = pTbdList->pNext;
        }

    /* kick the transmitter & enable interrupts */

    level = intLock();
#ifdef NO_TX_INT

    if (TaskStart( pDrvCtrl->xmitTaskNo, FALSE, pDrvCtrl->xmitTaskNo, FALSE) != TASK_ERR_NO_ERR)

#else

    if (TaskStart( pDrvCtrl->xmitTaskNo, TASK_AUTOSTART_ENABLE, pDrvCtrl->xmitTaskNo, TRUE) != TASK_ERR_NO_ERR)

#endif
        {
        intUnlock(level);
        FEC_END_LOG (FEC_END_DBG_TX, ("m5200FecPktTransmit: failed to start TX task\n"),
                            1, 2, 3, 4, 5, 6);
        return (ERROR);
        }
    intUnlock(level);

    /* Flush the write pipe */

    CACHE_PIPE_FLUSH ();

    /* up-date statistics */

    if (pktType == PKT_TYPE_MULTI)
        {
        pDrvCtrl->endObj.mib2Tbl.ifOutNUcastPkts += 1;
        }
    else
        {
        END_ERR_ADD (&pDrvCtrl->endObj, MIB2_OUT_UCAST, +1);
        }

    return (OK);
    }
#endif

/******************************************************************************
*
* m5200FecPktCopyTransmit - copy and transmit a packet
*
* This routine transmits the packet described by the given parameters
* over the network, after copying the mBlk to the driver's buffer.
* It also updates statistics.
*
* RETURNS: OK, or ERROR if no resources were available.
*/

LOCAL STATUS m5200FecPktCopyTransmit
    (
    DRV_CTRL *  pDrvCtrl,       /* pointer to DRV_CTRL structure */
    M_BLK *     pMblk,          /* pointer to the mBlk */
    UINT8   fragNum,    /* number of fragments */
    UINT16  pktType     /* packet type */
    )
    {
    int         len = 0;    /* length of data to be sent */
    char *      pBuf = NULL;    /* pointer to data to be sent */
#ifdef WORKAROUND_BESTCOMM_FEC
    static char *   pStartOldBuf = NULL; /* Start of old buffer */
    static char *   pEndOldBuf = NULL;   /* End of old buffer */
#endif
    FEC_END_TBD_ID  pTbd = NULL;    /* pointer to the current ready TBD */
    FEC_END_TBD_LIST_ID pTbdList = NULL;/* pointer to the TBD list*/
    UINT16      tbdStatus;  /* holder for the TBD status */
    int                 gotNetBuffer = FALSE;
    UINT8               txBuffAvailableOnEntry = pDrvCtrl->txBuffAvailable;
    UINT16              thisBufferType = BUF_TYPE_CL;
    STATUS              retVal = OK;
    int level;

    FEC_END_LOG (FEC_END_DBG_TX, ("m5200FecPktCopyTransmit(0x%x, 0x%x, %i, 0x%x)...\n"),
                 (int) pDrvCtrl, (int) pMblk, fragNum, pktType, 5, 6);

    /* get a cluster buffer from the pool in order to transmit this data */

    if ((pBuf = NET_BUF_ALLOC()) != NULL)
        {
        gotNetBuffer = TRUE;
        }

    if ((gotNetBuffer == FALSE) && (pDrvCtrl->txBuffAvailable == TRUE))
        {
        /*
         * To get this packet out, use the local TX buffer
         * This allows us to free a network buffer now which prevents
         * tx network buffer lockup under heavy load condition.
         */

        pDrvCtrl->txBuffAvailable = FALSE;

        pBuf = pDrvCtrl->pTxBuffLocal;
        thisBufferType = BUF_TYPE_LOCAL;
        }

    else if (gotNetBuffer == FALSE)
        {
        retVal = END_ERR_BLOCK;
        goto errorExit;
        }

#ifdef WORKAROUND_BESTCOMM_FEC
    /*
     * WORKAROUND: Bestcomm/SDMA/Buffer Descriptor (pdr)
     *
     * i)  Ensure that the buffer pointer is different to the previous.
     *
     * ii) When the BestComm reads data from the XLB memory bus, it reads it in
     * blocks of 32 bytes. If the BestComm is requesting sequential addresses
     * with lengths of less than 32 bytes, an optimization is utilized which
     * allows the BestComm to do a single access to the XLB memory bus and then
     * read data from one of four Read Line Buffers (RLB) between the BestComm
     * and the XLB. With current silicon a subsequent read of data within the RLB
     * will not result in an access to the SDRAM, hence the data could be stale.
     * So we must ensure that the end of the previous tx buffer and the start
     * of this tx buffer are not within 32 byte boundaries of each other.
     */

    if ((pBuf != pDrvCtrl->pTxBuffLocal) &&
        ((pBuf == pStartOldBuf) ||
        (ROUND_DOWN (pEndOldBuf, 32) == ROUND_DOWN (pBuf, 32))))
        {
        /*
         * IF  this is not the TX local buffer
         * AND (new buffer is equal to old buffer OR end of old buffer is
         *      within the same 32 byte boundary as the start of the new buffer)
         *
         * ----------------------------------------------------
         *
         * Just use the local tx buffer, the size of which will
         * ensure that the Read Line Buffers within Bestcomm are
         * not read (the TX buffer is at least 32 bytes larger
         * than the maximum packet size).
         */

        if (pDrvCtrl->txBuffAvailable == TRUE)
            {
            /* If a buffer was allocated on entry, free it */

            if (gotNetBuffer == TRUE)
                {
                NET_BUF_FREE ((UCHAR *) pBuf);
                gotNetBuffer = FALSE;
                }

            /* To get this packet out, use the local TX buffer */

            pDrvCtrl->txBuffAvailable = FALSE;

            pBuf = pDrvCtrl->pTxBuffLocal;
            thisBufferType = BUF_TYPE_LOCAL;

            FEC_END_NEW_BUFF_ADD;
            }
        else
            {
            retVal = END_ERR_BLOCK;
            goto errorExit;
            }
        }
#endif

    /* get the current free TBD list */


    pTbdList = m5200FecTbdListSet (pDrvCtrl, fragNum);

    if (pTbdList == (FEC_END_TBD_LIST_ID) NULL)
        {
        retVal = END_ERR_BLOCK;
        goto errorExit;
        }

    /* store the packet type */

    pTbdList->pktType = pktType;

    /* store the buffer pointer and its type */

    pTbdList->bufType = thisBufferType;
    pTbdList->pBuf = (UINT8 *)pBuf;

    /* copy data and free the Mblk */

    len = netMblkToBufCopy (pMblk, (char *) pBuf, NULL);
    netMblkClChainFree (pMblk);
    len = max (ETHERSMALL, len);

#ifdef WORKAROUND_BESTCOMM_FEC
    /* Update these for the next packet that we send. */

    pStartOldBuf = pBuf;
    pEndOldBuf = pBuf + len - 1;
#endif

    /* flush the cache, if necessary */

    FEC_END_CACHE_FLUSH (pBuf, len);

    /* get and set up the current TBD */

    pTbd = pTbdList->pTbd;

    FEC_END_LOG (FEC_END_DBG_TX, ("m5200FecPktCopyTransmit: pBuf = 0x%x, pTbd = 0x%x\n"),
                 (int) pBuf, (int) pTbd, 3, 4, 5, 6);

    FEC_END_BD_LONG_WR (pTbd, FEC_END_BD_ADDR_OFF, (UINT32) pBuf);

    FEC_END_BD_WORD_WR (pTbd, FEC_END_BD_LEN_OFF, (UINT32) len);

    FEC_END_BD_WORD_RD (pTbd, FEC_END_BD_STAT_OFF, tbdStatus);

    FEC_END_BD_WORD_WR (pTbd, FEC_END_BD_STAT_OFF,
                        (FEC_END_TBD_LAST | FEC_END_TBD_INT | FEC_END_TBD_CRC |
                         FEC_END_TBD_RDY | tbdStatus));

    /* kick the transmitter & enable interrupts */

    level = intLock();
#ifdef NO_TX_INT

    if (TaskStart( pDrvCtrl->xmitTaskNo, FALSE, pDrvCtrl->xmitTaskNo, FALSE) != TASK_ERR_NO_ERR)

#else

    if (TaskStart( pDrvCtrl->xmitTaskNo, TASK_AUTOSTART_ENABLE, pDrvCtrl->xmitTaskNo, TRUE) != TASK_ERR_NO_ERR)

#endif
        {
        intUnlock(level);
        FEC_END_LOG (FEC_END_DBG_TX, ("m5200FecPktCopyTransmit: failed to start TX task\n"),
                            1, 2, 3, 4, 5, 6);
        retVal = ERROR;
        goto errorExit;
        }
    intUnlock(level);

    /* Flush the write pipe */

    CACHE_PIPE_FLUSH ();

    /* up-date statistics */

    if (pktType == PKT_TYPE_MULTI)
        {
        pDrvCtrl->endObj.mib2Tbl.ifOutNUcastPkts += 1;
        }
    else
        {
        END_ERR_ADD (&pDrvCtrl->endObj, MIB2_OUT_UCAST, +1);
        }

    FEC_END_LOG (FEC_END_DBG_TX, ("m5200FecPktCopyTransmit: done\n"), 1, 2, 3, 4, 5, 6);

    return (OK);

errorExit:
    /* On error - undo anything that we did on entry */

    pDrvCtrl->txBuffAvailable = txBuffAvailableOnEntry;

    if (gotNetBuffer == TRUE)
       {
       NET_BUF_FREE ((UCHAR *) pBuf);
       }

    return (retVal);
    }

/******************************************************************************
*
* m5200FecMblkWalk - walk the mBlk
*
* This routine walks the mBlk whose address is in <pMblk>, computes
* the number of fragments it is made of, and returns it in the parameter
* <pFragNum>. In addition, it finds out whether the specified packet
* is unicast or multicast, and sets <pPktType> accordingly.
*
* RETURNS: OK, or ERROR in case of invalid mBlk.
*/

LOCAL STATUS m5200FecMblkWalk
    (
    M_BLK *     pMblk,          /* pointer to the mBlk */
    UINT8 * pFragNum,   /* number of fragments */
    UINT16 *    pPktType    /* packet type */
    )
    {
    M_BLK *     pCurr = pMblk;      /* the current mBlk */

    if (pMblk == NULL)
        {
        FEC_END_LOG (FEC_END_DBG_TX, ("m5200FecSend: invalid pMblk\n"),
                                      0, 0, 0, 0, 0, 0);
        return (ERROR);
        }

    /* walk this mBlk */

    while (pCurr != NULL)
        {
        /*
        * keep track of the number of fragments in this mBlk.
        * Do not count fragments with no data (SPR 29513)
        */

        if (pCurr->mBlkHdr.mLen != 0)
            (*pFragNum)++;

        pCurr = ((M_BLK *) pCurr->mBlkHdr.mNext);
        }

    /* set the packet type to multicast or unicast */

    if (pMblk->mBlkHdr.mData[0] & (UINT8) 0x01)
        {
        (*pPktType) = PKT_TYPE_MULTI;
        }
    else
        {
        (*pPktType) = PKT_TYPE_UNI;
        }

    return (OK);
    }

/******************************************************************************
*
* m5200FecTbdListSet - set up a list of available transmit descriptors
*
* This routine sets up a list of available buffer descriptors for use by
* the send routine.
*
* RETURNS: a pointer to a transmit descriptors list, or NULL.
*/

LOCAL FEC_END_TBD_LIST_ID m5200FecTbdListSet
    (
    DRV_CTRL *  pDrvCtrl,       /* pointer to DRV_CTRL structure */
    UINT8   tbdNum      /* number of tbd */
    )
    {
    FEC_END_TBD_ID  pTbd = NULL;        /* pointer to the current TBD */
    UINT16      tbdIndex = pDrvCtrl->tbdIndex;  /* TBD index */
    UINT16      nextIndex = 0;      /* index to the next TBD */
    UINT16      currIndex = 0;      /* index to the current TBD */
    UINT16      i = 0;          /* an index */
    FEC_END_TBD_LIST_ID pTbdList = NULL;

    /* set the number of TBDs in this list */

    pDrvCtrl->pTbdList [tbdIndex]->fragNum = tbdNum;

    /* initialize the list */

    for (i = 0; i < tbdNum; i++)
        {

        /* get the current free TBD */

        if ((pTbd = m5200FecTbdGet (pDrvCtrl)) == NULL)
            {
            return (NULL);
            }

        currIndex = pDrvCtrl->tbdIndex;
        nextIndex = (currIndex + 1) % (pDrvCtrl->tbdNum);

        pTbdList = pDrvCtrl->pTbdList [currIndex];

        pTbdList->pTbd = pTbd;
        pTbdList->pBuf = NULL;

        pTbdList->pNext = pDrvCtrl->pTbdList [nextIndex];


        /* update some indeces for a correct handling of the TBD ring */

        pDrvCtrl->tbdIndex = (pDrvCtrl->tbdIndex + 1) % pDrvCtrl->tbdNum;
        }

    /* terminate the list properly */

    pTbdList->pNext = NULL;

    /* keep track of the clean TBDs */

    pDrvCtrl->cleanTbdNum -= tbdNum;

    return (pDrvCtrl->pTbdList [tbdIndex]);
    }

/******************************************************************************
*
* m5200FecTbdGet - get the first available transmit descriptor
*
* This routine gets the first available buffer descriptor in the transmit
* ring. It checks the descriptor is owned by the host and it has been
* cleaned by the m5200FecTbdClean() routine.
*
* RETURNS: an available transmit descriptor, otherwise NULL.
*/

LOCAL FEC_END_TBD_ID m5200FecTbdGet
    (
    DRV_CTRL *  pDrvCtrl       /* pointer to DRV_CTRL structure */
    )
    {
    FEC_END_TBD_ID  nextTbd = NULL; /* holder for the next TBD */
    UINT16  tbdStatus;  /* holder for the TBD status */

    FEC_END_NEXT_TBD (pDrvCtrl, nextTbd);

    /* Make cache consistent with memory */

    FEC_END_BD_CACHE_INVAL (nextTbd, FEC_END_TBD_SZ);

    FEC_END_BD_WORD_RD ((char *) nextTbd, FEC_END_BD_STAT_OFF, tbdStatus);

    if ((tbdStatus & FEC_END_TBD_RDY) == FEC_END_TBD_RDY)
        {
        FEC_END_LOG (FEC_END_DBG_TX, ("m5200FecSend... not our Tbd \n"),
                        1, 2, 3, 4, 5, 6);
        return (NULL);
        }

    return ((FEC_END_TBD_ID) nextTbd);
    }

/******************************************************************************
*
* m5200FecTbdClean - clean the transmit buffer descriptors ring
*
* This routine is run in netTask's context. It cleans the transmit
* buffer descriptors ring. It also calls m5200FecTbdCheck() to check
* for any transmit errors, and possibly up-dates statistics.
*
* SEE ALSO: m5200FecTbdCheck()
*
* RETURNS: N/A.
*/

LOCAL void m5200FecTbdClean
    (
    DRV_CTRL *  pDrvCtrl       /* pointer to DRV_CTRL structure */
    )
    {
    FEC_END_TBD_ID  pUsedTbd;       /* first used TBD */
    BOOL        tbdError = FALSE;   /* no error reported */
    UINT32      retVal = 0;     /* return value */

    FEC_END_LOG (FEC_END_DBG_TX, ("m5200FecTbdClean\n"), 1, 2, 3, 4, 5, 6);

    END_TX_SEM_TAKE (&pDrvCtrl->endObj, WAIT_FOREVER);

    /* process all the consumed TBDs */

    while (pDrvCtrl->cleanTbdNum < pDrvCtrl->tbdNum)
        {
        FEC_END_TBD_LIST_ID pTbdList;
        FEC_END_TBD_LIST_ID pTempList;

        pTbdList = pDrvCtrl->pTbdList [pDrvCtrl->usedTbdIndex];
        pTempList = pTbdList;

        /* first, we detect abnormal events */

        while (pTempList != NULL)
            {
            pUsedTbd = pTempList->pTbd;

            /* check on this TBD */

            retVal = m5200FecTbdCheck (pDrvCtrl, pUsedTbd);

            if (retVal == FEC_END_TBD_BUSY)
                {
                END_TX_SEM_GIVE (&pDrvCtrl->endObj);
                return;
                }

            if (retVal == FEC_END_TBD_ERROR)
                {
                tbdError = TRUE;
                break;
                }

            /* move on to the next element in the list */

            pTempList = pTempList->pNext;
            }

        /*
         * free the Mblk or the cluster, whichever is appropriate
         * For BUF_TYPE_LOCAL, just make it available again.
         */

        if (pTbdList->bufType == (UINT16) BUF_TYPE_LOCAL)
            {
            /* mark the local tx buffer available */
            pDrvCtrl->txBuffAvailable = TRUE;

            pTbdList->pBuf = NULL;
            }

            else if ((pTbdList->bufType == ((UINT16) BUF_TYPE_MBLK)) &&
                    (pTbdList->pBuf != NULL))
            {
            netMblkClChainFree ((M_BLK *) pTbdList->pBuf);

            pTbdList->pBuf = NULL;
            }
        else if ((pTbdList->bufType == ((UINT16) BUF_TYPE_CL)) &&
                (pTbdList->pBuf != NULL))
            {
            /* it's a cluster */

            NET_BUF_FREE (pTbdList->pBuf);

            pTbdList->pBuf = NULL;
            }
        else
            {
            FEC_END_TX_FREE_ERROR;
            }

        /* correct statistics, if necessary */

        if (tbdError == TRUE)
            {
            END_ERR_ADD (&pDrvCtrl->endObj, MIB2_OUT_ERRS, +1);

            if (pTbdList->pktType == PKT_TYPE_MULTI)
                {
                pDrvCtrl->endObj.mib2Tbl.ifOutNUcastPkts -= 1;
                }
            else
                {
                END_ERR_ADD (&pDrvCtrl->endObj, MIB2_OUT_UCAST, -1);
                }
            tbdError = FALSE;
            }

        pTempList = pTbdList;

        /* finally, we clean this TBD list */

        while (pTempList != NULL)
            {
            pUsedTbd = pTempList->pTbd;

            /* clean this buffer descriptor, mirror m5200FecTbdInit() */

			FEC_END_BD_WORD_WR (pUsedTbd, FEC_END_BD_STAT_OFF, 0);

            /* update some indeces for a correct handling of the TBD ring */

            pDrvCtrl->cleanTbdNum++;
            pDrvCtrl->usedTbdIndex = (pDrvCtrl->usedTbdIndex + 1) %
                                    (pDrvCtrl->tbdNum);

            /* move on to the next element in the list */

            pTempList = pTempList->pNext;
            }

        /* soon notify upper protocols TBDs are available */

        if (pDrvCtrl->txStall)
            {
            FEC_END_LOG (FEC_END_DBG_TX, ("Restart mux \n"),
                        0, 0, 0, 0, 0, 0);

            (void) netJobAdd ((FUNCPTR) muxTxRestart,
                    (int) &pDrvCtrl->endObj,
                    0, 0, 0, 0);

            pDrvCtrl->txStall = FALSE;
            }
        }

    END_TX_SEM_GIVE (&pDrvCtrl->endObj);

    FEC_END_LOG (FEC_END_DBG_TX, ("m5200FecTbdClean...Done\n"),
                  1, 2, 3, 4, 5, 6);
    }

#if 0

/*
 * This function is not currently used and so it is removed from the build.
 */

/******************************************************************************
*
* m5200FecTxRestart - Restart the transmit side
*
* This routine is run in netTask's context to restart the TX side of the driver.
*
* RETURNS: N/A.
*/

LOCAL void m5200FecTxRestart
    (
    DRV_CTRL * pDrvCtrl
    )
    {
    BDIdx freeIdx; /* holder for TaskBDReset return value */
    int retValBc; /* holder for Bestcomm API return values */
    int level;

    FEC_END_LOG (FEC_END_DBG_RESTART, ("m5200FecTxRestart\n"), 1, 2, 3, 4, 5, 6);

    END_TX_SEM_TAKE (&pDrvCtrl->endObj, WAIT_FOREVER);

    /* Reclaim network buffers from the TXBD ring. */

    m5200FecTbdReclaim (pDrvCtrl);

    /* Reset the TBD ring data variables */

    pDrvCtrl->tbdIndex = 0;
    pDrvCtrl->usedTbdIndex = 0;
    pDrvCtrl->cleanTbdNum = pDrvCtrl->tbdNum;

    /* Clear out any existing pending interrupts from Bestcomm */

    level = intLock();
    TaskIntClear (pDrvCtrl->xmitTaskNo);
    intUnlock(level);

    /*
     * Reset the Bestcomm TBD ring
     * And verify the return values is as we expect (0) which help prevent
     * problems arising from future bestcomm API updates.
     */

    freeIdx = TaskBDReset (pDrvCtrl->xmitTaskNo);

    /* Simple check to ensure the API doesn't change in future updates */

    if (freeIdx != 0)
        {
        FEC_END_LOG (FEC_END_DBG_ERR, ("m5200FecTxRestart: TaskBDReset: %d\n"),
                     freeIdx, 2, 3, 4, 5, 6);
        }

    /* Restart the TX Bestcomm task */

    level = intLock();
    retValBc = TaskStart (pDrvCtrl->xmitTaskNo, TASK_AUTOSTART_ENABLE, pDrvCtrl->xmitTaskNo, TRUE);
    intUnlock(level);

    if (retValBc != TASK_ERR_NO_ERR)
        {
        FEC_END_LOG (FEC_END_DBG_ERR, ("m5200FecTxRestart error: TaskStart %d\n"),
                     retValBc, 2, 3, 4, 5, 6);
        }

    /* Notify upper protocols that TBDs are now available */

    if (pDrvCtrl->txStall)
        {
        FEC_END_LOG (FEC_END_DBG_TX,
                     ("m5200FecTxRestart: Restart mux\n"), 1, 2, 3, 4, 5, 6);

        (void) netJobAdd ((FUNCPTR) muxTxRestart, (int) &pDrvCtrl->endObj, 0, 0, 0, 0);

        pDrvCtrl->txStall = FALSE;
        }

    END_TX_SEM_GIVE (&pDrvCtrl->endObj);

    /* Reenable the FEC */

    FEC_END_ETH_ENABLE;
    }
#endif

/******************************************************************************
*
* m5200FecTbdReclaim - Reclaim the transmit buffer descriptors ring
*
* This routine is run in netTask's context. It reclaims the network buffers on
* the transmit buffer descriptors ring.
*
* NOTE: The caller must own the TX Semaphore.
*
* RETURNS: N/A.
*/

LOCAL void m5200FecTbdReclaim
    (
    DRV_CTRL * pDrvCtrl       /* pointer to DRV_CTRL structure */
    )
    {
    FEC_END_TBD_LIST_ID pTbdList;
    int n = 0; /* Limit times around loop - i.e. make sure we do exit. */

    FEC_END_LOG (FEC_END_DBG_RESTART, ("m5200FecTbdReclaim\n"), 1, 2, 3, 4, 5, 6);

    /* reclaim any buffers assigned to TBDs */

    while ((pDrvCtrl->cleanTbdNum < pDrvCtrl->tbdNum) && (n <= pDrvCtrl->tbdNum))
        {
        pTbdList = pDrvCtrl->pTbdList [pDrvCtrl->usedTbdIndex];

        /* free the Mblk or the cluster, whichever is appropriate */

        if (pTbdList->bufType == (UINT16) BUF_TYPE_LOCAL)
            {
            /* mark the local tx buffer available */
            pDrvCtrl->txBuffAvailable = TRUE;
            }

        else if ((pTbdList->bufType == ((UINT16) BUF_TYPE_MBLK)) &&
                 (pTbdList->pBuf != NULL))
            {
            /* it's an mblk */

            netMblkClChainFree ((M_BLK *) pTbdList->pBuf);

            pTbdList->pBuf = NULL;
            }

        else if ((pTbdList->bufType == ((UINT16) BUF_TYPE_CL)) &&
                 (pTbdList->pBuf != NULL))
            {
            /* it's a cluster */

            NET_BUF_FREE (pTbdList->pBuf);

            pTbdList->pBuf = NULL;
            }

        else
            {
            FEC_END_LOG (FEC_END_DBG_ERR,
                        ("m5200FecTbdReclaim. Error - unknown bufType %d 0x%08x\n"),
                        pTbdList->bufType, (int)pTbdList->pBuf, 3, 4, 5, 6);
            }


        /* finally, we clean this TBD list (handle multiple fragments) */

        while (pTbdList != NULL)
            {
            /* update some indeces for a correct handling of the TBD ring */

            pDrvCtrl->cleanTbdNum++;

            /* Allow for wrap around */

            pDrvCtrl->usedTbdIndex =
                (pDrvCtrl->usedTbdIndex + 1) % (pDrvCtrl->tbdNum);

            /* move on to the next element in the list */

            pTbdList = pTbdList->pNext;
            }

        n++; /* Maximum number of iterations is tbdNum */
        }

    /*
     * Simple check to ensure above loop exits as intended.
     * Normally tbdNum should not be exceeded.
     */

    if (n == pDrvCtrl->tbdNum)
        {
        /* We got out on an error limit - this should not occur */
        FEC_END_LOG (FEC_END_DBG_ERR,
                     ("m5200FecTbdReclaim Error: cleanTbdNum %d, tbdNum %d\n"),
                     pDrvCtrl->cleanTbdNum, pDrvCtrl->tbdNum, 3, 4, 5, 6);

        }

    FEC_END_LOG (FEC_END_DBG_RESTART, ("m5200FecTbdReclaim done...\n"),
                 1, 2, 3, 4, 5, 6);
    }

/******************************************************************************
*
* m5200FecTbdCheck - check the transmit buffer descriptor
*
* This routine checks for any transmit errors by looking at the
* descriptor status.
*
* SEE ALSO: m5200FecTbdClean()
*
* RETURNS: FEC_END_TBD_OK, or FEC_END_TBD_BUSY if the specified TBD is
* not ready, or FEC_END_TBD_ERROR in case of errored TBD.
*/

LOCAL UINT32 m5200FecTbdCheck
    (
    DRV_CTRL *      pDrvCtrl,       /* pointer to DRV_CTRL structure */
    FEC_END_TBD_ID  pUsedTbd    /* holder for the first used TBD */
    )
    {
    UINT16  tbdStatus;  /* holder for the TBD status */

    /* Make cache consistent with memory */

    FEC_END_BD_CACHE_INVAL (pUsedTbd, FEC_END_TBD_SZ);

    FEC_END_BD_WORD_RD ((char *) pUsedTbd, FEC_END_BD_STAT_OFF, tbdStatus);

    /* if it's not ready, don't touch it! */

    if ((tbdStatus & FEC_END_TBD_RDY) == FEC_END_TBD_RDY)
        {
        return (FEC_END_TBD_BUSY);
        }


    return (FEC_END_TBD_OK);
    }

/**************************************************************************
*
* m5200FecTbdInit - initialize the transmit buffer ring
*
* This routine initializes the transmit buffer descriptors ring.
*
* SEE ALSO: m5200FecRbdInit()
*
* RETURNS: OK, always.
*/

LOCAL STATUS m5200FecTbdInit
    (
    DRV_CTRL *  pDrvCtrl        /* pointer to DRV_CTRL structure */
    )
    {
    UINT16      ix;     /* a counter */
    FEC_END_TBD_ID  pTbd;       /* generic TBD pointer */

    /* carve up the shared-memory region */

    /* there are four pointers in front as used by the SDMA tasks */

    pDrvCtrl->tbdBase = pTbd = (FEC_END_TBD_ID)TaskGetBDRing(pDrvCtrl->xmitTaskNo);

    for (ix = 0; ix < pDrvCtrl->tbdNum; ix++)
        {
        /*
        * put a fragment of each frame in a single buffer and
        * generate the 4-byte CRC at the end of each frame.
        * Do not make it ready for transmission yet.
        */

        FEC_END_BD_WORD_WR (pTbd, FEC_END_BD_STAT_OFF, 0);

        pTbd += FEC_END_TBD_SZ;

        /* get memory for the TBD list */

        pDrvCtrl->pTbdList [ix] = (FEC_END_TBD_LIST_ID)
                    calloc (sizeof (FEC_END_TBD_LIST), 1);
        }

    /* have the last TBD to close the ring */

    pTbd -= FEC_END_TBD_SZ;
    FEC_END_BD_WORD_WR (pTbd, FEC_END_BD_STAT_OFF, (0));

    /* get a cluster buffer from the pool */

    pDrvCtrl->pTxPollBuf = (UINT8 *)NET_BUF_ALLOC();

    return (OK);
    }

/******************************************************************************
*
* m5200FecRxHandle - service task-level interrupts for rx frames
*
* This routine is run in netTask's context.  The ISR scheduled
* this routine so that it could handle receive and transmit packets
* at task level.
*
* RETURNS: N/A
*/

LOCAL void m5200FecRxHandle
    (
    DRV_CTRL *  pDrvCtrl    /* pointer to DRV_CTRL structure */
    )
    {
    int         intLevel;   /* current intr level */

    FEC_END_LOG (FEC_END_DBG_RX, ("m5200FecRxHandle: \n"),
                  0, 0, 0, 0, 0, 0);

    /* while rx interrupts to be processed */

    m5200FecRxProcess:

    m5200FecHandleRecvInt (pDrvCtrl);

    intLevel = intLock();

    /*
     * This piece of code is to deal with any events that have occurred
     * since the events were masked off in the ISR. These events will
     * have occurred but no interrupt would have been generated.
     */

    if (TaskIntStatus( pDrvCtrl->recvTaskNo ) == pDrvCtrl->recvTaskNo)
        {
        TaskIntClear( pDrvCtrl->recvTaskNo );
        intUnlock (intLevel);
        goto m5200FecRxProcess;
        }
    else
        {

        /* re-enable interrupts */

#ifndef NO_RX_INT
        SDMA_INT_ENABLE( SDMA_INT_MASK, pDrvCtrl->recvTaskNo );
#endif

        intUnlock (intLevel);
        }
    }

/******************************************************************************
*
* m5200FecTxHandle - service task-level interrupts for tx frames
*
* This routine is run in netTask's context.  The ISR scheduled
* this routine so that it could handle receive and transmit packets
* at task level.
*
* RETURNS: N/A
*/

LOCAL void m5200FecTxHandle
    (
    DRV_CTRL *  pDrvCtrl    /* pointer to DRV_CTRL structure */
    )
    {
    int         intLevel;   /* current intr level */

    FEC_END_LOG (FEC_END_DBG_RX, ("m5200FecTxHandle: \n"),
                  0, 0, 0, 0, 0, 0);

    /* while tx/rx interrupts to be processed */

    m5200FecTxProcess:

    m5200FecTbdClean (pDrvCtrl);

    intLevel = intLock();

    /*
     * This piece of code is to deal with any events that have occurred
     * since the events were masked off in the ISR. These events will
     * have occurred but no interrupt would have been generated.
     */

    if (TaskIntStatus( pDrvCtrl->xmitTaskNo ) == pDrvCtrl->xmitTaskNo)
        {
        TaskIntClear( pDrvCtrl->xmitTaskNo );
        intUnlock (intLevel);
        goto m5200FecTxProcess;
        }
    else
        {
        /* re-enable interrupts */

#ifndef NO_TX_INT
        SDMA_INT_ENABLE( SDMA_INT_MASK, pDrvCtrl->xmitTaskNo );
#endif

        intUnlock (intLevel);
        }
    }

/******************************************************************************
*
* m5200FecHandleRecvInt - service task-level interrupts for receive frames
*
* This routine is run in netTask's context. It is called by m5200FecRxTxHandle()
* so that it could handle receive packets at task level.
*
* RETURNS: N/A
*/

LOCAL void m5200FecHandleRecvInt
    (
    DRV_CTRL *  pDrvCtrl    /* pointer to DRV_CTRL structure */
    )
    {
    FEC_END_RBD_ID  pUsedRbd;   /* a used rbd pointer */
    UINT16      rbdStatus = 0;  /* holder for the status of this rbd */

    FEC_END_LOG (FEC_END_DBG_RX, ("m5200FecHandleRecvInt: \n"),
                  0, 0, 0, 0, 0, 0);

    /* while RBDs to be processed */

    do
        {
        /* get the first used RBD */

        FEC_END_NEXT_RBD (pDrvCtrl, pUsedRbd);

        /* Make cache consistent with memory */

        FEC_END_BD_CACHE_INVAL (pUsedRbd, FEC_END_RBD_SZ);

        /* read the RBD status word */

        FEC_END_BD_WORD_RD (pUsedRbd, FEC_END_BD_STAT_OFF, rbdStatus);

            /* if it's not ready, don't touch it! */

        if ((rbdStatus & FEC_END_RBD_EMPTY) == FEC_END_RBD_EMPTY)
            {
            break;
            }

        /* process this RBD */

        m5200FecReceive (pDrvCtrl, pUsedRbd);

        } while (TRUE);

    /* Flush the write pipe */

    CACHE_PIPE_FLUSH ();

    FEC_END_LOG (FEC_END_DBG_RX, ("m5200FecHandleRecvInt... Done, \n"),
                  0, 0, 0, 0, 0, 0);
    }

/******************************************************************************
*
* m5200FecBusMonitor - main body of bus monitor task.
*
* This routine checks the line status of cable. If a transition of link down to 
* up found, call m5200FecRelink. If a transition of link up to down found, call
* m5200FecDelink.
*
* RETURNS: N/A
*/

LOCAL void m5200FecBusMonitor
    (
    DRV_CTRL *  pDrvCtrl    /* pointer to DRV_CTRL structure */
    )
    {
    	
    static BOOL linkdown = TRUE;
    static BOOL linkStatusChange = FALSE;
    
    STATUS  status;
    
    status = m5200FecMiiBasicCheck(pDrvCtrl, pDrvCtrl->phyInfo->phyAddr);
    
    if(status == ERROR) 
    {
    	FEC_END_LOG (FEC_END_DBG_MII, ("m5200FecBusMonitor.basicchaeck ERROR 0X%x.\n"),
                  ERROR, 0, 0, 0, 0, 0);
      /*logMsg("busmonitor bcheck error 0x%x\n",ERROR,0,0,0,0,0);*/
    	return;
    }
    if( status != OK)
        {
        if(linkdown == TRUE)
           linkStatusChange = FALSE;
        else
           linkStatusChange = TRUE; 
           
    	  linkdown = TRUE;
        }
    else
        {
        if(linkdown == FALSE)
           linkStatusChange = FALSE;
        else
           linkStatusChange = TRUE;
           
    	  linkdown = FALSE;
        }
    FEC_END_LOG (FEC_END_DBG_MII, ("m5200FecBusMonitor. linkdown=%d.\n"),
                  linkdown, 0, 0, 0, 0, 0);
    /*logMsg("busmonitor linkdown=0x%x\n",linkdown,0,0,0,0,0);*/

                  
    if((linkStatusChange == TRUE)&&(linkdown == FALSE))
        m5200FecRelink(pDrvCtrl);
    if((linkStatusChange == TRUE)&&(linkdown == TRUE))
        m5200FecDelink(pDrvCtrl);
  
    }

/******************************************************************************
*
* m5200FecRelink - Reinitialize routine for cable attached  
*
* This routine only modify the TX_CNTL/RX_CNTL regs, and update flags.
*
* RETURNS: N/A
*/

LOCAL STATUS m5200FecRelink
    (
    DRV_CTRL *  pDrvCtrl    /* pointer to DRV_CTRL structure */
    )
    {
    
    FEC_END_LOG (FEC_END_DBG_START, ("Relinking end...\n"),1, 2, 3, 4, 5, 6);
 
    if (pDrvCtrl->phyInfo->phyFlags & FEC_END_PHY_AUTO)
        {
        if (m5200FecMiiAnRun (pDrvCtrl, pDrvCtrl->phyInfo->phyAddr) != OK)
            FEC_END_LOG (FEC_END_DBG_START, ("Relinking an failed.\n"), 1, 2, 3, 4, 5, 6);
        }
    else if (m5200FecMiiModeForce (pDrvCtrl, pDrvCtrl->phyInfo->phyAddr) != OK)
        m5200FecMiiDefForce (pDrvCtrl, pDrvCtrl->phyInfo->phyAddr);
        
	if (m5200FecPostPhyConfig (pDrvCtrl) == ERROR)
        return (ERROR);

    /* correct the speed for the mib2 stuff */

    pDrvCtrl->endObj.mib2Tbl.ifSpeed = pDrvCtrl->phyInfo->phySpeed;

    if (END_FLAGS_ISSET (IFF_MULTICAST))
        FEC_END_FLAG_SET (FEC_END_MCAST);

    END_FLAGS_SET (&pDrvCtrl->endObj, (IFF_UP | IFF_RUNNING));

    return (OK); 
    }

/******************************************************************************
*
* m5200FecDelink - uninitialize routine for cable detached 
*
* This routine only update flags.
*
* RETURNS: OK / ERROR
*/

LOCAL STATUS  m5200FecDelink
    (
    DRV_CTRL *  pDrvCtrl    /* pointer to DRV_CTRL structure */
    )
    {
    
    /* make sure driver is operational before stopping it */

    if (END_FLAGS_GET(&pDrvCtrl->endObj) & (IFF_UP | IFF_RUNNING))
        {
				
        /* mark the interface as down */

        END_FLAGS_CLR (&pDrvCtrl->endObj, (IFF_UP | IFF_RUNNING));

        /* issue a graceful transmit command */

        FEC_END_CSR_WR (FEC_END_TX_CTRL_OFF, FEC_END_TX_CTRL_GRA);

        /* wait for the related interrupt - with Timeout */

        FEC_END_GRA_SEM_TAKE;

        }

    FEC_END_LOG (FEC_END_DBG_LOAD, ("m5200FecDelink... Done \n"),
                    1, 2, 3, 4, 5, 6);

    return OK;     
    }


#if 0

/*
 * m5200FecRxRestart not currently called from the driver so it's removed from
 * the build.
 */

/******************************************************************************
*
* m5200FecRxRestart - Restart the RX
*
* This routine is run in netTask's context.
*
* RETURNS: N/A
*/

LOCAL void m5200FecRxRestart
    (
    DRV_CTRL *  pDrvCtrl    /* pointer to DRV_CTRL structure */
    )
    {
    int ix;
    int level;

    FEC_END_LOG (FEC_END_DBG_RX, ("m5200FecRxRestart: \n"), 0, 0, 0, 0, 0, 0);

    /* reset PTR */

    pDrvCtrl->rbdIndex = 0;

    for (ix = 0; ix < pDrvCtrl->rbdNum; ix++)
        {
        if (pDrvCtrl->rxBuf [ix] != NULL)
            {
            NET_BUF_FREE (pDrvCtrl->rxBuf [ix]);
            }
        else
            {
            FEC_END_LOG (FEC_END_DBG_ERR,
                        ("m5200FecRxRestart: Freeing buffer: %d\n"),
                         ix, 0, 0, 0, 0, 0);
            }
        }

    pDrvCtrl->rbdIndex = TaskBDReset (pDrvCtrl->recvTaskNo);

    /* Error check - must get 0 back from this function */

    if (pDrvCtrl->rbdIndex != 0)
        {
        FEC_END_LOG (FEC_END_DBG_ERR, ("m5200FecRxRestart: TaskBDReset: %d\n"),
            pDrvCtrl->rbdIndex, 2, 3, 4, 5, 6);
        }

    /* restart receiver task */
#ifdef NO_RX_INT
    level = intLock();
    TaskIntClear( pDrvCtrl->recvTaskNo );
    intUnlock(level);

    if ((pDrvCtrl->rxPollTID =
            taskSpawn( "tFecRxPoll", 30, 0, 0x1000, m5200FecRxPoll,
                        pDrvCtrl, 2,3,4,5,6,7,8,9,0)) == ERROR)
        {
        FEC_END_LOG (FEC_END_DBG_LOAD,
                     ("m5200FecRxRestart: failed to spawn tFecRxPoll task\n"),
                     1, 2, 3, 4, 5, 6);
        }

    level = intLock();
    if (TaskStart( pDrvCtrl->recvTaskNo, TASK_AUTOSTART_ENABLE, pDrvCtrl->recvTaskNo, FALSE) != TASK_ERR_NO_ERR)

#else

    level = intLock();
    if (TaskStart( pDrvCtrl->recvTaskNo, TASK_AUTOSTART_ENABLE, pDrvCtrl->recvTaskNo, TRUE) != TASK_ERR_NO_ERR)
#endif
        {
        intUnlock(level);
        FEC_END_LOG (FEC_END_DBG_RX,
                     ("m5200FecRxRestart: failed to restart RX task\n"),
                     1, 2, 3, 4, 5, 6);
        }
    intUnlock(level);

    /* Configure the RX buffer descriptor ring */

    m5200FecRbdInit (pDrvCtrl);

    /* reenable FEC */

    FEC_END_ETH_ENABLE;

    }
#endif

/******************************************************************************
*
* m5200FecReceive - pass a received frame to the next layer up
*
* This routine hands the packet off to the upper layer after cleaning the
* RBD that was used for its reception.
*
* RETURNS: N/A
*/

LOCAL void m5200FecReceive
    (
    DRV_CTRL *      pDrvCtrl,       /* pointer to DRV_CTRL structure */
    FEC_END_RBD_ID      pUsedRbd        /* pointer to a RBD */
    )
    {
    char *  pData = NULL;   /* a rx data pointer */
    char *  pBuf = NULL;    /* a rx data pointer */
    M_BLK_ID    pMblk  = NULL;  /* mBlock pointer */
    CL_BLK_ID   pClBlk = NULL;  /* cluster block pointer */
    UINT16  rbdStatus = 0;  /* holder for the status of this RBD */
    UINT16  rbdLen = 0; /* number of bytes received by this RBD */
    void *  retVal = NULL;  /* convenient holder for return value */

    /* pass the packet up only if reception was Ok */

    FEC_END_BD_WORD_RD (pUsedRbd, FEC_END_BD_STAT_OFF, rbdStatus);

    FEC_END_LOG (FEC_END_DBG_RX,
                 ("m5200FecReceive rbdIndex=%d status=0x%x\n"),
                  pDrvCtrl->rbdIndex, rbdStatus, 0, 0, 0, 0);

    if (rbdStatus & FEC_END_RBD_ERR)
    {
    FEC_END_LOG (FEC_END_DBG_RX, ("m5200FecReceive: bad rbd\n"),
                      1, 2, 3, 4, 5, 6);

        END_ERR_ADD (&pDrvCtrl->endObj, MIB2_IN_ERRS, +1);

#ifdef FEC_END_DBG
    if ((rbdStatus & FEC_END_RBD_FEMPTY) == FEC_END_RBD_FEMPTY)
        {
        FEC_END_LOG (FEC_END_DBG_RX, ("m5200FecReceive: FIFO Empty\n"),
                      1, 2, 3, 4, 5, 6);
        FEC_END_RX_FEMPTY_ADD;
        }

    if ((rbdStatus & FEC_END_RBD_ALARM) == FEC_END_RBD_ALARM)
        {
        FEC_END_LOG (FEC_END_DBG_RX, ("m5200FecReceive: FIFO Alarm\n"),
                      1, 2, 3, 4, 5, 6);
        FEC_END_RX_ALARM_ADD;
        }

    if ((rbdStatus & FEC_END_RBD_FULL) == FEC_END_RBD_FULL)
        {
        FEC_END_LOG (FEC_END_DBG_RX, ("m5200FecReceive: FIFO Full\n"),
                      1, 2, 3, 4, 5, 6);
        FEC_END_RX_FULL_ADD;
        }

    if ((rbdStatus & FEC_END_RBD_UF) == FEC_END_RBD_UF)
        {
        FEC_END_LOG (FEC_END_DBG_RX, ("m5200FecReceive: FIFO Underflow\n"),
                      1, 2, 3, 4, 5, 6);
        FEC_END_RX_UF_ADD;
        }

    if ((rbdStatus & FEC_END_RBD_OV) == FEC_END_RBD_OV)
        {
        FEC_END_LOG (FEC_END_DBG_RX, ("m5200FecReceive: rx overrun\n"),
                      1, 2, 3, 4, 5, 6);
        FEC_END_RX_OV_ADD;
        }

#endif /* FEC_END_DBG */

    /* put the errored RBD on the RBD queue */

    m5200FecRbdClean (pDrvCtrl, pBuf);

    return;
    }

    /* get the actual amount of received data */

    FEC_END_BD_WORD_RD (pUsedRbd, FEC_END_BD_LEN_OFF,
            rbdLen);

    FEC_END_LOG (FEC_END_DBG_RX, ("m5200FecReceive rbdLen=%d\n"),
                  rbdLen, 0, 0, 0, 0, 0);

    if (rbdLen < ETHERSMALL)
        {
        FEC_END_RX_LS_ADD;

        /* put the errored RBD on the RBD queue */

        m5200FecRbdClean (pDrvCtrl, pBuf);

        return;
        }

    if (rbdLen > FEC_END_MAX_PCK_SZ)
        {
        FEC_END_RX_LS_ADD;

        /* put the errored RBD on the RBD queue */

        m5200FecRbdClean (pDrvCtrl, pBuf);

        return;
        }

    /* Allocate an MBLK, and a replacement buffer */

    pMblk = NET_MBLK_ALLOC();
    pBuf = NET_BUF_ALLOC ();
    pClBlk = NET_CL_BLK_ALLOC();

    if ((pMblk == NULL) || (pBuf == NULL) ||
    (pClBlk == NULL))
        {
        FEC_END_RX_MEM_ADD;

        FEC_END_LOG (FEC_END_DBG_RX, ("m5200FecReceive mem problem\n"),
                        0, 0, 0, 0, 0, 0);

        goto m5200FecRecvError;
        }

    FEC_END_BD_LONG_RD (pUsedRbd, FEC_END_BD_ADDR_OFF, pData);

    NET_CL_BLK_JOIN (pClBlk, (char *) (pDrvCtrl->rxBuf [pDrvCtrl->rbdIndex]),
                     FEC_END_MAX_CL_LEN, retVal);

    if (retVal == NULL)
        {
        goto m5200FecRecvError;
        }

    NET_MBLK_CL_JOIN(pMblk, pClBlk, retVal);
    if (retVal == NULL)
        {
        goto m5200FecRecvError;
        }

    /* set up the mBlk properly */

    pMblk->mBlkHdr.mFlags |= M_PKTHDR;
    pMblk->mBlkHdr.mData = pData;
    pMblk->mBlkHdr.mLen = (rbdLen - ETHER_CRC_LEN) & ~0xc000;
    pMblk->mBlkPktHdr.len = pMblk->mBlkHdr.mLen;

    /* Make cache consistent with memory */

    FEC_END_CACHE_INVAL ((char *) pData, pMblk->mBlkHdr.mLen);

    /* up-date statistics */

    if ((*pData ) & (UINT8) 0x01)
        {
        pDrvCtrl->endObj.mib2Tbl.ifInNUcastPkts += 1;
        }
    else
        {
        END_ERR_ADD (&pDrvCtrl->endObj, MIB2_IN_UCAST, +1);
        }

    /* put the new RBD on the RBD queue */

    m5200FecRbdClean (pDrvCtrl, pBuf);

    END_RCV_RTN_CALL (&pDrvCtrl->endObj, pMblk);

    FEC_END_LOG (FEC_END_DBG_RX, ("m5200FecReceive... Done, \n"),
                  0, 0, 0, 0, 0, 0);

    return;

m5200FecRecvError:

    /* free buffers/clusters, clean the RBD */

    if (pMblk != NULL)
    NET_MBLK_FREE (pMblk);
    if (pClBlk != NULL)
    NET_CL_BLK_FREE (pClBlk);
    if (pBuf != NULL)
    NET_BUF_FREE ((UCHAR *) pBuf);

    pBuf = NULL; /* Its just been made free again. */

    /* put the errored RBD on the RBD queue */

    m5200FecRbdClean (pDrvCtrl, pBuf);

    }


/**************************************************************************
*
* m5200FecRbdClean - clean a receive buffer descriptor
*
* This routine cleans a receive buffer descriptor and initializes it
* with the data pointer <pBuf>.
*
* RETURNS: N/A
*
*/

LOCAL void m5200FecRbdClean
    (
    DRV_CTRL *  pDrvCtrl,       /* pointer to DRV_CTRL structure */
    char *  pBuf        /* pointer to a new data buffer */
    )
    {
    FEC_END_RBD_ID      pUsedRbd;   /* pointer to a RBD */
    char *      pData = NULL;   /* a rx data pointer */

    /* get the first used RBD */

    FEC_END_NEXT_RBD (pDrvCtrl, pUsedRbd);

    /* up-date the data pointer with the one provided by the pool */

    if (pBuf != NULL)
        {
        pDrvCtrl->rxBuf [(pDrvCtrl->rbdIndex)] = (UCHAR *) pBuf;

        pData = (char *) NET_TO_FEC_END_BUF (pBuf);

        }
    else
        {
        pData =
            (char *) NET_TO_FEC_END_BUF (pDrvCtrl->rxBuf [(pDrvCtrl->rbdIndex)]);
        }
    pDrvCtrl->rbdIndex = TaskBDRelease( pDrvCtrl->recvTaskNo );

    /*
     * Add a new buffer to the ring. This simple driver just adds
     * the same BD.
     */

    TaskBDAssign( pDrvCtrl->recvTaskNo, pData, NULL, FEC_END_MAX_PCK_SZ, 0 );

    FEC_END_LOG (FEC_END_DBG_RX, ("m5200FecRbdClean... Done, \n"),
                  0, 0, 0, 0, 0, 0);
    }

/**************************************************************************
*
* m5200FecRbdInit - initialize the receive buffer ring
*
* This routine initializes the receive buffer descriptors ring.
*
* SEE ALSO: m5200FecTbdInit()
*
* RETURNS: OK, or ERROR if not enough clusters were available.
*/

LOCAL STATUS m5200FecRbdInit
    (
    DRV_CTRL *  pDrvCtrl        /* pointer to DRV_CTRL structure */
    )
    {
    char *      pData = NULL;   /* a rx data pointer */
    char *      pBuf = NULL;    /* a rx data pointer */
    UINT16      ix;     /* a counter */

    for (ix = 0; ix < pDrvCtrl->rbdNum; ix++)
        {
        /* get a cluster buffer from the pool */

        if ((pBuf = NET_BUF_ALLOC()) == NULL)
            {
            FEC_END_LOG (FEC_END_DBG_ERR, ("m5200FecRbdInit, alloc failed\n"), 0, 0, 0, 0, 0, 0);
            return (ERROR);
            }

        /*
        * what we need to do here is to save the cluster pointer
        * for use by the net pool later. We won't be using that
        * cluster as is, because the FEC requires receive buffers
        * being aligned by 16. So we move that cluster pointer to
        * the next 16-byte aligned boundary, and program the chip
        * with that value. When we receive the packet, we'll have
        * to join the saved cluster pointer to the mBlk.
        */

        pDrvCtrl->rxBuf [ix] = (UCHAR *) pBuf;

        pData = (char *) NET_TO_FEC_END_BUF (pBuf);

        TaskBDAssign(pDrvCtrl->recvTaskNo, FEC_END_VIRT_TO_PHYS (pData),
                        NULL, FEC_END_MAX_PCK_SZ, 0 );
        }

    return (OK);
    }

/**************************************************************************
*
* m5200FecBdFree - free the receive buffers
*
* This routine frees the netpool clusters associated with the receive
* buffer descriptors ring. It is called by m5200FecStop(), in order to
* properly release the driver's resources when the device is stopped.
*
* SEE ALSO: m5200FecStop()
*
* RETURNS: OK, always.
*/

LOCAL STATUS m5200FecBdFree
    (
    DRV_CTRL *  pDrvCtrl        /* pointer to DRV_CTRL structure */
    )
    {
    UINT16      ix;     /* a counter */

    for (ix = 0; ix < pDrvCtrl->rbdNum; ix++)
    {
        /* return the cluster buffer to the pool */

        if ((pDrvCtrl->rxBuf [ix]) != NULL)
            NET_BUF_FREE ((pDrvCtrl->rxBuf [ix]));
    }

    /* free the transmit poll buffer */

    if ((pDrvCtrl->pTxPollBuf) != NULL)
        NET_BUF_FREE ((UCHAR *) (pDrvCtrl->pTxPollBuf));

    return (OK);
    }

/**************************************************************************
*
* m5200FecPrePhyConfig - configure the chip before the PHY is initialized
*
* This routine programs all the CSRs that are not concerned with the
* PHY configuration. It follows closely but not entirely the
* initialization sequence recommended in the FEC User's Manual.
*
* SEE ALSO: m5200FecPostPhyConfig()
*
* RETURNS: OK, always.
*/

LOCAL STATUS m5200FecPrePhyConfig
    (
    DRV_CTRL *  pDrvCtrl        /* pointer to DRV_CTRL structure */
    )
    {
    UINT32  csrVal = 0; /* holder for the CSR value */
    UINT32  csr0Val = 0;    /* holder for the CSR0 value */
    UINT32  csr1Val = 0;    /* holder for the CSR1 value */
    UINT32  crcVal = 0;     /*crc of physical address */
    static BOOL first = TRUE;   /* first initialization */
    UINT32      miiSpeed;   /* calculated value for MII_SPEED */
    int level;


   /*
     * we enable the following event interrupts:
     * heartbeat check fail, only if the user requested it;
     * tx and rx babbling errors;
     * tx and rx frame completion;
     * graceful transmit command;
     * mii management interface frame completion;
     * U-bus access error.
     */

    if (FEC_END_USR_FLAG_ISSET (FEC_END_USR_HBC))
        {
        csrVal |= FEC_END_EVENT_HB;

        FEC_END_LOG (FEC_END_DBG_START,
                ("m5200FecPrePhyConfig: heartbeat control \n"),
                0, 0, 0, 0, 0, 0);
        }
    else
        csrVal &= ~FEC_END_EVENT_HB;

    csrVal |= (FEC_END_EVENT_GRA | FEC_END_EVENT_BABR |
               FEC_END_EVENT_BABT | FEC_END_EVENT_BERR | FEC_END_EVENT_LCOL |
               FEC_END_EVENT_COL_RL | FEC_END_EVENT_XFIFO_UN |
               FEC_END_EVENT_FIFO_ERR | FEC_END_EVENT_RFIFO_ERR );

    FEC_END_CSR_WR (FEC_END_MASK_OFF, csrVal);

    /* save the interrupt mask register */

    pDrvCtrl->intMask = csrVal;

    /* clear all interrupts */

    FEC_END_CSR_WR (FEC_END_EVENT_OFF, FEC_END_EVENT_MSK);

    /* Set Rx FIFO alarm and granularity value */
    FEC_END_CSR_WR (FEC_END_rfifo_cntrl_OFF, FEC_RFIFO_CNTRL_FRAME | FEC_RFIFO_CNTRL_GR);
    FEC_END_CSR_WR (FEC_END_rfifo_alarm_OFF, FEC_RFIFO_ALARM);

    /*
     * Set Tx FIFO granularity value
     * -- also must manually set an undocumented bit (TXW_MASK) to fix Rev.B
     */
    FEC_END_CSR_WR (FEC_END_tfifo_cntrl_OFF,
                    FEC_XFIFO_CNTRL_FRAME | FEC_XFIFO_CNTRL_GR |
                    FEC_XFIFO_CNTRL_TXW_MASK);

    /* Set transmit fifo watermark register(X_WMRK), default = 64 */
    FEC_END_CSR_WR (FEC_END_tfifo_alarm_OFF, 256);
    FEC_END_CSR_WR (FEC_END_x_wmrk_OFF, FEC_XFIFO_WMRK_256);

    /* program the individual enet address */

    if (m5200FecAddrRegValGet (pDrvCtrl, &csr0Val, &csr1Val) != OK)
        return (ERROR);

    FEC_END_CSR_WR (FEC_END_ADDR_L_OFF, csr0Val);
    FEC_END_CSR_WR (FEC_END_ADDR_H_OFF, csr1Val);

    crcVal = m5200FecCrcGet ((char *) FEC_END_ADDR_GET (&pDrvCtrl->endObj));
    crcVal = crcVal >> 26;

    /* Set individual hash table register */
    if (crcVal >= 32)
        {
        FEC_END_CSR_WR (FEC_END_iaddr1_OFF, (1 << (crcVal - 32)));
        FEC_END_CSR_WR (FEC_END_iaddr2_OFF, 0);
        }
    else
        {
        FEC_END_CSR_WR (FEC_END_iaddr1_OFF, 0);
        FEC_END_CSR_WR (FEC_END_iaddr2_OFF, (1 << crcVal));
        }

    /* Turn ON cheater FSM */
    FEC_END_CSR_WR (FEC_END_xmit_fsm_OFF, 0x03000000);

    /* reset the hash table */
    /* question: does muxDevStop/Start re-init the multi table? */

    if (first)
        {
        first = FALSE;
        FEC_END_CSR_WR (FEC_END_HASH_L_OFF, 0);
        FEC_END_CSR_WR (FEC_END_HASH_H_OFF, 0);
        }

    /*
     * if no clock speed is defined, assume a clock speed of 25 MHz.
     * Otherwise, derive the MII clock speed according to the formula
     * MII_SPEED = ceil(pDrvCtrl->clockSpeed / (2 * MII_CLOCK_MAX))
     */

    if (pDrvCtrl->clockSpeed == 0)
        miiSpeed = FEC_END_MII_SPEED_25;
    else
        miiSpeed = (pDrvCtrl->clockSpeed + (2 * FEC_END_MII_CLOCK_MAX) - 1) /
                    (2 * FEC_END_MII_CLOCK_MAX);
    FEC_END_LOG (FEC_END_DBG_MII, "m5200FecPrePhyConfig: mii Speed=%i\n",
                    miiSpeed, 0, 0, 0, 0, 0);

    /* MII_SPEED is left-shifted in the MII_SPEED register */

    FEC_END_CSR_WR (FEC_END_MII_SPEED_OFF, miiSpeed << FEC_END_MII_SPEED_SHIFT);

    /* Clear SmartDMA task interrupt pending bits */
    level = intLock();
    TaskIntClear( pDrvCtrl->xmitTaskNo );
    intUnlock(level);

    return (OK);
    }


/**************************************************************************
*
* m5200FecPostPhyConfig - configure the chip after the PHY is initialized
*
* This routine programs all the CSRs that are concerned with the
* PHY configuration.
*
* SEE ALSO: m5200FecPrePhyConfig()
*
* RETURNS: OK, always.
*/

LOCAL STATUS m5200FecPostPhyConfig
    (
    DRV_CTRL *  pDrvCtrl        /* pointer to DRV_CTRL structure */
    )
    {
    UINT32  rxCtrlVal;  /* holder for the rx CSR value */
    UINT32  txCtrlVal;  /* holder for the tx CSR value */

    /* get the proper value for the rx CSR and program it accordingly */

    if (m5200FecRxCtrlRegValGet (pDrvCtrl, &rxCtrlVal) == ERROR)
        return (ERROR);

    FEC_END_CSR_WR (FEC_END_RX_CTRL_OFF, rxCtrlVal);

    /*
     * get the proper value for the tx CSR and program it accordingly.
     * At this point we do not have to check whether the FEC is
     * active, as we already know.
     */

    if (m5200FecTxCtrlRegValGet (pDrvCtrl, &txCtrlVal) == ERROR)
        return (ERROR);

    FEC_END_CSR_WR (FEC_END_TX_CTRL_OFF, txCtrlVal);

    return (OK);
    }

/**************************************************************************
*
* m5200FecRxCtrlRegValGet - get the proper value for the receive CSR
*
* This routine finds out the proper value to be programmed in the
* receive CSR by looking at some of the user/driver flags. It deals
* with options like promiscous mode, full duplex, loopback and
* the physical serial interface.
*
* This routine does not program the receive CSR.
*
* SEE ALSO: m5200FecTxCtrlRegValGet(), m5200FecPostPhyConfig()
*
* RETURNS: OK or ERROR if rxCtrlVal is NULL pointer.
*/

LOCAL STATUS m5200FecRxCtrlRegValGet
    (
    DRV_CTRL *  pDrvCtrl,   /* pointer to DRV_CTRL structure */
    UINT32 *    rxCtrlVal   /* holder for the rx CSR value */
    )
    {

    /* Initialize holder for the rx CSR value */

    if (rxCtrlVal == NULL)
        {
        FEC_END_LOG (FEC_END_DBG_START,
                     ("m5200FecRxCtrlRegValGet: rxCtrlVal is NULL \n"),
                     0, 0, 0, 0, 0, 0);
        return (ERROR);
        }
    else
#if 0
        *rxCtrlVal = (FEC_END_MAX_PCK_SZ -14) << 16;
#else
        *rxCtrlVal = (FEC_END_MAX_PCK_SZ) << 16;
#endif

    /* deal with promiscous mode */

    if (FEC_END_FLAG_ISSET (FEC_END_PROM))
        {
        *rxCtrlVal |= FEC_END_RX_CTRL_PROM;

        FEC_END_LOG (FEC_END_DBG_START,
                ("m5200FecRxCtrlRegValGet: promiscous mode \n"),
                0, 0, 0, 0, 0, 0);
        }
    else
        *rxCtrlVal &= ~FEC_END_RX_CTRL_PROM;

    /*
     * enable full duplex mode if the PHY was configured
     * to work in full duplex mode.
     */

    if (FEC_END_PHY_FLAGS_ISSET (FEC_END_PHY_FD))
        {
        *rxCtrlVal &= ~FEC_END_RX_CTRL_DRT;

        FEC_END_LOG (FEC_END_DBG_START,
                ("m5200FecRxCtrlRegValGet: full duplex \n"),
                0, 0, 0, 0, 0, 0);
        }
    else
        *rxCtrlVal |= FEC_END_RX_CTRL_DRT;

    /*
     * enable the 7-wire serial interface if the
     * related user flag was set.
     */

    if (FEC_END_USR_FLAG_ISSET (FEC_END_USR_SER))
        {
        *rxCtrlVal &= ~FEC_END_RX_CTRL_MII;

        FEC_END_LOG (FEC_END_DBG_START,
                ("m5200FecRxCtrlRegValGet: serial interface \n"),
                0, 0, 0, 0, 0, 0);
        }
    else
        *rxCtrlVal |= FEC_END_RX_CTRL_MII;

    /*
     * if the user wishes to go in loopback mode,
     * enable it. Also enable receiver and transmitter
     * to work independently from each other.
     */

    if (FEC_END_USR_FLAG_ISSET (FEC_END_USR_LOOP))
        {
        *rxCtrlVal |= FEC_END_RX_CTRL_LOOP;
        *rxCtrlVal &= ~FEC_END_RX_CTRL_DRT;

        FEC_END_LOG (FEC_END_DBG_START,
                ("m5200FecRxCtrlRegValGet: loopback mode \n"),
                0, 0, 0, 0, 0, 0);
        }
    else
        *rxCtrlVal &= ~FEC_END_RX_CTRL_LOOP;

    return (OK);
    }

/**************************************************************************
*
* m5200FecTxCtrlRegValGet - get the proper value for the transmit CSR
*
* This routine finds out the proper value to be programmed in the
* transmit CSR by looking at some of the user/driver flags. It deals
* with options like full duplex mode and the heartbeat control.
*
* This routine does not program the transmit CSR.
*
* RETURNS: OK or ERROR if txCtrlVal is NULL pointer.
*
* SEE ALSO: m5200FecRxCtrlRegValGet(), m5200FecPostPhyConfig()
*
*/

LOCAL STATUS m5200FecTxCtrlRegValGet
    (
    DRV_CTRL *  pDrvCtrl,   /* pointer to DRV_CTRL structure */
    UINT32 *    txCtrlVal   /* holder for the tx CSR value */
    )
    {

    /* Initialize holder for the tx CSR value */

    if (txCtrlVal == NULL)
        {
        FEC_END_LOG (FEC_END_DBG_START,
                     ("m5200FecTxCtrlRegValGet: txCtrlVal is NULL \n"),
                     0, 0, 0, 0, 0, 0);
        return (ERROR);
        }
    else
        *txCtrlVal = 0;

    /* deal with the heartbeat control */

    if (FEC_END_USR_FLAG_ISSET (FEC_END_USR_HBC))
        {
        *txCtrlVal |= FEC_END_TX_CTRL_HBC;

        FEC_END_LOG (FEC_END_DBG_START,
                ("m5200FecTxCtrlRegValGet: heartbeat control \n"),
                0, 0, 0, 0, 0, 0);
        }
    else
        *txCtrlVal &= ~FEC_END_TX_CTRL_HBC;

    /*
     * enable full duplex mode if the PHY was configured
     * to work in full duplex mode.
     */

    if (FEC_END_PHY_FLAGS_ISSET (FEC_END_PHY_FD))
        {
        *txCtrlVal |= FEC_END_TX_CTRL_FD;

        FEC_END_LOG (FEC_END_DBG_START,
                ("m5200FecTxCtrlRegValGet: full duplex \n"),
                0, 0, 0, 0, 0, 0);
        }
    else
        *txCtrlVal &= ~FEC_END_TX_CTRL_FD;

    return (OK);
    }

/**************************************************************************
*
* m5200FecAddrRegValGet - get the values to program in CSR0 and CSR1
*
* This routine finds out the proper values to be programmed in the
* two 32-bit perfect match address registers (CSR0 and CSR1).
*
* This routine does not program neither CSR0 nor CSR1.
*
* RETURNS: OK, always.
*/

LOCAL STATUS m5200FecAddrRegValGet
    (
    DRV_CTRL *  pDrvCtrl,   /* pointer to DRV_CTRL structure */
    UINT32 *    csr0Val,    /* holder for the CSR0 value */
    UINT32 *    csr1Val     /* holder for the CSR1 value */
    )
    {
    char *  enetAddr = NULL;    /* holder for the enet address */

    /*
     * programming the enet address is done by writing
     * its low-order 4 bytes to CSR0 and its high-order
     * 2 bytes to the MSW of CSR1.
     */

    enetAddr = (char *) FEC_END_ADDR_GET (&pDrvCtrl->endObj);

    *csr0Val = * (UINT32 *) enetAddr;

    /*
     * this way we'll zero-out the low-order 16 bits in the
     * CSR, as recommended in the documentation.
     */

    *csr1Val = ((UINT32) (* (UINT16 *) (enetAddr + 4)) << 16);

    return (OK);
    }

/*******************************************************************************
*
* m5200FecIoctl - interface ioctl procedure
*
* Process an interface ioctl request.
*
* RETURNS: OK, or ERROR.
*/

LOCAL int m5200FecIoctl
    (
    DRV_CTRL *  pDrvCtrl,       /* pointer to DRV_CTRL structure */
    int         cmd,        /* command to process */
    caddr_t     data            /* pointer to data */
    )
    {
    int         error = OK;
    INT8        savedFlags;
    long        value;
    END_OBJ *   pEndObj=&pDrvCtrl->endObj;

    FEC_END_LOG (FEC_END_DBG_IOCTL,
                    ("Ioctl unit=0x%x cmd=%d data=0x%x\n"),
                    pDrvCtrl->unit, cmd, (int)data, 0, 0, 0);

    switch (cmd)
        {
        case EIOCSADDR:
            if (data == NULL)
                error = EINVAL;
            else
                {
                UINT32  csr0Val;    /* holder for the CSR0 value */
                UINT32  csr1Val;    /* holder for the CSR1 value */

                        /* Copy and install the new address */

                bcopy ((char *) data,
                    (char *) FEC_END_ADDR_GET (&pDrvCtrl->endObj),
                    FEC_END_ADDR_LEN_GET (&pDrvCtrl->endObj));

                /* stop the FEC */

                /* Prevent queue of new transmissions as we are stopping */

                pDrvCtrl->stoppingTx = TRUE;

                if (m5200FecStop (pDrvCtrl) != OK)
                    return (ERROR);

                /* program the individual enet address */

                if (m5200FecAddrRegValGet (pDrvCtrl, &csr0Val, &csr1Val) != OK)
                    return (ERROR);

                FEC_END_CSR_WR (FEC_END_ADDR_L_OFF, csr0Val);
                FEC_END_CSR_WR (FEC_END_ADDR_H_OFF, csr1Val);

                /* restart the FEC */

                if (m5200FecStart (pDrvCtrl) != OK)
                    return (ERROR);
                }

            break;

        case EIOCGADDR:
            if (data == NULL)
                error = EINVAL;
            else
                bcopy ((char *) FEC_END_ADDR_GET (&pDrvCtrl->endObj),
                    (char *) data,
                    FEC_END_ADDR_LEN_GET (&pDrvCtrl->endObj));

            break;

        case EIOCSFLAGS:
            value = (long) data;
            if (value < 0)
                {
                value = -value;
                value--;
                END_FLAGS_CLR (pEndObj, value);
                }
            else
                END_FLAGS_SET (pEndObj, value);

            FEC_END_LOG (FEC_END_DBG_IOCTL, ("endFlags=0x%x \n"),
                         END_FLAGS_GET(pEndObj),
                         0, 0, 0, 0, 0);

            /* handle IFF_PROMISC */

            savedFlags = FEC_END_FLAG_GET();
            if (END_FLAGS_ISSET (IFF_PROMISC))
                {
                FEC_END_FLAG_SET (FEC_END_PROM);

                if ((FEC_END_FLAG_GET () != savedFlags) &&
                    (END_FLAGS_GET (pEndObj) & IFF_UP))
                    {
                    UINT32  rxCtrlVal;

                    /* config down */

                    END_FLAGS_CLR (pEndObj, IFF_UP | IFF_RUNNING);

                    /* program the rx CSR to promiscous mode */

                    if (m5200FecRxCtrlRegValGet (pDrvCtrl, &rxCtrlVal) == ERROR)
                    return (ERROR);

                    FEC_END_CSR_WR (FEC_END_RX_CTRL_OFF, rxCtrlVal);

                    /* config up */

                    END_FLAGS_SET (pEndObj, IFF_UP | IFF_RUNNING);
                    }
                }
            else
                FEC_END_FLAG_CLEAR (FEC_END_PROM);

            /* handle IFF_MULTICAST */

            if (END_FLAGS_GET(pEndObj) & (IFF_MULTICAST))
                FEC_END_FLAG_SET (FEC_END_MCAST);
            else
                FEC_END_FLAG_CLEAR (FEC_END_MCAST);

            FEC_END_LOG (FEC_END_DBG_IOCTL, ("EIOCSFLAGS: 0x%x: 0x%x\n"),
                         pEndObj->flags, savedFlags,
                         0, 0, 0, 0);

            break;

        case EIOCGFLAGS:
            FEC_END_LOG (FEC_END_DBG_IOCTL, ("EIOCGFLAGS: 0x%x: 0x%x\n"),
                        pEndObj->flags, *(long *)data,
                        0, 0, 0, 0);

            if (data == NULL)
                error = EINVAL;
            else
                *(long *)data = END_FLAGS_GET(pEndObj);

            break;

        case EIOCMULTIADD:
            error = m5200FecMCastAddrAdd (pDrvCtrl, (char *) data);
            break;

        case EIOCMULTIDEL:
            error = m5200FecMCastAddrDel (pDrvCtrl, (char *) data);
            break;

        case EIOCMULTIGET:
            error = m5200FecMCastAddrGet (pDrvCtrl, (MULTI_TABLE *) data);
            break;

        case EIOCPOLLSTART:
        FEC_END_LOG (FEC_END_DBG_POLL, ("IOCTL call m5200FecPollStart\n"),
                        0, 0, 0, 0, 0, 0);

            m5200FecPollStart (pDrvCtrl);
            break;

        case EIOCPOLLSTOP:
        FEC_END_LOG (FEC_END_DBG_POLL, ("IOCTL call m5200FecPollStop\n"),
                        0, 0, 0, 0, 0, 0);

            m5200FecPollStop (pDrvCtrl);
            break;

        case EIOCGMIB2:
            if (data == NULL)
                error=EINVAL;
            else
                bcopy ((char *) &pEndObj->mib2Tbl, (char *) data,
                        sizeof (pEndObj->mib2Tbl));
            break;

        default:
            FEC_END_LOG (FEC_END_DBG_IOCTL, ("INVALID IO COMMAND!! \n"),
                         0, 0, 0, 0, 0, 0);
            error = EINVAL;
        }

    return (error);
    }

/**************************************************************************
*
* m5200FecPhyInit - initialize and configure the PHY devices
*
* This routine scans, initializes and configures PHY devices.
*
* This routine is called from the driver's Start routine to
* perform media inialization and configuration. To access the PHY
* device, it uses the routines: m5200FecPhyRead(), m5200FecPhyWrite(),
* which exploit the support to the MII interface built-in in the
* FEC.
*
* Before it attempts to bring the link up, this routine checks the
* phyInfo structure in the driver control structure for a device that
* needs to be electrically isolated by the MII interface; if a valid
* device is found it is isolated.
*
* The routine then scans for all possible PHY addresses in the range 0-31,
* checking for an MII-compliant PHY, and attempts to  establish a
* valid link for it. If none is found, ERROR is returned.
* Typically PHYs are scanned from address 0, but if the user specifies
* an alternative start PHY address via the parameter phyAddr in the
* driver's load string, all (32) PHYs are scanned in order starting
* with the specified PHY address.
*
* This routine offers two strategies to select a PHY and establish a
* valid link. The default strategy is to use the standard 802.3 style
* auto-negotiation, where both link partners negotiate all their
* technology abilities at the same time, and the highest common
* denominator ability is chosen. Before the auto-negotiation
* is started, the next-page exchange mechanism is disabled.
*
* The user can prevent the PHY from negotiating certain abilities via
* userFlags -- <FEC_END_USR_PHY_NO_FD>, <FEC_END_USR_PHY_NO_100>,
* <FEC_END_USR_PHY_NO_HD>, and <FEC_END_USR_PHY_NO_10>. When
* <FEC_END_USR_PHY_NO_FD> is specified, full duplex will not be
* negotiated; when <FEC_END_USR_PHY_NO_HD> is specified half duplex
* will not be negotiated, when <FEC_END_USR_PHY_NO_100> is specified,
* 100Mbps ability will not negotiated; when <FEC_END_USR_PHY_NO_10>
* 10Mbps ability will not be negotiated.
*
* When <FEC_END_USR_PHY_TBL> is set in the user flags, the BSP specific
* table <m5200FecPhyAnOrderTbl> is used to obtain the list, and the order
* of technology abilities to be negotiated.
* The entries in this table are ordered such that entry 0 is the
* highest priority, entry 1 in next and so on. Entries in this table
* may be repeated, and multiple technology abilities can
* be OR'd to create a single  entry. If a PHY cannot support a
* ability in an entry, that entry is ignored. Currently, only one
* table per driver is supported.
*
* If no PHY provides a valid link, the PHYs are scanned again in the
* same order, and the first PHY that supports the default abilities
* defined in the <phyDefMode> of the Load string will be selected,
* regardless of the link status.
*
* RETURNS: OK or ERROR if the PHY could not be initialised.
*
*/

LOCAL STATUS m5200FecPhyInit
    (
    DRV_CTRL *  pDrvCtrl       /* pointer to DRV_CTRL structure */
    )
    {
    UINT8   phyAddr;    /* address of a PHY */
    UINT8   isoPhyAddr; /* address of a PHY to be isolated */
    UINT8   ix;     /* an index */
    int     retVal;     /* convenient holder for return value */
    BOOL    found = FALSE;  /* no PHY has been found */

    /* isolate the PHY in the phyInfo structure */

    isoPhyAddr = pDrvCtrl->phyInfo->isoPhyAddr;

    if (isoPhyAddr != FEC_END_PHY_NULL)
        {
        /* check the PHY is there */

        retVal = m5200FecMiiProbe (pDrvCtrl, isoPhyAddr);
        if (retVal == ERROR)
            return (ERROR);

        if (retVal == OK)
            if (m5200FecMiiIsolate (pDrvCtrl, isoPhyAddr) == ERROR)
            return (ERROR);
        }

    /*
     * there may be several PHYs, with distinct logical addresses
     * and these can be as high as 31. Start with the one the
     * user suggested and, in case of failure, scan the whole range.
     */

    for (ix = 0; ix < MII_MAX_PHY_NUM; ix++)
        {
        phyAddr = (ix + pDrvCtrl->phyInfo->phyAddr) % MII_MAX_PHY_NUM;

        /* check this is not the isolated PHY */

        if (phyAddr == isoPhyAddr)
            continue;

        /* check the PHY is there */

        retVal = m5200FecMiiProbe (pDrvCtrl, phyAddr);
        if (retVal == ERROR)
            return (ERROR);

        if (retVal == FEC_END_PHY_NULL)
            continue;

        found = TRUE;

        /*
        * start the auto-negotiation process,
        * unless the user dictated the contrary.
        */

        if (pDrvCtrl->phyInfo->phyFlags & FEC_END_PHY_AUTO)
            if (m5200FecMiiAnRun (pDrvCtrl, phyAddr) == OK)
            return (OK);

        /*
        * the auto-negotiation process did not succeed
        * in establishing a valid link: try to do it
        * manually, enabling as high priority abilities
        * as you can.
        */

        if (m5200FecMiiModeForce (pDrvCtrl, phyAddr) == OK)
            return (OK);

        /*
        * Trying to force a specific operating mode was
        * unsuccessful, too. Force default parameters:
        * field phyDefMode in the PHY info structure.
        */

        if (m5200FecMiiDefForce (pDrvCtrl, phyAddr) == OK)
            return (OK);
        }

    if (!(found))
        return (ERROR);

    /* if we're here, none of the PHYs could be initialized */
    /* 
   
    logMsg ("m5200FecPhyInit check cable connection \n",
            0, 0, 0, 0, 0, 0);
    */

    /* try to establish a default operating mode, do not check the link! */

    for (ix = 0; ix < MII_MAX_PHY_NUM; ix++)
        {
        phyAddr = (ix + pDrvCtrl->phyInfo->phyAddr) % MII_MAX_PHY_NUM;

        /* check this is not the isolated PHY */

        if (phyAddr == isoPhyAddr)
            continue;

        /* check the PHY is there */

        retVal = m5200FecMiiProbe (pDrvCtrl, phyAddr);
        if (retVal == ERROR)
            return (ERROR);

        if (retVal == FEC_END_PHY_NULL)
            continue;

        /* return OK even if the link is not up */

        retVal = m5200FecMiiDefForce (pDrvCtrl, phyAddr);
        if (retVal == ERROR)
            return (ERROR);

        /* if the PHY does not have the default abilities... */

        if (retVal == FEC_END_PHY_NO_ABLE)
            continue;

        return (OK);
        }

    return (ERROR);
    }

/**************************************************************************
*
* m5200FecMiiProbe - probe the PHY device
*
* This routine probes the PHY device by reading its control register.
*
* RETURNS: OK, ERROR in case of fatal errors, or FEC_END_PHY_NULL, if
* the device was not found.
*/

LOCAL STATUS m5200FecMiiProbe
    (
    DRV_CTRL *  pDrvCtrl,       /* pointer to DRV_CTRL structure */
    UINT8   phyAddr     /* the PHY being read */
    )
    {
    UINT8   regAddr;    /* the PHY's register being read */
    UINT16  data;       /* data to be written to the control reg */

    regAddr = MII_CTRL_REG;
    if (m5200FecMiiRead (pDrvCtrl, phyAddr, regAddr, &data) != OK)
        return (ERROR);

    if (data == 0xffff)
        return (FEC_END_PHY_NULL);

    FEC_END_LOG (FEC_END_DBG_MII, ("m5200FecMiiProbe... ends (%i)\n"),
                   phyAddr, 0, 0, 0, 0, 0);

    return (OK);
    }

/**************************************************************************
*
* m5200FecPhyPreInit - initialize some fields in the phy info structure
*
* This routine initializes some fields in the phy info structure,
* for use of the m5200FecPhyInit() routine.
*
* RETURNS: OK, always.
*/

LOCAL STATUS m5200FecPhyPreInit
    (
    DRV_CTRL *  pDrvCtrl       /* pointer to DRV_CTRL structure */
    )
    {
    /* set some default values */

    pDrvCtrl->phyInfo->phySpeed = FEC_END_10MBS;
    pDrvCtrl->phyInfo->phyMode = FEC_END_PHY_HD;
    pDrvCtrl->phyInfo->phyFlags = 0;

    /* handle some user-to-physical flags */

    if (!(FEC_END_USR_FLAG_ISSET (FEC_END_USR_PHY_NO_AN)))
        FEC_END_PHY_FLAGS_SET (FEC_END_PHY_AUTO);
    else
        FEC_END_PHY_FLAGS_CLEAR (FEC_END_PHY_AUTO);

    if (FEC_END_USR_FLAG_ISSET (FEC_END_USR_PHY_TBL))
        FEC_END_PHY_FLAGS_SET (FEC_END_PHY_TBL);
    else
        FEC_END_PHY_FLAGS_CLEAR (FEC_END_PHY_TBL);

    if (!(FEC_END_USR_FLAG_ISSET (FEC_END_USR_PHY_NO_100)))
        FEC_END_PHY_FLAGS_SET (FEC_END_PHY_100);
    else
        FEC_END_PHY_FLAGS_CLEAR (FEC_END_PHY_100);

    if (!(FEC_END_USR_FLAG_ISSET (FEC_END_USR_PHY_NO_FD)))
        FEC_END_PHY_FLAGS_SET (FEC_END_PHY_FD);
    else
        FEC_END_PHY_FLAGS_CLEAR (FEC_END_PHY_FD);

    if (!(FEC_END_USR_FLAG_ISSET (FEC_END_USR_PHY_NO_10)))
        FEC_END_PHY_FLAGS_SET (FEC_END_PHY_10);
    else
        FEC_END_PHY_FLAGS_CLEAR (FEC_END_PHY_10);

    if (!(FEC_END_USR_FLAG_ISSET (FEC_END_USR_PHY_NO_HD)))
        FEC_END_PHY_FLAGS_SET (FEC_END_PHY_HD);
    else
        FEC_END_PHY_FLAGS_CLEAR (FEC_END_PHY_HD);

    /* mark it as pre-initilized */

    FEC_END_PHY_FLAGS_SET (FEC_END_PHY_PRE_INIT);

    return (OK);
    }

/**************************************************************************
*
* m5200FecMiiRead - read the MII register
*
* This routine reads the register specified by <phyReg> in the PHY device
* whose address is <phyAddr>. The value read is returned in the location
* pointed to by <retVal>.
*
* SEE ALSO: m5200FecMiiWrite()
*
* RETURNS: OK, always.
*
*/

STATUS m5200FecMiiRead
    (
    DRV_CTRL *  pDrvCtrl,   /* pointer to DRV_CTRL structure */
    UINT8   phyAddr,    /* the PHY being read */
    UINT8   regAddr,    /* the PHY's register being read */
    UINT16  *retVal     /* the value read */
    )
    {
    UINT32  reg;    /* convenient holder for the PHY register */
    UINT32  phy;    /* convenient holder for the PHY */
    UINT32  miiVal; /* value written/read to/from the FEC's MII register */
    volatile UINT32  status = 0;
    int  i=0;
    const int TIMEOUT=500;

    /*
     * reading from any PHY's register is done by properly
     * programming the FEC's MII data register.
     */

    reg = regAddr << FEC_END_MII_RA_SHIFT;
    phy = phyAddr << FEC_END_MII_PA_SHIFT;

    miiVal = (FEC_END_MII_ST | FEC_END_MII_OP_RD | FEC_END_MII_TA |
              phy | reg);

    FEC_END_MII_SEM_TAKE;

    FEC_END_CSR_WR (FEC_END_MII_DATA_OFF, miiVal);
    
    do
        {
        FEC_END_CSR_RD (FEC_END_EVENT_OFF, status);
    	if(i++>TIMEOUT) 
    	    {
            FEC_END_MII_SEM_GIVE;
    	    return ERROR;
            } 	
    	}while((status & FEC_END_EVENT_MII) != FEC_END_EVENT_MII);
        
    FEC_END_CSR_WR (FEC_END_EVENT_OFF, FEC_END_EVENT_MII);

    /* it's now safe to read the PHY's register */

    FEC_END_CSR_RD (FEC_END_MII_DATA_OFF, miiVal);

    *retVal = (UINT16) miiVal;
    
    FEC_END_MII_SEM_GIVE;

    return (OK);

    }

/**************************************************************************
*
* m5200FecMiiWrite - write to the MII register
*
* This routine writes the register specified by <phyReg> in the PHY device,
* whose address is <phyAddr>, with the 16-bit value included in <data>.
*
* SEE ALSO: m5200FecMiiRead()
*
* RETURNS: OK, always.
*/

STATUS m5200FecMiiWrite
    (
    DRV_CTRL *  pDrvCtrl,   /* pointer to DRV_CTRL structure */
    UINT8   phyAddr,    /* the PHY being written */
    UINT8   regAddr,    /* the PHY's register being written */
    UINT16  data        /* the value written to the PHY register */
    )
    {
    UINT32      reg;    /* convenient holder for the PHY register */
    UINT32      phy;    /* convenient holder for the PHY */
    UINT32      miiVal; /* value written to the mii reg */
    volatile UINT32  status = 0;
    int i=0;
    const int TIMEOUT = 500;

    reg = regAddr << FEC_END_MII_RA_SHIFT;
    phy = phyAddr << FEC_END_MII_PA_SHIFT;
    
    miiVal = (FEC_END_MII_ST | FEC_END_MII_OP_WR | FEC_END_MII_TA |
              phy | reg | data);

    FEC_END_LOG (FEC_END_DBG_MII, ("m5200FecMiiWrite reg=%d \n\
                   PHY=%d data=0x%x\n"),
                   regAddr, phyAddr, miiVal, 0, 0, 0);
    
    FEC_END_MII_SEM_TAKE;

    FEC_END_CSR_WR (FEC_END_MII_DATA_OFF, miiVal);

    do
        {
    	FEC_END_CSR_RD (FEC_END_EVENT_OFF, status);
    	if(i++>TIMEOUT) 
    	    {
            FEC_END_MII_SEM_GIVE;
    	    return ERROR;
            }
    	}while((status & FEC_END_EVENT_MII) != FEC_END_EVENT_MII);
    
    FEC_END_CSR_WR (FEC_END_EVENT_OFF, FEC_END_EVENT_MII);

    FEC_END_MII_SEM_GIVE;

    return (OK);

    }

/*******************************************************************************
*
* m5200FecMiiIsolate - isolate the PHY device
*
* This routine isolates the PHY device whose address is specified in
* the parameter <isoPhyAddr>.
*
* RETURNS: OK or ERROR.
*/

LOCAL STATUS m5200FecMiiIsolate
    (
    DRV_CTRL *  pDrvCtrl,       /* pointer to DRV_CTRL structure */
    UINT8   isoPhyAddr  /* address of a PHY to be isolated */
    )
    {
    UINT8   regAddr;    /* PHY register */
    UINT16  ix = 0;     /*  a counter */
    UINT16  data;       /* data to be written to the control reg */

    if (isoPhyAddr == FEC_END_PHY_NULL)
        return (OK);

    data = MII_CR_ISOLATE;
    regAddr = MII_CTRL_REG;

    if (m5200FecMiiWrite (pDrvCtrl, isoPhyAddr, regAddr, data) != OK)
        return (ERROR);

    /* check isolate bit is asserted */

    ix = 0;
    while (ix++ < FEC_END_PHY_MAX_WAIT)
        {
        taskDelay (1);

        if (m5200FecMiiRead (pDrvCtrl, isoPhyAddr, regAddr, &data) != OK)
            return (ERROR);

        if ((data & MII_CR_ISOLATE))
            break;
        }

    if (ix >= FEC_END_PHY_MAX_WAIT)
        {
        FEC_END_LOG (FEC_END_DBG_MII, ("m5200FecMiiIsolate fail\n"),
                        0, 0, 0, 0, 0, 0);
        return (ERROR);
        }

    FEC_END_LOG (FEC_END_DBG_MII, ("m5200FecMiiIsolate... ends\n"),
                   0, 0, 0, 0, 0, 0);

    return (OK);
    }

/*******************************************************************************
*
* m5200FecMiiAnRun - run the auto-negotiation process
*
* This routine runs the auto-negotiation process for the PHY whose
* address is specified in the parameter <phyAddr>, without enabling the
* next page function. It also calls m5200FecMiiAnCheck() to ensure
* a valid link has been established.
*
* RETURNS: OK or ERROR.
*/

LOCAL STATUS m5200FecMiiAnRun
    (
    DRV_CTRL *  pDrvCtrl,       /* pointer to DRV_CTRL structure */
    UINT8   phyAddr     /* address of a PHY */
    )
    {
    UINT8   regAddr;    /* PHY register */
    UINT16  phyAds;     /* holder for the PHY ads register value */
    UINT16  status;     /* PHY auto-negotiation status */
    UINT16  ix;     /* a counter */
    int     retVal;     /* holder for return value */

    regAddr = MII_AN_ADS_REG;

    if (m5200FecMiiRead (pDrvCtrl, phyAddr, regAddr, &phyAds) != OK)
        return (ERROR);

    FEC_END_LOG (FEC_END_DBG_MII, ("m5200FecMiiAnRun phyAds=0x%x\n"),
                   phyAds, 0, 0, 0, 0, 0);

    /* disable the next page function */

    phyAds &= (~MII_NP_NP);

    if (m5200FecMiiWrite (pDrvCtrl, phyAddr, regAddr, phyAds) != OK)
        return (ERROR);

    /* Read ANER to clear status from previous operations */

    regAddr = MII_AN_EXP_REG;
    if (m5200FecMiiRead (pDrvCtrl, phyAddr, regAddr, &status) != OK)
        return (ERROR);

    FEC_END_LOG (FEC_END_DBG_MII, ("m5200FecMiiAnRun expStat=0x%x\n"),
                   status, 0, 0, 0, 0, 0);

    /* Read ANAR */

    regAddr = MII_AN_ADS_REG;
    if (m5200FecMiiRead (pDrvCtrl, phyAddr, regAddr, &phyAds) != OK)
        return (ERROR);

    /* store the current registers values */

    pDrvCtrl->phyInfo->miiRegs.phyAds = phyAds;

    /*
     * start the auto-negotiation process, possibly
     * following the order the user dictated.
     */

    for (ix = 0; ; ix++)
        {
        if (FEC_END_PHY_FLAGS_ISSET (FEC_END_PHY_TBL))
            {
            /* check we're not at the end of the user table */

            if ((m5200FecPhyAnOrderTbl [ix]) == -1)
                return (ERROR);

            /* just negotiate one ability at a time */

            phyAds &= ~MII_TECH_MASK;

            /* translate user settings */

            phyAds |= miiAnLookupTbl [(m5200FecPhyAnOrderTbl [ix])];

            /* check the PHY has the desidered ability */

            if (!(phyAds & pDrvCtrl->phyInfo->miiRegs.phyAds))
                continue;

            /* set the ANAR accordingly */

            regAddr = MII_AN_ADS_REG;
            if (m5200FecMiiWrite (pDrvCtrl, phyAddr, regAddr, phyAds) != OK)
                return (ERROR);
            }
        else
            {
            /* check the PHY flags and possibly mask some abilities off */

            if (!(FEC_END_PHY_FLAGS_ISSET (FEC_END_PHY_FD)))
                phyAds &= ~(MII_TECH_10BASE_FD | MII_TECH_100BASE_TX_FD |
                            MII_TECH_100BASE_T4);

            if (!(FEC_END_PHY_FLAGS_ISSET (FEC_END_PHY_HD)))
                phyAds &= ~(MII_TECH_10BASE_T | MII_TECH_100BASE_TX);

            if (!(FEC_END_PHY_FLAGS_ISSET (FEC_END_PHY_100)))
                phyAds &= ~(MII_TECH_100BASE_TX | MII_TECH_100BASE_TX_FD |
                            MII_TECH_100BASE_T4);

            if (!(FEC_END_PHY_FLAGS_ISSET (FEC_END_PHY_10)))
                phyAds &= ~(MII_TECH_10BASE_T | MII_TECH_10BASE_FD);

            /* Write ANAR */

            regAddr = MII_AN_ADS_REG;
            if (m5200FecMiiWrite (pDrvCtrl, phyAddr, regAddr, phyAds) != OK)
                return (ERROR);

            /* store the current registers values */

            pDrvCtrl->phyInfo->miiRegs.phyAds = phyAds;

            FEC_END_LOG (FEC_END_DBG_MII, ("m5200FecMiiAnRun phyAds=0x%x\n"),
                        phyAds, 0, 0, 0, 0, 0);

            }

        /*
        * start the auto-negotiation process: return
        * only in case of fatal error.
        */

        retVal = m5200FecMiiAnStart (pDrvCtrl, phyAddr);
        if (retVal == ERROR)
            return (ERROR);

        /*
        * in case of failure, we return only if we're using
        * the standard auto-negotiation process.
        */

        if (retVal == MII_AN_FAIL)
            {
            if (!(FEC_END_PHY_FLAGS_ISSET (FEC_END_PHY_TBL)))
                return (retVal);
            else
                continue;
            }

        /* check the negotiation was successful */

        if (m5200FecMiiAnCheck (pDrvCtrl, phyAddr) == OK)
            return (OK);

        /*
        * we are here if the negotiation went wong:
        * if the user did not force any priority order,
        * we return ERROR, as all the PHY abilities
        * were negotiated at once.
        */

        if (!(FEC_END_PHY_FLAGS_ISSET (FEC_END_PHY_TBL)))
            return (ERROR);
        }

    return (OK);
    }

/*******************************************************************************
*
* m5200FecMiiAnStart - start the auto-negotiation process
*
* This routine starts the auto-negotiation process for the PHY whose
* address is specified in the parameter <phyAddr>.
*
* RETURNS: OK, ERROR in case of fatal errors or MII_AN_FAIL, if the
* auto-negotiation did not complete within a reasonable amount of time.
*/

LOCAL STATUS m5200FecMiiAnStart
    (
    DRV_CTRL *  pDrvCtrl,       /* pointer to DRV_CTRL structure */
    UINT8   phyAddr     /* address of a PHY */
    )
    {
    UINT16  data;       /* data to be written to the control reg */
    UINT8   regAddr;    /* PHY register */
    UINT16  phyStatus;  /* holder for the PHY status */
    UINT16  ix = 0;     /* a counter */

    regAddr = MII_STAT_REG;

    if (m5200FecMiiRead (pDrvCtrl, phyAddr, regAddr, &phyStatus) != OK)
        return (ERROR);

    /* check the PHY has this ability */

    if ((phyStatus & MII_SR_AUTO_SEL) != MII_SR_AUTO_SEL)
        {
        FEC_END_LOG (FEC_END_DBG_MII, ("m5200FecMiiAnStart phy not \n\
                        auto neg capable\n"),
                        0, 0, 0, 0, 0, 0);

        return (ERROR);
        }

    /* restart the auto-negotiation process */

    regAddr = MII_CTRL_REG;
    data = (MII_CR_RESTART | MII_CR_AUTO_EN);

    if (m5200FecMiiWrite (pDrvCtrl, phyAddr, regAddr, data) != OK)
        return (ERROR);

    /* save status info */

    pDrvCtrl->phyInfo->miiRegs.phyCtrl = data;

    /* let's check the PHY status */

    regAddr = MII_STAT_REG;

    /* spin until it is done */

    do
        {
        taskDelay (1);

        if (ix++ >= FEC_END_PHY_MAX_SEC)
            {
            FEC_END_LOG (FEC_END_DBG_ERR,
                    ("m5200FecMiiAnStart no link - error\n"),
                    0, 0, 0, 0, 0, 0);
            break;
            }

        if (m5200FecMiiRead (pDrvCtrl, phyAddr, regAddr, &phyStatus) != OK)
            return (ERROR);

        if ((phyStatus & MII_SR_LINK_STATUS) != MII_SR_LINK_STATUS)
            {
            /* wait for a while */

            FEC_END_LOG (FEC_END_DBG_MII,
                    ("m5200FecMiiAnStart no link\n"),
                    0, 0, 0, 0, 0, 0);
            taskDelay (sysClkRateGet());
            }
        else
            {
            FEC_END_LOG (FEC_END_DBG_MII,
                    ("m5200FecMiiAnStart link up!\n"),
                    0, 0, 0, 0, 0, 0);
            }

        } while ((phyStatus & MII_SR_AUTO_NEG) != MII_SR_AUTO_NEG);

    if (ix >= FEC_END_PHY_MAX_SEC)
        {
        FEC_END_LOG (FEC_END_DBG_MII,
                ("m5200FecMiiAnStart auto neg fail\n"),
                0, 0, 0, 0, 0, 0);
        return (MII_AN_FAIL);
        }
    else
        {
        FEC_END_LOG (FEC_END_DBG_MII,
                ("m5200FecMiiAnStart auto neg done phyStat=0x%x\n"),
                phyStatus, 0, 0, 0, 0, 0);
        }

    return (OK);
    }

/*******************************************************************************
*
* m5200FecMiiBasicCheck - run a basic check on the PHY status register
*
* This routine runs a basic check on the PHY status register to
* ensure a valid link has been established and no faults have been
* detected.
*
* RETURNS: OK, MII_STAT_FAIL, if an error was reported in the PHY's
* status register, or ERROR, in case of unrecoverable errors.
*/

LOCAL STATUS m5200FecMiiBasicCheck
    (
    DRV_CTRL *  pDrvCtrl,       /* pointer to DRV_CTRL structure */
    UINT8   phyAddr     /* address of a PHY */
    )
    {
    UINT8   regAddr;    /* PHY register */
    UINT16  phyStatus;  /* holder for the PHY status */

    /* let's check the PHY status */

    regAddr = MII_STAT_REG;
    if (m5200FecMiiRead (pDrvCtrl, phyAddr, regAddr, &phyStatus) != OK)
        return (ERROR);

    if ((phyStatus & MII_SR_LINK_STATUS) != MII_SR_LINK_STATUS)
        {
        /* wait for a while */

        taskDelay (10);

        /* we need to read it twice, as this's a latched function */

        if (m5200FecMiiRead (pDrvCtrl, phyAddr, regAddr, &phyStatus) != OK)
            return (ERROR);

        if ((phyStatus & MII_SR_LINK_STATUS) != MII_SR_LINK_STATUS)
            {
            FEC_END_LOG (FEC_END_DBG_MII, ("m5200FecMiiBasicCheck \n\
                        phy stat=0x%x\n"),
                        phyStatus, 0, 0, 0, 0, 0);

            return (MII_STAT_FAIL);
            }
        }

    FEC_END_LOG (FEC_END_DBG_MII, ("m5200FecMiiBasicCheck\n\
                   Link up! status=0x%x\n"),
                   phyStatus, 0, 0, 0, 0, 0);

    /* check for remote fault condition */

    if (m5200FecMiiRead (pDrvCtrl, phyAddr, regAddr, &phyStatus) != OK)
        return (ERROR);

    if (m5200FecMiiRead (pDrvCtrl, phyAddr, regAddr, &phyStatus) != OK)
        return (ERROR);

    if ((phyStatus & MII_SR_REMOTE_FAULT) == MII_SR_REMOTE_FAULT)
        {
        FEC_END_LOG (FEC_END_DBG_MII, ("m5200FecMiiBasicCheck remote fault\n"),
                        0, 0, 0, 0, 0, 0);
        return (MII_STAT_FAIL);
        }

    /* store the current registers values */

    pDrvCtrl->phyInfo->miiRegs.phyStatus = phyStatus;

    return (OK);
    }

/*******************************************************************************
*
* m5200FecMiiAnCheck - check the auto-negotiation process result
*
* This routine checks the auto-negotiation process has completed
* successfully and no faults have been detected by any of the PHYs
* engaged in the process.
*
* RETURNS: OK or ERROR.
*/

LOCAL STATUS m5200FecMiiAnCheck
    (
    DRV_CTRL *  pDrvCtrl,       /* pointer to DRV_CTRL structure */
    UINT8   phyAddr     /* address of a PHY */
    )
    {
    UINT8   regAddr;    /* PHY register */
    UINT16  phyAds;     /* PHY advertisement register value */
    UINT16  phyPrtn;    /* PHY partner ability register value */
    UINT16  phyExp;     /* PHY expansion register value */
    UINT16  negAbility; /* abilities after negotiation */
    int     retVal;     /* convenient holder for return value */

    /* run a check on the status bits of basic registers only */

    retVal = m5200FecMiiBasicCheck (pDrvCtrl, phyAddr);
    if (retVal != OK)
        return (retVal);

    /* we know the auto-negotiation process has terminated */

    regAddr = MII_AN_EXP_REG;

    if (m5200FecMiiRead (pDrvCtrl, phyAddr, regAddr, &phyExp) != OK)
        return (ERROR);

    /* check for faults detected by the parallel function */

    if ((phyExp & MII_EXP_FAULT) == MII_EXP_FAULT)
        {
        FEC_END_LOG (FEC_END_DBG_MII, ("m5200FecMiiAnCheck:\n\
                        fault expStat=0x%x\n"),
                        phyExp, 0, 0, 0, 0, 0);

        return (ERROR);
        }

    /* check for remote faults */

    regAddr = MII_AN_PRTN_REG;

    if (m5200FecMiiRead (pDrvCtrl, phyAddr, regAddr, &phyPrtn) != OK)
        return (ERROR);

    if ((phyPrtn & MII_BP_FAULT) == MII_BP_FAULT)
        {
        FEC_END_LOG (FEC_END_DBG_MII, ("m5200FecMiiAnCheck partner stat=0x%x\n"),
                        phyPrtn, 0, 0, 0, 0, 0);

        return (ERROR);
        }


    regAddr = MII_AN_ADS_REG;
    if (m5200FecMiiRead (pDrvCtrl, phyAddr, regAddr, &phyAds) != OK)
        return (ERROR);

    /* store the current registers values */

    pDrvCtrl->phyInfo->miiRegs.phyAds = phyAds;
    pDrvCtrl->phyInfo->miiRegs.phyPrtn = phyPrtn;
    pDrvCtrl->phyInfo->miiRegs.phyExp = phyExp;

    /* find out the max common abilities */

    negAbility = phyPrtn & phyAds & MII_TECH_MASK;

    FEC_END_LOG (FEC_END_DBG_MII, ("m5200FecMiiAnCheck phyAds=0x%x\n\
                   phyPrtn=0x%x common=0x%x phyExp=0x%x\n"),
                   phyAds,
                   phyPrtn,
                   negAbility,
                   phyExp, 0, 0);

    if (negAbility & MII_TECH_100BASE_TX_FD)
        {
        pDrvCtrl->phyInfo->phySpeed = FEC_END_100MBS;
        pDrvCtrl->phyInfo->phyMode = FEC_END_PHY_FD;

        FEC_END_LOG (FEC_END_DBG_MII, ("m5200FecMiiAnCheck speed=%d mode=%s\n"),
                        pDrvCtrl->phyInfo->phySpeed,
                        (int) "full duplex", 0, 0, 0, 0);
        }
    else if ((negAbility & MII_TECH_100BASE_T4) ||
             (negAbility & MII_TECH_100BASE_TX))
        {
        pDrvCtrl->phyInfo->phySpeed = FEC_END_100MBS;
        pDrvCtrl->phyInfo->phyMode = FEC_END_PHY_HD;

        FEC_END_LOG (FEC_END_DBG_MII, ("m5200FecMiiAnCheck speed=%d mode=%s\n"),
                        pDrvCtrl->phyInfo->phySpeed,
                        (int) "half duplex", 0, 0, 0, 0);
        }
    else if (negAbility & MII_TECH_10BASE_FD)
        {
        pDrvCtrl->phyInfo->phySpeed = FEC_END_10MBS;
        pDrvCtrl->phyInfo->phyMode = FEC_END_PHY_FD;

        FEC_END_LOG (FEC_END_DBG_MII, ("m5200FecMiiAnCheck speed=%d mode=%s\n"),
                        pDrvCtrl->phyInfo->phySpeed,
                        (int) "full duplex", 0, 0, 0, 0);
        }
    else if (negAbility & MII_TECH_10BASE_T)
        {
        pDrvCtrl->phyInfo->phySpeed = FEC_END_10MBS;
        pDrvCtrl->phyInfo->phyMode = FEC_END_PHY_HD;

        FEC_END_LOG (FEC_END_DBG_MII, ("m5200FecMiiAnCheck speed=%d mode=%s\n"),
                        pDrvCtrl->phyInfo->phySpeed,
                        (int) "half duplex", 0, 0, 0, 0);
        }
    else
        {
        FEC_END_LOG (FEC_END_DBG_MII, ("m5200FecMiiAnCheck fail!\n"),
                        0, 0, 0, 0, 0, 0);
        return (ERROR);
        }

    /* handle some flags */

    if (pDrvCtrl->phyInfo->phySpeed == FEC_END_100MBS)
        FEC_END_PHY_FLAGS_SET (FEC_END_PHY_100);
    else
        FEC_END_PHY_FLAGS_CLEAR (FEC_END_PHY_100);

    if (pDrvCtrl->phyInfo->phyMode == FEC_END_PHY_FD)
       FEC_END_PHY_FLAGS_SET (FEC_END_PHY_FD);
    else
       FEC_END_PHY_FLAGS_CLEAR (FEC_END_PHY_FD);

    FEC_END_LOG (FEC_END_DBG_MII, ("m5200FecMiiAnCheck OK\n"),
                   0, 0, 0, 0, 0, 0);

    return (OK);
    }

/*******************************************************************************
*
* m5200FecMiiModeForce - force an operating mode for the PHY
*
* This routine forces an operating mode for the PHY whose address is
* specified in the parameter <phyAddr>. It also calls m5200FecMiiBasicCheck()
* to ensure a valid link has been established.
*
* RETURNS: OK or ERROR.
*/

LOCAL STATUS m5200FecMiiModeForce
    (
    DRV_CTRL *  pDrvCtrl,       /* pointer to DRV_CTRL structure */
    UINT8   phyAddr     /* address of a PHY */
    )
    {
    UINT16  data;       /* data to be written to the control reg */
    UINT8   regAddr;    /* PHY register */
    UINT16  phyStatus;  /* holder for the PHY status */

    FEC_END_LOG (FEC_END_DBG_MII, ("m5200FecMiiModeForce \n"),
                   0, 0, 0, 0, 0, 0);

    /* find out what abilities the PHY features */

    regAddr = MII_STAT_REG;
    if (m5200FecMiiRead (pDrvCtrl, phyAddr, regAddr, &phyStatus) != OK)
        return (ERROR);

    /*
     * force as a high priority as possible operating
     * mode, not overlooking what the user dictated.
     */

    data = MII_CR_NORM_EN;

    if ((phyStatus & (MII_SR_TX_FULL_DPX | MII_SR_TX_HALF_DPX)) &&
        (FEC_END_PHY_FLAGS_ISSET (FEC_END_PHY_100)))
        {
        data |= MII_CR_100;
        pDrvCtrl->phyInfo->phySpeed = FEC_END_100MBS;
        }

    if ((phyStatus & (MII_SR_TX_FULL_DPX | MII_SR_10T_FULL_DPX | MII_SR_T4)) &&
        (FEC_END_PHY_FLAGS_ISSET (FEC_END_PHY_FD)))
        {
        pDrvCtrl->phyInfo->phyMode = FEC_END_PHY_FD;
        data |= MII_CR_FDX;
        }

    pDrvCtrl->phyInfo->miiRegs.phyCtrl = data;

    regAddr = MII_CTRL_REG;

    if (m5200FecMiiWrite (pDrvCtrl, phyAddr, regAddr, data) != OK)
        return (ERROR);

    /* run a check on the status bits of basic registers only */

    if (m5200FecMiiBasicCheck (pDrvCtrl, phyAddr) != OK)
        return (ERROR);

    /* handle some flags */

    if (pDrvCtrl->phyInfo->phySpeed == FEC_END_100MBS)
        FEC_END_PHY_FLAGS_SET (FEC_END_PHY_100);
    else
        FEC_END_PHY_FLAGS_CLEAR (FEC_END_PHY_100);

    if (pDrvCtrl->phyInfo->phyMode == FEC_END_PHY_FD)
        FEC_END_PHY_FLAGS_SET (FEC_END_PHY_FD);
    else
        FEC_END_PHY_FLAGS_CLEAR (FEC_END_PHY_FD);

    FEC_END_LOG (FEC_END_DBG_MII, ("m5200FecMiiModeForce speed=%d mode=%d\n"),
                   pDrvCtrl->phyInfo->phySpeed,
                   pDrvCtrl->phyInfo->phyMode,
                   0, 0, 0, 0);

    return (OK);
    }

/*******************************************************************************
*
* m5200FecMiiDefForce - force a default operating mode for the PHY
*
* This routine forces a default operating mode for the PHY whose address is
* specified in the parameter <phyAddr>. It also calls m5200FecMiiBasicCheck()
* to ensure a valid link has been established.
*
* RETURNS: OK or ERROR.
*/

LOCAL STATUS m5200FecMiiDefForce
    (
    DRV_CTRL *  pDrvCtrl,       /* pointer to DRV_CTRL structure */
    UINT8   phyAddr     /* address of a PHY */
    )
    {
    UINT16  data;       /* data to be written to the control reg */
    UINT8   regAddr;    /* PHY register */
    int     retVal;     /* convenient holder for return value */
    UINT16  phyStatus;  /* holder for the PHY status */

    FEC_END_LOG (FEC_END_DBG_MII, ("m5200FecMiiDefForce \n"),
                   0, 0, 0, 0, 0, 0);

    /* translate user settings */

    data = miiCrLookupTbl [(pDrvCtrl->phyInfo->phyDefMode)];
    
    data |= MII_CR_AUTO_EN;

    /* find out what abilities the PHY features */

    regAddr = MII_STAT_REG;
    if (m5200FecMiiRead (pDrvCtrl, phyAddr, regAddr, &phyStatus) != OK)
        return (ERROR);

    if (data & MII_CR_100)
        {
        if (!(phyStatus &
              (MII_SR_TX_HALF_DPX | MII_SR_TX_FULL_DPX | MII_SR_T4)))
            return (FEC_END_PHY_NO_ABLE);

        pDrvCtrl->phyInfo->phySpeed = FEC_END_100MBS;
        }
    else
        {
        if (!(phyStatus & (MII_SR_10T_HALF_DPX | MII_SR_10T_FULL_DPX)))
            return (FEC_END_PHY_NO_ABLE);

        /* handle phy flags */

        FEC_END_PHY_FLAGS_CLEAR (FEC_END_PHY_100);

        pDrvCtrl->phyInfo->phySpeed = FEC_END_10MBS;
        }

    if (data & MII_CR_FDX)
        {
        if (!(phyStatus &
                (MII_SR_10T_FULL_DPX | MII_SR_TX_FULL_DPX | MII_SR_T4)))
            return (FEC_END_PHY_NO_ABLE);

        pDrvCtrl->phyInfo->phyMode = FEC_END_PHY_FD;
        }
    else
        {
        if (!(phyStatus & (MII_SR_10T_HALF_DPX | MII_SR_TX_HALF_DPX)))
            return (FEC_END_PHY_NO_ABLE);

        /* handle phy flags */

        FEC_END_PHY_FLAGS_CLEAR (FEC_END_PHY_FD);

        pDrvCtrl->phyInfo->phyMode = FEC_END_PHY_HD;
        }

    pDrvCtrl->phyInfo->miiRegs.phyCtrl = data;

    regAddr = MII_CTRL_REG;

    if (m5200FecMiiWrite (pDrvCtrl, phyAddr, regAddr, data) != OK)
        return (ERROR);

    /* run a check on the status bits of basic registers only */

    retVal = m5200FecMiiBasicCheck (pDrvCtrl, phyAddr);

    FEC_END_LOG (FEC_END_DBG_MII, ("m5200FecMiiDefForce speed=%d mode=%d\n"),
                   pDrvCtrl->phyInfo->phySpeed,
                   pDrvCtrl->phyInfo->phyMode,
                   0, 0, 0, 0);

    return (retVal);
    }

/*******************************************************************************
*
* m5200FecMCastAddrAdd - add a multicast address for the device
*
* This routine adds a multicast address to whatever the driver
* is already listening for.
*
* SEE ALSO: m5200FecMCastAddrDel() m5200FecMCastAddrGet()
*
* RETURNS: OK or ERROR.
*/

LOCAL STATUS m5200FecMCastAddrAdd
    (
    DRV_CTRL *  pDrvCtrl,       /* pointer to DRV_CTRL structure */
    char *      pAddr       /* address to be added */
    )
    {
    int     retVal;     /* convenient holder for return value */

    FEC_END_LOG (FEC_END_DBG_IOCTL, ("m5200FecMCastAddrAdd addr = 0x%x 0x%x\n\
                     0x%x 0x%x 0x%x 0x%x\n"),
                     (int) (*pAddr + 0), (int) (*pAddr + 1),
                     (int) (*pAddr + 2), (int) (*pAddr + 3),
                     (int) (*pAddr + 4), (int) (*pAddr + 5));

    retVal = etherMultiAdd (&pDrvCtrl->endObj.multiList, pAddr);

    if (retVal == ENETRESET)
        {
        pDrvCtrl->endObj.nMulti++;

        return (m5200FecHashTblAdd (pDrvCtrl, pAddr));
        }

    return ((retVal == OK) ? OK : ERROR);
    }

/*****************************************************************************
*
* m5200FecMCastAddrDel - delete a multicast address for the device
*
* This routine deletes a multicast address from the current list of
* multicast addresses.
*
* SEE ALSO: m5200FecMCastAddrAdd() m5200FecMCastAddrGet()
*
* RETURNS: OK or ERROR.
*/

LOCAL STATUS m5200FecMCastAddrDel
    (
    DRV_CTRL *  pDrvCtrl,       /* pointer to DRV_CTRL structure */
    char *      pAddr       /* address to be deleted */
    )
    {
    int     retVal;     /* convenient holder for return value */

    FEC_END_LOG (FEC_END_DBG_IOCTL, ("m5200FecMCastAddrDel addr = 0x%x 0x%x\n\
                     0x%x 0x%x 0x%x 0x%x\n"),
                     (int) (*pAddr + 0), (int) (*pAddr + 1),
                     (int) (*pAddr + 2), (int) (*pAddr + 3),
                     (int) (*pAddr + 4), (int) (*pAddr + 5));

    retVal = etherMultiDel (&pDrvCtrl->endObj.multiList, pAddr);

    if (retVal == ENETRESET)
        {
        pDrvCtrl->endObj.nMulti--;

        /* stop the FEC */

        /* Prevent queue of new transmissions as we are stopping */

        pDrvCtrl->stoppingTx = TRUE;

        if (m5200FecStop (pDrvCtrl) != OK)
            return (ERROR);

        /* populate the hash table */

        retVal = m5200FecHashTblPopulate (pDrvCtrl);

        /* restart the FEC */

        if (m5200FecStart (pDrvCtrl) != OK)
            return (ERROR);
        }

    return ((retVal == OK) ? OK : ERROR);
    }

/*******************************************************************************
*
* m5200FecMCastAddrGet - get the current multicast address list
*
* This routine returns the current multicast address list in <pTable>
*
* SEE ALSO: m5200FecMCastAddrAdd() m5200FecMCastAddrDel()
*
* RETURNS: OK or ERROR.
*/

LOCAL STATUS m5200FecMCastAddrGet
    (
    DRV_CTRL *  pDrvCtrl,       /* pointer to DRV_CTRL structure */
    MULTI_TABLE *pTable     /* table into which to copy addresses */
    )
    {
    FEC_END_LOG (FEC_END_DBG_IOCTL, ("m5200FecMCastAddrGet\n"),
                     0, 0, 0, 0, 0, 0);

    return (etherMultiGet (&pDrvCtrl->endObj.multiList, pTable));
    }

/*******************************************************************************
*
* m5200FecHashTblAdd - add an entry to the hash table
*
* This routine adds an entry to the hash table for the address pointed to
* by <pAddr>.
*
* RETURNS: OK, or ERROR.
*/

LOCAL STATUS m5200FecHashTblAdd
    (
    DRV_CTRL *  pDrvCtrl,       /* pointer to DRV_CTRL structure */
    char *      pAddr       /* address to be added */
    )
    {
    UINT32  crcVal;     /* convenient holder for the CRC value */
    UINT32  csrOldVal;  /* current value in the hash register */
    UINT16  offset;     /* offset into the Internal RAM */
    UINT32  csrVal;     /* value to be written to the hash register */

    FEC_END_LOG (FEC_END_DBG_IOCTL, ("m5200FecHashTblAdd addr = 0x%x 0x%x\n\
                     0x%x 0x%x 0x%x 0x%x\n"),
                     (int) (*pAddr + 0), (int) (*pAddr + 1),
                     (int) (*pAddr + 2), (int) (*pAddr + 3),
                     (int) (*pAddr + 4), (int) (*pAddr + 5));

    /* get the CRC for the given address */

    crcVal = m5200FecCrcGet (pAddr);

    /* get the value to be written to the proper hash register */

    if (m5200FecHashRegValGet (pDrvCtrl, crcVal, &csrVal, &offset) != OK)
        return (ERROR);

    FEC_END_LOG (FEC_END_DBG_IOCTL, ("m5200FecHashTblAdd hashReg=0x%x\n"),
                     (int) csrVal,
                     0, 0, 0, 0, 0);

    /*
     * write to the proper hash register: be careful not
     * to override the current value.
     */

    FEC_END_CSR_RD (offset, csrOldVal);
    FEC_END_CSR_WR (offset, (csrOldVal | csrVal));

    return (OK);
    }

/*******************************************************************************
*
* m5200FecHashTblPopulate - populate the hash table
*
* This routine populates the hash table with the entries found in the
* multicast table. We have to reset the hash registers first, and
* populate them again, as more than one address may be mapped to
* the same bit.
*
* RETURNS: OK, or ERROR.
*/

LOCAL STATUS m5200FecHashTblPopulate
    (
    DRV_CTRL *  pDrvCtrl       /* pointer to DRV_CTRL structure */
    )
    {
    UINT32  crcVal;     /* convenient holder for the CRC value */
    UINT16  offset;     /* offset into the Internal RAM */
    UINT32  csrVal;     /* value to be written to the hash register */
    UINT32  csrOldVal;  /* current value in the hash register */
    ETHER_MULTI *   mCastNode = NULL;

    /* reset the hash table registers first */

    FEC_END_CSR_WR (FEC_END_HASH_H_OFF, 0);
    FEC_END_CSR_WR (FEC_END_HASH_L_OFF, 0);

    /* scan the multicast address list */

    for (mCastNode = (ETHER_MULTI *) lstFirst (&pDrvCtrl->endObj.multiList);
         mCastNode != NULL;
         mCastNode = (ETHER_MULTI *) lstNext (&mCastNode->node))
        {
        /* get the CRC for the current address in the list */

        crcVal = m5200FecCrcGet ((char *) mCastNode->addr);

        /* get the value to be written to the proper hash register */

        if (m5200FecHashRegValGet (pDrvCtrl, crcVal, &csrVal, &offset) != OK)
            {
            return (ERROR);
            }

        /*
        * write to the proper hash register: be careful not
        * to override the current value.
        */

        FEC_END_CSR_RD (offset, csrOldVal);
        FEC_END_CSR_WR (offset, (csrOldVal | csrVal));
        }

    return (OK);
    }

/*******************************************************************************
*
* m5200FecCrcGet - compute the cyclic redoundant code
*
* This routine computes the 32-bit cyclic redoundant code (CRC) for the
* 6-byte array pointed to by <pAddr>.
*
* RETURNS: The 32-bit value representing the CRC.
*/

LOCAL UINT32 m5200FecCrcGet
    (
    char * pAddr
    )
    {
    UINT32 crc = INIT_REFLECTED;
    UINT32 len = 6;

    while (len--)
        crc = crctable[(crc ^ *pAddr++) & 0xFFL] ^ (crc >> 8);

    return crc ^ XOROT;
    }


/*******************************************************************************
*
* m5200FecHashRegValGet - get the value to be written to the hash register
*
* This routine computes the value to be written to the hash register
* for the 4-byte value specified by <crcVal>. It also figures out which
* of the two 32-bit hash register that value should be written to, and
* returns its offset into the internal RAM in the variable pointed to
* by <offset>.
*
* RETURNS: OK, always.
*/

LOCAL STATUS m5200FecHashRegValGet
    (
    DRV_CTRL *  pDrvCtrl,   /* pointer to DRV_CTRL structure */
    UINT32  crcVal,     /* CRC value */
    UINT32 *    csrVal,     /* value to be written to the hash register */
    UINT16 *    offset      /* offset into the Internal RAM */
    )
    {
    UINT32  hashIndex;  /* index into the hash register */

    /*
     * bits 26-30 in the CRC value determine the index in the
     * hash register specified by bit 31.
     */

    hashIndex = ((crcVal & FEC_END_HASH_MASK) >> FEC_END_HASH_SHIFT);

    *csrVal = (0x80000000 >> hashIndex);

    /* It appears that starting at register 0x0E08 HASH_TABLE_HIGH high
     * order bit is really bit 0 of the hash table and low order bit
     * of 0x0E0C HASH_TABLE_LOW is bit 63 of the hash table.
     *
     * 0x0E0C  HASH_TABLE_LOW  [32:64]   ==>   hash_index[63:32]
     * 0x0E08  HASH_TABLE_HIGH [0 :31]   ==>   hash_index[31: 0]
     *
     * So this means that bit 31 of the CRC if 1 points to
     * the HASH_TABLE_LOW register, not the HASH_TABLE_HIGH.
     */

    *offset = (crcVal & 0x80000000) ? FEC_END_HASH_L_OFF : FEC_END_HASH_H_OFF;

    return (OK);
    }

/*****************************************************************************
*
* m5200FecPollSend - transmit a packet in polled mode
*
* This routine is called by a user to try and send a packet on the
* device. It sends a packet directly on the network, without having to
* go through the normal process of queuing a packet on an output queue
* and then waiting for the device to decide to transmit it.
*
* These routine should not call any kernel functions.
*
* SEE ALSO: m5200FecPollReceive()
*
* RETURNS: OK or EAGAIN.
*/

LOCAL STATUS m5200FecPollSend
    (
    DRV_CTRL    *pDrvCtrl,  /* pointer to DRV_CTRL structure */
    M_BLK_ID    pMblk       /* pointer to the mBlk/cluster pair */
    )
    {
    UINT16      pktType = 0;    /* packet type (unicast or multicast) */
    int         retVal;     /* holder for return value */
    char *      pBuf = NULL;    /* pointer to data to be sent */
    FEC_END_TBD_ID  pTbd = NULL;    /* pointer to the current ready TBD */
    int         ix = 0;     /* a counter */
    int         len = 0;    /* length of data to be sent */
    UINT16      tbdStatus;  /* holder for the TBD status */
    int level;

    FEC_END_LOG (FEC_END_DBG_POLL, ("m5200FecPollSend\n"), 0, 0, 0, 0, 0, 0);

    if (pMblk == NULL)
        {
        goto m5200FecPollSendError;
        }

    /* set the packet type to multicast or unicast */

    if (pMblk->mBlkHdr.mData[0] & (UINT8) 0x01)
        {
        pktType = PKT_TYPE_MULTI;
        }
    else
        {
        pktType = PKT_TYPE_UNI;
        }

    /* get the current free TBD */

    pTbd = m5200FecTbdGet (pDrvCtrl);

    /* get our own cluster */

    pBuf = (char *)pDrvCtrl->pTxPollBuf;

    if ((pTbd == NULL) || (pBuf == NULL))
        {
        goto m5200FecPollSendError;
        }

    /* copy data but do not free the Mblk */

    len = netMblkToBufCopy (pMblk, pBuf, NULL);
    len = max (ETHERSMALL, len);

    /* flush the cache, if necessary */

    FEC_END_CACHE_FLUSH (pBuf, len);

    /* set up the current TBD */

    FEC_END_BD_LONG_WR (pTbd, FEC_END_BD_ADDR_OFF, (UINT32) pBuf);

    FEC_END_BD_WORD_WR (pTbd, FEC_END_BD_LEN_OFF, (UINT32) len);

    FEC_END_BD_WORD_RD (pTbd, FEC_END_BD_STAT_OFF, tbdStatus);

    FEC_END_BD_WORD_WR (pTbd, FEC_END_BD_STAT_OFF,
                        (FEC_END_TBD_LAST | FEC_END_TBD_INT | FEC_END_TBD_CRC |
                         FEC_END_TBD_RDY | tbdStatus));

    /* kick the transmitter without interrupts */

    level = intLock();
    if (TaskStart( pDrvCtrl->xmitTaskNo, TASK_AUTOSTART_ENABLE, pDrvCtrl->xmitTaskNo, FALSE) != TASK_ERR_NO_ERR)
        {
        intUnlock(level);
        FEC_END_LOG (FEC_END_DBG_POLL,
                     ("m5200FecPollSend: failed to start TX task\n"),
                     1, 2, 3, 4, 5, 6);
        goto m5200FecPollSendError;
        }
    intUnlock(level);

    /* Flush the write pipe */

    CACHE_PIPE_FLUSH ();

    /* up-date statistics */

    if (pktType == PKT_TYPE_MULTI)
        {
        pDrvCtrl->endObj.mib2Tbl.ifOutNUcastPkts += 1;
        }
    else
        {
        END_ERR_ADD (&pDrvCtrl->endObj, MIB2_OUT_UCAST, +1);
        }

    do
        {
        retVal = m5200FecTbdCheck (pDrvCtrl, pTbd);

        if (ix++ == FEC_END_WAIT_MAX)
            break;

        } while (retVal == FEC_END_TBD_BUSY);

        /* correct statistics, if necessary */

    if ((retVal == FEC_END_TBD_ERROR) || (ix == FEC_END_WAIT_MAX))
        {
        END_ERR_ADD (&pDrvCtrl->endObj, MIB2_OUT_ERRS, +1);

        if (pktType == PKT_TYPE_MULTI)
            {
            pDrvCtrl->endObj.mib2Tbl.ifOutNUcastPkts -= 1;
            }
        else
            {
            END_ERR_ADD (&pDrvCtrl->endObj, MIB2_OUT_UCAST, -1);
            }
        }

    /* clean this buffer descriptor, mirror m5200FecTbdInit() */

    FEC_END_BD_WORD_WR (pTbd, FEC_END_BD_STAT_OFF, 0);

    /* Flush the write pipe */

    CACHE_PIPE_FLUSH ();

    /* update some indeces for a correct handling of the TBD ring */

    pDrvCtrl->tbdIndex = (pDrvCtrl->tbdIndex + 1) % (pDrvCtrl->tbdNum);

    pDrvCtrl->usedTbdIndex = (pDrvCtrl->usedTbdIndex + 1) % (pDrvCtrl->tbdNum);

    if (ix == FEC_END_WAIT_MAX)
        return (EAGAIN);

    return (OK);

m5200FecPollSendError:

    END_ERR_ADD (&pDrvCtrl->endObj, MIB2_OUT_ERRS, +1);

    return (EAGAIN);
    }

/*******************************************************************************
*
* m5200FecPollReceive - receive a packet in polled mode
*
* This routine is called by a user to try and get a packet from the
* device. It returns EAGAIN if no packet is available. The caller must
* supply a M_BLK_ID with enough space to contain the received packet. If
* enough buffer is not available then EAGAIN is returned.
*
* These routine should not call any kernel functions.
*
* SEE ALSO: m5200FecPollSend()
*
* RETURNS: OK or EAGAIN.
*/

LOCAL STATUS m5200FecPollReceive
    (
    DRV_CTRL    *pDrvCtrl,      /* pointer to DRV_CTRL structure */
    M_BLK_ID    pMblk           /* pointer to the mBlk/cluster pair */
    )
    {
    int         retVal = OK;    /* holder for return value */
    FEC_END_RBD_ID  pRbd = NULL;    /* generic rbd pointer */
    UINT16      rbdStatus = 0;  /* holder for the status of this RBD */
    UINT16      rbdLen = 0; /* number of bytes received */
    char *      pBuf = NULL;    /* a rx data pointer */
    char *      pData = NULL;   /* a rx data pointer */

    FEC_END_LOG (FEC_END_DBG_POLL, ("m5200FecPollReceive\n"), 0, 0, 0, 0, 0, 0);

    if ((pMblk->mBlkHdr.mFlags & M_EXT) != M_EXT)
        return (EAGAIN);

    /* get the first available RBD */

    FEC_END_NEXT_RBD (pDrvCtrl, pRbd);

    /* Make cache consistent with memory */

    FEC_END_BD_CACHE_INVAL (pRbd, FEC_END_RBD_SZ);

    /* read the RBD status word */

    FEC_END_BD_WORD_RD (pRbd, FEC_END_BD_STAT_OFF, rbdStatus);

    /* if it's not ready, don't touch it! */

    if ((rbdStatus & FEC_END_RBD_EMPTY) == FEC_END_RBD_EMPTY)
        {
        return (EAGAIN);
        }

    /* pass the packet up only if reception was Ok */

    /* get the actual amount of received data */

    FEC_END_BD_WORD_RD (pRbd, FEC_END_BD_LEN_OFF, rbdLen);

    if (rbdLen < ETHERSMALL)
        {
        FEC_END_RX_LS_ADD;

        goto m5200FecPollReceiveEnd;
        }

    /*
     * Upper layer provides the buffer. If buffer is not large enough,
     * we do not copy the received buffer.
     */

    if (pMblk->mBlkHdr.mLen < rbdLen)
        {
        goto m5200FecPollReceiveEnd;
        }

    FEC_END_BD_LONG_RD (pRbd, FEC_END_BD_ADDR_OFF, pData);

    /* up-date statistics */

    if ((*pData ) & (UINT8) 0x01)
        {
        pDrvCtrl->endObj.mib2Tbl.ifInNUcastPkts += 1;
        }
    else
        {
        END_ERR_ADD (&pDrvCtrl->endObj, MIB2_IN_UCAST, +1);
        }

    /* set up the mBlk properly */

    pMblk->mBlkHdr.mFlags   |= M_PKTHDR;
    pMblk->mBlkHdr.mLen = (rbdLen - ETHER_CRC_LEN) & ~0xc000;
    pMblk->mBlkPktHdr.len   = pMblk->mBlkHdr.mLen;

    /* Make cache consistent with memory */

    FEC_END_CACHE_INVAL ((char *) pData, pMblk->mBlkHdr.mLen);

    bcopy ((char *) pData, (char *) pMblk->mBlkHdr.mData,
            ((rbdLen - ETHER_CRC_LEN) & ~0xc000));

    /* put the used RBD on the RBD queue */

    m5200FecRbdClean (pDrvCtrl, pBuf);

    return (retVal);

m5200FecPollReceiveEnd:

    END_ERR_ADD (&pDrvCtrl->endObj, MIB2_IN_ERRS, +1);

    /* put the errored RBD on the RBD queue */

    m5200FecRbdClean (pDrvCtrl, pBuf);

    return (EAGAIN);
    }

/*******************************************************************************
*
* m5200FecPollStart - start polling mode
*
* This routine starts polling mode by disabling ethernet interrupts and
* setting the polling flag in the END_CTRL stucture.
*
* SEE ALSO: m5200FecPollStop()
*
* RETURNS: OK, or ERROR if polling mode could not be started.
*/

LOCAL STATUS m5200FecPollStart
    (
    DRV_CTRL    *pDrvCtrl       /* pointer to DRV_CTRL structure */
    )
    {
    int         intLevel;   /* current intr level */
    int     retVal;     /* convenient holder for return value */

    FEC_END_LOG (FEC_END_DBG_POLL, ("m5200FecPollStart\n"), 0, 0, 0, 0, 0, 0);

    intLevel = intLock();

    /* disable system interrupt: reset relevant bit in SIMASK */

    SYS_FEC_INT_DISABLE (pDrvCtrl, retVal);
    if (retVal == ERROR)
        return (ERROR);

    /* mask chip interrupts   */

    FEC_END_INT_DISABLE;

    FEC_END_FLAG_SET (FEC_END_POLLING);

    intUnlock (intLevel);

    return (OK);
    }

/*******************************************************************************
*
* m5200FecPollStop - stop polling mode
*
* This routine stops polling mode by enabling ethernet interrupts and
* resetting the polling flag in the END_CTRL structure.
*
* SEE ALSO: m5200FecPollStart()
*
* RETURNS: OK, or ERROR if polling mode could not be stopped.
*/

LOCAL STATUS m5200FecPollStop
    (
    DRV_CTRL    *pDrvCtrl       /* pointer to DRV_CTRL structure */
    )
    {
    int         intLevel;
    int     retVal;     /* convenient holder for return value */

    intLevel = intLock();

    /* enable system interrupt: set relevant bit in SIMASK */

    SYS_FEC_INT_ENABLE (pDrvCtrl, retVal);
    if (retVal == ERROR)
        return (ERROR);

    /* enable chip interrupts */

    FEC_END_INT_ENABLE;

    /* set flags */

    FEC_END_FLAG_CLEAR (FEC_END_POLLING);

    intUnlock (intLevel);

    FEC_END_LOG (FEC_END_DBG_POLL, ("m5200FecPollStop... end\n"),
                    0, 0, 0, 0, 0, 0);

    return (OK);
    }

#ifdef FEC_END_DBG
/*******************************************************************************
*
* m5200FecCsrShow - print FEC CSR
*
* This routine displays the FECs CSR structure.
*
* RETURNS: N/A
*
* NOMANUAL
*/

void m5200FecCsrShow
    (
    void
    )
    {
    UINT32  csrVal;
    DRV_CTRL *  pDrvCtrl = pDrvCtrlDbg;

    FEC_END_CSR_RD (FEC_END_ADDR_L_OFF, csrVal);
    FEC_END_LOG (FEC_END_DBG_ANY, ("Addr low:      0x%x\n"),
                      csrVal, 0, 0, 0, 0, 0);

    FEC_END_CSR_RD (FEC_END_ADDR_H_OFF, csrVal);
    FEC_END_LOG (FEC_END_DBG_ANY, ("Addr high:      0x%x\n"),
                      csrVal, 0, 0, 0, 0, 0);

    FEC_END_CSR_RD (FEC_END_HASH_H_OFF, csrVal);
    FEC_END_LOG (FEC_END_DBG_ANY, ("Hash table high:      0x%x\n"),
                      csrVal, 0, 0, 0, 0, 0);

    FEC_END_CSR_RD (FEC_END_HASH_L_OFF, csrVal);
    FEC_END_LOG (FEC_END_DBG_ANY, ("Hash table low:      0x%x\n"),
                      csrVal, 0, 0, 0, 0, 0);

    FEC_END_CSR_RD (FEC_END_CTRL_OFF, csrVal);
    FEC_END_LOG (FEC_END_DBG_ANY, ("Ethernet Controller:      0x%x\n"),
                   (csrVal & FEC_END_CTRL_MASK),
                   0, 0, 0, 0, 0);

    FEC_END_CSR_RD (FEC_END_EVENT_OFF, csrVal);
    FEC_END_LOG (FEC_END_DBG_ANY, ("Interrupt event:      0x%x\n"),
                   (csrVal & FEC_END_EVENT_MSK),
                   0, 0, 0, 0, 0);

    FEC_END_CSR_RD (FEC_END_MASK_OFF, csrVal);
    FEC_END_LOG (FEC_END_DBG_ANY, ("Interrupt mask:      0x%x\n"),
                   (csrVal & FEC_END_EVENT_MSK),
                   0, 0, 0, 0, 0);

    FEC_END_CSR_RD (FEC_END_MII_DATA_OFF, csrVal);
    FEC_END_LOG (FEC_END_DBG_ANY, ("MII data :      0x%x\n"),
                   csrVal, 0, 0, 0, 0, 0);

    FEC_END_CSR_RD (FEC_END_MII_SPEED_OFF, csrVal);
    FEC_END_LOG (FEC_END_DBG_ANY, ("MII speed :      0x%x\n"),
                   csrVal, 0, 0, 0, 0, 0);

    FEC_END_CSR_RD (FEC_END_RX_CTRL_OFF, csrVal);
    FEC_END_LOG (FEC_END_DBG_ANY, ("Rx Control Register:      0x%x\n"),
                   csrVal, 0, 0, 0, 0, 0);

    FEC_END_CSR_RD (FEC_END_TX_CTRL_OFF, csrVal);
    FEC_END_LOG (FEC_END_DBG_ANY, ("Tx Control Register:      0x%x\n"),
                   csrVal, 0, 0, 0, 0, 0);
    }

/*******************************************************************************
*
* m5200FecRbdShow - print FEC read buffer info
*
* This routine displays the FECs read buffer info.
*
* RETURNS: N/A
*
* NOMANUAL
*/

void m5200FecRbdShow
    (
    int rbdNum
    )
    {
    UINT16  status;
    UINT16  length;
    UINT32  buffer;

   FEC_END_RBD_ID bd = (FEC_END_RBD_ID)TaskGetBD( pDrvCtrlDbg->recvTaskNo, rbdNum );

    FEC_END_WORD_RD ((UINT32 *)(bd + FEC_END_BD_STAT_OFF), status);

    FEC_END_WORD_RD ((UINT32 *)(bd + FEC_END_BD_LEN_OFF), length);

    FEC_END_LONG_RD ((UINT32 *)(bd + FEC_END_BD_ADDR_OFF), buffer);

    FEC_END_LOG (FEC_END_DBG_ANY, ("rbdStatus=0x%x rbdLen=0x%x rbdBuf=0x%x\n"),
                   status, length, buffer, 0, 0, 0);
    }

/*******************************************************************************
*
* m5200FecErrorShow - print FEC error info
*
* This routine displays the FECs error info.
*
* RETURNS: N/A
*
* NOMANUAL
*/

void m5200FecErrorShow
    (
    )
    {
    FEC_END_LOG (FEC_END_DBG_ANY, ("TxBab=0x%x RxBab=0x%x LCol=0x%x ColRl=0x%x xUn=0x%x\n"),
                   m5200FecBabTxErr,
                   m5200FecBabRxErr,
                   m5200FecLColErr,
                   m5200FecColRlErr,
                   m5200FecTxUnErr,
                   0);

    FEC_END_LOG (FEC_END_DBG_ANY, ("TxLc=0x%x TxUr=0x%x TxCsl=0x%x \n\
                   TxRl=0x%x\n"),
                   m5200FecTxLcErr,
                   m5200FecTxUrErr,
                   m5200FecTxCslErr,
                   m5200FecTxRlErr,
                   0, 0);

    FEC_END_LOG (FEC_END_DBG_ANY, ("RxFEmpty=0x%x RxAlarm=0x%x RxFull=0x%x \n\
                   RxOv=0x%x RxUf=0x%x\n"),
                   m5200FecRxFEmptyErr,
                   m5200FecRxAlarmErr,
                   m5200FecRxFullErr,
                   m5200FecRxOvErr,
                   m5200FecRxUfErr,
                   0);

    FEC_END_LOG (FEC_END_DBG_ANY, ("RxLs=0x%x RxMem=0x%x, TxFreeErr\n"),
                   m5200FecRxLsErr,
                   m5200FecRxMemErr,
                   m5200FecStatTxFreeError,
                   0, 0, 0);
    }

/*******************************************************************************
*
* m5200FecDrvShow - print FEC driver info
*
* This routine displays the FECs driver info.
*
* RETURNS: N/A
*
* NOMANUAL
*/

void m5200FecDrvShow
    (
    )
    {
    DRV_CTRL *  pDrvCtrl = pDrvCtrlDbg;

    FEC_END_LOG (FEC_END_DBG_ANY, ("pDrvCtrl=0x%x pNetPool=0x%x \n\
                   pClPool=0x%x\n"),
                   (int) pDrvCtrl,
                   (int) pDrvCtrl->endObj.pNetPool,
                   (int) pDrvCtrl->pClPoolId, 0, 0, 0);

    FEC_END_LOG (FEC_END_DBG_ANY, ("bufBase=0x%x bufSize=0x%x pClBlkArea=0x%x\n\
                   clBlkSize=0x%x pMBlkArea=0x%x \n\
                   mBlkSize=0x%x\n"),
                   (int) pDrvCtrl->pBufBase,
                   pDrvCtrl->bufSize,
                   (int) pDrvCtrl->pClBlkArea,
                   pDrvCtrl->clBlkSize,
                   (int) pDrvCtrl->pMBlkArea,
                   pDrvCtrl->mBlkSize);

    FEC_END_LOG (FEC_END_DBG_ANY, ("tbdNum=%d tbdBase=0x%xtbdIndex=%d \n\
                   usedTbdIndex=%d \n\
                   cleanTbdNum=%d txStall=%d \n"),
                   pDrvCtrl->tbdNum,
                   (int) pDrvCtrl->tbdBase,
                   pDrvCtrl->tbdIndex,
                   pDrvCtrl->usedTbdIndex,
                   pDrvCtrl->cleanTbdNum,
                   pDrvCtrl->txStall);

    FEC_END_LOG (FEC_END_DBG_ANY, ("rbdNum=%d recvTaskNo=%d rbdIndex=%d \n"),
                   pDrvCtrl->rbdNum,
                   (int) pDrvCtrl->recvTaskNo,
                   pDrvCtrl->rbdIndex,
                   0, 0, 0);
    }

/*******************************************************************************
*
* m5200FecMiiShow - print FEC MII info
*
* This routine displays the FECs MII info.
*
* RETURNS: N/A
*
* NOMANUAL
*/

void m5200FecMiiShow
    (
    )
    {
    DRV_CTRL *  pDrvCtrl = pDrvCtrlDbg;
    UCHAR       speed [20];
    UCHAR       mode [20];

    strcpy ((char *) speed, (pDrvCtrl->phyInfo->phySpeed == FEC_END_100MBS) ?
                    "100Mbit/s" : "10Mbit/s");
    strcpy ((char *) mode, (pDrvCtrl->phyInfo->phyMode == FEC_END_PHY_FD) ?
                   "full duplex" : "half duplex");

    FEC_END_LOG (FEC_END_DBG_ANY, ("phySpeed=%s phyMode=%s phyAddr=0x%x\n\
                                   isoPhyAddr=0x%x phyFlags=0x%x\n\
                                   phyDefMode=0x%x\n"),
                                   (int) &speed[0],
                                   (int) &mode[0],
                                   pDrvCtrl->phyInfo->phyAddr,
                                   pDrvCtrl->phyInfo->isoPhyAddr,
                                   pDrvCtrl->phyInfo->phyFlags,
                                   pDrvCtrl->phyInfo->phyDefMode);

    FEC_END_LOG (FEC_END_DBG_ANY, ("phyStatus=0x%x phyCtrl=0x%x phyAds=0x%x\n\
                                   phyPrtn=0x%x phyExp=0x%x phyNext=0x%x\n"),
                                   pDrvCtrl->phyInfo->miiRegs.phyStatus,
                                   pDrvCtrl->phyInfo->miiRegs.phyCtrl,
                                   pDrvCtrl->phyInfo->miiRegs.phyAds,
                                   pDrvCtrl->phyInfo->miiRegs.phyPrtn,
                                   pDrvCtrl->phyInfo->miiRegs.phyExp,
                                   pDrvCtrl->phyInfo->miiRegs.phyNext);
    }

/*******************************************************************************
*
* m5200FecMiiShow - print FEC MIB info
*
* This routine displays the FECs MIB info.
*
* RETURNS: N/A
*
* NOMANUAL
*/

void m5200FecMibShow
    (
    )
    {
    DRV_CTRL *  pDrvCtrl = pDrvCtrlDbg;

    FEC_END_LOG (FEC_END_DBG_ANY, ("ifOutNUcastPkts=%d ifOutUcastPkts=%d\n\
                                   ifInNUcastPkts=%d\n\
                                   ifInUcastPkts=%d ifOutErrors=%d\n"),
                                   pDrvCtrl->endObj.mib2Tbl.ifOutNUcastPkts,
                                   pDrvCtrl->endObj.mib2Tbl.ifOutUcastPkts,
                                   pDrvCtrl->endObj.mib2Tbl.ifInNUcastPkts,
                                   pDrvCtrl->endObj.mib2Tbl.ifInUcastPkts,
                                   pDrvCtrl->endObj.mib2Tbl.ifOutErrors,
                                   pDrvCtrl->endObj.mib2Tbl.ifInErrors
                                   );

    }
#endif /* FEC_END_DBG */

/*******************************************************************************
*
* m5200FecSdmaTaskInit - initialize the SDMA tasks
*
* This routine initializes the Bestcomm DMA tasks needed by the driver whose control
* structure is passed in <pDrvCtrl>.
*
* RETURNS: OK or ERROR
*/

LOCAL STATUS m5200FecSdmaTaskInit
    (
    DRV_CTRL *  pDrvCtrl            /* pointer to DRV_CTRL structure */
    )
    {
    TaskSetupParamSet_t fec_recv_param;
    TaskSetupParamSet_t fec_xmit_param;

    FEC_END_LOG (FEC_END_DBG_LOAD, ("m5200FecSdmaTaskInit\n"),
                    0, 0, 0, 0, 0, 0);

    /*
    * Initialize the setup structure with defaults
    */
    fec_xmit_param.NumBD = pDrvCtrl->tbdNum;
    fec_xmit_param.Size.MaxBuf = FEC_END_MAX_PCK_SZ;

    /* Buffer in SDRAM, note: not longword aligned */
    fec_xmit_param.IncrSrc = 4;
    fec_xmit_param.SzSrc = 4;

    /* FIFO in FEC, always longword aligned */
    fec_xmit_param.StartAddrDst = (UINT32)MBAR_VALUE + (UINT32)MBAR_ETH + (UINT32)FEC_END_tfifo_data_OFF;
    fec_xmit_param.IncrDst = 4;
    fec_xmit_param.SzDst = 4;

    /*
    * Now the xmit task can be setup using TaskSetup.
    */

    pDrvCtrl->xmitTaskNo = TaskSetup_TASK_FEC_TX (TASK_FEC_TX_api, &fec_xmit_param);

    if (pDrvCtrl->xmitTaskNo < 0)
        {
        FEC_END_LOG (FEC_END_DBG_LOAD, ("m5200FecSdmaTaskInit: failed to setup TX task\n"),
                            1, 2, 3, 4, 5, 6);
        return ERROR;
        }

    /*
    * Initialize the setup structure with defaults
    */
    fec_recv_param.NumBD = pDrvCtrl->rbdNum;
    fec_recv_param.Size.MaxBuf = FEC_END_MAX_PCK_SZ;
    fec_recv_param.Initiator = 0;

    /* FIFO in FEC, always longword aligned */
    fec_recv_param.StartAddrSrc = (UINT32)MBAR_VALUE + (UINT32)MBAR_ETH + (UINT32)FEC_END_rfifo_data_OFF;
    fec_recv_param.IncrSrc = 4;
    fec_recv_param.SzSrc = 4;

    /* Buffer in SDRAM, note: not longword aligned */
    fec_recv_param.IncrDst = 4;
    fec_recv_param.SzDst = 4;

    /*
    * Now the recv task can be setup using TaskSetup.
    */
    pDrvCtrl->recvTaskNo = TaskSetup_TASK_FEC_RX (TASK_FEC_RX_api, &fec_recv_param);
    if (pDrvCtrl->recvTaskNo < 0)
        {
        FEC_END_LOG (FEC_END_DBG_LOAD, ("m5200FecSdmaTaskInit: failed to setup RX task\n"),
                            1, 2, 3, 4, 5, 6);
        return ERROR;
        }

    /* set TASK priority mode to PTD control register */
    sysSDMA->PtdCntrl = 1;

    /* set initiator priority, requestor for FEC RX is 3 and FEC TX is 4 */
    SDMA_SET_INITIATOR_PRIORITY(sysSDMA, 3, 6);
    SDMA_SET_INITIATOR_PRIORITY(sysSDMA, 4, 5);

    return OK;
    }


#ifdef FEC_END_TST

/*
 * Additional routines used for generating certain test conditions:
 *
 * m5200FecMemShow
 * m5200FecStartTest
 * m5200FecStopTest
 *
 */
IMPORT void netPoolShow (NET_POOL_ID pNetPool);

/*******************************************************************************
*
* m5200FecMemShow - print FEC memory pool info
*
* This routine displays the FECs memory pool info.
*
* RETURNS: N/A
*
* NOMANUAL
*/

int m5200FecMemShow (void)
    {
    if (pTstDrvCtrl == NULL)
        {
        return 1;
        }

    netPoolShow (pTstDrvCtrl->endObj.pNetPool);

    return 0;
    }

/*******************************************************************************
*
* m5200FecFifoError - FEC FIFO error test
*
* This routine runs an FEC FIFO error test.
*
* RETURNS: N/A
*
* NOMANUAL
*/

int m5200FecFifoError (void)
    {
    DRV_CTRL * pDrvCtrl = pTstDrvCtrl;
    unsigned int ix;
    int data = 1;

    if (pDrvCtrl == NULL)
        {
        return 1;
        }

    for (;;)
        {
        logMsg ("Start FIFO error...\n", 1,2,3,4,5,6);

        tstFifoErrorFlag = FALSE;

        level = intLock();
#if 0
        TaskStop (pDrvCtrl->xmitTaskNo);
#endif

#if 1
        TaskStop (pDrvCtrl->recvTaskNo);
#endif
        intUnlock(level);

        for (ix = 0; ix < 1000000; ix++)
            {
            if (tstFifoErrorFlag == FALSE)
                {
#if 0
                FEC_END_CSR_RD (FEC_END_tfifo_data_OFF, data);
#endif

#if 1
                FEC_END_CSR_WR (FEC_END_rfifo_data_OFF, data);
#endif
                }
            else
                {
                break;
                }
            }

        if (ix == 1000000)
            {
            logMsg ("ERROR: No FIFO error interrupt... %d\n", ix,2,3,4,5,6);
            }
        else
            {
            logMsg ("FIFO error interrupt... %d\n", ix,2,3,4,5,6);
            }

        taskDelay (434);

        (void)m5200FecMemShow ();

        taskDelay (334);
        }

    return 0;
    }

/*******************************************************************************
*
* m5200FecStartTest - start the FEC FIFO error test
*
* This routine starts the FEC FIFO error test.
*
* RETURNS: N/A
*
* NOMANUAL
*/

int m5200FecStartTest (void)
    {
    taskSpawn ("tFECtest", 10, 0, 4096, (FUNCPTR) m5200FecFifoError, 0,0,0,0,0,0,0,0,0,0);
    return 0;
    }

/*******************************************************************************
*
* m5200FecStopTest - stop the FEC FIFO error test
*
* This routine stops the FEC FIFO error test.
*
* RETURNS: N/A
*
* NOMANUAL
*/

int m5200FecStopTest (void)
    {
    taskDelete (taskNameToId ("tFECtest"));
    return 0;
    }


#endif /* FEC_END_TST */
