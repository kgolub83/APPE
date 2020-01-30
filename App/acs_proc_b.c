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

#include "acs_proc_b.h"
#include "acs_processing_common.h"
#include "acs_communications.h"
#include "logger.h"
#include <stdio.h>

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/* moving average filter static pointers to static data objects*/
static moving_avg_t *movAvgFilterA_gp;
static moving_avg_t *movAvgFilterB_gp;

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
void procInitCodeB(uint8_t procID)
{
    /*Moving average filters static data objects and initialisation*/
    static mov_avg_data_t movAvgInBuffA;
    static mov_avg_data_t movAvgWindowA[MOV_AVG_WIN_SAMPLES];
    static mov_avg_data_t movAvgOutBuffA;

    static mov_avg_data_t movAvgInBuffB;
    static mov_avg_data_t movAvgWindowB[MOV_AVG_WIN_SAMPLES];
    static mov_avg_data_t movAvgOutBuffB;
    tst_data_attributes_t dataAttributes;
    
    static moving_avg_t movAvgFilterA =
    {
        .avgWinData = movAvgWindowA,
        .bufferSamples = FILTER_BUFFERING_SAMPLES,
        .inputData = &movAvgInBuffA,
        .outputData = &movAvgOutBuffA,
        .avgWinSamples = MOV_AVG_WIN_SAMPLES,
    };
    
    static moving_avg_t movAvgFilterB =
    {
        .avgWinData = movAvgWindowB,
        .bufferSamples = FILTER_BUFFERING_SAMPLES,
        .inputData = &movAvgInBuffB,
        .outputData = &movAvgOutBuffB,
        .avgWinSamples = MOV_AVG_WIN_SAMPLES,
    };

    getTestDataAttributes(&dataAttributes);
    
    logTest("ProcB test");
    
    /*Set filter global pointer and initialises filter*/
    movAvgFilterA_gp = &movAvgFilterA;
    movingAvgInit(movAvgFilterA_gp, dataAttributes.guardRegion);
    
    /*Set filter global pointer and initialises filter*/
    movAvgFilterB_gp = &movAvgFilterB;
    movingAvgInit(movAvgFilterB_gp, dataAttributes.resolution - dataAttributes.guardRegion);
    
    printf("Processor ID:%d init OK...\n", procID);
}

void processorCodeB(input_data_pt inputData, com_data_pt outputData) 
{
    processing_state_e state;
    dsp_data_t processedSignalA, processedSignalB, invertedSignalA;
    dsp_data_t processedSignal;
    tst_data_attributes_t dataAttributes;
       acs_flags_t flags;
    bool signalAsymmetry;
    
    getTestDataAttributes(&dataAttributes);   
    
    /*load data samples to filter*/
    *movAvgFilterA_gp->inputData = (mov_avg_data_t)inputData->sensorSampleA;
    *movAvgFilterB_gp->inputData = (mov_avg_data_t)inputData->sensorSampleB;
    
    /*Filter signals A and B*/
    movingAverage(movAvgFilterA_gp);
    processedSignalA = *movAvgFilterA_gp->outputData;
    movingAverage(movAvgFilterB_gp);
    processedSignalB = *movAvgFilterB_gp->outputData;
    
    /*Check constraints for signals A and B*/
    flags = checkSignalConstraints(processedSignalA, processedSignalB, &dataAttributes);
    
    state = PROC_FSM_SIGNAL_HEALTHY;
    
    if(ACS_SYSTEM_OK == flags)  /*check if everithing OK*/
    {
        state = PROC_FSM_SIGNAL_HEALTHY; 
    } else if (!(flags & ACS_SENS_A_FAULTS_MASK)) /*check if signal A OK*/
    {
        state = PROC_FSM_SIGNAL_B_FAIL; 
    } else if (!(flags & ACS_SENS_B_FAULTS_MASK)) /*check if signal B OK*/
    {
        state = PROC_FSM_SIGNAL_A_FAIL;
    } else
    {
        state = PROC_FSM_SIGNAL_FAIL;
    }
    
    signalAsymmetry = !checkComplementarity(processedSignalA, processedSignalB, &dataAttributes);
    
    if(signalAsymmetry && PROC_FSM_SIGNAL_HEALTHY == state)
    {
        state = PROC_FSM_COMPLEMENTARITY_FAIL;
    }
    
    switch (state)
    {
        case PROC_FSM_SIGNAL_HEALTHY:
            invertedSignalA = invertSignal(processedSignalA, (dsp_data_t)dataAttributes.resolution);    /*invert signal A*/
            processedSignal = (invertedSignalA + processedSignalB)*0.5;                                 /*average signals*/
            processedSignal = invertSignal(processedSignal, (dsp_data_t)dataAttributes.resolution);     /*invert resulting signal*/
            flags = ACS_SYSTEM_OK;
            break;
        
        case PROC_FSM_SIGNAL_A_FAIL:
            /*Invert signal B and use as output signal*/
            processedSignal = invertSignal(processedSignalB, (dsp_data_t)dataAttributes.resolution);
            
            if(signalAsymmetry) /*check asymmetry*/
            {
                flags |= ACS_SIGNALS_ASYMMETRY;
            } 
            break;
        break;
        
        case PROC_FSM_SIGNAL_B_FAIL:
            processedSignal = processedSignalA; /*set signal A as valid signal*/
            
            if(signalAsymmetry) /*check asymmetry*/
            {
                flags |= ACS_SIGNALS_ASYMMETRY;
            }
            break;
        break;
        
        case PROC_FSM_COMPLEMENTARITY_FAIL:
            invertedSignalA = invertSignal(processedSignalA, (dsp_data_t)dataAttributes.resolution);    /*invert signal A*/
            processedSignal = (invertedSignalA + processedSignalB)*0.5;                                 /*average signals*/
            processedSignal = invertSignal(processedSignal, (dsp_data_t)dataAttributes.resolution);     /*invert resulting signal*/
            flags |= ACS_SIGNALS_ASYMMETRY;
            break;
        
        case PROC_FSM_SIGNAL_FAIL:
            processedSignal = 0;
            flags |= ACS_SYSTEM_FAULT;
            break;
        
        default:
            processedSignal = 0;
            flags |= ACS_SYSTEM_FAULT;
    }

    outputData->channelDebug[DEBUG_CHANNEL_A] = processedSignalA;
    outputData->channelDebug[DEBUG_CHANNEL_B] = processedSignalB;
    
    outputData->dataSample = processedSignal;
    outputData->flags = flags;
    
    packComData();
}

/******************************************************************************
**                               End Of File
*******************************************************************************/
