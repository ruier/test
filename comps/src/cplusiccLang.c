/* cplusgnuLang.c - force-link of C++ compiler support  */

/* Copyright 2000 Wind River Systems, Inc. */

/*
modification history
--------------------
01e,03jun03,sn   added gcc 3.x support
01d,04apr03,dtr  Changes for GCC3.x.
01c,06nov01,sn   use new prefixed names
01b,24jan01,sn   use wrapper symbols
01a,11oct00,sn   wrote
*/

/*
DESCRIPTION
This file pulls in C++ compiler (libgcc) support.
 
NOMANUAL
*/

#ifndef __INCcplusLibgccC
#define __INCcplusLibgccC

extern char __gplusplus_intrinsicsInit;

char * __cplusLangObjs [] =
{
    &__gplusplus_intrinsicsInit,
    0
};
#endif /* __INCcplusLibgccC */
