/* 20wrload.cdf - VxWorks Multicore WRLoad components */

/*
 * Copyright (c) 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01c,26aug11,agf  created
*/

Parameter WRLOAD_PRI_SCRATCH_OFFSET {
    NAME            Primary's Virtual Offset
    SYNOPSIS        Offset the primary core applies to the ELF load memory \
address (LMA) before placing it in local virtual memory. The physical memory \
address will be the ELF load address plus the Physical Load Bias.
    DEFAULT         0x00000000
    _CFG_PARAMS     INCLUDE_WRLOAD
}

Parameter WRLOAD_CPU_MEM_MAP_FUNC {
    NAME            Memory Map function
    SYNOPSIS        Routine wrload will call to map the ELF load memory address\
 (LMA) to virtual space in the primary core.
    DEFAULT         usrWrloadMap
    _CFG_PARAMS     INCLUDE_WRLOAD_REGISTRATION_API
}

Parameter WRLOAD_CPU_MEM_UNMAP_FUNC {
    NAME            Memory Unmap function
    SYNOPSIS        Routine wrload will call to unmap memory of the primary \
core after it is finished loading an ELF section into memory.
    DEFAULT         usrWrloadUnmap
    _CFG_PARAMS     INCLUDE_WRLOAD_REGISTRATION_API
}

