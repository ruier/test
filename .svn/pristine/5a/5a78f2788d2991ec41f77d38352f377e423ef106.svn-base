# Makefile - makefile for Freescale P1021MDS board
#
# Copyright (c) 2010 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01a,22oct10,y_y  derived from fsl_p2020_ds/Makefile
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Freescale P1021MDS reference board
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

TARGET_DIR      = fsl_p1021_mds
VENDOR          = Freescale
BOARD           = P1021MDS
RELEASE_CMD    += bootrom.bin

## The constants ROM_TEXT_ADRS, ROM_SIZE, are defined
## in config.h and Makefile. All definitions for these constants must be
## identical.more comments can reference the bootInit_nand.c

ROM_TEXT_ADRS   = 1fc00100 # ROM entry address
ROM_SIZE        = 02000000 # number of bytes of ROM space 
ROM_WARM_ADRS   = 1fc00108 # ROM warm entry address
ROM_LINK_ADRS   = 1fc00000

RAM_LOW_ADRS    = 00200000 # RAM text/data address
RAM_HIGH_ADRS   = 01c00000 # RAM text/data address

## The following microcode patch is required for the
## QE in the P1021.

fsl_qe_ucode_1021_10_A.obj:
	objcopyppc  -I binary -O elf32-powerpc-vxworks  \
                -N _binary_p1021qe_size             \
                -B powerpc:e500 fsl_qe_ucode_1021_10_A.bin fsl_qe_ucode_1021_10_A.obj
		
MACH_EXTRA     += fsl_qe_ucode_1021_10_A.obj
USR_ENTRY       = usrInit 

## Only redefine make definitions above this point, or the expansion of
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
