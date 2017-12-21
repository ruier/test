/* romStart.c - ROM initialization module */

/*
 * Copyright (c) 1997-1998, 2001-2005, 2007-2012 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
02z,04jan12,c_l  Remove the build warning for default_romResident image.
                 (WIND00317509)
02b,11aug11,z_l  Add BSP_BOOT_CACHE_SYNC_POST. (WIND00265986)
02a,04aug11,d_c  Defect WIND00291057 Fix WIND00291556.
                 Add long_call attribute to all functions when compiling
		 with GNU and INCLUDE_FAST_LOAD is enabled.
01z,24mar11,j_l  LP64 non-compressed non-uefi bootApp adaptation for I80X86 only.
01y,17jan11,pgh  Add MIPS LMM support.
01x,13jan11,pgh  Speed up ROM image boot time.
01w,07jul10,jb   Fix for Defect WIND00222124 - Bootline not initialized on cold
                 boot
01v,18may10,cwl  add the summary of macro "RAM_DST_ADRS". (WIND00198042)
01u,10feb10,j_l  LP64 compressed non-uefi bootApp adaptation for I80X86 only.
01t,18sep09,slk  make sure jump to absEntry is in unampped space for MIPS
01s,21oct08,pch  CQ135132: generate a build error if bad mappings cause
                 overflow in RAM sizes
01r,13oct08,rbc  Update for efi header move.
01q,27aug08,slk  enable SDA support for MIPS
01p,22aug08,rbc  Changes for passing EFI memory map and ACPI table pointer.
01o,16jul07,dcc  Added SDA initialization for MIPS, although not yet enabled.
01n,18jun07,to   fixed data segment copy size for ROM_RESIDENT
01m,08may07,dcc  added SDA initialization for PPC in romStart().
01l,07apr05,cjj  fixed copyLongs() compiler warnings (SPR107455)
01k,16sep04,j_b  correct RAM clearing (SPR 99727)
01j,19aug04,j_b  clear BOOT_LINE_ADRS on cold boot (SPR 100637)
01i,18aug04,j_b  PM_RESERVED_MEM is dependent on INCLUDE_EDR_PM
01h,11may04,j_b  add support for PM_RESERVED_MEM
01g,13jan03,j_b  add ARM THUMB support for ROM_COPY and ROM_COMPRESSED images
                 (SPR 80008)
01f,26mar02,sbs  correcting data section relocation for ROM compressed images
                 (SPR 73785)
01e,29nov01,sbs  Adding dual copy mechanism for copying data sections from
                 ROM to RAM using new WRS labels for sdata and edata.
01d,27nov01,tpw  Manually unroll copyLongs and fillLongs.
01c,06jul98,jmp  added USER_RESERVED_MEM definition if USER_RESERVED_MEM is
                 not defined.
01b,20may98,ms   minor cleanup to docs and code.
01a,22aug97,ms   derived from 04p of bootInit.c and simplified.
*/

