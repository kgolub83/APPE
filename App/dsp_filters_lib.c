/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file dsp_filters_lib.c
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2020-01-08
 * 
 * @brief signal procesing digital filters implementations
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

#include "dsp_filters_lib.h"

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/*******************************************************************************
**                                 Code
*******************************************************************************/

/*!*****************************************************************************
* @function recursiveAverage
* 
* @brief first order recursive IIR filter implementation
*
* @param filter - pointer to filter structure
*
* @return void function
*******************************************************************************/
inline void recursiveAverage(recursive_avg_pt filter)
{
    iir_avg_buffer_samples_t i;

    for (i=0; i < filter->bufferSamples; i++)
    {
      filter->outputData[i] = filter->coefficient*(filter->inputData[i] - filter->lastSample) + filter->lastSample;
      filter->lastSample = filter->outputData[i];
    }
}

/*!*****************************************************************************
* @function movingAvgInit
* 
* @brief Initializes moving average filter
*
* @param filter - pointer to moving average filter structure
* @param initData - initialisation data
*
* @return void function
*******************************************************************************/
dsp_filter_return_e movingAvgInit(moving_avg_pt filter, mov_avg_data_t initData)
{
    mov_avg_window_samples_t i;
    dsp_filter_return_e retVal;
    bool ctrlLogic;
    
    /* init moving awerage sliding window */
    filter->sum = initData*(float)filter->avgWinSamples;
    filter->avgWinPointer = 0;

    for(i=0; i < filter->avgWinSamples ;i++)
    {
        filter->avgWinData[i] = initData;
    }
    
    /* check filter settings */
    ctrlLogic = (filter->bufferSamples > 0) &&
                (filter->avgWinSamples > 0);
    
    if(ctrlLogic)
    {
        filter->coefficient = 1/(float)filter->avgWinSamples;
        retVal = DSP_FILTER_SUCCESS;
    } else
    {
        retVal = DSP_FILTER_INIT_FAULT; 
    }
    
    return retVal;
}

/*!*****************************************************************************
* @function movingAvgInit
* 
* @brief Apply moving average to data
*
* @param filter - pointer to moving average filter structure
* @param initData - initialisation data
*
* @return void function
*******************************************************************************/
inline void movingAverage(moving_avg_pt filter)
{
    mov_avg_buffer_samples_t i;

    for (i=0; i < filter->bufferSamples; i++)
    {
        /*add new data sample to sum and sliding window substract oldest data sample */
        filter->sum = filter->sum + filter->inputData[i] - filter->avgWinData[filter->avgWinPointer];
        filter->avgWinData[filter->avgWinPointer] = filter->inputData[i];

        /* increment sliding window position pointer */
        filter->avgWinPointer++;
        
        /* sliding window circular pointer arithmetics - pointer reset */
        if(filter->avgWinPointer == filter->avgWinSamples)
        {
            filter->avgWinPointer = 0;
        }
        
        /*calculate output sample */
        filter->outputData[i] = filter->sum * filter->coefficient;
    }
}

/*!*****************************************************************************
* @function invertSignal
* 
* @brief Invert data from origin
*
* @param inputSignal - input data
* @param origin - inversion origin
*
* @return inverted data
*******************************************************************************/
inline dsp_data_t invertSignal(dsp_data_t inputSignal, dsp_data_t origin)
{
    return (origin - inputSignal);
}

/******************************************************************************
**                               End Of File
*******************************************************************************/
