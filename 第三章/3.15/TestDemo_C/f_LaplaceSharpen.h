/*************************************************************************
Copyright:   Trent.
Author:		 Trent1985
Date:		 2018-9-23
Mail:        dongtingyueh@163.com
Description: Laplace sharpen
*************************************************************************/
#ifndef __TEST_LAPLACE_SHARPEN__
#define __TEST_LAPLACE_SHARPEN__

#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

/************************************************************
*Function:  Laplace sharpen
*Description: Laplace sharpen process
*Params:    
*srcData:  image bgra data     
*width  :image width
*height :image height
*stride :image stride
*mode   :0-MASK4,1-MASK8
*Return :0-OK,or failed    
************************************************************/
EXPORT int f_LaplaceSharpen(unsigned char* srcData,int width, int height,int stride,int mode);


#else

#ifdef __cplusplus
extern "C" {
#endif   
	int f_LaplaceSharpen(unsigned char* srcData,int width, int height,int stride,int mode);

	////
	#ifdef __cplusplus
}
#endif


#endif
#endif
