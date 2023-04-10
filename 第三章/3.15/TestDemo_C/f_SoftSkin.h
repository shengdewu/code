
/*************************************************************************
Copyright:   HZ.
Author:		 Hu Yaowu
Date:		 2018-11-23
Mail:        dongtingyueh@163.com
Description: Soft skin .
*************************************************************************/
#ifndef __T_SOFT_SKIN__
#define __T_SOFT_SKIN__


#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

/*************************************************************************
*Function: SOFT SKIN 
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*ratio: Intensity of softskin,range [0,100]
*Return: 0-OK,other failed
**************************************************************************/
EXPORT int f_Softskin(unsigned char* srcData, int width, int height, int stride, int skinMode, int ratio);


#else

#ifdef __cplusplus
extern "C" {
#endif    

   int f_Softskin(unsigned char* srcData, int width, int height, int stride, int skinMode, int ratio);

#ifdef __cplusplus
}
#endif


#endif



#endif
