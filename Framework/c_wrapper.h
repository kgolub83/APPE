/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file c_wrapper.h
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2020-01-08
 * 
 * @brief wrapper for c functions and specific user implementation  
 *
 * @version
 *
 * @section REVISION HISTORY
 *  - KG 2020-01-08 Initial implementation 
 *
 ******************************************************************************/

/*******************************************************************************
**                                 Guards
*******************************************************************************/

#ifndef C_WRAPPER_H
#define C_WRAPPER_H

#ifdef __cplusplus
extern "C"
{
#endif /* extern "C" */

/*******************************************************************************
**                                Includes
*******************************************************************************/

#include <stdint.h>
#include <math.h>
#include "appe_decoder.h"
/*******************************************************************************
**                               Constants
*******************************************************************************/  

/*32 bit register bit mask deffinitions*/
#define BIT0    0x00000001U
#define BIT1    0x00000002U
#define BIT2    0x00000004U
#define BIT3    0x00000008U
#define BIT4    0x00000010U
#define BIT5    0x00000020U
#define BIT6    0x00000040U
#define BIT7    0x00000080U
#define BIT8    0x00000100U
#define BIT9    0x00000200U
#define BIT10   0x00000400U
#define BIT11   0x00000800U
#define BIT12   0x00001000U
#define BIT13   0x00002000U
#define BIT14   0x00004000U
#define BIT15   0x00008000U
#define BIT16   0x00010000U
#define BIT17   0x00020000U
#define BIT18   0x00040000U
#define BIT19   0x00080000U
#define BIT20   0x00100000U
#define BIT21   0x00200000U
#define BIT22   0x00400000U
#define BIT23   0x00800000U
#define BIT24   0x01000000U
#define BIT25   0x02000000U
#define BIT26   0x04000000U
#define BIT27   0x08000000U
#define BIT28   0x10000000U
#define BIT29   0x20000000U
#define BIT30   0x40000000U
#define BIT31   0x80000000U

/*Sensor A flags*/
#define APPE_SENS_A_OUT_OF_BOUNDS    BIT0
#define APPE_SENS_A_STALL            BIT1
#define APPE_SENS_A_DYNAMIC_FAULT    BIT2
#define APPE_SENS_A_HEALTHY          BIT3
#define APPE_SENS_A_WARNING          BIT4
#define APPE_SENS_A_FAULT            BIT5

#define APPE_SENS_A_FLAGS_MASK       0x000000FFU
#define APPE_SENS_A_FLAGS_OFFSET     0

/*Sensor B flags*/
#define APPE_SENS_B_OUT_OF_BOUNDS    BIT8
#define APPE_SENS_B_STALL            BIT9
#define APPE_SENS_B_DYNAMIC_FAULT    BIT10
#define APPE_SENS_B_HEALTHY          BIT11
#define APPE_SENS_B_WARNING          BIT12
#define APPE_SENS_B_FAULT            BIT13

#define APPE_SENS_B_FLAGS_MASK       0x0000FF00U
#define APPE_SENS_B_FLAGS_OFFSET     8

/*APPE communication channel A flags*/
#define APPE_WATCHDOG_A              BIT16
#define APPE_DATA_INTEGRITY_A        BIT17
#define APPE_COM_SEQUENCE_A          BIT18
#define APPE_AUTHENTICATION_A        BIT19

#define APPE_COM_A_FLAGS_MASK        0x000F0000U
#define APPE_COM_A_FLAGS_OFFSET      16

/*APPE communication channel B flags*/
#define APPE_WATCHDOG_B              BIT20
#define APPE_DATA_INTEGRITY_B        BIT21
#define APPE_COM_SEQUENCE_B          BIT22
#define APPE_AUTHENTICATION_B        BIT23

#define APPE_COM_B_FLAGS_MASK        0x00F00000U
#define APPE_COM_B_FLAGS_OFFSET      20

/*APPE System flags*/
#define APPE_SIGNALS_ASYMMETRY       BIT24
#define APPE_DATA_ASYMMETRY          BIT25
#define APPE_INVALID_DATA            BIT26
#define APPE_SYSTEM_WARNING          BIT27
#define APPE_SYSTEM_FAULT            BIT28
#define APPE_LUT_A_CORRUPTED         BIT29
#define APPE_LUT_B_CORRUPTED         BIT30

#define APPE_SYSTEM_OK               0x00000000U

#define APPE_SUPERVISOR_FAULTS_MASK  0xFF000000U

#define FATAL_OUTPUT_VALUE      0U
#define XTEA_WORD_LENGTH        2U
#define MD5_WORD_LENGTH         4U
#define AES256_WORD_LENGTH      8U

#define SIGNATURE_WORDS         XTEA_WORD_LENGTH

#define DATA_VECTORS_NO         2U
#define DATA_VECTOR_A           0U
#define DATA_VECTOR_B           1U

/*******************************************************************************
**                            Data Structures
*******************************************************************************/

typedef uint32_t sample_data_t;         /*input data sample type definition*/
typedef uint8_t data_vector_no_t;       /*number of input data vectors type*/
typedef uint16_t data_attribute_t;      /*test data attributes type*/
typedef uint32_t samples_no_t;          /*number of test data samples type*/
typedef uint32_t appe_flags_t;          /*appe system flags register type*/
typedef uint32_t time_stamp_t;          /*communication time stamp type*/
typedef uint16_t sequence_no_t;         /*communication mesage sequence number type*/
typedef uint32_t authentication_t;      /*communication authentication signature type*/
typedef uint16_t crc_t;                 /*communication CRC checksum type*/
typedef uint16_t mesage_id_t;           /*mesage identifier type*/
typedef uint32_t debug_vector_data_t;   /*debug data type*/

/*APPE system states definitions*/
typedef enum
{
    APPE_IDLE = 0,
    APPE_ACTIVE,
    APPE_WARNING_OPERATIONAL,
    APPE_WARNING_RESTRICTIVE,
    APPE_ERROR_SPEED_LIMIT,
    APPE_FATAL_SAFE_STOP
} appe_state_e;

/*APPE system falgs definitions*/
typedef enum
{
    APPE_SYS_OK = 0,
    APPE_SYS_WAR,
    APPE_COM_WAR,
    APPE_SIGNAL_WAR,
    APPE_PROCESSING_DIF,
    APPE_SYS_FAULT,
    APPE_COM_ERR,
    APPE_SIG_ERR
} appe_sys_faults_e;

/*APPE debug channels definitions*/
enum debugVectorsNames
{
    DEBUG_CHANNEL_A = 0,
    DEBUG_CHANNEL_B,
    /*Number of debug vectors*/
    DEBUG_VECTORS
};

/*Test data attributes and meta data structure*/
typedef struct
{
    data_attribute_t resolution;
    data_attribute_t guardRegion;
    data_attribute_t sampleRate;
    data_attribute_t samplesNo;
    data_attribute_t decoderRange;
} tst_data_attributes_t;

typedef tst_data_attributes_t* const tst_data_attributes_pt;  /*const pointer to tst_data_attributes_t*/

/*Input data structure - mocks actual ADC data*/ 
typedef struct
{
    sample_data_t sensorSampleA;
    sample_data_t sensorSampleB;
    samples_no_t sampleNo;   
} input_data_t;

typedef input_data_t* const input_data_pt;  /*const pointer to proces_data_t*/

typedef struct
{
    sample_data_t dataSample;
    appe_flags_t flags;
    time_stamp_t time;
    debug_vector_data_t channelDebug[DEBUG_VECTORS];
    authentication_t signature;
    mesage_id_t dataID;
    sequence_no_t seqNo;
    crc_t crc;
} com_data_t;

typedef com_data_t* const com_data_pt;  /*const pointer to com_data_t*/

#define APPE_PAYLOAD_DATA_BYTES  ( (uint8_t)(sizeof(mesage_id_t) + sizeof(sequence_no_t)                \
                                + sizeof(sample_data_t) + sizeof(appe_flags_t) + sizeof(time_stamp_t)   \
                                + sizeof(debug_vector_data_t)*DEBUG_VECTORS) )

