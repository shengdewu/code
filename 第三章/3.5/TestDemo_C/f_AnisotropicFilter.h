
/*************************************************************************
Copyright:   HZ.
Author:		 Hu Yaowu
Date:		 2018-4-23
Mail:        dongtingyueh@163.com
Description: Anisotropic Filter .
Refference: Scale-space and edge detection using anisotropic diffusion
*************************************************************************/
#ifndef __T_ANISOTROPIC_FILTER__
#define __T_ANISOTROPIC_FILTER__


#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

/*************************************************************************
*Function: Anisotropic filter
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*iter: iteration num, [0,+],default 7
*k:  coefficient of diffusion, [0,+],default 10
*lambda:cofficient of smoothing, default 0.23
*Return: 0-OK,other failed
**************************************************************************/
EXPORT int f_AnisotropicFilter(unsigned char* srcData, int width, int height, int stride, int iter, float k, float lambda);


#else

#ifdef __cplusplus
extern "C" {
#endif    

    int f_AnisotropicFilter(unsigned char* srcData, int width, int height, int stride, int iter, float k, float lambda);

#ifdef __cplusplus
}
#endif


#endif



#endif
