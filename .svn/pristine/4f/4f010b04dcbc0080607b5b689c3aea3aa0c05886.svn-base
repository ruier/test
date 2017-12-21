/* usrvxTestV2Api.c - VxTestV2 API wrapper configlette */

/*
 * Copyright (c) 2009 Wind River Systems, Inc.
 *
 *
 * The right to copy, distribute, modify or otherwise make use
 * of this software may be licensed only pursuant to the terms
 * of an applicable Wind River license agreement.
 */

/*
modification history
--------------------
01b,21apr09,d_c  Add documentation
01a,01apr09,d_c  written
*/

/*
DESCRIPTION

This configlette defines the vxTestV2 external API as wrapper
functions. Each wrapper function calls either the small-footprint
version of the API, or the large-footprint version of the API,
depending on which driver is included with the build.  This
effectively separates the small footprint and large footprint drivers,
and prevents the project build tools from dragging in the large
footprint driver to resolve APIs called by test modules.


INCLUDE FILES: vxTestV2Lf.h, vxTestV2Sf.h

SEE ALSO: vxTestV2Sf.c, vxTestV2Int.c, vxTestV2Base6.c
*/

#if defined(INCLUDE_VXTESTV2_SF_DRIVER)

#include <vxTestV2Sf.h>

#else

#include <vxTestV2Lf.h>

#endif

#ifdef BASE6

/***************************************************************************
* vxTestMsgInt - output a message from test code
* 
* This function is used within a test case to output information in a
* consistent and parseable format.
*
* This function is used in BASE6 to add compiler-generated file
* and line data to output messages. vxTestMsg is defined as a macro,
* that calls vxTestMsgInt with __LINE__ and __FILE__
*
* RETURNS: N/A
*
* SEE ALSO: vxTestLfMsgInt, vxTestSfMsgInt
*
* ERRNO: N/A
*
* \NOMANUAL
*/

void vxTestMsgInt
    (
    int              line,  /* Line in file */
    char *           file,  /* File name */
    VXTEST_VERBOSITY level, /* Verbose level associated with the message */
    char *           pMsg,  /* Message to be sent to the output device */
    ...                     /* variable parameters */
    )
    {
    va_list          vaList;

    va_start (vaList, pMsg);
    VXTEST_MSG_INT (line, file, level, pMsg, vaList);
    va_end (vaList);
    }

#else /* ! BASE6 */

/* Not BASE6. The API is not turned into a macro. It is called directly.
 */

/***************************************************************************
* vxTestMsg - output a message from test code
* 
* This function is used within a test case to output information in a
* consistent and parseable format.
*
* RETURNS: N/A
*
* SEE ALSO: vxTestLfMsg, vxTestSfMsg
*
* ERRNO: N/A
*/

void vxTestMsg
    (
    VXTEST_VERBOSITY level, /* Verbose level associated with the message */
    char *           pMsg,  /* Message to be sent to the output device */
    ...                     /* variable parameters */
    )
    {
    va_list          vaList;

    va_start (vaList, pMsg);
    VXTEST_MSG (level, pMsg, vaList);
    va_end (vaList);
    }

#endif /* BASE6 */


/***************************************************************************
* vxTestWindBaseGet - get the wind base environment variable
* 
* This utility function is provided for test cases that needs to load
* modules or do some other manipulations that requires knowledge of
* WIND_BASE.
*
* RETURNS: A pointer to $WIND_BASE
*
* SEE ALSO: vxTestLfWindBaseGet, vxTestSfWindBaseGet
*
* ERRNO: N/A
*/

char * vxTestWindBaseGet (void)
    {
    return VXTEST_WIND_BASE_GET ();
    }

/***************************************************************************
* vxTestMdlFdGet - get a file descriptor for a test module
* 
* Given the name of a module, this routine returns the corresponding file
* descriptor. The caller is responsible of closing the descriptor.
* In case of an error, the routine also properly handles error messages.
*
* RETURNS: a file descriptor corresponding to the given module name, ERROR
* otherwise.
*
* ERRNO: N/A
*
* SEE ALSO: vxTestLfMdlFdGet, vxTestSfMdlFdGet
*/

int vxTestMdlFdGet
    (
    char * moduleName
    )
    {
    return VXTEST_MDL_FD_GET (moduleName);
    }

/***************************************************************************
* vxTestParamGet - get parameter from driver (deprecated)
* 
* This function takes a string and returns the other half of the
* parameter string, <param>=<value>. That is, given <param> returns
* <value>.
*
* RETURNS: A pointer to the value of the parameter, or NULL if the value
* could not be found.
*
* SEE ALSO: vxTestLfParamGet, vxTestSfParamGet
*
* ERRNO: N/A
*/

char * vxTestParamGet
    (
    char * paramName
    )
    {
    return VXTEST_PARAM_GET (paramName);
    }

/***************************************************************************
* vxTestBenchmark - output information from benchmark test
* 
* This function is intended to be called by benchmark tests to report results
* in a uniform manner. This will allow performance tests to be executed under
* vxTestV2, the results of which can be parsed and compared.
*
* RETURNS: N/A
*
* SEE ALSO: vxTestLfBenchmark, vxTestSfBenchmark
*
* ERRNO: N/A
*/

void vxTestBenchmark
    ( 
    char *benchmarkID,      /* Benchmark identification string */
    double benchmarkValue,  /* Benchmark value */
    char *units,            /* Units of benchmark value */
    long iterations         /* Number of iterations used */
    )
    {
    VXTEST_BENCHMARK (benchmarkID, benchmarkValue, units, iterations);
    }

/***************************************************************************
* vxTestUsDelay - Perform a microsecond delay.
*
* This function delays for a given number of microseconds using
* a calibrated, software-based delay loop.
*
* RETURNS: N/A
*
* SEE ALSO: vxTestLfUsDelay, vxTestSfUsDelay
*
* ERRNO: N/A
*/

void vxTestUsDelay
    (
    int uSecs
    )
    {
    VXTEST_US_DELAY (uSecs);
    }



