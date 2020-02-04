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
#include "acs_communications.h"
#include "acs_processing_common.h"
#include "integrator.h"
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
    
    integratorTest(50, 20);
    /*setup logging parameters*/
    logInit(VERBOSE, LOG_WRITE_FULL, LOG_TO_FILE_ENABLED, WRITE_TO_STDERR_ENABLED, VT100_ENABLED);
    
    LogFull_m(SYS_INIT, "Supervisor init OK", 1, 0);
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

void supervisorCode(com_channel_pt comFrameA, com_channel_pt comFrameB, output_data_pt outputData)
{
    com_data_t comDataProcA, comDataProcB;
    sample_data_t resultSample;
    acs_flags_t comStatA, comStatB, flags;
    acs_sys_faults_e acsStatus;
    acs_state_e acsState;
    tst_data_attributes_t dataAttributes;
    static uint32_t mesageCntA, mesageCntB;
    
    /* get test meta data*/
    getTestDataAttributes(&dataAttributes);
    
    acsStatus = ACS_SYS_OK;
    
    /*check communication channel A status*/
    if(!(comStatA = unpackComData(&comDataProcA, comFrameA)))
    {
        resultSample = comDataProcA.dataSample;
        if(mesageCntA == comDataProcA.seqNo)
        {
            mesageCntA++; /*Next espected sequence number*/
        } else
        {
            printf("#ERR: Com A sequence number error\n");
            mesageCntA = comDataProcA.seqNo + 1;
        }
        
    } else
    {
        acsStatus = ACS_COM_WAR;
        printf("Proc A communication fault!\n");
    }
    
    /*check communication channel B status*/
    if(!(comStatB = unpackComData(&comDataProcB, comFrameB)))
    {
        resultSample = comDataProcB.dataSample;
        if(mesageCntB == comDataProcB.seqNo)
        {
            mesageCntB++; /*Next espected sequence number*/
        } else
        {
            printf("#ERR: Com B sequence number error\n");
            mesageCntB = comDataProcB.seqNo + 1;
        }
    } else if(acsStatus == ACS_COM_WAR)
    {
        acsStatus = ACS_COM_ERR;
        printf("Proc A communication fault!\n");
    } else
    {
        acsStatus = ACS_COM_WAR;
        printf("System communication fault!\n");
    }
    
    flags = comStatA | (comStatB << (ACS_COM_B_FLAGS_OFFSET-ACS_COM_A_FLAGS_OFFSET));
    
    if(!flags)      /*in case of no communication errors*/
    {
        int dataDifference = (comDataProcA.dataSample - comDataProcB.dataSample);
        int dataTolerance = dataAttributes.resolution * SIGNAL_TOLERANCE;
        
        if(dataDifference < 0)        /*calculate absolute difference*/
        {
            dataDifference *= (-1);
        }
        
        /*check if input signal difference detected*/
        if((comDataProcA.flags & ACS_SIGNALS_ASYMMETRY) || (comDataProcB.flags == ACS_SIGNALS_ASYMMETRY))
        {
            acsStatus = ACS_SIGNAL_WAR;
            printf("Saignal assyimetry warning!\n");
        }
        
        /*compare datata form both channels*/
        if((comDataProcA.flags != comDataProcB.flags) || (dataDifference > dataTolerance))
        {
            acsStatus = ACS_PROCESSING_DIF;
            flags |= ACS_DATA_ASYMMETRY;
            printf("Supervisor different data received!\n");
        }
        
        /*check if both processing units returned error*/
        if((comDataProcA.flags & ACS_SYSTEM_FAULT) && (comDataProcB.flags & ACS_SYSTEM_FAULT))
        {
            acsStatus = ACS_SIG_ERR;
            resultSample = 0;
            printf("Supervisor received both procesors fault!\n");
        } else if(comDataProcA.flags & ACS_SYSTEM_FAULT) /*check if processor A returned fault*/
        {
            resultSample = comDataProcB.dataSample;
        } else if(comDataProcB.flags & ACS_SYSTEM_FAULT) /*check if processor B returned fault*/
        {
            resultSample = comDataProcA.dataSample;
        } else 
        {
            resultSample = (comDataProcA.dataSample + comDataProcB.dataSample)*0.5;
        }
    }
    
    switch (acsStatus)
    {
        case ACS_SYS_OK:
            resultSample = (comDataProcA.dataSample + comDataProcB.dataSample)*0.5;
            acsState = ACS_ACTIVE;
            break;
        case ACS_SYS_WAR:
            resultSample = (comDataProcA.dataSample + comDataProcB.dataSample)*0.5;
            acsState = ACS_WARNING_OPERATIONAL;
            printf("#WAR: Supervisor SYS!\n");
            break;
        case ACS_COM_WAR:
            resultSample = resultSample;
            acsState = ACS_WARNING_RESTRICTIVE;
            printf("#WAR: Supervisor COM!\n");
            break;
        case ACS_SIGNAL_WAR:
            resultSample = (comDataProcA.dataSample + comDataProcB.dataSample)*0.5;
            acsState = ACS_WARNING_RESTRICTIVE;
            printf("#WAR: Supervisor signal diference!\n");
            break;
        case ACS_PROCESSING_DIF:
            resultSample = (comDataProcA.dataSample + comDataProcB.dataSample)*0.5;
            printf("#WAR: Supervisor data diference!\n");
            acsState = ACS_ERROR_SPEED_LIMIT;
            break;
        case ACS_SYS_FAULT:
            resultSample = 0;
            acsState = ACS_FATAL_SAFE_STOP;
            printf("#WAR: Supervisor SYS!\n");
            break;
        case ACS_COM_ERR:
            resultSample = 0;
            acsState = ACS_FATAL_SAFE_STOP;
            printf("#ERR: Supervisor COM!\n");
            break;
        case ACS_SIG_ERR:
            resultSample = 0;
            acsState = ACS_FATAL_SAFE_STOP;
            printf("#ERR: Supervisor signal error!\n");
            break;
        default:
            resultSample = 0;
            acsState = ACS_FATAL_SAFE_STOP;  
    }
    
    outputData->output = acsDecodingLUT[resultSample];
    outputData->flags = comDataProcB.flags | comDataProcA.flags;
    outputData->state = acsState;
    
    outputData->outputDebug[DEBUG_CHANNEL_A] = comDataProcA.channelDebug[DEBUG_CHANNEL_A];
    outputData->outputDebug[DEBUG_CHANNEL_B] = comDataProcA.channelDebug[DEBUG_CHANNEL_B];
    
    outputData->outputDebug[DEBUG_VECTORS+DEBUG_CHANNEL_A] = comDataProcB.channelDebug[DEBUG_CHANNEL_A];
    outputData->outputDebug[DEBUG_VECTORS+DEBUG_CHANNEL_B] = comDataProcB.channelDebug[DEBUG_CHANNEL_B];
    
    //printf("|%x %x|", comDataProcA.flags, comDataProcB.flags);
}

void supervisorExitRoutine(uint8_t procID)
{
    logWrite();     /* dump log data from memory to destination instances*/
}

/******************************************************************************
**                               End Of File
*******************************************************************************/

