/* usrWrload.c - wrload memory map support functions */

/*
* Copyright(c) 2011 Wind River Systems, Inc.
*
* The right to copy, distribute, modify or otherwise make use
* of this software may be licensed only pursuant to the terms
* of an applicable Wind River license agreement.
*/

/*
modification history
--------------------
01b,18oct11,agf  fix upper 32bit calc of page alignment in usrWrloadMap
                 (WIND00312514)
01a,09aug11,ers  CQ253685: inspired by version 01d of arch/mips/wrLoadMemMap.c
*/

/*
DESCRIPTION
This module initializes the wrload support routines for loading images to 
memory and kicking-off a secondary core to execute the loaded image. As these
support routines are at least arch-specific and sometimes target specific,
wrload provides this initialization as a means for target customization without
necessitating any changes or modifications to the core wrload itself.

usrWrloadInit() is automatically invoked in usrRoot() when the component
INCLUDE_WRLOAD is configured into the vxWorks kernel image. For any of the
defines WRLOAD_CPU_MAP_FUNC, WRLOAD_CPU_UNMAP_FUNC, WRLOAD_CPU_MEM_READ_FUNC,
etc. that are defined to real functions, the wrload core will call
the defined function. If they are not defined wrload will run with the
legacy functionality. Use of the WRLOAD_..._FUNC routines allow generic
arch-dependent and arch-independent implementations where possible, thus
relieving the amount of customization required in the BSP.

Additionally, this module contains a pair of generic memory map and unmap
routines that can be used by any vxWorks kernel that has memory management
available to it. These are arch-independent functions which only require the
vmBaseLib library. The generic usrWrloadMap and usrWrloadUnmap routines are
passed addresses and sizes to map which are not required to be page aligned.
The map routine must return the corresponding unaligned address, and the
subsequent unmap routine must calculate the page-aligned size correctly.

If INCLUDE_SHOW_ROUTINES is defined, usrWrloadPeek() allows dumping normally
inaccessible mapped memory from the shell.  This is useful for dumping the
exception line of a secondary core from the primary. For example:
usrWrloadPeek(0,0x10004300,300,0,1);

Besides the WRLOAD_..._FUNC macros, there are two configuration parameters
of potential interest. The first is WRLOAD_PRI_SCRATCH_OFFSET. It is
used to a add a virtual bias to the incoming ELF image so that if the lower 
32-bits of the loading ELF image were to overlay CPU 0's own virtual space it
can push the image to an available virtual area instead. This parameter is
not required by, and not available in all arch's and all targets. For targets
that support this parameter it will appear under the wrload component in
the WB kernel configurator, or it may be manipulated directly from the command
line with 'vxprj'.

The second configuration parameter is WRLOAD_IMAGE_BUILD_PHYS_BIAS. It adds a
physical memory bias to the incoming ELF image so a (for example) 1GB mapped
image can be moved to any 1GB boundary within a 64-bit physical address. This
parameter belongs to INCLUDE_WRLOAD_IMAGE_BUILD. It is not supported by all
targets. For targets that do not support it, setting it will have no effect.
The bootrom must support and will constrain the precision of this parameter.

Finally, the usrWrloadBiasHighGet() and usrWrloadBiasLowGet() routines provide
the WRLOAD_IMAGE_BUILD_PHYS_BIAS value so that boot (ROM) initialization MMU
mappings may dynamically map the compiled virtual address to the physical
load address.

INCLUDE FILES: vxWorks.h, vmLib.h, wrload.h, stdio.h, string.h
*/

/* #define  USR_WRLOAD_DEBUG */

/* includes */

#include <vxWorks.h>
#include <vmLib.h>
#include <wrload.h>
#include <stdio.h>  /* for printErr() */
#include <string.h> /* for memcpy() */

#ifdef  USR_WRLOAD_DEBUG 
#include <logLib.h>
#endif


/* globals */

