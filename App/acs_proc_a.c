/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file acs_proc.c
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2020-01-08
 * 
 * @brief   - redundant - asymmetric processor A implementation 
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

#include "acs_proc_a.h"
#include "acs_processing_common.h"
#include "acs_communications.h"
#include "fletcher.h"
#include "logger.h"
#include "dsp_filters_lib.h"
#include <stdio.h>

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/*IIR average filter static pointers to static data objects*/
static recursive_avg_t *iirAvgFilterA_gp;
static recursive_avg_t *iirAvgFilterB_gp;

/*******************************************************************************
**                                 Code
*******************************************************************************/

/*!*****************************************************************************
* @function procInitCodeA
* 
* @brief modeled position sensor processor A initialisation function  
*
* @param processor ID - concurent thread ID
*
* @return void function
*******************************************************************************/

void procInitCodeA(uint8_t procID)
{
    /*IIR average filters static data objects and initialisation*/
    static iir_avg_data_t iirFilterInBuffA;
    static iir_avg_data_t iirFilterOutBuffA;
    static iir_avg_data_t iirFilterInBuffB;
    static iir_avg_data_t iirFilterOutBuffB;
    tst_data_attributes_t dataAttributes;
    
    static recursive_avg_t iirAvgFilterA =
    {
        .coefficient = IIR_AVG_COEFFICIENT,
        .bufferSamples = FILTER_BUFFERING_SAMPLES,
        .inputData = &iirFilterInBuffA,
        .outputData = &iirFilterOutBuffA
    };

    static recursive_avg_t iirAvgFilterB = 
    {
        .coefficient = IIR_AVG_COEFFICIENT,
        .bufferSamples = FILTER_BUFFERING_SAMPLES,
        .inputData = &iirFilterInBuffB,
        .outputData = &iirFilterOutBuffB
    };
    
    /*read test mockup meta data and parameters*/
    getTestDataAttributes(&dataAttributes);    
    
    logTest("ProcA test");

    /* set initial filter conditions */
    iirAvgFilterA.lastSample = dataAttributes.guardRegion;
    iirAvgFilterA_gp = &iirAvgFilterA;
    
    iirAvgFilterB.lastSample = dataAttributes.resolution - dataAttributes.guardRegion;
    iirAvgFilterB_gp = &iirAvgFilterB;
    
    printf("Processor ID:%d init OK...\n", procID);
    
}

/*!*****************************************************************************
* @function processorCodeA
* 
* @brief processor A implementation code
*
* @param inputData  - pointer to input data samples (ADC mock data) from orthogonal sensors
* @param outputData - pointer to processed output samples 
*                   
* @return void function
*******************************************************************************/

void processorCodeA(input_data_pt inputData, com_data_pt outputData) 
{
    processing_state_e state;
    dsp_data_t processedSignalA, processedSignalB, invertedSignalB;
    dsp_data_t processedSignal;
    tst_data_attributes_t dataAttributes;
    acs_flags_t flags;
    bool signalAsymmetry;
    
    /* get test meta data*/
    getTestDataAttributes(&dataAttributes);
    
    /*load data samples to filter*/
    *iirAvgFilterA_gp->inputData = (iir_avg_data_t)inputData->sensorSampleA;
    *iirAvgFilterB_gp->inputData = (iir_avg_data_t)inputData->sensorSampleB;
    
    /*Filter signals A and B*/
    recursiveAverage(iirAvgFilterA_gp);
    processedSignalA = *iirAvgFilterA_gp->outputData;
    recursiveAverage(iirAvgFilterB_gp);
    processedSignalB = *iirAvgFilterB_gp->outputData;
    
    /*Check constraints for signals A and B*/
    flags = checkSignalConstraints(processedSignalA, processedSignalB, &dataAttributes);
    
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
    
    /* check input sensor signals complementarity */
    signalAsymmetry = !checkComplementarity(processedSignalA, processedSignalB, &dataAttributes);
    
    if(signalAsymmetry && (PROC_FSM_SIGNAL_HEALTHY == state))
    {
        state = PROC_FSM_COMPLEMENTARITY_FAIL;
    }
    
    /* input signal conditional processing multiplexer */
    switch (state)
    {
        case PROC_FSM_SIGNAL_HEALTHY:
            invertedSignalB = invertSignal(processedSignalB, (dsp_data_t)dataAttributes.resolution); /*invert signal B*/
            processedSignal = (int)(processedSignalA + invertedSignalB) >> 1;   /*average signals*/
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
        
        case PROC_FSM_SIGNAL_B_FAIL:
            processedSignal = processedSignalA; /*set signal A as valid signal*/
            
            if(signalAsymmetry) /*check asymmetry*/
            {
                flags |= ACS_SIGNALS_ASYMMETRY;
            }
            break;
        
        case PROC_FSM_COMPLEMENTARITY_FAIL:
            invertedSignalB = invertSignal(processedSignalB, (dsp_data_t)dataAttributes.resolution); /*invert signal B*/
            processedSignal = (int)(processedSignalA + invertedSignalB) >> 1;   /*average signals*/        
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

    /* write processor A debug data */
    outputData->channelDebug[DEBUG_CHANNEL_A] = processedSignalA;
    outputData->channelDebug[DEBUG_CHANNEL_B] = processedSignalB;
    
    /* write processed data and processing flags */
    outputData->dataSample = processedSignal;
    outputData->flags = flags;
    
    /* pack communication data */
    packComData(outputData);
}

/******************************************************************************
**                               End Of File
*******************************************************************************/
