
/*************************************************************************
Copyright:   HZ.
Author:		 Hu Yaowu
Date:		 2018-11-23
Mail:        dongtingyueh@163.com
Description: Auto Bigeye .
*************************************************************************/
#ifndef __T_AUTOBIGEYE__
#define __T_AUTOBIGEYE__


#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

/*************************************************************************
*Function: Auto Bigeye
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*facePoints: 101 face points of image.
*Return: 0-OK,other failed
**************************************************************************/
EXPORT int f_AutoBigeye(unsigned char* srcData, int width, int height, int stride, int facePoints[]);
#else

#ifdef __cplusplus
extern "C" {
#endif    

   int f_AutoBigeye(unsigned char* srcData, int width, int height, int stride, int facePoints[]);
#ifdef __cplusplus
}
#endif


#endif



#endif
