# Makefile - makefile for Freescale P10XXRDB board
#
# Copyright (c) 2012, 2013 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01c,20may13,wyt  added support for P1023RDB
# 01b,15aug12,syt  added support for P1021RDB
# 01a,22may12,syt  derived from version 01a of fsl_p1020_rdb/Makefile
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Freescale P10XXRDB reference board
#
# INCLUDES
#     makeTarget
#

CPU           = PPC32
TOOL          = e500v2diab
CPU_VARIANT   = _ppc85XX_e500v2

TGT_DIR = $(WIND_BASE)/target

include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.

TARGET_DIR    = fsl_p10xx_rdb
VENDOR        = Freescale
BOARD         = P10XXRDB

RELEASE_CMD   += bootrom.bin bootrom_uncmp.bin

ROM_TEXT_ADRS = fff00100 # ROM entry address
ROM_WARM_ADRS = fff00104 # ROM warm entry address
ROM_LINK_ADRS = fff00000

ROM_SIZE      = 00100000 # number of bytes of ROM space

RAM_LOW_ADRS  = 00100000 # RAM text/data address
RAM_HIGH_ADRS = 01000000 # RAM text/data address

## The following microcode patch is required for the
## QE in the P1025.

fsl_qe_ucode_1021_10_A.obj:
	objcopyppc  -I binary -O elf32-powerpc-vxworks  \
                -N _binary_p1021qe_size             \
                -B powerpc:e500 fsl_qe_ucode_1021_10_A.bin fsl_qe_ucode_1021_10_A.obj
		
MACH_EXTRA     += fsl_qe_ucode_1021_10_A.obj

## Vitesse switch firmware

vsc2bin.obj:
	objcopyppc  -I binary -O elf32-powerpc-vxworks  \
                -N _binary_vsc2bin_size             \
                -B powerpc:e500 vsc2bin vsc2bin.obj

MACH_EXTRA  += vsc2bin.obj

## The following microcode patch is required for the
## FMAN in the P1023.

fsl_fman_ucode_P1023_160_13.obj:
	objcopyppc -I binary -O elf32-powerpc-vxworks \
		-N _binary_fsl_fman_ucode_P1023_160_13_bin_size \
		-B powerpc:e500 fsl_fman_ucode_P1023_160_13.bin \
		fsl_fman_ucode_P1023_160_13.obj

MACH_EXTRA      += fsl_fman_ucode_P1023_160_13.obj

USR_ENTRY       = usrInit 

## Only redefine make definitions above this point, or the expansion of
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
