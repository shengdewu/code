
/*************************************************************************
Copyright:   HZ.
Author:		 Hu Yaowu
Date:		 2018-4-23
Mail:        dongtingyueh@163.com
Description: __T_BILATERAL_FILTER__ .
*************************************************************************/
#ifndef __T_BILATERAL_FILTER__
#define __T_BILATERAL_FILTER__


#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif
/******************************************************************
*Function: Bilatera filter
*Params:
*srcData: 32BGRA image data
*width: width of image
*height: height of image
*stride: Stride of image
*radius: filter radius, [0,]
*std: [0,100]
*******************************************************************/
EXPORT int f_Bilateralfilter(unsigned char* srcData, int width, int height, int stride, int radius, double std);


#else

#ifdef __cplusplus
extern "C" {
#endif    

    int f_Bilateralfilter(unsigned char* srcData, int width, int height, int stride, int radius, double std);

#ifdef __cplusplus
}
#endif


#endif



#endif
