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
 * 		CRC-16-CCITT is an error detection scheme that does not impose any 
 * 		additional transmission overhead. This scheme was first employed by IBM 
 * 		in its SDLC data link protocol and is used today in other modern data 
 * 		link protocols such as HDLC, SS7, and ISDN. Like a checksum, the CCITT-CRC 
 * 		does not impose any additional transmission overhead at the character level.
 * 		It can detect errors in any arbitrary number of bits of data, and its 
 * 		error detection rate is 99.9984 percent, worse case.
 * 		The basic idea is to treat the entire message as a binary number, which 
 * 		both the sender and receiver divide using the same divisor. The quotient 
 * 		is discarded, and the remainder is sent as the CRC. If the message is 
 * 		received without error, the receiver's calculation will match the 
 * 		sender's calculation, and the CRC's will agree. The CRC is actually 
 * 		the one's complement of the remainder obtained from modulo 2 division 
 * 		of the message by a generation polynomial.
 * 		The CCITT-CRC uses polynomial: x16 + x12 + x5 + 1
 *
 * 		Implemented as described on: https://ghsi.de/CRC/index.php
 * 
 * Generating polinoomial: 0x1021 - normal representation x^16 omitted
 *
 * Dependences:
 *
 * Revision history:
 *  	KG	02.02.2017 
 *      
 ************************************************************************************************/

#include <stdio.h>
#include "crc16.h"

uint16_t crc16_lut[256];			//used for runtime CRC LUT generation

const uint16_t crc_ccitt_table[256] = {
    0X0000, 0X1021, 0X2042, 0X3063, 0X4084, 0X50A5, 0X60C6, 0X70E7, 
    0X8108, 0X9129, 0XA14A, 0XB16B, 0XC18C, 0XD1AD, 0XE1CE, 0XF1EF, 
    0X1231, 0X0210, 0X3273, 0X2252, 0X52B5, 0X4294, 0X72F7, 0X62D6, 
    0X9339, 0X8318, 0XB37B, 0XA35A, 0XD3BD, 0XC39C, 0XF3FF, 0XE3DE, 
    0X2462, 0X3443, 0X0420, 0X1401, 0X64E6, 0X74C7, 0X44A4, 0X5485, 
    0XA56A, 0XB54B, 0X8528, 0X9509, 0XE5EE, 0XF5CF, 0XC5AC, 0XD58D, 
    0X3653, 0X2672, 0X1611, 0X0630, 0X76D7, 0X66F6, 0X5695, 0X46B4, 
    0XB75B, 0XA77A, 0X9719, 0X8738, 0XF7DF, 0XE7FE, 0XD79D, 0XC7BC, 
    0X48C4, 0X58E5, 0X6886, 0X78A7, 0X0840, 0X1861, 0X2802, 0X3823, 
    0XC9CC, 0XD9ED, 0XE98E, 0XF9AF, 0X8948, 0X9969, 0XA90A, 0XB92B, 
    0X5AF5, 0X4AD4, 0X7AB7, 0X6A96, 0X1A71, 0X0A50, 0X3A33, 0X2A12, 
    0XDBFD, 0XCBDC, 0XFBBF, 0XEB9E, 0X9B79, 0X8B58, 0XBB3B, 0XAB1A, 
    0X6CA6, 0X7C87, 0X4CE4, 0X5CC5, 0X2C22, 0X3C03, 0X0C60, 0X1C41, 
    0XEDAE, 0XFD8F, 0XCDEC, 0XDDCD, 0XAD2A, 0XBD0B, 0X8D68, 0X9D49, 
    0X7E97, 0X6EB6, 0X5ED5, 0X4EF4, 0X3E13, 0X2E32, 0X1E51, 0X0E70, 
    0XFF9F, 0XEFBE, 0XDFDD, 0XCFFC, 0XBF1B, 0XAF3A, 0X9F59, 0X8F78, 
    0X9188, 0X81A9, 0XB1CA, 0XA1EB, 0XD10C, 0XC12D, 0XF14E, 0XE16F, 
    0X1080, 0X00A1, 0X30C2, 0X20E3, 0X5004, 0X4025, 0X7046, 0X6067, 
    0X83B9, 0X9398, 0XA3FB, 0XB3DA, 0XC33D, 0XD31C, 0XE37F, 0XF35E, 
    0X02B1, 0X1290, 0X22F3, 0X32D2, 0X4235, 0X5214, 0X6277, 0X7256, 
    0XB5EA, 0XA5CB, 0X95A8, 0X8589, 0XF56E, 0XE54F, 0XD52C, 0XC50D, 
    0X34E2, 0X24C3, 0X14A0, 0X0481, 0X7466, 0X6447, 0X5424, 0X4405, 
    0XA7DB, 0XB7FA, 0X8799, 0X97B8, 0XE75F, 0XF77E, 0XC71D, 0XD73C, 
    0X26D3, 0X36F2, 0X0691, 0X16B0, 0X6657, 0X7676, 0X4615, 0X5634, 
    0XD94C, 0XC96D, 0XF90E, 0XE92F, 0X99C8, 0X89E9, 0XB98A, 0XA9AB, 
    0X5844, 0X4865, 0X7806, 0X6827, 0X18C0, 0X08E1, 0X3882, 0X28A3, 
    0XCB7D, 0XDB5C, 0XEB3F, 0XFB1E, 0X8BF9, 0X9BD8, 0XABBB, 0XBB9A, 
    0X4A75, 0X5A54, 0X6A37, 0X7A16, 0X0AF1, 0X1AD0, 0X2AB3, 0X3A92, 
    0XFD2E, 0XED0F, 0XDD6C, 0XCD4D, 0XBDAA, 0XAD8B, 0X9DE8, 0X8DC9, 
    0X7C26, 0X6C07, 0X5C64, 0X4C45, 0X3CA2, 0X2C83, 0X1CE0, 0X0CC1, 
    0XEF1F, 0XFF3E, 0XCF5D, 0XDF7C, 0XAF9B, 0XBFBA, 0X8FD9, 0X9FF8, 
    0X6E17, 0X7E36, 0X4E55, 0X5E74, 0X2E93, 0X3EB2, 0X0ED1, 0X1EF0
};
    
