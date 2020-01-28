/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file main.cpp
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2019-12-20
 * 
 * @brief   32bit SipHash implementation - Half SIP Hash
 *          pseudorandom functions crypto hashing algorithm optimized for speed 
 *          and short messages
 *          https://131002.net/siphash/
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

#include "sip_hash.h"
#include <assert.h>
#include <stdio.h>
#include "helper_functions.h"

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

static const uint32_t sip32TestHashVactor[SIP32_TEST_HASHES] = 
{
    0x5B9F35A9, 0xB85A4727, 0x03A662FA, 0x04E7FE8A, 
    0x89466E2A, 0x69B6FAC5, 0x23FC6358, 0xC563CF8B, 
    0x8F84B8D0, 0x79E706F8, 0x3479B094, 0x50300808, 
    0x2F87F057, 0xFF63E677, 0x7CF8FFD6, 0x972BFE74, 
    0x84ACB5D9, 0x5B6474C4, 0x9B8D5B46, 0x87E3EF7B, 
    0x45104DE3, 0xB3623F61, 0xFE67F370, 0xBDB8ADE6, 
    0x630C4027, 0x75787826, 0x5F7B564F, 0x69E6B03A, 
    0x004064B0, 0xB40F67FF, 0x8B339E50, 0x1A9F585D, 
    0x1221E7FE, 0x59327533, 0x8C4F436A, 0x29B728FE, 
    0xECC65CE7, 0x548D7E69, 0x0F8B6863, 0xB4620B65, 
    0x4018BCB6, 0x0545075D, 0x2EFD4224, 0x3A86B77B, 
    0x48D50577, 0xB10852D7, 0xC899D4B6, 0x2E209208, 
    0xE32CE169, 0xE580B58D, 0xC6649736, 0x04026E01, 
    0xD4F3853B, 0xBE66DBFE, 0x3A2A691E, 0xC08489C6, 
    0x40B9C5A5, 0x8CE8E99B, 0x4081BC7D, 0xC58E077C, 
    0x736CE7D4, 0xB9CB8F42, 0x7A9983BD, 0x744AEA59
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
     
uint32_t sipHash32(const uint8_t *in, const siphash_size_t inlen, const uint32_t key[SIP32_32BIT_KEYS])
{
    uint32_t v0, v1, v2, v3;
    uint32_t k0, k1;
    uint32_t m, b;
    siphash_size_t i;
    
    const uint8_t *end4aligned_pt = in + inlen - (inlen % sizeof(uint32_t));
    const uint8_t left = inlen & LS_2_BITS_MASK;
    
    b = ((uint32_t)inlen) << 24;
    
    /*get keys*/
    k0 = key[0];
    k1 = key[1];
    
    v0 = SIP_HASH_VAR0_INIT;
    v1 = SIP_HASH_VAR1_INIT;
    v2 = SIP_HASH_VAR2_INIT;
    v3 = SIP_HASH_VAR3_INIT;
    
    /*scramble keys and calculation variables*/
    v3 ^= k1;
    v2 ^= k0;
    v1 ^= k1;
    v0 ^= k0;

    /* process quad byte groups of input data */
    for (; end4aligned_pt != in; in += 4) {
        bytesToUint32_m(m, in, 0);
        v3 ^= m;

        for (i = 0; i < SIP_HASH_C_ROUNDS; ++i)
        {
            sipHashProcess_m;
        }

        v0 ^= m;
    }
    
    /* process ramaining input bytes */
    switch (left) 
    {
        case 3:
            b |= ((uint32_t)in[2]) << 16;
        case 2:
            b |= ((uint32_t)in[1]) << 8;
        case 1:
            b |= ((uint32_t)in[0]);
            break;
        case 0:
            break;
    }

    v3 ^= b;

    for (i = 0; i < SIP_HASH_C_ROUNDS; ++i)
    {
        sipHashProcess_m;
    }

    v0 ^= b;
    v2 ^= BYTE_MASK;

    for (i = 0; i < SIP_HASH_D_ROUNDS; ++i)
    {
        sipHashProcess_m;
    }
    
    /*calculate final hash*/
    b = v1 ^ v3;
    
    return b;
}

bool sipHashTest(void)
{
    uint8_t i;
    uint8_t in[SIP32_TEST_HASHES], k[SIP32_KEY_BYTES];
    uint32_t keys[SIP32_32BIT_KEYS];
    uint32_t hash;
    bool retVal;
    
    retVal = true;
    /* generate test keys */
    for (i = 0; i < SIP32_KEY_BYTES; ++i)
    {
        k[i] = i;
    }
    
    for(i=0; i<SIP32_32BIT_KEYS; i++)
    {
        bytesToUint32_m(keys[i],k,i*4);
    }
    
    /* calculate hashes for test data */
    for (i = 0; i < SIP32_TEST_HASHES; ++i) 
    {
        in[i] = i;  /*set test data */
        hash = sipHash32(in, i, keys); /* calculate hash */
        
        if(sip32TestHashVactor[i] != hash) /* check hashes */
        {
            printf("SIP#ERR %d: %08X\n", i, hash);
            retVal = false;
        }
        
    }

    if(true == retVal)
    {
        printf("SIP Test OK...\n");
    }
    
    return retVal;
}

/******************************************************************************
**                               End Of File
*******************************************************************************/