/* currenly used by usrWrloadMap() to offset the image */

VIRT_ADDR wrloadPriScratchOffset = 
#ifdef WRLOAD_PRI_SCRATCH_OFFSET
    (VIRT_ADDR) WRLOAD_PRI_SCRATCH_OFFSET; 
#else
    0;
#endif

/* locals */

/* externals */

#if (    defined(WRLOAD_CPU_MEM_READ_FUNC)  \
      && defined(WRLOAD_CPU_MEM_WRITE_FUNC) \
      && defined(WRLOAD_CPU_MEM_FILL_FUNC)  \
    )
extern ssize_t WRLOAD_CPU_MEM_READ_FUNC();
extern ssize_t WRLOAD_CPU_MEM_WRITE_FUNC();
extern ssize_t WRLOAD_CPU_MEM_FILL_FUNC();
#endif


#if (    defined(WRLOAD_CPU_MEM_OPEN_FUNC) \
      && defined(WRLOAD_CPU_MEM_CLOSE_FUNC)\
    )
extern STATUS WRLOAD_CPU_MEM_OPEN_FUNC();
extern STATUS WRLOAD_CPU_MEM_CLOSE_FUNC();
#endif


#if (    defined(WRLOAD_CPU_MEM_MAP_FUNC)  \
      && defined(WRLOAD_CPU_MEM_UNMAP_FUNC)\
    )
extern void * WRLOAD_CPU_MEM_MAP_FUNC();
extern void   WRLOAD_CPU_MEM_UNMAP_FUNC();
#endif


#if (    defined(WRLOAD_CPU_PREPARE_FUNC)  \
      && defined(WRLOAD_CPU_ENABLE_FUNC)   \
    )
extern STATUS WRLOAD_CPU_PREPARE_FUNC();
extern STATUS WRLOAD_CPU_ENABLE_FUNC();
#endif

/* forward declarations */

/* typedefs */

/* macros */

#ifndef  TLB_CACHE_MODE
#define  TLB_CACHE_MODE         VM_STATE_CACHEABLE_WRITETHROUGH
#endif

/******************************************************************************
*
* usrWrloadBiasHighGet - Get the secondary's physical load bias (high 32-bits)
*
* Called by wrload to map a physical address to a virtual address.
*
* RETURNS:
*   The upper 32-bits of the physical load address bias.
*
* SEE ALSO: usrWrloadBiasLowGet() 
*/

UINT32 usrWrloadBiasHighGet(WRLOAD_ID id)
    {
    WRLOAD_ADDR_T value;
    UINT32  result = 0;

    if (wrloadSymValueGet(id,"_wrload_bias",&value, sizeof(value)) == OK)
        result = (UINT32)(value >> 32);

#ifdef  USR_WRLOAD_DEBUG
    logMsg("wrloadAimBasePhysHighGet:%x\n",result,0,0,0,0,0);
#endif
    return result;
    }

/******************************************************************************
*
* usrWrloadBiasLowGet - Get the secondary's physical load bias (low 32-bits)
*
* Called by wrload to map a physical address to a virtual address.
*
* RETURNS:
*   The lower 32-bits of the physical load address bias.
*
* SEE ALSO: usrWrloadBiasHighGet() 
*/

UINT32 usrWrloadBiasLowGet(WRLOAD_ID id)
    {
    WRLOAD_ADDR_T value;
    UINT32  result = 0;

    if (wrloadSymValueGet(id,"_wrload_bias",&value, sizeof(value)) == OK)
        result = (UINT32)value;

#ifdef  USR_WRLOAD_DEBUG
    logMsg("wrloadAimBasePhysLowGet:%x\n",result,0,0,0,0,0);
#endif
    return result;
    }


