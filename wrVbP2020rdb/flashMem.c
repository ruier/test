/* flashMem.c - flash memory device driver */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,03mar09,rec  written.
*/

/*
DESCRIPTION
This library contains routines to read and write flash memory.  The read
routine is needed in order to read the MAC address from the board.  The write
routine is a stub that is provided to prevent link errors.  It doesn't
actually write to flash.  

*/

#include <drv/mem/flashDev.h>


/******************************************************************************
*
* sysFlashGet - get the contents of flash memory
*
* This routine copies the contents of flash memory into a specified
* string.  The string is terminated with an EOS.
*
* RETURNS: OK, or ERROR if access is outside the flash memory range.
*
* SEE ALSO: sysFlashSet()
*
* INTERNAL
* If multiple tasks are calling sysFlashSet() and sysFlashGet(),
* they should use a semaphore to ensure mutually exclusive access.
*/

STATUS sysFlashGet
    (
    char *  string,     /* where to copy flash memory      */
    int     strLen,     /* maximum number of bytes to copy */
    int     offset      /* byte offset into flash memory   */
    )
    {
    if ((offset < 0) || (strLen < 0) || ((offset + strLen) > FLASH_SIZE))
        return (ERROR);

    bcopyBytes ((char *) (FLASH_ADRS + offset), string, strLen);
    string [strLen] = EOS;

    return (OK);
    }

/******************************************************************************
*
* sysFlashSet - write to flash memory
*
* This is a stub routine and doesn't actuall write to memory.
*
* RETURNS: ERROR
*
*/

STATUS sysFlashSet
    (
    char *  string,     /* string to be copied into flash memory */
    int     strLen,     /* maximum number of bytes to copy       */
    int     offset      /* byte offset into flash memory         */
    )
    {
    return (ERROR);
    }
