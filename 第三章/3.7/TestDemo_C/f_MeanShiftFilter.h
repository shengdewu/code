
/*************************************************************************
Copyright:   HZ.
Author:		 Hu Yaowu
Date:		 2018-4-23
Mail:        dongtingyueh@163.com
Description: MeanShift Filter .
Refference: None
*************************************************************************/
#ifndef __T_MEAN_SHIFT__
#define __T_MEAN_SHIFT__


#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

/*************************************************************************
*Function: MeanShift Filter
*Params:
*srcData:32BGRA image buffer
*nWidth:  width of image
*nHeight: height of image
*nStride: Stride of image
*radius: radius of filter, [0,+]
*threshold: threshold of pixels to count,[0,255]
*maxIter: max iteration number of filter
*Return: 0-OK,other failed
**************************************************************************/
EXPORT int f_MeanShiftFilter(unsigned char* srcData, int width, int height, int stride, int radius, int threshold, int maxIter);


#else

#ifdef __cplusplus
extern "C" {
#endif    

    int f_MeanShiftFilter(unsigned char* srcData, int width, int height, int stride, int radius, int threshold, int maxIter);

#ifdef __cplusplus
}
#endif


#endif



#endif
