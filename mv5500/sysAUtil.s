/* sysAUtil.s - system-dependent assembly routines */

/* Copyright 1984-2004 Wind River Systems, Inc. */
/* Copyright 1996-2003 Motorola, Inc. All Rights Reserved */

    .data
    .globl    copyright_wind_river
    .long    copyright_wind_river

/*
modification history
--------------------
01f,30sep04,rec  SPR102132 - remove reference to sysProbeExc
01e,02sep04,rec  SPR-93334 replace sysMemProbeSup with vxMemProbeSup
01d,09apr03,yyz  Fixed formatting for T2.2 compliance.
01c,10mar03,cak  Added sysSioRead() and sysSioWrite() to read from and 
		 write to the PCx7307 Super IO device.
01b,29jan03,cak  Added sysTimeBaseLPut() routine.
01a,01oct02,scb  Copied from hxeb100 base (ver 01a).
*/

/*
DESCRIPTION
This module contains system-dependent routines written in assembly
language.
*/

#define _ASMLANGUAGE

/* includes */

#include "vxWorks.h"
#include "sysLib.h"
#include "config.h"
#include "regs.h"
#include "asm.h"

/* defines */

/* Special Purpose Registers */

#define MSSCR0              1014         /* Memory Subsystem Control Register */
#define L2CR                1017         /* L2 Cache Control Register */
#define L3CR                1018         /* L3 Cache Control Register */
#define DEC                 22           /* Decrementer Register */
#define L3PM                983          /* L3 Private Memory Ctrl Register */
#define TBLR                284          /* Time Base Lower Register */

/* globals */

    FUNC_EXPORT(sysPciInByte)
    FUNC_EXPORT(sysPciOutByte)
    FUNC_EXPORT(sysPciInWord)
    FUNC_EXPORT(sysPciOutWord)
    FUNC_EXPORT(sysPciInLong)
    FUNC_EXPORT(sysPciOutLong)
    FUNC_EXPORT(sysInWord)
    FUNC_EXPORT(sysOutWord)
    FUNC_EXPORT(sysInLong)
    FUNC_EXPORT(sysOutLong)
    FUNC_EXPORT(sysL2crPut)
    FUNC_EXPORT(sysL2crGet)
    FUNC_EXPORT(sysTimeBaseLPut)
    FUNC_EXPORT(sysTimeBaseLGet)
    FUNC_EXPORT(sysHid1Get)
    FUNC_EXPORT(sysGetDec)
    FUNC_EXPORT(sysL3pmPut)
    FUNC_EXPORT(sysL3pmGet)
    FUNC_EXPORT(sysMsscr0Put)
    FUNC_EXPORT(sysMsscr0Get)
    FUNC_EXPORT(sysL3crPut)
    FUNC_EXPORT(sysL3crGet)
#ifdef INCLUDE_PCX7307_SIO
    FUNC_EXPORT(sysSioRead)
    FUNC_EXPORT(sysSioWrite)
#endif /* INCLUDE_PCX7307_SIO */

    /* externals */
     
    _WRS_TEXT_SEG_START

/*****************************************************************************
*
* sysPciInByte - reads a byte from PCI Config Space.
*
* This function reads a byte from a specified PCI Config Space address.
*
* ARGUMENTS:
* r3 = Config Space address
*
* RETURNS:
* r3 = byte from address.
*/

FUNC_BEGIN(sysPciInByte)
    lbzx    r3,r0,r3                /* Read byte from PCI space */
    bclr    20,0                    /* Return to caller */
FUNC_END(sysPciInByte)

/*****************************************************************************
*
* sysPciInWord - reads a word (16-bit big-endian) from PCI Config Space.
*
* This function reads a word from a specified PCI Config Space (little-endian)
* address.
*
* ARGUMENTS:
* r3 = Config Space address
*
* RETURNS:
* r3 = word (16-bit big-endian) from address.
*/

FUNC_BEGIN(sysInWord)
FUNC_BEGIN(sysPciInWord)
    lhbrx   r3,r0,r3                /* Read 16 bit byte reversed */
    bclr    20,0                    /* Return to caller */
FUNC_END(sysInWord)
FUNC_END(sysPciInWord)

/*****************************************************************************
*
* sysPciInLong - reads a long (32-bit big-endian) from PCI Config Space.
*
* This function reads a long from a specified PCI Config Space (little-endian)
* address.
*
* ARGUMENTS:
* r3 = Config Space address
*
* RETURNS:
* r3 = long (32-bit big-endian) from address.
*/

FUNC_BEGIN(sysInLong)
FUNC_BEGIN(sysPciInLong)
    lwbrx   r3,r0,r3                /* Read 32 bit byte reversed */
    bclr    20,0                    /* Return to caller */
FUNC_END(sysInLong)
FUNC_END(sysPciInLong)

