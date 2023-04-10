
/*************************************************************************
Copyright:   HZ.
Author:		 Hu Yaowu
Date:		 2018-11-23
Mail:        dongtingyueh@163.com
Description: Skin Color.
*************************************************************************/
#ifndef __T_SKIN_COLOR__
#define __T_SKIN_COLOR__


#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

/*************************************************************************
*Function: SKIN COLOR
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*skinMask: skin mask
*lutData: 32BGRA buffer of lut image.
*ratio: Intensity of skin colored,range [0,100]
*Return: 0-OK,other failed
**************************************************************************/
EXPORT int f_SkinColor(unsigned char* srcData, int width, int height, int stride, unsigned char* lutData, int ratio);

#else

#ifdef __cplusplus
extern "C" {
#endif    

   int f_SkinColor(unsigned char* srcData, int width, int height, int stride, unsigned char* lutData, int ratio);
#ifdef __cplusplus
}
#endif


#endif



#endif
