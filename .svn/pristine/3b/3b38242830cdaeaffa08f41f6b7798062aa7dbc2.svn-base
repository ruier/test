# Makefile - makefile for MVME3100 BSP
#
# Copyright (c) 2002-2009 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# Copyright 2002-2005 Motorola, Inc. All Rights Reseerved
#
# modification history
# --------------------
# 01w,19aug09,e_d  Remove ataDrv.o from the MACH_EXTRA. (WIND00171937)
# 01v,15oct08,y_w  Remove bootrom_uncmp.bin from the release list
# 01u,04sep08,pgh  Remove + from MACH_EXTRA and BOOT_EXTRA
# 01t,22aug08,l_g  add mpc8540End.obj build rule
# 01s,02aug07,agf  making RELEASE macro more standard
# 01r,31jul07,agf  rename BSP from mpc8540 to mv3100, remove GPP/PNE
#                  distinction from mpc8540End driver object code
# 01q,23may07,y_w  Modified MACH_EXTRA line for 6.5
# 01p,11dec06,sam  (WRS) Modified MACH_EXTRA line for 6.4.
# 01o,11dec06,jch  (WRS) Updated for VxWorks 6.x compatibility.
# 01n,09sep05,cak  Modified release and documentation macros.
# 01m,18jul05,cak  Modifications for firmware booting.
# 01l,06jul05,cak  Added sysMemoryTest.o to MACH_EXTRA and BOOT_EXTRA so
#                  that sysMemoryTest() can be called from romInit.s.
# 01k,09may05,efb  Change mpc8540TsecEnd.obj to mpc8540End.obj on MACH_EXTRA
# 01j,10mar05,efb  Added mpc8540TsecEnd.obj the MACH_EXTRA
# 01i,25jan05,cak  Modified RAM_LOW_ADRS, necessary for shared memory
#                  support.
# 01h,20jan05,cak  Add commentary regarding SPR 94153, compiling with GNU
#                  and replace motTsecEnd.o with motTsecEnd.obj on
#                  MACH_EXTRA.
# 01g,18nov04,cak  Add sysMpc8540Smc.o to MACH_EXTRA and BOOT_EXTRA.
# 01f,09nov04,scb  Add sysMotI2c.o & sysMpc8540I2c.o to MACH_EXTRA, BOOT_EXTRA.
# 01e,22sep04,scb  Move flash banks to 0xf0000000, add "sysAUtil.o".
# 01d,23aug04,scb  Starting point for "Big Easy".
# 01c,21oct03,dtr  Adding sysMotTsecEnd.o to MACH_EXTRA.
# 01b,20aug03,dtr  Adding TSEC driver to MACH_EXTRA.
# 01a,03oct02,dtr  Created.
#
# DESCRIPTION
# This file contains rules for building VxWorks for Motorola MPC8540
# based products.
#
# INCLUDES
#     makeTarget
#*/

CPU              = PPC32
CPU_VARIANT      = _ppc85XX
TOOL             = sfdiab

TGT_DIR = $(WIND_BASE)/target

include $(TGT_DIR)/h/make/defs.bsp

# DOC_FILES is defined in h/make/defs.bsp.  It is being redefined
# here so that the appropriate html documentation is created when
# a make man command is executed.

DOC_FILES       = sysLib tyCoDrv mkboot pciCfgShow \
                  sysMotVpdShow sysMpc8540Show sysStatusRegsShow \
                  sysTempeShow sysErrorHandler sysErrorHandlerShow

# EXTRA_DOC_FLAGS is included so that all routines will be included
# in the html documentation (make man), regardless of whether certain
# macros are defined or not.

EXTRA_DOC_FLAGS = -DINCLUDE_RTC -DINCLUDE_BOOT_IMAGE_PROGRAM \
                  -DINCLUDE_WDOG_TIMER -DINCLUDE_DS1621 \
                  -DINCLUDE_ERROR_HANDLING -DINCLUDE_QUAD_UART \
                  -DINCLUDE_MPC8540_DMA -DINCLUDE_PERFMON \
                  -DINCLUDE_WDOG_TIMER -DINCLUDE_MPC8540_TIMERS \
                  -DINCLUDE_TICK_TIMERS

# Only redefine make definitions below this point, or your definitions will
# be overwritten by the makefile stubs above.

TARGET_DIR      = mv3100
VENDOR          = Motorola
BOARD           = MVME3100 

EXTRA_DEFINE    =

# The constants ROM_TEXT_ADRS, ROM_SIZE are defined in config.h and Makefile.
# All definitions for these constants must be identical.

ROM_TEXT_ADRS   = fff00100 # ROM entry address
ROM_WARM_ADRS   = fff00108 # ROM warm entry address
ROM_LINK_ADRS   = fff00000

ROM_SIZE        = 00100000 # number of bytes of ROM space
RAM_LOW_ADRS    = 00100000 # RAM text/data address
RAM_HIGH_ADRS   = 00d00000 # RAM text/data address

# mpc8540End.obj
# building MPC8540 END driver objects

MPC8540END_DIR = $(TGT_DIR)/config/mv3100/mpc8540End

mpc8540End.obj:
	$(MAKE) -C $(MPC8540END_DIR) CPU=PPC85XX TOOL=$(TOOL) ../mpc8540End.obj

# BOOT_EXTRA and MACH_EXTRA
#
# romInit.s should not call out to subroutines declared in other modules.
# If an outside module is necessary, it can be linked into the system by
# adding the module to BOOT_EXTRA.  If the same module is referenced by
# other BSP code, then the module must be added to MACH_EXTRA as well.
# This is done to prevent the module from being compressed, since it is
# needed before the uncompression has taken place.
#
# The BSP Developer's Kit defines BOOT_EXTRA as "Additional modules to be
# linked with compressed ROM images.  These modules are not linked with
# uncompressed or ROM-resident images, just compressed images.  Adding a
# module to MACH_EXTRA makes it available to uncompressed or ROM-resident
# images.  MACH_EXTRA is used to "add an object module to VxWorks...This
# macro causes the linker to link it into the output object."  It is used
# to include object modules that are not "included" via sysLib.c.
#

MACH_EXTRA      = mpc8540End.obj sysMpc8540End.o sysMpc8540Smc.o \
                  sysMpc8540I2c.o sysMotI2c.o sysAUtil.o sysMemoryTest.o 

BOOT_EXTRA      = sysMpc8540Smc.o sysMpc8540I2c.o sysMotI2c.o sysAUtil.o \
                  sysMemoryTest.o

LIB_EXTRA       =

VMA_START       = 0x0
USR_ENTRY       = usrInit

RELEASE        += bootrom.bin

# Only redefine make definitions above this point, or the expansion of
# makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
