// filename: utils.h

#ifndef __UTILS_H__
#define __UTILS_H__

#include<reg52.h>
#define uchar unsigned char;
void sleep(unsigned int xms);
void resetT0();

void processInput(unsigned char a);
uchar displayNum(int num);