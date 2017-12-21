# Makefile - makefile for Freescale MDS8569 BSP
#
# Copyright (c) 2009, 2011 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01b,07jul11,y_c  Support VxWorks native bootrom. (WIND00240156)
# 01a,10jul09,x_z  derived from version 01c of fsl_mds8568/Makefile.
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Freescale MDS8569 reference board.
#

CPU             = PPC32
TOOL            = e500v2diab
CPU_VARIANT     = _ppc85XX_e500v2

TGT_DIR         = $(WIND_BASE)/target

include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.

TARGET_DIR      = fsl_mds8569
VENDOR          = Freescale
BOARD           = MDS8569

RELEASE_CMD    += bootrom.bin

## The constants ROM_TEXT_ADRS & ROM_SIZE are defined in config.h and Makefile.
## All definitions for these constants must be identical.

ROM_TEXT_ADRS   = FFF00100 # ROM entry address
ROM_WARM_ADRS   = FFF00104 # ROM warm entry address
ROM_LINK_ADRS   = FFF00000

RAM_LOW_ADRS    = 00100000 # RAM text/data address
RAM_HIGH_ADRS   = 01000000 # RAM text/data address
ROM_SIZE        = 00100000 # number of bytes of ROM space

DATA_SEG_ALIGN  = 00080000 # Align data section for TLB locking

VMA_START       = 0x0
USR_ENTRY       = usrInit

fsl_qe_ucode_8569_10_bin.obj:
	objcopyppc  -I binary -O elf32-powerpc-vxworks  \
                -N _binary_fsl_qe_ucode_8569_10_bin_size             \
                -B powerpc:e500 fsl_qe_ucode_8569_10_bin fsl_qe_ucode_8569_10_bin.obj

MACH_EXTRA  += fsl_qe_ucode_8569_10_bin.obj

## Only redefine make definitions above this point, or the expansion of
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
