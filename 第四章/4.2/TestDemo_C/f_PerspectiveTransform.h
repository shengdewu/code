
/*************************************************************************
Copyright:   HZ.
Author:		 Hu Yaowu
Date:		 2018-11-23
Mail:        dongtingyueh@163.com
Description: Perspective Transform .
*************************************************************************/
#ifndef __T_AFFINE_TRANSFROM__
#define __T_AFFINE_TRANSFROM__


#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

/*************************************************************************
*Function: Perspective Transform
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*H: perspective matrix,[a11, a12, a13, a21, a22, a23, a31, a32, a33]
*Return: 0-OK,other failed
**************************************************************************/
EXPORT int f_PerspectiveTransform(unsigned char* srcData, int width, int height, int stride, float H[9]);


#else

#ifdef __cplusplus
extern "C" {
#endif    

   int f_PerspectiveTransform(unsigned char* srcData, int width, int height, int stride, float H[9]);
   
#ifdef __cplusplus
}
#endif


#endif



#endif
