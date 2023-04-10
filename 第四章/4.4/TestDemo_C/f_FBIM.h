
/*************************************************************************
Copyright:   HZ.
Author:		 Hu Yaowu
Date:		 2018-11-23
Mail:        dongtingyueh@163.com
Description: FBIM
Reference:Feature Based Image Metamorphosis .
*************************************************************************/
#ifndef __T_FBIM__
#define __T_FBIM__


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
*inputlinePoints: input feature line points of source image 
*outputlinePoints: output feature line points of dest image
*lineNum: count of lines
*Return: 0-OK,other failed
**************************************************************************/
EXPORT int f_FBIM(unsigned char* srcData, int width, int height, int stride, int inputlinePoints[], int outputlinePoints[], int lineNum);
#else

#ifdef __cplusplus
extern "C" {
#endif    

   int f_FBIM(unsigned char* srcData, int width, int height, int stride, int inputlinePoints[], int outputlinePoints[], int lineNum);
#ifdef __cplusplus
}
#endif


#endif



#endif
