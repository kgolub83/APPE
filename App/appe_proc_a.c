/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file appe_proc.c
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

#include "appe_proc_a.h"
#include "appe_processing_common.h"
#include "appe_communications.h"
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

    /* set initial filter conditions */
    iirAvgFilterA.lastSample = dataAttributes.guardRegion;
    iirAvgFilterA_gp = &iirAvgFilterA;
    
    iirAvgFilterB.lastSample = dataAttributes.resolution - dataAttributes.guardRegion;
    iirAvgFilterB_gp = &iirAvgFilterB;
    
    LogFull_m(SYS_INIT, "Processor A init OK", 1, procID);
    
}

/*!*****************************************************************************
* @function processorCodeA
* 
* @brief processor A implementation code
*
* @param inputData  - pointer to input data samples (ADC mock data) from orthogonal sensors
* @param comData - pointer to processed output samples 
*                   
* @return void function
*******************************************************************************/
void processorCodeA(input_data_pt inputData, com_channel_pt comFrame) 
{
    processing_state_e state;
    com_data_t comData;
    dsp_data_t processedSignalA, processedSignalB, invertedSignalB;
    dsp_data_t processedSignal;
    tst_data_attributes_t dataAttributes;
    appe_flags_t flags;
    static sequence_no_t sequenceNo;
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
        
        case PROC_FSM_SIGNAL_B_FAIL:
            processedSignal = processedSignalA; /*set signal A as valid signal*/
            
            if(signalAsymmetry) /*check asymmetry*/
            {
                flags |= APPE_SIGNALS_ASYMMETRY;
            }
            break;
        
        case PROC_FSM_COMPLEMENTARITY_FAIL:
            invertedSignalB = invertSignal(processedSignalB, (dsp_data_t)dataAttributes.resolution); /*invert signal B*/
            processedSignal = (int)(processedSignalA + invertedSignalB) >> 1;   /*average signals*/        
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
    
    /* write processor A debug data */
    comData.channelDebug[DEBUG_CHANNEL_A] = processedSignalA;
    comData.channelDebug[DEBUG_CHANNEL_B] = processedSignalB;
    
    /* write processed data and processing flags */
    comData.dataSample = processedSignal;
    comData.flags = flags;
    
     /* circular sequence number counter insertion */
    comData.seqNo = sequenceNo++;
    
    /* add message data identifier */
    comData.dataID = APPE_MESAGE_ID_COM_PROC_A;
    
    /* pack communication data */
    packComData(&comData, comFrame);
}

/******************************************************************************
**                               End Of File
*******************************************************************************/
