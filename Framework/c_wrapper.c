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
#include <string.h>

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

static inline void testProcCode(input_data_pt inputData, com_channel_pt comFrame)
{
    int sample;
    sample_data_t resultSample;
    int inputDataRange;
    uint8_t index, dataSize;
    
    inputDataRange = testDataAtributes.resolution - 2*testDataAtributes.guardRegion;
    
    //comFrame->seqNo = inputData->sampleNo;
    dataSize = sizeof(sequence_no_t); 
    if(dataSize < ACS_COM_CHANNEL_BYTES)
    {    
        memcpy(comFrame->comChannelData, &inputData->sampleNo, dataSize);
    } else
    {
        printf("#ERR: Processor com data test fail!");
    }
    index = dataSize;
    
    sample = ((int)(inputData->sensorSampleA - inputData->sensorSampleB + inputDataRange)/2)+testDataAtributes.guardRegion;
    resultSample = (sample_data_t)sample;

   //comFrame->dataSample = (sample_data_t)resultSample;    
    dataSize = sizeof(sample_data_t);
    if((index+dataSize) < ACS_COM_CHANNEL_BYTES)
    {
        memcpy(comFrame->comChannelData+index, &resultSample, dataSize);
    } else
    {
        printf("#ERR: Processor com data test fail!");
    }
}

static inline void testSupervisorCode(com_channel_pt comFrameProcA, com_channel_pt comFrameProcB, output_data_pt outputData)
{
    uint32_t acsOutput;
    sequence_no_t seqNoA, seqNoB;
    sample_data_t sampleA, sampleB;
    uint8_t dataSize, index;
    
    dataSize = sizeof(sequence_no_t);
    memcpy(&seqNoA, comFrameProcA->comChannelData, dataSize);
    memcpy(&seqNoB, comFrameProcB->comChannelData, dataSize);
    index = dataSize;
    
    dataSize = sizeof(sample_data_t);
    memcpy(&sampleA, comFrameProcA->comChannelData+index, dataSize);
    memcpy(&sampleB, comFrameProcA->comChannelData+index, dataSize);
    
    if(sampleA == sampleB && seqNoA == seqNoB)
    {
        acsOutput = acsDecodingLUT[sampleA];
        printf(" |*N: %d S: %d O: %d*| ", seqNoA, sampleA, acsOutput);
    } else
    {
        printf("#ERR: SAMPLE FAULT\n");
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

__weak void processorCodeA(input_data_pt inputData, com_channel_pt comFrame) 
{
    testProcCode(inputData, comFrame);
}

__weak void processorExitRoutineA(uint8_t procID)
{
    testInitCode(procID);
}

__weak void processorCodeB(input_data_pt inputData, com_channel_pt comFrame) 
{
    testProcCode(inputData, comFrame);
}

__weak void procInitCodeB(uint8_t procID)
{
    testInitCode(procID);
}

__weak void processorExitRoutineB(uint8_t procID)
{
    testInitCode(procID);
}

__weak void supervisorCode(com_channel_pt comFrameProcA, com_channel_pt comFrameProcB, output_data_pt outputData)
{
    testSupervisorCode(comFrameProcA, comFrameProcB, outputData);
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
