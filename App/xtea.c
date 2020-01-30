/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file xtea.c
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
**                                Includes
*******************************************************************************/

#include "xtea.h"
#include "helper_functions.h"
#include <stdio.h>

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/* XTEA test vectors - results for one hot coded keys */
static const uint64_t xteaKeyTest[] =
{
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

/* XTEA test vectors - results for one hot coded data */
static const uint64_t xteaDataTest[] = 
{  
    0x37A3CBD9F11C28D5, 0xBA13E75AC2A5A822, 0x85FC1A1D611EEE20, 0x5FD951CF2C0EE5B3,
    0x8F7BE95C376B4AE8, 0x2E9D3CEE86A265A7, 0x4DA8AE5F1B4C72EB, 0x8104F05BE73B792F,
    0x8CC479C5C796BDF5, 0xDB9624E776FBF690, 0x0010596E95997CF6, 0x1DA11BF5EB203169,
    0xD84C8B01D9B02C98, 0x77380F036974AA17, 0x5AB58FC6A4611DEB, 0x1B968322FFCCD844,
    0x6CBF0995D3378DBC, 0x529348CD78D44B7C, 0xD05A15D62D871060, 0x332426226A827C15,
    0x939DA4F4C06FBB50, 0x0887E46423672943, 0x62A7765FB86D6555, 0x2D1774541561E935,
    0xA387B6CD9D875463, 0x054BD4AE7EB2AB0B, 0xF8581ECE047B514B, 0x9E596C6099197D34,
    0xCF1118300B8A2D38, 0xAAC63D7962E76615, 0xB063F59EF08DEE04, 0xA12E34EE8D374EF5,
    0x04A1EA610CBC468B, 0x82884825FA40F21A, 0x850438051D12E62B, 0xD6091EC280935DEA,
    0xEF9BF9A02A6B0E4E, 0xAD3B49321DCDF009, 0xF36F98736C04059F, 0x45A1900684AD9AC0,
    0x665E0411896D342F, 0x1C15C6F3BBC1C372, 0xB679213660BB0F95, 0x4708A5278930FE0D,
    0xC19F3FC0CBDC242B, 0x38CBBECF65733670, 0x3D285E63FEAAC3B9, 0x3560E4C6D544A15C,
    0x99759A8F624E55E0, 0xA576978E82837779, 0x554466CDF8677820, 0xC32DBB5807791754,
    0xEB4BF9684D3308B9, 0x1CB0BF5C78304C18, 0xCAAB743C19D1B177, 0x981C81403F0B1487,
    0x2234EEFE50CBA46B, 0x7B3797FDCFEBF074, 0x47BEE85DEE699930, 0xA1EAF7CF0476AC6B,
    0xF97364517E73138D, 0x75177A2A6DB3458B, 0x59BBF3ABD6E7B32F, 0x47186468D1D2343D
};

/*******************************************************************************
**                                 Code
*******************************************************************************/

/*!*****************************************************************************
* @function xteaEncrypt
* 
* @brief XTEA encryption algorithm
*
* @param key[] - 128bit encrytion key vector 
* 
* @param input[] - 64bit input data vector 
* 
* @param output[] - 64bit output key vector 
*
* @return void function
*******************************************************************************/

void xteaEncrypt(const uint32_t key[XTEA_32BIT_KEYS], const uint8_t input[XTEA_DATA_BYTES], uint8_t output[XTEA_DATA_BYTES])
{
    uint32_t v0, v1, sum, delta; 
    uint8_t i;

    /*parse input data to 32bit variables*/
    bytesToUint32Reverse_m( v0, input, 0 );
    bytesToUint32Reverse_m( v1, input, 4 );

    /*set initial values*/
    sum = 0; 
    delta = XTEA_DELTA_FACTOR;

    /*run XTEA scrumbling*/
    for( i = 0; i < XTEA_ROUNDS; i++ )
    {
        v0 += (((v1 << XTEA_LSHIFT) ^ (v1 >> XTEA_RSHIFT)) + v1) ^ (sum + key[sum & LS_2_BITS_MASK]);
        sum += delta;
        v1 += (((v0 << XTEA_LSHIFT) ^ (v0 >> XTEA_RSHIFT)) + v0) ^ (sum + key[(sum>>XTEA_SUM_SHIFT) & LS_2_BITS_MASK]);
    }

    /*parse output data*/
    uint32ToBytesReverse_m( v0, output, 0 );
    uint32ToBytesReverse_m( v1, output, 4 );
}

/*!*****************************************************************************
* @function xteaDecrypt
* 
* @brief XTEA decryption algorithm
*
* @param key[] - 128bit encription key vector 
* 
* @param input[] - 64bit input data vector 
* 
* @param output[] - 64bit output key vector 
*
* @return void function
*******************************************************************************/

void xteaDecrypt(const uint32_t key[XTEA_32BIT_KEYS], const uint8_t input[XTEA_DATA_BYTES], uint8_t output[XTEA_DATA_BYTES])
{
    uint32_t v0, v1, sum, delta; 
    uint8_t i;

    /*parse input data to 32bit variables*/
    bytesToUint32Reverse_m( v0, input, 0 );
    bytesToUint32Reverse_m( v1, input, 4 );
    
    /*set initial values*/
    delta = XTEA_DELTA_FACTOR;
    sum = delta * XTEA_ROUNDS;

    /*run XTEA scrumbling*/
    for( i = 0; i < XTEA_ROUNDS; i++ )
    {
        v1 -= (((v0 << XTEA_LSHIFT) ^ (v0 >> XTEA_RSHIFT)) + v0) ^ (sum + key[(sum>>XTEA_SUM_SHIFT) & LS_2_BITS_MASK]);
        sum -= delta;
        v0 -= (((v1 << XTEA_LSHIFT) ^ (v1 >> XTEA_RSHIFT)) + v1) ^ (sum + key[sum & LS_2_BITS_MASK]);
    }
    
    /*parse output data*/
    uint32ToBytesReverse_m( v0, output, 0 );
    uint32ToBytesReverse_m( v1, output, 4 );
}

/*!*****************************************************************************
* @function xteaTest
* 
* @brief XTEA algorithm test function
*
* @return true if all tests finished successfully, false if any test fails
*******************************************************************************/

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
        for(j=0; j<WORD32_BITS; j++)
        {
            testKeys[i] = 1<<(WORD32_BITS-1-j); /*generate test key vector*/
        
            xteaEncrypt(testKeys, testData, encryptedData); /*encrypt data*/
            
            for(k = 0; k<XTEA_DATA_BYTES; k++)    /*check encryption result*/
            {
                if(((uint8_t*)xteaKeyTest)[XTEA_DATA_BYTES-1-k+(j+i*WORD32_BITS)*XTEA_DATA_BYTES] != encryptedData[k])
                {
                    retVal = false;
                    printf("#XTEA KEY#ENCRYPT#ERR\n");
                }
            }
            
            xteaDecrypt(testKeys, encryptedData, decryptedData);    /*decript data*/
            
            for(k=0; k<XTEA_DATA_BYTES; k++)
            {
                if(testData[k] != decryptedData[k])     /*check decription result*/
                {
                    retVal = false;
                    printf("#XTEA KEY#DECRYPT#ERR\n");
                }
            }
        }
        testKeys[i] = 0;
    }
    
    for(i = 0; i<XTEA_32BIT_KEYS; i++)
    {
        testKeys[i] = 0;
    }
    
    /*"one hot" 64bit input data test - each bit active, keys null vector */ 
    for(i = 0; i<XTEA_DATA_BYTES; i++)
    {
        for(j=0; j<BYTE_BITS; j++)
        {
            testData[i] = 1<<(BYTE_BITS-1-j);     /*generate test key vector*/
        
            xteaEncrypt(testKeys, testData, encryptedData);         /*encrypt data*/
            
            for(k = 0; k<XTEA_DATA_BYTES; k++)
            {
                if(((uint8_t*)xteaDataTest)[XTEA_DATA_BYTES-1-k+(j+i*BYTE_BITS)*XTEA_DATA_BYTES] != encryptedData[k]) /*check encryption result*/
                {
                    retVal = false;
                    printf("#XTEA DATA#ENCRYPT#ERR\n");
                }
            }

            xteaDecrypt(testKeys, encryptedData, decryptedData);    /*decript data*/
            
            for(k=0; k<XTEA_DATA_BYTES; k++)
            {
                if(testData[k] != decryptedData[k])     /*check decription result*/
                {
                    retVal = false;
                    printf("#XTEA DATA#DECRYPT#ERR\n");
                }
            }
        }
        testData[i] = 0;
    }
    
    if(true == retVal)
    {
        printf("XTEA test OK...\n");
    }
    
    return retVal;
}

/******************************************************************************
**                               End Of File
*******************************************************************************/
