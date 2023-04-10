/*************************************************************************
Copyright:   Trent.
Author:		 Trent1985
Date:		 2018-9-30
Mail:        dongtingyueh@163.com
Description: Sphere Filter
*************************************************************************/
#ifndef __TEST_LUT_FILTER__
#define __TEST_LUT_FILTER__

#ifdef _MSC_VER

#ifdef __cplusplus
#define EXPORT extern "C" _declspec(dllexport)
#else
#define EXPORT __declspec(dllexport)
#endif

/************************************************************
*Function: ConvexMirrorFilter
*Description: Convex Mirror filter
*Params:    
*srcData:  image bgr data     
*width  :image width
*height :image height
*stride :image stride
*x      :center x of filter area
*y      :center y of filter area
*k      :intensity of filter,range [0,+]
*Return :0-OK,or failed    
************************************************************/
EXPORT int f_ConvexMirrorFilter(unsigned char* srcData, int width, int height, int stride, int x, int y, int k);
/************************************************************
*Function: ConcaveMirrorFilter
*Description: Convex Mirror filter
*Params:
*srcData:  image bgr data
*width  :image width
*height :image height
*stride :image stride
*x      :center x of filter area
*y      :center y of filter area
*k      :intensity of filter,range [0,+]
*Return :0-OK,or failed
************************************************************/
EXPORT int f_ConcaveMirrorFilter(unsigned char* srcData, int width, int height, int stride, int x, int y, int k);

#else

#ifdef __cplusplus
extern "C" {
#endif   
	int f_ConvexMirrorFilter(unsigned char* srcData, int width, int height, int stride, int x, int y, int k);
	int f_ConcaveMirrorFilter(unsigned char* srcData, int width, int height, int stride, int x, int y, int k);

	////
	#ifdef __cplusplus
}
#endif


#endif
#endif
