/***************************************Copyright (c)*********************************************
 *
 * 
 * 
 * File:   crc16.c
 *
 * Author: Kristijan Golub
 *
 * Date: 2017-02-02
 * 
 * Description:
 *      CRC-16-CCITT is an error detection scheme that does not impose any 
 *      additional transmission overhead. This scheme was first employed by IBM 
 *      in its SDLC data link protocol and is used today in other modern data 
 *      link protocols such as HDLC, SS7, and ISDN. Like a checksum, the CCITT-CRC 
 *      does not impose any additional transmission overhead at the character level.
 *      It can detect errors in any arbitrary number of bits of data, and its 
 *      error detection rate is 99.9984 percent, worse case.
 *      The basic idea is to treat the entire message as a binary number, which 
 *      both the sender and receiver divide using the same divisor. The quotient 
 *      is discarded, and the remainder is sent as the CRC. If the message is 
 *      received without error, the receiver's calculation will match the 
 *      sender's calculation, and the CRC's will agree. The CRC is actually 
 *      the one's complement of the remainder obtained from modulo 2 division 
 *      of the message by a generation polynomial.
 *      The CCITT-CRC uses polynomial: x16 + x12 + x5 + 1
 *
 *      Implemented as described on: http://www.ghsi.de/pages/subpages/Online%20CRC%20Calculation/
 * 
 * Generating polynomial: 0x1021 - normal representation x^16 omitted
 *
 * Dependences:
 *
 * Revision history:
 *      KG  02.02.2017 
 *      
 ************************************************************************************************/

#ifndef CRC16_H
#define CRC16_H

#include <stdint.h> 
#include <stdbool.h>

/*#define CRC_DEBUG  turn on debugging routines*/

#define CRC16_SEED          0x0000      /*Seed for CRC calculation 0x0000 default*/
#define CRC16_FINAL_XOR     0x0000      /*Final CRC XORing number 0x0000 default*/
#define POLYNOMIAL_16       0x1021      /*Polynomial CCITT x16+x12+x5+1 - normal representation x^16 omitted*/
/*#define POLYNOMIAL        0x3D65      Polynomial DNP x16 + x13 + x12 + x11 + x10 + x8 + x6 + x5 + x2 + 1  - normal representation x^16 omitted*/
#define WIDTH_16            16
#define TOPBIT_16           (1 << (WIDTH_16 - 1))

typedef enum
{
    NORMAL,     /*MSB first - from BIG ENDIAN perspective*/
    REVERSE     /*LSB first - from BIG ENDIAN perspective*/
} crc_mode_e;

extern void crc16LutGenerate(void);
extern uint16_t crc16Calculate(uint8_t *data, uint16_t length, crc_mode_e mode);
extern bool crc16Test(void);

#endif /*CRC16_H*/
