/* sysMotFccEnd.c - system configuration module for motFccEnd driver */

/* Copyright 1984-2004 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
modification history
--------------------
01n,24sep04,mdo  Documentation fixes for apigen
01m,17aug04,j_b  fix error msg
01l,29jul04,rec  MAC address scheme ('M') (SPR #93020)
01k,04jun04,j_b  fix compiler warning
01j,29may03,jb3  remove debug print.
01i,22may03,jb3  debugging fcc3.
01h,21may03,jb3  debugging fcc3.
01g,20may03,jb3  debugging fccNUm based on unit
01f,20may03,jb3  assign fccNUm based on unit number.
01e,13may03,jb3  remove ATM code.
01d,14jan03,gjc  SPR#85164 and SPR#85098 :Added support for motFcc2End.
01c,07mar02,kab  SPR 70817: *EndLoad returns NULL on failure
01b,17sep01,g_h  change to work with the new m8206FccEnd driver rev 01g.
01a,05jul01,g_h  created from ads860/sysMotCpmEnd.c
*/

/*
DESCRIPTION
This is the WRS-supplied configuration module for the VxWorks
m8260FccEnd END driver.

It performs the dynamic parameterization of the motFccEnd driver.
This technique of 'just-in-time' parameterization allows driver
parameter values to be declared as any other defined constants rather
than as static strings.
*/

#include "vxWorks.h"
#include "config.h"
#include "vmLib.h"
#include "stdio.h"
#include "sysLib.h"
#include "logLib.h"
#include "stdlib.h"
#include "string.h"
#include "end.h"
#include "intLib.h"
#include "lstLib.h"
#include "drv/mem/m82xxDpramLib.h"
#include "miiLib.h"


/* defines */

#ifdef INCLUDE_MOTFCCEND

#define MOT_FCC_NUM             0x02    /* FCC being used */
#define MOT_FCC_TBD_NUM         0x40    /* transmit buffer descriptors (TBD)*/
#define MOT_FCC_RBD_NUM         0x20    /* receive buffer descriptors (RBD) */
#define MOT_FCC_PHY_ADDR1       0x00    /* FCC1 PHY address */
#define MOT_FCC_PHY_ADDR2       0x01    /* FCC2 PHY address */
#define MOT_FCC_PHY_ADDR3       0x02    /* FCC3 PHY address */
#define MOT_FCC_DEF_PHY_MODE    0x02    /* jb3 PHY default operating mode */
                                        /* MiiLib.h polled only timeout value too*/
#define MOT_FCC_BDS_SIZE        (2 * 1024)


/* imports */

IMPORT END_OBJ * motFcc2EndLoad (char *);
IMPORT UINT32    sysPCGet (void);

/* globals */

/* locals */

LOCAL STATUS sysMiiPhyInit ( PHY_INFO * pPhyInfo );
LOCAL STATUS sysMiiPhyDuplex ( PHY_INFO * pPhyInfo,int * pDuplex );
LOCAL STATUS sysMiiPhySpeed ( PHY_INFO * pPhyInfo,int * pSpeed );

/*
 * This table may be customized by the user to force a
 * particular order how different technology abilities may be
 * negotiated by the PHY. Entries in this table may be freely combined
 * and even OR'd together.
 */

LOCAL INT16 motFccAnOrderTbl [] = {
                                MII_TECH_100BASE_TX,    /* 100Base-T */
                                MII_TECH_100BASE_T4,    /* 10Base-T */
                                MII_TECH_10BASE_T,      /* 100Base-T4 */
                                MII_TECH_10BASE_FD,     /* 100Base-T FD*/
                                MII_TECH_100BASE_TX_FD, /* 10Base-T FD */
                                -1                      /* end of table */
                               };

/* forward declarations */

STATUS sysFccEnetAddrGet (int unit, UCHAR *address);
STATUS sysFccEnetEnable (UINT32 immrVal, UINT8 fccNum);
STATUS sysFccEnetDisable (UINT32 immrVal, UINT8 fccNum);
STATUS sysFccMiiBitWr (UINT32 immrVal, UINT8 fccNum, INT32 bitVal);
STATUS sysFccMiiBitRd (UINT32 immrVal, UINT8 fccNum, INT8 * bitVal);

#define SYS_MII_PHY_INT     1

