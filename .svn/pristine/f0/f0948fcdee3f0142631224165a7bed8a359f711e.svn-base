/* usrSecurity.c - system security initialization library */

/* Copyright 1998, 2004-2005, 2009-2010 Wind River Systems, Inc. 
 *
 * The right to copy, distribute, modify or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement.
 */

/*
modification history
--------------------
01i,24nov10,v_r  Only warn when default credentials could not be added (def
                 CQ:WIND00240920).
01h,30sep10,ggz  Add salt to login password.
01g,24sep10,v_r  Network servers now handling security on their own (followup of
		 CQ:WIND00190686).
01f,28jul10,v_r  Temporary bypass the SHELL_SECURE test until network servers
                 handle security on their own.
01e,06jul10,v_r  Added login security facility (req CQ:WIND00190686).
01d,08sep09,v_r  Now including loginLibP.h (part of def CQ:WIND00155318 fix).
01c,04aug05,vvv  fixed initialization (SPR #106294)
01b,26apr04,bpn  Fixed SPR#96149: use loginPrompt2().
01a,03jun98,ms   written
*/

/*
DESCRIPTION
Initialize system security.
*/

#include <private/loginLibP.h>
#include <errnoLib.h>
#include <string.h>

/******************************************************************************
*
* usrSecurity - initialize the system security
*/

void usrSecurity (void)
    {
    loginInit ();                           /* initialize login table */

    /*
     * Register the default system credentials. In case of an error, print a
     * message but do not abort, as it could disable security if enabled.
     *
     * Note that the current initialization sequence calls usrSecurity() before
     * usrToolsInit() which sets up the error table. This means that the user
     * will currently only get the errno number instead of the errno string,
     * despite the strerror() call. We keep it though in case this is improved
     * in the future.
     */

    if (loginUserAdd (LOGIN_USER_NAME, LOGIN_PASSWORD,
                      LOGIN_PASSWORD_SALT) == ERROR)
        {
        printErr ("usrSecurity: error adding default user '%s' to the "
                  "system (%s)!\n", LOGIN_USER_NAME, strerror (errnoGet ()));
        }

#if SYSFLG_NO_SECURITY_ENABLE == TRUE
    /* 
     * Only enable security if SYSFLG_NO_SECURITY is not present on the boot
     * line.
     */

    if ((sysFlags & SYSFLG_NO_SECURITY) != SYSFLG_NO_SECURITY)
	systemSecurityIsEnabled = TRUE;

#else /* SYSFLG_NO_SECURITY_ENABLE == TRUE */
    /* 
     * Always enable security given that INCLUDE_SECURITY is included without
     * SYSFLG_NO_SECURITY support.
     */ 

    systemSecurityIsEnabled = TRUE;

#endif /* SYSFLG_NO_SECURITY_ENABLE == FALSE */

#ifdef INCLUDE_SHELL
    /* 
     * INCLUDE_SECURITY can be used without the shell. When both are used and
     * SHELL_SECURE is set, console shell login is enabled.
     *
     * Note that even if network servers handle security on their own based on
     * systemSecurityIsEnabled, we need to call shellLoginInstall() as soon as
     * the shell is included. If we did so only when SHELL_SECURE is set, we
     * would lose the ability to request secure sessions with
     * shellGenericInit().
     * 
     * Note that SHELL_SECURE cannot be set without INCLUDE_SECURITY. We have a
     * warning within usrShell() to that end. It is currently hard to do better
     * given that it is not possible to force-include a component when a
     * parameter is defined in a project build (this can be done by means of
     * usrExtra for the BSP build though).
     */

    if (systemSecurityIsEnabled)
	shellLoginInstall ((SHELL_LOGIN_RTN) loginPrompt2, 0);
#endif /* INCLUDE_SHELL */
    }

