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

bool checkDecoderLUT(const uint32_t lut[])
{
    uint16_t tempLUT[ACS_DECODER_LUT_ELEMENTS-1];       /*temprary LUT table to convert values to 16 bit*/
    uint32_t checksumCalc, i;
    
    /*convert LUT data for fletcher checksum calculation, expecting 16bit input data*/
    for(i=0; i<ACS_DECODER_LUT_ELEMENTS-1; i++)
    {
        tempLUT[i] = (uint16_t)lut[i];
    }
    
    checksumCalc = fletcher32(tempLUT, ACS_DECODER_LUT_ELEMENTS-1);   /*check checksum*/

    if(lut[ACS_DECODER_LUT_ELEMENTS-1] == checksumCalc)
    {
        return true;
    } else
    {
        return false;
    }
}

void supervisorInitCode(uint8_t procID)
{
    assert(fletcher32Test());       /*test fletcher32 checksum implementation*/
    LogFull_m(INFO_SYS_EVENT, "Flatcher checksum test OK", 0,0);
    assert(sipHashTest());          /*test SIP hash implementation*/
    LogFull_m(INFO_SYS_EVENT, "SIP hash test OK", 0,0);
    assert(crc16Test());            /*test CRC checksum implementation*/
    LogFull_m(INFO_SYS_EVENT, "CRC checksum test OK", 0,0);
    assert(xteaTest());             /*test XTEA encryption implementation*/
    LogFull_m(INFO_SYS_EVENT, "XTEA encryption test OK", 0,0);
    assert(integratorTest(50, 20)); /*test error integrator*/
    LogFull_m(INFO_SYS_EVENT, "Error integrator test OK", 0,0);
    
    int checkLUT = 2;
    
    /*setup system logger*/
    logInit(VERBOSE, LOG_WRITE_FULL, LOG_TO_FILE_ENABLED, WRITE_TO_STDERR_ENABLED, VT100_ENABLED);
    
    if(checkDecoderLUT(acsPrimDecodingLUT))
    {
        LogFull_m(DECODING_LUT_CHECKED, "Primary decoding LUT checked", 1, 1);
    } else
    {
        LogFull_m(ERR_DECODING_LUT, "Primary decoding LUT corrupted!", 1, 1);
        checkLUT--;
    }
    
    if(checkDecoderLUT(acsBackDecodingLUT))
    {
        LogFull_m(DECODING_LUT_CHECKED, "Backup decoding LUT checked", 1, 2);
    } else
    {
        LogFull_m(ERR_DECODING_LUT, "Backup decoding LUT corrupted!", 1, 2);
        checkLUT--;
    }
    
    assert(checkLUT>=1);     /*check if both LUTs corrupted*/
    
    LogFull_m(SYS_INIT, "Supervisor init OK...", 1, procID);
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
            LogFull_m(ERR_COM_SEQUENCE, "Com A sequence number error", 1, 1);
            mesageCntA = comDataProcA.seqNo + 1;
        }
        
    } else
    {
        acsStatus = ACS_COM_WAR;
        LogFull_m(ERR_COM_CHANNEL_A, "Proc A communication fault!", 0, 0);
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
            LogFull_m(ERR_COM_SEQUENCE, "Com B sequence number error", 1, 2);
            mesageCntB = comDataProcB.seqNo + 1;
        }
    } else if(acsStatus == ACS_COM_WAR)
    {
        acsStatus = ACS_COM_ERR;
        LogFull_m(ERR_COM_CHANNEL_A, "Processor A communication fault", 1, 1);
    } else
    {
        acsStatus = ACS_COM_WAR;
        LogFull_m(ERR_SYS_COM_FAULT, "System communication fault", 0, 0);
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
            LogFull_m(WAR_SIGNAL_ASYMMETRY, "Supervisor signal asymetry warning", 1, comDataProcA.seqNo);
        }
        
        /*compare datata form both channels*/
        if((comDataProcA.flags != comDataProcB.flags) || (dataDifference > dataTolerance))
        {
            acsStatus = ACS_PROCESSING_DIF;
            flags |= ACS_DATA_ASYMMETRY;
            LogFull_m(WAR_SIGNAL_ASYMMETRY, "Supervisor different data received!", 1, comDataProcA.seqNo);
        }
        
        /*check if both processing units returned error*/
        if((comDataProcA.flags & ACS_SYSTEM_FAULT) && (comDataProcB.flags & ACS_SYSTEM_FAULT))
        {
            acsStatus = ACS_SIG_ERR;
            resultSample = 0;
            LogFull_m(FATAL_PROCESSING_FAULT, "Supervisor received both procesors fault", 1, comDataProcA.seqNo);
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
            LogFull_m(WAR_SUPERVISOR_SYS, "Supervisor SYS warning", 1, comDataProcA.seqNo);
            break;
        case ACS_COM_WAR:
            resultSample = resultSample;
            acsState = ACS_WARNING_RESTRICTIVE;
            LogFull_m(WAR_SUPERVISOR_COM, "Supervisor COM warning", 1, comDataProcA.seqNo);
            break;
        case ACS_SIGNAL_WAR:
            resultSample = (comDataProcA.dataSample + comDataProcB.dataSample)*0.5;
            acsState = ACS_WARNING_RESTRICTIVE;
            LogFull_m(WAR_SUPERVISOR_SIGNAL_DIF, "Supervisor signal diference", 1, comDataProcA.seqNo);
            break;
        case ACS_PROCESSING_DIF:
            resultSample = (comDataProcA.dataSample + comDataProcB.dataSample)*0.5;
            LogFull_m(WAR_SUPERVISOR_DATA_DIF, "Supervisor data diference", 1, comDataProcA.seqNo);
            acsState = ACS_ERROR_SPEED_LIMIT;
            break;
        case ACS_SYS_FAULT:
            resultSample = 0;
            acsState = ACS_FATAL_SAFE_STOP;
            LogFull_m(FATAL_SUPERVISOR_SYS, "Supervisor system fault", 1, comDataProcA.seqNo);
            break;
        case ACS_COM_ERR:
            resultSample = 0;
            acsState = ACS_FATAL_SAFE_STOP;
            LogFull_m(FATAL_SUPERVISOR_SYS, "Supervisor communication fault", 1, comDataProcA.seqNo);
            break;
        case ACS_SIG_ERR:
            resultSample = 0;
            acsState = ACS_FATAL_SAFE_STOP;
            LogFull_m(FATAL_SUPERVISOR_DATA, "Supervisor position signals error", 1, comDataProcA.seqNo);
            break;
        default:
            resultSample = 0;
            acsState = ACS_FATAL_SAFE_STOP;
            LogFull_m(FATAL_SUPERVISOR_SYS, "Supervisor communication fault", 1, comDataProcA.seqNo);
    }
    
    outputData->output = acsPrimDecodingLUT[resultSample];
    outputData->flags = comDataProcB.flags | comDataProcA.flags;
    outputData->state = acsState;
    
    outputData->outputDebug[DEBUG_CHANNEL_A] = comDataProcA.channelDebug[DEBUG_CHANNEL_A];
    outputData->outputDebug[DEBUG_CHANNEL_B] = comDataProcA.channelDebug[DEBUG_CHANNEL_B];
    
    outputData->outputDebug[DEBUG_VECTORS+DEBUG_CHANNEL_A] = comDataProcB.channelDebug[DEBUG_CHANNEL_A];
    outputData->outputDebug[DEBUG_VECTORS+DEBUG_CHANNEL_B] = comDataProcB.channelDebug[DEBUG_CHANNEL_B];
    
}

void supervisorExitRoutine(uint8_t procID)
{
    LogFull_m(LOG_SYS_EVENT, "Supervisor routine exited", 1, procID);
    logWrite();     /* dump log data from memory to destination instances*/
}

/******************************************************************************
**                               End Of File
*******************************************************************************/