/*
DESCRIPTION
This module provides a generic ROM startup facility.  The target-specific
romInit.s module performs the minimal preliminary board initialization and
then jumps to the C routine romStart(). It is assumed that the romInit.s
module has already set the initial stack pointer to STACK_ADRS.
This module optionally clears memory to avoid parity errors, initializes
the RAM data area, and then jumps to the routine "relocEntry".
This module is a replacement for config/all/bootInit.c.

CLEARING MEMORY
One feature of this module is to optionally clear memory to avoid parity
errors.
Generally, we clear for the bottom of memory (the BSP's LOCAL_MEM_LOCAL_ADRS
macro), to the top of memory (LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE),
but avoiding the stack area. In order to avoid trashing the stack area,
the BSP's romInit.s must have set the stack pointer to the macro STACK_ADRS.
There are three configuration macros which can be overridden in the
BSP's config.h file to change how memory is cleared:
ROMSTART_BOOT_CLEAR undefine to not clear memory when booting
USER_RESERVED_MEM   area at the top of memory to not clear
RESERVED        area at the bottom of memory to not clear
.bS 22
    RAM
    --------------  0x00100000 = LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE
    |            |
    |------------|      = " - USER_RESERVED_MEM
    |            |
    |  0 filled  |
    |            |
    |------------|  0x00001000  = RAM_DATA_ADRS
    | initial sp |
    |------------|      = RAM_DATA_ADRS - STACK_SAVE
    |            |
    | 0 filled   |
    |            |  exc vectors, bp anchor, exc msg, bootline
    |            |
    |------------|      = LOCAL_MEM_LOCAL_ADRS + RESERVED
    |            |
    --------------  0x00000000  = LOCAL_MEM_LOCAL_ADRS
.bE
Note: The STACK_ADRS macro defaults to either RAM_DATA_ADRS, or
RAM_DATA_ADRS - STACK_SAVE, depending on if the stack grows up
or down for the given processor.
Note: The RAM_DATA_ADRS macro is passed to us from the Make system.
It represents the address to which the data segment is linked
and defaults to the BSP Makefile's RAM_LOW_ADRS macro. For details on
how to change the addresses to which VxWorks is linked, refer to the
documentation in h/make/defs.vxWorks.

INITIALIZING DATA
The other main feture of this module is to initialize the RAM data
area.
The way this is done depends on the kind of image you are building.
There are three ROM image types that are supported by the Make
system:
  ROM_RESIDENT  run from ROM, with only the data segment placed in RAM.
  ROM_COPY  copy the main image from ROM to RAM and jump to it.
  ROM_COMPRESS  uncompress the main image from ROM to RAM and jump to it.
The make system will define one of these macros for you, depending on the
name of the target you are building (%.rom_res, %.rom_cpy, or %.rom_cmp).
There are separate version of romStart for each of these cases.
Let's start with the simplest case; ROM_RESIDENT.

ROM_RESIDENT IMAGES
When the board is powered on, the processor starts executing instructions
from ROM. The ROM contains some text (code) and a copy of the data
segment. The pictured below is a typical example:
.bS 6
    ROM
    --------------
    |            |
    |------------|
    |    data    |
    |------------|  0xff8xxxxx  = ROM_DATA_ADRS
    |    text    |
    |            |  0xff800008  = ROM_TEXT_ADRS
    --------------  0xff800000  = ROM_BASE_ADRS
.bE
romStart() copies the data segment from ROM_DATA_ADRS to RAM_DATA_ADRS -
the address to which it is really linked.
The picture below is an example of RAM addresses on a 1 meg board.
.bS 22
    RAM
    --------------  0x00100000 = LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE
    |            |
    |            |
    |------------|      = RAM_DATA_ADRS + data segment size
    |            |
    |data segment|
    |            |
    |------------|  0x00001000  = RAM_DATA_ADRS
    | initial sp |
    |------------|      = RAM_DATA_ADRS - STACK_SAVE
    |            |
    |            |
    --------------  0x00000000  = LOCAL_MEM_LOCAL_ADRS
.bE

ROM_COPY and ROM_COMPRESS IMAGES
These images are slightly more complex than ROM_RESIDENT images.
The Make system produces an intermediate image, called %.ram_reloc,
which runs from RAM. It then embeds the binary for this subimage within
a ROM image. The subimage is located between the global symbols
"binArrayStart" and "binArrayEnd". The main difference between
the ROM_COPY and the ROM_COMPRESS images is how the subimage is
stored.
For ROM_COMPRESS, the subimage is compressed to save ROM space.
A modified version of the Public Domain \f3zlib\fP library is used to
uncompress the VxWorks boot ROM executable linked with it.  Compressing
object code typically achieves over 55% compression, permitting much
larger systems to be burned into ROM.  The only expense is the added few
seconds delay while the first two stages complete.
For ROM_COPY, the subimage is not compressed to make the
startup sequence shorter.
In both cases, the ROM image initially looks like this, with
binArrayStart and binArrayEnd at the end of the ROM data area.
The pictured below is a typical example:
.bS 6
    ROM
    --------------
    |            |
    |------------|  0xff8xxxxx  = binArrayEnd
    |  subImage  |
    |------------|  0xff8xxxxx  = binArrayStart
    |    data    |
    |------------|  0xff8xxxxx  = ROM_DATA_ADRS
    |    text    |
    |            |  0xff800008  = ROM_TEXT_ADRS
    --------------  0xff800000  = ROM_BASE_ADRS
.bE
For ROM_COPY images, romStart() copies the subimage from binArrayStart to
RAM_DST_ADRS
For ROM_UNCOMPRESS images, romStart() first copies the uncompression
code's data segment to RAM_DATA_ADRS, and then runs the uncompression
routine to place the subimage at RAM_DST_ADRS.
Both the RAM_DST_ADRS and RAM_DATA_ADRS macros are link addressed
defined by the Make system, which default to the BSP Makefile's
RAM_LOW_ADRS and RAM_HIGH_ADRS respectively.
For information on how to change link addresses, refer to the file
target/h/make/rules.bsp.
The picture below is an example of RAM addresses on a 1 meg board.
"temp data" below refers to the uncompression code's data segment.
.bS 22
        RAM
    --------------  0x00100000 = LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE
    |            |
    |            |
    |------------|
    | temp data  |
    |------------|  0x00090000  = RAM_DATA_ADRS
    | initial sp |
    |------------|              = RAM_DATA_ADRS - STACK_SAVE
    |            |
    |            |
    |------------|
    |            |
    |  subimage  |
    |            |
    |------------|  0x00001000  = RAM_DST_ADRS (for non-resident images)
    |            |
    |            |
    --------------  0x00000000  = LOCAL_MEM_LOCAL_ADRS
.bE

SEE ALSO:
inflate(), romInit(), and deflate.

AUTHOR
The original compression software was written by Jean-loup Gailly
and Mark Adler. See the manual pages of inflate and deflate for
more information on their freely available compression software.

SUMMARY OF CONFIGURATION MACROS
Macro       defined     synopsys
-----       -------     --------
ROM_BASE_ADRS   config.h    Start of ROM
ROM_TEXT_ADRS   Makefile    Start of text segment within ROM, typically
                            a CPU-dependant offset from ROM_BASE_ADRS.
ROM_DATA_ADRS   Here        Location in ROM containing a copy of the
                            data segment. This must eventually be copied
                            to the RAM address to which it is linked.
RAM_DST_ADRS   defs.vxWorks (for non-romResident VxWorks ROM images) Location
                            in RAM to which the code segment is really linked,
                            it's the entry of VxWorks executable image, it's
                            always the link address of usrEntry() (for vxWork
                            romCompress&romCopy image),or sysInit()(for the
                            downloadnable VxWorks image), see CQ:WIND00198048.

STACK_ADRS           configAll.h  Initial stack pointer.
ROMSTART_BOOT_CLEAR  configAll.h  Undefine to not clear memory when booting
USER_RESERVED_MEM    configAll.h  Area at the top of memory to not clear
RESERVED             configAll.h  Area at the bottom of memory to not clear
LOCAL_MEM_LOCAL_ADRS config.h     on-board memory base
LOCAL_MEM_SIZE       config.h     on-board memory size

CHANGES from bootInit.c:
Three separate versions of romStart to make it readable.
One each for ROM_COMPRESS, ROM_RESIDENT, and ROM_COPY images.
* The ROM_COPY image copies the main image from binArrayStart to
  RAM_DST_ADRS and then jumps to it.
  The array binArrayStart and the macro RAM_DST_ADRS are provided by the
  Make system.
* The ROM_COMPRESS image uncompresses the main image from binArrayStart to
  RAM_DST_ADRS and then jumps to it.
  Before the uncompression can run, this image needs to initialize the
  uncompression code's data segment. It does this by copying data from
  ROM_DATA to RAM_DATA_ADRS.
  The array binArrayStart and the macros RAM_DST_ADRS and RAM_DATA_ADRS
  are provided by the Make system. The macro ROM_DATA is a toolchain
  dependant macro, which defaults to the global symbol "etext".
* The ROM_RESIDENT image copys its data segment from ROM to RAM. As with
  the ROM_COMPRESS image, it does this by copying from ROM_DATA to
  RAM_DATA_ADRS.
*/

#include <vxWorks.h>
#include <sysLib.h>
#include <config.h>
#include <usrConfig.h>

/* default uncompression routine for compressed ROM images */

#ifndef UNCMP_RTN
#   define  UNCMP_RTN   inflate
#endif

/* toolchain dependant location of the data segment within ROM */

#ifndef ROM_DATA_ADRS
#   define  ROM_DATA_ADRS   (UINT *)(etext)
#endif

/* MIPS needs to write to uncached memory, others just do normal writes */

