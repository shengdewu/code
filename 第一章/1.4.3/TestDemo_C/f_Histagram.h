/*************************************************************************
Copyright:   Trent.
Author:		 Trent1985
Date:		 2018-9-23
Mail:        dongtingyueh@163.com
Description: Histagram
*************************************************************************/
#ifndef __TEST_HISTAGRAM__
#define __TEST_HISTAGRAM__

#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

/************************************************************
*Function:  Histagram
*Description: Compute histagram
*Params:    
*srcData:  image bgr data     
*width  :image width
*height :image height
*stride :image stride
*mode: 0-Gray histagram,1-Red histagram, 2-Green histagram, 3-Blue histagram
*Return :0-OK,or failed    
************************************************************/
EXPORT int f_Histagram(unsigned char *srcData, int width, int height, int stride, int hist[], int mode);


#else

#ifdef __cplusplus
extern "C" {
#endif   
	int f_Histagram(unsigned char *srcData, int width, int height, int stride, int hist[], int mode);


	////
	#ifdef __cplusplus
}
#endif


#endif
#endif
