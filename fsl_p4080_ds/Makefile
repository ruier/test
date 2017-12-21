# Makefile - makefile for Freescale P4080 board
#
# Copyright (c) 2007-2010, 2012, 2013 Wind River Systems, Inc.
# 
# modification history
# --------------------
# 01o,11jan13,ylu  Add rev 3 silicon fman microcode patch.
# 01n,16may12,mpc  Updated Freescale FMAN microcode. (WIND00349107)
# 01m,22jun10,pmr  revert to diab
# 01l,22feb10,wap  Update rev 1 silicon fman microcode patch, add support for
#                  rev 2 silicon fman microcode patch
# 01k,05jan10,pmr  add LOCAL_MEM_LOCAL_ADRS for local builds
# 01j,13nov09,pmr  BSP name change
# 01i,26aug09,pmr  take gnu out of make release
# 01h,13oct09,wap  Add microcode build target
# 01g,10sep08,pmr  BSP name change
# 01f,17jan08,dtr  Remove e500v2 option .
# 01e,19dec07,dtr  Update for AMP support.
# 01d,14nov07,dtr  change tool to e500v2diab.
# 01c,08nov07,dtr  Use ev2diab tool for hard-float.
# 01b,12jul07,vik  removed cmdLine.c dependency for sysLib.c build
# 01a,07feb07,dtr  Modify from cds8548/Makefile/01a
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Freescale P4080 Simics model.
#
# INCLUDES
#     makeTarget
#*/

CPU              = PPC32
#optional soft-float kernel 
TOOL             = diab

#Support for 36 bit physical addresses in the MMU can be added for 
#vxWorks 6.2 and later with the following CPU_VARIANT.
CPU_VARIANT      = _ppc85XX_e500mc

#Use diab for vxWorks 5.5.1
#TOOL = diab

TGT_DIR = $(WIND_BASE)/target

include $(TGT_DIR)/h/make/defs.bsp
#include $(TGT_DIR)/h/make/make.$(CPU)$(TOOL)
#include $(TGT_DIR)/h/make/defs.$(WIND_HOST_TYPE)

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.

TARGET_DIR	= fsl_p4080_ds
VENDOR		= Freescale
BOARD		= fsl_p4080_ds

RELEASE		+= bootrom_uncmp.hex bootrom_uncmp.bin 

EXTRA_DEFINE    =

## The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
## in config.h and Makefile. All definitions for these constants must be
## identical.

ROM_TEXT_ADRS    = fff00100 # ROM entry address
ROM_SIZE         = 00100000 # number of bytes of ROM space
ROM_WARM_ADRS    = fff00108 # ROM warm entry address 
ROM_LINK_ADRS	 = fff00000
RAM_LOW_ADRS     = 00100000 # RAM text/data address
RAM_HIGH_ADRS    = 01000000 # RAM text/data address

LOCAL_MEM_LOCAL_ADRS = 0x00000000

## The following microcode patch is required for the
## frame manager in the P4080.

fsl_fman_ucode_p4080_92_2.obj:
	objcopyppc -I binary -O elf32-powerpc-vxworks \
		-N _binary_fsl_fman_ucode_p4080_92_2_bin_size \
		-B powerpc:603 fsl_fman_ucode_p4080_92_2.bin \
		fsl_fman_ucode_p4080_92_2.obj

fsl_fman_ucode_P3_P4_P5_101_8.obj:
	objcopyppc -I binary -O elf32-powerpc-vxworks \
		-N _binary_fsl_fman_ucode_P3_P4_P5_101_8_bin_size \
		-B powerpc:603 fsl_fman_ucode_P3_P4_P5_101_8.bin \
		fsl_fman_ucode_P3_P4_P5_101_8.obj

fsl_fman_ucode_P4080_106_2_4.obj:
	objcopyppc -I binary -O elf32-powerpc-vxworks \
                -N _binary_fsl_fman_ucode_P4080_106_2_4_bin_size \
                -B powerpc:603 fsl_fman_ucode_P4080_106_2_4.bin \
                fsl_fman_ucode_P4080_106_2_4.obj

MACH_EXTRA       = fsl_fman_ucode_P4080_106_2_4.obj fsl_fman_ucode_p4080_92_2.obj \
                   fsl_fman_ucode_P3_P4_P5_101_8.obj

BOOT_EXTRA       = 
LIB_EXTRA        = 

VMA_START        = 0x0
USR_ENTRY        = usrInit
## Only redefine make definitions above this point, or the expansion of 
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
#include $(TGT_DIR)/h/make/rules.$(WIND_HOST_TYPE)

