
/*************************************************************************
Copyright:   HZ.
Author:		 Hu Yaowu
Date:		 2018-4-23
Mail:        dongtingyueh@163.com
Description: BEEPS Filter .
Refference: Bi-Exponential Edge-Preserving Smoother
*************************************************************************/
#ifndef __T_BEEPS_FILTER__
#define __T_BEEPS_FILTER__


#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

/*************************************************************************
*Function: BEEPS Filter
*Params:
*srcData:32BGRA image buffer
*nWidth:  width of image
*nHeight: height of image
*nStride: Stride of image
*radius: radius of filter, [0,+]
*delta: standard deviation of gaussian,[0,+]
*delta_s: spatial contraDecay of lambda,[0,+]
*Return: 0-OK,other failed
**************************************************************************/
EXPORT int f_BeepsFilter(unsigned char* srcData, int nWidth, int nHeight, int nStride, float delta, float delta_s);


#else

#ifdef __cplusplus
extern "C" {
#endif    

   int f_BeepsFilter(unsigned char* srcData, int nWidth, int nHeight, int nStride, float delta, float delta_s);

#ifdef __cplusplus
}
#endif


#endif



#endif
