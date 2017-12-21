/* hostSio.c - host serial device driver (VxSim) */

/*
 * Copyright (c) 2004,2007-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use of this
 * software may be licensed only pursuant to the terms of an applicable
 * Wind River license agreement.
 */

/*
modification history
--------------------
01f,27may08,elp  fixed gnu compiler warnings.
01e,12jun07,elp  fixed missing brace
01d,28mar07,elp  fixed typo.
01c,27feb07,elp  moved to vxsim host call usage.
01b,09apr04,jeg  updated documentation.
01a,22mar04,jeg  written
*/

/*
DESCRIPTION

This library provides a driver to access a host serial device in asynchronous
mode. 

USAGE

The driver is typically called only by the BSP. The directly callable routines
in this module are:

.CS
	hostSioDevInit()
	hostSioDevInit2()
.CE

The BSP's sysHwInit() routine typically calls sysSerialHwInit(), which calls
hostSioDevInit() if the host serial support is selected (INCLUDE_HOST_SIO macro
defined into BSP configuration). Then, hostSioDevInit() loads the host specific
dynamic library located under :

.CS
$(WIND_BASE)/host/$(WIND_HOST_TYPE)/lib/vxsim/serial
.CE

Then, It checks that all the host APIs required for this driver are available
and fills the HOST_SIO_CHAN channel structure. At this step, only polled mode
is available.

The BSP's sysHwInit2() routine typically calls sysSerialHwInit2(), which calls
hostSioDevInit2() if the host serial support is selected. hostSioDevInit2() API
enables interrupt mode. But interrupts will be connected only when the device
will be opened.

Once the hostSio driver initialized, the "/tyCo/1" vxWorks device is ready for 
use.

MODES

This driver supports both polled and interrupted modes. It can switch modes
dynamically by using the SIO_MODE_SET control command. Polled mode is provided
solely for WDB system mode usage. 

BAUD RATE
Once the device is opened, the baud rate can be changed by using the 
SIO_BAUD_SET control command. The possible baud rate values for this driver 
are :
.CS
50, 75, 110, 134, 150, 300, 600, 1200, 1800, 2400, 4800, 9600, 19200, 38400.
.CE

EXAMPLE

The following exemple send the string "Hello world!" by the host serial device
using 19200 baud rate speed.

.CS

#include <fcntl.h>
#include <ioLib.h>
#include <string.h>
#include <sioLib.h>

void serialHelloWorld ()
    {
    /@ open the host serial device @/

    int hostSerialFd = open ("/tyCo/1", O_RDWR, 0); 

    if (hostSerialFd > 0 )
    	{
	/@ set 19200 as baud rate @/

	ioctl (hostSerialFd, SIO_BAUD_SET, 19200);

	/@ send the string @/

	write (hostSerialFd, "Hello world!", strlen ("Hello world!"));

	close (hostSerialFd);
    	}
    }
.CE

RESTRICTION
Only baud rate device parameter can be configured. The other device parameters 
are :
.CS
	8 data bits
	no parity
	1 stop bit
	no hardware or software flow control
.CE

Only one host serial device can be used.

Only port 0 or 1 can be selected (HOST_SIO_PORT_NUMBER macro defined in BSP
configuration).

INCLUDES:
sioLib.h ttyLib.h 

SEE ALSO: sioLib ttyLib
*/

/* includes */

#include <vxWorks.h>
#include <stdio.h>
#include <sioLib.h>
#include <intLib.h>
#include <errnoLib.h>
#include <errno.h>
#include <ttyLib.h>
#include <iv.h>
#include <string.h>
#include <stdlib.h>
#include <config.h>
#include <vxsimHostLib.h>


/* defines */

#define HSIO_RCBUF_SIZE	2048		/* input buffer size		*/
#define HSIO_SNBUF_SIZE	2048		/* output buffer size		*/
#define ROUT_NAME_MAX	20		/* host routine name max length */
#define DEF_BAUD_RATE	9600		/* default baud rate		*/

