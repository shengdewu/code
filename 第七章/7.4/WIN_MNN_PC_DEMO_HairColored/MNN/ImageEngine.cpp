#include"ImageEngine.h"
#include"MNN.h"
#include"f_HairColored.h"
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

int SF_HairColored(unsigned char* srcData, int width, int height, int stride, unsigned char* filtermap, unsigned char* colorMask, int ratio)
{
	unsigned char* hairMask = (unsigned char*)malloc(sizeof(unsigned char) * height * stride);
	memcpy(hairMask, srcData, sizeof(unsigned char) * height * stride);
	int ret = f_MNN_Process(hairMask, width, height, stride);
	ret = f_HairColored(srcData, width, height, stride, hairMask, filtermap, colorMask, ratio);
	free(hairMask);
	return ret;
};
