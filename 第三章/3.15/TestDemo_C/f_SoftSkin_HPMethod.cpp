#include"Commen.h"
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"f_SoftSkin_HPMethod.h"
#include"f_GaussFilter.h"
#include"f_LUTFilter.h"
#include"f_SkinPDF.h" //3.10
#include"f_SkinDetect.h"//3.9
#include"f_SmartBlur.h"
#include"f_SurfaceBlur.h"
#include"f_Commen_MixLayer.h"


/*************************************************************************
*Function: SOFT SKIN HP METHOD
*Params:
*srcData:32BGRA image buffer
*width:  width of image
*height: height of image
*stride: Stride of image
*textureRatio: intensity of details controled, range [0,100] 
*ratio: Intensity of softskin,range [0,100]
*Return: 0-OK,other failed
**************************************************************************/
int f_Softskin_HP(unsigned char* srcData, int width, int height, int stride, int textureRatio, int ratio)
{
	int ret = 0;
	int length = height * stride;
	unsigned char* smoothData = (unsigned char*)malloc(sizeof(unsigned char) * length);
	unsigned char* hpData = (unsigned char*)malloc(sizeof(unsigned char) * length);
	memcpy(smoothData, srcData, sizeof(unsigned char) * length);
	unsigned char* skinPDF = (unsigned char*)malloc(sizeof(unsigned char) * length);
	int smoothRadius = 8;
	int smoothThreshold = 38;
	int maskSmoothRadius = 3;
	ret = f_SurfaceBlur(smoothData, width, height, stride, smoothRadius, smoothThreshold);
	memcpy(skinPDF, smoothData, sizeof(unsigned char) * length);
	ret = f_SkinPDF(skinPDF, width, height, stride);
	ret = f_FastGaussFilter(skinPDF, width, height, stride, maskSmoothRadius);
	unsigned char* pSrc = srcData;
	unsigned char* pSkin = skinPDF;
	unsigned char* pSmooth = smoothData;
	unsigned char* pHP = hpData;
	int k = ratio * 128 / 100;
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			pHP[0] = CLIP3(pSmooth[0] - pSrc[0] + 128, 0, 255);
			pHP[1] = CLIP3(pSmooth[1] - pSrc[1] + 128, 0, 255);
			pHP[2] = CLIP3(pSmooth[2] - pSrc[2] + 128, 0, 255);

			pHP += 4;
			pSmooth += 4;
			pSrc += 4;
		}
	}
	float hpRadius = 3.5f * textureRatio / 100.0f;
	ret = f_FastGaussFilter(hpData, width, height, stride, hpRadius);
	pSmooth = smoothData;
	pHP = hpData;
	pSrc = srcData;
	
	for(int j = 0; j < height; j++)
	{
		for(int i = 0; i < width; i++)
		{
			int hpb = pHP[0];
			int hpg = pHP[1];
			int hpr = pHP[2];
			hpb = ModeLinearLight(pSrc[0], hpb);
			hpg = ModeLinearLight(pSrc[1], hpg);
			hpr = ModeLinearLight(pSrc[2], hpr);
			int mask = (pSkin[0] + pSkin[1] + pSkin[2]) / 3;
			hpb = CLIP3((hpb * mask + pSmooth[0] * (255 - mask)) / 255, 0, 255);
			hpg = CLIP3((hpg * mask + pSmooth[1] * (255 - mask)) / 255, 0, 255);
			hpr = CLIP3((hpr * mask + pSmooth[2] * (255 - mask)) / 255, 0, 255);
			pSrc[0] = CLIP3((hpb * k + pSrc[0] * (128 - k)) >> 7, 0, 255);
			pSrc[1] = CLIP3((hpg * k + pSrc[1] * (128 - k)) >> 7, 0, 255);
			pSrc[2] = CLIP3((hpr * k + pSrc[2] * (128 - k)) >> 7, 0, 255);
			pSrc += 4;
			pHP += 4;
			pSmooth += 4;
			pSkin += 4;
		}
	}
	free(skinPDF);
	free(smoothData);
	free(hpData);
	return ret = 0;
};