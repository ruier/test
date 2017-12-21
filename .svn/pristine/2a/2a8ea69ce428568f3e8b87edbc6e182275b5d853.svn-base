# Makefile - makefile for Freescale cds85xx BSP
#
# Copyright 1984-2004 Wind River Systems, Inc. */
# 
# modification history
# --------------------
# 01f,10jul07,p_g  removed cmdLine.c dependency for sysLib.c build
# 01e,02mar07,rec  WIND00089420 - remove ipnet makefile workaround
# 01d,15sep06,jmt  Modify for IPNet
# 01c,13may05,slk  Fix TOOL macro
# 01b,22mar05,dtr  Remove TSEC. SPR 107045.
# 01a,13aug04,jln  Modify from ads8560/Makefile/01c
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# FreeScale MPC8555/8541 CDS reference boards
#
# INCLUDES
#     makeTarget
#*/

CPU              = PPC85XX
TOOL             = sfdiab

TGT_DIR = $(WIND_BASE)/target

include $(TGT_DIR)/h/make/defs.bsp
#include $(TGT_DIR)/h/make/make.$(CPU)$(TOOL)
#include $(TGT_DIR)/h/make/defs.$(WIND_HOST_TYPE)

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.


TARGET_DIR	= cds85xx
VENDOR		= Freescale
BOARD		= MPC85XX CDS

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
RAM_HIGH_ADRS    = 00e00000 # RAM text/data address

MACH_EXTRA       =  

BOOT_EXTRA       = 
LIB_EXTRA        = 

VMA_START        = 0x0
USR_ENTRY        = usrInit
## Only redefine make definitions above this point, or the expansion of 
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp
#include $(TGT_DIR)/h/make/rules.$(WIND_HOST_TYPE)

