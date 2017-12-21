/* sysMotSccEnd.c - system configuration module for m8260SccEnd driver */
 
/* Copyright 1984-2004 Wind River Systems, Inc. */
#include "copyright_wrs.h"
 
/*
modification history
--------------------
01g,24sep04,mdo  Documentation fixes for apigen
01f,30jul04,rec  unit test, don't swap bytes after calling
                 sysNetMacNVRamAddrGet
01e,19jul04,rec  sysSccEnetAddrGet must swap bytes to support new 
                 MAC address scheme ('M') (SPR #93020)
01d,25aug03,ckj  SPR 85477: Support multiple MAC address in sysSccEnetAddrGet
01c,16jan03,gjc  SPR#85071 and SPR#85071 :Added mechanism to support
                 initial duplex setting.
01b,23sep02,gjc  Fixing SPRs #81417 and #82303 - sysSccEnetDisable
         sets bits in portB instead of portD. Always use
         global mac[1] for scc0. Fixes Duplicate MAC.
01a,07mar02,kab  SPR 70817: *EndLoad returns NULL on failure
01b,10may01, g_h  rename to sysMotSccEnd.c
01a,08mar01, g_h  created from sbc8260/sysMotFccEnd.c
*/
 
/*
DESCRIPTION
This is the WRS-supplied configuration module for the VxWorks
motCpmEnd END driver.

It performs the dynamic parameterization of the motCpmEnd driver.
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
#include "drv/mem/m82xxDpramLib.h" 
#include "drv/end/m8260SccEnd.h"

/* defines */

#ifdef INCLUDE_MOTSCCEND

#define MOT_SCC_INTVEC_NUM  0x23  /* Interrupt vector number */
#define MOT_SCC_NUM         0x1   /* SCC number */
#define MOT_SCC_TBD_NUM     0x20  /* number of transmit buffer descriptors (TBD)*/
#define MOT_SCC_RBD_NUM     0x20  /* number of receive buffer descriptors (RBD)*/

/* imports */
 
IMPORT END_OBJ * m8260SccEndLoad (char *);
IMPORT UINT32    sysPCGet (void);

/* forward declarations */

void   sysSccEnetDisable (int unit);
void   sysSccEnetIntDisable (int unit);
STATUS sysSccEnetEnable ( int unit );
STATUS sysSccEnetAddrGet (int  unit, UINT8 *addr);
STATUS sysSccEnetCommand (int  unit, UINT16 command);
void   sysSccEnetIntEnable (int unit);
void   sysSccEnetIntClear (int unit);

/***********************************************************************
*
* sysMotSccEndLoad - load an instance of the m8260SccEnd driver
*
* This routine loads the m8260SccEnd driver with proper parameters. It also
* reads the BCSR3 to find out which type of processor is being used, and
* sets up the load string accordingly.
*
* The END device load string formed by this routine is in the following
* format.
* <motCpmAddr>:<ivec>:<sccNum>:<txBdNum>:<rxBdNum>:<txBdBase>:<rxBdBase>:
* <bufBase>
*
* \is
* \i <motCpmAddr>
* Internal memory address
* \i <ivec>
* Interrupt vector 
* \i <sccNum>
* SCC number being used
* \i <txBdNum>
* number of TBDs or NULL
* \i <rxBdNum>
* number of RBDs or NULL
* \i <txBdBase>
* base location of the TBDs
* \i <rxBdBase>
* base location of the RBDs
* \i <bufBase>
* address of the shared memory region
* \i <flags>
* SCC_USR_RUNNING_FROM_ROM
* SCC_USR_DUPLEX_HALF or SCC_USR_DUPLEX_FULL 
*
* This routine only loads and initializes one instance of the device.
* If the user wishes to use more than one m8260SccEnd devices, this routine
* should be changed. 
* The driver interface supports setting the duplex mode to half or full 
* via the flag load string parameter. The default is half duplex.
* \ie
* 
* RETURNS: pointer to END object or NULL.
*
* ERRNO
*
* SEE ALSO: m8260EndLoad ()
*/
 
