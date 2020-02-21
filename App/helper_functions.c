/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file helper_functions.c
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2020-01-08
 * 
 * @brief main function call 
 *
 * @version 0.1
 *
 * @section REVISION HISTORY
 *  - 0.1 KG 2020-01-08 Initial implementation 
 *
 ******************************************************************************/
 
/*******************************************************************************
**                                Includes
*******************************************************************************/

#include "helper_functions.h"

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/*******************************************************************************
**                                 Code
*******************************************************************************/

/*!*****************************************************************************
* @function 
* 
* @brief 
*
* @param 
*
* @return 
*******************************************************************************/
inline bool isInRange (const signalVar_t var, const signalVar_t min, const signalVar_t max)
{
    if ((var >= min) && (var <= max))
    {
        return true;
    } else
    {
        return false;
    }
}

/*!*****************************************************************************
* @function 
* 
* @brief 
*
* @param 
*
* @return 
*******************************************************************************/
inline signalCheck_e signalCheck(const signalVar_t var, const signalVar_t min, const signalVar_t max)
{
    signalCheck_e retVal;
    
    retVal = SIGNAL_FAULT;
    
    retVal = isInRange_m(var,min,max) ? SIGNAL_IN_RANGE : retVal; 
    retVal = (var < min) ? SIGNAL_UNDER : retVal;
    retVal = (var > max) ? SIGNAL_OVER : retVal;
    retVal = (var == min) ? SIGNAL_IDLE : retVal;
    retVal = (var == max) ? SIGNAL_PEAK : retVal;
    
    return retVal;
 }

/******************************************************************************
**                               End Of File
*******************************************************************************/
