/* sysTffs.c - BSP stub for TrueFFS Socket Component Driver */

/* Copyright 1984-2004 Wind River Systems, Inc. */
/* Copyright 2005 Motorola Inc. All Rights Reserved */

#include "copyright_wrs.h"

/*
modification history
--------------------
01k,20aug05,cak  Integrated with driver for AMD parts on MVME3100. 
01j,23aug04,scb  Starting point for "Big Easy".
01i,17sep03,dtr  Modify to support PILOT and PROTOTYPE rev of ADS board.
01h,05aug03,dtr  Removing FLASH_BASE_ADRS define.
01g,30nov01,dtr  Changing some comments.
01f,07jan00,an   written
*/

/*
DESCRIPTION
This library provides board-specific hardware access routines for TrueFFS.
In effect, these routines comprise the socket component driver (or drivers)
for your flash device hardware.  At socket registration time, TrueFFS stores
pointers to the functions of this socket component driver in an 'FLSocket'
structure.  When TrueFFS needs to access the flash device, it uses these
functions.
 
Because this file is, for the most part, a device driver that exports its
functionality by registering function pointers with TrueFFS, very few of the
functions defined here are externally callable.  For the record, these
external functions are flFitInSocketWindow() and flDelayLoop().  You should
never have any need to call these functions.
 
However, one of the most important functions defined in this file is neither
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

The sysTffsFormat() routine is an optional but BSP-specific externally
callable helper function.  Internally, it calls tffsDevFormat() with a
pointer to a 'FormatParams' structure initialized to values that leave a
space on the flash device for a boot image. This space is outside the
region managed by TrueFFS.  This special region is necessary for boot
images because the normal translation and wear-leveling services of TrueFFS
are incompatible with the needs of the boot program and the boot image it
relies upon.  To write a boot image (or any other data) into this area,
use tffsBootImagePut().
 
Finally, this file also contains define statements for symbolic constants
that determine which MTDs, translation layer modules, and other utilities
are ultimately included in TrueFFS.  These defines are as follows:
 
.IP "INCLUDE_TL_NFTL"
To include the NAND-based translation layer module.
.IP "INCLUDE_TL_FTL"
To include the NOR-based translation layer module.
.IP "INCLUDE_TL_SSFDC"
To include the SSFDC-appropriate translation layer module.
.IP "INCLUDE_MTD_I28F016"
For Intel 28f016 flash devices.
.IP "INCLUDE_MTD_I28F008"
For Intel 28f008 flash devices.
.IP "INCLUDE_MTD_I28F008_BAJA"
For Intel 28f008 flash devices on the Heurikon Baja 4700.
.IP "INCLUDE_MTD_AMD"
For AMD, Fujitsu: 29F0{40,80,16} 8-bit flash devices.
.IP "INCLUDE_MTD_MIRRORBIT"
For AMD, S29GL512/256/128N MirrorBit 16-bit flash devices.
.IP "INCLUDE_MTD_CDSN"
For Toshiba, Samsung: NAND CDSN flash devices.
.IP "INCLUDE_MTD_DOC2"
For Toshiba, Samsung: NAND DOC flash devices.
.IP "INCLUDE_MTD_CFISCS"
For CFI/SCS flash devices.
.IP "INCLUDE_MTD_WAMD"
For AMD, Fujitsu 29F0{40,80,16} 16-bit flash devices.
.IP "INCLUDE_TFFS_BOOT_IMAGE"
To include tffsBootImagePut() in TrueFFS for Tornado.
.LP
To exclude any of the modules mentioned above, edit sysTffs.c and undefine
its associated symbolic constant.

INCLUDE FILES: flsocket.h
*/

/* includes */

#include "vxWorks.h"
#include "config.h"
#include "dosFsLib.h"
#include "tffs/flsocket.h"
#include "tffs/tffsDrv.h"
#include "taskLib.h"
#include "intLib.h"
#include "logLib.h"

/* defines */

