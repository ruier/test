/* sysALib.s - wrSbcPowerQuiccII system-dependent assembly routines */

/*
 * Copyright (c) 2003-2005, 2007, 2008, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01t,21aug12,l_z  Better implementation for sysPCGet. (WIND00076160)
01s,17oct08,jmt  Setup default value for RAM_HIGH_ADRS
01r,24jul07,z_l  Removed hardware initialization while using software FP
                 library (WIND00094352)
01q,03sep05,mil  Removed reference to VXLAYER.
01p,08aug05,mil  Added .data for sysCpuId.
01o,24jul05,mil  Added condition to include sysCacheLockALib.s.
01n,13jul05,mil  Removed reference to copyright_wind_river.
01m,04mar04,j_b  reset type based on startType passed into sysInit (SPR#94336)
01l,22jan04,j_b  clarify PCI and i8259A dependencies (SPR#93321)
01k,16jul03,j_b  add RTC support; remove vware support
01j,18jun03,j_b  cleanup
01i,03jun03,scm  add better delay routine based off TBL reg...
01h,29may03,j_b  remove INCLUDE_FEI_END condition
01g,09may03,scm  update for PCI support...
01f,30apr03,j_b  remove sysCpuIdInit; replace INIT_ register names with PQII_
01e,25apr03,j_b  remove sysStatusRegSet, register is read-only
01d,24apr03,j_b  add CPU determination and status register support
01c,22apr03,scm  add low level support routines for i8259A & PCI...
01b,16apr03,j_b  add boot progress tracking on LED
01a,01apr03,j_b  adapted from wrSbc8260Atm (ver 01c)
*/

/*
DESCRIPTION
This module contains system-dependent routines written in assembly
language.

This module must be the first specified in the \f3ld\f1 command used to
build the system.  The sysInit() routine is the system start-up code.
*/

#define _ASMLANGUAGE
/* includes */
#include <vxWorks.h>
#include <sysLib.h>
#include "config.h"
#include <regs.h>
#include <asm.h>
#include <drv/timer/m8260Clock.h>
#include "wrSbcPowerQuiccII.h"
#include "sysLed.h"

#ifdef RAM_LOW_ADRS
#define SYS_RAM_LOW_ADRS RAM_LOW_ADRS
#else
#define SYS_RAM_LOW_ADRS 0x01C00000
#endif /* RAM_LOW_ADRS */

	/* globals */

    FUNC_EXPORT(_sysInit)           /* start of system code */
    FUNC_EXPORT(sysPCGet)           /* get the value of the PC register */
    FUNC_EXPORT(sysCPUIdGet)        /* get the CPU ID value */
    FUNC_EXPORT(sysStatusRegGet)    /* get the status register value */

#ifdef INCLUDE_i8259A
        FUNC_EXPORT(sysTimeBaseLGet)

        FUNC_EXPORT(sysInWord)
        FUNC_EXPORT(sysOutWord)
        FUNC_EXPORT(sysInLong)
        FUNC_EXPORT(sysOutLong)
        FUNC_EXPORT(sysInByte)
        FUNC_EXPORT(sysOutByte)

#  ifdef INCLUDE_PCI
        FUNC_EXPORT(sysPciRead32)
        FUNC_EXPORT(sysPciWrite32)

        FUNC_EXPORT(sysPciInByte)
        FUNC_EXPORT(sysPciOutByte)
        FUNC_EXPORT(sysPciInWord)
        FUNC_EXPORT(sysPciOutWord)
        FUNC_EXPORT(sysPciInLong)
        FUNC_EXPORT(sysPciOutLong)
#  endif /* INCLUDE_PCI */
#endif /* INCLUDE_i8259A */

	/* externals */

	FUNC_IMPORT(usrInit)
	FUNC_IMPORT(vxImmrGet)

/* local CPU ID storage */
	.data
	.balign 4
sysCpuId:
	.long 	(MAX_CPU_ID + 1)

	_WRS_TEXT_SEG_START

