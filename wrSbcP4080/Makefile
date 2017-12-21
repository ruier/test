# Makefile - makefile for Wind River wrSbcP4080 board
#
# Copyright (c) 2010, 2012 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
# 
# modification history
# --------------------
# 01c,16may12,mpc  Updated Freescale FMAN microcode. (WIND00349107)
# 01b,25nov10,e_d  code clean
# 01a,21oct10,wap  created from fsl_p4080_ds Makefile version 01m
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Wind River wrSbcP4080 board
#
# INCLUDES
#     makeTarget
#

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

TARGET_DIR	= wrSbcP4080
VENDOR		  = Wind River
BOARD		    = wrSbcP4080

RELEASE		+= bootrom.bin 

## Until GCC supports e500mc
RELEASE_PRJ     = prj_diab

EXTRA_DEFINE    =

## The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
## in config.h and Makefile. All definitions for these constants must be
## identical.

ROM_TEXT_ADRS    = fff00100 # ROM entry address
ROM_SIZE         = 00100000 # number of bytes of ROM space
ROM_WARM_ADRS    = fff00108 # ROM warm entry address 
ROM_LINK_ADRS	   = fff00000
RAM_LOW_ADRS     = 00100000 # RAM text/data address
RAM_HIGH_ADRS    = 01000000 # RAM text/data address

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

MACH_EXTRA       = fsl_fman_ucode_p4080_92_2.obj fsl_fman_ucode_P3_P4_P5_101_8.obj

BOOT_EXTRA       = 
LIB_EXTRA        = 

VMA_START        = 0x0
USR_ENTRY        = usrInit

## Only redefine make definitions above this point, or the expansion of 
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
#include $(TGT_DIR)/h/make/rules.$(WIND_HOST_TYPE)

