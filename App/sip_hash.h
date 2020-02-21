/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file sip_hash.h
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2020-01-08
 * 
 * @brief   32bit SipHash implementation - Half SIP Hash
 *          pseudorandom functions crypto hashing algorithm optimized for speed 
 *          and short messages
 *          https://131002.net/siphash/
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

#ifndef SIP_HASH_H
#define SIP_HASH_H

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

#define SIP_HASH_C_ROUNDS       2U
#define SIP_HASH_D_ROUNDS       4U

#define SIP_HASH_VAR0_INIT      0U
#define SIP_HASH_VAR1_INIT      0U
#define SIP_HASH_VAR2_INIT      0x6c796765U
#define SIP_HASH_VAR3_INIT      0x74656462U

#define SIP32_TEST_HASHES       64U
#define SIP32_KEY_BYTES         8U
#define SIP32_32BIT_KEYS        (SIP32_KEY_BYTES/(sizeof(uint32_t)))
#define SIP_SIGNATURE_BYTES     4
#define SIP_PROCESSING_BYTES    4

/*******************************************************************************
**                                Macros
*******************************************************************************/

/* SipHash processing algorithm */

#define sipHashProcess_m                \
{                                       \
    v0 += v1;                           \
    v1 = rotLeft32_m(v1, 5);            \
    v1 ^= v0;                           \
    v0 = rotLeft32_m(v0, 16);           \
    v2 += v3;                           \
    v3 = rotLeft32_m(v3, 8);            \
    v3 ^= v2;                           \
    v0 += v3;                           \
    v3 = rotLeft32_m(v3, 7);            \
    v3 ^= v0;                           \
    v2 += v1;                           \
    v1 = rotLeft32_m(v1, 13);           \
    v1 ^= v2;                           \
    v2 = rotLeft32_m(v2, 16);           \
}

/*******************************************************************************
**                            Data Structures
*******************************************************************************/

typedef uint32_t siphash_size_t;     /*sip hash data elements range*/

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/*******************************************************************************
**                     Public function prototypes - API
*******************************************************************************/

extern uint32_t sipHash32(const uint8_t *in, const siphash_size_t inlen, const uint32_t key[2]);
extern bool sipHashTest(void);

#ifdef __cplusplus
} /* extern "C" */
#endif /* extern "C" */

#endif /* SIP_HASH_H */

/******************************************************************************
**                               End Of File
*******************************************************************************/
