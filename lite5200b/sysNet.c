/* sysNet.c - system network interface support library */

/*
 * Copyright (c) 2001, 2003-2008 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01k,06oct08,b_m  remove legacy fei/gei driver support.
01j,17oct07,pmr  remove big endian byte swap code
01i,30jan07,b_m  Add sysFecEnetAddrDefault[] to avoid invalid mac addr.
01h,09jan07,jw   change includes to std drivers
01g,03feb06,pkr  add U_BOOT support
01f,11feb05,j_b  comment out sysEnetAddrGet/Set with 'M' command.
01e,10dec05,k_p  support for 'M' command.
01d,30dec04,k_p  SPR #99747,removed INCLUDE_NETWORK dependencies
01c,11nov03,pkr  added support for fei.
01b,14oct03,pdr  added support for elPci.
01a,21mar01,g_h  written.
*/

/* includes */

#include <vxWorks.h>
#include <taskLib.h>
#include <sysLib.h>
#include "config.h"
#include "sysNet.h"

IMPORT STATUS sysEnetAddrGet(UINT32, UCHAR *);

#ifdef INCLUDE_FEC_END

/* set the following array to a unique Ethernet hardware address */

unsigned char sysFecEnetAddr [6] = {0x00, 0x04, 0x9f, 0x00, 0x5b, 0x7a};
unsigned char sysFecEnetAddrDefault [6] = {0x00, 0x04, 0x9f, 0x00, 0x5b, 0x7a};

IMPORT STATUS sysFecEnetDisable (UINT32 fecBaseaddr);

#include <end.h>

#define FEC_END_CTRL_OFF        0x0024  /* FEC control register */

#endif /* INCLUDE_FEC_END */

#ifdef BOOT_FROM_UBOOT
#   if (UBOOT_BOOT_OPTION == TFTP_BOOT)
int getenv_r (UCHAR *name, UCHAR *buf, unsigned len);
static int envmatch (UCHAR *s1, int i2);
UCHAR env_get_char (int index);
#   endif /* UBOOT_BOOT_OPTION */
#endif /* BOOT_FROM_UBOOT */

#if defined (INCLUDE_FEC_END) && defined (ETHERNET_MAC_HANDLER)

LOCAL UINT8 sysInvalidAddr[2][MAC_ADRS_LEN] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff }
};

const char *sysNetDevName[MAX_MAC_DEVS] = {"fec"};


/*******************************************************************************
*
* sysMacIndex2Unit - convert index range to unit number
*
* This routine converts an index range 0..MAX_MAC_ADRS-1
* to a unit number.
*
* RETURNS: unit number of indexed device
*/

int sysMacIndex2Unit
    (
    int index
    )
    {
    return (0); /* first index is fec */
    }

/*******************************************************************************
*
* sysMacIndex2Dev - convert index range to device string
*
* This routine converts an index range 0..MAX_MAC_ADRS-1 to a device string
* index e.g. motfcc.
*
* RETURNS: index access device name in sysNetDevName
*/

int sysMacIndex2Dev
    (
    int index
    )
    {
    return (0); /* first index is fec */
    }

/*******************************************************************************
*
* sysMacOffsetGet - Calculate table offset
*
* This routine calculates which table entry corresponds to the specified
* interface.
*
* Two values are calculated and returned in the parameters pointed to by
* ppEnet and pOffset.
*
* RETURNS: ERROR if the interface is not known; OK otherwise
*/

STATUS sysMacOffsetGet
    (
    char *  ifName,     /* interface name */
    int     ifUnit,     /* interface unit */
    char ** ppEnet,     /* pointer to glbEnetAddr[][] entry */
    int *   pOffset     /* offset in NVRAM */
    )
    {
    /* search through device names */

    if (strcmp(ifName, sysNetDevName[0]) == 0 )
        {
        if (ifUnit != 0)
            return (ERROR);

        *ppEnet  = (char*)sysFecEnetAddr;
        *pOffset = 0;

        return (OK);
        }

    return (ERROR);
    }