END_OBJ * sysMotSccEndLoad
    (
    char * pParamStr,   /* ptr to initialization parameter string */
    void * unused       /* unused optional argument */
    )
    {
    /*
     * The motCpmEnd driver END_LOAD_STRING should be:
     * <motCpmAddr>:<ivec>:<sccNum>:<txBdNum>:<rxBdNum>:<txBdBase>:
     * <rxBdBase>:<bufBase>:<userFlags>
     * Note that unit string is prepended by the mux, so we
     * don't put it here.
     */
 
    char * pStr = NULL;
    char   paramStr [200];
    UINT32 regPC;
    UINT32 motSccTxBufDescBase;
    UINT32 motSccRxBufDescBase;
    UINT32 motSccBufBase;
    UINT32 motSccFlags;

    static char motSccEndParamTemplate [] = "0x%x:0x%x:%d:0x%x:0x%x:0x%x:0x%x:0x%x:0x%x";

    END_OBJ * pEnd;
 
    if (strlen (pParamStr) == 0)
        {
        /*
         * muxDevLoad() calls us twice.  If the string is
         * zero length, then this is the first time through
         * this routine.
         */
        pEnd = (END_OBJ *) m8260SccEndLoad (pParamStr);
        }
    else
        {
        regPC = sysPCGet ();

        motSccFlags = 0;

        if ((regPC & ROM_BASE_ADRS) == ROM_BASE_ADRS)
            {
            /* We are running from ROM */

            motSccBufBase = 0x04100000;               /* BD in local bus */
            motSccFlags   |= SCC_USR_RUNNING_FROM_ROM; /* user flags */
            }
        else
            {
            /* We are running from RAM */

            motSccBufBase = -1UL; /* BD in 60x bus */
            }

        motSccFlags |= SCC_USR_DUPLEX_HALF;  /* user flags */

        /*
         * On the second pass through here, we actually create
         * the initialization parameter string on the fly.
         * Note that we will be handed our unit number on the
         * second pass and we need to preserve that information.
         * So we use the unit number handed from the input string.
         */

        if ((motSccTxBufDescBase = (UINT32)m82xxDpramAlignedMalloc (0x280,8)) == 0)
            return 0;

        if ((motSccRxBufDescBase = (UINT32)m82xxDpramAlignedMalloc (0x280,8)) == 0)
            return 0;

        pStr = strcpy (paramStr, pParamStr);

        /* Now, we get to the end of the string */

        pStr += strlen (paramStr);


        /* finish off the initialization parameter string */

        sprintf (pStr, motSccEndParamTemplate,
                 (UINT) vxImmrGet (),
                 MOT_SCC_INTVEC_NUM,
                 MOT_SCC_NUM,
                 MOT_SCC_TBD_NUM,   
                 MOT_SCC_RBD_NUM,   
                 motSccTxBufDescBase,
                 motSccRxBufDescBase,
                 motSccBufBase,
                 motSccFlags);

        if ((pEnd = (END_OBJ *) m8260SccEndLoad  (paramStr)) == (END_OBJ *)NULL)
            {
            logMsg ("Error: m8260SccEndLoad  failed to load driver\n",
                    0, 0, 0, 0, 0, 0);
            }
        }

    return (pEnd);
    }

/***********************************************************************
*
* sysSccEnetDisable - disable the Ethernet controller
*
* This routine is expected to perform any target specific functions required
* to disable the Ethernet controller.  This usually involves disabling the
* Transmit Enable (TENA) signal.
*
* RETURNS: N/A
*
* ERRNO
*/

void sysSccEnetDisable
    (
    int         unit    /* not used - only slave SCC1 is wired to port */
    )
    {
    /* Get the physical location of the IMMR register */
    int immrVal = vxImmrGet();


    *M8260_IOP_PDPAR(immrVal) &= ~(PD29);    /* clear port D pin 29 -> RTS1 = *TENA */
    
    }

/***********************************************************************
*
* sysSccEnetIntDisable - disable the Ethernet interface interrupt
*
* This routine disables the interrupt for the Ethernet interface specified
* by <unit>.
*
* RETURNS: N/A
*
* ERRNO
*/

void sysSccEnetIntDisable
    (
    int     unit
    )
    {

    /* Get the physical location of the IMMR register  */

    int immrVal = vxImmrGet ();

    *M8260_SIMR_L(immrVal) &= ~0x00800000;   /* Mask SCC1 */
    }

/***********************************************************************
*
* sysSccEnetEnable - enable the Ethernet controller
*
* This routine is expected to perform any target specific functions required
* to enable the Ethernet controller.  These functions typically include
* enabling the Transmit Enable signal (TENA) and connecting the transmit
* and receive clocks to the SCC.
*
* RETURNS: OK, or ERROR if the Ethernet controller cannot be enabled.
*
* ERRNO
*/