LOCAL FUNCPTR sysMotFccFuncs[] =
    {
    (FUNCPTR) sysMiiPhyInit,      /* bsp MiiPhy init function */
    (FUNCPTR) NULL,               /* Interrupt End Driver function called by BSP */
    (FUNCPTR) sysFccMiiBitRd,     /* BSP Bit Read function    */
    (FUNCPTR) sysFccMiiBitWr,     /* BSP Bit Write function  */
    (FUNCPTR) sysMiiPhyDuplex,    /* duplex status call back */
    (FUNCPTR) sysMiiPhySpeed,     /* speed status call back  */
    (FUNCPTR) NULL,               /* heart beat error        */
    (FUNCPTR) NULL,               /* disconnect function */
    (FUNCPTR) m82xxDpramFree,     /* Dpram Free routine */
    (FUNCPTR) m82xxDpramFccMalloc,/* Dpram Malloc */
    (FUNCPTR) m82xxDpramFccFree,  /* Dpram FCC Free */
    NULL, NULL, NULL, NULL
    };

/***********************************************************************
*
* sysMotFccEndLoad - load an instance of the motFccEnd driver
*
* This routine loads the motFccEnd driver with proper parameters. It also
* reads the BCSR3 to find out which type of processor is being used, and
* sets up the load string accordingly.
*
* The END device load string formed by this routine is in the following
* format.
* <immrVal>:<fccNum>:<bdBase>:<bdSize>:<bufBase>:<bufSize>:<fifoTxBase>
* :<fifoRxBase>:<tbdNum>:<rbdNum>:<phyAddr>:<phyDefMode>:<pAnOrderTbl>:
* <userFlags>:<mBlkRatio>:<mBlkRatio>:<miiPhyParams>:<dpramParams>
*
* \is
* \i <immrVal>
* Internal memory address
* \i <fccNum>
* FCC number being used
* \i <bdBase>
* buffer descriptors base address
* \i <bdSize>
* buffer descriptors space size
* \i <bufBase>
* data buffers base address
* \i <bufSize>
* data buffers space size
* \i <fifoTxBase>
* tx buffer in internal memory
* \i <fifoRxBase>
* rx buffer in internal memory
* \i <tbdNum>
* number of TBDs or NONE
* \i <rbdNum>
* number of RBDs or NONE
* \i <phyAddr>
* address of a MII-compliant PHY device
* \i <phyDefMode>
* default operating mode of a MII-compliant PHY device
* \i <pAnOrderTbl>
* auto-negotiation order table for a MII-compliant PHY device or NONE
* \i <userFlags>
* User flags
* \i <sysMotFccFuncs>
* Pointer to a predefined array of FUNCPTRs
*   sysMiiPhyInit         bsp MiiPhy init function
*   NULL                  Interrupt End Driver function called by BSP
*   sysFccMiiBitR,        BSP Bit Read function
*   sysFccMiiBitWr        BSP Bit Write function
*   sysMiiPhyDuplex       duplex status call back
*   sysMiiPhySpeed        speed status call back
*   NULL                  heartbeat error
*   NULL                  disconnect function
*   m82xxDpramFree        Dpram Free routine (main pool)
*   m82xxDpramFccMalloc   Dpram Malloc from the FCC pool
*   m82xxDpramFccFree     Dpram Free from the FCC pool
*
* This routine only loads and initializes one instance of the device.
* If the user wishes to use more than one motFccEnd devices, this routine
* should be changed.
* \ie
* 
* RETURNS: pointer to END object or NULL.
*
* ERRNO
*
* SEE ALSO: motFccEndLoad ()
*/