#ifdef INCLUDE_MMU_BASIC
/******************************************************************************
*
* usrWrloadMap - Map physical block for wrload
*
* Called by wrload to map a physical address to a virtual address. This is
* a generic implementation provided for convenience to all arch's and targets
* that support vmBaseLib. To use it set the parameter WRLOAD_CPU_MEM_MAP_FUNC
* to usrWrloadMap in the BSP.
*
* RETURNS:
*   The virtual address wrload will write to.
*/

void * usrWrloadMap  
    (
    WRLOAD_ID      id,     /* wrload ID */
    WRLOAD_ADDR_T  pAddr,  /* start physical address */
    size_t         size    /* size in bytes to map */
    )
    {
    WRLOAD_ADDR_T  pAddrAligned;
    VIRT_ADDR      vAddrAligned;
    STATUS         status;
    size_t         sizeAligned, difference;
    size_t         pgSz      = vmPageSizeGet();

    /* Align the address, increase size for unaligned pages. */

    pAddrAligned = pAddr & ~((WRLOAD_ADDR_T)pgSz-1);
    difference   = (pAddr - pAddrAligned);
    sizeAligned  = ROUND_UP(size + difference, pgSz);

    /* 1-to-1 mapping to the lower 32-bits of the physical address */

    vAddrAligned = ((VIRT_ADDR) pAddrAligned) + wrloadPriScratchOffset;

#ifdef  USR_WRLOAD_DEBUG
    logMsg("%x) Map:0x%x%x->%x, 0x%x->%x\n", id, (int)(pAddr>>32), (int)pAddr,
                 vAddrAligned, size, sizeAligned);
#endif

    status = vmPageMap(NULL, vAddrAligned, (PHYS_ADDR)pAddrAligned, sizeAligned,
                 MMU_ATTR_VALID_MSK | MMU_ATTR_PROT_MSK | MMU_ATTR_CACHE_MSK,
                 MMU_ATTR_VALID     | MMU_ATTR_SUP_RWX  | TLB_CACHE_MODE |
                 MMU_ATTR_CACHE_COHERENCY);

    return (status == OK) ? (void *) (vAddrAligned + difference) : NULL;
    }


/******************************************************************************
*
* usrWrloadUnmap - Unmap wrload memory block 
*
* Called by wrload after an ELF section is loaded to unmap the virtual space
* used to load the section. This is a generic implementation provided for
* convenience to all arch's and targets that support vmBaseLib. To use it set
* the parameter WRLOAD_CPU_MEM_MAP_FUNC to usrWrloadUnmap in the BSP.
*
* RETURNS: N/A
*/

void usrWrloadUnmap 
    (
    WRLOAD_ID  id,     /* wrload ID */
    void *     vAddr,  /* start virtual address */
    size_t     size    /* size in bytes to map */
    )
    {
    VIRT_ADDR  vAddrAligned;
    size_t     sizeAligned, difference;
    size_t     pgSz       = vmPageSizeGet();

    /* Align the address, increase size for unaligned pages. */

    vAddrAligned = ROUND_DOWN(vAddr, pgSz);
    difference   = ((size_t)vAddr - vAddrAligned);
    sizeAligned  = ROUND_UP(size + difference, pgSz);

#ifdef  USR_WRLOAD_DEBUG
    logMsg("%x) UNMap:0x%x->%x, 0x%x->%x\n", id, vAddr, vAddrAligned,
                                                 size,  sizeAligned, 0);
#endif

    if (vmPageUnmap(NULL, vAddrAligned, sizeAligned) != OK)
        printErr ("usrWrloadUnmap->vmPageUnmap(0,0x%x,0x%x) failed\n", 
                   vAddrAligned, sizeAligned);
    }

#endif /* INCLUDE_MMU_BASIC */

/******************************************************************************
*
* usrWrloadInit - Initialize support for wrload
*
* This function is always called when the INCLUDE_WRLOAD component is included.  
* wrloadCpuRtnSet() plugs the map/unmap, etc. functions into wrload if their
* corresponding parameter has been defined.  Backwards compatibility is 
* maintained by leaving the parameters undefined as is done in the top level
* 10wrload.cdf.  To use these functions, copy the desired parameter definitions
* from 10wrload.cdf to the local BSP directory then uncomment and specify the
* DEFAULT function for the parameter definitions of interest.
*
* RETURNS: N/A
*
* NOMANUAL
*/

