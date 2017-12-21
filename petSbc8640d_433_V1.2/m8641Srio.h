#ifndef	INCm8641Srio
#define	INCm8641Srio
/* RapidIO Message Unit */

    /* RapidIO Outbound Message Registers */

/* Outbound mode register R/W 0x0000_0000 */
#define M8641RIO_OMR(base) (CAST(VUINT32 *)(((char*)base) + 0x13000)) 

/* Outbound status register Special1 0x0000_0000 */
#define M8641RIO_OSR(base) (CAST(VUINT32 *)(((char*)base) + 0x13004))

/* Outbound descriptor queue dequeue pointer address register R/W 
 * 0x0000_0000 */
#define M8641RIO_ODQDPAR(base) (CAST(VUINT32 *)(((char*)base) + 0x1300C)) 

/* Extended outbound source address register R/W 0x0000_0000 */
#define M8641RIO_EOSAR(base) (CAST(VUINT32 *)(((char*)base) + 0x13010))

/* Outbound source address register R/W 0x0000_0000 */
#define M8641RIO_OSAR(base) (CAST(VUINT32 *)(((char*)base) + 0x13014))

/* Outbound destination port register R/W 0x0000_0000 */
#define M8641RIO_ODPR(base) (CAST(VUINT32 *)(((char*)base) + 0x13018))

/* Outbound destination attributes register R/W 0x0006_0000 */
#define M8641RIO_ODATR(base) (CAST(VUINT32 *)(((char*)base) + 0x1301C))

/* Outbound double-word count register R/W 0x0000_0000 */
#define M8641RIO_ODCR(base) (CAST(VUINT32 *)(((char*)base) + 0x13020))

/* Outbound descriptor queue enqueue pointer address register R/W 
 * 0x0000_0000 */
#define M8641RIO_ODQEPAR(base) (CAST(VUINT32 *)(((char*)base) + 0x13028))

#define M8641RIO_OMRETCR(base) (CAST(VUINT32 *)(((char*)base) + 0x1302c))

#define M8641RIO_OMMGR(base) (CAST(VUINT32 *)(((char*)base) + 0x13030))

#define M8641RIO_OMMLR(base) (CAST(VUINT32 *)(((char*)base) + 0x13034))

    /* RapidIO Inbound Message Registers */

/* Inbound mailbox mode register R/W 0x0000_0000 */
#define M8641RIO_IMR(base) (CAST(VUINT32 *)(((char*)base) + 0x13060))

/* Inbound mailbox status register Special1 0x0000_0000 */
#define M8641RIO_ISR(base) (CAST(VUINT32 *)(((char*)base) + 0x13064))

/*Inbound frame queue dequeue pointer address register R/W 0x0000_0000 */
#define M8641RIO_IFQDPAR(base) (CAST(VUINT32 *)(((char*)base) + 0x1306C)) 

/* Inbound frame queue enqueue pointer address register R/W 0x0000_0000 */
#define M8641RIO_IFQEPAR(base) (CAST(VUINT32 *)(((char*)base) + 0x13074))


    /* RapidIO Doorbell Registers */

/* Inbound Doorbell mode register R/W 0x0000_0000 */
#define M8641RIO_IDMR(base) (CAST(VUINT32 *)(((char*)base) +  0x13460  ))

/* Inbound Doorbell status register Special1 0x0000_0000 */
#define M8641RIO_IDSR(base) (CAST(VUINT32 *)(((char*)base) +  0x13464 ))

/* Inbound Doorbell queue dequeue pointer address register R/W 0x0000_0000 */
#define M8641RIO_IDQDPAR(base) (CAST(VUINT32 *)(((char*)base) +  0x1346C ))

/* Inbound Doorbell queue enqueue pointer address register R/W 0x0000_0000 */
#define M8641RIO_IDQEPAR(base) (CAST(VUINT32 *)(((char*)base) +  0x13474 ))

    /* RapidIO Port-Write Registers */
/* Port-write mode register R/W 0x0000_0000 */
#define M8641RIO_IPWMR(base) (CAST(VUINT32 *)(((char*)base) +  0x134E0 ))

