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
 *          TODO: provjeriti razliku izmedju little endian i big endian
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

#include "fletcher32.h"
#include "helper_functions.h"

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/*test data sets*/
static uint8_t test[] = "abcdefgh";

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
* @returns 
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

/******************************************************************************
**                               End Of File
*******************************************************************************/


