/*************************************************
Copyright:  Copyright HZ.
Author:		Hu Yaowu
Date:		2015-04-21
Mail:       dongtingyueh@163.com
Description:RGB to YCbCr.
**************************************************/
#ifndef TRGB2YCbCr
#define TRGB2YCbCr
#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void RGBToYCbCr(int R, int G, int B, int*Y,int*Cb, int* Cr);
void YCbCrToRGB(int Y, int Cb, int Cr, int*R,int*G, int* B);
#endif