END_OBJ * sysMotFccEndLoad
    (
    char * pParamStr,   /* ptr to initialization parameter string */
    void * unused       /* unused optional argument */
    )
    {
    /*
     * The motFccEnd driver END_LOAD_STRING should be:
     * <immrVal>:<fccNum>:<bdBase>:<bdSize>:<bufBase>:<bufSize>:<fifoTxBase>
     * :<fifoRxBase>:<tbdNum>:<rbdNum>:<phyAddr>:<phyDefMode>
     * :<pAnOrderTbl>:<userFlags>:<sysMotFccFuncs>
     * Note that unit string is prepended by the mux, so we
     * don't put it here.
     */

    char * pStr = NULL;
    char   paramStr [300];
    UINT32 regPC;
    UINT32 motFccBufsDescAdr;
    UINT32 motFccBufsDescSize;
    UINT32 motFccBufsAdr;
    UINT32 motFccBufsSize;
    UINT32 motFccFlags;
    UINT32 unit;			/* use the unit number for 1 to 1 mapping. */
    UINT32 motFccPhy = 0;


    static char motFccEndParamTemplate [] =
    "0x%x:0x%x:0x%x:0x%x:0x%x:0x%x:0x%x:0x%x:0x%x:0x%x:0x%x:0x%x:0x%x:0x%x:0x%x";

    END_OBJ * pEnd;


    if (strlen (pParamStr) == 0)
        {
        /*
        * muxDevLoad() calls us twice.  If the string is
        * zero length, then this is the first time through
        * this routine.
        */

        pEnd = (END_OBJ *) motFcc2EndLoad  (pParamStr);
        }
    else
        {
        regPC = sysPCGet ();

        if ((regPC & ROM_BASE_ADRS) == ROM_BASE_ADRS)
            {
        /* We are running from ROM */

        /*
        * ROM resident applications MUST use the option, while RAM resident
        * MAY use the option, which forces all
        * driver buffers (rx and tx) to reside in local bus space. The
        * drawback to having buffers on the local bus is that the driver
        * MUST copy all transmit packets to the local bus -- so zcopy
        * must be inhibited. The driver still passes rx data zcopy.
        */

        /* for ROM apps, specify local bus space for the buffers */
            motFccBufsAdr  = LOCAL_BUS_SDRAM_ADRS + (LOCAL_BUS_SDRAM_SIZE >> 1);
            motFccBufsSize = LOCAL_BUS_SDRAM_SIZE >> 1;

        /* for ROM apps, MOT_FCC_FLAGS *must* include the following. */
            motFccFlags = 0x0000D000;
        /*(MOT_FCC_USR_BUF_LBUS|MOT_FCC_USR_NO_ZCOPY|MOT_FCC_USR_DPRAM_ALOC)*/

        /*
        * NOTE! The FCC driver internally forces MOT_FCC_USR_NO_ZCOPY if
        * MOT_FCC_USR_BUF_LBUS is set in MOT_FCC_FLAGS, making the
        * inclusion of 'NO_ZCOPY not strictly required. The 'NO_ZCOPY
        * flag can be used independently of the bus on which buffers
        * reside -- for those rare apps that might need this option.
        */
            }
        else
            {
            /* We are running from RAM */

            /* for RAM apps, let the driver find space for the buffers */
            motFccBufsAdr  = -1UL;
            motFccBufsSize = -1UL;
            motFccFlags    = 0x00008000; /* MOT_FCC_USR_DPRAM_ALOC */
            }
        /*
        * NOTE! Regarding bus choices for BDs and buffers.
        * If you give a local bus address for BDs and don't include
        * MOT_FCC_USR_BD_LBUS in MOT_FCC_FLAGS, the CPM will hang.
        * If you give a local bus address for buffers and don't include
        * MOT_FCC_USR_BUF_LBUS in MOT_FCC_FLAGS, the CPM will hang.
        * If you give a 60x bus address for buffers and/or BDs and
        * mistakenly include either a 'LBUS flag, the CPM will hang.
        */

        /*
        * If you want the driver to decide where to put the BDs, set BOTH of the
        * following motFccBufsDescAdr & motFccBufsDescSize to -1UL.
        * If you decide to force the driver to use BDs on the local bus, set the
        * following motFccBufsDescAdr to a local bus address and size AND be sure to add
        * MOT_FCC_USR_BD_LBUS to the definition of MOT_FCC_FLAGS below.
        * The defaults use internal DPRAM for BDS. You may optionally specify
        * a 60x bus address for BDS.
        */

        motFccBufsDescSize = MOT_FCC_BDS_SIZE;

        if ((motFccBufsDescAdr = (UINT32)m82xxDpramAlignedMalloc (MOT_FCC_BDS_SIZE,8)) == 0)
            {
            motFccBufsDescAdr = -1UL;
            }

        /*
        * On the second pass through here, we actually create
        * the initialization parameter string on the fly.
        * Note that we will be handed our unit number on the
        * second pass and we need to preserve that information.
        * So we use the unit number handed from the input string.
        */

        pStr = strcpy (paramStr, pParamStr);

        /* Extract the unit number */

        unit = atoi (pStr);

        switch (unit)
            {
            case 0:
                motFccPhy = MOT_FCC_PHY_ADDR1;
                break;
            case 1:
                motFccPhy = MOT_FCC_PHY_ADDR2;
                break;
            case 2:
                motFccPhy = MOT_FCC_PHY_ADDR3;
                break;
            }

        /* Now, we get to the end of the string */

        pStr += strlen (paramStr);

        /* finish off the initialization parameter string */
        sprintf (pStr, motFccEndParamTemplate,
            (UINT) vxImmrGet (),
            unit+1,			/* FccNum is unit plus 1. (not 1 to 1) */
            motFccBufsDescAdr,
            motFccBufsDescSize,
            motFccBufsAdr,
            motFccBufsSize,
            -1UL,-1UL,
            MOT_FCC_TBD_NUM,
            MOT_FCC_RBD_NUM,
            motFccPhy,
            MOT_FCC_DEF_PHY_MODE,
            &motFccAnOrderTbl,
            motFccFlags,
            &sysMotFccFuncs);

        if ((pEnd = (END_OBJ *) motFcc2EndLoad  (paramStr)) == (END_OBJ *)NULL)
            {
            logMsg ("Error: motFcc2EndLoad failed to load driver\n", 0, 0, 0, 0, 0, 0);
            }
        }

    return pEnd;
    }

