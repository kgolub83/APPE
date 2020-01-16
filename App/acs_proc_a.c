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

#include "acs_proc_a.h"
#include "acs_processing_common.h"
#include "dsp_filters_lib.h"
#include "acs_communications.h"
#include <stdio.h>

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/*IIR average filters static data objects and initialisation*/
static iir_avg_data_t iirFilterInBuffA;
static iir_avg_data_t iirFilterOutBuffA;
static iir_avg_data_t iirFilterInBuffB;
static iir_avg_data_t iirFilterOutBuffB;

static recoursive_avg_t iirAvgFilterA_g =
{
    .coeficient = IIR_AVG_COEFICIENT,
    .bufferSamples = FILTER_BUFFERING_SAMPLES,
    .inputData = &iirFilterInBuffA,
    .outputData = &iirFilterOutBuffA,
    .lastSample = IIR_INIT_VALUE_A
};

static recoursive_avg_t iirAvgFilterB_g = 
{
    .coeficient = IIR_AVG_COEFICIENT,
    .bufferSamples = FILTER_BUFFERING_SAMPLES,
    .inputData = &iirFilterInBuffB,
    .outputData = &iirFilterOutBuffB,
    .lastSample = IIR_INIT_VALUE_B
};

/*IIR average filter static const pointers to static data objects*/
static recoursive_avg_pt iirAvgFilterA_gp = &iirAvgFilterA_g;
static recoursive_avg_pt iirAvgFilterB_gp = &iirAvgFilterB_g;

/*******************************************************************************
**                                 Code
*******************************************************************************/

void initProcA(void)
{

}

/*!*****************************************************************************
* @function 
* 
* @brief 
*
* @param 
*
* @returns 
*******************************************************************************/

void processorCodeA(input_data_pt inputData, com_data_pt outputData) 
{
    processing_state_e state;
    dsp_data_t processedSignalA, processedSignalB, invertedSignalB;
    dsp_data_t processedSignal;
    
    /*load data samples to filter*/
    *iirAvgFilterA_g.inputData = (iir_avg_data_t)inputData->sensorSampleA;
    *iirAvgFilterB_g.inputData = (iir_avg_data_t)inputData->sensorSampleB;
    
    /*Filter signals A and B*/
    recoursiveAverage(iirAvgFilterA_gp);
    processedSignalA = *iirAvgFilterA_g.outputData;
    recoursiveAverage(iirAvgFilterB_gp);
    processedSignalB = *iirAvgFilterB_g.outputData;
    
    /*Check constraints for signals A and B*/
    checkSignalIntegrity();
    checkComplementarity();
    
    state = PROC_FSM_SIGNAL_HEALTHY;
    
    switch (state)
    {
        case PROC_FSM_SIGNAL_HEALTHY:
            invertedSignalB = invertSignal(processedSignalB, (dsp_data_t)ADC_RANGE); /*invert signal B*/
            processedSignal = (processedSignalA + invertedSignalB)/2.0;   /*average signals*/
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
    printf("%f ", processedSignal);
}

/******************************************************************************
**                               End Of File
*******************************************************************************/
