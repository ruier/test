/* usrSpe.c - spe support library */

/* Copyright 2002 - 2004 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
modification history
--------------------
01e,26oct04,bpn  Removed shell references.
01d,12feb04,mil  Added support for PAL coproc.
01c,06feb04,mil  Adapted to the new shell API.
01b,09sep03,dtr  Adding in hook SPE save restore routines.
*/

/*
DESCRIPTION

This file is used to intialize the SPE support. This file is included by
usrConfig.c.

NOMANUAL
*/

#include "speLib.h"

IMPORT char	__speSaveRest_o;

char * __Spe_runtime [] =
    {
    &__speSaveRest_o
    };

void usrSpeInit (void)
    {
#if !defined (_WRS_PAL_COPROC_LIB)

    speInit();

#ifdef  INCLUDE_SHOW_ROUTINES
    speShowInit ();                     /* install dsp show routine */
#endif  /* INCLUDE_SHOW_ROUTINES */

#endif	/* _WRS_PAL_COPROC_LIB */

    }