/***************************************************************************
*
* sysInit - start after boot
*
* This is the system start-up entry point for VxWorks in RAM, the
* first code executed after booting.  It disables interrupts, sets up
* the stack, and jumps to the C routine usrInit() in usrConfig.c.
*
* The initial stack is set to grow down from the address of sysInit().  This
* stack is used only by usrInit() and is never used again.  Memory for the
* stack must be accounted for when determining the system load address.
*
* SYNOPSIS
* \ss
* sysInit
*     (
*     int startType     /@ THIS IS NOT A CALLABLE ROUTINE @/
*     )
* \se
*
* NOTE: This routine should not be called by the user.
*
* RETURNS: N/A
*/

FUNC_BEGIN(_sysInit)

#ifdef USE_LEDS_FOR_BOOT_DEBUG
 	li      r5,LED_4			/* start of _sysInit */
	lis     r0,HIADJ(BSCR_LED_REGISTER)
	addi	r0,r0,LO(BSCR_LED_REGISTER)
	stb     r5,0(r0)
#endif

        /* disable external interrupts (by zeroing out msr) */
	xor     r5,r5,r5
	isync
	mtmsr	r5
	isync

#ifdef M48T59_RTC_SRAM
       /* disable MCP, machine check input signal used by RTC interrupts */
    lis     r5,HIADJ(_PPC_HID0_EMCP)
#endif /* M48T59_RTC_SRAM */

       /* invalidate and disable the MPU's data/instruction caches */
	mfspr	r6,HID0
	ori	    r5,r5,(_PPC_HID0_ICE | _PPC_HID0_DCE)
	andc	r6,r6,r5	 /* clear cache enable bits in r6 */
	mr	    r5,r6
	ori     r5,r5,(_PPC_HID0_ICE | _PPC_HID0_DCE | _PPC_HID0_ICFI | _PPC_HID0_DCFI)
	sync
	mtspr	HID0,r5		 /* invalidate both caches with 2 stores */
	isync
	mtspr	HID0,r6		 /* leaving them both disabled */
	isync

	mtspr   SPRG0,r3
	mtspr   SPRG1,r4

	bl	sysClearBATs

#ifdef USE_LEDS_FOR_BOOT_DEBUG
 	li	r5,LED_5			/* end of sysClearBATs */
	lis	r0,HIADJ(BSCR_LED_REGISTER)
	addi	r0,r0,LO(BSCR_LED_REGISTER)
	stb	r5,0(r0)
#endif

	bl	sysInvalidateTLBs

#ifdef USE_LEDS_FOR_BOOT_DEBUG
 	li	r5,LED_6			/* end of sysInvalidateTLBs */
	lis	r0,HIADJ(BSCR_LED_REGISTER)
	addi	r0,r0,LO(BSCR_LED_REGISTER)
	stb	r5,0(r0)
#endif

	bl	sysClearSegs

#ifdef USE_LEDS_FOR_BOOT_DEBUG
 	li	r5,LED_7			/* end of sysClearSegs */
	lis	r0,HIADJ(BSCR_LED_REGISTER)
	addi	r0,r0,LO(BSCR_LED_REGISTER)
	stb	r5,0(r0)
#endif

	mfspr   r3,SPRG0
	mfspr   r4,SPRG1

    /* save startType */
    mr      r7,r3

    /* Zero-out registers: r0 & SPRGs */
    xor     r0,r0,r0
	mtspr	SPRG0,r0
	mtspr	SPRG1,r0
	mtspr	SPRG2,r0
	mtspr	SPRG3,r0

#ifdef _PPC_MSR_FP
	/* Turn on FP (temporarily) */
        ori	r3,r0,_PPC_MSR_FP
        mtmsr   r3
        sync

        /* Init the floating point control/status register */
        mtfsfi  7,0x0
        mtfsfi  6,0x0
        mtfsfi  5,0x0
        mtfsfi  4,0x0
        mtfsfi  3,0x0
        mtfsfi  2,0x0
        mtfsfi  1,0x0
        mtfsfi  0,0x0
        isync

        /* Initialize the floating point data registers to a known state */

        bl      ifpdrValue
       .long   0x3f800000      /* IEEE single-precision value of 1.0 */