/*******************************************************************************
*
* sysNetMacNVRamAddrGet - Get interface MAC address
*
* This routine gets the current MAC address from the Non Volatile RAM, and
* stores it in the ifMacAddr buffer provided by the caller.
*
* It is not required for the BSP to provide NVRAM to store the MAC address.
* Also, some interfaces do not allow the MAC address to be set by software.
* In either of these cases, this routine simply returns ERROR.
*
* Given a MAC address m0:m1:m2:m3:m4:m5, the byte order
* of ifMacAddr is:
*   m0 @ ifMacAddr
*   m1 @ ifMacAddr + 1
*   m2 @ ifMacAddr + 2
*   m3 @ ifMacAddr + 3
*   m4 @ ifMacAddr + 4
*   m5 @ ifMacAddr + 5
*
* RETURNS: OK, if MAC address available, ERROR otherwise
*/

STATUS sysNetMacNVRamAddrGet
    (
    char *  ifName,
    int     ifUnit,
    UINT8 * ifMacAddr,
    int     ifMacAddrLen
    )
    {
    int   offset;
    char *pEnet;

    /* fetch address line & offset from glbEnetAddr[] table */

    if (sysMacOffsetGet(ifName, ifUnit, &pEnet, &offset) != OK)
        return (ERROR);

#if (NV_RAM_SIZE != NONE)

    /* get MAC address from NvRAM */

    sysNvRamGet (pEnet, ifMacAddrLen, NV_MAC_ADRS_OFFSET);

#endif /* (NV_RAM_SIZE != NONE) */

    if (memcmp(pEnet, sysInvalidAddr[0], MAC_ADRS_LEN) == 0)
        return (ERROR);
    if (memcmp(pEnet, sysInvalidAddr[1], MAC_ADRS_LEN) == 0)
        return (ERROR);

    /* copy MAC address to memory for local use */

    memcpy (ifMacAddr, pEnet, ifMacAddrLen);

    return (OK);
    }

/*******************************************************************************
*
* sysNetMacAddrGet - Get interface MAC address
*
* This routine gets the current MAC address from the network interface, and
* stores it in the ifMacAddr buffer provided by the caller.
*
* If the network interface cannot be queried about the MAC address, this
* routine returns ERROR.
*
* RETURNS: OK, if MAC address available, ERROR otherwise
*
* ERRNO
*/

STATUS sysNetMacAddrGet
    (
    char *  ifName,
    int     ifUnit,
    UINT8 * ifMacAddr,
    int     ifMacAddrLen
    )
    {
    /*
     * None of our interfaces can be queried directly.
     * Return ERROR to indicate that we need to use
     * RAM/NVRAM instead.
     */

    return (ERROR);
    }

/*******************************************************************************
*
* sysNetMacAddrSet - Save interface MAC address
*
* This routine saves the MAC address specified in ifMacAddr to the
* appropriate location in NVRam (if possible) and update the specified
* interface to use the specified MAC address.
*
* If the network interface MAC address cannot be set, this routine returns
* ERROR.
*
* RETURNS: OK, if MAC address available, ERROR otherwise
*
* ERRNO
*/

STATUS sysNetMacAddrSet
    (
    char *  ifName,
    int     ifUnit,
    UINT8 * ifMacAddr,
    int     ifMacAddrLen
    )
    {
    int   offset;
    char *pEnet;

    /* fetch address line & offset from glbEnetAddr[] table */

    if (sysMacOffsetGet(ifName, ifUnit, &pEnet, &offset) != OK)
        return (ERROR);

#if (NV_RAM_SIZE != NONE)

    /* check MAC address in NvRAM */

    sysNvRamGet (pEnet, ifMacAddrLen, NV_MAC_ADRS_OFFSET);
    if (0 == memcmp (ifMacAddr, pEnet, ifMacAddrLen))
        {
        /* same address so don't erase and rewrite flash */

        printf("Address unchanged\n");
        return(OK);
        }

    sysNvRamSet ((char *)ifMacAddr, ifMacAddrLen, NV_MAC_ADRS_OFFSET);

#endif /* (NV_RAM_SIZE != NONE) */

    /* copy mmac address to memory for local use */

    memcpy (ifMacAddr, pEnet, ifMacAddrLen);

    return (OK);
    }

