/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file appe_communications.c
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2020-01-08
 * 
 * @brief   - acceleration control sensor communication model functions
 *          - defines application, data integrity, safety, security and packet layer
 *          - communication between processors and supervisor
 *
 * COMMUNICATION FRAME MODEL:
 * 
 * MSB                                                                     LSB
 * |dataID:2|seqNo:2|dataSample:4|flags:4|time:4|debug:8|crc16:2|signature:4|
 *
 * BIG ENDIAN - MSB first
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

#include "appe_communications.h"
#include <time.h>
#include <stdio.h>
#include "sip_hash.h"
#include "crc16.h"
#include "xtea.h"
#include "helper_functions.h"
#include "logger.h"

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/*SIP cryptographic signature keys*/
static const uint32_t sipKeys[SIP32_32BIT_KEYS] = 
{
    0xDEADBEEF, 0xBAADF00D
};

static const uint32_t xteaKeys[XTEA_32BIT_KEYS] =
{
    0x1CEB00DA, 0xC001D00D, 0xC0DE2BAD, 0xDABBAD00
};

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
void packComData(com_data_pt comData, com_channel_pt comFrame)
{
    clock_t processorTime;
    uint8_t index;
    
    processorTime = clock();
    
    /* time tag insertion */
    comData->time = (time_stamp_t)processorTime;
    
    /*parse data to byte vector*/
    index = 0;
    uint16ToBytesReverse_m(comData->dataID, comFrame->comChannelData, index);
    uint16ToBytesReverse_m(comData->seqNo, comFrame->comChannelData, index);
    uint32ToBytesReverse_m(comData->dataSample, comFrame->comChannelData,index);
    uint32ToBytesReverse_m(comData->flags, comFrame->comChannelData,index);
    uint32ToBytesReverse_m(comData->time, comFrame->comChannelData,index);
    uint32ToBytesReverse_m(comData->channelDebug[DEBUG_CHANNEL_A], comFrame->comChannelData, index);
    uint32ToBytesReverse_m(comData->channelDebug[DEBUG_CHANNEL_B], comFrame->comChannelData, index);
    
    /* CRC checksum calculation */
    comData->crc = crc16Calculate(comFrame->comChannelData, APPE_PAYLOAD_DATA_BYTES, CRC_NORMAL_CALC);
    
    uint16ToBytesReverse_m(comData->crc, comFrame->comChannelData, index); /*add crc checksum to com byte array*/
    
    /* SIP cryptographic mesage signing */
    comData->signature = sipHash32(comFrame->comChannelData,APPE_PAYLOAD_DATA_BYTES+CRC_BYTES, sipKeys);
    
    uint32ToBytesReverse_m(comData->signature, comFrame->comChannelData, index); /*add signature to byte array*/
    
    /* XTEA encryption */

    xteaByteArrayEncrypt(xteaKeys, comFrame->comChannelData, APPE_COM_CHANNEL_BYTES);

}

/*!*****************************************************************************
* @function 
* 
* @brief 
*
* @param 
*
* @return 
*******************************************************************************/
appe_flags_t unpackComData(com_data_pt comData, com_channel_pt comFrame)
{
    authentication_t sipCalculated;
    appe_flags_t retVal;
    crc_t crcCalculated;
    uint8_t index;
    
    retVal = APPE_SYSTEM_OK;
    
    /* XTEA encryption */
    if(xteaByteArrayDecrypt(xteaKeys, comFrame->comChannelData, APPE_COM_CHANNEL_BYTES))
    {
        LogFull_m(ERR_XTEA_DECRYPTION, "Com decryption fault!", 0, 0);
        retVal |= APPE_AUTHENTICATION_A;
    } else
    {
        /*parse data to byte vector*/
        index = 0;
        bytesToUint16Reverse_m(comData->dataID, comFrame->comChannelData, index);
        bytesToUint16Reverse_m(comData->seqNo, comFrame->comChannelData, index);
        bytesToUint32Reverse_m(comData->dataSample, comFrame->comChannelData, index);
        bytesToUint32Reverse_m(comData->flags, comFrame->comChannelData,index);
        bytesToUint32Reverse_m(comData->time, comFrame->comChannelData,index);
        bytesToUint32Reverse_m(comData->channelDebug[DEBUG_CHANNEL_A], comFrame->comChannelData, index);
        bytesToUint32Reverse_m(comData->channelDebug[DEBUG_CHANNEL_B], comFrame->comChannelData, index);
        bytesToUint16Reverse_m(comData->crc, comFrame->comChannelData, index);
        bytesToUint32Reverse_m(comData->signature, comFrame->comChannelData, index);
        
        /* SIP signature verification */
        sipCalculated = sipHash32(comFrame->comChannelData, APPE_PAYLOAD_DATA_BYTES+CRC_BYTES, sipKeys);
        
        if(sipCalculated != comData->signature)
        {
            LogFull_m(ERR_SIP_SIGNATURE, "Com signature fault!", 1, comData->dataID);
            retVal = APPE_AUTHENTICATION_A;
        } 
        
        /* CRC checksum verification */
        crcCalculated = crc16Calculate(comFrame->comChannelData, APPE_PAYLOAD_DATA_BYTES, CRC_NORMAL_CALC);    
        
        if(crcCalculated != comData->crc)
        {
            LogFull_m(ERR_CRC_CHECKSUM, "Data integrity fault!",1, comData->dataID);
            retVal = APPE_DATA_INTEGRITY_A;
        }
    }
    
    return retVal;
}

/******************************************************************************
**                               End Of File
*******************************************************************************/
