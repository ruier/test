/* wdbEnd.c - WDB END communication initialization library */

/*
 * Copyright 1998-2005, 2008, 2010, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01l,14jan13,wyt  Wrapped configNet.h in angle bracket.
01k,29jan10,dbt  Fixed MTU handling.
01j,07jul08,pgh  Add boot support for SATA.
01i,17jun08,dlk  Access endDevTbl only if INCLUDE_END is defined.
01h,15apr05,wap  Add WDB_END_DEVICE_ADDRESS (SPR #82098)
01g,09jun04,elg  Fix compilation warning.
01f,02sep03,elg  Reworked mechanism to select network interface (SPR #31947).
01e,20mar02,jhw  Remove network initialization calls. (SPR 73517)
01d,27feb02,j_s  return error if end device table is empty (SPR 73604).
01c,21nov01,jhw  Get WDB END device from sysBootParams (SPR 71595).
01b,30jul99,pul  add NPT support
01a,18may98,dbt   written based on usrWdb.c ver 02t
*/

/*
DESCRIPTION
Initializes the END network connection for the WDB agent.

NOMANUAL
*/

/* includes */

#include <configNet.h>

/* defines */

#if	WDB_MTU > WDB_END_PKT_MTU
#undef	WDB_MTU
#define	WDB_MTU	WDB_END_PKT_MTU
#endif	/* WDB_MTU > WDB_END_PKT_MTU */

char *wdbEndDeviceAddress = NULL;

extern void usrNetDevNameGet(char **, int *);

/******************************************************************************
*
* wdbCommDevInit - initialize the END connection
*
* This routine initializes the END connection used by the WDB agent.
*
* RETURNS :
* OK or ERROR if the END connection can not be initialized.
*
* NOMANUAL
*/

STATUS wdbCommDevInit
    (
    WDB_COMM_IF *	pCommIf,
    char ** 		ppWdbInBuf,
    char **		ppWdbOutBuf
    )
    {
    static WDB_END_PKT_DEV	wdbEndPktDev;	/* END packet device */
    END_OBJ *			pEnd = NULL;
    static uint_t		wdbInBuf [WDB_MTU/4];
    static uint_t		wdbOutBuf [WDB_MTU/4];
#ifdef	WDB_END_DEVICE_NAME
    char *			wdbEndDeviceName = WDB_END_DEVICE_NAME;
#endif	/* WDB_END_DEVICE_NAME */
    char *          pNetDev = NULL; /* network device name */
    int             netUnit = 0;    /* network device index */

    /* update input & output buffer pointers */

    *ppWdbInBuf = (char *) wdbInBuf;
    *ppWdbOutBuf = (char *) wdbOutBuf;

    /* update communication interface mtu */

    wdbCommMtu = WDB_MTU;

#ifdef  WDB_END_DEVICE_ADDRESS
    wdbEndDeviceAddress = WDB_END_DEVICE_ADDRESS;
#endif

#ifdef	WDB_END_DEVICE_NAME
    /*
     * First look if the user has specified a network adapter to use for
     * WDB connection.
     */

    if ((wdbEndDeviceName != NULL) && (wdbEndDeviceName [0] != EOS))
    	{
	/* Try to find this device */

    	pEnd = endFindByName (wdbEndDeviceName, WDB_END_DEVICE_UNIT);

    	if (pEnd == NULL)
	    {
	    if (_func_printErr != NULL)
	    	_func_printErr ("wdbCommDevInit: Could not find device "
			    	"%s, unit %d !\n",
			    	wdbEndDeviceName,
			    	WDB_END_DEVICE_UNIT, 0, 0, 0, 0);
	    }
	}
#endif	/* WDB_END_DEVICE_NAME */

    usrNetDevNameGet(&pNetDev, &netUnit);   /* get net device name and index */
    if ((pEnd == NULL) && (pNetDev != NULL) && (*pNetDev != EOS))
    	{
	/* No device is specified. Use the one initialized at boot time. */

	pEnd = endFindByName (pNetDev, netUnit);
	if (pEnd == NULL)
	    {
	    if (_func_printErr != NULL)
		_func_printErr ("wdbCommDevInit: Could not find device "
				"%s, unit %d !\n",
				pNetDev, netUnit,
				0, 0, 0, 0);
	    }
	}

#ifdef INCLUDE_END  /* Might have INCLUDE_END2 but not INCLUDE_END... */
    if (pEnd == NULL)
	{
	/*
	 * We have not found the boot device, try the first device in
	 * END device table
	 */

	char	devName [END_NAME_MAX];

	bzero (devName, END_NAME_MAX);
	if ((endDevTbl->endLoadFunc == NULL) ||
	    (endDevTbl->endLoadFunc(devName, NULL) != OK))
	    {
	    if (_func_printErr != NULL)
	    	_func_printErr ("wdbCommDevInit: could not get device name\n",
				0, 0, 0, 0, 0, 0);
	    return ERROR;
	    }

	/* get the END OBJ by name and unit */

	pEnd = endFindByName (devName, endDevTbl->unit);

	if (pEnd == NULL)
	    {
	    if (_func_printErr != NULL)
	    	_func_printErr ("wdbCommDevInit: Could not find a device "
				"to use!\n", 0, 0, 0, 0, 0, 0);
	    return ERROR;
	    }
	}
#endif /* INCLUDE_END */

    if (pEnd != NULL)
        {
        if (wdbEndPktDevInit(&wdbEndPktDev, udpRcv,
                         (char *)pEnd->devObject.name,
                         pEnd->devObject.unit) == ERROR)
	    return (ERROR);


	/*
	 * If the driver has detected a MTU smaller than the one specified in
	 * VxWorks image configuration, use this one.
	 */

	wdbCommMtu = min (wdbEndPktDev.wdbDrvIf.mtu, wdbCommMtu);

        if (udpCommIfInit(pCommIf, &wdbEndPktDev.wdbDrvIf) == ERROR)
	    return (ERROR);

        return (OK);
        }
    
    return (ERROR);
    }