/******************************************************************************
*
* sysPciOutByte - writes a byte to PCI Config Space.
*
* This function writes a byte to a specified PCI Config Space address.
*
* ARGUMENTS:
* r3 = Config Space address
* r4 = byte to write
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysPciOutByte)
    stbx    r4,r0,r3                /* Write a byte to PCI space */
    sync                            /* Sync I/O operation */
    bclr    20,0                    /* Return to caller */
FUNC_END(sysPciOutByte)

/******************************************************************************
*
* sysPciOutWord - writes a word (16-bit big-endian) to PCI Config Space.
*
* This function writes a word to a specified PCI Config Space (little-endian)
* address.
*
* ARGUMENTS:
* r3 = Config Space address
* r4 = word (16-bit big-endian) to write
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysOutWord)
FUNC_BEGIN(sysPciOutWord)
    sthbrx  r4,r0,r3                /* Write byte-reversed 16 bit value */
    eieio
    sync                            /* Sync I/O operation */
    bclr    20,0                    /* Return to caller */
FUNC_END(sysOutWord)
FUNC_END(sysPciOutWord)

/******************************************************************************
*
* sysPciOutLong - writes a long (32-bit big-endian) to PCI Config Space.
*
* This function writes a long to a specified PCI Config Space (little-endian)
* address.
*
* ARGUMENTS:
* r3 = Config Space address
* r4 = long (32-bit big-endian) to write
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysOutLong)
FUNC_BEGIN(sysPciOutLong)
    stwbrx  r4,r0,r3                /* Write byte-reversed long */
    eieio
    sync                            /* Sync I/O operation */
    bclr    20,0                    /* Return to caller */
FUNC_END(sysOutLong)
FUNC_END(sysPciOutLong)


/*******************************************************************************
*
* sysL2crPut - write to L2CR register
*
* This routine will write the contents of r3 to the L2CR
* register.
*
* From a C point of view, the routine is defined as follows:
*
* void sysL2crPut
* (
* ULONG       value to write
* )
*
* RETURNS: NA
*/

FUNC_BEGIN(sysL2crPut)
    sync
    mtspr   L2CR,r3
    isync
    bclr    20,0
FUNC_END(sysL2crPut)

/*******************************************************************************
*
* sysL2crGet - read from L2CR register
*
* This routine will read the contents the L2CR register.
*
* From a C point of view, the routine is defined as follows:
*
* UINT sysL2crGet()
*
* RETURNS: value of SPR1017 (in r3)
*/

FUNC_BEGIN(sysL2crGet)
    sync
    mfspr   r3,L2CR
    sync
    bclr    20,0
FUNC_END(sysL2crGet)

/*******************************************************************************
*
* sysMsscr0Put - write to MSSCR0 register
*
* This routine will write the contents of r3 to the MSSCR0
* register.
*
* From a C point of view, the routine is defined as follows:
*
* void sysMsscr0Put
* (
* ULONG       value to write
* )
*
* RETURNS: NA
*/

FUNC_BEGIN(sysMsscr0Put)
    sync
    mtspr   MSSCR0,r3
    isync
    bclr    20,0
FUNC_END(sysMsscr0Put)

/*******************************************************************************
*
* sysMsscr0Get - read from MSSCR0 register
*
* This routine will read the contents of the MSSCR0 register.
*
* From a C point of view, the routine is defined as follows:
*
* UINT sysMsscr0Get()
*
* RETURNS: value of SPR1014 (in r3)
*/

FUNC_BEGIN(sysMsscr0Get)
    sync
    mfspr   r3,MSSCR0
    sync
    bclr    20,0
FUNC_END(sysMsscr0Get)

/*******************************************************************************
*
* sysL3crPut - write to L3CR register
*
* This routine will write the contents of r3 to the L3CR
* register.
*
* From a C point of view, the routine is defined as follows:
*
* void sysL3crPut
* (
* ULONG       value to write
* )
*
* RETURNS: NA
*/

FUNC_BEGIN(sysL3crPut)
    sync
    mtspr   L3CR,r3
    isync
    bclr    20,0
FUNC_END(sysL3crPut)

/*******************************************************************************
*
* sysL3crGet - read from L3CR register
*
* This routine will read the contents the L3CR register.
*
* From a C point of view, the routine is defined as follows:
*
* UINT sysL3crGet()
*
* RETURNS: value of SPR1018 (in r3)
*/

FUNC_BEGIN(sysL3crGet)
    sync
    mfspr   r3,L3CR
    sync
    bclr    20,0
FUNC_END(sysL3crGet)

/*******************************************************************************
*
* sysTimeBaseLPut - write to lower half of Time Base register
*
* This routine will write the contents of r3 to the L3CR
* register.
*
* From a C point of view, the routine is defined as follows:
*
* void sysTimeBaseLPut
* (
* ULONG       value to write
* )
*
* RETURNS: NA
*/

