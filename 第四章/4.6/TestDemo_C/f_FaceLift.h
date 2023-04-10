
/*************************************************************************
Copyright:   HZ.
Author:		 Hu Yaowu
Date:		 2018-11-23
Mail:        dongtingyueh@163.com

Reference: Interactive Image Warping
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
*centerX: center x of warp circle area 
*centerY: center y of warp circle area
*rmax: radius of warp circle area
*mx: x of the point after moving
*my: y of the point after moving
*strength: warp intensity, [0, 100]
*Return: 0-OK,other failed
**************************************************************************/
EXPORT int f_FaceLift(unsigned char* srcData, int width, int height, int stride, int centerX, int centerY, int rmax, int mx, int my, int strength);
#else

#ifdef __cplusplus
extern "C" {
#endif    
/*************************************************************************
*Function: FaceLift
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*centerX: center x of warp circle area 
*centerY: center y of warp circle area
*rmax: radius of warp circle area
*mx: x of the point after moving
*my: y of the point after moving
*strength: warp intensity, [0, 100]
*Return: 0-OK,other failed
**************************************************************************/
   int f_FaceLift(unsigned char* srcData, int width, int height, int stride, int centerX, int centerY, int rmax, int mx, int my, int strength);
#ifdef __cplusplus
}
#endif


#endif



#endif
