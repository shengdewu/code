#include"ImageEngine.h"
#include"MNN.h"
#include"f_SF_FaceDetect.h"
int SF_MNN_Initialize(char* modelpath, int tensorSize, int stdMode, int numThread)
{
	int ret = 0;
	ret = f_MNN_Initialize(modelpath, tensorSize, stdMode, numThread);
	return ret;
};
void SF_MNN_Unitialize()
{
	return f_MNN_Unitialize();
};
/************************************************************
*Function:  SF_FaceskinSegment
*Description: FaceSkin Segment
*Params:
*srcData:  image bgra data
*width  :image width
*height :image height
*stride :image stride
*isThreshold: 1-binary process, other's not.
*Return :0-OK,or failed
************************************************************/
int SF_FaceskinSegment(unsigned char* srcData, int width, int height, int stride, int isThreshold)
{
	int ret = f_MNN_Process(srcData, width, height, stride);
	if (isThreshold)
	{
		unsigned char* pSrc = srcData;
		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				int gray = (pSrc[0] + pSrc[1] + pSrc[2]) / 3 < 128 ? 0 : 255;
				pSrc[0] = gray;
				pSrc[1] = gray;
				pSrc[2] = gray;
				pSrc += 4;
			}

		}
	}
	return ret;
};
int SF_FaceDetect(unsigned char* srcData, int width, int height, int stride, int faceRects[1 + 4 * 4])
{
	int ret = 0;
	unsigned char* maskData = (unsigned char*)malloc(sizeof(unsigned char) * height * stride);
	memcpy(maskData, srcData, sizeof(unsigned char) * height * stride);
	ret = SF_FaceskinSegment(maskData, width, height, stride, 1);
	//memcpy(srcData, maskData, sizeof(unsigned char) * height * stride);
	ret = f_FaceDetect(maskData, width, height, stride, faceRects);
	free(maskData);
	return ret;
};