#if (CPU == SIMNT) 
#define SERIAL_DLL 	"serial/commSio.dll" /* Windows VxSim host DLL	*/
#define ROUT_PREF	"commSio"	/* Windows VxSim routine prefix	*/

#define GET_VEC(pChan)\
	(USER_INT_RANGE_BASE + pChan->port) /* on Windows VxSim, the 	*/
					/* interrupt vector is defined	*/
					/* statically. For more details */
	 				/* refer to config.h		*/
	
#else
#define SERIAL_DLL 	"serial/ttySio.so" /* UNIX VxSim host DLL	*/
#define ROUT_PREF	"ttySio"	/* UNIX VxSim routine prefix    */
#define GET_VEC(pChan)\
	FD_TO_IVEC(pChan->hostFd)	/* on UNIX VXSIM, the interrupt */
					/* vector is computed regarding	*/
					/* the file descriptor number.	*/
#endif /* (CPU == SIMNT) */


/* typedefs */

typedef struct HOST_SIO_CHAN	/* host serial channel 		*/
    {
    SIO_CHAN 	sio;            /* standard SIO_CHAN element 	*/

    /* callbacks */

    STATUS      (*getTxChar) ();  /* installed Tx callback routine   */
    STATUS      (*putRcvChar) (); /* installed Rx callback routine   */
    void *      getTxArg;	  /* argument to Tx callback routine */ 
    void *      putRcvArg;	  /* argument to Rx callback routine */

    /* host info */

    UINT32	hostFd;		/* host file descriptor */
    UINT32	port;		/* port number = 0 or 1 */

    /* UART settings */

    UINT32	baudRate;       /* the current baud rate 	*/
    UINT32	mode;		/* interrupt or polled		*/
    BOOL        created;	/* TRUE when channel created	*/
    } HOST_SIO_CHAN;

/* locals */

/* pointers to host routines loaded from the host specific dynamic library */

LOCAL	FUNCPTR	pHostLibInit		= NULL;	
LOCAL	FUNCPTR pHostLibOpen		= NULL;
LOCAL	FUNCPTR pHostLibIntSet		= NULL;
LOCAL	FUNCPTR pHostLibIntClear	= NULL;
LOCAL	FUNCPTR pHostLibClose		= NULL;
LOCAL	FUNCPTR pHostBaudRateSet	= NULL;
LOCAL	FUNCPTR pHostLibDataGet		= NULL;
LOCAL	FUNCPTR pHostLibDataSend	= NULL;

/* function prototypes */

LOCAL STATUS 	hostSioDummyCallback 	(void);
LOCAL int	hostSioIoctl 		(SIO_CHAN * pSioChan, int request, 
					 int arg);
LOCAL int	hostSioTxStartup 	(SIO_CHAN * pSioChan);
LOCAL int	hostSioCallbackInstall 	(SIO_CHAN * pSioChan, int callbackType,
                               		 STATUS (*callback)(), 
					 void * callbackArg);
LOCAL int	hostSioPollInput 	(SIO_CHAN * pSioChan, char *);
LOCAL int	hostSioPollOutput 	(SIO_CHAN * pSioChan, char);
LOCAL STATUS 	hostSioIntSet 		(HOST_SIO_CHAN * pSioChan);
LOCAL void 	hostSioIntRx 		(HOST_SIO_CHAN * pSioChan);
LOCAL STATUS	hostSioBaudRateSet	(HOST_SIO_CHAN * pSioChan, 
					 UINT32 baudRate);


/* driver functions */

LOCAL SIO_DRV_FUNCS hostSioDrvFuncs =
    {
    (int (*)())hostSioIoctl,
    hostSioTxStartup,
    (FUNCPTR)hostSioCallbackInstall,
    hostSioPollInput,
    hostSioPollOutput
    };

/* globals */

HOST_SIO_CHAN hostSioChan;	/* host device channel */

