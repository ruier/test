/* universe.c - Tundra Universe chip VME-to-PCI bridge interface library */

/*
 * Copyright 1998-2001, 2004-2005, 2008, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/* Copyright 1996-2001 Motorola, Inc. All Rights Reserved */

/*
modification history
--------------------
01s,10oct11,c_t  Fix sysBusIntGen to avoid generating interrupt when there 
                 already one. (WIND00296177)
01r,03jun08,fao  modify sysBusRmvEnable() comments (WIND000045698)
01q,18mar05,bwa  added INCLUDE_TIPC_MEDIA_SM case for mailbox interrupts init.
01p,31aug04,bwa  BUG FIX : Mailbox interrupts were only initialized if
                           SM_NET was included.
01o,07may04,jln  fix compiler warning
01n,15oct01,kab  SPR #70442: Changed from Location Monitor mapping to 
                 Universe II register access image mapping (RAI) for 
		 VME access to Universe II mailbox registers [efb@mot.com]
		 Fixed universe Reset to add VSI[45].
		 Clean up for T2.2
01m,18jun01,mil  Cleaned up compiler warnings.
01l,08mar01,rhk  Additional code cleanup.
01k,01dec00,dmw  Fixed macro in sysBusRmwEnable().
01j,16aug00,dmw  Update following Wind River coding standards review.
01i,07apr00,srr  Modification history updated to show this came from mv2304.
01h,04jun99,rhk  added code to enable the A24 slave window.
01g,16apr99,rhk  changed PCI space define names.
01f,05apr99,rhk  legacy cleanup, removed references to "raven".
01e,05mar99,rhk  changed sysMailbox routines to use LM0 interrupt,
		 added support for UNIV II location monitors/mailbox intrs.
01d,24feb99,rhk  setup to use dynamic base address for registers.
01c,15feb99,dmw  changed include from ravenMpic.h to kahluaEpic.h.
01b,11dec98,srr  changed comment from mv2600.h to mv2100.h.
01a,11dec98,srr  Written (from version 01v of 2304 bsp).
*/

/*
DESCRIPTION

The routines addressed here include:

Initialization of Universe chip

Bus interrupt functions:
.IP "-"
enable/disable VMEbus interrupt levels
.IP "-"
enable/disable additional VME interrupts
.IP "-"
install handlers for the additional VME interrupts
.IP "-"
generate bus interrupts

Mailbox/locations monitor functions:
.IP "-"
- enable mailbox monitor interrupts

All byte I/O is done via the macros UNIV_IN_BYTE and UNIV_OUT_BYTE which may be
redefined by the BSP.  By default, sysInByte() and sysOutByte() are used.  All
other I/O (32-bit) is handled by the macros UNIV_IN_LONG and UNIV_OUT_LONG
which may be redefined by the BSP.  By default, sysPciRead32() and
sysPciWrite32() are used.
*/

/* includes */

#include "vxWorks.h"
#include "config.h"
#include "vxLib.h"
#ifdef  MV5100
#   include "hawkMpic.h"
#endif /* MV5100 */
#ifdef  MV2100
#   include "kahluaEpic.h"
#endif /* MV2100 */
#include "universe.h"

/* defines */

#define UNIVERSE_VINT_MASKALL  (0xFE << 24)   /* INT1~ INT7 */

#ifndef UNIV_IN_BYTE
# define UNIV_IN_BYTE(adr,pVal) \
  *(volatile UCHAR *)(pVal)=sysInByte((volatile ULONG)(adr))
#endif /* UNIV_IN_BYTE */

#ifndef UNIV_OUT_BYTE
# define UNIV_OUT_BYTE(adr,val) \
  sysOutByte((volatile ULONG)(adr),(volatile UCHAR)(val))
#endif /* UNIV_OUT_BYTE */

#ifndef UNIV_IN_LONG
# define UNIV_IN_LONG(adr,pVal) \
  *(UINT32 *)pVal = sysPciInLong((UINT32)(adr));
#endif /* UNIV_IN_LONG */

#ifndef UNIV_OUT_LONG
# define UNIV_OUT_LONG(adr,val) \
  sysPciOutLong((UINT32)(adr),(UINT32)(val));
#endif /* UNIV_OUT_LONG */

# ifndef CPU_INT_LOCK
#   define CPU_INT_LOCK(pData) (*pData = intLock ())
# endif /* CPU_INT_LOCK */

# ifndef CPU_INT_UNLOCK
#   define CPU_INT_UNLOCK(data) (intUnlock (data))
# endif /* CPU_INT_UNLOCK */

/* forward declarations */

LOCAL FUNCPTR sysMailboxRoutine  = (FUNCPTR) NULL;
LOCAL int sysMailboxArg          = 0;

#ifdef  INCLUDE_VME_DMA

LOCAL STATUS sysVmeDmaCopy(UCHAR *, UCHAR *, UINT32, UINT32);

#endif /* INCLUDE_VME_DMA */

/* extern declarations */

IMPORT int    intEnable (int);
IMPORT int    intDisable (int);
IMPORT int    intLock ();
IMPORT void   intUnlock (int);
IMPORT STATUS intConnect (VOIDFUNCPTR *, VOIDFUNCPTR, _Vx_usr_arg_t);
IMPORT int    logMsg (char *, _Vx_usr_arg_t, _Vx_usr_arg_t,
                      _Vx_usr_arg_t, _Vx_usr_arg_t,_Vx_usr_arg_t,
					  _Vx_usr_arg_t);
IMPORT char * sysPhysMemTop (void);
IMPORT void   sysOutByte (ULONG, UCHAR);
IMPORT UCHAR  sysInByte (ULONG);
IMPORT void   sysPciRead32  (UINT32, UINT32 *);
IMPORT void   sysPciWrite32 (UINT32, UINT32);
IMPORT void   sysUsDelay (UINT32);

IMPORT INT_HANDLER_DESC * sysIntTbl [256];

/* globals */

int    sysUnivIntsEnabled = 0;	/* currently enabled Universe interrupts */
int    sysUnivIntLevel    = 0;	/* current level at which ints are disabled */
UINT32 sysUnivVERRCnt     = 0;	/* #VME errors since power on */
UINT32 univBaseAdrs;

/*
 * Universe interrupt priority mapping table 
 *
 * Interrupt priority level is equal to the index into the following array
 * where 0 is the lowest priority.  The prioritization scheme used here
 * is arbitrary.  If the scheme is changed, the interrupt masks (last column)
 * must be redefined accordingly.  See universe.h and the Universe Manual for
 * bit assignments and further information.
 */