STATUS sysSccEnetEnable
    (
    int     unit    /* not used - only slave SCC1 is wired to port */
    )
    {
    UINT32 temp;
    int immrVal = vxImmrGet ();

    /*
     * Configure port D pins to enable TXD1 and RXD1.
     *
     * Set PPARD[30,31] and PDIRD[30] and clear PDIRD[31] 
     * and PSORD[31].  Set PSORD[30].
     */
    *M8260_IOP_PDPAR(immrVal) |=  (PD30 | PD31);  /* Set PD30 and PD31 */
    *M8260_IOP_PDDIR(immrVal) |=  (PD30);         /* Set PD30 */
    *M8260_IOP_PDDIR(immrVal) &= ~(PD31);         /* Clear PD31 */
    *M8260_IOP_PDSO(immrVal)  |=  (PD30);         /* Set PD30 */
    *M8260_IOP_PDSO(immrVal)  &= ~(PD31);         /* Clear PD31 */

    /*
     * Configure ports C and D to enable TENA1 (RTS1), CLSN1 (CTS1),
     * and RENA1 (CD1).
     *
     * Set PPARD[29], PPARC[14,29], PDIRD[29], and PSORC[29] and clear 
     * PDIRC[14,29], PSORC[14] and PSORD[29].
     */
    *M8260_IOP_PDPAR(immrVal) |=  (PD29);         /* Set PD29 */
    *M8260_IOP_PCPAR(immrVal) |=  (PC14 | PC29);  /* Set PC14 and PC29 */
    *M8260_IOP_PDDIR(immrVal) |=  (PD29);         /* Set PD29 */
    *M8260_IOP_PCSO(immrVal)  |=  (PC29);         /* Set PC29 */
    *M8260_IOP_PCDIR(immrVal) &= ~(PC14 | PC29);  /* Clear PC14 and PC29 */
    *M8260_IOP_PCSO(immrVal)  &= ~(PC14);         /* Clear PC14 */
    *M8260_IOP_PDSO(immrVal)  &= ~(PD29);         /* Clear PD29 */

    /*
     * Configure port C pins to enable CLK11 and CLK12.
     *
     * Set PPARC[20,21] and clear PDIRC[20,21] and PSORC[20,21].
     */
    *M8260_IOP_PCPAR(immrVal) |=  (PC20 | PC21);  /* Set PC20 and PC21 */
    *M8260_IOP_PCDIR(immrVal) &= ~(PC20 | PC21);  /* Clear PC20 and PC21 */
    *M8260_IOP_PCSO(immrVal)  &= ~(PC20 | PC21);  /* Clear PC20 and PC21 */


    /*
     * Connect CLK11 to the SCC1 receiver and CLK12 to the
     * transmitter using the CPM mux.  Also, connect the SCC1
     * to the NMSI.
     *
     * Program CMXSCR[R1CS] to 0b100 and CMXSCR[T1CS] to 0b101.
     * Clear CMXSCR[SC1].
     */
    temp = (*M8260_CMXSCR(immrVal) & 0x00ffffff);
    *M8260_CMXSCR(immrVal) = (temp | 0x25000000);
 
    return (OK);
    }


/***********************************************************************
*
* sysSccEnetAddrGet - gets the 6 byte ethernet address
*
* The routine gets the 6 byte ethernet address used by the ethernet device.
*
* RETURNS: OK, or ERROR
*
* ERRNO
*/

STATUS sysSccEnetAddrGet
    (
    int     unit ,       
    UINT8 * pAddr   /* location address is returned in  */
    )
    {
    /* need extra room for a 'EOS' -- driver may not provide it */
    UINT8   eAdrs[MAC_ADRS_LEN + 2];
    STATUS  status;
    int ix;

    status = sysNetMacNVRamAddrGet ("motscc", unit, eAdrs, MAC_ADRS_LEN); 
    if (status == OK)
        {

        for (ix = 0; ix < MAC_ADRS_LEN; ix++)
            pAddr[ix] = eAdrs[ix];
        }
    return status;
    }

/***********************************************************************

*
* sysSccEnetCommand - issue a command to the Ethernet interface controller
*
* The routine issues a command to the Ethernet interface controller.
*
* RETURNS: OK, or ERROR if the Ethernet controller could not be restarted.
*
* ERRNO
*/

STATUS sysSccEnetCommand
    (
    int     unit,
    UINT16  command
    )
    {
    int immrVal = vxImmrGet ();

    while (*M8260_CPCR(immrVal) & M8260_CPCR_FLG);

    *M8260_CPCR(immrVal) = M8260_CPCR_PAGE(M8260_CPCR_PAGE_SCC1) | \
                           M8260_CPCR_SBC(M8260_CPCR_SBC_SCC1)   | \
                           command                               | \
                           M8260_CPCR_FLG;
    
    while (*M8260_CPCR(immrVal) & M8260_CPCR_FLG);

    return (OK);
    }

/***********************************************************************
*
* sysSccEnetIntEnable - enable the Ethernet interface interrupt
*
* This routine enables the interrupt for the Ethernet interface specified
* by <unit>.
*
* RETURNS: N/A.
*
* ERRNO
*/

void sysSccEnetIntEnable
    (
    int     unit
    )
    {

    /* Get the physical location of the IMMR register */

    int immrVal = vxImmrGet ();

    *M8260_SIMR_L(immrVal) |= 0x00800000;   /* Enable SCC1 */
    }

/***********************************************************************
*
* sysSccEnetIntClear - clear the Ethernet interface interrupt
*
* This routine clears the interrupt for the Ethernet interface specified
* by <unit>.
*
* RETURNS: N/A.
*
* ERRNO
*/

void sysSccEnetIntClear
    (
    int     unit
    )
    {
    /* Get the physical location of the IMMR register */

    int immrVal = vxImmrGet ();

    *M8260_SIPNR_L(immrVal) = 0x00800000;
    }

#endif /* INCLUDE_MOTSCCEND */


