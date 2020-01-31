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

#ifndef ACS_COMMUNICATIONS_H
#define ACS_COMMUNICATIONS_H

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

enum acsCommunicationID
{
    ACS_COM_PROC_A_MESAGE_ID,
    ACS_COM_PROC_B_MESAGE_ID
};

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/*******************************************************************************
**                     Public function prototypes - API
*******************************************************************************/

extern void packComData(com_data_pt comData);
extern void unpackComData(com_data_pt comData);

#ifdef __cplusplus
} /* extern */
#endif /* extern */

#endif /* ACS_COMMUNICATIONS_H */ 


/******************************************************************************
**                               End Of File
*******************************************************************************/