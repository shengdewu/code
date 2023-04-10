#ifndef  __T_MAKEUP_BASE__
#define  __T_MAKEUP_BASE__

/*************************************************************************
*Function: Makeup base
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*srcFacePoints: 101 face points.
*mskData: blush image buffer with format bgra32
*mWidth: width of mask image 
*mHeight: height of mask image
*mStride: Stride of mask image
*maskKeyPoints£º 3 key points of blush mask.
*ratio: intensity of effect, [0, 100]
*Return: 0-OK,other failed
**************************************************************************/
int f_MakeupBase(unsigned char* srcData, int width, int height, int stride, int srcKeyPoints[2 * 3], unsigned char* mskData, int mWidth, int mHeight, int mStride, int maskKeyPoints[2 * 3], int ratio);

#endif