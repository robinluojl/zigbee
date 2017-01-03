#ifndef _MYDATAPROCESS_H
#define _MYDATAPROCESS_H

#include "math.h"
#include <stdio.h>
#include <stdint.h>
#include <cstring>

typedef int8_t s8;
typedef uint8_t u8;
typedef uint16_t u16;
typedef int16_t s16;

void EncodeDoubleData(double* f, unsigned char* buff);
void DecodeDoubleData(double* f, unsigned char* buff);

void EncodeFloatData(float* f, unsigned char* buff);
void DecodeFloatData(float* f, unsigned char* buff);

void EncodeS32Data(int32_t* f, unsigned char* buff);
void DecodeS32Data(int32_t* f, unsigned char* buff);

void EncodeS16Data(s16* f, unsigned char* buff);
void DecodeS16Data(s16* f, unsigned char* buff);

void EncodeU16Data(u16* f, unsigned char* buff);
void DecodeU16Data(u16* f, unsigned char* buff);

void EncodeU12Data(u16* f, unsigned char* buff);
void DecodeU12Data(u16* f, unsigned char* buff);

void EncodeU16to5Dec(u16* f, unsigned char* buff);

#endif