/*******************************************************************************
*
* hostSioDevInit - create an hostSio device.
*
* This routine creates a new hostSio device. It first loads the host dynamic
* library, gets the address of the needed host APIs and calls the host
* initialization routine. Then, it initializes the hostSio channel structure.
* It is called from sysHwInit() with interrupts locked.
* Polled mode serial operations are possible, but not interrupt mode.
* Interrupt mode is enabled in sysSerialHwInit2().
*
* RETURNS: OK on success, else ERROR.
*
* ERRNO: N/A
*/

STATUS hostSioDevInit 
    (
    HOST_SIO_CHAN *	pChan,	/* host device channel	*/
    UINT32		port	/* port number 		*/
    )
    {
    static BOOL		libInitialized = FALSE;	   /* hostSioInit () called ? */
    char		hApiName [ROUT_NAME_MAX];  /* host APIs name	      */

    /* if already initialized, then return ERROR */

    if (libInitialized)
    	return (ERROR);

    /* load the DLL containing host specific routines */
 
    if (vxsimHostDllLoad (SERIAL_DLL) != OK)
    	{
    	return (ERROR);
	}

    /* get the addresses of the host routines loaded from the DLL */

    sprintf (hApiName, "%s%s", ROUT_PREF, "Init"); 
    pHostLibInit 	= vxsimHostProcAddrGet (hApiName);

    sprintf (hApiName, "%s%s", ROUT_PREF, "Open"); 
    pHostLibOpen	= vxsimHostProcAddrGet (hApiName);

    sprintf (hApiName, "%s%s", ROUT_PREF, "IntSet"); 
    pHostLibIntSet	= vxsimHostProcAddrGet (hApiName);

    sprintf (hApiName, "%s%s", ROUT_PREF, "IntClear"); 
    pHostLibIntClear	= vxsimHostProcAddrGet (hApiName);

    sprintf (hApiName, "%s%s", ROUT_PREF, "Close"); 
    pHostLibClose	= vxsimHostProcAddrGet (hApiName);

    sprintf (hApiName, "%s%s", ROUT_PREF, "BaudRateSet"); 
    pHostBaudRateSet	= vxsimHostProcAddrGet (hApiName);

    sprintf (hApiName, "%s%s", ROUT_PREF, "DataGet"); 
    pHostLibDataGet	= vxsimHostProcAddrGet (hApiName);

    sprintf (hApiName, "%s%s", ROUT_PREF, "DataSend"); 
    pHostLibDataSend	= vxsimHostProcAddrGet (hApiName);

    /* check that all host APIs are available */

    if ((pHostLibInit == NULL) || (pHostLibOpen == NULL) || 
    	(pHostLibIntSet == NULL) || (pHostLibIntClear == NULL) ||
	(pHostLibClose == NULL) || (pHostBaudRateSet == NULL) ||
	(pHostLibDataGet == NULL) || (pHostLibDataSend == NULL))
	{
	/* if at least one host API is not found, the drive could not run */

	return (ERROR);
	}

    /* call the host initialization routine */

    if (vxsimHostProcCall (pHostLibInit, port, 0,0,0,0,0,0,0,0) == ERROR) 
    	{
	/* host init routine failed */

    	return (ERROR);
	}

    if (pChan == NULL)
    	{
	/* not a valid channel */

    	return (ERROR);
	}

    /* fill the host serial device channel structure */

    pChan->created 	= FALSE;
    pChan->hostFd 	= 0;		    	/* devive not open yet	*/
    pChan->getTxChar 	= hostSioDummyCallback;
    pChan->putRcvChar 	= hostSioDummyCallback;
    pChan->sio.pDrvFuncs = &hostSioDrvFuncs;

    pChan->port 	= port;			/* save port number 	*/
    pChan->baudRate 	= DEF_BAUD_RATE;	/* save init baud rate 	*/	
    pChan->mode 	= SIO_MODE_INT;		/* polled mode 		*/

    /* 
     * the init process succeded, so the serial channel is marked as created 
     * and the library as initialized.
     */

    pChan->created = TRUE;
    libInitialized = TRUE;

    return (OK);
    }

