/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file .h
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
**                                 Guards
*******************************************************************************/

#ifndef FLETCHER32_H
#define FLETCHER32_H

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

#define FCS32_ACCUMULATOR_WORDS      359U
#define FCS32_16BIT_MASK             0xFFFFU
#define FCS32_INITIAL_SUM            0U

/*******************************************************************************
**                                    Macros
*******************************************************************************/

/*******************************************************************************
**                            Data Structures
*******************************************************************************/

typedef struct
{
    const uint32_t testResult;    
    const uint32_t dataWordsNo;
    const uint16_t *testData;    
} flatcehr32Test_t;

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/*******************************************************************************
**                     Public function prototypes - API
*******************************************************************************/

extern uint32_t fletcher32(uint16_t const *data, int words);
extern bool fletcher32Test(void);

#ifdef __cplusplus
} /* extern "C" */
#endif /* nextern "C" */

#endif /* FLETCHER32_H */

/******************************************************************************
**                               End Of File
*******************************************************************************/
