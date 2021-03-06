/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file appe_processing_common.c
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2020-01-08
 * 
 * @brief common processing functions for modeled processors A and B 
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

#include "appe_processing_common.h"
#include "helper_functions.h"
#include "logger.h"
#include <stdio.h>

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/*******************************************************************************
**                                 Code
*******************************************************************************/

/*!*****************************************************************************
* @function checkSignalIntegrity
* 
* @brief checks input signal constraints
*
* @param data - signal data pointer
*
* @returnappe_flag_t signal classification 
*******************************************************************************/

appe_flags_t checkSignalConstraints(const dsp_data_t sampleA, const dsp_data_t sampleB, const tst_data_attributes_pt constraints)
{
    signalCheck_e signalClass;
    appe_flags_t retVal;
    signalVar_t signalMin, signalMax, signalTolerance;
    
    /*calculate values with acceptable tolerance*/
    signalTolerance = constraints->guardRegion*SIGNAL_TOLERANCE;
    signalMin = constraints->guardRegion - signalTolerance;
    signalMax = constraints->resolution - constraints->guardRegion + signalTolerance;
    
    /*check sensor A signal boundaries*/
    signalClass = signalCheck(sampleA, signalMin, signalMax);
    
    retVal = 0; /*initialise flag register*/
    
    /*set flags for sensor A signal*/
    switch (signalClass)
    {
        case SIGNAL_FAULT:
            retVal |= APPE_SENS_A_FAULT;
            break;
        case SIGNAL_OVER:
            retVal |= APPE_SENS_A_OUT_OF_BOUNDS;
            LogFull_m(ERR_SIGNAL_OUT_OF_BOUNDS, "Sensor signal A over limits", 0,0);
            break;
        case SIGNAL_UNDER:
            retVal |= APPE_SENS_A_OUT_OF_BOUNDS;
            LogFull_m(ERR_SIGNAL_OUT_OF_BOUNDS, "Sensor signal A under limits", 0,0);
            break;
        case SIGNAL_IN_RANGE:
            retVal |= APPE_SYSTEM_OK;
            break;
        case SIGNAL_PEAK:
            retVal |= APPE_SYSTEM_OK;
            break;
        case SIGNAL_IDLE:
            retVal |= APPE_SYSTEM_OK;
            break;
        default:
            retVal |= APPE_SENS_A_FAULT;
    }

    /*check sensor B signal boundaries*/
    signalClass = signalCheck(sampleB, signalMin, signalMax);
    
    /*set flags for sensor B signal*/
    switch (signalClass)
    {
        case SIGNAL_FAULT:
            retVal |= APPE_SENS_B_FAULT;
            break;
        case SIGNAL_OVER:
            retVal |= APPE_SENS_B_OUT_OF_BOUNDS;
            LogFull_m(ERR_SIGNAL_OUT_OF_BOUNDS, "Sensor signal B over limits", 0,0);
            break;
        case SIGNAL_UNDER:
            retVal |= APPE_SENS_B_OUT_OF_BOUNDS;
            LogFull_m(ERR_SIGNAL_OUT_OF_BOUNDS, "Sensor signal B under limits", 0,0);
            break;
        case SIGNAL_IN_RANGE:
            retVal |= APPE_SYSTEM_OK;
            break;
        case SIGNAL_PEAK:
            retVal |= APPE_SYSTEM_OK;
            break;
        case SIGNAL_IDLE:
            retVal |= APPE_SYSTEM_OK;
            break;
        default:
            retVal |= APPE_SENS_B_FAULT;
    }
    
    return retVal;
}

/*!*****************************************************************************
* @function checkComplementarity 
* 
* @brief checks input signals  
*
* @param data - signal data pointer
*
* @returnappe_flag_t signal classification 
*******************************************************************************/
bool checkComplementarity(const dsp_data_t sampleA, const dsp_data_t sampleB, const tst_data_attributes_pt constraints)
{
    dsp_data_t signalSum, signalTolerance, signalOffset;
    dsp_data_t minVal, maxVal;
    bool retVal;
    
    /*calculate signal tolerance*/
    signalTolerance = constraints->resolution * SIGNAL_TOLERANCE;
    minVal = constraints->resolution - signalTolerance;
    maxVal = constraints->resolution + signalTolerance;
    
    signalSum = sampleA + sampleB; /*should match ADC resolution range*/
    
    if(isInRange_m(signalSum, minVal, maxVal))
    {
        retVal = true;
    } else
    {
        signalOffset = constraints->resolution - signalSum;
        LogFull_m(WAR_DATA_DIFFERENCE, "Sampple data difference", 1, (int)signalOffset);
        retVal = false;
    }
    
    return retVal;
}

/******************************************************************************
**                               End Of File
*******************************************************************************/
