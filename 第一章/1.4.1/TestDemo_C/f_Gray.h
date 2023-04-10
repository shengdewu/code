/*************************************************************************
Copyright:   Trent.
Author:		 Trent1985
Date:		 2018-9-23
Mail:        dongtingyueh@163.com
Description: RGB2GRAY
*************************************************************************/
#ifndef __TEST_GRAY__
#define __TEST_GRAY__

#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

/************************************************************
*Function:  Gray
*Description: Color image convert to gray image.
*Params:    
*srcData:  image bgra data     
*width  :image width
*height :image height
*stride :image stride
*mode: 0-mean gray,1-classic gray, 2-ps gray
*Return :0-OK,or failed    
************************************************************/
EXPORT int f_Gray(unsigned char *srcData, int width, int height, int stride, int mode);


#else

#ifdef __cplusplus
extern "C" {
#endif   
	int f_Gray(unsigned char *srcData, int width, int height, int stride, int mode);

	////
	#ifdef __cplusplus
}
#endif


#endif
#endif