/*******************************************************************************
*
* hostSioDevInit2 - set interrpt mode  
*
* This routine usually connects the serial device interrupts, and calls the
* second step driver initializations if any.  It is called from
* sysHwInit2().  Serial device interrupts could not be connected in
* sysSerialHwInit() because the kernel memory allocator was not initialized
* at that point, and intConnect() calls malloc().
*
* RETURNS: OK on success, else ERROR.
*
* ERRNO: N/A
*/

STATUS hostSioDevInit2 
    (
    HOST_SIO_CHAN *	pChan	/* host device channel	*/
    )
    {
    /* if the hostSioDevInit () call succeded, then set the interrupt mode */

    if ((pChan != NULL) && (pChan->created))
    	{
    	pChan->mode = SIO_MODE_INT;
	return (OK);
	}

    /* channel was not ready */

    return (ERROR);
    }

/*******************************************************************************
*
* hostSioDummyCallback - dummy callback routine.
*
* RETURNS: ERROR, always.
*
* ERRNO: N/A.
*/

LOCAL STATUS hostSioDummyCallback (void)
    {
    return ERROR;
    }

/*******************************************************************************
*
* hostSioOpen - open the serial channel.
* 
* this routine calls the host routine in order to open the host serial 
* device. The HOST_SIO_CHAN is then filled with the host file descriptor.
* The device is opened in non blocking mode. Once opened, the serial device
* is configured to use the default baud rate (DEF_BAUD_RATE macro). 
*
* RETURNS: OK on success, else ERROR.
*
* ERRNO: N/A.
*/

LOCAL STATUS hostSioOpen
    (
    HOST_SIO_CHAN *	pChan	/* host device channel */
    )
    {
    /* call the host specific API to open the serial device */

    if ((pChan->hostFd = vxsimHostProcCall (pHostLibOpen, 0,0,0,0,0,0,0,0,0))
    	== ERROR)
	{
	pChan->hostFd = 0;

	/* host open routine failed */

    	return (ERROR);
	}
     
    /* set default baud rate */

    hostSioBaudRateSet (pChan, pChan->baudRate);

    /* setup interrupted mode */

    if (hostSioIntSet (pChan) == ERROR)
    	{
 	/* interrupt mode setup failed */

	return (ERROR);
	}

    return (OK);
    }

/*******************************************************************************
*
* hostSioIntSet  - set interrupt mode on a serial device
*
* This routine configures a serial port to generates interrupts when data
* are ready. It also connects the vxWorks interrupts handler (hostSioIntRx ()) 
* which will be called each time a new data will be ready for input.
* RETURNS: OK on success, else ERROR.
*
* RETURNS: OK on success, else ERROR.
*
* ERRNO: N/A.
*/

LOCAL STATUS hostSioIntSet 
    (
    HOST_SIO_CHAN *	pChan	/* host serial device channel */
    )
    {
    /* check channel validity */

    if (pChan->hostFd == 0)
    	{
	/* not a valid channel */

	return (ERROR);
	}

    /* call the host API which set interrupt mode */

    if (vxsimHostProcCall (pHostLibIntSet, (UINT32)GET_VEC (pChan),
     			   0,0,0,0,0,0,0,0) == ERROR)
	{
	/* host API failed */

	return (ERROR);
	}

    /* 
     * connect vxWorks interrupt. hostSioIntRx () will be called each time
     * a data will be ready for input.
     */

    if (intConnect ((void *) GET_VEC (pChan), hostSioIntRx,
	(UINT32) pChan) == ERROR)
	{
	/* intConnect call failed */

	return (ERROR);
	}

    return (OK);
    }

/*******************************************************************************
*
* hostSioIntClear  - disable interrupt mode on a serial port
*
* This routine disable the interrupt mode on a serial port. 
*
* RETURNS: OK on success, else ERROR.
*
* ERRNO: N/A.
*/

