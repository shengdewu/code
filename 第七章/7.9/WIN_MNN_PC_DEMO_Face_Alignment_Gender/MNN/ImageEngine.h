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
EXPORT int SF_MNN_Initialize(char* fdmodelPath, char* famodelPath, int tensorSize, int stdMode, int numThread);
EXPORT void SF_MNN_Unitialize();
/************************************************************
*Function:  SF_FaceskinSegment
*Description: FaceSkin Segment
*Params:
*srcData:  image bgra data
*width  :image width
*height :image height
*stride :image stride
*isThreshold: 1-binary process, other's not.
*Return :0-OK,or failed
************************************************************/
EXPORT int SF_FaceskinSegment(unsigned char* srcData, int width, int height, int stride, int isThreshold);
/************************************************************
*Function:  SF_FaceDetect
*Description: Face detection
*Params:
*srcData:  image bgra data
*width  :image width
*height :image height
*stride :image stride
*faceRects: face info array, [faceNum, x0, y0, w0, h0, x1, y1, w1, h1, x2, y2, w2, h2, x3, y3, w3, h3].
*Return :0-OK,or failed
************************************************************/
EXPORT int SF_FaceDetect(unsigned char* srcData, int width, int height, int stride, int faceRects[1 + 4 * 4]);
/************************************************************
*Function:  SF_FaceAlignment
*Description: Face alignment
*Params:
*srcData:  image bgra data
*width  :image width
*height :image height
*stride :image stride
*faceRect: face rect info.
*facePoints:face points to output
*Return :0-OK,or failed
************************************************************/
EXPORT int SF_FaceAlignment(unsigned char* srcData, int width, int height, int stride, int faceRect[], float facePoints[]);
/////////////////////////////////////////////////////////////////////////////////////////GENDER////////////////////////////////////////////////////////////////////////////////////////////////
EXPORT int SF_Gender_MNN_Initialize(char* genderModelPath, int stdMode, int numThread);
EXPORT void SF_Gender_MNN_Unitialize();
/************************************************************
*Function:  SF_FaceGender
*Description: Sex detection
*Params:
*srcData:  image bgra data
*width  :image width
*height :image height
*stride :image stride
*outGender: 0-female,1-male.
*Return :0-OK,or failed
************************************************************/
EXPORT int SF_FaceGender(unsigned char* srcData, int width, int height, int stride, float outGender[2]);
#else

#ifdef __cplusplus
extern "C" {
#endif    
	int SF_MNN_Initialize(char* famodelpath, char* famodelPath, int tensorSize, int stdMode, int numThread);
	void SF_MNN_Unitialize();
	int SF_FaceskinSegment(unsigned char* srcData, int width, int height, int stride, int isThreshold);
	int SF_FaceDetect(unsigned char* srcData, int width, int height, int stride, int faceRects[1 + 4 * 4]);
	int SF_FaceAlignment(unsigned char* srcData, int width, int height, int stride, int faceRect[], float facePoints[]);

	int SF_Gender_MNN_Initialize(char* genderModelPath, int stdMode, int numThread);
	void SF_Gender_MNN_Unitialize();
	int SF_FaceGender(unsigned char* srcData, int width, int height, int stride, float outGender[2]);
	}
#endif
#endif