static INT_LEVEL_MAP univIntTable[UNIV_NUM_INT + 1] =
  {
  /* Int Bit Mask	       Int Vector		 Int Level Mask */
  /* ------------------------  ------------------------  -------------- */
    {0,                        0,                         0x00FFF7FF},
    {UNIVERSE_VOWN_INT,        UNIV_VOWN_INT_VEC,         0x00FFF7FE},
    {UNIVERSE_LM0_INT,	       UNIV_LM0_INT_VEC,	  0x00DFF7FE},
    {UNIVERSE_LM1_INT,	       UNIV_LM1_INT_VEC,	  0x009FF7FE},
    {UNIVERSE_LM2_INT,	       UNIV_LM2_INT_VEC,          0x001FF7FE},
    {UNIVERSE_LM3_INT,         UNIV_LM3_INT_VEC,          0x001EF7FE},
    {UNIVERSE_MBOX1_INT,       UNIV_MBOX1_INT_VEC,        0x001CF7FE},
    {UNIVERSE_MBOX2_INT,       UNIV_MBOX2_INT_VEC,        0x0018F7FE},
    {UNIVERSE_MBOX3_INT,       UNIV_MBOX3_INT_VEC,        0x0010F7FE},
    {LVL1,                     -1,                        0x0010F7FC},
    {LVL2,                     -1,                        0x0010F7F8},
    {LVL3,                     -1,                        0x0010F7F0},
    {LVL4,                     -1,                        0x0010F7E0},
    {LVL5,                     -1,                        0x0010F7C0},
    {LVL6,                     -1,                        0x0010F780},
    {LVL7,                     -1,                        0x0010F700},
    {UNIVERSE_DMA_INT,         UNIV_DMA_INT_VEC,          0x0010F600},
    {UNIVERSE_MBOX0_INT,       UNIV_MBOX0_INT_VEC,	  0x0000F600},
    {UNIVERSE_VME_SW_IACK_INT, UNIV_VME_SW_IACK_INT_VEC,  0x0000E600},
    {UNIVERSE_PCI_SW_INT,      UNIV_PCI_SW_INT_VEC,       0x0000C600},
    {UNIVERSE_LERR_INT,        UNIV_LERR_INT_VEC,         0x0000C400},
    {UNIVERSE_VERR_INT,        UNIV_VERR_INT_VEC,         0x0000C000},
    {UNIVERSE_SYSFAIL_INT,     UNIV_SYSFAIL_INT_VEC,      0x00008000},
    {UNIVERSE_ACFAIL_INT,      UNIV_ACFAIL_INT_VEC,       0x00000000}
  };

/* locals */

#ifdef  INCLUDE_VME_DMA

LOCAL BOOL  sysVmeDmaReady = FALSE;

#endif /* INCLUDE_VME_DMA */


/*******************************************************************************
*
* sysUniverseReset - reset the Universe VME chip
*
* This routine performs the reseting of the Universe chip.  All functions
* and VME mapping are disabled.
*
* RETURNS: N/A
*/

void sysUniverseReset (void)
    {
    UINT32  reg;

    /* initialize registers with defaults and disable mapping */

    UNIV_OUT_LONG(UNIVERSE_SCYC_CTL,  0);
    UNIV_OUT_LONG(UNIVERSE_SCYC_ADDR, 0);
    UNIV_OUT_LONG(UNIVERSE_SCYC_EN,   0);
    UNIV_OUT_LONG(UNIVERSE_LMISC,     LMISC_CRT_128_USEC);
    UNIV_OUT_LONG(UNIVERSE_DCTL,      0);
    UNIV_OUT_LONG(UNIVERSE_DTBC,      0);
    UNIV_OUT_LONG(UNIVERSE_DLA,       0);
    UNIV_OUT_LONG(UNIVERSE_DVA,       0);
    UNIV_OUT_LONG(UNIVERSE_DCPP,      0);
    UNIV_OUT_LONG(UNIVERSE_LINT_EN,   0);
    UNIV_OUT_LONG(UNIVERSE_LINT_MAP0, 0);
    UNIV_OUT_LONG(UNIVERSE_LINT_MAP1, 0);
    UNIV_OUT_LONG(UNIVERSE_VINT_EN,   0);
    UNIV_OUT_LONG(UNIVERSE_VINT_MAP0, 0);
    UNIV_OUT_LONG(UNIVERSE_VINT_MAP1, 0);

    UNIV_OUT_LONG(UNIVERSE_VSI0_CTL,  0);
    UNIV_OUT_LONG(UNIVERSE_VSI1_CTL,  0);
    UNIV_OUT_LONG(UNIVERSE_VSI2_CTL,  0);
    UNIV_OUT_LONG(UNIVERSE_VSI3_CTL,  0);
    UNIV_OUT_LONG(UNIVERSE_VSI4_CTL,  0);
    UNIV_OUT_LONG(UNIVERSE_VSI5_CTL,  0);

    UNIV_OUT_LONG(UNIVERSE_LSI0_CTL,  0);
    UNIV_OUT_LONG(UNIVERSE_LSI1_CTL,  0);
    UNIV_OUT_LONG(UNIVERSE_LSI2_CTL,  0);
    UNIV_OUT_LONG(UNIVERSE_LSI3_CTL,  0);

    UNIV_OUT_LONG(UNIVERSE_LM_CTL, 0);
    UNIV_OUT_LONG(UNIVERSE_VRAI_CTL, 0);

    /* clear the SYSFAIL signal */

    UNIV_OUT_LONG(UNIVERSE_VCSR_CLR,  VCSR_CLR_SYSFAIL);

    /* clear any outstanding interrupts/error conditions */

    UNIV_OUT_LONG(UNIVERSE_LINT_STAT, LINT_STAT_CLEAR);
    UNIV_OUT_LONG(UNIVERSE_VINT_STAT, VINT_STAT_CLEAR);
    UNIV_OUT_LONG(UNIVERSE_V_AMERR, V_AMERR_V_STAT);
    UNIV_IN_LONG(UNIVERSE_PCI_CSR, &reg);
    reg |= (PCI_CSR_D_PE | PCI_CSR_S_SERR | PCI_CSR_R_MA |
            PCI_CSR_R_TA | PCI_CSR_S_TA);
    UNIV_OUT_LONG(UNIVERSE_PCI_CSR, reg);
    UNIV_OUT_LONG(UNIVERSE_L_CMDERR, L_CMDERR_L_ENABLE);
    UNIV_OUT_LONG(UNIVERSE_DGCS, DGCS_STOP | DGCS_HALT | DGCS_DONE |
                  DGCS_LERR | DGCS_VERR | DGCS_P_ERR);

    sysUnivIntsEnabled = 0;
    sysUnivIntLevel    = 0;
    }


/*******************************************************************************
*
* sysUniverseInit - initialize registers of the Universe chip
*
* This routine initializes registers of the Universe VME-to-PCI bridge and maps
* access to the VMEbus memory space.
*
* NOTE: The sysProcNumSet() routine maps the master node's local memory on the
* VMEbus.
*
* RETURNS: OK, always.
*/

