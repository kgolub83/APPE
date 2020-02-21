/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file appe_supervisor.c
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
#include "appe_supervisor.h"
#include "appe_communications.h"
#include "appe_processing_common.h"
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
* @function 
* 
* @brief 
*
* @param 
*
* @returns 
*******************************************************************************/
bool checkDecoderLUT(const uint32_t lut[])
{
    uint16_t tempLUT[APPE_DECODER_LUT_ELEMENTS-1];       /*temprary LUT table to convert values to 16 bit*/
    uint32_t checksumCalc, i;
    
    /*convert LUT data for fletcher checksum calculation, expecting 16bit input data*/
    for(i=0; i<APPE_DECODER_LUT_ELEMENTS-1; i++)
    {
        tempLUT[i] = (uint16_t)lut[i];
    }
    
    checksumCalc = fletcher32(tempLUT, APPE_DECODER_LUT_ELEMENTS-1);   /*check checksum*/

    if(lut[APPE_DECODER_LUT_ELEMENTS-1] == checksumCalc)
    {
        return true;
    } else
    {
        return false;
    }
}

/*!*****************************************************************************
* @function 
* 
* @brief 
*
* @param 
*
* @returns 
*******************************************************************************/
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
    logInit(INFO, LOG_WRITE_FULL, LOG_TO_FILE_ENABLED, WRITE_TO_STDERR_ENABLED, VT100_ENABLED);
    
    if(checkDecoderLUT(appePrimDecodingLUT))
    {
        LogFull_m(DECODING_LUT_CHECKED, "Primary decoding LUT checked", 1, 1);
    } else
    {
        LogFull_m(ERR_DECODING_LUT, "Primary decoding LUT corrupted!", 1, 1);
        checkLUT--;
    }
    
    if(checkDecoderLUT(appeBackDecodingLUT))
    {
        LogFull_m(DECODING_LUT_CHECKED, "Backup decoding LUT checked", 1, 2);
    } else
    {
        LogFull_m(ERR_DECODING_LUT, "Backup decoding LUT corrupted!", 1, 2);
        checkLUT--;
    }
    
    if(checkLUT<1) /*check if both LUTs corrupted*/
    {
        LogFull_m(FATAL_DECODING_LUTS, "Both decoding tables corrupted!", 1, 2);
    }
    
    LogFull_m(SYS_INIT, "Supervisor init OK", 1, procID);
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
    appe_flags_t comStatA, comStatB, flags;
    appe_sys_faults_e appeStatus;
    appe_state_e appeState;
    tst_data_attributes_t dataAttributes;
    static appe_state_e oldState;
    static uint32_t mesageCntA, mesageCntB, iterationCnt;
    
    /* get test meta data*/
    getTestDataAttributes(&dataAttributes);
    
    /*initialize system status and system flags*/
    appeStatus = APPE_SYS_OK;
    flags = APPE_SYS_OK;
    resultSample = 0;

    iterationCnt++;
    
    /*check communication channel A status*/
    if(!(comStatA = unpackComData(&comDataProcA, comFrameA)))
    {
        /*check sequnce number and data ID */
        if((mesageCntA == comDataProcA.seqNo) && APPE_MESAGE_ID_COM_PROC_A == comDataProcA.dataID)
        {
            mesageCntA++; /*Next expected sequence number*/
            resultSample = comDataProcA.dataSample; /*store channel a data as valid data*/
        } else
        {
            LogFull_m(ERR_COM_SEQUENCE, "Com A sequence number or data ID error", 1, iterationCnt);
            mesageCntA = comDataProcA.seqNo + 1; /*set new expected sequence */ 
            appeStatus = APPE_COM_WAR;
            flags |= APPE_COM_SEQUENCE_A;
        }    
    } else
    {
        appeStatus = APPE_COM_WAR;
        LogFull_m(ERR_COM_CHANNEL_A, "Proc A communication fault!", 1, iterationCnt);
    }
    
    /*check communication channel B status*/
    if(!(comStatB = unpackComData(&comDataProcB, comFrameB)))
    {
        /*check sequnce number and data ID */
        if((mesageCntB == comDataProcB.seqNo) && (APPE_MESAGE_ID_COM_PROC_B == comDataProcB.dataID))
        {
            mesageCntB++; /*Next expected sequence number*/
            resultSample = comDataProcB.dataSample; /*store channel a data as valid data*/
        } else
        {
            LogFull_m(ERR_COM_SEQUENCE, "Com B sequence number or message ID error", 1, iterationCnt);
            mesageCntB = comDataProcB.seqNo + 1;
            appeStatus = APPE_COM_WAR;
            flags |= APPE_COM_SEQUENCE_B;
        }
    } else
    {
        LogFull_m(ERR_SYS_COM_FAULT, "System communication fault", 0, 0);
        appeStatus = APPE_COM_WAR;
    }
    
    flags |= comStatA | (comStatB << (APPE_COM_B_FLAGS_OFFSET-APPE_COM_A_FLAGS_OFFSET));

    /*check if both channels returned fault flags*/
    if(((flags & APPE_COM_A_FLAGS_MASK)>0) && ((flags & APPE_COM_B_FLAGS_MASK)>0)) 
    {
        LogFull_m(ERR_SYS_COM, "Both communication channels fault", 1, iterationCnt);
        appeStatus = APPE_COM_ERR;
        flags |= APPE_INVALID_DATA;
    }   
    
    if(!flags)      /*in case of no communication errors*/
    {
        int dataDifference = (comDataProcA.dataSample - comDataProcB.dataSample);
        int dataTolerance = dataAttributes.resolution * SIGNAL_TOLERANCE;
        
        if(dataDifference < 0)        /*calculate absolute difference*/
        {
            dataDifference *= (-1);
        }
        
        /*check if input signal difference detected*/
        if((comDataProcA.flags & APPE_SIGNALS_ASYMMETRY) || (comDataProcB.flags == APPE_SIGNALS_ASYMMETRY))
        {
            appeStatus = APPE_SIGNAL_WAR;
            LogFull_m(WAR_SIGNAL_ASYMMETRY, "Supervisor signal asymmetry warning", 1, iterationCnt);
        }
        
        /*compare datata form both channels*/
        if((comDataProcA.flags != comDataProcB.flags) || (dataDifference > dataTolerance))
        {
            appeStatus = APPE_PROCESSING_DIF;
            flags |= APPE_DATA_ASYMMETRY;
            LogFull_m(WAR_SIGNAL_ASYMMETRY, "Supervisor different data received!", 1, iterationCnt);
        }
        
        /*check if both processing units returned error*/
        if((comDataProcA.flags & APPE_SYSTEM_FAULT) && (comDataProcB.flags & APPE_SYSTEM_FAULT))
        {
            appeStatus = APPE_SIG_ERR;
            resultSample = 0;
            LogFull_m(FATAL_PROCESSING_FAULT, "Supervisor received both procesors fault", 1, iterationCnt);
        } else if(comDataProcA.flags & APPE_SYSTEM_FAULT) /*check if processor A returned fault*/
        {
            resultSample = comDataProcB.dataSample;
        } else if(comDataProcB.flags & APPE_SYSTEM_FAULT) /*check if processor B returned fault*/
        {
            resultSample = comDataProcA.dataSample;
        } else 
        {
            resultSample = (comDataProcA.dataSample + comDataProcB.dataSample)*0.5;
        }
    }
    
    switch (appeStatus)
    {
        case APPE_SYS_OK:
            resultSample = (comDataProcA.dataSample + comDataProcB.dataSample)*0.5;
            appeState = APPE_ACTIVE;
            break;
        case APPE_SYS_WAR:
            resultSample = (comDataProcA.dataSample + comDataProcB.dataSample)*0.5;
            appeState = APPE_WARNING_OPERATIONAL;
            LogFull_m(WAR_SUPERVISOR_SYS, "Supervisor SYS warning", 2, iterationCnt, appeState);
            break;
        case APPE_COM_WAR:
            resultSample = resultSample;
            appeState = APPE_WARNING_RESTRICTIVE;
            LogFull_m(WAR_SUPERVISOR_COM, "Supervisor COM warning", 2, iterationCnt, appeState);
            break;
        case APPE_SIGNAL_WAR:
            resultSample = (comDataProcA.dataSample + comDataProcB.dataSample)*0.5;
            appeState = APPE_WARNING_RESTRICTIVE;
            LogFull_m(WAR_SUPERVISOR_SIGNAL_DIF, "Supervisor signal difference", 2, iterationCnt, appeState);
            break;
        case APPE_PROCESSING_DIF:
            resultSample = (comDataProcA.dataSample + comDataProcB.dataSample)*0.5;
            appeState = APPE_ERROR_SPEED_LIMIT;
            LogFull_m(WAR_SUPERVISOR_DATA_DIF, "Supervisor data difference", 2, iterationCnt, appeState);
            break;
        case APPE_SYS_FAULT:
            resultSample = 0;
            appeState = APPE_FATAL_SAFE_STOP;
            LogFull_m(FATAL_SUPERVISOR_SYS, "Supervisor system fault", 2, iterationCnt, appeState);
            break;
        case APPE_COM_ERR:
            resultSample = 0;
            appeState = APPE_FATAL_SAFE_STOP;
            LogFull_m(FATAL_SUPERVISOR_SYS, "Supervisor communication fault", 2, iterationCnt, appeState);
            break;
        case APPE_SIG_ERR:
            resultSample = 0;
            appeState = APPE_FATAL_SAFE_STOP;
            LogFull_m(FATAL_SUPERVISOR_DATA, "Supervisor position signals error", 2, iterationCnt, appeState);
            break;
        default:
            resultSample = 0;
            appeState = APPE_FATAL_SAFE_STOP;
            LogFull_m(FATAL_SUPERVISOR_SYS, "Supervisor communication fault", 2, iterationCnt, appeState);
    }
    
    if(checkDecoderLUT(appePrimDecodingLUT)) /* check primary LUT */
    {
        outputData->output = appePrimDecodingLUT[resultSample];
    } else if(checkDecoderLUT(appeBackDecodingLUT)) /* check secondary LUT */
    {
        outputData->output = appeBackDecodingLUT[resultSample];
        flags |= APPE_LUT_A_CORRUPTED | APPE_SYSTEM_WARNING;    /* set system flags*/
        appeState = APPE_WARNING_OPERATIONAL;    /* set system state*/
    } else /* in case both LUTs are corrupted */
    {
        outputData->output = FATAL_OUTPUT_VALUE;
        flags |= APPE_LUT_A_CORRUPTED | APPE_LUT_B_CORRUPTED | APPE_SYSTEM_FAULT; /*set system flags*/
        appeState = APPE_FATAL_SAFE_STOP;    /*set system state*/
    }
    
    outputData->flags = comDataProcB.flags | comDataProcA.flags | flags;
    outputData->state = appeState;
    
    if(outputData->state != oldState)
    {
        LogFull_m(SYSTEM_STATE_CHANGED, "APPE state changed", 3, iterationCnt, (int)outputData->state, (int)outputData->flags);
        oldState = outputData->state;
    }
    
    LogFull_m(TRACE_APPE_OUTPUT, "Processed supervisor output", 5, mesageCntA, mesageCntB, outputData->output, outputData->state, flags);
    outputData->outputDebug[DEBUG_CHANNEL_A] = comDataProcA.channelDebug[DEBUG_CHANNEL_A];
    outputData->outputDebug[DEBUG_CHANNEL_B] = comDataProcA.channelDebug[DEBUG_CHANNEL_B];
    
    outputData->outputDebug[DEBUG_VECTORS+DEBUG_CHANNEL_A] = comDataProcB.channelDebug[DEBUG_CHANNEL_A];
    outputData->outputDebug[DEBUG_VECTORS+DEBUG_CHANNEL_B] = comDataProcB.channelDebug[DEBUG_CHANNEL_B];
    
}

/*!*****************************************************************************
* @function 
* 
* @brief 
*
* @param 
*
* @returns 
*******************************************************************************/
void supervisorExitRoutine(uint8_t procID)
{
    LogFull_m(LOG_SYS_EVENT, "Supervisor routine exited", 1, procID);
    logWrite();     /* dump log data from memory to destination instances*/
}

/******************************************************************************
**                               End Of File
*******************************************************************************/