void usrWrloadInit ()
    {

    /* If the map/unmap func's have been defined in this BSP, attach them */

#if (defined(WRLOAD_CPU_MEM_MAP_FUNC) && defined(WRLOAD_CPU_MEM_UNMAP_FUNC))
        {
        if (wrloadCpuRtnSet (WRLOAD_CPU_MEM_MAP,  
             (WRLOAD_CPU_RTN)WRLOAD_CPU_MEM_MAP_FUNC)   != OK)
            printErr("usrWrloadInit:Error setting WRLOAD_CPU_MEM_MAP_FUNC\n");

        else if (wrloadCpuRtnSet (WRLOAD_CPU_MEM_UNMAP,
                  (WRLOAD_CPU_RTN)WRLOAD_CPU_MEM_UNMAP_FUNC) != OK)
            printErr("usrWrloadInit:Error setting WRLOAD_CPU_MEM_UNMAP_FUNC\n");

        #ifdef  USR_WRLOAD_DEBUG
        else
            logMsg("Installed mapping functions:%x, %x\n",
                   WRLOAD_CPU_MEM_MAP_FUNC, WRLOAD_CPU_MEM_UNMAP_FUNC,0,0,0,0);
        #endif
	}
#endif /* defined(WRLOAD_CPU_MEM_MAP_FUNC) */

    /* If read/write/fill func's have been defined in this BSP, attach them */

#if (   defined(WRLOAD_CPU_MEM_READ_FUNC)  \
     && defined(WRLOAD_CPU_MEM_WRITE_FUNC) \
     && defined(WRLOAD_CPU_MEM_FILL_FUNC)   )
        {
        if (wrloadCpuRtnSet (WRLOAD_CPU_MEM_READ,  
             (WRLOAD_CPU_RTN)WRLOAD_CPU_MEM_READ_FUNC)    != OK)
            printErr("usrWrloadInit:Error setting WRLOAD_CPU_MEM_READ_FUNC\n");

        else if (wrloadCpuRtnSet (WRLOAD_CPU_MEM_WRITE, 
                  (WRLOAD_CPU_RTN)WRLOAD_CPU_MEM_WRITE_FUNC)   != OK)
            printErr("usrWrloadInit:Error setting WRLOAD_CPU_MEM_WRITE_FUNC\n");

        else if (wrloadCpuRtnSet (WRLOAD_CPU_MEM_FILL,  
                  (WRLOAD_CPU_RTN)WRLOAD_CPU_MEM_FILL_FUNC)    != OK)
            printErr("usrWrloadInit:Error setting WRLOAD_CPU_MEM_FILL_FUNC\n");

        #ifdef  USR_WRLOAD_DEBUG
        else
            logMsg("Installed read/write/fill functions:%x, %x, %x\n",
                    WRLOAD_CPU_MEM_READ_FUNC, WRLOAD_CPU_MEM_WRITE_FUNC, 
                    WRLOAD_CPU_MEM_FILL_FUNC, 0,0,0);
        #endif
        }
#endif /* defined(WRLOAD_CPU_MEM_READ_FUNC) */

    /* If open/close func's have been defined in this BSP, attach them */

