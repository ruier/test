/* usrVrfs.c - VRFS initialization */

/* 
 * Copyright (c) 2005 Wind River Systems, Inc. 
 * 
 * The right to copy, distribute, modify or otherwise make use 
 * of this software may be licensed only pursuant to the terms 
 * of an applicable Wind River license agreement. 
 */

/*
modification history
--------------------
01a,21may05,rfr   Initial Version.
*/

/*
DESCRIPTION
This file contains the initialization code for the Virtual Root File System.

NOMANUAL
*/


#ifndef __INCusrVrfs
#define __INCusrVrfs

/* includes */

#include <vrfsLib.h>

STATUS vrfsInit(void);
STATUS vrfsDevCreate(void);

STATUS usrVrfsInit
    (

    )

    {

    if (vrfsInit() != OK)
	return (ERROR);

    if (vrfsDevCreate() != OK)
	return (ERROR);

    return (OK);

    }


#endif