#ifndef UNCACHED
#    define UNCACHED(adr)   adr
#endif

/* USER_RESERVED_MEM must be set to 0 if not defined */
#ifndef USER_RESERVED_MEM
#    define USER_RESERVED_MEM 0
#endif

/* handy macros */

#ifdef INCLUDE_FAST_LOAD

#ifdef __GNUC__
#define _WRS_LONG_CALL          __attribute__ ((long_call))
#else  /* ! __GNUC__ */
#define _WRS_LONG_CALL
#endif /* __GNUC__ */

#define _WRS_INFLATE_IN_RAM     _WRS_LONG_CALL \
                                __attribute__ ((__section__ (".inflate.text")))
#define _WRS_COPYLONG_IN_RAM    _WRS_LONG_CALL \
                                __attribute__ ((__section__ (".copy.text")))

#else /* INCLUDE_FAST_LOAD */

#define _WRS_INFLATE_IN_RAM
#define _WRS_COPYLONG_IN_RAM

#endif /* INCLUDE_FAST_LOAD */

#if defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86)
#define ROM_DATA(adrs) ((ULONG)adrs + ((ULONG)ROM_DATA_ADRS - RAM_DATA_ADRS))
#else
#define ROM_DATA(adrs) ((UINT)adrs + ((UINT)ROM_DATA_ADRS - RAM_DATA_ADRS))
#endif /* defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86) */

#ifdef  INCLUDE_EDR_PM
#    define SYS_MEM_TOP \
        (LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE - USER_RESERVED_MEM \
         - PM_RESERVED_MEM)
#else
#    define SYS_MEM_TOP \
        (LOCAL_MEM_LOCAL_ADRS + LOCAL_MEM_SIZE - USER_RESERVED_MEM)
#endif

#define SYS_MEM_BOTTOM \
    (LOCAL_MEM_LOCAL_ADRS + RESERVED)

#if defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86)
#define BINARRAYEND_ROUNDOFF    (ROUND_DOWN(binArrayEnd, sizeof(UINT)))
#elif defined(INCLUDE_LARGE_MEMORY_MODEL) && (CPU_FAMILY == MIPS)
#define BINARRAYEND_ROUNDOFF    (ROUND_DOWN(unmappedBinArrayEnd, sizeof(long)))
#else /* defined(INCLUDE_LARGE_MEMORY_MODEL) && (CPU_FAMILY == MIPS) */
#define BINARRAYEND_ROUNDOFF    (ROUND_DOWN(binArrayEnd, sizeof(long)))
#endif /* defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86) */

#if defined (ROMSTART_BOOT_CLEAR) && !defined (ROM_RESIDENT)
# define LD_IMAGE_END  ((UINT)RAM_DST_ADRS + \
               (UINT)(ROUND_UP(binArrayEnd - binArrayStart, sizeof(long))))
#endif /* defined (ROMSTART_BOOT_CLEAR) && !defined (ROM_RESIDENT) */

/* forward declarations */

/* 
 * BSP_BOOT_CACHE_SYNC_POST is an optional defined macro that a BSP
 * can use. BSP_BOOT_CACHE_SYNC_POST is used before the final jump of 
 * romStart(). BSP developer can add any codes in this definition. For example,
 * if MMU/cache is enabled in romInit.s, and the developer does not want the 
 * routines after romStart() to be effected by romInit.s, then operation to 
 * disable MMU/cache can be implemented by BSP in BSP_BOOT_CACHE_SYNC_POST.
 *
 * The BSP provided function has no arguments.
 *
 * .eg:
 * #define BSP_BOOT_CACHE_SYNC_POST sysBspBootCacheSyncPost()
 */

#ifdef BSP_BOOT_CACHE_SYNC_POST
void BSP_BOOT_CACHE_SYNC_POST;
#endif /* BSP_BOOT_CACHE_SYNC_POST */

/* 
 * copyLongs() is only used for some image types.
 * This prevents compiler warnings.
 */
#if (defined(ROM_COMPRESS) || defined(ROM_COPY) || defined(ROM_RESIDENT))
#if defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86)
LOCAL void copyLongs (FAST UINT *source, FAST UINT *destination, size_t nlongs);
#else
#ifdef INCLUDE_FAST_LOAD
LOCAL void romCopyLongs (FAST UINT *, FAST UINT *, UINT);
LOCAL _WRS_COPYLONG_IN_RAM void copyLongs (FAST UINT *, FAST UINT *, UINT);
#else /* INCLUDE_FAST_LOAD */
LOCAL void copyLongs (FAST UINT *source, FAST UINT *destination, UINT nlongs);
#endif /* INCLUDE_FAST_LOAD */
#endif /* defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86) */
#endif /* ROM_xxx */

#ifdef  ROMSTART_BOOT_CLEAR
#if defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86)
LOCAL void fillLongs (FAST UINT *buf, size_t nlongs, FAST UINT val);
#else
LOCAL void fillLongs (FAST UINT *buf, UINT nlongs, FAST UINT val);
#endif /* defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86) */
LOCAL void bootClear (void);
#endif

#if defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86)
#ifdef INCLUDE_UEFI_BOOT_SUPPORT
typedef FUNCPTR 	START_FUNC_TYPE; /* ptr to start function */
#else
typedef int 		(*START_FUNC_TYPE) (int); /* ptr to start function */
#endif /* INCLUDE_UEFI_BOOT_SUPPORT */
#endif /* defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86) */

/* imports */

#ifdef INCLUDE_FAST_LOAD
IMPORT char     wrs_kernel_copy_text_start[];
IMPORT char     wrs_kernel_copy_text_end[];
IMPORT _WRS_INFLATE_IN_RAM int  UNCMP_RTN (unsigned char *, unsigned char *, int);
#else /* INCLUDE_FAST_LOAD */
IMPORT STATUS   UNCMP_RTN ();
#endif /* INCLUDE_FAST_LOAD */
IMPORT void     relocEntry ();
IMPORT UCHAR    binArrayStart [];       /* compressed binary image */
IMPORT UCHAR    binArrayEnd [];     /* end of compressed binary image */
IMPORT char     etext [];               /* defined by the loader */
IMPORT char     end [];                 /* defined by the loader */
IMPORT char     wrs_kernel_data_end []; /* defined by the loader */
#ifdef INCLUDE_LARGE_MEMORY_MODEL
IMPORT char unmappedBinArrayEnd[];
IMPORT char unmappedBinArrayStart[];
#endif /* INCLUDE_LARGE_MEMORY_MODEL */

