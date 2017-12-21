/* sysTffs.c - Wind River SBC PowerQUICCII system-dependent TrueFFS library */

/*
 * Copyright (c) 2003-2007, 2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01u,30aug12,clx  fix unaligned SIMM write issue. (WIND00071923)
01t,26apr07,jlk  Defect WIND00093533: Bug in PQIIsimmWrite()
01s,14feb07,h_k  fixed NULL pointer access in PQIIsimmMTDErase. (CQ:88626)
01r,10nov05,dee  Fix SPR#113227, define HALF_FORMAT when
                 BOOT_FLASH==TFFS_FLASH
01q,28sep05,dee  SPR#112977 - cleanup file compile warnings
01p,05may05,j_b  fix dependency generation error (SPR# 108867)
01o,11may05,jb3  Only overwrite in PQIIsimmMTDWrite if flWriteFault.
01m,28apr05,jb3  Add overwrite mode to PQIIsimmMTDWrite.
01n,24nov04,j_b  bypass TFFS flash selection for DOC build; remove printf
01m,10nov04,j_b  project support for TFFS flash selection (SPR 103790)
01l,07oct04,j_b  fix status checking in PQIIsimmMTDErase() (SPR 92186)
01k,23sep04,mdo  Documentation fixes for apigen
01j,12apr04,jb3  INCLUDE TFFS hangs the target spr 95355.
01i,01dec03,j_b  fix unaligned SIMM writes (SPR 92231)
01h,26nov03,j_b  remove unnecessary, confusing line of code (SPR #92231)
01g,06oct03,j_b  Flash selection done in config.h
01f,12sep03,jb3  added cache pipe flushes for resident Flash array
01e,12sep03,jb3  undefine DEBUG_PRINT
01d,20aug03,jb3  resident Flash array support.
01c,08aug03,jb3  remove one last debug print!.
01b,08aug03,jb3  clean up warnings.
01a,14may03,scm ported from wrSbc8260 BSP.
*/

/*
DESCRIPTION
This library provides board-specific hardware access routines for TrueFFS.
In effect, these routines comprise the socket component drivers for the
Wind River SBC PowerQUICCII on board and SIMM flash devices.  At socket
registration time, TrueFFS stores pointers to the functions of this socket
component driver in an 'FLSocket' structure.  When TrueFFS needs to access
the flash device, it uses these functions.

Because this file is, for the most part, a device driver that exports its
functionality by registering function pointers with TrueFFS, very few of the
functions defined here are externally callable.  For the record, these
external functions are flFitInSocketWindow() and flDelayLoop().  You should
never have any need to call these functions.

However, one of the most import functions defined in this file is neither
referenced in an 'FLSocket' structure, nor is it externally callable.  This
function is sysTffsInit().  TrueFFS calls this function at initialization
time to register socket component drivers for all the flash devices attached
to your target.  It is this call to sysTffs() that results in assigning
drive numbers to the flash devices on your target hardware.  Drive numbers
are assigned by the order in which the socket component drivers are registered.
The first to be registered is drive 0, the second is drive 1, and so on up to
4.  As shipped, TrueFFS supports up to five flash drives.

After registering socket component drivers for a flash device, you may
format the flash medium even though there is not yet a block device driver
associated with the flash (see the reference entry for the tffsDevCreate()
routine).  To format the flash medium for use with TrueFFS,
call tffsDevFormat() or, for some BSPs, sysTffsFormat().

The sysTffsFormat() routine is an optional, but BSP-specific, externally
callable helper function.  Internally, it calls tffsDevFormat() with a
pointer to a 'FormatParams' structure initialized to values that leave a
space on the flash device for a boot image. This space is outside the
region managed by TrueFFS.  This special region is necessary for boot
images because the normal translation and wear-leveling services of TrueFFS
are incompatible with the needs of the boot program and the boot image it
relies upon.  To write a boot image (or any other data) into this area,
use tffsBootImagePut().

Finally, this file also contains define statements for symbolic constants
that determine which Memory Technology Drivers (MTDs), translation layer
modules and other utilities are ultimately included in TrueFFS:

\is
\i INCLUDE_TL_NFTL
To include the NAND-based translation layer module.
\i INCLUDE_TL_FTL
To include the NOR-based translation layer module.
\i INCLUDE_TL_SSFDC
To include the SSFDC-appropriate translation layer module.
\i INCLUDE_MTD_I28F016
For Intel 28f016 flash devices.
\i INCLUDE_MTD_I28F008
For Intel 28f008 flash devices.
\i INCLUDE_MTD_I28F008_BAJA
For Intel 28f008 flash devices on the Heurikon Baja 4700.
\i INCLUDE_MTD_AMD
For AMD, Fujitsu: 29F0{40,80,16} 8-bit flash devices.
\i INCLUDE_MTD_CDSN
For Toshiba, Samsung: NAND CDSN flash devices.
\i INCLUDE_MTD_DOC2
For Toshiba, Samsung: NAND DOC flash devices.
\i INCLUDE_MTD_CFISCS
For CFI/SCS flash devices.
\i INCLUDE_MTD_WAMD
For AMD, Fujitsu 29F0{40,80,16} 16-bit flash devices.
\i INCLUDE_MTD_USR
For user-defined MTDs.
\i INCLUDE_TFFS_BOOT_IMAGE
To include tffsBootImagePut() in TrueFFS for Tornado.
\ie

To exclude any of the modules mentioned above, edit sysTffs.c and undefine
its associated symbolic constant.

The function sysTffsFormat() is provided so one might easily structure the
Flash with some reserved memory (for a boot image, for example) and the TFFS
file system in the rest.  The macro HALF_FORMAT provides a switch for providing
reserved memory.

INCLUDE FILES: flsocket.h, tffsDrv.h
*/


/* includes */

#include <vxWorks.h>
#include <taskLib.h>
#include "config.h"
#include <tffs/flsocket.h>
#include <tffs/tffsDrv.h>

#include <tffs/flflash.h>
#include <tffs/backgrnd.h>
#include <cacheLib.h>

#ifdef INCLUDE_TFFS  /* prevent dependency generation error when not included */

/* defines */


#define INCLUDE_MTD_USR 	/* custom MTD layer */

#ifndef PRJ_BUILD
#undef	INCLUDE_MTD_I28F016	/* Intel: 28f016 */
#undef	INCLUDE_MTD_I28F008	/* Intel: 28f008 */
#undef	INCLUDE_MTD_AMD 	/* AMD, Fujitsu: 29f0{40,80,16} 8bit */
#undef	INCLUDE_MTD_CDSN	/* Toshiba, Samsung: NAND, CDSN */
#undef	INCLUDE_MTD_DOC2	/* Toshiba, Samsung: NAND, DOC */
#undef	INCLUDE_MTD_CFISCS	/* CFI/SCS */
#undef	INCLUDE_MTD_WAMD	/* AMD, Fujitsu: 29f0{40,80,16} 16bit */
#undef	INCLUDE_TL_NFTL 	/* NFTL translation layer */
#define INCLUDE_TL_FTL		/* FTL translation layer */
#undef	INCLUDE_TL_SSFDC	/* SSFDC translation layer */
#endif /* PRJ_BUILD */

#define INCLUDE_SOCKET_SIMM	/* SIMM socket interface */
#undef	INCLUDE_SOCKET_PCMCIA	/* PCMCIA socket interface */

#undef INCLUDE_TFFS_BOOT_IMAGE /* include tffsBootImagePut() */

/*
   IF mounting TFFS in the flash boot device HALF_FLASH must be defined,
   this will cause the TFFS to skip the hardware configuration word in 
   the selected on board Flash device.	The upper limit of the on board
   Flash device is faked in the PQIIrfaIdentify() routine to reserve the
   upper half for the bootrom image.  See the following table for options.

	    |  Booting from  |	TFFS in  |  HALF_FORMAT |
   Default -   on board 	SIMM	    undefined
	       on board 	on board    defined
	       SIMM		on board    undefined
	       SIMM		SIMM	    defined
*/
#undef HALF_FORMAT    /* reserve an area of low memory */

/* Flash Selection */
#ifdef INCLUDE_ON_BOARD_TFFS_FLASH
#  undef TFFS_FLASH
#  define TFFS_FLASH	     ON_BOARD_FLASH
#elif defined INCLUDE_SIMM_TFFS_FLASH
#  undef TFFS_FLASH
#  define TFFS_FLASH	     SIMM_FLASH
#endif

#if (TFFS_FLASH == BOOT_FLASH)
#  define HALF_FORMAT     /* adjust start and end of TFFS format */
#endif

#if (TFFS_FLASH == ON_BOARD_FLASH)
#  define MTD_USR_IDENTIFY    PQIIrfaIdentify  /* On-board Flash */
#  define FLASH_BASE_ADRS     CS0_FLASH_ADRS /* Flash memory base address */
#  define FLASH_SIZE	      CS0_FLASH_SIZE /* Flash memory size */
#elif (TFFS_FLASH == SIMM_FLASH)
#  define MTD_USR_IDENTIFY    PQIIsimmMTDIdentify  /* SIMM Flash */
#  define FLASH_BASE_ADRS     CS6_FLASH_ADRS /* Flash memory base address */
#  define FLASH_SIZE	      CS6_FLASH_SIZE /* Flash memory size */
#else
#  ifndef DOC
#    error TFFS Flash must be selected
#  endif /* DOC */
#endif /* TFFS_FLASH */

