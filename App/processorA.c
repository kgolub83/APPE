#include "acs_processing.h"



void executeProcessorA(void)
{
    processing_state_e state;
    
    /*Filter signals A and B*/
    recoursiveAverage();
    
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
        
        case PROC_FSM_COMPLEMENTARITY_WARNING:
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
    }
    
    packComData();
    
}

