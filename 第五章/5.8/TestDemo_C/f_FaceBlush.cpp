#include"Commen.h"
#include"f_FaceBlush.h"
#include"f_MakeupBase.h"

/*************************************************************************
*Function: Face Blush
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
*isLeft: left or right face.
*ratio: intensity of effect, [0, 100]
*Return: 0-OK,other failed
**************************************************************************/
int f_FaceBlush(unsigned char* srcData, int width, int height, int stride, int srcFacePoints[202], unsigned char* mskData, int mWidth, int mHeight, int mStride, int maskKeyPoints[2 * 3], bool isLeft, int ratio)
{
	if(isLeft)
	{
	    int srcKeyPoints[2 * 3] = {srcFacePoints[2 * 1], srcFacePoints[2 * 1 + 1], srcFacePoints[2 * 6], srcFacePoints[2 * 6 + 1], srcFacePoints[2 * 64], srcFacePoints[2 * 64 + 1]};
	    return f_MakeupBase(srcData, width, height, stride,srcKeyPoints, mskData, mWidth, mHeight, mStride, maskKeyPoints, ratio);
	}
	else
	{
		int srcKeyPoints[2 * 3] = {srcFacePoints[2 * 17], srcFacePoints[2 * 17 + 1], srcFacePoints[2 * 12], srcFacePoints[2 * 12 + 1], srcFacePoints[2 * 73], srcFacePoints[2 * 73 + 1]};
	    return f_MakeupBase(srcData, width, height, stride,srcKeyPoints, mskData, mWidth, mHeight, mStride, maskKeyPoints, ratio);
	}
};