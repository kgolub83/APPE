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

#ifndef ACS_PROCESSING_COMMON_H
#define ACS_PROCESSING_COMMON_H

#ifdef __cplusplus
extern "C"
{
#endif // nextern "C"

/*******************************************************************************
**                                Includes
*******************************************************************************/

#include "../Framework/c_wrapper.h"
#include <stdbool.h>
#include "dsp_filters_lib.h"

/*******************************************************************************
**                               Constants
*******************************************************************************/  

#define FILTER_BUFFERING_SAMPLES    1U
#define SIGNAL_TOLERANCE            0.05f

/*******************************************************************************
**                                Macros
*******************************************************************************/

/*******************************************************************************
**                            Data Structures
*******************************************************************************/

typedef enum
{
    PROC_FSM_SIGNAL_HEALTHY,
    PROC_FSM_SIGNAL_A_FAIL,
    PROC_FSM_SIGNAL_B_FAIL,
    PROC_FSM_COMPLEMENTARITY_WAR,
    PROC_FSM_COMPLEMENTARITY_FAIL,
    PROC_FSM_SIGNAL_FAIL
} processing_state_e;

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/*******************************************************************************
**                     Public function prototypes - API
*******************************************************************************/

acs_flags_t checkSignalIntegrity(const input_data_pt data);
acs_flags_t checkSignalConstraints(const dsp_data_t sampleA, const dsp_data_t sampleB, const tst_data_attributes_pt constraints);
bool checkComplementarity(const dsp_data_t sampleA, const dsp_data_t sampleB, const tst_data_attributes_pt constraints);

#ifdef __cplusplus
} // extern "C"
#endif // nextern "C"

#endif /* ACS_PROCESSING_COMMON_H */


/******************************************************************************
**                               End Of File
*******************************************************************************/