#ifdef INCLUDE_UEFI_BOOT_SUPPORT
#include <private/efi/uefiVxWorksTypes.h>  /* UEFI types in VxWorksish form */
#include <private/efi/efiVxWorks.h> /* UEFI definitions */
IMPORT VxWorksOSLoaderMemoryMapHeader *pRomUefiMemAddr;
IMPORT UINT32 *pRomUefiAcpiAddr;
#endif


#if (CPU_FAMILY == PPC) || (CPU_FAMILY == MIPS)
IMPORT void vxSdaInit (void);
#endif	/* (CPU_FAMILY == PPC) || (CPU_FAMILY == MIPS) */

#ifdef  ROM_COMPRESS
/*******************************************************************************
*
* romStart - generic ROM initialization for compressed images
*
* This is the first C code executed after reset.
*
* This routine is called by the assembly start-up code in romInit().
* It clears memory, copies ROM to RAM, and invokes the uncompressor.
* It then jumps to the entry point of the uncompressed object code.
*
* RETURNS: N/A
*/

void romStart
    (
    FAST int startType      /* start type */
    )
    {
#if defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86)
    volatile START_FUNC_TYPE absEntry;
#else
    volatile FUNCPTR absEntry;
#endif /* defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86) */
#ifdef INCLUDE_FAST_LOAD
    int     (*uncompress) (unsigned char *, unsigned char *, int);
    void    (*cpyLngPtr) (FAST UINT *, FAST UINT *, UINT);
    int     copyLongLength; /* length of the copyLong() function */
#endif /* INCLUDE_FAST_LOAD */

#if (CPU_FAMILY == PPC) || (CPU_FAMILY == MIPS)
    
    /* 
     * For PPC and MIPS, the call to vxSdaInit() must be the first operation 
     * in sysStart(). This is because vxSdaInit() sets the SDA registers
     * (r2 and r13 on PPC, gp on MIPS) to the SDA base values. No C code 
     * must be placed before this call.
     */

    _WRS_ASM ("");   /* code barrier to prevent compiler moving vxSdaInit() */
    vxSdaInit ();    /* this MUST be the first operation in usrInit() for PPC */
    _WRS_ASM ("");   /* code barrier to prevent compiler moving vxSdaInit() */

#endif	/* (CPU_FAMILY == PPC) || (CPU_FAMILY == MIPS) */

#if defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86)
    absEntry = (volatile START_FUNC_TYPE)((ULONG)RAM_DST_ADRS);
#else
    absEntry = (FUNCPTR) RAM_DST_ADRS;
#endif /* defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86) */

    /* relocate the data segment of the decompression stub */

#if defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86)
    copyLongs ((UINT *)ROM_DATA_ADRS, (UINT *)UNCACHED(RAM_DATA_ADRS),
               (size_t)(((size_t)(void *)binArrayStart - (size_t)RAM_DATA_ADRS) / sizeof (UINT)));

    copyLongs ((UINT *)((ULONG)ROM_DATA_ADRS + ((ULONG)BINARRAYEND_ROUNDOFF -
               (ULONG)RAM_DATA_ADRS)), (UINT *)UNCACHED(BINARRAYEND_ROUNDOFF),
               (size_t)(((size_t)(void *)wrs_kernel_data_end - (size_t)(void *)binArrayEnd) / sizeof (UINT)));
#elif defined(INCLUDE_LARGE_MEMORY_MODEL) && (CPU_FAMILY==MIPS)
#ifdef INCLUDE_FAST_LOAD
    /*
     * Copy the initial values of the .data section from ROM to the .data
     * section in RAM.  The copyLongs() function and the inflate() functions
     * are located in the .data section.  The copyLongs() function is located
     * at the beginning of the .data section by the linker script.
     */

    cpyLngPtr = copyLongs;
    copyLongLength = wrs_kernel_copy_text_end - wrs_kernel_copy_text_start;
    romCopyLongs (ROM_DATA_ADRS, (UINT *)RAM_DATA_ADRS, copyLongLength);
    cpyLngPtr (ROM_DATA_ADRS + copyLongLength,
               (UINT *)RAM_DATA_ADRS + copyLongLength,
               ((UINT)(void *)binArrayStart - (UINT)(RAM_DATA_ADRS + copyLongLength)) /
                sizeof (long));
    cpyLngPtr ((UINT *)((UINT)ROM_DATA_ADRS +
                        ((UINT)BINARRAYEND_ROUNDOFF - (UINT)RAM_DATA_ADRS)),
               (UINT *)(BINARRAYEND_ROUNDOFF),
               ((UINT)(void *)wrs_kernel_data_end - (UINT)(void *)unmappedBinArrayEnd) / 
                sizeof (long));
#else /* INCLUDE_FAST_LOAD */
    copyLongs (ROM_DATA_ADRS, (UINT *)(RAM_DATA_ADRS),
               ((UINT)(void *)binArrayStart - (UINT)RAM_DATA_ADRS) / sizeof (long));
    copyLongs ((UINT *)((UINT)ROM_DATA_ADRS + 
                        ((UINT)BINARRAYEND_ROUNDOFF - (UINT)RAM_DATA_ADRS)),
               (UINT *)(BINARRAYEND_ROUNDOFF),
               ((UINT)(void *)wrs_kernel_data_end - (UINT)(void *)unmappedBinArrayEnd) / 
                sizeof (long));
#endif /* INCLUDE_FAST_LOAD */
#else /* defined(INCLUDE_LARGE_MEMORY_MODEL) && (CPU_FAMILY==MIPS) */
#ifdef INCLUDE_FAST_LOAD
    /*
     * Copy the initial values of the .data section from ROM to the .data
     * section in RAM.  The copyLongs() function and the inflate() functions
     * are located in the .data section.  The copyLongs() function is located
     * at the beginning of the .data section by the linker script.
     */

    cpyLngPtr = copyLongs;
    copyLongLength = wrs_kernel_copy_text_end - wrs_kernel_copy_text_start;
    romCopyLongs (ROM_DATA_ADRS, (UINT *)UNCACHED(RAM_DATA_ADRS),
                  copyLongLength);
    cpyLngPtr (ROM_DATA_ADRS + copyLongLength,
               (UINT *)UNCACHED(RAM_DATA_ADRS) + copyLongLength,
               ((UINT)(void *)binArrayStart - (UINT)(RAM_DATA_ADRS + copyLongLength)) /
                sizeof (long));
    cpyLngPtr ((UINT *)((UINT)ROM_DATA_ADRS +
                        ((UINT)BINARRAYEND_ROUNDOFF - (UINT)RAM_DATA_ADRS)),
               (UINT *)UNCACHED(BINARRAYEND_ROUNDOFF),
               ((UINT)(void *)wrs_kernel_data_end - (UINT)(void *)binArrayEnd) / 
                sizeof (long));
#else /* INCLUDE_FAST_LOAD */
    copyLongs (ROM_DATA_ADRS, (UINT *)UNCACHED(RAM_DATA_ADRS),
              ((UINT)(void *)binArrayStart - (UINT)RAM_DATA_ADRS) / sizeof (long));

    copyLongs ((UINT *)((UINT)ROM_DATA_ADRS + ((UINT)BINARRAYEND_ROUNDOFF -
        (UINT)RAM_DATA_ADRS)), (UINT *)UNCACHED(BINARRAYEND_ROUNDOFF),
    ((UINT)(void *)wrs_kernel_data_end - (UINT)(void *)binArrayEnd) / sizeof (long));
#endif /* INCLUDE_FAST_LOAD */
#endif /* defined(INCLUDE_LARGE_MEMORY_MODEL) && (CPU_FAMILY==MIPS) */

    /* if cold booting, start clearing memory to avoid parity errors */

    if (startType & BOOT_CLEAR)
        {
#if  defined(ROMSTART_BOOT_CLEAR) && !defined(_WRS_CONFIG_LP64) && !defined(INCLUDE_LARGE_MEMORY_MODEL)
        /* low memory is cleared up to the stack */
        bootClear();
#endif /* defined(ROMSTART_BOOT_CLEAR) && !defined(_WRS_CONFIG_LP64) && !defined(INCLUDE_LARGE_MEMORY_MODEL) */

        /*
         * Ensure the boot line is null. This is necessary for those
         * targets whose boot line is excluded from cleaning or a value
         * other than zero is written to the boot line.
         */

        *(BOOT_LINE_ADRS) = EOS;
        }

    /* decompress the main image */

#if (CPU_FAMILY == MIPS) && defined(INCLUDE_LARGE_MEMORY_MODEL)
#ifdef INCLUDE_FAST_LOAD
    uncompress = UNCMP_RTN;
    if (uncompress ((unsigned char *)ROM_DATA(unmappedBinArrayStart),
                    (unsigned char *)LOCAL_UNMAPPED_BASE_ADRS,
                    (unmappedBinArrayEnd - unmappedBinArrayStart)) != OK)
        {
        return;
        }
    if (uncompress ((unsigned char *)ROM_DATA(binArrayStart),
                    (unsigned char *)RAM_DST_ADRS,
                    (binArrayEnd - binArrayStart)) != OK)
        {
        return;
        }
#else /* INCLUDE_FAST_LOAD */
    if (UNCMP_RTN (ROM_DATA(unmappedBinArrayStart),
                    LOCAL_UNMAPPED_BASE_ADRS,
                    (unmappedBinArrayEnd - unmappedBinArrayStart)) != OK)
        {
        return;
        }
    if (UNCMP_RTN (ROM_DATA(binArrayStart),
                    RAM_DST_ADRS,
                    (binArrayEnd - binArrayStart)) != OK)
        {
        return;
        }
#endif /* INCLUDE_FAST_LOAD */
#else /* (CPU_FAMILY == MIPS) && defined(INCLUDE_LARGE_MEMORY_MODEL) */
#ifdef INCLUDE_FAST_LOAD

    uncompress = UNCMP_RTN;
    if (uncompress ((unsigned char *)ROM_DATA(binArrayStart),
                    (unsigned char *)UNCACHED(RAM_DST_ADRS),
                    binArrayEnd - binArrayStart) != OK)
        {
        return;
        }
#else /* INCLUDE_FAST_LOAD */
    if (UNCMP_RTN (ROM_DATA(binArrayStart),
                    UNCACHED(RAM_DST_ADRS),
                    binArrayEnd - binArrayStart) != OK)
        {
        return;
        }
#endif /* INCLUDE_FAST_LOAD */
#endif /* (CPU_FAMILY == MIPS) && defined(INCLUDE_LARGE_MEMORY_MODEL) */

    /* if cold booting, finish clearing memory */

#ifdef  ROMSTART_BOOT_CLEAR
    if (startType & BOOT_CLEAR)
    /* clear past the stack to the top of memory */
# if (RAM_DATA_ADRS > SYS_MEM_TOP)
#error Bad size, RAM_DATA_ADRS > SYS_MEM_TOP, check LOCAL_MEM SIZE and RAM_HIGH ADRS settings
# endif	/* RAM_DATA_ADRS > SYS_MEM_TOP */
#if defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86)
    fillLongs ((UINT *)UNCACHED(RAM_DATA_ADRS),
               (size_t)(((size_t)SYS_MEM_TOP - (size_t)RAM_DATA_ADRS) / sizeof(UINT), 0));
#else
    fillLongs ((UINT *)UNCACHED(RAM_DATA_ADRS),
        ((UINT)SYS_MEM_TOP - (UINT)RAM_DATA_ADRS) / sizeof(long), 0);
#endif /* defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86) */
#endif /* ROMSTART_BOOT_CLEAR */

#ifdef BSP_BOOT_CACHE_SYNC_POST
    BSP_BOOT_CACHE_SYNC_POST;
#endif /* BSP_BOOT_CACHE_SYNC_POST */

#if     ((CPU_FAMILY == ARM) && ARM_THUMB)
     absEntry = (FUNCPTR)((UINT32)absEntry | 1);         /* force Thumb state */
#endif  /* CPU_FAMILY == ARM */

#if (CPU_FAMILY == MIPS)
#ifdef  INCLUDE_LARGE_MEMORY_MODEL
    /*
    * For large memory model, jump to the base address of the unmapped section.
    */
    absEntry = (FUNCPTR)LOCAL_UNMAPPED_BASE_ADRS;
#else /* INCLUDE_LARGE_MEMORY_MODEL */
    /* mapped ROMs have start address in kseg2 so it needs to be modified
     * so we jump to unmapped space
     */
    absEntry = (FUNCPTR)KX_TO_K0(absEntry);
#endif /* INCLUDE_LARGE_MEMORY_MODEL */
#endif /* (CPU_FAMILY == MIPS) */

/* and jump to the entry */

#ifdef INCLUDE_UEFI_BOOT_SUPPORT
    /* For UEFI we must pass the UEFI memory map and ACPI pointer */
    absEntry (startType, pRomUefiMemAddr, pRomUefiAcpiAddr);
#else
    absEntry (startType);
#endif

    }
