// filename: utils.h

#ifndef __UTILS_H__
#define __UTILS_H__

#include<reg52.h>
#define uchar unsigned char;
void sleep(unsigned int xms);
void resetT0();

void processInput(unsigned char a);
//void output_bit(sbit b);
void output_int(unsigned int num);
void output_char(unsigned char ch);
void output_string(unsigned char* str);
unsigned char* int2string(int num);

#endif