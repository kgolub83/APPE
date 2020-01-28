/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file main.cpp
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2019-12-20
 * 
 * @brief main function call 
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

#include "xtea.h"
#include "helper_functions.h"
#include <stdio.h>

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

const uint64_t sipTestHash[] = {
0x057E8C0550151937, 0x89598070902CD40E, 0x5FB5B0AD37E48AD6, 0x3AF684A75A5147FB, 
0xD2F3B71258E07935, 0xBB8712D8971F2DA6, 0xA38A09D1C9941692, 0xAC6176429FE586CE, 
0x1C2EDBAD8EB5EDF2, 0xF6E79333A663E1AF, 0x0163BE481B1FA595, 0x55437D3BAC4CBC79, 
0xB37D26C43061FDA1, 0x9DB753BFFB7224B2, 0xF25051857FBD5653, 0xADB76B56DE2AF0CC, 
0xA96996D3376B41B0, 0x26292AA3168A1C8F, 0x2FC8306ECFDB1600, 0x1D0E78EA21F98780, 
0xBBC9E2812B9D2F45, 0xA2F55FD5B13E7006, 0x17B8F1FA6D93C9E2, 0x5DFC2196E32F44E8, 
0x874A60AC1BC1DF62, 0x6B6E03497CF13D3F, 0xD0B0132855E50656, 0xCF06D4A07AC9E646, 
0x85FF754DD36533C7, 0x25C504E6598527C0, 0x3060C4D622544B10, 0xF6D49F47BEBDA63A, 
0x4F190CCFC8DEABFC, 0x1E92DCE665425870, 0x3E32351309A4613B, 0x723EE378CC8335FE, 
0xD74B94A6413DDF0A, 0xD9C38DCF48C27648, 0x6E76FA73E5C1923C, 0x2AA2591E2EF2C12C, 
0x20F3C626EC8DEECE, 0x4C98E9FB0FB19996, 0x4B659BC186112260, 0x93095547E89C06AE, 
0x82A1A38474C48472, 0xA3571F7AE044856E, 0x4F6995F57258D1CD, 0x94B95F169726E564, 
0x56D222C4B3C2C8D6, 0x42B01D4123BFF6F8, 0xF4F916143731BC7A, 0x12878F7E038411ED, 
0x669F06EC05BFB4F3, 0x97A286BE6E272CBB, 0x580C8DC05CF68BAF, 0xB22B78F250141309, 
0x751C895707822EF4, 0x29FB3245F692F76B, 0x781D64FF6E5BC204, 0x86BC4E99C3A08E92, 
0xB7FF2C530C823788, 0x313813ADBEE6108E, 0xDBED436EBC8173A0, 0xE7C6923AB603FF42, 
0x70AEB3D7177D9093, 0x122BFD56040FB665, 0xB61822BFCFFDEB8A, 0x3AA8CAD543C9C29B, 
0x4DB46E34F8CAFDAF, 0x07FAD5CFA9384AE8, 0xE380854EAEA76A96, 0xC071907987323AC4, 
0x383D0B8BCAC923E4, 0x0AAD1D63C2945F73, 0x9F057AAAC3ACD5C4, 0xFA51F81F49AE9D66, 
0xEEA19D0E6E82264B, 0xDEDF335FCBB234B2, 0xE3E532AF56830947, 0x44A5771DD2537110, 
0x3EB533413B8E0A86, 0xBAF9B99B32C96B06, 0x9776223F6F90CBCE, 0x0E4DAFD91F7630FA, 
0xC5C4889F42854948, 0x077CEC6752FA89AD, 0x71F182C8D8A4445A, 0x21E76F7295299598, 
0x413B0F81AC90390C, 0xB750EEAFA754CEC2, 0xF0A8D25E6CF62B7C, 0x579DDACE1B60B3EE, 
0xCFBA07274B8A1F4B, 0x03EB0C8AF495360F, 0xEF1C8C7EAEBC9CA7, 0x6F50086EA5FAC1F8, 
0x5612980DEDC8F6AC, 0x218F4EE454732DAC, 0x1D79D210533D2DF5, 0x6B6006BCE161CA40, 
0x01D970816E5FAAB0, 0x7829BB13E3BEB043, 0xEE0A657F83602539, 0xA05EE2B67549BED1, 
0xC4404652E384E446, 0x7306D364A6E5EF6D, 0x0DF4C1F5FAEB430F, 0x51816E1946ECD779, 
0xB460A5D685B0B2F2, 0xD285EBC617BA85A2, 0x6C633518C5A87E47, 0x281941F8F60D2639, 
0x0BA76EBE02F3BAC0, 0x094B157CDB20BE90, 0x4FB4A4E2C6128D08, 0x2FBE07F7DCFAECC4, 
0x8685885467B52E5C, 0x3BD49DBFB5A5C64D, 0xDDF3D83832DCB3A2, 0xD818E18F03235CD3, 
0xD9702927B9EDFEF5, 0x9CCB14F1F3188CA4, 0xBA898B48743134DE, 0x61982E6F0DDEEE07, 
0xC6C77EBF05B0548F, 0x1A5A806001580FBF, 0x5BED0F1B5A2D1BB0, 0x5BAC83CE2B9DD3CC
};