#undef	INCLUDE_MTD_I28F016	/* Intel: 28f016 */
#undef	INCLUDE_MTD_I28F008	/* Intel: 28f008 */
#undef 	INCLUDE_MTD_AMD		/* AMD, Fujitsu: 29f0{40,80,16} 8bit */
#undef	INCLUDE_MTD_CDSN	/* Toshiba, Samsung: NAND, CDSN */
#undef	INCLUDE_MTD_DOC2	/* Toshiba, Samsung: NAND, DOC */
#undef	INCLUDE_MTD_CFISCS	/* CFI/SCS */
#undef	INCLUDE_MTD_WAMD	/* AMD, Fujitsu: 29f0{40,80,16} 16bit */
#undef	INCLUDE_TL_NFTL		/* NFTL translation layer */
#define	INCLUDE_TL_FTL		/* FTL translation layer */
#undef	INCLUDE_TL_SSFDC	/* SSFDC translation layer */
#undef	INCLUDE_TFFS_BOOT_IMAGE	/* include tffsBootImagePut() */
#define	INCLUDE_MTD_MIRRORBIT	/* AMD S29GL512/256/128N MirrorBit Flash */

/* BSP contains MTD for AMD MirrorBit Flash */

#ifdef INCLUDE_MTD_MIRRORBIT
#   define INCLUDE_MTD_USR
#   define MTD_USR_IDENTIFY	sysFlashTFFSIdentify
#endif

/* typedefs */

/* globals */

/* locals */

/* forward declarations */

LOCAL FLBoolean	rfaCardDetected (FLSocket vol);
LOCAL void	rfaVccOn (FLSocket vol);
LOCAL void	rfaVccOff (FLSocket vol);
#ifdef	SOCKET_12_VOLTS
LOCAL FLStatus	rfaVppOn (FLSocket vol);
LOCAL void	rfaVppOff (FLSocket vol);
#endif	/* SOCKET_12_VOLTS */
LOCAL FLBoolean	rfaGetAndClearCardChangeIndicator (FLSocket vol);
LOCAL FLBoolean	rfaWriteProtected (FLSocket vol);
LOCAL void	rfaSetWindow (FLSocket vol);
LOCAL void	rfaSetMappingContext (FLSocket vol, UINT page);
LOCAL FLStatus	rfaSocketInit (FLSocket vol);
LOCAL FLStatus	rfaRegister (void);
void sysTffsMountDevs ();

/* externals */

#include "sysFlashTffs.c"	/* AMD MirrorBit MTD */

#ifndef DOC
#   include "tffs/tffsConfig.c"
#endif /* DOC */

#ifdef INCLUDE_RTC
   IMPORT void setAnsiSysTime (void);
#endif

IMPORT UINT32 tffsBankSize;
IMPORT UINT32 getFlashBaseAddress (void);

/*******************************************************************************
*
* sysTffsInit - initialize TFFS flash socket interface routine.
*
* This routine calls the socket registration routines for the socket component
* drivers that will be used with this BSP. The order of registration signifies
* the logical drive number assigned to the drive associated with the socket.
*
* RETURNS: N/A
*/

LOCAL void sysTffsInit (void)
    {
    rfaRegister ();
    }

/*******************************************************************************
*
* rfaRegister - install routines for the RFA
*
* This routine installs necessary functions for the RFA.
*
* RETURNS: flOK, or flTooManyComponents if there're too many drives
*/

static FLStatus rfaRegister (void)
    {
    FLSocket vol = flSocketOf (noOfDrives);

    if (noOfDrives >= DRIVES)
        return (flTooManyComponents);

    tffsSocket[noOfDrives] = "RFA";
    noOfDrives++;

    vol.serialNo = 0;
    vol.window.baseAddress = getFlashBaseAddress() >> 12;

    /* fill in function pointers */

    vol.cardDetected      = rfaCardDetected;
    vol.VccOn             = rfaVccOn;
    vol.VccOff            = rfaVccOff;
#ifdef SOCKET_12_VOLTS
    vol.VppOn             = rfaVppOn;
    vol.VppOff            = rfaVppOff;
#endif
    vol.initSocket        = rfaSocketInit;
    vol.setWindow         = rfaSetWindow;
    vol.setMappingContext = rfaSetMappingContext;
    vol.getAndClearCardChangeIndicator =
                          rfaGetAndClearCardChangeIndicator;
    vol.writeProtected    = rfaWriteProtected;

    return (flOK);
    }

/*******************************************************************************
*
* rfaCardDetected - detect if a card is present (inserted)
*
* This routine detects if a card is present (inserted).
* Always return TRUE in RFA environments since device is not removable.
*
* RETURNS: TRUE, or FALSE if the card is not present.
*/

static FLBoolean rfaCardDetected
    (
    FLSocket vol
    )
    {
    return (TRUE);
    }

