/*************************************************************************
Copyright:   Trent.
Author:		 Trent1985
Date:		 2018-9-23
Mail:        dongtingyueh@163.com
Description: Saturation adjust
*************************************************************************/
#ifndef __TEST_FILTER__
#define __TEST_FILTER__

#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

/************************************************************
*Function:  ColorFilter 
*Description: Color Filter
*Params:    
*srcData:  image bgr data     
*width  :image width
*height :image height
*stride :image stride
*Return :0-OK,or failed    
************************************************************/
EXPORT int f_ColorFilter(unsigned char *srcData, int width, int height, int stride);


#else

#ifdef __cplusplus
extern "C" {
#endif   
	int f_ColorFilter(unsigned char *srcData, int width, int height, int stride);


	////
	#ifdef __cplusplus
}
#endif


#endif
#endif
