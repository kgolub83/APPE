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

/*******************************************************************************
**                               Constants
*******************************************************************************/  

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

extern void checkSignalIntegrity(void);
extern void checkComplementarity(void);

#ifdef __cplusplus
} // extern "C"
#endif // nextern "C"

#endif /* ACS_PROCESSING_COMMON_H */


/******************************************************************************
**                               End Of File
*******************************************************************************/