#if ( defined(WRLOAD_CPU_MEM_OPEN_FUNC)  && defined(WRLOAD_CPU_MEM_CLOSE_FUNC))
        {
        if (wrloadCpuRtnSet (WRLOAD_CPU_MEM_OPEN,  
             (WRLOAD_CPU_RTN)WRLOAD_CPU_MEM_OPEN_FUNC)    != OK)
            printErr("usrWrloadInit:Error setting WRLOAD_CPU_MEM_OPEN_FUNC\n");

        if (wrloadCpuRtnSet (WRLOAD_CPU_MEM_CLOSE, 
             (WRLOAD_CPU_RTN)WRLOAD_CPU_MEM_CLOSE_FUNC)   != OK)
            printErr("usrWrloadInit:Error setting WRLOAD_CPU_MEM_CLOSE_FUNC\n");

        #ifdef  USR_WRLOAD_DEBUG
        else
            logMsg("Installed open/close functions:%x, %x, \n",
                   WRLOAD_CPU_MEM_OPEN_FUNC, WRLOAD_CPU_MEM_CLOSE_FUNC,0,0,0,0);
        #endif
        }
#endif /* defined(WRLOAD_CPU_MEM_OPEN_FUNC) */

    /* If prepare/enable func's have been defined in this BSP, attach them */

#if ( defined(WRLOAD_CPU_PREPARE_FUNC) && defined(WRLOAD_CPU_ENABLE_FUNC))
        {
        if (wrloadCpuRtnSet (WRLOAD_CPU_PREPARE,   
             (WRLOAD_CPU_RTN)WRLOAD_CPU_PREPARE_FUNC) != OK)
            printErr("usrWrloadInit:Error setting WRLOAD_CPU_PREPARE_FUNC\n");

        if (wrloadCpuRtnSet (WRLOAD_CPU_ENABLE,    
             (WRLOAD_CPU_RTN)WRLOAD_CPU_ENABLE_FUNC)  != OK)
            printErr("usrWrloadInit:Error setting WRLOAD_CPU_ENABLE_FUNC\n");

        #ifdef  USR_WRLOAD_DEBUG
         else
            logMsg("Installed prepare/enable functions:%x, %x, \n",
                    WRLOAD_CPU_PREPARE_FUNC, WRLOAD_CPU_ENABLE_FUNC, 0,0,0,0);
        #endif
        }
#endif /* defined(WRLOAD_CPU_PREPARE_FUNC) */

    }

#if (   defined(INCLUDE_SHOW_ROUTINES) \
     && defined(WRLOAD_CPU_MEM_MAP_FUNC) && defined(WRLOAD_CPU_MEM_UNMAP_FUNC))
/********************************************************************************
* usrWrloadPeek - Dump a physical address range (or copy to a buffer).
*
* This routine is handy for confirming from core 0 that the image loaded as
* expected.
*/

void usrWrloadPeek
    (
    UINT32     physAddrHi,
    UINT32     physAddrLo,
    size_t     nBytes,        /* size in bytes to map */
    char     * buf,           /* optional buffer to copy to */
    BOOL       printAsString  /* if TRUE, print the address as a string */
    )
    {
    WRLOAD_ADDR_T pAddr  = ((PHYS_ADDR)physAddrHi << 32) + physAddrLo;
    VIRT_ADDR     vAddr  = NULL;
    char          errStr[256];

    if (!physAddrHi && !physAddrLo && !nBytes && !buf && !printAsString)
        printf("\nUsage: usrWrloadPeek(physAddrHi,"
                                    "physAddrLo,nBytes,buf,printAsString)\n\n");
    if (nBytes > 0)
        vAddr = (VIRT_ADDR) WRLOAD_CPU_MEM_MAP_FUNC(0, pAddr, nBytes);

    if (vAddr != NULL)
        {
        if (buf != NULL)
            memcpy (buf, (char *) vAddr, nBytes);

        if (printAsString) 
            printf("\n0x%8.8x:%.300s\n\n",vAddr,vAddr);
        else if (memoryDump ((char *) vAddr, nBytes/2, 2, &errStr[0]) == ERROR)
            printErr ("usrWrloadPeek: %s\n", errStr);

        WRLOAD_CPU_MEM_UNMAP_FUNC(0, vAddr, nBytes);
        }
    }
#endif /* INCLUDE_SHOW_ROUTINES */