STATUS sysUniverseInit (void)
    {
    UINT32	temp_data;

    /* Put vme chip into a power-up/reset state */

    sysUniverseReset ();

    UNIV_OUT_LONG(UNIVERSE_MAST_CTL, (MAST_CTL_RTRY_FOREVER |
                                      MAST_CTL_PWON_4096    |
                                      MAST_CTL_VRL3         |
                                      MAST_CTL_VRM_FAIR     |
                                      MAST_CTL_VREL_ROR     |
                                      MAST_CTL_PABS_128     ));
	
    UNIV_IN_LONG(UNIVERSE_MISC_CTL, &temp_data);

    /* maintain power-up option bits */

    temp_data &= ( MISC_CTL_SYSCON | MISC_CTL_V64AUTO );
    temp_data |= ( MISC_CTL_VBTO_256USEC   |
		   MISC_CTL_VARB_RROBIN    |
		   MISC_CTL_VARBTO_256USEC |
		   MISC_CTL_RESCIND        );
    UNIV_OUT_LONG(UNIVERSE_MISC_CTL, temp_data);
    UNIV_OUT_LONG(UNIVERSE_USER_AM,  0);
    UNIV_OUT_LONG(UNIVERSE_VRAI_CTL, 0);
    UNIV_OUT_LONG(UNIVERSE_VCSR_CTL, 0);

    /* clear the SYSFAIL signal */

    UNIV_OUT_LONG(UNIVERSE_VCSR_CLR, VCSR_CLR_SYSFAIL);

    /* set the latency timer to max value */

    UNIV_OUT_LONG(UNIVERSE_PCI_MISC0, PCI_MISC0_LATENCY_TIMER);

    /* Map to get to VMEbus  using A32 */

    UNIV_OUT_LONG(UNIVERSE_LSI1_BS,  VAL_LSI1_BS);
    UNIV_OUT_LONG(UNIVERSE_LSI1_BD,  VAL_LSI1_BD);
    UNIV_OUT_LONG(UNIVERSE_LSI1_TO,  VAL_LSI1_TO);
    UNIV_OUT_LONG(UNIVERSE_LSI1_CTL, VAL_LSI1_CTL);

    /* Map to get to VMEbus  using A24 */

    UNIV_OUT_LONG(UNIVERSE_LSI2_BS,  VAL_LSI2_BS);
    UNIV_OUT_LONG(UNIVERSE_LSI2_BD,  VAL_LSI2_BD);
    UNIV_OUT_LONG(UNIVERSE_LSI2_TO,  VAL_LSI2_TO);
    UNIV_OUT_LONG(UNIVERSE_LSI2_CTL, VAL_LSI2_CTL);

    /* Map to get to VMEbus using A16 */

    UNIV_OUT_LONG(UNIVERSE_LSI3_BS,  VAL_LSI3_BS);
    UNIV_OUT_LONG(UNIVERSE_LSI3_BD,  VAL_LSI3_BD);
    UNIV_OUT_LONG(UNIVERSE_LSI3_TO,  VAL_LSI3_TO);
    UNIV_OUT_LONG(UNIVERSE_LSI3_CTL, VAL_LSI3_CTL);

    /* Map to get to VMEbus Universe Mailbox Registers using A32 */

    UNIV_OUT_LONG(UNIVERSE_LSI0_BS,  VAL_LSI0_BS);
    UNIV_OUT_LONG(UNIVERSE_LSI0_BD,  VAL_LSI0_BD);
    UNIV_OUT_LONG(UNIVERSE_LSI0_TO,  VAL_LSI0_TO);
    UNIV_OUT_LONG(UNIVERSE_LSI0_CTL, VAL_LSI0_CTL);

    return (OK);
    }


/*******************************************************************************
*
* sysUniverseInit2 - enable local memory accesses from the VMEbus
*
* This routine enables local resources to be accessed from the VMEbus.
* All target boards have an A32 window opened to access the VMEbus Universe
* mailbox registers (RAI) when shared memory is included. However, only the 
* master node has an A32 and an A24 window open to its local memory space.
*
* RETURNS: N/A
*
* NOMANUAL
*/

void sysUniverseInit2
    (
    int         procNum                 /* processor number */
    )
    {
#ifndef DOC

#   if  (defined(INCLUDE_SM_NET) ||         \
         defined(INCLUDE_SM_OBJ) ||         \
         defined(INCLUDE_TIPC_MEDIA_SM))

        /* setup the VME RAI address range */

        UNIV_OUT_LONG(UNIVERSE_VRAI_CTL, VAL_RAI_CTL);
        UNIV_OUT_LONG(UNIVERSE_VRAI_BS, VAL_RAI_BS);
#   endif /* (defined(INCLUDE_*SM*)) */

    if (procNum == 0)
        {
        /* setup the A32 window */

        UNIV_OUT_LONG(UNIVERSE_VSI1_BS,  VAL_VSI1_BS);
        UNIV_OUT_LONG(UNIVERSE_VSI1_BD,  VAL_VSI1_BD);
        UNIV_OUT_LONG(UNIVERSE_VSI1_TO,  VAL_VSI1_TO);

#   ifdef A24_SLV_WINDOW
        UNIV_OUT_LONG(UNIVERSE_VSI2_BS,  VAL_VSI2_BS);
        UNIV_OUT_LONG(UNIVERSE_VSI2_BD,  VAL_VSI2_BD);
        UNIV_OUT_LONG(UNIVERSE_VSI2_TO,  VAL_VSI2_TO);
#   endif /* A24_SLV_WINDOW */

        UNIV_OUT_LONG(UNIVERSE_VSI1_CTL, VAL_VSI1_CTL | VSI_CTL_PWEN);
#   ifdef A24_SLV_WINDOW
        UNIV_OUT_LONG(UNIVERSE_VSI2_CTL, VAL_VSI2_CTL | VSI_CTL_PWEN);
#   endif /* A24_SLV_WINDOW */

#   if (SM_OFF_BOARD == FALSE)
#       ifndef ANY_BRDS_IN_CHASSIS_NOT_RMW

       /*
        * All slave boards in the chassis generate a VMEbus RMW, and
        * the master is capable of translating an incoming RMW into
        * an atomic operation.
        */

       /*
        * The A32 access range is now divided into 3 separate windows.
        * The first window will allow normal access to the start of local
        * memory up to the Shared Memory Region.  It is defined above thru
        * the use of a conditional #define VAL_VSI1_BD in the board.h file.
        * The second window will allow Read-Modify-Write (RMW) access to
        * the Standard VxWorks and VxMP's Shared Memory Region.  The
        * third window will allow normal access to the local memory
        * starting after the Shared Memory Region up to the end of
        * physical memory.
        */

        /* setup the second A32 window */

        UNIV_OUT_LONG(UNIVERSE_VSI4_BS,  VAL_VSI4_BS);
        UNIV_OUT_LONG(UNIVERSE_VSI4_BD,  VAL_VSI4_BD);
        UNIV_OUT_LONG(UNIVERSE_VSI4_TO,  VAL_VSI4_TO);
        UNIV_OUT_LONG(UNIVERSE_VSI4_CTL, VAL_VSI4_CTL);

        /* setup the third A32 window */

        UNIV_OUT_LONG(UNIVERSE_VSI5_BS,  VAL_VSI5_BS);
        UNIV_OUT_LONG(UNIVERSE_VSI5_BD,  VAL_VSI5_BD);
        UNIV_OUT_LONG(UNIVERSE_VSI5_TO,  VAL_VSI5_TO);
        UNIV_OUT_LONG(UNIVERSE_VSI5_CTL, VAL_VSI5_CTL);

#       endif	/* ANY_BRDS_IN_CHASSIS_NOT_RMW */
#   endif	/* SM_OFF_BAORD */
        }
#endif /* ifndef DOC */
    }


/*******************************************************************************
*
* sysIntDisable - disable a bus interrupt level
*
* This routine disables reception of a specified VMEbus interrupt level.
*
* NOTE: revision 1.0 Universe chips can fail and lockup if two or more interrupt
* levels are enabled.  For more details see Tundra Universe Errata sheet.
*
* RETURNS: OK, or ERROR if <intLevel> is not in the range 1 - 7.
*
* SEE ALSO: sysIntEnable()
*/

STATUS sysIntDisable
    (
    int intLevel        /* interrupt level to disable (1-7) */
    )
    {
    int mask;

    if (intLevel < 1 || intLevel > 7)
        return (ERROR);

    /* clear any pending intr. for this level */

    mask = 1 << intLevel;
    UNIV_OUT_LONG(UNIVERSE_LINT_STAT, mask);

    /* disable the interrupt */

    sysUnivIntsEnabled &= ~mask;
    UNIV_OUT_LONG(UNIVERSE_LINT_EN, sysUnivIntsEnabled);

    return (OK);
    }


/*******************************************************************************
*
* sysIntEnable - enable a bus interrupt level
*
* This routine enables reception of a specified VMEbus interrupt level.
*
* NOTE: revision 1.0 Universe chips can fail and lockup if two or more interrupt
* levels are enabled.  For more details see Tundra Universe Errata sheet.
*
* RETURNS: OK, or ERROR if <intLevel> is not in the range 1 - 7.
*
* SEE ALSO: sysIntDisable()
*/