ifpdrValue:
        mflr    r3
        lfs     f0,0(r3)
        lfs     f1,0(r3)
        lfs     f2,0(r3)
        lfs     f3,0(r3)
        lfs     f4,0(r3)
        lfs     f5,0(r3)
        lfs     f6,0(r3)
        lfs     f7,0(r3)
        lfs     f8,0(r3)
        lfs     f9,0(r3)
        lfs     f10,0(r3)
        lfs     f11,0(r3)
        lfs     f12,0(r3)
        lfs     f13,0(r3)
        lfs     f14,0(r3)
        lfs     f15,0(r3)
        lfs     f16,0(r3)
        lfs     f17,0(r3)
        lfs     f18,0(r3)
        lfs     f19,0(r3)
        lfs     f20,0(r3)
        lfs     f21,0(r3)
        lfs     f22,0(r3)
        lfs     f23,0(r3)
        lfs     f24,0(r3)
        lfs     f25,0(r3)
        lfs     f26,0(r3)
        lfs     f27,0(r3)
        lfs     f28,0(r3)
        lfs     f29,0(r3)
        lfs     f30,0(r3)
        lfs     f31,0(r3)
        sync

	/*
	 * MSR: clear DCE,ICE,EE,DR,IR,ME -- set FP,RI
	 * Do not enable Machine Check until the vectors are set up.
	 */
	ori	r4,r0,(_PPC_MSR_FP | _PPC_MSR_RI)
	mtmsr   r4
	sync
#else
    ori	r4,r0,(_PPC_MSR_RI)
	mtmsr   r4
	sync
#endif
        /* fake a Decrementer Exception Handler (i.e. rfi @ 0x900) */
	lis 	r3,HI(0x4C000064)
	ori	r3,r3,LO(0x4C000064) /* load rfi (0x4c000064) to */
	stw 	r3,0x900(r0)	     /* store rfi at 0x00000900  */

#ifdef USE_LEDS_FOR_BOOT_DEBUG
 	li	r5,LED_8			/* end of rfi */
	lis	r0,HIADJ(BSCR_LED_REGISTER)
	addi	r0,r0,LO(BSCR_LED_REGISTER)
	stb	r5,0(r0)
#endif

        /* Get internal register base address */
	lis	r4, HIADJ(INTERNAL_MEM_MAP_ADDR + PQII_REG_BASE)
	addi	r4, r4, LO(INTERNAL_MEM_MAP_ADDR + PQII_REG_BASE)
	sync

        /* Set SCCR */
#ifdef INCLUDE_PCI
        lis     r5, HIADJ(M8260_SCCR_DFBRG_4 | M8260_SCCR_PCI_MODE_BIT)  /* divide by 4 */
        addi    r5, r5, LO(M8260_SCCR_DFBRG_4 | M8260_SCCR_PCI_MODE_BIT) /* divide by 4 */
#else
        lis     r5, HIADJ(M8260_SCCR_DFBRG_4)  /* divide by 4 */
        addi    r5, r5, LO(M8260_SCCR_DFBRG_4) /* divide by 4 */
#endif
	stw	r5, PQII_SCCR(r4)

#if FALSE	  			    /* XXX TPR NO SDA for now */
    /* initialize Small Data Area (SDA) start address */
	lis     r2, HI(_SDA2_BASE_)
	ori 	r2, r2, LO(_SDA2_BASE_)

	lis     r13, HI(_SDA_BASE_)
	ori	r13, r13, LO(_SDA_BASE_)
#endif

    /* initialize the stack pointer */
	lis	sp, HI(SYS_RAM_LOW_ADRS)	/* NOTE:  sp = r1 */
	ori	sp, sp, LO(SYS_RAM_LOW_ADRS)
	addi	sp, sp, -FRAMEBASESZ    /* get frame stack */

    /* jump to usrInit with arg indicating type of boot (startType) */

    mr  r3, r7

	isync