#endif  /* ROM_COMPRESS */

#ifdef  ROM_COPY
/*******************************************************************************
*
* romStart - generic ROM initialization for uncompressed ROM images
*
* This is the first C code executed after reset.
*
* This routine is called by the assembly start-up code in romInit().
* It clears memory, copies ROM to RAM, and then jumps to the entry
* point of the copied object code.
*
* RETURNS: N/A
*/

void romStart
    (
    FAST int startType      /* start type */
    )
    {
#if defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86)
    volatile START_FUNC_TYPE absEntry;
#else
    volatile FUNCPTR absEntry;
#endif /* defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86) */
#ifdef INCLUDE_FAST_LOAD
    void    (*cpyLngPtr) (FAST UINT *, FAST UINT *, UINT);
    int     copyLongLength; /* length of the copyLong() function */
#endif /* INCLUDE_FAST_LOAD */

#if (CPU_FAMILY == PPC) || (CPU_FAMILY == MIPS)
    
    /* 
     * For PPC and MIPS, the call to vxSdaInit() must be the first operation 
     * in sysStart(). This is because vxSdaInit() sets the SDA registers
     * (r2 and r13 on PPC, gp on MIPS) to the SDA base values. No C code 
     * must be placed before this call.
     */

    _WRS_ASM ("");   /* code barrier to prevent compiler moving vxSdaInit() */
    vxSdaInit ();    /* this MUST be the first operation in usrInit() for PPC */
    _WRS_ASM ("");   /* code barrier to prevent compiler moving vxSdaInit() */

#endif	/* (CPU_FAMILY == PPC) || (CPU_FAMILY == MIPS) */

#if (CPU_FAMILY == MIPS) && defined(INCLUDE_LARGE_MEMORY_MODEL)
    /*
    * For large memory model, jump to the base address of the unmapped section.
    */
    absEntry = (FUNCPTR)LOCAL_UNMAPPED_BASE_ADRS;
#elif ((CPU_FAMILY == ARM) && ARM_THUMB)
    absEntry = (FUNCPTR)((UINT32)absEntry | 1);         /* force Thumb state */
#elif ((CPU_FAMILY==I80X86) && defined(_WRS_CONFIG_LP64))
    absEntry = (volatile START_FUNC_TYPE)((ULONG)RAM_DST_ADRS);
#else
    absEntry = (volatile FUNCPTR) RAM_DST_ADRS;
#endif  /* CPU_FAMILY == ARM,MIPS && defined(INCLUDE_LARGE_MEMORY_MODEL) */

    /* copy the main image into RAM */

#if ((CPU_FAMILY==I80X86) && defined(_WRS_CONFIG_LP64))
    copyLongs ((UINT *)ROM_DATA(binArrayStart),
        (UINT *)UNCACHED(RAM_DST_ADRS),
        ((size_t)(void *)binArrayEnd - (size_t)(void *)binArrayStart) / sizeof (UINT));
#elif (CPU_FAMILY == MIPS) && defined(INCLUDE_LARGE_MEMORY_MODEL)
#ifdef INCLUDE_FAST_LOAD
    cpyLngPtr = copyLongs;
    copyLongLength = wrs_kernel_copy_text_end - wrs_kernel_copy_text_start;
    romCopyLongs (ROM_DATA_ADRS, (UINT *)RAM_DATA_ADRS, copyLongLength);
    cpyLngPtr ((UINT *)ROM_DATA(unmappedBinArrayStart),
               (UINT *)LOCAL_UNMAPPED_BASE_ADRS,
               (unmappedBinArrayEnd - unmappedBinArrayStart) / sizeof (long));
    cpyLngPtr ((UINT *)ROM_DATA(binArrayStart),
               (UINT *)RAM_DST_ADRS,
               (binArrayEnd - binArrayStart) / sizeof (long));
#else /* INCLUDE_FAST_LOAD */
    copyLongs ((UINT *)ROM_DATA(unmappedBinArrayStart),
                (UINT *)LOCAL_UNMAPPED_BASE_ADRS,
                (unmappedBinArrayEnd - unmappedBinArrayStart) / sizeof (long));
    copyLongs ((UINT *)ROM_DATA(binArrayStart),
                (UINT *)RAM_DST_ADRS,
                (binArrayEnd - binArrayStart) / sizeof (long));
#endif /* INCLUDE_FAST_LOAD */
#else /* (CPU_FAMILY == MIPS) && defined(INCLUDE_LARGE_MEMORY_MODEL) */
#ifdef INCLUDE_FAST_LOAD
    cpyLngPtr = copyLongs;
    copyLongLength = wrs_kernel_copy_text_end - wrs_kernel_copy_text_start;
    romCopyLongs (ROM_DATA_ADRS, (UINT *)UNCACHED(RAM_DATA_ADRS),
                  copyLongLength);
    cpyLngPtr ((UINT *)ROM_DATA(binArrayStart),
                (UINT *)UNCACHED(RAM_DST_ADRS),
                (binArrayEnd - binArrayStart) / sizeof (long));
#else /* INCLUDE_FAST_LOAD */
    copyLongs ((UINT *)ROM_DATA(binArrayStart),
        (UINT *)UNCACHED(RAM_DST_ADRS),
        (binArrayEnd - binArrayStart) / sizeof (long));
#endif /* INCLUDE_FAST_LOAD */
#endif /* (CPU_FAMILY == MIPS) && defined(INCLUDE_LARGE_MEMORY_MODEL) */

    /* If cold booting, clear memory to avoid parity errors */

    if (startType & BOOT_CLEAR)
        {
#if  defined(ROMSTART_BOOT_CLEAR) && !defined(_WRS_CONFIG_LP64) && !defined(INCLUDE_LARGE_MEMORY_MODEL)
        /* low memory is cleared up to the stack */
        bootClear();
#endif /* defined(ROMSTART_BOOT_CLEAR) && !defined(_WRS_CONFIG_LP64) && !defined(INCLUDE_LARGE_MEMORY_MODEL) */

        /*
         * Ensure the boot line is null. This is necessary for those
         * targets whose boot line is excluded from cleaning or a value
         * other than zero is written to the boot line.
         */

        *(BOOT_LINE_ADRS) = EOS;
        }

#ifdef BSP_BOOT_CACHE_SYNC_POST
    BSP_BOOT_CACHE_SYNC_POST;
#endif /* BSP_BOOT_CACHE_SYNC_POST */

#if (CPU_FAMILY == MIPS)
    /* mapped ROMs have start address in kseg2 so it needs to be modified
     * so we jump to uncached space
     */
    absEntry = (FUNCPTR)KX_TO_K0(absEntry);
#endif /* (CPU_FAMILY == MIPS) */

    /* and jump to the entry */


#ifdef INCLUDE_UEFI_BOOT_SUPPORT
    /* For UEFI we must pass the UEFI memory map and ACPI pointer */
    absEntry (startType, pRomUefiMemAddr, pRomUefiAcpiAddr);
#else
    absEntry (startType);
#endif

    }
