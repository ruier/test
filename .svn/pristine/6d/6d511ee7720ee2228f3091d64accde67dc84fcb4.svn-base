/* usrMsd.c - initialization of MSD */

/*
 * Copyright (c) 2008-2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01l,23sep10,jlk  WIND00234231 - Cast error truncates ioctl() calls for 64 bit
01k,06apr10,als  Initial changes in support of 64-bit memory addressing;
                 fix resource leaks; enhance configuration error checking
01j,18nov09,als  Revise MSD single threading to work with Linux (WIND00190478)
01i,05nov09,ebh  Add socket locks to single thread access to MIPC
01h,31aug09,jlk  WIND00179592 - Fence post issue when autoconfiguring with 128
		 nodes.
01g,27aug09,jlk  WIND00178836 - Auto-configuration changes for nodes other than
		 0.
01f,28jul09,ebh  Fix check of error return check from msdCreate()
01e,24oct08,jlk  WIND00141002 - Error when parsing bus name.
01d,18oct08,jlk  WIND00136113 - Configure multiple MSDs
01c,25aug08,jlk  Updated as per code review.
01b,08aug08,jlk  Updated as per HLD review.
01a,15jul08,jlk  written
*/

#include <vxWorks.h>
#include <stdio.h>
#include <ioLib.h>
#include <multios_ipc/msdLib.h>

/*
 * DESCRIPTION
 *
 * The configuration string has the following format:
 * "#dev=/ttyMsdx node=n instance=i bus=busName console=n #/dev=/ttyMsdx node=n instance=i ..."
 * Where:
 *  '#' is separator
 *  dev=/ttyMsdx is the name of the MSD to configure
 *  node= is the remote node to configure
 *  instance=i is the remote instance to configure
 *  bus=busName is the bus on which the MSD resides.
 * The configuration string is broken up into groups delimited by the '#'
 * character. It is mandatory that the string begin with this character. The
 * items within the group describe the configuration of an MSD.
 * The 'dev=' argument is used to specify which MSD to configure. The 'node=' is
 * used to specify on which node the remote endpoint resides. The 'instance='
 * argument is used to specify which particular MSD on the specified remote
 * node to connect to. The 'bus=' argument is used to specify the name of the
 * bus on which the remote endpoint and the MSD being configured reside. In
 * addition the 'console=' parameter can be used to specify if the MSD (not the
 * remote one) is to be used as the console. Valid options for this one are:
 * 'y', 'Y', 'yes' and 'YES' anything else will be interpreted as no console.
 * If more than one device is specified as the console only the first in the
 * string is configured as such. I.e. Other console arguments are ignored.
 *
 * There can be no space between the arguments name and the '=' sign. Also there
 * can not be any spaces after the '=' and the arguments value. I.e. the
 * following are invalid:
 *	"dev =/ttyMsd0"
 *	"dev= /ttyMsd0"
 *	"dev = /ttyMsd0"
 * There is a special argument, "auto=" that be used to auto-configure the MSDs
 * on a node. The syntax of this argument follows the same rules as the
 * console argument. The auto-configure option is to allow the configuration of
 * multiple MSDs without specifying each individually in the configuration
 * string. The formula is as follows:
 * For node 0:
 * An MSD is created from /ttyMsd0 to /ttyMsdx. Where x is project parameter
 * MSD_NUM_DEVS minus 1. For example if MSD_NUM_DEVS is set to 4, MSDs /ttyMsd0
 * to /ttyMsd2 are created. They are configured as follows: Each MSD's remote
 * instance is set to 0. Each MSD's remote node is set to 1 plus the local MSD's
 * instance number. In the previous example, /ttyMsd0 would be connected to
 * node 1, /ttyMsd1 would be connect to node 2 etc. The bus name is set for all
 * the MSDs to whatever is supplied in with the 'bus=' argument. The console
 * argument, if present, is ignored and no attempt is made to set any of the
 * configured MSDs to be the console.
 * For other nodes:
 * A single MSD is created, /ttyMsd0. It's remote instance number is set to one
 * less than local node number. The remote node number is set to 0. The console
 * is set to this MSD reguardless of the console= argument.
 * The auto-configuration arguments must reside completely in their own group.
 * It is possible though to have other groups with the auto-configure group in
 * the string but they are ignored.
 * An example auto-configuration string:
 *	"auto=y bus=main"
 */

#define USR_MSD_SEP '#'
#define USR_MSD_AUTO_ARG "auto="
#define USR_MSD_AUTO_ARG_SZ 5
#define USR_MSD_DEV_ARG "dev="
#define USR_MSD_DEV_ARG_SZ 4
#define USR_MSD_BUS_ARG "bus="
#define USR_MSD_BUS_ARG_SZ 4
#define USR_MSD_NODE_ARG "node="
#define USR_MSD_NODE_ARG_SZ 5
#define USR_MSD_INST_ARG "instance="
#define USR_MSD_INST_ARG_SZ 9
#define USR_MSD_CON_ARG "console="
#define USR_MSD_CON_ARG_SZ 8

