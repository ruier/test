/* cplusgnuIos.c - VxWorks iostreams class library initialization */

/* Copyright 1995 Wind River Systems, Inc. */

/*
modification history
--------------------
03f,03jun03,sn   added gcc 3.x support
03e,06nov01,sn   use new prefixed names
03d,24jan01,sn   use wrapper symbols
03c,10oct00,sn   regenerated
03b,03jun98,sn   moved the iostdio files to cplusIosTest.c
                 unless INCLUDE_CPLUS_IOSTREAMS_FULL is defined only
		 pull in what's required by iostream.o
02b,05may98,sn   added the "iostdio" object files
                 fixed a typo in the modification history
02a,15jan98,sn   rewrote to force-link GNU iostreams
01c,22jul96,jmb  fix comment attached to final endif
01b,07jun95,srh  correct duplicate naming problem (SPR 5100)
01a,14jun95,srh  written.
*/

/*
DESCRIPTION
This file is used to include the iostreams classes in the 
VxWorks build. The routines are only included when this file is 
included by usrConfig.c.

NOMANUAL
*/

#ifndef __INCcplusIosc
#define __INCcplusIosc

extern char __gnu_cplusplus_std_libraryInit;

char * __cplusIosObjs [] =
{
    &__gnu_cplusplus_std_libraryInit,
    0
};
#endif /* __INCcplusIosc */