#define TFFS_FLASH_SIZE  FLASH_SIZE

#define VCC_DELAY_MSEC	    100 	   /* Millisecs to wait for Vcc ramp up */
#define VPP_DELAY_MSEC	    100 	   /* Millisecs to wait for Vpp ramp up */

#ifdef	INCLUDE_SOCKET_PCMCIA
#define BCSR1_PCCVPP_12V    0x00200000
#endif

#define KILL_TIME_FUNC	    ((iz * iz) / (iz + iz)) / ((iy + iz) / (iy * iz))


/* locals */
#ifdef	INCLUDE_SOCKET_PCMCIA
LOCAL UINT32 sysTffsMsecLoopCount = 0;
#endif	/* INCLUDE_SOCKET_PCMCIA */

/* externals */
IMPORT int flFileSysSectorStart;  /* protects overwriting TFFS at sector 0 */

/* forward declarations */
#ifdef	INCLUDE_SOCKET_SIMM
LOCAL FLBoolean simmCardDetected (FLSocket vol);
LOCAL void	simmVccOn (FLSocket vol);
LOCAL void	simmVccOff (FLSocket vol);

#ifdef	SOCKET_12_VOLTS
LOCAL FLStatus	simmVppOn (FLSocket vol);
LOCAL void	simmVppOff (FLSocket vol);
#endif	/* SOCKET_12_VOLTS */

LOCAL FLStatus	simmInitSocket (FLSocket vol);
LOCAL void	simmSetWindow (FLSocket vol);
LOCAL void	simmSetMappingContext (FLSocket vol, unsigned page);
LOCAL FLBoolean simmGetAndClearCardChangeIndicator (FLSocket vol);
LOCAL FLBoolean simmWriteProtected (FLSocket vol);
LOCAL void	simmRegister (void);
#endif	/* INCLUDE_SOCKET_SIMM */

#ifdef	INCLUDE_SOCKET_PCMCIA
LOCAL FLBoolean pcCardDetected (FLSocket vol);
LOCAL void	pcVccOn (FLSocket vol);
LOCAL void	pcVccOff (FLSocket vol);

#ifdef	SOCKET_12_VOLTS
LOCAL FLStatus	pcVppOn (FLSocket vol);
LOCAL void	pcVppOff (FLSocket vol);
#endif	/* SOCKET_12_VOLTS */

LOCAL FLStatus	pcInitSocket (FLSocket vol);
LOCAL void	pcSetWindow (FLSocket vol);
LOCAL void	pcSetMappingContext (FLSocket vol, unsigned page);
LOCAL FLBoolean pcGetAndClearCardChangeIndicator (FLSocket vol);
LOCAL FLBoolean pcWriteProtected (FLSocket vol);
LOCAL void	pcRegister (void);
#endif	/* INCLUDE_SOCKET_PCMCIA */

#ifndef DOC
#include <tffs/tffsConfig.c>
/* #include <cfiscs.c> */
#endif /* DOC */

/*******************************************************************************
*
* sysTffsInit - board level initialization for TFFS
*
* This routine calls the socket registration routines for the socket component
* drivers that will be used with this BSP. The order of registration signifies
* the logical drive number given to the drive associated with the socket.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void sysTffsInit
    (
    void
    )
    {
#ifdef	INCLUDE_SOCKET_PCMCIA
    UINT32 ix = 0;
    UINT32 iy = 1;
    UINT32 iz = 2;
    int oldTick;
#endif	/* INCLUDE_SOCKET_PCMCIA */

#ifdef	INCLUDE_SOCKET_SIMM
    simmRegister ();		  /* SIMM socket interface register */
#endif	/* INCLUDE_SOCKET_SIMM */

#ifdef	INCLUDE_SOCKET_PCMCIA
    /* we assume following:
     *	 - no interrupts except timer is happening.
     *	 - the loop count that consumes 1 msec is in 32 bit.
     *	 - it is done in the early stage of usrRoot() in tffsDrv().
     */
    oldTick = tickGet ();
    while (oldTick == tickGet ()) /* wait for next clock interrupt */
    ;

    oldTick = tickGet ();
    while (oldTick == tickGet ()) /* loop one clock tick */
	{
	iy = KILL_TIME_FUNC;	      /* consume time */
	ix++;			      /* increment the counter */
	}
    sysTffsMsecLoopCount = ix * sysClkRateGet () / 1000;
    pcRegister ();		  /* PCMCIA socket interface register */
#endif	/* INCLUDE_SOCKET_PCMCIA */
    }

#ifdef	INCLUDE_SOCKET_SIMM
/*******************************************************************************
*
* simmRegister - install routines for the Flash SIMM
*
* This routine installs necessary functions for the Flash SIMM.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void simmRegister
    (
    void
    )
    {
    FLSocket vol = flSocketOf (noOfDrives);

#if (TFFS_FLASH_SIZE == FLASH_SIZE_2MB)
    tffsSocket[noOfDrives] = "OnBoard";
#else /* On Board FLASH */
    tffsSocket[noOfDrives] = "SIMM";
#endif /* (CS6_FLASH_SIZE == FLASH_SIZE_2MB) */

    vol.window.baseAddress = (unsigned int)(FLASH_BASE_ADRS >> 12);
    vol.cardDetected =	     simmCardDetected;
    vol.VccOn = 	     simmVccOn;
    vol.VccOff =	     simmVccOff;

#ifdef SOCKET_12_VOLTS
    vol.VppOn = 	     simmVppOn;
    vol.VppOff =	     simmVppOff;
#endif

    vol.initSocket =	     simmInitSocket;
    vol.setWindow =	     simmSetWindow;
    vol.setMappingContext =  simmSetMappingContext;
    vol.getAndClearCardChangeIndicator = simmGetAndClearCardChangeIndicator;
    vol.writeProtected =     simmWriteProtected;
    noOfDrives++;
    }

/*******************************************************************************
*
* simmCardDetected - detect if a card is present (inserted)
*
* This routine detects if a card is present (inserted).
*
* RETURNS: TRUE, or FALSE if the card is not present.
*
* ERRNO
*/

LOCAL FLBoolean simmCardDetected
    (
    FLSocket vol	/* pointer identifying drive */
    )
    {
    return (TRUE);
    }

/*******************************************************************************
*
* simmVccOn - turn on Vcc (3.3/5 Volts)
*
* This routine turns on Vcc (3.3/5 Volts).  Vcc must be known to be good
* on exit.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void simmVccOn
    (
    FLSocket vol	/* pointer identifying drive */
    )
    {
    }

/*******************************************************************************
*
* simmVccOff - turn off Vcc (3.3/5 Volts)
*
* This routine turns off Vcc (3.3/5 Volts).
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void simmVccOff
    (
    FLSocket vol	/* pointer identifying drive */
    )
    {
    }

#ifdef SOCKET_12_VOLTS
/*******************************************************************************
*
* simmVppOn - turns on Vpp (12 Volts)
*
* This routine turns on Vpp (12 Volts). Vpp must be known to be good on exit.
*
* RETURNS: flOK always.
*
* ERRNO
*/

LOCAL FLStatus simmVppOn
    (
    FLSocket vol	/* pointer identifying drive */
    )
    {
    return (flOK);
    }

/*******************************************************************************
*
* simmVppOff - turns off Vpp (12 Volts)
*
* This routine turns off Vpp (12 Volts).
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void simmVppOff
    (
    FLSocket vol	/* pointer identifying drive */
    )
    {
    }
#endif	/* SOCKET_12_VOLTS */

/*******************************************************************************
*
* simmInitSocket - perform all necessary initializations of the socket
*
* This routine performs all necessary initializations of the socket.
*
* RETURNS: flOK always.
*
* ERRNO
*/

LOCAL FLStatus simmInitSocket
    (
    FLSocket vol	/* pointer identifying drive */
    )
    {
    return (flOK);
    }

/*******************************************************************************
*
* simmSetWindow - set current window attributes, Base address, size, etc
*
* This routine sets current window hardware attributes: Base address, size,
* speed and bus width.	The requested settings are given in the 'vol.window'
* structure.  If it is not possible to set the window size requested in
* 'vol.window.size', the window size should be set to a larger value,
* if possible. In any case, 'vol.window.size' should contain the
* actual window size (in 4 KB units) on exit.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void simmSetWindow
    (
    FLSocket vol	/* pointer identifying drive */
    )
    {
    /* Physical base as a 4K page */

    vol.window.baseAddress = (unsigned int)(FLASH_BASE_ADRS >> 12);

    flSetWindowSize (&vol, FLASH_SIZE >> 12);
    }

/*******************************************************************************
*
* simmSetMappingContext - sets the window mapping register to a card address
*
* This routine sets the window mapping register to a card address.
* The window should be set to the value of 'vol.window.currentPage',
* which is the card address divided by 4 KB. An address over 128MB,
* (page over 32K) specifies an attribute-space address.
*
* The page to map is guaranteed to be on a full window-size boundary.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void simmSetMappingContext
    (
    FLSocket vol,	/* pointer identifying drive */
    unsigned page	/* page to be mapped */
    )
    {
    }

