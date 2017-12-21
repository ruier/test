/* usrWvFileUploadPath.c - configuration file for Windview File upload path */

/* Copyright 1997, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,10feb10,ctr  update for 64bit
01a,13may98,pr   written
*/

/*
DESCRIPTION
*/

#include <wvLib.h>

extern int wvArgHtons;

VOID usrWvFileUploadPathInit ()
    {

    _func_wvOnUploadPathCreate = (FUNCPTR) fileUploadPathCreate;
    _func_wvOnUploadPathClose = (FUNCPTR) fileUploadPathClose;
    wvArgHtons = 0;
    wvLogMsgString1 = "fname=";
    wvLogMsgString2 = "flag=";

    fileUploadPathLibInit ();
    }