#define CRC_BYTES       ( (uint8_t)sizeof(crc_t) )
#define SIGNATURE_BYTES ( (uint8_t)sizeof(authentication_t) )

#define APPE_COM_DATA_BYTES      ( (uint8_t)(APPE_PAYLOAD_DATA_BYTES + CRC_BYTES + SIGNATURE_BYTES) )

#define APPE_COM_CHANNEL_BYTES   ( (uint8_t)(APPE_COM_DATA_BYTES + (8-APPE_COM_DATA_BYTES%8) ) )    /*com channel data aligned on 8 bytes - 32 bytes of data*/

typedef struct
{
    uint8_t comChannelData[APPE_COM_CHANNEL_BYTES];
} com_channel_t;

typedef com_channel_t* const com_channel_pt;  /*const pointer to com_data_t*/

/*supervisor output data structure - APPE processed data and flags*/
typedef struct
{
    sample_data_t output;
    appe_flags_t flags;
    appe_state_e state;
    debug_vector_data_t outputDebug[DEBUG_VECTORS*DATA_VECTORS_NO];    
} output_data_t;

typedef output_data_t* const output_data_pt; /*const pointer to output_data_t*/

/*******************************************************************************
**                                Macros
*******************************************************************************/

#ifndef __weak
#define __weak  __attribute__((weak))
#endif

#define COM_SEQUENCE_NUMBER_RANGE   ((sequence_no_t)pow(2,sizeof(sequence_no_t)*8))

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/*******************************************************************************
**                     Public function prototypes - API
*******************************************************************************/

extern void procInitCodeA(uint8_t procID);
extern void processorCodeA(input_data_pt, com_channel_pt); 
extern void processorExitRoutineA(uint8_t procID); 
extern void procInitCodeB(uint8_t procID);
extern void processorCodeB(input_data_pt, com_channel_pt); 
extern void processorExitRoutineB(uint8_t procID); 
extern void supervisorInitCode(uint8_t procID); 
extern void supervisorCode(com_channel_pt, com_channel_pt, output_data_pt);
extern void supervisorExitRoutine(uint8_t procID);
extern void setTestDataAttributes(tst_data_attributes_pt);
extern void getTestDataAttributes(tst_data_attributes_pt);

#ifdef __cplusplus
} /* extern "C" */
#endif /* extern "C" */

#endif /* C_WRAPPER_H */

/******************************************************************************
**                               End Of File
*******************************************************************************/