/*******************************************************************************
*
* simmGetAndClearCardChangeIndicator - return the hardware card-change indicator
*
* This routine returns the hardware card-change indicator and clears it if set.
*
* RETURNS: FALSE, or TRUE if the card has been changed
*
* ERRNO
*/

LOCAL FLBoolean simmGetAndClearCardChangeIndicator
    (
    FLSocket vol	/* pointer identifying drive */
    )
    {
    return (FALSE);
    }

/*******************************************************************************
*
* simmWriteProtected - return the write-protect state of the media
*
* This routine returns the write-protect state of the media
*
* RETURNS: FALSE, or TRUE if the card is write-protected
*
* ERRNO
*/

LOCAL FLBoolean simmWriteProtected
    (
    FLSocket vol	/* pointer identifying drive */
    )
    {
    return (FALSE);
    }
#endif	/* INCLUDE_SOCKET_SIMM */

#ifdef	INCLUDE_SOCKET_PCMCIA
/*******************************************************************************
*
* pcRegister - install routines for the PCMCIA Flash Card
*
* This routine installs necessary functions for the PCMCIA Flash Card.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void pcRegister
    (
    void
    )
    {
    FLSocket vol = flSocketOf (noOfDrives);

    tffsSocket[noOfDrives] = "PCMCIA";
    vol.window.baseAddress = (PC_BASE_ADRS_0) >> 12;
    vol.cardDetected =	     pcCardDetected;
    vol.VccOn = 	     pcVccOn;
    vol.VccOff =	     pcVccOff;

#ifdef SOCKET_12_VOLTS
    vol.VppOn = 	     pcVppOn;
    vol.VppOff =	     pcVppOff;
#endif

    vol.initSocket =	    pcInitSocket;
    vol.setWindow =	    pcSetWindow;
    vol.setMappingContext = pcSetMappingContext;
    vol.getAndClearCardChangeIndicator = pcGetAndClearCardChangeIndicator;
    vol.writeProtected =    pcWriteProtected;
    noOfDrives++;
    }

/*******************************************************************************
*
* pcCardDetected - detect if a card is present (inserted)
*
* This routine detects if a card is present (inserted).
*
* RETURNS: TRUE, or FALSE if the card is not present.
*
* ERRNO
*/

LOCAL FLBoolean pcCardDetected
    (
    FLSocket vol	/* pointer identifying drive */
    )
    {
    int immrVal = vxImmrGet();

    if ((*PIPR (immrVal) & 0x18000000) == 0)
	return(TRUE);
    else
	return(FALSE);
    }

#ifdef	INCLUDE_SOCKET_PCMCIA
/*******************************************************************************
*
* pcVccOn - turn on Vcc (3.3/5 Volts)
*
* This routine turns on Vcc (3.3/5 Volts). Vcc must be known to be good
* on exit.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void pcVccOn
    (
    FLSocket vol	/* pointer identifying drive */
    )
    {
    *BCSR1 &= ~(BCSR1_PCCARD_VCCON_L);	/* on PC card VCC */
    flDelayMsecs (VCC_DELAY_MSEC);  /* wait for Vcc to ramp up */
    }

/*******************************************************************************
*
* pcVccOn - turn off Vcc (3.3/5 Volts)
*
* This routine turns off Vcc (3.3/5 Volts).
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void pcVccOff
    (
    FLSocket vol	/* pointer identifying drive */
    )
    {
    *BCSR1 |= (BCSR1_PCCARD_VCCON_L);	/* off PC card VCC */
    }

#ifdef SOCKET_12_VOLTS
/*******************************************************************************
*
* pcVppOn - turn on Vpp (12 Volts)
*
* This routine turns on Vpp (12 Volts). Vpp must be known to be good on exit.
*
* RETURNS: flOK always
*
* ERRNO
*/

LOCAL FLStatus pcVppOn
    (
    FLSocket vol	/* pointer identifying drive */
    )
    {
    *BCSR1 = (*BCSR1 & ~BCSR1_PCCVPP_MSK) | BCSR1_PCCVPP_12V;
    flDelayMsecs (VPP_DELAY_MSEC);  /* wait for Vpp to ramp up */
    return (flOK);
    }

/*******************************************************************************
*
* pcVppOff - turn off Vpp (12 Volts)
*
* This routine turns off Vpp (12 Volts).
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void pcVppOff
    (
    FLSocket vol	/* pointer identifying drive */
    )
    {
    *BCSR1 = (*BCSR1 & ~BCSR1_PCCVPP_MSK) | BCSR1_PCCVPP_MSK;
    }
#endif	/* SOCKET_12_VOLTS */

/*******************************************************************************
*
* pcInitSocket - perform all necessary initializations of the socket
*
* This routine performs all necessary initializations of the socket.
*
* RETURNS: flOK always
*
* ERRNO
*/

LOCAL FLStatus pcInitSocket
    (
    FLSocket vol	/* pointer identifying drive */
    )
    {
    int immrVal = vxImmrGet();

#if FALSE
    printf ("-- pcInitSocket --\n");
    printf ("immrVal=0x%x\n", immrVal);
    printf ("BR0=0x%x, OR0=0x%x\n", *BR0(immrVal), *OR0(immrVal));
    printf ("BR1=0x%x, OR1=0x%x\n", *BR1(immrVal), *OR1(immrVal));
    printf ("BR2=0x%x, OR2=0x%x\n", *BR2(immrVal), *OR2(immrVal));
    printf ("BR3=0x%x, OR3=0x%x\n", *BR3(immrVal), *OR3(immrVal));
    printf ("BR4=0x%x, OR4=0x%x\n", *BR4(immrVal), *OR4(immrVal));
    printf ("BR5=0x%x, OR5=0x%x\n", *BR5(immrVal), *OR5(immrVal));
    printf ("BR6=0x%x, OR6=0x%x\n", *BR6(immrVal), *OR6(immrVal));
    printf ("BR7=0x%x, OR7=0x%x\n", *BR7(immrVal), *OR7(immrVal));
#endif	/* FALSE */

    *BR4(immrVal) = (PC_BASE_ADRS_1 & BR_BA_MSK) | BR_V;
    *OR4(immrVal) = 0xfc000000 | OR_BI | OR_SCY_8_CLK | OR_TRLX | 0x02;

    /* clear PCMCIA base registers */

    *PBR0 (immrVal) = PC_BASE_ADRS_0;
    *PBR1 (immrVal) = PC_BASE_ADRS_1;
    *PBR2 (immrVal) = 0x0;
    *PBR3 (immrVal) = 0x0;
    *PBR4 (immrVal) = 0x0;
    *PBR5 (immrVal) = 0x0;
    *PBR6 (immrVal) = 0x0;
    *PBR7 (immrVal) = 0x0;

    /* clear PCMCIA option registers */

    *POR0 (immrVal) = 0xf0000000 |   /* bank size:     1MB */
		      0x00020000 |   /* strobe hold:   2 clocks */
		      0x00001000 |   /* strobe setup:  1 clock */
		      0x00000300 |   /* strobe length: 6 clocks */
		      0x00000040 |   /* port size:     2 byte */
		      0x00000010 |   /* region select: attribute */
		      0x00000000 |   /* slot ID:       slot-A */
		      0x00000000 |   /* write protect: off */
		      0x00000001;    /* PCMCIA valid:  1 */

    *POR1 (immrVal) = 0xa8000000 |   /* bank size:     32MB */
		      0x00020000 |   /* strobe hold:   2 clocks */
		      0x00001000 |   /* strobe setup:  1 clock */
		      0x00000300 |   /* strobe length: 6 clocks */
		      0x00000040 |   /* port size:     2 byte */
		      0x00000000 |   /* region select: common mem */
		      0x00000000 |   /* slot ID:       slot-A */
		      0x00000000 |   /* write protect: off */
		      0x00000001;    /* PCMCIA valid:  1 */

    *POR2 (immrVal) = 0x0;
    *POR3 (immrVal) = 0x0;
    *POR4 (immrVal) = 0x0;
    *POR5 (immrVal) = 0x0;
    *POR6 (immrVal) = 0x0;
    *POR7 (immrVal) = 0x0;

    *PSCR (immrVal) = 0x0;

    *PGCRA (immrVal) = 0x0;		/* XXX disable interrupt */
    *PGCRB (immrVal) = 0x0;		/* XXX disable interrupt */

    *BCSR1 &= ~(BCSR1_PCCARD_EN_L);	/* enable the PCMCIA */
    *BCSR1 |= BCSR1_PCCVPP_MSK; 	/* setup PC card Vpp to Hi-Z */

    *BCSR1 &= ~(BCSR1_PCCARD_VCCON_L);	/* on PC card VCC */
    flDelayMsecs (VCC_DELAY_MSEC);	/* wait for Vcc to ramp up */
    *PGCRA (immrVal) |= 0x00000040;	/* reset the card A */
    flDelayMsecs (10);			/* wait for the reset */
    *PGCRA (immrVal) &= ~0x00000040;

#if FALSE
    printf ("PIPR=0x%x\n", *PIPR(immrVal));
    printf ("PSCR=0x%x\n", *PSCR(immrVal));
    printf ("PER=0x%x\n", *PER(immrVal));
    printf ("PGCRA=0x%x\n", *PGCRA(immrVal));
    printf ("PGCRB=0x%x\n", *PGCRB(immrVal));
#endif	/* FALSE */

    return (flOK);
    }

