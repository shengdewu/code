/*************************************************************************
Copyright:   Trent.
Author:		 Trent1985
Date:		 2018-9-23
Mail:        dongtingyueh@163.com
Description: Saturation adjust
*************************************************************************/
#ifndef __TEST_SATURATION__
#define __TEST_SATURATION__

#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

/************************************************************
*Function:  Saturation adjust
*Description: Saturation process
*Params:    
*srcData:  image bgr data     
*width  :image width
*height :image height
*stride :image stride
*saturation: range [-100,100]
*Return :0-OK,or failed    
************************************************************/
EXPORT int f_Saturation(unsigned char *srcData, int width, int height, int stride, int saturation);


#else

#ifdef __cplusplus
extern "C" {
#endif   
	int f_Saturation(unsigned char *srcData, int width, int height, int stride, int saturation);


	////
	#ifdef __cplusplus
}
#endif


#endif
#endif
