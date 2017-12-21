/* usrMnd.c - initialization of MIPC Network Device */

/*
 * Copyright (c) 2009, 2010 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01f,28mar11,brk  added bootline backplne IP assignment
01e,23jun10,h_w  upgrade to MND 2.1
01d,25sep09,spw  Tx, Rx buffer configuration
01c,22sep09,spw  Input robustness
01b,03jun09,spw  Bug fixes
01a,06may09,spw  written
*/

#include <vxWorks.h>
#include <stdio.h>
#include <ioLib.h>
#include <ipProto.h>
#include <muxLib.h>
#include <multios_ipc/mipcnetIpc.h>	/* mipcnet private header */

/* defines */

#define MIPCNET_DRV_NAME	"mnd"

/* externals */

IMPORT END_OBJ * mipcnetLoad ();

/* locals */

LOCAL STATUS usrMndConfig(struct mipcnetUnitConfig*, int, int, int);

/*******************************************************************************
*
* usrMndInit - Initialization configlette for Multi-OS IPC Network Device (MND)
*
* This routine parses configuration details about the desired MIPC network to
* initialize.
*
* mtu is the desired maximum transmission unit across all mipcnet segments
*
* txBufs is the number of transmission buffers of the underlying MIPC control
* and data sockets.
*
* rxBufs is the number of receive buffers of the underlying MIPC control and
* data sockets.
*
* The configuration string has the following format:
*	"#unit=unitNum segment=segNum [dport=portNum] bus=busName #unit= ..."
* 	Where:
*       '#' is a separator between MND devices.
*       unit=unitNum is the MND device number, i.e. mnd0, mnd1, ... , mndn.
*       segment=segNum is the mipcnet segment on which to attach the MND unit.
*       dport=portNum is the port used to send data packets. If this parameter
*           is set to -1 or omitted, portNum is chosen automatically. Otherwise,
*           portNum must be unique for each unit on a given bus and must be
*           smaller than (MIPC_SM_PORTS - 1).
*       bus=busName is the MIPC bus on which the MND unit resides.
*
*	Example:
*		"#unit=0 segment=0 dport=24 bus=main"
*
* This is executed only once at BOOT time.
*
* RETURNS: OK on success or ERROR on failure.
*
* NOMANUAL
*/
STATUS usrMndInit
(
    int		mtu,		/* Size of the MND frame payload */
    int 	txBufs, 	/* Number of tx buffers for underlying MIPC sockets */
    int 	rxBufs, 	/* Number of rx buffers for underlying MIPC sockets */
    int 	ctrlPort,	/* port number of bus control sockets */
    char *	cfgStr		/* Configuration string for MNDs */
)
    {
    struct mipcnetUnitConfig cfg;
    char * start;
    char * end = cfgStr;
    void * pBus;

    if (!cfgStr)
        {
        printf("usrMndInit: Error - the MND configuration string is NULL\n");
        errnoSet (EINVAL);
        return ERROR;
        }

    while ((*end != '\0') && (*end != '#'))
        end++;

    while (*end != '\0')
        {
        end++;
        start = end;
        while ((*end != '\0') && (*end != '#'))
            end++;

        if (start != end)
            {
            if (mipcnetUnitParser (start, end, &cfg))
                {
                printf("usrMndInit: Error - parse the MND configuration string failed\n");
                errnoSet (EINVAL);
                return ERROR;
                }

            /* create and start the mipcnet bus */

            pBus = (void *) mipcnetBusCreate (cfg.busName,
                                              txBufs,
                                              rxBufs,
                                              ctrlPort);
            if (pBus == NULL)
                {
                printf("usrMndInit: Error initializing mipcnet bus [%s]\n",
                       cfg.busName);
                continue;
                }

            /* create and start the MND */

            if (OK != usrMndConfig (&cfg, mtu, txBufs, rxBufs))
                {
                printf("usrMndInit: Error configuring and starting mnd unit(%d) on mipcnet bus [%s]\n",
                       cfg.unit, cfg.busName);
                continue;
                }
            }
        }
    return OK;
    }

/*******************************************************************************
*
* usrMndConfig - Configure and initialize a mipcnet unit
*
* This routine attaches the mipcnet device to the MUX, sets the desired
* ethernet address and attempts to start operation of the unit on the
* specified mipcnet segment, port, and bus
*
* This is executed once per device
*
* RETURNS: OK on success or ERROR on failure.
*
* NOMANUAL
*/
LOCAL STATUS usrMndConfig
(
    struct mipcnetUnitConfig* cfg,	/* mnd configure related parameters */
    int mtu,						/* Size of the MND frame payload */
    int txBufs,						/* transmit buffers on the data socket */
    int rxBufs						/* receive buffers on the data socket */
)
    {
    void *pUnit;				/* cookie from mipcnetUnitCreate() */
    void *pCookie;				/* cookie from muxDevLoad() */
    char config[MND_IF_NAME_MAX_LEN];/* passed to mipcnetLoad */

    sprintf (config, "%s:%d:%d:%d:%d:%d", cfg->busName, cfg->segment,
             cfg->dataPort, mtu, txBufs, rxBufs);

    /* Attach to mux */

    pCookie = muxDevLoad (cfg->unit, mipcnetLoad, config, 0, &pUnit);
    if (pCookie == NULL)
        return (ERROR);

    if (muxDevStart(pCookie) == ERROR)
        return (ERROR);

#ifdef INCLUDE_END_POLLED_STATS
    endPollStatsInit (pCookie, END_POLL_STATS_ROUTINE_HOOK);
#endif /* INCLUDE_END_POLLED_STATS */

    /* Attach to stack */

    if (ipAttach(cfg->unit, MIPCNET_DRV_NAME) == ERROR)
        return (ERROR);

    mipcnetUnitStart (pUnit);

    return (OK);
    }

/*******************************************************************************
*
* usrMndBpIpSet - Configure mipcnet unit 0 IP address
*
* This routine sets the first mipcnet device to the
* ethernet address IP address specified in the VxWorks bootline
* as the backplane address
*
* RETURNS: OK on success or ERROR on failure.
*
* NOMANUAL
*/
#ifdef INCLUDE_MND_BACKPLANE_IP

LOCAL STATUS usrMndBpIpSet(void)
    {
    
    unsigned int bpmask = 0;
    if (sysBootParams.bad[0] == EOS)
    	{ 
    	return (ERROR);
    	}
    else
    	{
    	if (bootNetmaskExtract (sysBootParams.bad, (int*)&bpmask) <1)
    		bpmask = 0xffffff00;   /* set to a default class C */
    		printf("usrMndBpIpSet: set mask = 0x%x for %s\n",bpmask,sysBootParams.bad);
    	}
    	
    	if (bpmask == 0)		
    		printf("usrMndBpIpSet: mask = 0x%x no ip traffic allowed on %s\n",bpmask,sysBootParams.bad);

    	/* configuration with boot parameters */
    	usrNetBootConfig ("mnd", 0, sysBootParams.bad,
    	                 bpmask, sysBootParams.gad); 
    	return OK;
    }

#endif
