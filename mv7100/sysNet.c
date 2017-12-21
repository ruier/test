/* sysNet.c - system-dependent Network Library */

/*
 * Copyright (c) 2006-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01j,17dec09,e_d  Remove M command support
01i,12jan09,e_d  Add M command support
01h,14jul08,l_z  Support END2 versions of the drivers
01g,02jul07,wap  Switch to new VxBus TSEC drivers
01f,12mar07,wap  Fix byte swap problem
01e,08feb07,dtr  Check for vxWorks rev for byte swapping of MAC address.
01d,20dec06,dtr  Modify device name.
01c,03may06,dtr  SPR#120513 - check status of sysNetMacNVRamAddrGet.
01b,28apr06,dtr  Changing device name to motetsec. SPR#120448.
01a,06mar06,dtr  Initial support for TSEC MAC address for MPC8641.
*/

/*
DESCRIPTION

This module provides BSP functionality to support getting MAC
address from EEPROM.
MAC adddress routines provided by the BSP in this file is:
    sysNetMacNVRamAddrGet()
*/

/*******************************************************************************
*
* sysNetMacNVRamAddrGet - Get interface MAC address
*
*  This routine gets the current MAC address from the
*  Non Volatile RAM, and store it in the ifMacAddr
*  buffer provided by the caller.
*
*  It is not required for the BSP to provide NVRAM to store
*  the MAC address.  Also, some interfaces do not allow
*  the MAC address to be set by software.  In either of
*  these cases, this routine simply returns ERROR.
*
*  Given a MAC address m0:m1:m2:m3:m4:m5, the byte order
*  of ifMacAddr is:
*   m0 @ ifMacAddr
*   m1 @ ifMacAddr + 1
*   m2 @ ifMacAddr + 2
*   m3 @ ifMacAddr + 3
*   m4 @ ifMacAddr + 4
*   m5 @ ifMacAddr + 5
*
* RETURNS: OK, if MAC address available, ERROR otherwise
*
* ERRNO: N/A
*/


STATUS sysNetMacNVRamAddrGet
    (
    char *  ifName,
    int     ifUnit,
    UINT8 * ifMacAddr,
    int     ifMacAddrLen
    )
    {
    STATUS      status = OK;
    VPD_PACKET *etherVpd = NULL;

    if (sysVpdPkts[0] == NULL)
        {
        if (sysMotVpdInit() != OK )
            {
            return ERROR;
            }
        }

    if (ifMacAddr != NULL)
        {
        if (sysMotVpdPktGet (VPD_PID_EA, ifUnit, sysVpdPkts, &etherVpd) !=
            ERROR)
            {
            bcopy ((char *)etherVpd->data, (char *)ifMacAddr, ifMacAddrLen);
            }
        else
            {

            /* VPD get failed */

            status = ERROR;
            }
        }
    else
        {

        /* Invalid parameter */

        status = ERROR;
        }
    return (status);
    }
