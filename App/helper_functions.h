/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file .h
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2019-12-20
 * 
 * @brief 
 *
 * @version
 *
 * @section REVISION HISTORY
 *  - KG 2019-12-20 Initial implementation 
 *
 ******************************************************************************/

/*******************************************************************************
**                                 Guards
*******************************************************************************/

#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

#ifdef __cplusplus
extern "C"
{
#endif /* extern "C" */

/*******************************************************************************
**                                Includes
*******************************************************************************/

#include <stdbool.h>

/*******************************************************************************
**                               Constants
*******************************************************************************/  

/*******************************************************************************
**                            Data Structures
*******************************************************************************/

typedef int signalVar_t;

typedef enum 
{
    SIGNAL_IN_RANGE = 0,
    SIGNAL_OVER,
    SIGNAL_UNDER,
    SIGNAL_PEAK,
    SIGNAL_IDLE,
    SIGNAL_FAULT
} signalCheck_e;

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/*******************************************************************************
**                                Macros
*******************************************************************************/

/*checks if var is in between min and max including boundaries*/ 
#define isInRange_m(var, min, max) ((((var) >= (min)) && ((var) <= (max))) ? true : false)

/*returns a if a is smaller then b, in other cases returns b*/
#define min_m(a,b) (((a) < (b)) ? (a) : (b))

/*returns a if a is greater then b, in other cases returns b*/
#define max_m(a,b) (((a) > (b)) ? (a) : (b))

/*******************************************************************************
**                     Public function prototypes - API
*******************************************************************************/

extern bool isInRange (const int var, int min, int max);
extern signalCheck_e signalCheck(const signalVar_t var, const signalVar_t min, const signalVar_t max);

#ifdef __cplusplus
} /* extern "C" */
#endif /* nextern "C" */

#endif /* HELPER_FUNCTIONS_H */


/******************************************************************************
**                               End Of File
*******************************************************************************/