#ifdef USE_LEDS_FOR_BOOT_DEBUG
	li	r5,LED_9            /* end of _sysInit */
	lis	r0,HIADJ(BSCR_LED_REGISTER)
	addi	r0,r0,LO(BSCR_LED_REGISTER)
	stb	r5,0(r0)
#endif

	b	usrInit		 /* never returns - starts up kernel */
                                /* (jumps to usrConfig.c )          */
FUNC_END(_sysInit)

/******************************************************************************
*
* sysPCGet - Get the value of the PC (Program Counter)
*
* This routine returns the value of the PC.
*
* SYNOPSIS
* \ss
* UINT32 sysPCGet
*     (
*     void
*     )
* \se
*
* RETURNS: the Program Counter Register (PC) value.
*/

FUNC_BEGIN(sysPCGet)
	mflr	r3 
	blr
FUNC_END(sysPCGet)

/***************************************************************************
*
* sysClearBATs - clear all the BAT's register
*
* This routine will zero the BAT's register.
*
* SYNOPSIS
* \ss
* void sysClearBATs
*     (
*     void
*     )
* \se
*
* SEE ALSO: sysInvalidateTLBs()
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysClearBATs)

        /* zero out the BAT registers */
	xor	r3,r3,r3
	isync
	mtspr	IBAT0U,r3	/* clear all upper BATS first */
	mtspr	IBAT1U,r3
	mtspr	IBAT2U,r3
	mtspr	IBAT3U,r3
	mtspr	DBAT0U,r3
	mtspr	DBAT1U,r3
	mtspr	DBAT2U,r3
	mtspr	DBAT3U,r3

	mtspr	IBAT0L,r3	/* then clear lower BATS */
	mtspr	IBAT1L,r3
	mtspr	IBAT2L,r3
	mtspr	IBAT3L,r3
	mtspr	DBAT0L,r3
	mtspr	DBAT1L,r3
	mtspr	DBAT2L,r3
	mtspr	DBAT3L,r3
	isync
	blr
FUNC_END(sysClearBATs)

/***************************************************************************
*
* sysClearSegs - clearing all the SEG's register.
*
* This routine will zero the SEG's register.
*
* SYNOPSIS
* \ss
* void sysClearSegs
*     (
*     void
*     )
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysClearSegs)

	/* Init the Segment registers */
	xor	r3, r3, r3
        isync
        mtsr    0,r3
        mtsr    1,r3
        mtsr    2,r3
        mtsr    3,r3
        mtsr    4,r3
        mtsr    5,r3
        mtsr    6,r3
        mtsr    7,r3
        mtsr    8,r3
        mtsr    9,r3
        mtsr    10,r3
        mtsr    11,r3
        mtsr    12,r3
        mtsr    13,r3
        mtsr    14,r3
        mtsr    15,r3
        isync
	blr

FUNC_END(sysClearSegs)

/***************************************************************************
*
* sysInvalidateTLBs - invalidate all TLB entries
*
* This routine will invalidate instruction and data TLB entries.
*
* SYNOPSIS
* \ss
* void sysInvalidateTLBs
*     (
*     void
*     )
* \se
*
* SEE ALSO: sysClearBATs()
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysInvalidateTLBs)
	isync
	/* invalidate entries within both 64-entry TLBs */
	li	r3,128
	mtctr	r3		/* CTR = 128  */
	xor	r3,r3,r3	/* r3 = 0    */
	isync			/* context sync req'd before tlbie */
tlbloop:
	tlbie	r3
	sync			/* sync instr req'd after tlbie      */
	addi	r3,r3,0x1000	/* increment bits 15-19 */
	bdnz	tlbloop		/* decrement CTR, branch if CTR != 0 */
	isync
	blr
FUNC_END(sysInvalidateTLBs)

/******************************************************************************
*
* sysCPUIdGet - get the CPU Identification value
*
* This routine returns the value of the CPU ID based on user switch settings
* or a hardcoded value.
*
* SYNOPSIS
* \ss
* UINT32 sysCPUIdGet
*     (
*     void
*     )
* \se
*
* RETURNS: CPU ID value.
*/