STATUS sysIntEnable
    (
    int intLevel        /* interrupt level to enable (1-7) */
    )
    {
    int mask;

    if (intLevel < 1 || intLevel > 7)
        return (ERROR);

    /* clear any pending intr. for this level */

    mask = 1 << intLevel;
    UNIV_OUT_LONG(UNIVERSE_LINT_STAT, mask);

    /* enable the interrupt */

    sysUnivIntsEnabled |= mask;
    UNIV_OUT_LONG(UNIVERSE_LINT_EN, sysUnivIntsEnabled);

    /* map the interrupt.  Currently all VME ints are mapped to LINT#0 */

    UNIV_OUT_LONG(UNIVERSE_LINT_MAP0, 0);

    return (OK);
    }


/*******************************************************************************
*
* sysBusIntAck - acknowledge a bus interrupt
*
* This routine acknowledges a specified VMEbus interrupt level.
*
* NOTE: This routine is included for BSP compliance only.  Since VMEbus
* interrupts are re-enabled in the interrupt handler, and acknowledged
* automatically by hardware, this routine is a no-op.
*
* RETURNS: NULL.
*
* SEE ALSO: sysBusIntGen()
*/

int sysBusIntAck
    (
    int intLevel        /* interrupt level to acknowledge */
    )
    {
    return (0);
    }


/*******************************************************************************
*
* sysBusIntGen - generate a bus interrupt
*
* This routine generates a VMEbus interrupt for a specified level with a
* specified vector.  Only one VME interrupt can be generated at a time and
* none can be generated if a previously generated VME interrupt has not been
* acknowledged, i.e., if no VME bus IACK cycle has completed.
*
* RETURNS: OK, or ERROR if <level> or <vector> are out of range or if an
* interrupt is already in progress.
*
* SEE ALSO: sysBusIntAck()
*/

STATUS sysBusIntGen
    (
    int level,          /* interrupt level to generate    */
    int vector          /* interrupt vector for interrupt */
    )
    {
    volatile UINT32 enReg;
    volatile UINT32 intReg;
    volatile UINT32 statidReg;
    int            lockKey;

    /* Validate interrupt level */

    if (level < 1 || level > 7 || vector > 255 || vector < 2)
        return (ERROR);

    /* lock interrupts so there will be no interference */

    CPU_INT_LOCK(&lockKey);

    /* Ensure no bus interrupt is already asserted */

    UNIV_IN_LONG(UNIVERSE_VINT_STAT, &intReg);
    if (intReg & UNIVERSE_VINT_MASKALL)
        {
        CPU_INT_UNLOCK(lockKey);
        return (ERROR);
        }

    /*
     * Clear Software Interrupt bit before setting it
     * It needs a 0 to 1 transition to generate an interrupt
     */

    UNIV_IN_LONG(UNIVERSE_VINT_EN, &enReg);
    enReg &= ~((1 << level) << 24);
    UNIV_OUT_LONG(UNIVERSE_VINT_EN, enReg);

    /*
     * store vector number, wiping out LSB of vector number
     * This implies that only even number vectors should be used
     * (Universe FEATURE ?)
     */

    UNIV_IN_LONG(UNIVERSE_STATID, &statidReg);
    vector >>= 1;
    statidReg = (statidReg & STATID_MASK) | (vector << 25);
    UNIV_OUT_LONG(UNIVERSE_STATID, statidReg);

    /* Generate a SW interrupt on the VMEbus at the requested level */

    UNIV_IN_LONG(UNIVERSE_VINT_EN, &enReg);
    enReg |= ((1 << level) << 24);
    UNIV_OUT_LONG(UNIVERSE_VINT_EN, enReg);

    /* unlock the interrupt */

    CPU_INT_UNLOCK(lockKey);

    return (OK);

    }


/*******************************************************************************
*
* sysUnivIntEnable - enable Universe-supported interrupt(s)
*
* This routine enables the specified type(s) of interrupt supported by the
* Universe VME-to-PCI bridge.
*
* RETURNS: OK, or ERROR if invalid interrupt type(s).
*
* SEE ALSO: sysUnivIntDisable()
*
*/

STATUS sysUnivIntEnable
    (
    int univIntType	/* interrupt type */
    )
    {

    /* make sure the interrupt type is valid */

    if ((univIntType & UNIVERSE_INT_MASK)     == 0 ||
	(univIntType & UNIVERSE_RESERVED_INT) != 0   )
	return(ERROR);

    /* clear any pending intr. for the type */

    UNIV_OUT_LONG(UNIVERSE_LINT_STAT, univIntType);

    /* enable the interrupt */

    sysUnivIntsEnabled |= univIntType;
    UNIV_OUT_LONG(UNIVERSE_LINT_EN, sysUnivIntsEnabled);

    /* map the interrupt.  Currently all VME ints are mapped to LINT#0 */

    UNIV_OUT_LONG(UNIVERSE_LINT_MAP1, 0);

    return (OK);
    }


/*******************************************************************************
*
* sysUnivIntDisable - disable Universe-supported interrupt(s)
*
* This routine disables the specified type(s) of interrupt supported by the 
* Universe VME-to-PCI bridge.
*
* RETURNS: OK, or ERROR if invalid interrupt type(s).
*
* SEE ALSO: sysUnivIntEnable()
*
*/

STATUS sysUnivIntDisable
    (
    int univIntType /* interrupt type */
    )
    {

    /* make sure the interrupt type is valid */

    if ((univIntType & UNIVERSE_INT_MASK)     == 0 ||
	(univIntType & UNIVERSE_RESERVED_INT) != 0   )
        return (ERROR);

    /* disable the interrupt */

    sysUnivIntsEnabled &= ~univIntType;
    UNIV_OUT_LONG(UNIVERSE_LINT_EN, sysUnivIntsEnabled);

    /* clear any pending intr. for the type */

    UNIV_OUT_LONG(UNIVERSE_LINT_STAT, univIntType);

    return (OK);
    }


/*******************************************************************************
*
* sysUnivIntLevelSet - set a Universe-supported interrupt level
*
* This routine disables all interrupts supported by the Universe at and below
* the specified level.  The lowest level is 0, the highest is 23.  The
* priority mapping is:
* .CS
*    0		no interrupts masked
*    1		UNIVERSE_VOWN_INT
*    2          UNIVERSE_LM0_INT
*    3          UNIVERSE_LM1_INT
*    4          UNIVERSE_LM2_INT
*    5          UNIVERSE_LM3_INT
*    6          UNIVERSE_MBOX1_INT
*    7          UNIVERSE_MBOX2_INT
*    8          UNIVERSE_MBOX3_INT
*    9		VMEBUS_LVL1
*    10		VMEBUS_LVL2
*    11		VMEBUS_LVL3
*    12		VMEBUS_LVL4
*    13		VMEBUS_LVL5
*    14		VMEBUS_LVL6
*    15		VMEBUS_LVL7
*    16		UNIVERSE_DMA_INT
*    17         UNIVERSE_MBOX0_INT
*    18		UNIVERSE_LERR_INT
*    19		UNIVERSE_VERR_INT
*    20		UNIVERSE_VME_SW_IACK_INT
*    21		UNIVERSE_PCI_SW_INT
*    22		UNIVERSE_SYSFAIL_INT
*    23		UNIVERSE_ACFAIL_INT
* .CE
*
* If the level specified is -1, the level is not changed, just the current
* level is returned.
*
* RETURNS: previous interrupt level.
*
* SEE ALSO: sysUnivIntDisable(), sysUnivIntEnable()
*/

