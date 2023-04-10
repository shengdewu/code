/*************************************************************************
Copyright:   Trent.
Author:		 Trent1985
Date:		 2018-9-23
Mail:        dongtingyueh@163.com
Description: RGB2HSV
*************************************************************************/
#ifndef __TEST_HSI__
#define __TEST_HSI__

#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

/************************************************************
*Function:  Test
*Description: Test
*Params:    
*srcData:  image bgra data     
*width  :image width
*height :image height
*stride :image stride
*hIntensity: hue,range [0,360]
*sIntensity:saturation,range [0,1.0]
*iIntensity:lightness,range [0,1.0]
*Return :0-OK,or failed    
************************************************************/
EXPORT int f_HSVAdjust(unsigned char *srcData, int width, int height, int stride, float hIntensity, float sIntensity, float vIntensity);


#else

#ifdef __cplusplus
extern "C" {
#endif   
	int f_HSVAdjust(unsigned char *srcData, int width, int height, int stride, float hIntensity, float sIntensity, float vIntensity);

	////
	#ifdef __cplusplus
}
#endif


#endif
#endif
