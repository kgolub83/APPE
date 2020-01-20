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
#include "dsp_filters_lib.h"
#include "acs_communications.h"
#include <stdio.h>

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

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
* @returns 
*******************************************************************************/
void procInitCodeB(uint8_t id)
{
    /*IIR average filters static data objects and initialisation*/
    static mov_avg_data_t movAvgInBuffA;
    static mov_avg_data_t movAvgWindowA[MOV_AVG_WIN_SAMPLES];
    static mov_avg_data_t movAvgOutBuffA;

    static mov_avg_data_t movAvgInBuffB;
    static mov_avg_data_t movAvgWindowB[MOV_AVG_WIN_SAMPLES];
    static mov_avg_data_t movAvgOutBuffB;

    static moving_avg_t movAvgFilterA =
    {
        .avgWinData = movAvgWindowA,
        .bufferSamples = FILTER_BUFFERING_SAMPLES,
        .inputData = &movAvgInBuffA,
        .outputData = &movAvgOutBuffA,
        .avgWinSamples = MOV_AVG_WIN_SAMPLES,
    };
    
    movAvgFilterA_gp = &movAvgFilterA;
    movingAvgInit(movAvgFilterA_gp, FILTER_INIT_VALUE_A);

    static moving_avg_t movAvgFilterB =
    {
        .avgWinData = movAvgWindowB,
        .bufferSamples = FILTER_BUFFERING_SAMPLES,
        .inputData = &movAvgInBuffB,
        .outputData = &movAvgOutBuffB,
        .avgWinSamples = MOV_AVG_WIN_SAMPLES,
    };
    
    movAvgFilterB_gp = &movAvgFilterB;
    movingAvgInit(movAvgFilterB_gp, FILTER_INIT_VALUE_B);
}

void processorCodeB(input_data_pt inputData, com_data_pt outputData) 
{
    processing_state_e state;
    dsp_data_t processedSignalA, processedSignalB, invertedSignalB;
    dsp_data_t processedSignal;
    
    /*load data samples to filter*/
    *movAvgFilterA_gp->inputData = (mov_avg_data_t)inputData->sensorSampleA;
    *movAvgFilterB_gp->inputData = (mov_avg_data_t)inputData->sensorSampleB;
    
    /*Filter signals A and B*/
    movingAverage(movAvgFilterA_gp);
    processedSignalA = *movAvgFilterA_gp->outputData;
    movingAverage(movAvgFilterB_gp);
    processedSignalB = *movAvgFilterB_gp->outputData;
    
    /*Check constraints for signals A and B*/
    checkSignalIntegrity();
    checkComplementarity();
    
    state = PROC_FSM_SIGNAL_HEALTHY;
    
    switch (state)
    {
        case PROC_FSM_SIGNAL_HEALTHY:
            invertedSignalB = invertSignal(processedSignalB, (dsp_data_t)ADC_RANGE); /*invert signal B*/
            processedSignal = (processedSignalA + invertedSignalB)*0.5;   /*average signals*/
            //set flags
        break;
        
        case PROC_FSM_SIGNAL_A_FAIL:
            //integrate_error
            //invert signal A
            //set flags
        break;
        
        case PROC_FSM_SIGNAL_B_FAIL:
            //integrate_error
            //invert signal A
            //set flags
        break;
        
        case PROC_FSM_COMPLEMENTARITY_WAR:
            //integrate_error
            //invert signal A
            //set flags
        break;
        
        case PROC_FSM_COMPLEMENTARITY_FAIL:
            //set flags
        break;
        
        case PROC_FSM_SIGNAL_FAIL:
            //set flags
        break;
        
        default:
            break;
    }

    packComData();
    
    outputData->dataSample = processedSignal;
}

/******************************************************************************
**                               End Of File
*******************************************************************************/
