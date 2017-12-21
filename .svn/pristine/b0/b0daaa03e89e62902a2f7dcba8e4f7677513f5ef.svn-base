/* 40vxbMipsIntCtlr.cdf - MIPS interrupt controller */

/*
 * Copyright (c) 2007-2008, 2010, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River License agreement.
 */

/*
modification history
--------------------
01l,08mar11,x_f  Add DRV_INTCTLR_MIPS_XLP defines.
01k,26jul10,d_c  Add requires for INCLUDE_CAVIUM_LIB component
01j,16jul10,rlg  remove vxbMipsCavIntCtlr.c CONFIGLETTE
01i,19oct08,jrp  WIND00136163
01h,06jun08,h_k  split ISR and IPI Emit portions from vxbMipsIntCtlr,
                 vxbMipsSbIntCtlr and vxbMipsXlrIntCtlr.
01g,08may08,slk  add DRV_INTCTLR_MIPS_MT_IPI define to controllers
01g,26dec07,l_z  Add INCLUDE_VXB_LEGACY_INTERRUPTS to xlr controllers
01f,09nov07,slk  add INCLUDE_VXB_LEGACY_INTERRUPTS define to controllers
01e,26jul07,pes  Make vxbMipsCavIntCtlr.c a CONFIGLETTE.
01e,26jul07,l_z  Add DRV_INTCTLR_MIPS_XLR define. 
01d,19jul07,jmt  Add DRV_INTCTLR_MIPS_CAV define.
01c,14jun07,h_k  changed to library build for DRV_INTCTLR_MIPS_SBE.
01b,13jun07,h_k  changed to library build for DRV_INTCTLR_MIPS.
01a,24Apr07,jmt  created
*/

Component DRV_INTCTLR_MIPS {
	NAME		MIPS Interrupt Controller driver
	SYNOPSIS	MIPS Interrupt Controller driver
	MODULES		vxbMipsIntCtlr.o \
			vxbMipsIntCtlrIsr.o
	REQUIRES	INCLUDE_VXBUS \
			INCLUDE_PLB_BUS \
			INCLUDE_VXB_LEGACY_INTERRUPTS \
			INCLUDE_INTCTLR_LIB
	_CHILDREN       FOLDER_DRIVERS
	INIT_RTN	vxbMipsIntCtlrRegister();
	PROTOTYPE	void vxbMipsIntCtlrRegister (void);
        INIT_AFTER      INCLUDE_PLB_BUS
        _INIT_ORDER     hardWareInterFaceBusInit
}

Component DRV_INTCTLR_MIPS_SBE {
	NAME		MIPS Sibyte Interrupt Controller driver
	SYNOPSIS	MIPS Sibyte Interrupt Controller driver
	MODULES		vxbMipsSbIntCtlr.o \
			vxbMipsSbIntCtlrIsr.o
	REQUIRES	INCLUDE_VXBUS \
			INCLUDE_PLB_BUS \
			INCLUDE_VXB_LEGACY_INTERRUPTS \
			INCLUDE_INTCTLR_LIB
	_CHILDREN       FOLDER_DRIVERS
	INIT_RTN	vxbMipsSbIntCtlrRegister();
	PROTOTYPE	void vxbMipsSbIntCtlrRegister (void);
        INIT_AFTER      INCLUDE_PLB_BUS
        _INIT_ORDER     hardWareInterFaceBusInit
}

Component DRV_INTCTLR_MIPS_CAV {
	NAME		MIPS Cavium Interrupt Controller driver
	SYNOPSIS	MIPS Cavium Interrupt Controller driver
	REQUIRES	INCLUDE_VXBUS \
			INCLUDE_PLB_BUS \
			INCLUDE_VXB_LEGACY_INTERRUPTS \
			INCLUDE_INTCTLR_LIB \
                        INCLUDE_CAVIUM_LIB
	_CHILDREN       FOLDER_DRIVERS
	INIT_RTN	vxbMipsCavIntCtlrRegister();
	PROTOTYPE	void vxbMipsCavIntCtlrRegister (void);
        INIT_AFTER      INCLUDE_PLB_BUS
        _INIT_ORDER     hardWareInterFaceBusInit
}

Component DRV_INTCTLR_MIPS_XLR {
	NAME		MIPS xlr Interrupt Controller driver
	SYNOPSIS	MIPS xlr Interrupt Controller driver
	MODULES		vxbMipsXlrIntCtlr.o \
			vxbMipsXlrIntCtlrIsr.o
	REQUIRES	INCLUDE_VXBUS \
			INCLUDE_PLB_BUS \
			INCLUDE_VXB_LEGACY_INTERRUPTS \
			INCLUDE_INTCTLR_LIB
	_CHILDREN	FOLDER_DRIVERS
	INIT_RTN	vxbMipsXlrIntCtlrRegister();
	PROTOTYPE	void vxbMipsXlrIntCtlrRegister (void);
	INIT_AFTER	INCLUDE_PLB_BUS
	_INIT_ORDER	hardWareInterFaceBusInit
}

Component DRV_INTCTLR_MIPS_XLP {
	NAME		MIPS NetLogic XLP Interrupt Controller driver
	SYNOPSIS	MIPS NetLogic XLP Interrupt Controller driver
	MODULES		vxbMipsXlpIntCtlr.o 
	REQUIRES	INCLUDE_VXBUS \
                        INCLUDE_PLB_BUS \
                        INCLUDE_INTCTLR_LIB
	_CHILDREN	FOLDER_DRIVERS
	INIT_RTN	xlpIntCtlrRegister();
	PROTOTYPE	void xlpIntCtlrRegister (void);
	INIT_AFTER	INCLUDE_PLB_BUS
	_INIT_ORDER	hardWareInterFaceBusInit
}

Component       DRV_INTCTLR_MIPS_MT_IPI {
        NAME            MT IPI IntCtlr driver
        SYNOPSIS        MT IPI IntCtlr driver to generate IPIs
        _CHILDREN       FOLDER_DRIVERS
        _INIT_ORDER     hardWareInterFaceBusInit
        INIT_RTN        vxbMipsMtIntCtlrRegister();
        REQUIRES        INCLUDE_VXBUS \
			INCLUDE_PLB_BUS \
			INCLUDE_VXB_LEGACY_INTERRUPTS \
			INCLUDE_INTCTLR_LIB
        INIT_AFTER      INCLUDE_PLB_BUS
        PROTOTYPE       void vxbMipsMtIntCtlrRegister (void);
}