int sysUnivIntLevelSet
    (
    int  univIntLvl	/* Universe interrupt level */
    )
    {
    int  intLvl;
    int  key;

    /* Just return current level if so requested */

    if (univIntLvl == -1)
	return (sysUnivIntLevel);

    /* Lock out interrupts during level change */

    CPU_INT_LOCK(&key);

    /* disable the interrupt levels at and below the current level */

    intLvl = univIntTable[univIntLvl].intMask;
    intLvl &= sysUnivIntsEnabled;
    UNIV_OUT_LONG(UNIVERSE_LINT_EN, intLvl);

    /* save current level for return */

    intLvl = sysUnivIntLevel;

    /* set new mask */

    sysUnivIntLevel = univIntLvl;

    /* Unlock interrupts */

    CPU_INT_UNLOCK(key);

    /* return previous mask */

    return (intLvl);
    }


/******************************************************************************
*
* sysUnivIntConnect - connect an interrupt handler for an interrupt type
*
* This routine connects an interrupt handler for a specified interrupt type 
* to the system vector table of the Universe VME-to-PCI bridge.
*
* NOTE: the interrupt MBOX0 is reserved for the sysMailbox routines.
*
* RETURNS: OK, or ERROR if any argument is invalid or interrupt type
* doesn't match the table entries.
*/

STATUS sysUnivIntConnect
    (
    int		univIntType,  /* the interrupt type to connect with */
    VOIDFUNCPTR	routine,      /* routine to be called */
    int		parameter     /* parameter to be passed to routine */
    )
    {
    int			univVecNum = 0;

    /* make sure the interrupt type is valid */

    if ((univIntType & UNIVERSE_INT_MASK) == 0)
        return (ERROR);

    /* determine the vector number for the interrupt */

    switch(univIntType)
	{
	case UNIVERSE_DMA_INT:
	    univVecNum = UNIV_DMA_INT_VEC;
	    break;
	case UNIVERSE_LERR_INT:
	    univVecNum = UNIV_LERR_INT_VEC;
	    break;
	case UNIVERSE_VERR_INT:
	    univVecNum = UNIV_VERR_INT_VEC;
	    break;
	case UNIVERSE_SYSFAIL_INT:
	    univVecNum = UNIV_SYSFAIL_INT_VEC;
	    break;
	case UNIVERSE_ACFAIL_INT:
	    univVecNum = UNIV_ACFAIL_INT_VEC;
	    break;
	case UNIVERSE_PCI_SW_INT:
	    univVecNum = UNIV_PCI_SW_INT_VEC;
	    break;
	case UNIVERSE_VME_SW_IACK_INT:
	    univVecNum = UNIV_VME_SW_IACK_INT_VEC;
	    break;
	case UNIVERSE_VOWN_INT:
	    univVecNum = UNIV_VOWN_INT_VEC;
	    break;
        case UNIVERSE_MBOX1_INT:
            univVecNum = UNIV_MBOX1_INT_VEC;
            break;
        case UNIVERSE_MBOX2_INT:
            univVecNum = UNIV_MBOX2_INT_VEC;
            break;
        case UNIVERSE_MBOX3_INT:
            univVecNum = UNIV_MBOX3_INT_VEC;
            break;
        case UNIVERSE_LM0_INT:
            univVecNum = UNIV_LM0_INT_VEC;
            break;
        case UNIVERSE_LM1_INT:
            univVecNum = UNIV_LM1_INT_VEC;
            break;
        case UNIVERSE_LM2_INT:
            univVecNum = UNIV_LM2_INT_VEC;
            break;
        case UNIVERSE_LM3_INT:
            univVecNum = UNIV_LM3_INT_VEC;
            break;
	default:

	    /* doesn't match one of the intr. types, so return an error */

	    return(ERROR);
	}

    /* install the handler in the vector table */

    if (intConnect (INUM_TO_IVEC(univVecNum), routine, parameter) == ERROR)
        return(ERROR);

    return (OK);
    }


/*******************************************************************************
*
* sysMailboxInt - mailbox interrupt handler
*
* This routine calls the installed mailbox routine, if it exists.
*/

LOCAL void sysMailboxInt (void)

    {
    if (sysMailboxRoutine != NULL)
        sysMailboxRoutine (sysMailboxArg);
    }


/*******************************************************************************
*
* sysMailboxConnect - connect a routine to the mailbox interrupt
*
* This routine specifies the interrupt service routine to be called at each
* mailbox interrupt.
*
* NOTE: The mailbox interrupt is the mailbox 0 interrupt on the
* UNIVERSE II chip.
*
* RETURNS: OK, or ERROR if the routine cannot be connected to the interrupt.
*
* SEE ALSO: intConnect(), sysMailboxEnable()
*/

STATUS sysMailboxConnect
    (
    FUNCPTR routine,    /* routine called at each mailbox interrupt */
    int arg             /* argument with which to call routine      */
    )
    {
    static BOOL sysMailboxConnected = FALSE;

    if (!sysMailboxConnected &&
        intConnect (INUM_TO_IVEC (UNIV_MBOX0_INT_VEC),
                    sysMailboxInt, 0) == ERROR)
        {
        return (ERROR);
        }

    sysMailboxConnected = TRUE;
    sysMailboxRoutine   = routine;
    sysMailboxArg       = arg;

    return (OK);
    }


/*******************************************************************************
*
* sysMailboxEnable - enable the mailbox interrupt
*
* This routine enables the mailbox interrupt.
*
* NOTE: The mailbox interrupt is the mailbox 0 interrupt on the
* UNIVERSE II chip.
*
* RETURNS: OK, if mailbox interrupt becomes enabled, else ERROR.
*
* SEE ALSO: sysMailboxConnect(), sysMailboxDisable()
*/

STATUS sysMailboxEnable
    (
    char *mailboxAdrs           /* address of mailbox (ignored) */
    )
    {

    /* enable the mailbox 0 interrupt */

    if (sysUnivIntEnable(UNIVERSE_MBOX0_INT) == OK)
        return (OK);
    else
	return (ERROR);
    }


/*******************************************************************************
*
* sysMailboxDisable - disable the mailbox interrupt
*
* This routine disables the mailbox interrupt.
*
* NOTE: The mailbox interrupt is the mailbox 0 interrupt on the
* UNIVERSE II chip.
*
* RETURNS: OK, if the mailbox interrupt gets disabled, else ERROR
*
* SEE ALSO: sysMailboxConnect(), sysMailboxEnable()
*/

STATUS sysMailboxDisable
    (
    char *mailboxAdrs           /* address of mailbox (ignored) */
    )
    {

    /* disable the mailbox interrupt */

    if (sysUnivIntDisable(UNIVERSE_MBOX0_INT) == OK)
        return (OK);
    else
        return (ERROR);
    }


/*******************************************************************************
*
* sysUnivVERRClr - Universe VMEbus Error Clear routine
*
* This is the VMEbus Error clear routine for the Tundra Universe PCI to VME
* Bridge.  It counts the ocuurances at the specified counter and clears the
* error condition in the three registers associated with VME Bus Errors:
* LINT_STAT, VINT_STAT, and V_AMERR.
*
* RETURNS: N/A
*/

void sysUnivVERRClr (void)
    {
    UINT32 status;

    /* Count occurances */

    ++sysUnivVERRCnt;

    /* Get current address error status */

    UNIV_IN_LONG(UNIVERSE_V_AMERR, &status);

    /* Clear any error */

    status &= V_AMERR_V_STAT;
    UNIV_OUT_LONG(UNIVERSE_V_AMERR, status);

    /* Get current VME error status */

    UNIV_IN_LONG(UNIVERSE_VINT_STAT, &status);

    /* Clear any error */

    status &= VINT_STAT_VERR;
    UNIV_OUT_LONG(UNIVERSE_VINT_STAT, status);

    /* Get current PCI error status */

    UNIV_IN_LONG(UNIVERSE_LINT_STAT, &status);

    /* Clear any error */

    status &= LINT_STAT_VERR;
    UNIV_OUT_LONG(UNIVERSE_LINT_STAT, status);

    /* Force write due to Write-Posting and get updated status */

    UNIV_IN_LONG(UNIVERSE_PCI_CSR, &status);
    }


