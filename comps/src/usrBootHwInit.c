/* usrBootHwInit.c - hardware initialization module callded when systerm bootup */
  
/*
 * Copyright 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01a,26feb13,s_z  written.
*/

/*
DESCRIPTION

This module is used to initialize the hardware resource when the system boot up.

usrBootHwInit() routine is provided to initialize the needed hardware early in the
boot process. Right now, only UHCI reset is included in this routine and only 
for IA BSPs, more modules may be included here in the future.

usrBootHwInit[n]() routines can be added in this module and put in the right place if needed.
*/

/* includes */

#include <vxWorks.h>
#include <drv/pci/pciConfigLib.h>

/* Only used for IA arch */

#if (CPU_FAMILY == I80X86) && (defined(INCLUDE_BOOT_UHCI_RESET))

/* define */

#define USER_BOOT_UHCI_USBCMD_HCRESET    (1) 

#define USER_BOOT_PCI_CONFIG_BDF_PACK(busNo, deviceNo, funcNo) \
                     ((((busNo)    << 16) & 0x00ff0000) |     \
                      (((deviceNo) << 11) & 0x0000f800) |     \
                      (((funcNo)   << 8)  & 0x00000700))

/* extern */

extern void sysUsDelay (int counter);
extern UCHAR sysInByte (_WRS_IOLONG port);
extern ULONG sysInLong (_WRS_IOLONG port);
extern void	sysOutByte (_WRS_IOLONG port, _WRS_IOCHAR data);
extern void	sysOutLong (_WRS_IOLONG port, _WRS_IOLONG data);


/*******************************************************************************
*
* usrBootPciConfigRegReadLong - read the pci configuration registers
*
* This routine is used to read the pci configuration registers
*
* RETURNS: OK
*/

LOCAL STATUS usrBootPciConfigRegReadLong
    (
    int      busNo,     /* bus number */
    int	     deviceNo,  /* device number */
    int	     funcNo,    /* function number */
    int	     offset,    /* offset into the configuration space */
    UINT32 * pData      /* data read from the offset */
    )
    {
    sysOutLong(PCI_CONFIG_ADDR,
               USER_BOOT_PCI_CONFIG_BDF_PACK(busNo, deviceNo, funcNo) |
               (offset & 0xfc) | 0x80000000 );
    
    *(UINT32*)pData = sysInLong (PCI_CONFIG_DATA);
    
    return OK;
    }                                     

/*******************************************************************************
*
* usrBootUhciReset - resets the USB UHCI controllers
*
* This routine is used to reset the UHCI controller on some target like D2500, 
* to avoid hang up. Some targets have BIOS bugs that will leave the UHCI 
* controller in a bad state. 
*
* This routine will search and reset all the UHCI controllers on PCI Bus 0.
*
* RETURNS: OK if applied or no need to apply, or ERROR if failed.
*/

LOCAL STATUS usrBootUhciReset 
    (
    void
    )
    {
    UINT32 uClassType;
    UINT32 uAddrBase4;
    UINT8  uDeviceNum;
    UINT8  uFuncIndex;
    UINT8  uByte;
             
    /* Search all the UHCI controllers on Bus 0, no need check other BUS */
     
    for (uDeviceNum = 0; uDeviceNum < PCI_MAX_DEV; uDeviceNum ++)
        {
        for (uFuncIndex = 0; uFuncIndex < 8; uFuncIndex ++)
            {
            uClassType = 0;
            
            /* 
             * Try to get the Class/Subclass/Prog Interface code 
             * Check if it is UHCI with 0C/03/00
             */
            
            if ((usrBootPciConfigRegReadLong (0, uDeviceNum, uFuncIndex, 
                                              PCI_CFG_REVISION, 
                                              &uClassType) != OK) ||
                 ((uClassType & 0xFFFFFF00) != 0x0C030000))
                {
                continue;
                }
                 
            /*
             * Check base address 4.
             */

            if (usrBootPciConfigRegReadLong (0, uDeviceNum, uFuncIndex, 
                                             PCI_CFG_BASE_ADDRESS_4,
                                             &uAddrBase4) != OK)
                {
                continue;
                }

            uAddrBase4 &= 0xfffffff0;
        
            uByte = sysInByte(uAddrBase4);
            
            sysOutByte(uAddrBase4, (1 << USER_BOOT_UHCI_USBCMD_HCRESET));
            
            /* 10ms per UHCI spec */
             
            sysUsDelay(10000);   

            /* Restore the original value */

            sysOutByte(uAddrBase4, uByte);
            }
        }
    return (OK);
    }
    
#endif /* #if (CPU_FAMILY == I80X86) && ... */    


/*******************************************************************************
*
* usrBootHwInit - hardware initialization when system bootup
*
* This routine is used to initialize the required hardware just after the 
* system bootup.
*
* RETURNS: N/A.
*/

void usrBootHwInit
    (
    void
    )
    {
#if (CPU_FAMILY == I80X86) && (defined(INCLUDE_BOOT_UHCI_RESET))
    usrBootUhciReset();
#endif /* #if (CPU_FAMILY == I80X86) && ...*/
    
    }