FUNC_BEGIN(sysCPUIdGet)
	lis	r5,HI(sysCpuId)
	ori	r5,r5,LO(sysCpuId)
	lwz	r3,0(r5)

        /* if sysCpuId has been initialized, return the value */
	cmpwi   r3,MAX_CPU_ID
	ble	cpuIdRdy

        /* initialize sysCpuId */
#ifndef HARDCODED_CPU_ID
	lis	r5,HI(BSCR_USER_SWITCH)
	ori	r5,r5,LO(BSCR_USER_SWITCH)
	lbz	r3,0(r5)
#else
	li	r3,CPU_ID
#endif
	andi.	r3,r3,CPU_ID_MASK
	lis	r5,HI(sysCpuId)
	ori	r5,r5,LO(sysCpuId)
	stw	r3,0(r5)

cpuIdRdy:
	blr
FUNC_END(sysCPUIdGet)

/******************************************************************************
*
* sysStatusRegGet - get the status register value
*
* This routine returns the status register value.
* In order to read the entire 16-bit register, the port size attribute for
* its chip select (CS5) must be changed temporarily from 8-bits to 16-bits.
*
* SYNOPSIS
* \ss
* UINT16 sysStatusRegGet
*     (
*     void
*     )
* \se
*
* RETURNS: status register value
*/

FUNC_BEGIN(sysStatusRegGet)
	/* read BR5 contents */
	mflr	r7
	bl	vxImmrGet
	lis	r4,HI(PQII_REG_BASE)
	add	r4,r4,r3
	lwz	r5,PQII_BR5(r4)

	/* change CS5 port size to 16 bits */
	lis	r6,HI(BR_PORT_SZ_MASK)
	ori	r6,r6,LO((BR_PORT_SZ_MASK + BR_VALID))
	andc	r5,r5,r6
	ori	r5,r5,LO((BR_PORT_SZ_16BIT + BR_VALID))
	stw	r5,PQII_BR5(r4)

	/* read the status register */
	lis	r8,HI(BSCR_STATUS)
	ori	r8,r8,LO(BSCR_STATUS)
	lhz	r3,0(r8)

	/* reset CS5 port size to 8 bits */
	andc	r5,r5,r6
	ori	r5,r5,LO((BR_PORT_SZ_8BIT + BR_VALID))
	stw	r5,PQII_BR5(r4)

	mtlr	r7
	blr
FUNC_END(sysStatusRegGet)

#ifdef INCLUDE_i8259A
/******************************************************************************
*
* sysTimeBaseLGet - Get lower half of Time Base Register
*
* This routine will read the contents the lower half of the Time
* Base Register (TBL - TBR 268).
*
* From a C point of view, the routine is defined as follows:
* SYNOPSIS
* \ss
*    UINT32 sysTimeBaseLGet(void)
* \se
* RETURNS: value of TBR 268 (in r3)
*/

FUNC_BEGIN(sysTimeBaseLGet)
    mftb 3
    bclr 20,0
FUNC_END(sysTimeBaseLGet)

/*****************************************************************************
*
* sysInByte - reads a byte from an io address.
*
* This function reads a byte from a specified io address.
*
* RETURNS: byte from address.

* UCHAR sysInByte
*     (
*     UCHAR *  pAddr            /@ Virtual I/O addr to read from @/
*     )

*/

FUNC_BEGIN(sysInByte)
        eieio                   /* Sync I/O operation */
        sync
        lbzx    p0,r0,p0        /* Read byte from I/O space */
        bclr    20,0            /* Return to caller */
FUNC_END(sysInByte)

/******************************************************************************
*
* sysOutByte - writes a byte to an io address.
*
* This function writes a byte to a specified io address.
*
* RETURNS: N/A

* VOID sysOutByte
*     (
*     UCHAR *  pAddr,           /@ Virtual I/O addr to write to @/
*     UCHAR    data             /@ data to be written @/
*     )

*/

FUNC_BEGIN(sysOutByte)
        stbx    p1,r0,p0        /* Write a byte to PCI space */
        eieio                   /* Sync I/O operation */
        sync
        bclr    20,0            /* Return to caller */
FUNC_END(sysOutByte)