#endif /*  (INCLUDE_FEC_END) && defined (ETHERNET_MAC_HANDLER) */


/*******************************************************************************
*
* sysNetHwInit - initialize the network interface
*
* This routine initializes the network hardware to a quiescent state.  It
* does not connect interrupts.
*
* Only polled mode operation is possible after calling this routine.
* Interrupt mode operation is possible after the memory system has been
* initialized and sysNetHwInit2() has been called.
*
* RETURNS: N/A
*
* SEE ALSO: sysHwInit()
*/

void sysNetHwInit (void)
    {
#ifdef INCLUDE_FEC_END
    /* disable the FEC */
    sysFecEnetDisable ((UINT32)ETH_BASE_ADRS);

    *GPIO_PCR &= ~GPIO_STD_PC_ETH_MASK;
    *GPIO_PCR |= GPIO_STD_PC_ETH_18WIRE;
#endif /* INCLUDE_FEC_END */
    }

#ifdef INCLUDE_FEC_END

/*******************************************************************************
*
* sysFecEnetEnable - enable the MII interface of the Fast Ethernet controller
*
* This routine is expected to perform any target specific functions required
* to enable the Ethernet device and the MII interface of the Fast Ethernet
* controller. None required here
*
* This routine does not enable the 7-wire serial interface.
*
* RETURNS: OK, or ERROR if the Fast Ethernet controller cannot be enabled.
*/

STATUS sysFecEnetEnable
    (
    UINT32  fecBaseAddr /* base address of the on-chip RAM */
    )
    {
    return (OK);
    }

/*******************************************************************************
*
* sysFecEnetDisable - disable MII interface to the Fast Ethernet controller
*
* This routine is expected to perform any target specific functions required
* to disable the Ethernet device and the MII interface to the Fast Ethernet
* controller.  This involves restoring the default values for all the Port
* signals.
*
* RETURNS: OK, always.
*/

STATUS sysFecEnetDisable
    (
    UINT32  fecBaseAddr /* base address of the on-chip RAM */
    )
    {
    int intLevel = intLock();

    volatile UINT32 *FEC_END_CTRL =
        (volatile UINT32 *)((void *)(fecBaseAddr + FEC_END_CTRL_OFF));

    /* disable the Ethernet device for the FEC */

    *FEC_END_CTRL = 0;  /* remove ETHER_EN */

    intUnlock (intLevel);

    return (OK);
    }

/*******************************************************************************
*
* sysFecEnetAddrGet - get the hardware Ethernet address
*
* This routine provides the six byte Ethernet hardware address that will be
* used by each individual Fast Ethernet device unit.  This routine must copy
* the six byte address to the space provided by <addr>.
*
* RETURNS: OK, or ERROR if the Ethernet address cannot be returned.
*/