/***********************************************************************
*
* sysMiiPhyDuplex - check if link is Full Duplex
*
* This routine checks if the link is Full Duplex or not
*
* RETURNS: 1 if Full Duplex or 0 if not.
*
* ERRNO
*/

LOCAL STATUS sysMiiPhyDuplex
    (
    PHY_INFO * pPhyInfo,
    int      * pDuplex
    )
    {
    UINT16 miiStat;
    int    retVal;

    MII_READ(pPhyInfo->phyAddr,28,&miiStat,retVal);

    if (retVal == OK)
        {
        *pDuplex = (miiStat & 0x001) ? 1 : 0;
        }
    return retVal;
    }

/***********************************************************************
*
* sysMiiPhySpeed - check Speed
*
* This routine checks the link Speed
*
* RETURNS: 2 if 1000, 1 if 100 or 0 if 10 BPS.
*
* ERRNO
*/

LOCAL STATUS sysMiiPhySpeed
    (
    PHY_INFO * pPhyInfo,
    int      * pSpeed
    )
    {
    UINT16 miiStat;
    int    retVal;

    MII_READ(pPhyInfo->phyAddr,28,&miiStat,retVal);

    if (retVal == OK)
        {
        *pSpeed = (miiStat & 0x0002) ? 1 : 0;
        }
    return retVal;
    }

/***********************************************************************
*
* sysMiiPhyInit - initialize and configure the PHY devices
*
* This routine scans, initializes and configures the PHY device.
*
* RETURNS: OK, or ERROR.
*
* ERRNO
*/

LOCAL STATUS sysMiiPhyInit
    (
    PHY_INFO * pPhyInfo
    )
    {
    int    retVal;

    retVal = miiPhyInit (pPhyInfo);

    return retVal;
    }

/***********************************************************************
*
* sysFccEnetEnable - enable the MII interface to the FCC controller
*
* This routine is expected to perform any target specific functions required
* to enable the Ethernet device and to connect the MII interface to the FCC.
*
* RETURNS: OK, or ERROR if the FCC controller cannot be enabled.
*
* ERRNO
*/