/*****************************************************************************
*
* sysInWord - reads a word from an address, swapping the bytes.
*
* This function reads a swapped word from a specified
* address.
*
* RETURNS:
* Returns swapped 16 bit data from the specified address.

* USHORT sysInWord
*     (
*     ULONG  address,           /@ addr to read from @/
*     )

*/

FUNC_BEGIN(sysInWord)
        eieio                   /* Sync I/O operation */
        sync
        lhbrx   p0,r0,p0        /* Read and swap */
        bclr    20,0            /* Return to caller */
FUNC_END(sysInWord)

/*****************************************************************************
*
* sysOutWord - writes a word to an address swapping the bytes.
*
* This function writes a swapped word to a specified
* address.
*
* RETURNS: N/A

* VOID sysOutWord
*     (
*     ULONG address,            /@ Virtual addr to write to @/
*     UINT16  data              /@ Data to be written       @/
*     )

*/

FUNC_BEGIN(sysOutWord)
        sthbrx  p1,r0,p0        /* Write with swap to address */
        eieio                   /* Sync I/O operation */
        sync
        bclr    20,0            /* Return to caller */
FUNC_END(sysOutWord)

/*****************************************************************************
*
* sysInLong - reads a long from an address.
*
* This function reads a long from a specified PCI Config Space (little-endian)
* address.
*
* RETURNS:
* Returns 32 bit data from the specified register.  Note that for PCI systems
* if no target responds, the data returned to the CPU will be 0xffffffff.

* ULONG sysInLong
*     (
*     ULONG  address,           /@ Virtual addr to read from @/
*     )

*/

FUNC_BEGIN(sysInLong)
        eieio                   /* Sync I/O operation */
        sync
        lwbrx   p0,r0,p0        /* Read and swap from address */
        bclr    20,0            /* Return to caller */
FUNC_END(sysInLong)


/******************************************************************************
*
* sysOutLong - write a swapped long to address.
*
* This routine will store a 32-bit data item (input as big-endian)
* into an address in little-endian mode.
*
* RETURNS: N/A

* VOID sysOutLong
*     (
*     ULONG   address,          /@ Virtual addr to write to @/
*     ULONG   data              /@ Data to be written @/
*     )

*/

FUNC_BEGIN(sysOutLong)
        stwbrx  p1,r0,p0        /* store data as little-endian */
        eieio                   /* Sync I/O operation */
        sync
        bclr    20,0
FUNC_END(sysOutLong)

#  ifdef INCLUDE_PCI
/******************************************************************************
*
* sysPciRead32 - read 32 bit PCI data
*
* This routine will read a 32-bit data item from PCI (I/O or
* memory) space.
*
* RETURNS: N/A

* VOID sysPciRead32
*     (
*     ULONG *  pAddr,           /@ Virtual addr to read from @/
*     ULONG *  pResult          /@ location to receive data @/
*     )

*/

FUNC_BEGIN(sysPciRead32)
        eieio                   /* Sync I/O operation */
        lwbrx   p0,r0,p0        /* get the data and swap the bytes */
        stw     p0,0(p1)        /* store into address ptd. to by p1 */
        bclr    20,0
FUNC_END(sysPciRead32)

/******************************************************************************
*
* sysPciWrite32 - write a 32 bit data item to PCI space
*
* This routine will store a 32-bit data item (input as big-endian)
* into PCI (I/O or memory) space in little-endian mode.
*
* RETURNS: N/A

* VOID sysPciWrite32
*     (
*     ULONG *  pAddr,           /@ Virtual addr to write to @/
*     ULONG   data              /@ Data to be written @/
*     )

*/

FUNC_BEGIN(sysPciWrite32)
        stwbrx  p1,r0,p0        /* store data as little-endian */
        bclr    20,0
FUNC_END(sysPciWrite32)

/*****************************************************************************
*
* sysPciInByte - reads a byte from PCI Config Space.
*
* This function reads a byte from a specified PCI Config Space address.
*
* RETURNS:
* Returns 8 bit data from the specified register.  Note that for PCI systems
* if no target responds, the data returned to the CPU will be 0xff.

* UINT8 sysPciInByte
*     (
*     UINT8 *  pAddr,           /@ Virtual addr to read from @/
*     )

*/

