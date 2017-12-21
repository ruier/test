/* usrDsiSocket.c - DSI sockets init */

/* Copyright 2004-2005, 2009 Wind River Systems, Inc. */

/*
modification history
--------------------
01d,01jun09,dlk  LP64 support & POSIXification of socket APIs
01c,04jan05,bwa  removed 'Net' part of routine names (SPR#103232); doc cleanup.
01b,11jun04,ymz  removed compiler warning.
01a,17apr04,bwa  written.
*/

struct sockFunc;

extern STATUS sockLibAdd (struct sockFunc * (*sockLibInitRtn)(void), int domainMap,
                          int domainReal);
extern struct sockFunc * dsiSockLibInit (void);

/*******************************************************************************
*
* usrDsiSocket.c - DSI socket configuration
*
* This routine adds DSI socket backend for AF_LOCAL.
*
* RETURNS: N/A
*
* NOMANUAL
*/

STATUS usrDsiSocket (void)
    {
#ifdef INCLUDE_UN
    if (sockLibAdd (dsiSockLibInit, AF_LOCAL, AF_LOCAL) == ERROR)
        {
        if (_func_printErr)
            (*_func_printErr) ("usrDsiSocket: failed sockLibAdd for AF_LOCAL\n");
        return (ERROR);
        }
#endif  /* INCLUDE_UN */
    
    return (OK);
    }