/*******************************************************************************
*
* sysUnivLevelDecode - decode highest pending priority Universe interrupt
*
* This routine decodes the highest pending priority Universe interrupt from a
* bit field of interrupts and returns the associated interrupt vector, priority
* level and priority level bit mask.
*
* RETURNS: highest pending interrupt priority level bit mask
*
* SEE ALSO: register and bit field defs in universe.h
*/

int sysUnivLevelDecode
    (
    int   bitField,	/* one interrupt per bit, up to 23 bits */
    int * vecNum,	/* where to return associated vector */
    int * intLvl	/* where to return associated Universe int level */
    )
    {
    int    bitLvlMsk;
    int    i;
    int    vector = 0;

    /* Compare bits in order of highest priority first */

    for (i = UNIV_NUM_INT; i >= 0; --i)
	{
	if (bitField & univIntTable[i].bitMask)
	    break;
	}

    /* Determine and return interrupt vector, priority level and level mask */

    bitLvlMsk = univIntTable[i].bitMask;
    if (univIntTable[i].vector != -1)
        *vecNum = univIntTable[i].vector;
    else
	{
	switch (bitLvlMsk)
            {
            case LVL7:
                UNIV_IN_LONG(UNIVERSE_V7_STATID, &vector);
                break;

            case LVL6:
                UNIV_IN_LONG(UNIVERSE_V6_STATID, &vector);
                break;

            case LVL5:
                UNIV_IN_LONG(UNIVERSE_V5_STATID, &vector);
                break;

            case LVL4:
                UNIV_IN_LONG(UNIVERSE_V4_STATID, &vector);
                break;

            case LVL3:
                UNIV_IN_LONG(UNIVERSE_V3_STATID, &vector);
                break;

            case LVL2:
                UNIV_IN_LONG(UNIVERSE_V2_STATID, &vector);
                break;

            case LVL1:
                UNIV_IN_LONG(UNIVERSE_V1_STATID, &vector);
                break;
            }

	/*
	 * Check for errors at this point,
	 * if a VME bus error has occured, it must be
	 * cleared by the sysUnivVERRClr() routine and the vector discarded.
	 * The vector returned during a VME bus error is bogus.
	 */

	if ((vector & V1_STATID_ERR) != 0)
        {

	    /* Clear VME bus error */

	    sysUnivVERRClr ();

            /* Discard bad vector */

            *vecNum = 0;
        }
        else
            *vecNum = vector & 0xFF;  /* only eight bits of useful info */
	}
    *intLvl = i;

    return (bitLvlMsk);
    }


/*******************************************************************************
*
* sysUnivVmeIntr - VMEbus interrupt handler
*
* This is the VMEbus interrupt handler for the Motorola PCI Host
* Bridge (PHB) and Embedded Processor Interrupt Controller (EPIC).  It is
* connected to the single VMEbus interrupt from the EPIC and examines the
* Universe chip to determine the interrupt level and vector of the
* interrupt source.  Having obtained the vector number, this routine then
* indexes into the system vector table, calling the specified interrupt
* handler routine.
*
* RETURNS: N/A
*/

void sysUnivVmeIntr (void)
    {
    int                 pendingIntr;
    int                 highestIntr;
    int			prevIntLvl;
    int			univIntLvl;
    int			vec_num = 0;
    INT_HANDLER_DESC *  currHandler;

    /* get pending interrupt level(s) */

    UNIV_IN_LONG(UNIVERSE_LINT_STAT, &pendingIntr);
    pendingIntr &= LINT_STAT_INT_MASK;
    pendingIntr &= sysUnivIntsEnabled;

    /*
     * Handle pending interrupts in order of highest priority.
     *
     * For each interrupt, determine the level, get vector associated
     * with the interrupt, set the appropriate interrupt level, clear the
     * interrupt in the Universe, and dispatch to all associated ISRs.
     */

    if (pendingIntr != 0)
	{

	/* Determine highest interrupt priority level and vector */

        highestIntr = sysUnivLevelDecode (pendingIntr, &vec_num, &univIntLvl);

	/* Set intr level: mask out all ints at and below this level */

        prevIntLvl = sysUnivIntLevelSet (univIntLvl);

        vec_num   &= 0xff;
        if (vec_num == 0)
	    {
	    logMsg ("bad vme interrupt %d\n", vec_num, 0,0,0,0,0);
	    }

        else if ((currHandler = sysIntTbl[vec_num]) == NULL)
	    logMsg ("uninitialized vme interrupt %d\n", vec_num, 0,0,0,0,0);

        else
	    {
	    while (currHandler != NULL)
	        {
	        currHandler->vec (currHandler->arg);
	        currHandler = currHandler->next;
	        }
	    }

        /* Clear interrupt request in Universe */

	UNIV_OUT_LONG(UNIVERSE_LINT_STAT, highestIntr);

	/* restore previous interrupt level */

	(void)sysUnivIntLevelSet (prevIntLvl);
        }

    return;
    }


/******************************************************************************
*
* sysBusRmwEnable - enable Read-Modify-Write (RMW) cycle on VMEbus
*
* NOTE: These parameters are written to the SCG as Big-Endian values.
*
* RETURNS: N/A
*/

void sysBusRmwEnable
    (
    UINT swapCompareEnable,	/* Enabled bits involved in compare and swap */
    UINT compareData,		/* Data to compare with read portion of RMW */
    UINT swapData,		/* Data to write during write portion of RMW */
    char *  rmwAddress		/* RMW address */
    )
    {
    *UNIVERSE_SCYC_EN = swapCompareEnable;
    *UNIVERSE_SCYC_CMP = compareData;
    *UNIVERSE_SCYC_SWP = swapData;

    /* Convert RMW address to a PCI address */

#ifdef  EXTENDED_VME
    UNIV_OUT_LONG(UNIVERSE_SCYC_ADDR,  (UINT)((UINT)rmwAddress +
                    (UINT)(CPU2PCI_OFFSET0_VAL << 16)));
#else   /* EXTENDED_VME */
    UNIV_OUT_LONG(UNIVERSE_SCYC_ADDR,  (UINT)((UINT)rmwAddress +
                    (UINT)(CPU2PCI_OFFSET2_VAL << 16)));
#endif  /* EXTENDED_VME */

    /* Configure Special Cycle Generator to generate a RMW cycle */

    UNIV_OUT_LONG(UNIVERSE_SCYC_CTL, SCYC_CTL_RMW);
    }


/******************************************************************************
*
* sysBusRmwDisable - Disable Read-Modify-Write (RMW) cycle on the VMEbus.
*
* Disable Read-Modify-Write (RMW) cycle on the VMEbus.
*
* RETURNS: N/A
*/

void sysBusRmwDisable ()
    {
    UNIV_OUT_LONG(UNIVERSE_SCYC_CTL, SCYC_CTL_DISABLE);
    }


#ifdef	INCLUDE_VME_DMA

/*
 * The following routines whose names start with "sysVmeDma"
 * comprise the DMA driver.
 */

