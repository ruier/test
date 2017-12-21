/* usbOtg.c - USB OTG Configlette */

/*
 * Copyright (c) 2010, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify, or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01d,23mar11,w_x  Address more code review comments and documentation cleanup
01c,08mar11,w_x  Address code review comments and some testing code improvments
01b,23feb11,w_x  Fix duplicates by usrDepend.c and usrUsbInit.c (WIND00247788) 
01a,30may10,w_x  written
*/

/*
DESCRIPTION

This file is the USB OTG user configlet file.

INCLUDE FILES: drv/erf/erfLib.h usb/usbOtg.h
*/

#ifndef __INCusrUsbOtgInitc
#define __INCusrUsbOtgInitc

#include <drv/erf/erfLib.h>
#include <usb/usbOtg.h>

/* defines */

#ifndef USB_OTG_MGR_BASE_NAME
#define USB_OTG_MGR_BASE_NAME              "/usbOtg"
#endif /* USB_OTG_MGR_BASE_NAME */

#ifndef USB_OTG_MGR_EVENT_WAIT_TIME
#define USB_OTG_MGR_EVENT_WAIT_TIME      (1000) /* ms */
#endif /* USB_OTG_MGR_EVENT_WAIT_TIME */

#define USB_OTG_MGR_EVENT_WAIT_TIME_MIN  (500)  /* ms */
#define USB_OTG_MGR_EVENT_WAIT_TIME_MAX  (5000) /* ms */

#ifndef USB_OTG_HNP_POLLING_INTERVAL
#define USB_OTG_HNP_POLLING_INTERVAL        (1500)  /* ms */
#endif /* USB_OTG_HNP_POLLING_INTERVAL */

#define USB_OTG_HNP_POLLING_INTERVAL_MIN    (1000)  /* ms */
#define USB_OTG_HNP_POLLING_INTERVAL_MAX    (2000)  /* ms */

#ifndef USB_OTG_MGR_TASK_PRIORITY
#define USB_OTG_MGR_TASK_PRIORITY       5
#endif /* USB_OTG_MGR_TASK_PRIORITY */

#define USB_OTG_MGR_TASK_PRIORITY_MIN   (5)  
#define USB_OTG_MGR_TASK_PRIORITY_MAX   (50) 

#ifndef USB_OTG_TEST_CODE_ENABLED
#define USB_OTG_TEST_CODE_ENABLED       TRUE
#endif /* USB_OTG_TEST_CODE_ENABLED */

/* locals */

LOCAL int usrUsbOtgHnpPollingInterval = /* value selected below */ 
#if (USB_OTG_HNP_POLLING_INTERVAL > USB_OTG_HNP_POLLING_INTERVAL_MAX)
    USB_OTG_HNP_POLLING_INTERVAL_MAX;
#elif (USB_OTG_HNP_POLLING_INTERVAL < USB_OTG_HNP_POLLING_INTERVAL_MIN)
    USB_OTG_HNP_POLLING_INTERVAL_MIN;
#else
    USB_OTG_HNP_POLLING_INTERVAL;
#endif

LOCAL int usrUsbOtgMgrEventWaitTime = /* value selected below */ 
#if (USB_OTG_MGR_EVENT_WAIT_TIME > USB_OTG_MGR_EVENT_WAIT_TIME_MAX)
    USB_OTG_MGR_EVENT_WAIT_TIME_MAX;
#elif (USB_OTG_MGR_EVENT_WAIT_TIME < USB_OTG_MGR_EVENT_WAIT_TIME_MIN)
    USB_OTG_MGR_EVENT_WAIT_TIME_MIN;
#else
    USB_OTG_MGR_EVENT_WAIT_TIME;
#endif

/*******************************************************************************
*
* usrUsbIsMhdrcOtgEnabled - determine whether MUSBMHDRC OCD support is selected
*
* This routine determines whether MUSBMHDRC OCD support is selected.
*
* RETURNS: TRUE if it is the matching device, FALSE if not
*
* ERRNO: N/A
*/

