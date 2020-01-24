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

#include "dsp_filters_lib.h"

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
inline void recoursiveAverage(recoursive_avg_pt filter)
{
    iir_avg_buffer_samples_t i;

    for (i=0; i < filter->bufferSamples; i++)
    {
      filter->outputData[i] = filter->coeficient*(filter->inputData[i] - filter->lastSample) + filter->lastSample;
      filter->lastSample = filter->outputData[i];
    }
}

/*!********************************************************
* @function movingAvgInit
* @brief Initializes moving average filter
*
* Initializes movingAvg_t structure
*
* @param *movingAvgFilter pointer to moving average
*         filter structure
* @returns void function
**********************************************************/
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
        filter->coeficient = 1/(float)filter->avgWinSamples;
        retVal = DSP_FILTER_SUCCESS;
    } else
    {
        retVal = DSP_FILTER_INIT_FAULT; 
    }
    
    return retVal;
}

/*!********************************************************
* @function movingAvgFilter
* @brief Apply moving average filter
*
* Apply moving average filter to array of data. Results
* are stored in output array
*
* @param *movingAvgFilter pointer to moving average
*                         filter structure
* @returns void function
**********************************************************/
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
        filter->outputData[i] = filter->sum * filter->coeficient;
    }
}

inline dsp_data_t invertSignal(dsp_data_t inputSignal, dsp_data_t origin)
{
    return (origin - inputSignal);
}

/******************************************************************************
**                               End Of File
*******************************************************************************/

