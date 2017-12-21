/* usrUsbBrcmAudio.c - USB Audio API */

/*
 * Copyright (c) 2006-2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
Modification history
--------------------
01p,23feb11,w_x  Fix duplicates by usrDepend.c and usrUsbInit.c (WIND00247788) 
01o,07jan11,ghs  Clean up compile warnings (WIND00247082)
01n,24jun10,m_y  Set all the resources to NULL after release,
                 modify code logic to improve usb stack robustness (WIND00183499)
01m,05jul10,ghs  Modify display logs(WIND00216586)
01l,23mar10,j_x  Changed for USB debug (WIND00184542)
01k,03mar10,ghs  Fix LP64 adapting(WIND00202103)
01j,13jan10,ghs  vxWorks 6.9 LP64 adapting
01i,21aug08,w_x  Free audio buffer after audio device closed (WIND00130647)
01h,16jul08,j_x  Fixed headset microphone test issue (WIND00127265)
01g,27jul07,jrp  WIND00099649 - USB documentation changes
01f,20Apr07,jrp  WIND00093113 and logitech 350 changes
01f,05apr07,jrp  vxBus SMP changes
01e,18jan06,jrp  Productized from services
01d,06oct06,j_l  Cleanup compiler warnings.
01c,02oct06,j_l  Add HID functions
01b,13sep06,j_l  Added headset speaker mono to stereo conversion
                 and microphone stereo to mono conversion options.
                 usbBrcmAudioOpen() can be called again after
                 usbBrcmAudioClose() without a reboot.
01a,08sep06,j_l  Created
*/

/*
 DESCRIPTION
 This library implements a bi-directional speaker interface such as the ones
 found on USB headsets.  This library operates with the following headsets:


 a. Logitech 350
 b. Plantronics CS50-USB
 c. GN Netcom GN 9350 NC USB/TEL (Part 9326-607-105)


 USING THIS LIBRARY
 The library is included in the vxWorks image by either defining
 INCLUDE_USB_HEADSET_DEMO or through the use of the project facility.

 It should be noted that the demo can be affected by shell operations or by
 programs run from the shell.

 */

#ifndef __INCusrUsbBrcmAudioc
#define __INCusrUsbBrcmAudioc

/* includes */

#include <vxWorks.h>
#include <config.h>
#include <usb/usbPlatform.h>
#include <usb/ossLib.h>     /* operations system srvcs */
#include <usb/usbOsal.h>
#include <usb/usb.h>
#include <drv/usb/usbSpeakerLib.h>
#include <usb/tools/wavFormat.h>    /* Microsoft .wav file format */
#include <usb/usbAudio.h>
#include <usb/usbHid.h>
#include <iv.h>
#include <ioLib.h>
#include <iosLib.h>
#include <tyLib.h>
#include <intLib.h>
#include <errnoLib.h>
#include <sioLib.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <logLib.h>
#include <selectLib.h>
#include <taskLib.h>

/* defines */

#define USB_BRCM_IOCTL_FUNC_ATTACH_SET            0x01
#define USB_BRCM_IOCTL_FUNC_RX_SET                0x02
#define USB_BRCM_IOCTL_CALLBK_PER                 0x04
#define USB_BRCM_IOCTL_FUNC_CFG_SET               0x05
#define USB_BRCM_IOCTL_GET_FORMAT_COUNT           0x06
#define USB_BRCM_IOCTL_GET_FORMAT_COUNT_MIC       0x07
#define USB_BRCM_IOCTL_GET_FORMAT_LIST            0x08
#define USB_BRCM_IOCTL_GET_FORMAT_LIST_MIC        0x09
#define USB_BRCM_IOCTL_SET_AUDIO_FORMAT           0x0a
#define USB_BRCM_IOCTL_SET_AUDIO_FORMAT_MIC       0x0b

#define USB_BRCM_IOCTL_GET_CHANNEL_COUNT          0x0c
#define USB_BRCM_IOCTL_GET_CHANNEL_CONFIG         0x0d
#define USB_BRCM_IOCTL_GET_CHANNEL_CAPS           0x0e
#define USB_BRCM_IOCTL_GET_CHANNEL_COUNT_MIC      0x0f
#define USB_BRCM_IOCTL_GET_CHANNEL_CONFIG_MIC     0x10
#define USB_BRCM_IOCTL_GET_CHANNEL_CAPS_MIC       0x11

#define USB_BRCM_IOCTL_SET_MUTE                   0x12
#define USB_BRCM_IOCTL_SET_VOLUME                 0x13
#define USB_BRCM_IOCTL_SET_MUTE_MIC               0x14
#define USB_BRCM_IOCTL_SET_VOLUME_MIC             0x15

#define USB_BRCM_IOCTL_SET_SPKR_MONO2STEREO       0x16
#define USB_BRCM_IOCTL_SET_MIC_STEREO2MONO        0x17

#define USB_BRCM_IOCTL_GET_DEV_INFO               0x18
#define USB_BRCM_IOCTL_SET_HID_CALLBACK           0x19
#define USB_BRCM_IOCTL_GET_HID_REPORT             0x1a
#define USB_BRCM_IOCTL_SET_HID_REPORT             0x1b
#define USB_BRCM_IOCTL_READ_HID_REPORT            0x1c
#define USB_BRCM_IOCTL_WRITE_HID_REPORT           0x1d
#define USB_BRCM_IOCTL_FUNC_RX_STOP               0x1e

#define USB_BRCM_OK                    (0)
#define USB_BRCM_ERROR                 (-1)

#ifndef USB_BRCM_SPKR_NAME
#define USB_BRCM_SPKR_NAME             "/usbSp/0"
#endif

/*
 * Optimal call back period in milliseconds
 */
#ifndef USB_BRCM_AUDIO_CALLBACK_PERIOD
#define USB_BRCM_AUDIO_CALLBACK_PERIOD    10
#endif
/* Fall back if the polling task can't support that rate in milliseconds */
#define USB_BRCM_AUDIO_CALLBACK_PERIOD_FALLBACK 250
/* Priority of the polling task */
#ifndef USB_BRCM_POLL_TASK_PRIORITY
#define USB_BRCM_POLL_TASK_PRIORITY 200
#endif

#ifndef SPKR_NAME_LEN_MAX
#define SPKR_NAME_LEN_MAX 100
#endif

#ifndef SPKR_MAX_DEVS
#define SPKR_MAX_DEVS 1
#endif


/* Stack size of the polling task */
#define POLL_TASK_STACK 0x1000
#define MILLISECPERSEC 1000

/* typedefs */

typedef enum
    {
    USB_BRCM_FREQ_ERROR,
    USB_BRCM_FREQ_8000HZ=8000,
    USB_BRCM_FREQ_16000HZ=16000
    } USB_BRCM_FREQ_TYPE;

typedef enum
    {
    USB_BRCM_PER_ERROR,
    USB_BRCM_PER_1MS,
    USB_BRCM_PER_2MS,
    USB_BRCM_PER_3MS,
    USB_BRCM_PER_4MS,
    USB_BRCM_PER_5MS,
    USB_BRCM_PER_6MS,
    USB_BRCM_PER_7MS,
    USB_BRCM_PER_8MS,
    USB_BRCM_PER_9MS,
    USB_BRCM_PER_10MS
    } USB_BRCM_PER_TYPE;

typedef struct
    {
    USB_BRCM_FREQ_TYPE sampFreq;
    } usbBrcmHalCfg;

typedef VOID (*USB_BRCM_USER_ATTACH_CALLBACK)
    (
    UINT16 attachCode,              /* defined as USB_SPKR_xxxx */
    UINT16 fmtCount,                /* headset speaker format count */
    pUSB_SPKR_AUDIO_FORMAT pFmt,    /* headset speaker formats */
    UINT16 micFmtCount,             /* headset microphone format count */
    pUSB_SPKR_AUDIO_FORMAT pMicFmt  /* headset microphone formats */
    );

typedef int (*USB_BRCM_CFG_FUNC_TYPE)
    (
    usbBrcmHalCfg configData
    );

typedef VOID (*USB_BRCM_RX_FUNC_TYPE)
    (
    void
    );


typedef struct brcm_audio_properties_type
    {
    BOOL inUse;                             /* this node in use */
    BOOL attached;                          /* headset attached  */
    int fd;                                 /* lower level file descriptor */
    int mode;                               /* open mode */
    int flags;                              /* open flags: O_RDONLY, etc */
    char spkrName[SPKR_NAME_LEN_MAX];       /* device name */
    SEQ_DEV *pSeqDev;                       /* SEQ_DEV interface pointer */
    USB_SPKR_ATTACH_CALLBACK dynAttCallback;/* USB dynamic attach callback */
    USB_BRCM_USER_ATTACH_CALLBACK usrDynAttCallback;
    /* User dynamic attach callback */
    USB_BRCM_CFG_FUNC_TYPE cfgFunc;  /* PhonexChange configuration callback */
    USB_SPKR_HID_CALLBACK_TYPE hidCallback;
    /* Asynchronous HID report receive callback */
    USB_BRCM_RX_FUNC_TYPE rxFunc;   /* call back used when streaaming data */
    USB_BRCM_PER_TYPE callbackPer;      /* Callback period - Msecs */
    USB_BRCM_PER_TYPE ticks;            /* Clock ticks for streaming task */
    WDOG_ID pWatchDog;                  /* Watchdog Id when streaming data */
    SEM_ID pStreamSem;                  /* Semaphore used when streaming data */
    volatile BOOL streamData;           /* Set while streaming data */
    UINT16 fmtCount;                    /* headset speaker format count */
    pUSB_SPKR_AUDIO_FORMAT pFmt;        /* headset speaker formats */
    UINT16 micFmtCount;                 /* headset microphone format count */
    pUSB_SPKR_AUDIO_FORMAT pMicFmt;     /* headset microphone formats */
    USB_SPKR_AUDIO_FORMAT desiredFmt;
    /* desired format to set headset speaker format to */
    USB_SPKR_AUDIO_FORMAT desiredMicFmt;
    /* desired format to set headset microphone format to */
    BOOL rdWrtActive;                   /* Reads and writes active */
    BOOL spkrMono2Stereo;       /* Convert speaker output from mono to stereo */
    char *spkrConversionBuf;    /* Conversion temporary buffer */
    int spkrConversionBufSize;  /* Conversion temporary buffer size */
    BOOL micStereo2Mono;      /* Convert microphone input from stereo to mono */
    char *micConversionBuf;     /* Conversion temporary buffer */
    int micConversionBufSize;   /* Conversion temporary buffer size */
    } BRCM_AUDIO_PROPERTIES_TYPE;



/* local variables */

LOCAL BRCM_AUDIO_PROPERTIES_TYPE usbBrcmAudioProperties[SPKR_MAX_DEVS];
LOCAL int usbBrcmAudioActiveNode = USB_BRCM_ERROR;
int usbBrcmCallbackPeriod=-1;
LOCAL SEM_ID usbBrcmAudioMutex = NULL;

/* forward declarations */

LOCAL void usbBrcmAudioAttachCallback
    (
    void * arg,             /* caller-defined argument */
    SEQ_DEV *pSeqDev,		/* pointer to affected SEQ_DEV */
    UINT16 attachCode		/* defined as USB_SPKR_xxxx */
    );

/***************************************************************************
*
* computeCallbackPeriod - This routine computes the rate at which the
* usb headset stream audio stream is refreshed.  This routine takes the
* processor clock rate (sysClkRateGet) into consideration and will adjust
* the call back period accordingly
*
* RETURNS: none
*
* ERRNO: none
*
* \NOMANUAL
*/
LOCAL void computeCallbackPeriod
    (
    int proposedCallbackInterval,
    int *milliSeconds,
    int *ticks
    )
    {
    INT32 milliSecsPerTick;
    INT32 ticksPerMillisecond;
    /*
     * If the system clock can support the requested rate, great.  If not,
     * use the fall back rate is is defined a 1/4 second.  That works for
     * 60hz clocks which is the lowest clock rate for vxWorks.
     */
    if (sysClkRateGet() <= MILLISECPERSEC)
        {
        milliSecsPerTick = (MILLISECPERSEC + (sysClkRateGet() -1)) /
                            sysClkRateGet();
        *milliSeconds = proposedCallbackInterval;
        if (*milliSeconds <= milliSecsPerTick)
            {
            *milliSeconds = USB_BRCM_AUDIO_CALLBACK_PERIOD_FALLBACK;
            }
        /*
         *
         */
        *ticks = (*milliSeconds + (milliSecsPerTick - 1))/milliSecsPerTick;

        USB1_SPK_DBG("%s: Poll rate at %d millisecs or %d "
                     "ticks at %d ticks per millisecond \n",
                     __FUNCTION__, *milliSeconds, *ticks, milliSecsPerTick, 5, 6);
        }
    else
        {
        ticksPerMillisecond = (sysClkRateGet() + (MILLISECPERSEC - 1)) /
        MILLISECPERSEC;
        *milliSeconds = proposedCallbackInterval;
        *ticks = *milliSeconds * ticksPerMillisecond;
        }

    return;
    }
/***************************************************************************
*
* usbBrcmTimerISR - This routine is invoked by the watchdog timer at the
* completion of the each period.  It is responsible for re-enabling itself
* (or not based on the streamData flag) and giving the semaphore to the
* task that actually does the data stream.  NOTE:  THIS IS EXECUTED IN
* ISR CONTEXT
*
* RETURNS: OK
*
* ERRNO: OK
*
* \NOMANUAL
*/

LOCAL INT32 usbBrcmTimerISR
    (
    INT32 channel
    )
    {
    semGive (usbBrcmAudioProperties[channel].pStreamSem);
    if (usbBrcmAudioProperties[channel].streamData == TRUE)
        {
        /*
         * There is more data to process - reset the watchdog
         * (oversample a bit)
         */
        wdStart(usbBrcmAudioProperties[channel].pWatchDog,
            usbBrcmAudioProperties[channel].ticks-1,
            (WD_RTN)usbBrcmTimerISR, channel);
        }
    return (OK);
    }