FUNC_BEGIN(sysTimeBaseLPut)
    sync
    mtspr   TBLR,r3
    isync
    bclr    20,0
FUNC_END(sysTimeBaseLPut)

/******************************************************************************
*
* sysTimeBaseLGet - Get lower half of Time Base Register
*
* This routine will read the contents the lower half of the Time
* Base Register (TBL - TBR 268).
*
* From a C point of view, the routine is defined as follows:
*
* UINT32 sysTimeBaseLGet(void)
*
* RETURNS: value of TBR 268 (in r3)
*/

FUNC_BEGIN(sysTimeBaseLGet)
    mftb 3
    bclr 20,0
FUNC_END(sysTimeBaseLGet)

/******************************************************************************
*
* sysHid1Get - read from HID1 register SPR1009.
*
* This routine will read the contents the HID1 (SPR1009)
*
* From a C point of view, the routine is defined as follows:
*
* UINT sysHid1Get()
*
* RETURNS: value of SPR1009 (in r3)
*/

FUNC_BEGIN(sysHid1Get)
    sync
    mfspr r3,HID1
    sync
    bclr 20,0
FUNC_END(sysHid1Get)

/******************************************************************************
*
* sysGetDec - read from the Decrementer register SPR22.
*
* This routine will read the contents the decrementer (SPR22)
*
* From a C point of view, the routine is defined as follows:
*
* UINT sysGetDec()
*
* RETURNS: value of SPR22 (in r3)
*/

FUNC_BEGIN(sysGetDec)
    sync
    mfspr r3,DEC
    sync
    bclr 20,0
FUNC_END(sysGetDec)

/*******************************************************************************
*
* sysL3pmPut - write to the L3PM (Private Memory Control Register) of
* Apollo CPU
*
* This routine will write the contents of r3 to the L3PM
* register.
*
* From a C point of view,the routine is defined as follows:
*
* void sysL3pmPut
* (
* ULONG value to write
* )
*
* RETURNS: NA
*/

FUNC_BEGIN(sysL3pmPut)
    sync
    mtspr   L3PM,r3
    isync
    bclr    20,0            /* Return to caller */
FUNC_END(sysL3pmPut)

/*******************************************************************************
*
* sysL3pmGet - read from L3PM (Private Memory Control Register)
* of the Apollo CPU
*
* This routine will read the contents the L3PM register.
*
* From a C point of view,the routine is defined as follows:
*
* UINT sysL3pmGet()
*
* RETURNS: value of SPR983 (in r3)
*/

FUNC_BEGIN(sysL3pmGet)
    sync
    mfspr   r3,L3PM
    sync
    bclr    20,0            /* Return to caller */
FUNC_END(sysL3pmGet)

#ifdef INCLUDE_PCX7307_SIO
/*******************************************************************************
*
* sysSioRead - this function reads a register from the UIO chip
*
* SYNOPSIS
* \ss
* UINT8 sysSioRead
*    (
*    UINT8 * pSioIndexReg,      /@ pointer to SIO index register base addr @/
*    UINT8 sioRegOffset         /@ offset of register to read from @/
*    )
* \se
*
* In order to read data from the desired Super IO register, the index register
* must be written to with the offset of the of the register to be read.  The
* desired byte of data is then read from the data register.
*
* RETURNS: byte read from data register
*/

FUNC_BEGIN(sysSioRead)
    stb     r4,0(r3)        /* write index register with register offset */
    eieio
    sync
    lbz     r3,1(r3)        /* retrieve specified reg offset contents */
    eieio
    sync
    bclr    20,0            /* return to caller */
FUNC_END(sysSioRead)

/*******************************************************************************
*
* sysSioWrite - this function writes a register to the UIO chip
*
* In order to write data to the desired Super IO register, the index
* register must be written to with the offset of the register to be
* modified.  The desired byte of data can then be written via the data
* register.
*
* SYNOPSIS
* \ss
* void sysSioWrite
*    (
*    UINT8 * pSioIndexReg,      /@ pointer to SIO index register base addr @/
*    UINT8 sioRegOffset,        /@ offset of register to write to @/
*    UINT8 data                 /@ 8-bit data to be written @/
*    )
* \se
*
* RETURNS: N/A
*/

FUNC_BEGIN(sysSioWrite)
    stb     r4,0(r3)        /* write index register with register offset */
    eieio
    sync
    stb     r5,1(r3)        /* 1st write */
    eieio
    sync
    stb     r5,1(r3)        /* 2nd write */
    eieio
    sync
    bclr    20,0            /* return to caller */
FUNC_END(sysSioWrite)
#endif /* INCLUDE_PCX7307_SIO */
