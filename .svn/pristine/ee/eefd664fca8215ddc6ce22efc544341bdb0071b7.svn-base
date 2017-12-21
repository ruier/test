# Makefile - makefile for bestComm DMA library
#
# Copyright (c) 2003-2005 Wind River Systems, Inc. 
# 
# The right to copy, distribute, modify, or otherwise make use 
# of this software may be licensed only pursuant to the terms 
# of an applicable Wind River license agreement. 
#
# modification history
# --------------------
# 01f,07mar05,j_b  fix build issues
# 01e,16apr04,bjn  Use __inline__ for build (works with -ansi)
# 01d,15mar04,bjn  Support for Bestcomm 2.0 release
# 01c,03mar04,bjn  Removed tasksetup.c.
#                  Build dma_image.reloc.c instead of m5200SdmaFirmware.s &
#                  dma_image.reloc.s because dma_image.reloc.s wouldn't
#                  build easily with diab das)
# 01b,27nov03,pdr  added all tasks.
# 01a,18nov03,pdr  added comments.
#
# DESCRIPTION
# This file contains the makefile rules for building Bestcomm objects
#
#*/

TGT_DIR 	= $(WIND_BASE)/target

BESTCOMM_VARIANT=image_rtos1

#ADDED_CFLAGS	= -g # -O0

BUILD_ALL_OBJS	= 1

#EXTRA_DEFINE   += -I. -Icapi -Icode_dma/$(BESTCOMM_VARIANT) -DDEBUG_BESTCOMM_API=99
EXTRA_DEFINE    += -I. -I.. -Icapi -Icode_dma/$(BESTCOMM_VARIANT) -DDEBUG_BESTCOMM_API=0
EXTRA_DEFINE    += -Dinline=__inline__

# Library and object locations
LIBDIRNAME = BestComm
LIBDIR = ./obj$(LIBDIRNAME)