#endif	/* INCLUDE_SOCKET_PCMCIA */
/*******************************************************************************
*
* pcSetWindow - set current window attributes, Base address, size, etc
*
* This routine sets current window hardware attributes: Base address, size,
* speed and bus width.	The requested settings are given in the 'vol.window'
* structure.  If it is not possible to set the window size requested in
* 'vol.window.size', the window size should be set to a larger value,
* if possible. In any case, 'vol.window.size' should contain the
* actual window size (in 4 KB units) on exit.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void pcSetWindow
    (
    FLSocket vol	/* pointer identifying drive */
    )
    {
    /* Physical base as a 4K page */
    vol.window.baseAddress = PC_BASE_ADRS_1 >> 12;

    flSetWindowSize (&vol, PC_BASE_ADRS_1 >> 12);
    }

/*******************************************************************************
*
* pcSetMappingContext - sets the window mapping register to a card address
*
* This routine sets the window mapping register to a card address.
* The window should be set to the value of 'vol.window.currentPage',
* which is the card address divided by 4 KB. An address over 128MB,
* (page over 32K) specifies an attribute-space address.
*
* The page to map is guaranteed to be on a full window-size boundary.
*
* RETURNS: N/A
*
* ERRNO
*/

LOCAL void pcSetMappingContext
    (
    FLSocket vol,	/* pointer identifying drive */
    unsigned page	/* page to be mapped */
    )
    {
    int immrVal = vxImmrGet();

    if (page & ATTRIBUTE_SPACE_MAPPED)
	*POR1 (immrVal) |= 0x00000010;	    /* attribute mem */
    else
	*POR1 (immrVal) &= ~0x00000010;     /* common mem */
    }

/*******************************************************************************
*
* pcGetAndClearCardChangeIndicator - return the hardware card-change indicator
*
* This routine returns the hardware card-change indicator and clears it if set.
*
* RETURNS: FALSE, or TRUE if the card has been changed
*
* ERRNO
*/

LOCAL FLBoolean pcGetAndClearCardChangeIndicator
    (
    FLSocket vol	/* pointer identifying drive */
    )
    {
    return (FALSE);
    }

/*******************************************************************************
*
* pcWriteProtected - return the write-protect state of the media
*
* This routine returns the write-protect state of the media.
*
* RETURNS: FALSE, or TRUE if the card is write-protected
*
* ERRNO
*/

LOCAL FLBoolean pcWriteProtected
    (
    FLSocket vol	/* pointer identifying drive */
    )
    {
    return (FALSE);
    }
#endif	/* INCLUDE_SOCKET_PCMCIA */

/*******************************************************************************
*
* flFitInSocketWindow - check whether the flash array fits in the socket window
*
* This routine checks whether the flash array fits in the socket window.
*
* RETURNS: A chip size guaranteed to fit in the socket window.
*
* ERRNO
*/

long int flFitInSocketWindow
    (
    long int chipSize,	    /* size of single physical chip in bytes */
    int      interleaving,  /* flash chip interleaving (1,2,4 etc) */
    long int windowSize     /* socket window size in bytes */
    )
    {
    if (chipSize*interleaving > windowSize) /* doesn't fit in socket window */
	{
	int  roundedSizeBits;

	/* fit chip in the socket window */
	chipSize = windowSize / interleaving;

	/* round chip size at powers of 2 */
	for (roundedSizeBits = 0; (0x1L << roundedSizeBits) <= chipSize;
	     roundedSizeBits++)
	;

	chipSize = (0x1L << (roundedSizeBits - 1));
	}

    return (chipSize);
    }

#if FALSE
/*******************************************************************************
*
* sysTffsCpy - copy memory from one location to another
*
* This routine copies <size> characters from the object pointed
* to by <source> into the object pointed to by <destination>. If copying
* takes place between objects that overlap, the behavior is undefined.
*
* INCLUDE FILES: string.h
*
* RETURNS: A pointer to <destination>.
*
* ERRNO
*
* \NOMANUAL
*/

void * sysTffsCpy
    (
    void *	 destination,	/* destination of copy */
    const void * source,	/* source of copy */
    size_t	 size		/* size of memory to copy */
    )
    {
    bcopy ((char *) source, (char *) destination, (size_t) size);
    return (destination);
    }

/*******************************************************************************
*
* sysTffsSet - set a block of memory
*
* This routine stores <c> converted to an `unsigned char' in each of the
* elements of the array of `unsigned char' beginning at <m>, with size <size>.
*
* INCLUDE FILES: string.h
*
* RETURNS: A pointer to <m>.
*
* ERRNO
*
* \NOMANUAL
*/

void * sysTffsSet
    (
    void * m,			/* block of memory */
    int    c,			/* character to store */
    size_t size 		/* size of memory */
    )
    {
    bfill ((char *) m, (int) size, c);
    return (m);
    }
#endif	/* FALSE */

#ifdef	INCLUDE_SOCKET_PCMCIA
/*******************************************************************************
*
* flDelayMsecs - wait for specified number of milliseconds
*
* This routine waits for specified number of milliseconds.
*
* RETURNS: N/A
*
* ERRNO
*
* \NOMANUAL
*/

void flDelayMsecs
    (
    unsigned milliseconds	/* milliseconds to wait */
    )
    {
    UINT32 ix;
    UINT32 iy = 1;
    UINT32 iz = 2;

    /* it doesn't count time consumed in interrupt level */

    for (ix = 0; ix < milliseconds; ix++)
	for (ix = 0; ix < sysTffsMsecLoopCount; ix++)
	    {
	    tickGet (); 	/* dummy */
	    iy = KILL_TIME_FUNC;    /* consume time */
	    }
    }
#endif	/* INCLUDE_SOCKET_PCMCIA */

/*******************************************************************************
*
* flDelayLoop - consume the specified time
*
* This routine consumes the specified time.
*
* RETURNS: N/A
*
* ERRNO
*/

void flDelayLoop
    (
    int  cycles
    )
    {
    while (--cycles)
    ;
    }

/*******************************************************************************
*
* sysTffsFormat - format the flash memory above an offset
*
* This routine formats the flash memory.  If the symbolic constant HALF_FORMAT
* is defined, a low area of the specified flash memory is left unformatted.
* If a lower section of the flash memory was previously formated by TrueFFS
* and you are trying to format the upper section you need to erase the lower
* section of the flash memory before you format the upper section.
* To do this, you could use:
* \cs
* tffsRawio(0, 3, 0, 8, 0)
* \ce
* The first argument in the tffsRawio() command shown above is the TrueFFS
* drive number, 0.  The second argument, 3, is the function number (also
* known as TFFS_PHYSICAL_ERASE).  The third argument, 0, specifies the unit
* number of the first erase unit you want to erase.  The fourth argument, 8,
* specifies how many erase units you want to erase.  The last argument is
* unused for the TFFS_PHYSICAL_ERASE function.
*
* RETURNS: OK, or ERROR if it fails.
*
* ERRNO
*/

STATUS sysTffsFormat
    (
    void
    )
    {
    STATUS status;
    tffsDevFormatParams params =
    {
/*
   HALF_FORMAT is used with the on board Flash device to skip the hardware
   configuration word.	We start at sector 4 because they are all the same
   size thereafter. The upper limit of the on board Flash device is faked
   to reserve the upper half for the bootrom code, which starts at 0xFE100100.
*/
#ifdef	HALF_FORMAT
    {0x010000l, 99, 1, 0x10000l, NULL, {0,0,0,0}, NULL, 2, 0, NULL},
#else
    {0x000000l, 99, 1, 0x10000l, NULL, {0,0,0,0}, NULL, 2, 0, NULL},
#endif	/* HALF_FORMAT */
   FTL_FORMAT
    };

    /* we assume that the drive number 0 is SIMM */
#if 0
    tffsRawio(0, TFFS_PHYSICAL_ERASE, 0, 10, 0);
#endif

    status = tffsDevFormat (0, (int)&params);
    return (status);
    }

/*
********************************************************************************
* This section of the file implements the functionality of an MTD for
* the SIMM flash array on the PowerQuiccII, consisting of four
* Intel 28F128J3A flash chips.
********************************************************************************
*/

#define Int28F128J3_FLASH   0x8918


typedef union
    {
    UCHAR   uchar[4];
    UINT32  uint32;
    } FL_DWORD;

/******************************************************************************
*
* PQIIsimmMTDStatusRead - read the flash status
*
* This routine reads the flash using the 28F128J3A algorithm.
*
* RETURNS:
*
* ERRNO
*
* \NOMANUAL
*/

FLStatus PQIIsimmMTDStatusRead
    (
    volatile UINT32 *	flashPtr,	/* address to write to */
    UINT32		value,		/* data to write */
    FLBoolean		issueCommand	/* TRUE to send status command first */
    )
    {
    unsigned long   statusTimeout;
    static FL_DWORD	dword;

    /* Set timeout to 5 seconds from now */

    statusTimeout = flMsecCounter + 5000;

    /* status command */

    if (issueCommand)
       *flashPtr = 0x70707070;

    /* wait for ready */

    do
	{
	if (flMsecCounter >= statusTimeout)
	    {
	    *flashPtr = 0x50505050;
	    return flWriteFault;
	    }
	dword.uint32 = *flashPtr;
	} while ((dword.uint32 & 0x80808080) != 0x80808080);

    if (dword.uint32 & 0x1a1a1a1a)  /* bits for full status check */
	{
	*flashPtr = 0x50505050;
	return flWriteFault;
	}

    return flOK;
    }
