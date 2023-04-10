#include"f_MakeupBase.h"
#include"f_SteroNose.h"
#include<math.h>
#include<string.h>
#include<stdlib.h>   

int f_SteroNose(unsigned char* srcData, int width, int height, int stride, int srcFacePoints[202], unsigned char* mskData, int mWidth, int mHeight, int mStride, int maskKeyPoints[2 * 3], int ratio)
{
	int srcKeyPoints[2 * 3] = {srcFacePoints[2 * 97], srcFacePoints[2 * 97 + 1], srcFacePoints[2 * 66], srcFacePoints[2 * 66 + 1], srcFacePoints[2 * 71], srcFacePoints[2 * 71 + 1]};
	return f_MakeupBaseSteronose(srcData, width, height, stride,srcKeyPoints, mskData, mWidth, mHeight, mStride, maskKeyPoints, ratio);
};
