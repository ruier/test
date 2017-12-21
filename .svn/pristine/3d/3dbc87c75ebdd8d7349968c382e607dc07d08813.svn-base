/* usrDebugkwrite.c - user provided output routine library */

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
01b,26mar09,s_s Fixed a typo - WIND00160968. Updated the documentation.
01a,17mar09,s_s written 
*/

/*
DESCRIPTION
This file contains the output routine provided by the user for kprintf() and 
kputs() functions.  

SEE ALSO: kwriteSioLib, kprintfLib, kputsLib
*/

/* includes */

#include <vxWorks.h>
#include <private/kwriteLibP.h>

/* global */


/* defines */


/* locals */

/**************************************************************************
*
* usrKwriteOut - writes a string to the user defined output device
*
* This routine should be provided by the user if they want to write the 
* kprintf() and kputs() messages to the output device other than the 
* serial device. The user can provide this routine as part of the project 
* in usrAppInit.c or include it as a separate file. The routine should not
* use any kind of interrupts to output the messages and should have the 
* format as mentioned below.
*
* This routine outputs a string to the output device defined by the user.
* This routine takes buffer and its length as input. 
*
* SYNOPSIS
* \ss
* LOCAL STATUS usrKwriteOut
*     (
*     char * buffer,
*     size_t len
*     )
*     {
*
*     } 
* \se
*
* RETURNS: OK, or ERROR for failure
*
* SEE ALSO: kprintf
*
* \NOMANUAL
*/
/*****************************************************************************
*
* usrKwriteInit - Initializes kprintf() and kputs() with the user provided 
* output routine.
*
* This routine assigns the function pointer _func_kwrite with the user 
* provided output routine. This makes kprintf() and kputs() output the message 
* to the user defined output device. The output routine provided by the user
* should be of the form
*
* LOCAL STATUS usrKwriteOut
*     (
*     char * buffer,
*     size_t len
*     )
*     {
*
*     } 
*
*
* This routine should return OK for success and ERROR for failure. 
*
* RETURNS
*  N/A
*
*\NOMANUAL
*/

void usrKwriteInit 
    (
    	STATUS (*usrRtn) (char * pBuf, size_t len) /* user provided output routine */
    )
    {
    _func_kwrite = usrRtn;
    }
