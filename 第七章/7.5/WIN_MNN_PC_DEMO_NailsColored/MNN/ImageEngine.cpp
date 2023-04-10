#include"ImageEngine.h"
#include"MNN.h"
#include"f_Colornails.h"
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

int SF_NailsColored(unsigned char* srcData, int width, int height, int stride, int R, int G, int B, int ratio)
{
	unsigned char* nailsMask = (unsigned char*)malloc(sizeof(unsigned char) * height * stride);
	memcpy(nailsMask, srcData, sizeof(unsigned char) * height * stride);
	int ret = f_MNN_Process(nailsMask, width, height, stride);
	
	ret = f_Colornails(srcData, width, height, stride, nailsMask, width, height, stride, R, G, B, ratio);
	free(nailsMask);
	return ret;
};