uint16_t crc16_calc(uint8_t *data, uint16_t length, crc_mode_e mode)
{

    uint16_t count;
    uint32_t crc = CRC16_SEED;
    uint32_t temp;
    
    if(mode == REVERSE) //reverse calculation 
    {
        uint8_t *pt;
        pt = data + length -1;
        for (count = 0; count < length; ++count)
        {
#ifdef CRC_DEBUG
            PRINTF("\n%X ", *pt);
#endif
            temp = ((*pt--) ^ (crc >> 8)) & 0xFF;
        //		crc = crc16_lut[temp] ^ (crc << 8);		//read from runtime pre calculated table
            crc = crc_ccitt_table[temp] ^ (crc << 8);	//read from pre compiled LUT
        }	
    } else //normal calculation
    {
        for (count = 0; count < length; ++count)
        {
#ifdef CRC_DEBUG
            PRINTF("\n%X ", *data);
#endif			
            temp = ((*data++) ^ (crc >> 8)) & 0xFF;
        //		crc = crc16_lut[temp] ^ (crc << 8);		//read from runtime pre calculated table
            crc = crc_ccitt_table[temp] ^ (crc << 8);	//read from pre compiled LUT
        }
    }
#ifdef CRC_DEBUG
            PRINTF("\n%X ", (uint16_t)(crc ^ CRC16_FINAL_XOR));
#endif
    return (uint16_t)(crc ^ CRC16_FINAL_XOR);
}

/*********************************************************************
 *
 * Function:    crcInit()
 *
 * Description: Populate the partial CRC lookup table.
 *
 * Notes:		This function must be rerun any time the CRC standard
 *				is changed.  If desired, it can be run "offline" and
 *				the table results stored in an embedded system's ROM.
 *
 * Returns:		None defined.
 *
 *********************************************************************/

void crc16_init(void)
{
    uint16_t remainder;
    uint32_t dividend;
    unsigned char bit;

    for (dividend = 0; dividend < 256; ++dividend)		//Compute the remainder of each possible dividend
    {
        remainder = dividend << (WIDTH_16 - 8);					//Start with the dividend followed by zeros

        for (bit = 8; bit > 0; --bit)										//Perform modulo-2 division, a bit at a time
        {
            if (remainder & TOPBIT_16)										//Try to divide the current data bit
            {
                remainder = (remainder << 1) ^ POLYNOMIAL_16;
            }
            else
            {
                remainder = (remainder << 1);
            }
        }
        crc16_lut[dividend] = remainder;				//Store the result into the table.
    }
}