/******************************************************************************
*
* PQIIsimmMTDWriteDword - write one dword to the flash
*
* This routine writes one dword to the flash using the 28F128J3A algorithm.
*
* RETURNS:
*
* ERRNO
*
* \NOMANUAL
*/

FLStatus PQIIsimmMTDWriteDword
    (
    volatile UINT32 *	flashPtr,    /* address to write to */
    UINT32		value	     /* data to write */
    )
    {
    UINT32		statusRead = 0;

    /* Program sequence */

    if (PQIIsimmMTDStatusRead(flashPtr, statusRead, TRUE) != flOK)
	{
	*flashPtr = 0xffffffff;
	return flWriteFault;
	}

    /*	*flashPtr    = 0x40404040; */ /* write command */
    *flashPtr	=  0x40404040;

    if (PQIIsimmMTDStatusRead(flashPtr, statusRead, FALSE) != flOK)
	{
	*flashPtr = 0xffffffff;
	return flWriteFault;
	}

    *flashPtr	 = value;

    if (PQIIsimmMTDStatusRead(flashPtr, statusRead, FALSE) != flOK)
	{
	*flashPtr = 0xffffffff;
	return flWriteFault;
	}

    *flashPtr = 0xffffffff;

    if (value != *flashPtr )
	{
	return flWriteFault;
	}
    return flOK;
    }

/******************************************************************************
*
* PQIIsimmWrite - Write a block of bytes to Flash
*
* This routine writes a block of bytes to the flash.
* The PowerQuiccII SIMM Suports Block Mode Writes.
*
* RETURNS: flOK, or error if it fails
*
* ERRNO
*/

FLStatus PQIIsimmWrite
    (
    FLFlash	    vol,	    /* Pointer identifying drive */
    CardAddress     address,	    /* address to write to */
    const void FAR1 *buffer,	    /* pointer to data to write */
    int 	    length	    /* length of data */
    )
    {
    int 	i;
    int 	cLength;
    int         cSize = 4;
    int 	tmp;
    UCHAR *	flashCPtr;
    UINT32 *	    flashPtr;
    UCHAR *	    bufCPtr;
    FL_DWORD	    dword;

    if (flWriteProtected (vol.socket))
        return flWriteProtect;

    /* Initialize pointers */

    bufCPtr   = (UCHAR *) buffer;
    flashCPtr = (UCHAR *) vol.map (&vol, address, 0);
    flashPtr  = (UINT32 *)((UINT32)flashCPtr & ~0x3);
    cLength   = length;


#ifdef DEBUG_PRINT
     DEBUG_PRINT("PQIIsimmWrite: length %d flashPtr %lx\n",cLength ,vol.map(&vol,address,0));
#endif /* DEBUG_PRINT */

    /* first take care of unaligned flashPtr */

    dword.uint32 = *flashPtr;

    if ((tmp = ((UINT32)flashCPtr & 0x3)) != 0)
        {
        /* assemble a dword and write to flash */

       *flashPtr = 0xffffffff; /* write leaves flash in read status mode. */
        
        /* avoid overwritten with unknown/garbage data */

        if ((tmp + cLength) < 4)
            cSize = cLength + tmp;

        for (i = tmp ; i < cSize; i++)
            dword.uchar[i] = *(bufCPtr + i - tmp);

        if (PQIIsimmMTDWriteDword (flashPtr, dword.uint32) != flOK)
            {
            return flWriteFault;
            }

        cLength -= (cSize - tmp);
        bufCPtr += (cSize - tmp);
        flashPtr++;
        }

    /* Write all properly aligned 32-bit data */

    while (cLength >= 4)
        {
        for (i = 0; i < 4; i++)
            dword.uchar[i] = *bufCPtr++;

        if (PQIIsimmMTDWriteDword (flashPtr, dword.uint32) != flOK)
            {
            return flWriteFault;
            }

        flashPtr++;
        cLength -=4;
        }

    /* take care of remaining bytes */

    if (cLength > 0)
        {
        /* Assemble the last dword and write to flash or first if less than 4 bytes! */

        *flashPtr = 0xffffffff; /* write leaves flash in read status mode. */
        dword.uint32 = *flashPtr;

        for (i = 0; i < cLength; i++)
            dword.uchar[i] = *(bufCPtr + i);

        if (PQIIsimmMTDWriteDword (flashPtr, dword.uint32) != flOK)
            {
            return flWriteFault;
            }
        }

    /* Write leaves flash in read status mode so clear it. */

    flashPtr  = (UINT32 *)((UINT32)flashCPtr & ~0x3);
    *flashPtr = 0xffffffff; /* write leaves flash in read status mode. */

    /* Verify by comparing data */

    if (tffscmp ((void FAR0 *)flashCPtr, buffer, length))
        {
        return flWriteFault;
        }

    return flOK;
    }

/******************************************************************************
*
* PQIIsimmMTDErase - erase the contents of one or more sectors
*
* This routine erases one or more contiguous Flash erasable blocks. It is
* registered as the MTD's vol.erase routine.
*
* RETURNS: flOK, or error if it fails
*
* ERRNO
*/

FLStatus PQIIsimmMTDErase
    (
    FLFlash	vol,		    /* Pointer identifying drive */
    int 	firstBlock,	    /* Number of first block to erase */
    int 	numOfBlocks	    /* Number of blocks to erase */
    )
    {
    UINT32 *	    flashPtr = NULL;
    int 	    iBlock;
    FLStatus	    statusErase = 0;
    unsigned long   statusTimeout;
    FL_DWORD	    dword;

    if (flWriteProtected(vol.socket))
    {
	return flWriteProtect;
    }

    for (iBlock = firstBlock;  iBlock < firstBlock + numOfBlocks;  iBlock++)
	{
	flashPtr = vol.map (&vol, iBlock * vol.erasableBlockSize, 0);

	if (PQIIsimmMTDStatusRead(flashPtr, statusErase, TRUE) != flOK)
	    {
	    *flashPtr = 0x50505050;
	    return flWriteFault;
	    }

	/* Erase sequence */

	*flashPtr    = 0x20202020;
	*flashPtr    = 0xD0D0D0D0;

	/* Set timeout to 5 seconds from now */

	statusTimeout = flMsecCounter + 5000;

	/* wait for ready */

	do
	    {
	    if (flMsecCounter >= statusTimeout)
		{
		*flashPtr = 0x50505050;
		return flWriteFault;
		}
	    dword.uint32 = *flashPtr;
	    } while ((dword.uint32 & 0x80808080) != 0x80808080);

	if (dword.uint32 & 0x7f7f7f7f)	/* erase error */
	    {
	    if (PQIIsimmMTDStatusRead(flashPtr, statusErase, TRUE) != flOK)
		{
		*flashPtr = 0x50505050;
		return flWriteFault;
		}
	    }

	*flashPtr = 0xffffffff;
	}

    return flOK;
    }

/******************************************************************************
*
* PQIIsimmMTDWrite - Write a block of bytes to Flash
*
* This routine writes a block of bytes to the flash. It is registered as
* the MTD's vol.write routine. The PowerQuiccII SIMM Suports Block Mode
* Writes.
*
* RETURNS: flOK, or error if it fails
*
* ERRNO
*/

FLStatus PQIIsimmMTDWrite
    (
    FLFlash	    vol,	    /* Pointer identifying drive */
    CardAddress     address,	    /* address to write to */
    const void FAR1 *buffer,	    /* pointer to data to write */
    int 	    length,	    /* length of data */
    FLBoolean	    overwrite	    /* TRUE if Flash needs to be erased */
    )
    {
    volatile UCHAR *	 pFlash;
    UINT32 *	    pSave;
    int 	saveSize = 0;
    int 	eUnitCount = 0;
    int 	eUnit;
    int 	offset;
    int 	freeSectors;
    STATUS	rc;

    if (flWriteProtected (vol.socket))
    {
	return flWriteProtect;
    }

    rc = PQIIsimmWrite (&vol, address, buffer, length);

    if ((flWriteFault == rc) && overwrite)
	{
	eUnit = address / vol.erasableBlockSize;
	offset = address % vol.erasableBlockSize;

    do
	{
	saveSize += vol.erasableBlockSize;
	++eUnitCount;
	    }
    while (saveSize < length + offset);

    pSave = (UINT32*) malloc(saveSize);

	if (pSave == 0)
	    {
	    freeSectors = errnoGet();
#ifdef DEBUG_PRINT
	    DEBUG_PRINT("PQIIsimmMTDWrite: No memory\n",);
#endif
	    return(flNotEnoughMemory);
	    }

	/* Determine eUnit and offset */

#ifdef DEBUG_PRINT
	DEBUG_PRINT("Overwrite eUnit: 0x%08x, offset: 0x%08x\n",
		    eUnit,offset);
#endif

	/* Get a pointer to the flash eUnit */
	pFlash = vol.map (&vol, eUnit * vol.erasableBlockSize, eUnitCount);

	/* Copy the eUnit from flash to memory */

	memcpy(pSave, (void*) pFlash, saveSize);


	logMsg("Overwrite eUnit: %d, offset: 0x%08x Length 0x%04x old 0x%08x new 0x%08x\n", eUnit,offset,length, *(int *)(&pSave[offset]), (int)buffer,0);

	/* Overwrite the eUnit in memory */

	memcpy(((UINT8*) pSave) + offset, buffer, length);

	/* Erase eUnit */

	rc = PQIIsimmMTDErase(&vol, eUnit, eUnitCount);
	if (rc == flOK)
	    {
	    rc = PQIIsimmWrite (&vol, (address - offset), pSave, (eUnitCount * vol.erasableBlockSize));
	    }

    free (pSave);
	}
    return(rc);
    }

