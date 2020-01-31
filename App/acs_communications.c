/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file acs_communications.c
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2020-01-08
 * 
 * @brief   - acceleration control sensor communication model functions
 *          - model covers application, data integrity, safety and security layer 
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

#include "acs_communications.h"
#include <time.h>
#include <stdio.h>
#include "sip_hash.h"
#include "crc16.h"
#include "helper_functions.h"

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/*SIP cryptographic signtaure keys*/
static const uint32_t sipKeys[SIP32_32BIT_KEYS] = 
{
    0xDEADBEEF, 0xBAADF00D
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
void packComData(com_data_pt comData)
{
    clock_t processorTime;
    uint8_t comDataVector[ACS_COM_DATA_BYTES];
    uint8_t index;
    
    processorTime = clock();
    
    /* time tag insertion */
    comData->time = (time_stamp_t)processorTime;
    
    /* circular sequence number counter insertion */
    comData->seqNo++;
    
    /*parse data to byte vector*/
    index = 0;
    uint16ToBytes_m(comData->dataID, comDataVector, index);
    uint16ToBytes_m(comData->seqNo, comDataVector, index);
    uint32ToBytes_m(comData->dataSample, comDataVector,index);
    uint32ToBytes_m(comData->flags, comDataVector,8);
    uint32ToBytes_m(comData->time, comDataVector,12);
    uint32ToBytes_m(comData->channelDebug[DEBUG_CHANNEL_A], comDataVector, 16);
    uint32ToBytes_m(comData->channelDebug[DEBUG_CHANNEL_B], comDataVector, 20);
    
    /* CRC checksum calculation */
    comData->crc = crc16Calculate(comDataVector, ACS_PAYLOAD_DATA_BYTES, CRC_NORMAL_CALC);
    
    index = 24;
    uint16ToBytes_m(comData->crc, comDataVector, index); /*add crc checksum to com byte array*/
    
    /* SIP cryptographic mesage signing */
    comData->signature = sipHash32(comDataVector,ACS_PAYLOAD_DATA_BYTES+CRC_BYTES, sipKeys);
    
    /* XTEA encryption */
}

void unpackComData(com_data_pt comData)
{
    authentication_t sipCalculated;
    crc_t crcCalculated;
    uint8_t comDataVector[ACS_COM_DATA_BYTES];
    uint8_t index;
    
    /*parse data to byte vector*/
    index = 0;
    uint16ToBytes_m(comData->dataID, comDataVector, index);
    uint16ToBytes_m(comData->seqNo, comDataVector, index);
    uint32ToBytes_m(comData->dataSample, comDataVector,4);
    uint32ToBytes_m(comData->flags, comDataVector,8);
    uint32ToBytes_m(comData->time, comDataVector,12);
    uint32ToBytes_m(comData->channelDebug[DEBUG_CHANNEL_A], comDataVector, 16);
    uint32ToBytes_m(comData->channelDebug[DEBUG_CHANNEL_B], comDataVector, 20);
    index = 24;
    uint16ToBytes_m(comData->crc, comDataVector, index);
    
    /* CRC checksum verification */
    crcCalculated = crc16Calculate(comDataVector, ACS_PAYLOAD_DATA_BYTES, CRC_NORMAL_CALC);
    
    /* SIP signature verification */
    sipCalculated = sipHash32(comDataVector,ACS_PAYLOAD_DATA_BYTES+CRC_BYTES, sipKeys);
    
    printf("|CRC: %04X %04X SIP: %08X %08X|", comData->crc, crcCalculated, comData->signature, sipCalculated);
    
    /* XTEA encryption */    
}

/******************************************************************************
**                               End Of File
*******************************************************************************/