/******************************************************************************
*
* sysVmeDmaInit - Inititalize the Universe's DMA engine
*
* This routine will initialize the Universe's DMA engine.  This involves
* using the following #defines listed in config.h as defaults to configure
* the appropriate fields in the DMA's DCTL and DGCS registers.
* .CS
*	VME_DMA_XFER_TYPE
*	VME_DMA_ADDR_SPACE
*	VME_DMA_DATA_TYPE
*	VME_DMA_USER_TYPE
* .CE
* The DMA engine will also be permanently configured so that the PCI Bus
* will perform 64-bit transactions.
* 
* NOTE: This low-level driver is strictly non-sharable; however,
* it contains no guards to prevent multiple tasks from calling
* it simultaneously.  It assumes that the application layer will
* provide atomic access to this driver through the use of a
* semaphore or similar guards.
*
* In addition, the internals of this driver only operates in a
* non-interrupt (polled) mode; it uses a busy loop to sample the
* DMA transfer status.  This implies that the calling task will
* wait until the DMA transfer has terminated.  As a precaution,
* it is recommended by the Tundra User's Manual that the calling
* task set up a background timer to prevent an infinite wait
* caused by a system problem.  Also, tasks transferring large
* blocks of data should lower their priority level to allow other
* tasks to run, and tasks transferring small blocks of data
* should use bcopy() instead of calling this driver.
*
* RETURNS: OK
*/

STATUS sysVmeDmaInit (void)
    {

    /* Configure the DMA Transfer Control register */

    UNIV_OUT_LONG(UNIVERSE_DCTL, (VME_DMA_XFER_TYPE  |
				  VME_DMA_ADDR_SPACE |
				  VME_DMA_DATA_TYPE  |
				  VME_DMA_USER_TYPE  |
				  DCTL_LD64EN ));

    sysVmeDmaReady = TRUE;
    return (OK);
    }

/******************************************************************************
*
* sysVmeDmaCnfgGet - Get DMA transfer configuration parameters
*
* This routine will get the DMA transfer parameters.
*
* RETURNS: OK or ERROR
*/

STATUS sysVmeDmaCnfgGet
    (
    UINT32 *xferType,	/* output: Ptr to VMEbus data transfer type	*/
			/* Valid range:					*/
			/* (DCTL_VDW_8  | DCTL_VCT_SINGLE)		*/
			/* (DCTL_VDW_16 | DCTL_VCT_SINGLE)		*/
			/* (DCTL_VDW_32 | DCTL_VCT_SINGLE)		*/
			/* (DCTL_VDW_64 | DCTL_VCT_SINGLE)		*/
			/* (DCTL_VDW_32 | DCTL_VCT_BLK)	       ** BLT  	*/
			/* (DCTL_VDW_64 | DCTL_VCT_BLK)	       ** MBLT 	*/
    UINT32 *addrSpace,	/* output: Ptr to VMEbus Address Space type	*/
			/* Valid range:					*/
			/* DCTL_VAS_A16					*/
			/* DCTL_VAS_A24					*/
			/* DCTL_VAS_A32					*/
    UINT32 *dataType,	/* output: Ptr to Program/Data AM Code		*/
			/* Valid range:					*/
			/* DCTL_PGM_DATA				*/
			/* DCTL_PGM_PRGM				*/
    UINT32 *userType	/* output: Ptr to Supervisor/User AM Code	*/
			/* Valid range:					*/
			/* DCTL_SUPER_USER				*/
			/* DCTL_SUPER_SUP				*/
    )
    {
    UINT32  dctlReg;

    if (!sysVmeDmaReady)
	{
	return (ERROR);
	}

    /* Get contents of DMA Transfer Control register */

    UNIV_IN_LONG(UNIVERSE_DCTL, &dctlReg);

    /* Extract the VMEbus data transfer type */

    *xferType = (dctlReg & (DCTL_VDW_MSK | DCTL_VCT_MSK));

    /* Extract the VMEbus Address Space type */

    *addrSpace = (dctlReg & DCTL_VAS_MSK);

    /* Extract the Program/Data AM Code */

    *dataType = (dctlReg & DCTL_PGM_MSK);

    /* Extract the Supervisor/User AM Code */

    *userType = (dctlReg & DCTL_SUPER_MSK);

    return (OK);
    }

/******************************************************************************
*
* sysVmeDmaCnfgSet -  Set DMA transfer configuration parameters
*
* This routine will configure the DMA transfer parameters.
*
* RETURNS: OK or ERROR
*/

STATUS sysVmeDmaCnfgSet
    (
    UINT32 xferType,	/* input: VMEbus data transfer type		*/
			/* Valid range:					*/
			/* (DCTL_VDW_8  | DCTL_VCT_SINGLE)		*/
			/* (DCTL_VDW_16 | DCTL_VCT_SINGLE)		*/
			/* (DCTL_VDW_32 | DCTL_VCT_SINGLE)		*/
			/* (DCTL_VDW_64 | DCTL_VCT_SINGLE)		*/
			/* (DCTL_VDW_32 | DCTL_VCT_BLK)	        ** BLT  */
			/* (DCTL_VDW_64 | DCTL_VCT_BLK)	        ** MBLT */
    UINT32 addrSpace,	/* input: VMEbus Address Space type		*/
			/* Valid range:					*/
			/* DCTL_VAS_A16					*/
			/* DCTL_VAS_A24					*/
			/* DCTL_VAS_A32					*/
    UINT32 dataType,	/* imput: Ptr to Program/Data AM Code		*/
			/* Valid range:					*/
			/* DCTL_PGM_DATA				*/
			/* DCTL_PGM_PRGM				*/
    UINT32 userType	/* input: Supervisor/User AM Code		*/
			/* Valid range:					*/
			/* DCTL_SUPER_USER				*/
			/* DCTL_SUPER_SUP				*/
    )
    {
    UINT32  dctlReg;

    if (!sysVmeDmaReady)
	{
	return (ERROR);
	}

    /* Get contents of DMA Transfer Control register */

    UNIV_IN_LONG(UNIVERSE_DCTL, &dctlReg);

    /* Setup the VMEbus data transfer type */

    dctlReg &= ~(DCTL_VDW_MSK | DCTL_VCT_MSK);
    dctlReg |= (xferType & (DCTL_VDW_MSK | DCTL_VCT_MSK));

    /* Setup the VMEbus Address Space type */

    dctlReg &= ~(DCTL_VAS_MSK);
    dctlReg |= (addrSpace & DCTL_VAS_MSK);

    /* Setup the Program/Data AM Code */

    dctlReg &= ~(DCTL_PGM_MSK);
    dctlReg |= (dataType & DCTL_PGM_MSK);

    /* Setup the Supervisor/User AM Code */

    dctlReg &= ~(DCTL_SUPER_MSK);
    dctlReg |= (userType & DCTL_SUPER_MSK);

    /* Update contents of DMA Transfer Control register */

    UNIV_OUT_LONG(UNIVERSE_DCTL, dctlReg);

    return (OK);
    }

/******************************************************************************
*
* sysVmeDmaStatusGet - Get DMA transfer Status
*
* This routine will return the status of the DMA transfer.
*
* RETURNS: OK or ERROR
*/

STATUS sysVmeDmaStatusGet
    (
    UINT32  *transferStatus	/* State of DMA transfer defined as	*/
				/* DGCS register status bits		*/
				/* DGCS_ACT  | DGCS_STOP | DGCS_HALT |	*/
				/* DGCS_DONE | DGCS_LERR | DGCS_VERR |	*/
				/* DGCS_P_ERR				*/
    )
    {
    UINT32  dgcsReg;

    if (!sysVmeDmaReady)
	{
	return (ERROR);
	}

    /* Get contents of DMA General Control/Status register */

    UNIV_IN_LONG(UNIVERSE_DGCS, &dgcsReg);

    /* Return the status of the DMA transfer */

    *transferStatus = (dgcsReg & DGCS_STATUS_MSK);

    return (OK);
    }


