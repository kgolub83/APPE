/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file .c
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2019-12-20
 * 
 * @brief 
 *
 * @version
 *
 * @section REVISION HISTORY
 *  - KG 2019-12-20 Initial implementation 
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
#endif /* nextern "C" */

/*******************************************************************************
**                                Includes
*******************************************************************************/

#include <stdint.h>

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
#define ACS_SENS_A_OUT_OF_BOUNDS    BIT0
#define ACS_SENS_A_STALL            BIT1
#define ACS_SENS_A_DYNAMIC_FAULT    BIT2
#define ACS_SENS_A_HEALTHY          BIT3
#define ACS_SENS_A_WARNING          BIT4
#define ACS_SENS_A_FAULT            BIT5

#define ACS_SENS_A_FAULTS_MASK      0x000000FFU

/*Sensor B flags*/
#define ACS_SENS_B_OUT_OF_BOUNDS    BIT8
#define ACS_SENS_B_STALL            BIT9
#define ACS_SENS_B_DYNAMIC_FAULT    BIT10
#define ACS_SENS_B_HEALTHY          BIT11
#define ACS_SENS_B_WARNING          BIT12
#define ACS_SENS_B_FAULT            BIT13

#define ACS_SENS_B_FAULTS_MASK      0x0000FF00U

/*ACS System flags*/
#define ACS_WATCHDOG_A              BIT16
#define ACS_DATA_INTEGRITY_A        BIT17
#define ACS_TIME_INTEGRITY_A        BIT18
#define ACS_AUTHENTICATION_A        BIT19
#define ACS_WATCHDOG_B              BIT20
#define ACS_DATA_INTEGRITY_B        BIT21
#define ACS_TIME_INTEGRITY_B        BIT22
#define ACS_AUTHENTICATION_B        BIT23

#define ACS_SIGNALS_ASYMMETRY       BIT24
#define ACS_DATA_ASYMMETRY          BIT25
#define ACS_INVALID_DATA            BIT26
#define ACS_SYSTEM_FAULT            BIT27

#define ACS_SYSTEM_OK               0x00000000U

#define ACS_SUPERVISOR_FAULTS_MASK  0xFFFF0000U

#define XTEA_WORD_LENGTH        2U
#define MD5_WORD_LENGTH         4U
#define AES256_WORD_LENGTH      8U

#define SIGNATURE_WORDS         XTEA_WORD_LENGTH

#define DATA_VECTORS_NO         2U
#define DATA_VECTOR_A           0U
#define DATA_VECTOR_B           1U

/*******************************************************************************
**                                Macros
*******************************************************************************/

#ifndef __weak
#define __weak  __attribute__((weak))
#endif

/*******************************************************************************
**                            Data Structures
*******************************************************************************/

typedef uint32_t sample_data_t;     /*input data sample type deffinition*/
typedef uint8_t data_vector_no_t;   /*number of input data vectors type*/
typedef uint16_t data_attribute_t;  /*test data attributes type*/
typedef uint32_t samples_no_t;      /*number of test data samples type*/
typedef uint32_t acs_flags_t;       /*acs system flags register type*/
typedef uint32_t time_stamp_t;      /*communication time stamp type*/
typedef uint8_t sequence_no_t;      /*communication mesage sequence number type*/
typedef uint32_t authentication_t;  /*communication authentication signature type*/
typedef uint16_t crc_t;             /*communication CRC checksum type*/
typedef uint32_t debug_vector_data_t;

/*ACS system states definitions*/
typedef enum
{
    ACS_IDLE,
    ACS_ACTIVE,
    ACS_WARNING_OPERATIONAL,
    ACS_WARNING_RESTRICTIVE,
    ACS_ERROR_SPEED_LIMIT,
    ACS_FATAL_SAFE_STOP
} acs_state_e;

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

/*Input data strucutre - mocks actual ADC data*/ 
typedef struct
{
    sample_data_t sensorSampleA;
    sample_data_t sensorSampleB;
    samples_no_t sampleNo;   
} input_data_t;

typedef input_data_t* const input_data_pt;  /*const pointer to proces_data_t*/

/*communication data structure between processors and supervisor*/
typedef struct
{
    sample_data_t dataSample;
    acs_flags_t flags;
    time_stamp_t time;
    sequence_no_t seqNo;
    debug_vector_data_t channelDebug[DEBUG_VECTORS];    
    authentication_t signature[SIGNATURE_WORDS];
    crc_t crc;
} com_data_t;

typedef com_data_t* const com_data_pt;  /*const pointer to com_data_t*/

/*supervisor output data structure - ACS processed data and flags*/
typedef struct
{
    sample_data_t output;
    acs_flags_t flags;
    acs_state_e state;
    debug_vector_data_t outputDebug[DEBUG_VECTORS*DATA_VECTORS_NO];    
} output_data_t;

typedef output_data_t* const output_data_pt; /*const pointer to output_data_t*/
    
/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

extern const uint32_t acsDecodingLUT[];   /*ACS position decoding table*/

/*******************************************************************************
**                     Public function prototypes - API
*******************************************************************************/

extern void procInitCodeA(uint8_t procID);
extern void processorCodeA(input_data_pt, com_data_pt);  
extern void procInitCodeB(uint8_t procID);
extern void processorCodeB(input_data_pt, com_data_pt); 
extern void supervisorInitCode(uint8_t procID); 
extern void supervisorCode(com_data_pt, com_data_pt, output_data_pt);
extern void setTestDataAttributes(tst_data_attributes_pt);
extern void getTestDataAttributes(tst_data_attributes_pt);

#ifdef __cplusplus
} /* extern "C" */
#endif /* nextern "C" */

#endif /* C_WRAPPER_H */

/******************************************************************************
**                               End Of File
*******************************************************************************/

