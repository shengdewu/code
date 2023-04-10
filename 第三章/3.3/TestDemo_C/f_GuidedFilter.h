
/*************************************************************************
Copyright:   HZ.
Author:		 Hu Yaowu
Date:		 2015-4-23
Mail:        dongtingyueh@163.com
Description: Guided Image filter .
Refference: Guided Image filter
*************************************************************************/
#ifndef __T_GUIDED_FILTER__
#define __T_GUIDED_FILTER__


#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif
/*************************************************************************
*Function: Guided filter
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*radius: filter radius,[0,++],default 10
*delta:  delta,[0,++],default 0.003
*Return: 0-OK,other failed
**************************************************************************/
EXPORT int f_GuidedFilter(unsigned char* srcData, int width, int height, int stride, int radius, float delta);


#else

#ifdef __cplusplus
extern "C" {
#endif    

    int f_GuidedFilter(unsigned char* srcData, int width, int height, int stride, int radius, float delta);

#ifdef __cplusplus
}
#endif


#endif



#endif
