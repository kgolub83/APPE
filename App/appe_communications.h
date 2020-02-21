/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file main.cpp
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
**                                 Guards
*******************************************************************************/

#ifndef APPE_COMMUNICATIONS_H
#define APPE_COMMUNICATIONS_H

#ifdef __cplusplus
extern "C"
{
#endif /* extern "C" */

/*******************************************************************************
**                                Includes
*******************************************************************************/

#include "../Framework/c_wrapper.h"

/*******************************************************************************
**                               Constants
*******************************************************************************/  

/*******************************************************************************
**                                Macros
*******************************************************************************/

/*******************************************************************************
**                            Data Structures
*******************************************************************************/

enum appeCommunicationID
{
    APPE_MESAGE_ID_COM_PROC_A,
    APPE_MESAGE_ID_COM_PROC_B
};

typedef enum
{
    APPE_COM_SUCCESS,
    APPE_TIMEOUT_ERR,
    APPE_COM_CRC_ERR,
    APPE_COM_SEQUENCE_ERR,
    APPE_COM_SIGNATURE_ERR,
    APPE_COM_CRYPTO_ERR
} appe_com_ret_val_e;

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/*******************************************************************************
**                     Public function prototypes - API
*******************************************************************************/

extern void packComData(com_data_pt comData, com_channel_pt comFrame);
extern appe_flags_t unpackComData(com_data_pt comData, com_channel_pt comFrame);

#ifdef __cplusplus
} /* extern */
#endif /* extern */

#endif /* APPE_COMMUNICATIONS_H */ 

/******************************************************************************
**                               End Of File
*******************************************************************************/