STATUS sysFccEnetEnable
    (
    UINT32  immrVal, /* base address of the on-chip RAM */
    UINT8   fccNum   /* FCC being used */
    )
    {
    int             intLevel;

    intLevel = intLock ();
    switch (fccNum)
        {
        case 1:
            /* This port is only available on the non ATM boards.*/
            /* set Port A and C to use MII signals */

            /* turn off the first FEC transceiver (set bit 3 control status register U45) */
            /* *((volatile UINT8 *)(0x21000000)) = 0x80;       turn off the HotSwap LED for a tst */

            *M8260_IOP_PCPAR(immrVal) |=  (PC23 | PC22);  /* RXCK  CLK9 & TXCK CLK10 */
            *M8260_IOP_PCSO(immrVal)  &= ~(PC23 | PC22);  /* select as - clk9 & clk10 */
            *M8260_IOP_PCDIR(immrVal) &= ~(PC23 | PC22);  /* they are inputs */
            *M8260_IOP_PCPAR(immrVal) &= ~(PC10 | PC9);   /* MCI0 & MDC are general io*/


            *M8260_IOP_PAPAR(immrVal) |=  (PA31 | PA30 | PA29 | PA28 | PA27 | PA26 | PA21 |
                                           PA20 | PA19 | PA18 | PA17 | PA16 | PA15 | PA14);
            *M8260_IOP_PADIR(immrVal) |=  (PA29 | PA28 | PA21 | PA20 | PA19 | PA18);
            *M8260_IOP_PADIR(immrVal) &= ~(PA31 | PA30 | PA27 | PA26 | PA17 | PA16 | PA15 | PA14);
            *M8260_IOP_PASO(immrVal)  &= ~(PA21 | PA20 | PA19 | PA18 | PA17 | PA16 | PA15 | PA14);
            *M8260_IOP_PASO(immrVal)  |=  (PA31 | PA30 | PA29 | PA28 | PA27 | PA26);

            /* connect FCC1 clocks */

            *M8260_CMXFCR (immrVal)  |=  (0x25000000);  /* RXCK-CLK9  TXCK-CLK10 */
            *M8260_CMXFCR (immrVal)  &= ~(0x40000000);

            /* turn on the first FEC transceiver (clear bit 3 control status register U45) */
            break;
        case 2:
            /* set Port B and C to use MII signals */

            /* turn off the second FEC transceiver (set bit 4 control status register U45) */

            *M8260_IOP_PCPAR(immrVal) |=  (PC19 | PC18);    /* Rx CLK13 and Tx CLK14 */
            *M8260_IOP_PCSO(immrVal)  &= ~(PC19 | PC18);
            *M8260_IOP_PCDIR(immrVal) &= ~(PC19 | PC18);
            *M8260_IOP_PCPAR(immrVal) &= ~(PC10 | PC9);

            *M8260_IOP_PBPAR(immrVal) |=  (PB31 | PB30 | PB29 | PB28 | PB27 | PB26 | PB25 |
                                           PB24 | PB23 | PB22 | PB21 | PB20 | PB19 | PB18);
            *M8260_IOP_PBDIR(immrVal) |=  (PB31 | PB29 | PB25 | PB24 | PB23 | PB22);
            *M8260_IOP_PBDIR(immrVal) &= ~(PB30 | PB28 | PB27 | PB26 | PB21 | PB20 | PB19 | PB18);
            *M8260_IOP_PBSO(immrVal)  &= ~(PB31 | PB30 | PB28 | PB27 | PB26 | PB25 | PB24 |
                                           PB23 | PB22 | PB21 | PB20 | PB19 | PB18);
            *M8260_IOP_PBSO(immrVal)  |=  (PB29);

            /* connect FCC2 clocks */

            *M8260_CMXFCR (immrVal)  |=  (0x00250000);
            *M8260_CMXFCR (immrVal)  &= ~(0x004a0000);

            /* turn on the second FEC transceiver (clear bit 4 control status register U45) */
            break;
        case 3:
            /* set Port B and C to use MII signals */

            /* turn off the third FEC transceiver (set bit 5 control status register U45) */

            *M8260_IOP_PCPAR(immrVal) |=  (PC17 | PC16);    /* Tx CLK15 and Rx CLK16 */
            *M8260_IOP_PCSO(immrVal)  &= ~(PC17 | PC16);
            *M8260_IOP_PCDIR(immrVal) &= ~(PC17 | PC16);
            *M8260_IOP_PCPAR(immrVal) &= ~(PC10 | PC9);

            *M8260_IOP_PBPAR(immrVal) |=  (PB13 | PB12 | PB15 | PB14 | PB17 | PB16 | PB4 |
                                           PB5 | PB6 | PB7 | PB8 | PB9 | PB10 | PB11);
            *M8260_IOP_PBDIR(immrVal) |=  (PB15 | PB14 | PB4 | PB5 | PB6 | PB7);
            *M8260_IOP_PBDIR(immrVal) &= ~(PB13 | PB12 | PB17 | PB16 | PB8 | PB9 | PB10 | PB11);
            *M8260_IOP_PBSO(immrVal)  &= ~(PB13 | PB12 | PB15 | PB14 | PB17 | PB16 | PB4 |
                                           PB5 | PB6 | PB7 | PB8 | PB9 | PB10 | PB11);

            /* connect FCC3 clocks */

            *M8260_CMXFCR (immrVal)  |=  (0x00003700);  /* RXCK=CLK15 TXCK=CLK16 */
            *M8260_CMXFCR (immrVal)  &= ~(0x00004800);

            /* turn on the third FEC transceiver (clear bit 5 control status register U45) */
            break;
        default:
            break;
        }
    intUnlock (intLevel);

    taskDelay (sysClkRateGet() >> 2);

    return(OK);
    }