LOCAL STATUS hostSioIntClear
    (
    HOST_SIO_CHAN *	pChan	/* host serial device channel */
    )
    {
    /* check channel validity */

    if (pChan->hostFd == 0)
    	{
	/* not a valid channel */

	return (ERROR);
	}

    /* call the host API */

    if (vxsimHostProcCall (pHostLibIntClear, 0,0,0,0,0,0,0,0,0) == ERROR)
    	{
	/* host API failed */

	return (ERROR);
	}

    return (OK);
    }

/*******************************************************************************
*
* hostSioClose - close the serial channel.
* 
* This routine calls the host routine in order to close the serial line
* device. The host routine will retore the original device control parameters.
*
* RETURNS: OK on success, else ERROR.
*
* ERRNO: N/A.
*/

LOCAL STATUS hostSioClose
    (
    HOST_SIO_CHAN *	pChan	/* host serial channel */
    )
    {
    /* check channel validity */

    if (pChan->hostFd == 0)
    	{
	/* not a valid channel */

	return (ERROR);
	}

    /* call the host API */

    if (vxsimHostProcCall (pHostLibClose, 0,0,0,0,0,0,0,0,0) == ERROR)
	{
	/* host API failed */

    	return (ERROR);
	}
    
    pChan->hostFd = 0;		/* set channel as closed */

    return (OK);
    }

/*******************************************************************************
*
* hostSioBaudRateSet - change the serial device baud rate
* 
* This routine changes the serial device baud rate. It is called by the driver
* ioctl routine with SIO_BAUD_SET IOCTL.
*
* RETURNS: OK if device baud rate changed, else ERROR.
*
* ERRNO: N/A.
*/

LOCAL STATUS hostSioBaudRateSet 
    (
    HOST_SIO_CHAN *	pChan, 		/* serial device channel */
    UINT32		baudRate	/* new baud rate to set	 */
    )
    {
    /* check channel validity */

    if (pChan->hostFd == 0)
    	{
	/* not a valid channel */

	return (ERROR);
	}

    /* call the host API which update the device baud rate */

    if (pHostBaudRateSet(baudRate) == ERROR)
	{
	/* host API call failed */

    	return (ERROR);
	}

    return (OK);
    }

/*******************************************************************************
*
* hostSioIoctl - host serial line device control
* 
* This routine handles the IOCTL messages from the user.
*
* RETURNS: OK on success, ENOSYS on unsupported request, EIO on failed
* request.
*
* ERRNO: N/A
*/

LOCAL int hostSioIoctl
    (
    SIO_CHAN *  pSioChan,       /* ptr to SIO_CHAN describing this channel */
    int         request,        /* request code 			   */
    int         arg             /* some argument 			   */
    )
    {
    UINT32		status= OK;	/* status to return		*/	
    HOST_SIO_CHAN *	pChan;		/* host serial line IO channel	*/

    pChan = (HOST_SIO_CHAN *) pSioChan;

    switch (request)
        {
	case SIO_BAUD_SET:	/* set the baud rate requested */

	    if (hostSioBaudRateSet (pChan, arg) == OK)
		{
		pChan->baudRate = arg;
		}
	    else
	   	{
		status = EIO;
		}
	    break;

	case SIO_BAUD_GET:	/* Get the baud rate */

	    *(int *)arg = pChan->baudRate;
	    break;
	    
	case SIO_MODE_SET:	/* Set the mode (interrupt or polled) */

	    if ((arg != SIO_MODE_POLL) && (arg != SIO_MODE_INT))
		{
		status = EIO;
		break;
		}

	    if (arg == SIO_MODE_INT)
	        {
		if (pChan->mode != SIO_MODE_INT)
		    {
		    if (hostSioIntSet (pChan) == ERROR)
			status = EIO;
		    }
		}
	    else
	    	{
		if (pChan->mode != SIO_MODE_POLL)
		    {
		    if (hostSioIntClear (pChan) == ERROR)
			status = EIO;
		    }
		}

	    pChan->mode = arg;
	    break;
	    
	case SIO_AVAIL_MODES_GET:	/* Get the available modes */

	    *(int *)arg = SIO_MODE_INT | SIO_MODE_POLL;
	    break;
	    
	case SIO_OPEN:			/* open the serial device */

	    if (hostSioOpen (pChan) == ERROR)
	    	{
	    	status = EIO;
		}
	    break;

	case SIO_HUP:			/* close the serial device */

	    if (hostSioClose (pChan) == ERROR)
	    	{
	    	status = EIO;
		}
	    break;

	case SIO_HW_OPTS_SET:
	case SIO_HW_OPTS_GET:

	    /*
	     * Optional command to get the hardware options (as defined
	     * in sioLib.h). It is not supported by this driver, so
	     * return ENOSYS.
	     */

	default :
	    status = ENOSYS;
	}

	return (status);
    }