/*******************************************************************************
*
* rfaVccOn - turn on Vcc (3.3/5 Volts)
*
* This routine turns on Vcc (3.3/5 Volts).  Vcc must be known to be good
* on exit.  Assumed to be ON constantly in RFA environment.
*
* RETURNS: N/A
*/

static void rfaVccOn 
    (
    FLSocket vol
    )
    {
    }

/*******************************************************************************
*
* rfaVccOff - turn off Vcc (3.3/5 Volts)
*
* This routine turns off Vcc (3.3/5 Volts).  Assumed to be ON
* constantly in RFA environment. 
*
* RETURNS: N/A
*/

static void rfaVccOff 
    (
    FLSocket vol
    )
    {
    }

#ifdef SOCKET_12_VOLTS
/*******************************************************************************
*
* rfaVppOn - turns on Vpp (12 Volts)
*
* This routine turns on Vpp (12 Volts). Vpp must be known to be good on exit.
* Assumed to be ON constantly in RFA environment. This is not optional and
* must always be implemented.
*
* RETURNS: flOK always
*/

static FLStatus rfaVppOn
    (
    FLSocket vol		/* pointer identifying drive */
    )
    {
    return (flOK);
    }

/*******************************************************************************
*
* rfaVppOff - turns off Vpp (12 Volts)
*
* This routine turns off Vpp (12 Volts).  Assumed to be ON constantly
* in RFA environment.This is not optional and must always be implemented.
*
* RETURNS: N/A
*/

static void rfaVppOff 
    (
    FLSocket vol		/* pointer identifying drive */
    ) 
    {
    }
#endif	/* SOCKET_12_VOLTS */

/*******************************************************************************
*
* rfaSocketInit - perform all necessary initializations of the socket
*
* This routine performs all necessary initializations of the socket.
*
* RETURNS: flOK always
*/

static FLStatus rfaSocketInit
    (
    FLSocket vol		/* pointer identifying drive */
    ) 
    {
    return (flOK);
    }

/*******************************************************************************
*
* rfaSetWindow - set current window attributes, Base address, size, etc
*
* This routine sets current window hardware attributes: Base address, size,
* speed and bus width.  The requested settings are given in the 'vol.window' 
* structure.  If it is not possible to set the window size requested in
* 'vol.window.size', the window size should be set to a larger value, 
* if possible. In any case, 'vol.window.size' should contain the 
* actual window size (in 4 KB units) on exit.
*
* RETURNS: N/A
*/

static void rfaSetWindow
    (
    FLSocket vol		/* pointer identifying drive */
    ) 
    {

    /* Physical base as a 4K page */

    vol.window.baseAddress = getFlashBaseAddress() >> 12;

    flSetWindowSize (&vol, tffsBankSize >> 12);
    }

/*******************************************************************************
*
* rfaSetMappingContext - sets the window mapping register to a card address
*
* This routine sets the window mapping register to a card address.
* The window should be set to the value of 'vol.window.currentPage',
* which is the card address divided by 4 KB. An address over 128MB,
* (page over 32K) specifies an attribute-space address.
*
* The page to map is guaranteed to be on a full window-size boundary.
*
* RETURNS: N/A
*/

static void rfaSetMappingContext 
    (
    FLSocket vol,		/* pointer identifying drive */
    UINT page			/* page to be mapped */
    )
    {
    }

/*******************************************************************************
*
* rfaGetAndClearCardChangeIndicator - return the hardware card-change indicator
*
* This routine returns the hardware card-change indicator and clears it if set.
*
* RETURNS: FALSE, or TRUE if the card has been changed
*/

static FLBoolean rfaGetAndClearCardChangeIndicator
    (
    FLSocket vol		/* pointer identifying drive */
    ) 
    {
    return (FALSE);
    }

/*******************************************************************************
*
* rfaWriteProtected - return the write-protect state of the media
*
* This routine returns the write-protect state of the media
*
* RETURNS: FALSE, or TRUE if the card is write-protected
*/

static FLBoolean rfaWriteProtected 
    (
    FLSocket vol		/* pointer identifying drive */
    ) 
    {
    return (FALSE);
    }

/*******************************************************************************
*
* flFitInSocketWindow - check whether the flash array fits in the socket window
*
* This routine checks whether the flash array fits in the socket window.
*
* RETURNS: A chip size guaranteed to fit in the socket window.
*/

