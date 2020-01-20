/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file .c
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2019-12-20
 * 
 * @brief 
 *
 * @version
 *
 * @section REVISION HISTORY
 *  - KG 2019-12-20 Initial implementation 
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
#endif // nextern "C"

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

/*recoursive averageing filter data types definitions*/
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
    
    iir_avg_data_t coeficient;
    iir_avg_data_t lastSample;
} recoursive_avg_t;

typedef recoursive_avg_t * const recoursive_avg_pt;     /*const pointer to recoursive_avg_t structure*/

typedef struct
{
    mov_avg_data_t * const inputData;
    mov_avg_data_t * const outputData;
    const mov_avg_buffer_samples_t bufferSamples;
    
    mov_avg_data_t * const avgWinData;
    const mov_avg_window_samples_t avgWinSamples;
    mov_avg_data_t coeficient;
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

extern void recoursiveAverage(recoursive_avg_t * const filter);
extern void movingAverage(moving_avg_pt filter);
extern dsp_filter_return_e movingAvgInit(moving_avg_pt filter, mov_avg_data_t initData);

extern dsp_data_t invertSignal(dsp_data_t inputSignal, dsp_data_t origin);

#ifdef __cplusplus
} /* extern "C" */
#endif /* nextern "C" */

#endif /* FILTER_LIB_H */


/******************************************************************************
**                               End Of File
*******************************************************************************/