/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file xtea.h
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2020-01-08
 * 
 * @brief XTEA cryptographic encryption algorithm 
 *
 * @version 0.1
 *
 * @section REVISION HISTORY
 *  - 0.1 KG 2020-01-08 Initial implementation 
 *
 ******************************************************************************/

/*******************************************************************************
**                                 Guards
*******************************************************************************/

#ifndef XTEA_H
#define XTEA_H

#ifdef __cplusplus
extern "C"
{
#endif /* extern "C" */

/*******************************************************************************
**                                Includes
*******************************************************************************/

#include <stdint.h>
#include <stdbool.h>

/*******************************************************************************
**                               Constants
*******************************************************************************/  

#define XTEA_DATA_BYTES         8
#define XTEA_32BIT_KEYS         4
#define XTEA_DELTA_FACTOR       0x9E3779B9
#define XTEA_ROUNDS             32
#define XTEA_LSHIFT             4
#define XTEA_RSHIFT             5
#define XTEA_SUM_SHIFT          11
#define MBEDTLS_XTEA_ENCRYPT    1
#define MBEDTLS_XTEA_DECRYPT    0

/*******************************************************************************
**                                Macros
*******************************************************************************/

/*******************************************************************************
**                            Data Structures
*******************************************************************************/

typedef enum
{
    XTEA_SUCCESS,
    XTEA_ENCRYPTION_FAIL,
    XTEA_DECRYPTION_FAIL
} xtea_ret_val_e;

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/*******************************************************************************
**                     Public function prototypes - API
*******************************************************************************/

extern void xteaEncrypt(const uint32_t key[XTEA_32BIT_KEYS], const uint8_t input[XTEA_DATA_BYTES], uint8_t output[XTEA_DATA_BYTES]);
extern xtea_ret_val_e xteaByteArrayEncrypt(const uint32_t key[XTEA_32BIT_KEYS], uint8_t *dataToEncrypt, const uint32_t dataBytes);
extern void xteaDecrypt(const uint32_t key[XTEA_32BIT_KEYS], const uint8_t input[XTEA_DATA_BYTES], uint8_t output[XTEA_DATA_BYTES]);
extern xtea_ret_val_e xteaByteArrayDecrypt(const uint32_t key[XTEA_32BIT_KEYS], uint8_t *dataToEncrypt, const uint32_t dataBytes);
extern bool xteaTest(void);

#ifdef __cplusplus
} /* extern "C" */
#endif /* extern "C" */

#endif /* XTEA_H */

/******************************************************************************
**                               End Of File
*******************************************************************************/