#endif  /* ROM_COPY */

#ifdef  ROM_RESIDENT
/*******************************************************************************
*
* romStart - generic ROM initialization for ROM resident images
*
* This is the first C code executed after reset.
*
* This routine is called by the assembly start-up code in romInit().
* It clears memory, copies ROM to RAM, and invokes the uncompressor.
* It then jumps to the entry point of the uncompressed object code.
*
* RETURNS: N/A
*/

void romStart
    (
    FAST int startType      /* start type */
    )
    {
#if (CPU_FAMILY == PPC) || (CPU_FAMILY == MIPS)
    
    /* 
     * For PPC and MIPS, the call to vxSdaInit() must be the first operation 
     * in sysStart(). This is because vxSdaInit() sets the SDA registers
     * (r2 and r13 on PPC, gp on MIPS) to the SDA base values. No C code 
     * must be placed before this call.
     */

    _WRS_ASM ("");   /* code barrier to prevent compiler moving vxSdaInit() */
    vxSdaInit ();    /* this MUST be the first operation in usrInit() for PPC */
    _WRS_ASM ("");   /* code barrier to prevent compiler moving vxSdaInit() */

#endif	/* (CPU_FAMILY == PPC) || (CPU_FAMILY == MIPS) */

    /* relocate the data segment into RAM */

    copyLongs ((UINT *)ROM_DATA_ADRS, (UINT *)UNCACHED(RAM_DATA_ADRS),
        ((UINT)(void *)wrs_kernel_data_end - (UINT)RAM_DATA_ADRS) / sizeof (long));

    /* If cold booting, clear memory to avoid parity errors */

    if (startType & BOOT_CLEAR)
        {
#if  defined(ROMSTART_BOOT_CLEAR) && !defined(_WRS_CONFIG_LP64)
        /* low memory is cleared up to the stack */
        bootClear();
#endif /* defined(ROMSTART_BOOT_CLEAR) && !defined(_WRS_CONFIG_LP64) */

        /*
         * Ensure the boot line is null. This is necessary for those
         * targets whose boot line is excluded from cleaning or a value
         * other than zero is written to the boot line.
         */

        *(BOOT_LINE_ADRS) = EOS;
        }

    /* and jump to the entry */

#ifdef BSP_BOOT_CACHE_SYNC_POST
    BSP_BOOT_CACHE_SYNC_POST;
#endif /* BSP_BOOT_CACHE_SYNC_POST */

#ifdef INCLUDE_UEFI_BOOT_SUPPORT
    /* For UEFI we must pass the UEFI memory map and ACPI pointer */
    usrInit (startType, pRomUefiMemAddr, pRomUefiAcpiAddr);
#else
    usrInit (startType);
#endif

    }
