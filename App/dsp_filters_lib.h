/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file dsp_filters_lib.h
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2020-01-08
 * 
 * @brief signal procesing digital filters implementations
 *
 * @version 0.1
 *
 * @section REVISION HISTORY
 *  - 0.1 KG 2020-01-08 Initial implementation 
 *
 ******************************************************************************/

/*******************************************************************************
**                                 Guards
*******************************************************************************/

#ifndef FILTER_LIB_H
#define FILTER_LIB_H

#ifdef __cplusplus
extern "C"
{
#endif /* extern */

/*******************************************************************************
**                                Includes
*******************************************************************************/

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
**                               Constants
*******************************************************************************/  

/*******************************************************************************
**                                Macros
*******************************************************************************/

/*******************************************************************************
**                            Data Structures
*******************************************************************************/

/*recursive averageing filter data types definitions*/
typedef float iir_avg_data_t;
typedef uint16_t iir_avg_buffer_samples_t;

/*FIR move average filter data types definitions*/
typedef float mov_avg_data_t;
typedef uint16_t mov_avg_buffer_samples_t;
typedef uint16_t mov_avg_window_samples_t;

/*general DSP data type definitions*/
typedef float dsp_data_t;

typedef enum
{
    DSP_FILTER_SUCCESS,
    DSP_FILTER_INIT_FAULT,
    DSP_FILTER_PROCESSING_FAULT
} dsp_filter_return_e;

typedef struct
{
    iir_avg_data_t * const inputData;
    iir_avg_data_t * const outputData;
    const iir_avg_buffer_samples_t bufferSamples;
    
    iir_avg_data_t coefficient;
    iir_avg_data_t lastSample;
} recursive_avg_t;

typedef recursive_avg_t * const recursive_avg_pt;     /*const pointer to recursive_avg_t structure*/

typedef struct
{
    mov_avg_data_t * const inputData;
    mov_avg_data_t * const outputData;
    const mov_avg_buffer_samples_t bufferSamples;
    
    mov_avg_data_t * const avgWinData;
    const mov_avg_window_samples_t avgWinSamples;
    mov_avg_data_t coefficient;
    mov_avg_window_samples_t avgWinPointer;
    
    mov_avg_data_t sum;
} moving_avg_t;

typedef moving_avg_t * const moving_avg_pt;     /*const pointer to moving_avg_t structure*/

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/*******************************************************************************
**                     Public function prototypes - API
*******************************************************************************/

extern void recursiveAverage(recursive_avg_t * const filter);
extern void movingAverage(moving_avg_pt filter);
extern dsp_filter_return_e movingAvgInit(moving_avg_pt filter, mov_avg_data_t initData);

extern dsp_data_t invertSignal(dsp_data_t inputSignal, dsp_data_t origin);

#ifdef __cplusplus
} /* extern "C" */
#endif /* extern "C" */

#endif /* FILTER_LIB_H */

/******************************************************************************
**                               End Of File
*******************************************************************************/
