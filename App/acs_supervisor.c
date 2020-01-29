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
    assert(fletcher32Test());       /*test fletcher32 checksum implementation*/ 
    assert(sipHashTest());          /*test SIP hash implementation*/
    assert(crc16Test());            /*test CRC checksum implementation*/
    assert(xteaTest());             /*test XTEA encryption implementation*/
    
    int i;
    
    /*setup logging parameters*/
    logInit(VERBOSE, LOG_WRITE_FULL, LOG_TO_FILE_ENABLED, WRITE_TO_STDERR_ENABLED, VT100_ENABLED);
    
    /*demo use example -> generate all logger level mesages - Full logging executed */
    for(i=0; i<LOG_LEVELS_NO; i++)
    {
        LogFull_m((LOG_LEVEL_POSITION*i), "This is a C logger!", 2, 100, i);
    }
    
        /*demo use example -> log user event - Compact logging executed */
    LogCompact_m(PROGRAM_START_SUCCESS, "Program started successfully, with codes:", 5, 10, 20, 30, 40, 50);
    
    /*demo use example -> log user event - Basic logging executed */
    LogBasic_m(PROGRAM_START_SUCCESS, 5, 10, 20, 30, 40, 50);
    
    /*Flush log events from RAM FIFO to destinatin*/
    logWrite();
    
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