/***************************************************************************
*
* usbHdsetStreamingTask - This task which is started when the headset has
* its streaming data started spins on a semaphore and calls the function
* - which is passed in when the task starts, each time the semaphore is
* given.  Note that this task starts the watchdog that gives the semaphore
* at the desired interval.
*
* RETURNS: OK, ERROR is returned if system resources are exausted.
*
* ERRNO: none
*
* \NOMANUAL
*/
LOCAL INT32 usbHdsetStreamingTask
    (
    void (*func)(void),
    INT32 channel
    )
    {
    /*
     * There is a potential race condition between a streaming task
     * that is being shutdown and a new that is being started for the
     * same channel.
     *
     * To get around this, we wait until the previous task associated
     * with a channel sets the watch dog id to null (which is the last
     * thing it does).
     */
    while (usbBrcmAudioProperties[channel].pWatchDog != NULL)
        {
        USB1_SPK_WARN("%s[L%d]: Previous streaming task still active, Waiting... \n",
                      __FILE__, __LINE__, 3, 4, 5, 6);
        taskDelay(sysClkRateGet()); /* Task delay one second */
        }
    /*
     * Create a watchdog timer.  The watchdog will give the semaphore
     * and reset itself.  This is done to minimize the drift experienced
     * by a simple taskDelay loop
     */
    usbBrcmAudioProperties[channel].pWatchDog = wdCreate();
    if (usbBrcmAudioProperties[channel].pWatchDog == NULL)
        {
        USB1_SPK_ERR("Unable to create timer for streaming task \n",
                     1, 2, 3, 4, 5, 6);
        exit(ERROR);
        }
    /*
     * The semaphore is released by the watchdog at each
     * refresh interval
     */
    usbBrcmAudioProperties[channel].pStreamSem =
        semBCreate(SEM_Q_FIFO, SEM_EMPTY);
    if (usbBrcmAudioProperties[channel].pStreamSem == NULL)
        {
        USB1_SPK_ERR("Unable to create streaming task semaphore \n",
                     1, 2, 3, 4, 5, 6);
        wdDelete (usbBrcmAudioProperties[channel].pWatchDog);
        usbBrcmAudioProperties[channel].pWatchDog = NULL;
        exit(ERROR);
        }
    /*
     * The streamData flag allows for an orderly shutdown of the
     * streaming data task
     */
    usbBrcmAudioProperties[channel].streamData = TRUE;
    /*
     * Start the timer and pass the fileDescriptor of the
     * desired speaker instance;
     */
    wdStart(usbBrcmAudioProperties[channel].pWatchDog,
        usbBrcmAudioProperties[channel].ticks,
        (WD_RTN)usbBrcmTimerISR, channel);

    /*
     * Loop on the semaphore until someone sets the streamData flag to
     * false.  Note the watchdog also looks at the flag and will stop
     * rescheduling itself
     */
    do
        {
        semTake(usbBrcmAudioProperties[channel].pStreamSem,
            WAIT_FOREVER);
        USB1_SPK_VDBG("%s[L%d]: Executing rxfunc \n",
                      __FILE__, __LINE__, 3, 4, 5, 6);
        (*func)();
        } while (usbBrcmAudioProperties[channel].streamData == TRUE);
    /*
     * When the loop is broken, a shutdown has been requested.
     *
     * Wait for a period to make sure the watch dog has detected
     * the shutdown.  The watchdog is an ISR so a simple wait
     * is sufficent
     */
    taskDelay(usbBrcmAudioProperties[channel].ticks + 1);
    /*
     * So the watchdog has fired and didn't restart itself.
     * It is now safe to delete the watchdog and the semaphore
     */
    semDelete (usbBrcmAudioProperties[channel].pStreamSem);
    usbBrcmAudioProperties[channel].pStreamSem = NULL;
    /* */
    wdDelete(usbBrcmAudioProperties[channel].pWatchDog);
    usbBrcmAudioProperties[channel].pWatchDog = NULL;
    return(OK);
    }


/***************************************************************************
*
* usbBrcmAudioChannelsInitializeNode - initializes headset data structure
*                                      node
*
* RETURNS: none
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL VOID usbBrcmAudioChannelsInitializeNode
    (
    INT32 node               /* array index */
    )
    {
    usbBrcmAudioProperties[node].inUse = FALSE;
    usbBrcmAudioProperties[node].attached = FALSE;
    usbBrcmAudioProperties[node].fd = USB_BRCM_ERROR;
    usbBrcmAudioProperties[node].mode = 0666;
    usbBrcmAudioProperties[node].flags = O_RDWR;
    usbBrcmAudioProperties[node].pSeqDev = (SEQ_DEV *)NULL;
    usbBrcmAudioProperties[node].dynAttCallback = usbBrcmAudioAttachCallback;
    usbBrcmAudioProperties[node].usrDynAttCallback = NULL;
    usbBrcmAudioProperties[node].cfgFunc = NULL;
    usbBrcmAudioProperties[node].hidCallback.function = NULL;
    usbBrcmAudioProperties[node].rxFunc = NULL;
    computeCallbackPeriod (USB_BRCM_AUDIO_CALLBACK_PERIOD,
        (int *) &usbBrcmAudioProperties[node].callbackPer,
        (int *) &usbBrcmAudioProperties[node].ticks);
    usbBrcmCallbackPeriod = usbBrcmAudioProperties[node].callbackPer;
    /* callBackPer is in milliseconds */
    usbBrcmAudioProperties[node].pWatchDog = NULL;
    usbBrcmAudioProperties[node].pStreamSem = NULL;
    usbBrcmAudioProperties[node].streamData = FALSE;
    usbBrcmAudioProperties[node].rdWrtActive = FALSE;
    usbBrcmAudioProperties[node].fmtCount = 0;
    usbBrcmAudioProperties[node].pFmt = NULL;
    usbBrcmAudioProperties[node].micFmtCount = 0;
    usbBrcmAudioProperties[node].pMicFmt = NULL;
    usbBrcmAudioProperties[node].spkrMono2Stereo = FALSE;
    usbBrcmAudioProperties[node].spkrConversionBuf = NULL;
    usbBrcmAudioProperties[node].spkrConversionBufSize = 0;
    usbBrcmAudioProperties[node].micStereo2Mono = FALSE;
    usbBrcmAudioProperties[node].micConversionBuf = NULL;
    usbBrcmAudioProperties[node].micConversionBufSize = 0;
    bzero ((char *)&usbBrcmAudioProperties[node].desiredFmt,
        sizeof (usbBrcmAudioProperties[node].desiredFmt));
    bzero ((char *)&usbBrcmAudioProperties[node].desiredMicFmt,
        sizeof (usbBrcmAudioProperties[node].desiredMicFmt));
    bzero ((char *)&usbBrcmAudioProperties[node].spkrName,
        sizeof (&usbBrcmAudioProperties[node].spkrName));

    USB1_SPK_DBG("%s: Call back time in millsecs %d \n",
                 __FUNCTION__,
                 usbBrcmAudioProperties[node].callbackPer, 3, 4, 5, 6);
    return;
    }

/***************************************************************************
*
* usbBrcmAudioChannelsInitialize - initializes headset data structures
*
* RETURNS: none
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL VOID usbBrcmAudioChannelsInitialize
    (
    void
    )
    {
    int i;
    static BOOL initialized=FALSE;

    if (initialized == TRUE)
        {
        return;
        }

    for (i = 0; i < SPKR_MAX_DEVS; i++)
        {
        usbBrcmAudioChannelsInitializeNode(i);
        }

    initialized = TRUE;
    }

/***************************************************************************
*
* usbBrcmAudioChannelsFindSpkrName - find headset data structure with
*                                    specified device name
*
* RETURNS: matching node number or USB_BRCM_ERROR if not found
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL INT32 usbBrcmAudioChannelsFindSpkrName
    (
    char *devp               /* device name such as /usbSp/0 */
    )
    {
    int i;

    if (devp == NULL)
        {
        return USB_BRCM_ERROR;
        }

    /* search for name */
    for (i = 0; i < SPKR_MAX_DEVS; i++)
        {
        if (strcmp(usbBrcmAudioProperties[i].spkrName, devp)==0)
            {
            return i;
            }
        }

    /* match not found */
    return USB_BRCM_ERROR;
    }

/***************************************************************************
*
* usbBrcmAudioChannelsFindFree - find free headset node
*
* RETURNS: free node number or USB_BRCM_ERROR if not found
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL INT32 usbBrcmAudioChannelsFindFree
    (
    char *devp               /* device name such as /usbSp/0 */
    )
    {
    INT32 i;

    if (devp == NULL)
        {
        return USB_BRCM_ERROR;
        }

    /* verify that the named device is not in use */

    if (usbBrcmAudioChannelsFindSpkrName(devp) != USB_BRCM_ERROR)
        {
        return USB_BRCM_ERROR;
        }

    /* find a free headset node */

    for (i = 0; i < SPKR_MAX_DEVS; i++)
        {
        if (usbBrcmAudioProperties[i].inUse == FALSE)
            {
            return i;
            }
        }

    /* not found */
    return USB_BRCM_ERROR;
    }

/***************************************************************************
*
* usbBrcmAudioAttach - headset operations upon an attach
*
* RETURNS: none
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL VOID usbBrcmAudioAttach
    (
    int node                            /* file descriptor */
    )
    {
    int fileDescr = 0;

    if (usbBrcmAudioProperties[node].inUse == TRUE)
        {
        if (usbBrcmAudioProperties[node].fd == ERROR)
            {
            /* open the named device */
            if ((fileDescr=open(usbBrcmAudioProperties[node].spkrName,
                        usbBrcmAudioProperties[node].flags,
                        usbBrcmAudioProperties[node].mode)) == ERROR)
                {
                usbBrcmAudioProperties[node].fd = ERROR;
                USB1_SPK_ERR("Cannot open '%s' \n",
                             USB_BRCM_SPKR_NAME, 2, 3, 4, 5, 6);
                }
            else
                {
                USB1_SPK_DBG("%s: Speaker %s at node %d opened \n",
                             __FUNCTION__,
                             usbBrcmAudioProperties[node].spkrName, node,
                             4, 5, 6);

                /* store 'real' file descriptor */
                usbBrcmAudioProperties[node].fd = fileDescr;
                USB1_SPK_DBG("%s: open '%s' \n",
                             __FUNCTION__, USB_BRCM_SPKR_NAME, 3, 4, 5, 6);
                /* restore desired headset properties */
                ioctl(fileDescr, USB_SPKR_IOCTL_SET_BUF_FULL_BLOCK, FALSE);
                ioctl(fileDescr, USB_SPKR_IOCTL_SET_HALF_BUF_IN_MSEC,
                    usbBrcmAudioProperties[node].callbackPer);
                ioctl(fileDescr, USB_SPKR_IOCTL_SET_HALF_BUF_IN_MSEC_MIC,
                    usbBrcmAudioProperties[node].callbackPer);
                ioctl(fileDescr, USB_SPKR_IOCTL_SET_AUDIO_FORMAT,
                    (ULONG) &usbBrcmAudioProperties[node].desiredFmt);
                USB1_SPK_DBG("%s: In usbBrcmAudioAttach set mic format \n",
                             __FUNCTION__, 2, 3, 4, 5, 6);
                ioctl(fileDescr, USB_SPKR_IOCTL_SET_AUDIO_FORMAT_MIC,
                    (ULONG) &usbBrcmAudioProperties[node].desiredMicFmt);
                if (usbBrcmAudioProperties[node].hidCallback.function != NULL)
                    {
                    ioctl(fileDescr, USB_SPKR_IOCTL_SET_HID_CALLBACK,
                        (ULONG)&usbBrcmAudioProperties[node].hidCallback);
                    }
                usbBrcmAudioProperties[node].rdWrtActive = TRUE;
                }
            }
        }
    }

/***************************************************************************
*
* usbBrcmAudioDetach - headset operations upon a detach
*
* RETURNS: none
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL VOID usbBrcmAudioDetach
    (
    int node                          /* file descriptor */
    )
    {
    if (usbBrcmAudioProperties[node].inUse == TRUE)
        {
        if (usbBrcmAudioProperties[node].fd != ERROR)
            {
            /* disable read/write activity */
            usbBrcmAudioProperties[node].rdWrtActive = FALSE;
            /* close named device */
            close(usbBrcmAudioProperties[node].fd);
            USB1_SPK_DBG("%s: Speaker %s closed \n",
                         __FUNCTION__, USB_BRCM_SPKR_NAME,
                         3, 4, 5, 6);
            usbBrcmAudioProperties[node].fd = ERROR;
            }
        }
    return;
    }

/*************************************************************************
*
* usbBrcmAudioAttachCallback - receives attach callbacks from speaker SIO
*                              driver
*
* This function is called whenever the speaker device is connected or
* removed.
*
* RETURNS: none
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL VOID usbBrcmAudioAttachCallback
    (
    void *   arg,			/* caller-defined argument */
    SEQ_DEV *pSeqDev,		/* pointer to affected SEQ_DEV */
    UINT16   attachCode		/* defined as USB_SPKR_xxxx */
    )
    {
    ULONG node = (ULONG) arg;

    if (semTake(usbBrcmAudioMutex, WAIT_FOREVER) != OK)
        {
        return;
        }

    if (attachCode == USB_SPKR_ATTACH)
        {
        USB1_SPK_INFO("Found BrcmAudio device %p at node %d \n",
                      pSeqDev, node, 3, 4, 5, 6);

        if (usbSpeakerSeqDevLock (pSeqDev) != OK)
            {
            USB1_SPK_ERR("usbSpeakerSeqDevLock() returned ERROR \n",
                         1, 2, 3, 4, 5, 6);
            }
        else
            {
            if (usbBrcmAudioProperties[node].pSeqDev != NULL)
                {
                usbSpeakerSeqDevUnlock (pSeqDev);
                semGive(usbBrcmAudioMutex);
                return;
                }

            usbBrcmAudioProperties[node].pSeqDev = pSeqDev;

            usbBrcmAudioAttach(node);

            /* look up headset speaker properties */

            if (ioctl(usbBrcmAudioProperties[node].fd,
                    USB_SPKR_IOCTL_GET_FORMAT_COUNT,
                    (ULONG) &usbBrcmAudioProperties[node].fmtCount) !=OK)
                {
                USB1_SPK_ERR("IOCTL GET_FORMAT_COUNT returned ERROR \n",
                             1, 2, 3, 4, 5, 6);
                }

            if (ioctl(usbBrcmAudioProperties[node].fd,
                    USB_SPKR_IOCTL_GET_FORMAT_LIST,
                    (ULONG) &usbBrcmAudioProperties[node].pFmt) !=OK)
                {
                USB1_SPK_ERR("IOCTL GET_FORMAT_LIST returned ERROR \n",
                             1, 2, 3, 4, 5, 6);
                }

                /* look up headset microphone properties */

            if (ioctl(usbBrcmAudioProperties[node].fd,
                    USB_SPKR_IOCTL_GET_FORMAT_COUNT_MIC,
                    (ULONG) &usbBrcmAudioProperties[node].micFmtCount) !=OK)
                {
                USB1_SPK_ERR("IOCTL GET_FORMAT_COUNT_MIC returned ERROR \n",
                             1, 2, 3, 4, 5, 6);
                }

            if (ioctl(usbBrcmAudioProperties[node].fd,
                    USB_SPKR_IOCTL_GET_FORMAT_LIST_MIC,
                    (ULONG) &usbBrcmAudioProperties[node].pMicFmt) != OK)
                {
                USB1_SPK_ERR("IOCTL GET_FORMAT_LIST_MIC returned ERROR \n",
                             1, 2, 3, 4, 5, 6);
                }

            semGive(usbBrcmAudioMutex);

            /* if the user has registered a callback, call it */

            if (usbBrcmAudioProperties[node].usrDynAttCallback != NULL)
                {
                (*(usbBrcmAudioProperties[node].usrDynAttCallback))
                    (
                        attachCode,
                        usbBrcmAudioProperties[node].fmtCount,
                        usbBrcmAudioProperties[node].pFmt,
                        usbBrcmAudioProperties[node].micFmtCount,
                        usbBrcmAudioProperties[node].pMicFmt
                    );
                }

            if (semTake(usbBrcmAudioMutex, WAIT_FOREVER)!=OK)
                {
                USB1_SPK_ERR("semTake() returned ERROR \n",
                             1, 2, 3, 4, 5, 6);
                }
            usbBrcmAudioProperties[node].attached = TRUE;
            }
        }
    else if (attachCode == USB_SPKR_REMOVE)
        {
        USB1_SPK_INFO("BrcmAudio device %p at node %d detached \n",
                      pSeqDev, node, 3, 4, 5, 6);

        if (usbBrcmAudioProperties[node].pSeqDev != pSeqDev)
            {
            semGive(usbBrcmAudioMutex);
            return;
            }

        usbBrcmAudioProperties[node].attached = FALSE;

        usbBrcmAudioDetach(node);

        usbBrcmAudioProperties[node].pSeqDev = (SEQ_DEV *) NULL;

        /* if the user has registered a callback, call it */

        if (usbBrcmAudioProperties[node].usrDynAttCallback != NULL)
            {
            (*(usbBrcmAudioProperties[node].usrDynAttCallback))
                (
                    attachCode,
                    usbBrcmAudioProperties[node].fmtCount,
                    usbBrcmAudioProperties[node].pFmt,
                    usbBrcmAudioProperties[node].micFmtCount,
                    usbBrcmAudioProperties[node].pMicFmt
                );
            }

        if (usbSpeakerSeqDevUnlock (pSeqDev) != OK)
            {
            USB1_SPK_ERR("usbSpeakerSeqDevUnlock() returned ERROR \n",
                         1, 2, 3, 4, 5, 6);
            }
        }
    semGive(usbBrcmAudioMutex);
    return;
    }