/******************************************************************************
*
* PQIIsimmMTDIdentify - Identifies the SIMM flash media on the SBC PowerQuiccII
*
* This routine fills out the Flash structure and registers the write and
* erase routines.
*
* RETURNS: 0 on success, failed otherwise
*
* ERRNO
*/

FLStatus PQIIsimmMTDIdentify
    (
    FLFlash vol 		/* Pointer identifying drive */
    )
    {
    /*
     * This version simply sets the known configuration values.
     */
#ifdef DEBUG_PRINT
    DEBUG_PRINT("Debug: In PQIIsimmMTDIdentify \n");
#endif

    flSetWindowBusWidth (vol.socket,32);      /* use 32-bits */
    flSetWindowSpeed (vol.socket,150);	      /* 150 nsec.   */

    /* Flash configuration */

    vol.type		  = Int28F128J3_FLASH;
    vol.interleaving	  = 4;

    /*
     * Due to the sector protection logic in target/src/tffs/fatlite.c,
     * we may need to limit the chipsize to account for protecting the first
     * sector.	Otherwise we'd attempt to write beyond the end of the flash
     * device.
     *
     * For each protected FAT sector, reduce by 512 bytes.
     * NOTE: we would need to reduce significantly more if were defining
     * HALF_FORMAT, but we are no longer doing that (we are using the
     * flash SIMM).
     */
    vol.chipSize	  = (TFFS_FLASH_SIZE - (512 * flFileSysSectorStart)) /
			     vol.interleaving;

    vol.noOfChips	  = 4;
    vol.erasableBlockSize = 0x20000l * vol.interleaving;
    vol.flags		 |= SUSPEND_FOR_WRITE;

    /* Register our flash handlers */

    vol.write		  = PQIIsimmMTDWrite;
    vol.erase		  = PQIIsimmMTDErase;

    return flOK;
    }



/*
********************************************************************************
* The remainder of this file implements the functionality of an MTD for
* the resident flash array on the PowerQuiccII, consisting of one
* AMD 29LV160D flash chip.  The part is hardwired in 8 bit mode.
* We skip over the variable size bottom boot block sectors.
* If HALF_FORMAT is defined, stop before we get to the bootrom area.
********************************************************************************
*/

#define     MAXSECTORS	128 /* should work with any value */

typedef enum
    {
    BOOTBLOCK_NONE,
    BOOTBLOCK_TOP,
    BOOTBLOCK_BOTTOM
    }BOOTBLOCK;

/* Instance specific CFI data so that a system may have more than one CFI
 * device.
 */
typedef struct {

  unsigned	   commandSetId;	    /* id of a specific command set. */
  unsigned	   altCommandSetId;	    /* id of alternate command set.  */
  FLBoolean	   wordMode;		    /* TRUE - word mode.	     */
					    /* FALSE - byte mode.	     */
  int		   multiplier;		    /* the number of bytes between   */
					    /* 1st Q and 1st R in query      */
  int	       interleaveWidth; 	    /* 1 - byte 2 - word interleaved */
  unsigned	   maxBytesWrite;	    /* maximum number of bytes	     */
					    /* in multi-byte write.	     */
  FLBoolean	   vpp; 		    /* if = TRUE, need vpp.	     */
  long		   optionalCommands;	    /* optional commands supported   */
					    /* (1 = yes, 0 = no):	     */
					    /* bit 0 - chip erase.	     */
					    /* bit 1 - suspend erase.	     */
					    /* bit 2 - suspend write	     */
					    /* bit 3 - lock/unlock.	     */
					    /* bit 4 - queued erase.	     */
  unsigned	   afterSuspend;	    /* functions supported after     */
					    /* suspend (1 = yes, 0 = no):    */
					    /* bit 0 - write after erase     */
					    /*	       suspend. 	     */
  int	       sectorDefs;		    /* Number of sector definitions  */
  struct {				    /* sector descriptions as found  */
      long     secSize; 		    /* in CFI query.		     */
      int      numSecs;
  }secDesc[8];				    /* assume 8 is enough	     */
  struct {
      long     sectorSize;		    /* sector size		     */
      UINT32	   sectorBaseAdrs;	    /* base address of sector	     */
  } secInfo[MAXSECTORS];		    /* per sector info		     */
  UINT32 unlockAddr1;			    /* offset for first unlock	     */
  UINT32 unlockAddr2;			    /* offset for 2nd unlock	     */
  int sectorsInCFI;			    /* sector count for device	     */
  int bootBlockSectors; 		    /* sectors that makeup boot block*/
  BOOTBLOCK bootBlockType;		    /* Top, Bottom or None	     */

} CFI;


LOCAL CFI mtdVars[DRIVES];


#define Am29LV160DT_FLASH      0x01c4 /* 31 64KB, 1 32KB, 2 8KB, and 1 16KB sectors */
#define Am29LV160DB_FLASH      0x0149 /* 1 16KB, 2 8KB, 1 32KB, and 31 64KB sectors */

/* command set IDs */

#define INTEL_COMMAND_SET      0x0001
#define AMDFUJ_COMMAND_SET     0x0002
#define INTEL_ALT_COMMAND_SET  0x0001
#define AMDFUJ_ALT_COMMAND_SET 0x0004
#define ALT_NOT_SUPPORTED      0x0000


/* CFI identification strings */

#define ID_STR_LENGTH	   3
#define QUERY_ID_STR	   "QRY"
#define PRIMARY_ID_STR	   "PRI"
#define ALTERNATE_ID_STR   "ALT"

#define thisCFI   ((CFI *)vol.mtdVars)

/* commands */


#define QUERY	0x98

#define AMD_SETUP_ERASE     0x80
#define AMD_SETUP_WRITE     0xa0
#define AMD_READ_ID	    0x90
#define AMD_SUSPEND_ERASE   0xb0
#define AMD_SECTOR_ERASE    0x30
#define AMD_RESUME_ERASE    0x30
#define AMD_READ_ARRAY	    0xf0

#define AMD_NO_UNLOCK_ADDR 0xffffffffL

#define AMD_UNLOCK_1	    0xaa
#define AMD_UNLOCK_2	    0x55

#define AMD_WW_UNLOCK_ADDR1 0x555u  /* Word wide devices */
#define AMD_WW_UNLOCK_ADDR2 0x2aau  /* Word wide devices */
#define AMD_BW_UNLOCK_ADDR1 0xaaau  /* Byte wide devices */
#define AMD_BW_UNLOCK_ADDR2 0x555u  /* Byte wide devices */

#define AMD_D2	    4	   /* Toggles when erase suspended */
#define AMD_D5	    0x20   /* Set when programming timeout */
#define AMD_D6	    0x40   /* Toggles when programming */


/******************************************************************************
*
* PQIIrfaWrite - Write a block of bytes to Flash
*
* This routine will be registered as the MTD vol.write routine. Given a pointer
* to a buffer that must be written, the write width determined from the
* identification process is used as the stepping stone to iterate through the
* buffer.
*
* RETURNS: 0 on success, failed otherwise
*
* ERRNO
*/

LOCAL FLStatus PQIIrfaWrite
    (
    FLFlash vol,		/* Pointer identifying drive */
    CardAddress address,	/* Card address to write to */
    const void FAR1 *buffer,	/* Address of data to write */
    int length, 		/* Number of bytes to write */
    FLBoolean overwrite 	/* TRUE if overwriting Flash contents */
    )
    {
    /* Set timeout to 5 seconds from now */
    unsigned long writeTimeout = flMsecCounter + 5000;
    int cLength, count = 0, index = 0;
    FlashPTR base;
    FlashPTR flashPtr;
    UINT32 read_array = 0;
    int i;

#define bFlashPtr  flashPtr
#define bBuffer ((const unsigned char FAR1 *) buffer)
#define wFlashPtr ((FlashWPTR) flashPtr)
#define wBuffer ((const unsigned short FAR1 *) buffer)
#define dFlashPtr ((FlashDPTR) flashPtr)
#define dBuffer ((const unsigned long FAR1 *) buffer)

    if (flWriteProtected(vol.socket))
	return flWriteProtect;

    i = 0;
    while ((thisCFI->secInfo[i].sectorBaseAdrs <= address) && (i < thisCFI->sectorsInCFI))
	i++;
    i--;

    base = (FlashPTR)
	   vol.map(&vol, thisCFI->secInfo[i].sectorBaseAdrs,
		   vol.interleaving * thisCFI->interleaveWidth);

    flashPtr = (FlashPTR)
	       vol.map(&vol, address,
		       vol.interleaving * thisCFI->interleaveWidth);

    cLength = length;

    /* vol.interleaving and vol.interleavingWidh are 1 on this part. */
    base = (FlashPTR) flMap(vol.socket, 0);
#ifdef DEBUG_PRINT
    DEBUG_PRINT("Debug: write in PQIIrfaWrite cLength = 0x%x.\n", cLength);
#endif
    while (cLength >= 1)
	{
	index = count * vol.interleaving;
	CACHE_PIPE_FLUSH();
	base[thisCFI->unlockAddr1] = AMD_UNLOCK_1;
	CACHE_PIPE_FLUSH();
	base[thisCFI->unlockAddr2] = AMD_UNLOCK_2;
	CACHE_PIPE_FLUSH();
	base[thisCFI->unlockAddr1] = AMD_SETUP_WRITE;
	CACHE_PIPE_FLUSH();
	flashPtr[index] = bBuffer[count];
	base[thisCFI->unlockAddr1] = AMD_SETUP_WRITE;

	while (flashPtr[index] != bBuffer[count] && flMsecCounter < writeTimeout)
	    {
	    if ((flashPtr[index] & AMD_D5) && flashPtr[index] != bBuffer[count])
		{
		base[count] = read_array;
#ifdef DEBUG_PRINT
		DEBUG_PRINT("Debug: PQIIrfaWrite failed count = 0x%x.\n", count);
		DEBUG_PRINT("Debug: flashPtr at  0x%x\n", (UINT32) flashPtr);
#endif
		return flWriteFault;
		}
	    }
	cLength--;
	count++;
	}

#if FALSE
    if (tffscmp((void FAR0 *) flashPtr,buffer,length))
	{
#ifdef DEBUG_PRINT
	DEBUG_PRINT("Debug: write failed in AMD MTD on verification.\n");
#endif
	return flWriteFault;
	}
#endif
    return flOK;
    }