#endif  /* ROM_RESIDENT */

#if  defined(ROMSTART_BOOT_CLEAR) && !defined(_WRS_CONFIG_LP64)
/******************************************************************************
*
* bootClear - clear memory
*
* If cold booting, clear memory not loaded with text & data.
*
* We are careful about initializing all memory (except
* STACK_SAVE bytes) due to parity error generation (on
* some hardware) at a later stage.  This is usually
* caused by read accesses without initialization.
*/

LOCAL void bootClear (void)
    {
#ifdef  ROM_RESIDENT

  /* fill from the bottom of memory to the load image */

# if ((STACK_SAVE+SYS_MEM_BOTTOM) > RAM_DST_ADRS)
#error Bad size, (STACK_SAVE+SYS_MEM_BOTTOM) > RAM_DST_ADRS, check RAM_*_ADRS settings
# endif	/* (STACK_SAVE+SYS_MEM_BOTTOM) > RAM_DST_ADRS */
    fillLongs ((UINT *)SYS_MEM_BOTTOM,
       ((UINT)RAM_DST_ADRS - STACK_SAVE - (UINT)SYS_MEM_BOTTOM) / sizeof (long),
        0);

    /* fill from the load image to the top of memory */

    fillLongs ((UINT *)end, ((UINT)SYS_MEM_TOP - (UINT)end) / sizeof(long), 0);

#else /* ROM_RESIDENT */

    /* fill from the bottom of memory to the load image */

# if (SYS_MEM_BOTTOM > RAM_DST_ADRS)
#error Bad size, (STACK_SAVE+SYS_MEM_BOTTOM) > RAM_DST_ADRS, check RAM_*_ADRS settings
# endif	/* SYS_MEM_BOTTOM > RAM_DST_ADRS */
    fillLongs ((UINT *)UNCACHED(SYS_MEM_BOTTOM),
        ((UINT)RAM_DST_ADRS - (UINT)SYS_MEM_BOTTOM) / sizeof (long), 0);

    /*
    * fill from the end of the load image to the stack
    * (end of a decompressed image isn't known, but this is ok as long as
    *  clearing is done before decompression is performed)
    */
    fillLongs ((UINT *)UNCACHED(LD_IMAGE_END),
        ((UINT)RAM_DATA_ADRS - STACK_SAVE - LD_IMAGE_END) / sizeof (long),
        0);

#  ifndef  ROM_COMPRESS
    /*
     * fill past the stack to the top of memory
     * (this section is cleared later with compressed images)
     */

# if (RAM_DATA_ADRS > SYS_MEM_TOP)
#error Bad size, RAM_DATA_ADRS > SYS_MEM_TOP, check LOCAL_MEM SIZE and RAM_HIGH ADRS settings
# endif	/* RAM_DATA_ADRS > SYS_MEM_TOP */
    fillLongs ((UINT *)UNCACHED(RAM_DATA_ADRS),
	((UINT)SYS_MEM_TOP - (UINT)RAM_DATA_ADRS) / sizeof(long), 0);

#  endif /* ROM_COMPRESS */

#endif /* ROM_RESIDENT */
    }