#define USR_MSD_NAME_LEN 12

static BOOL consoleSet = FALSE;

LOCAL STATUS usrMsdCreate(int, int);
LOCAL STATUS usrMsdAutoCfg(char *, int, int);
LOCAL char * usrMsdEndpGet(char *, char *, struct msd_endpoint_set *, BOOL *,
    BOOL *);

STATUS usrMsdInit
    (
    int       txBufs,   /* Number of MIPC tx buffers. Must be greater than 0 */
    int       txPrio,   /* Priority of the transmit task */
    int       txOpts,   /* Options for the transmit task. Typically 0 */
    int       mtu,      /* Size of the MSD payload. Must be greater than 0 */
    int       ttyBufSz, /* Size of the TTY rx and tx buffers */
    int       numDevs,  /* Number of MSD devices to create */
    char *    cfgStr    /* Configuration string for MSDs */
    )
    {
    int fd;
    int i;
    struct msd_endpoint_set endp;
    char *cur;
    BOOL console = FALSE;
    BOOL automatic = FALSE;
    char name[USR_MSD_NAME_LEN];

    if (!cfgStr)
	{
	errnoSet (EINVAL);
	return ERROR;
	}

    if (numDevs > MSD_MAX_DEVS)
	return ERROR;

    if (numDevs > MSD_NUM_DEVS)	/* verify our locking array is big enough */
	return ERROR;

    if (ttyBufSz < mtu)
	ttyBufSz = mtu;

    msdLibInit(txBufs, txPrio, txOpts, mtu);

    /* Create the MSDs */
    for (i = 0; i < numDevs; i++)
	{
	if (usrMsdCreate (i, ttyBufSz) != OK)
	    break;
	}

    if (i == 0)
	/* Must be able to create at least one MSD */
	return ERROR;

    cur = cfgStr;
    while ( cur[0] != EOS )
	{
	cur = usrMsdEndpGet (cur, name, &endp, &console, &automatic);
	if (cur == NULL)
	    return ERROR;

	if (automatic)
	    {
	    return usrMsdAutoCfg (cur, ttyBufSz, numDevs);
	    }

	fd = open(name, O_RDWR, 0);
	if (fd < 0)
	    continue;

	ioctl (fd, MSD_SET_ENDPOINT, (_Vx_ioctl_arg_t)&endp);

	if (console && !consoleSet)
	    {
	    ioctl (fd, FIOSETOPTIONS, OPT_TERMINAL);
	    if (consoleFd >= 0)
		close (consoleFd);
	    consoleFd = fd;
	    ioGlobalStdSet (STD_IN,  fd);
	    ioGlobalStdSet (STD_OUT, fd);
	    ioGlobalStdSet (STD_ERR, fd);
	    consoleSet = TRUE;
	    }
	 else
	    close (fd);

	}

    return OK;
    }


LOCAL char *usrMsdArgGet
    (
    char *s1,	/* string to search */
    char *arg,	/* arguement to search for */
    int argLen,	/* arguement length */
    int *n       /* length of the value of the argument */
    )
    {
    char *s2;
    char prevChar;

again:
    /* Look for device name */
    s2 = strstr (s1, arg);
    if (s2 == NULL)
	return s2;

    prevChar = *(s2-1);
    s2 += argLen; /* Skip the arguement string */
    if ((prevChar != '#') && (prevChar != ',') && (prevChar != ' '))
	{
	s1 = s2;
        /* Partial match */
	goto again;
	}

    /* Find the end of the argument */
    *n = strcspn ( s2, " ,#");

    return s2;
    }


LOCAL STATUS usrMsdCreate
    (
    int  inst,  /* Local instance */
    int  sz	/* tty buffer size */
    )
    {
    SIO_CHAN *sio;
    char name[USR_MSD_NAME_LEN];

    sprintf (name, "/ttyMsd%d", inst);

    sio = msdCreate ();
    if (sio == (SIO_CHAN *)ERROR)
	return ERROR;
    if (ttyDevCreate (name, sio, sz, sz) != OK)
	{
	msdDestroy (sio);
	return ERROR;
	}

    return OK;
    }