BOOL usrUsbIsMhdrcOtgEnabled (void)
    {
#ifdef INCLUDE_USB_MHDRC_OCD
    return TRUE;
#else
    return FALSE;
#endif
    }

/*******************************************************************************
*
* usrUsbOtgInit - initialize USB OTG stack
*
* This routine initializes USB OTG stack. It registers the OTG controller 
* drivers included by the user configuration.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usrUsbOtgInit(void)
    {
#ifdef INCLUDE_USB_MHDRC_OCD
    vxbUsbMhdrcOcdRegister();
#endif /* INCLUDE_USB_MHDRC_OCD */
    }

/*******************************************************************************
*
* usrUsbOtgMgrEventWaitTimeGet - get OTG manager event wait time in ms
*
* This routine is to get OTG manager event notification wait time in ms.
*
* RETURNS: OTG parameter change timeout in ms.
*
* ERRNO: N/A
*/

int usrUsbOtgMgrEventWaitTimeGet (void)
    {
    return usrUsbOtgMgrEventWaitTime;
    }

/*******************************************************************************
*
* usrUsbOtgMgrEventWaitTimeSet - set OTG manager event wait time in ms
*
* This routine is to set OTG manager event notification wait time in ms.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usrUsbOtgMgrEventWaitTimeSet 
    (
    int time
    )
    {
    /* 
     * Control the time limit at set time, the usrUsbOtgMgrEventWaitTimeGet()
     * can just return it without the calculation overhead each time.
     */
    
    if (time > USB_OTG_MGR_EVENT_WAIT_TIME_MAX)
        time = USB_OTG_MGR_EVENT_WAIT_TIME_MAX;
    else if (time < USB_OTG_MGR_EVENT_WAIT_TIME_MIN)
        time = USB_OTG_MGR_EVENT_WAIT_TIME_MIN;
    
    usrUsbOtgMgrEventWaitTime = time;
    }

/*******************************************************************************
*
* usrUsbOtgHnpPollingIntervalGet - get OTG HNP polling interval in ms
*
* This routine is to get OTG HNP polling interval in ms.
*
* RETURNS: OTG HNP polling interval in ms.
*
* ERRNO: N/A
*/

int usrUsbOtgHnpPollingIntervalGet (void)
    {
    return usrUsbOtgHnpPollingInterval;
    }

/*******************************************************************************
*
* usrUsbOtgHnpPollingIntervalSet - set OTG HNP polling interval in ms
*
* This routine is to OTG HNP polling interval in ms.
*
* RETURNS: N/A
*
* ERRNO: N/A
*/

void usrUsbOtgHnpPollingIntervalSet 
    (
    int time
    )
    {
    /* 
     * Control the time limit at set time, the usrUsbOtgHnpPollingIntervalGet()
     * can just return it without the calculation overhead each time.
     */
    
    if (time > USB_OTG_HNP_POLLING_INTERVAL_MAX)
        time = USB_OTG_HNP_POLLING_INTERVAL_MAX;
    else if (time < USB_OTG_HNP_POLLING_INTERVAL_MIN)
        time = USB_OTG_HNP_POLLING_INTERVAL_MIN;
    
    usrUsbOtgHnpPollingInterval = time;
    }

/*******************************************************************************
*
* usrUsbOtgMgrTaskPriorityGet - get OTG manager task priority
*
* This routine is to get OTG manager task priority.
*
* RETURNS: OTG manager task priority.
*
* ERRNO: N/A
*/

int usrUsbOtgMgrTaskPriorityGet (void)
    {
    /* The selection is a compile time cost */

    #if (USB_OTG_MGR_TASK_PRIORITY > USB_OTG_MGR_TASK_PRIORITY_MAX)
        return USB_OTG_MGR_TASK_PRIORITY_MAX;
    #elif (USB_OTG_MGR_TASK_PRIORITY < USB_OTG_MGR_TASK_PRIORITY_MIN)
        return USB_OTG_MGR_TASK_PRIORITY_MIN;
    #else
        return USB_OTG_MGR_TASK_PRIORITY;
    #endif
    }

