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

#include "../Framework/c_wrapper.h"
#include "acs_supervisor.h"
#include "fletcher.h"
#include "sip_hash.h"
#include <stdio.h>
#include <assert.h>

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
* @return
*******************************************************************************/

void supervisorInitCode(uint8_t procID)
{
    assert(fletcher32Test());       /*test fletcher32 implementation*/ 
    sipHashTest();
    printf("Supervisor ID:%d init OK...\n", procID);
}

void supervisorCode(com_data_pt comDataProcA, com_data_pt comDataProcB, output_data_pt outputData)
{
    sample_data_t resultSample;
    
    resultSample = (comDataProcA->dataSample + comDataProcB->dataSample)*0.5;
    outputData->output = acsDecodingLUT[resultSample];
    outputData->flags = comDataProcA->flags;
    outputData->state = ACS_ACTIVE;
    
    outputData->outputDebug[DEBUG_CHANNEL_A] = comDataProcA->channelDebug[DEBUG_CHANNEL_A];
    outputData->outputDebug[DEBUG_CHANNEL_B] = comDataProcA->channelDebug[DEBUG_CHANNEL_B];
    
    outputData->outputDebug[DEBUG_VECTORS+DEBUG_CHANNEL_A] = comDataProcB->channelDebug[DEBUG_CHANNEL_A];
    outputData->outputDebug[DEBUG_VECTORS+DEBUG_CHANNEL_B] = comDataProcB->channelDebug[DEBUG_CHANNEL_B];
    
    //printf("|%x %x|", comDataProcA->flags, comDataProcB->flags);
}

/******************************************************************************
**                               End Of File
*******************************************************************************/