/***************************************************************************
*
* PQIIrfaErase - erases a block of the resident flash array
*
* This routine skips over the non-uniform blocks and erases a block
*
* RETURNS: 0 on success, failed otherwise
*
* ERRNO
*/
LOCAL  FLStatus PQIIrfaErase
    (
    FLFlash vol,		/* Pointer identifying drive */
    int firstErasableBlock,	/* Number of first block to erase */
    int numOfErasableBlocks	/* Number of blocks to erase */
    )
    {
    int thisSector = firstErasableBlock;
    int sectorsToErase = numOfErasableBlocks;

    if (flWriteProtected(vol.socket))
	{
	return flWriteProtect;
	}

#ifdef DEBUG_PRINT
    if (thisCFI->bootBlockType == BOOTBLOCK_BOTTOM)
	DEBUG_PRINT("Debug: Erase bottom boot block thisSector =  %d	  \
		    numOfEraseable = %d\n", thisSector, numOfErasableBlocks);
#endif

    thisSector += 3;
    firstErasableBlock += 3;

    for ( ; thisSector < firstErasableBlock + sectorsToErase; thisSector++)
	{
	FLBoolean finishing = TRUE;
	FlashPTR flashPtr;
	FlashPTR flashBasePtr = (FlashPTR) flMap(vol.socket, 0);

#ifdef DEBUG_PRINT
	DEBUG_PRINT("Clearing sector %d\n", thisSector);
#endif

	/* we know we are on a unit boundary so mapBase is not necessary */
	flashPtr = (FlashPTR)
		   vol.map(&vol, thisCFI->secInfo[thisSector].sectorBaseAdrs, 0);
#ifdef DEBUG_PRINT
	DEBUG_PRINT("Erase: flashPtr at  0x%x\n", (UINT32) flashPtr);
#endif

	CACHE_PIPE_FLUSH();
	flashBasePtr[AMD_BW_UNLOCK_ADDR1] = AMD_UNLOCK_1;
	CACHE_PIPE_FLUSH();
	flashBasePtr[AMD_BW_UNLOCK_ADDR2] = AMD_UNLOCK_2;
	CACHE_PIPE_FLUSH();
	flashBasePtr[AMD_BW_UNLOCK_ADDR1] = AMD_SETUP_ERASE;
	CACHE_PIPE_FLUSH();
	flashBasePtr[AMD_BW_UNLOCK_ADDR1] = AMD_UNLOCK_1;
	CACHE_PIPE_FLUSH();
	flashBasePtr[AMD_BW_UNLOCK_ADDR2] = AMD_UNLOCK_2;
	CACHE_PIPE_FLUSH();
	flashPtr[0] = AMD_SECTOR_ERASE;
	CACHE_PIPE_FLUSH();

	while (finishing)
	    {
	    if (flashPtr[0] != 0xff)
		{
		if ((flashPtr[0] & AMD_D5) &&
		    flashPtr[0] != 0xff)
		    {
		    flashPtr[0] = AMD_READ_ARRAY;
#ifdef DEBUG_PRINT
		    DEBUG_PRINT("Debug: Erase failed in AMD MTD.\n");
#endif
		    return flWriteFault;
		    }
		}
	    else
		{
		finishing = FALSE;
		}
	    }
	} /* next block */

    return flOK;
    }

