
/*************************************************************************
Copyright:   HZ.
Author:		 Hu Yaowu
Date:		 2018-4-23
Mail:        dongtingyueh@163.com
Description: BeautyCamera .
Refference: None
*************************************************************************/
#ifndef __T_BEAUTY_CAMERA__
#define __T_BEAUTY_CAMERA__


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
*curveMap: light curve lut, 512x512
*softRatio: intensity of softskin, [0,100]
*skinWhiteRatio: intensity of skin white, [0,100]
*skinColorRatio: intensity of skin color, [-50,50]
*sharpenRatio: intensity of sharpen, [0, 100],default 30
*Return: 0-OK,other failed
**************************************************************************/
EXPORT int f_BeautyCamera(unsigned char* srcData, int width, int height, int stride, unsigned char* curveMap, int softRatio, int skinWhiteRatio, int skinColorRatio, int sharpenRatio);


#else

#ifdef __cplusplus
extern "C" {
#endif    

    int f_BeautyCamera(unsigned char* srcData, int width, int height, int stride, unsigned char* curveMap, int softRatio, int skinWhiteRatio, int skinColorRatio, int sharpenRatio);

#ifdef __cplusplus
}
#endif


#endif



#endif
