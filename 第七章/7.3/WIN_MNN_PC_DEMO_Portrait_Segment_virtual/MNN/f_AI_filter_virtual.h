/*************************************************************************
Copyright:   Trent.
Author:		 Trent
Date:		 2019-9-23
Mail:        shuofeng@in66.com
*************************************************************************/
#ifndef __AI_FILTER_VIRTUAL__
#define __AI_FILTER_VIRTUAL__
#define MASK_NUM_MAX 3
#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

/************************************************************
*Function:  f_FocusVirtualFilter
*Description: Virtual filter
*Params:    
*srcData:  image bgra data     
*width  :image width
*height :image height
*stride :image stride
*mask: the person mask of image.
*intensity: intensity of effect, [0,100].
*Return :0-OK,or failed    
************************************************************/
EXPORT int f_FocusVirtualFilter(unsigned char* srcData, int width, int height, int stride, unsigned char* mask, int intensity);
/************************************************************
*Function:  f_PortraitEffect
*Description: Filter effect
*Params:
*srcData:  image bgra data
*width  :image width
*height :image height
*stride :image stride
*mask: the person mask of image.
*intensity: intensity of effect, [0,100].
*effectId: Effect ID,0-Mascia,1-Diffusion, 2-Zoomblur, 3-Oilpaint
*Return :0-OK,or failed
************************************************************/
EXPORT int f_PortraitEffect(unsigned char* srcData, int width, int height, int stride, unsigned char* mask, int intensity, int effectId);
#else

#ifdef __cplusplus
extern "C" {
#endif   
	int f_FocusVirtualFilter(unsigned char* srcData, int width, int height, int stride, unsigned char* mask, int intensity);
	int f_PortraitEffect(unsigned char* srcData, int width, int height, int stride, unsigned char* mask, int intensity, int effectId);
	////
	#ifdef __cplusplus
}
#endif


#endif
#endif
