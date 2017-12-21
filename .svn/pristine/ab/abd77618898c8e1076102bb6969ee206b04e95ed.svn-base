/* 10wrload.cdf - VxWorks Multicore WRLoad components */

/*
 * Copyright (c) 2008, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,26aug11,ers  Adding generic wrload map/unmap support.
01b,02sep08,ebh  remove the REQUIRE INCLUDE_AMP (breaks hpcNet8641_AMP_coreX
                 if not other BSPs)
01a,14apr08,dtr  created
*/


Folder FOLDER_WRLOAD {
    NAME          WR Load
    SYNOPSIS      AMP ELF image loader and core.
    _CHILDREN     FOLDER_MULTIOS
}


Component INCLUDE_WRLOAD {
    NAME           WRLoad
    SYNOPSIS       ELF image loader and routines for executing a separate OS \
on a secondary core.
    CONFIGLETTES   usrWrload.c
    PROTOTYPE      void usrWrloadInit(void);
    INIT_RTN       usrWrloadInit();
    _INIT_ORDER    usrToolsInit
    INIT_BEFORE    usrShellInit
    _CHILDREN      FOLDER_WRLOAD
    LINK_SYMS      wrload
}

Component INCLUDE_WRLOAD_IMAGE_BUILD {
    NAME           WRLoad Build Downloadable Image
    SYNOPSIS       Configures the build for vxWorks kernel images to support \
being loaded and run on secondary cores via wrload. This is should not be \
included for CPU 0.
    _CHILDREN      FOLDER_WRLOAD
    CFG_PARAMS     WRLOAD_IMAGE_BUILD_PHYS_BIAS
}

Parameter WRLOAD_IMAGE_BUILD_PHYS_BIAS {
    NAME            Image Physical Load Bias
    SYNOPSIS        Offset added to the ELF file's load memory address (LMA) \
when placing it into physical memory. This is not supported by all arch's and \
all BSPs. For targets that don't support it, setting this parameter will have \
no effect.
    DEFAULT         0x0000000000000000
}

/* 
 * The following component is place-holder to attach the WRLOAD_..._FUNC
 * parameters whenever they are included in a BSP. As the parameters are
 * almost always set once at BSP creation and forgotten, we are placing this
 * component that holds them in the hidden folder. Doing so prevents unnecessary
 * clutter in the WB Kernel Configuration tool.
 */

Component INCLUDE_WRLOAD_REGISTRATION_API {
    NAME           WRLoad Registration
    SYNOPSIS       Unconditional wrload component for holding the conditional \
parameters for specifiying the BSP & arch specific routines.
    _REQUIRES      INCLUDE_WRLOAD
    _CHILDREN      FOLDER_NOT_VISIBLE
}


/*
 * The parameters below are used by comps/src/usrWrload.c to specify routines 
 * to provide memory for loading an image.  Setting the WRLOAD_CPU_MEM_MAP_FUNC
 * DEFAULT to usrWrloadMap and the WRLOAD_CPU_MEM_UNMAP_FUNC DEFAULT to 
 * usrWrloadUnmap uses the generic routines contained within usrWrload.c.
 * This is done by placing the Parameter blocks below (un-commented) with the 
 * DEFAULT set to an appropriate function, into 20wrload.cdf in the BSP 
 * directory.
 *
 */

/*
 * Parameter WRLOAD_PRI_SCRATCH_OFFSET {
 *     NAME            Primary's Virtual Offset
 *     SYNOPSIS        Offset the primary core applies to the ELF load memory \
 * address (LMA) before placing it in local virtual memory. The physical memory \
 * address will be the ELF load address plus the Physical Load Bias.
 *     DEFAULT         0x00000000
 *    _CFG_PARAMS      INCLUDE_WRLOAD
 * }
 * 
 * Parameter WRLOAD_CPU_MEM_MAP_FUNC {
 *     NAME            Memory Map function
 *     SYNOPSIS        Routine wrload will call to map the ELF load memory address \
 * (LMA) to virtual space in the primary core.
 *     DEFAULT         NULL
 *    _CFG_PARAMS      INCLUDE_WRLOAD_REGISTRATION_API 
 * }
 * 
 * Parameter WRLOAD_CPU_MEM_UNMAP_FUNC {
 *     NAME            Memory Unmap function
 *     SYNOPSIS        Routine wrload will call to unmap memory of the primary \
 * core after it is finished loading an ELF section into memory.
 *     DEFAULT         NULL
 *    _CFG_PARAMS      INCLUDE_WRLOAD_REGISTRATION_API 
 * }
 * 
 * Parameter WRLOAD_CPU_MEM_READ_FUNC {
 *     NAME            wrload Read function
 *     SYNOPSIS        Read a buffer of memory: read(id,addr,buffer,size).
 *     DEFAULT         NULL
 *    _CFG_PARAMS      INCLUDE_WRLOAD_REGISTRATION_API
 * }
 * 
 * Parameter WRLOAD_CPU_MEM_WRITE_FUNC {
 *     NAME            wrload Write function
 *     SYNOPSIS        Write a buffer of memory: write(id,addr,buffer,size).
 *     DEFAULT         NULL
 *    _CFG_PARAMS      INCLUDE_WRLOAD_REGISTRATION_API 
 * }
 * 
 * Parameter WRLOAD_CPU_MEM_FILL_FUNC {
 *     NAME            wrload Fill function
 *     SYNOPSIS        Fill a buffer of memory: fill(id,addr,buffer,size).
 *     DEFAULT         NULL
 *    _CFG_PARAMS      INCLUDE_WRLOAD_REGISTRATION_API 
 * }
 * 
 * Parameter WRLOAD_CPU_MEM_OPEN_FUNC {
 *     NAME            wrload Open function
 *     SYNOPSIS        Open a shared memory file to access memory.
 *     DEFAULT         NULL
 *    _CFG_PARAMS      INCLUDE_WRLOAD_REGISTRATION_API 
 * }
 * 
 * Parameter WRLOAD_CPU_MEM_CLOSE_FUNC {
 *     NAME            wrload Close function
 *     SYNOPSIS        Close the file opened by WRLOAD_CPU_OPEN_FUNC.
 *     DEFAULT         NULL
 *    _CFG_PARAMS      INCLUDE_WRLOAD_REGISTRATION_API 
 * }
 * 
 * Parameter WRLOAD_CPU_PREPARE_FUNC {
 *     NAME            CPU Prepare
 *     SYNOPSIS        Used only when a generic arch implementation exists for BSP \
 * sysAmpCpuPrep()
 *     DEFAULT         NULL
 *    _CFG_PARAMS      INCLUDE_WRLOAD_REGISTRATION_API 
 * }
 * 
 * Parameter WRLOAD_CPU_ENABLE_FUNC {
 *     NAME            CPU Enable
 *     SYNOPSIS        Used only when a generic arch implementation exists for BSP \
 * sysAmpCpuEnable()
 *     DEFAULT         NULL
 *    _CFG_PARAMS      INCLUDE_WRLOAD_REGISTRATION_API 
 * }
 */
