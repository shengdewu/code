#include"ImageEngine.h"
#include"MNN.h"
#include"f_AI_filter_virtual.h"
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

int SF_PortraitSegment(unsigned char* srcData, int width, int height, int stride, int isThreshold)
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
int SF_FocusVirtualFilter(unsigned char* srcData, int width, int height, int stride, int intensity)
{
	int ret = 0;
	unsigned char* maskData = (unsigned char*)malloc(sizeof(unsigned char) * height * stride);
	memcpy(maskData, srcData, sizeof(unsigned char) * height * stride);
	ret = SF_PortraitSegment(maskData, width, height, stride, 0);
	ret = f_FocusVirtualFilter(srcData, width, height, stride, maskData, intensity);
	free(maskData);
	return ret;
};
int SF_PortraitEffect(unsigned char* srcData, int width, int height, int stride, int intensity, int effectId)
{
	int ret = 0;
	unsigned char* maskData = (unsigned char*)malloc(sizeof(unsigned char) * height * stride);
	memcpy(maskData, srcData, sizeof(unsigned char) * height * stride);
	ret = SF_PortraitSegment(maskData, width, height, stride, 0);
	ret = f_PortraitEffect(srcData, width, height, stride, maskData, intensity, effectId);
	free(maskData);
	return ret;
};