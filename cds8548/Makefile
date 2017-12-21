# Makefile - makefile for Freescale cds8548 BSP
#
# Copyright (c) 2005-2008 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01f,29dec08,z_l  Redefine DATA_SEG_ALIGN. (WIND00128435)
# 01e,07dec07,dtr  Update for E500v2 hard float support.
# 01d,29may07,vik  removed cmdLine.c build 
# 01c,29apr07,cjj  fix missing mod history when IP_PORT_VXWORKS 
#                  stuff was added.
# 01c,02mar07,rec  WIND00089420 - remove ipnet makefile workaround
# 01b,01dec06,d_c  Add dependency on sysLib.c to create cmdLine.c
# 01a,13jun05,dtr  Modify from cds85xx/Makefile/01b
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# FreeScale MPC8548 CDS reference board
#
# INCLUDES
#     makeTarget
#*/

CPU              = PPC32
CPU_VARIANT      = _ppc85XX_e500v2
#TOOL             = sfdiab
TOOL             = e500v2diab
#Support for 36 bit physical addresses in the MMU can be removed by 
#using the standard e500 variant.
#CPU_VARIANT     = _ppc85XX

#Use diab for vxWorks 5.5.1
#TOOL = diab

TGT_DIR = $(WIND_BASE)/target

include $(TGT_DIR)/h/make/defs.bsp
#include $(TGT_DIR)/h/make/make.$(CPU)$(TOOL)
#include $(TGT_DIR)/h/make/defs.$(WIND_HOST_TYPE)

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.


TARGET_DIR   = cds8548
VENDOR       = Freescale
BOARD        = MPC8548 CDS

RELEASE      += bootrom_uncmp.hex bootrom_uncmp.bin 

EXTRA_DEFINE += -DDATA_SEG_ALIGN=0x2000

## The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
## in config.h and Makefile. All definitions for these constants must be
## identical.

ROM_TEXT_ADRS    = fff00100 # ROM entry address
ROM_SIZE         = 00100000 # number of bytes of ROM space
ROM_WARM_ADRS    = fff00108 # ROM warm entry address 
ROM_LINK_ADRS	 = fff00000

RAM_LOW_ADRS     = 00100000 # RAM text/data address
RAM_HIGH_ADRS    = 01000000 # RAM text/data address

MACH_EXTRA       = 

BOOT_EXTRA       = 
LIB_EXTRA        = 

VMA_START        = 0x0
USR_ENTRY        = usrInit

## Only redefine make definitions above this point, or the expansion of 
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
#include $(TGT_DIR)/h/make/rules.$(WIND_HOST_TYPE)