/*****************************************************************************
*
* hostSioIntRx - handle a receiver interrupt 
*
* This routine handles read interrupts from the host serial device. It is
* called when a data is ready to be read when interrupt mode is selected.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

LOCAL void hostSioIntRx
    (
    HOST_SIO_CHAN *	pChan   	/* host serial device channel 	*/
    )
    {
    char 	buf [HSIO_RCBUF_SIZE];	/* buffer to store data read	*/
    int		nread;			/* number of data read		*/
    int 	ix;			/* loop counter			*/

    /* call the host API which read received data */

    nread = vxsimHostProcCall (pHostLibDataGet, (UINT32)buf, HSIO_RCBUF_SIZE,
    			       0,0,0,0,0,0,0);

    /* call the driver Rx callback routine */

    for (ix = 0; ix < nread; ix++)
	{
	(*pChan->putRcvChar) (pChan->putRcvArg, buf[ix]);
	}
    }

/*****************************************************************************
*
* hostSioTxStartup - transmitter startup routine
*
* Data output routine when interrupt mode is selected.
*
* RETURNS: OK on success, ENOSYS if the device is polled-only, or
* EIO on hardware error.
*
* ERRNO: N/A
*/

LOCAL int hostSioTxStartup
    (
    SIO_CHAN *	pSioChan	/* host serial device channel */
    )
    {
    char 	buf [HSIO_SNBUF_SIZE];	  /* data to write	     	*/
    UINT32 	ix;			  /* loop counter	     	*/

    HOST_SIO_CHAN * pChan = (HOST_SIO_CHAN *)pSioChan;

    /* check channel validity */

    if (pChan->hostFd == 0)
	{
	/* not a valid channel */

	return (ERROR);
	}

    if (pChan->mode == SIO_MODE_POLL)
    	{
	/* this routine should not be called if interrupt mode not selected */

        return (ENOSYS);
	}

    /* write until nothing more to send ... */

    do
	{
	/* check if characters need to be sent */

	for (ix = 0; ix < sizeof (buf); ix++)
	    {
	    if ((*pChan->getTxChar) (pChan->getTxArg, &buf[ix]) != OK)
		{
		break;
		}
	    }

	/* send data by the serial device */
	
	if (vxsimHostProcCall (pHostLibDataSend, (UINT32)buf, ix,
			       0,0,0,0,0,0,0) == ERROR)
	    {
	    /* host API failed */

	    return (EIO);
	    }
        }

    while (ix == sizeof (buf));

    return (OK);
    }

/*****************************************************************************
*
* hostSioPollOutput - output a character in polled mode.
*
* This routine send a character by the serial device if the polled mode is
* selected. This routine is used by WDB agent when in system mode. Interrupts
* are already locked.
*
* RETURNS: OK if a character arrived, EIO on device error, EAGAIN if the output
* buffer if full. ENOSYS if the device is interrupt-only.
*
* ERRNO: N/A.
*/

