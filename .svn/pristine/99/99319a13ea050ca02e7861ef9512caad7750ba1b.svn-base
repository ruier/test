/* 01vxTestV2_usb.cdf - BSPVTS usb test suite test components */

/*
 * Copyright (c) 2008-2009 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,24feb09,d_c  Reorg in preparation for internal test module
                 componentization
01a,28aug08,mdo   Created components for USB Generic tests


DESCRIPTION

This file contains vxTestV2 test component definitions for test modules
in the BSPVTS 'usb' test suite ($WIND_BASE/vxtest/src/config/usb.prm).
The 'usb' test suite performes tests on generic USB features.

### NOTE ###
The test module components defined in this file are delivered with the
vxWorks product release. Do not add any test module components to this
file unless you intend to deliver the corresponding test modules as
part of the vxWorks product.

*/


Component             INCLUDE_TM_USBSHOW {
      NAME            usbShow test module
      SYNOPSIS        This component adds the tmUsbShow.c test module
      LAYER           4
      REQUIRES        INCLUDE_VXTESTV2 \
                      INCLUDE_USB \
                      INCLUDE_USB_INIT \
                      INCLUDE_USB_MS_BULKONLY \
                      INCLUDE_USB_MS_BULKONLY_INIT \
                      INCLUDE_UHCI  \
                      INCLUDE_UHCI_BUS
                      INCLUDE_OHCI  \
                      INCLUDE_OHCI_BUS \
                      INCLUDE_EHCI  \
                      INCLUDE_EHCI_BUS\
                      INCLUDE_VXBUS_SHOW \
                      INCLUDE_PCI_CFGSHOW

      SOURCE          $(WIND_BASE)/vxtest/src/tests/usb/usbshow
      MODULES         tmUsbShow.o
      PROTOTYPE       void tmUsbShowInit(void);
      INIT_RTN        tmUsbShowInit();
}

/*
 * Test Init Group
 */
InitGroup		usrVxTestV2UsbTestsInit {
    INIT_RTN		usrVxTestV2UsbTestsInit ();
    SYNOPSIS		BSPVTS VxTestV2 usb tests initialization sequence
    INIT_ORDER		INCLUDE_TM_USBSHOW
    _INIT_ORDER 	usrVxTestV2Init
}

/*
 * USB VTS Tests Folder
 */
Folder			FOLDER_VXTESTV2_USB {
	NAME	        VxTestV2 usb test components
	SYNOPSIS	Used to group usb test components
	CHILDREN	INCLUDE_TM_USBSHOW
	DEFAULTS	INCLUDE_TM_USBSHOW
       _CHILDREN	FOLDER_VXTESTV2_TESTS
}
