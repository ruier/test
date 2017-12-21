/* sysBusPci.h - Platform-specific PCI support constants header file */

/* Copyright 2003 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,24apr03,scm  Created from ads826x BSP.
*/

#ifndef __INCsysBusPcih
#define __INCsysBusPcih

#ifdef __cplusplus
extern "C" {
#endif

#ifndef _ASMLANGUAGE

#if (defined(__STDC__) || defined(__cplusplus))

STATUS sysPciAutoconfigInclude
    (
    PCI_SYSTEM * pSys,      /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UINT devVend			/* deviceID/vendorID of device */
    );

UCHAR sysPciAutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,      /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UCHAR pin				/* contents of PCI int pin register */
    );

void sysPciAutoconfigPreEnumBridgeInit
    (
    PCI_SYSTEM * pSys,      /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UINT devVend			/* deviceID/vendorID of device */
    );

void sysPciAutoconfigPostEnumBridgeInit
    (
    PCI_SYSTEM * pSys,      /* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UINT devVend			/* deviceID/vendorID of device */
    );

#else	/* __STDC__ */

STATUS sysPciAutoconfigInclude ();
UCHAR sysPciAutoconfigIntrAssign ();
void sysPciAutoconfigPreEnumBridgeInit ();
void sysPciAutoconfigPostEnumBridgeInit  ();

#endif	/* __STDC__ */

#endif	/* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif /* __INCsysBusPcih */
