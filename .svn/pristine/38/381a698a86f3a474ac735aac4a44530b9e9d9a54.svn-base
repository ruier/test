/* sysBusPci.h - Platform-specific PCI support constants header file */

/* Copyright 1984-2005 Wind River Systems, Inc. */

/*
modification history
--------------------
01b,16aug05,dtr  Add support for dual PCI interfaces.
01a,04feb05,dtr    Created from ads8540 BSP.
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
    PCI_SYSTEM * pSys,			/* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UINT devVend			/* deviceID/vendorID of device */
    );

STATUS sysPci2AutoconfigInclude 
    (
    PCI_SYSTEM * pSys,			/* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UINT devVend			/* deviceID/vendorID of device */
    );

UCHAR sysPci2AutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,			/* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UCHAR pin				/* contents of PCI int pin register */
    );

UCHAR sysPciAutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,			/* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UCHAR pin				/* contents of PCI int pin register */
    );

UCHAR sysPci2AutoconfigIntrAssign
    (
    PCI_SYSTEM * pSys,			/* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UCHAR pin				/* contents of PCI int pin register */
    );

void sysPciAutoconfigPreEnumBridgeInit
    (
    PCI_SYSTEM * pSys,			/* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UINT devVend			/* deviceID/vendorID of device */
    );

void sysPciAutoconfigPostEnumBridgeInit
    (
    PCI_SYSTEM * pSys,			/* PCI_SYSTEM structure pointer */
    PCI_LOC * pLoc,			/* pointer to function in question */
    UINT devVend			/* deviceID/vendorID of device */
    );


#else	/* __STDC__ */

STATUS sysPciAutoconfigInclude ();
STATUS sysPci2AutoconfigInclude ();
UCHAR sysPciAutoconfigIntrAssign ();
UCHAR  sysPci2AutoconfigIntrAssign ();
void sysPciAutoconfigPreEnumBridgeInit ();
void sysPciAutoconfigPostEnumBridgeInit  ();

#endif	/* __STDC__ */

#endif	/* _ASMLANGUAGE */

#ifdef __cplusplus
}
#endif

#endif /* __INCsysBusPcih */
