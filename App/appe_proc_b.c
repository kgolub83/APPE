/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file appe_proc.h
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2020-01-08
 * 
 * @brief   - redundant - asymmetric processor B implementation 
 *          - one of two acceleration pedal position encoder processing models 
 *          - procesing, filtering and domain checking of two orthogonal position 
 *          sensor signals  
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

#include "appe_proc_b.h"
#include "appe_processing_common.h"
#include "appe_communications.h"
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
* @function procInitCodeB
* 
* @brief modeled position sensor processor B initialisation function  
*
* @param processor ID - concurent thread ID
*
* @return void function
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
    
    /*Set filter global pointer and initialises filter*/
    movAvgFilterA_gp = &movAvgFilterA;
    movingAvgInit(movAvgFilterA_gp, dataAttributes.guardRegion);
    
    /*Set filter global pointer and initialises filter*/
    movAvgFilterB_gp = &movAvgFilterB;
    movingAvgInit(movAvgFilterB_gp, dataAttributes.resolution - dataAttributes.guardRegion);
    
    LogFull_m(SYS_INIT, "Processor B init OK", 1, procID);
}

/*!*****************************************************************************
* @function processorCodeB
* 
* @brief processor B implementation code
*
* @param inputData  - pointer to input data samples (ADC mock data) from orthogonal sensors
* @param comData - pointer to processed output samples 
*                   
* @return void function
*******************************************************************************/
void processorCodeB(input_data_pt inputData, com_channel_pt comFrame) 
{
    processing_state_e state;
    com_data_t comData;
    dsp_data_t processedSignalA, processedSignalB, invertedSignalA;
    dsp_data_t processedSignal;
    tst_data_attributes_t dataAttributes;
    appe_flags_t flags;
    static sequence_no_t sequenceNo;
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
    
    if(APPE_SYSTEM_OK == flags)  /*check if everithing OK*/
    {
        state = PROC_FSM_SIGNAL_HEALTHY; 
    } else if (!(flags & APPE_SENS_A_FLAGS_MASK)) /*check if signal A OK*/
    {
        state = PROC_FSM_SIGNAL_B_FAIL; 
    } else if (!(flags & APPE_SENS_B_FLAGS_MASK)) /*check if signal B OK*/
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
            flags = APPE_SYSTEM_OK;
            break;
        
        case PROC_FSM_SIGNAL_A_FAIL:
            /*Invert signal B and use as output signal*/
            processedSignal = invertSignal(processedSignalB, (dsp_data_t)dataAttributes.resolution);
            
            if(signalAsymmetry) /*check asymmetry*/
            {
                flags |= APPE_SIGNALS_ASYMMETRY;
            } 
            break;
        break;
        
        case PROC_FSM_SIGNAL_B_FAIL:
            processedSignal = processedSignalA; /*set signal A as valid signal*/
            
            if(signalAsymmetry) /*check asymmetry*/
            {
                flags |= APPE_SIGNALS_ASYMMETRY;
            }
            break;
        break;
        
        case PROC_FSM_COMPLEMENTARITY_FAIL:
            invertedSignalA = invertSignal(processedSignalA, (dsp_data_t)dataAttributes.resolution);    /*invert signal A*/
            processedSignal = (invertedSignalA + processedSignalB)*0.5;                                 /*average signals*/
            processedSignal = invertSignal(processedSignal, (dsp_data_t)dataAttributes.resolution);     /*invert resulting signal*/
            flags |= APPE_SIGNALS_ASYMMETRY;
            break;
        
        case PROC_FSM_SIGNAL_FAIL:
            processedSignal = 0;
            flags |= APPE_SYSTEM_FAULT;
            break;
        
        default:
            processedSignal = 0;
            flags |= APPE_SYSTEM_FAULT;
    }

    comData.channelDebug[DEBUG_CHANNEL_A] = processedSignalA;
    comData.channelDebug[DEBUG_CHANNEL_B] = processedSignalB;
    
    comData.dataSample = processedSignal;
    comData.flags = flags;
    
    /* circular sequence number counter insertion */
    comData.seqNo = sequenceNo++;
    
    /* add message data identifier */
    comData.dataID = APPE_MESAGE_ID_COM_PROC_B;
    
    packComData(&comData, comFrame);
}

/******************************************************************************
**                               End Of File
*******************************************************************************/
