/*************************************************************************
Copyright:   HZ.
Author:		 Hu Yaowu
Date:		 2015-4-23
Mail:        dongtingyueh@163.com
Description: Test filter
*************************************************************************/
#ifndef __TESTFILTER__
#define __TESTFILTER__
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
/************************************************************
*Function:  SF_PortraitSegment
*Description: Portrait Segment
*Params:
*srcData:  image bgra data
*width  :image width
*height :image height
*stride :image stride
*isThreshold: 1-binary process, other's not.
*Return :0-OK,or failed
************************************************************/
EXPORT int SF_PortraitSegment(unsigned char* srcData, int width, int height, int stride, int isThreshold);

#else

#ifdef __cplusplus
extern "C" {
#endif    
	int SF_MNN_Initialize(char* modelpath, int tensorSize, int stdMode, int numThread);
	void SF_MNN_Unitialize();
	int SF_PortraitSegment(unsigned char* srcData, int width, int height, int stride, int isThreshold);
	}
#endif
#endif

