# Makefile - makefile for bsp/config/ads834x
#
# Copyright (c) 2005-2009 Wind River Systems, Inc.
#
# The right to copy, distribute, modify or otherwise make use
# of this software may be licensed only pursuant to the terms
# of an applicable Wind River license agreement.
#
# modification history
# --------------------
# 01j,09sep09,s_z  Remove un-used files when integrate the USB TCD support 
#                  by INCLUDE_FSL_TCD(WIND00180575)
# 01i,16dec08,z_l  Support USB host and target. (WIND00118187)
# 01h,24jul08,z_l  VxWorks 6.7 update, support MDS8349EA.
# 01g,09jul07,p_g  removed cmdLine.c dependency for sysLib.c build
# 01f,02mar07,rec  WIND00089420 - remove ipnet makefile workaround
# 01e,29jan07,rec  vxWorks 6.5 updates
# 01d,01dec06,d_c  Add dependency on sysLib.c to create cmdLine.c
# 01c,03aug05,dtr  Remove romExtras.a and put cfiscs.c as include in sysTffs.c
# 01b,25mar05,dtr  Use CPU_VARIANT for 83xx.
# 01a,19jan05,dtr  created from ads827x Makefile
#
# DESCRIPTION
# This file contains rules for building VxWorks for the
# Freescale Voyager Application Development Support Board with a PowerPC 834X
#
# INCLUDES
#     makeTarget
#*/

CPU         = PPC32
CPU_VARIANT = _ppc603_83xx
TOOL        = diab

TGT_DIR = $(WIND_BASE)/target
include $(TGT_DIR)/h/make/defs.bsp

## Only redefine make definitions below this point, or your definitions will
## be overwritten by the makefile stubs above.

TARGET_DIR  = ads834x
VENDOR      = Freescale
BOARD       = ads834x

USR_ENTRY   = usrInit

LIB_EXTRA   = 

RELEASE_CMD += bootrom.bin

## The constants ROM_TEXT_ADRS, ROM_SIZE, and RAM_HIGH_ADRS are defined
## in config.h and Makefile. All definitions for these constants must be
## identical.

## ROM address definitions for MDS8349EA
ROM_TEXT_ADRS   = fe000100 # ROM entry address
ROM_SIZE        = 00100000 # number of bytes of ROM space
ROM_WARM_ADRS   = fe000108 # ROM warm entry address 
ROM_LINK_ADRS   = fe000000

## ROM address definitions for ADS834X
## ROM_TEXT_ADRS    = ff800100 # ROM entry address
## ROM_SIZE         = 00100000 # number of bytes of ROM space
## ROM_WARM_ADRS    = ff800108 # ROM warm entry address 
## ROM_LINK_ADRS    = ff800000

RAM_LOW_ADRS     = 00010000 # RAM text/data address
RAM_HIGH_ADRS    = 00c00000 # RAM text/data address

# uncomment the following line to enable debugging
#ADDED_CFLAGS +=  -g -O
EXTRA_DEFINE   = -DVXWORKS -DMPC83XX

MACH_EXTRA      = sysAUtils.o

EXTRA_MODULES   = 

## Only redefine make definitions above this point, or the expansion of 
## makefile target dependencies may be incorrect.

include $(TGT_DIR)/h/make/rules.bsp