/*******************************************************************************
*
* usrUsbOtgMgrBaseNameGet - get OTG controller iosLib base name
*
* This routine is to get OTG controller iosLib base name.
*
* RETURNS: OTG controller iosLib base name.
*
* ERRNO: N/A
*/

char * usrUsbOtgMgrBaseNameGet (void)
    {
    return USB_OTG_MGR_BASE_NAME;
    }

#if (USB_OTG_TEST_CODE_ENABLED == TRUE)

/* User code debug control */

#define USR_USBOTG_DEBUG

#ifdef USR_USBOTG_DEBUG
#define USR_USBOTG_DBG(...)                 \
    do{ if (gUsbOtgUsrDebug) printf(__VA_ARGS__); } while (0)
#else
#define USR_USBOTG_DBG(msk, ...)            \
    do{ } while (0)
#endif

/* globals */

int gUsbOtgUsrDebug = 1;
BOOL usrUsbOtgTestHnpActive = TRUE;
BOOL usrUsbOtgTestSrpActive = TRUE;
BOOL usrUsbOtgTestHnpReqGot = FALSE;

/*******************************************************************************
*
* usrUsbOtgTestCallback - USB OTG user message callback
*
* This routine is USB OTG user message callback, it is installed by user 
* application through a call to usrUsbOtgCallbackInstall().
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

VOID usrUsbOtgTestCallback
    (
    void *           pUsrMsgBuff,
    void *           pUsrContext
    )
    {
    int otgFd = (int)pUsrContext;
    STATUS sts = ERROR;
    pUSBOTG_USR_MSG_HDR pUsrMsgHdr = (pUSBOTG_USR_MSG_HDR)pUsrMsgBuff;
    
    if (pUsrMsgBuff == NULL)
        {
        USR_USBOTG_DBG("pUsrMsgBuff is NULL\n");
        return;
        }
    
    switch (pUsrMsgHdr->uMsgId)
        {
        case USBOTG_USR_MSG_ID_SRP_ACTIVE:
            {
            USR_USBOTG_DBG("usrUsbOtgTestCallback - USBOTG_USR_MSG_ID_SRP_ACTIVE\n");
            }
            break;
        case USBOTG_USR_MSG_ID_SRP_FAIL:
            {
            USR_USBOTG_DBG("usrUsbOtgTestCallback - USBOTG_USR_MSG_ID_SRP_FAIL\n");
            }
            break;
        case USBOTG_USR_MSG_ID_VRISE_FAIL:
            {
            USR_USBOTG_DBG("usrUsbOtgTestCallback - USBOTG_USR_MSG_ID_VRISE_FAIL\n");
            }
            break;
        case USBOTG_USR_MSG_ID_HOST_NO_HNP:
            {
            USR_USBOTG_DBG("usrUsbOtgTestCallback - USBOTG_USR_MSG_ID_HOST_NO_HNP\n");
            }
            break;
        case USBOTG_USR_MSG_ID_DEV_UNSUPPORTED:
            {
            pUSBOTG_USR_MSG_DEV_UNSUPPORTED pUsrMsg = 
                (pUSBOTG_USR_MSG_DEV_UNSUPPORTED)pUsrMsgHdr;  
            
            USR_USBOTG_DBG("usrUsbOtgTestCallback - USBOTG_USR_MSG_ID_DEV_UNSUPPORTED,"
                "the device handle is 0x%X\n", pUsrMsg->hDevice);
            }
            break;
        case USBOTG_USR_MSG_ID_DEV_REQ_HOST:
            {
            pUSBOTG_USR_MSG_DEV_REQ_HOST pUsrMsg = 
                (pUSBOTG_USR_MSG_DEV_REQ_HOST)pUsrMsgHdr; 
                
            USR_USBOTG_DBG("usrUsbOtgTestCallback - USBOTG_USR_MSG_ID_DEV_REQ_HOST,"
                "the device handle is 0x%X\n", pUsrMsg->hDevice);
            
            usrUsbOtgTestHnpReqGot = TRUE;
            }
            break;
        case USBOTG_USR_MSG_ID_REQ_HOST_RISKY:
            {
            USR_USBOTG_DBG("usrUsbOtgTestCallback - USBOTG_USR_MSG_ID_REQ_HOST_RISKY\n");
            }
            break;
        case USBOTG_USR_MSG_ID_HOST_ACTIVE:
            {
            USR_USBOTG_DBG("usrUsbOtgTestCallback - USBOTG_USR_MSG_ID_HOST_ACTIVE\n");
            }
            break;
        case USBOTG_USR_MSG_ID_TARGET_ACTIVE:
            {
            USR_USBOTG_DBG("usrUsbOtgTestCallback - USBOTG_USR_MSG_ID_TARGET_ACTIVE\n");

            usrUsbOtgHostRequest(otgFd, 
                                 USBOTG_HOST_REQUEST_HNP_BASIC,
                                 NO_WAIT);
            }
            break;
        case USBOTG_USR_MSG_ID_DEV_UNRESPONDING:
            {
            pUSBOTG_USR_MSG_DEV_UNRESPONDING pUsrMsg = 
                (pUSBOTG_USR_MSG_DEV_UNRESPONDING)pUsrMsgHdr; 
            
            USR_USBOTG_DBG("usrUsbOtgTestCallback - USBOTG_USR_MSG_ID_DEV_UNRESPONDING,"
                "the device handle is 0x%X\n", pUsrMsg->hDevice);
            }
            break;
        case USBOTG_USR_MSG_ID_SRP_DETECTED:
            {
            USR_USBOTG_DBG("usrUsbOtgTestCallback - USBOTG_USR_MSG_ID_SRP_DETECTED\n");
            
            usrUsbOtgVbusPowerOn(otgFd, 5 * sysClkRateGet());
            }
            break;
        case USBOTG_USR_MSG_ID_HNP_ENABLED:
            {
            USR_USBOTG_DBG("usrUsbOtgTestCallback - USBOTG_USR_MSG_ID_HNP_ENABLED\n");

            usrUsbOtgHostRequest(otgFd, USBOTG_HOST_REQUEST_HNP_BASIC, NO_WAIT);
            }
            break;
        default:
            {
            USR_USBOTG_DBG("usrUsbOtgTestCallback - Unknown USB OTG message %d\n", 
                pUsrMsgHdr->uMsgId);
            }
            break;
        }
    }

/*******************************************************************************
*
* usrUsbOtgTestHnp - USB OTG test code entry point for HNP testing
*
* This routine is USB OTG test code entry point. 
* 
* To test role switch using basic HNP (USBOTG_HOST_REQUEST_HNP_BASIC):
*
* #sp usrUsbOtgTestHnp, "/usbOtg0", 0
*
* To test role switch using HNP polling (USBOTG_HOST_REQUEST_HNP_POLLING):
*
* #sp usrUsbOtgTestHnp, "/usbOtg0", 1
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

VOID usrUsbOtgTestHnp
    (
    char *                      otgName,
    USBOTG_HOST_REQUEST_TYPE    reqType
    )
    {
    int otgFd;
    STATUS sts = ERROR;
    USBOTG_STATE otgStateBefore1;
    USBOTG_STATE otgStateBefore2;
    USBOTG_STATE otgStateAfter;
    UINT32 waitCount;
    USBOTG_USR_CALLBACK_INFO usrCallbackInfo;
    UINT32 uGiveupCount = 0;
    UINT32 uPassCount = 0;
    UINT32 uFailCount = 0;
    UINT32 uRequestCount = 0;
    
    otgFd = usrUsbOtgOpen(otgName);

    if (otgFd <= 0)
        {
        USR_USBOTG_DBG("usrUsbOtgOpen fail, maybe already opened!\n");
                
        return;
        }

    gUsbOtgUsrDebug = 1;

    /* Install the default user callback */
    
    sts = usrUsbOtgCallbackInstall(otgFd, 
                                   usrUsbOtgTestCallback, (void *)otgFd);

    if (sts != OK)
        {
        USR_USBOTG_DBG("usrUsbOtgCallbackInstall fail!\n");

        usrUsbOtgClose(otgFd);

        return;
        }
    
    while (usrUsbOtgTestHnpActive)
        {
        sts = usrUsbOtgStateGet(otgFd, &otgStateBefore1);

        if ((otgStateBefore1 != USBOTG_STATE_a_host) && 
            (otgStateBefore1 != USBOTG_STATE_b_host))
            {
            uRequestCount++;
            
            USR_USBOTG_DBG("\r\nHNP TRY - Requesting host role in %s state - %d\r\n", 
                usrUsbOtgStateNameGet(otgStateBefore1), uRequestCount);
            
            sts = usrUsbOtgHostRequest(otgFd, 
                                      reqType,
                                      5 * sysClkRateGet());

            if (sts != OK)
                {
                taskDelay(sysClkRateGet());

                USR_USBOTG_DBG("\r\nHNP TRY - Request host role not allowed - %d!\r\n",
                    uRequestCount);
                
                continue;
                }

            taskDelay(5 * sysClkRateGet());

            sts = usrUsbOtgStateGet(otgFd, &otgStateBefore2);

            if ((otgStateBefore2 != USBOTG_STATE_a_host) && 
                (otgStateBefore2 != USBOTG_STATE_b_host))
                {
                USR_USBOTG_DBG("\r\nHNP WAIT - Requested but still in %s state - %d\r\n", 
                    usrUsbOtgStateNameGet(otgStateBefore2), uRequestCount);
                
                continue;
                }
            }

        sts = usrUsbOtgStateGet(otgFd, &otgStateBefore2);

        if (reqType == USBOTG_HOST_REQUEST_HNP_POLLING)
            {
            USR_USBOTG_DBG("\r\nHNP POLLING - Wait in %s for remote host request - %d\r\n", 
                usrUsbOtgStateNameGet(otgStateBefore2), uRequestCount);
            
            while (usrUsbOtgTestHnpReqGot == FALSE)
                taskDelay(sysClkRateGet());
            
            usrUsbOtgTestHnpReqGot = FALSE;
            
            uPassCount ++;

            USR_USBOTG_DBG("\r\nHNP POLLING PASS - Got remote host request in %s - %d\r\n", 
                usrUsbOtgStateNameGet(otgStateBefore2), uPassCount);
            
            usrUsbOtgHostGiveup(otgFd, NO_WAIT);

            taskDelay(5 * sysClkRateGet());

            continue;
            }

        uGiveupCount++;
        
        USR_USBOTG_DBG("\r\nHNP TRY - Give up host role - %d!\r\n",
            uGiveupCount);
        
        sts = usrUsbOtgHostGiveup(otgFd, 5 * sysClkRateGet());

        if (sts != OK)
            {
            USR_USBOTG_DBG("\r\nHNP RETRY - Give up host role not allowed - %d!\r\n",
                uGiveupCount);
            
            continue;
            }
        
        waitCount = 1000;
        
        do
            {
            taskDelay(10);
            
            sts = usrUsbOtgStateGet(otgFd, &otgStateAfter);
            
            waitCount--;

            if ((otgStateAfter == USBOTG_STATE_a_peripheral) ||
                (otgStateAfter == USBOTG_STATE_b_peripheral) ||
                (otgStateAfter == USBOTG_STATE_a_suspend))
                {
                break;
                }
            
            }while (waitCount != 0);

        if (waitCount == 0)
            {
            uFailCount++;
            
            USR_USBOTG_DBG("\r\nHNP FAIL - %d (%s --> %s --> %s)!\r\n", 
                uFailCount, 
                usrUsbOtgStateNameGet(otgStateBefore1),
                usrUsbOtgStateNameGet(otgStateBefore2),
                usrUsbOtgStateNameGet(otgStateAfter));
            }
        else
            {
            uPassCount ++;
            
            USR_USBOTG_DBG("\r\nHNP PASS - %d (%s --> %s --> %s)!\r\n", 
                uPassCount, 
                usrUsbOtgStateNameGet(otgStateBefore1),
                usrUsbOtgStateNameGet(otgStateBefore2),
                usrUsbOtgStateNameGet(otgStateAfter));
            }

        taskDelay(5 * sysClkRateGet());
        }

    usrUsbOtgClose(otgFd);

    USR_USBOTG_DBG("\r\nHNP - Giveup host role %d times\r\n", 
        uGiveupCount);

    USR_USBOTG_DBG("\r\nHNP - PASS %d times!\r\n", uPassCount);
    
    USR_USBOTG_DBG("\r\nHNP - FAIL %d times!\r\n", uFailCount);
    }