FUNC_BEGIN(sysPciInByte)
        eieio                   /* Sync I/O operation */
        lbzx    p0,r0,p0        /* Read byte from PCI space */
        bclr    20,0            /* Return to caller */
FUNC_END(sysPciInByte)

/*****************************************************************************
*
* sysPciInWord - reads a word (16-bit big-endian) from PCI Config Space.
*
* This function reads a word from a specified PCI Config Space (little-endian)
* address.
*
* RETURNS:
* Returns 16 bit data from the specified register.  Note that for PCI systems
* if no target responds, the data returned to the CPU will be 0xffff.

* USHORT sysPciInWord
*     (
*     USHORT *  pAddr,          /@ Virtual addr to read from @/
*     )

*/

FUNC_BEGIN(sysPciInWord)
        eieio                   /* Sync I/O operation */
        lhbrx   p0,r0,p0        /* Read and swap from PCI space */
        bclr    20,0            /* Return to caller */
FUNC_END(sysPciInWord)

/*****************************************************************************
*
* sysPciInLong - reads a long (32-bit big-endian) from PCI Config Space.
*
* This function reads a long from a specified PCI Config Space (little-endian)
* address.
*
* RETURNS:
* Returns 32 bit data from the specified register.  Note that for PCI systems
* if no target responds, the data returned to the CPU will be 0xffffffff.

* ULONG sysPciInLong
*     (
*     ULONG *  pAddr,           /@ Virtual addr to read from @/
*     )

*/

FUNC_BEGIN(sysPciInLong)
        eieio                   /* Sync I/O operation */
        lwbrx   p0,r0,p0        /* Read and swap from PCI space */
        bclr    20,0            /* Return to caller */
FUNC_END(sysPciInLong)

/******************************************************************************
*
* sysPciOutByte - writes a byte to PCI Config Space.
*
* This function writes a byte to a specified PCI Config Space address.
*
* RETURNS: N/A

* VOID sysPciOutByte
*     (
*     UINT8 *  pAddr,           /@ Virtual addr to write to @/
*     UINT8  data               /@ Data to be written       @/
*     )

*/

FUNC_BEGIN(sysPciOutByte)
        stbx    p1,r0,p0        /* Write a byte to PCI space */
        bclr    20,0            /* Return to caller */
FUNC_END(sysPciOutByte)

/******************************************************************************
*
* sysPciOutWord - writes a word (16-bit big-endian) to PCI Config Space.
*
* This function writes a word to a specified PCI Config Space (little-endian)
* address.
*
* RETURNS: N/A

* VOID sysPciOutWord
*     (
*     USHORT *  pAddr,          /@ Virtual addr to write to @/
*     USHORT  data              /@ Data to be written       @/
*     )

*/

FUNC_BEGIN(sysPciOutWord)
        sthbrx  p1,r0,p0        /* Write with swap to PCI space */
        bclr    20,0            /* Return to caller */
FUNC_END(sysPciOutWord)

/******************************************************************************
*
* sysPciOutLong - writes a long (32-bit big-endian) to PCI Config Space.
*
* This function writes a long to a specified PCI Config Space (little-endian)
* address.
*
* RETURNS: N/A
* VOID sysPciOutLong
*     (
*     ULONG *  pAddr,           /@ Virtual addr to write to @/
*     ULONG  data               /@ Data to be written       @/
*     )
*/

FUNC_BEGIN(sysPciOutLong)
        stwbrx  p1,r0,p0        /* Write big-endian long to little-endian */
        mr      p0,p1           /* PCI space */
        bclr    20,0            /* Return to caller */
FUNC_END(sysPciOutLong)

#  endif /* INCLUDE_PCI */
#endif /* INCLUDE_i8259A */

#ifdef INCLUDE_CACHE_SUPPORT
#  include "sysCacheLockALib.s"
#endif /* INCLUDE_CACHE_SUPPORT */
