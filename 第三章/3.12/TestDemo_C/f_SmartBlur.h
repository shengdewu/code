
/*************************************************************************
Copyright:   HZ.
Author:		 Hu Yaowu
Date:		 2018-4-23
Mail:        dongtingyueh@163.com
Description: Smart Blur .
Refference: None
*************************************************************************/
#ifndef __T_SMART_BLUR__
#define __T_SMART_BLUR__


#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

/*************************************************************************
*Function: Smart Blur
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*radius: radius of filter, [0,+]
*threshold: threshold of pixels to count,[0,255]
*Return: 0-OK,other failed
**************************************************************************/
EXPORT int f_SmartBlur(unsigned char* srcData, int width, int height, int stride, int radius, int threshold);


#else

#ifdef __cplusplus
extern "C" {
#endif    

    int f_SmartBlur(unsigned char* srcData, int width, int height, int stride, int radius, int threshold);

#ifdef __cplusplus
}
#endif


#endif



#endif