xteaDataTest{
0x37a3cbd9f11c28d5,
0xba13e75ac2a5a822,
0x85fc1a1d611eee20,
0x5fd951cf2c0ee5b3,
0x8f7be95c376b4ae8,
0x2e9d3cee86a265a7,
0x4da8ae5f1b4c72eb,
0x8104f05be73b792f,
0x8cc479c5c796bdf5,
0xdb9624e776fbf690,
0x0010596e95997cf6,
0x1da11bf5eb203169,
0xd84c8b01d9b02c98,
0x77380f036974aa17,
0x5ab58fc6a4611deb,
0x1b968322ffccd844,
0x6cbf0995d3378dbc,
0x529348cd78d44b7c,
0xd05a15d62d871060,
0x332426226a827c15,
0x939da4f4c06fbb50,
0x0887e46423672943,
0x62a7765fb86d6555,
0x2d1774541561e935,
0xa387b6cd9d875463,
0x054bd4ae7eb2ab0b,
0xf8581ece047b514b,
0x9e596c6099197d34,
0xcf1118300b8a2d38,
0xaac63d7962e76615,
0xb063f59ef08dee04,
0xa12e34ee8d374ef5,
0x04a1ea610cbc468b,
0x82884825fa40f21a,
0x850438051d12e62b,
0xd6091ec280935dea,
0xef9bf9a02a6b0e4e,
0xad3b49321dcdf009,
0xf36f98736c04059f,
0x45a1900684ad9ac0,
0x665e0411896d342f,
0x1c15c6f3bbc1c372,
0xb679213660bb0f95,
0x4708a5278930fe0d,
0xc19f3fc0cbdc242b,
0x38cbbecf65733670,
0x3d285e63feaac3b9,
0x3560e4c6d544a15c,
0x99759a8f624e55e0,
0xa576978e82837779,
0x554466cdf8677820,
0xc32dbb5807791754,
0xeb4bf9684d3308b9,
0x1cb0bf5c78304c18,
0xcaab743c19d1b177,
0x981c81403f0b1487,
0x2234eefe50cba46b,
0x7b3797fdcfebf074,
0x47bee85dee699930,
0xa1eaf7cf0476ac6b,
0xf97364517e73138d,
0x75177a2a6db3458b,
0x59bbf3abd6e7b32f,
0x47186468d1d2343d
}

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

/*
 * XTEA encrypt function
 */

void xteaEncrypt(const uint32_t key[XTEA_32BIT_KEYS], const uint8_t input[XTEA_DATA_BYTES], uint8_t output[XTEA_DATA_BYTES])
{
    uint32_t v0, v1, sum, delta; 
    uint8_t i;

    bytesToUint32Reverse_m( v0, input, 0 );
    bytesToUint32Reverse_m( v1, input, 4 );

    sum = 0; 
    delta = XTEA_DELTA_FACTOR;

    for( i = 0; i < XTEA_ROUNDS; i++ )
    {
        v0 += (((v1 << XTEA_LSHIFT) ^ (v1 >> XTEA_RSHIFT)) + v1) ^ (sum + key[sum & LS_2_BITS_MASK]);
        sum += delta;
        v1 += (((v0 << XTEA_LSHIFT) ^ (v0 >> XTEA_RSHIFT)) + v0) ^ (sum + key[(sum>>XTEA_SUM_SHIFT) & LS_2_BITS_MASK]);
    }

    uint32ToBytesReverse_m( v0, output, 0 );
    uint32ToBytesReverse_m( v1, output, 4 );
}

