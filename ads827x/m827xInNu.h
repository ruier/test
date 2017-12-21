/* m827xInUn.h - MPC 827x interrupt numbers adapter file */

/*
 * Copyright (c) 2009, 2013 Wind River Systems, Inc.
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,29may13,d_l  Fix IVEC_MAX. (WIND00419006)
01a,10jun09,wqi  Create file for correct MPC827x's interrupt numbers. 
*/

#ifndef __INCm827xInUnh
#define __INCm827xInUnh


#ifdef __cplusplus
extern "C" {
#endif


/* interrupt numbers  */
#undef INUM_PC15
#undef INUM_PC14
#undef INUM_PC13
#undef INUM_PC12
#undef INUM_PC11
#undef INUM_PC10
#undef INUM_PC9 
#undef INUM_PC8 
#undef INUM_PC7 
#undef INUM_PC6 
#undef INUM_PC5 
#undef INUM_PC4 
#undef INUM_PC1 
#undef INUM_PC0 

#define INUM_USB   11           /*    11    USB */
#define INUM_SEC   40           /*    47    SEC */
#define INUM_PC29  41           /*    48    PC29*/
#define INUM_PC23  42           /*    49    PC23*/
#define INUM_PC15  43           /*    50    PC15*/
#define INUM_PC14  44           /*    51    PC14*/
#define INUM_PC13  45           /*    52    PC13*/
#define INUM_PC12  46           /*    53    PC12*/
#define INUM_PC11  47           /*    54    PC11*/
#define INUM_PC10  48           /*    55    PC10*/
#define INUM_PC9   49           /*    56    PC9 */
#define INUM_PC8   50           /*    57    PC8 */
#define INUM_PC7   51           /*    58    PC7 */
#define INUM_PC6   52           /*    59    PC6 */
#define INUM_PC5   53           /*    60    PC5 */
#define INUM_PC4   54           /*    61    PC4 */
#define INUM_PC1   55           /*    62    PC1 */
#define INUM_PC0   56           /*    63    PC0 */

#undef  IVEC_MAX
#define IVEC_MAX   63

#ifdef __cplusplus
}
#endif

#endif /* __INCm827xInUnh */
