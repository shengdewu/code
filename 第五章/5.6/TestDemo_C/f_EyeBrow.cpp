#include"f_MakeupBase.h"
#include"f_EyeBrow.h"
#include<math.h>
#include<string.h>
#include<stdlib.h>


int f_EyeBrow(unsigned char* srcData, int width, int height, int stride, int srcFacePoints[202], unsigned char* mskData, int mWidth, int mHeight, int mStride, int maskKeyPoints[2 * 4], bool isLeft, int ratio)
{
	if(isLeft)
	{
	    int srcKeyPoints[2 * 3] = {srcFacePoints[2 * 19], srcFacePoints[2 * 19 + 1], (srcFacePoints[2 * 22] + srcFacePoints[2 * 26]) / 2, (srcFacePoints[2 * 22 + 1] + srcFacePoints[2 * 26 + 1]) / 2, srcFacePoints[2 * 24], srcFacePoints[2 * 24 + 1]};
	    return f_MakeupBase(srcData, width, height, stride,srcKeyPoints, mskData, mWidth, mHeight, mStride, maskKeyPoints, ratio);
	}
	else
	{
		int srcKeyPoints[2 * 3] = {srcFacePoints[2 * 29], srcFacePoints[2 * 29 + 1], (srcFacePoints[2 * 31] + srcFacePoints[2 * 37]) / 2, (srcFacePoints[2 * 37 + 1] + srcFacePoints[2 * 31 + 1]) / 2, srcFacePoints[2 * 34], srcFacePoints[2 * 34 + 1]};
	    return f_MakeupBase(srcData, width, height, stride,srcKeyPoints, mskData, mWidth, mHeight, mStride, maskKeyPoints, ratio);
	}
};
