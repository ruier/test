/* sysUtil.c - Legacy X86 functions needed by eg. ataDrv.c */

/*
 * Copyright 2003 Wind River Systems, Inc.
 */

/*
modification history
--------------------
01a,09jun03,scm  written
*/

/*

DESCRIPTION

*/

/* includes */

/* defines */

/***********************************************************************
*
* sysInWordString - reads a string of words from an io address.
*
* This function reads a word string from a specified io address.
*
* RETURNS: N/A
*/

void sysInWordString
    (
    ULONG   ioAddr,  /* I/O address */
    UINT16 *pBufPtr, /* pointer to data buffer */
    int     nWords   /* number of 2 byte words to read */
    )
    {
    int loopCtr;

    for (loopCtr = 0; loopCtr < nWords; loopCtr++)
        *pBufPtr++ = (*(short *)ioAddr);
    }

/***********************************************************************
*
* sysInWordStringRev - byte reversed word read.
*
* This function reads a byte reversed word string from a specified io address.
*
* RETURNS: N/A
*/

void sysInWordStringRev
    (
    ULONG   ioAddr, /* I/O address */
    UINT16 *pBufPtr, /* pointer to data buffer */
    int     nWords  /* number of 2 byte words to read */
    )
    {
    int loopCtr;

    for (loopCtr = 0; loopCtr < nWords; loopCtr++)
        {
        *pBufPtr = *(UINT16*)ioAddr;
        *pBufPtr++ = ((*pBufPtr & 0xff) << 8) | ((*pBufPtr & 0xff00) >> 8);
        }
    }

/***********************************************************************
*
* sysOutWordString - writes a string of words to an io address.
*
* This function writes a word string from a specified io address.
*
* RETURNS: N/A
*/

void sysOutWordString
    (
    ULONG   ioAddr,  /* I/O address */
    UINT16 *pBufPtr, /* pointer to data buffer */
    int     nWords   /* number of 2 byte words to write */
    )
    {
    int loopCtr;

    for (loopCtr = 0; loopCtr < nWords; loopCtr++)
        *(short*)ioAddr = *pBufPtr++;
    }

/***********************************************************************
*
* sysInLongString - reads a string of longwords from an io address.
*
* RETURNS: N/A
*/

void sysInLongString
    (
    ULONG  ioAddr,  /* I/O address */
    ULONG *pBufPtr, /* pointer to data buffer */
    int    nLongs   /* number of 4 byte words to read */
    )
    {
    int loopCtr;

    for (loopCtr = 0; loopCtr < nLongs; loopCtr++)
        *pBufPtr++ = *(int *)ioAddr;
    }

/***********************************************************************
*
* sysOutLongString - writes a string of longwords to an io address.
*
* This function writes a longword string from a specified io address.
*
* RETURNS: N/A
*/

void sysOutLongString
    (
    ULONG  ioAddr,  /* I/O address */
    ULONG *pBufPtr, /* pointer to data buffer */
    int    nLongs   /* number of 4 byte words to write */
    )
    {
    int loopCtr;

    for (loopCtr = 0; loopCtr < nLongs; loopCtr++)
        *(int*)ioAddr = *pBufPtr++;
    }


/********************************************************************************
* sysLanIntEnable - enable 3c90x ethernet device interrupts
*
* This routine enables lan interrupts.  This may involve operations on
* interrupt control hardware.
*
* RETURNS: OK or ERROR for invalid arguments.
*/
STATUS sysLanIntEnable
    (
    int    level        /* level number */
    )
    {
    logMsg ("Enabling intlevel %d\n", level, 2, 3, 4, 5, 6);
    return i8259IntEnable (level);
    }

/********************************************************************************
* sysLanIntDisable - disable 3c90x ethernet device interrupts
*
* This routine disables lan interrupts.  This may involve operations on
* interrupt control hardware.
*
* RETURNS: OK or ERROR for invalid arguments.
*/
STATUS sysLanIntDisable
    (
    int    level        /* level number */
    )
    {
    return i8259IntDisable (level);
    }