/* Port-write status register Special1 0x0000_0000 */
#define M8641RIO_IPWSR(base) (CAST(VUINT32 *)(((char*)base) +  0x134E4 ))

/* Port-write queue base address register R/W 0x0000_0000 */
#define M8641RIO_IPWQBAR(base) (CAST(VUINT32 *)(((char*)base) +  0x134EC ))


/* oubound Doorbell Registers */
#define M8641RIO_ODMR(base) (CAST(VUINT32 *)(((char*)base) +  0x13400))
#define M8641RIO_ODSR(base) (CAST(VUINT32 *)(((char*)base) +  0x13404))
#define M8641RIO_ODDPR(base) (CAST(VUINT32 *)(((char*)base) +  0x13418))
#define M8641RIO_ODDATR(base) (CAST(VUINT32 *)(((char*)base) +  0x1341C))
#define M8641RIO_ODRETCR(base) (CAST(VUINT32 *)(((char*)base) +  0x1342C))

#define RAPIDIO_REG_BASE 0xc0000

/********************ATMU****************************/

#define M8641RIO_RIWTAR(base) (CAST(VUINT32 *)(((char*)base) + 0x10d60)) /*RIWTAR4*/
#define M8641RIO_RIWBAR(base) (CAST(VUINT32 *)(((char*)base) + 0x10d68)) /*RIWBAR4*/
#define M8641RIO_RIWAR(base) (CAST(VUINT32 *)(((char*)base) + 0x10d70)) /*RIWAR4*/

#define M8641RIO_ROWTAR(base) (CAST(VUINT32 *)(((char*)base) + 0x10c20)) /*ROWTAR1*/
#define M8641RIO_ROWTEAR(base) (CAST(VUINT32 *)(((char*)base) + 0x10c24)) /*ROWTEAR1*/
#define M8641RIO_ROWBAR(base) (CAST(VUINT32 *)(((char*)base) + 0x10c28)) /*ROWBAR1*/
#define M8641RIO_ROWAR(base) (CAST(VUINT32 *)(((char*)base) + 0x10c30)) /*ROWAR1*/
#define M8641RIO_ROWS1R(base) (CAST(VUINT32 *)(((char*)base) + 0x10c34))/*ROWS1R1*/
#define M8641RIO_ROWS2R(base) (CAST(VUINT32 *)(((char*)base) + 0x10c38))/*ROWS1R2*/
#define M8641RIO_ROWS3R(base) (CAST(VUINT32 *)(((char*)base) + 0x10c3c))/*ROWS1R3*/


typedef struct rioInboundMsgCtrl
{
	UINT8 unit;/*Inbound controler NO (0 or 1)*/
	UINT32 localAddr;/*the addr is to store the inbound msg*/
	UINT16 MIQ_Thresh; 
	UINT16 FrameSize;
	UINT16 CIRQ_Size;	
	/*UINT32 valueIMnMR;*/
}RIO_IN_MSG_CTRL;

typedef struct rioInboundDoorbellCtrl
{
	UINT32 localAddr;/*the addr is to store the inbound msg*/
	UINT16 MIQ_Thresh; 
	UINT16 CIRQ_Size;
/*	UINT32 valueIDMR;*/
}RIO_IN_DOORBELL_CTRL;



typedef struct rioOutboundMsgCtrl
{
	UINT8 unit;/*Inbound controler NO (0 or 1)*/
	UINT16 targetId; /*the ID of target device*/
	UINT32 sendNum;/*Transfer count*/ /*sendNum:8µÄ±¶Êý*/
	UINT32 localAddr; /*the value of OMnSAR*/
	/*UINT32 valueOMnMR;*/
	UINT8 mailbox;/*(0 or 1)*/
	UINT8 MulticastEnable;/*(0 or 1)*/
	UINT8 MulticastGroup; /*(0-7)*/
	UINT32 MulticastList;
	
}RIO_OUT_MSG_CTRL;

typedef struct rioOutboundDoorbellCtrl
{
	UINT16 targetId; /*the ID of target device*/
	UINT16 data;
	UINT8 RET;/*Retry error threshold.*/
}RIO_OUT_DOORBELL_CTRL;


#endif /* INCm8641Srio */
