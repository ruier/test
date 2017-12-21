/*-----------------------------------------------------------------------------+
*
*  File Name:   patchtblb.s
*
*  Function:    File that reserves space in the .text section so that
*               the patch utility can use it for fixing 405GP rev B errata.
*
*  Author:      M. Game
*
*  Change Activity-
*
*  Date        Description of Change                                       BY
*  ---------   ---------------------                                       ---
*  14-Jun-99   Created                                                     MCG
*  17-Feb-00   Modified for rev B                                           NL
*
+-----------------------------------------------------------------------------*/

/*
 * To reserve the appropriate amount of space in "patch_tbl", modify the
 * constant PATCH_ENTRIES below to reflect the number of load/store with update
 * instruction pairs in the application.
 *
 * To determine how many of these situations exist in an application, run the
 * patch utility against the application with PATCH_ENTRIES set to 1.  The
 * initial run of the patch utility will then tell you the exact value to set
 * PATCH_ENTRIES for the application.  PATCH_ENTRIES can be set to a larger
 * value than required if desired.
 */


        .set PATCH_ENTRIES, 650


/*############################################################################*/
/*#################### DO NOT MODIFY BELOW THIS POINT ########################*/
/*############################################################################*/

        .text
        .align  2                           /* must be on a 4 byte boundary */
        FUNC_EXPORT(patchtbb)

patchtbb:
        .space  PATCH_ENTRIES*12            /* 12 bytes for each patch      */
        .size  patchtbb,.-patchtbb