/*************************************************************************
*
* usbBrcmAudioInit - initialization
*
* RETURNS: USB_BRCM_OK if ok, USB_BRCM_ERROR if error
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL int usbBrcmAudioInit(void)
    {
    static int initialized=FALSE;

    if (initialized == TRUE)
        {
        return USB_BRCM_OK;
        }

    if ((usbBrcmAudioMutex = semMCreate (SEM_Q_FIFO|SEM_DELETE_SAFE)) == NULL)
        {
        USB1_SPK_ERR("semMCreate ERROR \n",
                     1, 2, 3, 4, 5, 6);
        return USB_BRCM_ERROR;
        }

    if (semTake(usbBrcmAudioMutex, WAIT_FOREVER) != OK)
        {
        return USB_BRCM_ERROR;
        }

    usbBrcmAudioChannelsInitialize();

    initialized = TRUE;

    semGive(usbBrcmAudioMutex);

    return USB_BRCM_OK;
    }

/*************************************************************************
*
* usbBrcmAudioSetActiveNode - record current active file descriptor
*
* RETURNS: none
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL VOID usbBrcmAudioSetActiveNode
    (
    int node                            /* file descriptor */
    )
    {
    usbBrcmAudioActiveNode = node;
    }

/*************************************************************************
*
* usbBrcmAudioGetActiveNode - retrieves current active file descriptor
*
* RETURNS: Node or USB_BRCM_ERROR if none are active
*
* ERRNO: none
*
*/

LOCAL int usbBrcmAudioGetActiveNode( void )
    {
    return usbBrcmAudioActiveNode;
    }

/***************************************************************************
*
* usbBrcmAudioIoctl - IOCTL routine
*
* This function handles the various ioctl functions
*
* RETURNS: USB_BRCM_OK if ok, USB_BRCM_ERROR if error
*
* ERRNO: none
*
*/

LOCAL INT32 usbBrcmAudioIoctl
    (
    int   node,                         /* file descriptor */
    int   cmd,                          /* ioctl() function code */
    pVOID arg                           /* function-specific argument */
    )
    {
    /* Handle IOCTL */
    usbBrcmHalCfg configData = {USB_BRCM_FREQ_ERROR};
    STATUS stat = USB_BRCM_OK;
    BOOL restartStreamer;

    if (semTake(usbBrcmAudioMutex, WAIT_FOREVER)!=OK)
        {
        return USB_BRCM_ERROR;
        }

    if (usbBrcmAudioProperties[node].inUse == FALSE)
        {
        semGive(usbBrcmAudioMutex);
        return USB_BRCM_ERROR;
        }
    USB1_SPK_DBG("%s: IOCTL switch 0x%X \n",
                 __FUNCTION__, cmd, 3, 4, 5, 6);
    switch (cmd)
        {
        case USB_BRCM_IOCTL_FUNC_ATTACH_SET:
            usbBrcmAudioProperties[node].usrDynAttCallback =
            (USB_BRCM_USER_ATTACH_CALLBACK) arg;
            if (usbBrcmAudioProperties[node].attached == TRUE)
                {
                (*(usbBrcmAudioProperties[node].usrDynAttCallback))
                    (
                    USB_SPKR_ATTACH,
                    usbBrcmAudioProperties[node].fmtCount,
                    usbBrcmAudioProperties[node].pFmt,
                    usbBrcmAudioProperties[node].micFmtCount,
                    usbBrcmAudioProperties[node].pMicFmt
                    );
                }
            break;

        case USB_BRCM_IOCTL_FUNC_RX_SET:
            usbBrcmAudioProperties[node].rxFunc = (USB_BRCM_RX_FUNC_TYPE) arg;
            USB1_SPK_DBG("%s: rxFunc set to 0x%X \n",
                         __FUNCTION__, arg, 3, 4, 5, 6);
                {
                char taskName[26];
                sprintf(taskName, "tStrmAudio%d", node);
                taskSpawn(taskName,
                        USB_BRCM_POLL_TASK_PRIORITY,
                        0,
                        POLL_TASK_STACK,
                        usbHdsetStreamingTask,
                        (ULONG)usbBrcmAudioProperties[node].rxFunc,
                        node,
                        0, 0, 0, 0, 0, 0, 0, 0);
                }
            break;

        case USB_BRCM_IOCTL_FUNC_RX_STOP:
            /*
             * Stop the streaming task.  This is done by setting the flag
             * to false and waiting long enough for the ISR watchdog to fire.
             */
            usbBrcmAudioProperties[node].streamData = FALSE;
            taskDelay(usbBrcmAudioProperties[node].ticks+1);
            break;

        case USB_BRCM_IOCTL_CALLBK_PER:
            /*
             * If the streaming task is running, shut it down and restart
             * once we have recomputed the poll period.
             */
            if (usbBrcmAudioProperties[node].streamData == TRUE)
                {
                restartStreamer = TRUE;
                /*
                 * Shutdown any current executing audio streams by setting
                 * flag to false.  wait a bit to at least let the watch dog
                 * fire
                 */
                usbBrcmAudioProperties[node].streamData = FALSE;
                taskDelay(usbBrcmAudioProperties[node].ticks+1);
                }
            else
                {
                restartStreamer = FALSE;
                }
            /*
             * Recompute the poll rate for the streaming data
             */
            computeCallbackPeriod ((USB_BRCM_FREQ_TYPE) arg,
                (int *)&usbBrcmAudioProperties[node].callbackPer,
                (int *)&usbBrcmAudioProperties[node].ticks);
            USB1_SPK_DBG("%s: Callback period reset to %d \n",
                         __FUNCTION__,
                         usbBrcmAudioProperties[node].callbackPer, 3, 4, 5, 6);
            if (usbBrcmAudioProperties[node].fd != ERROR)
                {
                usbBrcmAudioDetach(node);
                usbBrcmAudioAttach(node);
                }

            if (restartStreamer == TRUE)
                {
                char taskName[26];
                sprintf(taskName, "tStrmAudio%d", node);
                taskSpawn(taskName,
                    USB_BRCM_POLL_TASK_PRIORITY,
                    0,
                    POLL_TASK_STACK,
                    usbHdsetStreamingTask,
                    (ULONG)usbBrcmAudioProperties[node].rxFunc,
                    node,
                    0, 0, 0, 0, 0, 0, 0, 0);
                }

            break;

        case USB_BRCM_IOCTL_FUNC_CFG_SET:
            usbBrcmAudioProperties[node].cfgFunc = (USB_BRCM_CFG_FUNC_TYPE) arg;
            break;

        case USB_BRCM_IOCTL_GET_FORMAT_COUNT:
            stat = ioctl(usbBrcmAudioProperties[node].fd,
                USB_SPKR_IOCTL_GET_FORMAT_COUNT, arg);
            break;

        case USB_BRCM_IOCTL_GET_FORMAT_COUNT_MIC:
            stat = ioctl(usbBrcmAudioProperties[node].fd,
                USB_SPKR_IOCTL_GET_FORMAT_COUNT_MIC, arg);
            break;

        case USB_BRCM_IOCTL_GET_FORMAT_LIST:
            stat = ioctl(usbBrcmAudioProperties[node].fd,
                USB_SPKR_IOCTL_GET_FORMAT_LIST, arg);
            break;

        case USB_BRCM_IOCTL_GET_FORMAT_LIST_MIC:
            USB1_SPK_DBG("%s: USB_BRCM_IOCTL_GET_FORMAT_LIST_MIC, arg = 0x%x \n",
                         __FUNCTION__,
                         arg, 3, 4, 5, 6);
            stat = ioctl(usbBrcmAudioProperties[node].fd,
                USB_SPKR_IOCTL_GET_FORMAT_LIST_MIC, arg);
            break;

        case USB_BRCM_IOCTL_SET_AUDIO_FORMAT:
            memcpy((char *)&usbBrcmAudioProperties[node].desiredFmt,(char *)arg,
                sizeof(usbBrcmAudioProperties[node].desiredFmt));

            USB1_SPK_DBG("%s: Setting Speaker audio format to type 0x%X 0x%X 0x%X \n",
                         __FUNCTION__,
                         usbBrcmAudioProperties[node].desiredFmt.formatType,
                         ((pUSB_SPKR_AUDIO_FORMAT)arg)->formatType, arg, 5, 6);
            /*
             * Obtain the sample frequency from the format.  This is stored
             * differently depending on the format.
             *
             * If it is a discrete Frequency (type 5), select the first
             * one.  If its a range and 8k is possible, take that one.  It
             * seems to work best
             */
            if (usbBrcmAudioProperties[node].desiredFmt.freqType == 0)
                {
                if (usbBrcmAudioProperties[node].desiredFmt.lowerFreq <= 8000 &&
                    usbBrcmAudioProperties[node].desiredFmt.upperFreq >= 8000)
                    {
                    usbBrcmAudioProperties[node].
                        desiredFmt.sampleFrequency = 8000;
                    }
                else
                    {
                    usbBrcmAudioProperties[node].
                         desiredFmt.sampleFrequency =
                         usbBrcmAudioProperties[node].desiredFmt.lowerFreq;
                    }
                 }
            else
                {
                usbBrcmAudioProperties[node].desiredFmt.sampleFrequency =
                    usbBrcmAudioProperties[node].desiredFmt.freq[0];
                }

            USB1_SPK_DBG("%s: USB_BRCM_IOCTL_SET_AUDIO_FORMAT: " \
                         "Type %d  Frequency Selected - %d \n",
                         __FUNCTION__,
                         usbBrcmAudioProperties[node].desiredFmt.freqType,
                         usbBrcmAudioProperties[node].desiredFmt.sampleFrequency,
                         4, 5, 6);
            /*
             * Set the speaker format using the speaker Library ioctl.
             */
            stat = ioctl(usbBrcmAudioProperties[node].fd,
                USB_SPKR_IOCTL_SET_AUDIO_FORMAT,
                &usbBrcmAudioProperties[node].desiredFmt);

            USB1_SPK_DBG("%s: status of ioctl in set audio format ioctl - %d \n",
                         __FUNCTION__, stat, 3, 4, 5, 6);
            /*
             * If there is a configuration change callback, call it
             */
            if (usbBrcmAudioProperties[node].cfgFunc != NULL)
                {
                (*(usbBrcmAudioProperties[node].cfgFunc))(configData);
                }
            break;

        case USB_BRCM_IOCTL_SET_AUDIO_FORMAT_MIC:
            USB1_SPK_DBG("%s: Setting Microphone audio format to type 0x%X 0x%X 0x%X \n",
                         __FUNCTION__,
                         usbBrcmAudioProperties[node].desiredFmt.formatType,
                         ((pUSB_SPKR_AUDIO_FORMAT)arg)->formatType,
                         arg, 5, 6);
            memcpy((char *)&usbBrcmAudioProperties[node].desiredMicFmt,
                (char *)arg,sizeof(usbBrcmAudioProperties[node].desiredMicFmt));

            /*
             * Obtain the sample frequency from the format.  This is stored
             * differently depending on the format. This is just like the
             * speaker ioctl above.
             */

            if (usbBrcmAudioProperties[node].desiredMicFmt.freqType == 0)
                {
                if (usbBrcmAudioProperties[node].desiredMicFmt.lowerFreq
                                                                    <= 8000 &&
                     usbBrcmAudioProperties[node].desiredMicFmt.upperFreq
                                                                      >= 8000)
                     {
                     usbBrcmAudioProperties[node].
                         desiredMicFmt.sampleFrequency = 8000;
                     }
                 else
                     {
                     usbBrcmAudioProperties[node].
                          desiredMicFmt.sampleFrequency =
                          usbBrcmAudioProperties[node].desiredMicFmt.lowerFreq;
                     }
                 }
            else
                {
                usbBrcmAudioProperties[node].desiredMicFmt.sampleFrequency =
                           usbBrcmAudioProperties[node].desiredMicFmt.freq[0];
                }

            USB1_SPK_DBG("%s: USB_BRCM_IOCTL_SET_AUDIO_FORMAT_MIC: " \
                         "Type %d  Frequency Selected - %d \n",
                         __FUNCTION__,
                         usbBrcmAudioProperties[node].desiredMicFmt.freqType,
                         ((pUSB_SPKR_AUDIO_FORMAT)arg)->formatType, 4, 5, 6);
            USB1_SPK_DBG("%s: Setting Microphone audio format to type 0x%X 0x%X \n",
                         __FUNCTION__,
                         usbBrcmAudioProperties[node].desiredMicFmt.formatType,
                         usbBrcmAudioProperties[node].desiredMicFmt.sampleFrequency,
                         4, 5, 6);

            stat = ioctl(usbBrcmAudioProperties[node].fd,
                USB_SPKR_IOCTL_SET_AUDIO_FORMAT_MIC,
                &usbBrcmAudioProperties[node].desiredMicFmt);

            USB1_SPK_DBG("%s: Status of the microphone format set ioctl - %d \n",
                         __FUNCTION__, stat, 3, 4, 5, 6);
            break;

        case USB_BRCM_IOCTL_GET_CHANNEL_COUNT:
            stat = ioctl(usbBrcmAudioProperties[node].fd,
                USB_SPKR_IOCTL_GET_CHANNEL_COUNT,
                arg);
            break;

        case USB_BRCM_IOCTL_GET_CHANNEL_COUNT_MIC:
            stat = ioctl(usbBrcmAudioProperties[node].fd,
                USB_SPKR_IOCTL_GET_CHANNEL_COUNT_MIC,
                arg);
            break;

        case USB_BRCM_IOCTL_GET_CHANNEL_CONFIG:
            stat = ioctl(usbBrcmAudioProperties[node].fd,
                USB_SPKR_IOCTL_GET_CHANNEL_CONFIG,
                arg);
            break;

        case USB_BRCM_IOCTL_GET_CHANNEL_CONFIG_MIC:
            stat = ioctl(usbBrcmAudioProperties[node].fd,
                USB_SPKR_IOCTL_GET_CHANNEL_CONFIG_MIC,
                arg);
            break;

        case USB_BRCM_IOCTL_GET_CHANNEL_CAPS:
            stat = ioctl(usbBrcmAudioProperties[node].fd,
                USB_SPKR_IOCTL_GET_CHANNEL_CAPS,
                arg);
            break;

        case USB_BRCM_IOCTL_GET_CHANNEL_CAPS_MIC:
            stat = ioctl(usbBrcmAudioProperties[node].fd,
                USB_SPKR_IOCTL_GET_CHANNEL_CAPS_MIC,
                arg);
            break;

        case USB_BRCM_IOCTL_SET_MUTE:
            stat = ioctl(usbBrcmAudioProperties[node].fd,
                USB_SPKR_IOCTL_SET_MUTE,
                arg);
            break;

        case USB_BRCM_IOCTL_SET_MUTE_MIC:
            stat = ioctl(usbBrcmAudioProperties[node].fd,
                USB_SPKR_IOCTL_SET_MUTE_MIC,
                arg);
            break;

        case USB_BRCM_IOCTL_SET_VOLUME:
            stat = ioctl(usbBrcmAudioProperties[node].fd,
                USB_SPKR_IOCTL_SET_VOLUME,
                arg);
            break;

        case USB_BRCM_IOCTL_SET_VOLUME_MIC:
            stat = ioctl(usbBrcmAudioProperties[node].fd,
                USB_SPKR_IOCTL_SET_VOLUME_MIC,
                arg);
            break;

        case USB_BRCM_IOCTL_SET_SPKR_MONO2STEREO:
            usbBrcmAudioProperties[node].spkrMono2Stereo = *((BOOL *)(arg));
            break;

        case USB_BRCM_IOCTL_SET_MIC_STEREO2MONO:
            usbBrcmAudioProperties[node].micStereo2Mono = *((BOOL *)(arg));
            break;

        case USB_BRCM_IOCTL_GET_DEV_INFO:
            stat = ioctl(usbBrcmAudioProperties[node].fd,
                USB_SPKR_IOCTL_GET_DEV_INFO,
                arg);
            break;

        case USB_BRCM_IOCTL_SET_HID_CALLBACK:
            usbBrcmAudioProperties[node].hidCallback.arg = node;
            usbBrcmAudioProperties[node].hidCallback.function =
            (USB_SPKR_HID_CALLBACK)(arg);
            stat = ioctl(usbBrcmAudioProperties[node].fd,
                USB_SPKR_IOCTL_SET_HID_CALLBACK,
                &usbBrcmAudioProperties[node].hidCallback);
            break;

        case USB_BRCM_IOCTL_GET_HID_REPORT:
            stat = ioctl(usbBrcmAudioProperties[node].fd,
                USB_SPKR_IOCTL_GET_HID_REPORT,
                arg);
            break;

        case USB_BRCM_IOCTL_SET_HID_REPORT:
            stat = ioctl(usbBrcmAudioProperties[node].fd,
                USB_SPKR_IOCTL_SET_HID_REPORT,
                arg);
            break;

        case USB_BRCM_IOCTL_WRITE_HID_REPORT:
            stat = ioctl(usbBrcmAudioProperties[node].fd,
                USB_SPKR_IOCTL_WRITE_HID_REPORT,
                arg);
            break;

        case USB_BRCM_IOCTL_READ_HID_REPORT:
            stat = ioctl(usbBrcmAudioProperties[node].fd,
                USB_SPKR_IOCTL_READ_HID_REPORT,
                arg);
            break;


        default:
            semGive(usbBrcmAudioMutex);
            return USB_BRCM_ERROR;
        }

    semGive(usbBrcmAudioMutex);
    return (stat);
    }