/*******************************************************************************
*
* usrUsbOtgTestSrp - USB OTG test code entry point for SRP
*
* This routine is USB OTG test code entry point; Call it like below from shell:
*
* #sp usrUsbOtgTestSrp, "/usbOtg0"
*
* RETURNS: N/A
*
* ERRNO: N/A
*
* \NOMANUAL
*/

VOID usrUsbOtgTestSrp
    (
    char * otgName
    )
    {
    int otgFd;
    STATUS sts = ERROR;
    USBOTG_STATE otgStateBefore;
    USBOTG_STATE otgStateAfter;
    UINT32 waitCount;
    UINT32 uRequestCount = 0;
    USBOTG_DEVICE_TYPE devType;
    USBOTG_VBUS_STATE  vbusState;
    
    otgFd = usrUsbOtgOpen(otgName);

    if (otgFd <= 0)
        {
        USR_USBOTG_DBG("usrUsbOtgOpen fail, maybe already opened!\n");
                
        return;
        }

    gUsbOtgUsrDebug = 1;
    
    while (usrUsbOtgTestSrpActive)
        {
        uRequestCount++;
        
        sts = usrUsbOtgDeviceTypeGet(otgFd, &devType);

        if (sts != OK)
            {
            USR_USBOTG_DBG("usrUsbOtgDeviceTypeGet fail, exit!\n");
            
            usrUsbOtgClose(otgFd);
            
            return;
            }

        if (devType == USBOTG_A_DEVICE)
            {
            /*
             * As A-device, the VBUS would be powered on intially, or later 
             * by SRP from B-device, so we could expect VBUS to be on in
             * the last...
             */
             
            USR_USBOTG_DBG("\r\nVBUS - Wait VBUS to be powered on on A-device - %d!\r\n", 
                uRequestCount);

            /* Let's power on ... */
            
            sts = usrUsbOtgVbusPowerOn(otgFd, 5000);

            /* Wait someone to make VBUS on ... */
            
            waitCount = 100;
            do
                {
                sts = usrUsbOtgVbusStateGet(otgFd, &vbusState);

                if (sts != OK)
                    {                    
                    USR_USBOTG_DBG("usrUsbOtgVbusStateGet fail before power off, exit!\n");

                    usrUsbOtgClose(otgFd);

                    return;
                    }
                
                if (vbusState == USBOTG_VBUS_STATE_VBusValid)
                    break;
                
                waitCount--;
                
                if (waitCount == 0)
                    break;
                
                taskDelay(sysClkRateGet());
                } while (TRUE);

            if (waitCount == 0)
                USR_USBOTG_DBG("\r\nVBUS FAIL - VBUS is NOT powered on on A-device - %d!\r\n", 
                    uRequestCount);
            else
                USR_USBOTG_DBG("\r\nVBUS PASS - VBUS is powered on on A-device - %d!\r\n", 
                    uRequestCount);

            /*
             * Now its time to make VBUS off, and let the B-device use SRP to 
             * make it on again...or, in the worst case if no SRP is got, the 
             * OTG stack will try to power on VBUS after some time staying
             * in the a_idle state...
             */
             
            USR_USBOTG_DBG("\r\nVBUS - Try to power off VBUS on A-device - %d!\r\n", 
                uRequestCount);

            /* Let's power off... */
            
            sts = usrUsbOtgVbusPowerOff(otgFd, 5000);
            
            waitCount = 100;
            do
                {
                sts = usrUsbOtgVbusStateGet(otgFd, &vbusState);

                if (sts != OK)
                    {                    
                    USR_USBOTG_DBG("usrUsbOtgVbusStateGet fail after power off, exit!\n");

                    usrUsbOtgClose(otgFd);

                    return;
                    }

                if (vbusState == USBOTG_VBUS_STATE_SessEnd)
                    break;
                
                waitCount--;
                
                if (waitCount == 0)
                    break;
                
                taskDelay(sysClkRateGet());
                
                } while (TRUE);

            if (waitCount == 0)
                USR_USBOTG_DBG("\r\nVBUS FAIL - VBUS is NOT powered off on A-device - %d!\r\n", 
                    uRequestCount);
            else
                USR_USBOTG_DBG("\r\nVBUS PASS - VBUS is powered off on A-device - %d!\r\n", 
                    uRequestCount);

            }
        else
            {
            /* 
             * As B-device, we must wait A-device to power off VBUS 
             */
             
            USR_USBOTG_DBG("\r\nVBUS - Wait VBUS to be powered off by A-device - %d!\r\n", 
                uRequestCount);
            
            /* Wait to be Session End */
            
            while (((sts = usrUsbOtgVbusStateGet(otgFd, &vbusState)) == OK) &&
                    (vbusState == USBOTG_VBUS_STATE_VBusValid))
                taskDelay(sysClkRateGet());

            if (sts != OK)
                {                    
                USR_USBOTG_DBG("usrUsbOtgVbusStateGet fail before power on, exit!\n");

                usrUsbOtgClose(otgFd);
                
                return;
                }

            USR_USBOTG_DBG("\r\nVBUS - Try to request power on VBUS by A-device - %d!\r\n", 
                uRequestCount);

            /* Let's request to power on ... */
            
            sts = usrUsbOtgVbusPowerOn(otgFd, 5000);

            waitCount = 100;
            do
                {
                sts = usrUsbOtgVbusStateGet(otgFd, &vbusState);
                
                if (sts != OK)
                    {                    
                    USR_USBOTG_DBG("usrUsbOtgVbusStateGet fail after power on, exit!\n");

                    usrUsbOtgClose(otgFd);

                    return;
                    }
                
                if (vbusState == USBOTG_VBUS_STATE_VBusValid)
                    break;
                
                waitCount--;
                if (waitCount == 0)
                    break;
                
                taskDelay(sysClkRateGet());
                
                } while (TRUE);

            if (waitCount == 0)
                USR_USBOTG_DBG("\r\nSRP FAIL - VBUS is NOT powered on by A-device - %d!\r\n", 
                    uRequestCount);
            else
                USR_USBOTG_DBG("\r\nSRP PASS - VBUS is powered on by A-device - %d!\r\n", 
                    uRequestCount);

            }

        taskDelay(5 * sysClkRateGet());

        }
    
    usrUsbOtgClose(otgFd);
    }
#endif /* (USB_OTG_TEST_CODE_ENABLED == TRUE) */

#endif /* __INCusrUsbOtgInitc */
