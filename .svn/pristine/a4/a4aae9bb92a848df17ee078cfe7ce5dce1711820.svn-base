# Makefile - makefile for Freescale MPC8360 RDK board
#
# Copyright (c) 2007-2008 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01c,07jul08,y_w  Fixed ROM realted vxWorks images can't boot problem
# 01b,26dec07,z_l  Updated to vxWorks6.6
# 01a,15aug07,z_l  Created from mds8360/Makefile
#
# DESCRIPTION
# This file contains rules for building VxWorks for the Freescale MPC8360 RDK 
# Board with a PowerPC 8360E
#
# INCLUDES
#     makeTarget
#

CPU             = PPC32
CPU_VARIANT     = _ppc603_83xx
TOOL            = diab

TGT_DIR = $(WIND_BASE)/target
include $(TGT_DIR)/h/make/defs.bsp

# Only redefine make definitions below this point, or your definitions will
# be overwritten by the makefile stubs above.

TARGET_DIR      = fsl_mpc8360_rdk
VENDOR          = Freescale
BOARD           = MPC8360-RDK

USR_ENTRY       = usrInit

LIB_EXTRA       = romExtras.a

RELEASE_CMD    += bootrom.bin

# The constants ROM_TEXT_ADRS and ROM_SIZE are defined in config.h and Makefile.
# All definitions for these constants must be identical.

ROM_TEXT_ADRS   = ff800100 # vxWorks bootrom entry address
ROM_SIZE        = 00800000 # number of bytes of ROM space
ROM_WARM_ADRS   = ff800108 # ROM warm entry address 
ROM_LINK_ADRS   = ff800000

RAM_LOW_ADRS    = 00010000 # RAM text/data address
RAM_HIGH_ADRS   = 00e00000 # RAM text/data address

EXTRA_DEFINE   += -DVXWORKS -DMPC83XX

# Only redefine make definitions above this point, or the expansion of 
# makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp

EXTRA_OBJS = sysAUtils.o

romExtras.a: $(EXTRA_OBJS)
	$(AR) crus $@ $(EXTRA_OBJS)
