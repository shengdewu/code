/*************************************************************************
Copyright:   Trent.
Author:		 Trent1985
Date:		 2018-9-23
Mail:        dongtingyueh@163.com
Description: SOBEL 
*************************************************************************/
#ifndef __TEST_SOBEL__
#define __TEST_SOBEL__

#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

/************************************************************
*Function:  Sobel edge detection
*Description: Sobel edge detection
*Params:    
*srcData:  image bgr data     
*width  :image width
*height :image height
*stride :image stride
*threshold:[0,255]
*Return :0-OK,or failed    
************************************************************/
EXPORT int f_Sobel(unsigned char *srcData, int width, int height,int stride, int threshold);
/************************************************************
*Function:  Find edges of photoshop
*Description: Find edges of photoshop
*Params:    
*srcData:  image bgr data     
*width  :image width
*height :image height
*stride :image stride
*Return :0-OK,or failed    
************************************************************/
EXPORT int f_FindEdges(unsigned char *srcData, int width, int height,int stride);
#else

#ifdef __cplusplus
extern "C" {
#endif   
	int f_Sobel(unsigned char *srcData, int width, int height,int stride, int threshold);
	int f_FindEdges(unsigned char *srcData, int width, int height,int stride);
	////
	#ifdef __cplusplus
}
#endif


#endif
#endif
