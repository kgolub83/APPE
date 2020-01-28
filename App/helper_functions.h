/*!*******************************Copyright (c)*********************************
 *                                GlobalLogic
 * 
 * @file .h
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

#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

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

#define BYTE_MASK           0x000000FF
#define BYTE_BITS           8U
#define	WORD16_MASK         0x0000FFFF
#define WORD16_MSBYTE_MASK  0x0000FF00
#define WORD32_MASK         0xFFFFFFFF
#define WORD16_BITS         16U
#define WORD32_BITS         32U
#define LOW_NIBBLE_MASK     0x0F
#define HI_NIBBLE_MASK      0xF0
#define NIBBLE_BITS         4U

#define LS_12_BITS_MASK     0x00000FFF
#define LS_11_BITS_MASK     0x000007FF
#define LS_10_BITS_MASK     0x000003FF
#define LS_9_BITS_MASK      0x000001FF
#define LS_8_BITS_MASK      0x000000FF
#define LS_7_BITS_MASK      0x0000007F
#define LS_6_BITS_MASK      0x0000003F
#define LS_5_BITS_MASK      0x0000001F
#define LS_4_BITS_MASK      0x0000000F
#define LS_3_BITS_MASK      0x00000007
#define LS_2_BITS_MASK      0x00000003

/*******************************************************************************
**                            Data Structures
*******************************************************************************/

typedef int signalVar_t;

typedef enum 
{
    SIGNAL_IN_RANGE = 0,
    SIGNAL_OVER,
    SIGNAL_UNDER,
    SIGNAL_PEAK,
    SIGNAL_IDLE,
    SIGNAL_FAULT
} signalCheck_e;

typedef union{
    struct{
        uint8_t b0 :1;
        uint8_t b1 :1;
        uint8_t b2 :1;
        uint8_t b3 :1;
        uint8_t b4 :1;
        uint8_t b5 :1;
        uint8_t b6 :1;
        uint8_t b7 :1;
        uint8_t b8 :1;
        uint8_t b9 :1;
        uint8_t b10 :1;
        uint8_t b11 :1;
        uint8_t b12 :1;
        uint8_t b13 :1;
        uint8_t b14 :1;
        uint8_t b15 :1;
        uint8_t b16 :1;
        uint8_t b17 :1;
        uint8_t b18 :1;
        uint8_t b19 :1;
        uint8_t b20 :1;
        uint8_t b21 :1;
        uint8_t b22 :1;
        uint8_t b23 :1;
        uint8_t b24 :1;
        uint8_t b25 :1;
        uint8_t b26 :1;
        uint8_t b27 :1;
        uint8_t b28 :1;
        uint8_t b29 :1;
        uint8_t b30 :1;
        uint8_t b31 :1;
    } bit;
    volatile uint32_t reg;
} reg32_bits_t;

/*******************************************************************************
**                       Global and static variables
*******************************************************************************/

/*******************************************************************************
**                                Macros
*******************************************************************************/

/*checks if var is in between min and max including boundaries*/ 
#define isInRange_m(var, min, max) ((((var) >= (min)) && ((var) <= (max))) ? true : false)

/*checks if variables a and b are equal*/
#define isEqual_m(a,b) (((a) == (b)) ? true : false)

/*returns a if a is smaller then b, in other cases returns b*/
#define min_m(a,b) (((a) < (b)) ? (a) : (b))

/*returns a if a is greater then b, in other cases returns b*/
#define max_m(a,b) (((a) > (b)) ? (a) : (b))

/* rotates 32 bit variable var for bitsNo number of bits*/
#define rotLeft32_m(var, bitsNo) (uint32_t)(((var) << (bitsNo)) | ((var) >> (32 - (bitsNo))))

#define bytesToUint32_m(var32, bytes, index)            \
{                                                       \
(var32) = ( (uint32_t) (bytes)[(index)    ]       )     \
        | ( (uint32_t) (bytes)[(index) + 1] <<  8 )     \
        | ( (uint32_t) (bytes)[(index) + 2] << 16 )     \
        | ( (uint32_t) (bytes)[(index) + 3] << 24 );    \
}

#define bytesToUint32Reverse_m(var32, bytes, index)     \
{                                                       \
(var32) = ( (uint32_t) (bytes)[(index)    ] << 24 )     \
        | ( (uint32_t) (bytes)[(index) + 1] << 16 )     \
        | ( (uint32_t) (bytes)[(index) + 2] <<  8 )     \
        | ( (uint32_t) (bytes)[(index) + 3]       );    \
}

#define uint32ToBytes_m(var32, bytes, index)       \
{                                                       \
    (bytes)[(index)    ] = (uint8_t) ( (var32)       ); \
    (bytes)[(index) + 1] = (uint8_t) ( (var32) >>  8 ); \
    (bytes)[(index) + 2] = (uint8_t) ( (var32) >> 16 ); \
    (bytes)[(index) + 3] = (uint8_t) ( (var32) >> 24 ); \
}

#define uint32ToBytesReverse_m(var32, bytes, index)       \
{                                                       \
    (bytes)[(index)    ] = (uint8_t) ( (var32) >> 24 ); \
    (bytes)[(index) + 1] = (uint8_t) ( (var32) >> 16 ); \
    (bytes)[(index) + 2] = (uint8_t) ( (var32) >>  8 ); \
    (bytes)[(index) + 3] = (uint8_t) ( (var32)       ); \
}

/*******************************************************************************
**                     Public function prototypes - API
*******************************************************************************/

extern bool isInRange (const int var, int min, int max);
extern signalCheck_e signalCheck(const signalVar_t var, const signalVar_t min, const signalVar_t max);

#ifdef __cplusplus
} /* extern "C" */
#endif /* nextern "C" */

#endif /* HELPER_FUNCTIONS_H */


/******************************************************************************
**                               End Of File
*******************************************************************************/


