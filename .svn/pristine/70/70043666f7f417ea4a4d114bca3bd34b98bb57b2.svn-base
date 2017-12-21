/* vxbMsgSupport.h - message transmit support library */

/*
 * Copyright (c) 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use
 * of this software may be licensed only pursuant to the terms
*  of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,20dec12,j_z  change MSG_RXCB_FUNC, MSG_TXCB_FUNC prototype 
01a,30nov12,j_z  add common message transmit struct.
*/

/*
DESCRIPTION
This module provides common message transmit API definitions.
*/

#ifndef __INCvxbMsgSupportH
#define __INCvxbMsgSupportH

typedef struct msgbuf
    {
    void  * buf;
    UINT32  len;
    void  * next;
    } MSG_TRAN_BUF;

typedef int (*MSG_RXCB_FUNC) (MSG_TRAN_BUF *, STATUS status, void * cbArg);
typedef int (*MSG_TXCB_FUNC) (STATUS status, void * cbArg);

typedef struct msg_trans_api
    {
    int (*intRxAddHook) (VXB_DEVICE_ID pServDev, ULONG arg,
        MSG_RXCB_FUNC rxCbFunc, void * cbArg);
    int (*intSend) (VXB_DEVICE_ID pServDev, ULONG arg,
        MSG_TRAN_BUF * desc, MSG_TXCB_FUNC txCbFunc, void * cbArg);
    int (*pollSend) (VXB_DEVICE_ID pServDev, void * arg, MSG_TRAN_BUF * desc);
    int (*pollRcv) (VXB_DEVICE_ID pServDev, void * arg, void * buf);
    int (*pollIntSwitch) (VXB_DEVICE_ID pServDev, void * arg);
    } MSG_TRANS_API;

#endif /* __INCvxbMsgSupportH */
