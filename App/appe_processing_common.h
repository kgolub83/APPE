/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file appe_processing_common.h
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2020-01-08
 * 
 * @brief common processing functions for modeled processors A and B 
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

#ifndef APPE_PROCESSING_COMMON_H
#define APPE_PROCESSING_COMMON_H

#ifdef __cplusplus
extern "C"
{
#endif /* extern */

/*******************************************************************************
**                                Includes
*******************************************************************************/

#include "../Framework/c_wrapper.h"
#include <stdbool.h>
#include "dsp_filters_lib.h"
#include "fletcher.h"
#include "sip_hash.h"
#include "crc16.h"
#include "xtea.h"
#include "logger.h"

/*******************************************************************************
**                               Constants
*******************************************************************************/  

#define FILTER_BUFFERING_SAMPLES    1U          /* input samples buffering */
#define SIGNAL_TOLERANCE            0.05f       /* signal classification tolerance*/

/*******************************************************************************
**                                Macros
*******************************************************************************/

/*******************************************************************************
**                            Data Structures
*******************************************************************************/

/* signal processing state enumeration */
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

appe_flags_t checkSignalConstraints(const dsp_data_t sampleA, const dsp_data_t sampleB, const tst_data_attributes_pt constraints);
bool checkComplementarity(const dsp_data_t sampleA, const dsp_data_t sampleB, const tst_data_attributes_pt constraints);

#ifdef __cplusplus
} /* extern */
#endif /* extern */

#endif /* APPE_PROCESSING_COMMON_H */

/******************************************************************************
**                               End Of File
*******************************************************************************/
