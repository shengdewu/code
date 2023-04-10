
/*************************************************************************
Copyright:   HZ.
Author:		 Hu Yaowu
Date:		 2018-11-23
Mail:        dongtingyueh@163.com
Description: Eye bigger .
*************************************************************************/
#ifndef __T_EYE_BIG__
#define __T_EYE_BIG__


#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

/*************************************************************************
*Function: IDW
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*cenX: X of center points for eye
*cenY: Y of center points for eye
*radius: Radius of warp area
*intensity: Intensity of eye big processing
*Return: 0-OK,other failed
**************************************************************************/
EXPORT int f_BigEye(unsigned char* srcData, int width, int height, int stride, int cenX, int cenY, int radius, int intensity);
#else

#ifdef __cplusplus
extern "C" {
#endif    

   int f_BigEye(unsigned char* srcData, int width, int height, int stride, int cenX, int cenY, int radius, int intensity);
#ifdef __cplusplus
}
#endif


#endif



#endif