/***********************************************************************
*
* sysFccEnetDisable - disable MII interface to the FCC controller
*
* This routine is expected to perform any target specific functions required
* to disable the Ethernet device and the MII interface to the FCC
* controller.  This involves restoring the default values for all the Port
* B and C signals.
*
* RETURNS: OK, always.
*
* ERRNO
*/

STATUS sysFccEnetDisable
    (
    UINT32  immrVal,    /* base address of the on-chip RAM */
    UINT8   fccNum  /* FCC being used */
    )
    {
    int             intLevel;

    intLevel = intLock ();
 
    /* 
     * configure all Port B and C pins previously used as general 
     * purpose input pins disable the FEC transceiver 
     */

    switch (fccNum)
        {
        case 1:
            /* turn off the second FEC transceiver (set bit 4)*/

            *M8260_IOP_PCPAR(immrVal) &= ~(PC23 | PC22);    /* RXCK  CLK9 & TXCK CLK10 */


            *M8260_IOP_PAPAR(immrVal) &=  ~(PA31 | PA30 | PA29 | PA28 | PA27 | PA26 | PA21 |
                                            PA20 | PA19 | PA18 | PA17 | PA16 | PA15 | PA14);
            *M8260_IOP_PADIR(immrVal) &= ~(PA29 | PA28 | PA21 | PA20 | PA19 | PA18);
            intDisable(INUM_FCC1);
            break;
        case 2:
            /* turn off the second FEC transceiver (set bit 4)*/

            *M8260_IOP_PCPAR(immrVal) &= ~(PC19 | PC18);    /* Rx CLK13 and Tx CLK14 */


            *M8260_IOP_PBPAR(immrVal) &= ~(PB31 | PB30 | PB29 | PB28 | PB27 | PB26 | PB25 |
                                           PB24 | PB23 | PB22 | PB21 | PB20 | PB19 | PB18);
            *M8260_IOP_PBDIR(immrVal) &= ~(PB31 | PB29 | PB25 | PB24 | PB23 | PB22);
            intDisable(INUM_FCC2);
            break;
        case 3:
            /* turn off the third FEC transceiver (set bit 5)*/
            /* *((volatile UINT8 *)(0x21000000)) = 0x80;       turn off the HotSwap LED for a tst */

            *M8260_IOP_PCPAR(immrVal) &= ~(PC17 | PC16);    /* Tx CLK15 and Rx CLK16 */


            *M8260_IOP_PBPAR(immrVal) &= ~(PB13 | PB12 | PB15 | PB14 | PB17 | PB16 | PB4 |
                                           PB5 | PB6 | PB7 | PB8 | PB9 | PB10 | PB11);
            *M8260_IOP_PBDIR(immrVal) &= ~(PB15 | PB14 | PB4 | PB5 | PB6 | PB7);
            intDisable(INUM_FCC3);
            break;
        }

    intUnlock (intLevel);  
    
    return(OK);
    }

#ifndef NSDELAY
#define MOTOROLA_FCC_LOOP_NS 2

#define NSDELAY(nsec)                                                   \
    {                                                                   \
    volatile int nx = 0;                                                \
    volatile int loop = (int)(nsec*MOTOROLA_FCC_LOOP_NS);               \
                                                                        \
    for (nx = 0; nx < loop; nx++);                                      \
    }
#endif /* NSDELAY */