/*************************************************************************
*
* usbBrcmAudioSpkrVolume - displays or sets speaker volume
*
* If level is -1, current, minimum, and maximum volume settings are printed.
* If level is -2, sets volume to maximum.
* If level is -3, sets volume to minimum.
* Otherwise, sets volume to desired level.
*
* RETURNS: none
*
* ERRNO: none
*
 **************************************************************************/

LOCAL VOID usbBrcmAudioSpkrVolume
    (
    INT32 level
    )

    {
    int fd;
    short volSetting;
    UINT16 channels;
    pUSB_SPKR_CHANNEL_CAPS pCaps;
    UINT16 i;

    /* verify a speaker is available */
    if ((fd=usbBrcmAudioGetActiveNode()) == USB_BRCM_ERROR)
        {
        USB1_SPK_ERR("Speaker currently not attached or unavailable \n",
                     1, 2, 3, 4, 5, 6);
        return;
        }

    /* Try to get channel information. */
    if (usbBrcmAudioIoctl(fd,
            USB_BRCM_IOCTL_GET_CHANNEL_COUNT,
            &channels) !=OK)
        {
        USB1_SPK_ERR("IOCTL GET_CHANNEL_COUNT returned ERROR \n",
                     1, 2, 3, 4, 5, 6);
        return;
        }

    /* Get headset speaker capabilities */
    if (usbBrcmAudioIoctl(fd,
            USB_BRCM_IOCTL_GET_CHANNEL_CAPS,
            &pCaps) !=OK)
        {
        USB1_SPK_ERR("IOCTL GET_CHANNEL_CAPS returned ERROR \n",
                     1, 2, 3, 4, 5, 6);
        return;
        }

    /* If volume specified, then set it, else display current volume info */

    for (i = 0; i <= channels; i++)
        {
        if (!pCaps [i].volume.supported)
            {
            USB1_SPK_INFO("usbBrcmAudioSpkrVolume: Channel = %d Volume not "
                          "supported.\n", i, 2, 3, 4, 5, 6);
            }
        else
            {
            if (level == -1)
                {
                /* Show volume settings. */
                USB1_SPK_INFO("Volume: res = %hx, min = %hx, max ="
                    " %hx, cur = %hx.\n",
                    pCaps [i].volume.res, pCaps [i].volume.min,
                    pCaps [i].volume.max, pCaps [i].volume.cur,
                    5, 6);
                }
            else
                {
                switch (level)
                    {
                    case -3:
                        /* set to minimum */
                        volSetting = (short) (pCaps [i].volume.min & 0xffff);
                        break;

                    case -2:
                        /* set to maximum */
                        volSetting = (short) (pCaps [i].volume.max & 0xffff);
                        break;

                    default:
                        volSetting = (short) (level & 0xffff);
                        break;
                    }

                /* Set volume */
                if (usbBrcmAudioIoctl(fd,
                        USB_BRCM_IOCTL_SET_VOLUME,
                        (pVOID)(ULONG)((i << 16) | ((UINT16) volSetting))) == OK)
                    {
                    USB1_SPK_INFO("usbBrcmAudioSpkrVolume: Channel = %d "
                                  "Volume set to %hx.\n",
                                  i, volSetting, 3, 4, 5, 6);
                    }
                else
                    {
                    USB1_SPK_ERR("usbBrcmAudioSpkrVolume: "
                                 "Channel = %d Error setting volume \n",
                                 1, 2, i, 4, 5, 6);
                    }

                }
            }
        }
    return;
    }

/*************************************************************************
*
* usbBrcmAudioMicMute - displays and sets microphone mute setting
*
* unmutes microphone if level is zero.
* mutes microphone if level is one.
*
* RETURNS: none
*
* ERRNO: none
*
**************************************************************************/

VOID usbBrcmAudioMicMute
    (
    BOOL level
    )

    {
    int fd;
    short muteSetting;
    UINT16 channels;
    pUSB_SPKR_CHANNEL_CAPS pCaps;
    UINT16 i;

    /* verify a speaker is available */
    if ((fd=usbBrcmAudioGetActiveNode()) == USB_BRCM_ERROR)
        {
        USB1_SPK_ERR("Speaker currently not attached or unavailable \n",
                     1, 2, 3, 4, 5, 6);
        return;
        }

    /* Try to get channel information. */
    if (usbBrcmAudioIoctl(fd,
            USB_BRCM_IOCTL_GET_CHANNEL_COUNT,
            &channels) !=OK)
        {
        USB1_SPK_ERR("IOCTL GET_CHANNEL_COUNT returned ERROR \n",
                     1, 2, 3, 4, 5, 6);
        return;
        }

    /* Get headset speaker capabilities */
    if (usbBrcmAudioIoctl(fd,
            USB_BRCM_IOCTL_GET_CHANNEL_CAPS,
            &pCaps) !=OK)
        {
        USB1_SPK_ERR("IOCTL GET_CHANNEL_CAPS returned ERROR \n",
                     1, 2, 3, 4, 5, 6);
        return;
        }

    /* If mute specified, then set it, else display current mute info */

    for (i = 0; i <= channels; i++)
        {
        if (!pCaps [i].mute.supported)
            {
            USB1_SPK_INFO("usbBrcmAudioMicMute: usbBrcmAudioMicMute: "
                          "Channel = %d Mute not supported.\n",
                          i, 2, 3, 4, 5, 6);
            }
        else
            {
            muteSetting = (short) (level & 0xffff);

            /* Set mute */
            if (usbBrcmAudioIoctl(fd,
                    USB_BRCM_IOCTL_SET_MUTE_MIC,
                    (pVOID)(ULONG)((i << 16) | ((UINT16) muteSetting))) == OK)
                {
                USB1_SPK_INFO("sbBrcmAudioMicMute: usbBrcmAudioMicMute: "
                              "Channel = %d Mute set to %hx.\n",
                              i, muteSetting, 3, 4, 5, 6);
                }
            else
                {
                USB1_SPK_ERR("sbBrcmAudioMicMute: usbBrcmAudioMicMute:"
                             " Channel = %d Error setting mute \n",
                             i, 2, 3, 4, 5, 6);
                }
            }   /* Endif */
        }   /* END for */
    return;
    }

/*************************************************************************
*
* usbBrcmAudioFormatListPrint - displays channel format information
*
* RETURNS: none
*
* ERRNO: none
*
*/

LOCAL VOID usbBrcmAudioFormatListPrint
    (
    char *                 header, /* description string */
    UINT16                 count,  /* number of formats */
    pUSB_SPKR_AUDIO_FORMAT pFmt    /* formats */
    )
    {
    int i, j;

    printf("==%s: %02d formats========\n", header, count);

    for (i = 0; i < count; i++)
        {
        printf("  --- %02d --------------------\n", i);
        printf("  interface         = 0x%x\n", pFmt[i].interface);
        printf("  altSetting        = 0x%x\n", pFmt[i].altSetting);
        printf("  endpoint          = 0x%x\n", pFmt[i].endpoint);
        printf("  maxPacketSize     = 0x%x\n", pFmt[i].maxPacketSize);
        printf("  attributes        = 0x%x\n", pFmt[i].attributes);
        printf("  formatTag         = 0x%x\n", pFmt[i].formatTag);
        switch (pFmt[i].formatType)
            {
            case USB_AUDIO_FORMAT_TYPE1:
                printf("  formatType        = 0x%x (Type I)\n",
                    pFmt[i].formatType);
                printf("    channels        = 0x%x\n",
                    pFmt[i].channels);
                printf("    subFrameSize    = 0x%x\n",
                    pFmt[i].subFrameSize);
                printf("    bitRes          = 0x%x\n",
                    pFmt[i].bitRes);
                printf("    SampleFrequency = 0x%x\n",
                     pFmt[i].sampleFrequency);
                break;

            case USB_AUDIO_FORMAT_TYPE2:
                printf("  formatType        = 0x%x (Type II)\n",
                    pFmt[i].formatType);
                printf("    maxBitRate      = 0x%x\n",
                    pFmt[i].maxBitRate);
                printf("    samplesPerFrame = 0x%x\n",
                    pFmt[i].samplesPerFrame);
                break;

            case USB_AUDIO_FORMAT_TYPE3:
                printf("  formatType        = 0x%x (Type III)\n",
                    pFmt[i].formatType);
                printf("    channels        = 0x%x\n",
                    pFmt[i].channels);
                printf("    subFrameSize    = 0x%x\n",
                    pFmt[i].subFrameSize);
                printf("    bitRes          = 0x%x\n",
                    pFmt[i].bitRes);
                printf("    SampleFrequency = 0x%x\n",
                    pFmt[i].sampleFrequency);
                break;
            }

        if (pFmt[i].freqType == 0)
            {
            printf("    freqType        = 0x%x (Continuous)\n",
                pFmt[i].freqType);
            printf("      lowerFreq     = 0x%x\n",
                pFmt[i].lowerFreq);
            printf("      upperFreq     = 0x%x\n",
                pFmt[i].upperFreq);
            }
        else
            {
            printf("    freqType        = 0x%x (Discrete)\n",
                pFmt[i].freqType);
            for (j=0; j<pFmt[i].freqType; j++) {
                printf("      freq      = 0x%x (%d)\n", pFmt[i].freq[j],
                    pFmt[i].freq[j]);
            }
        }
    }
}

/*************************************************************************
*
* usbBrcmAudioFormatListShow - Show audio format list
*
* displays available  headset speaker and microphone format information
*
* RETURNS: none
*
* ERRNO: none
*
*/

VOID usbBrcmAudioFormatListShow( void )
    {
    int fd;

    /* verify a speaker is available */
    if ((fd = usbBrcmAudioGetActiveNode()) == USB_BRCM_ERROR)
        {
        USB1_SPK_ERR("Speaker currently not attached or unavailable \n",
                     1, 2, 3, 4, 5, 6);
        return;
        }

    usbBrcmAudioFormatListPrint("Speaker   ",
        usbBrcmAudioProperties[fd].fmtCount,
        usbBrcmAudioProperties[fd].pFmt);
    usbBrcmAudioFormatListPrint("Microphone",
        usbBrcmAudioProperties[fd].micFmtCount,
        usbBrcmAudioProperties[fd].pMicFmt);

    return;
    }

/*************************************************************************
*
* usbBrcmAudioInfoShow - displays manufacturer and model info
*
* This displays the configuration header info on stdout.
*
* RETURNS: none
*
* ERRNO: none
*
*/

VOID usbBrcmAudioInfoShow( void )
    {
    int fd;
    pUSB_SPKR_DEV_INFO_TYPE pDevInfo;

    /* verify a speaker is available */
    if ((fd=usbBrcmAudioGetActiveNode()) == USB_BRCM_ERROR)
        {
        USB1_SPK_ERR("Speaker currently not attached or unavailable \n",
                     1, 2, 3, 4, 5, 6);
        return;
        }

    /* Try to get channel information. */
    if (usbBrcmAudioIoctl(fd,
            USB_BRCM_IOCTL_GET_DEV_INFO,
            &pDevInfo) !=OK)
        {
        USB1_SPK_ERR("IOCTL_GET_DEV_INFO returned ERROR \n",
                     1, 2, 3, 4, 5, 6);
        return;
        }

    printf("Vendor ID    = 0x%04x\n", pDevInfo->vendorId);
    printf("Product ID   = 0x%04x\n", pDevInfo->productId);
    printf("Manufacturer = '%s'\n", pDevInfo->manufacturerName);
    printf("Product      = '%s'\n", pDevInfo->productName);
    }

