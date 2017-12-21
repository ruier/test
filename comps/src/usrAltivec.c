/* usrAltivec.c - Altivec support library */

/* Copyright 1984-2009 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
modification history
--------------------
01d,25sep09,pmr  add Altivec non-java option
01c,26oct04,bpn  Removed shell references.
01b,17jul03,bpn  Adapted to the new shell API.
01a,10may01,dtr  Created
*/

/*
DESCRIPTION

This file is used to intialize the Altivec support. This file is included by
usrConfig.c.

NOMANUAL
*/

#include "altivecLib.h"

void usrAltivecInit (void)
    {
    altivecInit();

#ifdef ALTIVEC_NON_JAVA_MODE
    altivecSetNonJava();
#endif

#ifdef  INCLUDE_SHOW_ROUTINES
    altivecShowInit ();                     /* install altivec show routine */
#endif  /* INCLUDE_SHOW_ROUTINES */
    }

