/*************************************************************************
Copyright:   Trent.
Author:		 Trent1985
Date:		 2018-9-23
Mail:        dongtingyueh@163.com
Description: RGB2YUV
*************************************************************************/
#ifndef __TEST_YUV__
#define __TEST_YUV__

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
*mode: 0-Y image,1-U image, 2-V image
*Return :0-OK,or failed    
************************************************************/
EXPORT int f_YUVAdjust(unsigned char *srcData, int width, int height, int stride, int mode);


#else

#ifdef __cplusplus
extern "C" {
#endif   
	int f_YUVAdjust(unsigned char *srcData, int width, int height, int stride, int mode);

	////
	#ifdef __cplusplus
}
#endif


#endif
#endif
