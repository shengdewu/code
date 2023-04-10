/*************************************************************************
Copyright:   HZ.
Author:		 Hu Yaowu
Date:		 2015-4-23
Mail:        dongtingyueh@163.com
Description: IMAGE ENGINE
*************************************************************************/
#ifndef __SF_ENGINE__
#define __SF_ENGINE__
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MIN2(a, b) ((a) < (b) ? (a) : (b))
#define MAX2(a, b) ((a) > (b) ? (a) : (b))
#define CLIP3(x, a, b) MIN2(MAX2(a,x), b)

#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif
EXPORT int SF_MNN_Initialize(char* modelpath, int tensorSize, int stdMode, int numThread);
EXPORT void SF_MNN_Unitialize();
EXPORT int SF_NailsColored(unsigned char* srcData, int width, int height, int stride, int R, int G, int B, int ratio);

#else

#ifdef __cplusplus
extern "C" {
#endif    
	int SF_MNN_Initialize(char* modelpath, int tensorSize, int stdMode, int numThread);
	void SF_MNN_Unitialize();
	int SF_NailsColored(unsigned char* srcData, int width, int height, int stride, int R, int G, int B, int ratio);
	}
#endif
#endif

