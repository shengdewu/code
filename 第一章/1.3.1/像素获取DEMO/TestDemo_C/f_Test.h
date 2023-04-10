/*************************************************************************
Copyright:   Trent.
Author:		 Trent1985
Date:		 2018-9-23
Mail:        dongtingyueh@163.com
Description: RGB2HSI
*************************************************************************/
#ifndef __TEST__
#define __TEST__

#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif
//BEAUTY FACE
/************************************************************
*Function:  Test
*Description: Test
*Params:    
*srcData:  image bgra data     
*width  :image width
*height :image height
*stride :image stride
*Return :0-OK,or failed    
************************************************************/
EXPORT int f_getPixel(unsigned char *srcData, int width, int height, int stride, int x, int y, int rgb[3]);


#else

#ifdef __cplusplus
extern "C" {
#endif   
	int f_getPixel(unsigned char *srcData, int width, int height, int stride, int x, int y, int rgb[3]);

	////
	#ifdef __cplusplus
}
#endif


#endif
#endif
