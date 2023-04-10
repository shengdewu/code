
/*************************************************************************
Copyright:   HZ.
Author:		 Hu Yaowu
Date:		 2018-11-23
Mail:        dongtingyueh@163.com
Description: IDW .
Reference:Image Warping with Scattered Data Interpolation
*************************************************************************/
#ifndef __T_IDW__
#define __T_IDW__


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
*inputPoints: source control points
*outputPoints: points after moving
*Return: 0-OK,other failed
**************************************************************************/
EXPORT int f_IDW(unsigned char* srcData, int width, int height, int stride, int inputPoints[], int outputPoints[], int pointNum);
#else

#ifdef __cplusplus
extern "C" {
#endif    

   int f_IDW(unsigned char* srcData, int width, int height, int stride, int inputPoints[], int outputPoints[], int pointNum);
#ifdef __cplusplus
}
#endif


#endif



#endif