long int flFitInSocketWindow
    (
    long int chipSize,		/* size of single physical chip in bytes */
    int      interleaving,	/* flash chip interleaving (1,2,4 etc) */
    long int windowSize		/* socket window size in bytes */
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

/*******************************************************************************
*
* flDelayLoop - consume the specified time
*
* This routine consumes the specified time.
*
* RETURNS: N/A
*/

void flDelayLoop 
    (
    int cycles			/* loop count to be consumed */
    )
    {
    volatile int counter;

    for (counter = 0; counter < cycles; counter++)
	;
    }

/*******************************************************************************
*
* sysTffsFormat - format the flash memory above an offset
*
* This routine formats the flash memory.  Because this function defines
* the symbolic constant, HALF_FORMAT, the lower half of the specified flash
* memory is left unformatted.  If the lower half of the flash memory was
* previously formated by TrueFFS, and you are trying to format the upper half,
* you need to erase the lower half of the flash memory before you format the
* upper half.  To do this, you could use:
* .CS
* tffsRawio(0, 3, 0, 8)
* .CE
* The first argument in the tffsRawio() command shown above is the TrueFFS
* drive number, 0.  The second argument, 3, is the function number (also
* known as TFFS_PHYSICAL_ERASE).  The third argument, 0, specifies the unit
* number of the first erase unit you want to erase.  The fourth argument, 8,
* specifies how many erase units you want to erase.
*
* RETURNS: OK, or ERROR if it fails.
*/

STATUS sysTffsFormat 
    (
    int driveNo /* TFFS drive number */
    )
    {
    STATUS status =0;
    char devName[5] = "RFAn";
    tffsDevFormatParams params = 
	{
	{0x00000000, 99, 1, 0x10000l, NULL, {0,0,0,0}, NULL, 2, 0, NULL},
	FTL_FORMAT_IF_NEEDED
	};

    devName[3] = '0' + driveNo;
    params.formatParams.volumeLabel = devName;
    tffsRawio(driveNo, TFFS_PHYSICAL_ERASE, 0, 64, 0);
    status = tffsDevFormat (driveNo, (int)&params);
    return (status);
    }

/*******************************************************************************
*
* sysTffsMount - Mount tffs filesystem if it exists
*
* Mount tffs filesystem if it exists.
*
* RETURNS: 1 if FLASH FS is usable, 0 otherwise.
*/

int sysTffsMount
    (
    int driveNo /* TFFS drive number */
    )
    {
    int status, fd;
    char devName[6] = "/RFAn";
    char devNameOpen[8] = "/RFAn/.";

    devName[4] = '0'+driveNo;
    devNameOpen[4] = '0'+driveNo;

    if (usrTffsConfig(driveNo, 0, devName) == OK)
        status = 1;
    else
        status = 0;

    if (status)
        {
#ifndef INCLUDE_RTC
        struct timespec ts;

        /* Set time later than 1/99 toa void start-up warnings */

        ts.tv_sec = 32 * 365 * 24 * 60 * 60; /* ~2002 */
        ts.tv_nsec = 0;
        clock_settime(CLOCK_REALTIME,&ts);
#endif

        /*
         * Check filesystem consistency at start-up -- unfortunately
         * it does print that the fs is clean and really isn't silent.
         */

        fd = open(devNameOpen, O_RDONLY, 0);
        if (fd >= 0)
            {
            ioctl(fd, FIOCHKDSK, DOS_CHK_REPAIR|DOS_CHK_VERB_SILENT);
            close(fd);
            }
        }
    return status;
    }

/*******************************************************************************
*
* sysTffsMountDevs - mount all available TFFS devices
*
* This routine mounts all available TFFS devices.
*
*/

void sysTffsMountDevs ()
    {
    int flshDevNum;

#ifdef INCLUDE_RTC
    setAnsiSysTime();
#endif

    for (flshDevNum = 0; flshDevNum < NUM_TFFS_VOLS; flshDevNum++)
        {
        if (!sysTffsMount(flshDevNum))
            {
            printf("TFFS Volume %d is unusable. Formatting...",flshDevNum);
            if ( sysTffsFormat(flshDevNum) == ERROR)
                {
                printf("Failed\n");
                }
            else
                {
                printf("Done\nMounting Drive...\n");
                if (!sysTffsMount(flshDevNum))
                    printf("Mount failed again. Giving up\n");
                else
                    printf("Mount successful\n");
                }
            }
        }
    }
