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

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

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

void executeProcessorA(void)
{
    processing_state_e state;
    recoursive_avg_t inputSample;

    /*Filter signals A and B*/
    recoursiveAverage(&inputSample);
    
    /*Check constraints for signals A and B*/
    checkSignalIntegrity();
    checkComplementarity();
    
    switch (state)
    {
        case PROC_FSM_SIGNAL_HEALTHY:
            //invert signal B
            //sum signals
            //average signals
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
    
}


/******************************************************************************
**                               End Of File
*******************************************************************************/

