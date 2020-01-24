/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file main.cpp
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2019-12-20
 * 
 * @brief main function call 
 *
 * @version 0.1
 *
 * @section REVISION HISTORY
 *  - 0.1 KG 2019-12-20 Initial implementation 
 *
 ******************************************************************************/
 
/*******************************************************************************
**                                Includes
*******************************************************************************/

#include "acs_processing_common.h"
#include "helper_functions.h"
#include <stdio.h>

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
* @returns 
*******************************************************************************/
acs_flags_t checkSignalIntegrity(const input_data_pt data)
{

    return ACS_SYSTEM_FAULT;    
}

acs_flags_t checkSignalConstraints(const dsp_data_t sampleA, const dsp_data_t sampleB, const tst_data_attributes_pt constraints)
{
    signalCheck_e signalClass;
    acs_flags_t retVal;
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
            retVal |= ACS_SENS_A_FAULT;
            break;
        case SIGNAL_OVER:
            retVal |= ACS_SENS_A_OUT_OF_BOUNDS;
            break;
        case SIGNAL_UNDER:
            retVal |= ACS_SENS_A_OUT_OF_BOUNDS;
            break;
        case SIGNAL_IN_RANGE:
            retVal |= ACS_SYSTEM_OK;
            break;
        case SIGNAL_PEAK:
            retVal |= ACS_SYSTEM_OK;
            break;
        case SIGNAL_IDLE:
            retVal |= ACS_SYSTEM_OK;
            break;
        default:
            retVal |= ACS_SENS_A_FAULT;
    }

    /*check sensor B signal boundaries*/
    signalClass = signalCheck(sampleB, signalMin, signalMax);
    
    /*set flags for sensor B signal*/
    switch (signalClass)
    {
        case SIGNAL_FAULT:
            retVal |= ACS_SENS_B_FAULT;
            break;
        case SIGNAL_OVER:
            retVal |= ACS_SENS_B_OUT_OF_BOUNDS;
            break;
        case SIGNAL_UNDER:
            retVal |= ACS_SENS_B_OUT_OF_BOUNDS;
            break;
        case SIGNAL_IN_RANGE:
            retVal |= ACS_SYSTEM_OK;
            break;
        case SIGNAL_PEAK:
            retVal |= ACS_SYSTEM_OK;
            break;
        case SIGNAL_IDLE:
            retVal |= ACS_SYSTEM_OK;
            break;
        default:
            retVal |= ACS_SENS_B_FAULT;
    }
    
    return retVal;
}

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
        printf(" #DIF:%d # ", (int)signalOffset);
        retVal = false;
    }
    
    return retVal;
}

/******************************************************************************
**                               End Of File
*******************************************************************************/