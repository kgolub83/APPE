/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file acs_supervisor.c
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2020-01-08
 * 
 * @brief supervisor processing model implementation
 *
 * @version 0.1
 *
 * @section REVISION HISTORY
 *  - 0.1 KG 2020-01-08 Initial implementation 
 *
 ******************************************************************************/
 
/*******************************************************************************
**                                Includes
*******************************************************************************/

#include "../Framework/c_wrapper.h"
#include "acs_supervisor.h"
#include "fletcher.h"
#include "sip_hash.h"
#include "crc16.h"
#include "xtea.h"
#include "logger.h"
#include <stdio.h>
#include <assert.h>

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/*******************************************************************************
**                                 Code
*******************************************************************************/

/*!*****************************************************************************
* @function supervisorInitCode
* 
* @brief supervisor processor model initialisation code
*
* @param virtual processor ID - concurent thread ID
*
* @return void function
*******************************************************************************/

void supervisorInitCode(uint8_t procID)
{
    assert(fletcher32Test());       /*test fletcher32 checksum implementation*/ 
    assert(sipHashTest());          /*test SIP hash implementation*/
    assert(crc16Test());            /*test CRC checksum implementation*/
    assert(xteaTest());             /*test XTEA encryption implementation*/
    
    logTest("Test Supervisor");
    
    /*Flush log events from RAM FIFO to destinatin*/
    logWrite();
    
    printf("Supervisor ID:%d init OK...\n", procID);
}

/*!*****************************************************************************
* @function supervisorCode
* 
* @brief supervisor (arbitration) processor model implementation code
*
* @param comDataProcA - processor A communication channel data
* @param comDataProcB - processor B communication channel data
* @param outputData - arbitration output
*
* @return void function
*******************************************************************************/

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

