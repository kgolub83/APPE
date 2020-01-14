/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file c_wrapper.c
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2019-12-20
 * 
 * @brief wrapper for c functions  
 *
 * @version
 *
 * @section REVISION HISTORY
 *  - KG 2019-12-20 Initial implementation 
 *
 ******************************************************************************/

/*******************************************************************************
**                                Includes
*******************************************************************************/   
#include "c_wrapper.h"
#include "acs_decoder.h"
#include <stdio.h> 
#include <stdint.h>
#include <time.h>

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

__weak void processorCode(input_data_pt inputData, com_data_pt outputData) 
{
    int resultSample;
    
    outputData->seqNo = inputData->sampleNo;
    resultSample = ((int)(inputData->sensorSampleA - inputData->sensorSampleB + 818)/2)+102;
    outputData->dataSample = (sample_data_t)resultSample;
    outputData->time = clock();
    
    //printf("|*%d: %d %d %d *|", inputData->sampleNo, inputData->sensorSamlpleA, inputData->sensorSampleB, outputData->dataSample);
}

__weak void supervisorCode(com_data_pt comDataProcA, com_data_pt comDataProcB, output_data_pt outputData)
{
    uint32_t acsOutput;
    
    acsOutput = acsDecodingLUT[comDataProcA->dataSample];
    printf(" |*A: %d B: %d O: %d*| ", comDataProcA->dataSample, comDataProcB->dataSample, acsOutput);
    
    outputData->output = acsOutput;
    outputData->state = ACS_ACTIVE;
}

/******************************************************************************
**                               End Of File
*******************************************************************************/