LOCAL_CLEAN  = libBestComm.a $(TGT_DIR)/config/lite5200b/bestComm/objBestComm/*

all: subdirs mk_libdir libBestComm.a

mk_libdir:
	-$(MKDIR) ./obj$(LIBDIRNAME)

OBJS =	./obj$(LIBDIRNAME)/bestcomm_api.o \
		./obj$(LIBDIRNAME)/load_task.o \
		./obj$(LIBDIRNAME)/tasksetup_bdtable.o \
		./obj$(LIBDIRNAME)/dma_image.o\
		./obj$(LIBDIRNAME)/dma_image.reloc.o \
		./obj$(LIBDIRNAME)/tasksetup_ata.o \
		./obj$(LIBDIRNAME)/tasksetup_crc16_dp_0.o \
		./obj$(LIBDIRNAME)/tasksetup_crc16_dp_1.o \
		./obj$(LIBDIRNAME)/tasksetup_fec_rx_bd.o \
		./obj$(LIBDIRNAME)/tasksetup_fec_tx_bd.o \
		./obj$(LIBDIRNAME)/tasksetup_gen_dp_0.o \
		./obj$(LIBDIRNAME)/tasksetup_gen_dp_1.o \
		./obj$(LIBDIRNAME)/tasksetup_gen_dp_2.o \
		./obj$(LIBDIRNAME)/tasksetup_gen_dp_3.o \
		./obj$(LIBDIRNAME)/tasksetup_gen_dp_bd_0.o \
		./obj$(LIBDIRNAME)/tasksetup_gen_dp_bd_1.o \
		./obj$(LIBDIRNAME)/tasksetup_gen_rx_bd.o \
		./obj$(LIBDIRNAME)/tasksetup_gen_tx_bd.o \
		./obj$(LIBDIRNAME)/tasksetup_lpc.o \
		./obj$(LIBDIRNAME)/tasksetup_pci_rx.o \
		./obj$(LIBDIRNAME)/tasksetup_pci_tx.o \
		./obj$(LIBDIRNAME)/m5200Sdma.o \
		./obj$(LIBDIRNAME)/m5200FecEnd.o


libBestComm.a: $(OBJS)
	$(AR) r $@ $^

include $(TGT_DIR)/h/make/rules.library

./obj$(LIBDIRNAME)/bestcomm_api.o : capi/bestcomm_api.c
	$(CC) $(CFLAGS) -o $@ -c $<

./obj$(LIBDIRNAME)/tasksetup_bdtable.o : capi/task_api/tasksetup_bdtable.c
	$(CC) $(CFLAGS) -o $@ -c $<

./obj$(LIBDIRNAME)/tasksetup_fec_tx_bd.o : code_dma/$(BESTCOMM_VARIANT)/task_capi/tasksetup_fec_tx_bd.c
	$(CC) $(CFLAGS) -Icapi/task_api -o $@ -c $<

./obj$(LIBDIRNAME)/tasksetup_fec_rx_bd.o : code_dma/$(BESTCOMM_VARIANT)/task_capi/tasksetup_fec_rx_bd.c
	$(CC) $(CFLAGS) -Icapi/task_api -o $@ -c $<

./obj$(LIBDIRNAME)/tasksetup_ata.o : code_dma/$(BESTCOMM_VARIANT)/task_capi/tasksetup_ata.c
	$(CC) $(CFLAGS) -Icapi/task_api -o $@ -c $<

./obj$(LIBDIRNAME)/tasksetup_crc16_dp_0.o : code_dma/$(BESTCOMM_VARIANT)/task_capi/tasksetup_crc16_dp_0.c
	$(CC) $(CFLAGS) -Icapi/task_api -o $@ -c $<

./obj$(LIBDIRNAME)/tasksetup_crc16_dp_1.o : code_dma/$(BESTCOMM_VARIANT)/task_capi/tasksetup_crc16_dp_1.c
	$(CC) $(CFLAGS) -Icapi/task_api -o $@ -c $<

./obj$(LIBDIRNAME)/tasksetup_gen_dp_0.o : code_dma/$(BESTCOMM_VARIANT)/task_capi/tasksetup_gen_dp_0.c
	$(CC) $(CFLAGS) -Icapi/task_api -o $@ -c $<

./obj$(LIBDIRNAME)/tasksetup_gen_dp_1.o : code_dma/$(BESTCOMM_VARIANT)/task_capi/tasksetup_gen_dp_1.c
	$(CC) $(CFLAGS) -Icapi/task_api -o $@ -c $<

./obj$(LIBDIRNAME)/tasksetup_gen_dp_2.o : code_dma/$(BESTCOMM_VARIANT)/task_capi/tasksetup_gen_dp_2.c
	$(CC) $(CFLAGS) -Icapi/task_api -o $@ -c $<

./obj$(LIBDIRNAME)/tasksetup_gen_dp_3.o : code_dma/$(BESTCOMM_VARIANT)/task_capi/tasksetup_gen_dp_3.c
	$(CC) $(CFLAGS) -Icapi/task_api -o $@ -c $<

./obj$(LIBDIRNAME)/tasksetup_gen_dp_bd_0.o : code_dma/$(BESTCOMM_VARIANT)/task_capi/tasksetup_gen_dp_bd_0.c
	$(CC) $(CFLAGS) -Icapi/task_api -o $@ -c $<

./obj$(LIBDIRNAME)/tasksetup_gen_dp_bd_1.o : code_dma/$(BESTCOMM_VARIANT)/task_capi/tasksetup_gen_dp_bd_1.c
	$(CC) $(CFLAGS) -Icapi/task_api -o $@ -c $<

./obj$(LIBDIRNAME)/tasksetup_gen_rx_bd.o : code_dma/$(BESTCOMM_VARIANT)/task_capi/tasksetup_gen_rx_bd.c
	$(CC) $(CFLAGS) -Icapi/task_api -o $@ -c $<

./obj$(LIBDIRNAME)/tasksetup_gen_tx_bd.o : code_dma/$(BESTCOMM_VARIANT)/task_capi/tasksetup_gen_tx_bd.c
	$(CC) $(CFLAGS) -Icapi/task_api -o $@ -c $<

./obj$(LIBDIRNAME)/tasksetup_lpc.o : code_dma/$(BESTCOMM_VARIANT)/task_capi/tasksetup_lpc.c
	$(CC) $(CFLAGS) -Icapi/task_api -o $@ -c $<

./obj$(LIBDIRNAME)/tasksetup_pci_rx.o : code_dma/$(BESTCOMM_VARIANT)/task_capi/tasksetup_pci_rx.c
	$(CC) $(CFLAGS) -Icapi/task_api -o $@ -c $<

./obj$(LIBDIRNAME)/tasksetup_pci_tx.o : code_dma/$(BESTCOMM_VARIANT)/task_capi/tasksetup_pci_tx.c
	$(CC) $(CFLAGS) -Icapi/task_api -o $@ -c $<

./obj$(LIBDIRNAME)/load_task.o : capi/load_task.c
	$(CC) $(CFLAGS) -o $@ -c $<

./obj$(LIBDIRNAME)/dma_image.o : code_dma/$(BESTCOMM_VARIANT)/dma_image.c
	$(CC) $(CFLAGS) -o $@ -c $<

./obj$(LIBDIRNAME)/dma_image.reloc.o : code_dma/$(BESTCOMM_VARIANT)/dma_image.reloc.c
	$(CC) $(CFLAGS) -o $@ -c $<

./obj$(LIBDIRNAME)/m5200Sdma.o: m5200Sdma.c
	$(CC) -c $(CFLAGS) -O0 -o $@ $<

./obj$(LIBDIRNAME)/m5200FecEnd.o: m5200FecEnd.c
	$(CC) -c $(CFLAGS) -o $@ $<
