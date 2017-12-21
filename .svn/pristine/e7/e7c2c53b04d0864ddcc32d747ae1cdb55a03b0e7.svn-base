/* 40vxbFileNvRam.cdf - Component configuration file */ 
        
/* Copyright (c) 2007 Wind River Systems, Inc. */
                                                         
/*
modification history
--------------------
01b,05sep07,tor  defect00101978
01a,18jul07,tor created
*/

Component       DRV_NVRAM_FILE {
        NAME            Driver for file-based non-volatile RAM support
        SYNOPSIS            Driver for file-based non-volatile RAM support
        _CHILDREN       FOLDER_DRIVERS
        _INIT_ORDER     hardWareInterFaceBusInit
        INIT_RTN        vxbFileNvRamRegister();
	PROTOTYPE       void vxbFileNvRamRegister(void);
	MODULES		vxbFileNvRam.o
        REQUIRES        INCLUDE_PLB_BUS \
                        INCLUDE_NON_VOLATILE_RAM
}
