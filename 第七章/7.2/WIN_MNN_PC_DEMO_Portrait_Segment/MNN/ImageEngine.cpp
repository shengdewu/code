#include"ImageEngine.h"
#include"MNN.h"

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
