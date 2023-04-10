/*************************************************************************
Copyright:   Trent.
Author:		 Trent1985
Date:		 2018-9-23
Mail:        dongtingyueh@163.com
Description: RGB2LAB
*************************************************************************/
#ifndef __TEST_LAB__
#define __TEST_LAB__

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
*mode: 0-L image,1-A image, 2-B image
*Return :0-OK,or failed    
************************************************************/
EXPORT int f_LABAdjust(unsigned char *srcData, int width, int height, int stride, int mode);


#else

#ifdef __cplusplus
extern "C" {
#endif   
	int f_LABAdjust(unsigned char *srcData, int width, int height, int stride, int mode);

	////
	#ifdef __cplusplus
}
#endif


#endif
#endif
