/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file .c
 *
 * @author Kristijan Golub - kristijan.golub@globallogic.com
 *
 * @date 2019-12-20
 * 
 * @brief   Efficient Flacher32 check sum algorithm. Implemented as described at
 *          https://en.wikipedia.org/wiki/Fletcher%27s_checksum .This reduces to the 
 *          range 1..65535 rather than 0..65534. Modulo 65535, the values 65535 = 0xffff 
 *          and 0 are equivalent, but it is easier to detect overflow if the former 
 *          convention is used. This also provides the guarantee that the resultant 
 *          checksum will never be zero, so that value is available for a special flag, 
 *          such as "checksum not yet computed".
 *          65536 ? 1 mod 65535, so the end-around carry expression (x & 0xffff) + (x >> 16) 
 *          reduces x modulo 65535. Only doing it once is not guaranteed to be complete, but 
 *          it will be in the range 1..0x1fffe. A second repetition guarantees a fully 
 *          reduced sum in the range of 1..0xffff.
 *          This uses a 32-bit accumulator to perform a number of sums before doing any 
 *          modular reduction. The magic value 359 is the largest number of sums that
 *          can be performed without numeric overflow, given the possible initial starting 
 *          value of sum1 = 0x1fffe. Any smaller value is also permissible; 256 may be 
 *          convenient in many cases. The limit is 360 if starting from sum1 = 0xffff, 
 *          but the example code only partially reduces sum1 between inner loops. 
 *          @todo implement endian conversion - swapp byte order in 16bit input data
 *
 * @version 0.1
 *
 * @section REVISION HISTORY
 *  - KG 2019-12-20 Initial implementation 
 *
 ******************************************************************************/

 
/*******************************************************************************
**                                Includes
*******************************************************************************/

#include "fletcher.h"
#include "helper_functions.h"

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/*******************************************************************************
**                                 Code
*******************************************************************************/

/*!*****************************************************************************
* @method 
* 
* @brief 
*
* @param 
*
* @return
*******************************************************************************/

uint32_t fletcher32(uint16_t const *data, int words)
{
    uint32_t sum1, sum2;
    uint16_t blockSize, i;
    
    sum1 = sum2 = FCS32_INITIAL_SUM;
    
    while (words > 0) 
    {
        blockSize = min_m(words, FCS32_ACCUMULATOR_WORDS);
        
        for(i=0; i<blockSize; i++)  
        {
            sum1 += *data++;
            sum2 += sum1;
        }
        
        /* reduce sums to 16 bits */
        sum1 = (sum1 & FCS32_16BIT_MASK) + (sum1 >> 16);
        sum2 = (sum2 & FCS32_16BIT_MASK) + (sum2 >> 16);
        
        words -= blockSize;  /* count processed words */
    }
    /* Second reduction step to guarantie reduced sums to 16 bits */
    sum1 = (sum1 & FCS32_16BIT_MASK) + (sum1 >> 16);
    sum2 = (sum2 & FCS32_16BIT_MASK) + (sum2 >> 16);

    return ((sum2 << 16) | sum1);
}

bool fletcher32Test(void)
{
    /* test are done assuming Litle Endian byte order from ASCII string input data 
     *  \0 is natively used for padding 16bit input data*/
    uint8_t testData0[] = "abcde";
    uint8_t testData1[] = "abcdef"; 
    uint8_t testData2[] = "abcdefgh";
    
    flatcehr32Test_t testArray[] = 
    {
        {   /*test structure element 0*/
            .testData = (uint16_t*)testData0,
            .dataWordsNo = sizeof(testData0)/sizeof(uint16_t), 
            .testResult = 0xF04FC729
        },
        {   /*test structure element 1*/
            .testData = (uint16_t*)testData1,
            .dataWordsNo = sizeof(testData1)/sizeof(uint16_t), 
            .testResult = 0x56502D2A
        },
        {   /*test structure element 2*/
            .testData = (uint16_t*)testData2,
            .dataWordsNo = sizeof(testData2)/sizeof(uint16_t), 
            .testResult = 0xEBE19591
        }        
    };
    
    const uint32_t testElements = sizeof(testArray)/sizeof(flatcehr32Test_t);
    uint32_t  passedTests, i;
    
    passedTests = 0;
    
    for(i = 0; i<testElements; i++)
    {
        if(testArray[i].testResult == fletcher32(testArray[i].testData, testArray[i].dataWordsNo))
        {
            passedTests++;
        }
    }
    
    return isEqual_m(testElements, passedTests);
}

/******************************************************************************
**                               End Of File
*******************************************************************************/