/*************************************************************************
 *
 * usbBrcmAudioOpen - opens headset named device
 *
 * USB headset driver start getting/sending samples.
 * Should be called only once, during application startup.
 *
 * RETURNS: device node or USB_BRCM_ERROR if error
 *
 * ERRNO: none
 *
 */

LOCAL int usbBrcmAudioOpen
    (
    int   fd,                /* not used */
    char *devp,              /* device name such as /usbSp/0 */
    int   flags              /* O_RDONLY, O_WRONLY, O_RDWR, or O_CREAT */
    )
    {
    int fileDescr;

    if (usbBrcmAudioInit() == USB_BRCM_ERROR)
        {
        USB1_SPK_ERR("initialization ERROR \n",
                     1, 2, 3, 4, 5, 6);
        return USB_BRCM_ERROR;
        }

    if (semTake(usbBrcmAudioMutex, WAIT_FOREVER) != OK)
        {
        return USB_BRCM_ERROR;
        }

    if (devp == NULL)
        {
        USB1_SPK_ERR("invalid parameter of devp \n",
                    1, 2, 3, 4, 5, 6);
        semGive(usbBrcmAudioMutex);
        return USB_BRCM_ERROR;
        }

    if ((fileDescr = usbBrcmAudioChannelsFindFree(devp)) == USB_BRCM_ERROR)
        {
        USB1_SPK_ERR("no more free channels ERROR \n",
                     1, 2, 3, 4, 5, 6);
        semGive(usbBrcmAudioMutex);
        return USB_BRCM_ERROR;
        }

    if (usbBrcmAudioProperties[fileDescr].inUse == FALSE)
        {
        usbBrcmAudioProperties[fileDescr].inUse = TRUE;
        if (usbBrcmAudioProperties[fileDescr].dynAttCallback != NULL)
            {
            strcpy(usbBrcmAudioProperties[fileDescr].spkrName, devp);
            usbBrcmAudioProperties[fileDescr].flags = flags;
            if (usbSpeakerDynamicAttachRegister(
                    usbBrcmAudioProperties[fileDescr].dynAttCallback,
                    (pVOID)(ULONG) fileDescr) != OK)
                {
                USB1_SPK_ERR("usbSpeakerDynamicAttachRegister() " \
                             "returned ERROR \n",
                             1, 2, 3, 4, 5, 6);
                usbBrcmAudioChannelsInitializeNode(fileDescr);
                semGive(usbBrcmAudioMutex);
                return USB_BRCM_ERROR;
                }
            }
        usbBrcmAudioSetActiveNode(fileDescr);
        }
    semGive(usbBrcmAudioMutex);
    return fileDescr;
    }

/*************************************************************************
 *
 * usbBrcmAudioClose - closes USB headset driver
 *
 * RETURNS: USB_BRCM_OK if ok, or USB_BRCM_ERROR if error
 *
 * ERRNO: none
 *
 */

LOCAL STATUS usbBrcmAudioClose
    (
    int fd                           /* file descriptor */
    )
    {
    int status=USB_BRCM_OK;
    int node;
    if (semTake(usbBrcmAudioMutex, WAIT_FOREVER) != OK)
        {
        return USB_BRCM_ERROR;
        }

    node = usbBrcmAudioGetActiveNode();
    if (usbBrcmAudioProperties[fd].inUse == TRUE)
        {
        /* Turn off watchdog */
        usbBrcmAudioIoctl(node,USB_BRCM_IOCTL_FUNC_RX_STOP,NULL);

        usbBrcmAudioProperties[fd].rdWrtActive = FALSE;
        usbBrcmAudioSetActiveNode(USB_BRCM_ERROR);

        if (usbBrcmAudioProperties[fd].dynAttCallback != NULL)
            {
            if (usbSpeakerDynamicAttachUnRegister(
                    usbBrcmAudioProperties[fd].dynAttCallback, (pVOID)(ULONG)fd)
                        != OK)
                {
                status = USB_BRCM_ERROR;
                }
            }

        usbBrcmAudioDetach(fd);

        if (usbBrcmAudioProperties[fd].pSeqDev != (SEQ_DEV *) NULL)
            {
            usbSpeakerSeqDevUnlock (usbBrcmAudioProperties[fd].pSeqDev);
            usbBrcmAudioProperties[fd].pSeqDev = (SEQ_DEV *) NULL;
            }

        if (usbBrcmAudioProperties[fd].spkrConversionBuf != NULL)
            {
            free(usbBrcmAudioProperties[fd].spkrConversionBuf);
            usbBrcmAudioProperties[fd].spkrConversionBuf = NULL;
            }

        if (usbBrcmAudioProperties[fd].spkrConversionBuf != NULL)
            {
            free(usbBrcmAudioProperties[fd].micConversionBuf);
            usbBrcmAudioProperties[fd].micConversionBuf = NULL;
            }

        usbBrcmAudioChannelsInitializeNode(fd);
        }
    else
        {
        status = USB_BRCM_ERROR;
        }

    semGive(usbBrcmAudioMutex);
    return status;
    }

/*************************************************************************
 *
 * usbBrcmAudioRead - Read from microphone
 *
 * RETURNS: bytes read, or USB_BRCM_ERROR if error
 *
 * ERRNO: none
 *
 */

LOCAL int usbBrcmAudioRead
    (
    int   node,                         /* local node */
    char* bufp,                         /* buffer to store data */
    int   len                           /* length of data to be read */
    )
    {
    int count=USB_BRCM_ERROR;
    int i;
    int sampleCount;
    char *ptrS;
    char *ptrD;

#ifdef INCLUDE_WINDVIEW
    wvEvent(3, NULL, 0);
#endif
    USB1_SPK_DBG("%s: entered for len %d \n",
                 __FUNCTION__, len, 3, 4, 5, 6);
    if (usbBrcmAudioProperties[node].rdWrtActive == FALSE)
        {
        USB1_SPK_ERR("rdWrt Not active \n",
                     1, 2, 3, 4, 5, 6);
        bzero(bufp, len);
        return count;
        }

    if (semTake(usbBrcmAudioMutex, 1) !=OK)
        {
        USB1_SPK_ERR("Mutex fault \n",
                     1, 2, 3, 4, 5, 6);
        bzero(bufp, len);
        return count;
        }

    if (usbBrcmAudioProperties[node].rdWrtActive == FALSE)
        {
        USB1_SPK_ERR("Mutex with read/write active \n",
                     1, 2, 3, 4, 5, 6);
        bzero(bufp, len);
        semGive(usbBrcmAudioMutex);
        return USB_BRCM_ERROR;
        }

    if (usbBrcmAudioProperties[node].fd != ERROR)
        {
        USB1_SPK_DBG("%s: fd not in error \n",
                     __FUNCTION__, 2, 3, 4, 5, 6);
        if (usbBrcmAudioProperties[node].micStereo2Mono == TRUE)
            {
            USB1_SPK_DBG("%s: MicStero2Mono is true \n",
                         __FUNCTION__, 2, 3, 4, 5, 6);
            if (usbBrcmAudioProperties[node].desiredMicFmt.subFrameSize==0)
                {
                bzero(bufp, len);
                semGive(usbBrcmAudioMutex);
                USB1_SPK_ERR("SubframeSize is zero \n",
                             1, 2, 3, 4, 5, 6);
                return USB_BRCM_ERROR;
                }
            if (usbBrcmAudioProperties[node].micConversionBuf == NULL)
                {
                if ((usbBrcmAudioProperties[node].micConversionBuf=
                                malloc (len * 2)) == NULL)
                    {
                    semGive(usbBrcmAudioMutex);
                    USB1_SPK_ERR("malloc error \n",
                                 1, 2, 3, 4, 5, 6);
                    return USB_BRCM_ERROR;
                    }
                usbBrcmAudioProperties[node].micConversionBufSize = len * 2;
                }
            else if (usbBrcmAudioProperties[node].micConversionBufSize
                < (len * 2))
                {
                if ((usbBrcmAudioProperties[node].micConversionBuf = realloc (
                         usbBrcmAudioProperties[node].micConversionBuf, len*2))
                    == NULL)
                    {
                    semGive(usbBrcmAudioMutex);
                    USB1_SPK_ERR("realloc error \n",
                                 1, 2, 3, 4, 5, 6);
                    return USB_BRCM_ERROR;
                    }
                usbBrcmAudioProperties[node].micConversionBufSize = len * 2;
                }
            USB1_SPK_DBG("%s: Reading into the conversion buffer \n",
                         __FUNCTION__, 2, 3, 4, 5, 6);
            if ((count=read(usbBrcmAudioProperties[node].fd,
                        usbBrcmAudioProperties[node].micConversionBuf, len*2))
                        != (len*2))
                {
                bzero(bufp, len);
                semGive(usbBrcmAudioMutex);
                USB1_SPK_ERR("Microphone Conversion Buffer error \n",
                             1, 2, 3, 4, 5, 6);
                return USB_BRCM_ERROR;
                }
            ptrS = usbBrcmAudioProperties[node].micConversionBuf;
            ptrD = bufp;
            sampleCount = len /
                usbBrcmAudioProperties[node].desiredMicFmt.subFrameSize;
            for (i=0; i<sampleCount; i++)
                {
                memcpy(ptrD, ptrS,
                    usbBrcmAudioProperties[node].desiredMicFmt.subFrameSize);
                ptrD += usbBrcmAudioProperties[node].desiredMicFmt.subFrameSize;
                ptrS +=
                    usbBrcmAudioProperties[node].desiredMicFmt.subFrameSize * 2;
                }
            count = len;
            USB1_SPK_DBG("%s: Standard exit \n",
                         __FUNCTION__, 2, 3, 4, 5, 6);
            }
        else if ((count=read(usbBrcmAudioProperties[node].fd, bufp, len))
                != len)
            {
            USB1_SPK_DBG("%s: Speaker read function called \n",
                         __FUNCTION__, 2, 3, 4, 5, 6);
            if (count < 0)
                {
                int retrys=10;
                do
                    {
                    retrys--;
                    USB1_SPK_WARN("%s[L%d]: usbBrcmAudioRead:  Incomplete " \
                                  "read - retrying %d \n",
                                  __FILE__, __LINE__, retrys, 4, 5, 6);
                    /*
                     * A -1 indicates there is no data available.  Pause a bit
                     * and give the usb stack a chance to fill the buffer
                     */
                    taskDelay((usbBrcmAudioProperties[node].ticks/2 > 0) ?
                        usbBrcmAudioProperties[node].ticks/2 :
                        usbBrcmAudioProperties[node].ticks);
                    count=read(usbBrcmAudioProperties[node].fd, bufp, len);
                    } while (count < 0 && retrys > 0);
                }
            }
        }
    else
        {
        USB1_SPK_ERR("Node in error state \n",
                     1, 2, 3, 4, 5, 6);
        bzero(bufp, len);
        }

    semGive(usbBrcmAudioMutex);
    return count;
    }

/*************************************************************************
*
* usbBrcmAudioWrite - Write to Speaker
*
* Performs a sequential write to the speaker.
*
* RETURNS: bytes written, or USB_BRCM_ERROR if error
*
*  ERRNO: none
*
*/

LOCAL int usbBrcmAudioWrite
    (
    int   fd,                           /* file descriptor */
    char* bufp,                         /* data to be written */
    int   len                           /* length of data to be written */
    )
    {
    int count=USB_BRCM_ERROR;
    int sampleCount;
    int i;
    char *ptrS;
    char *ptrD;

#ifdef INCLUDE_WINDVIEW
    wvEvent(4, NULL, 0);
#endif
    if (usbBrcmAudioProperties[fd].rdWrtActive == FALSE)
        {
        return count;
        }

    if (semTake(usbBrcmAudioMutex, 1) !=OK )
        {
        return count;
        }

    if (usbBrcmAudioProperties[fd].rdWrtActive == FALSE)
        {
        semGive(usbBrcmAudioMutex);
        return USB_BRCM_ERROR;
        }
    USB1_SPK_DBG("%s: Valid fd \n", __FUNCTION__, 2, 3, 4, 5, 6);
    if (usbBrcmAudioProperties[fd].fd != ERROR)
        {
        if (usbBrcmAudioProperties[fd].spkrMono2Stereo == TRUE)
            {
            if (usbBrcmAudioProperties[fd].desiredFmt.subFrameSize == 0)
                {
                semGive(usbBrcmAudioMutex);
                return USB_BRCM_ERROR;
                }

            if (usbBrcmAudioProperties[fd].spkrConversionBuf == NULL)
                {
                if ((usbBrcmAudioProperties[fd].spkrConversionBuf=
                                            malloc (len * 2)) == NULL)
                    {
                    semGive(usbBrcmAudioMutex);
                    return USB_BRCM_ERROR;
                    }
                usbBrcmAudioProperties[fd].spkrConversionBufSize = len * 2;
                }
            else if (usbBrcmAudioProperties[fd].spkrConversionBufSize
                < (len * 2))
                {
                if ((usbBrcmAudioProperties[fd].spkrConversionBuf = realloc
                    (usbBrcmAudioProperties[fd].spkrConversionBuf, len*2))
                    == NULL)
                    {
                    semGive(usbBrcmAudioMutex);
                    return USB_BRCM_ERROR;
                    }
                usbBrcmAudioProperties[fd].spkrConversionBufSize = len * 2;
                }
            ptrD = usbBrcmAudioProperties[fd].spkrConversionBuf;
            ptrS = bufp;
            sampleCount = len /
                    usbBrcmAudioProperties[fd].desiredFmt.subFrameSize;

            for (i = 0; i < sampleCount; i++)
                {
                memcpy(ptrD, ptrS,
                    usbBrcmAudioProperties[fd].desiredFmt.subFrameSize);
                ptrD += usbBrcmAudioProperties[fd].desiredFmt.subFrameSize;
                memcpy(ptrD, ptrS,
                    usbBrcmAudioProperties[fd].desiredFmt.subFrameSize);
                ptrD += usbBrcmAudioProperties[fd].desiredFmt.subFrameSize;
                ptrS += usbBrcmAudioProperties[fd].desiredFmt.subFrameSize;
                }
            if ((count=write(usbBrcmAudioProperties[fd].fd,
                        usbBrcmAudioProperties[fd].spkrConversionBuf,
                        len*2)) != (len*2))
                {
                count = USB_BRCM_ERROR;
                }
            else
                {
                count = len;
                }
            }
        else
            {
            int bytesWritten=0;
            int bytesLeftToWrite = len;
            do
                {
                USB1_SPK_DBG("%s: Attempting to write %d bytes \n",
                             __FUNCTION__, bytesLeftToWrite, 3, 4, 5, 6);
                count = write(usbBrcmAudioProperties[fd].fd,
                    &bufp[bytesWritten], bytesLeftToWrite);
                USB1_SPK_DBG("%s: Wrote %d bytes \n",
                             __FUNCTION__, count, 3, 4, 5, 6);
                if (count > 0)
                    {
                    bytesWritten += count;
                    bytesLeftToWrite -= count;
                    if (bytesLeftToWrite > 0)
                        USB1_SPK_DBG("%s: Write %d bytes - retry for %d bytes "
                                     "of %d for a total of %d bytes written \n",
                                     __FUNCTION__, count,
                                     bytesLeftToWrite, len, bytesWritten, 6);
                    }
                else
                    {
                    /*
                     * A -1 return usually means that the sender has
                     * filled the buffers.  Retry it.  But first pause
                     * and give the stack a chance to clear the
                     * buffers.  Note - this could cause a break in the
                     * audio.
                     */
                    USB1_SPK_DBG("%s: Task delaying on full buffers \n",
                                 __FUNCTION__, 2, 3, 4, 5, 6);
                    taskDelay((usbBrcmAudioProperties[fd].ticks/2 > 0) ?
                             usbBrcmAudioProperties[fd].ticks/2 :
                             usbBrcmAudioProperties[fd].ticks);
                    }
                } while (bytesLeftToWrite > 0);

            }

        }

    semGive(usbBrcmAudioMutex);
    return count;
    }

