/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file c_wrapper.c
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2020-01-08
 * 
 * @brief wrapper for c functions and specific user implementation  
 *
 * @version
 *
 * @section REVISION HISTORY
 *  - KG 2020-01-08 Initial implementation 
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

/* holds input tetst data settings */
static tst_data_attributes_t testDataAtributes; 

/*******************************************************************************
**                                 Code
*******************************************************************************/

void setTestDataAttributes(tst_data_attributes_pt attributes)
{
    testDataAtributes.decoderRange = attributes->decoderRange;
    testDataAtributes.guardRegion = attributes->guardRegion;
    testDataAtributes.resolution = attributes->resolution;
    testDataAtributes.sampleRate = attributes->sampleRate;
    testDataAtributes.samplesNo = attributes->samplesNo;
}

void getTestDataAttributes(tst_data_attributes_pt attributes)
{
    attributes->decoderRange = testDataAtributes.decoderRange;
    attributes->guardRegion = testDataAtributes.guardRegion;
    attributes->resolution = testDataAtributes.resolution ;
    attributes->sampleRate = testDataAtributes.sampleRate; 
    attributes->samplesNo = testDataAtributes.samplesNo;
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

static inline void testInitCode(uint8_t id)
{
    printf("Processor %d initialised!", id);
}

static inline void testProcCode(input_data_pt inputData, com_data_pt comData)
{
    int resultSample;
    int inputDataRange;
    
    inputDataRange = testDataAtributes.resolution - 2*testDataAtributes.guardRegion;
    
    comData->seqNo = inputData->sampleNo;
    resultSample = ((int)(inputData->sensorSampleA - inputData->sensorSampleB + inputDataRange)/2)+testDataAtributes.guardRegion;
    comData->dataSample = (sample_data_t)resultSample;
    comData->time = clock();    
}

static inline void tetsSupervisorCode(com_data_pt comDataProcA, com_data_pt comDataProcB, output_data_pt outputData)
{
    uint32_t acsOutput;
    
    if(comDataProcA->dataSample == comDataProcB->dataSample)
    {
        acsOutput = acsDecodingLUT[comDataProcA->dataSample];
        printf(" |*A: %d B: %d O: %d*| ", comDataProcA->dataSample, comDataProcB->dataSample, acsOutput);
    } else
    {
        printf("\n\n###SAMPLE FAULT###\n\n");
    }
    
    outputData->output = acsOutput;
    outputData->state = ACS_ACTIVE;
}

static inline void testExitCode(uint8_t id)
{
    printf("User process %d exited sucessfully!", id);
}

/**** Function place holders for user implementation ****/
__weak void procInitCodeA(uint8_t procID)
{
    testInitCode(procID);
}

__weak void processorCodeA(input_data_pt inputData, com_data_pt comData) 
{
    testProcCode(inputData, comData);
}

__weak void processorExitRoutineA(uint8_t procID)
{
    testInitCode(procID);
}

__weak void processorCodeB(input_data_pt inputData, com_data_pt comData) 
{
    testProcCode(inputData, comData);
}

__weak void procInitCodeB(uint8_t procID)
{
    testInitCode(procID);
}

__weak void processorExitRoutineB(uint8_t procID)
{
    testInitCode(procID);
}

__weak void supervisorCode(com_data_pt comDataProcA, com_data_pt comDataProcB, output_data_pt outputData)
{
    tetsSupervisorCode(comDataProcA, comDataProcB, outputData);
}

__weak void supervisorInitCode(uint8_t procID)
{
    testInitCode(procID);
}

__weak void supervisorExitRoutine(uint8_t procID)
{
    testInitCode(procID);
}

/******************************************************************************
**                               End Of File
*******************************************************************************/
