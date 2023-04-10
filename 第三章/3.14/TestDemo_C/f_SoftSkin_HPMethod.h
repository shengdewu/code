
/*************************************************************************
Copyright:   HZ.
Author:		 Hu Yaowu
Date:		 2018-11-23
Mail:        dongtingyueh@163.com
Description: Soft skin .
*************************************************************************/
#ifndef __T_SOFT_SKIN_HP_METHOD__
#define __T_SOFT_SKIN_HP_METHOD__


#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

/*************************************************************************
*Function: SOFT SKIN HP METHOD
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*textureRatio: intensity of details controled, range [0,100] 
*ratio: Intensity of softskin,range [0,100]
*Return: 0-OK,other failed
**************************************************************************/
EXPORT int f_Softskin_HP(unsigned char* srcData, int width, int height, int stride, int textureRatio, int ratio);


#else

#ifdef __cplusplus
extern "C" {
#endif    

   int f_Softskin_HP(unsigned char* srcData, int width, int height, int stride, int textureRatio, int ratio);

#ifdef __cplusplus
}
#endif


#endif



#endif
