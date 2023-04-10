
/*************************************************************************
Copyright:   HZ.
Author:		 Hu Yaowu
Date:		 2018-11-23
Mail:        dongtingyueh@163.com
Description: Skin White.
*************************************************************************/
#ifndef __T_SKIN_WHITE__
#define __T_SKIN_WHITE__


#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

/*************************************************************************
*Function: SKIN WHITE
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*skinMask: skin mask
*lutData: 32BGRA buffer of lut image.
*ratio: Intensity of softskin,range [0,100]
*Return: 0-OK,other failed
**************************************************************************/
EXPORT int f_SkinWhite(unsigned char* srcData, int width, int height, int stride, unsigned char* lutData, int ratio);

/*************************************************************************
*Function: SKIN WHITE USING CURVE 
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*belta: intensity of curve enhancement,range[2,10],default:2
*ratio: Intensity of softskin,range [0,100]
*Return: 0-OK,other failed
*Reference: "A Two-Stage Contrast Enhancement Algorithm for Digital Images"
**************************************************************************/
EXPORT int f_SkinWhiteCurve(unsigned char* srcData, int width, int height, int stride, int belta, int ratio);
/*************************************************************************
*Function: SKIN WHITE USING SMOOTH LIGHT OF PS 
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*ratio: Intensity of softskin,range [0,100]
*Return: 0-OK,other failed
**************************************************************************/
EXPORT int f_SkinWhitePS(unsigned char* srcData, int width, int height, int stride, int ratio);
#else

#ifdef __cplusplus
extern "C" {
#endif    

   int f_SkinWhite(unsigned char* srcData, int width, int height, int stride, unsigned char* lutData, int ratio);
   int f_SkinWhiteCurve(unsigned char* srcData, int width, int height, int stride, int belta, int ratio);
   int f_SkinWhitePS(unsigned char* srcData, int width, int height, int stride, int ratio);
#ifdef __cplusplus
}
#endif


#endif



#endif
