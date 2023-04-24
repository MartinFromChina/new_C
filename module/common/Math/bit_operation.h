#ifndef __BIT_OPERATION_H
#define __BIT_OPERATION_H

#ifdef __cplusplus
 extern "C"{
#endif


#include "../x_cross_platform.h" //!!! makefile is sentive to "/" and "\"  , use "/"！！！

#define Set  (0 == 0)
#define Clear (0 != 0)

uint8_t byte_rotate_bit(uint8_t source);// ! careful about the hardware platform ;test arm 32 ok
uint8_t byte_setbit(uint8_t source,uint8_t bitnumber);
uint8_t byte_clearbit(uint8_t source,uint8_t bitnumber);
uint8_t byte_getbit(uint8_t source,uint8_t bitnumber);
uint8_t byte_bitoverturn(uint8_t source,uint8_t bitnumber);
uint8_t ByteFactorSet(uint8_t factor,uint8_t bit_number,X_Boolean SetOrClear);

uint16_t twobyte_setbit(uint16_t source,uint8_t bitnumber);
uint16_t twobyte_clearbit(uint16_t source,uint8_t bitnumber);
uint16_t twobyte_getbit(uint16_t source,uint8_t bitnumber);
uint16_t twobyte_bitoverturn(uint16_t source,uint8_t bitnumber);
uint16_t TwoByteFactorSet(uint16_t factor,uint8_t bit_number,X_Boolean SetOrClear);

uint32_t uint32_rotate_bit(uint32_t source);// ! careful about the hardware platform

uint32_t fourbyte_setbit(uint32_t source,uint8_t bitnumber);
uint32_t fourbyte_clearbit(uint32_t source,uint8_t bitnumber);
uint32_t fourbyte_getbit(uint32_t source,uint8_t bitnumber);
uint32_t fourbyte_bitoverturn(uint32_t source,uint8_t bitnumber);
uint32_t FourByteFactorSet(uint32_t factor,uint8_t bit_number,X_Boolean SetOrClear);

uint8_t GetBinaryBitNumber(uint32_t value);
uint8_t GetLeadZeroCount(uint32_t source);
uint8_t GetRearZeroCount(uint32_t source);

#ifdef __cplusplus
 	}
#endif


#endif
