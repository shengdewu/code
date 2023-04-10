
/*************************************************************************
Copyright:   HZ.
Author:		 Hu Yaowu
Date:		 2018-11-23
Mail:        dongtingyueh@163.com
Description: Auto Facelift .
*************************************************************************/
#ifndef __T_FACELIFT__
#define __T_FACELIFT__


#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

/*************************************************************************
*Function: FaceLift
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*facePoints: 101 face points of image.
*intensity: intensity of facelift, [0, 100]
*Return: 0-OK,other failed
**************************************************************************/
EXPORT int f_AutoFacelift(unsigned char* srcData, int width, int height, int stride, int facePoints[], int intensity);
#else

#ifdef __cplusplus
extern "C" {
#endif    

   int f_AutoFacelift(unsigned char* srcData, int width, int height, int stride, int facePoints[], int intensity);
#ifdef __cplusplus
}
#endif


#endif



#endif
