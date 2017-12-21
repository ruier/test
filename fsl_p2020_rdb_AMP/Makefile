# Makefile - makefile for Freescale P2020RDB board AMP core 0
#
# Copyright (c) 2010 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01a,22feb10,syt  derived from fsl_p2020_rdb/Makefile/01a
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Freescale P2020RDB reference board
#
# INCLUDES
#     makeTarget
#

CPU              = PPC32
TOOL             = e500v2diab
#optional soft-float kernel
#TOOL            = sfdiab
CPU_VARIANT      = _ppc85XX_e500v2

TGT_DIR = $(WIND_BASE)/target

include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.

TARGET_DIR      = fsl_p2020_rdb_AMP
VENDOR          = Freescale
BOARD           = P2020RDB

EXTRA_DEFINE   += -D_WRS_VX_AMP

ROM_TEXT_ADRS   = fff00100 # ROM entry address
ROM_WARM_ADRS   = fff00108 # ROM warm entry address
ROM_LINK_ADRS   = fff00000
ROM_SIZE        = 00100000 # number of bytes of ROM space

RAM_LOW_ADRS    = 00100000 # RAM text/data address
RAM_HIGH_ADRS   = 01000000 # RAM text/data address

LOCAL_MEM_LOCAL_ADRS = 0x00000000

VMA_START       = 0x0

# Vitesse switch firmware

vsc2bin.obj:
	objcopyppc  -I binary -O elf32-powerpc-vxworks  \
                -N _binary_vsc2bin_size             \
                -B powerpc:e500 vsc2bin vsc2bin.obj

MACH_EXTRA  += vsc2bin.obj

## Only redefine make definitions above this point, or the expansion of
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