LOCAL int hostSioPollOutput
    (
    SIO_CHAN *	pSioChan,       /* host serial device channel	*/
    char        outChar         /* char to output 		*/
    )
    {
    UINT32		nbWrite = 0;	/* number of bytes written	*/
    HOST_SIO_CHAN * 	pChan;		/* current channel		*/

    pChan = (HOST_SIO_CHAN*)pSioChan;

    /* check channel validity */

    if (pChan->hostFd == 0)
    	{
	/* not a valid channel */

	return (EIO);
	}

    /* check the current mode validity */

    if (pChan->mode != SIO_MODE_POLL)
    	{
	/* this API should not be called if the polloed mode is not selected */

        return (ENOSYS);
	}

    /* 
     * call the host API which write data by the serial device. Interrupts
     * are already locked.
     */

    nbWrite = vxsimHostProcCall (pHostLibDataSend, (UINT32)&outChar, 1,
    				 0,0,0,0,0,0,0);

    /* compute returned value */

    if (nbWrite == -1)
    	{
        return (EIO);
	}
    else if (nbWrite == 0)
    	{
        return (EAGAIN);
	}

    return (OK);
    }

/*****************************************************************************
*
* hostSioPollInput - poll the device for input.
*
* This routine read a character by the serial device when the polled mode is
* selected. This routine is used by WDB agent when in system mode. Interrupts
* are already locked.
*
* RETURNS: OK if a character arrived, EIO on device error, ENOSYS if the 
* the device is interrupt-only, EAGAIN if nothing received.
*
* ERRNO: N/A
*/

LOCAL int hostSioPollInput
    (
    SIO_CHAN *  pSioChan,       /* host serial device channel	*/
    char *      pChar         	/* character read 		*/
    )
    {
    HOST_SIO_CHAN * 	pChan;		/* current channel	*/
    UINT32		nbDataRead;	/* number of data read	*/

    pChan = (HOST_SIO_CHAN*)pSioChan;

    /* check channel validity */

    if (pChan->hostFd == 0)
    	{
	/* not a valid channel */

	return (EIO);
	}

    /* check if the current mode validity */

    if (pChan->mode != SIO_MODE_POLL)
    	{
	/* this API should not be called if the polloed mode is not selected */

        return (ENOSYS);
	}

    /* 
     * call the host API which read data by the serial device. Interrupts
     * are already locked.
     */

    nbDataRead = vxsimHostProcCall (pHostLibDataGet, (UINT32)pChar, 1,
    				    0,0,0,0,0,0,0);

    /* compute returned value */

    if (nbDataRead == -1)
    	{ 
	return (EIO);
	}
    else if (nbDataRead == 0)
    	{
	return (EAGAIN);
	}

    return (OK);
    }

/*****************************************************************************
*
* hostSioCallbackInstall - install ISR callbacks to get/put chars.
*
* This routine installs the callback functions for the driver
*
* RETURNS: OK on success or ENOSYS on unsupported callback type.
*
* ERRNO: N/A
*/

LOCAL int hostSioCallbackInstall 
    (
    SIO_CHAN *	pSioChan,	/* pointer to device to control	*/
    int		callbackType,	/* callback type(tx or receive)	*/
    STATUS	(*callback)(),	/* pointer to callback function	*/
    void *	callbackArg	/* callback function argument 	*/
    )
    {
    HOST_SIO_CHAN * 	pChan; 		/* current channel	*/

    pChan = (HOST_SIO_CHAN*)pSioChan;

    switch (callbackType)
	{
	case SIO_CALLBACK_GET_TX_CHAR:
	    pChan->getTxChar    = callback;
	    pChan->getTxArg     = callbackArg;
	    break;
	case SIO_CALLBACK_PUT_RCV_CHAR:
	    pChan->putRcvChar   = callback;
	    pChan->putRcvArg    = callbackArg;
	    break;
	default:
            return (ENOSYS);
	}

    return (OK);
    }