/*************************************************************************
 **************************************************************************
 **************************************************************************
 *
 * The following section contains sample/test/stub functions.
 *
 **************************************************************************
 **************************************************************************
 **************************************************************************
 * \NOMANUAL
 */
#ifdef INCLUDE_USB_HEADSET_DEMO
#define USB_BRCM_DEBUG_CHANNELS       1
#define USB_BRCM_DEBUG_SUBFRAMESIZE   2     /* bytes */
#define USB_BRCM_DEBUG_RESOLUTION     16    /* bits  */
#define USB_BRCM_DEBUG_FREQUENCY 8000
LOCAL char *usbBrcmAudioBfr = NULL;
int usbBrcmAudioBfrLineSize;
int usbBrcmAudioBfrLineCount;
int usbBrcmAudioSampleSize;
int usbBrcmAudioBytesPerSec;
UINT32 usbBrcmAudioWavDataLen;

/*************************************************************************
*
* usbBrcmAudioUsrAttachCallback - sample callback function to be registered
*                                 by user.  relays attach/detach information
*                                 as well as headset speaker and microhpone
*                                 format information.  Note that this is
*                                 invoked by the callback routine above.
*
* RETURNS: none
*
* ERRNO: none
*
* \NOMANUAL
*/
LOCAL void usbBrcmAudioUsrAttachCallback
    (
    UINT16                 attachCode,  /* defined as USB_SPKR_xxxx */
    UINT16                 fmtCount,    /* headset speaker format count */
    pUSB_SPKR_AUDIO_FORMAT pFmt,        /* headset speaker formats */
    UINT16                 micFmtCount, /* headset microphone format count */
    pUSB_SPKR_AUDIO_FORMAT pMicFmt      /* headset microphone formats */
    )
    {
    int status;

    if (attachCode == USB_SPKR_ATTACH)
        {
        printf("ATTACH code=%p fmtCount=%d "
               "pFmt=%p micFmtCount=%d pMicFmt=%p\n",
               attachCode, fmtCount, pFmt, micFmtCount, pMicFmt);
        /*
         * The formats are passed in.  Call the internal ioctl that sets the
         * formats.  Make sure formats are defined.
         */
        if (pFmt != NULL)
            {
            /* speaker */
            printf("usbBrcmAudioUsrAttachCallback:  "
                   "Speaker Format ioctl format tag - 0x%x format type - 0x%x"
                   " format channels - 0x%x  format SubFrameSize - 0x%x  BitRes - 0x%x"
                   " Sample Frequency - 0x%x\n",pFmt->formatTag, pFmt->formatType,
                   pFmt->channels, pFmt->subFrameSize, pFmt->bitRes,
                   pFmt->sampleFrequency);

            status = usbBrcmAudioIoctl(usbBrcmAudioGetActiveNode(),
                USB_BRCM_IOCTL_SET_AUDIO_FORMAT,
                pFmt);

            printf("usbBrcmAudioUsrAttachCallback: Status of speaker IOCTL"
                   " - 0x%x\n",status);
           }
        else
           {
            printf("usbBrcmAudioUsrAttachCallback:  ERROR Speaker format not "
                   "set in %s\n",__FILE__);
           }
        }
    else if (attachCode == USB_SPKR_REMOVE)
        {
        printf("REMOVE code=%p fmtCount=%d "
               "pFmt=%p micFmtCount=%d pMicFmt=%p\n",
               attachCode, fmtCount, pFmt, micFmtCount, pMicFmt);

        usbBrcmAudioClose (usbBrcmAudioGetActiveNode());

        /* if the audio buffer is not NULL we should free it for latter use */
        if (usbBrcmAudioBfr != NULL)
            {
            free(usbBrcmAudioBfr);
            usbBrcmAudioBfr = NULL;
            }
        }
    else
        {
        printf("UNKNOWN code=%p fmtCount=%d "
               "pFmt=%p micFmtCount=%d pMicFmt=%p\n",
               attachCode, fmtCount, pFmt, micFmtCount, pMicFmt);
        }
    }

/*************************************************************************
*
* usbBrcmAudioCfgCallback - temporary stub function to be registered
*                           for setting sampling frequency on external hw.
*                           Registered with USB_BRCM_IOCTL_FUNC_CFG_SET
*                           ioctl call.
*
* RETURNS:
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL int usbBrcmAudioCfgCallback
    (
    usbBrcmHalCfg configData          /* sampling frequency structure */
    )
    {
    printf("usbBrcmAudioCfgCallback: sampFreq=%d \n", configData.sampFreq);

    return USB_BRCM_OK;
    }


/*************************************************************************
*
* usbBrcmAudioDisplayRpt - Displays report
*
* RETURNS:
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL void usbBrcmAudioDisplayRpt
    (
    int    fd,     /* file descriptor */
    char  *header, /* 3 character header */
    UINT16 length, /* length of report */
    pUINT8 pReport /* report */
    )
    {
    char str[200];
    int offset;
    int byteOnLine;
    int curByte;

    sprintf(str, "usbBrcmAudioDisplayRpt: HID %s(%03d):", header, length);
    offset = strlen(str);
    byteOnLine = 0;

    for (curByte = 0; curByte < length; curByte++)
        {
        sprintf(str+offset, " %02x", pReport[curByte]);
        byteOnLine++;
        offset+=3;

        if (byteOnLine == 20)
            {
            str[offset] = '\0';
            printf("%s\n", str);
            if (curByte < length)
                {
                sprintf(str, "            :");
                offset = strlen(str);
                }
            byteOnLine=0;
            }
        }

    if (byteOnLine<20)
        {
        str[offset] = '\0';
        printf("%s\n", str);
        }
    }

/*************************************************************************
*
* usbBrcmAudioHidCallback - Asynchronous HID report receive callback
*
* RETURNS:
*
* ERRNO: none
*
* \NOMANUAL
*/

LOCAL int usbBrcmAudioHidCallback
    (
    int    fd,     /* file descriptor */
    UINT16 length, /* length of report */
    pUINT8 pReport /* HID report */
    )
    {
    usbBrcmAudioDisplayRpt(fd, "usbBrcmAudioDisplayRpt:  CB ", length, pReport);

    return USB_BRCM_OK;
    }


/*************************************************************************
*
* usbBrcmAudioReadHid - Asynchronous HID report receive callback
*
* Performs an IOCTL to get the HID report.
*
* RETURNS:
*
* ERRNO: none
*
* \NOMANUAL
*/

void usbBrcmAudioReadHid(void)
    {
    int fd;
    pUSB_SPKR_HID_READ_REPORT_TYPE pRpt;

    /* verify a speaker is available */
    if ((fd=usbBrcmAudioGetActiveNode()) == USB_BRCM_ERROR)
        {
        printf("usbBrcmAudioReadHid: Speaker currently not attached or "
        "  unavailable\n");
        return;
        }

    if (usbBrcmAudioIoctl(fd,
            USB_BRCM_IOCTL_READ_HID_REPORT,
            (&pRpt)) == OK)
        {
        usbBrcmAudioDisplayRpt(fd, "RD ", pRpt->length, pRpt->pReport);
        }
    else
        {
        printf("usbBrcmAudioReadHid: Error getting report.\n");
        }
    }


/*************************************************************************
*
* usbBrcmAudioCS50GetHid - Sample function
*
* Sample function that gets the HID report.
*
* RETURNS: none
*
* ERRNO: none
*
*/

void usbBrcmAudioCS50GetHid(void)
    {
    int fd;
    pUSB_SPKR_DEV_INFO_TYPE pDevInfo;
    USB_SPKR_HID_GET_REPORT_TYPE rpt;

    /* verify a speaker is available */
    if ((fd=usbBrcmAudioGetActiveNode()) == USB_BRCM_ERROR)
        {
        printf("usbBrcmAudioCS50GetHid: Speaker currently not attached or "
               "unavailable\n");
        return;
        }

    if (usbBrcmAudioIoctl(fd,
            USB_BRCM_IOCTL_GET_DEV_INFO,
            &pDevInfo) !=OK)
        {
        printf("usbBrcmAudioCS50GetHid: IOCTL_GET_DEV_INFO returned ERROR.\n");
        return;
        }

    if (pDevInfo->vendorId==0x047f && pDevInfo->productId==0x0410)
        {
        rpt.reportTypeAndId = 0x0301;
        rpt.length = 8;
        if ((rpt.pReport = malloc(rpt.length))==NULL)
            {
            printf("usbBrcmAudioCS50GetHid: malloc Error\n");
            return;
            }
        bzero((char *)rpt.pReport, rpt.length);
        }
    else
        {
        printf("usbBrcmAudioCS50GetHid: Error %s, %s unsupported\n",
            pDevInfo->manufacturerName, pDevInfo->productName);
        return;
        }

    if (usbBrcmAudioIoctl(fd,
            USB_BRCM_IOCTL_GET_HID_REPORT, (&rpt)) == OK)
        {
        usbBrcmAudioDisplayRpt(fd, "GET", rpt.length, rpt.pReport);
        }
    else
        {
        printf("usbBrcmAudioCS50GetHid: Error getting HID report.\n");
        }

    /* Free the report buffer if not NULL */
    if (rpt.pReport)
        {
        free(rpt.pReport);
        }

    return;
    }

/*************************************************************************
*
* usbBrcmAudioCS50SetRfHid - Sample function
*
* Sample function that uses HID Set Report
*
* RETURNS: none
*
* ERRNO: none
*
*/

VOID usbBrcmAudioCS50SetRfHid
    (
    BOOL lock
    )
    {
    int fd;
    pUSB_SPKR_DEV_INFO_TYPE pDevInfo;
    USB_SPKR_HID_GET_REPORT_TYPE rpt;

    /* verify a speaker is available */
    if ((fd=usbBrcmAudioGetActiveNode()) == USB_BRCM_ERROR)
        {
        printf("usbBrcmAudioCS50SetRfLockHid: Speaker currently not attached "
               "or unavailable\n");
        return;
        }

    if (usbBrcmAudioIoctl(fd,
            USB_BRCM_IOCTL_GET_DEV_INFO,
            &pDevInfo) !=OK)
        {
        printf("usbBrcmAudioCS50SetRfLockHid: IOCTL_GET_DEV_INFO returned ERROR.\n");
        return;
        }

    if (pDevInfo->vendorId==0x047f && pDevInfo->productId==0x0410)
        {
        rpt.reportTypeAndId = 0x0301;
        rpt.length = 8;
        if ((rpt.pReport = malloc(rpt.length))==NULL)
            {
            printf("usbBrcmAudioCS50SetRfLockHid: malloc Error\n");
            return;
            }
        bzero((char *)rpt.pReport, rpt.length);
        rpt.pReport[0] = 0x01;
        if (lock == TRUE)
            {
            rpt.pReport[1] = 0x01;
            }
        else
            {
            rpt.pReport[1] = 0x00;
            }
        }
    else
        {
        printf("usbBrcmAudioCS50SetRfLockHid: Error %s, %s unsupported\n",
               pDevInfo->manufacturerName, pDevInfo->productName);
        return;
        }

    if (usbBrcmAudioIoctl(fd,
            USB_BRCM_IOCTL_SET_HID_REPORT,
            (&rpt)) == OK)
        {
        usbBrcmAudioDisplayRpt(fd, "SET", rpt.length, rpt.pReport);
        }
    else
        {
        printf("usbBrcmAudioCS50SetRfLockHid: Error setting RF HID.\n");
        }

    /* Free the report buffer if not NULL */
    if (rpt.pReport)
        {
        free(rpt.pReport);
        }

    return;
    }

/*************************************************************************
*
* usbBrcmAudio9350WriteHid - Sample function that uses HID Write Report
*
*  Sample function that uses HID Write Report.
*
* RETURNS: none
*
* ERRNO: none
*
*/

VOID usbBrcmAudio9350WriteHid(void)
    {
    int fd;
    pUSB_SPKR_DEV_INFO_TYPE pDevInfo;
    USB_SPKR_HID_WRITE_REPORT_TYPE rpt;

    /* verify a speaker is available */
    if ((fd=usbBrcmAudioGetActiveNode()) == USB_BRCM_ERROR)
        {
        printf("usbBrcmAudio9350WriteHid: Speaker currently not attached or "
               "unavailable\n");
        return;
        }

    if (usbBrcmAudioIoctl(fd,
            USB_BRCM_IOCTL_GET_DEV_INFO,
            &pDevInfo) !=OK)
        {
        printf("usbBrcmAudio9350WriteHid: IOCTL_GET_DEV_INFO returned ERROR.\n");
        return;
        }

    if (pDevInfo->vendorId==0x0b0e && pDevInfo->productId==0x9350)
        {
        rpt.length = 2;
        if ((rpt.pReport = malloc(rpt.length))==NULL)
            {
            printf("usbBrcmAudio9350WriteHid: malloc Error\n");
            return;
            }
        bzero((char *)rpt.pReport, rpt.length);
        rpt.pReport[0] = 0x20;
        }
    else
        {
        printf("usbBrcmAudio9350WriteHid: Error %s, %s unsupported\n",
               pDevInfo->manufacturerName, pDevInfo->productName);
        return;
        }

    if (usbBrcmAudioIoctl(fd,
            USB_BRCM_IOCTL_WRITE_HID_REPORT,
            (&rpt)) == OK)
        {
        usbBrcmAudioDisplayRpt(fd, "WRITE", rpt.length, rpt.pReport);
        }
    else
        {
        printf("usbBrcmAudio9350WriteHid: Error setting mute.\n");
        }

    /* Free the report buffer if not NULL */
    if (rpt.pReport)
        {
        free(rpt.pReport);
        }

    return;
    }


/*************************************************************************
*
* usbBrcmAudioTestWrite - sample function that executes every
*                         5 msec to write to speakers
*                         Registered with USB_BRCM_IOCTL_FUNC_RX_SET
*                         ioctl call.
*
* RETURNS: none
*
* ERRNO: none
*
* \NOMANUAL
 */