/******************************************************************************
*
* sysVmeDmaL2VCopy - Copy data from local memory to VMEbus memory
*
* This routine copies data from local memory to VMEbus memory using
* the Universe's DMA engine.
*
* NOTE: This routine assumes that the DMA transfer configuration
* parameters have been previously set up either at driver init time
* (sysVmeDmaInit()) with the default parameters in config.h or
* dynamically by calling sysVmeDmaCnfgSet().
*
* .CS
* RETURNS: OK,
*	   ERROR - driver not initialized or invalid argument,
*	   DGCS_LERR - PCI Bus Error,
*	   DGCS_VERR - VMEbus Error,
*          or
*	   DGCS_P_ERR - Protocol Error
* .CE
*/

STATUS sysVmeDmaL2VCopy
    (
    UCHAR  *localAddr,		/* Local Address as seen by the CPU */
    UCHAR  *localVmeAddr,	/* VMEbus Address as seen by the CPU */
    UINT32  nbytes          	/* number of bytes to copy */
    )
    {
    return (sysVmeDmaCopy(localAddr,
			  localVmeAddr,
			  nbytes,
			  (UINT32)DCTL_L2V_PCI_VME));
    }


/******************************************************************************
*
* sysVmeDmaV2LCopy - Copy data from VMEbus memory to local memory
*
* This routine copies data from VMEbus memory to local memory using
* the Universe's DMA engine.
*
* NOTE: This routine assumes that the DMA transfer configuration
* parameters have been previously set up either at driver init time
* (sysVmeDmaInit()) with the default parameters in config.h or
* dynamically by calling sysVmeDmaCnfgSet().
*
* .CS
* RETURNS: OK,
*	   ERROR - driver not initialized or invalid argument,
*	   DGCS_LERR - PCI Bus Error,
*	   DGCS_VERR - VMEbus Error,
*          or
*	   DGCS_P_ERR - Protocol Error
* .CE
*/

STATUS sysVmeDmaV2LCopy
    (
    UCHAR  *localVmeAddr,	/* VMEbus Address as seen by the CPU */
    UCHAR  *localAddr,		/* Local Address as seen by the CPU */
    UINT32  nbytes          	/* number of bytes to copy */
    )
    {
    return (sysVmeDmaCopy(localAddr,
			  localVmeAddr,
			  nbytes,
			  (UINT32)DCTL_L2V_VME_PCI));
    }


/******************************************************************************
*
* sysVmeDmaCopy - Copy data between local and VMEbus memory
*
* This routine copies data between local and VMEbus memory using
* the Universe's DMA engine.  In addition to the parameters passed in,
* this routine uses the macros VME_DMA_MAX_BURST and VME_DMA_MIN_TIME_OFF_BUS
* defined in config.h to set up VON and VOFF, respectfully.
*
* NOTE: This routine assumes that the DMA transfer configuration
* parameters have been previously set up either at driver init time
* (sysVmeDmaInit()) with the default parameters in config.h or
* dynamically by calling sysVmeDmaCnfgSet().
*
* The sampling rate to determine the DMA transfer status
* will directly affect the throughput of the DMA transfer.
*
* .CS
* RETURNS: OK,
*	   ERROR - driver not initialized or invalid argument,
*	   DGCS_LERR - PCI Bus Error,
*	   DGCS_VERR - VMEbus Error,
*          or
*	   DGCS_P_ERR - Protocol Error
* .CE
*/

LOCAL STATUS sysVmeDmaCopy
    (
    UCHAR  *localAddr,		/* Local Address as seen by the CPU */
    UCHAR  *localVmeAddr,	/* VMEbus Address as seen by the CPU */
    UINT32  nbytes,		/* number of bytes to copy: */
				/* max = DTBC_VALID_BITS_MASK */
    UINT32  direction	/* Copy direction: DCTL_L2V_PCI_VME, DCTL_L2V_VME_PCI */
    )
    {
    UCHAR  *pciAddr;
    UCHAR  *vmeAddr;
    UINT32  dctlReg;
    UINT32  dgcsReg;

    if (!sysVmeDmaReady)
	{
	return (ERROR);
	}

    /* Check addresses for 8-byte alignment to each other */

    if (((UINT32)localAddr & 0x7) != ((UINT32)localVmeAddr & 0x7))
        {
	return (ERROR);
	}

    /* Check for too large a transfer byte count */

    if (nbytes > DTBC_VALID_BITS_MASK)
	{
	return (ERROR);
	}

    /*
     * Clearing the chain bit and setting the GO bit at the same time
     * does not work.  So, clearing the chain bit and setting up VON
     * and VOFF is done before setting up the source and destination
     * register as called out in the Universe manual.
     */

    UNIV_IN_LONG(UNIVERSE_DGCS, &dgcsReg);
    dgcsReg &= ~(DGCS_CHAIN_MSK | DGCS_VON_MSK | DGCS_VOFF_MSK);
    dgcsReg |= (VME_DMA_MAX_BURST | VME_DMA_MIN_TIME_OFF_BUS);
    UNIV_OUT_LONG(UNIVERSE_DGCS, dgcsReg);

    /* Setup transfer direction */

    UNIV_IN_LONG(UNIVERSE_DCTL, &dctlReg);
    dctlReg &= ~(DCTL_L2V_MSK);
    dctlReg |= (direction & DCTL_L2V_MSK);
    UNIV_OUT_LONG(UNIVERSE_DCTL, dctlReg);

    /*
     * Convert the VME address as seen by the CPU to a
     * VMEbus address as seen on the VMEbus, and store
     * it in the DMA engine's VMEbus address register.
     */

    switch (dctlReg & DCTL_VAS_MSK)
	{
	case DCTL_VAS_A32:
            vmeAddr = (UCHAR *)((UINT32)localVmeAddr -
				(UINT32)VME_A32_MSTR_LOCAL +
				(UINT32)VME_A32_MSTR_BUS);
            UNIV_OUT_LONG(UNIVERSE_DVA, vmeAddr);
	    break;

	case DCTL_VAS_A24:
            vmeAddr = (UCHAR *)((UINT32)localVmeAddr -
				(UINT32)VME_A24_MSTR_LOCAL +
				(UINT32)VME_A24_MSTR_BUS);
            UNIV_OUT_LONG(UNIVERSE_DVA, vmeAddr);
	    break;

	case DCTL_VAS_A16:
            vmeAddr = (UCHAR *)((UINT32)localVmeAddr -
				(UINT32)VME_A16_MSTR_LOCAL +
				(UINT32)VME_A16_MSTR_BUS);
            UNIV_OUT_LONG(UNIVERSE_DVA, vmeAddr);
	    break;

	default:
	    return (ERROR);
	}

    /*
     * Convert the local address to a PCI address as seen
     * on the PCI Bus, and store it in the DMA engine's
     * PCI Bus address register.
     */

    pciAddr = (UCHAR *)((UINT32)localAddr + (UINT32)PCI_SLV_MEM_BUS);
    UNIV_OUT_LONG(UNIVERSE_DLA, pciAddr);

    /* Configure DMA engine's byte count */

    UNIV_OUT_LONG(UNIVERSE_DTBC, nbytes);

    /* Start transfer by clearing status and setting GO bit */

    dgcsReg &= ~(DGCS_STATUS_MSK);
    dgcsReg |= DGCS_GO;
    UNIV_OUT_LONG(UNIVERSE_DGCS, dgcsReg);

    /* Wait for transfer to terminate */

    UNIV_IN_LONG(UNIVERSE_DGCS, &dgcsReg);
    while (dgcsReg & DGCS_ACT)
	{
	sysUsDelay(25);
	UNIV_IN_LONG(UNIVERSE_DGCS, &dgcsReg);
	}

    if (dgcsReg & DGCS_DONE)
	{
	return (OK);
	}
    else
	{
        return (dgcsReg & (DGCS_LERR | DGCS_VERR | DGCS_P_ERR));
	}
    }


#endif	/* INCLUDE_VME_DMA */

