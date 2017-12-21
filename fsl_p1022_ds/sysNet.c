/* sysNet.c - system-dependent Network Library */

/*
 * Copyright (c) 2010, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,06apr12,d_l  update for VxBus I2C driver.
01a,17aug10,e_d  derived from fsl_p2020_ds/sysNet.c
*/

/*
DESCRIPTION

This board provides storage in Eeprom for the MAC addresses
of the motetsec interfaces.
*/


#include <vxWorks.h>
#include "config.h"
#include <hwif/vxbus/vxbI2cLib.h>

/* locals */

/* defines */

/*
 * For Freescale P1022DS board, it stored the mac address in SystemId eeprom
 * at SYSTEMID_MAC_OFFSET offset. So we get the mac address using VxBus I2c
 * driver when net device is initialized.
 * The detail of the system ID is described in application note
 * AN3638 : The SystemID Format for Power Architecture.
 */

#define SYSTEMID_MAC_OFFSET    0x42

/* globals */

IMPORT int dynamicMacAddrGen
    (
    UINT8 * ifName,         /* interface name */
    int     ifUnit,         /* interface unit */
    UINT8 * ifMacAddr,      /* address buffer */
    int     ifMacAddrLen    /* length of buffer */
    );

/* locals */

IMPORT FUNCPTR _func_logMsg;

STATUS sysNetMacNVRamAddrGet
    (
    char *  ifName,
    int     ifUnit,
    UINT8 * ifMacAddr,
    int     ifMacAddrLen
    )
    {
    STATUS  status = OK;
    UINT32  offset;

    offset = SYSTEMID_MAC_OFFSET + (ifUnit * MAC_ADRS_LEN);

    if (ifMacAddr != NULL)
        {
        status = vxbI2cByNameRead("eeprom_at24c02",
                    2,      /* EEPROM unit number */
                    offset,
                    ifMacAddr,
                    ifMacAddrLen
                    );
        if (status != OK)
            {
            if (_func_logMsg != NULL)
                _func_logMsg("Failed getting mac address from SystemId EEPROM\r\n", 
                          0,0,0,0,0,0);
            }
        }
    return (status);
    }


