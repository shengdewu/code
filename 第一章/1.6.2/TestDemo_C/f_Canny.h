/*************************************************************************
Copyright:   Trent.
Author:		 Trent1985
Date:		 2018-9-23
Mail:        dongtingyueh@163.com
Description: Canny 
*************************************************************************/
#ifndef __TEST_CANNY__
#define __TEST_CANNY__

#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

/************************************************************
*Function:  Canny edge detection
*Description: Canny edge detection
*Params:    
*srcData:  image bgr data     
*width  :image width
*height :image height
*stride :image stride
*highThreshold:[0,255]
*lowThreshold: [0,255],default 0.4 * highThreshold
*Return :0-OK,or failed    
************************************************************/
EXPORT int f_CannyEdgedetection(unsigned char* srcData, int width ,int height, int stride, int highThreshold,int lowThreshold);

#else

#ifdef __cplusplus
extern "C" {
#endif   
	int f_CannyEdgedetection(unsigned char* srcData, int width ,int height, int stride, int highThreshold,int lowThreshold);
	////
	#ifdef __cplusplus
}
#endif


#endif
#endif