/*******************************************************************************
*
* fillLongs - fill a buffer with a value a long at a time
*
* This routine fills the first <nlongs> longs of the buffer with <val>.
*/

#if defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86)
LOCAL void fillLongs
    (
    FAST UINT * buf,    /* pointer to buffer              */
    size_t      nlongs, /* number of longs to fill        */
    FAST UINT   val /* char with which to fill buffer */
    )
#else
LOCAL void fillLongs
    (
    FAST UINT * buf,    /* pointer to buffer              */
    UINT    nlongs, /* number of longs to fill        */
    FAST UINT   val /* char with which to fill buffer */
    )
#endif /* defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86) */
    {
    FAST UINT *bufend = buf + nlongs;
#if defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86)
    FAST size_t nchunks;
#else
    FAST UINT nchunks;
#endif /* defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86) */

    /* Hop by chunks of longs, for speed. */
    for (nchunks = nlongs / 8; nchunks; --nchunks)
    {
#if (CPU_FAMILY == MC680X0)
    *buf++ = val;   /* 0 */
    *buf++ = val;   /* 1 */
    *buf++ = val;   /* 2 */
    *buf++ = val;   /* 3 */
    *buf++ = val;   /* 4 */
    *buf++ = val;   /* 5 */
    *buf++ = val;   /* 6 */
    *buf++ = val;   /* 7 */
#else
    buf[0] = val;
    buf[1] = val;
    buf[2] = val;
    buf[3] = val;
    buf[4] = val;
    buf[5] = val;
    buf[6] = val;
    buf[7] = val;
    buf += 8;
#endif
    }

    /* Do the remainder one long at a time. */
    while (buf < bufend)
    *buf++ = val;
    }
#endif /* defined(ROMSTART_BOOT_CLEAR) && !defined(_WRS_CONFIG_LP64) */

/* 
 * copyLongs() is only used for some image types.
 * This prevents compiler warnings.
 */
#if (defined(ROM_COMPRESS) || defined(ROM_COPY) || defined(ROM_RESIDENT))
#ifdef INCLUDE_FAST_LOAD
/*******************************************************************************
*
* romCopyLongs - copy one buffer to another a long at a time
*
* This routine copies the first <nlongs> longs from <source> to <destination>.
* This routine is used to copy the copyLongs() function from ROM to RAM, where
* it will be executed to copy the initial values of the .data section from ROM
* to RAM.
*/

LOCAL void romCopyLongs
    (
    FAST UINT * source,     /* pointer to source buffer      */
    FAST UINT * destination,    /* pointer to destination buffer */
    UINT    nlongs      /* number of longs to copy       */
    )
    {
    FAST UINT *dstend = destination + nlongs;
    FAST UINT nchunks;

    /* Hop by chunks of longs, for speed. */
    for (nchunks = nlongs / 8; nchunks; --nchunks)
        {
#if (CPU_FAMILY == MC680X0)
        *destination++ = *source++; /* 0 */
        *destination++ = *source++; /* 1 */
        *destination++ = *source++; /* 2 */
        *destination++ = *source++; /* 3 */
        *destination++ = *source++; /* 4 */
        *destination++ = *source++; /* 5 */
        *destination++ = *source++; /* 6 */
        *destination++ = *source++; /* 7 */
#else
        destination[0] = source[0];
        destination[1] = source[1];
        destination[2] = source[2];
        destination[3] = source[3];
        destination[4] = source[4];
        destination[5] = source[5];
        destination[6] = source[6];
        destination[7] = source[7];
        destination += 8, source += 8;
#endif
        }

    /* Do the remainder one long at a time. */
    while (destination < dstend)
        *destination++ = *source++;
    }
#endif /* INCLUDE_FAST_LOAD */

/*******************************************************************************
*
* copyLongs - copy one buffer to another a long at a time
*
* This routine copies the first <nlongs> longs from <source> to <destination>.
*/

#if defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86)
LOCAL void copyLongs
    (
    FAST UINT * source,     /* pointer to source buffer      */
    FAST UINT * destination,    /* pointer to destination buffer */
    size_t nlongs      /* number of longs to copy       */
    )
#else /* defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86) */
LOCAL _WRS_COPYLONG_IN_RAM void copyLongs
    (
    FAST UINT * source,     /* pointer to source buffer      */
    FAST UINT * destination,    /* pointer to destination buffer */
    UINT    nlongs      /* number of longs to copy       */
    )
#endif /* defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86) */
    {
    FAST UINT *dstend = destination + nlongs;
#if defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86)
    FAST size_t nchunks;
#else
    FAST UINT nchunks;
#endif /* defined(_WRS_CONFIG_LP64) && (CPU_FAMILY==I80X86) */

    /* Hop by chunks of longs, for speed. */
    for (nchunks = nlongs / 8; nchunks; --nchunks)
    {
#if (CPU_FAMILY == MC680X0)
    *destination++ = *source++; /* 0 */
    *destination++ = *source++; /* 1 */
    *destination++ = *source++; /* 2 */
    *destination++ = *source++; /* 3 */
    *destination++ = *source++; /* 4 */
    *destination++ = *source++; /* 5 */
    *destination++ = *source++; /* 6 */
    *destination++ = *source++; /* 7 */
#else
    destination[0] = source[0];
    destination[1] = source[1];
    destination[2] = source[2];
    destination[3] = source[3];
    destination[4] = source[4];
    destination[5] = source[5];
    destination[6] = source[6];
    destination[7] = source[7];
    destination += 8, source += 8;
#endif
    }

    /* Do the remainder one long at a time. */
    while (destination < dstend)
    *destination++ = *source++;
    }
#endif /* ROM_xxx */

#ifdef INCLUDE_FAST_LOAD
#include "inflateLib.c"
#endif /* INCLUDE_FAST_LOAD */
