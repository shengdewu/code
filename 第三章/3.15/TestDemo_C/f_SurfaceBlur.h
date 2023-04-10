
/*************************************************************************
Copyright:   HZ.
Author:		 Hu Yaowu
Date:		 2018-4-23
Mail:        dongtingyueh@163.com
Description: Surface blur .
*************************************************************************/
#ifndef __T_SURFACE_BLUR__
#define __T_SURFACE_BLUR__


#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

/*****************************************************
*Function: Surface blur
*Params:
*srcData-32BGRA image data
*width-width of image
*height-height of image
*stride-Stride of image
*radius-radius of surface blur, [0,100]
*threshold-Threshold of surface blur, [0,255]
*Return: 0-OK, or failed.
******************************************************/
EXPORT int f_SurfaceBlur(unsigned char* srcData, int width, int height, int stride, int radius, int threshold);


#else

#ifdef __cplusplus
extern "C" {
#endif    

    int f_SurfaceBlur(unsigned char* srcData, int width, int height, int stride, int radius, int threshold);

#ifdef __cplusplus
}
#endif


#endif



#endif