/***************************************************************************
*
* PQIIrfaIdentify - Identification routine for devices conforming to CFI/SC
*
* Identifies media based on CFI and the AMD/Fujitsu command set and registers
* as an MTD for such. This routine must be an extern routine. On successful
* identification, the Flash structure is filled out and the write and erase
* routines are registered.
*
* RETURNS: 0 on positive identification, failed otherwise
*
* ERRNO
*
* \NOMANUAL
*
*/
FLStatus PQIIrfaIdentify
    (
    FLFlash vol        /* Pointer identifying drive */
    )
    {
    FlashPTR flashPtr = (FlashPTR) flMap(vol.socket, 0);
    FlashDPTR flashDPtr = (FlashDPTR) flashPtr;

    unsigned primaryTable, secondaryTable;
    FLBoolean eightBitMode = FALSE;
    int thisSector = 0; 	    /* running count of sectors for this CFI */
    CardAddress sectorBaseAdrs = 0; /* base address of this sector */
    int ix = 0;

#ifdef DEBUG_PRINT
    DEBUG_PRINT("Debug: entering PQIIrfa identification routine.\n");
#endif


    flSetWindowBusWidth(vol.socket, 8); /* use 8-bits */
    flSetWindowSpeed(vol.socket, 120);	/* 120 nsec. */
    flSetWindowSize(vol.socket, 8);	/* 32 KBytes */

    vol.mtdVars = &mtdVars[flSocketNoOf(vol.socket)];
    thisCFI->bootBlockSectors = 1;

    /* Is this an 8 bit device in word mode 1*/
    CACHE_PIPE_FLUSH();
    flashPtr[0x55] = AMD_READ_ARRAY;
    CACHE_PIPE_FLUSH();
    flashPtr[0x55] = QUERY;
    CACHE_PIPE_FLUSH();

    if (flashPtr[0x10] == 0x51 && /* 'Q' */
	flashPtr[0x11] == 0x52 && /* 'R' */
	flashPtr[0x12] == 0x59)   /* 'Y' */
	{
#ifdef DEBUG_PRINT
	DEBUG_PRINT ("Debug: detected single 8 bit device\n");
#endif
	thisCFI->multiplier = 1;
	thisCFI->interleaveWidth = 1;
	vol.interleaving = 1;
	eightBitMode = TRUE;

	goto getCFI;
	}

    /* Is this an 16 bit device in 8 bit mode */
    CACHE_PIPE_FLUSH();
    flashPtr[0xAA] = AMD_READ_ARRAY;
    CACHE_PIPE_FLUSH();
    flashPtr[0xAA] = QUERY;
    CACHE_PIPE_FLUSH();

    if (flashPtr[0x20] == 0x51 && /* 'Q' */
	flashPtr[0x21] == 0x51 && /* 'Q' */
	flashPtr[0x22] == 0x52 && /* 'R' */
	flashPtr[0x23] == 0x52 && /* 'R' */
	flashPtr[0x24] == 0x59 && /* 'Y' */
	flashPtr[0x25] == 0x59)   /* 'Y' */
	{
	thisCFI->multiplier = 2;
	thisCFI->interleaveWidth = 1;
	vol.interleaving =1;
#ifdef DEBUG_PRINT
	DEBUG_PRINT ("Debug: detected 16 bit device 8bit mode\n");
#endif
	eightBitMode = TRUE;
	goto getCFI;
	}

    /* If we get here the device is probably not CFI */

#ifdef DEBUG_PRINT
    DEBUG_PRINT ("Debug: Failed to identify CFI... \n Exiting \n");
#endif

    flashDPtr[0xAA] = AMD_READ_ARRAY;

    return flUnknownMedia;

getCFI:

    if (!eightBitMode)
	{
	thisCFI->wordMode = TRUE; /* limiting ourselves to 8 and 16 bit devs */
	thisCFI->unlockAddr1 = AMD_WW_UNLOCK_ADDR1;
	thisCFI->unlockAddr2 = AMD_WW_UNLOCK_ADDR2;
#ifdef DEBUG_PRINT
	DEBUG_PRINT("ERROR WORD MODE !!\n");
#endif
	}
    else
	{
	thisCFI->wordMode = FALSE;
	thisCFI->unlockAddr1 = AMD_BW_UNLOCK_ADDR1;
	thisCFI->unlockAddr2 = AMD_BW_UNLOCK_ADDR2;
#ifdef DEBUG_PRINT
	DEBUG_PRINT("GOOD BYTE MODE !!\n");
#endif
	}

    /* check the command set ID */
    /* NOTE: Not swapped */
    thisCFI->commandSetId =  flashPtr[0x14 * thisCFI->multiplier]<< 8 | flashPtr[0x13 * thisCFI->multiplier];;
#ifdef DEBUG_PRINT
    DEBUG_PRINT ("Debug: device multiplier is %d\n", thisCFI->multiplier);
    DEBUG_PRINT ("Debug: Commandset ID is 0x%x\n", thisCFI->commandSetId);
#endif

    /* Only support the AMD/Fujitsu Command set */
    if ( thisCFI->commandSetId != AMDFUJ_COMMAND_SET)
	{
#ifdef DEBUG_PRINT
	DEBUG_PRINT("Debug: did not recognize command set.\n");
#endif
	return flUnknownMedia;
	}


    /* get address for primary algorithm extended table. */
    primaryTable =  flashPtr[0x15 * thisCFI->multiplier]<< 8 | flashPtr[0x16 * thisCFI->multiplier];;
#ifdef DEBUG_PRINT
    DEBUG_PRINT("Debug: primaryTable 0x%x.\n",primaryTable);
#endif

    /* check alternate command set ID. */
    /* NOTE: not swapped */
    thisCFI->altCommandSetId =	flashPtr[0x17 * thisCFI->multiplier]<< 8 | flashPtr[0x18 * thisCFI->multiplier];;
#ifdef DEBUG_PRINT
    DEBUG_PRINT("Debug: altCommandSetId 0x%x.\n",thisCFI->altCommandSetId);
#endif
    if (thisCFI->altCommandSetId != AMDFUJ_ALT_COMMAND_SET &&
	thisCFI->altCommandSetId != ALT_NOT_SUPPORTED)
	return flUnknownMedia;

    /* get address for secondary algorithm extended table. */
    secondaryTable =  flashPtr[0x19 * thisCFI->multiplier]<< 8 | flashPtr[0x1A * thisCFI->multiplier];;

    thisCFI->vpp = flashPtr[0x1d * thisCFI->multiplier];

    /* Get the number of erase block descriptions for the CFI*/
    thisCFI->sectorDefs = (int) flashPtr[0x2c * thisCFI->multiplier];

    /* We should bail out if this is greater than 8 */
#ifdef DEBUG_PRINT
    DEBUG_PRINT ("Debug: Number of erase block descriptions is 0x%x\n",
		 thisCFI->sectorDefs);
#endif

    vol.erasableBlockSize = 0;
    thisCFI->sectorsInCFI = 0;

    for ( ix = 0; ix < thisCFI->sectorDefs; ix++)
	{
	thisCFI->secDesc[ix].numSecs = (int)
				       flashPtr[(0x2d + (ix * 4)) * thisCFI->multiplier];
	thisCFI->secDesc[ix].numSecs++;

#ifdef DEBUG_PRINT
	DEBUG_PRINT ("Debug: Num sectors in %d - %d\n", ix, thisCFI->secDesc[ix].numSecs);
#endif

	thisCFI->secDesc[ix].secSize = (long)
				       flashPtr[(0x2f + (ix * 4)) * thisCFI->multiplier] +
				       flashPtr[(0x30 + (ix * 4)) * thisCFI->multiplier] * 0x100L;
#ifdef DEBUG_PRINT
	DEBUG_PRINT ("Debug: secSize is 0x%x Erase Block Size is %ld bytes\n",
		     (UINT32) thisCFI->secDesc[ix].secSize,
		     thisCFI->secDesc[ix].secSize * 256);
#endif

	/* TrueFFS does not support the flexible sector architecture so we
	 * take the largest sector size to be the erase block size.
	 */
	if (vol.erasableBlockSize < thisCFI->secDesc[ix].secSize)
	    vol.erasableBlockSize = thisCFI->secDesc[ix].secSize;

	/* Also track the number of sector on this device */
	thisCFI->sectorsInCFI += thisCFI->secDesc[ix].numSecs;
	}

    if (vol.erasableBlockSize == 0x00)
	vol.erasableBlockSize = 0x80L;
    else
	vol.erasableBlockSize *= 256;

    /* Get Device Size */
    if (eightBitMode)
	vol.chipSize = (1L << flashPtr[0x27 * thisCFI->multiplier]);
    else
	vol.chipSize = (1L << *(USHORT*)(&flashPtr[0x27 * thisCFI->multiplier]));

    vol.chipSize = 0xf0000;	/* fake upper limit to preserve boot image */
#ifdef HALF_FORMAT
    vol.chipSize = vol.erasableBlockSize * 15; /* fake size to preserve bootrom */
#endif
#ifdef DEBUG_PRINT
    DEBUG_PRINT("Debug: chipSize = 0x%lx\n", vol.chipSize);
#endif
    vol.erasableBlockSize *= vol.interleaving;

#ifdef SAVE_NVRAM_REGION

    /* Top boot block devices loose their entire boot block to NVRAM. Since
     * TrueFFS does not care about the subdivisions, we reserve an entire erase block
     * on each of the interleaved devices.
     */
    vol.chipSize -= vol.erasableBlockSize;
#endif

#ifdef DEBUG_PRINT
    DEBUG_PRINT("Debug: %d sectors on device\n", thisCFI->sectorsInCFI);
    DEBUG_PRINT("Debug: Erasable block size is 0x%lx\n", vol.erasableBlockSize);
    DEBUG_PRINT("Debug: Chip size is 0x%lx\n", vol.chipSize);
#endif

    /* Since boot blocks happen to be subdivided erase blocks and TrueFFS does
     * not support the sub division of erase blocks the MTD needs to be able
     * to detect the "sectors" that are sub divisions and erase all of them
     * (assuming that they are contiguous) together. Since the boot block can
     * appear either at the top or the bottom, the only way to keep track of
     * them is to read all sector descriptions and remember them for cross
     * reference. The size of the largest erase block is registered as the
     * eraseBlockSize for the volume.
     */

    /* Get JEDEC ID so we know if this is a top or bottom boot device */
    /* Is this an 8 bit device */
    flashPtr[0xAAA] = 0xF0; /* reset */
    flashPtr[0xAAA] = 0xAA;
    flashPtr[0x555] = 0x55;
    flashPtr[0xAAA] = 0x90;

    vol.type = flashPtr[0] << 8 | flashPtr[thisCFI->multiplier];

    /* Determine boot block type */
    if (vol.type == Am29LV160DT_FLASH)
	thisCFI->bootBlockType = BOOTBLOCK_TOP;
    else if (vol.type == Am29LV160DB_FLASH)
	thisCFI->bootBlockType = BOOTBLOCK_BOTTOM;
    else
	{
	/* Probably an unregistered device that needs to be added here */
#ifdef DEBUG_PRINT
	DEBUG_PRINT ("Unknown device of type 0x%x \n", vol.type);
#endif

	/* set device back to read array */
	*(UINT32 *) flashPtr = AMD_READ_ARRAY;

	return flUnknownMedia;
	}

    /* We now have all of the CFI info that we need. Now lets build up
     * the sector map and register the erase block sizes.
     */

    if (thisCFI->bootBlockType == BOOTBLOCK_BOTTOM)
	{
#ifdef DEBUG_PRINT
	DEBUG_PRINT("Debug: Bottom boot block device\n");
#endif
	for ( ix = 0; ix < thisCFI->sectorDefs; ix++)
	    {
	    int iy;

	    for (iy = 0; iy < thisCFI->secDesc[ix].numSecs; iy++, thisSector++)
		{
		if (thisCFI->secDesc[ix].secSize != 0x00)
		    thisCFI->secInfo[thisSector].sectorSize =
		    thisCFI->secDesc[ix].secSize * 0x100L;
		else
		    thisCFI->secInfo[thisSector].sectorSize = 0x80L;

		/* count boot blocks */
		if (thisCFI->secInfo[thisSector].sectorSize != vol.erasableBlockSize)
		    thisCFI->bootBlockSectors++;

		thisCFI->secInfo[thisSector].sectorBaseAdrs = sectorBaseAdrs;
		sectorBaseAdrs += (CardAddress) thisCFI->secInfo[thisSector].sectorSize;
		}
	    }
	}
    else    /* Top boot or uniform block device !*/
	{
#ifdef DEBUG_PRINT
	DEBUG_PRINT("Error: Not a bottom boot block device!\n");
#endif
	}
#ifdef DEBUG_PRINT
    DEBUG_PRINT("Debug: Number of boot block sectors is %d\n", thisCFI->bootBlockSectors);
#endif
    vol.noOfChips = 1;	/* only one chip on the wrSbcPowerQuiccII */
#ifdef DEBUG_PRINT
    DEBUG_PRINT ("Debug: No of chips detected is %d\n", vol.noOfChips);
#endif
    vol.write		  = PQIIrfaWrite;
    vol.erase		  = PQIIrfaErase;

    /* Might have to do this on a bus width basis but for now it seems to
     * work with Intel devices.
     */
    flashPtr[0] = AMD_READ_ARRAY;
    flashPtr[1] = AMD_READ_ARRAY;
    flashPtr[2] = AMD_READ_ARRAY;
    flashPtr[3] = AMD_READ_ARRAY;

    return flOK;
    }

#endif /* INCLUDE_TFFS */