void xteaDecrypt(const uint32_t key[XTEA_32BIT_KEYS], const uint8_t input[XTEA_DATA_BYTES], uint8_t output[XTEA_DATA_BYTES])
{
    uint32_t v0, v1, sum, delta; 
    uint8_t i;

    bytesToUint32Reverse_m( v0, input, 0 );
    bytesToUint32Reverse_m( v1, input, 4 );
    
    delta = XTEA_DELTA_FACTOR;
    sum = delta * XTEA_ROUNDS;

    for( i = 0; i < XTEA_ROUNDS; i++ )
    {
        v1 -= (((v0 << XTEA_LSHIFT) ^ (v0 >> XTEA_RSHIFT)) + v0) ^ (sum + key[(sum>>XTEA_SUM_SHIFT) & LS_2_BITS_MASK]);
        sum -= delta;
        v0 -= (((v1 << XTEA_LSHIFT) ^ (v1 >> XTEA_RSHIFT)) + v1) ^ (sum + key[sum & LS_2_BITS_MASK]);
    }
    
    uint32ToBytesReverse_m( v0, output, 0 );
    uint32ToBytesReverse_m( v1, output, 4 );
}

bool xteaTest(void)
{
    uint8_t testData[XTEA_DATA_BYTES]={0};
    uint8_t encryptedData[XTEA_DATA_BYTES];
    uint8_t decryptedData[XTEA_DATA_BYTES];
    uint32_t testKeys[XTEA_32BIT_KEYS]={0};
    uint8_t i, j, k;
    bool retVal;
    
    retVal = true;
    
    /*"one hot" 128bit key test - each bit active, input data null vector */ 
    for(i = 0; i<XTEA_32BIT_KEYS; i++)
    {
        for(j=0; j<32; j++)
        {
            testKeys[i] = 1<<(31-j); /*generate test data vector*/
        
            xteaEncrypt(testKeys, testData, encryptedData); /*encrypt data*/
            
            for(k = 0; k<8; k++)    /*check encryption result*/
            {
                if(((uint8_t*)sipTestHash)[7-k+(j+i*32)*8] != encryptedData[k])
                {
                    retVal = false;
                    printf("#XTEA#ENCRYPT#ERR\n");
                }
            }
            printf(", ");

            xteaDecrypt(testKeys, encryptedData, decryptedData);    /*decript data*/
            
            for(k=0; k<8; k++)
            {
                if(testData[k] != decryptedData[k]) /*check decription result*/
                {
                    retVal = false;
                    printf("#XTEA#DECRYPT#ERR\n");
                }
            }
        }
        testKeys[i] = 0;
    }
    
    /*"one hot" 64bit input data test - each bit active, keys null vector */ 
    for(i = 0; i<XTEA_32BIT_KEYS; i++)
    {
        for(j=0; j<32; j++)
        {
            testKeys[i] = 1<<(31-j);
        
            xteaEncrypt(testKeys, testData, encryptedData);
            
            for(k = 0; k<8; k++)
            {
                if(((uint8_t*)sipTestHash)[7-k+(j+i*32)*8] != encryptedData[k])
                {
                    retVal = false;
                    printf("#XTEA#ENCRYPT#ERR\n");
                }
            }
            printf(", ");

            xteaDecrypt(testKeys, encryptedData, decryptedData);
            
            for(k=0; k<8; k++)
            {
                if(testData[k] != decryptedData[k])
                {
                    retVal = false;
                    printf("#XTEA#DECRYPT#ERR\n");
                }
            }
        }
        testKeys[i] = 0;
    }
    
    if(true == retVal)l
    {
        printf("XTEA test OK...\n");
    }
    
    return retVal;
}

/******************************************************************************
**                               End Of File
*******************************************************************************/