/*********************************************************************** 
*
* sysFccEnetAddrGet - get the hardware Ethernet address
*
* This routine provides the six byte Ethernet hardware address that will be
* used by each individual FCC device unit.  This routine must copy
* the six byte address to the space provided by <addr>.
* The unit number must not be zero this is used for the scc.
*
* RETURNS: OK, or ERROR if the Ethernet address cannot be returned.
*
* ERRNO
*/
STATUS sysFccEnetAddrGet
    (
    int     unit, 
    UCHAR * pAddr
    )
    {
    /* need extra room for a 'EOS' -- driver may not provide it */
    UINT8 eAdrs[MAC_ADRS_LEN + 2];
    STATUS  status;
    int ix;

    status = sysNetMacNVRamAddrGet ("motfcc", unit, eAdrs, MAC_ADRS_LEN);
    
    if (status == OK)
        {

        for (ix = 0; ix < MAC_ADRS_LEN; ix++)
            pAddr[ix] = eAdrs[ix];
        }
    return status;
    }


/***********************************************************************
*
* sysFccMiiBitWr - write one bit to the MII interface
*
* This routine writes the value in <bitVal> to the MDIO line of a MII
* interface. The MDC line is asserted for a while, and then negated.
* If <bitVal> is NONE, then the MDIO pin should be left in high-impedance
* state.
*
* SEE ALSO: sysFccMiiBitRd()
*
* RETURNS: OK, or ERROR.
*
* ERRNO
*/
STATUS sysFccMiiBitWr
    (
    UINT32  immrVal,    /* base address of the on-chip RAM */
    UINT8   fccNum,     /* FCC being used */
    INT32   bitVal      /* the bit being written */
    )
    {
    /* 
     * We create the timing reference for transfers of info on the MDIO line 
     * MDIO as mapped on PC9, MDC on PC10. We need to keep the same data
     * on MDIO for at least 400 nsec.
     */

    *M8260_IOP_PCPAR(immrVal) &= ~(PC10 | PC9);
    *M8260_IOP_PCDIR(immrVal) |= (PC10 | PC9);
    *M8260_IOP_PCDAT(immrVal) |= (PC10);

    switch (bitVal)
        {
        case 0:
            *M8260_IOP_PCDAT(immrVal) &= ~(PC9);
            break;
        case 1:
            *M8260_IOP_PCDAT(immrVal) |= (PC9);
            break;
        case ((INT32) NONE):
            /* put it in high-impedance state */
            *M8260_IOP_PCDIR(immrVal) &= ~(PC9);
            break;
        default:
            return(ERROR);
        }

    /* delay about 200 nsec. */

    NSDELAY (200);

    /* now we toggle the clock and delay again */

    *M8260_IOP_PCDAT(immrVal) &= ~(PC10);

    NSDELAY (200);

    return (OK);
    }

/*********************************************************************** 
*
* sysFccMiiBitRd - read one bit from the MII interface
*
* This routine reads one bit from the MDIO line of a MII
* interface. The MDC line is asserted for a while, and then negated.
*
* SEE ALSO: sysFccMiiBitWr()
*
* RETURNS: OK, or ERROR.
*
* ERRNO
*/

STATUS sysFccMiiBitRd
    (
    UINT32      immrVal,    /* base address of the on-chip RAM */
    UINT8       fccNum,     /* FCC being used */
    INT8 *      bitVal      /* the bit being read */
    )
    {
    /* 
     * We create the timing reference for transfers of info on the MDIO line 
     * MDIO as mapped on PC9, MDC on PC10. We can read data on MDIO after
     * at least 400 nsec.
     */

    *M8260_IOP_PCPAR(immrVal) &= ~(PC10 | PC9);
    *M8260_IOP_PCDIR(immrVal) &= ~(PC9);

    *M8260_IOP_PCDIR(immrVal) |= (PC10);
    *M8260_IOP_PCDAT(immrVal) |= (PC10);

    /* delay about 200 nsec. */

    NSDELAY (200);

    /* now we toggle the clock and delay again */

    *M8260_IOP_PCDAT(immrVal) &= ~(PC10);

    NSDELAY (200);

    /* we can now read the MDIO data on PC9 */

    *bitVal = (*M8260_IOP_PCDAT(immrVal) & (PC9)) >> 22;
 
    return (OK);
    }

#endif /* INCLUDE_MOTFCCEND */