VOID usbBrcmAudioTestWrite (void)
    {
    static int count=0;
    char *ptr;
    int fd;
    printf("usbBrcmAudioTestWrite Called \n");
    printf("Count - %d  line size %d line count %d \n",
           count, usbBrcmAudioBfrLineSize, usbBrcmAudioBfrLineCount);

     if ((fd=usbBrcmAudioGetActiveNode()) == USB_BRCM_ERROR)
        {
        return;
        }
    ptr = ((char *) usbBrcmAudioBfr) + (count * usbBrcmAudioBfrLineSize);
    usbBrcmAudioWrite(fd, ptr, usbBrcmAudioBfrLineSize);
    count++;
    if (count >= usbBrcmAudioBfrLineCount)
        {
        count = 0;
        }

    }

/*************************************************************************
*
* usbBrcmAudioTestReadWrite - sample function that executes every
*                             5 msec to read from mic and write to speakers
*                             Registered with USB_BRCM_IOCTL_FUNC_RX_SET
*                             ioctl call.
*
* RETURNS: none
*
* ERRNO: none
*
* \NOMANUAL
*/
VOID usbBrcmAudioTestReadWrite ( void )
    {
    static char *ptr=NULL;
    static char *ptr2=NULL;
    static INT32 bufferLineSize=0;
    int fd;
    int bytesWritten=0;
    int bytesRead;
    static int count=0;

    count++;
    if ((fd=usbBrcmAudioGetActiveNode()) == USB_BRCM_ERROR)
        {
        return;
        }
    /*
     * If this is the first time through or a buffer size has been changed
     * (e.g. headset change), get the audio buffer and store the buffer
     * line size
     */

    if (bufferLineSize != usbBrcmAudioBfrLineSize)
        {
        if (ptr != NULL)
            {
            free(ptr);
            ptr = NULL;
            }

        if (ptr2 != NULL)
            {
            free(ptr2);
            ptr2=NULL;
            }
        }

    printf("AudioBufferLineSize = %d \n",usbBrcmAudioBfrLineSize);

    if (ptr == NULL)
        {
        ptr = malloc(usbBrcmAudioBfrLineSize);
        if (ptr == NULL)
            {
            printf("usbBrcmAudioTestReadWrite: malloc ptr ERROR\n");
            return;
            }
        }

    if (ptr2 == NULL)
        {
        ptr2 = malloc (usbBrcmAudioBfrLineSize);
        if (ptr2 == NULL)
            {
            printf("usbBrcmAudioTestReadWrite: malloc ptr2 ERROR\n");
            free(ptr);
            ptr = NULL;
            return;
            }
        }

   bufferLineSize = usbBrcmAudioBfrLineSize;

    /*
     * Ping pong the two buffers
     */
    if (count % 2)
        {
        bytesRead = usbBrcmAudioRead(fd, ptr, usbBrcmAudioBfrLineSize);
        if (bytesRead > 0)
            {
            bytesWritten = usbBrcmAudioWrite(fd, ptr, bytesRead);
            }
         printf("usbBrcmAudioTestReadWrite:  Bytes Read1 - %d  Bytes Written "
                "- %d\n",bytesRead, bytesWritten);
        }
    else
        {
        bytesRead = usbBrcmAudioRead(fd, ptr2, usbBrcmAudioBfrLineSize);
        if (bytesRead > 0)
            {
            bytesWritten = usbBrcmAudioWrite(fd, ptr2, bytesRead);
            }
        printf("usbBrcmAudioTestReadWrite: Bytes Read2 - %d  Bytes Written "
               "- %d\n",bytesRead, bytesWritten);
        }
    }

/***************************************************************************
 *
 * usbBrcmAudioparseWavFile - parses and displays info about a .wav file
 *
 * Attempts to play the .wav file on speakers
 *
 * RETURNS: OK if able to play file, else ERROR.
 *
 * ERRNO: none
 *
 * \NOMANUAL
 */

LOCAL STATUS usbBrcmAudioparseWavFile
    (
    FILE *wavFile
    )

    {
    RIFF_HDR riffHdr;
    char wavSig [RIFF_WAV_DATA_SIG_LEN];
    RIFF_CHUNK_HDR chunkHdr;
    WAV_FORMAT_CHUNK fmtChunk;
    UINT32 fileLen;
    UINT32 chunkLen;
    char str[256];
    UINT32 actLen;
    char *ptr;
    USB_SPKR_AUDIO_FORMAT fmt;
    INT32 status;

    /* Check the RIFF/WAV header. */

    if (fseek (wavFile, 0L, SEEK_SET) != 0)
        {
        printf("usbBrcmAudioparseWavFile: Cannot seek to beginning of "
               "file.\n");
        return ERROR;
        }

    if (fread (&riffHdr, 1, sizeof (riffHdr), wavFile) < sizeof (riffHdr) ||
        fread (&wavSig, 1, sizeof (wavSig), wavFile) < sizeof (wavSig))
        {
        printf("usbBrcmAudioparseWavFile: Unexpected end of file reading RIFF"
               " header.\n");
        return ERROR;
        }

    if (memcmp (&riffHdr.signature, RIFF_HDR_SIG, RIFF_HDR_SIG_LEN) != 0 ||
        memcmp (wavSig, RIFF_WAV_DATA_SIG, RIFF_WAV_DATA_SIG_LEN) != 0)
        {
        printf("usbBrcmAudioparseWavFile: Not a .wav file.\n");
        return ERROR;
        }


    /* Read and display known chunks */

    fileLen = FROM_LITTLEL (riffHdr.length) + sizeof (riffHdr);
    memcpy(str, riffHdr.signature, RIFF_HDR_SIG_LEN);
    str[RIFF_HDR_SIG_LEN]='\0';
    printf("usbBrcmAudioparseWavFile: riffHdr.signature = '%s'\n",
           str);
    printf("usbBrcmAudioparseWavFile: riffHdr.length    = 0x%x\n",
           (FROM_LITTLEL(riffHdr.length)));
    printf("usbBrcmAudioparseWavFile: .wav file size    = 0x%x\n",
           fileLen);

    while ((UINT32) ftell (wavFile) < fileLen)
        {
        /* Read the next chunk header. */

        if (fread (&chunkHdr, 1, sizeof (chunkHdr), wavFile) <
            sizeof (chunkHdr))
            {
            printf("usbBrcmAudioparseWavFile: Unexpected end of file reading"
                   " chunk header.\n");
            return ERROR;
            }

        chunkLen = FROM_LITTLEL (chunkHdr.length);
        memcpy(str, chunkHdr.chunkId, RIFF_CHUNK_ID_LEN);
        str[RIFF_CHUNK_ID_LEN]='\0';
        printf("usbBrcmAudioparseWavFile: chunkHdr.chunkId = '%s'\n",
               str);

        printf("usbBrcmAudioparseWavFile: chunkHdr.length  = 0x%x\n",
               chunkLen);

        /* If we recognize the chunkId, then display the chunk. */

        if (memcmp (chunkHdr.chunkId, RIFF_WAV_FMT_CHUNK_ID,
                        RIFF_CHUNK_ID_LEN) == 0)
            {
            /* Read the format chunk. */

            if (fread (&fmtChunk, 1, sizeof (fmtChunk), wavFile) <
                sizeof (fmtChunk))
                {
                printf("usbBrcmAudioparseWavFile: Unexpected end of file"
                       " reading format chunk.\n");
                return ERROR;
                }

            /* Display the format chunk */

            printf("usbBrcmAudioparseWavFile: formatTag = 0x%4.4x\n",
                   (FROM_LITTLEW (fmtChunk.formatTag)));
            printf("usbBrcmAudioparseWavFile: channels = %d\n",
                   (FROM_LITTLEW (fmtChunk.channels)));
            printf("usbBrcmAudioparseWavFile: samplesPerSec = %u\n",
                   (FROM_LITTLEL (fmtChunk.samplesPerSec)));
            printf("usbBrcmAudioparseWavFile: avgBytesPerSec = %u\n",
                   (FROM_LITTLEL (fmtChunk.avgBytesPerSec)));
            printf("usbBrcmAudioparseWavFile: blockAlign = %d\n",
                   (FROM_LITTLEW (fmtChunk.blockAlign)));

            if (FROM_LITTLEW (fmtChunk.formatTag) == WAV_FMT_MS_PCM)
                {
                printf("usbBrcmAudioparseWavFile: bitsPerSample = %d\n",
                       (FROM_LITTLEW (fmtChunk.fmt.msPcm.bitsPerSample)));
                }

            /* Attempt to set the audio format to match */

            if (FROM_LITTLEW (fmtChunk.formatTag) == WAV_FMT_MS_PCM)
                {
                memset (&fmt, 0, sizeof (fmt));

                fmt.formatTag = USB_AUDIO_TYPE1_PCM;
                fmt.formatType = USB_AUDIO_FORMAT_TYPE1;
                fmt.channels = FROM_LITTLEW (fmtChunk.channels);
                fmt.subFrameSize = FROM_LITTLEW (fmtChunk.blockAlign) /
                FROM_LITTLEW (fmtChunk.channels);
                fmt.bitRes = FROM_LITTLEW (fmtChunk.fmt.msPcm.bitsPerSample);
                fmt.sampleFrequency = FROM_LITTLEL (fmtChunk.samplesPerSec);
                /*
                 * The ioctl will reset the sampleFrequency based on the
                 * fmt.lowerFreg
                 */
                fmt.freqType = 0;
                fmt.lowerFreq = fmt.sampleFrequency;
                fmt.upperFreq = fmt.sampleFrequency;
                printf("usbBrcmAudioparseWavFile: Calling SET Audio Format"
                       " Ioctl\n");

                status = usbBrcmAudioIoctl(usbBrcmAudioGetActiveNode(),
                    USB_BRCM_IOCTL_SET_AUDIO_FORMAT,
                    &fmt);

                printf("usbBrcmAudioparseWavFile: Returned from SET audio "
                       " Format IOCTL with status %d\n", status);
                if (status != OK)
                    {
                    return ERROR;
                    }
                }

            if (sizeof(fmtChunk) < chunkLen)
                {
                printf("usbBrcmAudioparseWavFile: Skipping %d bytes\n",
                       chunkLen-sizeof(fmtChunk));
                /*
                 * In some sound files, the format chunkLen is longer the
                 * data structure we defined for it.  Therefore if necessary
                 * skip over to the next chunk.
                 */
                fseek (wavFile,chunkLen - sizeof(fmtChunk),SEEK_CUR);
                }
            }
        else if (memcmp (chunkHdr.chunkId, RIFF_WAV_DATA_CHUNK_SIG,
                RIFF_CHUNK_ID_LEN) == 0)
            {
            /* data chunk found */

            usbBrcmAudioWavDataLen = FROM_LITTLEL (chunkHdr.length);
            usbBrcmAudioSampleSize = fmt.channels * fmt.subFrameSize;
            usbBrcmAudioBytesPerSec =
                        usbBrcmAudioSampleSize * fmt.sampleFrequency;

            usbBrcmAudioBfrLineSize = usbBrcmAudioBytesPerSec / 1000 *
            usbBrcmCallbackPeriod;
            printf("usbBrcmAudioparseWavFile: usbBrcmAudioWavDataLen     "
                   "= %d\n", usbBrcmAudioWavDataLen);
            printf("usbBrcmAudioparseWavFile: usbBrcmAudioSampleSize     "
                   "= %d\n", usbBrcmAudioSampleSize);
            printf("usbBrcmAudioparseWavFile: usbBrcmAudioBytesPerSec    "
                   "= %d\n", usbBrcmAudioBytesPerSec);
            printf("usbBrcmAudioparseWavFile: usbBrcmAudioBfrLineSize    "
                   "= %d\n", usbBrcmAudioBfrLineSize);

            printf("malloc for usbBrcmAudioBfr %d\n", usbBrcmAudioBfrLineSize *
                   (usbBrcmAudioWavDataLen/usbBrcmAudioBfrLineSize+1));

            usbBrcmAudioBfr = malloc (usbBrcmAudioBfrLineSize *
                (usbBrcmAudioWavDataLen/usbBrcmAudioBfrLineSize+1));

            /* Check if the buffer is available */
            if (usbBrcmAudioBfr == NULL)
                {
                printf("Failed to malloc buffer\r\n");
                return ERROR;
                }

            /* Read audio data */
            usbBrcmAudioBfrLineCount = 0;
            ptr = (char *) usbBrcmAudioBfr;

            while (usbBrcmAudioWavDataLen > 0 &&(actLen = fread (ptr, 1,
                        min (usbBrcmAudioWavDataLen, usbBrcmAudioBfrLineSize),
                        wavFile)) > 0)
                {
                ptr += usbBrcmAudioBfrLineSize;
                usbBrcmAudioBfrLineCount++;
                usbBrcmAudioWavDataLen -= actLen;
                }

            if (usbBrcmAudioBfrLineCount > 0)
                {
                usbBrcmAudioBfrLineCount--;
                }

            printf("usbBrcmAudioparseWavFile: usbBrcmAudioBfrLineCount = %d \n",
                   usbBrcmAudioBfrLineCount);

            usbBrcmAudioIoctl(usbBrcmAudioGetActiveNode(),
                USB_BRCM_IOCTL_FUNC_RX_SET,
                usbBrcmAudioTestWrite);

            return OK;
            }
        else
            {
            /* Skip over the chunk. */

            fseek (wavFile, chunkLen, SEEK_CUR);
            }
        }

    return ERROR;
    }

/*************************************************************************
*
* usbBrcmAudioStart - initializes the headset.
*
* This routine is invoked
* by the startup code.  This currently does nothing - it simply ensures
* these routines are included in the link.
*
* RETURNS: none
*
* ERRNO: none
*
*
*/

VOID usbBrcmAudioStart( void )
    {
    return;
    }


/*************************************************************************
 *
 * usbBrcmAudioTestInit - initializes the headset.  This routine is invoked
 * by the whenever the tests are started.
 *
 * RETURNS: none
 *
 * ERRNO: none
 *
 *
*/
LOCAL VOID usbBrcmAudioTestInit( void )
    {
    int node;
    /* */

    node = usbBrcmAudioOpen(0, USB_BRCM_SPKR_NAME, O_RDWR);
    printf("usbBrcmAudioStart Called for node %d \n", node);
    if (node != USB_BRCM_ERROR)
        {
        /*
         * This sets the call back that will be invoked speaker/microphone
         * insertion and removal
         */
        usbBrcmAudioIoctl(node,
            USB_BRCM_IOCTL_FUNC_ATTACH_SET,
            usbBrcmAudioUsrAttachCallback);
        /*
         * This sets the finction that will invoked from the local ioctl routine
         * for dynamic configuration changes.  This is for future expansion,
         * It currently does nothing.
         */
        usbBrcmAudioIoctl(node,
            USB_BRCM_IOCTL_FUNC_CFG_SET,
            usbBrcmAudioCfgCallback);
        /*
         * This sets the function thats invoked on a HID SET_REPORT
         */
        usbBrcmAudioIoctl(node,
            USB_BRCM_IOCTL_SET_HID_CALLBACK,
            usbBrcmAudioHidCallback);
        /*
         * This informs the driver of the call back period of the watchdog
         * This is important for setting buffer size etc.
         */
        usbBrcmAudioIoctl(node,
            USB_BRCM_IOCTL_CALLBK_PER,
            (pVOID)(ULONG)(usbBrcmCallbackPeriod));
        }
    }