STATUS sysFecEnetAddrGet
    (
    UINT32  fecBaseAddr,    /* base address of the on-chip RAM */
    UCHAR * addr            /* where to copy the Ethernet address */
    )
    {
#ifdef BOOT_FROM_UBOOT
#   if (UBOOT_BOOT_OPTION == TFTP_BOOT)
    int env_size, env_present = 0, reg, i;
    char *s = NULL, *e, es[] = "11:22:33:44:55:66";
    UCHAR s_env_mac[64], v_env_mac[6];

    env_size = getenv_r ("ethaddr", s_env_mac, sizeof (s_env_mac));
    if ((env_size > 0) && (env_size < sizeof (es)))
        {
        /* exit if env is bad */
        printf ("\n*** ERROR: ethaddr is not set properly in u-boot environment\n");
        printf ("           using default MAC ");
        bcopy ((char *) sysFecEnetAddr, (char *) addr, sizeof (sysFecEnetAddr));
        for (i = 0; i < 6; i++)
            {
            printf("%02x", addr[i]);
            if (i != 5)
                printf(":");
            else
                printf("\n");
            }
        return (OK);
        }

    if (env_size < 0)
        {
        /* exit if env not set */
        printf ("\n*** ERROR: ethaddr is not set in u-boot environment\n");
        printf ("           using default MAC ");
        bcopy ((char *) sysFecEnetAddr, (char *) addr, sizeof (sysFecEnetAddr));
        for (i = 0; i < 6; i++)
            {
            printf("%02x", addr[i]);
            if (i != 5)
                printf(":");
            else
                printf("\n");
            }
        return (OK);
        }

    if (env_size > 0)
        {
        env_present = 1;
        s = s_env_mac;
        }

    for (reg = 0; reg < 6; ++reg)
        {
        /* turn string into mac value */

        v_env_mac[reg] = s ? strtoul (s, &e, 16) : 0;
        if (s)
            s = (*e) ? e + 1 : e;
        }

    bcopy ((char *) v_env_mac, (char *) addr, sizeof (sysFecEnetAddr));

#ifdef MAC_PRINT
    printf("\nMAC = ");
    for (i = 0; i < 6; i++)
        {
        printf("%02x", addr[i]);
        if (i != 5)
            printf(":");
        else
            printf("\n");
        }
#endif
#else
#ifdef ETHERNET_MAC_HANDLER
    if ((sysNetMacNVRamAddrGet ("fec", 0, addr, MAC_ADRS_LEN)) != OK)
#else
    if (sysEnetAddrGet(fecBaseAddr, addr) != OK)
#endif
        {
        bcopy ((char *) sysFecEnetAddrDefault, (char *)addr, sizeof (sysFecEnetAddrDefault));
        printf("Assign a fixed MAC address to prevent problems, using the "
           "bootrom 'M' command.\n");
        }
#endif
#else
#ifdef ETHERNET_MAC_HANDLER
    if ((sysNetMacNVRamAddrGet ("fec", 0, addr, MAC_ADRS_LEN)) != OK)
#else
    if (sysEnetAddrGet(fecBaseAddr, addr) != OK)
#endif
        {
        bcopy ((char *) sysFecEnetAddrDefault, (char *)addr, sizeof (sysFecEnetAddrDefault));
        printf("Assign a fixed MAC address to prevent problems, using the "
           "bootrom 'M' command.\n");
        }
#endif /* BOOT_FROM_UBOOT */

    return (OK);
    }

#endif /* INCLUDE_FEC_END */

#ifdef BOOT_FROM_UBOOT

#   if (UBOOT_BOOT_OPTION == TFTP_BOOT)

/*******************************************************************************
*
* some routines to read u-boot environment in flash rom
*
*/

int getenv_r
    (
    UCHAR *     name,
    UCHAR *     buf,
    unsigned    len
    )
    {
    int i, nxt;

    for (i = 0; env_get_char(i) != '\0'; i = nxt + 1)
        {
        int val, n;

        for (nxt = i; env_get_char(nxt) != '\0'; ++nxt)
            {
            if (nxt >= UBOOT_ENV_SIZE)
                {
                return (-1);
                }
            }

        if ((val = envmatch(name, i)) < 0)
            continue;

        /* found; copy out */

        n = 0;

        while ((len > n++) && (*buf++ = env_get_char(val++)) != '\0');

        if (len == n)
            *buf = '\0';

        return (n);
        }

    return (-1);
    }

UCHAR env_get_char
    (
    int index
    )
    {
    return (*((UCHAR *)(UBOOT_ENV + index)));
    }

/*******************************************************************************
*
* Match a name / name=value pair
*
* s1 is either a simple 'name', or a 'name=value' pair.
* i2 is the environment index for a 'name2=value2' pair.
* If the names match, return the index for the value2, else NULL.
*/

LOCAL int envmatch
    (
    UCHAR * s1,
    int     i2
    )
    {
    while (*s1 == env_get_char(i2++))
        if (*s1++ == '=')
            return (i2);

    if (*s1 == '\0' && env_get_char(i2 - 1) == '=')
        return (i2);

    return (-1);
    }

#   endif /* UBOOT_BOOT_OPTION */

#endif /* BOOT_FROM_UBOOT */