LOCAL STATUS usrMsdAutoCfg
    (
    char *s1,
    int ttyBufSz,
    int maxNodes
    )
    {
    int i;
    int n;
    int fd;
    char name [USR_MSD_NAME_LEN];
    char *s2;
    struct msd_endpoint_set endp;
    int node;
    int bus;

    if ((maxNodes < 1) || (maxNodes > 128))
	return ERROR;

    s2 = usrMsdArgGet (s1, USR_MSD_BUS_ARG, USR_MSD_BUS_ARG_SZ, &n);
    if (s2 == NULL)
	return ERROR;

    if (n > (MSD_BUS_NAME_LEN - 1))
	n = MSD_BUS_NAME_LEN - 1;
    /* Copy bus name out of string. */
    memset (endp.bus_name, EOS, MSD_BUS_NAME_LEN);
    memcpy (endp.bus_name, s2, n);

    if (mipc_getbusbyname(endp.bus_name, &bus) != MIPC_OK)
	{
	/* bus name is invalid */
	return ERROR;
	}

    if ((node = mipc_getnodebybus(bus)) < 0)
	{
	/* bus name is invalid */
	return ERROR;
	}

    if (node != 0)
	{
	/*
	 * Special case. Configure the node to point to node 0 with an instance
	 * number one less than our own node number. Also assume that the MSD
	 * will be the console.
	 */
	fd = open ("/ttyMsd0", O_RDWR, 0);
	if (fd < 0)
	    {
	    /* This needs to work to continue */
	    return ERROR;
	    }

	endp.instance = node - 1;
	endp.node = 0;

	ioctl (fd, MSD_SET_ENDPOINT, (_Vx_ioctl_arg_t)&endp);

	ioctl (fd, FIOSETOPTIONS, OPT_TERMINAL);
	if (consoleFd >= 0)
	    close (consoleFd);
	consoleFd = fd;
	ioGlobalStdSet (STD_IN,  fd);
	ioGlobalStdSet (STD_OUT, fd);
	ioGlobalStdSet (STD_ERR, fd);
	consoleSet = TRUE;

	return OK;
	}

    /* We are node 0 and are connecting to nodes 1 to maxNodes */
    endp.instance = 0;
    for (i = 0; i < maxNodes; i++)
	{
	sprintf (name, "/ttyMsd%d", i);
	fd = open (name, O_RDWR, 0);

	if (fd < 0)
	    break;

	endp.node = i + 1;

	ioctl (fd, MSD_SET_ENDPOINT, (_Vx_ioctl_arg_t)&endp);

   	close (fd);
	}

    return OK;
    }

LOCAL char * usrMsdEndpGet
    (
    char *str,
    char *name,
    struct msd_endpoint_set *endp,
    BOOL *console,
    BOOL *automatic
    )
    {
    int n;
    char *s1,*s2;
    char tmp[6]; /* Use to store node number as a string. Currently limited to 65535. Hence 6 bytes in size */
    unsigned long node, inst;
    int bn;

    /* Look for separator */
    s1 = strchr (str, USR_MSD_SEP);
    if (s1 == NULL)
	return s1;

    s1++;

    /* Process auto argument */
    s2 = usrMsdArgGet (s1, USR_MSD_AUTO_ARG, USR_MSD_AUTO_ARG_SZ, &n);
    if (s2 == NULL)
	goto skipAuto;

    if (n == 1)
	{
	if ((*s2 == 'y') || (*s2 == 'Y'))
	    *automatic = TRUE;
	}
    else if (n == 3)
	{
	if ((strncmp ("yes", s2, 3) == 0) || (strncmp ("YES", s2, 3) == 0))
	    *automatic = TRUE;
	}

    if (*automatic)
	return s2 + n;

skipAuto:
    /* Process device argument */
    s2 = usrMsdArgGet (s1, USR_MSD_DEV_ARG, USR_MSD_DEV_ARG_SZ, &n);
    if (s2 == NULL)
	return s2;

    if (n > (USR_MSD_NAME_LEN -1))
	return NULL;

    memcpy (name, s2, n);
    name[n] = EOS;

    /* Process bus argument */
    s2 = usrMsdArgGet (s1, USR_MSD_BUS_ARG, USR_MSD_BUS_ARG_SZ, &n);
    if (s2 == NULL)
	return s2;

    if (n > (MSD_BUS_NAME_LEN - 1))
	return NULL;

    memcpy (endp->bus_name, s2, n);
    endp->bus_name[n] = EOS;

    if (mipc_getbusbyname (endp->bus_name, &bn))
	return NULL;

    /* Process node argument */
    s2 = usrMsdArgGet (s1, USR_MSD_NODE_ARG, USR_MSD_NODE_ARG_SZ, &n);
    if (s2 == NULL)
        return s2;

    if (n > 4)
        return NULL;

    memcpy (tmp, s2, n);
    tmp[n] = EOS;

    node = strtoul (tmp, NULL, 0);
    if (node >= mipc_getbusmaxnodes(bn))
	return NULL;

    endp->node = node;

    /* Process instance argument */
    s2 = usrMsdArgGet (s1, USR_MSD_INST_ARG, USR_MSD_INST_ARG_SZ, &n);
    if (s2 == NULL)
	return s2;

    if (n > 3)
	return NULL;

    memcpy (tmp, s2, n);
    tmp[n] = EOS;

    inst = strtoul (tmp, NULL, 0);
    if (inst >= 128)
	return NULL;

    endp->instance = inst;

    /* Process console argument */
    s2 = usrMsdArgGet (s1, USR_MSD_CON_ARG, USR_MSD_CON_ARG_SZ, &n);
    if (s2 == NULL)
	return s2;

    if (n == 1)
	{
	if ((*s2 == 'y') || (*s2 == 'Y'))
	    *console = TRUE;
	}
    else if (n == 3)
	{
	if ((strncmp ("yes", s2, 3) == 0) || (strncmp ("YES", s2, 3) == 0))
	    *console = TRUE;
	}

    return s1;
    }