/***************************************************************************
*
* usbBrcmAudioSpkrTest - Thi tests speaker output by playing wav file
* in an endless loop.
*
* usbBrcmAudioStart() must be called prior to this call
*
* RETURNS: none
*
* ERRNO: none
*
*
*/
VOID usbBrcmAudioSpkrTest
    (
    char *filename                 /* wav file name */
    )
    {
    FILE *f;

    taskPrioritySet(0, 200);
    /*
     * Initalize the headset
     */
    usbBrcmAudioTestInit();

    /* adjust volume level */
#ifdef VOLUME_SUPPORTED
    usbBrcmAudioSpkrVolume(0xec00);
#endif

    /* Attempt to open the file. */

    if ((f = fopen (filename, "rb")) == NULL)
        {
        printf("usbBrcmAudioSpkrTest: Unable to open '%s' \n",
               filename);
        return;
        }


    /* Parse the file */

    printf("usbBrcmAudioSpkrTest: Parsing the wave file \n");

    if (usbBrcmAudioparseWavFile (f) != OK)
        {
        fclose (f);
        printf("usbBrcmAudioSpkrTest: Unable to parse wave file \n");
        return;
        }

    printf("Wave file parsed \n");

    /* We should close the file descriptor after we used the data */
    fclose (f);

    return;

    }

/***************************************************************************
*
* usbBrcmAudioMicrophoneTest - tests microphone.
*
* This routine tests input and  speaker output by
* looping back the microphone input directly
* to speaker output.  usbBrcmAudioStart() must be called prior to this call
*
* Note that there will be some delay between the audio input and the audio
* output.  This delay will be function of the USB_BRCM_AUDIO_CALLBACK_PERIOD
*
* RETURNS: none
*
* ERRNO: none
*
*
*/

void usbBrcmAudioMicrophoneTest(void)
    {
    pUSB_SPKR_AUDIO_FORMAT ppFmt=NULL;
    pUSB_SPKR_AUDIO_FORMAT pFmt=NULL;
    INT32 micFormatIndex;
    UINT16 micFormats;
    INT32 node;
    INT32 divisor;
    STATUS status;
    BOOL fmtSet = FALSE;
    /*
     * Initalize the headset
     */
    usbBrcmAudioTestInit();
    /*
     * Find out which device we are speaking to
     */
    node = usbBrcmAudioGetActiveNode();
     /*
     * We are going to look at all the available microphone formats.  We will
     * attempt to set the speakers up to match that format
     */
    status = usbBrcmAudioIoctl(node,
                            USB_BRCM_IOCTL_GET_FORMAT_COUNT_MIC,
                            &micFormats);
    if (status != OK)
        {
        printf("Unable to get the number of available formats for microphone \n");
        return ;
        }
    printf("usbBrcmAudioMicrophoneTest: Mic formats - %d %p\n",
           micFormats, &micFormats);


    status = usbBrcmAudioIoctl(node,
                            USB_BRCM_IOCTL_GET_FORMAT_LIST_MIC,
                            &ppFmt);

    if (status != OK)
         {
         printf(
             "Unable to get formats for microphone \n");
         return;
         }

    for (micFormatIndex = 0; micFormatIndex < (INT32)micFormats ;
                micFormatIndex++)
        {
        pFmt = &ppFmt[micFormatIndex];
        printf("usbBrcmAudioMicrophoneTest Trying to set speaker "
               "format %d (%d)\n",micFormatIndex,
               pFmt->sampleFrequency);

        status = usbBrcmAudioIoctl(node,USB_BRCM_IOCTL_SET_AUDIO_FORMAT,
                                   pFmt);
        if(status == OK)
            {
            fmtSet = TRUE;
            break;
            }
        }

    /*
     * If we didn't get a successful set, bail out
     */
    if (fmtSet != TRUE)
        {
        printf("Unable to set speaker format to match microphone format\n");
        return ;
        }
    /*
     * Set the microphone format to match what we just set the speaker to.
     * If there is an error, something is messed up.
     */

    status = usbBrcmAudioIoctl(node,USB_BRCM_IOCTL_SET_AUDIO_FORMAT_MIC,
                               pFmt);
    if (status != OK)
         {
         printf("Unable to set format for microphone \n");
         return;
         }

     usbBrcmAudioSampleSize = pFmt->channels * pFmt->subFrameSize;

     usbBrcmAudioBytesPerSec =
                    usbBrcmAudioSampleSize *
                    usbBrcmAudioProperties[node].desiredFmt.sampleFrequency;

    /* usbBrcmCallbackPeriod is in milliseconds */

    divisor = 1000/usbBrcmCallbackPeriod;
    if ((1000 % usbBrcmCallbackPeriod) != 0)
        divisor++;

    usbBrcmAudioBfrLineSize = (usbBrcmAudioBytesPerSec+(divisor-1)) /
                divisor;

    printf("Sample size - %d Bytes per sec - %d BufrLineSize %d sampleFreg"
           " - %d \n",
           usbBrcmAudioSampleSize,usbBrcmAudioBytesPerSec,usbBrcmAudioBfrLineSize,
           usbBrcmAudioProperties[node].desiredFmt.sampleFrequency);
     /* adjust volume level */
#if VOLUME_SUPPORTED
    usbBrcmAudioSpkrVolume(-2); /* set to max */
#endif
    /*
     * This IOCTL call sets the periodic function that is called every
     * time the periodic task executes.  Note that the periodic tasks starts
     * to run as an artiface of this ioctl.
     */
    usbBrcmAudioIoctl(usbBrcmAudioGetActiveNode(),
        USB_BRCM_IOCTL_FUNC_RX_SET, usbBrcmAudioTestReadWrite);
    }

/***************************************************************************
*
* usbBrcmAudioSpkrConversionTest - tests mono to stereo conversion.
*
* This routine tests the mono to stero  at speakers
* by looping back the mono microphone input
* to speaker output after mono to stereo conversion.
* usbBrcmAudioStart() must be called prior to this call
*
* RETURNS: none
*
* ERRNO: none
*
*
*/
void usbBrcmAudioSpkrConversionTest(void)
    {
    USB_SPKR_AUDIO_FORMAT fmt;
    BOOL convert;

    /* speaker */

    memset (&fmt, 0, sizeof(fmt));

    fmt.formatTag = USB_AUDIO_TYPE1_PCM;
    fmt.formatType = USB_AUDIO_FORMAT_TYPE1;
    fmt.channels = 2; /* stereo */
    fmt.subFrameSize = USB_BRCM_DEBUG_SUBFRAMESIZE;
    fmt.bitRes = USB_BRCM_DEBUG_RESOLUTION;
    fmt.sampleFrequency = USB_BRCM_DEBUG_FREQUENCY;

    convert = TRUE;
    usbBrcmAudioIoctl(usbBrcmAudioGetActiveNode(),
        USB_BRCM_IOCTL_SET_SPKR_MONO2STEREO,
        &convert);

    usbBrcmAudioIoctl(usbBrcmAudioGetActiveNode(),
        USB_BRCM_IOCTL_SET_AUDIO_FORMAT,
        &fmt);

    /* mic */

    memset (&fmt, 0, sizeof(fmt));

    fmt.formatTag = USB_AUDIO_TYPE1_PCM;
    fmt.formatType = USB_AUDIO_FORMAT_TYPE1;
    fmt.channels = 1; /* mono */
    fmt.subFrameSize = USB_BRCM_DEBUG_SUBFRAMESIZE;
    fmt.bitRes = USB_BRCM_DEBUG_RESOLUTION;
    fmt.sampleFrequency = USB_BRCM_DEBUG_FREQUENCY;

    usbBrcmAudioIoctl(usbBrcmAudioGetActiveNode(),
        USB_BRCM_IOCTL_SET_AUDIO_FORMAT_MIC,
        &fmt);

    /* adjust volume level */

    usbBrcmAudioSpkrVolume(-2); /* set to max */

    usbBrcmAudioSampleSize = 1 * USB_BRCM_DEBUG_SUBFRAMESIZE; /* mono */
    usbBrcmAudioBytesPerSec = usbBrcmAudioSampleSize * USB_BRCM_DEBUG_FREQUENCY;
    usbBrcmAudioBfrLineSize = usbBrcmAudioBytesPerSec / 1000 *
    usbBrcmAudioProperties[0].callbackPer;

    usbBrcmAudioIoctl(usbBrcmAudioGetActiveNode(),
        USB_BRCM_IOCTL_FUNC_RX_SET,
        usbBrcmAudioTestReadWrite);
    }

/***************************************************************************
*
* usbBrcmAudioMicConversionTest - tests stereo to mono conversion.
*
* This routine tests the  microphone
* by looping back the converted stereo microphone input
* to speaker output after stereo to mono conversion.
* usbBrcmAudioStart() must be called prior to this call
*
* RETURNS: none
*
* ERRNO: none
*
*
*/

VOID usbBrcmAudioMicConversionTest(void)
    {
    USB_SPKR_AUDIO_FORMAT fmt;
    BOOL convert;

    /* speaker */

    memset (&fmt, 0, sizeof(fmt));

    fmt.formatTag = USB_AUDIO_TYPE1_PCM;
    fmt.formatType = USB_AUDIO_FORMAT_TYPE1;
    fmt.channels = 1; /* mono */
    fmt.subFrameSize = USB_BRCM_DEBUG_SUBFRAMESIZE;
    fmt.bitRes = USB_BRCM_DEBUG_RESOLUTION;
    fmt.sampleFrequency = USB_BRCM_DEBUG_FREQUENCY;

    usbBrcmAudioIoctl(usbBrcmAudioGetActiveNode(),
        USB_BRCM_IOCTL_SET_AUDIO_FORMAT,
        &fmt);

    /* mic */

    memset (&fmt, 0, sizeof(fmt));

    fmt.formatTag = USB_AUDIO_TYPE1_PCM;
    fmt.formatType = USB_AUDIO_FORMAT_TYPE1;
    fmt.channels = 2; /* stereo */
    fmt.subFrameSize = USB_BRCM_DEBUG_SUBFRAMESIZE;
    fmt.bitRes = USB_BRCM_DEBUG_RESOLUTION;
    fmt.sampleFrequency = USB_BRCM_DEBUG_FREQUENCY;

    convert = TRUE;
    usbBrcmAudioIoctl(usbBrcmAudioGetActiveNode(),
        USB_BRCM_IOCTL_SET_MIC_STEREO2MONO,
        &convert);

    usbBrcmAudioIoctl(usbBrcmAudioGetActiveNode(),
        USB_BRCM_IOCTL_SET_AUDIO_FORMAT_MIC,
        &fmt);

    /* adjust volume level */

    usbBrcmAudioSpkrVolume(-2); /* set to max */


    usbBrcmAudioSampleSize = 1 * USB_BRCM_DEBUG_SUBFRAMESIZE; /* mono */
    usbBrcmAudioBytesPerSec = usbBrcmAudioSampleSize * USB_BRCM_DEBUG_FREQUENCY;
    usbBrcmAudioBfrLineSize = usbBrcmAudioBytesPerSec / 1000 *
    usbBrcmAudioProperties[0].callbackPer;

    usbBrcmAudioIoctl(usbBrcmAudioGetActiveNode(),
        USB_BRCM_IOCTL_FUNC_RX_SET,
        usbBrcmAudioTestReadWrite);
    }

/***************************************************************************
*
* usbBrcmAudioConversionTest - mono to stereo conversion tests.
*
* This routine tests the mono to stereo conversion at  microphone
* and mono to stereo conversion at speakers
*
*
* RETURNS: none
*
* ERRNO: none
*
* \NOMANUAL
*/

void usbBrcmAudioConversionTest(void)
    {
    USB_SPKR_AUDIO_FORMAT fmt;
    BOOL convert;

    /* speaker */

    memset (&fmt, 0, sizeof(fmt));

    fmt.formatTag = USB_AUDIO_TYPE1_PCM;
    fmt.formatType = USB_AUDIO_FORMAT_TYPE1;
    fmt.channels = 2; /* stereo */
    fmt.subFrameSize = USB_BRCM_DEBUG_SUBFRAMESIZE;
    fmt.bitRes = USB_BRCM_DEBUG_RESOLUTION;
    fmt.sampleFrequency = USB_BRCM_DEBUG_FREQUENCY;

    convert = TRUE;
    usbBrcmAudioIoctl(usbBrcmAudioGetActiveNode(),
        USB_BRCM_IOCTL_SET_SPKR_MONO2STEREO,
        &convert);

    usbBrcmAudioIoctl(usbBrcmAudioGetActiveNode(),
        USB_BRCM_IOCTL_SET_AUDIO_FORMAT,
        &fmt);

    /* mic */

    memset (&fmt, 0, sizeof(fmt));

    fmt.formatTag = USB_AUDIO_TYPE1_PCM;
    fmt.formatType = USB_AUDIO_FORMAT_TYPE1;
    fmt.channels = 2; /* stereo */
    fmt.subFrameSize = USB_BRCM_DEBUG_SUBFRAMESIZE;
    fmt.bitRes = USB_BRCM_DEBUG_RESOLUTION;
    fmt.sampleFrequency = USB_BRCM_DEBUG_FREQUENCY;

    convert = TRUE;
    usbBrcmAudioIoctl(usbBrcmAudioGetActiveNode(),
        USB_BRCM_IOCTL_SET_MIC_STEREO2MONO,
        &convert);

    usbBrcmAudioIoctl(usbBrcmAudioGetActiveNode(),
        USB_BRCM_IOCTL_SET_AUDIO_FORMAT_MIC,
        &fmt);

    /* adjust volume level */
    usbBrcmAudioSpkrVolume(-2); /* set to max */

    usbBrcmAudioSampleSize = 1 * USB_BRCM_DEBUG_SUBFRAMESIZE; /* mono */
    usbBrcmAudioBytesPerSec = usbBrcmAudioSampleSize * USB_BRCM_DEBUG_FREQUENCY;
    usbBrcmAudioBfrLineSize = usbBrcmAudioBytesPerSec / 1000 *
    usbBrcmAudioProperties[0].callbackPer;

    usbBrcmAudioIoctl(usbBrcmAudioGetActiveNode(),
        USB_BRCM_IOCTL_FUNC_RX_SET,
        usbBrcmAudioTestReadWrite);
    }
/***************************************************************************
*
* usbBrcmAudioTestStop -   This routine stops the audio test that is
*                          in progress.
*
* Stop the audio test in progress.
*
*
* RETURNS: none
*
* ERRNO: none
*
*
*/

void usbBrcmAudioTestStop (void)
    {
    INT32 node;
    printf("Stopping the audio demo \n");
    /*
     * Find out which device we are speaking to
     */
    node = usbBrcmAudioGetActiveNode();
    /*
     * Closing the device stops the streaming task
     */
    usbBrcmAudioClose(node);

    /* if the audio buffer is not NULL we should free it for latter use */
    if (usbBrcmAudioBfr != NULL)
        {
        free(usbBrcmAudioBfr);
        usbBrcmAudioBfr = NULL;
        }

    return;
    }

#endif /* INCLUDE_USB_HEADSET_DEMO */

#endif /* __INCusrUsbBrcmAudioc */
