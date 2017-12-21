# Makefile - makefile for Freescale HPC-NET 8641(D) board
#
# Copyright (c) 2006-2008, 2012 Wind River Systems, Inc.
# 
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01h,12jul12,l_z  Clean up
# 01g,11aug08,dtr  This BSP only supports legacy AMP.
# 01f,16jul07,ami  removed cmdLine.c dependency
# 01e,05jul07,mmi  switch to new build mechanism for PPC32 family
# 01d,16mar07,tkf  Changes for IPNET integration.
# 01c,01dec06,d_c  Add dependency on sysLib.c to create cmdLine.c
# 01d,26dec06,dtr  Add extra boot image support.
# 01c,15nov06,tor  update EXTRA_INCLUDE for VxBus interrupt controller
# 01b,07aug06,dtr  Change BSP name.
# 01a,10feb06,dtr  Created from sp7447/Makefile/01f.
#
# DESCRIPTION
# This file contains rules for building VxWorks for the template.
#
# INCLUDES
#     makeTarget
#*/

CPU              = PPC32
TOOL             = diab
CPU_VARIANT      = _ppc604_745x
TGT_DIR = $(WIND_BASE)/target

EXTRA_INCLUDE=-I$(TGT_DIR)/src/drv -I$(TGT_DIR)/src/hwif -I$(TGT_DIR)/src/hwif/h

include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.  All the board specific
## defines are listed below, but it is preferable to use the BSP that
## matches the hardware.  Runtime checks for some board differences are not
## possible, so the kernel should be configured to the specific board.

TARGET_DIR      = hpcNet8641
VENDOR          = Freescale



##########################################################################
BOARD           = HPC-NET Freescale 8641D

##########################################################################
#
#
# The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
# in config.h and Makefile.
# All definitions for these constants must be identical.
# Specify values in hexadecimal, without a leading 0x.
#


ROM_TEXT_ADRS   = fff00100 # ROM cold entry address
ROM_WARM_ADRS   = fff0010C # ROM warm entry address
ROM_SIZE        = 00100000 # number of bytes of ROM space (512K)
RAM_LOW_ADRS    = 00100000 # RAM text/data address for vxWorks
RAM_HIGH_ADRS   = 00e00000 # RAM text/data address for bootrom

# ROM_LINK_ADRS defaults to ROM_TEXT_ADRS, but needs to be different
# here due to complications related to the .space 0x100 at the start
# of romInit.s.
ROM_LINK_ADRS   = fff00000 

USR_ENTRY       = usrInit

DOC_FILES	=  sysCacheLib sysLib

RELEASE        += bootrom_uncmp.hex

## Only redefine make definitions above this point, or the expansion 
## of the makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
#include $(TGT_DIR)/h/make/rules.$(WIND_HOST_TYPE)

