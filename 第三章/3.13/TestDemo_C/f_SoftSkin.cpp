#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"Commen.h"
#include"f_GaussFilter.h"//1.5.2
#include"f_SkinPDF.h" //3.10
#include"f_SkinDetect.h"//3.9
#include"f_SmartBlur.h"//3.6
#include"f_SoftSkin.h"

/*************************************************************************
*Function: SOFT SKIN 
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*ratio: Intensity of softskin,range [0,100]
*Return: 0-OK,other failed
**************************************************************************/
int f_Softskin(unsigned char* srcData, int width, int height, int stride, int skinMode, int ratio)
{
	int ret = 0; 
	int length = height * stride;
	unsigned char* smoothData = (unsigned char*)malloc(sizeof(unsigned char) * length);
	memcpy(smoothData, srcData, sizeof(unsigned char) * length);
	unsigned char* skinPDF = (unsigned char*)malloc(sizeof(unsigned char) * length);
	memcpy(skinPDF, srcData, sizeof(unsigned char) * length);
	int smoothRadius = 8;
	int smoothThreshold = 38;
	int maskSmoothRadius = 3;
	ret = f_SmartBlur(smoothData, width, height, stride, smoothRadius, smoothThreshold);
	if(skinMode == 0)
	{
		ret = f_SkindetectionYCgCr(skinPDF, width, height, stride);
		maskSmoothRadius = 6;
	}
	else
	{
		ret = f_SkinPDF(skinPDF, width, height, stride);
	}
	ret = f_FastGaussFilter(skinPDF, width, height, stride, maskSmoothRadius);
	unsigned char* pSrc = srcData;
	unsigned char* pMask = skinPDF;
	unsigned char* pSmooth = smoothData;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			int mask = (pMask[0] + pMask[1] + pMask[2]) / 3;
			int b = (pSrc[0] * (255 - mask) + pSmooth[0] * mask) / 255;
			int g = (pSrc[1] * (255 - mask) + pSmooth[1] * mask) / 255;
			int r = (pSrc[2] * (255 - mask) + pSmooth[2] * mask) / 255;
			pSrc[0] = CLIP3((b * ratio + pSrc[0] * (100 - ratio)) / 100, 0, 255);
			pSrc[1] = CLIP3((g * ratio + pSrc[1] * (100 - ratio)) / 100, 0, 255);
			pSrc[2] = CLIP3((r * ratio + pSrc[2] * (100 - ratio)) / 100, 0, 255);
			pSrc += 4;
			pSmooth += 4;
			pMask += 4;
		}
	}
	free(skinPDF);
	free(smoothData);
	return ret;
};